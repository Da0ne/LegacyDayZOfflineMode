class Hologram
{
	protected 	ItemBase m_Parent; 		
	protected  	ItemBase m_Projection; 	
	protected 	PlayerBase m_Player;

	protected bool 	m_IsColliding;
	protected bool 	m_UpdatePosition;
		
	autoptr protected TimerUpdate m_UpdateTimer;

	protected const string 	SELECTION_PLACING 		= "placing";
	protected const string 	SELECTION_INVENTORY 	= "inventory";

	protected const float 	SMALL_OBJECT_RADIUS 	= 1.5;
	protected const float	DISTANCE_SMALL_OBJECT	= 2;
	
	void Hologram( string obj_name, PlayerBase player )
	{
		m_Player = player;
		m_Parent = GetParentObject();
		m_Projection = NULL;
		m_UpdatePosition = true;

		ItemBase hologram_item = GetGame().CreateObject( obj_name, player.GetPosition(), true ); //if True, object is not spawned on clients only on server 
		
		SetProjectionObject( hologram_item );
		
		if ( GetGame().IsMultiplayer() && GetGame().IsServer() )
		{
			return;
		}
		
		hologram_item.SetAffectPathgraph( false, false );
		
		if ( m_Projection.HasAnimation( SELECTION_PLACING ) )
		{
			m_Projection.SetAnimationPhase( SELECTION_PLACING, 0 );
		}
		else
		{
			UpdateSelections();
		}
			
		if ( m_Projection.HasAnimation( SELECTION_INVENTORY ) )
		{
			m_Projection.SetAnimationPhase( SELECTION_INVENTORY, 1 );
		}
		
		RefreshVisual( SELECTION_PLACING );
		
		m_UpdateTimer = new TimerUpdate(this, "Update", CALL_CATEGORY_SYSTEM);
	}
	
	void ~Hologram()
	{
		if ( m_Projection )
		{
			GetGame().ObjectDelete( m_Projection );
		}

		delete m_UpdateTimer;
	}
		
	// Updates selections on hologram object so they reflect the state of the parent object's selections.
	void UpdateSelections()
	{
		string cfg_access = "CfgVehicles " + m_Projection.GetType() + " AnimationSources ";
		
		if ( GetGame().ConfigIsExisting(cfg_access) )
		{
			int cfg_access_count = g_Game.ConfigGetChildrenCount(cfg_access);

			for ( int i = 0; i < cfg_access_count; ++i )
			{
				string found_anim;
				GetGame().ConfigGetChildName(cfg_access, i, found_anim);
				
				if ( found_anim != SELECTION_INVENTORY )
				{
					float anim_phase = m_Parent.GetAnimationPhase(found_anim);
					m_Projection.SetAnimationPhase(found_anim, anim_phase);
				}
			}
		}
	}
	
	// update loop for visuals and collisions of the hologram
	void Update()
	{
		// this == NULL or m_UpdatePosition == false disable update (e.g. while place object animation)
		if ( m_Projection == NULL || !GetUpdatePosition() )
		{
			return;
		} 
		
		// update hologram position	
		SetHologramMatrix( GetProjectionObjectPosition( m_Player ), m_Player.GetOrientation() );	
		EvaluateCollision();
		// check if the projection object is electric device and is interacting with the power source
		CheckPowerSource();
		RefreshVisual( SELECTION_PLACING );
		GetProjectionObject().OnHologramBeingPlaced( m_Player );
	}

	void EvaluateCollision()
	{
		bool is_trap = false;
		bool is_surface_water = IsSurfaceWater( m_Projection.GetPosition() );
		
		if( m_Projection.IsKindOf("EN5C_Trap_Base") )
		{
			is_trap = true;
		}

		if( CheckCollision() || IsProjectionLocatedInOrigin() )
		{
			SetCollisionState( true );
		}
		else
		{
			if( is_trap )
			{
				if( m_Projection.IsInherited( TrapSpawnBase ) )
				{
					TrapSpawnBase trap_spawn_base = m_Projection;
					SetCollisionState( !trap_spawn_base.IsPlaceableAtPosition( m_Projection.GetPosition() ) );
				}
				else
				{
					TrapBase trap_base = m_Projection;
					SetCollisionState( !trap_base.IsPlaceableAtPosition( m_Projection.GetPosition() ) );
				}
			}
			else
			{
				if( is_surface_water )
				{
					SetCollisionState( true );
				}
				else
				{
					SetCollisionState( false );
				}				
			}			
		}	
	}
	
	bool CheckCollision()
	{
		vector center = m_Projection.GetPosition();
		vector orientation = m_Projection.GetDirection();
		vector edge_length;
		autoptr array<Object> excluded_objects = new array<Object>;
		autoptr array<Object> collided_objects = new array<Object>;
		
		vector min_max[2];
		m_Projection.ClippingInfo( min_max );
		edge_length = GetCollisionBoxSize( min_max[0], min_max[1] );

		excluded_objects.Insert( m_Projection );

		if ( GetGame().IsBoxColliding( center, orientation, edge_length, excluded_objects, collided_objects ) )
		{					
			return true;	
		}
		else
		{
			return false;			
		}
	}
	
	void CheckPowerSource()
	{
		//in range of its power source.
		if ( m_Player != NULL && m_Parent.HasEnergyManager() && m_Parent.GetCompEM().IsPlugged() )
		{
			// Now we know we are working with an electric device which is plugged into a power source.
			ItemBase placed_item = m_Parent;
			string message;
			vector pos = GetProjectionPosition();
					
			// Unplug the device when the player is too far from the power source.
			placed_item.GetCompEM().UpdatePlugState();
			
			if ( !placed_item.GetCompEM().IsPlugged() )
			{
				m_Player.MessageImportant("The plug was ripped out!");
				
				// Cancel placement mode
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(m_Player, "TogglePlacingFromPlayer");
			}
		}				
	}

	// PlaceObject needs to have ItemBase parameter, because we might want to place hologram of different item than the one in hands e.g. shovel in hands for placing garden plot
	void PlaceObject( ItemBase item_for_placing, PlayerBase player )
	{	
		if( item_for_placing.GetType() == player.GetItemInHands().GetType() )
		{
			player.DropEntity( item_for_placing );
		}
		else
		{
			item_for_placing = GetGame().CreateObject( item_for_placing.GetType(), player.GetPosition(), true ); //if True, object is not spawned on clients only on server 
		}
		
		vector item_pos = player.GetHologram().GetProjectionPosition();
		
		item_for_placing.SetPosition( item_pos );
		
		item_for_placing.SetOrientation( player.GetHologram().GetProjectionOrientation() );
			
		item_for_placing.SetAffectPathgraph( true, false );

		if( item_for_placing.CanAffectPathgraph() )
		{
			Timer update_timer = new Timer ( CALL_CATEGORY_SYSTEM );
			update_timer.Run( 0.1, GetGame(), "UpdatePathgraphRegionByObject", new Param1<Object>(item_for_placing), false );
		}
	}		
		
	private vector GetCollisionBoxSize( vector min, vector max )
	{
		vector box_size;
		
		box_size[0] = max[0] - min[0];
		box_size[1] = max[1] - min[1];
		box_size[2] = max[2] - min[2];
		
		return box_size;
	}

	bool IsSurfaceWater( vector position )
	{
		if( GetGame().SurfaceIsSea( position[0], position[2] ) )
		{
			return true;
		}
		else if( GetGame().SurfaceIsPond( position[0], position[2] ) )
		{
			return true;
		}
		
		return false;
	}

	private vector GetProjectionObjectPosition( PlayerBase player )
	{
		float min_projection_dist; 
		float max_projection_dist; 
		float camera_to_player_distance = vector.Distance( player.GetCameraPosition(), player.GetPosition() );

		vector min_max[2];
		float projection_item_radius = m_Projection.ClippingInfo( min_max );
		
		if( projection_item_radius < SMALL_OBJECT_RADIUS )	// objects with radius smaller than 1.5m
		{
			min_projection_dist = camera_to_player_distance + ( DISTANCE_SMALL_OBJECT / 3 ); // min distance is 1/3 of the max distance
			max_projection_dist = camera_to_player_distance + DISTANCE_SMALL_OBJECT;
		}
		else
		{
			min_projection_dist = camera_to_player_distance + ( projection_item_radius / 3 ); // min distance is 1/3 of the max distance
			max_projection_dist = camera_to_player_distance + projection_item_radius;
		}
		
		vector from = player.GetCameraPosition();
		vector to = from + ( player.GetCameraDirection() * max_projection_dist );
		vector contact_pos;
		vector contact_dir;
		int contact_component;
		
		GetGame().Raycast( from, to, contact_pos, contact_dir, contact_component, NULL, player, m_Projection, false, false );

		float camera_to_projection_distance = vector.Distance( player.GetCameraPosition(), contact_pos );	
		
		if( camera_to_projection_distance < min_projection_dist || camera_to_projection_distance > max_projection_dist )
		{
			contact_pos = "0 0 0";
		}
 
		return contact_pos;
	}
		
	bool IsProjectionLocatedInOrigin()
	{
		vector origin;
		origin[0] = 0;
		origin[1] = 0;
		origin[2] = 0;	

		if( m_Projection.GetPosition() == origin )
		{
			return true;			
		}
		else
		{
			return false;			
		}		
	}
	
	void SetUpdatePosition( bool state )
	{
		m_UpdatePosition = state;
	}

	bool GetUpdatePosition()
	{
		return m_UpdatePosition;
	}

	ItemBase GetParentObject()
	{
		return m_Player.GetItemInHands();
	}
	
	void SetProjectionObject( ItemBase projection )
	{
		m_Projection = projection;
	}

	ItemBase GetProjectionObject()
	{
		return m_Projection;
	}

	void SetCollisionState( bool is_colliding )
	{
		m_IsColliding = is_colliding;
		
		RefreshVisual( SELECTION_PLACING );
	}

	bool IsColliding()
	{
		return m_IsColliding;
	}

	void SetHologramMatrix( vector position, vector orientation )
	{
		m_Projection.SetPosition( position );

		m_Projection.SetOrientation( orientation );
	}
	
	vector GetProjectionPosition()
	{
		return m_Projection.GetPosition();
	}
	
	vector GetProjectionOrientation()
	{
		return m_Projection.GetOrientation();
	}

	int GetHiddenSelection( string selection )
	{
		string config_path = "CfgVehicles" + " " + m_Projection.GetType() + " " + "hiddenSelections";
		autoptr array<string> hidden_selection_array = new array<string>;

		GetGame().ConfigGetTextArray( config_path, hidden_selection_array );	

		for (int i = 0; i < hidden_selection_array.Count(); i++)
		{
			if ( hidden_selection_array.Get(i) == selection )
			{
				return i;
			}			
		}	

		return 0;
	}
	
	void RefreshVisual( string selection )
	{
		if ( m_Projection )
		{
			ItemBase projection_entity = (EntityAI) m_Projection;
			string config_material = "CfgVehicles" + " " + m_Projection.GetType() + " " + "hologramMaterial";
			string hologram_material = GetGame().ConfigGetTextOut( config_material );
			string config_model = "CfgVehicles" + " " + m_Projection.GetType() + " " + "hologramMaterialPath";
			string hologram_material_path = GetGame().ConfigGetTextOut( config_model );
			string SEL_CORD_PLUGGED = m_Parent.GetCompEM().SEL_CORD_PLUGGED;
			string SEL_CORD_FOLDED = m_Parent.GetCompEM().SEL_CORD_FOLDED;

			if ( !IsColliding() )
			{
				if ( m_Parent.HasEnergyManager() )
				{
					if ( m_Parent.GetCompEM().IsPlugged() )
					{
						if ( m_Parent.GetCompEM().IsEnergySourceAtReach( GetProjectionPosition() ) )
						{
							projection_entity.SetObjectTexture( GetHiddenSelection( selection ), "#(argb,8,8,3)color(0.5,0.5,0.5,0.75,ca)" );
							projection_entity.SetObjectMaterial( GetHiddenSelection( selection ), hologram_material_path + "\\" + hologram_material + "_powered.rvmat" );
							//Print("s komponentom, zapojeny, nekoliduje, v dosahu");
		
							projection_entity.SetAnimationPhase( SEL_CORD_PLUGGED, 0);
							projection_entity.SetAnimationPhase( SEL_CORD_FOLDED, 1);	
							
 							return;
						}
						else
						{
							projection_entity.SetObjectTexture( GetHiddenSelection( selection ), "#(argb,8,8,3)color(0.5,0.5,0.5,0.75,ca)" );
							projection_entity.SetObjectMaterial( GetHiddenSelection( selection ), hologram_material_path + "\\" + hologram_material + "_deployable.rvmat" );
							//Print("s komponentom, zapojeny, nekoliduje, nie v dosahu");

							projection_entity.SetAnimationPhase( SEL_CORD_PLUGGED, 1);
							projection_entity.SetAnimationPhase( SEL_CORD_FOLDED, 0);
							
							return;
						}
					}
				}
			
				projection_entity.SetObjectTexture( GetHiddenSelection( selection ), "#(argb,8,8,3)color(0.5,0.5,0.5,0.75,ca)" );
				projection_entity.SetObjectMaterial( GetHiddenSelection( selection ), hologram_material_path + "\\" + hologram_material + "_deployable.rvmat" );
				//Print("nekoliduje");
				
				projection_entity.SetAnimationPhase( SEL_CORD_PLUGGED, 1);
				projection_entity.SetAnimationPhase( SEL_CORD_FOLDED, 0);
				
				return;
			}
			else
			{
				projection_entity.SetObjectTexture( GetHiddenSelection( selection ), "#(argb,8,8,3)color(0.5,0.5,0.5,0.75,ca)" );
				projection_entity.SetObjectMaterial( GetHiddenSelection( selection ), hologram_material_path + "\\" + hologram_material + "_undeployable.rvmat" );
				//Print("koliduje");
			}
		}
	}
};