class ActionDisinfectTargetCB : ActionSingleUseBaseCB
{
	private const float QUANTITY_USED_PER_SEC = 10;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CASingleUseQuantity(QUANTITY_USED_PER_SEC);
	}
};


class ActionDisinfectTarget: ActionSingleUseBase
{	
	private const float MAXIMAL_TARGET_DISTANCE = 2;
		
	void ActionDisinfectTarget()
	{
		m_CallbackClass = ActionDisinfectTargetCB;
		m_MessageSuccess = "I disinfected target.";
		m_MessageFail = "I moved and disinfecting was canceled.";
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
		return AT_DISINFECT_T;
	}
		
	string GetText()
	{
		return "disinfect";
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		//RemoveModifiers(target, item); ?
	}
};