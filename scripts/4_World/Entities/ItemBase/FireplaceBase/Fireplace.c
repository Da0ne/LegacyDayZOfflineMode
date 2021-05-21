class EN5C_Fireplace extends EN5C_FireplaceBase
{
	bool IsBaseFireplace()
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
			if ( IsItemAttached ( ATTACHMENT_TRIPOD ) || IsInAnimPhase( ANIMATION_OVEN ) ) 
			{
				return true;
			}
		}
		
		//tripod
		if ( item.IsKindOf ( ATTACHMENT_TRIPOD ) )
		{
			if ( !IsInAnimPhase( ANIMATION_OVEN ) && GetOwner() == NULL )
			{
				return true;
			}
		}
		
		//stones
		if ( item.IsKindOf ( ATTACHMENT_STONES ) && GetOwner() == NULL )
		{
			float item_quantity = item.GetQuantity();
				
			//can transform to oven
			if ( item_quantity >= 8 && !IsBurning() && !IsItemAttached ( ATTACHMENT_TRIPOD ) )
			{
				return true;
			}
			
			if ( item_quantity < 8 && item_quantity >= 1 )
			{
				return true;
			}
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
		
		//tripod
		if ( item.IsKindOf ( ATTACHMENT_TRIPOD ) && !GetCookingEquipment() )
		{
			return true;
		}
		
		//stones
		if ( item.IsKindOf ( ATTACHMENT_STONES ) )
		{
			float item_quantity = item.GetQuantity2();
			
			if ( IsInAnimPhase( ANIMATION_OVEN ) )
			{
				if ( IsBurning() || GetCookingEquipment() )
				{
					return false;
				}
				else
				{
					return true;
				}
			}
			
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
			
			//rotate handle (if not in 'Oven' stage)
			if ( !IsInAnimPhase ( ANIMATION_OVEN ) )
			{
				item_base.SetAnimationPhase ( ANIMATION_COOKWARE_HANDLE, 0 );
			}
			
			//refresh cookware visuals
			//TODO... event_fnc_cookingEquipmentState - add to cooking equipment script
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
		if ( HasAshes() || IsBurning() || IsInAnimPhase ( ANIMATION_OVEN ) || IsInAnimPhase ( ANIMATION_TRIPOD ) || IsInAnimPhase ( ANIMATION_STONES ) || !IsCargoEmpty() )
		{
			return false;
		}
		
		return true;
	}
	
	//ConditionIntoInventory
	bool ConditionIntoInventory( EntityAI player )
	{
		if ( HasAshes() || IsBurning() || IsInAnimPhase ( ANIMATION_OVEN ) || IsInAnimPhase ( ANIMATION_TRIPOD ) || IsInAnimPhase ( ANIMATION_STONES ) || !IsCargoEmpty() )
		{
			return false;
		}
		
		return true;
	}
}
