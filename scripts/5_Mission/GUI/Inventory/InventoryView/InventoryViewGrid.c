class InventoryViewGrid: InventoryViewBase
{
//	protected InventoryContainer m_container;
	protected InventoryGrid m_attachment_grid;
//	protected InventoryGrid m_icon_grid;
//	protected InventoryGrid m_container_icon_grid;
//	protected autoptr array<InventoryGrid> m_cargo_grids;
//	protected autoptr array<Cargo> m_cargos;	
//	protected autoptr Link<EntityAI> m_entity;
//	protected autoptr Timer m_update_lock_timer;
//	protected AutoHeightSpacer m_body_spacer;
//	protected AutoHeightSpacer m_content_spacer;
//	protected autoptr TItemsMap m_items;		//tmp
//	protected int m_properties;
	
//	protected Widget m_container_icon;
//	protected Widget m_root;
//	protected Widget m_content;
//	protected Widget m_body;
//	protected Widget m_image_frame;
//	protected TextWidget m_header;
//	protected Widget m_minimize_widget;
//	protected Widget m_maximize_widget;
//	protected bool m_IsExpandable;	
	
	
	void InventoryViewGrid( InventoryContainer container, EntityAI entity, int properties )
	{
		m_container = container;
		m_entity = new Link<EntityAI>(entity);
		m_cargo_grids = new array<InventoryGrid>;
		m_cargos = new array<Cargo>;
		m_items = new TItemsMap;
		m_update_lock_timer = new Timer();
		m_properties = properties;
	}

	void ~InventoryViewGrid()
	{
		SaveHistory();
		if (m_root) m_root.Destroy();
		if (m_container_icon) m_container_icon.Destroy();
	}

	void CreateLayout()
	{
		InitIconGrid();
	}

	void UpdateItems()
	{
		if (m_entity.IsNull()) return;
		if (m_update_lock_timer.IsRunning()) return;	

		UpdateIconGrid();

		if ( m_container_icon )
		{
			InventoryManager manager = InventoryManager.GetInstance();
			TextWidget item_quantity = m_container_icon.FindAnyWidget("Quantity");
			if ( item_quantity )
			{
				item_quantity.SetText( manager.GetItemQuantityText( (ItemBase)m_entity.Ptr() ) );
			}
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

	void InitIconGrid()
	{
		InventoryManager manager = InventoryManager.GetInstance();

		// grid view
		if (m_container.GetContentGridWidget())
		{
			// LogInfo(ToString( "InitIconGrid1"), LogTemplates.TEMPLATE_JANOSIK );
			m_container_icon = GetGame().GetWorkspace().CreateWidgets("gui/layouts/inventory/inventoryIcon.layout", m_container.GetContentGridWidget());
			if (m_container_icon) m_container_icon.GetScript(m_container_icon_grid);		
			if (m_container_icon_grid)
			{
				// LogInfo(ToString( "InitIconGrid2"), LogTemplates.TEMPLATE_JANOSIK );
				m_container_icon_grid.SetGridItemsSize(manager.GetAttachmentItemSize(),manager.GetAttachmentItemSize());
				m_container_icon_grid.SetGridSize(1,1);
				m_container_icon_grid.SetController(this);		
				m_container_icon_grid.GenerateBackgroundTiles(1);
				m_container_icon_grid.SetQuantityPanelSize( manager.GetQuantityStackPanelSize() );
				m_container_icon_grid.AddItem(m_entity.Ptr(), Vector(0, 1, 1), Vector(0,0,0) );
			}
			m_container_icon.Show( true );
			m_container_icon.SetColor( 0x00000000 ); // transparent background under item preview
			Widget contentGrid =  m_container.GetContentGridWidget();
			if ( m_container.isContentGrid() )
			{
				contentGrid.Show( true );
			}
			//	m_container_icon_grid.Show( true );
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

	protected bool HasProperty(int mask) 
	{
		if (m_properties & mask) return true;
		return false;	
	}	
	
		EntityAI GetEntity() {
		return m_entity.Ptr();
	}
	
	// ScriptedWidgetEventHandler override
	bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if ( w == m_header || w == m_minimize_widget || w == m_maximize_widget )
		{
			if  ( m_body && m_body_spacer && m_content_spacer )
			{
				if (m_body_spacer.MaxHeight)
				{
					m_body_spacer.MaxHeight = 0;
					if ( m_IsExpandable )
					{
						m_minimize_widget.Show( true );
						m_maximize_widget.Show( false );
					}
				}
				else
				{
					InventoryManager manager = InventoryManager.GetInstance();
					int gap = m_content_spacer.Gap;			
					m_body_spacer.MaxHeight = manager.GetAttachmentItemSize() + gap;
					if ( m_IsExpandable )
					{
						m_minimize_widget.Show( false );
						m_maximize_widget.Show( true );
					}
				}
				
				m_body_spacer.Update();
			}
		}
		return false;
	}
		
	// InventoryGridController override
	void OnItemEnter(InventoryGrid grid, Widget w, int row, int col)
	{
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
		if (m_entity.IsNull() || grid == NULL) return;
		InventoryItem itemAtPos = FindItem(grid, row, col);
		
		if (itemAtPos)
		{
			InventoryManager manager = InventoryManager.GetInstance();
  			manager.HideTooltip();
			InventoryContainerEntity container = m_container;
			container.ToggleExpandView( itemAtPos );
		}
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
		
		if (grid != m_attachment_grid) 
		{
			GetGame().GetInventoryItemSize(item, w, h);
		}
		
		return Vector(0, w, h);
	}

	InventoryItem FindItem(InventoryGrid grid, int row, int col)
	{
		if (m_entity.IsNull() || grid == NULL) return NULL;

		if (grid == m_attachment_grid) 
		{
			int index = grid.GetIndex(row, col);
			if (index >= m_entity.Ptr().AttachmentsCount()) return NULL;
			
			return m_entity.Ptr().GetAttachmentFromIndex(index);
		}
		
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
	}
	
	int GetItemColor(ScriptedWidgetEventHandler grid, InventoryItem item)
	{
		InventoryManager manager = InventoryManager.GetInstance();
		InventoryItem dragged_item = manager.GetItem();
		
		if ( dragged_item ) return 0;
		// int color = InventoryGrid.ITEM_COLOR_NORMAL;
		int color = InventoryGrid.ITEM_COLOR_TRANSPARENT;

		// if item icon is main icon (view, viewgrid )
		PlayerBase player = GetGame().GetPlayer();
		// if ( item.GetOwner() == player || item.GetOwner() == NULL )
		if ( item.IsInherited(InventoryItem) && item.GetOwner() && item.GetOwner() != player )
		{
			// color = InventoryGrid.ITEM_COLOR_TRANSPARENT;
			color = InventoryGrid.ITEM_COLOR_NORMAL;
		}

		if( item.IsInherited(InventoryItem) ) 
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

	void ClearHistory()
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
}
