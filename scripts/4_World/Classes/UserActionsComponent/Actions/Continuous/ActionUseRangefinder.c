class ActionUseRangefinderCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 1;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionUseRangefinder : ActionContinuousBase
{
	private autoptr Timer 	m_MeasureDistanceLoopTimer;
	EN5C_Rangefinder 		m_RangefinderItem;
	
	void ActionUseRangefinder()
	{
		m_CallbackClass = ActionUseRangefinderCB;
		m_MessageStartFail = "ActionUseRangefinder - m_MessageStartFail";
		m_MessageStart = "ActionUseRangefinder - m_MessageStart";
		m_MessageSuccess = "ActionUseRangefinder - m_MessageSuccess";
		m_MessageFail = "ActionUseRangefinder - m_MessageFail";
		m_MessageCancel = "ActionUseRangefinder - m_MessageCancel";
		////m_TimeToCompleteAction = 999; // TO DO: Must be infinite!
		m_MeasureDistanceLoopTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
		//m_Animation = "INJECTEPIPENS";	
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}
	
	void ~ActionUseRangefinder()
	{
		delete m_MeasureDistanceLoopTimer;
	}

	int GetType()
	{
		return AT_USE_RANGE_FINDER;
	}
		
	string GetText()
	{
		return "use range finder";
	}

	bool ActionCondition ( PlayerBase player, Object target, ItemBase item )
	{
		return item.GetCompEM().CanWork();
	}

	void OnStart( PlayerBase player, Object target, ItemBase item )
	{	
		m_RangefinderItem = item;
		m_MeasureDistanceLoopTimer.Run( m_RangefinderItem.GetMeasurementUpdateInterval() , this, "ShowDistance", NULL, true);
	}
	
	void ShowDistance()
	{
		if (m_RangefinderItem)
		{
			PlayerBase player = m_RangefinderItem.GetOwnerPlayer();
			
			if (player)
			{
				string message = m_RangefinderItem.DoMeasurement(player);
				SendMessageToClient(player,message);
			}
		}
	}
	
	void OnCancel ( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		m_MeasureDistanceLoopTimer.Stop();
		m_RangefinderItem = NULL;
	}
	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		m_MeasureDistanceLoopTimer.Stop();
		m_RangefinderItem = NULL;
	}
}