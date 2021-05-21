class ActionConsumeCB : ActionContinuousBaseCB
{
	private const float QUANTITY_USED_PER_SEC = 1;
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousQuantityEdible(QUANTITY_USED_PER_SEC);
	}
};

class ActionConsume: ActionContinuousBase
{
	
	
	void ActionConsume()
	{
		m_CallbackClass = ActionConsumeCB;
		m_MessageStartFail = "There's nothing left.";
		m_MessageStart = "I have started consuming.";
		m_MessageSuccess = "I have finished consuming.";
		m_MessageFail = "Player moved and consuming was canceled.";
		m_MessageCancel = "I stopped consuming.";
		//m_Animation = "EATTABLETS";		
	}
	
	void CreateConditionComponents() 
	{	
		
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}

	int GetType()
	{
		return AT_CONSUME;
	}
		
	string GetText()
	{
		return "consume";
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
	}
};