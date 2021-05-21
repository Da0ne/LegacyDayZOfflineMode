// This script is going through rewrites. Contact Boris Vacula if you want to consult something.

class GardenBase extends ItemBase
{
	// Paths to slot textures. Slots can have multiple states, so multiple textures must be generated
	static const string SLOT_TEXTURE_DIGGED_WET_LIME		= "dz\\plants2\\farming\\data\\soil_digged_wet_lime_CO.paa";
	static const string SLOT_TEXTURE_DIGGED_WET_PLANT		= "dz\\plants2\\farming\\data\\soil_digged_wet_plant_CO.paa";
	
	// slot names
	private static const string SLOT_SELECTION_DIGGED_PREFIX 	= "slotDigged_";
	private static const string SLOT_SELECTION_COVERED_PREFIX 	= "slotCovered_";
	private static const string SLOT_COMPONENT_PREFIX 			= "Component";
	private static const string SLOT_MEMORY_POINT_PREFIX 		= "slot_";
	
	
	private static const int 	CHECK_RAIN_TIME 				= 5;
	private static const int 	FIRST_SLOT_COMPONENT_INDEX 		= 2; // First slot component in p3d has index 2
	
	protected autoptr array<Slot> m_Slots;
	protected float m_BaseFertility;
	autoptr Timer m_DeleteWithDelayTimer; // Hack: This timer deletes attachments with a small delay. Attachments can't be deleted without it.
	autoptr Timer m_CheckRainTimer; // When rain starts, all slots must become watered.
	
	private static autoptr map<string,string> m_map_slots; // For the 'attachment slot -> plant slot' conversion. It is possible that this will be removed later.
	
	void GardenBase()
	{
		m_map_slots = new map<string,string>;
		m_DeleteWithDelayTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
		
		// Prepare m_map_slots
		for (int i = 1;  i <= GetGardenSlotsCount() + 1;  ++i)
		{
			// m_map_slots is supposed to be: <input, output>
			string input = "seedbase_" + i.ToString();
			string output = SLOT_COMPONENT_PREFIX;
			
			if (i < 10)
				output = output + "0"; // Example: '1' changes to '01'
			
			int i_add = i + 1; // + 1 because input like 'seedbase_5' must output 'component06' due to how p3d files work.
			output = output + i_add.ToString();
			
			m_map_slots.Set(input, output);
		}
		
		m_CheckRainTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
		m_CheckRainTimer.Run( CHECK_RAIN_TIME, this, "CheckRainTick", NULL, true );
	}

	void ~GardenBase()
	{
		int slots_count = GetGardenSlotsCount();
		for ( int i = 0; i < slots_count; i++ )
		{
			delete m_Slots.Get( i );
		}
		
		// Remove Plant objects from SQF horticulture 
		// TODO remove this for cycle when SQF horticulture is removed from game
		for ( i = 0; i < slots_count; i++ )
		{
			Object obj;
			if ( GetVariable("slotObj" + i.ToString(), obj) )
			{
				if ( obj != NULL  &&  GetGame() )
				{
					GetGame().ObjectDelete( obj );
				}
			}
		}
	}

	void InitializeSlots()
	{
		m_Slots = new array<Slot>;
		int slots_count = GetGardenSlotsCount();
		
		for ( int i = 0; i < slots_count; i++ )
		{
			Slot slot = new Slot(m_BaseFertility);
			slot.SetSlotID(i);
			slot.SetGarden(this);
			m_Slots.Insert( slot );
		}
	}
	
	void OnStoreLoad( ParamsReadContext ctx )
	{
		super.OnStoreLoad( ctx );
		
		autoptr Param1<float> param_float = new Param1<float>( 0.0 );
		ctx.Read( param_float );
		m_BaseFertility = param_float.param1;
		int slots_count = GetGardenSlotsCount();
		
		for ( int i = 0; i < slots_count; i++ )
		{
			Slot slot = m_Slots.Get( i );
			
			slot.OnStoreLoadCustom( ctx );
			UpdateSlotTexture( i );
			
			autoptr Param1<string> param_string = new Param1<string>( "" );
			ctx.Read( param_string );
			
			if ( param_string.param1 != "" )
			{
				EN5C_PlantBase plant = GetGame().CreateObject( param_string.param1, GetSlotPosition(i) );
				slot.SetPlant( plant );
				plant.OnStoreLoadCustom( ctx, this );
			}
		}
	}

	void OnStoreSave( ParamsWriteContext ctx )
	{
		super.OnStoreSave( ctx );
		
		autoptr Param1<float> param_float = new Param1<float>( m_BaseFertility );
		ctx.Write( param_float );
		
		int slots_count = GetGardenSlotsCount();
		for ( int i = 0; i < slots_count; i++ )
		{
			Slot slot = m_Slots.Get( i );
			
			slot.OnStoreSaveCustom( ctx );
			
			autoptr Param1<string> param_string = new Param1<string>( "" );
			if ( slot.GetPlant() )
			{
				param_string.param1 = slot.GetPlant().GetType();
				ctx.Write( param_string );
				EN5C_PlantBase plant = slot.GetPlant();
				plant.OnStoreSaveCustom( ctx );
			}
			else
			{
				param_string.param1 = "";
				ctx.Write( param_string );
			}
		}
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

	int GetGardenSlotsCount()
	{
		return 0;
	}
	
	// Digs the given slot. Parameters player and item are optional. Returns result message.
	string DigSlot( PlayerBase player, ItemBase item, int slot_index )
	{
		if (item) // Item is not always provided
			item.AddDamage( 0.005 );
		
		Slot slot = m_Slots.Get( slot_index );
		slot.m_State = Slot.STATE_DIGGED;	
		
		UpdateSlotTexture( slot_index );
		
		if (player)
		{
			PluginHorticulture module_horticulture = GetPlugin( PluginHorticulture );
			
			if ( module_horticulture.GiveWormsToPlayer(player, 0.1) )
			{
				return "I've prepared the slot and found a worm in the ground.";
			}
			else
			{
				return "I've prepared the slot.";
			}
		}
		
		return "";
	}

	bool CanPlantSeed( string selection_component )
	{
		Slot slot = GetSlotBySelection( selection_component );
		
		if ( slot != NULL && slot.m_State == Slot.STATE_DIGGED )
		{
			return  true;
		}
		else
		{
			return false;
		}
	}
	
	// Converts attachment slot name into plant slot name. Example: 'seedbase_1' -> 'component02'
	string ConvertAttSlotToPlantSlot(string attach_slot)
	{
		// Give result
		if ( m_map_slots.Contains(attach_slot) )
		{
			string return_value = m_map_slots.Get(attach_slot);
			return return_value;
		}
		
		return "";
	}
	
	void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);
		
		if ( !g_Game.IsServer() ) return;
		
		string path = "CfgVehicles " + item.GetType() + " Horticulture" + " PlantType";
		bool IsItemSeed = GetGame().ConfigIsExisting(path); // Is this item a seed?
		
		if ( IsItemSeed ) 
		{
			string converted_slot_name;
			Print(slot_name); // TO DO: (BLOKER) Until new system for attachment slots is implemented, slot_name is incorrect!
			vector pos = GetPosition();
			int index = GetNearestSlotIDByState( pos , Slot.STATE_DIGGED ) + 2; // +2 because selections are named differently than 'garden slot' IDs and 'attachment slots' IDs.
			
			if (index < 10)
			{
				converted_slot_name = SLOT_COMPONENT_PREFIX + "0" + index.ToString();
			}
			else
			{
				converted_slot_name = SLOT_COMPONENT_PREFIX + index.ToString();
			}
			
			Print(index);
			Print(converted_slot_name);
			
			PlantSeed( NULL, item, converted_slot_name);
		}
	}
	
	void EEItemDetached(EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);
		
		if ( !g_Game.IsServer() ) return;
		
		string path = "CfgVehicles " + item.GetType() + " Horticulture " + "PlantType";
		bool IsItemSeed = GetGame().ConfigIsExisting(path); // Is this item a seed?
		
		if ( IsItemSeed ) 
		{
			Print(item);
			Print(slot_name); // TO DO: (BLOKER) Until new system for attachment slots is implemented, slot_name is incorrect!
			string converted_slot_name = ConvertAttSlotToPlantSlot(slot_name);
			Print(converted_slot_name);
			Slot slot = GetSlotBySelection(converted_slot_name);
			Print(slot);
			if (slot)
			{
				// TO DO: Manipuluje sa tu so zlym slotom! Ma sa resetnut slot 4, ale v skutocnosti sa resetne slot 0!
				Print(slot.m_State);
				slot.m_State = Slot.STATE_DIGGED;
				Print(slot.m_State);
				slot.SetSeed(NULL);
			}
		}
	}
	
	// Sets all slots to be digged and ready for planting
	void DigAllSlots()
	{
		int slots_count = GetGardenSlotsCount();
		
		for ( int i = 0; i < slots_count; i++ )
		{
			DigSlot( NULL, NULL, i );
		}
	}
	
	// Plants the seed (item) into slot (selection_component). Returns message string depending on the result.
	string PlantSeed( PlayerBase player, ItemBase item, string selection_component )
	{
		int slot_index = GetSlotIndexBySelection( selection_component );
		
		if ( slot_index >= 0 )
		{
			PluginHorticulture module_horticulture = GetPlugin( PluginHorticulture );
			string plant_type = module_horticulture.GetPlantType( item );
			
			Print(slot_index);
			Slot slot = m_Slots.Get( slot_index );
			slot.m_State = Slot.STATE_PLANTED;
			slot.m_PlantType = plant_type;
			slot.SetSeed(item);
			
			slot.m_DiggedSlotComponent = selection_component;
			string message = "";
			
			if ( NeedsFertilization(selection_component) == false )
			{
				message = "I've planted a seed into the fertilized soil.";
			}
			else
			{
				message = "I've planted a seed.";
			}
			
			float rain_intensity = GetGame().GetRain();
			if ( rain_intensity < 0.5 )
			{
				message += " However the soil is too dry for the plant to grow.";
			}
		}
		
		return message;
	}
	
	// Creates a plant
	// TO DO: Try to remove parameter slot_index and use .GetSlotID() on the slot.
	void CreatePlant(Slot slot, int slot_index )
	{
		vector pos = GetSlotPosition(slot_index);
		EN5C_PlantBase plant = GetGame().CreateObject( slot.m_PlantType, pos );
		slot.SetPlant(plant);
		slot.m_State = Slot.STATE_COVERED;
		plant.Init( this, slot.m_Fertility, slot.m_HarvestingEfficiency, slot.GetWater() );
		ShowSelection(SLOT_SELECTION_COVERED_PREFIX + (slot_index + 1).ToStringLen(2));
		
		ItemBase seed = slot.GetSeed();
		RemoveAttachment(seed); 
		//this.TakeEntityAsAttachment(plant); // TO DO: Uncomment this when plant proxies are fixed
		
		Param1<ItemBase> param_seed = new Param1<ItemBase>(seed);
		m_DeleteWithDelayTimer.Run(0.1, this, "DeleteWithDelay", param_seed, false);
	}
	
	void DeleteWithDelay(ItemBase param_seed)
	{
		if (this)
		{
			ItemBase seed = param_seed;
			Print("Deleting seed...");
			Print(seed);
			if (seed)
			{
				GetGame().ObjectDelete( seed );
				Print("Seed deleted!");
			}
		}
	}
	
	// TO DO: Remove! And the user action as well!
	string FertilizeBit( PlayerBase player, ItemBase item, string selection_component )
	{
		string item_type = item.GetType();
		float consumed_quantity = GetGame().ConfigGetFloat( "cfgVehicles " + item_type + " Horticulture ConsumedQuantity" ) / 3.0;
		return Fertilize( player, item, consumed_quantity, selection_component );
	}
		
	string Fertilize( PlayerBase player, ItemBase item, float consumed_quantity, string selection_component )
	{
		Slot slot = GetSlotBySelection( selection_component );
		
		if ( slot != NULL )
		{
			string item_type = item.GetType();
			
			if ( slot.m_FertilizerType == ""  ||  slot.m_FertilizerType == item_type )
			{
				slot.m_FertilizerType = item_type;
				
				float add_energy_to_slot = GetGame().ConfigGetFloat( "cfgVehicles " + item_type + " Horticulture AddEnergyToSlot" );
				slot.m_FertilizerUsage = GetGame().ConfigGetFloat( "cfgVehicles " + item_type + " Horticulture ConsumedQuantity" );
				
				float coef = Math.Clamp( consumed_quantity / slot.m_FertilizerUsage, 0.0, 1.0 );
				add_energy_to_slot = coef * add_energy_to_slot;
				
				slot.m_FertilizerQuantity += consumed_quantity;
				slot.m_Fertility += add_energy_to_slot;
				
				item.SetQuantity2( item.GetQuantity2() );
				
				if ( NeedsFertilization(selection_component) == false )
				{
					string selection = selection_component;
					selection.Replace( SLOT_COMPONENT_PREFIX, SLOT_SELECTION_DIGGED_PREFIX );
					int slot_index = GetSlotIndexBySelection(selection);
					
					UpdateSlotTexture( slot_index );
					
					PluginExperience module_exp = GetPlugin(PluginExperience);
					slot.m_HarvestingEfficiency = module_exp.GetExpParamNumber(player, PluginExperience.EXP_FARMER_FERTILIZATION, "efficiency");
						
					return "I've fertilized the ground. Now it have enough of fertilizer.";
				}
				else
				{
					return "I've fertilized the ground a bit.";
				}
			}
		}
		
		return "This slot is fertilized with another fertilizer already.";
	}

	bool IsCorrectFertilizer( ItemBase item, string selection_component )
	{
		Slot slot = GetSlotBySelection( selection_component );
		
		if ( slot != NULL )
		{
			string item_type = item.GetType();
			
			if ( slot.m_FertilizerType == "" || slot.m_FertilizerType == item_type )
			{
				return true;
			}
		}
		
		return false;
	}

	bool NeedsFertilization( string selection_component )
	{
		Slot slot = GetSlotBySelection( selection_component );
		
		if ( slot  &&  !slot.IsCovered() )
		{
			if ( slot.m_FertilizerType == ""  ||  slot.m_FertilizerQuantity < slot.m_FertilizerUsage )
			{
				return true;
			}
		}
		
		return false;
	}

	TStringArray GetHiddenSelectionsTextures()
	{
		string garden_type = this.GetType();
		TStringArray textures = new TStringArray;
		GetGame().ConfigGetTextArray( "CfgVehicles " + garden_type + " hiddenSelectionsTextures", textures );
		
		return textures;
	}

	void UpdateSlotTexture( int slot_index )
	{
		Slot slot = m_Slots.Get( slot_index );
		
		// Show / Hide selections according to DIGGED or COVERED states.
		
		if ( slot.IsDigged()  ||  slot.IsPlanted() )
		{
			HideSelection( SLOT_SELECTION_COVERED_PREFIX + (slot_index + 1).ToStringLen(2) );
			ShowSelection( SLOT_SELECTION_DIGGED_PREFIX + (slot_index + 1).ToStringLen(2) );
		}
		else if ( slot.IsCovered() )
		{
			HideSelection( SLOT_SELECTION_COVERED_PREFIX + (slot_index + 1).ToStringLen(2) );
			ShowSelection( SLOT_SELECTION_DIGGED_PREFIX + (slot_index + 1).ToStringLen(2) );
		}
		
		if ( slot.m_State != Slot.STATE_NOT_DIGGED ) 
		{
			if ( slot.m_FertilizerType != "" )
			{
				SetSlotTextureFertilized( slot_index, slot.m_FertilizerType );
			}
			else 
			{
				SetSlotTextureDigged( slot_index );
			}
		}
	}

	void SetSlotTextureDigged( int slot_index )
	{
		TStringArray textures = GetHiddenSelectionsTextures();
		
		ShowSelection( SLOT_SELECTION_DIGGED_PREFIX + (slot_index + 1).ToStringLen(2) );
		SetObjectTexture( slot_index, textures.Get(0) );
		
		Slot slot = m_Slots.Get( slot_index );
		
		if ( slot.NeedsWater() )
		{
			// Set dry material
			this.SetObjectMaterial ( slot_index, "dz\\plants2\\farming\\data\\soil_cultivated.rvmat" ); // TO DO: Save into constant
		}
		else
		{
			// Set wet material
			this.SetObjectMaterial ( slot_index, "dz\\plants2\\farming\\data\\soil_cultivated_wet.rvmat" ); // TO DO: Save into constant
		}
		
		delete textures;
	}

	void SetSlotTextureFertilized( int slot_index, string item_type )
	{
		TStringArray textures = GetHiddenSelectionsTextures();
		
		int tex_id = GetGame().ConfigGetInt( "cfgVehicles " + item_type + " Horticulture TexId" );
		ShowSelection( SLOT_SELECTION_DIGGED_PREFIX + (slot_index + 1).ToStringLen(2) );
		SetObjectTexture( slot_index, textures.Get(tex_id) );
		
		Slot slot = m_Slots.Get( slot_index );
		
		int slot_index_offset = 0;
		
		if ( slot.IsCovered() )
		{
			slot_index_offset = slot_index_offset + GetGardenSlotsCount();
		}
		
		// Set material according to dry / wet states
		if ( slot.NeedsWater() )
		{
			// Set dry material for garden lime
			if ( slot.m_FertilizerType == "EN5C_GardenLime" )
			{
				this.SetObjectMaterial ( slot_index + slot_index_offset, "dz\\plants2\\farming\\data\\soil_cultivated_limed.rvmat" ); // TO DO: Save into constant
			}
			else if ( slot.m_FertilizerType == "EN5C_PlantMaterial" )
			{
				this.SetObjectMaterial ( slot_index + slot_index_offset, "dz\\plants2\\farming\\data\\soil_cultivated_compost.rvmat" ); // TO DO: Save into constant
			}
		}
		else
		{
			// Set dry material for compost
			if ( slot.m_FertilizerType == "EN5C_GardenLime" )
			{
				this.SetObjectMaterial ( slot_index + slot_index_offset, "dz\\plants2\\farming\\data\\soil_cultivated_limed_wet.rvmat" ); // TO DO: Save into constant
			}
			else if ( slot.m_FertilizerType == "EN5C_PlantMaterial" )
			{
				this.SetObjectMaterial ( slot_index + slot_index_offset, "dz\\plants2\\farming\\data\\soil_cultivated_compost_wet.rvmat" ); // TO DO: Save into constant
			}
		}
		
		delete textures;
	}

	void RemoveSlot( int index )
	{
		if ( m_Slots != NULL )
		{	
			Slot slot = m_Slots.Get( index );
			
			if ( slot.GetPlant() )
			{
				GetGame().ObjectDelete( slot.GetPlant() );
			}
			
			slot.Init( m_BaseFertility );
			slot.GiveWater( NULL, -9999 );
			
			HideSelection( SLOT_SELECTION_COVERED_PREFIX + (index + 1).ToStringLen(2) );
			//this.HideSelection( SLOT_SELECTION_DIGGED_PREFIX + (slot_index + 1).ToStringLen(2), 1 );
		}
	}

	void RemoveSlotPlant( Object plant )
	{
		int index = GetSlotIndexByPlant( plant );
		if ( index >= 0 )
		{
			RemoveSlot( index );
		}
	}

	Slot GetSlotBySelection( string selection_component )
	{
		int slot_index = GetSlotIndexBySelection( selection_component );
		if ( slot_index >= 0 )
		{
			return m_Slots.Get( slot_index );
		}
		else
		{
			return NULL;
		}
	}

	int GetSlotIndexBySelection( string selection_component )
	{
		int slot_index = -1;
		
		if ( m_Slots != NULL )
		{
			string selection_component_lower = selection_component; 
			selection_component_lower.ToLower();
			
			string slot_component_prefix_lower = SLOT_COMPONENT_PREFIX;
			slot_component_prefix_lower.ToLower();
			
			int start = selection_component_lower.IndexOf( slot_component_prefix_lower );
			if ( start > -1 )
			{
				start += SLOT_COMPONENT_PREFIX.Length();
				int end = start + 2;
				
				if ( selection_component.Length() >= end )
				{
					string num_str = selection_component.Substring( start, 2 );
					slot_index = num_str.ToInt();
				}
			}
		}
		
		if ( slot_index >= FIRST_SLOT_COMPONENT_INDEX )
		{
			slot_index -= FIRST_SLOT_COMPONENT_INDEX;
		}
		else
		{
			slot_index = -1;
		}
		
		return slot_index;
	}

	int GetSlotIndexByPlant( Object plant )
	{
		if ( m_Slots != NULL )
		{
			for ( int i = 0; i < m_Slots.Count(); i++ )
			{
				if ( m_Slots.Get(i).GetPlant() == plant )
				{
					return i;
				}
			}
		}
		
		return -1;
	}

	
	// TO DO: Remove! Player shouldn't dig slots anymore!
	int GetFreeSlotIndex( PlayerBase player )
	{
		vector from = player.GetCameraPosition();
		vector to = from + (player.GetCameraDirection() * 5);
		vector contact_pos;
		vector contact_dir;
		int contact_component;
		if ( GetGame().Raycast(from, to, contact_pos, contact_dir, contact_component) )   
		{
			int slot_index = GetNearestSlotIDByState( contact_pos , Slot.STATE_NOT_DIGGED );
			
			if ( slot_index > -1 )
			{
				return slot_index;
			}
		}
		
		return -1;
	}
	
	int GetNearestSlotIDByState( vector position, int slot_state)
	{
		float nearest_distance = 1000.0;
		int nearest_slot_index = -1;
		Print(slot_state);
		int slots_count = GetGardenSlotsCount();
		for ( int i = 0; i < slots_count; i++ )
		{
			Print(i);
			Slot slot = m_Slots.Get(i); // Move this line by a scope higher in this function after debugging
			
			if (slot)
			{
				Print(slot.m_State);
			}
			
			vector slot_pos = GetSlotPosition( i );
			float current_distance = vector.Distance( position, slot_pos );
			
			if ( current_distance < nearest_distance )
			{
				
				if ( slot != NULL && slot.m_State == slot_state )
				{
					nearest_distance = current_distance;
					nearest_slot_index = i;
				}
			}
		}
		
		return nearest_slot_index;
	}

	vector GetSlotPosition( int index )
	{
		string memory_point = SLOT_MEMORY_POINT_PREFIX + (index + 1).ToStringLen(2);
		vector pos = this.GetSelectionPosition( memory_point );
		
		return this.ModelToWorld( pos );
	}
	
	void CheckRainTick()
	{
		if (this)
		{
			float rain_intensity = GetGame().GetRain();
			
			if ( rain_intensity > 0 )
			{
				int slots_count = GetGardenSlotsCount();
				
				for ( int i = 0; i < slots_count; i++ )
				{
					Slot slot = m_Slots.Get( i );
					slot.GiveWater( NULL, slot.GetWaterUsage() );
				}
			}
		}
	}
}