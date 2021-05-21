class EN5C_BarrelHoles_ColorBase extends EN5C_FireplaceBase
{
	//Visual animations
	const string ANIMATION_OPENED 			= "LidOff";
	const string ANIMATION_CLOSED			= "LidOn";
	
	bool IsBarrelHoles()
	{
		return true;
	}
	
	//ATTACHMENTS
	bool CanAttachItem ( ItemBase item )
	{
		//kindling items
		if ( IsKindling ( item ) && IsOpened() )
		{
			return true;
		}
		
		//fuel items
		if ( IsFuel ( item ) && IsOpened() )
		{
			return true;
		}
		
		//cookware
		if ( item.IsKindOf ( ATTACHMENT_COOKING_POT ) && !IsOpened() )
		{
			return true;
		}
		
		return false;
	}

	bool CanDetachItem ( ItemBase item )
	{
		//kindling items
		if ( IsKindling ( item ) && !IsBurning() && IsOpened() )
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
		if ( IsFuel ( item ) && !IsBurning() && IsOpened() )
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
	
	//CONDITIONS
	//ConditionCargoIn/ConditionCargoOut
	bool ConditionCargoIn( EntityAI parent )
	{
		if ( parent && IsOpened() )
		{
			return false;
		}
		
		return true;
	}

	bool ConditionCargoOut( EntityAI parent )
	{
		if ( IsBurning() && IsOpened() )
		{
			return false;
		}
		
		return true;
	}
	
	//ConditionIntoHands
	bool ConditionIntoHands( EntityAI player )
	{
		if ( HasAshes() || IsBurning() || !IsCargoEmpty() || IsItemAttached ( ATTACHMENT_COOKING_POT ) )
		{
			return false;
		}
		
		return true;
	}
	
	//ConditionIntoInventory
	bool ConditionIntoInventory( EntityAI player )
	{
		if ( HasAshes() || IsBurning() || !IsCargoEmpty() || IsItemAttached ( ATTACHMENT_COOKING_POT ) )
		{
			return false;
		}
		
		return true;
	}
	
	//ACTIONS
	bool IsOpened()
	{
		if ( IsInAnimPhase( ANIMATION_OPENED ) )
		{
			return true;
		}
		
		return false;
	}
	
	void Open()
	{
		SetAnimationPhase ( ANIMATION_OPENED, 0 );
		SetAnimationPhase ( ANIMATION_CLOSED, 1 );
	}
	
	void Close()
	{
		SetAnimationPhase ( ANIMATION_OPENED, 1 );
		SetAnimationPhase ( ANIMATION_CLOSED, 0 );		
	}
}
