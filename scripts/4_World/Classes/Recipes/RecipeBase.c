const int MAX_NUMBER_OF_INGREDIENTS = 2;
const int MAXIMUM_RESULTS = 10;

class RecipeBase
{
	string m_ItemsToCreate[MAXIMUM_RESULTS];
	array<string> m_Ingredients[MAX_NUMBER_OF_INGREDIENTS];
	
	EntityAI m_Items[MAX_NUMBER_OF_INGREDIENTS];
	
	EntityAI m_IngredientsSorted[MAX_NUMBER_OF_INGREDIENTS]; //if the recipe is valid, this array will contain all ingredients sorted against the recipe ingredients
	
	//autoptr array<string> m_ItemsToCreate;
	
	string m_Result;
	//autoptr array<EntityAI> ingredientsLinks;
	string m_Name;
	
	int	m_ID;
	int m_NumberOfResults;
	bool m_IsActionType;
	array<string> ptr;

	float m_MinQuantityIngredient[MAX_NUMBER_OF_INGREDIENTS];
	float m_MaxQuantityIngredient[MAX_NUMBER_OF_INGREDIENTS];
	float m_MinDamageIngredient[MAX_NUMBER_OF_INGREDIENTS];
	float m_MaxDamageIngredient[MAX_NUMBER_OF_INGREDIENTS];

	float m_IngredientAddHealth[MAX_NUMBER_OF_INGREDIENTS];
	float m_IngredientAddQuantity[MAX_NUMBER_OF_INGREDIENTS];
	float m_IngredientSetHealth[MAX_NUMBER_OF_INGREDIENTS];
	bool m_IngredientDestroy[MAX_NUMBER_OF_INGREDIENTS];
	
	bool m_ResultSetFullQuantity[MAXIMUM_RESULTS];
	float m_ResultSetQuantity[MAXIMUM_RESULTS];
	float m_ResultSetHealth[MAXIMUM_RESULTS];
	int m_ResultToInventory[MAXIMUM_RESULTS];
	int m_ResultInheritsHealth[MAXIMUM_RESULTS];
	int m_ResultInheritsColor[MAXIMUM_RESULTS];
	


	void RecipeBase()
	{

		for(int i = 0; i < MAX_NUMBER_OF_INGREDIENTS; i++)
		{
			m_Ingredients[i] = new array<string>;
			m_IngredientsSorted[i] = NULL;
		}

		m_NumberOfResults = 0;
		m_IsActionType = false;
		
		m_Name = "RecipeBase default name";
		Init();
	}

	void ~RecipeBase()
	{
		for(int i = 0; i < MAX_NUMBER_OF_INGREDIENTS; i++)
		{
			delete m_Ingredients[i];
		}
	}

	void Init();
	
	bool CheckIngredientMatch(EntityAI item1, EntityAI item2)
	{
		if( item1 == NULL || item2 == NULL ) return false;
		
		m_Items[0] = item1;
		m_Items[1] = item2;
		
		bool found = false;
		for(int i = 0; i < MAX_NUMBER_OF_INGREDIENTS; i++)//all ingredients
		{
			found = false;
			array<string> tempArray = m_Ingredients[i];
			for(int x = 0; x < tempArray.Count(); x++)//particular ingredient array
			{
				for(int z = 0; z < MAX_NUMBER_OF_INGREDIENTS; z++)
				{
					if( m_Items[z] != NULL )
					{
						EntityAI item = m_Items[z];
						if( tempArray.Get(x) == item.GetType() || GetGame().IsKindOf(item.GetType(),tempArray.Get(x)))
						{
							found = true;//we found a match
							//m_IngredientsSorted.Insert(item);
							m_IngredientsSorted[i] = item;
							m_Items[z] = NULL;
						}
					}
					if(found) break;//we found a match, no need to check the remaining ingredients
				}
				if(found) break;//we found a match, no need to check this m_Ingredient array
			}
			if(!found) return false;// no match within an m_Ingredient array, no reason to continue the search, recipe is invalid
		}
		if(found)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void InsertIngredient(int index, string ingredient)
	{
		array<string> ptr = m_Ingredients[index];
		ptr.Insert(ingredient);
	}


	void AddResult(string item)
	{
		m_ItemsToCreate[m_NumberOfResults] = item;
		m_NumberOfResults++;
	}
	/*
	void AddResultColored(string item)
	{
		class_name = 
		color = 
		m_ItemsToCreate[m_NumberOfResults] = item;
		m_NumberOfResults++;
	}
	*/
	string GetName()
	{
		return m_Name;
	}

	bool IsAction()
	{
		return m_IsActionType;
	}



	//spawns results in the world
	void SpawnItems(EntityAI ingredients[], PlayerBase player, array<Object> spawned_objects/*out*/)
	{
		//vector item_position = player.GetPosition() + ( player.GetDirection() * 1 );
		Debug.Log("SpawnItems called","recipes");
		spawned_objects.Clear();//just to make sure
		Object object = NULL;
		
		for(int i = 0; i < m_NumberOfResults; i++)
		{
			string item_to_spawn = m_ItemsToCreate[i];
			
			if( m_ResultInheritsColor[i] != -1 )
			{
				EntityAI item = ingredients[m_ResultInheritsColor[i]];
				string class_name = item.GetType();
				string color = item.ConfigGetString("color");
				string new_class_name = m_ItemsToCreate[i] + color;
				item_to_spawn = new_class_name;
			}
			
			if( m_ResultToInventory[i] == -1 ) 
			{
				Debug.Log("  = "+m_ResultToInventory[i].ToString(),"recipes");
				object = player.SpawnItem(item_to_spawn, SPAWNTYPE_INVENTORY, 0, 0);
			}
			else if ( m_ResultToInventory[i] == -2 )
			{
				object = player.SpawnItem(item_to_spawn, SPAWNTYPE_GROUND, 0, 0.5);
			}
			else if (m_ResultToInventory[i] >= 0)
			{
				object = player.SpawnItem(item_to_spawn, SPAWNTYPE_GROUND, 0, 0.5);
				EntityAI item_swap_with = ingredients[m_ResultToInventory[i]];
				player.SwapEntities(item_swap_with,object);
			}
			spawned_objects.Insert(object);
			Debug.Log("spawning item "+item_to_spawn,"recipes");
		}

	}

	//applies final modifications to both ingredients and results
	void ApplyModifications(EntityAI sorted[], array<Object> results, Object result)
	{
		float all_ingredinets_health = 0;//this is used later in results
		
		for(int i = 0; i < MAX_NUMBER_OF_INGREDIENTS; i++)
		{
			ItemBase ingrd = sorted[i];
			all_ingredinets_health += ingrd.GetHealth("", "");//accumulate health of all ingredients, used in results	
		}
		//------------------- results ----------------------
		for(i = 0; i < m_NumberOfResults; i++)
		{
			Object res = results.Get(i);
			if( res.IsItemBase() )
			{
				ItemBase resIb = res;
				if( !resIb.IsMagazine() )//is not a magazine
				{
					if( m_ResultSetFullQuantity[i] == 1 )//<------m_ResultSetFullQuantity
					{
						resIb.SetQuantityMax2();
					}
					else if( m_ResultSetQuantity[i] != -1 )//<------m_ResultSetQuantity
					{
						resIb.SetQuantity2( m_ResultSetQuantity[i] );
					}
				}
				else//is magazine
				{
					Magazine mgzn = resIb;
					if( m_ResultSetFullQuantity[i] == 1 )//<------m_ResultSetFullQuantity
					{
						mgzn.SetAmmoMax();
					}
					else if( m_ResultSetQuantity[i] != -1 )//<------m_ResultSetQuantity
					{
						mgzn.SetAmmoCount( m_ResultSetQuantity[i] );
					}
				}
			}
			if( m_ResultSetHealth[i] != -1 )//<------m_ResultSetHealth
			{
				res.SetHealth("","",m_ResultSetHealth[i]);
			}
			if( m_ResultInheritsHealth[i] != -1 )//<------m_ResultInheritsHealth
			{
				if( m_ResultInheritsHealth[i] >= 0 )
				{
					int ing_number = m_ResultInheritsHealth[i];
					EntityAI ing = sorted[ing_number];
					
					if( ing )
					{
						float ing_health = ing.GetHealth("","");
						res.SetHealth("", "", ing_health);
						Debug.Log("Inheriting health from ingredient:"+m_ResultInheritsHealth[i].ToString(),"recipes");
					}
				}
				else if( m_ResultInheritsHealth[i] == -2 )
				{
					float average_health = all_ingredinets_health / MAX_NUMBER_OF_INGREDIENTS;
					res.SetHealth("", "", average_health);
				}
			}
		}
		
		//---------------------- ingredients ----------------------
		for(i = 0; i < MAX_NUMBER_OF_INGREDIENTS; i++)
		{
			ItemBase ingredient = sorted[i];
			
			if( m_IngredientDestroy[i] == 1 )//<------m_IngredientDestroy
			{
				if( ingredient ) ingredient.Delete();
				sorted[i] = NULL;
			}
			else
			{
				if( m_IngredientAddHealth[i] != 0 )//<------m_IngredientAddHealth
				{
					float health_delta = m_IngredientAddHealth[i];
					ingredient.AddHealth("","",health_delta);
				}
				else if(m_IngredientSetHealth[i] != -1)//<------m_IngredientSetHealth
				{
					float new_health = m_IngredientSetHealth[i];
					ingredient.SetHealth("","",new_health);
				}
				if( m_IngredientAddQuantity[i] != 0 )//<------m_IngredientAddQuantity
				{
					if( !ingredient.IsMagazine() )
					{
						ItemBase obj = ingredient;
						//obj.AddQuantityAutoDestroy2( m_IngredientAddQuantity[i] );
						bool isDestroyed = obj.AddQuantity2(m_IngredientAddQuantity[i], true);
						if( isDestroyed ) ingredient = NULL;
					}
					else
					{
						Magazine mag = ingredient;
						int newQuantity = mag.GetAmmoCount() + m_IngredientAddQuantity[i];
						if( newQuantity <= 0 )
						{
							mag.Delete();
							ingredient = NULL;
						}
						else
						{
							mag.SetAmmoCount( newQuantity );
						}
					}
				}
			}
		}
	}

	//checks the recipe conditions
	bool CheckConditions(EntityAI sorted[])
	{
		for(int i = 0; i < MAX_NUMBER_OF_INGREDIENTS; i++)
		{
			ItemBase ingredient = sorted[i];
			if( !ingredient.IsMagazine() )
			{
				if( ingredient.GetQuantityMax2() !=0 && m_MinQuantityIngredient[i] >= 0 && ingredient.GetQuantity2() < m_MinQuantityIngredient[i] )
				{
					//Debug.Log("Recipe condition check failing1: m_MinQuantityIngredient","recipes");
					return false;
				}
				if( m_MaxQuantityIngredient[i] >= 0 && ingredient.GetQuantity2() > m_MaxQuantityIngredient[i] )
				{
					//Debug.Log("Recipe condition check failing1: m_MaxQuantityIngredient","recipes");
					return false;
				}
			}
			else
			{
				Magazine mag1 = ingredient;
				if( m_MinQuantityIngredient[i] >= 0 && mag1.GetAmmoCount() < m_MinQuantityIngredient[i] )
				{
					//Debug.Log("Recipe condition check failing1: m_MinQuantityIngredient[0]","recipes");
					return false;
				}
				if( m_MaxQuantityIngredient[i] >= 0 && mag1.GetAmmoCount() > m_MaxQuantityIngredient[i] )
				{
					//Debug.Log("Recipe condition check failing1: m_MaxQuantityIngredient[0]","recipes");
					return false;
				}
			}
			int dmg3 = ingredient.GetDamageLevel();
			if( m_MinDamageIngredient[i] >= 0 && ingredient.GetDamageLevel() < m_MinDamageIngredient[i] )
			{
				int dmg = ingredient.GetDamageLevel();
				//Debug.Log("Recipe condition check failing1: m_MinDamageIngredient[0]","recipes");
				return false;
			}
			if( m_MaxDamageIngredient[i] >= 0 && ingredient.GetDamageLevel() > m_MaxDamageIngredient[i] )
			{
				int dmg2 = ingredient.GetDamageLevel();
				//Debug.Log("Recipe condition check failing1: m_MaxDamageIngredient[0]","recipes");
				return false;
			}
		}
		return true;
	}

	//checks overall validity of this recipe
	bool CheckRecipe(EntityAI item1, EntityAI item2, PlayerBase player)
	{
		if( item1 == NULL || item2 == NULL )
		{
			Error("CheckRecipe: recipe invalid, at least one of the ingredients is NULL");
			Debug.Log("recipe invalid, at least one of the ingredients is NULL","recipes");
			return false;
		}
		
		//m_IngredientsSorted.Clear();
		//Debug.Log(ToString(items),"recipes");
		m_IngredientsSorted[0] = item1;
		m_IngredientsSorted[1] = item2;
		//Debug.Log("CheckRecipe, Ingredient1: "+ToString(item1.GetType()),"recipes");
		//Debug.Log("CheckRecipe, Ingredient2: "+ToString(item2.GetType()),"recipes");
		
		if( CanDo( m_IngredientsSorted, player ) && CheckConditions( m_IngredientsSorted ) )
		{
			
			//Debug.Log("Recipe "+ToString(this)+ " Valid","recipes");
			//Debug.Log("Ingredient1: "+ToString(m_IngredientsSorted.Get(0).Ptr().GetType()),"recipes");
			//Debug.Log("Ingredient2: "+ToString(m_IngredientsSorted.Get(1).Ptr().GetType()),"recipes");
			//Do( m_IngredientsSorted.Get(0),m_IngredientsSorted.Get(1) );
			return true;
		}
		return false;
	}
	
	void OnSelectedRecipe(EntityAI item1, EntityAI item2, PlayerBase player)
	{
		if( item1 == NULL || item2 == NULL )
		{
			Error("CheckRecipe: recipe invalid, at least one of the ingredients is NULL");
			Debug.Log("recipe invalid, at least one of the ingredients is NULL","recipes");
			return false;
		}
		OnSelected(item1,item2,player);
	}
	
	void OnSelected(EntityAI item1, EntityAI item2, PlayerBase player)
	{
		
	}

	//performs this recipe
	void PerformRecipe(EntityAI item1, EntityAI item2, Man player)
	{
		if( item1 == NULL || item2 == NULL )
		{
			Error("PerformRecipe: recipe invalid, at least one of the ingredients is NULL");
			Debug.Log("PerformRecipe: at least one of the ingredients is NULL","recipes");
		}
		
		m_IngredientsSorted[0] = item1;
		m_IngredientsSorted[1] = item2;
		/*
		Debug.Log("PerformRecipe Ingredient 1: "+ToString(item1.Ptr().GetType()),"recipes");
		Debug.Log("PerformRecipe Ingredient 2: "+ToString(item2.Ptr().GetType()),"recipes");
		*/
		if( CheckRecipe(item1,item2,player) )
		{
			autoptr array<Object> spawned_objects = new array<Object>;
			SpawnItems(m_IngredientsSorted, player,spawned_objects );
			
			Do( m_IngredientsSorted, player, spawned_objects );
			
			ApplyModifications(m_IngredientsSorted, spawned_objects, NULL);
		}
		else
		{
			Debug.Log("CheckRecipe failed on server","recipes");
		}
	}


	bool CanDo(EntityAI ingredients[], PlayerBase player)
	{
		return true;
	}

	void Do(EntityAI ingredients[], Man player, array<Object> results)
	{
		Debug.Log("Called Do on a recipe id:" + m_ID.ToString(),"recipes");
	}

	int GetID()
	{
		return m_ID;
	}


	void SetID(int id)
	{
		m_ID = id;
	}

	void GetAllItems(array<string> items)
	{
		for(int i = 0; i < MAX_NUMBER_OF_INGREDIENTS; i++)
		{
			array<string> ptr = m_Ingredients[i];
			
			for(int x = 0; x < ptr.Count(); x++)
			{
				items.Insert( ptr.Get(x) );
			}
		}
	}

	bool IsItemInRecipe(string item)
	{
		for(int i = 0; i < MAX_NUMBER_OF_INGREDIENTS; i++)
		{
			ptr = m_Ingredients[i];
			
			for(int x = 0; x < ptr.Count(); x++)
			{
				if( ptr.Get(x) == item ) return true;
			}
		}
		return false;
	}
	//! returns a mask which marks ingredient positions for a given item in this recipe(for example mask of value 3 [....000011] means this item is both ingredient 1 and 2 in this recipe[from right to left])
	int GetIngredientMaskForItem(string item)
	{
		int mask = 0;
		
		for(int i = 0; i < MAX_NUMBER_OF_INGREDIENTS; i++)
		{
			ptr = m_Ingredients[i];
			
			for(int x = 0; x < ptr.Count(); x++)
			{
				if( ptr.Get(x) == item )
				{
					mask = ((int)Math.Pow(2, i)) | mask;
				}
			}
		}
		return mask;
	}
}