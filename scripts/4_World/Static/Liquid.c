class Liquid
{
	//---------------------------------------------------------------------------------------------------------
	static void Transfer(EntityAI source_ent, EntityAI target_ent)
	{
		if( !Liquid.CanTransfer(source_ent, target_ent) ) return;
		
		ItemBase source = source_ent;
		ItemBase target = target_ent;
		
		
		Debug.Log("Transfer, source:"+source.ToString()+" target: "+target.ToString(), "LiquidTransfer");
		
		float source_quantity 	= source.GetQuantity2();
		float target_quantity 	= target.GetQuantity2();
		int source_liquid_type 	= source.GetLiquidType2();
		int target_liquid_type 	= target.GetLiquidType2();
		int target_mask 		= target_ent.ConfigGetFloat("liquidContainerType");
		int source_mask 		= source_ent.ConfigGetFloat("liquidContainerType");
		
		float available_capacity = target.GetQuantityMax2() - target_quantity;
		
		Debug.Log("target_mask ="+target_mask.ToString(), "LiquidTransfer");
		Debug.Log("source_mask ="+source_mask.ToString(), "LiquidTransfer");
		Debug.Log("source_liquid_type ="+source_liquid_type.ToString(), "LiquidTransfer");
		Debug.Log("target_liquid_type ="+target_liquid_type.ToString(), "LiquidTransfer");
		

		//target.SetLiquidType2(source_liquid_type);//override target liquidType
		float quantity_to_transfer = Math.Clamp(source_quantity,0,available_capacity);
		//target.AddQuantity2(quantity_to_transfer);
		PluginTransmissionAgents m_mta = GetPlugin(PluginTransmissionAgents);
		m_mta.TransmitAgents(source_ent, target_ent, AGT_TRANSFER_LIQUID);

		source.AddQuantity2(-quantity_to_transfer);
		
		Liquid.FillContainer(target_ent,source_liquid_type,quantity_to_transfer);
		

	}
	
	static bool CanTransfer(EntityAI source_ent, EntityAI target_ent)
	{
	
		if( !source_ent.IsItemBase() || !target_ent.IsItemBase() )
		{
			//Debug.Log("One of the Items is not of ItemBase type", "LiquidTransfer");
			return false;
		}
		
		ItemBase source = source_ent;
		ItemBase target = target_ent;

		
		float source_quantity = source.GetQuantity2();
		if( source_quantity <= 0 ) 
		{
			//Debug.Log("source has no quantity", "LiquidTransfer");
			return false;//if there is nothing to transfer
		}
		
		int source_liquid_type 	= source.GetLiquidType2();	
		if( source_liquid_type < 1 ) 
		{
			//Debug.Log("source has some quantity, but does not have a valid liquidType set, liquidType = "+ToString(source_liquid_type), "LiquidTransfer");
			return false;//if source is not a container
		}
		
		if(!CanFillContainer(target_ent,source_liquid_type) )
		{
			return false;
		}
		
		
		return true;
	}
	static void FillContainer(ItemBase container, int liquid_type, float amount)
	{
		if(!CanFillContainer(container,liquid_type) )
		{
			return;
		}
		//filling
		container.SetLiquidType2(liquid_type);
		container.AddQuantity2(amount); 
		
	}
	
	static bool CanFillContainer(ItemBase container, int liquid_type)
	{
		if(!container) return false;
		bool is_container_full = container.IsFullQuantity2();
		
		if( is_container_full )
		{
			//Debug.Log("container is full", "LiquidTransfer");
			return false;
			
		}
		int container_mask 	= container.ConfigGetFloat("liquidContainerType");
		
		if( container_mask == 0 )
		{
			//Debug.Log("target is not a container", "LiquidTransfer");
			return false;//if the target liquidContainerType is set to 0
		}
		
		if( (liquid_type & container_mask) == 0 )
		{
			//Debug.Log("target liquidContainerType does not support this liquid type", "LiquidTransfer");
			return false;
		}
		
		float container_quantity 	= container.GetQuantity2();
		
		int container_liquid_type 	= container.GetLiquidType2();
		
		if( container_quantity > 0 && container_liquid_type != liquid_type) 
		{
			//Debug.Log("target is not empty AND is of different liquid type than liquid_type added in", "LiquidTransfer");
			return false;
		}
		return true;
	}
	
	private static string GetLiquidConfigProperty(int liquid_type, string property_name, bool is_nutrition_property = false)
	{
		string cfg_classname = "cfgLiquidDefinitions";
		string property_value = "NULL_PROPERTY";
		int cfg_item_count = g_Game.ConfigGetChildrenCount(cfg_classname);

		for ( int i = 0; i < cfg_item_count; i++ )
		{
			string liquid_class_name;
			GetGame().ConfigGetChildName(cfg_classname, i, liquid_class_name);
			string liquid_full_path = cfg_classname + " " + liquid_class_name;
			int config_liquid_type = GetGame().ConfigGetInt(liquid_full_path + " " + "type");
			
			if( config_liquid_type == liquid_type )// found the specific class, now lets extract the values
			{
				if(!is_nutrition_property) 
				{
					GetGame().ConfigGetText(liquid_full_path + " " + property_name,property_value);
					return property_value;
				}
				else
				{
					GetGame().ConfigGetText(liquid_full_path + " Nutrition " + property_name,property_value);
					return property_value;
				}
			}
		}
		return property_value;
	}
	
	static float GetToxicity(int liquid_type)
	{
		return Liquid.GetLiquidConfigProperty(liquid_type, "intoxication").ToFloat();
	}
	
	static float GetWaterContent(int liquid_type)
	{
		return Liquid.GetLiquidConfigProperty(liquid_type, "water", true).ToFloat();
	}
	
	static float GetEnergy(int liquid_type)
	{
		return Liquid.GetLiquidConfigProperty(liquid_type, "energy", true).ToFloat();
	}
	
	static float GetNutritionalIndex(int liquid_type)
	{
		return Liquid.GetLiquidConfigProperty(liquid_type, "nutritionalIndex", true).ToFloat();
	}
	
	static string GetName(int liquid_type)
	{
		return Liquid.GetLiquidConfigProperty(liquid_type, "name");
	}
	
	static float GetFlammability(int liquid_type)
	{
		return Liquid.GetLiquidConfigProperty(liquid_type, "flammability").ToFloat();
	}
	
	static float GetVolume(int liquid_type)
	{
		return Liquid.GetLiquidConfigProperty(liquid_type, "totalVolume", true).ToFloat();
	}
	
};