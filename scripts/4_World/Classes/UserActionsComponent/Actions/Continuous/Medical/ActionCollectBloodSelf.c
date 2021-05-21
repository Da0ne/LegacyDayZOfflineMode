class ActionCollectBloodSelfCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 2;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionCollectBloodSelf: ActionContinuousBase
{
	void ActionCollectBloodSelf()
	{
		m_CallbackClass = ActionCollectBloodSelfCB;
		m_MessageStartFail = "Bag is full.";
		m_MessageStart = "I have started collecting blood.";
		m_MessageSuccess = "I have collected my blood.";
		m_MessageFail = "I have moved and collecting blood was canceled.";
		m_MessageCancel = "I stopped collecting blood.";
		//m_Animation = "bandage";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}

	int GetType()
	{
		return AT_COLLECT_BLOOD_S;
	}
		
	string GetText()
	{
		return "collect blood";
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		item.TransferModifiers(player);
		player.IncreaseSkill(PluginExperience.EXP_MEDIC_TRANSFUSION_COLLECT, 3);
		ItemBase bag = GetGame().CreateObject("EN5C_BloodBagFull", player.GetPosition());
		MiscGameplayFunctions.TransferItemProperties(item, bag, true, false, true);
		bag.SetLiquidType2(player.m_PlayerStats.m_BloodType.Get());
		//bag.SetQuantity2(acdata.param1, false);
		item.Delete();
		player.TakeEntityToHands(bag);
	}
};