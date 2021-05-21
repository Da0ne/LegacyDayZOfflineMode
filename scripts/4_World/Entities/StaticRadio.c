//TRANSMITTER BASE
class EN5C_StaticRadio extends StaticTransmitter
{
	//--- ACTIONS
	//--- Frequency actions: Set frequency channels, show tuned frequency
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

	bool IsStaticTransmitter()
	{
		return true;
	}
	
	//--- POWER EVENTS
	void OnWorkStop()
	{
		//turn off device
		this.GetCompEM().SwitchOff();
	}

	void OnSwitchOn()
	{
		//turn device on
		this.SwitchOn ( true );
		
		//turn on broadcasting/receiving
		this.EnableBroadcast ( true );
	}

	void OnSwitchOff()
	{
		//turn device off
		this.SwitchOn ( false );
		
		//turn off broadcasting/receiving
		this.EnableBroadcast ( false );
	}
	
	//--- DEBUG
	void CheckStatus ( PlayerBase player )
	{
		string message = "Status: Is broadcasting? [" + this.IsBroadcasting().ToString() + "]";
		player.MessageAction( message );	
	}
}
