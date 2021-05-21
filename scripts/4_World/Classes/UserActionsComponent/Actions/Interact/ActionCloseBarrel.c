class ActionCloseBarrel: ActionInteractBase
{
	void ActionCloseBarrel()
	{
		m_MessageSuccess = "I have closed it.";
		//m_Animation = "close";
	}

	int GetType()
	{
		return AT_CLOSE_BARREL;
	}

	string GetText()
	{
		return "close";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		if ( target.IsItemBase() )
		{
			EN5C_Barrel_ColorBase ntarget = (EN5C_Barrel_ColorBase) target;
			if ( ntarget && !ntarget.IsLocked() && ntarget.IsOpen() )
			{
				return true;
			}
		}
		return false;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		EN5C_Barrel_ColorBase ntarget = (EN5C_Barrel_ColorBase) target;
		ntarget.DetermineAction(player);
	}
};