class ActionTurnOffWhileOnGround: ActionInteractBase
{	
	void ActionTurnOffWhileOnGround()
	{
		m_MessageSuccess = "I've switched it off.";
		m_MessageStart = "";
		//m_Animation = "close";
	}
	
	int GetType()
	{
		return AT_TURN_OFF_WHILE_ON_GROUND;
	}

	string GetText()
	{
		return "switch off";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		ItemBase target_IB = (ItemBase) target;
		if ( player.IsAlive()  &&  target_IB.HasEnergyManager()  &&  target_IB.GetCompEM().CanTurnOff() )
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
		if ( target )
		{
			ItemBase target_IB = (ItemBase) target;
			target_IB.GetCompEM().SwitchOff();
		}
	}
};