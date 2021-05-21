class ActionToggleFishing: ActionSingleUseBase
{
	void ActionToggleFishing()
	{
		m_MessageStart = " ";
		m_MessageSuccess = " ";
		m_MessageFail = "Fishing rod is ruined.";
		m_Sound = "CastingRod";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	int GetType()
	{
		return AT_TOGGLE_FISHING;
	}

	string GetText()
	{
		return "start or end fishing";
	}


	bool ActionCondition ( PlayerBase player, Object target, ItemBase item )
	{
		vector pos_cursor = GetGame().GetCursorPos();
		float distance = Math.AbsInt(vector.Distance(pos_cursor,player.GetPosition()));
		if ( distance <= 2 /*//m_MaximalActionDistance*/ && g_Game.SurfaceIsPond(pos_cursor[0], pos_cursor[2]) /*player.IsWaterContact()*/ )
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
		EN5C_FishingRod_Base nitem = item;
		if( nitem.IsFishingActive() )
		{
			nitem.DeactivateFishing();
			GetGame().ObjectSetAnimationPhase(item,"CloseRod",1);
			GetGame().ObjectSetAnimationPhase(item,"OpenRod",0);
		}
		else
		{
			nitem.ActivateFishing();
			GetGame().ObjectSetAnimationPhase(item,"CloseRod",0);
			GetGame().ObjectSetAnimationPhase(item,"OpenRod",1);
		}
	}
};