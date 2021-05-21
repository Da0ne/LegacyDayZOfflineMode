class InventoryManager
{
	private static InventoryManager s_instance;
	
	protected PlayerBase m_player;

	// place to hands
	//private autoptr Timer m_timer;
	//protected autoptr Link<EntityAI> m_entity_to_take;
	protected autoptr array<int> m_sorted_keys;
	
	// icon sizes	
	const float MAX_CARGO_GRID_WIDTH = 10;	
	const float MAX_ATTACHMENT_GRID_WIDTH = 7;	
	const float MAX_HANDS_ATTACHMENT_GRID_WIDTH = 7;
	const float GRID_CARGO_GAP = 1;
	const float GRID_ATTACHMENT_GAP = 2;

	protected	float m_normal_item_size;
	protected	float m_hands_normal_item_size;
	protected	float m_attachment_item_size;
	protected	float m_hands_attachment_item_size;
	protected	float m_icon_item_size;
	protected	float m_quantity_stack_panel_size;
	protected float m_panels_size;
	
	// drag & drop 	
	protected bool m_is_dragging;
	protected autoptr Link<InventoryItem> m_drag_item;
	protected InventoryGrid m_grid;
	//!InventoryDragFromType
	protected int m_from_type;

	// action menu
	protected autoptr TActionRefArray m_actions;
	protected autoptr Link<EntityAI> m_am_entity1;
	protected autoptr Link<EntityAI> m_am_entity2;
	protected int m_am_itemes_combination_flags;
	protected int m_am_pos_x;
	protected int m_am_pos_y;

	// tooltip
	private Widget m_tooltip_widget;
	private ItemPreviewWidget m_item_preview_widget;
		
	void InventoryManager()
	{
		s_instance = this;
		m_drag_item = new Link<InventoryItem>(NULL);	
		m_am_entity1 = new Link<EntityAI>(NULL);	
		m_am_entity2 = new Link<EntityAI>(NULL);	
		m_actions = new TActionRefArray;
		m_player = GetGame().GetPlayer();
		//m_entity_to_take = new Link<EntityAI>(NULL);
		//m_timer = new Timer();

		// sort order of items for player's inventory
		m_sorted_keys = new array<int>;
		m_sorted_keys.Insert( IS_HANDS);
		m_sorted_keys.Insert( IS_SHOULDER );
		m_sorted_keys.Insert( IS_BOW );
		m_sorted_keys.Insert( IS_MELEE );
		m_sorted_keys.Insert( IS_VEST );
		m_sorted_keys.Insert( IS_BODY );
		m_sorted_keys.Insert( IS_HIPS );
		m_sorted_keys.Insert( IS_LEGS );
		m_sorted_keys.Insert( IS_BACK );
		m_sorted_keys.Insert( IS_HEADGEAR );
		m_sorted_keys.Insert( IS_MASK );
		m_sorted_keys.Insert( IS_EYEWEAR );
		m_sorted_keys.Insert( IS_GLOVES );
		m_sorted_keys.Insert( IS_FEET );
		m_sorted_keys.Insert( IS_ARMBAND );

		// tooltip
		m_tooltip_widget = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_inventory_tooltip.layout", NULL );
		m_tooltip_widget.Show( false );
	}

	void ~InventoryManager()
	{
		s_instance = NULL;
		m_tooltip_widget.Destroy();
	}

	// drop to the ground
	void DropItem(InventoryItem item)
	{
		if (item && item.GetOwner() != NULL && (m_player.GetEntityInHands() != GetItem() || m_player.CanRemoveEntityInHands()))
		{
			m_player.DropEntity(item);

			InventoryMenu menu = GetGame().GetUIManager().FindMenu(MENU_INVENTORY);
			if ( menu )
			{
				menu.RefreshQuickbar();
			}
		}
	}

	// place to hands
	void TryTakeToHands()
	{
		//EntityAI item_in_hands = m_player.GetEntityInHands();
		
		//if (item_in_hands) return; // wait longer
		
		//if (m_entity_to_take.IsNull() == false)
		//{
		//	if (m_player.CanAddEntityInHands(m_entity_to_take.Ptr()))
		//	{
		//		PrintString("TryTakeToHands tick, retry!");
		//		m_player.TakeEntityToHands(m_entity_to_take.Ptr());
		//		//delete m_entity_to_take;
		//		//m_entity_to_take = new Link<EntityAI>(NULL);
		//	}
		//}
		//m_timer.Stop();
	}
	
	void TakeToHands(InventoryItem item)
	{
		//PrintString("TakeToHands stop timer");
		//m_timer.Stop();
		//delete m_entity_to_take;
		//PrintString("TakeToHands stop timer");
		//m_entity_to_take = new Link<EntityAI>(item);
		
		EntityAI item_in_hands = m_player.GetEntityInHands();
		if (item_in_hands) 
		{
			//PrintString("TakeToHands MoveItemFromHandsToInventory");
			if (m_player.CanRemoveEntityInHands())
				m_player.MoveItemFromHandsToInventory(item_in_hands);
		}
		
		if (item_in_hands)
		{
			//m_timer.Run(0.5, this, "TryTakeToHands", NULL, true);
		}
		else
		{
			//TryTakeToHands();
		}
		m_player.TakeEntityToHands(item);
	}

	void TakeAsOwned(EntityAI entity1, EntityAI entity2)
	{
		//m_timer.Stop();
		if (!entity1 || !entity2) return;
		
		if (entity1.IsInherited( Weapon) && entity2.IsInherited(Magazine))
		{
			TryReloadWeapon(m_player, entity1, entity2);
		}
		else
		{
			entity1.TakeEntityAsOwned(entity2);
		}
	}
	
	// drag & drop 	
	void Drag(int type, InventoryItem item, InventoryGrid grid)
	{
		// Print("Drag start: ");
		// Print(item);
		// if (item) PrintString( "class InventoryManager >>> Drag() >>> " + ToString(item.GetName()) );
		m_is_dragging = true;
		delete m_drag_item;
		m_drag_item = new Link<InventoryItem>(item);
		m_grid = grid;
		m_from_type = type;
		
		SetItemColor(InventoryGrid.ITEM_COLOR_DRAG);
		
		int item_w;
		int item_h;
		GetGame().GetInventoryItemSize(item, item_w, item_h);
		
		SetItemSize((item_w * GetNormalItemSize()) + (GRID_CARGO_GAP * (item_w - 1)), (item_h * GetNormalItemSize()) + (GRID_CARGO_GAP * (item_h - 1)));
	}

	void Drop()
	{
		// Print("Drag stop");
		if (m_grid && m_drag_item.Ptr()) m_grid.ResetItem(m_drag_item.Ptr());
		
		m_grid = NULL;
		delete m_drag_item;
		m_drag_item = new Link<InventoryItem>(NULL);
		
		m_is_dragging = false;

		InventoryMenu menu = GetGame().GetUIManager().FindMenu(MENU_INVENTORY);
		if ( menu )
		{
			menu.RefreshQuickbar();
		}
	}
	
	void SetItemColor(int color)
	{
		if (m_is_dragging && m_grid && m_drag_item.Ptr())
		{
			m_grid.SetItemColor(m_drag_item.Ptr(), color);
		}	
	}

	void SetItemSize(int pixels_w, int pixels_h)
	{
		if (m_is_dragging && m_grid && m_drag_item.Ptr())
		{
			m_grid.SetItemSize(m_drag_item.Ptr(), pixels_w, pixels_h);
		}	
	}

	bool IsDragging() 
	{
		return m_is_dragging;
	}	
	InventoryItem GetItem() 
	{
		return m_drag_item.Ptr();	
	}
	InventoryGrid GetGrid() 
	{
		return m_grid;		
	}
	
	array<int> GetSortedKeys() 
	{
		return m_sorted_keys;		
	}

	// action menu
	void OnPerformCraft(string recipe_name)
	{
		if (m_am_entity1.IsNull() || m_am_entity2.IsNull()) return;
		
		GetGame().RecipeProcess(recipe_name, m_player, m_am_entity2.Ptr(), m_am_entity1.Ptr());
		InventoryMenu menu = GetGame().GetUIManager().FindMenu(MENU_INVENTORY);
		if ( menu )
		{
			menu.RefreshQuickbar();
			menu.PerformRefreshInventory();
		}
	}

	void OnPerformRecipe(int id)
	{
		if (m_am_entity1.IsNull() || m_am_entity2.IsNull()) return;

		Debug.Log("OnPerformRecipe called for id:"+id.ToString(),"recipes");
		PluginRecipesManager plugin = GetPlugin(PluginRecipesManager);
		plugin.PerformRecipeClientRequest(id,m_am_entity1.Ptr(),m_am_entity2.Ptr());
	}	
	
	void OnPerformAction(Action action)
	{
		m_player.PerformAction(action);
		InventoryMenu menu = GetGame().GetUIManager().FindMenu(MENU_INVENTORY);
		if ( menu )
		{
			menu.RefreshQuickbar();
			menu.PerformRefreshInventory();
		}
		HideTooltip();
	}
	
	void InspectItem(InventoryItem item)
	{
		InventoryMenu menu = GetGame().GetUIManager().FindMenu(MENU_INVENTORY);
		InspectMenu inspect_menu = menu.EnterScriptedMenu(MENU_INSPECT);
		if (inspect_menu)
		{
			inspect_menu.SetItem(item);
		}
	}

	void ShowActionMenu(InventoryItem item)
	{
		HideTooltip();
		if (item) PrintString("Showing action menu for " + item.GetName());
		delete m_am_entity1;
		m_am_entity1 = new Link<EntityAI>(item);
		delete m_am_entity2;
		m_am_entity2 = new Link<EntityAI>(NULL);
		ContextMenu cmenu = GetGame().GetUIManager().GetMenu().GetContextMenu();
		
		cmenu.Hide();
		cmenu.Clear();
		
		if (m_am_entity1.IsNull()) return;
		
		if (m_player.GetEntityInHands() == item && item.CanRemoveEntity()) cmenu.Add("#inv_context_remove_from_hands", this, "TakeToHands", new Param1<EntityAI>(NULL));
		if (m_player.CanAddEntityInHands(item) && item.CanRemoveEntity()) cmenu.Add("#inv_context_take_to_hands", this, "TakeToHands", new Param1<EntityAI>(item));

		// if ( !IsInherited( item, ZombieBase ) && !IsInherited( item, TankOrCar ) ) 
		// {
			cmenu.Add("#inv_context_inspect", this, "InspectItem", new Param1<InventoryItem>(item));
		// }
		
		cmenu.Add("#inv_receiving", NULL, "", NULL);
		
		GetMousePos(m_am_pos_x, m_am_pos_y);
		cmenu.Show(m_am_pos_x, m_am_pos_y);
		
		item.GetActions(m_player);
	}

	void ShowAllValidRecipes(EntityAI entity1, EntityAI entity2,ContextMenu cmenu)
	{
		PlayerBase p = m_player;
		map<int,string> recipesMap = p.m_Recipes;
		PluginRecipesManager module_recipes_manager = (PluginRecipesManager)GetPlugin(PluginRecipesManager);
		module_recipes_manager.GetValidRecipes(entity1,entity2,recipesMap, m_player);

		int key = 0;
		for(int x = 0;x < recipesMap.Count();x++)
		{
			key = recipesMap.GetKey(x);

			cmenu.Add(recipesMap.Get(key), this, "OnPerformRecipe",new Param1<int>(key));
		}
	}
	
	void ShowActionMenuCombine(EntityAI entity1, EntityAI entity2, int combinationFlags)
	{
		if (entity1 && entity2) PrintString("Showing action menu for " + entity1.GetName() + " and " + entity2.GetName());
		delete m_am_entity1;
		m_am_entity1 = new Link<EntityAI>(entity1);
		delete m_am_entity2;
		m_am_entity2 = new Link<EntityAI>(entity2);
		
		m_am_itemes_combination_flags = combinationFlags;
		
		ContextMenu cmenu = GetGame().GetUIManager().GetMenu().GetContextMenu();
		
		cmenu.Hide();
		cmenu.Clear();
		
		if( GetGame().IsNewPlayer() && GetGame().IsDebugActions() )
		{
			if(combinationFlags & InventoryCombinationFlags.RECIPE)
			{
				ShowAllValidRecipes(entity1,entity2,cmenu);
			}
		}
		
		if (m_am_entity1.IsNull() || m_am_entity2.IsNull()) return;
		
		if (combinationFlags == InventoryCombinationFlags.NONE) return;
		
		if (combinationFlags & InventoryCombinationFlags.ADD_AS_ATTACHMENT)
		{
			cmenu.Add("#inv_context_add_as_attachment", this, "OnPerformCombination", new Param1<int>(InventoryCombinationFlags.ADD_AS_ATTACHMENT));		
		}
		
		if (combinationFlags & InventoryCombinationFlags.ADD_AS_CARGO)
		{
			cmenu.Add("#inv_context_add_as_cargo", this, "OnPerformCombination", new Param1<int>(InventoryCombinationFlags.ADD_AS_CARGO));			
		}
		
		if (combinationFlags & InventoryCombinationFlags.SWAP)
		{
			cmenu.Add("#inv_context_swap", this, "OnPerformCombination", new Param1<int>(InventoryCombinationFlags.SWAP));			
		}
		
		if (combinationFlags & InventoryCombinationFlags.TAKE_TO_HANDS)
		{
			cmenu.Add("#inv_context_take_to_hands", this, "OnPerformCombination", new Param1<int>(InventoryCombinationFlags.TAKE_TO_HANDS));			
		}
		
		if (combinationFlags & InventoryCombinationFlags.COMBINE_QUANTITY)
		{
			cmenu.Add("#inv_context_combine", this, "OnPerformCombination", new Param1<int>(InventoryCombinationFlags.COMBINE_QUANTITY));			
		}
		
		if (combinationFlags & InventoryCombinationFlags.CRAFT)
		{
			autoptr TStringArray recipes = new TStringArray;
			GetGame().RecipesGet(m_player, entity1, entity2, recipes);

			for (int i = 0; i < recipes.Count(); i++)
			{
				string recipe_name = recipes.Get(i);
				string recipe_desc;
				GetGame().RecipeGetDescription(recipe_name, m_player, entity2, entity1, recipe_desc);
				cmenu.Add(recipe_desc, this, "OnPerformCraft", new Param1<string>(recipe_name));		
			}
		}
		
		if (combinationFlags & InventoryCombinationFlags.ACTIONS)
		{
			cmenu.Add("#inv_receiving", NULL, "", NULL);
			entity2.GetActions(m_player, entity1);
		}	
		
		GetMousePos(m_am_pos_x, m_am_pos_y);
		m_am_pos_x -= 5;
		m_am_pos_y -= 5;
		
		cmenu.Show(m_am_pos_x, m_am_pos_y);
	}

	void UpdateEntityActions(EntityAI entity1, EntityAI entity2, array<Action> test)
	{
		/*
		compiler BUG???
		int pocet = test.Count();
		int pocet2 = m_actions.Count();
		*/
		ContextMenu cmenu = GetGame().GetUIManager().GetMenu().GetContextMenu();
		if (cmenu == NULL) return;	

		m_actions.Release();
		m_actions.Copy(test);
		m_actions.AddRef();

		// remove "receiving" line
		if (cmenu.Count()) cmenu.Remove( cmenu.Count() - 1 );
		
		if (m_actions.Count())
		{
			for (int i = 0; i < m_actions.Count(); i++)
			{
				string action_text;
				Action action = m_actions.Get(i);
				action.GetText(m_player, action_text);
				if (action_text != "")
				{
					cmenu.Add(action_text, this, "OnPerformAction", new Param1<Action>(action));
				}
			}
		}

		cmenu.Show(m_am_pos_x, m_am_pos_y);
	}

	//! Returns combionation of InventoryCombinationFlags
	int GetCombinationFlags(EntityAI entity1, EntityAI entity2)
	{
		int flags = 0;
		EntityAI entity_in_hands = m_player.GetEntityInHands();
		
		if (!entity1 || !entity2) return flags;
		
		if (entity1 == m_player.GetEntityInHands()) flags = flags | InventoryCombinationFlags.TAKE_TO_HANDS;
		else if ( CanSwapEntities( entity1, entity2 ) )
		{
			if ( !entity1.IsInherited( ZombieBase ) && !entity1.IsInherited( TankOrCar ) )
			{
				flags = flags | InventoryCombinationFlags.SWAP;
			}
		}
		
		if (entity2.HasActions(m_player, entity1)) flags = flags | InventoryCombinationFlags.ACTIONS;
		
		if ( entity1.IsInherited( ItemBase) && entity2.IsInherited( ItemBase))
		{
			ItemBase entity1ItemBase = entity1;
			if (entity1ItemBase.CanQuantityCombine(entity2)) flags = flags | InventoryCombinationFlags.COMBINE_QUANTITY;
		}
		
		if (entity1.CanAddAttachment(entity2)) 
		{
			if ( !entity1.IsInherited( ZombieBase ) && !entity1.IsInherited( TankOrCar ) && !entity2.IsInherited( ZombieBase ) && !entity2.IsInherited( TankOrCar ) )
			{
				flags = flags | InventoryCombinationFlags.ADD_AS_ATTACHMENT;
			}
		}
		if (entity1.CanAddEntityInCargo(entity2)) flags = flags | InventoryCombinationFlags.ADD_AS_CARGO;
		if (GetGame().RecipesHas(m_player, entity1, entity2)) flags = flags | InventoryCombinationFlags.CRAFT;
		if( GetGame().IsNewPlayer() && GetGame().IsDebugActions())//temporary VRATA
		{
			if(entity1 == m_player.GetEntityInHands())
			{
				Debug.Log("GetCombinationFlags called, setting comb. flag","recipes");
				flags = flags | InventoryCombinationFlags.RECIPE;
			}
		}
		return flags;
	}	
	
	// icon sizes
	float GetNormalItemSize() {
		return m_normal_item_size;
	}
	
	float GetAttachmentItemSize() {
		return m_attachment_item_size;
	}
	float GetIconItemSize() {
		return m_icon_item_size;
	}
	float GetHandsNormalItemSize() {
		return m_hands_normal_item_size;
	}
	float GetHandsAttachmentItemSize() {
		return m_hands_attachment_item_size;
	}
	float GetPanelsSize() {
		return m_panels_size;
	}

	float GetQuantityStackPanelSize() {
		return m_quantity_stack_panel_size;
	}

	void RecalculateIconSize(float maxGridWidth)
	{
		m_normal_item_size = 0.97 * ( maxGridWidth / MAX_CARGO_GRID_WIDTH ); //48 floor((maxGridWidth - ((MAX_CARGO_GRID_WIDTH - 1) * GRID_GAP)) / MAX_CARGO_GRID_WIDTH);
		m_attachment_item_size = 0.97 * ( maxGridWidth / MAX_ATTACHMENT_GRID_WIDTH ); // 46; //46 70 floor(m_normal_item_size * 1.75);
		m_icon_item_size = 70; //floor( m_normal_item_size * 1.85);
		
		m_hands_normal_item_size = 0.97 * ( maxGridWidth / MAX_CARGO_GRID_WIDTH ); //48; //floor((maxGridWidth - ((MAX_CARGO_GRID_WIDTH - 1) * GRID_GAP)) / MAX_CARGO_GRID_WIDTH);
		m_hands_attachment_item_size = 0.97 * ( maxGridWidth / MAX_HANDS_ATTACHMENT_GRID_WIDTH );//80;
		m_quantity_stack_panel_size = m_normal_item_size * 0.5;
	}

	void ShowTooltip( EntityAI item )
	{
		if ( IsDragging() ) return;
		if ( item.IsInherited(InventoryItem) )
		{
			UpdateItemInfo( m_tooltip_widget, item );
			int x, y, screen_w, screen_h;
			float w, h;

			GetMousePos(x,y);
			GetScreenSize(screen_w, screen_h);
			m_tooltip_widget.GetScreenSize(w,h);

			screen_w -= 10;
			screen_h -= 10;
			x += m_normal_item_size;
			y += m_normal_item_size;

			int right_edge = x + w;
			if (right_edge > screen_w) 
			{
				x = screen_w - w;
			}

			int bottom_edge = y + h;
			if (bottom_edge > screen_h)
			{
				y = y - h - (2*m_normal_item_size);
			}

			m_tooltip_widget.SetPos(x, y);
			m_tooltip_widget.Show(true);

			// item preview
			if ( m_item_preview_widget )
			{
				m_item_preview_widget.Destroy();
				m_item_preview_widget = NULL;
			}

			Widget preview_frame = m_tooltip_widget.FindAnyWidget("ItemFrameWidget");
			if (preview_frame)
			{
				float ww;
				float hh;
				preview_frame.GetSize(ww, hh);
				m_item_preview_widget = GetGame().GetWorkspace().CreateWidget(ItemPreviewWidgetTypeID, 0, 0, 1, 1, WidgetFlags.VISIBLE, ARGB(255, 255, 255, 255), 210, preview_frame);
				m_item_preview_widget.SetItem(item);
			}
		}
	}
	
	void HideTooltip()
	{
		m_tooltip_widget.Show(false);
		if (m_item_preview_widget)
		{
			m_item_preview_widget.Destroy();
			m_item_preview_widget = NULL;
		}
	}
	
	// singleton 	
	static InventoryManager GetInstance() 
	{ 
		return s_instance; 
	}

	string GetItemQuantityText( EntityAI item )
	{
		string quantity_text = "";
		if ( item.IsInherited( InventoryItem) )
		{
			ItemBase item_base = (ItemBase)item;
			float quantity = item_base.GetQuantity();
			int ammo;
			if ( item.IsInherited( Magazine) )
			{
				Magazine magazine_item = (Magazine)item;
				ammo = magazine_item.GetAmmoCount();
				quantity_text = ammo.ToString();
				return quantity_text;
			}
			else if ( item.IsInherited( ItemBook) )
			{
				return "";
			}
			int max = item.ConfigGetInt("stackedMax");
			string unit = item.ConfigGetString("stackedUnit");

			if (max > 0)
			{
				if (max == 1)
				{
					float tmp = Math.Round( ( quantity / max ) * 100 );
					quantity_text = tmp.ToString() + "%";
				}
				else
				{
					quantity_text = quantity.ToString();
					// if (unit == "ml")
					// {
					// 	float liters = round(quantity / 1000.0);
					// 	if ( quantity < 2000 )
					// 	{
					// 		liters =  ( round( (quantity / 100.0) ) ) / 10;
					// 	}
					// 	quantity_text = ftoa(liters) + "l";
					// }
					// else
					// {
					// 	quantity_text = itoa(quantity) + unit;
					// }
				}
			}
		}
		return quantity_text;
	}
	
	int HasItemQuantity( EntityAI item )
	{
		if ( item.IsInherited( InventoryItem ) )
		{
			ItemBase item_base = (ItemBase)item;
			if ( item.IsInherited( Magazine) )
			{
				return QUANTITY_COUNT;
			}
			else if ( item.IsInherited( ItemBook) )
			{
				return QUANTITY_HIDDEN;
			}
			int max = item.ConfigGetInt("stackedMax");
			bool bar = item.ConfigGetBool("quantityBar");
			if ( max > 0 )
			{
				if ( max == 1 || bar )
				{
					return QUANTITY_PROGRESS;
				}
				else
				{
					return QUANTITY_COUNT;
				}
			}
		}
		return QUANTITY_HIDDEN;
	}
	
	float GetItemQuantity( InventoryItem item )
	{
		float quantity = 0;
		if ( item.IsInherited( InventoryItem ) )
		{
			ItemBase item_base = (ItemBase)item;
			if ( item.IsInherited( Magazine) )
			{
				Magazine magazine_item = (Magazine)item;
				quantity = magazine_item.GetAmmoCount();
			}
			else
			{
				quantity = item_base.GetQuantity();
			}
		}
		return quantity;
	}
	
	void OnPerformCombination(int combinationFlags)
	{
		if (m_am_entity1.IsNull() || m_am_entity2.IsNull()) return;
		
		if (combinationFlags == InventoryCombinationFlags.NONE) return;
		/*
		if( GetGame().IsNewPlayer() && GetGame().IsDebugActions() )
		{
			if(combinationFlags == InventoryCombinationFlags.RECIPE)
			{
				Debug.Log("OnPerformCombination called","recipes");
				ModuleRecipesManager module = (ModuleRecipesManager)GetModule(Type("ModuleRecipesManager"));
				int id = module.GetValidRecipeID(m_am_entity1.Ptr(),m_am_entity2.Ptr(),m_player);
				Debug.Log("ID:"+ToString(id),"recipes");
			}
		}
		*/
		if (combinationFlags & InventoryCombinationFlags.ADD_AS_ATTACHMENT)
		{
			TakeAsOwned(m_am_entity1.Ptr(), m_am_entity2.Ptr());
		}
		
		if (combinationFlags & InventoryCombinationFlags.ADD_AS_CARGO)
		{
			m_am_entity1.Ptr().TakeEntityToCargo(m_am_entity2.Ptr());
		}
		
		if (combinationFlags & InventoryCombinationFlags.SWAP)
		{
			m_player.SwapEntities(m_am_entity1.Ptr(), m_am_entity2.Ptr());
		}
		
		if (combinationFlags & InventoryCombinationFlags.TAKE_TO_HANDS)
		{
			TakeToHands(m_am_entity2.Ptr());
		}
		
		if (m_am_entity1.Ptr().IsInherited(ItemBase) && m_am_entity2.Ptr().IsInherited(ItemBase))
		{
			ItemBase entity1ItemBase = m_am_entity1.Ptr();
			if (combinationFlags & InventoryCombinationFlags.COMBINE_QUANTITY)
			{
				entity1ItemBase.QuantityCombine(m_am_entity2.Ptr());
			}
		}
	}
}
