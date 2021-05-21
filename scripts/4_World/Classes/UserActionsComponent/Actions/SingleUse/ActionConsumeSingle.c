class ActionConsumeSingleCB : ActionSingleUseBaseCB
{
	private const float QUANTITY_USED_PER_SEC = 1;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CASingleUseQuantityEdible(QUANTITY_USED_PER_SEC);
	}
};

class ActionConsumeSingle: ActionSingleUseBase
{
	void ActionConsumeSingle()
	{
		m_CallbackClass = ActionConsumeSingleCB;
		m_MessageSuccess = "I have taken one.";
		m_MessageStart = "I started taking one.";
		m_MessageStartFail = "Its' empty";
	}
	
	void CreateConditionComponents() 
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}
	
	int GetType()
	{
		return AT_CONSUME_SINGLE;
	}

	string GetText()
	{
		return "take one";
	}
};