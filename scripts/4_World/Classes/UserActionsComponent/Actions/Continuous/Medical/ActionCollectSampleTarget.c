class ActionCollectSampleTargetCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 2;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionCollectSampleTarget: ActionContinuousBase
{	
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	
	void ActionCollectSampleTarget()
	{
		m_CallbackClass = ActionCollectSampleTargetCB;
		m_MessageStartFail = "iT'S full.";
		m_MessageStart = "Player started collectin sample of your blood.";
		m_MessageSuccess = "Player finished collecting sample of your blood.";
		m_MessageFail = "Player moved and collecting of sample was canceled.";
		m_MessageCancel = "You stopped collecting.";
		//m_Animation = "splint";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(MAXIMAL_TARGET_DISTANCE);
	}

	int GetType()
	{
		return AT_COLLECT_SAMPLE_T;
	}
		
	string GetText()
	{
		return "collect sample from target";
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		PlayerBase ntarget = target;
		ItemBase syringe = GetGame().CreateObject("EN5C_BloodSyringe", player.GetPosition());
		MiscGameplayFunctions.TransferItemProperties(item, syringe, true, false, true);
		syringe.SetLiquidType2(ntarget.m_PlayerStats.m_BloodType.Get());
		item.Delete();
		player.TakeEntityToHands(syringe);
	}
};