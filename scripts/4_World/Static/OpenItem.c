class OpenItem
{
	//! WIll open the 'item_target' by spawning a new entity and transferring item variables to the new one
	static void OpenAndSwitch(ItemBase item_tool, ItemBase item_target, PlayerBase player)
	{
		autoptr array<int> spill_range = new array<int>;
		
		if( item_tool.ConfigIsExisting("OpenItemSpillRange") )
		{
			item_tool.ConfigGetIntArray("OpenItemSpillRange", spill_range );
		}
		else
		{
			Debug.LogError("OpenItemSpillRange config parameter missing, default values used ! ");
			Error("OpenItemSpillRange config parameter missing, default values used !");
			spill_range.Insert(0);
			spill_range.Insert(100);
		}
		float spill_modificator = Math.RandomIntInclusive( spill_range.Get(0),spill_range.Get(1) ) / 100;
		ItemBase new_item = OpenItem.SwitchItems(item_target, "_Opened", player);
		
		float quantity_old = new_item.GetQuantity2();
		float spill_amount = quantity_old * spill_modificator;
		float quantity_new = quantity_old - spill_amount;
		
		Debug.Log("quantity before spill: "+quantity_old.ToString());
		Debug.Log("spill_amount: "+spill_amount.ToString());
		Debug.Log("quantity_new: "+quantity_new.ToString());
		
		new_item.SetQuantity2(quantity_new);
	}
	
	//! Will switch the 'item' for a new game entity, the new entity's classname will be formed by adding the 'suffix' to the classname of the old 'item'
	static ItemBase SwitchItems(ItemBase old_item, string suffix, PlayerBase player)
	{
		string old_classname = old_item.GetType();
		string new_classname = old_classname + suffix;
		vector position = old_item.GetPosition();
		
		int item_placement = 0;
		
		//the item could be in our player's inventory/hands, on the ground, in a container, or in someone else's inventory
		EntityAI owner_item	= old_item.GetOwner();
		ItemBase new_item = NULL;
		if( owner_item )
		{
			new_item = player.SpawnItem(new_classname,SPAWNTYPE_GROUND, false);
			player.SwapEntities(old_item, new_item);
		}
		
		else
		{
			new_item = player.SpawnItem(new_classname,SPAWNTYPE_GROUND, false,0, true, position);
		}
		
		if( new_item ) 
		{
			MiscGameplayFunctions.TransferItemProperties(old_item, new_item,true,false,true);
			old_item.Delete();
		}
		else
		{
			Debug.LogError("SwitchItems: failed to create new item","static");
		}
		return new_item;
	}
};