class FoodStage
{
	protected string m_FoodStageName;
	
	protected int m_SelectionIndex;
	protected int m_TextureIndex;
	protected int m_MaterialIndex;
	
	protected autoptr array<float> m_FoodProperties;
	protected autoptr array<string> m_Modifiers;
	
	protected bool m_IsRotten;
	
	protected float m_CookingTime;
	
	//constructor
	void FoodStage ( ItemBase food_item )
	{
		//default
		m_FoodStageName = FoodManager.FOOD_STAGE_RAW;
		
		m_IsRotten = false;
		
		//reset cooking time
		m_CookingTime = 0;
		
		//get config data
		string config_path = "CfgVehicles" + " " + food_item.GetType() + " " + "FoodStages" + " " + m_FoodStageName;
		
		//if not exists in config
		if ( !GetGame().ConfigIsExisting ( config_path ) )
		{
			return;
		}
		
		//set params
		m_SelectionIndex 	= GetGame().ConfigGetInt ( config_path + " " + "selection" );
		m_TextureIndex 	= GetGame().ConfigGetInt ( config_path + " " + "texture" );
		m_MaterialIndex 	= GetGame().ConfigGetInt ( config_path + " " + "material" );
		
		m_FoodProperties = new array<float>;
		GetGame().ConfigGetFloatArray ( config_path + " " + "food_properties", m_FoodProperties );
		m_Modifiers = new array<string>;
		GetGame().ConfigGetTextArray  ( config_path + " " + "modifiers", m_Modifiers );
	}
	
	//Name
	string GetName()
	{
		return m_FoodStageName;
	}
	void SetName( string name )
	{
		m_FoodStageName = name;
	}
	
	//Selection index
	int GetSelectionIndex()
	{
		return m_SelectionIndex;
	}
	void SetSelectionIndex( int index )
	{
		m_SelectionIndex = index;
	}
	
	//Texture index
	int GetTextureIndex()
	{
		return m_TextureIndex;
	}
	void SetTextureIndex( int index )
	{
		m_TextureIndex = index;
	}
	
	//Material index
	int GetMaterialIndex()
	{
		return m_MaterialIndex;
	}
	void SetMaterialIndex( int index )
	{
		m_MaterialIndex = index;
	}
	
	//Food properties
	array<float> GetFoodProperties()
	{
		return m_FoodProperties;
	}
	
	//Food properties
	array<string> GetModifiers()
	{
		return m_Modifiers;
	}
	
	//Is Rotten	
	bool IsRotten()
	{
		return m_IsRotten;
	}
	void SetRottenState( bool state )
	{
		m_IsRotten = state;
	}
	
	//Cooking time
	float GetCookingTime()
	{
		return m_CookingTime;
	}
	void SetCookingTime( float time )
	{
		m_CookingTime = time;
	}
}
