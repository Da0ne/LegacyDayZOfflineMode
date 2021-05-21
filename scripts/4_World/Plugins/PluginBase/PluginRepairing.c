class PluginRepairing extends PluginBase
{	
	bool CanRepair(ItemBase item_a, ItemBase item_b)
	{
		if( !GetGame().IsNewPlayer()) // temporary WIP
		{
			return false;
		}
		
		if ( !item_a || !item_b )
		{
			return false;
		}
				
		// ItemA can repair ItemB
		if ( IsRepairValid(item_a.ConfigGetInt("repairKitType"), item_b.ConfigGetInt("repairableWith")) )
		{
			return true;
		}
		
		// ItemB can repair ItemA
		if ( IsRepairValid(item_b.ConfigGetInt("repairKitType"), item_a.ConfigGetInt("repairableWith")) )
		{
			return true;
		}
		
		return false;
	}

	bool Repair(PlayerBase player, ItemBase item_a, ItemBase item_b, int repair_type)
	{
		if ( !CanRepair(item_a, item_b) )
		{
			return false;
		}

		if ( item_a.IsRuined() || item_b.IsRuined() )
		{
			Print("Some item is ruined !");
			return false;
		}
		
		ItemBase item = item_a;
		ItemBase item_kit = item_b;
		
		if ( IsRepairValid(item_a.ConfigGetInt("repairKitType"), item_b.ConfigGetInt("repairableWith")) )
		{
			item = item_b;
			item_kit = item_a;
		}
		
		int w, h;
		GetGame().GetInventoryItemSize(item, w, h);
		
		float item_size			= w * h;
		float item_size_sqrt 	= Math.Sqrt( item_size );

		float cur_item_health	= item.GetHealth("","");
		float cur_kit_health	= item_kit.GetHealth("","");
		PluginExperience module_exp = (PluginExperience)GetPlugin( PluginExperience );
		
		int exp_ID;
		switch (repair_type) 
		{
			case REPAIR_TYPE_SHARPEN:
				//exp_ID = ; Waiting for design decision: soft skill clasification
				exp_ID = module_exp.EXP_TAILOR_REPAIRING;
				break;
				
			case REPAIR_TYPE_CLEAN:
				//exp_ID = ; Waiting for design decision: soft skill clasification
				exp_ID = module_exp.EXP_TAILOR_REPAIRING;
				break;
				
			case REPAIR_TYPE_TAILOR:
				exp_ID = module_exp.EXP_TAILOR_REPAIRING;
				break;
				
			case REPAIR_TYPE_DUCTTAPE:
				//exp_ID = ; Waiting for design decision: soft skill clasification
				exp_ID = module_exp.EXP_TAILOR_REPAIRING;
				break;
		}
		
		//float reparing_exp_points = module_exp.GetExpPoints( player, exp_ID );
		//int repairing_level = module_exp.GetLevel( reparing_exp_points, exp_ID );	
		int repairing_efficiency = module_exp.GetExpParamNumber( player, exp_ID, "efficiency" );
		float repairing_tool_damage = module_exp.GetExpParamNumber( player, exp_ID, "toolDamage" );
		int repairing_can_repair_to_pristine = module_exp.GetExpParamNumber( player, exp_ID, "canRepairToPristine" );

		//calculate level of new item health
		int cur_item_health_level = GetRepairLevelByDamage( cur_item_health );
		int new_item_health_level = cur_item_health_level - repairing_efficiency;
		int min_item_damage_level = 1;
		
		if ( repairing_can_repair_to_pristine == 1 )
		{
			min_item_damage_level = 0;
		}
		
		new_item_health_level = Math.Clamp( new_item_health_level, min_item_damage_level, 4 );
		
		//log
		player.MessageStatus( "cur_item_health_level: " + cur_item_health_level.ToString() );
		player.MessageStatus( "new_item_health_level: " + new_item_health_level.ToString() );
		player.MessageStatus( "Can repair to pristine: " + repairing_can_repair_to_pristine.ToString() );
		player.MessageStatus( "Repairing with efficiency: " + repairing_efficiency.ToString() );
		
		if ( cur_item_health_level <= new_item_health_level )
		{
			player.MessageStatus( "It cant be repared !" );
			return false;
		}
		
		//calculate new item health
		new_item_health_level = Math.Clamp( new_item_health_level, 0, 4 );
		float sub_item_health = GetDamagedByRepairLevel( new_item_health_level );
		float new_item_health = 100 - sub_item_health;
		new_item_health = Math.Clamp( new_item_health, 0, 100 );
		
		//calculate new kit health
		float sub_kit_health = ( item_size_sqrt * repairing_tool_damage ) * 100;
		float new_kit_health = cur_kit_health - sub_kit_health;
		new_kit_health = Math.Clamp( new_kit_health, 0, 100 );
		
		//set health values to item and kit
		item.SetHealth( "", "", new_item_health );
		item_kit.SetHealth( "", "", new_kit_health );
		
		//log
		player.MessageStatus( "new_item_health: " + new_item_health.ToString() + " new_kit_health: " + new_kit_health.ToString() );
		
		string type;
		GetGame().ObjectGetType( item_kit, type );
		if ( type == "Tool_SewingKit" )
		{
			module_exp.Increase( player, exp_ID );
		}
		else if ( type == "Tool_LeatherSewingKit" )
		{
			module_exp.Increase( player, exp_ID, 2 );
		}
		
		return true;
	}
	
	int GetRepairLevelByDamage(float health)
	{
		if ( health >= 70 )
		{
			return 0; // Pristine
		}
		else if ( health > 50 )
		{
			return 1; // Worn
		}
		else if ( health > 30 )
		{
			return 2; // Damaged
		}
		else if ( health > 0 )
		{
			return 3; // Badly Damaged
		}
		
		return 4; // Ruined
	}
	
	//damage in item health (parameter) values
	float GetDamagedByRepairLevel(int lvl)
	{
		if ( lvl == 1 )
		{
			return 31;
		}
		else if ( lvl == 2 )
		{
			return 50;
		}
		else if ( lvl == 3 )
		{
			return 70;
		}	
		else if ( lvl == 4 )
		{
			return 100;
		}
		
		// if lvl == 0
		return 0;
	}
	
	private bool IsRepairValid(int repair_kit_type, int repair_with_type)
	{
		if ( repair_kit_type > 0 && repair_with_type > 0 )
		{
			if ( repair_kit_type == repair_with_type )
			{
				return true;
			}
		}
		
		return false;
	}
}
