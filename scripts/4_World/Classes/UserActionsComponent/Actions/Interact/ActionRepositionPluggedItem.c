class ActionRepositionPluggedItem: ActionInteractBase
{
	autoptr Timer m_TimerPlacement;
	
// Through this action players can reposition already placed electric devices without unplugging them from the power source.
	void ActionRepositionPluggedItem()
	{
		m_MessageStart = "";
		m_MessageSuccess = "I've picked up the device.";
		m_TimerPlacement = new Timer( CALL_CATEGORY_GAMEPLAY );
	}

	int GetType()
	{
		return AT_REPOSITION_PLUGGED_ITEM;
	}

	string GetText()
	{
		return "reposition";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		ItemBase target_IB = (ItemBase) target;
		PlayerBase item_owner = target_IB.GetOwnerPlayer();
		
		if (target_IB.HasEnergyManager())
		{
			if ( !item_owner )
			{
				if ( target_IB.GetCompEM().IsPlugged() )
				{
					return true;
				}
			}
		}
		else
		{
			string target_type = target_IB.GetType();
			string warning = "WARNING! " + target_type + " is not an electric device, so it cannot be repositioned!";
			DPrint(warning);
		}
		
		return false;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		ItemBase target_IB = (ItemBase) target;
		
		if ( target_IB.IsKindOf("EN5C_CableReel") )
		{
			CableReel CR = (CableReel) target_IB;
			CR.ForceIntoHandsNow(player);
		}
		else
		{
			player.TakeEntityToHands(target_IB);
		}
		
		if ( !m_TimerPlacement.IsRunning() ) // Timer is necesarry because putting the item into hands is delayed!
		{
			m_TimerPlacement.Run(0.1, this, "OnStartPlacement", new Param3<PlayerBase, Object, ItemBase>(player, target, target_IB), false );
		}
	}

	void OnStartPlacement( PlayerBase player, Object target, ItemBase target_IB )
	{
		if ( player  &&  !player.IsPlacingObject() )
		{
			ActionTogglePlaceObject toggle_action = (ActionTogglePlaceObject) player.m_ActionManager.GetAction ( AT_TOGGLE_PLACE_OBJECT );
			toggle_action.TogglePlacing ( player, target, target_IB );
		}
	}
};