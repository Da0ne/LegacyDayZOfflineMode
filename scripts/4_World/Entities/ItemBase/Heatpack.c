class EN5C_Heatpack : ItemBase
{
	void OnWorkStart()
	{
		SetTemperature2(60);
	}
	
	void OnWorkStop()
	{
		SetTemperature2(0);
	}
};
