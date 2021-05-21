class ActionOpenBarrel: ActionInteractBase
{
	void ActionOpenBarrel()
	{
		m_MessageSuccess = "I have opened it.";
		//m_Animation = "open";
	}

	int GetType()
	{
		return AT_OPEN_BARREL;
	}

	string GetText()
	{
		return "open";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{	
		if ( target.IsItemBase() )
		{
			EN5C_Barrel_ColorBase ntarget = (EN5C_Barrel_ColorBase) target;
			if ( ntarget && !ntarget.IsLocked() && !ntarget.IsOpen()  )
			{
				return true;
			}
		}
		return false;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		EN5C_Barrel_ColorBase ntarget = (EN5C_Barrel_ColorBase) target;
		ntarget.Open();
	}
};