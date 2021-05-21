class FoodManager
{
	protected array<string> m_ClearModifiers;
	
	static const string FOOD_STAGE_RAW 		= "Raw";
	static const string FOOD_STAGE_BAKED 	= "Baked";
	static const string FOOD_STAGE_BOILED 	= "Boiled";
	static const string FOOD_STAGE_DRIED 	= "Dried";
	static const string FOOD_STAGE_BURNED 	= "Burned";
	
	//constructor/destructor
	void FoodManager()
	{
		m_ClearModifiers = new array<string>;
		m_ClearModifiers.Insert ( "FoodPoisoning_MediumImpact" );
		m_ClearModifiers.Insert ( "FoodPoisoning_LightImpact" );
		m_ClearModifiers.Insert ( "Cholera" );
		m_ClearModifiers.Insert ( "Salmonellosis" );
	}

	static void ChangeFoodStage ( ItemBase food_item, string new_stage_name )
	{
		//can change to target stage?
		if ( FoodManager.CanChangeToNewStage ( food_item, new_stage_name ) )
		{
			return;
		}
		
		EN5C_Edible_Base food = ( EN5C_Edible_Base ) food_item;
		FoodStage food_stage = food.GetFoodStage();

		string config_path = "CfgVehicles" + " " + food.GetType() + " " + "FoodStages";
		
		//merge stages
		//name
		food_stage.SetName( new_stage_name );
		//selection index
		int index = GetGame().ConfigGetInt ( config_path + " " + new_stage_name + " " + "selection" );
		if ( index >= 0 )
		{
			food_stage.SetSelectionIndex( index );
		}
		//texture index
		index = GetGame().ConfigGetInt ( config_path + " " + new_stage_name + " " + "texture" );
		if ( index >= 0 )
		{
			food_stage.SetTextureIndex( index );
		}
		//material index
		index = GetGame().ConfigGetInt ( config_path + " " + new_stage_name + " " + "material" );
		if ( index >= 0 )
		{
			food_stage.SetMaterialIndex( index );
		}	
		//food properties
		food_stage.GetFoodProperties().Clear();
		GetGame().ConfigGetFloatArray ( config_path + " " + new_stage_name + " " + "food_properties", food_stage.GetFoodProperties() );
		
		//TODO - merge modifiers that are already on item
		//string message_1 = "Changed to new stage = " + new_stage_name + " | config_path = " + config_path + " | texture index = " + food_stage.GetTextureIndex().ToString();
		//Print (message_1);
		
		//refresh visual
		FoodManager.RefreshVisual ( food_item );
	}

	static void RefreshVisual ( ItemBase food_item )
	{
		EN5C_Edible_Base food = ( EN5C_Edible_Base ) food_item;
		FoodStage food_stage = food.GetFoodStage();
		
		//if item has no food stages
		if ( food_stage == NULL || !HasFoodStageInConfig ( food_item ) ) 
		{
			return;
		}
		
		//Selections
		string config_path;
		autoptr array<string> config_selections	= new array<string>;
		autoptr array<string> config_textures	= new array<string>;
		autoptr array<string> config_materials	= new array<string>;
			
		//selections
		config_path = "CfgVehicles" + " " + food_item.GetType() + " " + "hiddenSelections";
		GetGame().ConfigGetTextArray ( config_path, config_selections );
		//textures
		config_path = "CfgVehicles" + " " + food_item.GetType() + " " + "hiddenSelectionsTextures";
		GetGame().ConfigGetTextArray ( config_path, config_textures );
		//materials
		config_path = "CfgVehicles" + " " + food_item.GetType() + " " + "hiddenSelectionsMaterials";
		GetGame().ConfigGetTextArray ( config_path, config_materials );
		
		//hide all selection except the configured one
		for ( int i = 0; i < config_selections.Count(); i++ )
		{
			if ( config_selections.Get ( i ) != config_selections.Get ( food_stage.GetSelectionIndex() ) )
			{
				food_item.SetAnimationPhase ( config_selections.Get ( i ), 1 );
			}
		}
		
		//show selection
		food_item.SetAnimationPhase ( config_selections.Get ( food_stage.GetSelectionIndex() ), 0 );
		//set texture
		food_item.SetObjectTexture ( food_stage.GetSelectionIndex(), config_textures.Get ( food_stage.GetTextureIndex() ) );
		//set materials
		food_item.SetObjectMaterial ( food_stage.GetSelectionIndex(), config_materials.Get ( food_stage.GetMaterialIndex() ) );
	}
	
	static bool HasFoodStageInConfig( ItemBase item )
	{
		string config_path = "CfgVehicles" + " " + item.GetType() + " " + "FoodStages";
		return GetGame().ConfigIsExisting ( config_path );
	}
	
	//Checks if food stage can be changed to another stage
	static bool CanChangeToNewStage ( ItemBase food_item, string cooking_method )
	{
		if ( FoodManager.GetNextFoodStageName ( food_item, cooking_method ) == "" )
		{
			return false;
		}
		else
		{
			return true;
		}
	}

	//returns possible food stage name according to given cooking method
	static string GetNextFoodStageName ( ItemBase food_item, string cooking_method )
	{
		EN5C_Edible_Base food = ( EN5C_Edible_Base ) food_item;
		FoodStage food_stage = food.GetFoodStage();
		
		autoptr array<string> string_output = new array<string>;
		autoptr array<string> stage_transitions = new array<string>;
		
		//get stage transitions from config
		string config_path = "CfgVehicles" + " " + food.GetType() + " " + "FoodStages";
		GetGame().ConfigGetTextArray ( config_path + " " + food_stage.GetName() + " " + "stage_transitions", stage_transitions );
		
		for ( int i = 0; i < stage_transitions.Count(); i++ )
		{
			string_output.Clear();
			
			stage_transitions.Get ( i ).Split ( ">", string_output );		//'Process->Stage' 0 - Process, 1 - Stage
			
			if ( cooking_method == string_output.Get ( 0 ) )
			{
				return string_output.Get ( 1 );
			}
		}
		
		return "";
	}
	
	//check stages
	static bool IsFoodInStage( ItemBase item, string stage_name )
	{
		EN5C_Edible_Base food_item = ( EN5C_Edible_Base ) item;

		if ( food_item.GetFoodStage().GetName() == stage_name )
		{
			return true;
		}
		
		return false;
	}
	
	static bool IsFoodRaw( ItemBase food_item )
	{
		return FoodManager.IsFoodInStage( food_item, FOOD_STAGE_RAW );
	}
	
	static bool IsFoodBaked( ItemBase food_item )
	{
		return FoodManager.IsFoodInStage( food_item, FOOD_STAGE_BAKED );
	}
	
	static bool IsFoodBoiled( ItemBase food_item )
	{
		return FoodManager.IsFoodInStage( food_item, FOOD_STAGE_BOILED );
	}
	
	static bool IsFoodDried( ItemBase food_item )
	{
		return FoodManager.IsFoodInStage( food_item, FOOD_STAGE_DRIED );
	}
	
	static bool IsFoodBurned( ItemBase food_item )
	{
		return FoodManager.IsFoodInStage( food_item, FOOD_STAGE_BURNED );
	}
	
	static bool IsFoodRotten( ItemBase item )
	{
		EN5C_Edible_Base food_item = ( EN5C_Edible_Base ) item;
		
		return food_item.GetFoodStage().IsRotten();
	}
}
