class TentBase extends ItemBase
{
	static const int PACKED 	= 0;
	static const int PITCHED 	= 1;
	
	private int m_State;
	
	protected autoptr map<ToggleSelections, bool> m_ToggleSelections;
	protected autoptr array<string> m_ShowSelectionsWhenPitched;
	protected autoptr array<string> m_HideSelectionsWhenPacked;
	
	void TentBase()
	{
		m_ToggleSelections = new map<ToggleSelections, bool>;
		m_ShowSelectionsWhenPitched = new array<string>;
		m_HideSelectionsWhenPacked = new array<string>;
	}
	
	void Init()
	{
		Pack( false );
	}
	
	bool ConditionIntoInventory( EntityAI player )
	{
		return CanBeManipulated();
	}
	
	bool ConditionIntoHands( EntityAI player )
	{
		return CanBeManipulated();
	}

	bool ConditionOutOfHands( EntityAI player )
	{
		return CanBeManipulated();
	}
	
	void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);
		
		if ( !g_Game.IsServer() ) return;

		if ( item.IsInherited(EN5C_XmasLights) ) 
		{
			EN5C_XmasLights fence_light = (EN5C_XmasLights)item;
			fence_light.AttachToObject(this);
			SetAnimationPhase( "xlights", 0 );
		}
		else if ( item.IsKindOf ( "EN5C_CamoNet" ) ) 
		{
			SetAnimationPhase( "camonet", 0 );
			
			RegenerateNavmesh();
		}
	}

	void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);
		
		if ( !g_Game.IsServer() ) return;
		
		if ( item.IsInherited(EN5C_XmasLights) ) 
		{
			EN5C_XmasLights fence_light = (EN5C_XmasLights)item;
			fence_light.DetachFromObject(this);
			SetAnimationPhase( "xlights", 1 );
			SetAnimationPhase( "placing", 0 );
		}
		else if ( item.IsKindOf ( "EN5C_CamoNet" ) ) 
		{
			SetAnimationPhase( "camonet", 1 );
			SetAnimationPhase( "placing", 0 );
			
			RegenerateNavmesh();
		}
	}
	
	void OnHologramItemCreated()
	{
		Pitch( false );
	}
	
	void OnPlacementFinished( PlayerBase player )
	{
		Pitch();
	}
	
	int GetState()
	{
		return m_State;
	}

	bool CanBePacked()
	{
		if ( GetState() == PITCHED )
		{
			if ( GetCargo().GetItemCount() == 0 && AttachmentsCount() == 0 )
			{
				return true;
			}
		}
		
		return false;
	}

	bool CanBeManipulated()
	{
		if ( GetState() == PACKED )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	void Pack( bool update_navmesh = true )
	{
		m_State = PACKED;
		LockInventory(LOCK_FROM_SCRIPT);
		
		SetAnimationPhase( "placing", 1 );
		SetAnimationPhase( "inventory", 0 );
		
		for ( int i = 0; i < m_HideSelectionsWhenPacked.Count(); i++ )
		{
			SetAnimationPhase( m_HideSelectionsWhenPacked.Get(i), 1 );
		}
		
		if ( update_navmesh ) 
		{
			Print("Pack->RegenerateNavmesh");
			RegenerateNavmesh();	
		}
	}

	void Pitch( bool update_navmesh = true )
	{
		m_State = PITCHED;
		UnlockInventory(LOCK_FROM_SCRIPT);
		
		SetAnimationPhase( "placing", 0 );
		SetAnimationPhase( "inventory", 1 );
		
		for ( int i = 0; i < m_ShowSelectionsWhenPitched.Count(); i++ )
		{
			SetAnimationPhase( m_ShowSelectionsWhenPitched.Get(i), 0 );
		}
		
		if ( update_navmesh ) 
		{
			Print("Pitch->RegenerateNavmesh");
			RegenerateNavmesh();	
		}
	}
	
	bool CanToggleSelection( string selection )
	{
		for ( int i = 0; i < m_ToggleSelections.Count(); i++ )
		{
			ToggleSelections toggle = m_ToggleSelections.GetKey(i);
			
			if ( toggle.m_ToggleOff == selection || toggle.m_ToggleOn == selection )
			{
				return true;
			}
		}
		
		return false;
	}

	void ToggleSelection( string selection )
	{
		for ( int i = 0; i < m_ToggleSelections.Count(); i++ )
		{
			ToggleSelections toggle = m_ToggleSelections.GetKey(i);
			
			if ( toggle.m_ToggleOff == selection || toggle.m_ToggleOn == selection )
			{
				if ( m_ToggleSelections.GetElement(i) )
				{
					SetAnimationPhase( toggle.m_ToggleOff, 0 );
					SetAnimationPhase( toggle.m_ToggleOn, 1 );
					m_ToggleSelections.Set( toggle, false );
				}
				else
				{
					SetAnimationPhase( toggle.m_ToggleOff, 1 );
					SetAnimationPhase( toggle.m_ToggleOn, 0 );
					m_ToggleSelections.Set( toggle, true );
				}
			}
		}
	}
	
	void RegenerateNavmesh()
	{
		SetAffectPathgraph( true, false );
		
		Timer update_timer = new Timer ( CALL_CATEGORY_SYSTEM );
		update_timer.Run ( 0.1, GetGame(), "UpdatePathgraphRegionByObject", new Param1<Object>( this ), false );
	}
};
