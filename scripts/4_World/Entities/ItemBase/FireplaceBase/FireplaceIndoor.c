class EN5C_FireplaceIndoor extends EN5C_FireplaceBase
{
	FireplacePoint m_FireplacePoint;
	
	//fireplace point
	FireplacePoint GetFireplacePoint()
	{
		return m_FireplacePoint;
	}
	
	void SetFireplacePoint( FireplacePoint fireplace_point )
	{
		m_FireplacePoint = fireplace_point;
	}
	
	void ClearFireplacePoint()
	{
		SetFireplacePoint( NULL );
	}
	
	//State
	bool IsFireplaceIndoor()
	{
		return true;
	}
	
	//attachments
	bool CanAttachItem ( ItemBase item )
	{
		//kindling items
		if ( IsKindling ( item ) )
		{
			return true;
		}
		
		//fuel items
		if ( IsFuel ( item ) )
		{
			return true;
		}
		
		//cookware
		if ( item.IsKindOf ( ATTACHMENT_COOKING_POT ) )
		{
			return true;
		}
		
		return false;
	}

	bool CanDetachItem ( ItemBase item )
	{
		//kindling items
		if ( IsKindling ( item ) && !IsBurning() )
		{
			if ( HasLastFuelKindlingAttached() )
			{
				if ( HasLastAttachment() )
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return true;
			}
		}
		
		//fuel items
		if ( IsFuel ( item ) && !IsBurning() )
		{
			if ( HasLastFuelKindlingAttached() )
			{	
				if ( HasLastAttachment() )
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				return true;
			}
		}
		
		//cookware
		if ( item.IsKindOf ( ATTACHMENT_COOKING_POT ) )
		{
			return true;
		}
		
		return false;
	}

	void EEItemAttached ( EntityAI item, string slot_name ) 
	{
		super.EEItemAttached ( item, slot_name );
		
		ItemBase item_base = ( ItemBase ) item;
		
		//kindling / fuel
		if ( IsKindling ( item_base ) || IsFuel ( item_base ) )
		{
			//add to consumables
			AddToFireConsumables ( item_base );
		}
		
		//cookware
		if ( item_base.IsKindOf ( ATTACHMENT_COOKING_POT ) )
		{
			SetCookingEquipment( item_base );
			
			//rotate handle
			item_base.SetAnimationPhase ( ANIMATION_COOKWARE_HANDLE, 1 );
		}
		
		//refresh fireplace visuals
		RefreshFireplaceVisual();
	}

	void EEItemDetached ( EntityAI item, string slot_name ) 
	{
		super.EEItemDetached ( item, slot_name );
		
		ItemBase item_base = ( ItemBase ) item;
		
		//kindling / fuel
		if ( IsKindling ( item_base ) || IsFuel ( item_base ) )
		{
			//remove from consumables
			RemoveFromFireConsumables ( item_base );
			
			//no attachments left & no ashes are present
			if ( AttachmentsCount() == 0 && !HasAshes() )
			{
				//destroy fireplace
				DestroyFireplace();
			}
		}

		//cookware
		if ( item_base.IsKindOf ( ATTACHMENT_COOKING_POT ) )
		{
			ClearCookingEquipment();
			
			//rotate handle
			item_base.SetAnimationPhase ( ANIMATION_COOKWARE_HANDLE, 1 );
		}	
		
		//refresh fireplace visuals
		RefreshFireplaceVisual();
	}
	
	//ConditionCargoIn/ConditionCargoOut
	bool ConditionCargoIn( EntityAI parent )
	{
		if ( parent )
		{
			return false;
		}
		
		return true;
	}

	bool ConditionCargoOut( EntityAI parent )
	{
		if ( IsBurning() )
		{
			return false;
		}
		
		return true;
	}
	
	//ConditionIntoHands
	bool ConditionIntoHands( EntityAI player )
	{
		return false;
	}
	
	//ConditionIntoInventory
	bool ConditionIntoInventory( EntityAI player )
	{
		return false;
	}
}
