class ActionOpenBarrelHoles: ActionInteractBase
{
	void ActionOpenBarrelHoles()
	{
		m_MessageSuccess = "I have opened it.";
		//m_Animation = "open";
	}

	int GetType()
	{
		return AT_OPEN_BARREL_HOLES;
	}

	string GetText()
	{
		return "open";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{	
		if ( target.IsItemBase() )
		{
			EN5C_BarrelHoles_ColorBase ntarget = ( EN5C_BarrelHoles_ColorBase ) target;
			if ( ntarget && !ntarget.IsOpened() && !ntarget.IsItemAttached ( ntarget.ATTACHMENT_COOKING_POT ) )
			{
				return true;
			}
		}
		return false;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		EN5C_BarrelHoles_ColorBase ntarget = ( EN5C_BarrelHoles_ColorBase ) target;
		ntarget.Open();
	}
}