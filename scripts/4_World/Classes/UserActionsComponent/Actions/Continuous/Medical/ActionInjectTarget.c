class ActionInjectTargetCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 4;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionInjectTarget: ActionContinuousBase
{
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	
	void ActionInjectTarget()
	{
		m_CallbackClass = ActionInjectTargetCB;
		m_MessageStartFail = "There's nothing left.";
		m_MessageStart = "Player started injecting you.";
		m_MessageSuccess = "Player finished injecting you.";
		m_MessageFail = "Player moved and injecting was canceled.";
		m_MessageCancel = "You stopped injecting.";
		//m_Animation = "INJECTEPIPENT";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(MAXIMAL_TARGET_DISTANCE);
	}

	int GetType()
	{
		return AT_INJECT_T;
	}
		
	string GetText()
	{
		return "inject target";
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		item.Delete();
	}
};