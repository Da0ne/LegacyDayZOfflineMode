class ActionDisinfectSelfCB : ActionSingleUseBaseCB
{
	private const float QUANTITY_USED_PER_SEC = 10;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CASingleUseQuantity(QUANTITY_USED_PER_SEC);
	}
};


class ActionDisinfectSelf: ActionSingleUseBase
{
	float m_GramsConsumedPerUse;
	
	void ActionDisinfectSelf()
	{
		m_CallbackClass = ActionDisinfectSelfCB;
		m_MessageSuccess = "I disinfected myself";
		m_MessageStartFail = "Its' empty";
		//m_Animation = "lick";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}

	int GetType()
	{
		return AT_DISINFECT_S;
	}
		
	string GetText()
	{
		return "disinfect self";
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		//RemoveModifiers(target, item); ?
	}
};