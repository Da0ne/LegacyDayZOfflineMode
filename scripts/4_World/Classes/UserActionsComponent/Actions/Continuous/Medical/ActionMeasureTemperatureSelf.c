class ActionMeasureTemperatureSelfCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 12;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionMeasureTemperatureSelf: ActionContinuousBase
{
	void ActionMeasureTemperatureSelf()
	{
		m_CallbackClass = ActionMeasureTemperatureSelfCB;
		m_MessageStartFail = "There's nothing to measure.";
		m_MessageStart = "I have started measuring myself";
		m_MessageSuccess = "I have measured myself.";
		m_MessageFail = "I have moved and measuring was canceled.";
		m_MessageCancel = "I stopped measuring.";
		//m_Animation = "measure";
	}
	
	void CreateConditionComponents() 
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}

	int GetType()
	{
		return AT_MEASURE_TEMPERATURE_S;
	}
		
	string GetText()
	{
		return "measure temperature";
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		if (player.m_PlayerStats.m_Temperature)
		{
			float temperature = Math.Floor(player.m_PlayerStats.m_Temperature.Get()*10)/10;
			string message = "Thermometer displays "+temperature.ToString()+"°C";
			SendMessageToClient(player, message);
		}
	}
};