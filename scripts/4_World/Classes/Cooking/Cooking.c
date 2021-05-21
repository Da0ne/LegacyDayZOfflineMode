class Cooking
{
	//Cooking Methods
	static const string COOKING_METHOD_BOILING 			= "Boiling";
	static const string COOKING_METHOD_BAKING 			= "Baking";
	static const string COOKING_METHOD_DRYING 			= "Drying";
	
	//
	static const float COOKING_FOOD_TIME_INC_VALUE 		= 2;		//time increase when cooking a food
	static const float COOKING_LARD_DECREASE_COEF 		= 0.05;		//lard quantity decrease when cooking a food (Baking)
	//
	static const float TIMER_COOKING_UPDATE_INTERVAL 	= 2;
	static const float DEFAULT_COOKING_TEMPERATURE 		= 150;		//default temperature for cooking (e.g. cooking on stick)
	static const float FOOD_MAX_COOKING_TEMPERATURE		= 150;		//
	static const float PARAM_BURN_DAMAGE_COEF			= 0.05;		//value for calculating damage on items located in fireplace cargo
	//
	static const string COOKING_EQUIPMENT_POT	 		= "EN5C_Pot";
	static const string COOKING_INGREDIENT_LARD 		= "EN5C_Lard";
	//
	static const float LIQUID_BOILING_POINT 			= 100;	//boiling point for liquids
	static const float LIQUID_VAPOR_QUANTITY 			= 2;	//vapor quantity
	
	//--- SUPPORT
	protected bool CanBeCooked ( string item_type )
	{
		string config_path = "CfgVehicles" + " " + item_type + " " + "FoodStages";
		return GetGame().ConfigIsExisting ( config_path );
	}

	protected bool HasItemInCargo ( string item_type, ItemBase cooking_equipment )
	{
		Cargo cargo = cooking_equipment.GetCargo();
		
		for ( int i = 0; i < cargo.GetItemCount(); i++ )
		{
			if ( cargo.GetItem ( i ).IsKindOf ( item_type ) )
			{
				return true;
			}
		}
		
		return false;
	}

	protected ItemBase GetItemFromCargo ( string item_type, ItemBase cooking_equipment )
	{
		Cargo cargo = cooking_equipment.GetCargo();
		
		for ( int i = 0; i < cargo.GetItemCount(); i++ )
		{
			if ( cargo.GetItem ( i ).IsKindOf ( item_type ) )
			{
				return cargo.GetItem ( i );
			}
		}
		
		return NULL;
	}

	protected string GetCookingMethod( ItemBase cooking_equipment )
	{
		if ( cooking_equipment.IsKindOf ( COOKING_EQUIPMENT_POT ) )
		{
			//has water
			if ( cooking_equipment.GetQuantity2() > 0 )
			{
				return COOKING_METHOD_BOILING;
			}
			
			//has lard in cargo
			if ( HasItemInCargo ( COOKING_INGREDIENT_LARD, cooking_equipment ) )
			{
				return COOKING_METHOD_BAKING;
			}
			
			return COOKING_METHOD_DRYING;
		}
			
		return "";
	}

	//--- Cooking process
	void CookWithEquipment ( ItemBase cooking_equipment, float cooking_time_coef = 1 )
	{
		//check cooking conditions
		if ( cooking_equipment == NULL )
		{
			return;
		}
		
		//manage items in cooking equipment
		Cargo cargo = cooking_equipment.GetCargo();
		
		for ( int i = 0; i < cargo.GetItemCount(); i++ )
		{
			ItemBase item_to_cook = ( ItemBase ) cargo.GetItem ( i );
			
			if ( CanBeCooked ( item_to_cook.GetType() ) )
			{
				//update food
				string cooking_method = GetCookingMethod ( cooking_equipment );
				UpdateCookingState ( item_to_cook, cooking_method, cooking_equipment, cooking_time_coef );
			}
			else   
			{
				//damage item
				item_to_cook.AddDamage ( PARAM_BURN_DAMAGE_COEF );
			}
		}
		
		//boil liquid
		float cooking_equipment_temp = cooking_equipment.GetTemperature();
		if ( cooking_equipment_temp >= LIQUID_BOILING_POINT )
		{
			//remove agents
			PluginTransmissionAgents.RemoveAgent( cooking_equipment, AGT_CHOLERA );
			
			//vaporize liquid
			if ( cooking_equipment.GetQuantity2() > 0 )
			{
				//LIQUID_VAPOR_QUANTITY	
				cooking_equipment.AddQuantity2( -LIQUID_VAPOR_QUANTITY );
			}
		}
	}

	void CookOnStick ( array<ItemBase> items_on_stick, float cooking_time_coef = 1 )
	{
		//manage items on stick
		for ( int i = 0; i < items_on_stick.Count(); i++ )
		{
			ItemBase item_to_cook = items_on_stick.Get ( i );
			
			if ( CanBeCooked ( item_to_cook.GetType() ) )
			{
				//update food
				UpdateCookingState ( item_to_cook, COOKING_METHOD_BAKING, NULL, cooking_time_coef );
			}
		}
	}

	protected void UpdateCookingState ( ItemBase item_to_cook, string cooking_method, ItemBase cooking_equipment, float cooking_time_coef )
	{
		//food properties
		float food_temperature = item_to_cook.GetTemperature();
		
		EN5C_Edible_Base food = ( EN5C_Edible_Base ) item_to_cook;
		FoodStage food_stage = food.GetFoodStage();
		
		//{min_temperature, time_to_cook, max_temperature (optional)}
		//get next stage name - if next stage is not defined, next stage name will be empty "" and no cooking properties (food_min_temp, food_time_to_cook, food_max_temp) will be set
		string new_stage_name = FoodManager.GetNextFoodStageName ( item_to_cook, cooking_method );
		float food_min_temp = 0;
		float food_time_to_cook = 0;
		float food_max_temp = -1;
		
		//Set next stage cooking properties if next stage possible
		if ( FoodManager.CanChangeToNewStage ( item_to_cook, cooking_method ) )
		{
			autoptr array<float> next_stage_cooking_properties = new array<float>;
			
			string config_path = "CfgVehicles" + " " + food.GetType() + " " + "FoodStages";
			GetGame().ConfigGetFloatArray ( config_path + " " + new_stage_name + " " + "cooking_properties", next_stage_cooking_properties );
			
			food_min_temp = next_stage_cooking_properties.Get ( 0 );
			food_time_to_cook = next_stage_cooking_properties.Get ( 1 );
			
			if ( next_stage_cooking_properties.Count() > 2)
			{
				food_max_temp = next_stage_cooking_properties.Get ( 2 );
			}	
		}
		
		//set actual cooking temperature
		float actual_cooking_temp = DEFAULT_COOKING_TEMPERATURE;	//default
		if ( cooking_equipment )
		{
			actual_cooking_temp = cooking_equipment.GetTemperature();
		}
		
		//add temperature
		if ( actual_cooking_temp > food_temperature )
		{
			
			food_temperature = actual_cooking_temp * 0.5;
			food_temperature = Math.Clamp ( food_temperature, food_min_temp, FOOD_MAX_COOKING_TEMPERATURE );
			
			//set new temperature
			item_to_cook.SetTemperature ( food_temperature );
			
			//DEBUG
			/*
			PlayerBase player2 = ( PlayerBase ) GetGame().GetPlayer();
			string s_message2 = "food_temperature = " + ftoa(food_temperature) + " | food type = " + item_to_cook.GetType() + " food_min_temp = " + ftoa(food_min_temp);
			if (player2 != NULL) {player2.MessageAction (s_message2);}
			Print ( s_message2 );
			string s_message3 = "food_max_temp = " + ftoa(food_max_temp) + " | food_time_to_cook = " + ftoa(food_time_to_cook) + " food stage name = " + food_stage.m_FoodStageName;
			if (player2 != NULL) {player2.MessageAction (s_message3);}
			Print ( s_message3 );
			*/
		}
		
		//add cooking time if the food can be cooked by this method
		if ( food_min_temp > 0 && food_temperature >= food_min_temp )
		{
			//DEBUG
			/*
			autoptr PlayerBase player1 = ( PlayerBase ) GetGame().GetPlayer();
			string ss_message = "Cooking time = " + ftoa(food_stage.GetCookingTime()) + " COOKING_FOOD_TIME_INC_VALUE*cooking_time_coef = " + ftoa(COOKING_FOOD_TIME_INC_VALUE*cooking_time_coef) + " cooking_method = " + cooking_method;
			if (player1 != NULL) {player1.MessageAction (ss_message);}
			Print(ss_message);
			*/
			float new_cooking_time = food_stage.GetCookingTime() + COOKING_FOOD_TIME_INC_VALUE * cooking_time_coef;
			food_stage.SetCookingTime( new_cooking_time );
			
			//progress to next stage
			if ( food_stage.GetCookingTime() >= food_time_to_cook )
			{
				//if max temp is defined check next food stage
				if ( food_max_temp >= 0 )
				{
					if ( food_temperature > food_max_temp && food_stage.GetName() != FoodManager.FOOD_STAGE_BURNED )
					{
						new_stage_name = FoodManager.FOOD_STAGE_BURNED;
					}
				}
				
				//DEBUG
				/*
				autoptr PlayerBase player = ( PlayerBase ) GetGame().GetPlayer();
				string s_message = "Progress to next stage = " + new_stage_name;
				if (player1 != NULL) {player.MessageAction (s_message);}
				Print(s_message);
				*/
				
				//change food stage
				FoodManager.ChangeFoodStage ( item_to_cook, new_stage_name );
				
				//remove lard when baking with cooking equipment
				if ( cooking_equipment && cooking_method == COOKING_METHOD_BAKING )
				{
					if ( !item_to_cook.IsKindOf ( COOKING_INGREDIENT_LARD ) )
					{
						//get lard item
						ItemBase item_lard = GetItemFromCargo ( COOKING_INGREDIENT_LARD, cooking_equipment );
						
						//decrease lard quantity
						float lard_quantity = item_lard.GetQuantity2() - COOKING_LARD_DECREASE_COEF;
						lard_quantity = Math.Clamp ( lard_quantity, 0, item_lard.GetQuantityMax2() );
						item_lard.SetQuantity2( lard_quantity );
					}
				}
				
				//reset cooking time
				food_stage.SetCookingTime( 0 );
			}
		}	
	}
}
