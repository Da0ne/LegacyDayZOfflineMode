class EN5C_Flashlight extends ItemBase
{
	//--- POWER EVENTS
	void OnWorkStart()
	{
		SetPilotLight(true);
	}

	void OnWorkStop()
	{
		SetPilotLight(false);
	}
}