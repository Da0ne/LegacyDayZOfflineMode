class EN5C_Bark_Oak extends EN5C_Bark_ColorBase
{
	bool ConditionAttach ( EntityAI parent )
	{
		EN5C_FireplaceBase fireplace_target = ( EN5C_FireplaceBase ) parent;
		return fireplace_target.CanAttachItem ( this );
	}
	
	bool ConditionDetach ( EntityAI parent )
	{
		EN5C_FireplaceBase fireplace_target = ( EN5C_FireplaceBase ) parent;
		return fireplace_target.CanDetachItem ( this );
	}
}
