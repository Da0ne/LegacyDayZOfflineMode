class ActionForceDrinkCB : ActionContinuousBaseCB
{
	private const float QUANTITY_USED_PER_SEC2 = 50;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousQuantityEdible(QUANTITY_USED_PER_SEC2);
	}
};

class ActionForceDrink: ActionForceConsume
{	
	private const float MAXIMAL_TARGET_DISTANCE2 = 2;
	
	void ActionForceDrink()
	{
		m_CallbackClass = ActionForceDrinkCB;
		m_MessageStartFail = "There's nothing left.";
		m_MessageStart = "I started forcing target drink.";
		m_MessageStartTarget = "Player started forcing you drink.";
		m_MessageSuccess = "I finished force drinking.";
		m_MessageSuccessTarget = "Player finished sforce drinking you.";
		m_MessageCancelTarget = "Player stopped force drinking you.";
		m_MessageFail = "Player moved too far away.";
		m_MessageCancel = "I stopped forcing target to drink.";	
	}
	
	void CreateConditionComponents() 
	{
		
		m_ConditionTarget = new CCTMan(MAXIMAL_TARGET_DISTANCE2);
		m_ConditionItem = new CCINonRuined;
	}

	int GetType()
	{
		return AT_WATER;
	}
		
	string GetText()
	{
		return "water";
	}
};
	
	