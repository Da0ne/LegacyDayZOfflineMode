
// -----------------------------------------------------------
class InventoryGridController;
class InventoryGrid;

class InventoryGridController: ScriptedWidgetEventHandler
{
	void OnItemEnter(InventoryGrid grid, Widget w, int row, int col) {}
	void OnItemLeave(InventoryGrid grid, Widget w) {}
	void OnItemDrag(InventoryGrid grid, Widget w, int row, int col) {}
	void OnItemDraggingOver(InventoryGrid grid, Widget w, int row, int col) {}
	void OnItemDrop(InventoryGrid grid, Widget w) {}
	void OnItemDropReceived(InventoryGrid grid, Widget w, int row, int col) {}
	void OnItemClick(InventoryGrid grid, Widget w, int row, int col) {}
	void OnItemLeftClick(InventoryGrid grid, Widget w, int row, int col) {}
	void OnItemRightClick(InventoryGrid grid, Widget w, int row, int col) {}
	void OnItemDoubleClick(InventoryGrid grid, Widget w, int row, int col) {}
	// float GetItemQuantity(InventoryGrid grid, InventoryItem item) {}
	int GetItemColor(ScriptedWidgetEventHandler grid, InventoryItem item) { 
		return 0;
	}
	int GetQuickbarItemColor(InventoryGrid grid, InventoryItem item) {}
	vector GetItemSize(InventoryGrid grid, InventoryItem item)
	{
		return Vector(0, 1, 1);
	}
	string GetItemQuantityText( InventoryItem item );
	int HasItemQuantity( InventoryItem item );
	float GetItemQuantity( InventoryItem item );
};

// -----------------------------------------------------------
//! map: item x vector(index, width, height)
typedef map<InventoryItem, vector> TItemsMap

class InventoryGrid: ScriptedWidgetEventHandler
{
	// AARRGGBB
	static int ITEM_COLOR_QUICKBAR_NORMAL = 0x7F858585;
	static int ITEM_COLOR_QUICKBAR_GOOD = 0xFF6e980d;
	static int ITEM_COLOR_NORMAL = 0xFF999999;
	static int ITEM_COLOR_TRANSPARENT = 0x00FFFFFF;
	static int ITEM_COLOR_GRID = 0xFF393939;
	static int ITEM_COLOR_DRAG = 0xFFB7FFFC;
	static int ITEM_COLOR_GOOD = 0xFF6DFFB3;
	static int ITEM_COLOR_SWAP = 0xFF66C1FF;
	static int ITEM_COLOR_MULTIPLE = 0xFFFF884C;
	static int ITEM_COLOR_WRONG = 0xFFFF635B;
	
	protected reference int Border;
	protected reference int Gap;
	protected reference int ItemSizeWidth;
	protected reference int ItemSizeHeight;
	protected reference int ItemsHorizontal;
	protected reference int ItemsVertical;
	reference bool DebugOutput;

	protected float m_screen_x;
	protected	float m_screen_y;

	protected autoptr TItemsMap m_items;
	protected InventoryGridController m_controller;

	protected Widget m_root;
	protected int m_count;
	protected int m_quantity_panel_size;

	void InventoryGrid()
	{
		m_items = new TItemsMap;	
	}

	protected void OnWidgetScriptInit(Widget w)
	{
		m_root = w;
		m_count = 0;
	
		Widget child = m_root.GetChildren();
		while (child)
		{
			m_count++;
			child.SetFlags(WidgetFlags.EXACTPOS | WidgetFlags.EXACTSIZE, false);
			child = child.GetSibling();
		}
	
		m_root.SetHandler(this);
	}

	protected void ShowBackground(int row, int col, int w, int h, bool visible)
	{
		w += col; 
		h += row;
	
		for (int r = row; r < h; r++)
		{
			for (int c = col; c < w; c++)
			{
				if (r == row && c == col) continue;
				Widget item_w = GetItemAt(r, c);
				if (item_w) item_w.Show(visible);
			}	
		}
	}	

	protected void UpdateLayout()
	{
		m_root.GetScreenPos(m_screen_x, m_screen_y);
		m_root.SetSize(Border + (ItemsHorizontal * ItemSizeWidth) + ((ItemsHorizontal - 1) * Gap) + Border,	Border + (ItemsVertical * ItemSizeHeight) + ((ItemsVertical - 1) * Gap) + Border);
		
		if (m_count == 0) return;
	
		Widget child = m_root.GetChildren();
		
		int index = 0;
		while (child)
		{
			int col = index % ItemsHorizontal;
			int row = index / ItemsHorizontal;
			float itemX = Border + (col * (ItemSizeWidth + Gap));
			float itemY = Border + (row * (ItemSizeHeight + Gap));
	
			child.SetPos(itemX, itemY);
			child.SetSize(ItemSizeWidth, ItemSizeHeight);
			
			index++;
			child = child.GetSibling();
		}
	}
	
	// ScriptedWidgetEventHandler override
	bool OnUpdate(Widget w)
	{
		m_root.GetScreenPos(m_screen_x, m_screen_y);
		return false;
	}
	
	// -----------------------------------------------------------
	bool OnChildAdd( Widget  w, Widget  child)
	{
		if (w == m_root)
		{
			m_count++;
			child.SetFlags(WidgetFlags.EXACTPOS | WidgetFlags.EXACTSIZE, false);
		}
		return false;
	}
	
	// -----------------------------------------------------------
	bool OnChildRemove( Widget  w, Widget  child)
	{
		if (w == m_root)
		{
			m_count--;
		}
		return false;
	}
	
	// -----------------------------------------------------------
	bool OnMouseEnter(Widget w, int x, int y)
	{
		int col = GetCol(x);
		int row = GetRow(y);
		if ( !IsValidPos(row, col) ) return false;	
		
		if (DebugOutput)
		{
			PrintString (m_root.GetName() + "::OnMouseEnter(" + row.ToString() + "," + col.ToString() + ")");
		}
		
		if (m_controller) m_controller.OnItemEnter(this, w, row, col);
	
		return true;
	}
	
	// -----------------------------------------------------------
	bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		if (m_controller) m_controller.OnItemLeave(this, w);
	
		return true;
	}
	
	// -----------------------------------------------------------
	bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		if (button == MouseState.RIGHT || button == MouseState.LEFT)
		{
			int col = GetCol(x);
			int row = GetRow(y);
		
			if ( !IsValidPos(row, col) ) return false;
			
			if (DebugOutput)
			{
				PrintString (m_root.GetName() + "::OnMouseButtonDown(" + row.ToString() + "," + col.ToString() + ")");
			}
		
			if (m_controller) 
			{
				if (button == MouseState.RIGHT )
				{
					m_controller.OnItemRightClick(this, w, row, col);
				}
			}
			return true;
		}
		else
		{
			return false;
		}
	}

	// -----------------------------------------------------------
	bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		if (button == MouseState.RIGHT || button == MouseState.LEFT)
		{
			int col = GetCol(x);
			int row = GetRow(y);
		
			if ( !IsValidPos(row, col) ) return false;
			
			if (DebugOutput)
			{
				PrintString (m_root.GetName() + "::OnMouseButtonUp(" + row.ToString() + "," + col.ToString() + ")");
			}
		
			if (m_controller) 
			{
				if (button == MouseState.LEFT )
				{
					m_controller.OnItemLeftClick(this, w, row, col);
				}
			}
			return true;
		}
		else
		{
			return false;
		}
	}
	
	// -----------------------------------------------------------
	bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		if (button != MouseState.LEFT) return false;
		
		int col = GetCol(x);
		int row = GetRow(y);
	
		if ( !IsValidPos(row, col) ) return false;
		
		if (DebugOutput)
		{
			PrintString (m_root.GetName() + "::OnDoubleClick(" + row.ToString() + "," + col.ToString() + ")");
		}
	
		if (m_controller) m_controller.OnItemDoubleClick(this, w, row, col);
	
		return true;
	}
	
	//--------------------------------------------------------------------------
	bool OnDrop(Widget w, int x, int y, Widget reciever)
	{
		if (DebugOutput)
		{
			PrintString (m_root.GetName() + "::OnDrop()");
		}
	
		if (m_controller) m_controller.OnItemDrop(this, w);
		
		if (IsIcon())
		{
			ItemPreviewWidget item_preview = w.FindAnyWidget("Preview");
			if (item_preview)
			{
				item_preview.SetCloseup(IsIcon());
			}
		}

		return true;
	}
	
	//--------------------------------------------------------------------------
	bool OnDrag(Widget w, int x, int y)
	{
		int col = GetCol(x);
		int row = GetRow(y);
		
		if ( !IsValidPos(row, col) ) return false;
		
		if (DebugOutput)
		{
			PrintString (m_root.GetName() + "::OnDrag(" + row.ToString() + "," + col.ToString() + ")");
		}
	
		if (m_controller) m_controller.OnItemDrag(this, w, row, col);
	
		if (IsIcon())
		{
			ItemPreviewWidget item_preview = w.FindAnyWidget("Preview");
			if (item_preview)
			{
				item_preview.SetCloseup(false);
			}
		}
		return true;
	}
	
	//--------------------------------------------------------------------------
	bool OnDraggingOver(Widget w, int x, int y, Widget reciever)
	{
	
		int col = GetCol(x);
		int row = GetRow(y);
		
		if ( !IsValidPos(row, col) ) return false;
		
		if (DebugOutput)
		{
			PrintString (m_root.GetName() + "::OnDraggingOver(" + row.ToString() + "," + col.ToString() + ")");
		}
	
		if (m_controller) m_controller.OnItemDraggingOver(this, w, row, col);
	
		
		return true;
	}
	
	//--------------------------------------------------------------------------
	bool OnDropReceived(Widget w, int x, int y, Widget reciever)
	{
	
		int col = GetCol(x);
		int row = GetRow(y);
	
		if ( !IsValidPos(row, col) ) return false;
		
		if (DebugOutput)
		{
			PrintString (m_root.GetName() + "::OnDropReceived(" + row.ToString() + "," + col.ToString() + ")");
		}
	
		if (m_controller) m_controller.OnItemDropReceived(this, w, row, col);
	
		
		return true;
	}
	
	// Interface
	
	//! Gets column from screen position
	int GetCol(float x)
	{
		return (x - m_screen_x - Border) / (ItemSizeWidth + Gap);
	}
	
	//! Gets row from screen position
	int GetRow(float y)
	{
		return (y - m_screen_y - Border) / (ItemSizeHeight + Gap);
	}
	
	int GetIndex(int row, int col)
	{
		return (row * ItemsHorizontal) + col;
	}
	
	TItemsMap GetItems() {
		return m_items;	
	}
	
	bool IsIcon() 
	{
		return false;
	}
	
	Widget GetItemAt(int row, int col)
	{
		int index = GetIndex(row, col);
	
		return GetItem(index);
	}
	
	//--------------------------------------------------------------------------
	bool IsValidPos(int row, int col)
	{
		if (row >= ItemsVertical) return false;
		if (col >= ItemsHorizontal) return false;
		if (GetIndex(row, col) >= m_count) return false;
		return true;
	}
	
	//--------------------------------------------------------------------------
	Widget GetItem(int index)
	{
		Widget child = m_root.GetChildren();
		while (child && index)
		{
			index--;
			child = child.GetSibling();
		}
	
		return child;
	}
	
	//--------------------------------------------------------------------------
	Widget GetItemBackground(int index)
	{
		Widget w = GetItem(index);
		
		if (!w) return NULL;
		
		return w.FindAnyWidget("GridItem");
	}
	
	//--------------------------------------------------------------------------
	Widget GetItemBackgroundAt(int row, int col)
	{
		return GetItemBackground(GetIndex(row, col));
	}

	Widget GetRoot()
	{
		return m_root;
	}
	
	void SetItemColor(InventoryItem item, int color)
	{
		if (m_items.Contains(item))
		{
			vector data = m_items.Get(item);
			int index = Math.Round(data[0]);
			
			Widget w = GetItemBackground(index);
			if (w) w.SetColor(color);
		}
	}
	//! Set size in pixels
	void SetItemSize(InventoryItem item, int pixels_w, int pixels_h)
	{
		if (m_items.Contains(item))
		{
			vector data = m_items.Get(item);
			int index = Math.Round(data[0]);
			
			Widget w = GetItemBackground(index);
			if (w) w.SetSize(pixels_w, pixels_h);
		}
	}
	
	void SetController(InventoryGridController controller) { 
		m_controller = controller; 
	}

	InventoryGridController GetController() { 
		return m_controller; 
	}
	
	//--------------------------------------------------------------------------
	void GenerateBackgroundTiles(int count)
	{
		for (int i = 0; i < count; i++)
		{
			Widget root_widget = GetGame().GetWorkspace().CreateWidgets("gui/layouts/inventory/inventoryGridBackground.layout", m_root);
			TextWidget label_widget = (TextWidget)root_widget.FindAnyWidget("Label");
		}
		UpdateLayout();
	}
	
	//--------------------------------------------------------------------------
	void GenerateQuickBarBackgroundTiles(int count)
	{
		for (int i = 0; i < count; i++)
		{
			Widget root_widget = GetGame().GetWorkspace().CreateWidgets("gui/layouts/inventory/inventoryGridBackground.layout", m_root);
			TextWidget label_widget = (TextWidget)root_widget.FindAnyWidget("Label");
			label_widget.SetText( ( i+1).ToString() );
			label_widget.Show( true );
		}
		UpdateLayout();
	}

	
	//--------------------------------------------------------------------------
	void UpdateQuickbarItems(TItemsMap items )
	{
		int i;
		int c;
		int index;
		int width;
		int height;
		int row;
		int col;
		InventoryItem item;
		vector data;
		Widget bck;
		Widget item_w;
		
		// remove not actual items
		c = m_items.Count();
		for (i = 0; i < c; i++)
		{
			item = m_items.GetKey(i);
			
			bool remove_item = false;
			
			if (items.Contains(item) == false)
			{
				remove_item = true;
			}
			else
			{
				// check items position actual
				if ((m_items.Get(item) - items.Get(item)).LengthSq() > 0.01)
				{
					// item has different position or size
					remove_item = true;
				}
			}
			
			if (remove_item)
			{
				RemoveItem(item);
				c--;
				i--;			
			}
		}
		
		// add new items
		for (i = 0; i < items.Count(); i++)
		{
			item = items.GetKey(i);
			data = items.Get(item);
					
			if (m_items.Contains(item) == false)
			{
				AddItem(item, data, Vector(0,0,0) );
			}
		}
	
		// add new items
		for (i = 0; i < items.Count(); i++)
		{
			item = items.GetKey(i);
			data = items.Get(item);
			RefreshQuickbarItemVariables( item, data );
		}
	}
	
	
	//--------------------------------------------------------------------------
	void UpdateItems(TItemsMap items, bool show_quantity, bool show_temperature )
	{
		int i;
		int c;
		int index;
		int width;
		int height;
		int row;
		int col;
		InventoryItem item;
		vector data;
		Widget bck;
		Widget item_w;
		
			// remove not actual items
			c = m_items.Count();
			for (i = 0; i < c; i++)
			{
				item = m_items.GetKey(i);
				if(item != NULL)
				{
					bool remove_item = false;
					
					if (items.Contains(item) == false)
					{
						remove_item = true;
					}
					else
					{
						// check items position actual
						if ((m_items.Get(item) - items.Get(item)).LengthSq() > 0.01)
						{
							// item has different position or size
							remove_item = true;
						}
					}
					
					if (remove_item)
					{
						RemoveItem(item);
						c--;
						i--;			
					}
				}
			}
		
		// add new items
		for (i = 0; i < items.Count(); i++)
		{
			item = items.GetKey(i);
			data = items.Get(item);
					
			if (m_items.Contains(item) == false)
			{
				AddItem(item, data, Vector(0,0,0) );
				// color
				index = Math.Round(data[0]);
				bck = GetItem(index);
				item_w = bck.FindAnyWidget("GridItem");
				item_w.SetColor( m_controller.GetItemColor( this, item) );
			}
		}
	
		// add new items
		for (i = 0; i < items.Count(); i++)
		{
			item = items.GetKey(i);
			data = items.Get(item);
			// refresh quantity
			RefreshItemVariables( item, data, show_quantity, show_temperature );
		}
	}
	
	//--------------------------------------------------------------------------
	void UpdateQuantityItems()
	{
		int i;
		int index;
		InventoryItem item;
		vector data;
		for (i = 0; i < m_items.Count(); i++)
		{
			item = m_items.GetKey(i);
			data = m_items.Get(item); 
			index = Math.Round(data[0]);
			RefreshItemVariables( item, data, true, false );
		}
	}
	
	//--------------------------------------------------------------------------
	void UpdateTemperatureItems()
	{
		int i;
		int index;
		InventoryItem item;
		vector data;
		for (i = 0; i < m_items.Count(); i++)
		{
			item = m_items.GetKey(i);
			data = m_items.Get(item); 
			index = Math.Round(data[0]);
			RefreshItemVariables( item, data, false, true );
		}
	}
	
	//--------------------------------------------------------------------------
	void RefreshQuickbarItemVariables(InventoryItem item, vector data)
	{
		int	index = Math.Round(data[0]);
		Widget bck = GetItem(index);
		Widget item_w;
		if ( bck )
		{
			item_w = bck.FindAnyWidget("GridItem");
			if ( item_w )
			{
				int color = m_controller.GetQuickbarItemColor( this, item ); // !!!!!
				item_w.SetColor( color );
			}
		}
		RefreshItemVariables( item, data, true, false );
	}
	
	//--------------------------------------------------------------------------
	void RefreshItemVariables(InventoryItem item, vector data, bool show_quantity, bool show_temperature )
	{
		int	index = Math.Round(data[0]);
		Widget bck = GetItem(index);
		Widget item_w;
	
		if ( bck )
		{
			item_w = bck.FindAnyWidget("GridItem");
			if ( item_w )
			{
				// LogInfo( item.GetName(), LogTemplates.TEMPLATE_JANOSIK );
	
				int has_quantity = m_controller.HasItemQuantity( item );
				Widget quantity_panel = bck.FindAnyWidget("QuantityPanel");
				TextWidget item_quantity = bck.FindAnyWidget("Quantity");
				ProgressBarWidget quantity_progress = bck.FindAnyWidget("QuantityBar");
				Widget quantity_stack = bck.FindAnyWidget("QuantityStackPanel");
				if ( has_quantity == QUANTITY_HIDDEN )
				{
					quantity_panel.Show( false );
				}
				else
				{
					quantity_panel.Show( true );
					if ( has_quantity == QUANTITY_COUNT )
					{
						item_quantity.SetText( m_controller.GetItemQuantityText( item ) );
						quantity_stack.Show( true );
						quantity_progress.Show( false );
					}
					else if ( has_quantity == QUANTITY_PROGRESS )
					{
						// LogInfo( " QUANTITY_PROGRESS", LogTemplates.TEMPLATE_JANOSIK );
	
						float progress_max = quantity_progress.GetMax();
						int max = item.ConfigGetInt("stackedMax");
						int count = item.ConfigGetInt("count");
						float quantity = m_controller.GetItemQuantity( item );
						// LogInfo( " " + ftoa( max ), LogTemplates.TEMPLATE_JANOSIK );
						// LogInfo( " " + ftoa( quantity ), LogTemplates.TEMPLATE_JANOSIK );
						// quantity = item.GetVariableFloat("quantity");
						// LogInfo( " " + ftoa( quantity ), LogTemplates.TEMPLATE_JANOSIK );	
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
						quantity_stack.Show( false );
						quantity_progress.Show( true );
					}
				}
				
				if ( show_temperature )
				{
					if ( item && item.IsInherited(  InventoryItem) )
					{
						int color = m_controller.GetItemColor( this, item); // !!!!!
						if ( color )
						{
							item_w.SetColor( color );
						}
					}
				}
			}
		}
	}
	
	//--------------------------------------------------------------------------
	void SetQuantityPanelSize( int size )
	{
		m_quantity_panel_size = size;
	}
	
	//--------------------------------------------------------------------------
	void AddItem(InventoryItem item, vector data, vector rotation)
	{
		m_items.Set(item, data);			
				
		int index = Math.Round(data[0]);
		int width = Math.Round(data[1]);
		int height = Math.Round(data[2]);
				
		int col = index % ItemsHorizontal;
		int row = index / ItemsHorizontal;
		ShowBackground(row, col, width, height, false);
				
		Widget bck = GetItem(index);
		Widget item_w_bck = GetGame().GetWorkspace().CreateWidgets("gui/layouts/inventory/inventoryGridItem.layout", bck);
		item_w_bck.SetFlags(WidgetFlags.VISIBLE | WidgetFlags.EXACTPOS | WidgetFlags.EXACTSIZE);			
		item_w_bck.SetPos(0,0);
		item_w_bck.SetSize((ItemSizeWidth * width) + (Gap * (width - 1)), (ItemSizeHeight * height) + (Gap * (height - 1)));
			
		Widget item_w = item_w_bck.FindAnyWidget("GridItem");
		ResetItemWidget(item_w, width, height);
				
		ItemPreviewWidget item_preview = item_w.FindAnyWidget("Preview");
		item_preview.SetItem(item);
		item_preview.SetModelOrientation( rotation );
		item_preview.SetCloseup(IsIcon());

		Widget quantity_stack_panel = item_w.FindAnyWidget("QuantityStackPanel");
		quantity_stack_panel.SetSize( m_quantity_panel_size,m_quantity_panel_size );
		
		RefreshItemVariables( item, data, true, true );
	}
	
	//--------------------------------------------------------------------------
	protected void ResetItemWidget(Widget item_w, int width, int height)
	{
		if (item_w == NULL) return;
		item_w.SetSize((ItemSizeWidth * width) + (Gap * (width - 1)), (ItemSizeHeight * height) + (Gap * (height - 1)));					
		item_w.SetFlags(WidgetFlags.VISIBLE | WidgetFlags.EXACTPOS | WidgetFlags.EXACTSIZE | WidgetFlags.DRAGGABLE);
		item_w.SetColor(ITEM_COLOR_NORMAL);
	}
	
	//--------------------------------------------------------------------------
	void ResetItem(InventoryItem item)
	{
		if (m_items.Contains(item))
		{
			vector data = m_items.Get(item);
			int index = Math.Round(data[0]);
			int width = Math.Round(data[1]);
			int height = Math.Round(data[2]);
			Widget bck = GetItem(index);
			Widget item_w = bck.FindAnyWidget("GridItem");
			ResetItemWidget(item_w, width, height);
			RefreshItemVariables( item, data, true, true );
		}
	}
	
	//--------------------------------------------------------------------------
	bool HasItem(InventoryItem item)
	{
		return m_items.Contains(item);
	}
	
	//--------------------------------------------------------------------------
	void RemoveItem(InventoryItem item)
	{
		if (m_items.Contains(item))
		{
			vector data = m_items.Get(item);
			int index = Math.Round(data[0]);
			int width = Math.Round(data[1]);
			int height = Math.Round(data[2]);
			Widget bck = GetItem(index);
			Widget itemW = bck.FindAnyWidget("GridItemBck");
			if (itemW) 
			{
				itemW.Destroy();
			}
		
			m_items.Remove(item);			
			int col = index % ItemsHorizontal;
			int row = index / ItemsHorizontal;
		
			ShowBackground(row, col, width, height, true);
		}
	}
	
	// grid api
	void SetGridSize(int items_horizontal, int items_vertical)
	{
		ItemsHorizontal = items_horizontal;
		ItemsVertical = items_vertical;
	}
	
	void SetGridItemsSize( int item_size_width, int item_size_height )
	{
		ItemSizeWidth = item_size_width;
		ItemSizeHeight = item_size_height;
	}
	
	//--------------------------------------------------------------------------
	int GetGridItemsHeight()
	{
		return ItemSizeHeight;
	}
	
	//--------------------------------------------------------------------------
	void SetGridGapSize(int gap_size)
	{
		Gap = gap_size;
	}
};

// -----------------------------------------------------------
class InventoryGridIcon: InventoryGrid
{
	// InventoryGrid override
	protected void ResetItemWidget(Widget item_w, int width, int height)
	{
		if (item_w == NULL) return;
		item_w.SetSize((ItemSizeWidth * width) + (Gap * (width - 1)), (ItemSizeHeight * height) + (Gap * (height - 1)));					
		item_w.SetFlags(WidgetFlags.VISIBLE | WidgetFlags.EXACTPOS | WidgetFlags.EXACTSIZE | WidgetFlags.DRAGGABLE);
		item_w.SetColor(ITEM_COLOR_TRANSPARENT);
	}
	
	bool IsIcon() 
	{
		return true;
	}
	
};
