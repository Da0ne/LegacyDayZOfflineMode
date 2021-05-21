class InventoryQuickbar: InventoryGridController
{
	protected autoptr TItemsMap m_items;//tmp
	protected InventoryGrid m_grid;
	protected int m_dragging_index;
	protected int m_quickbar_size;

	private const int QUICKBAR_GRID_WIDTH = 50;
	
	void InventoryQuickbar(Widget quickbarGridWidget)
	{
		m_dragging_index = INDEX_NOT_FOUND;
		m_items = new TItemsMap;
		UpdateItems( quickbarGridWidget );
	}

	void ~InventoryQuickbar()
	{
	}
	
	void UpdateItems( Widget quickbarGridWidget )
	{
		int i;
		Man player = GetGame().GetPlayer();
		
		if (player == NULL) return;
		m_items.Clear();

		// create grid and align it to center
		if (quickbarGridWidget) quickbarGridWidget.GetScript(m_grid);
		if (m_grid)
		{
			if ( m_quickbar_size != player.GetQuickBarSize() )
			{
				// clear/remove
				m_items.Clear();
				m_grid.UpdateQuickbarItems(m_items);
				while(m_grid.GetRoot().GetChildren()) { m_grid.GetRoot().GetChildren().Destroy(); }

				m_quickbar_size = player.GetQuickBarSize();
				m_grid.SetGridItemsSize( QUICKBAR_GRID_WIDTH, QUICKBAR_GRID_WIDTH );
				m_grid.SetGridSize(m_quickbar_size,1);
				m_grid.SetController(this);		
				m_grid.GenerateQuickBarBackgroundTiles(m_quickbar_size);
				float size = QUICKBAR_GRID_WIDTH / 2.5;
				m_grid.SetQuantityPanelSize( size );

				// align quickbar to center 
				float width, height, x, y;
				int screen_w, screen_h;
				quickbarGridWidget.GetSize( width, height );
				quickbarGridWidget.GetPos(x, y);
				GetScreenSize(screen_w, screen_h);
				float new_x =  0.5 - ( ( width / screen_w ) * 0.5 );

				quickbarGridWidget.SetPos( new_x ,0 );
				quickbarGridWidget.SetColor( 0x7FFFFFFF );
			}
		}

		m_quickbar_size = player.GetQuickBarSize();
		for ( i = 0; i < m_quickbar_size; i++)
		{
			InventoryItem item = player.GetEntityInQuickBar(i);
			if (item) 
			{
				m_items.Set(item, Vector(i, 1, 1));
			}
		}
		
		if (m_grid) m_grid.UpdateQuickbarItems(m_items);
	}

	void Remove(InventoryItem itemToRemove)
	{
		Man player = GetGame().GetPlayer();

		for (int i = 0; i < m_quickbar_size; i++)
		{
			InventoryItem item = player.GetEntityInQuickBar(i);
			if (item == itemToRemove) 
			{
				player.SetEntityShortcut(NULL, i);
			}
		}
	}
	
	// InventoryGridController override
	void OnItemEnter(InventoryGrid grid, Widget w, int row, int col)
	{
		Widget quickbar = grid.GetRoot();
		if ( quickbar )
		{
			// quickbar.SetColor( 0xFFFFFFFF );
		}
	}

	void OnItemLeave(InventoryGrid grid, Widget w)
	{
		Widget quickbar = grid.GetRoot();
		if ( quickbar )
		{
			// quickbar.SetColor( 0x7FFFFFFF );
		}
	}

	void OnItemDraggingOver(InventoryGrid grid, Widget w, int row, int col)
	{
		Man player = GetGame().GetPlayer();

		InventoryMenu menu = GetGame().GetUIManager().GetMenu();
		if ( menu )
		{
			// menu.HidePanelBorders();
		}

		InventoryManager manager = InventoryManager.GetInstance();
		if ( manager )
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
	}

	void OnItemDropReceived(InventoryGrid grid, Widget w, int row, int col)
	{
		Man player = GetGame().GetPlayer();
		InventoryManager manager = InventoryManager.GetInstance();
		if ( manager )
		{
			InventoryItem dragged_item = manager.GetItem();
			InventoryItem quickbar_item = player.GetEntityInQuickBar(col);
			
			if (manager.IsDragging() == false && m_dragging_index == INDEX_NOT_FOUND)
			{
				return;
			}
			
			if (m_dragging_index != INDEX_NOT_FOUND)
			{
				dragged_item = player.GetEntityInQuickBar(m_dragging_index);
			}
				
			if (dragged_item && dragged_item.CanRemoveEntity())
			{
				player.SetEntityShortcut(dragged_item, col);
				
				if (quickbar_item && quickbar_item != dragged_item && m_dragging_index != INDEX_NOT_FOUND)
				{
					player.SetEntityShortcut(quickbar_item, m_dragging_index);
				}
			}
			m_dragging_index = INDEX_NOT_FOUND;
			manager.Drop();
		}
	}
	
	void OnItemDrop(InventoryGrid grid, Widget w)
	{
		if (m_dragging_index != INDEX_NOT_FOUND)
		{
			Man player = GetGame().GetPlayer();
			InventoryItem	dragged_item = player.GetEntityInQuickBar(m_dragging_index);
			Remove(dragged_item);
			m_dragging_index = INDEX_NOT_FOUND;
		}
	}

	void OnItemDrag(InventoryGrid grid, Widget w, int row, int col)
	{
		Man player = GetGame().GetPlayer();
		
		InventoryItem item = player.GetEntityInQuickBar(col);
		if (item)
		{	
			m_dragging_index = col;
			m_grid.SetItemColor(item, InventoryGrid.ITEM_COLOR_DRAG);
		}
	}
	
	bool OnMouseEnter(Widget w, int x, int y)
	{
	}
	
	bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
	}
	
	int GetQuickbarWidth()
	{
		return QUICKBAR_GRID_WIDTH;
	}
	
	int GetQuickbarItemColor(InventoryGrid grid, InventoryItem item)
	{
		int color = InventoryGrid.ITEM_COLOR_QUICKBAR_NORMAL;
		PlayerBase player = GetGame().GetPlayer();
		if( player && player.GetEntityInHands() == item )
		{
			color = InventoryGrid.ITEM_COLOR_QUICKBAR_GOOD;
		}
		return color;
	}

	
	int HasItemQuantity( InventoryItem item )
	{
		InventoryManager manager = InventoryManager.GetInstance();
		if ( manager )  
		{
			if ( item )
			{
				if ( item.IsInherited( InventoryItem) )
				{
					int has_quantity = manager.HasItemQuantity( item );
					return has_quantity;
				}
			}
		}
		return false;
	}

	string GetItemQuantityText( InventoryItem item )
	{
		InventoryManager manager = InventoryManager.GetInstance();
		string quantity_text = manager.GetItemQuantityText( item );
		return quantity_text;
	}

	float GetItemQuantity( InventoryItem item )
	{
		InventoryManager manager = InventoryManager.GetInstance();
		float quantity = manager.GetItemQuantity( item );
		return quantity;
	}

	/*void OnItemEnter(InventoryGrid grid, Widget w, int row, int col);
	void OnItemLeave(InventoryGrid grid, Widget w);
	void OnItemDrag(InventoryGrid grid, Widget w, int row, int col);
	void OnItemDraggingOver(InventoryGrid grid, Widget w, int row, int col);
	void OnItemDrop(InventoryGrid grid, Widget w);
	void OnItemDropReceived(InventoryGrid grid, Widget w, int row, int col);
	void OnItemRightClick(InventoryGrid grid, Widget w, int row, int col);
	void OnItemDoubleClick(InventoryGrid grid, Widget w, int row, int col);*/
}
