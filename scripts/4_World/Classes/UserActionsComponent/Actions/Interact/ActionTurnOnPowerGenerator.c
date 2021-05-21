class ActionTurnOnPowerGenerator: ActionInteractBase
{	
	void ActionTurnOnPowerGenerator()
	{
		m_MessageSuccess = "I've started the generator.";
		m_MessageStart = "";
		//m_Animation = "open";
	}	
	
	int GetType()
	{
		return AT_TURN_ON_POWER_GENERATOR;
	}

	string GetText()
	{
		return "switch on";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		ItemBase target_IB = (ItemBase) target;
		string selection;
		g_Game.GetPlayerCursorObjectComponentName( player, /*out*/ selection );
		
		if ( target_IB.HasEnergyManager()  &&  target_IB.GetCompEM().CanSwitchOn()  &&  selection == "switch" )
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