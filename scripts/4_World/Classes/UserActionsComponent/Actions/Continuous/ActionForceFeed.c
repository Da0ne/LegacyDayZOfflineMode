class ActionForceFeedCB : ActionContinuousBaseCB
{
	private const float QUANTITY_USED_PER_SEC2 = 50;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousQuantityEdible(QUANTITY_USED_PER_SEC2);
	}
};

class ActionForceFeed: ActionForceConsume
{	
	
	private const float MAXIMAL_TARGET_DISTANCE2 = 2;

	void ActionForceFeed()
	{
		m_CallbackClass = ActionForceFeedCB;
		m_MessageStartFail = "There's nothing left.";
		m_MessageStart = "I started feeding.";
		m_MessageSuccess = "I finished feeding.";
		m_MessageFail = "Player moved and feeding was canceled.";
		m_MessageCancel = "I stopped feeding.";
		m_MessageStartTarget = "Player started feeding you.";
		m_MessageSuccessTarget = "Player finished feeding you.";
		m_MessageCancelTarget = "Player stopped feeding.";
		//m_Animation = "feed";
	}
	
	void CreateConditionComponents()
	{
		
		m_ConditionTarget = new CCTMan(MAXIMAL_TARGET_DISTANCE2);
		m_ConditionItem = new CCINonRuined;
	}

	int GetType()
	{
		return AT_FEED;
	}
		
	string GetText()
	{
		return "feed";
	}
};