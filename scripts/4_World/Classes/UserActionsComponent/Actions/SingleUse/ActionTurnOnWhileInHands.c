class ActionTurnOnWhileInHands: ActionSingleUseBase
{
	void ActionTurnOnWhileInHands()
	{
		m_MessageSuccess = "It's turned on.";
		m_MessageFail = "It's out of energy";
		//m_Animation = "open";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	int GetType()
	{
		return AT_TURN_ON_WHILE_IN_HANDS;
	}

	string GetText()
	{
		return "switch on";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		if ( item.HasEnergyManager()  &&  item.GetCompEM().CanSwitchOn()  &&  item.GetCompEM().CanWork() )
		{
			return true;
		}
		
		return false;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )  //Takhle to nedelat, vyrob si action componentu a tehle check patri do jeji Execute metody. Message o selhani pak napis jako messageStartFail
	{
		if ( item.GetCompEM().CanWork() )
		{
			item.GetCompEM().SwitchOn();
		} 
		else
		{
			InformPlayers(player,target,UA_FAILED);
		}
	}
};
