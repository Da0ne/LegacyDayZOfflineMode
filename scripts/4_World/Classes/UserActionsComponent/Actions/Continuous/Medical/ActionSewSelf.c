class ActionSewSelfCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 10;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionSewSelf: ActionContinuousBase
{
	void ActionSewSelf()
	{
		m_CallbackClass = ActionSewSelfCB;
		m_MessageStartFail = "There's no sewing left.";
		m_MessageStart = "I have started sewing myself";
		m_MessageSuccess = "I have sewed myself.";
		m_MessageFail = "I have moved and sewing was canceled.";
		m_MessageCancel = "I stopped sewing.";
		//m_Animation = "bandage";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}

	int GetType()
	{
		return AT_SEW_S;
	}
			
	string GetText()
	{
		return "sew cuts";
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		item.AddHealth("GlobalHealth","Health",-10);
		player.m_ModifiersManager.DeactivateModifier(MDF_BLEEDING);
	}
};