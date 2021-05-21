class ActionWashHands: ActionSingleUseBase
{
	void ActionWashHands()
	{
		m_MessageSuccess = "I have washed my hands.";
		//m_Animation = "open";
	}
	
	void CreateConditionComponents() 
	{		
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTDummy;
	}

	int GetType()
	{
		return AT_WASH_HANDS;
	}

	string GetText()
	{
		return "Wash hands";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		if ( player.GetHasBloodyHands() && item.GetQuantity2() > 0 )
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
		player.WashHands();
	}
};