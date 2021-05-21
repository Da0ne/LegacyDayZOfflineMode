class InventoryContainerEntity: InventoryContainer
{
	protected autoptr array<EntityAI> m_entities;
	protected autoptr InventoryViewBase m_cargo_view;
	protected autoptr map<EntityAI, InventoryViewBase> m_view_grids;  // inventory grids
	protected autoptr map<EntityAI, InventoryViewBase> m_views;			// cargo views
	protected autoptr Link<EntityAI> m_entity;
	protected autoptr map<int, Widget> m_inventory_slots;

	void InventoryContainerEntity(Widget parent, bool ownedByPlayer, bool isContentGrid = true )
	{
		m_views = new map<EntityAI, InventoryViewBase>;
		m_view_grids = new map<EntityAI, InventoryViewBase>;
		m_entity = new Link<EntityAI>(NULL);
		m_entities = new array<EntityAI>;
		m_inventory_slots = new map<int, Widget>;
	}

	void ~InventoryContainerEntity()
	{
		int i, c;	
		c = m_views.Count();
		for (i = 0; i < c; i++) 
		{
			delete m_views.GetElement(i);
		}
		if ( m_cargo_view )
		{
			delete m_cargo_view;
		}
	}
	
	void Init(EntityAI entity)
	{
		delete m_entity;
		m_entity = new Link<EntityAI>(entity);
		if (entity.GetCargo())
		{
			m_cargo_view = new InventoryView(this, entity, InventoryViewProperties.CARGOS);
			m_cargo_view.CreateLayout();
		}

		if ( GetDayZGame().IsNewInventory() )
		{
			// get entities inventory slots
			string config_path_ghosts_icons = "CfgVehicles SurvivorBase" + " InventoryEquipment playerItemsIcons";
			string config_path_ghosts_slots = "CfgVehicles SurvivorBase" + " InventoryEquipment playerItemsSlots";
			autoptr array<string> player_ghosts_icons	= new array<string>;
			autoptr array<int> player_ghosts_slots	= new array<int>;
			GetGame().ConfigGetTextArray ( config_path_ghosts_icons, player_ghosts_icons );
			GetGame().ConfigGetIntArray ( config_path_ghosts_slots, player_ghosts_slots );

			// add all ghost icons into content grid
			InventoryManager manager = InventoryManager.GetInstance();
			for ( int i = 0; i < player_ghosts_icons.Count(); i++ )
			{
				int slot_id = player_ghosts_slots.Get ( i );
				Widget ghost_widget = GetGame().GetWorkspace().CreateWidgets("gui/layouts/inventory/inventoryGhost.layout", GetContentGridWidget() );
				ghost_widget.SetSize( manager.GetAttachmentItemSize(), manager.GetAttachmentItemSize() );
				string icon_name = player_ghosts_icons.Get ( i );
				ImageWidget ghost_icon = ghost_widget.FindAnyWidget("GhostIcon");
				if ( ghost_icon )
				{
					ghost_icon.LoadImageFile(0,"set:dayz_inventory image:" + icon_name );
				}
				m_inventory_slots.Set( slot_id, ghost_widget );
			}
		}
	}

	void ToggleExpandView( EntityAI entity )
	{
		InventoryViewBase view = m_views.Get( entity );
		if ( view ) 
		{
			Widget root_widget = view.GetRootWidget();
			if ( root_widget )
			{
				if ( root_widget.IsVisible() )
				{
					root_widget.Show( false );
				}
				else
				{
					root_widget.Show( true );
				}
				UpdateItems();
			}
		}
	}
	

	void UpdateItemsNewInventory()
	{
		/*for ( i = 0; i < m_inventory_slots.Count(); i++ )
		{
			m_inventory_slots.GetElement(i).Show( true );
		}*/
		EntityAI entity;
		Cargo cargo;
		int i, j, c, x;
		bool change = false;
		bool added_item = false;
		InventoryViewBase view ;

		InventoryManager manager = InventoryManager.GetInstance();
		
		// player's inventory
		c = m_entity.Ptr().AttachmentsCount();

		for (i = 0; i < c; i++)
		{
			entity = m_entity.Ptr().GetAttachmentFromIndex(i);
			if (entity && entity.IsInventoryItem() )
			{
				if ( m_view_grids.Contains(entity) == false )
				{	
					int attachment_slot_id = entity.GetSlotId();
					bool is_valid_slot = m_inventory_slots.Contains( attachment_slot_id );
					if ( is_valid_slot )
					{
						view = new InventoryViewGrid(this, entity, 0 );
						added_item = true;
						// GetContentGridWidget().RemoveChild( m_inventory_slots.Get(attachment_slot_id) );
						m_inventory_slots.Get(attachment_slot_id).Show( false );
						view.CreateLayout();
						m_view_grids.Set(entity, view );

						//ImageWidget expand_icon = view.m_content.FindAnyWidget("ExpandIcon");
						//if ( expand_icon )
						//{
						//	expand_icon.Show( true );
						//}

						if ( entity.GetCargo() )
						{
							Widget container_icon = view.GetContainerIcon();
							ImageWidget expand_icon = container_icon.FindAnyWidget("ExpandIcon");
							if ( expand_icon )
							{
								expand_icon.Show( true );
							}

							view = new InventoryView(this, entity, InventoryViewProperties.ATTACHMENTS | InventoryViewProperties.CARGOS);
							view.CreateLayout();
							m_views.Set(entity, view );
							view.GetRootWidget().Show( false );
						}

					}
				}
				m_entities.Insert(entity);
			}
		}
		

		// remove unnecesary entities
		c = m_view_grids.Count();
		for (i = 0; i < c; i++)
		{
			entity = m_view_grids.GetKey(i);
			if (m_entities.Find(entity) == INDEX_NOT_FOUND)
			{
				delete m_view_grids.GetElement(i);
				m_view_grids.RemoveElement(i);
				c--;
				i--;
				change = true;
			}
		}

		

		// update all
		c = m_view_grids.Count();
		for (i = 0; i < c; i++) 
		{
			m_view_grids.GetElement( i ).UpdateItems();
		}

		// SORT
		
		// PrintString("SORT???");
		if ( added_item || true )
		{
			// PrintString("SORT");
			autoptr array<EntityAI> unsorted_objects = new array<EntityAI>;
			autoptr array<Param2<EntityAI, Widget>> sorted_objects = new array<Param2<EntityAI, Widget>>;
			array <int> sorted_keys = manager.GetSortedKeys();

			// create table with unsorted objects
			c = m_view_grids.Count();
			for (i = 0; i < c; i++) 
			{
				unsorted_objects.Insert(m_view_grids.GetKey(i));
				// PrintString(itoa(i)+"-"+m_view_grids.GetKey(i).GetName());
			}

			for ( i = 0; i < sorted_keys.Count(); i++ )
			{
				int sort_key = sorted_keys.Get(i);
				bool is_found = false;
				string www = "";
				for (x = 0; x < unsorted_objects.Count(); x++)  // prejdi vsetky item preview objects
				{
					entity = unsorted_objects.Get(x);
					int sort_id = entity.GetSlotId();
					if ( sort_key == sort_id )
					{
						sorted_objects.Insert(new Param2<EntityAI, Widget>>(entity, NULL));
						www = entity.GetName();
						is_found = true;
						break;
					}
				}
				
				if ( is_found == false )  // nenasiel attachment, zistime ci existuje ghost, ak ano, tak pridajme ghost
				{
					//PrintString("NOT FOUND " + ToString(sort_key) );
					if ( m_inventory_slots.Contains( sort_key ) )
					{
						Widget ghost = m_inventory_slots.Get( sort_key );
						sorted_objects.Insert(new Param2<EntityAI, Widget>>(NULL,ghost));
						ghost.Show( true );
						www = "GHOST-"+ sort_key.ToString();
					}
				}
				// PrintString( "KEY=" +ToString(sort_key)+" - "+www);
			}
			

			
			// for (x = 0; x < unsorted_objects.Count(); x++)
			// {
			// 	sorted_objects.Insert( unsorted_objects.Get(x) );
			// }
			

			c = sorted_objects.Count();
			for (i = 0; i < c; i++) 
			{
				if ( sorted_objects.Get(i).param1 )
				{
					// PrintString("ITEM" + ToString(GetContentGridWidget()));
					entity = sorted_objects.Get(i).param1;
					m_view_grids.Get( entity ).UpdateSort();	
				}
				else
				{
					Widget ghost_widget = sorted_objects.Get(i).param2;
					GetContentGridWidget().RemoveChild(ghost_widget);
					GetContentGridWidget().AddChild(ghost_widget);
					ghost_widget.SetSize( manager.GetAttachmentItemSize(), manager.GetAttachmentItemSize() );
					// ghost_widget.Show( true );
				}
			}

			m_contentGrid.Update();
		}
		

		if (m_cargo_view)
		{
			m_cargo_view.UpdateItems();
		}
	}
	
	void UpdateItems()
	{
		if (m_entity.IsNull()) return;
		
		EntityAI entity;
		Cargo cargo;
		int i, j, c, x;
		bool change = false;
		bool added_item = false;

		m_entities.Clear();
		InventoryViewBase view ;

		InventoryManager manager = InventoryManager.GetInstance();

		if ( GetDayZGame().IsNewInventory() )
		{
			UpdateItemsNewInventory();
			return;
		}
		
	
		// first with cargo -----------------------
		
		if ( m_isOwnedByPlayer ) // we dont want to show views for zombie's cargo
		{
			// add new entities from attachments
			c = m_entity.Ptr().AttachmentsCount();
			for (i = 0; i < c; i++)
			{
				entity = m_entity.Ptr().GetAttachmentFromIndex(i);

				if (entity && entity.IsInventoryItem() && entity.GetCargo())
				{
					if (m_views.Contains(entity) == false)
					{	
						view = new InventoryView(this, entity, InventoryViewProperties.ATTACHMENTS | InventoryViewProperties.CARGOS);
						added_item = true;
						view.CreateLayout();
						m_views.Set( entity, view );
					}
					m_entities.Insert(entity);	
				}
			}
			
			// add new entities from cargo
			if (m_entity.Ptr().GetCargo())
			{
				cargo = m_entity.Ptr().GetCargo();
				c = cargo.GetItemCount();
				
				for (i = 0; i < c; i++)
				{
					entity = cargo.GetItem(i);

					if (entity && entity.IsInventoryItem() && entity.GetCargo())
					{
						if (m_views.Contains(entity) == false)
						{	
							view = new InventoryView(this, entity, InventoryViewProperties.ATTACHMENTS | InventoryViewProperties.CARGOS);
							added_item = true;
							view.CreateLayout();
							m_views.Set(entity, view );
						}
						m_entities.Insert(entity);
					}
				}
			}
		}

		// then without cargo -----------------------
		
		// add new entities from attachments
		c = m_entity.Ptr().AttachmentsCount();
		for (i = 0; i < c; i++)
		{
			entity = m_entity.Ptr().GetAttachmentFromIndex(i);

			if (entity && entity.IsInventoryItem() )
			{
				if (m_views.Contains(entity) == false)
				{	
					if ( entity.GetCargo() == NULL && entity.GetSlotsCount() < 1)
					{
						view = new InventoryViewGrid(this, entity, 0 );
						added_item = true;
					}
					else
					{
						view = new InventoryView(this, entity, InventoryViewProperties.ATTACHMENTS | InventoryViewProperties.CARGOS);
						added_item = true;
					}
					view.CreateLayout();
					m_views.Set(entity, view );	
				}
				m_entities.Insert(entity);
			}
		}
		
		// add new entities from cargo
		if (m_entity.Ptr().GetCargo())
		{
			cargo = m_entity.Ptr().GetCargo();
			c = cargo.GetItemCount();
			
			for (i = 0; i < c; i++)
			{
				entity = cargo.GetItem(i);

				if (entity && entity.IsInventoryItem() && !entity.GetCargo())
				{
					if (m_views.Contains(entity) == false)
					{			
						view = new InventoryViewGrid(this, entity, 0 );
						added_item = true;
						view.CreateLayout();
						m_views.Set(entity, view );
					}
					m_entities.Insert(entity);
				}
			}
		}
		
		// remove unnecesary entities
		c = m_views.Count();
		for (i = 0; i < c; i++)
		{
			entity = m_views.GetKey(i);
			if (m_entities.Find(entity) == INDEX_NOT_FOUND)
			{
				delete m_views.GetElement(i);
				m_views.RemoveElement(i);
				c--;
				i--;
				change = true;
			}
		}

		if ( m_empty_slot == NULL )
		{
			m_empty_slot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/inventory/inventoryGridItemEmpty.layout",  m_contentGrid );
			m_empty_slot.SetSize( manager.GetAttachmentItemSize(), manager.GetAttachmentItemSize() );
			m_empty_slot.SetHandler(this);
		}

		// update all
		c = m_views.Count();
		for (i = 0; i < c; i++) 
		{
			m_views.GetElement( i ).UpdateItems();
		}

		// SORT
		if ( added_item )
		{
			autoptr array<EntityAI> unsorted_objects = new array<EntityAI>;
			autoptr array<EntityAI> sorted_objects = new array<EntityAI>;
			array <int> sorted_keys = manager.GetSortedKeys();

			c = m_views.Count();
			for (i = 0; i < c; i++) 
			{
				unsorted_objects.Insert(m_views.GetKey(i));
			}

			for ( i = 0; i < sorted_keys.Count(); i++ )
			{
				int sort_key = sorted_keys.Get(i);
				for (x = 0; x < unsorted_objects.Count(); x++)
				{
					entity = unsorted_objects.Get(x);
					int sort_id = entity.GetSlotId();
					if ( sort_key == sort_id )
					{
						sorted_objects.Insert( entity );
						unsorted_objects.Remove(x);
						break;
					}
				}
			}

			for (x = 0; x < unsorted_objects.Count(); x++)
			{
				sorted_objects.Insert( unsorted_objects.Get(x) );
			}

			c = sorted_objects.Count();
			for (i = 0; i < c; i++) 
			{
				entity = sorted_objects.Get(i);
				m_views.Get( entity ).UpdateSort();
			}		
		}
		
		if (change)
		{
			m_contentGrid.Update();
		}
		
		if (m_cargo_view)
		{
			m_cargo_view.UpdateItems();
		}
	}
	
	// ScriptedWidgetEventHandler override	
	bool OnDraggingOver(Widget w, int x, int y, Widget reciever)
	{
		InventoryManager manager = InventoryManager.GetInstance();
		InventoryItem dragged_item = manager.GetItem();
		
		if (m_entity.IsNull() || dragged_item == NULL) return false;
		
		if (manager.IsDragging() == false) return false;
		
		if (dragged_item && dragged_item.CanRemoveEntity() == false)
		{
			manager.SetItemColor(InventoryGrid.ITEM_COLOR_WRONG);
		}
		else if (dragged_item == m_entity.Ptr())
		{
			manager.SetItemColor(InventoryGrid.ITEM_COLOR_DRAG);
		} 	
		else if ( m_entity.Ptr().CanTakeEntityAsOwned(dragged_item) ) 
		{
			manager.SetItemColor(InventoryGrid.ITEM_COLOR_GOOD);
		}
		else
		{
			manager.SetItemColor(InventoryGrid.ITEM_COLOR_WRONG);
		}
		return true;
	}

	bool OnDropReceived(Widget w, int x, int y, Widget reciever)
	{
		if (m_entity.IsNull()) return false;
		
		InventoryManager manager = InventoryManager.GetInstance();
		InventoryItem dragged_item = manager.GetItem();
		
		if (manager.IsDragging() == false || dragged_item == NULL) return false;
		
		if (dragged_item != m_entity.Ptr() && m_entity.Ptr().CanTakeEntityAsOwned(dragged_item) ) 
		{
			m_entity.Ptr().TakeEntityAsOwned(dragged_item);
		}
		
		manager.Drop();
		return true;
	}
}
