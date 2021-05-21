class EN5C_PortableGasLamp extends ItemBase
{
	private const string GAS_LIGHT_MATERIAL_ON 		= "dz\\gear\\cooking\\data\\GasLightOn.rvmat";
	private const string GAS_LIGHT_MATERIAL_OFF 	= "dz\\data\\data\\default.rvmat";

	//--- POWER EVENTS
	void OnWorkStart()
	{
		SetPilotLight( true );
		
		//refresh visual
		SetObjectMaterial( 0, GAS_LIGHT_MATERIAL_ON );
	}

	void OnWorkStop()
	{
		SetPilotLight( false );
		
		//refresh visual
		SetObjectMaterial( 0, GAS_LIGHT_MATERIAL_OFF );
	}
	
	//--- INVENTORY CONDITIONS
	bool ConditionIntoInventory ( EntityAI  player ) 
	{
		return true;
	}

	bool ConditionIntoHands ( EntityAI player ) 
	{
		return true;
	}
}