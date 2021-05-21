class CraftBaseBallBatNailed extends RecipeBase
{
	void Init()
	{
		m_Name = "Craft Baseball Bat With Nails";
		m_IsActionType = false;
		//conditions
		m_MinDamageIngredient[0] = -1;
		m_MaxDamageIngredient[0] = 3;
		m_MinQuantityIngredient[0] = 13;
		m_MaxQuantityIngredient[0] = -1;
		
		m_MinDamageIngredient[1] = -1;
		m_MaxDamageIngredient[1] = 3;
		m_MinQuantityIngredient[1] = -1;
		m_MaxQuantityIngredient[1] = -1;
		
		
		//ingredient 1
		InsertIngredient(0,"EN5C_Nail");//you can insert multiple ingredients this way

		m_IngredientAddHealth[0] = 0;
		m_IngredientSetHealth[0] = -1; // -1 = do nothing
		m_IngredientAddQuantity[0] = -13;
		m_IngredientDestroy[0] = 0;
		
		//ingredient 2
		InsertIngredient(1,"EN5C_BaseballBat");//you can insert multiple ingredients this way

		m_IngredientAddHealth[1] = 0;
		m_IngredientSetHealth[1] = -1; // -1 = do nothing
		m_IngredientAddQuantity[1] = 0;
		m_IngredientDestroy[1] = 1;
		
		// crafting multiple results
		//AddResult("CombatKnife");//additional results, must be processed individually in Do method (as "results" parameter)
		
		
		AddResult("EN5C_NailedBaseballBat");//single result
		m_ResultSetFullQuantity[0] = 0;
		m_ResultSetQuantity[0] = -1;
		m_ResultSetHealth[0] = -1;
		m_ResultInheritsHealth[0] = 1;// (value) == -1 means do nothing; a (value) >= 0 means this result will inherit health from ingredient number (value);(value) == -2 means this result will inherit health from all ingredients averaged(result_health = combined_health_of_ingredients / number_of_ingredients)
		m_ResultInheritsColor[0] = -1;// (value) == -1 means do nothing; a (value) >= 0 means this result classname will be a composite of the name provided in AddResult method and config value "color" of ingredient (value)
		m_ResultToInventory[0] = -2;
	}

	bool CanDo(EntityAI ingredients[], PlayerBase player)
	{
		return true;
	}

	void Do(EntityAI ingredients[], Man player,array<Object> results)
	{
		EntityAI ingredient = ingredients[1];
		float batHealth = ingredient.GetHealth("","");
		batHealth = batHealth * 0.9;//deduct 10%
		results.Get(0).SetHealth( "","",batHealth );
	}
};