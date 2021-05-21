class ActionForceConsumeCB : ActionContinuousBaseCB
{
	private const float QUANTITY_USED_PER_SEC = 50;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousQuantityEdible(QUANTITY_USED_PER_SEC);
	}
};

class ActionForceConsume: ActionContinuousBase
{		
	
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	
	void ActionForceConsume()
	{
		m_CallbackClass = ActionForceConsumeCB;
		m_MessageStartFail = "There's nothing left.";
		m_MessageStart = "I started feeding target.";
		m_MessageStartTarget = "Player started shoving something in your throat.";
		m_MessageSuccess = "I finished feeding.";
		m_MessageSuccessTarget = "Player finished shoving something in your throat.";
		m_MessageCancelTarget = "Player stopped showing something in your mouth.";
		m_MessageFail = "Player moved too far away.";
		m_MessageCancel = "I stopped shoving something in targets throat.";		
	}
	
	void CreateConditionComponents() 
	{
		m_ConditionTarget = new CCTMan(MAXIMAL_TARGET_DISTANCE);
		m_ConditionItem = new CCINonRuined;
	}

	int GetType()
	{
		return AT_FORCE_CONSUME;
	}
		
	string GetText()
	{
		return "shove into throat of target";
	}
};