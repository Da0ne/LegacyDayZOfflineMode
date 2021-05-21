class EN5C_PlantBase extends ItemBase
{
	// Plant states
	static const int STATE_DRY 					= 0;
	static const int STATE_GROWING 				= 1;
	static const int STATE_MATURE 				= 2;
	static const int STATE_SPOILED 				= 3;

	private int 	m_WaterUsage; 					// How much water is needed to water a plant from a bottle. Value is in mililitres
	private float 	m_SprayUsage; 					// How much spray is needed to stop infestation of plant
	
	private float 	m_InfestationChance;
	
	private int 	m_GrowthStagesCount; 
	private int 	m_CropsCount;
	private bool 	m_HasCrops;
	private string 	m_CropsType; 
	private float 	m_PlantMaterialMultiplier; 
	
	private int 	m_PlantState;
	private int 	m_PlantStateIndex;
	private float 	m_CurrentPlantMaterialQuantity;
	
	private bool 	m_IsInfested;
	
	private float 	m_WaterQuantity; // TO DO: Remove when you get to rewrite OnStoreSave and load!
	private float 	m_SprayQuantity;
	
	int 	m_DeleteDryPlantTime; 			// For how long in seconds can an unwatered plant exist before it disappears
	int 	m_SpoiledRemoveTime;			// For how long in seconds a spoiled plant will exist
	int 	m_FullMaturityTime;				// How much time needs plant to be full grown in seconds
	int 	m_SpoilAfterFullMaturityTime;	// How long in seconds it takes for plant to be spoiled after it is full grown
	int 	m_StateChangeTime;				// For how long in seconds will plant stay in one state before its going to next state
	
    autoptr Timer m_GrowthTimer = NULL;
    autoptr Timer m_InfestationTimer = NULL;
    autoptr Timer m_SpoilAfterFullMaturityTimer = NULL;
    autoptr Timer m_SpoiledRemoveTimer = NULL;
	autoptr Timer m_DeleteDryPlantTimer = NULL;
	
	private GardenBase m_GardenBase = NULL;
	private Slot m_Slot = NULL;
	
	private PluginHorticulture m_ModuleHorticulture;
	
	void EN5C_PlantBase()
	{
		m_ModuleHorticulture = GetPlugin( PluginHorticulture );
		
		m_WaterUsage = 180 + Math.RandomInt(0, 20);
		m_SprayUsage = 0.06;
		m_DeleteDryPlantTime = (60 * 10) + Math.RandomInt(0, 60 * 2);
		m_SpoiledRemoveTime = (60 * 20) + Math.RandomInt(0, 60 * 5);
		
		m_InfestationChance = 0.2;
		
		string plant_type = this.GetType();
		m_GrowthStagesCount = GetGame().ConfigGetInt( "cfgVehicles " + plant_type + " Horticulture GrowthStagesCount" );
		m_CropsCount = GetGame().ConfigGetInt( "cfgVehicles " + plant_type + " Horticulture CropsCount" );
		GetGame().ConfigGetText( "cfgVehicles " + plant_type + " Horticulture CropsType", m_CropsType );

		m_PlantStateIndex = -1;
		m_CurrentPlantMaterialQuantity = 0;
		
		m_IsInfested = false;
		
		m_SprayQuantity = 0.0;
		
		m_HasCrops = true;
	}

	void Init( GardenBase garden_base, float fertility, float harvesting_efficiency, float water )
	{
		m_GardenBase = garden_base;
		
		float divided = (float)((60 * 13) + Math.RandomInt(0, 60 * 4)) / fertility;
		m_FullMaturityTime = 10; //divided; // TO DO: use correct time when testing is done
		
		divided = (float)((60 * 30) + Math.RandomInt(0, 60 * 30)) * fertility;
		m_SpoilAfterFullMaturityTime = divided;

		divided = (float)((float)m_FullMaturityTime / ((float)m_GrowthStagesCount - 2.0));
		m_StateChangeTime = divided;
		m_StateChangeTime = 3; // TO DO: Temporal overwrite for faster testing

		float count = m_CropsCount * fertility * harvesting_efficiency;
		m_CropsCount = (int)Math.Ceil( count );

		m_PlantMaterialMultiplier = 0.1 * harvesting_efficiency;
		
		float rain_intensity = GetGame().GetRain();
		
		if ( rain_intensity > 0.0 )
		{
			CheckWater( NULL );
		}
		else
		{
			CheckWater( NULL );
			
			if ( NeedsWater() )
			{
				SetPlantState(STATE_DRY);
				
				m_DeleteDryPlantTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
				m_DeleteDryPlantTimer.Run( m_DeleteDryPlantTime, this, "DeleteDryPlantTick", NULL, false );
			}
		}
	}

	void OnStoreLoadCustom( ParamsReadContext ctx, GardenBase garden_base )
	{
		m_GardenBase = garden_base;
		
		autoptr Param1<int> param_int = new Param1<int>( 0 );
		ctx.Read( param_int );
		m_WaterUsage = param_int.param1;
		
		param_int.param1 = 0;
		ctx.Read( param_int );
		m_SprayUsage = param_int.param1;
		
		param_int.param1 = 0;
		ctx.Read( param_int );
		m_DeleteDryPlantTime = param_int.param1;
		
		param_int.param1 = 0;
		ctx.Read( param_int );
		m_SpoiledRemoveTime = param_int.param1;
		
		param_int.param1 = 0;
		ctx.Read( param_int );
		m_FullMaturityTime = param_int.param1;
		
		param_int.param1 = 0;
		ctx.Read( param_int );
		m_SpoilAfterFullMaturityTime = param_int.param1;
		
		param_int.param1 = 0;
		ctx.Read( param_int );
		m_StateChangeTime = param_int.param1;
		
		autoptr Param1<float> param_float = new Param1<float>( 0.0 );
		ctx.Read( param_float );
		m_InfestationChance = param_float.param1;
		
		param_int.param1 = 0;
		ctx.Read( param_int );
		m_GrowthStagesCount = param_int.param1;
		
		param_int.param1 = 0;
		ctx.Read( param_int );
		m_CropsCount = param_int.param1;
		
		autoptr Param1<string> param_string = new Param1<string>( "" );
		ctx.Read( param_string );
		m_CropsType = param_string.param1;
		
		param_float.param1 = 0.0;
		ctx.Read( param_float );
		m_PlantMaterialMultiplier = param_float.param1;
		
		param_int.param1 = 0;
		ctx.Read( param_int );
		m_PlantState = param_int.param1;
		
		param_int.param1 = 0;
		ctx.Read( param_int );
		m_PlantStateIndex = param_int.param1;
		
		param_float.param1 = 0.0;
		ctx.Read( param_float );
		m_CurrentPlantMaterialQuantity = param_float.param1;
		
		autoptr Param1<bool> param_bool = new Param1<bool>( false );
		ctx.Read( param_bool );
		m_IsInfested = param_bool.param1;
		
		param_float.param1 = 0.0;
		ctx.Read( param_float );
		m_WaterQuantity = param_float.param1; // TO DO: Remove when you get to rewrite OnStoreSave and load!
		 
		param_float.param1 = 0.0;
		ctx.Read( param_float );
		m_SprayQuantity = param_float.param1;
		
		
		
		param_bool.param1 = false;
		ctx.Read( param_bool );
		if ( param_bool.param1 )
		{
			m_GrowthTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
			m_GrowthTimer.Run( m_StateChangeTime, this, "GrowthTimerTick", NULL, true );
		}
		
		param_float.param1 = false;
		ctx.Read( param_float );
		if ( param_float.param1 > 0.0 )
		{
			m_InfestationTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
			m_InfestationTimer.Run( param_float.param1, this, "InfestationTimerTick", NULL, false );
		}
		
		param_float.param1 = false;
		ctx.Read( param_float );
		if ( param_float.param1 > 0.0 )
		{
			m_SpoilAfterFullMaturityTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
			m_SpoilAfterFullMaturityTimer.Run( param_float.param1, this, "SetSpoiled", NULL, false );
		}
		
		param_float.param1 = false;
		ctx.Read( param_float );
		if ( param_float.param1 > 0.0 )
		{
			m_SpoiledRemoveTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
			m_SpoiledRemoveTimer.Run( param_float.param1, this, "SpoiledRemoveTimerTick", NULL, false );
		}
		
		param_float.param1 = false;
		ctx.Read( param_float );
		if ( param_float.param1 > 0.0 )
		{
			m_DeleteDryPlantTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
			m_DeleteDryPlantTimer.Run( param_float.param1, this, "DeleteDryPlantTick", NULL, false );
		}
		
		UpdatePlant();
	}

	void OnStoreSaveCustom( ParamsWriteContext ctx )
	{
		autoptr Param1<int> param_int = new Param1<int>( m_WaterUsage );
		ctx.Write( param_int );
		
		param_int.param1 = m_SprayUsage;
		ctx.Write( param_int );
		
		param_int.param1 = m_DeleteDryPlantTime;
		ctx.Write( param_int );
		
		param_int.param1 = m_SpoiledRemoveTime;
		ctx.Write( param_int );
		
		param_int.param1 = m_FullMaturityTime;
		ctx.Write( param_int );
		
		param_int.param1 = m_SpoilAfterFullMaturityTime;
		ctx.Write( param_int );
		
		param_int.param1 = m_StateChangeTime;
		ctx.Write( param_int );
		
		autoptr Param1<float> param_float = new Param1<float>( m_InfestationChance );
		ctx.Write( param_float );
		
		param_int.param1 = m_GrowthStagesCount;
		ctx.Write( param_int );
		
		param_int.param1 = m_CropsCount;
		ctx.Write( param_int );
		
		autoptr Param1<string> param_string = new Param1<string>( m_CropsType );
		ctx.Write( param_string );
		
		param_float.param1 = m_PlantMaterialMultiplier;
		ctx.Write( param_float );
		
		param_int.param1 = m_PlantState;
		ctx.Write( param_int );
		
		param_int.param1 = m_PlantStateIndex;
		ctx.Write( param_int );
		
		param_float.param1 = m_CurrentPlantMaterialQuantity;
		ctx.Write( param_float );
		
		autoptr Param1<bool> param_bool = new Param1<bool>( m_IsInfested );
		ctx.Write( param_bool );
		
		param_float.param1 = m_WaterQuantity; // TO DO: Remove when you get to rewrite OnStoreSave and load!
		ctx.Write( param_float );
		 
		param_float.param1 = m_SprayQuantity;
		ctx.Write( param_float );
		
		param_bool.param1 = false;
		if ( m_GrowthTimer != NULL )
		{
			param_bool.param1 = true;
		}
		ctx.Write( param_bool );
		
		param_float.param1 = 0.0;
		if ( m_InfestationTimer != NULL )
		{
			param_float.param1 = m_InfestationTimer.GetRemaining();
		}
		ctx.Write( param_float );
		
		param_float.param1 = 0.0;
		if ( m_SpoilAfterFullMaturityTimer != NULL )
		{
			param_float.param1 = m_SpoilAfterFullMaturityTimer.GetRemaining();
		}
		ctx.Write( param_float );
		
		param_float.param1 = 0.0;
		if ( m_SpoiledRemoveTimer != NULL )
		{
			param_float.param1 = m_SpoiledRemoveTimer.GetRemaining();
		}
		ctx.Write( param_float );
		
		param_float.param1 = 0.0;
		if ( m_DeleteDryPlantTimer != NULL )
		{
			param_float.param1 = m_DeleteDryPlantTimer.GetRemaining();
		}
		ctx.Write( param_float );
	}

	bool ConditionIntoInventory( EntityAI player )
	{
		return false;
	}

	bool ConditionIntoHands( EntityAI player )
	{
		return false;
	}

	bool ConditionOutOfHands( EntityAI player )
	{
		return false;
	}

	void ChangeInfestation( bool is_infested )
	{
		m_IsInfested = is_infested;
		
		string plant_type = this.GetType();
		PlantMaterial material = m_ModuleHorticulture.GetPlantMaterial( plant_type );
		
		if ( m_IsInfested )
		{
			if ( material.m_InfestedTex != "" )
			{
				this.SetObjectTexture( 0, material.m_InfestedTex );
			}
			if ( material.m_InfestedMat != "" )
			{
				this.SetObjectMaterial( 0, material.m_InfestedMat );
			}
		}
		else 
		{
			if ( material.m_HealthyTex != "" )
			{
				this.SetObjectTexture( 0, material.m_HealthyTex );
			}
			if ( material.m_HealthyMat != "" )
			{
				this.SetObjectMaterial( 0, material.m_HealthyMat );
			}
		}
	}

	void UpdatePlant()
	{
		if ( m_PlantStateIndex > 0 )
		{
			string plant_state_index = m_PlantStateIndex.ToStringLen(2); 
			string prev_plant_state_index = ( m_PlantStateIndex - 1 ).ToStringLen( 2 );
			
			// HIDING PREVIOUS PLANT STATE AND SHOWING THE CURRENT ONE
			this.ShowSelection( "plantStage_" + plant_state_index ); // SHOW!
			this.HideSelection( "plantStage_" + prev_plant_state_index ); // HIDE!
			
			// HIDING PREVIOUS CROPS STATE AND SHOWING THE CURRENT ONE
			Print(HasCrops());
			if ( this.HasCrops() )
			{
				this.ShowSelection( "plantStage_" + plant_state_index + "_crops" ); // SHOW!
				this.HideSelection( "plantStage_" + prev_plant_state_index + "_crops" ); // HIDE!
			}
			else
			{
				this.HideSelection( "plantStage_" + plant_state_index + "_crops" ); // HIDE!
				this.HideSelection( "plantStage_" + prev_plant_state_index + "_crops" ); // HIDE!
			}

			// HIDING PREVIOUS SHADOW STATE AND SHOWING THE CURRENT ONE
			this.ShowSelection( "plantStage_" + plant_state_index + "_shadow" ); // SHOW!
			this.HideSelection( "plantStage_" + prev_plant_state_index + "_shadow" ); // HIDE!
		}
		
		float float_plant_state_index = (float)m_PlantStateIndex;
		m_CurrentPlantMaterialQuantity = m_PlantMaterialMultiplier * float_plant_state_index;
	}

	void GrowthTimerTick()
	{
		if ( IsGrowing() )
		{
			if ( m_PlantStateIndex < m_GrowthStagesCount - 2 )
			{
				m_PlantStateIndex++;
				UpdatePlant();
					
				if ( m_PlantStateIndex == 0 )
				{
					float infestation_time_min = (float)m_FullMaturityTime * 0.2;
					int int_infestation_time_min = (int)infestation_time_min;
						
					float infestation_time_max = (float)m_FullMaturityTime * 0.6;
					int int_infestation_time_max = (int)infestation_time_max;
						
					m_InfestationTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
					m_InfestationTimer.Run( Math.RandomInt(int_infestation_time_min, int_infestation_time_max), this, "InfestationTimerTick", NULL, false );
				}
					
				if ( m_PlantStateIndex == m_GrowthStagesCount - 2 )
				{
					if ( m_IsInfested )
					{
						SetSpoiled();
					}
					else
					{
						SetPlantState(STATE_MATURE);
					}
				}
			}
		}
		else if ( IsMature() )
		{
			// BUG: This secion gets executed every m_StateChangeTime seconds which creates additional 'm_SpoilAfterFullMaturityTimer' timers!
			m_SpoilAfterFullMaturityTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
			m_SpoilAfterFullMaturityTimer.Run( m_SpoilAfterFullMaturityTime, this, "SetSpoiled", NULL, false );
		}
	}

	void InfestationTimerTick()
	{
		float infestation_rnd = Math.RandomFloat01();
		
		if ( m_InfestationChance > infestation_rnd )
		{
			ChangeInfestation( true );
		}
	}

	void SpoiledRemoveTimerTick()
	{
		if ( m_GrowthTimer != NULL )
		{
			delete m_GrowthTimer;
			m_GrowthTimer = NULL;
		}
		
		//RemoveSlot();
	}
	
	void DeleteDryPlantTick()
	{
		/*if ( IsDry() )
		{
			RemoveSlot();
		}*/
	}

	void SetSpoiled()
	{
		if ( IsSpoiled() == false )
		{
			m_PlantStateIndex++;
			SetPlantState(STATE_SPOILED);
			
			UpdatePlant();
			
			m_SpoiledRemoveTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
			m_SpoiledRemoveTimer.Run( m_SpoiledRemoveTime, this, "SpoiledRemoveTimerTick", NULL, false );
		}
	}

	string CheckWater( ItemBase item )
	{
		if ( !NeedsWater() )
		{
			if ( m_DeleteDryPlantTimer )
			{
				delete m_DeleteDryPlantTimer;
				m_DeleteDryPlantTimer = NULL;
			}
			
			if (item)
			{
				string liquid_type = item.GetVariableString("LiquidType");
				liquid_type.ToLower();
				liquid_type.Replace( "\"", "" );

				// Liquid type check. Disabled due to DAYZ-12143
				/*if !( _bottle getVariable "LiquidType" == "Water" ) exitWith {
					[_user, format["The %1 is not filled with water.", displayName _bottle], "colorImportant"] call fnc_playerMessage;};*/
				if ( liquid_type != "water" ) 
				{
					string item_display_name = "";
					GetGame().ObjectGetDisplayName( item, item_display_name );
					
					RemoveSlot();
					
					return "The " + item_display_name + " contained some liquid which poisoned the plant.";
				}
			}
			
			SetPlantState(STATE_GROWING);
			
			m_GrowthTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
			m_GrowthTimer.Run( m_StateChangeTime, this, "GrowthTimerTick", NULL, true );
		
			return "I've watered the plant. Now it has enough of water to grow.";
		}
		else 
		{
			return "I've watered the plant a bit.";
		}
	}

	string StopInfestation( PlayerBase player, ItemBase item, float consumed_quantity )
	{
		m_SprayQuantity += consumed_quantity;
		
		if ( NeedsSpraying() == false )
		{
			if ( m_InfestationTimer != NULL )
			{
				delete m_InfestationTimer;
				m_InfestationTimer = NULL;
			}
			
			m_IsInfested = false;
			m_InfestationChance = 0;
			
			ChangeInfestation( false );
			UpdatePlant();
			
			return "I've sprayed the plant a bit. Now it is enough spayed.";
		}
		else
		{
			return "I've sprayed the plant a bit.";
		}
	}

	string Remove( PlayerBase player = NULL )
	{
		if ( m_CurrentPlantMaterialQuantity > 0.0  &&  player )
		{
			vector pos = GetPosition();
			ItemBase item = GetGame().CreateObject( "EN5C_PlantMaterial", pos );
			item.SetQuantity2( m_CurrentPlantMaterialQuantity * 1000.0 );
		}
		
		RemoveSlot();
		
		GetGame().ObjectDelete( this );
		
		return "I've removed the plant.";
	}

	string Harvest( PlayerBase player )
	{
		PluginExperience module_exp = GetPlugin(PluginExperience);
		float harvesting_efficiency = module_exp.GetExpParamNumber(player, PluginExperience.EXP_FARMER_HARVESTING, "efficiency");
		
		m_CropsCount = m_CropsCount * harvesting_efficiency;
		
		for ( int i = 0; i < m_CropsCount; i++ )
		{
			vector pos = player.GetPosition();
			ItemBase item = GetGame().CreateObject( m_CropsType, pos );
			item.SetQuantity2( 1.0 );
			
			if ( IsSpoiled() )
			{
				FoodManager.ChangeFoodStage( item, "Rotten" );
			}
		}
		
		m_HasCrops = false;
		
		UpdatePlant();
		
		return "I've harvested the crops.";
	}
	
	void SetPlantState(int state)
	{
		m_PlantState = state;
	}
	
	int GetPlantState()
	{
		return m_PlantState;
	}
	
	float GetWater()
	{
		if ( GetSlot() )
			return GetSlot().GetWater();
	}
	
	float GetWaterMax()
	{
		if ( GetSlot() )
			return GetSlot().GetWaterUsage();
	}
	
	bool NeedsWater()
	{
		if ( IsDry()  &&  GetSlot().GetWater() < GetSlot().GetWaterUsage() )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool NeedsSpraying()
	{
		if ( m_SprayQuantity < m_SprayUsage )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void RemoveSlot()
	{
		if ( m_GardenBase )
		{
			m_GardenBase.RemoveSlotPlant( this );
		}
	}
	
	void SetSlot(Slot slot)
	{
		if ( slot )
		{
			m_Slot = slot;
		}
	}
	
	Slot GetSlot()
	{
		return m_Slot;
	}

	bool IsDry()
	{
		if ( GetPlantState() == STATE_DRY )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool IsGrowing()
	{
		if ( GetPlantState() == STATE_GROWING )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool IsMature()
	{
		if ( GetPlantState() == STATE_MATURE )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool IsSpoiled()
	{
		if ( GetPlantState() == STATE_SPOILED )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	bool HasCrops()
	{
		return m_HasCrops;
	}
}

// TODO remove PlantBase and DiggedSoil classes below when SQF horticulture is removed from game
class PlantBase extends ItemBase
{
	bool ConditionIntoInventory( EntityAI player )
	{
		return false;
	}

	bool ConditionIntoHands( EntityAI player )
	{
		return false;
	}

	bool ConditionOutOfHands( EntityAI player )
	{
		return false;
	}
};

class DiggedSoil extends ItemBase
{
	bool ConditionIntoInventory( EntityAI player )
	{
		return false;
	}

	bool ConditionIntoHands( EntityAI player )
	{
		return false;
	}

	bool ConditionOutOfHands( EntityAI player )
	{
		return false;
	}
};