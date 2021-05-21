class EN5C_Stone extends ItemBase
{
	private const string FIREPLACE_TYPE = "EN5C_FireplaceBase";
	
	bool ConditionAttach ( EntityAI parent )
	{
		if ( parent.IsKindOf( FIREPLACE_TYPE ) )
		{
			EN5C_FireplaceBase fireplace_target = ( EN5C_FireplaceBase ) parent;
			return fireplace_target.CanAttachItem ( this );
		}
		
		return false;
	}
	
	bool ConditionDetach ( EntityAI parent )
	{
		if ( parent.IsKindOf( FIREPLACE_TYPE ) )
		{
			EN5C_FireplaceBase fireplace_target = ( EN5C_FireplaceBase ) parent;
			return fireplace_target.CanDetachItem ( this );
		}
		
		return false;
	}
}