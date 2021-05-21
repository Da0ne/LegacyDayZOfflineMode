class EN5C_PortableGasStove extends ItemBase
{
	private const string FLAME_BUTANE_TEXTURE 		= "dz\\gear\\cooking\\data\\flame_butane_ca.paa";
	private const string ATTACHMENT_COOKING_POT 	= "EN5C_Pot";
	
	//cooking
	private const float PARAM_COOKING_TEMP_THRESHOLD		= 100;		//temperature threshold for starting coooking process (degree Celsius)
	private const float PARAM_COOKING_EQUIP_TEMP_INCREASE	= 10;		//how much will temperature increase when attached on burning fireplace (degree Celsius)
	private const float PARAM_COOKING_EQUIP_MAX_TEMP		= 250;		//maximum temperature of attached cooking equipment (degree Celsius)
	private const float PARAM_COOKING_TIME_INC_COEF			= 0.5;		//cooking time increase coeficient, can be used when balancing how fast a food can be cooked
	
	//
	Cooking m_CookingProcess;
	ItemBase m_CookingEquipment;
	
	//--- ATTACHMENTS
	void EEItemAttached ( EntityAI item, string slot_name ) 
	{
		super.EEItemAttached( item, slot_name );
		
		//cookware
		if ( item.IsKindOf ( ATTACHMENT_COOKING_POT ) )
		{
			m_CookingEquipment = item;
		}
	}
	
	void EEItemDetached ( EntityAI item, string slot_name ) 
	{
		super.EEItemDetached( item, slot_name );
		
		//cookware
		if ( item.IsKindOf ( ATTACHMENT_COOKING_POT ) )
		{
			m_CookingEquipment = NULL;
		}	
	}
	
	//--- POWER EVENTS
	void OnWorkStart()
	{
		//refresh visual
		SetObjectTexture( 0, FLAME_BUTANE_TEXTURE );
	}

	void OnWorkStop()
	{
		//refresh visual
		SetObjectTexture( 0, "" );
	}
	
	//on update 
	void OnWork( float consumed_energy )
	{
		//manage cooking equipment
		if ( m_CookingEquipment )
		{
			float cook_equip_temp = m_CookingEquipment.GetTemperature();
			
			if ( cook_equip_temp >= PARAM_COOKING_TEMP_THRESHOLD )
			{
				//start cooking
				CookWithEquipment();
			}
			
			//set temperature to cooking equipment
			cook_equip_temp = cook_equip_temp + PARAM_COOKING_EQUIP_TEMP_INCREASE;
			cook_equip_temp = Math.Clamp ( cook_equip_temp, 0, PARAM_COOKING_EQUIP_MAX_TEMP );
			m_CookingEquipment.SetTemperature ( cook_equip_temp );
		}
	}
	
	void CookWithEquipment()
	{
		if ( m_CookingProcess == NULL )
		{
			m_CookingProcess = new Cooking;
		}
		
		m_CookingProcess.CookWithEquipment ( m_CookingEquipment, PARAM_COOKING_TIME_INC_COEF );
	}
}