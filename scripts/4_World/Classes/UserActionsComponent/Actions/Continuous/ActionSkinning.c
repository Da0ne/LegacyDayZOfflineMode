class ActionSkinningCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 4;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionSkinning: ActionContinuousBase
{
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	
	void ActionSkinning()
	{
		m_CallbackClass = ActionSkinningCB;
		m_MessageStartFail = "The blade is not sharp enough";
		m_MessageStart = "I have started skinning the target.";
		m_MessageSuccess = "I have skinned the target.";
		m_MessageFail = "I have moved and skinning was canceled.";
		m_MessageCancel = "I stopped skinning the target.";
		//m_Animation = "EAT";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(MAXIMAL_TARGET_DISTANCE);	
	}

	int GetType()
	{
		return AT_SKINNING;
	}
		
	string GetText()
	{
		return "skin";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{	
		if ( target != NULL )
		{
			if ( target.IsMan() )
			{
				ManBase target_MB = (ManBase)target;
				
				if ( target_MB.CanBeSkinned(player, item) ) 
				{
					return true;
				}
			}
		}
		
		return false;
	}

	// Spawns the loot according to the Skinning class in the dead agent's config
	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		local float skinning_efficiency = UseSkinningSoftSkill(player);
		
		// Prepare to read config variables
		local string item_to_spawn;
		local string cfg_skinning_organ_class;
		
		// Mark the body as skinned to forbid another skinning action on it
		ManBase body = (ManBase) target;
		body.MarkAsSkinned();
		
		// Get config path to the animal
		local string cfg_animal_class_path = "cfgVehicles " + body.GetType() + " " + "Skinning ";
		
		// Getting item type from the config
		local int child_count = g_Game.ConfigGetChildrenCount(cfg_animal_class_path);
		
		// Parsing of the 'Skinning' class in the config of the dead body
		for (int i1 = 0; i1 < child_count; i1++)
		{
			// To make configuration as convenient as possible, all classes are parsed and parameters are read
			g_Game.ConfigGetChildName(cfg_animal_class_path, i1, cfg_skinning_organ_class); // out cfg_skinning_organ_class
			cfg_skinning_organ_class = cfg_animal_class_path + cfg_skinning_organ_class + " ";
			g_Game.ConfigGetText(cfg_skinning_organ_class + "item", item_to_spawn); // out item_to_spawn
			
			if ( item_to_spawn != "") // Makes sure to ignore incompatible parameters in the Skinning class of the agent
			{
				// Spawning items in player's inventory
				local int item_count = g_Game.ConfigGetInt(cfg_skinning_organ_class + "count");
				
				// In case item_count is not found
				if (item_count == 0) 
				{
					item_count = 1;
				}
				
				for (local int i2 = 0; i2 < item_count; i2++)
				{
					CreateOrgan(player, item_to_spawn, cfg_skinning_organ_class, skinning_efficiency, item.GetDamage() );
					
					item.SetDamage(item.GetDamage() + 0.01);
				}
			}
		}	
		
		PluginLifespan module_lifespan = GetPlugin(PluginLifespan);
		module_lifespan.SetBloodyHands( player );
	}

	// Soft skill implementation. Returns current efficiency.
	float UseSkinningSoftSkill(PlayerBase player)
	{
		// Handle soft skills for skinning
		local PluginExperience module_exp 	= GetPlugin( PluginExperience );
		local int   exp_ID 					= module_exp.EXP_HUNTER_SKINNING;
		local float skinning_exp_value 		= module_exp.GetExpPoints(player, exp_ID);
		local int   skinning_level 			= module_exp.GetLevel(skinning_exp_value, exp_ID);
		local float skinning_efficiency 	= module_exp.GetExpParamNumber(player, exp_ID, "efficiency");
		
		module_exp.Increase(player, exp_ID);
		
		return skinning_efficiency;
	}

	// Spawns an organ defined in the given config
	void CreateOrgan(PlayerBase player, string item_to_spawn, string cfg_skinning_organ_class, float skinning_efficiency, float tool_damage)
	{
		// Create item from config
		local ItemBase added_item = GetGame().CreateObject(item_to_spawn, player.GetPosition(), false);
		
		// Set item's quantity from config, if it's defined there.
		local float item_quantity = g_Game.ConfigGetFloat(cfg_skinning_organ_class + "quantity");
		
		if (item_quantity > 0)
		{
			added_item.SetQuantity2(item_quantity, false);
		}
		
		local autoptr TFloatArray item_quantity_MinMax = new TFloatArray;
		g_Game.ConfigGetFloatArray(cfg_skinning_organ_class + "quantityMinMax", item_quantity_MinMax); // out item_quantity_MinMax
		
		if (item_quantity_MinMax.Count() > 0)
		{
			// Sets item's quantity while taking player's skill into account
			local float max_quantity = item_quantity_MinMax.Get(1) * skinning_efficiency;
			local float min_quantity = item_quantity_MinMax.Get(0) + (((max_quantity - item_quantity_MinMax.Get(0)) * skinning_efficiency) - (item_quantity_MinMax.Get(0) * skinning_efficiency));
			
			added_item.SetQuantity2(Math.RandomFloat(min_quantity, max_quantity), false);
			
			if (item_quantity > 0) 
			{
				local string warning_message = "WARNING! Member <" + cfg_skinning_organ_class + "quantityMinMax" + "> is ignored. It is pointless to have both members 'quantity' and 'quantityMinMax' defined there. One of them should be removed!";
				DPrint(warning_message);
			}
		}
		
		// Transfer tool's damage to the item's condition
		local int item_apply_damage = g_Game.ConfigGetInt(cfg_skinning_organ_class + "transferToolDamage");
		
		if (item_apply_damage == 1)
		{
			added_item.SetDamage( tool_damage );
		}
	}
};