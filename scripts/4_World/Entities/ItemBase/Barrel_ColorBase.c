class EN5C_Barrel_ColorBase : EN5C_Container_Base
{
	private bool m_IsLocked = false;
	private autoptr Timer m_BarrelOpener;
	autoptr RainProcurementManager m_RainProcurement;
	
	void EN5C_Barrel_ColorBase()
	{
		m_Opened = false;
		m_BarrelOpener = new Timer();	
	}
	
	bool IsLocked()
	{
		return m_IsLocked;
	}

	bool IsOpened()
	{
		if ( GetGame().ObjectGetAnimationPhase( this,"Lid") == 1 )
		{
			return true;
		}
		
		return false;
	}
	
	void Open()
	{
		super.Open();
		GetGame().ObjectSetAnimationPhase(this,"Lid",1);
		GetGame().ObjectSetAnimationPhase(this,"Lid2",0);	
		SetVariable("lidopen",true);
		m_RainProcurement = new RainProcurementManager( this );
		m_RainProcurement.InitRainProcurement();		
	}
	
	void Lock(float actiontime)
	{
		m_IsLocked = true;
		m_BarrelOpener.Run(actiontime, this, "Unlock", NULL,false);
	}
	
	void Unlock()
	{
		m_IsLocked = false;
		Open();
	}
	
	void Close()
	{
		super.Close();
		GetGame().ObjectSetAnimationPhase(this,"Lid",0);
		GetGame().ObjectSetAnimationPhase(this,"Lid2",1);
		SetVariable("lidopen",false);
		m_RainProcurement.StopRainProcurement();
		delete m_RainProcurement;
	}
	
	void DetermineAction ( PlayerBase player )
	{
		int slot_id;
		ItemBase ingredient; 
		slot_id = GetGame().GetInventorySlot("Lime");
		ingredient = FindAttachment(slot_id); 
		if ( ingredient && GetQuantity2() > 10000 ) 
		{
			TanPlets(ingredient, player);
		}
		else
		{
			slot_id = GetGame().GetInventorySlot("Nails");
			ingredient = FindAttachment(slot_id); 
			if ( ingredient && GetQuantity2() > 10000 ) 
			{
				ColourLeatherClothes(ingredient, player, "Black");
			}
			else
			{
				slot_id = GetGame().GetInventorySlot("BirchBark");
				ingredient = FindAttachment(slot_id); 
				if ( ingredient && GetQuantity2() > 10000 ) 
				{
					ColourLeatherClothes(ingredient, player, "Beige");
				}
				else
				{
					slot_id = GetGame().GetInventorySlot("OakBark");
					ingredient = FindAttachment(slot_id); 
					if ( ingredient && GetQuantity2() > 10000 ) 
					{
						ColourLeatherClothes(ingredient, player, "Brown");	
					}
					else
					{
						
						slot_id = GetGame().GetInventorySlot("Disinfectant");
						ingredient = FindAttachment(slot_id); 
						if ( ingredient && GetQuantity2() > 10000 )
						{
							BleachClothes(ingredient, player);
						}
						else
						{
							slot_id = GetGame().GetInventorySlot("Plant");
							ingredient = FindAttachment(slot_id); 
							slot_id = GetGame().GetInventorySlot("Guts");
							ItemBase reactant = FindAttachment(slot_id); 
							if ( ingredient && reactant && GetQuantity2() > 5000 )
							{
								ProduceFertilizer(ingredient,reactant,player);
							}
							else
							{
								slot_id = GetGame().GetInventorySlot("BerryR");
								ItemBase ingredientR = FindAttachment(slot_id); 
								slot_id = GetGame().GetInventorySlot("Plant");
								ItemBase ingredientG = FindAttachment(slot_id); 
								slot_id = GetGame().GetInventorySlot("BerryB");
								ItemBase ingredientB = FindAttachment(slot_id); 
								if ( (ingredientR || ingredientG || ingredientB) && GetQuantity2() > 10000) 
								{
									ColourClothes(ingredientR,ingredientG,ingredientB,player);	
								}
							}
						}
					}
				}
			}
		}
		Close();
	}
	
	void ProduceFertilizer( ItemBase guts, ItemBase plant, PlayerBase player )
	{
		ItemBase lime = GetGame().CreateObject("EN5C_GardenLime", player.GetPosition());
		float lime_quantity = plant.GetQuantity2()*BAREL_LIME_PER_PLANT;
		
		lime.SetQuantity2(lime_quantity);
		TakeEntityToCargo(lime);
		guts.Delete();
		plant.Delete();
		Lock(50);
	}
	
	void TanPlets( ItemBase lime, PlayerBase player )
	{
		EntityAI item;		
		int item_count = GetCargo().GetItemCount();
		int pelt_count = 0;
		int lime_amount = Math.Floor(lime.GetQuantity2()/BAREL_LIME_PER_PELT);
		
			
		for (int i = 0; i < item_count; i++)
		{
			item = GetCargo().GetItem(i);
			if ( item.IsPeltBase() )
			{
				pelt_count = g_Game.ConfigGetInt("cfgVehicles " + item.GetType() + " peltGain");
				if ( pelt_count <= lime_amount )
				{
					ItemBase tanned_pelt = GetGame().CreateObject("EN5C_TannedLeather", player.GetPosition());
					MiscGameplayFunctions.TransferItemProperties(item, tanned_pelt, true, false, true);
					tanned_pelt.SetQuantity2(pelt_count);
					TakeEntityToCargo(tanned_pelt);
					item.Delete();
					lime_amount -= pelt_count;
					if ( lime_amount <= 0 )
					{
						lime.Delete();
						break;
					}
				}
			}
		}	
		if ( lime ) lime.SetQuantity2(lime_amount*BAREL_LIME_PER_PELT);		
		if ( pelt_count > 0 ) Lock(30);
	}
	
	void ColourLeatherClothes( ItemBase color_source, PlayerBase player, string color )
	{
		EntityAI item;		
		int item_count = GetCargo().GetItemCount();
		string item_name = "";
		
		
		for (int i = 0; i < item_count; i++)
		{
			item = GetCargo().GetItem(i);
			if ( item.IsClothing() )
			{	
				if ( GetGame().ObjectIsKindOf (item, "EN5C_LeatherSack_Natural") ) item_name = "Bag_LeatherSack_"+color;
				else if ( GetGame().ObjectIsKindOf (item, "EN5C_LeatherStorageVest_Natural") ) item_name = "Vest_LeatherStorage_"+color;
				else if ( GetGame().ObjectIsKindOf (item, "EN5C_LeatherJacket_Natural") ) item_name = "Top_LeatherJacket_"+color;
				else if ( GetGame().ObjectIsKindOf (item, "EN5C_LeatherPants_Natural") ) item_name = "Pants_LeatherPants_"+color;
				else if ( GetGame().ObjectIsKindOf (item, "EN5C_LeatherMoccasinsShoes_Natural") ) item_name = "Shoes_LeatherMoccasins_"+color;
				else if ( GetGame().ObjectIsKindOf (item, "EN5C_LeatherHat_Natural") ) item_name = "Hat_Leather_"+color;
				if ( item_name != "" )
				{
					ItemBase leather = GetGame().CreateObject(item_name, player.GetPosition());
					MiscGameplayFunctions.TransferItemProperties(item, leather, true, false, true);
					TakeEntityToCargo(leather);
					item.Delete();
				}				
			}	
		}	
		if ( item_name != "" ) Lock(20);	
	}
	
	
	void BleachClothes( ItemBase bleach, PlayerBase player )
	{
		EntityAI item;	
		int bleach_amount = bleach.GetQuantity2();
		int item_count = GetCargo().GetItemCount();
		string item_name = "";
		
		for (int i = 0; i < item_count; i++)
		{
			item = GetCargo().GetItem(i);
			if ( item.IsClothing() )
			{	
				if ( bleach_amount >= BAREL_BLEACH_PER_CLOTH )
				{
					if ( GetGame().ObjectIsKindOf (item, "EN5C_TShirt_White") || GetGame().ObjectIsKindOf (item, "EN5C_Armband_White") )
					{
						item.SetObjectTexture ( 0, "#(argb,8,8,3)color(0.8,0.8,0.8,1.0)" );
						item.SetObjectTexture ( 1, "#(argb,8,8,3)color(0.8,0.8,0.8,1.0)" );
						item.SetObjectTexture ( 2, "#(argb,8,8,3)color(0.8,0.8,0.8,1.0)" );
						item.SetVariable("color","0.8,0.8,0.8");
						bleach_amount -= BAREL_BLEACH_PER_CLOTH;
					}
					if ( GetGame().IsKindOf (item.GetName(), "EN5C_LeatherSack_ColorBase") ) item_name = "EN5C_LeatherSack_Natural";
					else if ( GetGame().IsKindOf (item.GetName(), "EN5C_LeatherStorageVest_ColorBase") ) item_name = "EN5C_LeatherStorageVest_Natural";
					else if ( GetGame().IsKindOf (item.GetName(), "EN5C_LeatherJacket_ColorBase") ) item_name = "EN5C_LeatherJacket_Natural";
					else if ( GetGame().IsKindOf (item.GetName(), "EN5C_LeatherPants_ColorBase") ) item_name = "EN5C_LeatherPants_Natural";
					else if ( GetGame().IsKindOf (item.GetName(), "EN5C_LeatherMoccasinsShoes_ColorBase") ) item_name = "EN5C_LeatherMoccasinsShoes_Natural";
					else if ( GetGame().IsKindOf (item.GetName(), "EN5C_LeatherHat_ColorBase") ) item_name = "EN5C_LeatherHat_Natural";
					if ( item_name != "" )
					{
						ItemBase leather = GetGame().CreateObject(item_name, player.GetPosition());
						MiscGameplayFunctions.TransferItemProperties(item, leather, true, false, true);
						TakeEntityToCargo(leather);
						item.Delete();
						bleach_amount -= BAREL_BLEACH_PER_CLOTH;
					}
				}
				else
				{
					bleach.Delete();
					break;
				}
			}	
		}	
		if ( bleach ) bleach.SetQuantity2(bleach_amount);		
		if ( item_name != "" ) Lock(10);
	}		
	
	void ColourClothes( ItemBase rci, ItemBase gci, ItemBase bci, PlayerBase player )
	{
		EntityAI item;		
		float r = 0;
		float g = 0;
		float b = 0;
		int item_count = GetCargo().GetItemCount();
		bool was_colored = false;
		if ( rci ) 
		{
			r = rci.GetQuantity2()/10; 
		}
		if ( gci )
		{ 
			g = gci.GetQuantity2()/10; 
		}
		if ( bci ) 
		{
			b = bci.GetQuantity2()/10; 
		}
		for (int i = 0; i < item_count; i++)
		{
			item = GetCargo().GetItem(i);
			if ( item.IsClothing() )
			{	
				if ( GetGame().ObjectIsKindOf (item, "EN5C_TShirt_White") || GetGame().ObjectIsKindOf (item, "EN5C_Armband_White") )
				{
					string save_color = r.ToString()+","+g.ToString()+","+b.ToString();
					string color = "#(argb,8,8,3)color("+save_color+",1.0,CO)";
					item.SetObjectTexture(0, color);
					item.SetObjectTexture(1, color);
					item.SetObjectTexture(2, color);					
					item.SetVariable("color",save_color);
					was_colored = true;
				}
			}	
		}	
		if ( was_colored ) 
		{
			if ( rci ) rci.Delete();
			if ( gci ) gci.Delete();
			if ( bci ) bci.Delete();
			Lock(30);
		}
	}
}
