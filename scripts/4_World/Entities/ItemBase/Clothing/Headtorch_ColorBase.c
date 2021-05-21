class EN5C_Headtorch_ColorBase extends Clothing
{
	void EN5C_Headtorch_ColorBase()
	{
		
	}

	void OnWorkStart()
	{
		SetPilotLight(true);
	}

	void OnWorkStop()
	{
		SetPilotLight(false);
	}
}
