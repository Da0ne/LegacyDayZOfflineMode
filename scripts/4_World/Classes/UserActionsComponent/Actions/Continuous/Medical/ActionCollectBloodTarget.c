class ActionCollectBloodTargetCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 2;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionCollectBloodTarget: ActionContinuousBase
{
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	
	void ActionCollectBloodTarget()
	{
		m_CallbackClass = ActionCollectBloodTargetCB;
		m_MessageStartFail = "There's no bandage left.";
		m_MessageStart = "Player started bandaging you.";
		m_MessageSuccess = "Player finished bandaging you.";
		m_MessageFail = "Player moved and bandaging was canceled.";
		m_MessageCancel = "You stopped bandaging.";
		//m_Animation = "collectblood";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(MAXIMAL_TARGET_DISTANCE);
	}

	int GetType()
	{
		return AT_COLLECT_BLOOD_T;
	}
		
	string GetText()
	{
		return "collect blood from target";
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		PlayerBase ntarget = target;
		item.TransferModifiers(ntarget);
		player.IncreaseSkill( PluginExperience.EXP_MEDIC_TRANSFUSION_COLLECT, 3 );
		ItemBase bag = GetGame().CreateObject("EN5C_BloodBagFull", player.GetPosition());
		MiscGameplayFunctions.TransferItemProperties(item, bag, true, false, true);
		bag.SetLiquidType2(ntarget.m_PlayerStats.m_BloodType.Get());
		item.Delete();
		player.TakeEntityToHands(bag);
	}
};