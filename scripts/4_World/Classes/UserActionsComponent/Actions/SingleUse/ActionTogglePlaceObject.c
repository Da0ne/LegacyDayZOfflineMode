class ActionTogglePlaceObject: ActionSingleUseBase
{

	void ActionTogglePlaceObject()
	{
		/*
		m_MessageStartFail = "Start failed.";
		m_MessageStart = "Player started placing object.";
		m_MessageSuccess = "Player placed the object."
		m_MessageFail = "Player failed to place the object.";
		m_MessageCancel = "You canceled action.";
		*/
		
		m_MessageSuccess = "I've attached it.";
	}
	
	void CreateConditionComponents() 
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	int GetType()
	{
		return AT_TOGGLE_PLACE_OBJECT;
	}

	string GetText()
	{
		return "Toggle place object";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		if( player.IsAlive() && player.GetEntityInHands() == item )
		{
			return true;
		}	
		
		return false;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		TogglePlacing( player, target, item );
	}

	// Toggle placing
	void TogglePlacing( PlayerBase player, Object target, ItemBase item )
	{
		//deactivate if already activated
		if( player.IsPlacingObject() )
		{
			player.PlacingStop();
			AttachItemToTarget( player, target, item );
			if( item )
			{
				item.OnPlacementCancelled( player );
			}
		}
		else
		{
			player.PlacingStart( player.GetItemInHands().GetType() );
		}
	}

	// Attaches held item to target object, if any exists.
	void AttachItemToTarget( PlayerBase player, Object target, ItemBase item )
	{
		if ( target  &&  item )
		{
			EntityAI target_EAI = (EntityAI)target;
			
			if ( target_EAI.CanTakeEntityAsAttachment(item) )
			{
				string item_name;
				string target_name;
				GetGame().ObjectGetDisplayName(item, item_name);
				GetGame().ObjectGetDisplayName(target, target_name);
				target_EAI.TakeEntityAsAttachment(item);
				m_MessageSuccess = "I've attached " + item_name + " onto " + target_name + ".";
			}
		}
	}
};