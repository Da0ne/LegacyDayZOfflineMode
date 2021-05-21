class ActionToggleDigTile: ActionSingleUseBase
{
	const string OBJECT_NAME = "EN5C_GardenPlot";
		
	void ActionToggleDigTile()
	{
		/*
		m_MessageStartFail = "Start failed.";
		m_MessageStart = "Player started placing object.";
		m_MessageSuccess = "Player placed the object."
		m_MessageFail = "Player failed to place the object.";
		m_MessageCancel = "You canceled action.";
		*/
	}
	
	void CreateConditionComponents() 
	{		
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	int GetType()
	{
		return AT_TOGGLE_DIG_TILE;
	}

	string GetText()
	{
		return "Toggle dig tile";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		if( player.IsAlive() && player.GetEntityInHands() == item )
		{
			return true;
		}	
		
		return false;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		TogglePlacing( player, target, item );
	}

	// Toggle placing
	void TogglePlacing( PlayerBase player, Object target, ItemBase item )
	{
		//deactivate if already activated
		if( player.IsPlacingObject() )
		{
			player.PlacingStop();
		}
		else
		{
			player.PlacingStart( OBJECT_NAME );
		}
	}
};