class ActionOpen: ActionSingleUseBase
{
	void ActionOpen()
	{
		m_MessageSuccess = "I have opened it.";
		//m_Animation = "open";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	int GetType()
	{
		return AT_OPEN;
	}

	string GetText()
	{
		return "open";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		if ( item && !item.IsOpen() )
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
		item.Open();
	}
};