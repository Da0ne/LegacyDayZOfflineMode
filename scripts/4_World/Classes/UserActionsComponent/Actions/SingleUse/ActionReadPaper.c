class ActionReadPaper: ActionSingleUseBase
{
	void ActionReadPaper()
	{
		m_MessageSuccess = "I broke chemlight.";
		m_MessageFail = "It's out of energy";
		//m_Animation = "break";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	int GetType()
	{
	 return AT_READ_PAPER;
	}

	string GetText()
	{
		return "read";
	}
		

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
	//display note
	}
};