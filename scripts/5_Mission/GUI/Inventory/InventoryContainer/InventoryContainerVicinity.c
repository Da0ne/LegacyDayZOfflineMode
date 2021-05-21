class InventoryContainerVicinity: InventoryContainer
{
	protected autoptr map<EntityAI, InventoryViewBase> m_views;

	void InventoryContainerVicinity(Widget parent, bool ownedByPlayer, bool isContentGrid = true )
	{
		m_views = new map<EntityAI, InventoryViewBase>;
	}

	void ~InventoryContainerVicinity()
	{
		int i, c;	
		c = m_views.Count();
		for (i = 0; i < c; i++) 
		{
			delete m_views.GetElement(i);
		}
	}
	
	void Init()
	{	
	}
	
	void UpdateItems()
	{
		Man player = GetGame().GetPlayer();
		vector pos = player.GetPosition();
		vector dir = player.GetDirection();
		int i, j, c, x;
		EntityAI entity;
		Cargo cargo;
		Object obj;
		EntityAI e;
		autoptr array<Object> objects = new array<Object>;
		autoptr array<Cargo> proxyCargos = new array<Cargo>;
		GetGame().GetObjectsAtPosition(pos + (dir * 0.75), 1.0, NULL, objects, proxyCargos);
		InventoryManager manager = InventoryManager.GetInstance();

		c = proxyCargos.Count();

		for (i = 0; i < c; i++)
		{
			cargo = proxyCargos.Get(i);
			objects.Insert(cargo.GetParent());
		}
		
		// remove unnecesary entities
		c = m_views.Count();
		for (i = 0; i < c; i++)
		{
			entity = m_views.GetKey(i);
			if (objects.Find(entity) == INDEX_NOT_FOUND)
			{
				delete m_views.GetElement(i);
				m_views.RemoveElement(i);
				c--;
				i--;
			}
		}

		// add new entities
		for (i = 0; i < objects.Count(); i++)
		{
			obj = objects.Get(i);

			if ( CanShowItemInInventory(player, obj) && !obj.IsInherited(PlayerBase) && !obj.IsInherited(ZombieBase) )
			{
				e = obj;
				if ( m_views.Contains(e) == false )
				{	
					bool addEntity = true;
					if ( e.IsInherited(Building) )
					{
						if ( e.GetCargo() == NULL || e.GetSlotsCountCorrect() < 1 )
						{
							addEntity = false;
						}
					}

					if ( addEntity )
					{	
						
						// Print("--------");
						// Print( e.GetName() );
						// Print( ToString( e.AttachmentsCount() ) );
						// Print( ToString( e.GetSlotsCount() ) );
						// Print( ToString( e.GetCargo() ) );
						// Print( ToString( m_entity.Ptr().GetCargo() ) );
						// if ( m_entity.Ptr().GetCargo() != NULL || m_entity.Ptr().AttachmentsCount() > 0 )
						// m_entity.Ptr().AttachmentsCount() == 0
						// if ( m_entity.Ptr().GetCargo() == NULL &&  )
						// if ( m_entity.Ptr().AttachmentsCount() == 0 && m_entity.Ptr().GetCargo() == NULL)

						InventoryViewBase view;
						if ( e.GetCargo() == NULL && e.GetSlotsCount() < 1 )
						{
							// LogInfo(ToString( "InventoryViewGridInventoryViewGridInventoryViewGridInventoryViewGridInventoryViewGrid"), LogTemplates.TEMPLATE_JANOSIK );
							view = new InventoryViewGrid(this, e, InventoryViewProperties.ATTACHMENTS | InventoryViewProperties.CARGOS);
						}
						else if ( GetGame().IsInventoryTreeLocked(obj, player) && e.IsInherited(TentBase) )
						{
							// LogInfo(ToString( "InventoryViewGridInventoryViewGridInventoryViewGridInventoryViewGridInventoryViewGrid"), LogTemplates.TEMPLATE_JANOSIK );
							view = new InventoryViewGrid(this, e, InventoryViewProperties.ATTACHMENTS | InventoryViewProperties.CARGOS);
						}
						else
						{
							view = new InventoryView(this, e, InventoryViewProperties.ATTACHMENTS | InventoryViewProperties.CARGOS);
						}
					
						view.CreateLayout();
						c = proxyCargos.Count();
						for (j = 0; j < c; j++)
						{
							cargo = proxyCargos.Get(j);
							if (cargo.GetParent() == e)
							{
								view.AddProxyCargo(cargo);
							}
						}
						
						m_views.Set(e, view );
					}
				}
			}
		}

		if ( m_empty_slot == NULL )
		{
			m_empty_slot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/inventory/inventoryGridItemEmpty.layout",  m_contentGrid );
			m_empty_slot.SetSize( manager.GetAttachmentItemSize(), manager.GetAttachmentItemSize() );
			m_empty_slot.SetHandler(this);
		}

		// remove not accessible items
		c = m_views.Count();
		for (i = 0; i < c; i++) 
		{
			e = m_views.GetKey(i);
			if ( !CanShowItemInInventory(player, e) )
			{
				delete m_views.GetElement(i);
				m_views.RemoveElement(i);
				c--;
				i--;
			}
		}
		
		// update all
		c = m_views.Count();
		for (i = 0; i < c; i++) 
		{
			m_views.GetElement(i).UpdateItems();
		}

		if (m_spacer) m_spacer.Update();
	}

	// ScriptedWidgetEventHandler override	
	bool OnDraggingOver(Widget w, int x, int y, Widget reciever)
	{
		InventoryManager manager = InventoryManager.GetInstance();
		InventoryItem dragged_item = manager.GetItem();
		
		if (manager.IsDragging() == false || dragged_item == NULL) return false;
		
		PlayerBase player = GetGame().GetPlayer();
		if (dragged_item && !(player.GetEntityInHands() != dragged_item || player.CanRemoveEntityInHands()))
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
		
		if (manager.IsDragging() == false || dragged_item == NULL ) return false;
		
		// InventoryMenu menu = GetGame().GetUIManager().FindMenu(MENU_INVENTORY);
		// if (menu)
		// {
		// 	LogInfo(ToString( "DROPITEM3"), LogTemplates.TEMPLATE_JANOSIK );
		// 	menu.UpdatePlayerPreviewAnim( player.GetEntityInHands() );
		// }	

		manager.DropItem(dragged_item);
		manager.Drop();
		return true;
	}
}
