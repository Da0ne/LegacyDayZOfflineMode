class ActionForceConsumeSingleCB : ActionSingleUseBaseCB
{
	const float QUANTITY_USED_PER_SEC = 1;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CASingleUseQuantityEdible(QUANTITY_USED_PER_SEC);
	}
};

class ActionForceConsumeSingle: ActionSingleUseBase
{	
	const float MAXIMAL_TARGET_DISTANCE = 2;
	
	void ActionForceConsumeSingle()
	{
		m_CallbackClass = ActionForceConsumeSingleCB;
		m_MessageStart = "Player started shoving something in your throat.";
		m_MessageSuccess = "Player finished shoving something in your throat.";
		m_MessageFail = "Player moved and shoving something in your throat was canceled.";
		m_MessageStartFail = "Its' empty";
		//m_Animation = "forcefeed";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(MAXIMAL_TARGET_DISTANCE);
	}

	int GetType()
	{
		return AT_FORCE_CONSUME_SINGLE;
	}
		
	string GetText()
	{
		return "shove into throat of target";
	}

		
	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		if ( target ) GetGame().ObjectSetAnimationPhase(target,"Stump",1);

	}
};