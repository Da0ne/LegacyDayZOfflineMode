class ActionCloseBarrelHoles: ActionInteractBase
{
	void ActionCloseBarrelHoles()
	{
		m_MessageSuccess = "I have closed it.";
		//m_Animation = "close";
	}

	int GetType()
	{
		return AT_CLOSE_BARREL_HOLES;
	}

	string GetText()
	{
		return "close";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		if ( target.IsItemBase() )
		{
			EN5C_BarrelHoles_ColorBase ntarget = ( EN5C_BarrelHoles_ColorBase ) target;
			if ( ntarget && ntarget.IsOpened() )
			{
				return true;
			}
		}
		return false;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		EN5C_BarrelHoles_ColorBase ntarget = ( EN5C_BarrelHoles_ColorBase ) target;
		ntarget.Close();
	}
}