class ActionCoverPlantedSeed: ActionInteractBase
{
	void ActionCoverPlantedSeed()
	{
		m_MessageSuccess = "";
		m_MessageStart = "";
	}

	int GetType()
	{
		return AT_COVER_PLANTED_SEED;
	}

	string GetText()
	{
		return "cover planted slot";
	}


	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		if ( target  &&  target.IsInherited( GardenBase) )
		{
			GardenBase garden_base = (GardenBase)target;
			
			string selection;
			GetGame().GetPlayerCursorObjectComponentName( player, selection );
			
			Slot slot = garden_base.GetSlotBySelection( selection );
			
			if ( slot  &&  slot.m_State == Slot.STATE_PLANTED  &&  slot.GetPlant() == NULL  &&  slot.GetSeed() )
			{
				return true;
			}
		}
		
		return false;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		bool can_complete = ActionCondition( player, target, item );
		
		if (can_complete) // Double check status of the slot after the animation
		{
			GardenBase garden_base = (GardenBase)target;
			string selection;
			GetGame().GetPlayerCursorObjectComponentName( player, selection );
			
			Slot slot = garden_base.GetSlotBySelection( selection );
			int slot_index = garden_base.GetSlotIndexBySelection( selection );
			
			garden_base.CreatePlant(slot, slot_index);
		}
	}
};