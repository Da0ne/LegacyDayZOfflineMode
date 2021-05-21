//TRANSMITTER BASE
class EN5C_Transmitter_Base extends ItemTransmitter
{
	//--- COMMON
	bool CanOperate()
	{
		return this.GetCompEM().IsSwitchedOn();	
	}

	void SetNextFrequency ( PlayerBase player )
	{	
	}

	void SetPreviousFrequency ( PlayerBase player )
	{	
	}
	
	void DisplayTunedFrequency ( PlayerBase player )
	{	
	}
	//debug
	void CheckStatus ( PlayerBase player )
	{	
	}
		
	//--- POWER EVENTS
	void OnWorkStop()
	{
		//turn off device
		this.GetCompEM().SwitchOff();
	}
}
