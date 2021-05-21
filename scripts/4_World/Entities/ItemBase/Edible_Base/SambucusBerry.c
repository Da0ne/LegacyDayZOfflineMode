class EN5C_SambucusBerry extends EN5C_Edible_Base
{
	void EN5C_SambucusBerry()
	{
	}
	
	bool ConditionAttach ( EntityAI parent )
	{
		//if EN5C_Barrel_ColorBase
		if ( parent.IsInherited( EN5C_Barrel_ColorBase ) )
		{
			EN5C_Barrel_ColorBase barrel = ( EN5C_Barrel_ColorBase ) parent;
			
			if ( barrel.IsOpened() && !barrel.FindAttachmentBySlotName( "Nails" ) && !barrel.FindAttachmentBySlotName( "OakBark" ) && !barrel.FindAttachmentBySlotName( "BirchBark" ) && !barrel.FindAttachmentBySlotName( "Lime" ) && !barrel.FindAttachmentBySlotName( "Disinfectant" ) && !barrel.FindAttachmentBySlotName( "Guts" ) )
			{
				return true;
			}
		}
		
		return false;
	}
	
	bool ConditionDetach ( EntityAI parent )
	{
		//if EN5C_Barrel_ColorBase
		if ( parent.IsInherited( EN5C_Barrel_ColorBase ) )
		{
			EN5C_Barrel_ColorBase barrel = ( EN5C_Barrel_ColorBase ) parent;
			
			if ( !barrel.IsOpened() )
			{
				return false;
			}
		}
		
		return true;
	}
}

