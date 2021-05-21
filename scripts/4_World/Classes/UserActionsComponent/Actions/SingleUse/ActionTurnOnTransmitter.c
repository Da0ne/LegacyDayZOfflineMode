class ActionTurnOnTransmitter: ActionTurnOnWhileInHands
{
	void ActionTurnOnTransmitter()
	{
		m_MessageSuccess = "I have turned it on.";
		m_MessageFail = "Cannot turn on the device without power source.";
		//m_Animation = "";
	}

	int GetType()
	{
		return AT_TURN_ON_TRANSMITTER;
	}

	string GetText()
	{
		return "turn on";
	}
		
	bool ActionCondition ( PlayerBase player, Object target, ItemBase item )
	{
		if ( item.HasEnergyManager()  &&  item.GetCompEM().CanSwitchOn() )
		{
			return true;
		}
		
		return false;
	}
		
	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		if ( item.GetCompEM().IsPlugged() && item.GetCompEM().GetEnergySource().GetCompEM().GetEnergy() >= item.GetCompEM().GetEnergySource().GetCompEM().GetEnergyUsage() ) //Takhle to nedelat, vyrob si action componentu a tehle check patri do jeji Execute metody. Message o selhani pak napis jako messageStartFail
		{
			item.GetCompEM().SwitchOn();
		}
		else
		{
			InformPlayers(player,target,UA_FAILED);
			//player.MessageAction( m_MessageFail );	//No power 
		}	
	}
};
