class EN5C_MediumGasCannister extends ItemBase
{
	bool ConditionAttach ( EntityAI parent )
	{
		return true;
	}
	
	bool ConditionDetach ( EntityAI parent )
	{
		return true;
	}

	bool CanExplodeInFire()
	{
		return true;
	}	
}