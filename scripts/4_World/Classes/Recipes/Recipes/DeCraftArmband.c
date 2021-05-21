class DeCraftArmband extends RecipeBase
{	
	void Init()
	{
		m_Name = "Create Rag from Armband";
		m_IsActionType = false;
		//conditions
		m_MinDamageIngredient[0] = -1;
		m_MaxDamageIngredient[0] = 3;
		m_MinQuantityIngredient[0] = -1;
		m_MaxQuantityIngredient[0] = -1;
		
		m_MinDamageIngredient[1] = -1;
		m_MaxDamageIngredient[1] = 3;
		m_MinQuantityIngredient[1] = -1;
		m_MaxQuantityIngredient[1] = -1;
		
		
		//ingredient 1
		InsertIngredient(0,"EN5C_Armband_ColorBase");//you can insert multiple ingredients this way

		m_IngredientAddHealth[0] = 0;
		m_IngredientSetHealth[0] = -1; // -1 = do nothing
		m_IngredientAddQuantity[0] = -1;
		m_IngredientDestroy[0] = 0;
		
		//ingredient 2
		InsertIngredient(1,"EN5C_Sickle");//you can insert multiple ingredients this way
		InsertIngredient(1,"EN5C_Hacksaw");
		InsertIngredient(1,"EN5C_KitchenKnife");
		InsertIngredient(1,"EN5C_SteakKnife");
		InsertIngredient(1,"EN5C_HayHook");
		InsertIngredient(1,"EN5C_StoneKnife");
		InsertIngredient(1,"EN5C_Cleaver");
		InsertIngredient(1,"EN5C_CombatKnife");
		InsertIngredient(1,"EN5C_HuntingKnife");
		InsertIngredient(1,"EN5C_Machete");
		InsertIngredient(1,"EN5C_Screwdriver");
		InsertIngredient(1,"EN5C_Crowbar");
		InsertIngredient(1,"EN5C_Pickaxe");
		InsertIngredient(1,"EN5C_WoodAxe");
		InsertIngredient(1,"EN5C_Hatchet");
		InsertIngredient(1,"EN5C_FirefighterAxe");
		InsertIngredient(1,"EN5C_Sword");
		InsertIngredient(1,"EN5C_AK_Bayonet");
		InsertIngredient(1,"EN5C_M9A1_Bayonet");
		InsertIngredient(1,"EN5C_Mosin_Bayonet");
		InsertIngredient(1,"EN5C_SKS_Bayonet");
		
		m_IngredientAddHealth[1] = -10;
		m_IngredientSetHealth[1] = -1; // -1 = do nothing
		m_IngredientAddQuantity[1] = 0;
		m_IngredientDestroy[1] = 0;
		
		// crafting multiple results
		
		
		AddResult("EN5C_Rag");//single result
		
		m_ResultSetFullQuantity[0] = 0;
		m_ResultSetQuantity[0] = 1;
		m_ResultSetHealth[0] = -1;
		m_ResultInheritsHealth[0] = 0;// (value) == -1 means do nothing; a (value) >= 0 means this result will inherit health from ingredient number (value);(value) == -2 means this result will inherit health from all ingredients averaged(result_health = combined_health_of_ingredients / number_of_ingredients)
		m_ResultInheritsColor[0] = -1;// (value) == -1 means do nothing; a (value) >= 0 means this result classname will be a composite of the name provided in AddResult method and config value "color" of ingredient (value)
		m_ResultToInventory[0] = -2;
	}

	bool CanDo(EntityAI ingredients[], PlayerBase player)
	{
		return true;
	}

	void Do(EntityAI ingredients[], Man player,array<Object> results)//gets called upon recipe's completion
	{
		Debug.Log("Recipe Do method called","recipes");
	}
};