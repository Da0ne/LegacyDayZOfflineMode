class EN5C_Chemlight_ColorBase : ItemBase
{
	void OnWorkStart()
	{
		SetPilotLight(true);
	}
	
	void OnWorkStop()
	{
		SetPilotLight(false);
		SetObjectTexture(0, "");
	}
};
