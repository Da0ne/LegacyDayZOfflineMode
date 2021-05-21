class ActionRestrainTargetCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 8;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionRestrainTarget: ActionContinuousBase
{
	private const float MAXIMAL_TARGET_DISTANCE = 1;
	
	void ActionRestrainTarget()
	{
		m_CallbackClass = ActionRestrainTargetCB;
		m_MessageStartFail = "Restraints are damage.";
		m_MessageStart = "Player started restraining you.";
		m_MessageSuccess = "Player finished restraining you.";
		m_MessageFail = "Player moved and restraining was canceled.";
		m_MessageCancel = "You stopped restraining.";
		//m_Animation = "restrain";
		m_Sound = "action_handcuff_0";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionTarget = new CCTMan(MAXIMAL_TARGET_DISTANCE);
		m_ConditionItem = new CCINonRuined;
	}

	int GetType()
	{
		return AT_RESTRAIN_T;
	}
		
	string GetText()
	{
		return "restrain";
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		PlayerBase ntarget = target;
		//ntarget.SetCaptive();
		item.Delete();
	}
};