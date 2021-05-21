//TRANSMITTER BASE
class EN5C_BaseRadio extends EN5C_Transmitter_Base
{
	//--- ACTIONS
	bool CanOperate()
	{
		return this.GetCompEM().IsSwitchedOn();
	}
	
	void SetNextFrequency ( PlayerBase player )
	{
		this.SetNextChannel();
		
		float tuned_freq = this.GetTunedFrequency();
		string message = "I've set radio frequency to " + tuned_freq.ToString() + ".";
		player.MessageAction( message );	
	}

	void SetPreviousFrequency ( PlayerBase player )
	{
		this.SetPrevChannel();
		
		float tuned_freq = this.GetTunedFrequency();
		string message = "I've set radio frequency to " + tuned_freq.ToString() + ".";
		player.MessageAction( message );	
	}

	void DisplayTunedFrequency ( PlayerBase player )
	{
		float tuned_freq = this.GetTunedFrequency();

		string message = "The radio frequency is set to " + tuned_freq.ToString() + ".";
		player.MessageAction( message );
	}

	//--- POWER EVENTS
	void OnSwitchOn()
	{
		//turn on broadcasting/receiving
		this.EnableBroadcast ( true );
		this.EnableReceive ( true );
	}

	void OnSwitchOff()
	{
		//turn off broadcasting/receiving
		this.EnableBroadcast ( false );
		this.EnableReceive ( false );	
	}

	
	//--- DEBUG
	void CheckStatus ( PlayerBase player )
	{
		string message = "Status: Is broadcasting? [" + this.IsBroadcasting().ToString() + "] --- Is receiving? [" + this.IsReceiving().ToString() + "]";
		player.MessageAction( message );	
	}
}
