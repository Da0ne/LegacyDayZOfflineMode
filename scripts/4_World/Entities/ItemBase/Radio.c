class EN5C_Radio extends ItemRadio
{
	//const
	const string MESSAGE_RADIO_STATION_CHANGED = "I've changed the radio station.";
	
	//--- POWER EVENTS
	void OnWorkStop()
	{
		//turn off device
		this.GetCompEM().SwitchOff();
	}
	
	//--- ACTION EVENTS
	void OnSwitchOn()
	{
		//turn device on
		this.SwitchOn ( true );
	}

	void OnSwitchOff()
	{
		//turn device off
		this.SwitchOn ( false );
	}
	
	//--- RADIO ACTIONS
	void TuneNextStation ( PlayerBase player )
	{
		//tune next station
		this.TuneNext();
		
		//message player
		player.MessageAction ( MESSAGE_RADIO_STATION_CHANGED );
	}

	void TunePreviousStation ( PlayerBase player )
	{
		//tune previous station
		this.TunePrev();
		
		//message player
		player.MessageAction ( MESSAGE_RADIO_STATION_CHANGED );
	}
}
