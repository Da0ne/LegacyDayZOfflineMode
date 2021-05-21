class EN5C_Fence extends BaseBuildingBase
{
	//.P3D SELECTION NAMES
	static const string ANIMATION_BASE 						= "Base";
	static const string ANIMATION_BASE_DOWN 				= "BaseFence_Down";
	static const string ANIMATION_BASE_UP 					= "BaseFence_Up";
	static const string ANIMATION_WOODEN_PLANKS_DOWN 		= "WoodenPlanks_Down";
	static const string ANIMATION_WOODEN_PLANKS_UP 			= "WoodenPlanks_Up";
	static const string ANIMATION_METAL_PLATES_DOWN 		= "MetalPlates_Down";
	static const string ANIMATION_METAL_PLATES_UP 			= "MetalPlates_Up";
	
	//CONSTRUCTION PARTS - UID
	static const int PART_BASE_DOWN 						= 101;
	static const int PART_BASE_UP 							= 102;
	static const int PART_WOODEN_PLANKS_DOWN 				= 201;
	static const int PART_WOODEN_PLANKS_UP 					= 202;
	static const int PART_METAL_PLATES_DOWN 				= 301;
	static const int PART_METAL_PLATES_UP 					= 302;
	
	//PLAYER STATUS MESSAGE
	static const string MESSAGE_BASE_FENCE_PLACED 			= "I have placed the base fence.";
	static const string MESSAGE_CONSTRUCT_BASE_DOWN 		= "I have built a low fence.";
	static const string MESSAGE_CONSTRUCT_BASE_UP 			= "I have built a high base fence.";
	static const string MESSAGE_DECONSTRUCT_BASE_DOWN 		= "I have destroyed the low base fence.";
	static const string MESSAGE_DECONSTRUCT_BASE_UP 		= "I have destroyed the high base fence.";
	static const string MESSAGE_CONSTRUCT_WOODEN_DOWN 		= "I have built a low wooden plank fence.";
	static const string MESSAGE_CONSTRUCT_WOODEN_UP 		= "I have built a high wooden plank fence.";
	static const string MESSAGE_DECONSTRUCT_WOODEN_DOWN 	= "I have destroyed the low wooden plank fence.";
	static const string MESSAGE_DECONSTRUCT_WOODEN_UP 		= "I have destroyed the high wooden plank fence.";
	static const string MESSAGE_CONSTRUCT_METAL_DOWN 		= "I have built a low metal plates fence.";
	static const string MESSAGE_CONSTRUCT_METAL_UP 			= "I have built a high metal plates fence.";
	static const string MESSAGE_DECONSTRUCT_METAL_DOWN 		= "I have destroyed the low metal plates fence.";
	static const string MESSAGE_DECONSTRUCT_METAL_UP 		= "I have destroyed the high metal plates fence.";	
	
	void EN5C_Fence()
	{
		m_ConstructionParts = new map<int, ConstructionPart>;
		ConstructionPart constr_part;
		
		//REQUIRED TOOLS FOR CONSTRUCTION/DECONSTRUCTION
		m_ConstructionTools 			= new array<string>;
		m_DeconstructionTools 			= new array<string>;
		
		m_ConstructionTools.Insert 		( "EN5C_Hammer" );
		m_ConstructionTools.Insert 		( "EN5C_SledgeHammer" );
		m_ConstructionTools.Insert 		( "EN5C_Wrench" );
		
		m_DeconstructionTools.Insert 	( "EN5C_Crowbar" );
		m_DeconstructionTools.Insert 	( "EN5C_Wrench" );
		m_DeconstructionTools.Insert	( "EN5C_Iceaxe" );
		m_DeconstructionTools.Insert	( "EN5C_SledgeHammer" );
		
		m_DmgTrgLocalPos				= { "0 0 0" , "0.22 -0.75 0.55" }; // {"<right> <up> <forward>", "<right> <up> <forward>", "<right> <up> <forward>"} model coordinates. The first element must be "0 0 0" due to a bug in inventory screen which makes barbed wire icon invisible.
		
		//BASE
		//BASE DOWN
		constr_part = new ConstructionPart;
		constr_part.m_ConstructMaterials = new array<ConstructionMaterial>;					//materials to construct
		constr_part.m_ConstructMaterials.Insert ( new ConstructionMaterial ( "EN5C_Nails", 6 ) );
		constr_part.m_ConstructMaterials.Insert ( new ConstructionMaterial ( "EN5C_WoodenPlanks", 4 ) );
		constr_part.m_DeconstructMaterials = new array<ConstructionMaterial>;				//materials to deconstruct
		constr_part.m_DeconstructMaterials.Insert ( new ConstructionMaterial ( "EN5C_Nails", 5 ) );
		constr_part.m_DeconstructMaterials.Insert ( new ConstructionMaterial ( "EN5C_WoodenPlanks", 3 ) );
		m_ConstructionParts.Set ( PART_BASE_DOWN, constr_part );
		//BASE UP
		constr_part = new ConstructionPart;
		constr_part.m_ConstructMaterials = new array<ConstructionMaterial>;					//materials to construct
		constr_part.m_ConstructMaterials.Insert ( new ConstructionMaterial ( "EN5C_Nails", 6 ) );
		constr_part.m_ConstructMaterials.Insert ( new ConstructionMaterial ( "EN5C_WoodenPlanks", 4 ) );
		constr_part.m_DeconstructMaterials = new array<ConstructionMaterial>;				//materials to deconstruct
		constr_part.m_DeconstructMaterials.Insert ( new ConstructionMaterial ( "EN5C_Nails", 8 ) );
		constr_part.m_DeconstructMaterials.Insert ( new ConstructionMaterial ( "EN5C_WoodenPlanks", 3 ) );
		m_ConstructionParts.Set ( PART_BASE_UP, constr_part );	
		
		//WOODEN PLANKS
		//WOODEN PLANKS DOWN
		constr_part = new ConstructionPart;
		constr_part.m_ConstructMaterials = new array<ConstructionMaterial>;					//materials to construct
		constr_part.m_ConstructMaterials.Insert ( new ConstructionMaterial ( "EN5C_Nails", 10 ) );
		constr_part.m_ConstructMaterials.Insert ( new ConstructionMaterial ( "EN5C_WoodenPlanks", 6 ) );
		constr_part.m_DeconstructMaterials = new array<ConstructionMaterial>;				//materials to deconstruct
		constr_part.m_DeconstructMaterials.Insert ( new ConstructionMaterial ( "EN5C_Nails", 8 ) );
		constr_part.m_DeconstructMaterials.Insert ( new ConstructionMaterial ( "EN5C_WoodenPlanks", 5 ) );
		m_ConstructionParts.Set ( PART_WOODEN_PLANKS_DOWN, constr_part );
		//WOODEN PLANKS UP
		constr_part = new ConstructionPart;
		constr_part.m_ConstructMaterials = new array<ConstructionMaterial>;					//materials to construct
		constr_part.m_ConstructMaterials.Insert ( new ConstructionMaterial ( "EN5C_Nails", 10 ) );
		constr_part.m_ConstructMaterials.Insert ( new ConstructionMaterial ( "EN5C_WoodenPlanks", 6 ) );
		constr_part.m_DeconstructMaterials = new array<ConstructionMaterial>;				//materials to deconstruct
		constr_part.m_DeconstructMaterials.Insert ( new ConstructionMaterial ( "EN5C_Nails", 8 ) );
		constr_part.m_DeconstructMaterials.Insert ( new ConstructionMaterial ( "EN5C_WoodenPlanks", 5 ) );
		m_ConstructionParts.Set ( PART_WOODEN_PLANKS_UP, constr_part );
		
		//METAL PLATES
		//METAL PLATES DOWN
		constr_part = new ConstructionPart;
		constr_part.m_ConstructMaterials = new array<ConstructionMaterial>;					//materials to construct
		constr_part.m_ConstructMaterials.Insert ( new ConstructionMaterial ( "EN5C_Nails", 10 ) );
		constr_part.m_ConstructMaterials.Insert ( new ConstructionMaterial ( "EN5C_MetalPlates", 6 ) );
		constr_part.m_DeconstructMaterials = new array<ConstructionMaterial>;				//materials to deconstruct
		constr_part.m_DeconstructMaterials.Insert ( new ConstructionMaterial ( "EN5C_Nails", 8 ) );
		constr_part.m_DeconstructMaterials.Insert ( new ConstructionMaterial ( "EN5C_MetalPlates", 6 ) );
		m_ConstructionParts.Set ( PART_METAL_PLATES_DOWN, constr_part );
		//METAL PLATES UP
		constr_part = new ConstructionPart;
		constr_part.m_ConstructMaterials = new array<ConstructionMaterial>;					//materials to construct
		constr_part.m_ConstructMaterials.Insert ( new ConstructionMaterial ( "EN5C_Nails", 10 ) );
		constr_part.m_ConstructMaterials.Insert ( new ConstructionMaterial ( "EN5C_MetalPlates", 6 ) );
		constr_part.m_DeconstructMaterials = new array<ConstructionMaterial>;				//materials to deconstruct
		constr_part.m_DeconstructMaterials.Insert ( new ConstructionMaterial ( "EN5C_Nails", 8 ) );
		constr_part.m_DeconstructMaterials.Insert ( new ConstructionMaterial ( "EN5C_MetalPlates", 6 ) );
		m_ConstructionParts.Set ( PART_METAL_PLATES_UP, constr_part );	
	}

	// --- ATTACHMENTS
	void OnItemAttached ( EntityAI item ) 
	{	
	}
	
	void OnItemDetached ( EntityAI item ) 
	{
	}
	
	// --- CONSTRUCTION / DECONSTRUCTION
	// --- BASE LOW/HIGH 
	bool CanConstruct_Base_Down ( PlayerBase player )
	{
		bool has_materials = this.HasRequiredMaterials ( PART_BASE_DOWN );
		bool already_built = this.HasAnimationState ( ANIMATION_BASE_DOWN, 0 );
		EntityAI item_in_hands = player.GetEntityInHands();
		bool has_tools = this.HasRequiredTool ( item_in_hands, this.m_ConstructionTools );
		
		if ( has_materials && !already_built && has_tools )
		{
			return true;
		}
		else 
		{
			return false;
		}
	}
	
	bool CanConstruct_Base_Up ( PlayerBase player )
	{
		bool has_materials = this.HasRequiredMaterials ( PART_BASE_UP );
		bool already_built = this.HasAnimationState ( ANIMATION_BASE_UP, 0 );
		EntityAI item_in_hands = player.GetEntityInHands();
		bool has_tools = this.HasRequiredTool ( item_in_hands, this.m_ConstructionTools );
		
		if ( has_materials && !already_built && has_tools )
		{
			return true;
		}
		else 
		{
			return false;
		}
	}
	
	void Construct_Base_Down ( PlayerBase player )
	{
		//take materials
		this.TakeRequiredMaterials ( PART_BASE_DOWN );
		
		//animate
		this.SetAnimationPhase ( ANIMATION_BASE_DOWN, 0 );
		
		//player message
		player.MessageStatus ( MESSAGE_CONSTRUCT_BASE_DOWN );
	}
	
	void Construct_Base_Up( PlayerBase player )
	{
		//take materials
		this.TakeRequiredMaterials ( PART_BASE_UP );
		
		//animate
		this.SetAnimationPhase ( ANIMATION_BASE_UP, 0 );
		
		//player message
		player.MessageStatus ( MESSAGE_CONSTRUCT_BASE_UP );
	}
	
	bool CanDeconstruct_Base_Down ( PlayerBase player )
	{
		bool already_built = this.HasAnimationState ( ANIMATION_BASE_DOWN, 0 );
		bool metal_plates_already_built = this.HasAnimationState ( ANIMATION_METAL_PLATES_DOWN, 0 );
		bool wooden_planks_already_built = this.HasAnimationState ( ANIMATION_WOODEN_PLANKS_DOWN, 0 );
		EntityAI item_in_hands = player.GetEntityInHands();
		bool has_tools = this.HasRequiredTool ( item_in_hands, m_DeconstructionTools );
		
		if ( already_built && !metal_plates_already_built && !wooden_planks_already_built && has_tools )
		{
			return true;
		}
		else 
		{
			return false;
		}
	}
	
	bool CanDeconstruct_Base_Up ( PlayerBase player )
	{
		bool already_built = this.HasAnimationState ( ANIMATION_BASE_UP, 0 );
		bool metal_plates_already_built = this.HasAnimationState ( ANIMATION_METAL_PLATES_UP, 0 );
		bool wooden_planks_already_built = this.HasAnimationState ( ANIMATION_WOODEN_PLANKS_UP, 0 );
		EntityAI item_in_hands = player.GetEntityInHands();
		bool has_tools = this.HasRequiredTool ( item_in_hands, m_DeconstructionTools );
		
		if ( already_built && !metal_plates_already_built && !wooden_planks_already_built && has_tools )
		{
			return true;
		}
		else 
		{
			return false;
		}
	}
	
	void Deconstruct_Base_Down( PlayerBase player )
	{
		//receive materials
		this.ReceiveRemainingMaterials ( PART_BASE_DOWN, player );
		
		//animate
		this.SetAnimationPhase ( ANIMATION_BASE_DOWN, 1 );
		
		//player message
		player.MessageStatus ( MESSAGE_DECONSTRUCT_BASE_DOWN );
	}
	
	void Deconstruct_Base_Up( PlayerBase player )
	{
		//receive materials
		this.ReceiveRemainingMaterials ( PART_BASE_UP, player );
		
		//animate
		this.SetAnimationPhase ( ANIMATION_BASE_UP, 1 );
		
		//player message
		player.MessageStatus ( MESSAGE_DECONSTRUCT_BASE_UP );
	}
	
	// --- WOODEN PLANKS DOWN/UP
	bool CanConstruct_WoodenPlanks_Down ( PlayerBase player )
	{
		bool has_materials = this.HasRequiredMaterials ( PART_WOODEN_PLANKS_DOWN );
		bool already_built = this.HasAnimationState ( ANIMATION_WOODEN_PLANKS_DOWN, 0 );
		bool base_already_built = this.HasAnimationState ( ANIMATION_BASE_DOWN, 0 );
		bool metal_plates_already_built = this.HasAnimationState ( ANIMATION_METAL_PLATES_DOWN, 0 );
		EntityAI item_in_hands = player.GetEntityInHands();
		bool has_tools = this.HasRequiredTool ( item_in_hands, this.m_ConstructionTools );
		
		if ( has_materials && !already_built && base_already_built && !metal_plates_already_built && has_tools )
		{
			return true;
		}
		else 
		{
			return false;
		}
	}
	
	bool CanConstruct_WoodenPlanks_Up ( PlayerBase player )
	{
		bool has_materials = this.HasRequiredMaterials ( PART_WOODEN_PLANKS_UP );
		bool already_built = this.HasAnimationState ( ANIMATION_WOODEN_PLANKS_UP, 0 );
		bool base_already_built = this.HasAnimationState ( ANIMATION_BASE_UP, 0 );
		bool metal_plates_already_built = this.HasAnimationState ( ANIMATION_METAL_PLATES_UP, 0 );
		EntityAI item_in_hands = player.GetEntityInHands();
		bool has_tools = this.HasRequiredTool ( item_in_hands, this.m_ConstructionTools );
		
		if ( has_materials && !already_built && base_already_built && !metal_plates_already_built && has_tools )
		{
			return true;
		}
		else 
		{
			return false;
		}
	}

	void Construct_WoodenPlanks_Down( PlayerBase player )
	{
		//take materials
		this.TakeRequiredMaterials ( PART_WOODEN_PLANKS_DOWN );
		
		//animate
		this.SetAnimationPhase ( ANIMATION_WOODEN_PLANKS_DOWN, 0 );
		
		//player message
		player.MessageStatus ( MESSAGE_CONSTRUCT_WOODEN_DOWN );
	}

	void Construct_WoodenPlanks_Up( PlayerBase player )
	{
		//take materials
		this.TakeRequiredMaterials ( PART_WOODEN_PLANKS_UP );
		
		//animate
		this.SetAnimationPhase ( ANIMATION_WOODEN_PLANKS_UP, 0 );
		
		//player message
		player.MessageStatus ( MESSAGE_CONSTRUCT_WOODEN_UP );
	}
	
	bool CanDeconstruct_WoodenPlanks_Down ( PlayerBase player )
	{
		bool already_built = this.HasAnimationState ( ANIMATION_WOODEN_PLANKS_DOWN, 0 );
		EntityAI item_in_hands = player.GetEntityInHands();
		bool has_tools = this.HasRequiredTool ( item_in_hands, m_DeconstructionTools );
		
		if ( already_built && has_tools )
		{
			return true;
		}
		else 
		{
			return false;
		}
	}

	bool CanDeconstruct_WoodenPlanks_Up ( PlayerBase player )
	{
		bool already_built = this.HasAnimationState ( ANIMATION_WOODEN_PLANKS_UP, 0 );
		EntityAI item_in_hands = player.GetEntityInHands();
		bool has_tools = this.HasRequiredTool ( item_in_hands, m_DeconstructionTools );
		
		if ( already_built && has_tools )
		{
			return true;
		}
		else 
		{
			return false;
		}
	}
	
	void Deconstruct_WoodenPlanks_Down( PlayerBase player )
	{
		//receive materials
		this.ReceiveRemainingMaterials ( PART_WOODEN_PLANKS_DOWN, player );
		
		//animate
		this.SetAnimationPhase ( ANIMATION_WOODEN_PLANKS_DOWN, 1 );
		
		//player message
		player.MessageStatus ( MESSAGE_DECONSTRUCT_WOODEN_DOWN );
	}
	
	void Deconstruct_WoodenPlanks_Up( PlayerBase player )
	{
		//receive materials
		this.ReceiveRemainingMaterials ( PART_WOODEN_PLANKS_UP, player );
		
		//animate
		this.SetAnimationPhase ( ANIMATION_WOODEN_PLANKS_UP, 1 );
		
		//player message
		player.MessageStatus ( MESSAGE_DECONSTRUCT_WOODEN_UP );
	}
	
	// --- METAL PLATES DOWN/UP 
	bool CanConstruct_MetalPlates_Down ( PlayerBase player )
	{
		bool has_materials = this.HasRequiredMaterials ( PART_METAL_PLATES_DOWN );
		bool already_built = this.HasAnimationState ( ANIMATION_METAL_PLATES_DOWN, 0 );
		bool base_already_built = this.HasAnimationState ( ANIMATION_BASE_DOWN, 0 );
		bool wooden_planks_already_built = this.HasAnimationState ( ANIMATION_WOODEN_PLANKS_DOWN, 0 );
		EntityAI item_in_hands = player.GetEntityInHands();
		bool has_tools = this.HasRequiredTool ( item_in_hands, this.m_ConstructionTools );
		
		if ( has_materials && !already_built && base_already_built && !wooden_planks_already_built && has_tools )
		{
			return true;
		}
		else 
		{
			return false;
		}
	}
	
	bool CanConstruct_MetalPlates_Up ( PlayerBase player )
	{
		bool has_materials = this.HasRequiredMaterials ( PART_METAL_PLATES_UP );
		bool already_built = this.HasAnimationState ( ANIMATION_METAL_PLATES_UP, 0 );
		bool base_already_built = this.HasAnimationState ( ANIMATION_BASE_UP, 0 );
		bool wooden_planks_already_built = this.HasAnimationState ( ANIMATION_WOODEN_PLANKS_UP, 0 );
		EntityAI item_in_hands = player.GetEntityInHands();
		bool has_tools = this.HasRequiredTool ( item_in_hands, this.m_ConstructionTools );
		
		if ( has_materials && !already_built && base_already_built && !wooden_planks_already_built && has_tools )
		{
			return true;
		}
		else 
		{
			return false;
		}
	}
	
	void Construct_MetalPlates_Down( PlayerBase player )
	{
		//take materials
		this.TakeRequiredMaterials ( PART_METAL_PLATES_DOWN );
		
		//animate
		this.SetAnimationPhase ( ANIMATION_METAL_PLATES_DOWN, 0 );
		
		//player message
		player.MessageStatus ( MESSAGE_CONSTRUCT_METAL_DOWN );
	}
	
	void Construct_MetalPlates_Up( PlayerBase player )
	{
		//take materials
		this.TakeRequiredMaterials ( PART_METAL_PLATES_UP );
		
		//animate
		this.SetAnimationPhase ( ANIMATION_METAL_PLATES_UP, 0 );
		
		//player message
		player.MessageStatus ( MESSAGE_CONSTRUCT_METAL_UP );
	}
	
	bool CanDeconstruct_MetalPlates_Down ( PlayerBase player )
	{
		bool already_built = this.HasAnimationState ( ANIMATION_METAL_PLATES_DOWN, 0 );
		EntityAI item_in_hands = player.GetEntityInHands();
		bool has_tools = this.HasRequiredTool ( item_in_hands, m_DeconstructionTools );
		
		if ( already_built && has_tools )
		{
			return true;
		}
		else 
		{
			return false;
		}
	}

	bool CanDeconstruct_MetalPlates_Up ( PlayerBase player )
	{
		bool already_built = this.HasAnimationState ( ANIMATION_METAL_PLATES_UP, 0 );
		EntityAI item_in_hands = player.GetEntityInHands();
		bool has_tools = this.HasRequiredTool ( item_in_hands, m_DeconstructionTools );
		
		if ( already_built && has_tools )
		{
			return true;
		}
		else 
		{
			return false;
		}
	}
	
	void Deconstruct_MetalPlates_Down( PlayerBase player )
	{
		//receive materials
		this.ReceiveRemainingMaterials ( PART_METAL_PLATES_DOWN, player );
		
		//animate
		this.SetAnimationPhase ( ANIMATION_METAL_PLATES_DOWN, 1 );
		
		//player message
		player.MessageStatus ( MESSAGE_DECONSTRUCT_METAL_DOWN );
	}
	
	void Deconstruct_MetalPlates_Up( PlayerBase player )
	{
		//receive materials
		this.ReceiveRemainingMaterials ( PART_METAL_PLATES_UP, player );
		
		//animate
		this.SetAnimationPhase ( ANIMATION_METAL_PLATES_UP, 1 );
		
		//player message
		player.MessageStatus ( MESSAGE_DECONSTRUCT_METAL_UP );
	}
	
	// --- ACTIONS
	//Placing fence
	//*** REMOVE ***//
	bool CanPlaceFence ( PlayerBase player ) 
	{
		bool can_be_placed = false;
		EntityAI item_in_hands = player.GetEntityInHands();
		
		if ( item_in_hands ) 
		{
			if (item_in_hands == this)
			{
				can_be_placed = true;
			}		
		}
		
		return can_be_placed;
	}
	
	//*** REMOVE ***//
	void PlaceFence() 
	{
		this.PlaceObject ( MESSAGE_BASE_FENCE_PLACED );
	}
}
