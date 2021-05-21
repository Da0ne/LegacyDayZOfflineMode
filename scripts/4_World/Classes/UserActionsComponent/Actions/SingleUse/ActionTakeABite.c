class ActionTakeABiteCB : ActionSingleUseBaseCB
{
	private const float QUANTITY_USED_PER_SEC2 = 50;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CASingleUseQuantityEdible(QUANTITY_USED_PER_SEC2);
	}
};

class ActionTakeABite: ActionConsumeSingle
{
	void ActionTakeABite()
	{
		m_CallbackClass = ActionTakeABiteCB;
		m_MessageSuccess = "I took a bite.";
		m_Sound = "EatingSoft_0";
		//m_Animation = "eat";
	}
	
	void CreateConditionComponents() 
	{	
		
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}	
	
	int GetType()
	{
		return AT_TAKE_A_BITE;
	}

	string GetText()
	{
		return "take a bite";
	}
};