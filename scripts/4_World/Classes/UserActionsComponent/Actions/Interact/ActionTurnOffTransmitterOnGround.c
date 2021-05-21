class ActionTurnOffTransmitterOnGround: ActionInteractBase
{
	void ActionTurnOffTransmitterOnGround()
	{
		m_MessageSuccess = "I have turned it off.";
		m_MessageFail = "I have failed to turn it off.";
		//m_Animation = "";
	}

	int GetType()
	{
		return AT_TURN_OFF_TRANSMITTER_ON_GROUND;
	}

	string GetText()
	{
		return "turn off";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{	
		if ( player && target && target.IsStaticTransmitter() )
		{
			ItemBase target_entity = ( ItemBase ) target;
			string selection;
			GetGame().GetPlayerCursorObjectComponentName( player, selection );
			
			if ( target_entity.HasEnergyManager()  &&  target_entity.GetCompEM().CanTurnOff() && selection == "power_panel" )
			{
				return true;
			}
		}
	}
				
	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		ItemBase target_entity = ( ItemBase ) target;
		
		//Turn off
		target_entity.GetCompEM().SwitchOff();
	}
};