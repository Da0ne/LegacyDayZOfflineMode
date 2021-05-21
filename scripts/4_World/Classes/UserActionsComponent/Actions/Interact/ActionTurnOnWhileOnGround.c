class ActionTurnOnWhileOnGround: ActionInteractBase
{
	void ActionTurnOnWhileOnGround()
	{
		m_MessageSuccess = "I've flipped the switch on.";
		m_MessageFail = "I've flipped the switch off.";
		m_MessageStart = "";		
	}
	
	int GetType()
	{
		return AT_TURN_ON_WHILE_ON_GROUND;
	}

	string GetText()
	{
		return "switch on";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		ItemBase target_IB = (ItemBase) target;
		if ( player.IsAlive()  &&  target_IB.HasEnergyManager()  &&  target_IB.GetCompEM().CanSwitchOn() )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		if (target)
		{
			ItemBase target_IB = (ItemBase) target;
			target_IB.GetCompEM().SwitchOn();
		}
	}
};