class ActionDrinkPond: ActionInteractBase
{
	void ActionDrinkPond()
	{
		m_MessageSuccess = "I have taken a sip.";
		m_MessageStartFail = "Fail..";
		
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DRINKPOND;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
	}

	int GetType()
	{
		return AT_DRINK_POND;
	}

	string GetText()
	{
		return "take a sip";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{	
		vector pos_cursor = GetGame().GetCursorPos();
		if (  g_Game.SurfaceIsPond(pos_cursor[0], pos_cursor[2]) /*player.IsWaterContact()*/ )
		{
			return true;
		}
		return false;
	}


	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		Param1<float> nacdata = acdata;
		player.m_PlayerStats.m_StomachWater.Add( nacdata.param1 );
	}
};