class ActionTurnOnSpotlight: ActionInteractBase
{	
	void ActionTurnOnSpotlight()
	{
		m_MessageSuccess = "I've flipped the switch on.";
		m_MessageStart = "";
	}
	
	int GetType()
	{
		return AT_TURN_ON_SPOTLIGHT;
	}

	string GetText()
	{
		return "switch on spotlight";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		string selection;
		ItemBase target_IB = (ItemBase) target;
		g_Game.GetPlayerCursorObjectComponentName( player, selection );

		if ( selection == EN5C_Spotlight.SEL_REFLECTOR  &&  target_IB.HasEnergyManager()  &&  target_IB.GetCompEM().CanSwitchOn() )
		{
			return true;
		}
		
		return false;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		if (target)
		{
			ItemBase target_IB = (ItemBase) target;
			target_IB.GetCompEM().SwitchOn();
		}
	}
};