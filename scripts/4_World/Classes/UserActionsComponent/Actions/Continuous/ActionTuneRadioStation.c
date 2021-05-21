class ActionTuneRadioStationCB : ActionContinuousBaseCB
{
	private const float REPEAT_AFTER_SEC = 1;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousRepeat(REPEAT_AFTER_SEC);
	}
};

class ActionTuneRadioStation: ActionContinuousBase
{
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	
	void ActionTuneRadioStation()
	{
		m_CallbackClass = ActionTuneRadioStationCB;
		m_MessageStartFail = "I've failed to change the radio station.";
		m_MessageStart = "I've changed the radio station.";
		m_MessageFail = "I've failed to change the radio station.";
		m_MessageCancel = "I have stopped the radio tunning.";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionTarget = new CCTDummy;
		m_ConditionItem = new CCINonRuined;
	}

	int GetType()
	{
		return AT_TUNE_RADIO_STATION;
	}
		
	string GetText()
	{
		return "tune radio station";
	}

	bool ActionCondition ( PlayerBase player, Object target, ItemBase item )
	{	
		if ( item.HasEnergyManager()  &&  item.GetCompEM().IsSwitchedOn() && item.GetDamage() < 1 ) 
		{
			return true;
		}
		
		return false;
	}

	void OnRepeat ( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		EN5C_Radio radio = ( EN5C_Radio ) item;
		
		radio.TuneNextStation ( player );
	}
};