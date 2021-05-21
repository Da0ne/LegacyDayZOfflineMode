class ActionSewTargetCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 10;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionSewTarget: ActionContinuousBase
{
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	
	void ActionSewTarget()
	{
		m_CallbackClass = ActionSewTargetCB;
		m_MessageStartFail = "There's no sewing left.";
		m_MessageStart = "Player started sewing you.";
		m_MessageSuccess = "Player finished sewing you.";
		m_MessageFail = "Player moved and sewing was canceled.";
		m_MessageCancel = "You stopped sewing.";
		//m_Animation = "sew";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(MAXIMAL_TARGET_DISTANCE);
	}

	int GetType()
	{
		return AT_SEW_T;
	}
		
	string GetText()
	{
		return "sew target's cuts";
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		PlayerBase ntarget = target;
		ntarget.m_ModifiersManager.DeactivateModifier(MDF_BLEEDING);
		item.AddHealth("GlobalHealth","Health",-10);
	}
};