class Slot
{
	static const int 	STATE_NOT_DIGGED 	= 0;
	static const int 	STATE_DIGGED 		= 1;
	static const int 	STATE_PLANTED 		= 2;
	static const int 	STATE_COVERED 		= 3;
	
	private int 		m_WaterQuantity;
	private int 		m_WaterUsage; 					// How much water is needed to water a plant from a bottle. Value is in mililitres
	
	float m_Fertility;
	float m_FertilizerUsage;
	float m_FertilizerQuantity;
	int m_slot_ID;
	
	string m_FertilizerType;
	string m_DiggedSlotComponent; // example: "Component02" for the 1st slot in GardenBase
	string m_PlantType;
	private ItemBase m_Seed;
	private GardenBase m_Garden;
	
	float m_HarvestingEfficiency;
	
	int m_State;
	
	private EN5C_PlantBase m_Plant;
	
	void Slot( float base_fertility )
	{
		m_Seed = NULL;
		m_Plant = NULL;
		m_WaterQuantity = 0.0;
		m_WaterUsage = 180 + Math.RandomInt(0, 20);
		Init( base_fertility );
	}

	void ~Slot()
	{
		if (m_Plant  &&  GetGame()) // GetGame() returns NULL when the game is being quit!
		{
			GetGame().ObjectDelete( GetPlant() );
		}
	}
	
	int GetSlotID()
	{
		return m_slot_ID;
	}
	
	int SetSlotID(int index)
	{
		m_slot_ID = index;
	}
	
	void SetGarden(GardenBase garden)
	{
		m_Garden = garden;
	}
	
	GardenBase GetGarden()
	{
		return m_Garden;
	}
	
	void SetSeed(ItemBase seed)
	{
		m_Seed = seed;
	}
	
	EN5C_PlantBase GetPlant()
	{
		return m_Plant;
	}
	
	void SetPlant(EN5C_PlantBase plant)
	{
		m_Plant = plant;
		
		if (plant)
		{
			plant.SetSlot(this);
		}
	}
	
	ItemBase GetSeed()
	{
		return m_Seed;
	}
	
	string GiveWater( ItemBase item, float consumed_quantity )
	{
		m_WaterQuantity += consumed_quantity;
		
		if (m_WaterQuantity > GetWaterUsage())
			m_WaterQuantity = GetWaterUsage();
		
		if (m_WaterQuantity < 0)
			m_WaterQuantity = 0;
		
		if ( NeedsWater() )
		{
			if (item)
			{
				string liquid_type = item.GetVariableString("LiquidType");
				liquid_type.ToLower();
				liquid_type.Replace( "\"", "" );

				if ( liquid_type != "water" ) 
				{
					string item_display_name = "";
					GetGame().ObjectGetDisplayName( item, item_display_name );
					m_WaterQuantity = 0;
					return "The " + item_display_name + " did not contained water.";
				}
			}
		}
		else
		{
			if (GetPlant())
			{
				GetPlant().CheckWater(item );
			}
		}
		
		GetGarden().UpdateSlotTexture( GetSlotID() );
	}
	
	bool NeedsWater()
	{
		if ( m_WaterQuantity < GetWaterUsage() )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	float GetWater()
	{
		return m_WaterQuantity;
	}
	
	float GetFertility()
	{
		return m_Fertility;
	}
	
	float GetFertilityMax()
	{
		return m_FertilizerUsage;
	}
	
	void SetFertility(float fertility)
	{
		m_Fertility = fertility;
	}
	
	float GetFertilizerQuantity()
	{
		return m_FertilizerQuantity;
	}
	
	float GetFertilizerQuantityMax()
	{
		return m_FertilizerUsage;
	}
	
	void SetFertilizerQuantity(float fertility)
	{
		m_FertilizerQuantity = fertility;
	}
	
	string GetFertilityType()
	{
		return m_FertilizerType;
	}
	
	void SetFertilityType(string type)
	{
		m_FertilizerType = type;
	}
	
	float GetWaterUsage()
	{
		return m_WaterUsage;
	}
	
	int GetState()
	{
		return m_State;
	}
	
	void SetState(int new_state)
	{
		m_State = new_state;
	}
	
	bool IsNotDigged()
	{
		if (m_State==STATE_NOT_DIGGED)
		{
			return true;
		}
		
		return false;
	}
	bool IsDigged()
	{
		if (m_State==STATE_DIGGED)
		{
			return true;
		}
		
		return false;
	}
	bool IsPlanted()
	{
		if (m_State==STATE_PLANTED)
		{
			return true;
		}
		
		return false;
	}
	bool IsCovered()
	{
		if (m_State==STATE_COVERED)
		{
			return true;
		}
		
		return false;
	}
	
	void Init( float base_fertility )
	{
		m_Fertility = base_fertility;
		m_FertilizerUsage = 0.0;
		m_FertilizerQuantity = 0.0;
		m_FertilizerType = "";
		m_HarvestingEfficiency = 1.0;
		m_DiggedSlotComponent = "";
		m_State = STATE_DIGGED;
		m_Plant = NULL;
	}

	void OnStoreLoadCustom( ParamsReadContext ctx )
	{
		autoptr Param1<float> param_float = new Param1<float>( 0.0 );
		ctx.Read( param_float );
		m_Fertility = param_float.param1;
		
		param_float.param1 = 0.0;
		ctx.Read( param_float );
		m_FertilizerUsage = param_float.param1;
		
		param_float.param1 = 0.0;
		ctx.Read( param_float );
		m_FertilizerQuantity = param_float.param1;
		
		autoptr Param1<string> param_string = new Param1<string>( "" );
		ctx.Read( param_string );
		m_FertilizerType = param_string.param1;
		
		param_float.param1 = 0.0;
		ctx.Read( param_float );
		m_HarvestingEfficiency = param_float.param1;
		
		autoptr Param1<int> param_int = new Param1<int>( STATE_NOT_DIGGED );
		ctx.Read( param_int );
		m_State = param_int.param1;
	}

	void OnStoreSaveCustom( ParamsWriteContext ctx )
	{
		autoptr Param1<float> param_float = new Param1<float>( m_Fertility );
		ctx.Write( param_float );
		
		param_float.param1 = m_FertilizerUsage;
		ctx.Write( param_float );
		
		param_float.param1 = m_FertilizerQuantity;
		ctx.Write( param_float );
		
		autoptr Param1<string> param_string = new Param1<string>( m_FertilizerType );
		ctx.Write( param_string );
		
		param_float.param1 = m_HarvestingEfficiency;
		ctx.Write( param_float );
		
		autoptr Param1<int> param_int = new Param1<int>( m_State );
		ctx.Write( param_int );
	}
}