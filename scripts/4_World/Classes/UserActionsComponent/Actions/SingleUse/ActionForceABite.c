class ActionForceABiteCB : ActionSingleUseBaseCB
{
	private const float QUANTITY_USED_PER_SEC2 = 50;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CASingleUseQuantityEdible(QUANTITY_USED_PER_SEC2);
	}
};

class ActionForceABite: ActionForceConsumeSingle
{
	void ActionForceABite()
	{		
		m_CallbackClass = ActionForceABiteCB;
		m_MessageStart = "Player started feeding you a bite.";
		m_MessageSuccess = "Player fed you a bite.";
		m_MessageFail = "Player moved and shoving something in your throat was canceled.";
		//m_Animation = "eat";
	}
	
	void CreateConditionComponents() 
	{		
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(MAXIMAL_TARGET_DISTANCE);
	}
	
	int GetType()
	{
		return AT_FORCE_A_BITE;
	}

	string GetText()
	{
		return "feed a bite";
	}
};