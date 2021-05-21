// #include "scripts/GUI/Inventory/InventoryManager.c"

// #include "scripts/GUI/Inventory/InventoryView/InventoryView.c"
// #include "scripts/GUI/Inventory/InventoryView/InventoryViewBase.c"
// #include "scripts/GUI/Inventory/InventoryView/InventoryViewProperties.c"
// #include "scripts/GUI/Inventory/InventoryView/InventoryViewGrid.c"
// #include "scripts/GUI/Inventory/InventoryView/InventoryViewHands.c"

// #include "scripts/GUI/Inventory/InventoryCombinationFlags.c"
// #include "scripts/GUI/Inventory/InventoryDragFromType.c"
// #include "scripts/GUI/Inventory/InventoryDragToType.c"
// #include "scripts/GUI/Inventory/InventoryQuickbar.c"

// #include "scripts/GUI/Inventory/InventoryContainer/InventoryContainer.c"
// #include "scripts/GUI/Inventory/InventoryContainer/InventoryContainerEntity.c"
// #include "scripts/GUI/Inventory/InventoryContainer/InventoryContainerHands.c"
// #include "scripts/GUI/Inventory/InventoryContainer/InventoryContainerVicinity.c"

void EnfReadNote(string text)
{
	text.Replace("\n", "<br>");
	
	InventoryMenu menu = GetGame().GetUIManager().FindMenu(MENU_INVENTORY);
	if (menu)
	{	
		NoteMenu note_menu = menu.EnterScriptedMenu(MENU_NOTE);
		if (note_menu)
		{
			note_menu.InitRead(text);
		}
	}
	}

void EnfWriteNote(InventoryItem paper, InventoryItem pen)
{
	InventoryMenu menu = GetGame().GetUIManager().FindMenu(MENU_INVENTORY);
	if (menu)
	{	
		NoteMenu note_menu = menu.EnterScriptedMenu(MENU_NOTE);
		if (note_menu)
		{
			note_menu.InitWrite(paper, pen);
		}
	}
}

class InventoryMenu extends UIScriptedMenu
{
	const int TOOLTIP_TIME = 1.0;
	const int PLAYER_STATUS_COUNT = 8;
	private int m_characterRotationX;
	private int m_characterRotationY;
	private int m_characterScaleDelta;
	private bool m_isCharacterRotating;
	private vector m_characterOrientation;
	
	private autoptr Link<EntityAI> m_target;
	private EntityAI m_itemInHands;
	private autoptr Link<InventoryItem> m_tooltip_target;
	private Widget m_tooltip_widget;

	private autoptr array<Object> m_ground_objects;
	private autoptr array<InventoryContainerEntity> m_ground_objects_containers;

	private autoptr Timer m_refresh_timer;
	
	private autoptr ContextMenu m_context_menu;

	private PlayerPreviewWidget m_character_panel_widget;	
	private Widget	m_character_panel_area_widget;
	private Widget	m_left_panel;
	private Widget	m_right_panel;
	private Widget	m_center_panel;
	private Widget	m_left_panel_content;
	private Widget	m_right_panel_content;
	private Widget	m_center_panel_content;
	private Widget	m_center_panel_border;
	private Widget	m_left_panel_border;
	private Widget	m_right_panel_border;
	private Widget	m_vicinity_panel;
	private Widget	m_quickbar_widget;
	private autoptr InventoryContainerHands m_hands_container;
	autoptr InventoryContainerVicinity m_ground_container;
	autoptr InventoryContainerEntity m_equipment_container;
	private autoptr InventoryContainerEntity m_target_container;
	private autoptr InventoryQuickbar m_quickbar;
	private autoptr InventoryManager m_drag_manager;
	
	void InventoryMenu()
	{
		m_characterRotationX = 0;
		m_characterRotationY = 0;
		m_characterScaleDelta = 1;
		m_isCharacterRotating = false;
		
		m_context_menu = new ContextMenu;
		m_refresh_timer = new Timer(CALL_CATEGORY_GUI);
		m_drag_manager = new InventoryManager;

		m_ground_objects = new array<Object>;
		m_ground_objects_containers = new array<InventoryContainerEntity>;
	}

	void ~InventoryMenu()
	{
		s_ground_container_list_switch = false;
		s_equipment_container_list_switch = false;
		s_target_container_list_switch = false;
		
		if (m_ground_container != NULL)
		{
			s_ground_container_list_switch = m_ground_container.IsListView();
		}
			
		if (m_target_container != NULL)
		{
			s_target_container_list_switch = m_target_container.IsListView();
		}
		
		if (m_equipment_container != NULL)
		{
			s_equipment_container_list_switch = m_equipment_container.IsListView();
		}

		InventoryContainerEntity container;
		if (m_ground_objects_containers != NULL)
		{
			int c = m_ground_objects_containers.Count();
			for ( int i = 0; i < c; i++)
			{
				container = m_ground_objects_containers.Get(i);
				if ( container && container != NULL )
				{
					delete container;
				}
			}
		}
			
		GetGame().GetDragQueue().RemoveCalls(this);
		delete m_Player_Statuses;
		delete m_Player_StatusesBackground;
	}
	
#ifdef PLATFORM_XBOX
	bool UseMouse()
	{
		return true;
	}
#endif
	
	bool UseKeyboard() 
	{
		return false;
	}

	Widget Init()
	{
		if (layoutRoot) return layoutRoot;
		
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_inventory.layout", NULL);

		PlayerBase player = g_Game.GetPlayer();

		// context menu
		m_context_menu.Init(layoutRoot);

		// tooltip
		m_tooltip_widget = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_inventory_tooltip.layout", layoutRoot);
		m_tooltip_widget.Show( false );

		// player preview
		m_character_panel_area_widget = layoutRoot.FindAnyWidget("CharacterPanel");

		m_character_panel_widget = layoutRoot.FindAnyWidget("CharacterPanelWidget");
		m_character_panel_widget.SetPlayer(player);
		vector pivot;
		pivot[0] = 0;
		pivot[1] = 0;
		pivot[2] = 0.605;
		m_character_panel_widget.SetModelPosition( pivot );

		m_character_panel_widget.SetSize( 1.34, 1.34 );  // default scale
		UpdateScale();
		UpdatePlayerPreviewAnim(player.GetEntityInHands());
		
		// player status
		m_Player_Statuses = new array<TextWidget>;
		m_Player_Statuses.Insert( layoutRoot.FindAnyWidget("Status0") );
		m_Player_Statuses.Insert( layoutRoot.FindAnyWidget("Status1") );
		m_Player_Statuses.Insert( layoutRoot.FindAnyWidget("Status2") );
		m_Player_Statuses.Insert( layoutRoot.FindAnyWidget("Status3") );
		m_Player_Statuses.Insert( layoutRoot.FindAnyWidget("Status4") );
		m_Player_Statuses.Insert( layoutRoot.FindAnyWidget("Status5") );
		m_Player_Statuses.Insert( layoutRoot.FindAnyWidget("Status6") );
		m_Player_Statuses.Insert( layoutRoot.FindAnyWidget("Status7") );

		// player status
		m_Player_StatusesBackground = new array<ImageWidget>;
		m_Player_StatusesBackground.Insert( layoutRoot.FindAnyWidget("StatusBackground0") );
		m_Player_StatusesBackground.Insert( layoutRoot.FindAnyWidget("StatusBackground1") );
		m_Player_StatusesBackground.Insert( layoutRoot.FindAnyWidget("StatusBackground2") );
		m_Player_StatusesBackground.Insert( layoutRoot.FindAnyWidget("StatusBackground3") );
		m_Player_StatusesBackground.Insert( layoutRoot.FindAnyWidget("StatusBackground4") );
		m_Player_StatusesBackground.Insert( layoutRoot.FindAnyWidget("StatusBackground5") );
		m_Player_StatusesBackground.Insert( layoutRoot.FindAnyWidget("StatusBackground6") );
		m_Player_StatusesBackground.Insert( layoutRoot.FindAnyWidget("StatusBackground7") );

		float w,h;
		m_left_panel = layoutRoot.FindAnyWidget("LeftPanel");	
		m_right_panel = layoutRoot.FindAnyWidget("RightPanel");
		m_center_panel = layoutRoot.FindAnyWidget("CenterPanel");
		m_left_panel_content = m_left_panel.FindAnyWidget("Content");	
		m_right_panel_content = m_right_panel.FindAnyWidget("Content");
		m_center_panel_content = m_center_panel.FindAnyWidget("Content");

		m_quickbar_widget = layoutRoot.FindAnyWidget("QuickbarGrid");	

		m_center_panel_border = layoutRoot.FindAnyWidget("CenterPanelBorder");
		m_left_panel_border = layoutRoot.FindAnyWidget("LeftPanelBorder");
		m_right_panel_border = layoutRoot.FindAnyWidget("RightPanelBorder");
		m_vicinity_panel = layoutRoot.FindAnyWidget("VicinityPanel");
		
		m_left_panel_content.GetScreenSize(w,h);	
		
		InventoryManager manager = InventoryManager.GetInstance();
		manager.RecalculateIconSize(w);
		
		return layoutRoot;
	}
	
	void InitContainers(EntityAI target)
	{
		float w = 64;
		float h = 64;
		PlayerBase player = g_Game.GetPlayer();
		
		if (m_target && m_target.Ptr() != target)
		{
			delete m_target;
			m_target = NULL;
			
			delete m_target_container;
			m_target_container = NULL;
		}

		if (target && !m_target)
		{
			if (target.IsInherited( InventoryItem) || target.IsInherited( Building) || target.IsInherited( PlayerBase) || target.IsInherited( Car) || target.IsInherited( AnimalBase) )
			{
				m_target = new Link<EntityAI>(target);
			}
		}

		//-----
		if (m_hands_container == NULL)
		{
			m_hands_container = new InventoryContainerHands(m_center_panel_content, true, true );
			m_hands_container.Init();
			m_hands_container.SetHeaderText("EMPTY HANDS");
			m_hands_container.UpdateItems();
		}	
		
		if (/*target == NULL && */m_ground_container == NULL)
		{
			m_ground_container = new InventoryContainerVicinity(m_left_panel_content, false, true );
			m_ground_container.Init();
			m_ground_container.SetHeaderText("#container_vicinity");
			m_ground_container.UpdateItems();
		}
		
		if (m_target && m_target.IsNull() == false && m_ground_container)
		{
			delete m_ground_container;
			m_ground_container = NULL;
		}
			
		if (m_target_container == NULL && m_target && m_target.Ptr())
		{
			m_target_container = new InventoryContainerEntity(m_left_panel_content, false,true);
			m_target_container.Init(m_target.Ptr());
			m_target_container.SetHeaderText(m_target.Ptr().GetName());
			m_target_container.UpdateItems();
		}
		
		if (m_equipment_container == NULL)
		{
			m_equipment_container = new InventoryContainerEntity(m_right_panel_content, true,true);
			m_equipment_container.Init(GetGame().GetPlayer());
			m_equipment_container.SetHeaderText("#container_inventory");
			m_equipment_container.UpdateItems();	
		}
		
		if (m_quickbar == NULL)
		{
			m_quickbar = new InventoryQuickbar( m_quickbar_widget );
			m_quickbar.UpdateItems( m_quickbar_widget );
		}
		
		InventoryView.ClearHistory();
	}
	
	void RefreshPlayerStatus()
	{
		//if ( g_Game.IsNewPlayer() )
		{
			// clear table
			string ordered_notifiers[PLAYER_STATUS_COUNT];
			for ( int i = 0; i < PLAYER_STATUS_COUNT; i++ )
			{
				ordered_notifiers[i]="";
				m_Player_Statuses.Get(i).SetText( "" );
				m_Player_StatusesBackground.Get( i ).Show( false );
			}

			// sort keys (use index+key as key in temporary static table - we can use sort function )
			// create table for sorting
			PluginPlayerStatus module_player_status = (PluginPlayerStatus)GetPlugin(PluginPlayerStatus);
			if ( ! module_player_status ) return;
			
			for ( i = 0; i < module_player_status.m_NotifiersLabel.Count(); i++ )
			{
				int index = module_player_status.m_NotifiersIndexColor.GetByIndex(i).Get(0); 
				string key = String(  index.ToString() + module_player_status.m_NotifiersLabel.GetKeyByIndex(i).ToString() ); //string(index+key)
				ordered_notifiers[i] = key;
			}
			// sort
			Sort(ordered_notifiers, PLAYER_STATUS_COUNT);
			// remove index, so we have sorted table containing keys (sorted by index)
			for ( i = 0; i < PLAYER_STATUS_COUNT; i++ )
			{
				string index_key = ordered_notifiers[i];
				int length =  index_key.Length( ) - 1;
				if ( length > 0 )
				{
					ordered_notifiers[i] = index_key.Substring( 1, length);
				}
			}
			
			// Print("------------------------ sort");
			// for ( i = 0; i < PLAYER_STATUS_COUNT; i++ )  { Print( ordered_notifiers[i] ); }
			// Print("------------------------ ");

			// render statuses
			int status_text_widget_index = 0;
			for ( i = 0; i < PLAYER_STATUS_COUNT; i++ )
			{
				int modifier_key  = ordered_notifiers[i].ToInt();
				
				if ( modifier_key )
				{
					string label = module_player_status.m_NotifiersLabel.Get( modifier_key ).Get(0);
					int color = module_player_status.m_NotifiersIndexColor.Get( modifier_key ).Get(1);
					if ( label && color )
					{
						TextWidget player_status_widget = m_Player_Statuses.Get( status_text_widget_index );
						ImageWidget player_status_widget_background = m_Player_StatusesBackground.Get( status_text_widget_index );
						player_status_widget.SetText( label );
						player_status_widget_background.SetColor( color );
						player_status_widget_background.Show( true );
						status_text_widget_index = status_text_widget_index + 1;
					}
				}
			}
		}
	}
	
	void Blur(float intensity)
	{
		Material matColors = GetGame().GetWorld().GetMaterial("graphics/materials/postprocess/gauss");
		if (matColors)
			matColors.SetParam( "Intensity", intensity);
	}

	// menu overloads
	void OnShow()
	{
		super.OnShow();
		PPEffects.SetBlurInventory(1);

		PlayerBase player = g_Game.GetPlayer();
		m_refresh_timer.Run(0.5, this, "PerformRefreshInventory", NULL, true);
		
		// refresh ingame hud quickbar
		Mission mission = GetGame().GetMission();
		if ( mission )
		{
			IngameHud hud = mission.GetHud();
			if ( hud )
			{
				hud.HideQuickbar( true, true );
				hud.SetLeftStatsVisibility(false);
			}
		}
		RefreshQuickbar();
		ShowQuickbar();
			
		PerformRefreshInventory();
		Refresh();
	}

	void OnHide()
	{
		super.OnHide();
			
			PPEffects.SetBlurInventory(0);	

		m_refresh_timer.Stop();
		HidePanelBorders();
		
		InventoryManager manager = InventoryManager.GetInstance();	
		if ( manager )
		{
			manager.HideTooltip();
		}

		Mission mission = GetGame().GetMission();
		if ( mission )
		{
			IngameHud hud = mission.GetHud();
			if ( hud )
			{
				hud.SetLeftStatsVisibility(true);
				if ( hud.m_quickbar_state )
				{
					hud.ShowQuickbar();
				}
			}
		}
		HideQuickbar();
	}
	
	bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		
		return false;
	}

	bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		super.OnMouseButtonDown(w, x, y, button);
		
		if ( w == m_character_panel_area_widget )
		{
			GetGame().GetDragQueue().Call(this, "UpdateRotation");
			g_Game.GetMousePos(m_characterRotationX, m_characterRotationY);
		}
		return false;
	}

	bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		super.OnMouseButtonUp(w, x, y, button);
		
		m_isCharacterRotating = false;

		return false;
	}

	bool OnMouseWheel(Widget  w, int  x, int  y, int wheel)
	{
		super.OnMouseWheel(w, x, y, wheel);
		
		if ( w == m_character_panel_area_widget )
		{
			GetGame().GetDragQueue().Call(this, "UpdateScale");
			m_characterScaleDelta = wheel ;
		}
		return false;
	}
	
	bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		super.OnDoubleClick(w, x, y, button);
		
		if (w && w.GetTypeID() == ItemPreviewWidgetTypeID && button == MouseState.LEFT)
		{
			Man player = GetGame().GetPlayer();
			ItemPreviewWidget ipw = w;
			EntityAI item = ipw.GetItem();
			EntityAI item_in_hands = player.GetEntityInHands();

			if (item && item.IsInventoryItem() && item.GetOwnerPlayer() == player && item_in_hands != item)
			{
				player.MoveItemFromHandsToInventory(item_in_hands);
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(player, "TakeEntityToHands", new Param1<EntityAI>(item));
			}
			else if (item && player.CanTakeEntityAsOwned(item) &&  item.GetOwnerPlayer() != player)
			{
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(player, "TakeEntityAsOwned", new Param1<EntityAI>(item));
			} 
			else if (item && item.IsInventoryItem())
			{
				//InspectItem(item);
			}
		}
	}
	
	void UpdateRotation(int mouse_x, int mouse_y, bool is_dragging)
	{
		vector o = m_characterOrientation;
		o[1] = o[1] - (m_characterRotationX - mouse_x);
		m_character_panel_widget.SetModelOrientation(o);

		if (!is_dragging)
		{	
			m_characterOrientation = o;
		}
	}

	void OnPerformAction(Action action, PlayerBase player)
	{
		player.PerformAction(action);
		
		//GetContextMenu().Add(action_text, player, "PerformAction", new Param1<Action>(action));
	}

	void UpdateScale()
	{
		float w, h;
		m_character_panel_widget.GetSize(w,h);
		w = w + ( m_characterScaleDelta / 25 );
		h = h + ( m_characterScaleDelta / 25 );
		if ( w > 0.62 && w < 3 )
		{
			m_character_panel_widget.SetSize( w, h );
		}
	}

	void UpdatePlayerPreviewAnim(EntityAI inHands)
	{
		if (m_character_panel_widget == NULL || m_itemInHands == inHands)	return;
		m_itemInHands = inHands;		
		
		m_character_panel_widget.SetItemInHands(inHands);
		
		if (inHands == NULL)
		{	
			m_character_panel_widget.SwitchMove("menu_idleUnarmed0");
		}
		else if (inHands.IsKindOf("Pistol"))
		{
			m_character_panel_widget.SwitchMove("menu_idlePistol0");
		}
		else if (inHands.IsKindOf("Rifle"))
		{
			m_character_panel_widget.SwitchMove("menu_idlerifleLong0");
		}
		else if (inHands.IsKindOf("KnifeBase"))
		{
			m_character_panel_widget.SwitchMove("menu_idleKnife0");
		}
		else if (inHands.IsKindOf("BaseballBat"))
		{
			m_character_panel_widget.SwitchMove("menu_idleBat0");
		}
		else if (inHands.IsKindOf("Chainsaw"))
		{
			m_character_panel_widget.SwitchMove("menu_idleChainsaw0");
		}
		else if (inHands.IsKindOf("MeleeItemBase"))
		{
			m_character_panel_widget.SwitchMove("menu_idleHatchet0");
		}	
		else
		{
			m_character_panel_widget.SetItemInHands(NULL);
			m_character_panel_widget.SwitchMove("menu_idleUnarmed0");
		}
	}
	
	
	bool OnFocusLost(Widget w, int x, int y)
	{
		super.OnFocusLost(w, x, y);
		
		m_isCharacterRotating = false;
		
		return false;
	}

	bool OnMouseEnter(Widget w, int x, int y)
	{
		super.OnMouseEnter(w, x, y);
	}

	bool OnMouseLeave (Widget w, Widget enterW, int x, int y)
	{
		super.OnMouseLeave(w, enterW, x, y);
		
		return false;
	}

	bool OnSelect(Widget w, int x, int y)
	{
		super.OnSelect(w, x, y);
		
		return false;
	}
	
	void Refresh()
	{
		Man player = g_Game.GetPlayer();
		if (m_target && m_target.IsNull() == false && GetGame().IsObjectAccesible(m_target.Ptr(), player) == false)
		{
			InitContainers(NULL);
			return;
		}

		UpdatePlayerPreviewAnim(player.GetEntityInHands());
	}

	void RefreshQuickbar()
	{
		if ( m_quickbar )
		{
			m_quickbar.UpdateItems( m_quickbar_widget );
		}
	}
	
	void ShowQuickbar()
	{
		if ( m_quickbar_widget )
		{
			m_quickbar_widget.Show( true );
		}
	}

	void HideQuickbar()
	{
		if ( m_quickbar_widget )
		{
			m_quickbar_widget.Show( false );
		}
	}

	void HidePanelBorders()
	{
		if ( m_center_panel_border )
		{
			// m_center_panel_border.SetColor(0x00FFFFFF);
			m_left_panel_border.SetColor(0x00FFFFFF);
			m_right_panel_border.SetColor(0x00FFFFFF);
			m_center_panel_content.SetColor(0x00FFFFFF);
			m_center_panel.SetColor(0x00FFFFFF);
		}
	}

	void ShowHandsPanelBorder()
	{
		if ( m_center_panel )
		{
			m_center_panel.SetColor(0x3FFFFFFF);
		}
	}
	
	void ShowRightPanelBorder()
	{
		if ( m_right_panel_border )
		{
			m_right_panel_border.SetColor(0x3FFFFFFF);
		}
	}

	void ShowLeftPanelBorder()
	{
		if ( m_left_panel_border )
		{
			m_left_panel_border.SetColor(0x3FFFFFFF);
		}
	}

	void PerformRefreshInventory()
	{
		Man player = GetGame().GetPlayer();
		if( player == NULL)
		{
			return;
		}
		
		if (player.CanOpenInventory() == false)
		{
			GetGame().GetUIManager().GetMenu().UnlockControls();
			GetGame().GetUIManager().HideMenu(MENU_INVENTORY);
			return;
		}

		if (m_target_container) 
		{
			bool reset_condition = false;
			
			if ( m_target != NULL && m_target.IsNull() == false )
			{
				if ( !GetGame().IsObjectAccesible(m_target.Ptr(), player) || !CanShowItemInInventory(GetGame().GetPlayer(), m_target.Ptr()) )
				{
					reset_condition = true;
				}
			}
			
			if ( reset_condition )
			{
				InitContainers(NULL);
			}
			else
			{
				m_target_container.UpdateItems();
			}
		}
		
		if (m_hands_container) m_hands_container.UpdateItems();
		if (m_ground_container) m_ground_container.UpdateItems();
		if (m_equipment_container) m_equipment_container.UpdateItems();
		

		
		Object obj;
		Cargo cargo;
		int i,c;
		InventoryContainerEntity container;

		// find objects near player
		vector pos = player.GetPosition();
		vector dir = player.GetDirection();
		autoptr array<Object> objects = new array<Object>;
		autoptr array<Cargo> proxyCargos = new array<Cargo>;
		GetGame().GetObjectsAtPosition(pos + (dir * 0.75), 1.0, NULL, objects, proxyCargos);

		// remove container objects which are not anymore in list of nearby objects
		c = m_ground_objects.Count();
		for ( i = 0; i < c; i++)
		{
			obj = m_ground_objects.Get(i);
			int find_index = objects.Find( obj );
			if ( find_index == -1 )
			{
				container = m_ground_objects_containers.Get(i);
				if ( container != NULL )
				{
					delete container;
					container = NULL;
				}
				
				m_ground_objects.Remove( i );
				m_ground_objects_containers.Remove( i );
				c--;
				i--;
			}
			else
			{
				container = m_ground_objects_containers.Get(i);
				container.UpdateItems();
				objects.Remove(find_index);
			}
		}

		// create containers for nearby objects
		c = objects.Count();
		for ( i = 0; i < c; i++)
		{
			obj = objects.Get(i);
			if ( CanShowItemInInventory(player, obj) )
			{
				if ( obj.IsInherited(PlayerBase) || obj.IsInherited(ZombieBase) )
				{
					int container_index = m_ground_objects.Find( obj );
					if ( container_index < 0 )
					{
						bool show_viewgrid = true;
						if ( obj.IsInherited(ZombieBase) )
						{
							show_viewgrid = false;
						}

						container = new InventoryContainerEntity( m_left_panel_content, false, show_viewgrid );
						container.Init(obj);
						container.SetHeaderText( obj.GetName() );
						container.UpdateItems();
						
						m_ground_objects.Insert( obj );
						m_ground_objects_containers.Insert( container );
					}
				}
			}
		}
		
		RefreshQuickbar();
		RefreshPlayerStatus();

		Widget widget_under_cursor = GetWidgetUnderCursor();
		if ( widget_under_cursor == NULL )
		{
			m_drag_manager.HideTooltip();
		}
	}

	ContextMenu GetContextMenu()
	{
		return m_context_menu;
	}

	Widget GetCenterPanel()
	{
		return m_center_panel;
	}

	// ScriptedWidgetEventHandler override	
	bool OnDraggingOver(Widget w, int x, int y, Widget reciever)
	{
		super.OnDraggingOver(w, x, y, reciever);
		
		InventoryManager manager = InventoryManager.GetInstance();
		InventoryItem dragged_item = manager.GetItem();
		PlayerBase player = GetGame().GetPlayer();
		HidePanelBorders();

		if (dragged_item && !(player.GetEntityInHands() != dragged_item || player.CanRemoveEntityInHands()))
		{
			manager.SetItemColor(InventoryGrid.ITEM_COLOR_WRONG);
			return true;
		}
		else if (reciever == m_character_panel_area_widget && dragged_item)
		{
			if (player.CanTakeEntityAsOwned(dragged_item))
			{
				manager.SetItemColor(InventoryGrid.ITEM_COLOR_GOOD);
			} 	
			else
			{
				manager.SetItemColor(InventoryGrid.ITEM_COLOR_WRONG);
			}
			ShowRightPanelBorder();
		}
		else if (reciever == m_left_panel.GetChildren() && dragged_item)
		{
			manager.SetItemColor(InventoryGrid.ITEM_COLOR_GOOD);
			ShowLeftPanelBorder();
		}
		else if (reciever == m_left_panel_border && dragged_item)
		{
			manager.SetItemColor(InventoryGrid.ITEM_COLOR_GOOD);
			ShowLeftPanelBorder();
		}
		else if (reciever == m_vicinity_panel && dragged_item)
		{
			manager.SetItemColor(InventoryGrid.ITEM_COLOR_GOOD);
			ShowLeftPanelBorder();
		}
		else if (reciever == m_right_panel.GetChildren() && dragged_item)
		{
			if (player.CanTakeEntityAsOwned(dragged_item))
			{
				manager.SetItemColor(InventoryGrid.ITEM_COLOR_GOOD);
			} 	
			else
			{
				manager.SetItemColor(InventoryGrid.ITEM_COLOR_WRONG);
			}
			ShowRightPanelBorder();
		}
		else if (reciever == m_right_panel_border && dragged_item)
		{
			if (player.CanTakeEntityAsOwned(dragged_item))
			{
				manager.SetItemColor(InventoryGrid.ITEM_COLOR_GOOD);
			} 	
			else
			{
				manager.SetItemColor(InventoryGrid.ITEM_COLOR_WRONG);
			}
			ShowRightPanelBorder();
		}
		else
		{
			
			if ( reciever.GetName() == "HeaderBgHands" )
			{
				ShowHandsPanelBorder();
				return false;
			}
			else
			{
				manager.SetItemColor(InventoryGrid.ITEM_COLOR_DRAG);
			}
		}

		return true;	
	}

	bool OnDropReceived(Widget w, int x, int y, Widget reciever)
	{
		super.OnDropReceived(w, x, y, reciever);
		
		InventoryManager manager = InventoryManager.GetInstance();
		InventoryItem dragged_item = manager.GetItem();
		PlayerBase player = GetGame().GetPlayer();	
		
		if (dragged_item && (player.GetEntityInHands() != dragged_item || player.CanRemoveEntityInHands()))
		{

			// do not re-order when dropping on same view
			if ( ( reciever == m_character_panel_area_widget || reciever == m_right_panel.GetChildren() ) && dragged_item.GetOwnerPlayer() == player )
			{
				HidePanelBorders();
				return true;	
			}
			
			if (reciever == m_character_panel_area_widget && dragged_item)
			{
				if (player.CanTakeEntityAsOwned(dragged_item))
				{
					player.TakeEntityAsOwned(dragged_item);
				}
			}
			else if (reciever == m_left_panel.GetChildren() && dragged_item)
			{
				manager.DropItem(dragged_item);
			}
			else if (reciever == m_left_panel_border && dragged_item)
			{
				manager.DropItem(dragged_item);
			}
			else if (reciever == m_vicinity_panel && dragged_item)
			{
				manager.DropItem(dragged_item);
			}
			else if (reciever == m_right_panel.GetChildren() && dragged_item)
			{
				if (player.CanTakeEntityAsOwned(dragged_item))
				{
					player.TakeEntityAsOwned(dragged_item);
				}
			}
			else if (reciever == m_right_panel_border && dragged_item)
			{
				if (player.CanTakeEntityAsOwned(dragged_item))
				{
					player.TakeEntityAsOwned(dragged_item);
				}
			}
		}

		HidePanelBorders();

		manager.Drop();
		return true;
	}

	// drag & drop stuff
	bool OnDrag(Widget w, int x, int y)
	{
		super.OnDrag(w, x, y);
		
		return false;
	}

	bool OnDragging(Widget w, int x, int y, Widget reciever)
	{
		super.OnDragging(w, x, y, reciever);
		
		return false;
	}

	bool OnDrop(Widget w, int x, int y, Widget reciever)
	{
		super.OnDrop(w, x, y, reciever);
		
		return false;
	}
	
	// PLAYER STATUS
	private array<TextWidget> m_Player_Statuses;
	private array<ImageWidget> m_Player_StatusesBackground;

	// containers list switch history
	static bool s_ground_container_list_switch;
	static bool s_equipment_container_list_switch;
	static bool s_target_container_list_switch;
}
