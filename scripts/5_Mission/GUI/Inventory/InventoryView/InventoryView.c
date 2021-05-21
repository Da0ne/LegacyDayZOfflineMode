class InventoryView: InventoryViewBase
{
//	protected InventoryContainer m_container;
	
	//protected InventoryGrid m_icon_grid;
//	protected InventoryGrid m_container_icon_grid;
	protected InventoryGrid m_attachment_icon_grid;
	protected InventoryGrid m_attachment_slot_grid;
//	protected autoptr array<InventoryGrid> m_cargo_grids;
	protected autoptr map<EntityAI, InventoryGrid> m_attachment_grids;
//	protected autoptr array<Cargo> m_cargos;	
//	protected autoptr Link<EntityAI> m_entity;
//	protected autoptr Timer m_update_lock_timer;
//	protected AutoHeightSpacer m_body_spacer;
//	protected AutoHeightSpacer m_content_spacer;
	protected AutoHeightSpacer m_attachment_spacer;
//	protected autoptr TItemsMap m_items;//tmp
//	protected int m_properties;
	
//	protected Widget m_container_icon;
//	protected Widget m_root;
	protected Widget m_attachment_grid;
//	protected Widget m_content;
//	protected Widget m_body;
//	protected Widget m_image_frame;
	protected Widget m_header_bar;
//	protected TextWidget m_header;
	protected Widget m_header_bg;
	protected TextWidget m_count;
//	protected Widget m_minimize_widget;
//	protected Widget m_maximize_widget;
	protected Widget m_empty_attachment_slot;
//	protected bool m_IsExpandable;	
	
	
	void InventoryView( InventoryContainer container, EntityAI entity, int properties )
	{
		m_container = container;
		m_entity = new Link<EntityAI>(entity);
		m_cargo_grids = new array<InventoryGrid>;
		m_attachment_grids = new map<EntityAI, InventoryGrid>;
		m_cargos = new array<Cargo>;
		m_items = new TItemsMap;
		m_update_lock_timer = new Timer();
		m_properties = properties;
	}

	void ~InventoryView()
	{
		SaveHistory();
		if (m_root) m_root.Destroy();
		if (m_container_icon) m_container_icon.Destroy();
	}

	void CreateLayout()
	{
		InventoryManager manager = InventoryManager.GetInstance();
		// if (m_container && m_container.IsOwnedByPlayer())
		// {
		// 	m_root = CreateWidgets("gui/layouts/inventory/inventoryViewRight.layout", m_container.GetContentListWidget());
		// }
		// else
		// {
		// 	m_root = CreateWidgets("gui/layouts/inventory/inventoryView.layout", m_container.GetContentListWidget());
		// }
		m_root = GetGame().GetWorkspace().CreateWidgets("gui/layouts/inventory/inventoryViewRight.layout", m_container.GetContentListWidget());
		m_content = m_root.FindAnyWidget("Content");
		m_attachment_grid = m_root.FindAnyWidget("AttachmentGrid");
		m_body = m_root.FindAnyWidget("Body");
		m_image_frame = m_root.FindAnyWidget("ImageFrame");
		m_header_bar = m_root.FindAnyWidget("HeaderBar");
		m_header = m_root.FindAnyWidget("Header");
		m_count = m_root.FindAnyWidget("Count");
		m_minimize_widget = m_root.FindAnyWidget("MinimizeWidget");
		m_maximize_widget = m_root.FindAnyWidget("MaximizeWidget");
		
		if (m_header_bar) m_header_bar.SetHandler(this);
		if (m_header) m_header.SetHandler(this);
		if (m_minimize_widget) m_minimize_widget.SetHandler(this);	
		if (m_maximize_widget) m_maximize_widget.SetHandler(this);
		if (m_body) m_body.SetHandler(this);

		if (m_content) m_content.GetScript(m_content_spacer);
		if (m_body) m_body.GetScript(m_body_spacer);
		if (m_attachment_grid) m_attachment_grid.GetScript(m_attachment_spacer);
		
		if (m_entity.IsNull()) return;
		
		InitIconGrid();
		InitAttachmentGrid( TYPE_PANEL_DEFAULT );
		InitCargoGrid(m_entity.Ptr().GetCargo(), TYPE_PANEL_DEFAULT);

		string name = m_entity.Ptr().GetName();	
		name.ToUpper();
		SetHeaderText(name);
		
		m_minimize_widget.Show( false );
		m_maximize_widget.Show( false );

		// if container is expandable (minimize/maximize button is visible)
		m_IsExpandable = false;
		if ( m_entity.Ptr().GetCargo() && m_entity.Ptr().GetCargo().GetHeight() > 0 )
		{
			m_IsExpandable = true;
		}
		if ( m_entity.Ptr().GetSlotsCount() > 0 )
		{
			m_IsExpandable = true;
		}
		UpdateHeight( TYPE_PANEL_DEFAULT );
		
		if ( IsHistoryExpanded() || m_body.IsVisible() )
		{
			m_minimize_widget.Show( true );
			m_maximize_widget.Show( false );
			m_body.Show( true );
		}
		else
		{
			m_minimize_widget.Show( false );
			m_maximize_widget.Show( true );
			m_body.Show( false );
		}

	/*	m_body_spacer.Update();
		m_content_spacer.Update();

		Widget container_root = m_root;
		AutoHeightSpacer spacer;
		container_root.GetScript(spacer);
		spacer.Update();*/

		SetHeaderCountText( m_entity.Ptr() );

		// transparent background under item preview in view header
		Widget item_ww = m_root.FindAnyWidget("GridBck");
		if ( item_ww )
		{
			item_ww.SetColor( 0x00000000 );
		}

		// locked 
		ImageWidget lock_image = m_root.FindAnyWidget("LockImage");
		/*
		bool is_locked = GetGame().IsInventoryTreeLocked(m_entity.Ptr(), GetGame().GetPlayer());
		if ( is_locked )
		{
			if ( lock_image )
			{
				lock_image.Show( true );
			}
			m_root.SetColor(0x3FFFFFFF);
		}
		else
		{
			if ( lock_image )
			{
				lock_image.Show( false );
			}
			m_root.SetColor(0xFFFFFFFF);
		}*/

			lock_image.Show( false );
		
		// m_body.SetColor(0x1FFFFF1F);
		// m_header.SetColor(0x1FFFFF1F);

		// disable dragging for zombies
		if ( m_entity.Ptr().IsInherited(ZombieBase) )
		{
			if ( m_icon_grid )
			{
				Widget item_w = m_icon_grid.GetItemBackground(0);
				if ( item_w )
				{
					item_w.ClearFlags(WidgetFlags.DRAGGABLE);
				}
			}
		}

	}
	
	void UpdateItems()
	{
		if (m_entity.IsNull()) return;
		if (m_update_lock_timer.IsRunning()) return;	
		InventoryManager manager = InventoryManager.GetInstance();
		
		UpdateAttachmentGrid();
		
		int i;
		int c = m_cargos.Count();
		for (i = 0; i < c; i++)
		{
			UpdateCargoGrid(i);
		}

			UpdateIconGrid();

		if ( m_container_icon )
		{
			TextWidget item_quantity = m_container_icon.FindAnyWidget("Quantity");
			if ( item_quantity )
			{
				item_quantity.SetText( manager.GetItemQuantityText( (ItemBase)m_entity.Ptr() ) );
			}	
		}

		UpdateHeight( TYPE_PANEL_DEFAULT );
		
		SetHeaderCountText( m_entity.Ptr() );
		
		m_body_spacer.Update();
		m_content_spacer.Update();
	}
	
	void UpdateHeight( int panel_type )
	{
		InventoryManager manager = InventoryManager.GetInstance();

		int cargo_item_height = manager.GetNormalItemSize();
		int icon_height = manager.GetAttachmentItemSize();
		int max_grid_width = manager.MAX_ATTACHMENT_GRID_WIDTH;
		if ( panel_type == TYPE_PANEL_HANDS )
		{
			cargo_item_height = manager.GetHandsNormalItemSize();
			icon_height = manager.GetHandsAttachmentItemSize();
			max_grid_width = manager.MAX_HANDS_ATTACHMENT_GRID_WIDTH;
		}

		// update size
		int cargo_height = 0;
		int attachment_height = 0;
		if ( m_entity.Ptr().GetCargo() && m_entity.Ptr().GetCargo().GetHeight() > 0 )
		{
			cargo_height = m_entity.Ptr().GetCargo().GetHeight();
			cargo_height = cargo_height * ( cargo_item_height + manager.GRID_CARGO_GAP );
		}
		int ac = m_entity.Ptr().AttachmentsCount();
		int sc = m_entity.Ptr().GetSlotsCount();

		if ( sc > 0 && !m_entity.Ptr().IsInherited( ZombieBase) )
		{
			float x, y;
			m_attachment_grid.GetPos(x, y);
			m_attachment_grid.SetPos(x, cargo_height );
			attachment_height = ac;
			if ( attachment_height >= max_grid_width )
			{
				attachment_height = attachment_height / max_grid_width;
				attachment_height = attachment_height + 1;
			}
			else
			{
				attachment_height = 1;
			}
			attachment_height = attachment_height * icon_height;
		}
			
		int height = cargo_height + attachment_height;
		if ( cargo_height && attachment_height )
		{
			height = height + m_content_spacer.Gap;
		}
		m_body_spacer.MinHeight = height;
		m_body_spacer.MaxHeight = height;
	}
	
	void SetHeaderText(string text)
	{
		if (m_header) m_header.SetText(text);
	}
	
	void SetHeaderCountText( EntityAI entity)
	{
		int slots_used = 0;
		int slots_count = 0;
		if ( entity.GetCargo() && entity.GetCargo().GetHeight() > 0 )
		{
			slots_count = entity.GetCargo().GetWidth() * entity.GetCargo().GetHeight();

			int cargo_index = m_cargos.Count() - 1;
			int c, i, index, x, y, w, h;
			Cargo cargo = entity.GetCargo();
			if (cargo )
			{
				c = cargo.GetItemCount();
				for (i = 0; i < c; i++)
				{
					cargo.GetItemSize(i, w, h);
					slots_used = slots_used + ( w * h );
				}
			}
			if (m_count) m_count.SetText( slots_used.ToString() + "/" + slots_count.ToString() );
		}
		else if ( entity.GetSlotsCount() > 0 )
		{
			int ac = entity.AttachmentsCount();
			int	sc = entity.GetSlotsCount();
			bool is_binocular = entity.IsInherited( EN5C_BinocularsBase);

			// weapons has extra slot for muzzle, but binoculars inherit from weapon too, but dont have this extra slot... cool
			if (is_binocular == false)
			{
				if (entity.IsWeapon())
				{
					Weapon weapon = entity;
					sc += weapon.GetMuzzlesCount();
				}
				if (ac > sc) sc = ac; // fix of some weapons which has 1 attachments but 0 slots...
			}
			slots_count = sc;
			slots_used = ac;
			if (m_count) m_count.SetText( "+" + slots_used.ToString() );
		}
	}
	
	void AddProxyCargo(Cargo proxyCargo)
	{
		if (m_cargos.Find(proxyCargo) == INDEX_NOT_FOUND)
		{
			InitCargoGrid(proxyCargo, TYPE_PANEL_DEFAULT);
		}
	}
	
	void UpdateLock()
	{
		m_update_lock_timer.Run(0.5, this, "UpdateUnlock", NULL, false);
	}
	
	void UpdateUnlock()
	{
		UpdateItems();
	}
	
	void InitAttachmentGrid( int panel_type )
	{
		if ( !HasProperty(InventoryViewProperties.ATTACHMENTS) ) return;

		InventoryManager manager = InventoryManager.GetInstance();
		int ac = m_entity.Ptr().AttachmentsCount();
		int	sc = m_entity.Ptr().GetSlotsCount();
		
		int item_width = manager.GetAttachmentItemSize();
		int item_height = manager.GetAttachmentItemSize();
		int max_grid_width = manager.MAX_ATTACHMENT_GRID_WIDTH;
		if ( panel_type == TYPE_PANEL_HANDS )
		{
			item_width = manager.GetHandsAttachmentItemSize();
			item_height = manager.GetHandsAttachmentItemSize();
			max_grid_width = manager.MAX_HANDS_ATTACHMENT_GRID_WIDTH;
		}

		bool is_binocular = m_entity.Ptr().IsInherited( EN5C_BinocularsBase);

		// weapons has extra slot for muzzle, but binoculars inherit from weapon too, but dont have this extra slot... cool
		if (is_binocular == false)
		{
			if (m_entity.Ptr().IsWeapon())
			{
				// weapons muzzles fix
				Weapon weapon = m_entity.Ptr();
				sc += weapon.GetMuzzlesCount();
			}
				
			if (ac > sc) sc = ac; // fix of some weapons which has 1 attachments but 0 slots...
		}
		
		if (sc == 0) return;
		
		if (m_attachment_grid) m_attachment_grid.GetScript(m_attachment_icon_grid);
		if ( m_attachment_grid )
		{
			for (int i = 0; i < ac; i++)
			{	
				Widget attachment_icon;
				attachment_icon = GetGame().GetWorkspace().CreateWidgets("gui/layouts/inventory/inventoryIcon.layout", m_attachment_grid );

				if (attachment_icon) attachment_icon.GetScript( m_attachment_slot_grid );
				if ( m_attachment_slot_grid )
				{
					EntityAI entity = m_entity.Ptr().GetAttachmentFromIndex(i);
					m_attachment_grids.Set( entity, m_attachment_slot_grid );

					if ( entity )
					{
						m_attachment_slot_grid.SetGridItemsSize(manager.GetAttachmentItemSize(),manager.GetAttachmentItemSize());
						m_attachment_slot_grid.SetGridSize(1,1);
						m_attachment_slot_grid.SetController( this );		
						m_attachment_slot_grid.GenerateBackgroundTiles(1);
						m_attachment_slot_grid.SetQuantityPanelSize( manager.GetQuantityStackPanelSize() );
						m_attachment_slot_grid.AddItem( entity, Vector(0, 1, 1), Vector(0,0,0) );
					}
				}
			}

			if ( ac < sc )
			{
				m_empty_attachment_slot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/inventory/inventoryGridItemEmpty.layout", m_attachment_grid );
				m_empty_attachment_slot.SetSize( manager.GetAttachmentItemSize(), manager.GetAttachmentItemSize() );
				m_empty_attachment_slot.SetHandler(this);
			}
		}
	}
	
	void InitIconGrid()
	{
		Widget imageGridW = m_image_frame.FindAnyWidget("Grid");
		Widget imageW = m_image_frame.FindAnyWidget("Icon");
		InventoryManager manager = InventoryManager.GetInstance();
		
		if( !m_entity.Ptr().IsInherited( InventoryItem) ) 
		{
			/* if (imageGridW) imageGridW.Show(false);
			if (imageW) 
			{
				imageW.SetSize(manager.GetIconItemSize(), manager.GetIconItemSize());
				imageW.Show(true);
			}
			return;*/
		}
		
		if (imageGridW) imageGridW.Show(true);
		if (imageW) imageW.Show(false);
		
		if (imageGridW)	imageGridW.GetScript(m_icon_grid);
		// list view
		if (m_icon_grid)
		{
			m_icon_grid.SetController(this);
			m_icon_grid.SetGridItemsSize(manager.GetIconItemSize(),manager.GetIconItemSize());
			m_icon_grid.GenerateBackgroundTiles(1);
			m_icon_grid.SetQuantityPanelSize( manager.GetQuantityStackPanelSize() );
				m_icon_grid.AddItem(m_entity.Ptr(), Vector(0, 1, 1), Vector(0,0,0) );
		}
	}
	
	void InitCargoGrid(Cargo cargo, int panel_type)
	{
		if ( !HasProperty(InventoryViewProperties.CARGOS) ) return;
		
		if (cargo)
		{
			InventoryManager manager = InventoryManager.GetInstance();
			InventoryGrid cargo_grid;

			m_cargos.Insert(cargo);
			
			Widget cargoGridW;
			if (m_container && m_container.IsOwnedByPlayer())
			{
				cargoGridW = GetGame().GetWorkspace().CreateWidgets("gui/layouts/inventory/inventoryGridRight.layout", m_content);
			}
			else
			{
				cargoGridW = GetGame().GetWorkspace().CreateWidgets("gui/layouts/inventory/inventoryGrid.layout", m_content);
			}

			if (cargoGridW) cargoGridW.GetScript(cargo_grid);		
			if (cargo_grid)
			{
				m_cargo_grids.Insert(cargo_grid);
				int width = manager.GetNormalItemSize();
				if ( panel_type == TYPE_PANEL_HANDS )
				{
					width = manager.GetHandsNormalItemSize();
				}
				cargo_grid.SetGridItemsSize( width, width );
				cargo_grid.SetGridGapSize(manager.GRID_CARGO_GAP);
				cargo_grid.SetGridSize(cargo.GetWidth(), cargo.GetHeight());
				cargo_grid.SetController(this);
				cargo_grid.GenerateBackgroundTiles(cargo.GetWidth() * cargo.GetHeight());
				cargo_grid.SetQuantityPanelSize( manager.GetQuantityStackPanelSize() );
				
				UpdateCargoGrid(m_cargos.Count() - 1);
			}
		}
	}
	
	void UpdateAttachmentGrid()
	{
		if ( !HasProperty(InventoryViewProperties.ATTACHMENTS) ) return;
		InventoryManager manager = InventoryManager.GetInstance();

		int i;
		InventoryBase item;
		int	sc = m_entity.Ptr().GetSlotsCount();
		int ac = m_entity.Ptr().AttachmentsCount();
		
		EntityAI entity;
		Widget attachment_icon;

		autoptr array<EntityAI> m_entities = new array<EntityAI>;
		m_entities.Clear();

		// get list of all attachments
		for (i = 0; i < ac; i++)
		{
			entity = m_entity.Ptr().GetAttachmentFromIndex(i);
			m_entities.Insert(entity);

			
			// add new attachments
			if ( m_attachment_grids.Contains(entity) == false)
			{
				attachment_icon = GetGame().GetWorkspace().CreateWidgets("gui/layouts/inventory/inventoryIcon.layout", m_attachment_grid );
				if (attachment_icon) attachment_icon.GetScript( m_attachment_slot_grid );
				if ( m_attachment_slot_grid )
				{
					EntityAI attachment_entity = m_entity.Ptr().GetAttachmentFromIndex(i);
					m_attachment_grids.Set( attachment_entity, m_attachment_slot_grid );

					if ( attachment_entity )
					{
						m_attachment_slot_grid.SetGridItemsSize(manager.GetAttachmentItemSize(),manager.GetAttachmentItemSize());
						m_attachment_slot_grid.SetGridSize(1,1);
						m_attachment_slot_grid.SetController( this );		
						m_attachment_slot_grid.GenerateBackgroundTiles(1);
						m_attachment_slot_grid.SetQuantityPanelSize( manager.GetQuantityStackPanelSize() );
						m_attachment_slot_grid.AddItem( attachment_entity, Vector(0, 1, 1), Vector(0,0,0) );
					}
				}
			}
			else
			{			
				attachment_icon = m_attachment_grids.Get(entity).GetRoot();
			}

			// quantity
			if ( attachment_icon )
			{
				Widget quantity_panel = attachment_icon.FindAnyWidget("QuantityPanel");
				TextWidget item_quantity = attachment_icon.FindAnyWidget("Quantity");
				ProgressBarWidget quantity_progress = attachment_icon.FindAnyWidget("QuantityBar");
				Widget quantity_stack = attachment_icon.FindAnyWidget("QuantityStackPanel");
				if ( quantity_progress ) quantity_progress.Show( false );
				if ( quantity_stack ) quantity_stack.Show( false );

				int has_quantity = manager.HasItemQuantity( entity );
				if ( has_quantity == QUANTITY_HIDDEN )
				{
					if ( quantity_panel ) quantity_panel.Show( false );
				}
				else
				{
					if ( quantity_panel ) quantity_panel.Show( true );
					if ( has_quantity == QUANTITY_COUNT && item_quantity )
					{
						item_quantity.SetText( manager.GetItemQuantityText( entity ) );
						quantity_stack.Show( true );
					}
					else if ( has_quantity == QUANTITY_PROGRESS )
					{
						if ( quantity_progress )
						{
							float progress_max = quantity_progress.GetMax();
							int max = entity.ConfigGetInt("stackedMax");
							int count = entity.ConfigGetInt("count");
							float quantity = manager.GetItemQuantity( entity );
							if ( count > 0 )
							{
								max = count;
							}
							if ( max > 0 )
							{

								float value = Math.Round( ( quantity / max ) * 100 );
								// LogInfo( " ==" + ftoa( value ), LogTemplates.TEMPLATE_JANOSIK );
								quantity_progress.SetCurrent( value );
							}
							quantity_progress.Show( true );
						}
					}
				}

				// temperature
				Widget item_w = attachment_icon.FindAnyWidget("GridItem");
				if ( item_w )
				{
					int color = GetItemColor( this, entity);
					if ( color )
					{
						item_w.SetColor( color );
					}
				}
			}
		}
		
		// remove unnecesary attachments
		int c = m_attachment_grids.Count();
		for (i = 0; i < c; i++)
		{
			entity = m_attachment_grids.GetKey(i);
			if ( m_entities.Find(entity) == INDEX_NOT_FOUND )
			{
				InventoryGrid grid = m_attachment_grids.Get(entity);
				if( grid != NULL )
				{
					Widget root = grid.GetRoot();
					if( root != NULL )
					{
						root.Destroy();
					}
					m_attachment_grids.Remove( entity );
				}
				
				c--;
				i--;
			}
		}

		ac = m_entity.Ptr().AttachmentsCount();
		if ( ac == sc )
		{
			if (m_empty_attachment_slot) m_empty_attachment_slot.Destroy();
			m_empty_attachment_slot = NULL;
		}
		else if ( m_empty_attachment_slot == NULL )
		{
			m_empty_attachment_slot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/inventory/inventoryGridItemEmpty.layout", m_attachment_grid );
			m_empty_attachment_slot.SetSize( manager.GetAttachmentItemSize(), manager.GetAttachmentItemSize() );
			m_empty_attachment_slot.SetHandler(this);
		}

		if (m_attachment_spacer) m_attachment_spacer.Update();

		m_body_spacer.Update();
		m_content_spacer.Update();

		Widget container_root = m_root; //m_container.GetRootWidget();
		AutoHeightSpacer spacer;
		container_root.GetScript(spacer);
		if ( spacer )
		{
			spacer.Update();
		}
	}
	
	void UpdateIconGrid()
	{
		if ( m_icon_grid )
		{
			m_icon_grid.UpdateQuantityItems();
			m_icon_grid.UpdateTemperatureItems();
		}

		if ( m_container_icon_grid )
		{
			m_container_icon_grid.UpdateQuantityItems();
			m_container_icon_grid.UpdateTemperatureItems();
		}
	}
	
	void UpdateCargoGrid(int cargo_index)
	{
		if ( !HasProperty(InventoryViewProperties.CARGOS) ) return;
		
		if (cargo_index < m_cargos.Count())
		{
			int c, i, index, x, y, w, h;
			InventoryBase item;
			Cargo cargo = m_cargos.Get(cargo_index);
		
			InventoryGrid cargo_grid = m_cargo_grids.Get(cargo_index);
			if (cargo && cargo_grid)
			{
				m_items.Clear();
				c = cargo.GetItemCount();
				
				for (i = 0; i < c; i++)
				{
					item = cargo.GetItem(i);
					cargo.GetItemPos(i, x, y);
					cargo.GetItemSize(i, w, h);
					index = x + (y * cargo.GetWidth());
					m_items.Set(item, Vector(index, w, h));
				}
				
				cargo_grid.UpdateItems(m_items,true,true);
			}
		}
	}
	
	InventoryItem FindItem(InventoryGrid grid, int row, int col)
	{
		if (m_entity.IsNull() || grid == NULL) return NULL;

		if (grid == m_icon_grid)
		{
			return m_entity.Ptr();		
		}
		
		if (grid == m_container_icon_grid)
		{
			return m_entity.Ptr();	
		}

		int cargo_index = FindCargoIndex(grid);
		// use cargo index here
		if (cargo_index != INDEX_NOT_FOUND)	return m_cargos.Get(cargo_index).FindEntityInCargoOn(row, col);

		EntityAI entity = m_attachment_grids.GetKeyByValue( grid );
		if ( entity ) return entity;	
	}
	
	int FindCargoIndex(InventoryGrid grid)
	{
		for (int cargo_index = 0; cargo_index < m_cargo_grids.Count(); cargo_index++)
		{
			InventoryGrid cargo_grid = m_cargo_grids.Get(cargo_index);
			if (grid == cargo_grid)
			{
				return cargo_index;
			}
		}
		
		return INDEX_NOT_FOUND;
	}
	
	bool MoveItem(InventoryGrid grid, int row, int col, bool testOnly)
	{
		if (m_entity.IsNull() || grid == NULL) return false;

		PlayerBase player = GetGame().GetPlayer();
		InventoryManager manager = InventoryManager.GetInstance();
		InventoryItem dragged_item = manager.GetItem();
		InventoryItem itemAtPos = FindItem(grid, row, col);
		InventoryGrid gridSrc = manager.GetGrid();
		int index_from, index_to, flags;
		vector data;
		
		if (manager.IsDragging() == false || dragged_item == NULL) return false;	
		
		if (dragged_item && dragged_item.CanRemoveEntity() == false)
		{
			if (testOnly) manager.SetItemColor(InventoryGrid.ITEM_COLOR_WRONG);
				return false;
		}
		
		if (grid == m_icon_grid || grid == m_container_icon_grid)
		{
			// dragging icon
			if (dragged_item == m_entity.Ptr())
			{
				if (testOnly) manager.SetItemColor(InventoryGrid.ITEM_COLOR_DRAG);
				return false;
			}
			
			flags = manager.GetCombinationFlags(m_entity.Ptr(), dragged_item);
			if (flags == InventoryCombinationFlags.NONE)
			{
				if (testOnly) manager.SetItemColor(InventoryGrid.ITEM_COLOR_WRONG);
				return false;
			}
			else if (flags == InventoryCombinationFlags.ADD_AS_ATTACHMENT || flags == InventoryCombinationFlags.ADD_AS_CARGO )
			{
				if (testOnly) manager.SetItemColor(InventoryGrid.ITEM_COLOR_GOOD);
				else
				{
					if (grid.DebugOutput) player.MessageStatus(m_entity.Ptr().GetName() + " :TakeEntityAsOwned( " + dragged_item.GetName() + " )");

					if (gridSrc) gridSrc.RemoveItem(dragged_item);
							
					manager.TakeAsOwned(m_entity.Ptr(), dragged_item);		
				}
				return true;
			}
			else if (flags == InventoryCombinationFlags.TAKE_TO_HANDS)
			{
				if (testOnly) manager.SetItemColor(InventoryGrid.ITEM_COLOR_SWAP);
				else
				{
					manager.TakeToHands(dragged_item);
				}
				return true;
			}
			else
			{
				if (testOnly) manager.SetItemColor(InventoryGrid.ITEM_COLOR_MULTIPLE);
				else manager.ShowActionMenuCombine(itemAtPos, dragged_item, flags);
				return true;
			}
		}
		else if (itemAtPos && dragged_item != itemAtPos)
		{
			flags = manager.GetCombinationFlags(itemAtPos, dragged_item);

			if (flags == InventoryCombinationFlags.NONE)
			{
				if (testOnly) manager.SetItemColor(InventoryGrid.ITEM_COLOR_WRONG);
				return false;
			}
			else if (flags == InventoryCombinationFlags.SWAP)
			{
				if (testOnly) manager.SetItemColor(InventoryGrid.ITEM_COLOR_SWAP);
				else
				{
					if (grid.DebugOutput) player.MessageStatus("Swap: " + dragged_item.GetName() + " , " + itemAtPos.GetName());
					
					if (grid.HasItem(dragged_item))
					{
						gridSrc = grid;
					}
					
					if (gridSrc) index_from = Math.Round(gridSrc.GetItems().Get(dragged_item)[0]); 				
					index_to = Math.Round(grid.GetItems().Get(itemAtPos)[0]);				
									
					if (gridSrc) gridSrc.RemoveItem(dragged_item);	
					grid.RemoveItem(itemAtPos);
					
					if (gridSrc) 
					{
						data = gridSrc.GetController().GetItemSize(gridSrc, itemAtPos);
						data[0] = index_from;
						gridSrc.AddItem(itemAtPos, data, Vector(0,0,0) );
					}
					
					data = grid.GetController().GetItemSize(grid, dragged_item);
					data[0] = index_to;				
					grid.AddItem(dragged_item, data, Vector(0,0,0) );
					
					player.SwapEntities(dragged_item, itemAtPos);
				}
				return true;
			}
			else
			{
				if (testOnly) manager.SetItemColor(InventoryGrid.ITEM_COLOR_MULTIPLE);
				else manager.ShowActionMenuCombine(itemAtPos, dragged_item, flags);
				return true;
			}
		}
		/*
		else if (grid == m_attachment_grid) 
		{
			// dragging over empty attachment slot
			if (m_entity.Ptr().CanAddAttachment(dragged_item))
			{
				if (testOnly) 
				{
					manager.SetItemColor(InventoryGrid.ITEM_COLOR_GOOD);
				}
				else
				{
					if (grid.DebugOutput) player.MessageStatus(m_entity.Ptr().GetName() + " :TakeEntityAsAttachment( " + dragged_item.GetName() + " )");
					
					if (gridSrc) gridSrc.RemoveItem(dragged_item);
					
					data = grid.GetController().GetItemSize(grid, dragged_item);
					data[0] = grid.GetIndex(row, col);
					grid.AddItem(dragged_item, data, Vector(0,0,0) );				
					
					m_entity.Ptr().TakeEntityAsOwned(dragged_item);
				}
				return true;
			}
		}
		*/
		else
		{
			// dragging over empty cargo slot
			int cargo_index = FindCargoIndex(grid);
			
			if (cargo_index != INDEX_NOT_FOUND)	
			{
				Cargo cargo = m_cargos.Get(cargo_index);
				
				// use cargo index here
				if (m_entity.Ptr().CanAddEntityInCargoEx(dragged_item, /*cargo,*/ row, col))			
				{
					if (testOnly)
					{
						manager.SetItemColor(InventoryGrid.ITEM_COLOR_GOOD);
					}
					else
					{
						if (grid.DebugOutput) player.MessageStatus(m_entity.Ptr().GetName() + " :TakeEntityToCargoAtPos( " + dragged_item.GetName() + " )");
						
						if (gridSrc) 
						{
							if (dragged_item.GetOwner() != m_entity.Ptr()) 
							{
								gridSrc.RemoveItem(dragged_item);
							}
							else
							{
								grid.RemoveItem(dragged_item);
							}
						}
						
					
						data = grid.GetController().GetItemSize(grid, dragged_item);
						data[0] = grid.GetIndex(row, col);
						grid.AddItem(dragged_item, data, Vector(0,0,0) );			
							
						m_entity.Ptr().TakeEntityToCargoAtPos(dragged_item, cargo, row, col);
					}
					return true;
				}
			}		
		}
		
		if (testOnly) manager.SetItemColor(InventoryGrid.ITEM_COLOR_WRONG);
		return false;
	}
	
	protected bool HasProperty(int mask) 
	{
		if (m_properties & mask) return true;
		return false;	
	}	
	
	EntityAI GetEntity() 
	{
		return m_entity.Ptr();
	}
	
	// ScriptedWidgetEventHandler override
	bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if ( w == m_header_bar || w == m_header || w == m_minimize_widget || w == m_maximize_widget )
		{
			if  ( m_body && m_body_spacer && m_content_spacer )
			{
				if ( m_body.IsVisible() )
				{
					m_minimize_widget.Show( false );
					m_maximize_widget.Show( true );
					m_body.Show( false );
				}	
				else
				{
					m_minimize_widget.Show( true );
					m_maximize_widget.Show( false );
					m_body.Show( true );
				}
				m_body_spacer.Update();
				m_content_spacer.Update();

				Widget container_root = m_root; //m_container.GetRootWidget();
				AutoHeightSpacer spacer;
				container_root.GetScript(spacer);
				spacer.Update();
			}
		}
		return true;
	}
		
	// InventoryGridController override
	void OnItemEnter(InventoryGrid grid, Widget w, int row, int col)
	{
		if (m_update_lock_timer.IsRunning()) return;
		InventoryManager manager = InventoryManager.GetInstance();
		InventoryItem item = FindItem( grid, row, col );
		if ( item )
		{
			manager.ShowTooltip( item );
		}
	}

	void OnItemLeave(InventoryGrid grid, Widget w)
	{
		InventoryManager manager = InventoryManager.GetInstance();
		manager.HideTooltip();
	}

	void OnItemDrag(InventoryGrid grid, Widget w, int row, int col)
	{
		if (m_entity.IsNull()) return;
		
		InventoryManager manager = InventoryManager.GetInstance();
		InventoryItem item = FindItem(grid, row, col);
		
		manager.Drag(InventoryDragFromType.GRID, item, grid);
	}
	
	void OnItemDraggingOver(InventoryGrid grid, Widget w, int row, int col)
	{
		InventoryManager manager = InventoryManager.GetInstance();


		InventoryMenu menu = GetGame().GetUIManager().GetMenu();
		if ( menu )
		{
			menu.HidePanelBorders();
			if ( m_container == menu.m_equipment_container )
			{
				menu.ShowRightPanelBorder();
			}
			else if ( m_container == menu.m_ground_container )
			{
				menu.ShowLeftPanelBorder();
			}
		}

		// disable dragging on zombie
		if (m_entity.Ptr().IsInherited( ZombieBase) && grid == m_icon_grid )
		{
			manager.SetItemColor(InventoryGrid.ITEM_COLOR_WRONG);
			return;
		}
		
		PlayerBase player = GetGame().GetPlayer();
		EntityAI item_in_hands = player.GetEntityInHands();
		if ( item_in_hands == NULL && m_container.IsInherited( InventoryContainerHands) )
		{
			InventoryItem dragged_item = manager.GetItem();
			if (manager.IsDragging() == false || dragged_item == NULL) return;
			if (dragged_item && dragged_item.CanRemoveEntity() == false)
			{
				manager.SetItemColor(InventoryGrid.ITEM_COLOR_WRONG);
			}
			else
			{
				manager.SetItemColor(InventoryGrid.ITEM_COLOR_GOOD);
			}
		}
		else
		{
			MoveItem(grid, row, col, true);
		}
	}
	
	void OnItemDrop(InventoryGrid grid, Widget w)
	{
		InventoryManager manager = InventoryManager.GetInstance();
		manager.Drop();
		manager.HideTooltip();
		UpdateLock();
	}

	void OnItemDropReceived(InventoryGrid grid, Widget w, int row, int col)
	{
		InventoryManager manager = InventoryManager.GetInstance();
		
		// disable dropping on zombie
		if ( m_entity && m_entity.Ptr() && grid == m_icon_grid && m_entity.Ptr().IsInherited( ZombieBase)  )
		{
			return;
		}

		PlayerBase player = GetGame().GetPlayer();
		EntityAI item_in_hands = player.GetEntityInHands();
		Print(m_container );
		Print(m_container.IsInherited(InventoryContainerHands));
		if ( item_in_hands == NULL && m_container.IsInherited(InventoryContainerHands) )
		{
			InventoryItem dragged_item = manager.GetItem();
			
			if (manager.IsDragging() == false || dragged_item == NULL) return;
			
			if ( player.CanAddEntityInHands(dragged_item) && (player.GetEntityInHands() != dragged_item || player.CanRemoveEntityInHands()))
			{
				manager.TakeToHands(dragged_item);
				manager.Drop();	
			}
		}
		else
		{
			MoveItem(grid, row, col, false);
			manager.Drop();
			UpdateLock();
		}
		InventoryMenu menu = GetGame().GetUIManager().GetMenu();
		if ( menu )
		{
			menu.HidePanelBorders();
		}
	}
	
	void OnItemRightClick(InventoryGrid grid, Widget w, int row, int col)
	{
		if (m_entity.IsNull() || grid == NULL) return;
		
		PlayerBase player = GetGame().GetPlayer();
		InventoryManager manager = InventoryManager.GetInstance();
		InventoryItem itemAtPos = FindItem(grid, row, col);
		
		if (itemAtPos)
		{
			manager.ShowActionMenu(itemAtPos);		
		}
	}

	void OnItemLeftClick(InventoryGrid grid, Widget w, int row, int col)
	{
	}

	void OnItemDoubleClick(InventoryGrid grid, Widget w, int row, int col)
	{
		InventoryItem itemAtPos = FindItem(grid, row, col);
		if (m_container && itemAtPos) m_container.OnItemDoubleClick(this, itemAtPos);
		InventoryManager manager = InventoryManager.GetInstance();	
		manager.HideTooltip();
	}	
	
	vector GetItemSize(InventoryGrid grid, InventoryItem item)
	{
		int w = 1;
		int h = 1;
		
		if (grid != m_container_icon_grid) 
		{
			GetGame().GetInventoryItemSize(item, w, h);
		}
		
		return Vector(0, w, h);
	}

	int GetItemColor(ScriptedWidgetEventHandler grid, InventoryItem item)
	{
		InventoryManager manager = InventoryManager.GetInstance();
		InventoryItem dragged_item = manager.GetItem();
		PlayerBase player = GetGame().GetPlayer();
		
		bool is_view = grid.IsInherited(  InventoryView );
		if ( dragged_item ) return 0;
		// int color = InventoryGrid.ITEM_COLOR_NORMAL;
		int color = InventoryGrid.ITEM_COLOR_TRANSPARENT;
		if ( is_view ) 
		{
			color = InventoryGrid.ITEM_COLOR_GRID;
		}

		// if item icon is main icon (view, viewgrid )
		if ( item.IsInherited(  InventoryItem) && item.GetOwner() && item.GetOwner() != player )
		{
			color = InventoryGrid.ITEM_COLOR_NORMAL;
		}

		if( item.IsInherited(  InventoryItem)  ) 
		{
			ItemBase item_base = (ItemBase) item;
			float temperature = item_base.GetTemperature();
			if ( temperature )
			{
				color = GetTemperatureColor( temperature );
			}
		}
		return color;
	}
	
	string GetItemQuantityText( InventoryItem item )
	{
		InventoryManager manager = InventoryManager.GetInstance();
		string quantity_text = manager.GetItemQuantityText( item );
		return quantity_text;
	}

	int HasItemQuantity( InventoryItem item )
	{
		InventoryManager manager = InventoryManager.GetInstance();
		int has_quantity = manager.HasItemQuantity( item );
		return has_quantity;
	}

	float GetItemQuantity( InventoryItem item )
	{
		InventoryManager manager = InventoryManager.GetInstance();
		float quantity = manager.GetItemQuantity( item );
		return quantity;
	}
	
	protected const int TYPE_PANEL_DEFAULT = 0;
	protected const int TYPE_PANEL_HANDS = 1;

	// history saving
	protected static const int EXPANDED_HISTORY_SIZE = 20;
	protected static int s_expanded_history[EXPANDED_HISTORY_SIZE];
	protected static int s_expanded_history_index = 0;
	
	bool IsHistoryExpanded()
	{
		if (m_entity.IsNull()) return false;
		
		int id = m_entity.Ptr().GetID();
		
		for (int i = 0; i < EXPANDED_HISTORY_SIZE; i++)
		{
			if (s_expanded_history[i] == id) 
			{
				return true;
			}
		}
		
		return false;
	}

	static void ClearHistory()
	{
		for (int i = 0; i < EXPANDED_HISTORY_SIZE; i++)
		{
			s_expanded_history[i] = -1;
		}
		s_expanded_history_index = 0;
	}

	void SaveHistory()
	{
		if (m_entity.IsNull()) return;
		
		if  ( m_body && m_body_spacer && m_content_spacer )
		{
			if (m_body_spacer.MaxHeight == 0)
			{
				s_expanded_history[s_expanded_history_index] = m_entity.Ptr().GetID();
				s_expanded_history_index++;
			}		
		}
	}

	// ScriptedWidgetEventHandler override	
	bool OnDraggingOver(Widget w, int x, int y, Widget reciever)
	{
		OnItemDraggingOver( m_icon_grid, w, 0, 0 );
		return true;
	}

	bool OnDropReceived(Widget w, int x, int y, Widget reciever)
	{
		OnItemDropReceived( m_icon_grid, w, 0, 0 );
		return true;
	}
	
}
