class CraftFireplace extends RecipeBase
{
	void Init()
	{
		m_Name = "Craft Fireplace";
		m_IsActionType = false;//will this recipe be also a user action(only set true if you know what you are doing)
		
		//conditions
		m_MinDamageIngredient[0] = -1;//-1 = disable check
		m_MaxDamageIngredient[0] = 3;//-1 = disable check
		
		m_MinQuantityIngredient[0] = 1;//-1 = disable check
		m_MaxQuantityIngredient[0] = -1;//-1 = disable check
		
		m_MinDamageIngredient[1] = -1;//-1 = disable check
		m_MaxDamageIngredient[1] = 3;//-1 = disable check
		
		m_MinQuantityIngredient[1] = 1;//-1 = disable check
		m_MaxQuantityIngredient[1] = -1;//-1 = disable check
		//----------------------------------------------------------------------------------------------------------------------
		
		//INGREDIENTS
		//ingredient 1
		InsertIngredient(0,"EN5C_WoodenStick");//you can insert multiple ingredients this way
		InsertIngredient(0,"EN5C_Firewood");//you can insert multiple ingredients this way
		
		m_IngredientAddHealth[0] = 0;// 0 = do nothing
		m_IngredientSetHealth[0] = -1; // -1 = do nothing
		m_IngredientAddQuantity[0] = -1;// 0 = do nothing
		m_IngredientDestroy[0] = false;//true = destroy, false = do nothing
		
		//ingredient 2
		InsertIngredient(1,"EN5C_Rag");//you can insert multiple ingredients this way
		InsertIngredient(1,"EN5C_BandageDressing");//you can insert multiple ingredients this way
		InsertIngredient(1,"EN5C_Paper");//you can insert multiple ingredients this way
		InsertIngredient(1,"EN5C_Book_Base");//you can insert multiple ingredients this way
		InsertIngredient(1,"EN5C_Bark_Birch");//you can insert multiple ingredients this way
		InsertIngredient(1,"EN5C_Bark_Oak");//you can insert multiple ingredients this way
		
		m_IngredientAddHealth[1] = 0;// 0 = do nothing
		m_IngredientSetHealth[1] = -1; // -1 = do nothing
		m_IngredientAddQuantity[1] = -1;// 0 = do nothing
		m_IngredientDestroy[1] = false;// false = do nothing
		//----------------------------------------------------------------------------------------------------------------------
		
		//result1
		AddResult("EN5C_Fireplace");//add results here

		m_ResultSetFullQuantity[0] = false;//true = set full quantity, false = do nothing
		m_ResultSetQuantity[0] = -1;//-1 = do nothing
		m_ResultSetHealth[0] = -1;//-1 = do nothing
		m_ResultInheritsHealth[0] = -2;// (value) == -1 means do nothing; a (value) >= 0 means this result will inherit health from ingredient number (value);(value) == -2 means this result will inherit health from all ingredients averaged(result_health = combined_health_of_ingredients / number_of_ingredients)
		m_ResultInheritsColor[0] = -1;// (value) == -1 means do nothing; a (value) >= 0 means this result classname will be a composite of the name provided in AddResult method and config value "color" of ingredient (value)
		m_ResultToInventory[0] = -2;//(value) == -2 spawn result on the ground;(value) == -1 place anywhere in the players inventory, (value) >= 0 means switch position with ingredient number(value)
	}

	bool CanDo(EntityAI ingredients[], PlayerBase player)//final check for recipe's validity
	{
		return true;
	}

	void Do(EntityAI ingredients[], Man player,array<Object> results)//gets called upon recipe's completion
	{
		EntityAI result = results.Get(0);
		EntityAI ingredient1 = ingredients[0];
		string ingredient1_classname = ingredient1.GetType();
		ItemBase attachment1 = result.CreateAttachment(ingredient1_classname);
		attachment1.SetQuantity2(1);
	}
};