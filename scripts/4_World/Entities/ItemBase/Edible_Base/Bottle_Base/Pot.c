class EN5C_Pot extends EN5C_Bottle_Base
{
	private const string FIREPLACE_TYPE 		= "EN5C_FireplaceBase";
	private const string GAS_PORT_STOVE_TYPE 	= "EN5C_PortableGasStove";
	
	bool ConditionAttach ( EntityAI parent )
	{
		if ( parent.IsKindOf( FIREPLACE_TYPE ) )
		{
			EN5C_FireplaceBase fireplace_target = ( EN5C_FireplaceBase ) parent;
			return fireplace_target.CanAttachItem ( this );
		}
		
		if ( parent.IsKindOf( GAS_PORT_STOVE_TYPE ) )
		{
			if ( parent.GetOwner() )	//not in inventory or cargo
			{
				return false;
			}
			
			return true;
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
		
		if ( parent.IsKindOf( GAS_PORT_STOVE_TYPE ) )
		{
			return true;
		}
		
		return false;
	}
}