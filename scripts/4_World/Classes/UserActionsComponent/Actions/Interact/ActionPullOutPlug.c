class ActionPullOutPlug: ActionInteractBase
{
	void ActionPullOutPlug()
	{
		m_MessageSuccess = "I've unplugged the device.";
		m_MessageStart = "";
	}

	int GetType()
	{
		return AT_PULL_OUT_PLUG;
	}
		
	string GetText()
	{
		return "unplug";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		EntityAI target_EAI = (EntityAI) target; // cast to EntityAI
		
		if ( target_EAI.HasEnergyManager() )
		{
			string selection;
			g_Game.GetPlayerCursorObjectComponentName( player, selection );
			EntityAI device = target_EAI.GetCompEM().GetPlugOwner(selection);
			
			if ( device )
			{
				return true;
			}
		}
		
		return false;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		if ( target )
		{
			EntityAI target_EAI = (EntityAI) target;
			string selection;
			
			g_Game.GetPlayerCursorObjectComponentName( player, selection );
			EntityAI device = target_EAI.GetCompEM().GetPlugOwner(selection);
			
			if ( device )
			{
				device.GetCompEM().UnplugThis();
			}
			
			// Disable Advanced Placement
			if ( player.IsPlacingObject() )
			{
				ActionTogglePlaceObject toggle_action = (ActionTogglePlaceObject) player.m_ActionManager.GetAction ( AT_TOGGLE_PLACE_OBJECT );
				toggle_action.TogglePlacing ( player, target, item );
			}
		}
	}
};