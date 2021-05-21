class TempCrafting
{

	static void CraftFireplace ( ItemBase item_1, ItemBase item_2, PlayerBase player )
	{
		float use_quantity = 0;
		
		//create object
		ItemBase fireplace = player.CreateInInventory ( "EN5C_Fireplace" );
		
		//item 1
		ItemBase item_1_new = fireplace.CreateAttachment( item_1.GetType() );
		if ( item_1.GetQuantity() > 0 && item_1.GetQuantity() < 1 )
		{
			use_quantity = item_1.GetQuantity();
		}
		else
		{
			use_quantity = 1;
		}
		//quanity
		item_1_new.SetQuantity ( use_quantity );
		item_1.SetQuantity ( -use_quantity );
		//damage
		item_1_new.SetDamage ( item_1.GetDamage() );
		
		//item 2
		ItemBase item_2_new = fireplace.CreateAttachment( item_2.GetType() );
		if ( item_2.GetQuantity() > 0 && item_2.GetQuantity() < 1 )
		{
			use_quantity = item_2.GetQuantity();
		}
		else
		{
			use_quantity = 1;
		}
		//quanity
		item_2_new.SetQuantity ( use_quantity );
		item_2.SetQuantity ( -use_quantity );
		//damage
		item_2_new.SetDamage ( item_2.GetDamage() );	
		
		//Player message
		string message = "I made a fireplace.";
		player.MessageAction( message );
	}

	static bool CanCraftFireplace ( ItemBase item_1, ItemBase item_2, PlayerBase player )
	{
		int player_using_something = player.GetVariableInt ( "isUsingSomething" ); 
		EntityAI item_1_owner = item_1.GetOwner();
		EntityAI item_2_owner = item_2.GetOwner();
		
		if ( player_using_something == 0 && !item_1_owner.IsKindOf ( "FireplaceBase" ) && !item_2_owner.IsKindOf ( "FireplaceBase" ) )
		{
			return true;
		}
		
		return false;
	}
}