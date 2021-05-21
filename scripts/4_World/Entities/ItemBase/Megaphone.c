class EN5C_Megaphone extends ItemMegaphone
{
	//const
	const string MESSAGE_NO_POWER = "Cannot turn on the device without power source.";
	
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
}
