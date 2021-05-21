class ActionMeasureBattery: ActionSingleUseBase
{

	void ActionMeasureBattery()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_LICKBATTERY;
		m_MessageSuccess = "There's still some juice in it.";
		m_MessageFail = "It's dead.";
		m_MessageStartFail = "It's dead.";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	int GetType()
	{
		return AT_MEASURE_BATTERY;
	}
		
	string GetText()
	{
		return "lick battery";
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		
		float energy = 0;
		if ( item.HasEnergyManager() ) 
		{
			energy = item.GetCompEM().GetEnergy();
		}
		item.TransferModifiers(player);
		if(energy > 0) InformPlayers(player,target,UA_FINISHED);
		if(energy == 0) InformPlayers(player,target,UA_FAILED);
	}
};