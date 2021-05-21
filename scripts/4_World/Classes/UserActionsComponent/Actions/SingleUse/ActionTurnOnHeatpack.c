class ActionTurnOnHeatpack : ActionTurnOnWhileInHands
{
	void ActionTurnOnHeatpack()
	{
		m_MessageSuccess = "I've activated the heatpack.";
		m_MessageFail = "It's ran out.";
		//m_Animation = "break";
	}

	int GetType()
	{
	 	return AT_TURN_ON_HEATPACK;
	}

	string GetText()
	{
		return "activate";
	}
	
	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		if ( item.HasEnergyManager()  &&  item.GetCompEM().CanSwitchOn() )
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
		item.GetCompEM().SwitchOn();
	}
};