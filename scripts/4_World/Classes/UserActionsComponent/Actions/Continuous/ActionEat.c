class ActionEatCB : ActionContinuousBaseCB
{
	private const float QUANTITY_USED_PER_SEC2 = 50;

	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousQuantityEdible(QUANTITY_USED_PER_SEC2);
	}
};

class ActionEat: ActionConsume
{
	void ActionEat()
	{
		m_CallbackClass = ActionEatCB;
		m_MessageStartFail = "There's nothing to eat.";
		m_MessageStart = "I have started eating.";
		m_MessageSuccess = "I have finished eating.";
		m_MessageFail = "Player moved and eating was canceled.";
		m_MessageCancel = "I stopped eating.";
		m_Sound = "EatingSoft_0";
		//m_Animation = "EAT";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}

	int GetType()
	{
		return AT_EAT;
	}
		
	string GetText()
	{
		return "eat";
	}
};