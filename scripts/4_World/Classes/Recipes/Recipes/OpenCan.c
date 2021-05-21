class OpenCan extends RecipeBase
{	
	void Init()
	{
		m_Name = "Open can";
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
		InsertIngredient(0,"EN5C_BakedBeansCan");//you can insert multiple ingredients this way
		InsertIngredient(0,"EN5C_PeachesCan");//you can insert multiple ingredients this way
		InsertIngredient(0,"EN5C_TacticalBaconCan");//you can insert multiple ingredients this way
		InsertIngredient(0,"EN5C_SpaghettiCan");//you can insert multiple ingredients this way
		InsertIngredient(0,"EN5C_SardinesCan");//you can insert multiple ingredients this way
		
		m_IngredientAddHealth[0] = 0;// 0 = do nothing
		m_IngredientSetHealth[0] = -1; // -1 = do nothing
		m_IngredientAddQuantity[0] = 0;// 0 = do nothing
		m_IngredientDestroy[0] = false;//true = destroy, false = do nothing
		
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
		InsertIngredient(1,"EN5C_AK_Bayonet");
		InsertIngredient(1,"EN5C_M9A1_Bayonet");
		InsertIngredient(1,"EN5C_Mosin_Bayonet");
		InsertIngredient(1,"EN5C_SKS_Bayonet");
		InsertIngredient(1,"EN5C_Spear");
		InsertIngredient(1,"EN5C_CanOpener");
		InsertIngredient(1,"EN5C_Shovel");
		InsertIngredient(1,"EN5C_FieldShovel");
		InsertIngredient(1,"EN5C_Crowbar");
		InsertIngredient(1,"EN5C_Hammer");
		InsertIngredient(1,"EN5C_MeatTenderizer");
		InsertIngredient(1,"EN5C_Wrench");
		InsertIngredient(1,"EN5C_LugWrench");
		InsertIngredient(1,"EN5C_Screwdriver");
		InsertIngredient(1,"EN5C_FarmingHoe");
		InsertIngredient(1,"EN5C_SledgeHammer");
		InsertIngredient(1,"EN5C_Broom");
		InsertIngredient(1,"EN5C_Chainsaw");
		InsertIngredient(1,"EN5C_BaseballBat");
		InsertIngredient(1,"EN5C_NailedBaseballBat");
		InsertIngredient(1,"EN5C_PipeWrench");
		InsertIngredient(1,"EN5C_TelescopicBaton");
		InsertIngredient(1,"EN5C_NewHockeyStick");
		InsertIngredient(1,"EN5C_OldHockeyStick");
		InsertIngredient(1,"EN5C_Pitchfork");
		InsertIngredient(1,"EN5C_Pickaxe");
		InsertIngredient(1,"EN5C_Sword");
		InsertIngredient(1,"EN5C_WoodAxe");
		InsertIngredient(1,"EN5C_FirefighterAxe");
		InsertIngredient(1,"EN5C_Hatchet");
		
		m_IngredientAddHealth[1] = -10;// 0 = do nothing
		m_IngredientSetHealth[1] = -1; // -1 = do nothing
		m_IngredientAddQuantity[1] = 0;// 0 = do nothing
		m_IngredientDestroy[1] = false;// false = do nothing
		//----------------------------------------------------------------------------------------------------------------------
		
		//result1
		//AddResult("");//add results here

		m_ResultSetFullQuantity[0] = false;//true = set full quantity, false = do nothing
		m_ResultSetQuantity[0] = 0;//-1 = do nothing
		m_ResultSetHealth[0] = -1;//-1 = do nothing
		m_ResultInheritsHealth[0] = -1;// (value) == -1 means do nothing; a (value) >= 0 means this result will inherit health from ingredient number (value);(value) == -2 means this result will inherit health from all ingredients averaged(result_health = combined_health_of_ingredients / number_of_ingredients)
		m_ResultInheritsColor[0] = -1;// (value) == -1 means do nothing; a (value) >= 0 means this result classname will be a composite of the name provided in AddResult method and config value "color" of ingredient (value)
		m_ResultToInventory[0] = -2;//(value) == -2 spawn result on the ground;(value) == -1 place anywhere in the players inventory, (value) >= 0 means switch position with ingredient number(value)
		
		//----------------------------------------------------------------------------------------------------------------------
	}

	bool CanDo(EntityAI ingredients[], PlayerBase player)//final check for recipe's validity
	{
		return true;
	}

	void Do(EntityAI ingredients[], Man player,array<Object> results)//gets called upon recipe's completion
	{
		ItemBase can = ingredients[0];
		ItemBase tool = ingredients[1];
		OpenItem.OpenAndSwitch(tool, can, player);
	}
};