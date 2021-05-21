class InventoryViewBase: InventoryGridController
{
	protected InventoryContainer m_container;
	protected InventoryGrid m_icon_grid;
	protected InventoryGrid m_container_icon_grid;
	protected autoptr array<InventoryGrid> m_cargo_grids;
	protected autoptr array<Cargo> m_cargos;	
	protected autoptr Link<EntityAI> m_entity;
	protected autoptr Timer m_update_lock_timer;
	protected AutoHeightSpacer m_body_spacer;
	protected AutoHeightSpacer m_content_spacer;
	protected autoptr TItemsMap m_items;//tmp
	protected int m_properties;
	
	protected Widget m_container_icon;
	protected Widget m_root;
	protected Widget m_content;
	protected Widget m_body;
	protected Widget m_image_frame;
	protected TextWidget m_header;
	protected Widget m_minimize_widget;
	protected Widget m_maximize_widget;
	protected bool m_IsExpandable;	
	
	void InventoryViewBase( InventoryContainer container, EntityAI entity, int properties )
	{
		m_container = container;
		m_entity = new Link<EntityAI>(entity);
		m_cargo_grids = new array<InventoryGrid>;
		m_cargos = new array<Cargo>;
		m_items = new TItemsMap;
		m_update_lock_timer = new Timer();
		m_properties = properties;
	}

	void ~InventoryViewBase()
	{
	}

	void CreateLayout();
	
	void UpdateItems();
	
	protected InventoryItem FindItem(InventoryGrid grid, int row, int col);

	protected int FindCargoIndex(InventoryGrid grid);
	
	void UpdateSort()
	{
		if ( m_container.GetContentListWidget() && m_root )
		{
			m_container.GetContentListWidget().RemoveChild(m_root);
			m_container.GetContentListWidget().AddChild(m_root);
		}

		if ( m_container.GetContentGridWidget() && m_container_icon )
		{
			m_container.GetContentGridWidget().RemoveChild(m_container_icon);
			m_container.GetContentGridWidget().AddChild(m_container_icon);
		}
	}
	
	void SetHeaderText(string text)
	{
	}

	void AddProxyCargo(Cargo proxyCargo)
	{
	}
	
	void UpdateLock()
	{
		m_update_lock_timer.Run(0.5, this, "UpdateUnlock", NULL, false);
	}
	
	void UpdateUnlock()
	{
		//UpdateItems();
	}

	void InitAttachmentGrid( int panel_type )
	{	
	}
	
	void InitIconGrid()
	{
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
				cargo_grid.SetGridItemsSize(manager.GetNormalItemSize(),manager.GetNormalItemSize());
				cargo_grid.SetGridGapSize(manager.GRID_CARGO_GAP);
				cargo_grid.SetGridSize(cargo.GetWidth(), cargo.GetHeight());
				cargo_grid.SetController(this);
				cargo_grid.GenerateBackgroundTiles(cargo.GetWidth() * cargo.GetHeight());
				cargo_grid.SetQuantityPanelSize( manager.GetQuantityStackPanelSize() );
				UpdateCargoGrid(m_cargos.Count() - 1);
			}
		}
	}
	
	protected void UpdateAttachmentGrid()
	{
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
					m_entity.Ptr().TakeEntityAsOwned(dragged_item);
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
	
		EntityAI GetEntity() {
		return m_entity.Ptr();
	}
	
	// ScriptedWidgetEventHandler override
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
		
	// InventoryGridController override
	void OnItemEnter(InventoryGrid grid, Widget w, int row, int col)
	{
	}
	
	void OnItemLeave(InventoryGrid grid, Widget w)
	{
	}
	
	void OnItemDrag(InventoryGrid grid, Widget w, int row, int col)
	{
	}
	
	void OnItemDraggingOver(InventoryGrid grid, Widget w, int row, int col)
	{
		MoveItem(grid, row, col, true);
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
		MoveItem(grid, row, col, false);
		manager.Drop();
		UpdateLock();
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
	
	int GetItemColor(ScriptedWidgetEventHandler grid, InventoryItem item)
	{
		InventoryManager manager = InventoryManager.GetInstance();
		InventoryItem dragged_item = manager.GetItem();
		
		if ( dragged_item ) return 0;
		int color = InventoryGrid.ITEM_COLOR_NORMAL;
		// if item icon is main icon (view, viewgrid )
		PlayerBase player = GetGame().GetPlayer();
		if ( item.GetOwner() == player || item.GetOwner() == NULL )
		{
			color = InventoryGrid.ITEM_COLOR_TRANSPARENT;
		}
		if( item.IsInherited( InventoryItem) ) 
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

	Widget GetRootWidget() 
	{
  		return m_root;
  	}

  	Widget GetContainerIcon() 
	{
  		return m_container_icon;
  	}
}
