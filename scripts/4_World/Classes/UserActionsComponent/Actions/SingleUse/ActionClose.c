class ActionClose: ActionSingleUseBase
{
	void ActionClose()
	{
		m_MessageSuccess = "I have closed it.";
		//m_Animation = "close";
	}
	
	void CreateConditionComponents() 
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	int GetType()
	{
		return AT_CLOSE;
	}

	string GetText()
	{
		return "close";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		if ( item && item.IsOpen() )
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
		item.Close();
	}
};