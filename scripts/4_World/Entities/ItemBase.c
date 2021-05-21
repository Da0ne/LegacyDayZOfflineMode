class ItemBase;
typedef ItemBase EN5C_Inventory_Base;

class ItemBase extends InventoryItem
{
	
	private float	m_Quantity;
	private float	m_QuantityMax;
	private bool	m_QuantityCanCombine;
	private bool	m_QuantityCanSplit;
	

	
	// ============================================
	// Variable Manipulation System
	// ============================================
	autoptr map<string,string> 	m_ItemVarsString;
	
	int		m_VariablesMask;//this holds information about which vars have been changed from their default values
	float 	m_VarQuantity;
	float 	m_VarEnergy;
	float 	m_VarTemperature;
	float 	m_VarWet;
	int 	m_VarBloodType;
	int 	m_VarLiquidType;
	
	//open / close actions
	bool m_Opened = true;
	
	
	
	//==============================================
	// agent system
	private int	m_AttachedAgents;

	// Declarations
	void TransferModifiers( PlayerBase reciever );
	
	// -------------------------------------------------------------------------
	void ItemBase()
	{
		SetEventMask(EntityEvent.INIT); // Enable EOnInit event
		
		string class_path = "cfgVehicles " + this.GetType();
		string quantity_path =  class_path + " Quantity";
		
		m_QuantityMax = g_Game.ConfigGetInt(class_path + " stackedMax");
		
		//m_QuantityMax = g_Game.ConfigGetInt(quantity_path + " quantityMax");
		m_QuantityCanCombine = g_Game.ConfigGetInt(quantity_path + " quantityCanCombine");
		m_QuantityCanSplit = g_Game.ConfigGetInt(quantity_path + " quantityCanSplit");
	}
	// -------------------------------------------------------------------------
	void ~ItemBase()
	{
	}

	// -------------------------------------------------------------------------

	
	void CombineItemsClient(ItemBase entity2)
	{
		autoptr Param1<EntityAI> item = new Param1<EntityAI>(entity2);
		RPCSingleParam( RPC_ITEM_COMBINE,item,GetGame().GetPlayer() );
	}
	
	bool IsLiquidPresent()
	{
		
		if(GetLiquidType2() != 0) return true;
		else return false;
	}
	
	float GetNutritionalEnergy()
	{
		if( !IsLiquidPresent() )
		{
			string class_path = "cfgVehicles " + this.GetType() + " Nutrition";
			return g_Game.ConfigGetFloat(class_path + " energy");
		}
		else
		{
			return Liquid.GetEnergy( GetLiquidType2() );
		}
	}
	
	float GetNutritionalWaterContent()
	{
		if( !IsLiquidPresent() )
		{
			string class_path = "cfgVehicles " + this.GetType() + " Nutrition";
			return g_Game.ConfigGetFloat(class_path + " water");
		}
		else
		{
			return Liquid.GetWaterContent( GetLiquidType2() );
		}
	}
	
	float GetNutritionalIndex()
	{
		if( !IsLiquidPresent() )
		{
			string class_path = "cfgVehicles " + this.GetType() + " Nutrition";
			return g_Game.ConfigGetFloat(class_path + " nutritionalIndex");
		}
		else
		{
			return Liquid.GetNutritionalIndex( GetLiquidType2() );
		}
	}
	
	float GetNutritionalTotalVolume()
	{
		if( !IsLiquidPresent() )
		{
			string class_path = "cfgVehicles " + this.GetType() + " Nutrition";
			return g_Game.ConfigGetFloat(class_path + " totalVolume");
		}
		else
		{
			return Liquid.GetVolume( GetLiquidType2() );
		}
	}
	
	void ProcessNutritions( PlayerBase reciever, float consumedquantity )
	{	
		float energy = GetNutritionalEnergy();
		float waterratio = GetNutritionalWaterContent();
		float nutritions = GetNutritionalIndex();
		float consumedwater = (consumedquantity * waterratio);
		
		//this will be used, when configs are properly set
		/*
		reciever.m_PlayerStats.m_Stomach.Add((consumedquantity - consumedwater));
		reciever.m_PlayerStats.m_StomachEnergy.Add((consumedquantity * energy));
		reciever.m_PlayerStats.m_StomachWater.Add(consumedwater);
		*/
		
		//will be removed later
		float volume = GetNutritionalTotalVolume();
		if ( volume > 0 )
		{
			reciever.m_PlayerStats.m_StomachEnergy.Add((consumedquantity * (energy / volume) ));
			reciever.m_PlayerStats.m_StomachWater.Add( consumedquantity * (consumedwater / volume) );
		}
		else
		{
			Print("ZERO VOLUME! Fix config");
		}
	}

	// -------------------------------------------------------------------------
	void EEItemLocationChanged(EntityAI old_owner, EntityAI new_owner)
	{
		 Man owner_player_old = NULL;
		 Man owner_player_new = NULL;
		 if (old_owner)   
		 {      
			if (old_owner.IsMan())      
			{         
				owner_player_old = old_owner;
			}      
			else      
			{
				owner_player_old = old_owner.GetOwnerPlayer();
			}   
		}   
		if (new_owner)
		{     
			if ( new_owner.IsMan() )
			{
				owner_player_new = new_owner;
			}      
			else      
			{
				owner_player_new = new_owner.GetOwnerPlayer();
			}   
		}   
		if ( !owner_player_new && owner_player_old )   
		{
			OnInventoryExit(owner_player_old);
			// Debug.Log("Item dropped from inventory");   
		}   
		if ( owner_player_new && !owner_player_old )
		{
			OnInventoryEnter(owner_player_new);
			// Debug.Log("Item taken to inventory");
		}
		
		if ( HasEnergyManager() )
		{
			GetCompEM().UpdatePlugState(); // Unplug the el. device if it's necesarry.
		}
	}

	// -------------------------------------------------------------------------------
	void EOnInit(IEntity other, int extra)
	{
		
	}
	
	// -------------------------------------------------------------------------------
	void EEKilled( Object killer)
	{
		super.EEKilled( killer );
		
		//if item is able to explode in fire
		if ( CanExplodeInFire() )
		{
			float min_temp_to_explode	= 100;		//min temperature for item to explode
			
			if ( GetTemperature2() >= min_temp_to_explode )		//TODO ? add check for parent -> fireplace
			{
				if ( IsMagazine() )
				{
					Magazine magazine = ( Magazine ) this;					
					
					//explode ammo
					if ( magazine.GetAmmoCount() > 0 )
					{
						ExplodeAmmo();
					}
				}
				else
				{
					//explode item
					Explode();
				}
			}
		}
	}
	
	void ExplodeAmmo()
	{
		//timer
		autoptr Timer explode_timer = new Timer( CALL_CATEGORY_SYSTEM );
		
		//min/max time
		float min_time = 1;
		float max_time = 3;
		float delay = Math.RandomFloat( min_time, max_time );
		
		explode_timer.Run( delay, this, "DoAmmoExplosion" );
	}
	
	void DoAmmoExplosion()
	{
		Magazine magazine = ( Magazine ) this;
		int pop_sounds_count = 6;
		string pop_sounds[ 6 ] = { "ammopops_1","ammopops_2","ammopops_3","ammopops_4","ammopops_5","ammopops_6" };
		
		//play sound
		int sound_idx = Math.RandomInt( 0, pop_sounds_count - 1 );
		string sound_name = pop_sounds[ sound_idx ];
		GetGame().CreateSoundOnObject( this, sound_name, 20, false );
		
		//remove ammo count
		magazine.AddAmmoCount( -1 );
		
		//if condition then repeat -> ExplodeAmmo
		float min_temp_to_explode	= 100;		//min temperature for item to explode
				
		if ( magazine.GetAmmoCount() > 0 && GetTemperature2() >= min_temp_to_explode )	//TODO ? add check for parent -> fireplace
		{
			ExplodeAmmo();
		}
	}

	// -------------------------------------------------------------------------------
	void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, string component, string ammo, vector modelPos)
	{
		if ( IsClothing() || IsContainer() )
		{
			DamageItemsInCargo(damageResult);
		}
	}

	void DamageItemsInCargo(TotalDamageResult damageResult)
	{	
		if ( GetCargo() )
		{
			int item_count = GetCargo().GetItemCount();
			if ( item_count > 0 )
			{
				int random_pick = Math.RandomInt(0, item_count);
				ItemBase item = GetCargo().GetItem(random_pick);
				float dmg = damageResult.GetDamage("","Health") / -2;
				item.AddHealth("","",dmg);
			}
		}
	}


	bool CanBeSplit()
	{
		if(!GetGame().IsNewPlayer() ) return false;
		
		if( ConfigGetBool("canBeSplit") )
		{
			if( GetQuantity2() > 1 )
			{
				return true;
			}
			else 
			{
				return false;
			}
			
		}
		
		return false;
	}



	void SplitItem()
	{
		if ( !CanBeSplit() )
		{
			return;
		}
		
		float quantity = GetQuantity2();
		float split_quantity_new = Math.Floor( quantity / 2 );
		
		AddQuantity2(-split_quantity_new);
		PlayerBase player = this.GetOwnerPlayer();
		ItemBase new_item = NULL;
		if( player )
		{
			new_item = player.CopyInventoryItem( this );
		}
		else
		{
			new_item = GetGame().CreateObject(this.GetType(), this.GetPosition() );
		}
		
		MiscGameplayFunctions.TransferItemProperties(this,new_item);
		
		new_item.SetQuantity2( split_quantity_new );

		
		
	}
	
	void OnRightClick()
	{
		super.OnRightClick();
		
		if( CanBeSplit() && !GetDayZGame().IsLeftCtrlDown() )
		{
			RPCSingleParam( RPC_ITEM_SPLIT,NULL,GetGame().GetPlayer() );
		}
		
	}
	
	bool CanBeCombined(ItemBase other_item, PlayerBase player = NULL)
	{
		if(!GetGame().IsNewPlayer() ) return false;
		bool can_this_be_combined =  ConfigGetBool("canBeSplit");
		
		if(!can_this_be_combined) return false;
		if( player != NULL && (player.GetEntityInHands() == this || player.GetEntityInHands() == other_item )) return false;
		bool is_same_type = ( this.GetType() == other_item.GetType() );
		if( is_same_type )
		{
			return true;			
		}
		else
		{
			return false;		
		}
	}
	
	void CombineItems( ItemBase other_item )
	{
		if(!CanBeCombined(other_item)) return;
		if( !IsMagazine() )
		{
			float other_item_quantity = other_item.GetQuantity2();
			float this_free_space = GetQuantityMax2() - GetQuantity2();
			float quantity_used = 0;
			if( other_item_quantity > this_free_space )
			{
				quantity_used = this_free_space;
			}
			else
			{
				quantity_used = other_item_quantity;
			}
			if( quantity_used!= 0 )
			{
				this.AddQuantity2(quantity_used);
				other_item.AddQuantity2(-quantity_used);
			}
		}		
	}
	// -------------------------------------------------------------------------
	bool AddAction(int action_id, ActionMenuContext ctx, Man player, int action_type)
	{
		/*ActionBase action = this.GetOwnerPlayer().m_ActionManager.GetAction(action_id);
		if (action && action.CanDo(player, player, this))
		{
			ctx.AddAction(action.GetText(), action_id, new Param1<int>(action_type), 1000, true, true);
			return true;
		}
		return false;*/
	}
	 
	// -------------------------------------------------------------------------
	void OnGetActions (ActionMenuContext ctx, Man player)
	{  	
		super.OnGetActions(ctx, player);
		if ( GetDayZGame().IsNewPlayer() )
		{
			/*
			PlayerBase nplayer = player;
			if ( nplayer.m_CurrentTertiaryAction )
			{
				ctx.AddAction(nplayer.m_CurrentTertiaryActionText, 109, NULL, 1000, false, false);
			}
			*/
	
			ManBase mb_player = player;
			if( this == mb_player.GetCursorObject() )
			{
				PlayerBase p = player;
				map<int,string> recipesMap = p.m_Recipes;
				PluginRecipesManager module_recipes_manager = GetPlugin(PluginRecipesManager);
				module_recipes_manager.GetValidRecipes(this,player.GetEntityInHands(),recipesMap, player);

				int key = 0;
				
				for(int i = 0;i < recipesMap.Count();i++)
				{
					key = recipesMap.GetKey(i);
					int idWithOffset = key + 1000;
					ctx.AddAction(recipesMap.Get(key), idWithOffset, NULL, 100+key, false, false);
				}
				
			}
			
			if ( GetDayZGame().IsDebugActions() )
			{	
				ctx.AddAction("InjectInfluenza", 111, NULL, 1000, false, false);
				ctx.AddAction("InjectCholera", 141, NULL, 1000, false, false);
				ctx.AddAction("SetMaxQuantity", 112, NULL, 1024, false, false);
				//ctx.AddAction("PrintQuantityMax", 114, NULL, 1000, false, false);
				ctx.AddAction("Quantity -10%", 115, NULL, 1001, false, false);
				ctx.AddAction("Quantity 10%", 140, NULL, 1001, false, false);
				//ctx.AddAction("Print Classname", 116, NULL, 1000, false, false);
				//ctx.AddAction("Has In Cargo", 117, NULL, 1000, false, false);
				//ctx.AddAction("Print Health(new)", 118, NULL, 1000, false, false);
				//ctx.AddAction("Quantity -1", 119, NULL, 1004, false, false);
				//ctx.AddAction("IsMagazine", 120, NULL, 1005, false, false);
				//ctx.AddAction("Ammo +1", 121, NULL, 1006, false, false);
				//ctx.AddAction("Set Health 1", 122, NULL, 1002, false, false);
				//ctx.AddAction("Ammo -1", 123, NULL, 1007, false, false);
				//ctx.AddAction("Ammo SetMax", 124, NULL, 1008, false, false);
				ctx.AddAction("Inject String Tiger", 125, NULL, 1009, false, false);
				ctx.AddAction("Inject String Rabbit", 126, NULL, 1010, false, false);
				//ctx.AddAction("Predend Consume",127,NULL, 1011,false,false);
				//ctx.AddAction("IsEmpty",128,NULL, 1012,false,false);
				//ctx.AddAction("HasAnyCargo",129,NULL, 1013,false,false);
				//ctx.AddAction("LightOn",130,NULL, 1013,false,false);
				//ctx.AddAction("Set Health 200", 131, NULL, 1003, false, false);
				ctx.AddAction("Add Health 10", 132, NULL, 1020, false, false);
				ctx.AddAction("Add Health -10", 133, NULL, 1021, false, false);
				//ctx.AddAction("LightOff",134,NULL, 1022,false,false);
				ctx.AddAction("SetQuantity0", 135, NULL, 1023, false, false);
				ctx.AddAction("Watch",136,NULL,2000,false,false);
				ctx.AddAction("ShowID",142,NULL,2000,false,false);
				ctx.AddAction("RemoveInfluenza",143,NULL,2000,false,false);
			}
		}
	}

	// -------------------------------------------------------------------------
	bool OnAction(int action_id, Man player, ParamsReadContext ctx)
	{
		super.OnAction(action_id, player, ctx);
		if ( GetDayZGame().IsNewPlayer() )
		{
			if(action_id >= 1000)
			{
				int idWithOffset = action_id - 1000;
				PlayerBase p = player;
				if( action_id  < 1000 )
				{
					p.SetUpCrafting( idWithOffset, this, player.GetOwnerPlayer().GetEntityInHands() );
				}
				else//this part is for the [DEBUG] actions
				{
					PluginRecipesManager plugin_recipes_manager = GetPlugin(PluginRecipesManager);
					plugin_recipes_manager.PerformRecipeClientRequest( idWithOffset, this, player.GetOwnerPlayer().GetEntityInHands() );
				}
			}
		
			/*
			if( action_id == 109 ) //Ineract
			{
				PlayerBase nplayer = player;
				nplayer.OnInteractMenu();
			}
			*/
			
			if( action_id == 111 ) InsertAgent(AGT_INFLUENZA,100);//InjectInfluenza
			if( action_id == 141 ) InsertAgent(AGT_CHOLERA,100);//InjectCholera
			if( action_id == 112 ) //SetMaxQuantity
			{
				SetQuantityMax2();
			}
			if( action_id == 114 )//PrintQuantityMax
			{
				Debug.Log("max quantity for item " + this.ToString() + " is: " + GetQuantityMax2().ToString(),"ItemVars");
			}
			if( action_id == 115 ) //Quantity -10%
			{
				AddQuantity2(GetQuantityMax2()/-10);
				//PrintVariables();
			}
			if( action_id == 116 ) 
			{
				Debug.Log("classname:"+this.GetType(),"recipes");
			}
			if( action_id == 117 ) 
			{
				if( player.HasEntityInCargo(this) )
				{
					Debug.Log("+this item is in player's cargo space","recipes");
				}
				else
				{
					Debug.Log("-this item is NOT player's cargo space","recipes");
				}
			}
			if( action_id == 118 ) 
			{
				Debug.Log(this.GetHealth("","").ToString());
			}
			if( action_id == 119 ) 
			{
				this.AddQuantity2(-1,false,true);
			}
			if( action_id == 120 ) 
			{
				Debug.Log("IsMagazine:"+this.IsMagazine().ToString(),"recipes");
			}
			if( action_id == 121 ) 
			{
				if( IsMagazine() )
				{
					Magazine mag = this;
					mag.SetAmmoCount(mag.GetAmmoCount() + 1);
				}
			}
			if( action_id == 122 ) 
			{
				this.SetHealth("","",1);
			}
			
			if( action_id == 123 ) 
			{
				if( IsMagazine() )
				{
					Magazine mgz = this;
					mgz.AddAmmoCount(-1);
				}
			}
			if( action_id == 124 ) 
			{
				if( IsMagazine() )
				{
					Magazine mgz2 = this;
					mgz2.SetAmmoCount(mgz2.GetAmmoMax());
				}
			}
			if( action_id == 125 ) 
			{
				SetItemVariableString("varNote", "The tiger (Panthera tigris) is the largest cat species, most recognisable for their pattern of dark vertical stripes on reddish-orange fur with a lighter underside. The largest tigers have reached a total body length of up to 3.38 m (11.1 ft) over curves and have weighed up to 388.7 kg (857 lb) in the wild. The species is classified in the genus Panthera with the lion, leopard, jaguar and snow leopard. Tigers are apex predators, primarily preying on ungulates such as deer and bovids. They are territorial and generally solitary but social animals, often requiring large contiguous areas of habitat that support their prey requirements. This, coupled with the fact that they are indigenous to some of the more densely populated places on Earth, has caused significant conflicts with humans.");
			}
			if( action_id == 126 ) 
			{
				SetItemVariableString("varNote", "Rabbits are small mammals in the family Leporidae of the order Lagomorpha, found in several parts of the world. There are eight different genera in the family classified as rabbits, including the European rabbit (Oryctolagus cuniculus), cottontail rabbits (genus Sylvilagus; 13 species), and the Amami rabbit (Pentalagus furnessi, an endangered species on Amami ?shima, Japan). There are many other species of rabbit, and these, along with pikas and hares, make up the order Lagomorpha. The male is called a buck and the female is a doe; a young rabbit is a kitten or kit.");
			}
			if( action_id == 127 ) 
			{
				PluginTransmissionAgents m_mta = GetPlugin(PluginTransmissionAgents);
				m_mta.TransmitAgents(this, player, AGT_UACTION_CONSUME);
			}
			if( action_id == 128 ) 
			{
				Debug.Log(this.IsEmpty().ToString() );
			}
			if( action_id == 129 ) 
			{
				Debug.Log( "Has any cargo = "+this.HasAnyCargo().ToString() );
			}

			if( action_id == 130 ) 
			{	
				if (HasEnergyManager())
				{
					GetCompEM().SwitchOn();
				}
			}
			
			if( action_id == 131 ) 
			{
				this.SetHealth("","",200);
			}
			if( action_id == 132 ) 
			{
				this.AddHealth("","",10);
			}
			if( action_id == 133 ) 
			{
				this.AddHealth("","",-10);
			}
			if( action_id == 134 ) 
			{
				if (HasEnergyManager())
				{
					GetCompEM().SwitchOff();
				}
			}
			if( action_id == 135 ) //SetMaxQuantity
			{
				SetQuantity2(0);
			}
			
			if( action_id == 136 ) //SetMaxQuantity
			{
				PluginItemDiagnostic mid = GetPlugin(PluginItemDiagnostic);
				mid.RegisterDebugItem(this,player);
			}
			
			if( action_id == 140 )
			{
				AddQuantity2(GetQuantityMax2()/10);
				//PrintVariables();
			}

			if( action_id == 142 )
			{
				int id_1;
				int id_2;
				GetPersistentID(id_1,id_2);
				Debug.Log("id1: "+id_1.ToString());
				Debug.Log("id2: "+id_2.ToString());
			}
			
			if( action_id == 143 )
			{
				RemoveAgent(AGT_INFLUENZA);
			}

		}
	}
	// -------------------------------------------------------------------------
	/**
	\brief Add quantity
		\param quantity \p float How many quantity add
		\param [auto_destroy]=false \p bool \p true => Auto delete \p this object when result quantity is 0 or less
		\return \p void
		@code
			item.AddQuantity(5);		// Add quantity
			item.AddQuantity(-3, true);	// Add quantity, if result quantity is 0 or less, item will be deleted
			item.AddQuantity(-8, false);// Add quantity, if result quantity is 0 or less, item will NOT be deleted
		@endcode
	*/
	void AddQuantity(float quantity, bool auto_destroy = false)
	{
		if ( GetDayZGame().IsNewPlayer() )
		{
			AddQuantity2(quantity);
		}
		else
		{
			float quant_val = GetVariableFloat("quantity") + quantity;
			if ( quant_val < 0 ) 
			{
				quant_val = 0;
			}
			if ( quant_val > m_QuantityMax ) 
			{
				quant_val = m_QuantityMax;
			}
			SetQuantity(quant_val, auto_destroy);
		}
	}

	// -------------------------------------------------------------------------
	/**
	\brief Set quantity
		\param quantity \p float New quantity value
		\param [auto_destroy]=false \p bool \p true => Auto delete \p this object when result quantity is 0 or less
		\return \p void
		@code
			item.SetQuantity(5);		// Set quantity
			item.SetQuantity(-5, true);	// Set quantity, if new quantity value is 0 or less, item will be deleted
		@endcode
	*/
	void SetQuantity(float quantity, bool auto_destroy = false)
	{
		if ( GetDayZGame().IsNewPlayer() )
		{
			SetQuantity2(quantity,auto_destroy);
		}
		else
		{
			SetVariable("quantity", quantity);
			
			if ( auto_destroy && quantity <= 0 )
			{
				this.Delete();
			}
		}
	}

	// -------------------------------------------------------------------------
	/**
	\brief Returns current quantity
		\return \p float Returns current value of quantity
		@code
			float qunatity = item.GetQuantity();
			Print(qunatity);
			
			>> qunatity = 5
		@endcode
	*/
	float GetQuantity()
	{
		if ( GetDayZGame().IsNewPlayer() )
		{
			return GetQuantity2();
		}

		return GetVariableFloat("quantity");
	}

	// -------------------------------------------------------------------------
	/**
	\brief Set maximum of quantity, quantity cant be more
		\return \p void
		@code
			item.SetQuantityMax(5);
			item.SetQuantity(10);
			float qunatity = item.GetQuantity();
			Print(qunatity);
			
			>> qunatity = 5
		@endcode
	*/
	void SetQuantityMax( float quantity_max )
	{
		m_QuantityMax = quantity_max;
	}

	// -------------------------------------------------------------------------
	/**
	\brief Returns maximum of quantity
		\return \p float Maximum of quantity
		@code
			item.SetQuantityMax(5);
			item.SetQuantity(3);
			float qunatity_max = item.GetQuantityMax();
			Print(qunatity_max);
			
			>> qunatity_max = 5
		@endcode
	*/
	float GetQuantityMax()
	{
		if ( GetDayZGame().IsNewPlayer() )
		{
			return GetQuantityMax2();
		}
		
		return m_QuantityMax;
	}

	// -------------------------------------------------------------------------
	/**
	\brief Returns \p bool by whether that item can be put together with other item
		\param other_item \p ItemBase for check to combine
		\return \p bool Can combined with \p other_item
		@code
			item_stone.CanQuantityCombine(item_rags);		// returns false
			item_stone.CanQuantityCombine(item_stone);		// returns true
		@endcode
	*/
	bool CanQuantityCombine( ItemBase other_item )
	{
		if ( other_item == NULL || m_QuantityCanCombine == false )
		{
			return false;
		}
		
		string a = this.GetType();
		string b = other_item.GetType();

		if ( a != b )
		{
			return false;
		}
		
		return true;
	}

	// -------------------------------------------------------------------------
	/**
	\brief Quantity of this item will be moved to \p other_item, but only to maximum quantity of \p other_item
		\return \p bool from CanQuantityCombine()
		@code
			item_shovel.CanQuantitySplit();					// returns false, becouse shovel has quantity 1
			item_stone.CanQuantityCombine(item_stone);		// returns true, becouse stone has quantity 5
		@endcode
	*/
	bool QuantityCombine( ItemBase other_item )
	{
		if ( other_item == NULL )
		{
			return false;
		}
		
		if ( !CanQuantityCombine(other_item) )
		{
			MessageToOwnerAction("You can not combine these two items.");
			
			return false;
		}
		
		float can_move_quantity = other_item.GetQuantityMax() - other_item.GetQuantity();
		
		if ( can_move_quantity <= 0 )
		{
			MessageToOwnerAction("You can not combine, stack is maximum.");
			
			return false;
		}
		
		float keep_quantity = this.GetQuantity() - can_move_quantity;
		
		if ( keep_quantity > 0 ) 
		{
			other_item.AddQuantity(can_move_quantity, true);
		}
		else
		{
			other_item.AddQuantity(this.GetQuantity(), true);
		}
		
		SetQuantity( keep_quantity, true );
		
		return true;
	}

	// -------------------------------------------------------------------------
	/**
	\brief Returns \p bool by whether that item can be split to two items by quantity. If quantity is 1 or 0, item can not be split.
		\return \p bool ItemBase can be split to two items
		@code
			item_shovel.CanQuantitySplit();		// returns false, becouse shovel has quantity 1
			item_stone.CanQuantitySplit();		// returns true, becouse stone has quantity 5
		@endcode
	*/
	bool CanQuantitySplit()
	{
		if ( GetQuantityMax() <= 1 || GetQuantity() <= 1 || m_QuantityCanSplit == false )
		{
			return false;
		}
		
		return true;
	}

	// -------------------------------------------------------------------------
	/**
	\brief Half of quantity from this item will be moved to new item object. New item object will be copy of this. Extends damamge.
		\return \p ItemBase Instance of this object with half quantity
		@code
			ItemBase item_stone_new = item_stone.QuantitySplit();	// If this item has 2 or more quantity, will be splitted		
		@endcode
	*/
	ItemBase QuantitySplit()
	{
		if ( !CanQuantitySplit() )
		{
			return NULL;
		}
		
		float split_quantity = Math.Floor( GetQuantity() / 2 );
		
		AddQuantity(-split_quantity);
		PlayerBase player = GetGame().GetPlayer();
		ItemBase new_item = player.CopyInventoryItem( this );
		new_item.SetQuantity( split_quantity, true );
		
		return new_item;
	}

	// -------------------------------------------------------------------------
	/**
	\brief Creates a explosion by ammoType in config of object. If object dont have this parameter ("ammoType" like grenade) explsotion is default "G_GrenadeHand"
		\return \p void
		@code
			ItemBase item = GetGame().GetPlayer().CreateInInventory("GrenadeRGD5");
			
			item.Explode();
		@endcode
	*/
	void Explode()
	{
		string explosion_name = this.ConfigGetString("ammoType");
		
		if ( explosion_name == "" )
		{
			explosion_name = "G_GrenadeHand";
		}
		
		ItemBase explosion = GetGame().CreateObject(explosion_name, this.GetPosition(), false);
		
		if ( explosion != NULL )
		{
			this.Delete();
		}
	}

	//----------------------------------------------------------------
	//returns true if item is able to explode when put in fire
	bool CanExplodeInFire()
	{
		return false;
	}
	
	//----------------------------------------------------------------
	bool CanEat()
	{
		return true;
	}

	//----------------------------------------------------------------
	/**
	\brief Returns whether the object is ruined (damage 1).
	\return \p bool true = item have damage 1
	@code
		if ( item.IsRuined() )
		{
			Print("Object is ruined!");
		}
	@endcode
	*/
	bool IsRuined()
	{
		if ( GetDamage() >= 1 )
		{
			return true;
		}
		
		return false;
	}

	//----------------------------------------------------------------
	bool CanRepair(ItemBase item_repair_kit)
	{
		PluginRepairing module_repairing = GetPlugin(PluginRepairing);
		return module_repairing.CanRepair(this, item_repair_kit);
	}

	//----------------------------------------------------------------
	bool Repair(PlayerBase player, ItemBase item_repair_kit, int repair_type)
	{
		PluginRepairing module_repairing = GetPlugin(PluginRepairing);
		return module_repairing.Repair(player, this, item_repair_kit, repair_type);
	}

	//----------------------------------------------------------------
	int GetItemSize()
	{
		/*
		vector v_size = this.ConfigGetVector("itemSize");
		int v_size_x = v_size[0];
		int v_size_y = v_size[1];
		int size = v_size_x * v_size_y;
		return size;
		*/
		
		return 1;
	}

	//----------------------------------------------------------------
	/**
	\brief Send message to owner player in grey color
		\return \p void
		@code
			item_stone.MessageToOwnerStatus("Some Status Message");
		@endcode
	*/
	void MessageToOwnerStatus( string text )
	{
		PlayerBase player = this.GetOwnerPlayer();
		
		if ( player )
		{
			player.MessageStatus( text );
		}
	}

	//----------------------------------------------------------------
	/**
	\brief Send message to owner player in yellow color
		\return \p void
		@code
			item_stone.MessageToOwnerAction("Some Action Message");
		@endcode
	*/
	void MessageToOwnerAction( string text )
	{
		PlayerBase player = this.GetOwnerPlayer();
		
		if ( player )
		{
			player.MessageAction( text );
		}
	}

	//----------------------------------------------------------------
	/**
	\brief Send message to owner player in green color
		\return \p void
		@code
			item_stone.MessageToOwnerFriendly("Some Friendly Message");
		@endcode
	*/
	void MessageToOwnerFriendly( string text )
	{
		PlayerBase player = this.GetOwnerPlayer();
		
		if ( player )
		{
			player.MessageFriendly( text );
		}
	}

	//----------------------------------------------------------------
	/**
	\brief Send message to owner player in red color
		\return \p void
		@code
			item_stone.MessageToOwnerImportant("Some Important Message");
		@endcode
	*/
	void MessageToOwnerImportant( string text )
	{
		PlayerBase player = this.GetOwnerPlayer();
		
		if ( player )
		{
			player.MessageImportant( text );
		}
	}

	bool IsItemBase()
	{
		return true;
	}

	// Checks if item is of questioned kind
	bool KindOf( string tag )
	{
		bool found = false;
		string item_name = this.GetType();
		autoptr TStringArray item_tag_array = new TStringArray;
		g_Game.ConfigGetTextArray("cfgVehicles " + item_name + " itemInfo", item_tag_array);	
		
		int array_size = item_tag_array.Count();
		for (int i = 0; i < array_size; i++)
		{
			if ( item_tag_array.Get(i) == tag )
			{
				found = true;
				break;
			}
		}
		return found;
	}

	void OnRPC(int rpc_type,ParamsReadContext  ctx) 
	{
		//Debug.Log("OnRPC called");
		super.OnRPC(rpc_type,ctx);
		if( rpc_type == RPC_ITEM_SPLIT ) SplitItem();
		if( rpc_type == RPC_ITEM_COMBINE ) 
		{

			autoptr Param1<EntityAI> item = new Param1<EntityAI>(NULL);

			ctx.Read(item);
			ItemBase item_other = item.param1;
			
			CombineItems( item_other );
		}
		
		if( GetGame().IsDebug() )
		{
			if( rpc_type == RPC_ITEM_DIAG )
			{	
				PluginItemDiagnostic mid = GetPlugin(PluginItemDiagnostic);
				mid.OnRPC(this,ctx);
			}
		}
	}

	//-----------------------------
	// VARIABLE MANIPULATION SYSTEM
	//-----------------------------

	void TransferVariablesFloat(array<float> float_vars)
	{
		DeSerializeNumericalVars(float_vars);
	}
	
	void TransferVariablesString(map<string,string> string_vars)
	{
		if(m_ItemVarsString)
		{
			Debug.LogError("there already are string variables on this item, unable to transfer variables "+this.ToString());
			return;
		}
		
		if( string_vars )
		{
			m_ItemVarsString = new map<string,string>;
			m_ItemVarsString.Copy(string_vars);
		}
	}
	
	array<float> GetVariablesFloat()
	{
		CashedObjectsArrays.ARRAY_FLOAT.Clear();
		SerializeNumericalVars(CashedObjectsArrays.ARRAY_FLOAT);
		return CashedObjectsArrays.ARRAY_FLOAT;
	}
	
	map<string,string> GetVariablesString()
	{
		return m_ItemVarsString;
	}
	
	int NameToID(string name)
	{
		PluginVariables plugin = GetPlugin(PluginVariables);
		return plugin.GetID(name);
	}

	string IDToName(int id)
	{
		PluginVariables plugin = GetPlugin(PluginVariables);
		return plugin.GetName(id);
	}


	void SyncVariables(ParamsWriteContext ctx)//with ID optimization
	{
		//Debug.Log("SyncVariables called for item:  "+ ToString(this.GetType()),"varSync");
		int varFlags = 0;
		
		if( m_VariablesMask != 0 )
		{
			varFlags = varFlags | ItemVariableFlags.FLOAT;
		}
		
		if( m_ItemVarsString )
		{
			varFlags = varFlags | ItemVariableFlags.STRING;
		}
		
		CashedObjectsParams.PARAM1_INT.param1 = varFlags;
		ctx.Write(CashedObjectsParams.PARAM1_INT);
		
		if( varFlags & ItemVariableFlags.FLOAT )
		{
			WriteVarsToCTX(ctx);
		}
		
		if( varFlags & ItemVariableFlags.STRING )
		{
			SyncStrings(ctx);
		}
	}


	void OnSyncVariables(ParamsReadContext ctx)//with ID optimization
	{
		//Debug.Log("OnSyncVariables called for item:  "+ ToString(this.GetType()),"varSync");
		//read the flags
		//autoptr Param1<int> pflags = new Param1<int>(0);
		ctx.Read(CashedObjectsParams.PARAM1_INT);
		
		int varFlags = CashedObjectsParams.PARAM1_INT.param1;
		//--------------
		
		
		if( varFlags & ItemVariableFlags.FLOAT )
		{
			ReadVarsFromCTX(ctx);
		}
		if( varFlags & ItemVariableFlags.STRING )
		{
			OnSyncStrings(ctx);
		}
		
	}	
		
	void SerializeNumericalVars(array<float> floats_out)
	{
		// the order of serialization must be the same as the order of de-serialization
		floats_out.Insert(m_VariablesMask);
		//--------------------------------------------
		if( IsVariableSet(VARIABLE_QUANTITY) )
		{
			floats_out.Insert(m_VarQuantity);
		}
		//--------------------------------------------
		if( IsVariableSet(VARIABLE_ENERGY) )
		{
			floats_out.Insert(m_VarEnergy);
		}
		//--------------------------------------------
		if( IsVariableSet(VARIABLE_TEMPERATURE) )
		{
			floats_out.Insert(m_VarTemperature);
		}
		//--------------------------------------------
		if( IsVariableSet(VARIABLE_WET) )
		{
			floats_out.Insert(m_VarWet);
		}
		//--------------------------------------------
		if( IsVariableSet(VARIABLE_BLOODTYPE) )
		{
			floats_out.Insert(m_VarBloodType);
		}
		//--------------------------------------------
		if( IsVariableSet(VARIABLE_LIQUIDTYPE) )
		{
			floats_out.Insert(m_VarLiquidType);
		}
		//--------------------------------------------
	}
	
	void DeSerializeNumericalVars(array<float> floats)
	{
		// the order of serialization must be the same as the order of de-serialization
		int index = 0;
		int mask = Math.Round(floats.Get(index));
		
		index++;
		//--------------------------------------------
		if( mask & VARIABLE_QUANTITY )
		{
			m_VarQuantity = floats.Get(index);
			SetVariableMask(VARIABLE_QUANTITY);
			index++;
		}
		//--------------------------------------------
		if( mask & VARIABLE_ENERGY )
		{
			m_VarEnergy = floats.Get(index);
			SetVariableMask(VARIABLE_ENERGY);
			index++;
		}
		//--------------------------------------------
		if( mask & VARIABLE_TEMPERATURE )
		{
			m_VarTemperature = floats.Get(index);
			SetVariableMask(VARIABLE_TEMPERATURE);
			index++;
		}
		//--------------------------------------------
		if( mask & VARIABLE_WET )
		{
			m_VarWet = floats.Get(index);
			SetVariableMask(VARIABLE_WET);
			index++;
		}
		//--------------------------------------------
		if( mask & VARIABLE_BLOODTYPE )
		{
			m_VarBloodType = Math.Round(floats.Get(index));
			SetVariableMask(VARIABLE_BLOODTYPE);
			index++;
		}
		//--------------------------------------------
		if( mask & VARIABLE_LIQUIDTYPE )
		{
			m_VarLiquidType = Math.Round(floats.Get(index));
			SetVariableMask(VARIABLE_LIQUIDTYPE);
			index++;
		}
	}

	
	void WriteVarsToCTX(ParamsWriteContext ctx)
	{
		CashedObjectsArrays.ARRAY_FLOAT.Clear();
		SerializeNumericalVars(CashedObjectsArrays.ARRAY_FLOAT);
		
		int array_size = CashedObjectsArrays.ARRAY_FLOAT.Count();
		
		CashedObjectsParams.PARAM1_INT.param1 = array_size;
		ctx.Write(CashedObjectsParams.PARAM1_INT);
		
		for(int i = 0; i < array_size; i++)
		{
			CashedObjectsParams.PARAM1_FLOAT.param1 = CashedObjectsArrays.ARRAY_FLOAT.Get(i);
			ctx.Write(CashedObjectsParams.PARAM1_FLOAT);
		}
	}
	
	void ReadVarsFromCTX(ParamsReadContext ctx)//with ID optimization
	{
		ctx.Read(CashedObjectsParams.PARAM1_INT);
		int numOfItems = CashedObjectsParams.PARAM1_INT.param1;
		CashedObjectsArrays.ARRAY_FLOAT.Clear();
		
		for(int i = 0; i < numOfItems; i++)
		{
			ctx.Read(CashedObjectsParams.PARAM1_FLOAT);
			float value = CashedObjectsParams.PARAM1_FLOAT.param1;
			
			CashedObjectsArrays.ARRAY_FLOAT.Insert(value);
		}
		DeSerializeNumericalVars(CashedObjectsArrays.ARRAY_FLOAT);
	}
	
	void SyncStrings(ParamsWriteContext ctx)//with ID optimization
	{
		int numOfItems = m_ItemVarsString.Count();
		
		//autoptr Param1<int> count = new Param1<int>( numOfItems );
		CashedObjectsParams.PARAM1_INT.param1 = numOfItems;
		ctx.Write(CashedObjectsParams.PARAM1_INT);
		
		if( m_ItemVarsString ) 
		{
			Param2<int, string> p = CashedObjectsParams.PARAM2_INT_STRING;
			
			for(int i = 0; i < m_ItemVarsString.Count(); i++)
			{
				int key = NameToID(m_ItemVarsString.GetKey(i));
				string value = m_ItemVarsString.GetElement(i);
				//Debug.Log("write key: "+ToString(key) + " " + ToString(this.GetType()),"varSync");
				//Debug.Log("write value: "+ToString(value) + " " + ToString(this.GetType()),"varSync");
				p.param1 = key;
				p.param2 = value;
				ctx.Write(p);
			}
		}
		else
		{
			Debug.LogError("ItemBase::SyncStrings - Failed to read an object");
		}
	}

	void OnSyncStrings(ParamsReadContext ctx)//with ID optimization
	{
		//autoptr Param1<int> c = new Param1<int>(0);
		ctx.Read(CashedObjectsParams.PARAM1_INT);
		int numOfItems = CashedObjectsParams.PARAM1_INT.param1;
		

		if( IsVariableMapString() ) 
		{
			m_ItemVarsString.Clear();
		}
		else
		{
			CreateVariableMapString();
		}
		
		Param2<int, string> p = CashedObjectsParams.PARAM2_INT_STRING;
		for(int i = 0; i < numOfItems; i++)
		{
			ctx.Read(p);
			int key = p.param1;
			string value = p.param2;
			//Debug.Log("read key: "+ToString(key) + " " + ToString(this.GetType()),"varSync");
			//Debug.Log("read value: "+ToString(value) + " " + ToString(this.GetType()),"varSync");
			m_ItemVarsString.Set(IDToName(key), value);
		}
	
	}

	//----------------------------------------------------------------
	//! returns the string value of an item variable
	string GetItemVariableString(string name)
	{
		if(	m_ItemVarsString )
		{
			if( m_ItemVarsString.Contains(name) )
			{
				return m_ItemVarsString.Get(name);
			}
		}
		
		return ConfigGetString(name+"Init");
	}


	//----------------------------------------------------------------

	//! sets 'value' of the string item variable 'name'
	void SetItemVariableString(string name,string value)
	{
		if( !IsVariableMapString() ) CreateVariableMapString();
		//Debug.Log("setting variable:"+ToString(name),"recipes");
		m_ItemVarsString.Set(name, value);
		//SyncRequest();
		//Debug.Log("SetSynchDirty() called"+ " " + ToString(ClassName(this)),"varSync");
		SetSynchDirty();
	}

	//----------------------------------------------------------------
	void CreateVariableMapString()
	{
		m_ItemVarsString = new map<string,string>;
	}
	//----------------------------------------------------------------
	bool IsVariableMapString()
	{
		if( m_ItemVarsString )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	//----------------------------------------------------------------

	void SaveVariables(ParamsWriteContext ctx)
	{
		//Debug.Log("Saving Item Stage 0 "+ClassName(this)+" " + ToString(this));
		
		//first set the flags

		int varFlags = 0;
		
		if( m_VariablesMask )
		{
			varFlags = varFlags | ItemVariableFlags.FLOAT;
		}
		
		if( m_ItemVarsString )
		{
			varFlags = varFlags | ItemVariableFlags.STRING;
		}
		
		//autoptr Param1<int> pflags = new Param1<int>( varFlags );
		CashedObjectsParams.PARAM1_INT.param1 = varFlags;
		ctx.Write(CashedObjectsParams.PARAM1_INT);
		//-------------------
			
		//now serialize the variables
		
		//floats
		if( varFlags & ItemVariableFlags.FLOAT )
		{
			WriteVarsToCTX(ctx);
		}
		//string
		if( varFlags & ItemVariableFlags.STRING )
		{
			SaveStrings(ctx);
		}
		
	}

	void SaveStrings(ParamsWriteContext ctx)
	{
		//Debug.Log("Saving Item Stage 1 "+ClassName(this)+" " + ToString(this),"ItemVarsSave");
		int itemCount = m_ItemVarsString.Count();
		//autoptr Param1<int> p1 = new Param1<int>( itemCount );
		CashedObjectsParams.PARAM1_INT.param1 = itemCount;
		ctx.Write(CashedObjectsParams.PARAM1_INT);
		
		//Debug.Log("Saving Item Stage 2 "+ClassName(this)+" " + ToString(this),"ItemVarsSave");
		
		string key = "";
		string value = "";
		
		Param2<string, string> p2 = CashedObjectsParams.PARAM2_STRING_STRING;
		for(int i = 0; i < itemCount; i++)
		{
			//Debug.Log("Saving Item Stage 3 "+ClassName(this)+" " + ToString(this),"ItemVarsSave");
			value = m_ItemVarsString.GetElement(i);
			//Debug.Log( "value: " + ToString(value) ,"ItemVarsSave");
			key = m_ItemVarsString.GetKey(i);
			//Debug.Log("key: " + key ,"ItemVarsSave");
			p2.param1 = key;
			p2.param2 = value;
			ctx.Write(p2);
		}
	}

	//----------------------------------------------------------------
	void LoadVariables(ParamsReadContext ctx)
	{
		//read the flags
		ctx.Read(CashedObjectsParams.PARAM1_INT);
		
		int varFlags = CashedObjectsParams.PARAM1_INT.param1;
		//--------------
		if( varFlags & ItemVariableFlags.FLOAT )
		{
			ReadVarsFromCTX(ctx);
		}
		if( varFlags & ItemVariableFlags.STRING )
		{
			LoadStrings(ctx);
		}
	}

	void LoadStrings(ParamsReadContext ctx)
	{
		if ( ctx.Read(CashedObjectsParams.PARAM1_INT) )
		{
			int numOfItems = CashedObjectsParams.PARAM1_INT.param1;
			if( numOfItems == 0 ) 
			{
				Debug.LogError("Error: Item Variable system - flag set for saving with zero elements !");
				return;
			}
			Param2<string, string> p2 = CashedObjectsParams.PARAM2_STRING_STRING;
			if( !IsVariableMapString() ) 
			{
				CreateVariableMapString();
			}
			for(int i = 0;i < numOfItems;i++)
			{	
				ctx.Read(p2);
				m_ItemVarsString.Set( p2.param1,p2.param2 );
			}
			SetSynchDirty();
		}
		else
		{
			Debug.LogError("Error: Item Variable system - Failed to read an object");
		}
	}


	//----------------------------------------------------------------
	void OnStoreLoad(ParamsReadContext ctx)
	{   
		super.OnStoreLoad(ctx);

		if ( GetDayZGame().IsNewPlayer() )
		{
			LoadVariables(ctx);// variable management system
			LoadAgents(ctx);//agent trasmission system
		}
	}

	//----------------------------------------------------------------

	void OnStoreSave(ParamsWriteContext ctx)
	{
		super.OnStoreSave(ctx);
		
		if ( GetDayZGame().IsNewPlayer() )
		{
			SaveVariables(ctx);// variable management system
			SaveAgents(ctx);//agent trasmission system
		}
	}
	//----------------------------------------------------------------

	void OnSynchronize(ParamsWriteContext ctx)
	{
		//Debug.Log("OnSynchronize called: "+ ToString(this.GetType()),"varSync");
		super.OnSynchronize(ctx);
		if ( GetDayZGame().IsNewPlayer() )
		{
			SyncVariables(ctx);
		}
	}
	//----------------------------------------------------------------
	void OnSynchronized(ParamsReadContext ctx)
	{
		//Debug.Log("OnSynchronized called: "+ ToString(this.GetType()),"varSync");
		super.OnSynchronized(ctx);
		if ( GetDayZGame().IsNewPlayer() )
		{
			OnSyncVariables(ctx);
		}
	}
	//-------------------------	Quantity
	//----------------------------------------------------------------
	//! Set item quantity[related to varQuantity... config entry], destroy_config = true > if the quantity reaches varQuantityMin or lower and the item config contains the varQuantityDestroyOnMin = true entry, the item gets destroyed. destroy_forced = true means item gets destroyed when quantity reaches varQuantityMin or lower regardless of config setting, returns true if the item gets deleted
	bool SetQuantity2(float value, bool destroy_config = true, bool destroy_forced = false)
	{
		if( GetGame().IsClient() && GetGame().IsMultiplayer() ) Error("Attempting to change variable client side, variables are supposed to be changed on server only !!");
		bool on_min_value = value <= GetQuantityMin2();
		
		if( on_min_value )
		{
			if( destroy_config )
			{
				bool dstr = ConfigGetBool("varQuantityDestroyOnMin");
				if( dstr )
				{
					this.Delete();
					return true;
				}
			}
			else if( destroy_forced )
			{
				this.Delete();
				return true;
			}
			// we get here if destroy_config IS true AND dstr(config destroy param) IS false;
			RemoveAllAgents();//we remove all agents when we got to the min value, but the item is not getting deleted
		}
		
		float min = ConfigGetFloat("varQuantityMin");
		float max = ConfigGetFloat("varQuantityMax");
		
		m_VarQuantity = Math.Clamp(value, min, max);
		SetVariableMask(VARIABLE_QUANTITY);
		return false;
	}

	//----------------------------------------------------------------
	//! add item quantity[related to varQuantity... config entry], destroy_config = true > if the quantity reaches varQuantityMin or lower and the item config contains the varQuantityDestroyOnMin = true entry, the item gets destroyed. destroy_forced = true means item gets destroyed when quantity reaches varQuantityMin or lower regardless of config setting, returns true if the item gets deleted
	bool AddQuantity2(float value, bool destroy_config = true, bool destroy_forced = false)
	{	
		SetQuantity2(GetQuantity2() + value);
	}
	//----------------------------------------------------------------
	void SetQuantityMax2()
	{
		float max = ConfigGetFloat("varQuantityMax");
		SetQuantity2(max);
	}
	//----------------------------------------------------------------
	float GetQuantityMax2()
	{
		return ConfigGetFloat("varQuantityMax");
	}
	//----------------------------------------------------------------
	float GetQuantityMin2()
	{
		return ConfigGetFloat("varQuantityMin");
	}
	//----------------------------------------------------------------
	/*
	float GetQuantity2()
	{
		float min_quantity = GetQuantityMin2();
		float difference = GetQuantityMax2() - min_quantity;
		if( difference == 0 ) return 0;//taking care of the division by 0 bellow
		
		if( IsVariableFloatExist("varQuantity") ) 
		{
			return  GetItemVariable("varQuantity");
		}
		else
		{
			int id_1;
			int id_2;
			GetPersistentID(id_1,id_2);
			int quantity_pseudo_random = (Math.AbsInt(id_1) % difference) + min_quantity;//warning, division, make sure its not 0 when refactoring
			if( quantity_pseudo_random < GetQuantityMin2() ) quantity_pseudo_random = GetQuantityMin2();
			return quantity_pseudo_random;
		}
	}
	*/
	
	float GetQuantity2()
	{
		if( !IsVariableSet(VARIABLE_QUANTITY) ) 
		{
			return ConfigGetFloat("varQuantityInit");
		}
		else
		{
			return m_VarQuantity;
		}
	}
	
	bool IsFullQuantity2()
	{
		if( GetQuantity2() == GetQuantityMax2() )
		{
			return true;			
		}
		else
		{
			return false;			
		}
	}
	
	void SetVariableMask(int variable)
	{
		m_VariablesMask = variable | m_VariablesMask; 
		if( GetGame().IsServer() ) SetSynchDirty();
	}
	
	//!Removes variable from variable mask, making it appear as though the variable has never been changed from default
	void RemoveItemVariable(int variable)
	{
		m_VariablesMask = ~variable & m_VariablesMask;
	}
	
	//!'true' if this variable has ever been changed from default
	bool IsVariableSet(int variable)
	{
		return (variable & m_VariablesMask);
	}
	
	//-------------------------	Energy
	//----------------------------------------------------------------
	void SetEnergy2(float value)
	{
		if( GetGame().IsClient() && GetGame().IsMultiplayer() ) Error("Attempting to change variable client side, variables are supposed to be changed on server only !!");
		float min = ConfigGetFloat("varEnergyMin");
		float max = ConfigGetFloat("varEnergyMax");
		
		m_VarEnergy = Math.Clamp(value, min, max);
		SetVariableMask(VARIABLE_ENERGY);

	}
	//----------------------------------------------------------------
	void AddEnergy2(float value)
	{
		SetEnergy2( GetEnergy() + value );
	}
	//----------------------------------------------------------------
	void SetEnergyMax2()
	{
		float max = ConfigGetFloat("varEnergyMax");
		SetEnergy2(max);
	}
	//----------------------------------------------------------------
	float GetEnergy()
	{
		float energy = 0;
		if( this.HasEnergyManager() )
		{
			energy = this.GetCompEM().GetEnergy();
		}
		return energy;
	}
	//----------------------------------------------------------------
	float GetEnergy2()
	{
		if( !IsVariableSet(VARIABLE_ENERGY) ) 
		{
			return ConfigGetFloat("varEnergyInit");
		}
		return m_VarEnergy;
	}
	//-------------------------	Temperature
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	// SetTemperature
	//----------------------------------------------------------------
	void SetTemperature(float value)
	{
		SetVariable("temperature", value);
	}
	//----------------------------------------------------------------
	// AddTemperature
	//----------------------------------------------------------------
	void AddTemperature(float value)
	{
		value = value + GetTemperature();
		SetTemperature(value);
	}

	//----------------------------------------------------------------
	// GetTemperature
	//----------------------------------------------------------------
	float GetTemperature()
	{
		return GetVariableFloat("temperature");
	}

	void SetTemperature2(float value)
	{
		if( GetGame().IsClient() && GetGame().IsMultiplayer() ) Error("Attempting to change variable client side, variables are supposed to be changed on server only !!");
		float min = ConfigGetFloat("varTemperatureMin");
		float max = ConfigGetFloat("varTemperatureMax");
		
		m_VarTemperature = Math.Clamp(value, min, max);
		SetVariableMask(VARIABLE_TEMPERATURE);
	}
	//----------------------------------------------------------------
	void AddTemperature2(float value)
	{
		SetTemperature2( value + GetTemperature2() );
	}
	//----------------------------------------------------------------
	void SetTemperatureMax2()
	{
		float max = ConfigGetFloat("varTemperatureMax");
		SetTemperature2(max);
	}
	//----------------------------------------------------------------
	float GetTemperature2()
	{
		if( !IsVariableSet(VARIABLE_TEMPERATURE) ) 
		{
			return ConfigGetFloat("varTemperatureInit");
		}
		return m_VarTemperature;
	}
	//----------------------------------------------------------------
	//-------------------------	Wetness

	//----------------------------------------------------------------
	// SetWet
	//----------------------------------------------------------------
	void SetWet(float value)
	{
		SetVariable("wet", value);
	}

	//----------------------------------------------------------------
	// AddWet
	//----------------------------------------------------------------
	void AddWet(float value)
	{
		value = value + GetWet();
		SetWet(value);
	}

	//----------------------------------------------------------------
	// GetWet
	//----------------------------------------------------------------
	float GetWet()
	{
		return GetVariableFloat("wet");
	}

	//----------------------------------------------------------------
	void SetWet2(float value)
	{
		if( GetGame().IsClient() && GetGame().IsMultiplayer() ) Error("Attempting to change variable client side, variables are supposed to be changed on server only !!");
		float min = ConfigGetFloat("varWetMin");
		float max = ConfigGetFloat("varWetMax");
		
		m_VarWet = Math.Clamp(value, min, max);
		SetVariableMask(VARIABLE_WET);
	}
	//----------------------------------------------------------------
	void AddWet2(float value)
	{
		SetWet2( GetWet2() + value );
	}
	//----------------------------------------------------------------
	void SetWetMax2()
	{
		float max = ConfigGetFloat("varWetMax");
		SetWet2(max);
	}
	//----------------------------------------------------------------
	float GetWet2()
	{
		if( !IsVariableSet(VARIABLE_WET) ) 
		{
			return ConfigGetFloat("varWetInit");
		}
		return m_VarWet;
	}
	//----------------------------------------------------------------
	float GetWetMax2()
	{
		return ConfigGetFloat("varWetMax");
	}
	//----------------------------------------------------------------
	//-------------------------	BloodType

	void SetBloodType2(int value)
	{
		if( GetGame().IsClient() && GetGame().IsMultiplayer() ) Error("Attempting to change variable client side, variables are supposed to be changed on server only !!");
		m_VarBloodType = value;
		SetVariableMask(VARIABLE_BLOODTYPE);
	}
	
	
	int GetBloodType2()
	{
		if( !IsVariableSet(VARIABLE_BLOODTYPE) ) 
		{
			return ConfigGetFloat("varBloodTypeInit");
		}
		return m_VarBloodType;
	}
	//----------------------------------------------------------------
	//-------------------------	LiquidType

	void SetLiquidType2(int value)
	{
		if( GetGame().IsClient() && GetGame().IsMultiplayer() ) Error("Attempting to change variable client side, variables are supposed to be changed on server only !!");
		m_VarLiquidType = value;
		SetVariableMask(VARIABLE_LIQUIDTYPE);
	}
	
	int GetLiquidType2()
	{
		if( !IsVariableSet(VARIABLE_LIQUIDTYPE) ) 
		{
			return ConfigGetFloat("varLiquidTypeInit");
		}
		return m_VarLiquidType;
	}

	// -------------------------------------------------------------------------
	//! Event called on item when it is placed in the player(Man) inventory, passes the owner as a parameter
	void OnInventoryEnter(Man player)
	{
		if ( GetDayZGame().IsNewPlayer() && GetGame().IsServer() && player.IsInherited(PlayerBase) )
		{
			PlayerBase nplayer = player;
			//nplayer.CalculatePlayerLoad();
			nplayer.ChangePlayerLoad(this, false);
		}
	}

	// -------------------------------------------------------------------------
	//! Event called on item when it is removed from the player(Man) inventory, passes the old owner as a parameter
	void OnInventoryExit(Man player)
	{
		if ( GetDayZGame().IsNewPlayer() && GetGame().IsServer() && player.IsInherited(PlayerBase) )
		{
			PlayerBase nplayer = player;
			//nplayer.CalculatePlayerLoad();
			nplayer.ChangePlayerLoad(this, true);
		}
	}

	//-----------------------------
	// AGENT SYSTEM
	//-----------------------------
	//--------------------------------------------------------------------------
	bool ContainsAgent( int agent_id )
	{
		if( agent_id & m_AttachedAgents )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//--------------------------------------------------------------------------
	void RemoveAgent( int agent_id )
	{
		if( ContainsAgent(agent_id) )
		{
			m_AttachedAgents = ~agent_id & m_AttachedAgents;//invert the mask and apply AND to substitute missing XOR
		}
	}

	//--------------------------------------------------------------------------
	void RemoveAllAgents()
	{
		m_AttachedAgents = 0;
	}

	// -------------------------------------------------------------------------
	void InsertAgent(int agent, int count)
	{
		if( count<1 ) return;
		Debug.Log("Inserting Agent on item: " + agent.ToString() +" count: " + count.ToString());
		m_AttachedAgents = (agent | m_AttachedAgents);
	}
	
	//!transfer agents from another item
	void TransferAgents(int agents)
	{
		m_AttachedAgents = (m_AttachedAgents | agents);
	}
	// -------------------------------------------------------------------------
	int GetAgents()
	{
		return m_AttachedAgents;
	}

	// -------------------------------------------------------------------------
	void LoadAgents(ParamsReadContext ctx)
	{
		if( ctx.Read(CashedObjectsParams.PARAM1_INT) )
		{
			m_AttachedAgents = CashedObjectsParams.PARAM1_INT.param1;
		}
	}
	// -------------------------------------------------------------------------
	void SaveAgents(ParamsWriteContext ctx)
	{
		
		CashedObjectsParams.PARAM1_INT.param1 = m_AttachedAgents;
		ctx.Write(CashedObjectsParams.PARAM1_INT);
	}
	// -------------------------------------------------------------------------

	//---------------------------
	// ADVANCED PLACEMENT EVENTS
	//---------------------------

	void OnHologramItemCreated()
	{
		
	}
	
	// Most of the hologram function runs on the client side only. see Hologram.c for more info
	void OnHologramBeingPlaced( PlayerBase player )
	{
		
	}

	void OnPlacementFinished( PlayerBase player )
	{
		
	}

	void OnPlacementCancelled( PlayerBase player )
	{
		if ( HasEnergyManager() )
		{
			PlayerBase attached_to = GetOwner();
			if (!attached_to  ||  attached_to == player )// Check for exception with attaching a cable reel to an electric fence
				GetCompEM().UnplugThis();
		}
	}

	//-------------------------
	// OPEN/CLOSE USER ACTIONS
	//-------------------------
	void Open()
	{
		m_Opened = true;
	}

	void Close()
	{
		m_Opened = false;
	}

	bool IsOpen()
	{
		return m_Opened;
	}



	bool CanBePlaced( PlayerBase player, vector position )
	{
		return true;
	}

	//! Method which returns message why object can't be placed at given position
	string CanBePlacedFailMessage( PlayerBase player, vector position )
	{
		return "";
	}
	//-------------------------
	void Consume(PlayerBase player, float amount)
	{
		PluginTransmissionAgents mta = GetPlugin(PluginTransmissionAgents);
		mta.TransmitAgents(this, player, AGT_UACTION_CONSUME, amount);

		AddQuantity2(-amount, false);
		ProcessNutritions(player,amount);
	}
}

