class ActionTurnOffWhileInHands: ActionSingleUseBase
{
	void ActionTurnOffWhileInHands()
	{
		m_MessageSuccess = "I've flipped the switch off.";
		//m_Animation = "close";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	int GetType()
	{
		return AT_TURN_OFF_WHILE_IN_HANDS;
	}

	string GetText()
	{
		return "switch off";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		if ( item.HasEnergyManager()  &&  item.GetCompEM().CanTurnOff() )
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
		item.GetCompEM().SwitchOff();
	}
};