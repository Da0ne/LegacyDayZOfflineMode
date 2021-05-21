class ActionTurnOffSpotlight: ActionInteractBase
{	
	void ActionTurnOffSpotlight()
	{
		m_MessageStart = "";
		m_MessageSuccess = "I've flipped the switch off";
	}
	
	int GetType()
	{
		return AT_TURN_OFF_SPOTLIGHT;
	}

	string GetText()
	{
		return "switch off spotlight";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		string selection;
		ItemBase target_IB = (ItemBase) target;
		g_Game.GetPlayerCursorObjectComponentName( player, /*out*/ selection );
		
		if ( selection == EN5C_Spotlight.SEL_REFLECTOR  &&  target_IB.HasEnergyManager()  &&  target_IB.GetCompEM().CanTurnOff() )
		{
			return true;
		}
		
		return false;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		if ( target )
		{
			ItemBase target_IB = (ItemBase) target;
			target_IB.GetCompEM().SwitchOff();
		}
	}
};