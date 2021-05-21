class CleanRags extends RecipeBase
{	
	void Init()
	{
		m_Name = "Clean Rags";
		m_IsActionType = false;//will this recipe be also a user action(only set true if you know what you are doing)
		
		//conditions
		m_MinDamageIngredient[0] = -1;//-1 = disable check
		m_MaxDamageIngredient[0] = 3;//-1 = disable check
		
		m_MinQuantityIngredient[0] = -1;//-1 = disable check
		m_MaxQuantityIngredient[0] = -1;//-1 = disable check
		
		m_MinDamageIngredient[1] = -1;//-1 = disable check
		m_MaxDamageIngredient[1] = 3;//-1 = disable check
		
		m_MinQuantityIngredient[1] = -1;//-1 = disable check
		m_MaxQuantityIngredient[1] = -1;//-1 = disable check
		//----------------------------------------------------------------------------------------------------------------------
		
		//INGREDIENTS
		//ingredient 1
		InsertIngredient(0,"EN5C_Rag");//you can insert multiple ingredients this way
		
		m_IngredientAddHealth[0] = 0;// 0 = do nothing
		m_IngredientSetHealth[0] = -1; // -1 = do nothing
		m_IngredientAddQuantity[0] = 0;// 0 = do nothing
		m_IngredientDestroy[0] = false;//true = destroy, false = do nothing
		
		//ingredient 2
		InsertIngredient(1,"EN5C_Pot");//you can insert multiple ingredients this way
		InsertIngredient(1,"CanisterGasoline");//you can insert multiple ingredients this way
		InsertIngredient(1,"EN5C_Canteen");//you can insert multiple ingredients this way
		InsertIngredient(1,"EN5C_WaterBottle");//you can insert multiple ingredients this way
		InsertIngredient(1,"EN5C_Vodka");//you can insert multiple ingredients this way
		InsertIngredient(1,"EN5C_WaterPouch_ColorBase");//you can insert multiple ingredients this way
		InsertIngredient(1,"EN5C_Barrel_ColorBase");//you can insert multiple ingredients this way
		
		m_IngredientAddHealth[1] = 0;// 0 = do nothing
		m_IngredientSetHealth[1] = -1; // -1 = do nothing
		m_IngredientAddQuantity[1] = 0;// 0 = do nothing
		m_IngredientDestroy[1] = false;// false = do nothing
		//----------------------------------------------------------------------------------------------------------------------
		
		//result1
		//AddResult("");//add results here

		m_ResultSetFullQuantity[0] = false;//true = set full quantity, false = do nothing
		m_ResultSetQuantity[0] = -1;//-1 = do nothing
		m_ResultSetHealth[0] = -1;//-1 = do nothing
		m_ResultInheritsHealth[0] = -1;// (value) == -1 means do nothing; a (value) >= 0 means this result will inherit health from ingredient number (value);(value) == -2 means this result will inherit health from all ingredients averaged(result_health = combined_health_of_ingredients / number_of_ingredients)
		m_ResultInheritsColor[0] = -1;// (value) == -1 means do nothing; a (value) >= 0 means this result classname will be a composite of the name provided in AddResult method and config value "color" of ingredient (value)
		m_ResultToInventory[0] = -2;//(value) == -2 spawn result on the ground;(value) == -1 place anywhere in the players inventory, (value) >= 0 means switch position with ingredient number(value)
	}

	bool CanDo(EntityAI ingredients[], PlayerBase player)//final check for recipe's validity
	{
		ItemBase container = ingredients[1];
		
		if ( container.GetLiquidType2() == LIQUID_WATER && container.GetQuantity2() > 0 )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void Do(EntityAI ingredients[], Man player,array<Object> results)//gets called upon recipe's completion
	{
		/*
		this function will clean the rags and consume the appropriate amount of cleaning liquid,
		if there is insufficient liquid to cover the "repair", the rags will only repair proportionally
		*/
		
		ItemBase rags = ingredients[0];
		ItemBase container = ingredients[1];
		
		float 	rags_health 	= rags.GetHealth("","");
		int 	rags_quantity 	= rags.GetQuantity2();
		
		float 	rags_combined_damage = (100 - rags_health) * rags_quantity;
		
		int 	liquid_quantity = container.GetQuantity2();
		int 	liquid_required = rags_combined_damage * 2;
		
		float 	used_ratio = 0;
		float	heal = 0;
		
		if( liquid_quantity > liquid_required )
		{
			container.AddQuantity2(-liquid_required);
			heal = 	rags_combined_damage / rags_quantity;
		}
		else
		{
			container.SetQuantity2(0);
			used_ratio = liquid_quantity / liquid_required;
			heal = 	(rags_combined_damage / rags_quantity) * used_ratio;
			
		}
		rags.AddHealth("", "",heal);
		/*
		Debug.Log("liquid_quantity: "+ToString(liquid_quantity),"recipes");
		Debug.Log("used_ratio: "+ToString(used_ratio),"recipes");
		Debug.Log("rags_combined_damage: "+ToString(rags_combined_damage),"recipes");
		Debug.Log("liquid_required: "+ToString(liquid_required),"recipes");
		Debug.Log("healing applied: "+ToString(heal),"recipes");
		*/
		
		/*
		PluginTransmissionAgents mta = GetPlugin(PluginTransmissionAgents);
		mta.RemoveAllAgents(rags);
		*/
	}
};