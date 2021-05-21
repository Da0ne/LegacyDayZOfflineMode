class ActionTakeFireplaceIndoor: ActionInteractBase
{
	void ActionTakeFireplaceIndoor()
	{
		m_MessageSuccess = "I took the fireplace.";
		//m_Animation = "close";
	}

	int GetType()
	{
		return AT_TAKE_FIREPLACE_INDOOR;
	}

	string GetText()
	{
		return "take";
	}
	
	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		//empty hands	--interact action base condition
		if ( target )
		{
			EN5C_FireplaceIndoor fireplace_indoor = ( EN5C_FireplaceIndoor ) target;
	
			if ( !fireplace_indoor.HasAshes() && !fireplace_indoor.IsBurning() && fireplace_indoor.IsCargoEmpty() && !fireplace_indoor.GetCookingEquipment() )
			{
				return true;
			}
		}

		return false;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		EN5C_FireplaceIndoor fireplace_indoor = ( EN5C_FireplaceIndoor ) target;
		
		//create fireplace object
		EN5C_Fireplace fireplace = GetDayZGame().CreateObject( "EN5C_Fireplace", "0 0 0", true, false );
		
		//transfer all required parameters to this object (damage, wetness)
		fireplace.SetDamage( fireplace_indoor.GetDamage() );
		fireplace.SetWet( fireplace_indoor.GetWet() );
		
		//transfer all cargo items to this object, position of all items must be preserved
		//fireplace in hands cannot have any items in it but this can be changed in the future
		Cargo cargo = fireplace_indoor.GetCargo();
		for ( int i = 0; i < cargo.GetItemCount(); i++ )
		{
			int col;
			int row;
			cargo.GetItemPos( i, col, row );
			fireplace.TakeEntityToCargoAtPos( cargo.GetItem ( i ), fireplace.GetCargo(), row, col );
		}
		
		//transfer all attachments to this object
		int item_attachments_count = fireplace_indoor.AttachmentsCount();
		for ( int j = 0; j < item_attachments_count; j++ )
		{
			EntityAI entity = fireplace_indoor.GetAttachmentFromIndex( 0 );
			fireplace.TakeEntityAsAttachment( entity );
		}
		
		//set building settings (m_FireplacePoints) and fireplaceindoor settings
		fireplace_indoor.GetFireplacePoint().ClearObject();
		fireplace_indoor.ClearFireplacePoint();
	}
}