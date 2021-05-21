// #include "Scripts/GUI/IngameHud.c"

class ActivityGear extends EntityScriptedActivity
{
	protected InventoryMenu m_inventory_menu; 
	autoptr Link<EntityAI> m_target;

	void ~ActivityGear()
	{
		DestroyInventory();
	}
	
	bool OnSimulate(float timeslice)
	{
		bool init = false;
		UIScriptedMenu menu = GetGame().GetUIManager().GetMenu();
		
		if (menu == NULL && GetGame().GetPlayer().CanOpenInventory())
		{
			if (m_inventory_menu == NULL)
			{
				m_inventory_menu = GetGame().GetUIManager().EnterScriptedMenu(MENU_INVENTORY, NULL);
			}
			else if (GetGame().GetUIManager().FindMenu(MENU_INVENTORY) == NULL)
			{
				GetGame().GetUIManager().ShowScriptedMenu(m_inventory_menu, NULL);
			}		
			init = true;
		}
		
		if (menu && menu == m_inventory_menu)
		{
			init = true;
		}
		
		if (m_inventory_menu && init)
		{
			m_inventory_menu.InitContainers(m_target.Ptr());
		}

		return false;
	}

	void DestroyInventory()
	{
		if (m_inventory_menu)
		{
			m_inventory_menu.Close();
			m_inventory_menu = NULL;	
		}
	}
}

class MissionGameplay extends MissionBase
{
	//--
	float 	m_TimeTillNextSave;

	int		m_life_state;
	bool	m_initialized;
	bool	m_invUpdateThisFrame;
	Widget	m_hud_root_widget;
	ImageWidget m_microphone_icon;

	private autoptr InventoryManager m_drag_manager;
	
	autoptr Ref<ActivityGear>	m_gear_activity;
	autoptr Chat				m_chat;
	autoptr ActionMenu			m_actionMenu;
	autoptr IngameHud			m_hud;
	HudDebug					m_hud_debug;	

	autoptr Timer				m_chat_channel_hide_timer;
	autoptr WidgetFadeTimer		m_chat_channel_fade_timer;
	
	Widget						m_chat_channel_area;
	TextWidget					m_chat_channel_text;		
	
	void MissionGameplay()
	{
		//--
		MakeDirectory("$profile:db");
		MakeDirectory("$profile:db\\dead\\");
		MakeDirectory("$profile:db\\alive\\");
		//--
		/*
		Input input = GetGame().GetInput();
		autoptr TIntArray keys = new TIntArray;

		input.GetActionKeys(0, keys);
		keys.Debug();
		input.GetActionKeys(1, keys);
		keys.Debug();

		autoptr TIntArray keys = new TIntArray;
		for (int i = 0; i < UAN; i++)
		{
			GetGame().GetInput().GetCurrentProfileActionKeys(i + 1, keys);
			string name;
			input.GetActionKeyName(i + 1, name);
			Print("key: " + name);
			keys.Debug();
			Print("---");
		}
		*/

		m_initialized = false;
		m_hud_root_widget = NULL;
		m_chat = new Chat;
		m_actionMenu = new ActionMenu;
		m_life_state = -1;
		m_hud = new IngameHud;
		m_chat_channel_fade_timer = new WidgetFadeTimer;	
		m_chat_channel_hide_timer = new Timer(CALL_CATEGORY_GUI);

		m_drag_manager = new InventoryManager;
		Print("MissionGameplay()");
	}

	void ~MissionGameplay()
	{
		delete m_chat;
		delete m_actionMenu;
		delete m_hud;
		delete m_hud_debug;
		delete m_drag_manager;
	}

	void OnInit()
	{
		//if (m_initialized)
		//	return;
		
		Print("OnInit()");
		m_initialized = true;

		// init hud ui
		if (!m_hud_root_widget)
		{
			m_hud_root_widget = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_hud.layout");
			m_hud_root_widget.Show(false);
			m_chat.Init(m_hud_root_widget.FindAnyWidget("ChatFrameWidget"));
			m_actionMenu.Init( m_hud_root_widget.FindAnyWidget("ActionsPanel"), m_hud_root_widget.FindAnyWidget("DefaultActionWidget") );
			m_hud.Init( m_hud_root_widget.FindAnyWidget("HudPanel") );
			m_microphone_icon = (ImageWidget)m_hud_root_widget.FindAnyWidget("mic");

			// notification window
			m_notification_widget = new NotificationMessage( m_hud_root_widget );
			
			// chat channel
			m_chat_channel_area = m_hud_root_widget.FindAnyWidget("ChatChannelPanel");
			m_chat_channel_text = m_hud_root_widget.FindAnyWidget("ChatChannelText");
		}
		
		// init hud ui
		
		//if ( GetGame().IsDebug() )
		//{
			if (PluginConfigDebugProfile.GetInstance())
			{
				m_hud_debug = new HudDebug;
				if ( !m_hud_debug.IsInitialized() )
				{
					m_hud_debug.Init( GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_hud_debug.layout") );
					Debug.SetEnabledLogs(PluginConfigDebugProfile.GetInstance().GetLogsEnabled());	
				}
			}
		//}

		// init activities
		m_gear_activity = new Ref<ActivityGear>(new ActivityGear);
		m_gear_activity.AddRef();
	}

	void OnMissionStart()
	{
		if (m_hud_root_widget)
			m_hud_root_widget.Show(true);

		g_Game.GetUIManager().ShowCursor(false);
		g_Game.SetMissionState( DayZGame.MISSION_STATE_GAME );

		if (g_Game.m_OfflineMissions == 1)
		{
			GetGame().ExecuteSQF("DZ_OFFLINE_CLE = true;");
		}
	}

	
	void TickScheduler(float timeslice)
	{
		PlayerBase player = GetGame().GetPlayer();
		
		if( player) player.OnTick();
	}
	
	void OnMissionFinish()
	{
		PlayerBase player = GetGame().GetPlayer();
		
		if ( player != NULL )
		{
			int player_last_shaved_seconds = player.GetLastShavedSeconds();
			GetGame().SetProfileString( "lastShavedSeconds", player_last_shaved_seconds.ToString() );
			int player_cached_playtime = player.GetCachedPlaytime();
			GetGame().SetProfileString( "cachedPlaytime", player_cached_playtime.ToString() );
		}
		
		GetGame().GetUIManager().HideDialog();
		DestroyAllMenus();

		if (m_chat)
			m_chat.Destroy();

		if (m_hud_root_widget)
		{
			m_hud_root_widget.Destroy();
			m_hud_root_widget = NULL;
		}
		g_Game.GetUIManager().ShowCursor(false);
		g_Game.SetMissionState( DayZGame.MISSION_STATE_FINNISH );
	}

	void OnUpdate(float timeslice)
	{
		Man player = GetGame().GetPlayer();

		m_TimeTillNextSave += timeslice;
		//Save loop
		if (player && m_TimeTillNextSave >= 120)
		{
			int lifestate = 0;
			if (player)
			{
				if (player.GetLifeState() == LifeStateAlive)
				{
					lifestate = 1;
				}else{
					lifestate = 0;
				}
			}
			GetGame().ExecuteSQF("[" + lifestate +", 0, player] call dbSavePlayer;"); //lifestate, ID, player
			GetGame().ExecuteSQF("[player,\"Player progress saved to file!\",\"ColorFriendly\"] call fnc_playerMessage;");
			m_TimeTillNextSave = 0.0;
		}

		UIManager manager = GetGame().GetUIManager();
		TickScheduler(timeslice);
		UIScriptedMenu menu = manager.GetMenu();
		InventoryMenu inventory = manager.FindMenu(MENU_INVENTORY);
		InspectMenu inspect = manager.FindMenu(MENU_INSPECT);
		Input input = GetGame().GetInput();
		
		if (inspect && player.CanOpenInventory() == false)
		{
			manager.CloseMenu(MENU_INSPECT);
		}
		
		if (player && m_life_state == LifeStateAlive)
		{
			if (inventory && menu == inventory && input.GetActionDown(UAGear, false, true))
			{
				HideInventory();
			}


			if (input.GetActionDown(UAChat, false, true))
			{
				if (menu == NULL)
				{
					m_chat_channel_hide_timer.Stop();
					m_chat_channel_fade_timer.Stop();
					m_chat_channel_area.Show(false);
					manager.EnterScriptedMenu(MENU_CHAT_INPUT, NULL);
				}
			}

			if ( input.GetActionDown( UAUIQuickbarToggle, false, true) )
			{
				if ( menu == NULL )
				{
					if ( m_hud ) 
					{
						if ( m_hud.m_quickbar_state )
						{
							m_hud.HideQuickbar();
						}
						else
						{
							m_hud.ShowQuickbar();
						}
					}
				}
			}
			
			if ( g_Game.GetInput().GetActionDown( UAZeroingUp, false) || g_Game.GetInput().GetActionDown( UAZeroingDown, false) || g_Game.GetInput().GetActionDown( UAToggleWeapons, false) )
			{
				m_hud.ZeroingKeyPress();
			}

			// action menu
			GetGame().UpdateCursorTarget();

			if (menu == NULL)
			{
				GetGame().UpdateActionMenuActions();
				m_actionMenu.SetEnabled(true);

				if (m_actionMenu.HasDefaultAction() && input.GetActionDown(UAAction, false, true))
				{
					m_actionMenu.PerformAction();
				}

				if (m_actionMenu.IsVisible())
				{
					if (input.GetActionDown(UANextAction, false, true))
					{
						m_actionMenu.NextAction();
					}

					if (input.GetActionDown(UAPrevAction, false, true))
					{
						m_actionMenu.PrevAction();
					}
				}
			}
			else
			{
				m_actionMenu.SetEnabled(false);
			}
		}

		// life state check
		if (player)
		{
			int life_state = player.GetLifeState();
	 
			// life state changed
			if (m_life_state != life_state)
			{
				m_life_state = life_state;

				if (m_life_state != LifeStateAlive)
				{
					DestroyAllMenus();
				}
			}
		}

		if (menu && !menu.UseKeyboard() && menu.UseMouse())
		{
			int i;
			for (i = 0; i < 5; i++)
			{
				input.DisableKey(i | INPUT_DEVICE_MOUSE);
				input.DisableKey(i | INPUT_ACTION_TYPE_DOWN_EVENT | INPUT_DEVICE_MOUSE);
				input.DisableKey(i | INPUT_ACTION_TYPE_DOUBLETAP | INPUT_DEVICE_MOUSE);
			}

			for (i = 0; i < 6; i++)
			{
				input.DisableKey(i | INPUT_DEVICE_MOUSE_AXIS);
			}
		}

		if ( GetGame().IsDebug() )
		{
			PluginDeveloper module_dev = (PluginDeveloper)GetPlugin(PluginDeveloper);
			if ( module_dev )
			{
				module_dev.UpdateSqfDebugWatchers();
			}
		}
		
		if (!manager.IsDialogVisible())
		{
			if (manager.GetMenu())
			{
				if (manager.GetMenu().GetID() == MENU_INGAME)
				{
					if(input.GetActionDown(UAUIBack, false, true) || input.GetActionDown(UAUIMenu, false, true))
					{
						Continue();
					}
				}
				else if (manager.GetMenu() == inventory)
				{
					if(input.GetActionDown(UAUIBack, false, true))
					{
						HideInventory();
					}
				}
				else if(input.GetActionDown(UAUIBack, false, true))
				{
					manager.Back();
				}
			}
			else if (input.GetActionDown(UAUIMenu, false, true))
			{
				Pause();
			}
		}
		
	}

	override void OnMouseButtonPress(int button)
	{
		super.OnMouseButtonPress(button);
		if (PluginKeyBinding.instance)
			PluginKeyBinding.instance.OnMouseButtonPress(button);
	}

	override void OnMouseButtonRelease(int button)
	{
		super.OnMouseButtonRelease(button);
		if (PluginKeyBinding.instance)
			PluginKeyBinding.instance.OnMouseButtonRelease(button);
	}

	override void OnKeyPress(int key)
	{
		super.OnKeyPress(key);
		m_hud.KeyPress(key);

		if (PluginKeyBinding.instance)
			PluginKeyBinding.instance.OnKeyPress(key);

		PlayerBase pb = (PlayerBase)GetGame().GetPlayer();

		if (key == KeyCode.KC_BACK) // backspace
		{
			if (pb)
			{
				if (pb.m_Cam == NULL){
					GetGame().ExecuteSQF("nul = [] spawn { DZ_FREE_CAM = \"Camera\" camCreate position player; DZ_FREE_CAM cameraEffect [\"internal\",\"back\"]; DZ_FREE_CAM camCommit 0; player callMethod ['CreateFreeCamInstance', DZ_FREE_CAM]; }; ");
				}else{
					GetGame().ExecuteSQF("DZ_FREE_CAM cameraEffect [\"terminate\",\"back\"]; camDestroy DZ_FREE_CAM;"); 
				}
			}
		}
	}

	void OnEvent(EventType eventTypeId, Param params)
	{
		super.OnEvent(eventTypeId, params);
		InventoryMenu menu;
		Man player = GetGame().GetPlayer();
		
		switch(eventTypeId)
		{
		case ChatMessageEventTypeID:
			ChatMessageEventParams chat_params = params;
			if (chat_params.param1 == CCSystem && GetProfileValueBool(SYSTEM_CHAT_MSG, true) == false) break;
			if (chat_params.param1 == CCGlobal && GetProfileValueBool(GLOBAL_CHAT_MSG, true) == false) break;
			if (chat_params.param1 == CCItemTransmitter && GetProfileValueBool(RADIO_CHAT_MSG, true) == false) break;
			if (chat_params.param1 == CCDirect && GetProfileValueBool(DIRECT_CHAT_MSG, true) == false) break;
			if (chat_params.param1 == CCVehicle && GetProfileValueBool(VEHICLE_CHAT_MSG, true) == false) break;

			if (m_life_state == LifeStateAlive)
			{
				m_chat.Add(chat_params);
			}
			break;

		case SQFConsoleEventTypeID:
			SQFConsoleEventParams sqf_console_params = params;
			if (sqf_console_params.param1)
			{
				//Pause();
				//Continue();
				g_Game.GetUIManager().ShowCursor(true);
			}
			else
			{
				//Continue();
				g_Game.GetUIManager().ShowCursor(false);
			}
			break;

		case ActionMenuChangedEventTypeID:
			m_actionMenu.UpdateActions();
			break;

		case InventoryActionsEventTypeID:
			InventoryActionsEventParams act_params = params;
			menu = g_Game.GetUIManager().FindMenu(MENU_INVENTORY);
			if (menu && act_params.param1 == player)
			{
				//menu.UpdateEntityActions(act_params.param2, act_params.param3, act_params.param4);
				InventoryManager manager = InventoryManager.GetInstance();
				manager.UpdateEntityActions(act_params.param2, act_params.param3, act_params.param4);
			}
			break;
		
		case ChatChannelEventTypeID:
			ChatChannelEventParams cc_params = params;
			ChatInputMenu chatMenu = GetGame().GetUIManager().FindMenu(MENU_CHAT_INPUT);
			if (chatMenu)
			{
				chatMenu.UpdateChannel();
			}
			else
			{
				m_chat_channel_text.SetText(ChatInputMenu.GetChannelName(cc_params.param1));
				m_chat_channel_fade_timer.FadeIn(m_chat_channel_area, 0.5, true);
				m_chat_channel_hide_timer.Run(2, m_chat_channel_fade_timer, "FadeOut", new Param3<Widget, float, bool>(m_chat_channel_area, 0.5, true));
			}
			break;
			
		case WindowsResizeEventTypeID:
			CloseAllMenus();
			DestroyAllMenus();
			break;	
			
			case VONStateEventTypeID:
				VONStateEventParams vonStateParams = params;
				if (vonStateParams.param1)
				{
					m_microphone_icon.Show(true);
				}
				else
				{
					m_microphone_icon.Show(false);
				}
				break;				
		}
	}

	void OnItemUsed(InventoryItem item, Man owner)
	{
		if (item && GetGame().GetUIManager().GetMenu() == NULL)
		{
			if (item.IsInherited(ItemBook))
			{
				BookMenu bookMenu = GetGame().GetUIManager().EnterScriptedMenu(MENU_BOOK, NULL);
				if (bookMenu)
				{
					bookMenu.ReadBook(item);
				}
			}
			else if (item.IsInherited(ItemMap))
			{
				GetGame().GetUIManager().EnterScriptedMenu(MENU_MAP, NULL);			
			}
		}
	}
	
	void CloseAllMenus()
	{
		HideInventory();
		GetGame().GetUIManager().CloseAll();
	}

	void DestroyAllMenus()
	{
		GetGame().GetUIManager().HideDialog();
		m_gear_activity.Ptr().DestroyInventory();
		GetGame().GetUIManager().CloseAll();
		m_chat.Clear();
	}
	
	void HideInventory()
	{
		InventoryMenu inventory = GetGame().GetUIManager().FindMenu(MENU_INVENTORY);
		if (inventory)
		{
			GetGame().GetUIManager().HideScriptedMenu(inventory);
		}
	}
	
	EntityScriptedActivity	CreateActivity(Object target, Man man, int activity_type)
	{
		if (activity_type == ATGear)
		{
			if (m_gear_activity.Ptr().m_target)
			{
				delete m_gear_activity.Ptr().m_target;
			}
			m_gear_activity.Ptr().m_target = new Link<EntityAI>(target);
			return m_gear_activity.Ptr();
		}

		return NULL;
	}

	Hud GetHud()
	{
		return m_hud;
	}

	HudDebug GetHudDebug()
	{
		return m_hud_debug;
	}

	void RefreshCrosshairVisibility()
	{
		GetHudDebug().RefreshCrosshairVisibility();
	}

	bool IsPaused()
	{
		return GetGame().GetUIManager().IsMenuOpen(MENU_INGAME);
	}

	void Pause()
	{
		if (IsPaused()) return;

		CloseAllMenus();

		// open ingame menu
		GetGame().GetUIManager().EnterScriptedMenu(MENU_INGAME, NULL);
	}

	void Continue()
	{
		if (!IsPaused()) return;

		GetGame().GetUIManager().CloseMenu(MENU_INGAME);
	}
	
	bool IsMissionGameplay()
	{
		return true;
	}
}

Camera m_freecam;
bool m_FreeCamStatus;
static void CreateFreeCamInstance()
{
	vector pos = GetGame().GetPlayer().GetPosition();
	pos[1] = pos[1] + 1.5;

	m_freecam = GetGame().CreateObject("VPPFreeCam", pos, false, false);
	m_freecam.SetManual(true);
	m_FreeCamStatus = true;
	Print("CreateFreeCamInstance() " + m_freecam);
}

static bool IsFreeCamActive()
{
	return m_FreeCamStatus;
}

static Camera GetFreeCamInstance()
{
	return m_freecam;
}

static void DestroyFreeCam()
{
	if (m_freecam == NULL) return;
	m_FreeCamStatus = false;
	
	GetGame().ObjectDelete(m_freecam);
	GetGame().SelectPlayer(NULL,GetGame().GetPlayer());
	
	GetGame().GetInput().ResetGameFocus();
	GetGame().GetUIManager().ShowCursor(false);
}