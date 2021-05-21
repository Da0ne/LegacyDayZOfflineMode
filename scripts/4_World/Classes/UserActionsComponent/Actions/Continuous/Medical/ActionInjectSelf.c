class ActionInjectSelfCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 3;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionInjectSelf: ActionContinuousBase
{
	void ActionInjectSelf()
	{
		m_CallbackClass = ActionInjectSelfCB;
		m_MessageStartFail = "There's nothing left.";
		m_MessageStart = "I have started injecting myself";
		m_MessageSuccess = "I have injected myself.";
		m_MessageFail = "I have moved and injecting was canceled.";
		m_MessageCancel = "I stopped injecting.";
		
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_INJECTION;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
	}
	
	void CreateConditionComponents() 
	{	
		
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}

	int GetType()
	{
		return AT_INJECT_S;
	}
		
	string GetText()
	{
		return "inject";
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		item.Delete();
	}
};