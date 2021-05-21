class EN5C_Defibrillator extends ItemBase
{
	void OnWorkStop()
	{
		//turn off device
		this.GetCompEM().SwitchOff();
	}
	
	//--- ACTION EVENTS
	void OnSwitchOn()
	{
		SetPilotLight(true);
	}

	void OnSwitchOff()
	{
		SetPilotLight(false);
	}
}