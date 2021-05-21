class ActionUnplugThisByCord: ActionInteractBase
{
	void ActionUnplugThisByCord()
	{
		m_MessageSuccess = "I've unplugged and folded the cord.";
		m_MessageStart = "";
	}

	int GetType()
	{
		return AT_UNPLUG_THIS_BY_CORD;
	}
		
	string GetText()
	{
		return "fold the cord";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		ItemBase target_IB = (ItemBase) target;
		
		if ( target_IB  &&  target_IB.HasEnergyManager() )
		{
			string selection;
			g_Game.GetPlayerCursorObjectComponentName( player, selection );
			
			if ( selection == ComponentEnergyManager.SEL_CORD_PLUGGED )
			{
				return true;
			}
			
			// Special case for unfolded spotlight
			if ( selection == EN5C_Spotlight.SEL_CORD_PLUGGED )
			{
				return true;
			}
		}
		
		return false;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		ItemBase target_IB = (ItemBase) target; // cast to ItemBase
		target_IB.GetCompEM().UnplugThis();
		
		if ( target_IB.IsInherited(EN5C_Spotlight) )
		{
			target_IB.HideSelection ( EN5C_Spotlight.SEL_CORD_PLUGGED );
			target_IB.ShowSelection ( EN5C_Spotlight.SEL_CORD_FOLDED );
		}
	}
};