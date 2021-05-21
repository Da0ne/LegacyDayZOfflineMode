class ActionForceABiteCan: ActionForceABite
{
	int GetType()
	{
		return AT_FORCE_A_BITE_CAN;
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{	
		return true;
		/* 
		//if staging for cans is introduced
		if ( item && item.IsOpen() )
		{
			return true;
		}
		return false;
		*/
	}
};