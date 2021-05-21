class ActionTestBloodSelfCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 2;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionTestBloodSelf: ActionContinuousBase
{
	void ActionTestBloodSelf()
	{
		m_CallbackClass = ActionTestBloodSelfCB;
		m_MessageStartFail = "It's used up.";
		m_MessageStart = "I have started testing my blood.";
		m_MessageSuccess = "I have tested my blood.";
		m_MessageFail = "I have moved and testing was canceled.";
		m_MessageCancel = "I stopped testing my blood.";
		//m_Animation = "INJECTS";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}

	int GetType()
	{
		return AT_TEST_BLOOD_S;
	}
		
	string GetText()
	{
		return "test blood";
	}


	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		int bloodtype = player.m_PlayerStats.m_BloodType.Get();
		string bloodname = MiscGameplayFunctions.GetBloodTypeName(bloodtype);
		string message = "My blood type is " + bloodname + ".";
		SendMessageToClient(player, message);
		player.IncreaseSkill( PluginExperience.EXP_MEDIC_TRANSFUSION_TEST, 2 );
		item.Delete();
	}
};