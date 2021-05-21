class ActionTuneFrequencyCB : ActionContinuousBaseCB
{
	private const float REPEAT_AFTER_SEC = 1;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousRepeat(REPEAT_AFTER_SEC);
	}
};

class ActionTuneFrequency: ActionContinuousBase
{
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	
	void ActionTuneFrequency()
	{
		m_CallbackClass = ActionTuneFrequencyCB;
		m_MessageStartFail = "I have failed the tunning.";
		m_MessageStart = "I have started the tunning.";
		m_MessageFail = "I have failed the tunning.";
		m_MessageCancel = "I have stopped the tunning.";
		//m_Animation = "DRINK";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionTarget = new CCTNonRuined(MAXIMAL_TARGET_DISTANCE);
		m_ConditionItem = new CCINonRuined;
	}

	int GetType()
	{
		return AT_TUNE_FREQUENCY;
	}
		
	string GetText()
	{
		return "tune frequency";
	}

	bool ActionCondition ( PlayerBase player, Object target, ItemBase item )
	{	
		EN5C_Transmitter_Base transmitter = ( EN5C_Transmitter_Base ) item;
		
		if ( transmitter.CanOperate() && item.GetDamage() < 1 ) 
		{
			return true;
		}
		
		return false;
	}

	void OnRepeat ( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		EN5C_Transmitter_Base transmitter = ( EN5C_Transmitter_Base ) item;
		
		transmitter.SetNextFrequency ( player );
	}
};