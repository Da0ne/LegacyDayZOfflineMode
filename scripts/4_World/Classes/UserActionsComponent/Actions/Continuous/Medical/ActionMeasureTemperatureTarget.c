class ActionMeasureTemperatureTargetCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 12;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);	
	}
};

class ActionMeasureTemperatureTarget : ActionContinuousBase
{
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	
	void ActionMeasureTemperatureTarget()
	{
		m_CallbackClass = ActionMeasureTemperatureTargetCB;
		m_MessageStartFail = "There's nothing to measure.";
		m_MessageStart = "Player started messuring you.";
		m_MessageSuccess = "Player finished measuring you.";
		m_MessageFail = "Player moved and measuring was canceled.";
		m_MessageCancel = "You stopped measuring.";
		//m_Animation = "measure";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(MAXIMAL_TARGET_DISTANCE);
	}

	int GetType()
	{
		return AT_MEASURE_TEMPERATURE_T;
	}
		
	string GetText()
	{
		return "measure temperature of target";
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		PlayerBase ntarget = target;
		if (ntarget.m_PlayerStats.m_Temperature) 
		{ 
			float temperature = Math.Floor(ntarget.m_PlayerStats.m_Temperature.Get()*10)/10;
			string message = "Thermometer displays "+temperature.ToString()+"°C";
			SendMessageToClient(player, message);

		}
	}
};