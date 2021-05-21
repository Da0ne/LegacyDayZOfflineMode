class ActionSplintSelfCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 10;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionSplintSelf: ActionContinuousBase	
{
	void ActionSplintSelf()
	{
		m_CallbackClass = ActionSplintSelfCB;
		m_MessageStartFail = "There's no splint left";
		m_MessageStart = "I have started fixing myself";
		m_MessageSuccess = "I have fixed myself.";
		m_MessageFail = "I have moved and fixing was canceled.";
		m_MessageCancel = "I stopped fixing.";
		//m_Animation = "bandage";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}

	int GetType()
	{
		return AT_SPLINT_S;
	}
		
	string GetText()
	{
		return "apply splint";
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		item.TransferModifiers(player);
		player.SetDamage(0);
		player.m_NotifiersManager.DetachByType(NTF_FRACTURE);
		player.IncreaseSkill( PluginExperience.EXP_MEDIC_FIXATING, 2 );
		item.Delete();
	}
};