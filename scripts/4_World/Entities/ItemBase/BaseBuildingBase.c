//BASE BUILDING BASE
class BaseBuildingBase extends ItemBase
{
	//==============================================================================
	// CONSTRUCTING
	
	static const string ANIMATION_BARBED_WIRE 				= "BarbWire";
	static const string ANIMATION_CAMO_NET 					= "Camonet";
	static const string ANIMATION_LIGHTS 					= "xlights";
	
	
	autoptr map<int, ConstructionPart> 	m_ConstructionParts;
	autoptr array<string> 				m_ConstructionTools;
	
	autoptr array<string> 				m_DeconstructionTools;
	
	// constructing ^
	//==============================================================================
	
	//==============================================================================
	// ELECTRIC FENCES
	
	static const string BARBEDWIRE_SLOT_LEFT				= "barbedwireleft";
	static const string BARBEDWIRE_SLOT_RIGHT				= "barbedwireright";
	static const string BARBEDWIRE_SLOT_FRONT				= "barbedwirefront";
	static const string BARBEDWIRE_SLOTS[] = {BARBEDWIRE_SLOT_FRONT, BARBEDWIRE_SLOT_LEFT, BARBEDWIRE_SLOT_RIGHT};
	
	// Positions of damage triggers are local to the model. "0 0 0" means undefined and it will be ignored, therefore overwrite these in your item's constructor if you want to use them!
	const int 						DMG_TRIGGERS_COUNT						= 3;
	vector 							m_DmgTrgLocalPos[DMG_TRIGGERS_COUNT]	= {"0 0 0", "0 0 0", "0 0 0"}; // Order must be same with BARBEDWIRE_SLOTS!
	int								m_DmgTrgLocalDir[DMG_TRIGGERS_COUNT]	= {0,0,0}; // Direction of each trigger local to the model in degrees
	
	autoptr map<string,EN5C_BarbedWire>	m_BarbedWires;
	autoptr array<BarbedWireTrigger> 		m_DmgTriggers;
	
	// electric fences ^
	//==============================================================================
	
	
	// Constructor
	void BaseBuildingBase() 
	{
		SetFlags(EntityFlags.TRIGGER, false);
		
		m_DmgTriggers = new array<BarbedWireTrigger>;
		m_BarbedWires = new map<string,EN5C_BarbedWire>;
	}
	
	// Destructor
	void ~BaseBuildingBase() 
	{
		// DestroyDamageTriggers(); // Remove function!
	}
	
	// --- SUPPORT
	// Return attached object of the given type
	EntityAI GetAttachment ( string object_type ) 
	{
		int attachments_count = this.AttachmentsCount();
		EntityAI result = NULL;
		
		for (int i = 0; i < attachments_count; i++)
		{
			EntityAI attachment = this.GetAttachmentFromIndex ( i );
			if ( attachment.IsKindOf ( object_type ) )
			{
				result = attachment;
				break;
			}
		}
		
		return result;
	}
	
	//Check required construction materials 
	bool HasRequiredMaterials ( int part_uid )
	{
		ConstructionPart construction_part = m_ConstructionParts.Get ( part_uid );
		int attachments_count = this.AttachmentsCount();
		int materials_count = construction_part.m_ConstructMaterials.Count();
		int materials_available = 0;
		
		for (int i = 0; i < attachments_count; i++)
		{
			ItemBase item = this.GetAttachmentFromIndex ( i );
			
			for ( int j = 0; j < materials_count; j++ )
			{
				string material_type = construction_part.m_ConstructMaterials.Get ( j ).m_Type;
				float material_quantity = construction_part.m_ConstructMaterials.Get ( j ).m_Quantity;
				
				if ( item.IsKindOf ( material_type ) && item.GetQuantity() >= material_quantity )
				{
					materials_available++;
				}
			}
		}
		
		//return results
		if ( materials_available == materials_count ) 
		{ 
			return true; 
		} 
		else 
		{ 
			return false; 
		}
	}
	
	//Take required materials from the source
	void TakeRequiredMaterials ( int part_uid )
	{
		ConstructionPart construction_part = m_ConstructionParts.Get ( part_uid );
		int attachments_count = this.AttachmentsCount();
		int materials_count = construction_part.m_ConstructMaterials.Count();
		
		for ( int i = 0; i < materials_count; i++ )
		{
			string material_type = construction_part.m_ConstructMaterials.Get ( i ).m_Type;
			float material_quantity = construction_part.m_ConstructMaterials.Get ( i ).m_Quantity;
			ItemBase item = this.GetAttachment ( material_type );
			
			//set quantity
			item.AddQuantity ( -material_quantity, true );
		}
	}
	
	bool HasAnimationState ( string animation, float state )
	{
		float actual_state = this.GetAnimationPhase ( animation );
		
		bool is_state = false;
		if (actual_state == state)
		{
			is_state = true;
		}
		
		return is_state;
	}

	//Receive remaining materials from the source
	void ReceiveRemainingMaterials ( int part_uid, PlayerBase player )
	{
		ConstructionPart construction_part = m_ConstructionParts.Get ( part_uid );
		int attachments_count = this.AttachmentsCount();
		int materials_count = construction_part.m_ConstructMaterials.Count();
		
		for ( int i = 0; i < materials_count; i++ )
		{
			string material_type = construction_part.m_DeconstructMaterials.Get ( i ).m_Type;
			float material_quantity = construction_part.m_DeconstructMaterials.Get ( i ).m_Quantity;
			
			
			//create in inventory
			ItemBase item = player.CreateInInventory ( material_type );
			item.SetQuantity ( material_quantity );
		}
	}
	
	bool HasRequiredTool ( EntityAI item_in_hands, array<string> tools )
	{
		bool has_tool = false;
		
		if ( item_in_hands ) 
		{
			for ( int i = 0; i < tools.Count(); i++ )
			{
				string tool_type = tools.Get ( i ); 	
				if ( item_in_hands.IsKindOf ( tool_type ) ) 
				{
					has_tool = true;
					break;
				}
			}
		}
		
		return has_tool;	
	}
	
	// --- EVENTS
	void EEItemAttached ( EntityAI item, string slot_name )
	{
		super.EEItemAttached ( item, slot_name );
		// --- Server Only ---
		if ( !g_Game.IsServer() ) return;
		// ---
		
		ItemBase item_IB = (ItemBase) item;
		string item_type = item_IB.GetType();
		
		// Attachment control
		
		switch (item_type)
		{
			case "EN5C_BarbedWire":
				EN5C_BarbedWire attached_BW = (EN5C_BarbedWire) item_IB;
				OnBarbedWireAttached(attached_BW, slot_name); // Removed function
			break;
			
			case "EN5C_CableReel":
				EN5C_CableReel attached_cable_reel = (EN5C_CableReel) item_IB;
				// OnCableReelAttached(attached_cable_reel, slot_name); // Removed function
			break;
		
			case "EN5C_CamoNet":
				this.SetAnimationPhase ( ANIMATION_CAMO_NET, 0 );
			break;
				
			case "EN5C_XmasLights":
				this.SetAnimationPhase ( ANIMATION_LIGHTS, 0 );
				EN5C_XmasLights fence_light = (EN5C_XmasLights)item_IB;
				fence_light.AttachToObject( this );
			break;
		}
		
		// this.OnItemAttached ( item ); // Removed function
	}
	
	void EEItemDetached ( EntityAI item, string slot_name )
	{
		super.EEItemDetached ( item, slot_name );
		
		// --- Server Only ---
		if ( !g_Game.IsServer() ) return;
		// ---
		
		// Attachment control
		ItemBase item_IB = (ItemBase) item;
		string item_type = item_IB.GetType();
		
		switch (item_type)
		{
			case "EN5C_BarbedWire":
				EN5C_BarbedWire detached_BW = (EN5C_BarbedWire) item_IB;
				OnBarbedWireDetached(detached_BW, slot_name); // Removed function
			break;
		
			case "EN5C_CableReel":
				EN5C_CableReel detached_CR = (EN5C_CableReel) item_IB;
				// OnCableReelDetached(detached_CR, slot_name); // Removed function
			break;
			
			case "EN5C_CamoNet":
				this.SetAnimationPhase ( ANIMATION_CAMO_NET, 1 );
			break;
		
			case "EN5C_XmasLights":
				this.SetAnimationPhase ( ANIMATION_LIGHTS, 1 );
				EN5C_XmasLights fence_light = (EN5C_XmasLights)item_IB;
				fence_light.DetachFromObject( this );
			break;
		}	
		
		// this.OnItemDetached ( item ); // Removed function
	}
	
	// --- ACTIONS
	//Place building base object
	//*** REMOVE ***//
	void PlaceObject ( string message )
	{ 
		// --- Server Only ---
		if ( !g_Game.IsServer() ) return;

		if ( this.GetOwnerPlayer().CanDropEntity ( this) )
		{
			PlayerBase owner_player = this.GetOwnerPlayer();
			owner_player.PlayActionGlobal ( "deployItem" );
			owner_player.DropEntity ( this );

			//set position
			vector object_pos = ( owner_player.GetDirection() ) * 0.5;
			this.SetPosition ( owner_player.GetPosition() + object_pos );

			owner_player.MessageStatus ( message );
		}
	}
	
	
	
	/*==============================================
					ELECTRIC FENCES
	==============================================*/
	
	
	// When a barbed wire is attached to this fence/watchtower
	void OnBarbedWireAttached(EN5C_BarbedWire attached_BW, string slot_name)
	{
		SetWire(slot_name, attached_BW);
		UpdateDamageTriggers();
		ShowSelection(ANIMATION_BARBED_WIRE);
	}
	
	// Creates all damage triggers this structure needs. They are tied to the barbed wire item.
	void UpdateDamageTriggers()
	{
		DestroyDamageTriggers();
		
		for ( local int i = 0; i < 3; ++i )
		{
			local string slot_name = BARBEDWIRE_SLOTS[i];
			local EN5C_BarbedWire bw = FindAttachmentBySlotName( slot_name );
			
			if (bw)
			{
				local vector local_pos = m_DmgTrgLocalPos[ i ];
				if ( local_pos != "0 0 0" ) // "0 0 0" is considered as undefined!
				{
					local vector global_pos	= ModelToWorld (local_pos);
					local BarbedWireTrigger dmg_trg = GetGame().CreateObject( "BarbedWireTrigger", global_pos, false );
					vector mins = "-1.8 -0.4 -0.6";
					vector maxs = "1.8 0.4 0.6";
					local vector ori = GetOrientation();
					ori[0] = ori[0] + m_DmgTrgLocalDir[i]; // rotate the trigger
					dmg_trg.SetOrientation( ori );
					dmg_trg.SetExtents(mins, maxs);	
					dmg_trg.SetParentObject( bw );
					m_DmgTriggers.Insert(dmg_trg);
				}
			}
		}
	}
	
	// Removes all damage triggers
	void DestroyDamageTriggers()
	{
		local int count = m_DmgTriggers.Count();
		--count;
		for ( local int i = count; i >= 0; --i )
		{
			BarbedWireTrigger trg = m_DmgTriggers.Get(i);
			
			if ( trg  &&  GetGame() ) // It's necesarry to check if the game exists. Otherwise a crash occurs while quitting.
				GetGame().ObjectDelete( trg );
			
			m_DmgTriggers.Remove(i);
		}
	}
	
	void SetWire(string slot_name, EN5C_BarbedWire wire)
	{
		if (wire)
		{
			m_BarbedWires.Set(slot_name, wire);
		}
		else
		{
			m_BarbedWires.Set(slot_name, NULL);
		}
	}
	
	
	// !Returns the some barbed wire attached on this fence
	EN5C_BarbedWire GetWire()
	{
		EN5C_BarbedWire return_wire;
		autoptr array<EN5C_BarbedWire> wires = GetWires();
		
		if (wires.Count() > 0)
		{
			return_wire = wires.Get(0);
		}
		
		return return_wire;
	}
	
	void OnBarbedWireDetached(EN5C_BarbedWire detached_BW, string slot_name)
	{
		detached_BW.GetCompEM().UnplugAllDevices();
		SetWire(slot_name, NULL);
		UpdateDamageTriggers();
		HideSelection(ANIMATION_BARBED_WIRE);
	}
	
	// !Returns array of barbed wires attached to this BBB object
	array<EN5C_BarbedWire> GetWires()
	{
		array<EN5C_BarbedWire> return_array = new array<EN5C_BarbedWire>;
		
		for ( local int i = 0; i < 3; ++i )
		{
			EN5C_BarbedWire bw = m_BarbedWires.Get( BARBEDWIRE_SLOTS[i] );
			
			if (bw)
			{
				return_array.Insert(bw);
			}
		}
		
		return return_array;
	}
	
	//! Returns the number of barbed wires attached to this structure
	int GetWiresCount()
	{
		int total_count = m_BarbedWires.Count();
		int counter = 0;
		
		for ( local int i = 0; i < total_count; ++i )
		{
			local EN5C_BarbedWire bw = m_BarbedWires.Get( BARBEDWIRE_SLOTS[i] );
			
			if (bw)
			{
				counter++;
			}
		}
		
		return counter;
	}
}