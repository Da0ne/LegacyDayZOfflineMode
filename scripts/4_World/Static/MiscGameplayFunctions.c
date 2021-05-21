class MiscGameplayFunctions
{	
	static const int NUM_OF_BLOOD_TYPES = 8;
	
	//! will transform item' variables, agents and other local scripted properties as well as any relevant non-scripted properties like health
	static void TransferItemProperties(ItemBase source, ItemBase target, bool transfer_agents = true, bool transfer_variables = true, bool transfer_health = true, bool exclude_quantity = false)
	{
		if( transfer_agents ) target.TransferAgents( source.GetAgents() );
		
		if( transfer_variables )
		{
			target.TransferVariablesFloat( source.GetVariablesFloat() );
			target.TransferVariablesString( source.GetVariablesString() );
			if( exclude_quantity )
			{
				target.RemoveItemVariable(VARIABLE_QUANTITY);
			}
		}
		
		if( transfer_health ) target.SetHealth("", "", source.GetHealth("",""));
	}
	
	static int GenerateBloodType()
	{
		int types[NUM_OF_BLOOD_TYPES] 		= {LIQUID_BLOOD_0_P, LIQUID_BLOOD_0_N, LIQUID_BLOOD_A_P, LIQUID_BLOOD_A_N, LIQUID_BLOOD_B_P, LIQUID_BLOOD_B_N, LIQUID_BLOOD_AB_P, LIQUID_BLOOD_AB_N};
		int probability[NUM_OF_BLOOD_TYPES] 	= {42,10,25,5,9,3,2,2};
		
		int probability_sum = 0;
		
		for(int i = 0; i < NUM_OF_BLOOD_TYPES; i++)
		{
			probability_sum += probability[i];
		}
		
		//TIMERDEPRECATED - randomized blodtype for new character
		int probability_randomized = Math.RandomInt(0,probability_sum);
		
		int tmp = 0;
		int index;
		
		for(i = 0; i < NUM_OF_BLOOD_TYPES; i++)
		{
			tmp += probability[i];
			
			if( tmp > probability_randomized )
			{
				index = i;
				break;
			}
		}
		
		return types[index];
	}

	static bool MatchBloodCompatibility(int bloodtypetarget, int bloodtype)
	{
		bool result = false;
		switch ( bloodtype )
		{
			case LIQUID_BLOOD_0_P: 
				if ( bloodtypetarget == LIQUID_BLOOD_0_P || bloodtypetarget == LIQUID_BLOOD_0_N ) result = true;
			break;
			
			case LIQUID_BLOOD_0_N:
				if ( bloodtypetarget == LIQUID_BLOOD_0_N ) result = true;
			break;
			
			case LIQUID_BLOOD_A_P:
				if ( bloodtypetarget == LIQUID_BLOOD_A_P || bloodtypetarget == LIQUID_BLOOD_A_N || bloodtypetarget == LIQUID_BLOOD_0_P || bloodtypetarget == LIQUID_BLOOD_0_N ) result = true;
			break;
			
			case LIQUID_BLOOD_A_N: 
				if ( bloodtypetarget == LIQUID_BLOOD_A_N || bloodtypetarget == LIQUID_BLOOD_0_N ) result = true;
			break;
			
			case LIQUID_BLOOD_B_P: 
				if ( bloodtypetarget == LIQUID_BLOOD_B_P || bloodtypetarget == LIQUID_BLOOD_B_N || bloodtypetarget == LIQUID_BLOOD_0_P || bloodtypetarget == LIQUID_BLOOD_0_N ) result = true;
			break;
			
			case LIQUID_BLOOD_B_N:
				if ( bloodtypetarget == LIQUID_BLOOD_B_N || bloodtypetarget == LIQUID_BLOOD_0_N ) result = true;
			break;
			
			case LIQUID_BLOOD_AB_P:
				if ( bloodtypetarget == LIQUID_BLOOD_AB_P || bloodtypetarget == LIQUID_BLOOD_0_N || bloodtypetarget == LIQUID_BLOOD_0_P || bloodtypetarget == LIQUID_BLOOD_A_N || bloodtypetarget == LIQUID_BLOOD_A_P || bloodtypetarget == LIQUID_BLOOD_B_N || bloodtypetarget == LIQUID_BLOOD_B_P || bloodtypetarget == LIQUID_BLOOD_AB_N ) result = true;
			break;
			
			case LIQUID_BLOOD_AB_N: 
				if ( bloodtypetarget == LIQUID_BLOOD_AB_N || bloodtypetarget == LIQUID_BLOOD_0_N || bloodtypetarget == LIQUID_BLOOD_A_N || bloodtypetarget == LIQUID_BLOOD_B_N ) result = true;
			break;
			
			default: 
				Debug.Log("Incorrect blood type");
			break;
		}
		if ( result )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	static string GetBloodTypeName(int bloodtype)
	{
		switch ( bloodtype )
		{
			case LIQUID_BLOOD_0_P: 
				return "0+";
			break;
			
			case LIQUID_BLOOD_0_N:
				return "0-";
			break;
			
			case LIQUID_BLOOD_A_P:
				return "A+";
			break;
			
			case LIQUID_BLOOD_A_N: 
				return "A-";
			break;
			
			case LIQUID_BLOOD_B_P: 
				return "B+";
			break;
			
			case LIQUID_BLOOD_B_N:
				return "B-";
			break;
			
			case LIQUID_BLOOD_AB_P:
				return "AB+";
			break;
			
			case LIQUID_BLOOD_AB_N: 
				return "AB-";
			break;
			
			default: 
				return "bad value";
			break;
		}
	}
	
	static array<ItemBase> CreateItemBasePiles(string item_name, vector ground_position, float quantity,  float damage )
	{
		autoptr array<ItemBase>	items;
		float stack_size;
		ItemBase pile;
		
		items = new array<ItemBase>;
		stack_size = g_Game.ConfigGetInt("cfgVehicles " + item_name + " varQuantityMax");
		
		int piles_count = Math.Floor(quantity/stack_size);
		int rest = quantity - (piles_count*stack_size);
		
		for ( int i = 0; i < piles_count; i++ )
		{
			pile = GetGame().CreateObject(item_name, ground_position, false);
			pile.SetQuantity2(stack_size);
			items.Insert(pile);
		}
		if ( rest > 0)
		{
			pile = GetGame().CreateObject(item_name, ground_position, false);
			pile.SetQuantity2(rest);
			items.Insert(pile);
		}
		return items;
	}
	
	static array<Magazine> CreateMagazinePiles(string item_name, vector ground_position, float quantity,  float damage )
	{
		autoptr array<Magazine>	items;
		float stack_size;
		Magazine pile;
		
		items = new array<Magazine>;
		stack_size = g_Game.ConfigGetInt("cfgMagazines " + item_name + " count");
		
		int piles_count = Math.Floor(quantity/stack_size);
		int rest = quantity - (piles_count*stack_size);
		
		for ( int i = 0; i < piles_count; i++ )
		{
			pile = GetGame().CreateObject(item_name, ground_position, false);
			pile.SetAmmoCount(stack_size);
			items.Insert(pile);
		}
		if ( rest > 0)
		{
			pile = GetGame().CreateObject(item_name, ground_position, false);
			pile.SetAmmoCount(rest);
			items.Insert(pile);
		}
		return items;
	}
};
