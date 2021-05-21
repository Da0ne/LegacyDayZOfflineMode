class ActionTestBloodTargetCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 5;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionTestBloodTarget: ActionContinuousBase
{
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	
	void ActionTestBloodTarget()
	{
		m_CallbackClass = ActionTestBloodTargetCB;
		m_MessageStartFail = "It's used up.";
		m_MessageStart = "Player started testing your blood.";
		m_MessageSuccess = "Player finished testing your blood.";
		m_MessageFail = "Player moved and testing was canceled.";
		m_MessageCancel = "You stopped testing.";
	//	m_Animation = "testblood";
	}
	
	void CreateConditionComponents() 
	{
		
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(MAXIMAL_TARGET_DISTANCE);
	}

	int GetType()
	{
		return AT_TEST_BLOOD_T;
	}
		
	string GetText()
	{
		return "test blood of target";
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		PlayerBase ntraget = target;
		int bloodtype = ntraget.m_PlayerStats.m_BloodType.Get();
		string bloodname = MiscGameplayFunctions.GetBloodTypeName(bloodtype);
		string message = "Test kit shows " + bloodname + ".";
		SendMessageToClient(player, message);
		player.IncreaseSkill( PluginExperience.EXP_MEDIC_TRANSFUSION_TEST, 2 );
		item.Delete();
	}
};