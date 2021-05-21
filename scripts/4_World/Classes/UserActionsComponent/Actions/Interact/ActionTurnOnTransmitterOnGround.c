class ActionTurnOnTransmitterOnGround: ActionInteractBase
{
	void ActionTurnOnTransmitterOnGround()
	{
		m_MessageSuccess = "I have turned it on.";
		m_MessageFail = "Cannot turn on the device without power source.";
		//m_Animation = "";
	}

	int GetType()
	{
		return AT_TURN_ON_TRANSMITTER_ON_GROUND;
	}

	string GetText()
	{
		return "turn on";
	}
		
	bool ActionCondition ( PlayerBase player, Object target, ItemBase item )
	{
		if ( player && target && target.IsStaticTransmitter() )
		{
			//player.MessageAction( "ActionTurnOnTransmitterOnGround:ActionCondition > IsStaticTransmitter" );
			EntityAI target_entity = ( EntityAI ) target;
			string selection;
			GetGame().GetPlayerCursorObjectComponentName( player, selection );
			if ( target_entity.HasEnergyManager()  &&  target_entity.GetCompEM().CanSwitchOn() && selection == "power_panel" )
			{
				return true;
			}
		}
		return false;
	}
		
	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		EntityAI target_entity = ( EntityAI ) target;
		
		if ( target_entity.GetCompEM().IsPlugged() && target_entity.GetCompEM().CanWork() )
		{
			target_entity.GetCompEM().SwitchOn();
			
			InformPlayers(player,target,UA_FINISHED);	//Success
		}
		else
		{
			
			InformPlayers(player,target,UA_FAILED);//No power
		}	
	}
};