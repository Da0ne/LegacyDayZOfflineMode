class TrapSpawnBase extends ItemBase
{
	int   m_InitWaitTime; 			//After this time after deployment, the trap is activated
	int   m_UpdateWaitTime; 		//Time when timer will be called again until success or not succes catch is done
	float m_DefectRate; 			//Added damage after trap activation
	bool m_NeedInstalation;			//If installation of trap is needed
	bool m_BaitNeeded;				//if bait is needed for sucessfull catch
	bool m_IsFoldable;
	bool m_IsUsable;
	float m_MinimalDistanceFromPlayersToCatch;

	private bool m_IsActive;
	private bool m_IsInProgress;
	
	autoptr Timer m_Timer;
	autoptr Timer m_PrevTimer;
	private ItemBase m_Catch;
	
	autoptr Timer m_AlignCatchTimer;

	string m_AnimationPhaseSet;
	string m_AnimationPhaseTriggered;
	string m_AnimationPhaseUsed;

	autoptr map<string, float> m_SurfaceForSetup;	//map of surfaces where trap can be installed - string surface name, float catch_chance on that surface
	bool m_WaterSurfaceForSetup;	//if trap can be installed on water surface (cannot be detected via getsurfacetype)
	autoptr multiMap<string, float>	m_CatchesPond;	//array of catches that can be catched in trap - string key, float catch_chance, float minCatch, float maxCatch
	autoptr multiMap<string, float>	m_CatchesSea;	//array of catches that can be catched in trap - string key, float catch_chance, float minCatch, float maxCatch
	autoptr multiMap<string, float>	m_CatchesGroundAnimal;	//array of catches that can be catched in trap - string key, float catch_chance, float minCatch, float maxCatch
	string m_InfoSetup;

	void TrapSpawnBase()
	{
		m_DefectRate = 0.10; 			//Added damage after trap activation
		m_UpdateWaitTime = 0;
		m_IsInProgress = false;
		m_InitWaitTime = 99;
		m_NeedInstalation = true;
		m_BaitNeeded = false;
		m_InfoSetup = 				"I've placed the trap";
		m_IsFoldable = false;
		m_IsUsable = false;
		m_MinimalDistanceFromPlayersToCatch = 0;

		m_AnimationPhaseSet = "";
		m_AnimationPhaseTriggered = "";
		m_AnimationPhaseUsed = "";
		
		m_SurfaceForSetup = NULL;
		m_CatchesPond = NULL;
		m_CatchesSea = NULL;
		m_CatchesGroundAnimal = NULL;
		
		m_PrevTimer = NULL;
	}

	void ~TrapSpawnBase()
	{
		// TODO: ak ma v sebe cargo treba ho rusit v destruktore????
	}

	bool IsActive()
	{
		if ( m_IsActive && m_IsInProgress == false )
		{
			return true;
		}
		return false;
	}

	bool IsTakeable()
	{
		if ( g_Game.IsServer() )
		{
			Cargo itemcargo = this.GetCargo();
			int cargo_count = itemcargo.GetItemCount();
			if ( cargo_count > 0 )
			{
				return false;
			}
		}
		return true;
	}

	bool IsPlaceable()
	{
		if ( g_Game.IsServer() )
		{
			if ( this.GetOwnerPlayer() != NULL && this.GetOwnerPlayer().GetEntityInHands() == this )
			{
				GetGame().UpdateCursorTarget();
				
				PlayerBase player = (PlayerBase)this.GetOwnerPlayer();
				
				vector player_pos = player.GetPosition();
				vector aim_pos = player.GetAimPosition();
				
				if ( vector.Distance(player_pos, aim_pos) <= 1.5 )
				{
					return IsPlaceableAtPosition( aim_pos );
				}
			}
		}
		
		return false;
	}

	bool IsPlaceableAtPosition( vector position )
	{
		string surface_type;
		GetGame().SurfaceGetType(position[0], position[2], surface_type);

		// check water (sea) surface
		if ( m_WaterSurfaceForSetup && GetGame().SurfaceIsSea( position[0], position[2] ) )
		{
			return true;
		}

		// check water (pond) surface
		if ( m_WaterSurfaceForSetup && GetGame().SurfaceIsPond( position[0], position[2] ) )
		{
			return true;
		}
		// EN5C_SmallFishTrap is amphibious, on water serves as a fish trap, on surface serves as rain procurement vesel
		if ( IsKindOf("EN5C_SmallFishTrap") )
		{
			if ( !GetGame().SurfaceIsPond( position[0], position[2] ) &&  !GetGame().SurfaceIsSea( position[0], position[2] ) )
			{
				return true;
			} 
		}
		
		// check surface
		if ( m_SurfaceForSetup != NULL )
		{
			for (int i = 0; i < m_SurfaceForSetup.Count(); i++)
			{
				string surface = m_SurfaceForSetup.GetKey(i);
				if ( surface_type == surface )
				{
					return true;
				}
			}
		}
		
		return false;
	}

	void SetupTrap()
	{ 
		if ( GetGame().IsServer() )
		{
			if ( this.GetOwnerPlayer() && this.GetOwnerPlayer().CanDropEntity( this) )  // kvoli desyncu
			{
				SetupTrapPlayer( this.GetOwnerPlayer() );
			}
		}
	}

	void SetupTrapPlayer( PlayerBase player, bool set_position = true )
	{ 
		if ( GetGame().IsServer() )
		{
			if ( this.GetDamage() >= 1 ) 
			{
				player.MessageImportant( "The trap is ruined." );
			}
			else
			{
				player.PlayActionGlobal( "deployItem" );
				
				if ( set_position )
				{
					player.DropEntity( this );

					vector trapPos = ( player.GetDirection() * 1.5 );
					trapPos[1] = 0;
					this.SetPosition( player.GetPosition() + trapPos );
				}
				
				player.MessageStatus( m_InfoSetup );

				SetActive();
			}
		}
	}
	
	void Fold()
	{
		if ( g_Game.IsServer() && m_IsFoldable == true )
		{
			SetInactive();
		}
	}

	bool AddDefect()
	{
		if ( g_Game.IsServer() )
		{
			this.SetDamage( this.GetDamage() + m_DefectRate );
		}
	}
	
	void StartActivate( PlayerBase player )
	{
	}
	
	void SetActive()
	{
		if ( g_Game.IsServer() && m_IsActive == false )
		{
			m_IsInProgress = false;
			m_IsActive = true;

			this.AddDefect();
			if ( m_AnimationPhaseSet != "" && m_AnimationPhaseTriggered != "" )
			{
				this.SetAnimationPhase( m_AnimationPhaseSet, 1 );
				this.SetAnimationPhase( m_AnimationPhaseTriggered, 0 );
				this.SetAnimationPhase( m_AnimationPhaseUsed, 1 );
			}

			m_Timer = new Timer( CALL_CATEGORY_GAMEPLAY );
			m_Timer.Run( m_InitWaitTime, this, "SpawnCatch" );
		}
	}

	void SetInactive()
	{
		if ( g_Game.IsServer() )
		{
			if ( this.IsActive() )
			{
				this.AddDefect();
			}
			m_IsActive = false;
			if ( m_Timer )
			{
				m_Timer.Stop();
			}
			if ( m_AnimationPhaseSet != "" && m_AnimationPhaseTriggered != "" && !m_IsUsable )
			{
				this.SetAnimationPhase( m_AnimationPhaseSet, 0 );
				this.SetAnimationPhase( m_AnimationPhaseTriggered, 1 );
				this.SetAnimationPhase( m_AnimationPhaseUsed, 1 );
			}
		}
	}

	void SetUsed()
	{
		if ( g_Game.IsServer() )
		{
			if ( this.IsActive() )
			{
				this.AddDefect();
			}
			m_IsActive = false;
			if ( m_Timer )
			{
				m_Timer.Stop();
			}
			if ( m_AnimationPhaseSet != "" && m_AnimationPhaseTriggered != "" )
			{
				this.SetAnimationPhase( m_AnimationPhaseSet, 1 );
				this.SetAnimationPhase( m_AnimationPhaseTriggered, 1 );
				this.SetAnimationPhase( m_AnimationPhaseUsed, 0 );
			}
		}
	}
	
	void SpawnCatch()
	{
		if ( g_Game.IsServer() )
		{
			if ( m_MinimalDistanceFromPlayersToCatch > 0 && m_UpdateWaitTime > 0 )
			{
				if ( m_PrevTimer != NULL )
				{
					delete m_PrevTimer;
				}
				m_PrevTimer = m_Timer;
				m_Timer = new Timer( CALL_CATEGORY_GAMEPLAY );
				m_Timer.Run( m_UpdateWaitTime, this, "SpawnCatch" );
			}

			int i;
			bool canCatch = false;

			if ( m_BaitNeeded )
			{
				Cargo itemcargo = this.GetCargo();
				int cargo_count = itemcargo.GetItemCount();

				if ( itemcargo && cargo_count > 0 )
				{
					for ( i = 0; i < cargo_count; i++ )
					{
						InventoryItem ii = itemcargo.GetItem(i);
						string tmp;
						GetGame().ObjectGetType (ii, tmp);

						// if bait is needed and correct bait is used, spawn catch
						if ( tmp == "Food_Worm" ||  tmp == "Consumable_Bait" )	//todo: set food in table params
						{
							// delete bait
							g_Game.ObjectDelete( ii );
							if ( ii.GetDamage() < 0.4 )
							{
								canCatch = true;
							}
						}
					}
				}
			}
			else
			{
				canCatch = true;
			}
			
			if ( m_MinimalDistanceFromPlayersToCatch > 0 )
			{
				autoptr array<Man> players = new array<Man>;
				g_Game.GetWorld().GetPlayerList( players );
					
				bool minimal_distance_ok = true;	
					
				for ( i = 0; i < players.Count(); i++ )
				{
					if ( vector.Distance(players.Get(i).GetPosition(), this.GetPosition()) < m_MinimalDistanceFromPlayersToCatch )
					{
						minimal_distance_ok = false;
						break;
					}
				}
				
				if ( !minimal_distance_ok && canCatch )
				{
					canCatch = false;
				}
			}
			
			if ( canCatch )
			{
				float surface_chance_for_catch = 1.0;
				
				multiMap<string, float>	catches;
				vector pos = GetPosition();
				if ( m_WaterSurfaceForSetup )
				{
					if ( g_Game.SurfaceIsSea( pos[0], pos[2] ) ) 
					{
						catches = m_CatchesSea;
					}
					else if ( g_Game.SurfaceIsPond( pos[0], pos[2] ) ) 
					{
						catches = m_CatchesPond;
					}
				}
				else
				{
					if ( m_SurfaceForSetup != NULL )
					{
						string surface_type;
						GetGame().SurfaceGetType(pos[0], pos[2], surface_type);
						
						if ( m_SurfaceForSetup.Contains(surface_type) )
						{
							surface_chance_for_catch = m_SurfaceForSetup.Get( surface_type );
						}
					}
					
					catches = m_CatchesGroundAnimal;
				}

				if ( catches && catches.Count() > 0 )
				{
					// create catch in move to cargo
					// select random object from catches
					int count = catches.Count() - 1;
					int randomCatchIndex = Math.RandomInt( 0, count );
				
					float chance_for_catch = surface_chance_for_catch * catches.GetByIndex ( randomCatchIndex ).Get(0);
				
					if ( chance_for_catch > Math.RandomFloat(0, 1) )
					{
						// how many catches random(min,max)
						int minQuantity = catches.GetByIndex ( randomCatchIndex ).Get(1);
						int maxQuantity = catches.GetByIndex ( randomCatchIndex ).Get(2);
						int quantityCatch =  Math.RandomInt( minQuantity, maxQuantity );

						if ( quantityCatch > 0 )
						{
							for (int iii = 0; iii < quantityCatch; iii++)
							{			
								if ( catches != m_CatchesGroundAnimal )
								{
									m_Catch = g_Game.CreateObject( catches.GetKeyByIndex(randomCatchIndex), this.GetPosition(), false );
									m_Catch.SetQuantity( 1 );
									this.TakeEntityToCargo( m_Catch );
								}
								else 
								{
									m_Catch = g_Game.CreateObject( catches.GetKeyByIndex(randomCatchIndex), this.GetPosition(), false, true );
									m_Catch.SetDamage( 1 );
									
									m_AlignCatchTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
									m_AlignCatchTimer.Run( 2, this, "AlignCatch", new Param2<ItemBase, string>(m_Catch, catches.GetKeyByIndex(randomCatchIndex)), false );
									
									SetInactive();
								}
							}
						}
					}
					else if ( m_IsUsable )
					{
						SetUsed();
					}
				}
				
				if ( !m_IsUsable )
				{
					SetInactive();
				}
			}
		}
	}

	void AlignCatch( ItemBase obj, string catch_name )
	{

	}

	void EEItemLocationChanged  ( EntityAI old_owner, EntityAI new_owner ) 
	{
		if ( g_Game.IsServer() )
		{
			// throw trap from vicinity
			if ( new_owner == NULL && m_NeedInstalation == false )
			{
				SetActive();
			}
			else if ( old_owner == NULL && new_owner != NULL )
			{
				if ( m_IsFoldable )
				{
					Fold();
				}
				else
				{
					SetInactive();
				}
			}

		}
	}
	
	void OnPlacementFinished( PlayerBase player )
	{
		SetupTrapPlayer( player, false );
	}

	bool CanBePlaced( PlayerBase player, vector position )
	{
		return IsPlaceableAtPosition( position );
	}

	string CanBePlacedFailMessage( PlayerBase player, vector position )
	{
		return "Trap can't be placed on this surface type.";
	}
}

/*
TODO:
animation phases in params
*/