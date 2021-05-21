class ActionFillBottleBaseCB : ActionContinuousBaseCB
{
	private const float QUANTITY_FILLED_PER_SEC = 50;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousFill(QUANTITY_FILLED_PER_SEC);
	}
};

class ActionFillBottleBase: ActionContinuousBase
{
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	
	void ActionFillBottleBase()
	{
		m_CallbackClass = ActionFillBottleBaseCB;
		//m_CommandUID = DayZPlayerConstants.CMD_ACTION_FILLBOTTLEWELL;
		m_MessageStartFail = "It's ruined.";
		m_MessageStart = "I have started filling the bottle.";
		m_MessageSuccess = "I have finished filling the bottle..";
		m_MessageFail = "Player moved and filling the bottle was canceled.";
		m_MessageCancel = "I stopped filling the bottle.";
	}
	
	void CreateConditionComponents() 
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTDummy;
	}

	int GetType()
	{
		return AT_FILL_BOTTLE;
	}
		
	string GetText()
	{
		return " fill bottle";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{	
		vector pos_cursor = GetGame().GetCursorPos();
		if ( g_Game.SurfaceIsPond(pos_cursor[0], pos_cursor[2]) &&  vector.Distance(player.GetPosition(),pos_cursor) < MAXIMAL_TARGET_DISTANCE /*player.IsWaterContact()*/ && item.GetQuantity2() <= item.GetQuantityMax2() ) 
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};