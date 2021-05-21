class PileOfWoodenPlanks extends ItemBase
{
	// Constructor
	void PileOfWoodenPlanks()
	{
		SetQuantity2( GetQuantityMax2() );
	}
	
	// Shows/Hides selections. Call this in init or after every quantity change.
	void UpdateSelections()
	{
		// Show/Hide selections according to the current quantity
		if ( this )
		{
			float quantity = GetQuantity();
			float quantity_max = GetQuantityMax();
			Print(quantity);
			Print(quantity_max);
			if ( quantity > GetQuantityMax2() *0.66 )
			{
				// Show 3/3 amount of planks
				SetAnimationPhase ( "stage_3", 0 ); // 1 = hide, 0 = unhide!
				SetAnimationPhase ( "stage_2", 1 ); // 1 = hide, 0 = unhide!
				SetAnimationPhase ( "stage_1", 1 ); // 1 = hide, 0 = unhide!
			}
			
			if ( quantity > quantity_max *0.33  &&  quantity <= quantity_max *0.66 )
			{
				// Show 2/3 amount of planks
				SetAnimationPhase ( "stage_3", 1 ); // 1 = hide, 0 = unhide!
				SetAnimationPhase ( "stage_2", 0 ); // 1 = hide, 0 = unhide!
				SetAnimationPhase ( "stage_1", 1 ); // 1 = hide, 0 = unhide!
			}
			
			if ( quantity > 0  &&  quantity <= quantity_max *0.33 )
			{
				// Show 1/3 amount of planks
				SetAnimationPhase ( "stage_3", 1 ); // 1 = hide, 0 = unhide!
				SetAnimationPhase ( "stage_2", 1 ); // 1 = hide, 0 = unhide!
				SetAnimationPhase ( "stage_1", 0 ); // 1 = hide, 0 = unhide!
			}
			
			if ( quantity == 0 )
			{
				// Show 0 planks. Object should be deleted now.
				SetAnimationPhase ( "stage_3", 1 ); // 1 = hide, 0 = unhide!
				SetAnimationPhase ( "stage_2", 1 ); // 1 = hide, 0 = unhide!
				SetAnimationPhase ( "stage_1", 1 ); // 1 = hide, 0 = unhide!
			}
		}
	}
	
	// handle possible actions on this object
	void OnGetActions(ActionMenuContext ctx, Man player)
	{
		string action_name = "";
		ItemBase item_in_hands = player.GetEntityInHands();
		
		if ( item_in_hands )
		{
			if ( item_in_hands.GetType() == "Tool_Hacksaw"  &&  item_in_hands.GetDamage() < 1 )
			{
				action_name = String("Saw planks into shorter pieces");
				ctx.AddAction(action_name, ACN_SAW_PLANKS, NULL, 1000, true, true);
			}
		}
	}
	
	// Removes a number of long planks. Note that one (4m) long plank has the length of 3x normal planks!
	// Returns the number of removed long planks
	int RemovePlanks( int needed_planks )
	{
		// Make sure to not give more long planks than we have available
		int available_planks = needed_planks;
		if ( available_planks > GetQuantity2() )
		{
			available_planks = GetQuantity2();
		}
		
		// Remove long planks from this object
		AddQuantity( -available_planks, true ); // Autodelete enabled
		
		// Visual feedback
		UpdateSelections();
		
		// Return the number of removed long planks
		return available_planks;
	}

	// Handle actions on this object
	bool OnAction(int action_id, Man player, ParamsReadContext ctx)
	{
		if ( GetGame().IsServer() )
		{
			switch (action_id)
			{
			case ACN_SAW_PLANKS: // Use saw to cut planks into shorter pieces and add those to the player's inventory
				PlayerBase player_b = player;
				int gained_planks = RemovePlanks(1); // If you change this then update the status message as well!
				
				for ( int i = 0; i < gained_planks; ++i )
				{
					ItemBase planks = player_b.CreateInInventory( "Consumable_WoodenPlanks" );
					planks.SetQuantity( 3 );
				}
				
				// Add damage to the tool
				ItemBase item_in_hands = player.GetEntityInHands();
				item_in_hands.AddDamage(0.01);
				
				// Visual feedback
				player_b.PlayActionGlobal( "MeleeKnifeSlash" ); // TO DO: Use different animation instead of this placeholder
				string message = "I've sawn one long plank into 3 shorter pieces.";
				player_b.MessageAction( message );
				
				break;
			}
		}
	}
}

// Temporal class for new User Actions
class EN5C_PileOfWoodenPlanks extends PileOfWoodenPlanks
{
	
}
