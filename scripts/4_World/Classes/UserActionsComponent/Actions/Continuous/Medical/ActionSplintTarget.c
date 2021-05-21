class ActionSplintTargetCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 5;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionSplintTarget: ActionContinuousBase
{
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	
	void ActionSplintTarget()
	{
		m_CallbackClass = ActionSplintTargetCB;
		m_MessageStartFail = "There's no splint left.";
		m_MessageStart = "Player started fixing you.";
		m_MessageSuccess = "Player finished fixing you.";
		m_MessageFail = "Player moved and fixing was canceled.";
		m_MessageCancel = "You stopped fixing.";
		//m_Animation = "splint";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(MAXIMAL_TARGET_DISTANCE);
	}

	int GetType()
	{
		return AT_SPLINT_T;
	}
		
	string GetText()
	{
		return "apply splint on target";
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		PlayerBase ntarget = target;
		item.TransferModifiers(ntarget);
		ntarget.SetDamage(0);
		ntarget.m_NotifiersManager.DetachByType(NTF_FRACTURE);
		player.IncreaseSkill( PluginExperience.EXP_MEDIC_FIXATING, 2 );
		item.Delete();
	}
};