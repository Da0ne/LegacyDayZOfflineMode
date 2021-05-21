class EN5C_PAS_Broadcaster extends PASBroadcaster
{
	//--- ACTION CONDITION
	bool CanOperate()
	{
		return this.GetCompEM().IsSwitchedOn();
	}
	
	//--- POWER EVENTS
	void OnWorkStop()
	{
		//turn off device
		SwitchOn ( false );
	}
	
	//--- ACTION EVENTS
	void OnSwitchOn()
	{
		//turn device on
		SwitchOn ( true );
	}
	
	void OnSwitchOff()
	{
		//turn device off
		this.GetCompEM().SwitchOff();
	}
	
	//--- IDENTIFICATION
	bool IsStaticTransmitter()
	{
		return true;
	}
	
}
