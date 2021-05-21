class EN5C_Edible_Base extends ItemBase
{
	autoptr FoodStage m_FoodStage;
	
	void EN5C_Edible_Base()
	{
		if ( !m_FoodStage )
		{
			m_FoodStage = new FoodStage ( this );
		}
	}
	
	void Initialize()
	{
		FoodManager.RefreshVisual ( this );
	}
	
	//on store save/load
	void OnStoreSave( ParamsWriteContext ctx )
	{   
		super.OnStoreSave( ctx );
		
		//Food stage name 
		ctx.Write( GetFoodStage().GetName() );
		
		//Selection index
		ctx.Write( GetFoodStage().GetSelectionIndex() );
		
		//Texture index
		ctx.Write( GetFoodStage().GetTextureIndex() );
		
		//Material index
		ctx.Write( GetFoodStage().GetMaterialIndex() );
		
		//Food properties
		//size
		int food_prop_size = 0;
		if ( GetFoodStage().GetFoodProperties() )	//if not null
		{
			food_prop_size = GetFoodStage().GetFoodProperties().Count();
		}
		ctx.Write( food_prop_size );
		//data
		for ( int i = 0; i < food_prop_size; i++ )
		{
			ctx.Write( GetFoodStage().GetFoodProperties().Get( i ) );
		}
		
		//Modifiers
		//size
		int modifiers_size = 0;
		if ( GetFoodStage().GetModifiers() )	//if not null
		{
			modifiers_size = GetFoodStage().GetModifiers().Count();
		}
		ctx.Write( modifiers_size );
		//data
		for ( int j = 0; j < modifiers_size; j++ )
		{
			ctx.Write( GetFoodStage().GetModifiers().Get( j ) );
		}
		
		//Rotten state
		ctx.Write( GetFoodStage().IsRotten() );
	}
	
	void OnStoreLoad( ParamsReadContext ctx )
	{
		super.OnStoreLoad(ctx);
		
		//Food stage name 
		string stage_name = "";
		ctx.Read( stage_name );
		GetFoodStage().SetName( stage_name );
		
		//Selection index
		int selection_idx = 0;
		ctx.Read( selection_idx );
		GetFoodStage().SetSelectionIndex( selection_idx );
		
		//Texture index
		int texture_idx = 0;
		ctx.Read( texture_idx );
		GetFoodStage().SetTextureIndex( texture_idx );
		
		//Material index
		int material_idx = 0;
		ctx.Read( material_idx );
		GetFoodStage().SetMaterialIndex( material_idx );
		
		//Food properties
		//count
		int food_prop_size = 0;
		ctx.Read( food_prop_size );		
		//data
		for ( int i = 0; i < food_prop_size; i++ )
		{
			float food_prop;
			ctx.Read( food_prop );
			GetFoodStage().GetFoodProperties().Insert( food_prop );
		}
		
		//Modifiers
		//count
		int modifiers_size = 0;
		ctx.Read( modifiers_size );		
		//data
		for ( int j = 0; j < modifiers_size; j++ )
		{
			string modifier;
			ctx.Read( modifier );
			GetFoodStage().GetModifiers().Insert( modifier );
		}
		
		//Rotten state
		bool is_rotten;
		ctx.Read( is_rotten );
		GetFoodStage().SetRottenState( is_rotten );		
		
		//refresh visual after load
		FoodManager.RefreshVisual ( this );
	}
	
	FoodStage GetFoodStage()
	{
		return this.m_FoodStage;
	}
	
	//food types
	bool IsMeat()
	{
		return false;
	}
}

//Global function
//TODO - remove when INIT event in fixed
void FoodInitialize( ItemBase item )
{
	EN5C_Edible_Base food = ( EN5C_Edible_Base ) item;
	food.Initialize();
}
