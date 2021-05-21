class ActionTurnOffPowerGenerator: ActionInteractBase
{	
	void ActionTurnOffPowerGenerator()
	{
		m_MessageSuccess = "I've turned off the generator.";
		m_MessageStart = "";
		//m_Animation = "close";
	}
	
	int GetType()
	{
		return AT_TURN_OFF_POWER_GENERATOR;
	}

	string GetText()
	{
		return "switch off";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		ItemBase target_IB = (ItemBase) target;
		string selection;
		g_Game.GetPlayerCursorObjectComponentName( player, /*out*/ selection );
		
		if ( target_IB.HasEnergyManager()  &&  target_IB.GetCompEM().CanTurnOff()  &&  selection == "switch" )
		{
			return true;
		}
		else
		{
			return false;
		}
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