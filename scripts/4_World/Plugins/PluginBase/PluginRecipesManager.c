class PluginRecipesManager extends PluginBase
{
	autoptr Timer m_TestTimer;
	const string KEYWORD_NEW_ITEM = "Item:";
	const string PATH_CACHE_FILE = "Scripts/Data/cache_recipes.cache";
	const int MAX_NUMBER_OF_RECIPES = 256;
	const int MAX_CONCURENT_RECIPES = 20;
	const int MASK_BOTH_INGREDIENTS = 3;
	
	const int MAX_INGREDIENTS = 5;
	
	int m_CachedRecipes[MAX_CONCURENT_RECIPES];
	
	
	EntityAI m_Ingredients[MAX_INGREDIENTS];
	int m_IngredientBitMask[MAX_INGREDIENTS];
	int m_IngredientBitMaskSize[MAX_INGREDIENTS];
	
	int m_BitsResults[MAX_INGREDIENTS];
	int m_ResolvePasses;
	
	
	EntityAI m_ingredient1[MAX_CONCURENT_RECIPES];
	EntityAI m_ingredient2[MAX_CONCURENT_RECIPES];
	EntityAI m_ingredient3[MAX_CONCURENT_RECIPES];
	
	EntityAI m_sortedIngredients[2];
	
	autoptr array<int> m_RecipesTmp;
	autoptr array<int> m_RecipesTmp2;
	
	CacheObject m_Itm1;//member variable for max perf
	CacheObject m_Itm2;//member variable for max perf
	
	RecipeBase m_Recipe;//member variable for max perf
	 
	CacheObject co_item_a;
	CacheObject co_item_b;
	
	array<int> item_a_recipes;
	array<int> item_b_recipes;
	
	array<int> item_a_masks;
	array<int> item_b_masks;
	
	
	autoptr map<string,CacheObject> m_CacheItemMap;//this is the final result of caching
	autoptr map<string,CacheObject> m_CacheBasesMap;//this is an optimization base map, used as an intermediary to speed up the cache generation by not looking up the already resolved base classes in recipes
	
	array<string> m_CachedItems;
	array<string>	m_Items;
	int m_NumberOfRecipes = 0;

	int m_NumOfConfRecipes;
	
	
	RecipeBase m_RecipeList[MAX_NUMBER_OF_RECIPES];//all recipes
	
	
	void PluginRecipesManager()
	{
		//PrintString("PluginRecipesManager constructor called");
		//m_TestTimer = new Timer();
		//m_TestTimer.Run(1, this, "CallbackGenerateCache", NULL, false);
		//m_TestTimer.Run(30, this, "GenerateRecipeCache", NULL, false);
		//m_TestTimer.Run(10, this, "PrintCache", NULL, false);
		//m_TestTimer.Run(15, this, "ReadTest", NULL, false);
		
		m_RecipesTmp = new array<int>;
		m_RecipesTmp2 = new array<int>;
		
		m_CacheItemMap = new map<string,CacheObject>;
		m_CacheBasesMap = new map<string,CacheObject>;
		
		m_NumberOfRecipes = 0;
		
		for(int i = 0; i < MAX_NUMBER_OF_RECIPES; i++)
		{
			m_RecipeList[i] = NULL;
		}
		
		CreateAllRecipes();
		//SaveCacheToFile("Scripts/Data/cache_recipes.h");//generate the cache and save it to a file
		
		//SaveCacheToFile("Scripts/Data/cache_recipes2.h");
	}
	
	void ~PluginRecipesManager()
	{
		PrintString("PluginRecipesManager destructor called");
		//delete recipes
		for(int i = 0; i < MAX_NUMBER_OF_RECIPES; i++)
		{
			if(m_RecipeList[i] != NULL) delete m_RecipeList[i];
		}
		//delete cache items
		while( m_CacheItemMap.Count() > 0 )
		{
			if( m_CacheItemMap.GetElement(0) ) delete m_CacheItemMap.GetElement(0);
			m_CacheItemMap.RemoveElement(0);
		}
		//delete cache baseclasses
		while( m_CacheBasesMap.Count() > 0 )
		{
			if( m_CacheBasesMap.GetElement(0) ) delete m_CacheBasesMap.GetElement(0);
			m_CacheBasesMap.RemoveElement(0);
		}
	}
	
	//will fill the map 'ids' with valid recipes for the 'item1' and 'item2' items, where the key is the recipe ID, and the value is the recipe name
	int GetValidRecipes(EntityAI item1, EntityAI item2, map<int,string> ids, PlayerBase player)//fast version
	{
		if( item1 == NULL || item2 == NULL ) return 0;
		
		m_Ingredients[0] = item1;
		m_Ingredients[1] = item2;
		
		return GetValidRecipesProper(2,ids,player);

	}
	
	int GetValidRecipesProper(int num_of_items, map<int,string> ids, PlayerBase player)
	{
		ids.Clear();
		//Debug.Log("GetValidRecipes called 1","recipes");
		int numOfRecipes = GetRecipeIntersection(num_of_items,m_CachedRecipes);
		//will return number of cached recipes for the 2 items being considered, 
		//and save their indexes in m_CachedRecipes, please note the m_CachedRecipes is a static array, 
		//and does not clear up with each query, so the number of recipes returned is critical to make sure we don't accidentally
		//mix in some other indexes from previous queries(and obviously loop only as far as we have to)
		//this also saves the ingredients in the correct assignment as ingredient 1/2 into m_ingredient1/m_ingredient2 arrays, these 3 arrays
		//therefore provide you with information per each index with: recipeid,ingredience1,ingredience2
		if( numOfRecipes == 0 ) return 0;
		int found = 0;
		RecipeBase p_recipe = NULL;
		for(int i = 0; i < numOfRecipes; i++)
		{
			p_recipe = m_RecipeList[m_CachedRecipes[i]];

			if( p_recipe.CheckRecipe(m_ingredient1[i],m_ingredient2[i], player) == true )
			{
				ids.Insert( p_recipe.GetID(), p_recipe.GetName() );
				if(GetDayZGame().IsDebugActions())
				{
					ids.Insert( p_recipe.GetID()+1000, "[DEBUG]"+p_recipe.GetName() );
				}
				found++;
			}
		}
		//Debug.Log("GetValidRecipes called 3 , found = "+ToString(found) ,"recipes");
		return found;
	}
	
	bool GetValidAction(EntityAI item1, EntityAI item2, Param2<int,string> param, PlayerBase player)
	{
		if( item1 == NULL || item2 == NULL ) return 0;
		
		m_Ingredients[0] = item1;
		m_Ingredients[1] = item2;
		
		int numOfRecipes = GetRecipeIntersection(2,m_CachedRecipes);
		
		if( numOfRecipes == 0 ) return false;
		
		RecipeBase ptrRecipe = NULL;
		for(int i = 0; i < numOfRecipes; i++)
		{
			ptrRecipe = m_RecipeList[m_CachedRecipes[i]];
			if( ptrRecipe.IsAction() )
			{
				if( ptrRecipe.CheckRecipe(m_ingredient1[i],m_ingredient2[i], player) == true )
				{
					//Debug.Log("GetValidRecipeID returning id:" + ToString( m_RecipeList.Get(i).GetID() ),"recipes");
					param.param1 = ptrRecipe.GetID();
					param.param2 = ptrRecipe.GetName();
					return true;
				}
			}
		}
		return false;
	}
	
	void OnInit()
	{
		super.OnInit();
		ReadCacheFromFile(PATH_CACHE_FILE);//read the cache from a file
	}


	void CallbackGenerateCache()
	{
		Debug.Log("CallbackGenerateCache","recipes");
		SaveCacheToFile(PATH_CACHE_FILE);//generate the cache and save it to a file
		ReadCacheFromFile(PATH_CACHE_FILE);
	}

	void GenerateRecipeCache(array<string> cached_items)
	{
		GetGame().ProfilerStart("RECIPE_CACHE");
		
		m_CacheBasesMap.Clear();
		m_CacheItemMap.Clear();
		
		autoptr TStringArray all_config_paths = new TStringArray;
		
		all_config_paths.Insert(CFG_VEHICLESPATH);
		all_config_paths.Insert(CFG_WEAPONSPATH);
		all_config_paths.Insert(CFG_MAGAZINESPATH);
		//Debug.Log("Got here 0","caching");
		string config_path;
		string child_name;
		int scope;
		bool isInRecipe;
		autoptr TStringArray full_path = new TStringArray;
		
		for(int i = 0; i < all_config_paths.Count(); i++)
		{
			config_path = all_config_paths.Get(i);
			int children_count = GetGame().ConfigGetChildrenCount(config_path);
			
			for(int x = 0; x < children_count; x++)
			{
				GetGame().ConfigGetChildName(config_path, x, child_name);
				scope = GetGame().ConfigGetInt( config_path + " " + child_name + " scope" );

				if ( scope != 0 )
				{
					GetGame().ConfigGetFullPath(config_path +" "+ child_name,/*out*/ full_path);
					EvaluateFullPathAgainstRecipes(full_path, cached_items);
				}
			}
		}
		GetGame().ProfilerStop("RECIPE_CACHE");
	}

	void EvaluateFullPathAgainstRecipes(TStringArray full_path, array<string> cached_items)
	{
		ResolveBaseClasses(full_path);
		ResolveItemClasses(full_path);
		LeechRecipesFromBases(full_path);
	}


	
	void ResolveBaseClasses(TStringArray full_path)
	{
		int mask;
		string item;
		RecipeBase p_recipe;
		CacheObject co_base = NULL;
		for(int i = 1; i < full_path.Count(); i++)
		{
			item = full_path.Get(i);
			
			if( !m_CacheBasesMap.Contains( item ) )//resolve new base classes
			{
				for(int x = 0; x < MAX_NUMBER_OF_RECIPES; x++)
				{
					if (m_RecipeList[x] != NULL)
					{
						p_recipe = m_RecipeList[x];
						mask = p_recipe.GetIngredientMaskForItem( item );
						
						if( mask > 0 )
						{
							if(!co_base)
							{
								m_CacheBasesMap.Insert(item,new CacheObject);
								co_base = m_CacheBasesMap.Get(item);
							}
							co_base.AddRecipe(x, mask);
						}
					}
				}
			}
		}
		
	}
	
	void LeechRecipesFromBases(TStringArray full_path)
	{
		string item_name = full_path.Get(0);
		
		string base_name;
		
		RecipeBase p_recipe;
		
		array<int> temp_base_rcps_array = NULL;
		array<int> temp_base_msks_array = NULL;

		CacheObject co_item;
		for(int i = 1/*skip the item classname*/; i < full_path.Count(); i++)
		{
			base_name = full_path.Get(i);
			
			if( m_CacheBasesMap.Contains( base_name ) )
			{
				temp_base_rcps_array = m_CacheBasesMap.Get(base_name).GetRecipes();
				temp_base_msks_array = m_CacheBasesMap.Get(base_name).GetMasks();
				if( !m_CacheItemMap.Get(item_name) )
				{
					CreateNewCacheItem(item_name);
				}
				co_item = m_CacheItemMap.Get(item_name);
				for( int x = 0; x < temp_base_rcps_array.Count(); x++ )//base recipes
				{
					int recipe_id = temp_base_rcps_array.Get(x);
					int base_mask = temp_base_msks_array.Get(x);
					
					int item_mask = co_item.GetMaskByRecipeID(recipe_id);
						
					if( item_mask > 0 )
					{
						co_item.UpdateMask(recipe_id, base_mask | item_mask );
					}
					else
					{
						co_item.AddRecipe(recipe_id,base_mask);
					}
				}
				
			}
		}
	}
	
	//this will take the item class name and resolve it against all recipes
	void ResolveItemClasses(TStringArray full_path)
	{
		string item_name = full_path.Get(0);
		RecipeBase p_recipe;
		
		for(int i = 0; i < MAX_NUMBER_OF_RECIPES; i++)
		{
			if (m_RecipeList[i] != NULL)
			{
				p_recipe = m_RecipeList[i];
				int mask = p_recipe.GetIngredientMaskForItem( item_name );
				
	
				if( mask > 0)
				{
					if( m_CacheItemMap.Contains( item_name ) )//this should not happen as every item should be unique
					{
						m_CacheItemMap.Get(item_name).AddRecipe(i,mask);
					}
					else
					{
						CreateNewCacheItem(item_name);
						m_CacheItemMap.Get(item_name).AddRecipe(i,mask);
					}
				}
			}
		}
	}

	void CreateNewCacheItem(string item_name)
	{
		m_CacheItemMap.Insert(item_name, new CacheObject);
		m_CachedItems.Insert(item_name);
	}
/*
	void PerformRecipe(int id, EntityAI item1,EntityAI item2,Man player)
	{
		Debug.Log("PerformRecipe call on id: "+ToString(id),"recipes");
		RecipeBase ptrRecipe = m_RecipeList[id];
		ptrRecipe.PerformRecipe(item1,item2,player);
	}
*/
	void PerformRecipeServer(int id, EntityAI item_a,EntityAI item_b ,Man player)
	{
		m_Ingredients[0] = item_a;
		m_Ingredients[1] = item_b;
		
		SortIngredients(id, 2,m_Ingredients, m_sortedIngredients);

		RecipeBase ptrRecipe = m_RecipeList[id];
		ptrRecipe.PerformRecipe(m_sortedIngredients[0],m_sortedIngredients[1],player);
		
	}
	
	void RegisterRecipe(RecipeBase recipe, int id)
	{
		recipe.SetID(id);
		m_RecipeList[id] = recipe;
	}

	void PerformRecipeClientRequest(int id, EntityAI item_a, EntityAI item_b)
	{
		
		autoptr Param param1 = new Param1<int>(id);
		autoptr Param param2 = new Param1<EntityAI>(item_a);
		autoptr Param param3 = new Param1<EntityAI>(item_b);
		
		autoptr array<Param> params = new array<Param>;
		
		params.Insert(param1);
		params.Insert(param2);
		params.Insert(param3);
		
		GetGame().RPC(GetGame().GetPlayer(), RPC_RECIPE_SEND, params, GetGame().GetPlayer());
		
	}
	
	bool CheckRecipe(int id, EntityAI item1, EntityAI item2, PlayerBase player)//requires ordered items
	{
		RecipeBase p_recipe = m_RecipeList[id];
		return p_recipe.CheckRecipe(item1,item2, player);
	}
	
	void RecipeSelected(int id, EntityAI item1, EntityAI item2, PlayerBase player)//requires ordered items
	{
		RecipeBase p_recipe = m_RecipeList[id];
		p_recipe.OnSelectedRecipe(item1,item2, player);
	}
	
	void OnRPC(ParamsReadContext ctx, Man player)
	{
		Debug.Log("OnRPC called","recipes");
		autoptr Param1<int> param1 = new Param1<int>(0);
		autoptr Param1<EntityAI> param2 = new Param1<EntityAI>(NULL);
		autoptr Param1<EntityAI> param3 = new Param1<EntityAI>(NULL);
		
		ctx.Read(param1);
		ctx.Read(param2);
		ctx.Read(param3);
		
		Debug.Log("OnRPC i1: "+param2.param1.GetType(),"recipes");
		Debug.Log("OnRPC i2: "+param3.param1.GetType(),"recipes");
		
		int id = param1.param1;
		EntityAI item1 = param2.param1;
		EntityAI item2 = param3.param1;
		
		ReceivedRecipe(id,item1,item2,player);
	}

	
	bool CheckMaskOverlay(int mask_a, int mask_b )
	{
		if( mask_a | mask_b == MASK_BOTH_INGREDIENTS ) 
		{
			return true;
		}
		else 
		{
			return false;
		}
	}
	
	void ReceivedRecipe(int id, EntityAI item_a, EntityAI item_b, Man player)
	{
		Debug.Log("ReceivedRecipe called","recipes");
		
		Debug.Log("ReceivedRecipe i1: "+item_a.GetType(),"recipes");
		Debug.Log("ReceivedRecipe i2: "+item_b.GetType(),"recipes");
		
		bool is_debug_recipe = false;
		
		if( id >= 1000 && GetGame().IsDebugActions() )
		{
			id -= 1000;
			is_debug_recipe = true;
		}
		/*
		CacheObject co_item_a = m_CacheItemMap.Get( item_a.GetType() );
		CacheObject co_item_b = m_CacheItemMap.Get( item_b.GetType() );
		
		int mask_item_a = co_item_a.GetMaskByRecipeID(id);
		int mask_item_b = co_item_b.GetMaskByRecipeID(id);
		*/
		
		m_Ingredients[0] = item_a;
		m_Ingredients[1] = item_b;
		
		SortIngredients(id, 2,m_Ingredients,m_sortedIngredients);
		
		bool is_recipe_valid = CheckRecipe(id,m_sortedIngredients[0],m_sortedIngredients[1],player);
		
		PlayerBase p = player;
		if(is_recipe_valid)
		{
			Debug.Log("Recipe passed server test","recipes");
			
			//if(GetGame().IsServer()) Debug.Log("Is Server","recipes");
			//if(GetGame().IsClient()) Debug.Log("Is Client","recipes");
			RecipeBase ptrRecipe = m_RecipeList[id];
				
			if(!is_debug_recipe)
			{
				p.SetUpCrafting(id,item_a,item_b);
			}
			else
			{
				PerformRecipeServer(id, item_a, item_b, player);
			}
			RecipeSelected( id, m_sortedIngredients[0], m_sortedIngredients[1], player );
		}
		else
		{
			if(is_recipe_valid)
			{
				Debug.LogError("Recipe failed to pass server test","recipes");
				Error("Recipe failed to pass server test");
			
			}
		}
		
	}
	
	void PrintCache()
	{
		for(int i = 0; i < m_CacheItemMap.Count(); i++)
		{
			string key = m_CacheItemMap.GetKey(i);
			CacheObject value = m_CacheItemMap.GetElement(i);
			autoptr array<int> recipes = new array<int>;

			recipes.InsertAll( value.GetRecipes() );
			PrintString("Item: " + key);
			
			for(int x = 0; x < recipes.Count(); x++)
			{
				PrintString("Recipe: " + recipes.Get(x).ToString());
			}
		}
	}
	//!sorts ingredients correctly as either first or second ingredient based on their masks
	bool SortIngredients(int id, int num_of_ingredients, EntityAI ingredients_unsorted[], EntityAI ingredients_sorted[] )
	{
		ClearResults();
		
		co_item_a = m_CacheItemMap.Get( ingredients_unsorted[0].GetType() );
		co_item_b = m_CacheItemMap.Get( ingredients_unsorted[1].GetType() );
		
		
		m_IngredientBitMask[0] = co_item_a.GetMaskByRecipeID(id);
		m_IngredientBitMask[1] = co_item_b.GetMaskByRecipeID(id);
		
		m_IngredientBitMaskSize[0] = co_item_a.GetBitCountByRecipeID(id);
		m_IngredientBitMaskSize[1] = co_item_b.GetBitCountByRecipeID(id);
		
		bool result = ResolveIngredients(num_of_ingredients);
		
		if(result)
		{
			for(int i = 0; i < num_of_ingredients; i++)
			{
				int index = Math.Log2( m_BitsResults[i]);
				EntityAI rslt = m_Ingredients[index ];
				ingredients_sorted[index] = m_Ingredients[ i ];
			}
		}
		//PrintResultMasks(num_of_ingredients);
		return result;
	}
	
	void ClearResults()
	{
		for(int i = 0; i < MAX_INGREDIENTS; i++)
		{
			m_BitsResults[i] = 0;			
		}	
			
	}
	
	bool ResolveIngredients(int num_of_ingredients, int passes = 0)
	{
		int rightmost_bit;
		int smallest = 99999;
		int smallest_index = 0;

		for(int i = 0; i < num_of_ingredients; i++)
		{
			int count = m_IngredientBitMaskSize[i];
			if( count != 0 && count < smallest)
			{
				smallest = m_IngredientBitMaskSize[i];
				smallest_index = i;
			}
		}
		
		rightmost_bit = m_IngredientBitMask[smallest_index] & (-m_IngredientBitMask[smallest_index]);
		m_BitsResults[smallest_index] = m_BitsResults[smallest_index] | rightmost_bit;
		/*
		Debug.Log("smallest_index = " +ToString(smallest_index));
		Debug.Log("rightmost_bit = " +ToString(rightmost_bit));
*/
		//remove from source
		
		for(int x = 0; x < num_of_ingredients; x++)
		{
			m_IngredientBitMask[x] = ~rightmost_bit & m_IngredientBitMask[x];
			m_IngredientBitMask[smallest_index] = 0;
			m_IngredientBitMaskSize[smallest_index] = 0;
		}
		
		// check validity
		int check_sum_vectical = 0;
		
		for(int z = 0; z < num_of_ingredients; z++)
		{
			check_sum_vectical = check_sum_vectical | m_IngredientBitMask[z];//vertical sum
			check_sum_vectical = check_sum_vectical | m_BitsResults[z];//vertical sum
			if((m_IngredientBitMask[z] | m_BitsResults[z]) == 0) return false;//horizontal check
		}
		
		if( check_sum_vectical != (Math.Pow(2, num_of_ingredients) - 1)) return false;//vertical check
		
		passes++;
		
		if(passes < num_of_ingredients) 
		{
			ResolveIngredients(num_of_ingredients, passes);
		}
		else
		{
			return true;
		}
		
	}
	
	
	void PrintResultMasks(int num)
	{
		for(int i = 0; i < num; i++)
		{
			Debug.Log("results mask("+i.ToString()+") = " +m_BitsResults[i].ToString() );
		}
	}
	
	//!will return number of recipes which 'item_a' and 'item_b' share and where they can function as different ingredients, will also fill out 3 static arrays with additional information if they do
	int GetRecipeIntersection(int num_of_ingredients,  int intersection[])
	{
		int count = 0;
		int smallest = 9999;
		int smallest_index = 0;
		/*
		m_Ingredients[0] = item_a;
		m_Ingredients[1] = item_b;
		*/
		//find the item with smallest number of recipes
		CacheObject co_least_recipes;
		
		for(int i = 0; i < num_of_ingredients; i++)
		{
			CacheObject cobject = m_CacheItemMap.Get( m_Ingredients[i].GetType() );
			if(!cobject) 
			{
				return 0;
			}
			if(cobject.GetNumberOfRecipes() < smallest)
			{
				smallest = cobject.GetNumberOfRecipes();
				smallest_index = i;
				co_least_recipes = cobject;
			}
		}
		
		//look for matches
		array<int> recipes = co_least_recipes.GetRecipes();
		bool fail;
		for(int x = 0; x < recipes.Count(); x++)
		{
			int id = recipes.Get(x);
			fail = false;
			for(int z = 0; z < num_of_ingredients; z++)
			{
				if( z!= smallest_index)
				{
					CacheObject cobject2 = m_CacheItemMap.Get( m_Ingredients[z].GetType() );
					if( !cobject2.IsContainRecipe(id) )
					{
						fail = true;
					}
				}
			}
			if(!fail)
			{
				if(SortIngredients(id, num_of_ingredients,m_Ingredients, m_sortedIngredients))//...and now we need to determine which item will be which ingredient number
				{
					intersection[count] = id;
					
					m_ingredient1[count] = m_sortedIngredients[0];
					m_ingredient2[count] = m_sortedIngredients[1];
					m_ingredient3[count] = m_sortedIngredients[2];

					count++;
				}
			}
		}
		return count;
	}	
	
	
	void CreateAllRecipes()
	{
		#include "Scripts\4_World\Classes\Recipes\Recipes\_RecipeList.inc"
	}
	
	array<int> GetConfigRecipesSingle( EntityAI item )
	{
	}
	
	void ReadTest()
	{
		ReadCacheFromFile("PATH_CACHE_FILE");//read the cache from a file
	}

	
	//string m_CacheStringBuffer[128];
	
	void SaveCacheToFile(string filename)
	{
		FileHandle file = OpenFile(filename, FileMode.WRITE);
		if( file!=0 )
		{
			m_CachedItems = new array<string>;
			GenerateRecipeCache(m_CachedItems);
		
			for(int i = 0; i < m_CachedItems.Count(); i++)
			{
				string cache_key = m_CachedItems.Get(i);
				array<int> recipe_array = m_CacheItemMap.Get(cache_key).GetRecipes();
				array<int> mask_array = m_CacheItemMap.Get(cache_key).GetMasks();
				
				
				FPrintln(file, KEYWORD_NEW_ITEM);
				FPrintln(file,cache_key);
				
				for(int x = 0; x < recipe_array.Count(); x++)
				{
					int recipe_id = recipe_array.Get(x);
					int mask = mask_array.Get(x);
					FPrintln(file,"recipeID:");
					FPrintln(file,recipe_id);
					FPrintln(file,"mask:");
					FPrintln(file,mask);
				}
			}
			CloseFile(file);
			delete m_CachedItems;
		}
		else
		{
			Debug.Log("failed to open the cache file");
		}
	}
	
	void ReadCacheFromFile(string filename)
	{
		GetGame().ProfilerStart("RECIPE_CACHE_READ");
		Debug.Log("Reading cache from file","recipes");
		FileHandle file = OpenFile(filename, FileMode.READ);
		Print(file);
		int line_index = 0;
		int num_of_items = 0;
		string cache_key;
		string curr_line;
		m_CacheItemMap.Clear();
		
		while(true)
		{
			int fg = FGets(file,curr_line);
			//PrintString("fg: "+ToString(fg));
			if( fg == -1) break;
			//Debug.Log("Got here","recipes");
			if( curr_line == KEYWORD_NEW_ITEM)
			{
				FGets(file,curr_line);//skip the keyword to get to the classname
				cache_key = curr_line;
				m_CacheItemMap.Insert(cache_key, new CacheObject);
			}
			else
			{
				FGets(file,curr_line);//skip keyword for recipeID
				int item = curr_line.ToInt();
				FGets(file,curr_line);//skip keyword for mask
				FGets(file,curr_line);//get the mask
				int mask = curr_line.ToInt();
				m_CacheItemMap.Get(cache_key).AddRecipe(item, mask);
			}

		}
		//PrintCache();
		CloseFile(file);
		GetGame().ProfilerStop("RECIPE_CACHE_READ");
	}
}