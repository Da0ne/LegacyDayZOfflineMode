class ActionPlaceOnGroundCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 4;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionPlaceOnGround: ActionContinuousBase
{
	void ActionPlaceOnGround()
	{
		m_CallbackClass = ActionPlaceOnGroundCB;
		m_MessageStartFail = "There's nothing to place.";
		m_MessageStart = "I have started placing it on ground.";
		m_MessageSuccess = "I have placed it on ground.";
		m_MessageFail = "I have moved and placing was canceled.";
		m_MessageCancel = "I stopped placing it on the ground.";
		//m_Animation = "drop";
	}
	
	void CreateConditionComponents() 
	{		
		m_ConditionTarget = new CCTNone;
		m_ConditionItem = new CCIDummy;
	}

	int GetType()
	{
		return AT_PLACE;
	}
		
	string GetText()
	{
		return "place on ground";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{	
		if ( player && player.IsAlive() && item.GetDamage() < 1 ) //&& IsConscious && IsNotCaptured
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		player.DropItem(item);
	}
};