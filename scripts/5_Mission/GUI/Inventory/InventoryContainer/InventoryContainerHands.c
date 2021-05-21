class InventoryContainerHands: InventoryContainer
{
	protected InventoryViewBase m_view;
	
	void InventoryContainerHands(Widget parent, bool ownedByPlayer, bool isContentGrid = true )
	{
	}

	void ~InventoryContainerHands()
	{
		delete m_view;
	}
	
	void Init()
	{
	}
	
	void UpdateItems()
	{
		PlayerBase player = GetGame().GetPlayer();
		if ( player == NULL )
		{
			return;
		}
		
		EntityAI item_in_hands = player.GetEntityInHands();
		
		if ( item_in_hands == NULL )
		{
			if ( m_view ) delete m_view;
			m_view = new InventoryViewHands(this, item_in_hands, InventoryViewProperties.ATTACHMENTS | InventoryViewProperties.CARGOS);	
			m_view.CreateLayout();
		}

		if ( item_in_hands && m_view && m_view.GetEntity() != item_in_hands)	
		{
			delete m_view;
		}
		
		if ( item_in_hands && m_view == NULL )
		{
			m_view = new InventoryViewHands(this, item_in_hands, InventoryViewProperties.ATTACHMENTS | InventoryViewProperties.CARGOS);	
			m_view.CreateLayout();
		}
		
		if (m_view) m_view.UpdateItems();
		if (m_spacer) m_spacer.Update();
	}

	// InventoryContainer override
	Widget CreateLayout(Widget parent)
	{
		return GetGame().GetWorkspace().CreateWidgets("gui/layouts/inventory/inventoryContainerHands.layout", parent);
	}
	
	void OnItemDoubleClick(InventoryViewBase view, InventoryItem item)
	{
		PlayerBase player = GetGame().GetPlayer();
		if ( player == NULL )
		{
			return;
		}
		
		InventoryManager manager = InventoryManager.GetInstance();
		EntityAI item_in_hands = player.GetEntityInHands();
		
		if (item_in_hands == item)
		{
			manager.TakeToHands(NULL);
		}
		else
		{
			super.OnItemDoubleClick(view, item);
		}
	}

	// ScriptedWidgetEventHandler override	
	bool OnDraggingOver(Widget w, int x, int y, Widget reciever)
	{
		InventoryManager manager = InventoryManager.GetInstance();
		InventoryItem dragged_item = manager.GetItem();
		
		if (manager.IsDragging() == false || dragged_item == NULL) return false;
		
		if (dragged_item && dragged_item.CanRemoveEntity() == false)
		{
			manager.SetItemColor(InventoryGrid.ITEM_COLOR_WRONG);
		}
		else
		{
			manager.SetItemColor(InventoryGrid.ITEM_COLOR_GOOD);
		}
		return true;
	}

	bool OnDropReceived(Widget w, int x, int y, Widget reciever)
	{
		PlayerBase player = GetGame().GetPlayer();
		InventoryManager manager = InventoryManager.GetInstance();
		InventoryItem dragged_item = manager.GetItem();
		
		if (manager.IsDragging() == false || dragged_item == NULL) return false;
		
		if ( player.CanAddEntityInHands(dragged_item) && dragged_item.CanRemoveEntity() )
		{
			manager.TakeToHands(dragged_item);
			manager.Drop();	
		}
		
		return true;
	}
}
