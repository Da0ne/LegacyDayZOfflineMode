class ActionUnrestrainTargetCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 10;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionUnrestrainTarget: ActionContinuousBase
{
	private const float MAXIMAL_TARGET_DISTANCE = 1;
	
	void ActionUnrestrainTarget()
	{
		m_CallbackClass = ActionUnrestrainTargetCB;
		m_MessageStartFail = "It's broken.";
		m_MessageStart = "Player started unrestraining you using item.";
		m_MessageSuccess = "Player finished unrestraining you using item.";
		m_MessageFail = "Player moved and unrestraining was canceled.";
		m_MessageCancel = "You stopped unrestraining.";
		//m_Animation = "inject";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionTarget = new CCTMan(MAXIMAL_TARGET_DISTANCE);
		m_ConditionItem = new CCINonRuined;
	}

	int GetType()
	{
		return AT_UNRESTRAIN_T;
	}
		
	string GetText()
	{
		return "unrestrain";
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		PlayerBase ntarget = target;
		//restraintarget
		item.Delete();
	}
};