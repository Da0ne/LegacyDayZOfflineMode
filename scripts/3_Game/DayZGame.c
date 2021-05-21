class LoadingScreen
{
	Widget m_WidgetRoot;
	Widget m_FrameWidgetSmall;
	Widget m_FrameWidgetLarge;
	ProgressBarWidget m_ProgressBarSmall;
	ProgressBarWidget m_ProgressBarLarge;
	ProgressBarWidget m_ProgressBar;
	TextWidget m_TextWidgetTitle;
	ImageWidget m_ImageWidgetBackground;
	
	int m_Counter;
	
	void LoadingScreen(DayZGame game)
	{
		m_WidgetRoot = game.GetWorkspace().CreateWidgets("gui/layouts/loading.layout");
		m_FrameWidgetSmall = (Widget)m_WidgetRoot.FindAnyWidget("ProgressBarFrameSmall");
		m_FrameWidgetLarge = (Widget)m_WidgetRoot.FindAnyWidget("ProgressBarFrameLarge");
		m_ProgressBarSmall = (ProgressBarWidget)m_WidgetRoot.FindAnyWidget("ProgressBarWidgetSmall");
		m_ProgressBarLarge = (ProgressBarWidget)m_WidgetRoot.FindAnyWidget("ProgressBarWidgetLarge");
		m_TextWidgetTitle = (TextWidget)m_WidgetRoot.FindAnyWidget("TextWidget");
		m_ImageWidgetBackground = (ImageWidget)m_WidgetRoot.FindAnyWidget("ImageBackground");
	
		m_ProgressBar = m_ProgressBarLarge;
		m_FrameWidgetSmall.Show( false );
		m_ImageWidgetBackground.LoadImageFile( 0, GetRandomLoadingBackground() );
		m_ImageWidgetBackground.Show( false );
		m_ProgressBar.SetCurrent(0);
		m_Counter = 0;
	}
	
	void Inc()
	{
		m_Counter++;
	
		if ( m_Counter == 1 )
		{
			Show();
		}	
	}
	
	void Dec()
	{
		m_Counter = m_Counter - 1;
	
		if ( m_Counter <= 0 )
		{
			m_WidgetRoot.Show( false );
			m_Counter = 0;
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call( g_Game, "CheckDialogs" );
		}
	}
	
	bool IsLoading()
	{
		if ( m_WidgetRoot.IsVisible() )
		{
			return true;
		}
	
		return false;
	}

	void SetTitle( string title )
	{
		m_TextWidgetTitle.SetText( title );
	}
	
	void SetProgress( float val )
	{
		if ( m_Counter > 0 )
		{
			m_ProgressBar.SetCurrent( val * 100 );
		}
	}
	
	void OnUpdate( float timeslice )
	{
		
	}
	
	void Show()
	{
		if ( !m_WidgetRoot.IsVisible() )
		{
			if ( g_Game.GetUIManager().IsDialogVisible() )
			{
				g_Game.GetUIManager().HideDialog();
			}
			
			if ( g_Game.GetMissionState() == DayZGame.MISSION_STATE_MAINMENU )
			{
				m_ProgressBar = m_ProgressBarSmall;
				m_FrameWidgetSmall.Show( true );
				m_FrameWidgetLarge.Show( false );
				m_ImageWidgetBackground.Show( true );
			}
			else
			{
				m_ProgressBar = m_ProgressBarLarge;
				m_FrameWidgetSmall.Show( false );
				m_FrameWidgetLarge.Show( true );
				m_ImageWidgetBackground.Show( false );
			}
			
			m_WidgetRoot.Show( true );
			m_ProgressBar.SetCurrent( 0 );
			m_TextWidgetTitle.SetText( "" );
		}
	}
	
	void Hide()
	{
		if ( m_Counter <= 0 && m_WidgetRoot.IsVisible() )
		{
			m_WidgetRoot.Show( false );
		}
	}
};

class TileEffect
{
	private autoptr WidgetFadeTimer m_fade_timer;
	private Widget m_root_widget;
	private TextWidget m_titleWidget;
	private Widget m_backgroundWidget;

	// ------------------------------------------------------------
	void TileEffect(DayZGame game)
	{
		m_root_widget = game.GetWorkspace().CreateWidgets("gui/layouts/tileEffect.layout");
		m_titleWidget = (TextWidget)m_root_widget.FindAnyWidget("TextWidget");
		m_backgroundWidget = m_root_widget.FindAnyWidget("BackgroundWidget");
		m_root_widget.Show(false);
	}
	
	// ------------------------------------------------------------
	void ~TileEffect()
	{
	
	}
	
	// ------------------------------------------------------------
	void SetText(string text)
	{
		m_titleWidget.SetText(text);
	}
	
	// ------------------------------------------------------------
	void Show(int type, bool show, float duration, string text /*= ""*/)
	{
		if (!m_fade_timer) m_fade_timer = new WidgetFadeTimer;
		if (duration > 3.0) duration = 3.0;
		
		float fadedTime = 5.0;
	
		if (m_fade_timer.IsRunning() && m_fade_timer.m_fadeIn)
		{
			 fadedTime = m_fade_timer.GetRemaining(); 
		}
		
		m_fade_timer.Stop();
		m_titleWidget.SetText(text);
	
		if (!show)
		{
			m_root_widget.Show(false);
		}
		else
		{
			if (type == TitBlackIn)
			{
				m_backgroundWidget.Show(true);
				m_fade_timer.FadeOut(m_root_widget, duration, true);
			}
			else if (type == TitBlackFaded)
			{
				m_backgroundWidget.Show(true);
				if (fadedTime > 0) m_fade_timer.FadeIn(m_root_widget, fadedTime, true);
			}
			else if (type == TitBlackOut)
			{
				m_backgroundWidget.Show(true);
				m_fade_timer.FadeIn(m_root_widget, duration, true);
			}
			else 
			{
				m_backgroundWidget.Show(false);
				m_fade_timer.FadeOut(m_root_widget, duration, false);
			}
		}
	}
	
	// ------------------------------------------------------------
	bool IsVisible()
	{
		return m_root_widget.IsVisible();
	}
};

class DayZGame extends CGame
{
	int m_OfflineMissions = 0;
	bool m_OfflineCELoaded = false;
	//---

	const int MISSION_STATE_MAINMENU = 0;
	const int MISSION_STATE_GAME = 1;
	const int MISSION_STATE_FINNISH = 2;
	
	private int m_MissionState;
	
	private autoptr LoadingScreen m_loading;
	private autoptr TileEffect m_tileEffect;
	private CallQueue m_callQueue[CALL_CATEGORY_COUNT];
	private TimerQueue m_timerQueue[CALL_CATEGORY_COUNT];
	private autoptr DragQueue m_dragQueue;
	private bool m_early_access_dialog_accepted;
	private UIScriptedMenu m_keyboard_handler;
	private ScriptModule m_mission_module;
	private string m_mission_path;
	private bool	m_IsCtrlHolding;
	private bool	m_IsWinHolding;
	private bool 	m_IsLeftAltHolding;
	private bool 	m_IsRightAltHolding;
	
	// CGame override functions
	void DayZGame()
	{
		m_MissionState = MISSION_STATE_GAME;
		
		m_keyboard_handler = NULL;
		m_early_access_dialog_accepted = IsDebug();
		
		for (int i = 0; i < CALL_CATEGORY_COUNT; i++)
		{
			m_callQueue[i] = new CallQueue;
			m_timerQueue[i] = new TimerQueue;
		}
	
		m_dragQueue = new DragQueue;
	
		m_ParamUserActions	= false;
		m_ParamNewPlayer	= false;
		m_ParamNewInventory	= false;
		m_ParamNoLogs		= false;
		m_DebugActions		= false;
		m_MainMenu2			= false;
		
		string tmp;
		if ( IsDebug() )
		{
			if ( CommandlineGetParam("enUserActions", tmp) )
			{
				m_ParamUserActions = true;
			}		
			
			if ( CommandlineGetParam("enDebugActions", tmp) )
			{
				m_DebugActions = true;
			}
			
			if ( CommandlineGetParam("enNewPlayer", tmp) )
			{
				m_ParamNewPlayer = true;
			}

			if ( CommandlineGetParam("enNewInventory", tmp) )
			{
				m_ParamNewInventory = true;
			}
			
			if ( CommandlineGetParam("enNoLogs", tmp) )
			{
				m_ParamNoLogs = true;
			}
			
			if ( CommandlineGetParam("enMainMenu2", tmp) )
			{
				m_MainMenu2 = true;
			}
		}
	
	#ifndef NO_GUI	
		m_loading = new LoadingScreen(this);
		m_loading.Show();
	
		m_tileEffect = new TileEffect(this);
	#endif
	
		Debug.Init();
		Component.Init();
		LogTemplates.Init();
		
		InitializeSingletons();
	}

	string GetRandomPlayerChar()
	{
		string path = "cfgVehicles";
		string child_name = "";
		int count = ConfigGetChildrenCount(path);
		array<string> classNames = new array<string>();
		for (int p = 0; p < count; ++p)
		{
			ConfigGetChildName( path, p, child_name );
			
			if (ConfigGetInt(path + " " + child_name + " scope") == 2 && IsKindOf(child_name, "SurvivorBase"))
				classNames.Insert(child_name);
		}
		return classNames.GetRandomElement();
	}
	
	// ------------------------------------------------------------
	void ~DayZGame()
	{
		SetDispatcher(NULL);
		
		for (int i = 0; i < CALL_CATEGORY_COUNT; i++)
		{
			delete m_callQueue[i];
			delete m_timerQueue[i];
		}
		
		Debug.OnDestroy();
		//DebugPrint.OnDestroy();
		LogTemplates.OnDestroy();
		
		DestroySingletons();
		
		Print("~DayZGame()");
	}
	
	void SetMissionPath(string path)
	{
		m_mission_path = path;
	}

	CallQueue GetCallQueue(int call_category)
	{
		return m_callQueue[call_category];
	}

	TimerQueue GetTimerQueue(int call_category)
	{
		return m_timerQueue[call_category];
	}
	
	DragQueue GetDragQueue()
	{
		return m_dragQueue;
	}

	// ------------------------------------------------------------
	void InitializeSingletons()
	{
		m_CashedObjectsParams = new CashedObjectsParams;
		m_CashedObjectsArrays = new CashedObjectsArrays;
	}
	
	// ------------------------------------------------------------
	void DestroySingletons()
	{
	
		delete m_CashedObjectsParams;
		delete m_CashedObjectsArrays;
	}
	// ------------------------------------------------------------
	int GetMissionState()
	{
		return m_MissionState;
	}
	
	// ------------------------------------------------------------
	void SetMissionState( int state )
	{
		m_MissionState = state;
	}
	
	// ------------------------------------------------------------
	bool IsInventoryOpen()
	{
		if (GetUIManager().FindMenu(MENU_INVENTORY) != NULL)
		{
			return true;
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	void EarlyAccessDialog(UIScriptedMenu parent)
	{
		if (!m_early_access_dialog_accepted)
		{
			g_Game.GetUIManager().EnterScriptedMenu(MENU_EARLYACCESS, parent);
			m_early_access_dialog_accepted = true;
		}
	}
	
	// ------------------------------------------------------------
	//! create custom main menu part (submenu)
	UIScriptedMenu CreateScriptedMenu(int id)
	{
	#ifndef NO_GUI
		Mission mission = GetMission();
		if (mission)
		{
			return mission.CreateScriptedMenu(id);
		}
	#endif
		return NULL;
	}
	
	EntityScriptedActivity CreateActivity(Object target, Man man, int activity_type)
	{
	#ifndef NO_GUI_INGAME
		
		Mission mission = GetMission();
		if (mission)
		{
			return mission.CreateActivity(target, man, activity_type);
		}
	
	#endif
	
		return NULL;
	}
	
	// ------------------------------------------------------------
	void ReloadMission()
	{
	#ifdef DEVELOPER
		Print("Reloading mission module!");
		CreateMission(m_mission_path);
	#endif
	}
	
	// ------------------------------------------------------------
	void OnEvent(EventType eventTypeId, Param params)
	{
		switch(eventTypeId)
		{
		case StartupEventTypeID:
			break;
		
		case MPSessionStartEventTypeID:
			LoadingShow();
			break;
		
		case MPSessionEndEventTypeID:
			break;
		
		case MPSessionFailEventTypeID:
			LoadingHide();
			break;
		
		case WorldCleaupEventTypeID:
			LoadingShow();
			break;
		
		case DialogQueuedEventTypeID:
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(g_Game, "CheckDialogs");
			break;
	
		case ChatMessageEventTypeID:
			ChatMessageEventParams chat_params = params;
			
			#ifndef NO_GUI
			if (m_tileEffect.IsVisible() && (chat_params.param1 == CCSystem || chat_params.param1 == CCStatus))
			{
				m_tileEffect.SetText(chat_params.param3);
			}
			#endif
			break;
	
		case ProgressEventTypeID:
			ProgressEventParams prog_params = params;
			LoadProgressUpdate(prog_params.param1, prog_params.param2, prog_params.param3);
			break;
	
		case EntityNetworkUpdateEventTypeID:
			EntityNetworkUpdateEventParams ent_params = params;
			// update entity info
			break;
	
		case TileEffectEventTypeID:
			TileEffectEventParams tile_params = params;
	#ifndef NO_GUI
			m_tileEffect.Show(tile_params.param1, tile_params.param2, tile_params.param3, tile_params.param4);
	#endif		
			break;
			
		case ClientNewEventTypeID:
			Print("DayZGame::OnEvent::NEW event");
			break;
			
		case ClientRespawnEventTypeID:
			Print("DayZGame::OnEvent::RESPAWN event");
			break;
			
		case ClientReadyEventTypeID:
			Print("DayZGame::OnEvent::READY event");
			break;
		
		case ClientDisconnectedEventTypeID:
			Print("DayZGame::OnEvent::DISC event");
			break;
			
		case ClientQueuedEventTypeID:
			Print("DayZGame::OnEvent::Queued event");
			ClientQueuedEventParams newParams = params;
			// call after waiting is finished
			GetGame().SetPlayerWaited();
			GetGame().SetEVUser( 0 );
			break;
		}
	
		Mission mission = GetMission();
		if (mission)
		{
			mission.OnEvent(eventTypeId, params);
		}
	}
	
	// ------------------------------------------------------------
	void LoadProgressUpdate(int progressState, float progress, string title)
	{
	#ifndef NO_GUI
		switch(progressState)
		{
		case PROGRESS_START:
			{
				m_tileEffect.Show(0, false, 0, "");
				m_loading.Inc();
				m_loading.SetTitle(title);
			}
			break;
	
		case PROGRESS_FINISH:
			{
				m_loading.Dec();
			}
			break;
	
		case PROGRESS_PROGRESS:
			{
				m_loading.SetProgress(progress);
			}
			break;
	
		case PROGRESS_UPDATE:
			{
				m_loading.SetProgress(0);
			}
			break;
		}
	#endif
	}
	
	// ------------------------------------------------------------
	void OnAfterCreate()
	{
		Math.Randomize(-1);
	}
	
	bool IsLeftCtrlDown()
	{
		return m_IsCtrlHolding;
	}
	
	// ------------------------------------------------------------
	void OnKeyPress(int key)
	{
	
		if ( key == KeyCode.KC_LCONTROL ) 
		{
			m_IsCtrlHolding = true;
		}

		if (key == KeyCode.KC_LMENU)
		{
			m_IsLeftAltHolding = true;
		}
				
		if (key == KeyCode.KC_RMENU)
		{
			m_IsRightAltHolding = true;
		}

		if (m_keyboard_handler)
		{
			m_keyboard_handler.OnKeyDown(NULL, 0, 0, key);
		}
		
		Mission mission = GetMission();
		if (mission)
		{
			mission.OnKeyPress(key);
		}
	
#ifdef DEVELOPER

		if ((m_IsLeftAltHolding || m_IsRightAltHolding) && key == KeyCode.KC_F4)
		{
			RequestExit(0);
		}
		
#endif
		
	}
	
	// ------------------------------------------------------------
	void OnKeyRelease(int key)
	{
		if ( key == KeyCode.KC_LCONTROL ) 
		{
			m_IsCtrlHolding = false;
		}
		
		if ( key == KeyCode.KC_LWIN ) 
		{
			m_IsWinHolding = false;
		}

		if (key == KeyCode.KC_LMENU)
		{
			m_IsLeftAltHolding = false;
		}
				
		if (key == KeyCode.KC_RMENU)
		{
			m_IsRightAltHolding = false;
		}

		if (m_keyboard_handler)
		{
			m_keyboard_handler.OnKeyUp(NULL, 0, 0, key);
		}
	
		Mission mission = GetMission();
		if (mission)
		{
			mission.OnKeyRelease(key);
		}
	}
	
	// ------------------------------------------------------------	
	void OnMouseButtonPress(int button)
	{
		Mission mission = GetMission();
		if (mission)
		{
			mission.OnMouseButtonPress(button);
		}
	}
	
	// ------------------------------------------------------------
	void OnMouseButtonRelease(int button)
	{
		Mission mission = GetMission();
		if (mission)
		{
			mission.OnMouseButtonRelease(button);
		}
	}
	
	// ------------------------------------------------------------
	void OnUpdate(bool doSim, float timeslice)
	{
		Mission mission = GetMission();
		bool gameIsRunning = false;
	
		if (doSim && mission && !mission.IsPaused())
		{
			gameIsRunning = true;
		}
	
		if (doSim && mission)
		{
			mission.OnUpdate(timeslice);
		}
	
		// queues and timers update
		GetCallQueue(CALL_CATEGORY_SYSTEM).Tick();
		GetTimerQueue(CALL_CATEGORY_SYSTEM).Tick(timeslice);
	
	#ifndef NO_GUI	
		if (m_loading && m_loading.IsLoading())
		{
			m_loading.OnUpdate(timeslice);
		}
		else
		{
			GetCallQueue(CALL_CATEGORY_GUI).Tick();
			GetTimerQueue(CALL_CATEGORY_GUI).Tick(timeslice);
			GetDragQueue().Tick();
		}
	#endif
	
		if (gameIsRunning)
		{
			GetCallQueue(CALL_CATEGORY_GAMEPLAY).Tick();
			GetTimerQueue(CALL_CATEGORY_GAMEPLAY).Tick(timeslice);
		}
	}
	
	// ------------------------------------------------------------
	void OnRPC(Object target, int rpc_type, ParamsReadContext ctx)
	{
		//Print("DayZGame::OnRPC");
		if (target)
		{
			// call rpc on target
			target.OnRPC(rpc_type, ctx);
		}
		else
		{
			// global rpc's handling
		}
	}
	
	// ------------------------------------------------------------
	void CheckDialogs()
	{
		#ifndef NO_GUI	
		Mission mission = GetMission();
		if (mission && !m_loading.IsLoading() && GetUIManager().IsDialogQueued())
		{
			mission.Pause();
			GetUIManager().ShowQueuedDialog();
		}
		#endif
	}
	
	// ------------------------------------------------------------
	bool IsLoading()
	{
		if (m_loading && m_loading.IsLoading())
		{
			return true;
		}
		
		return false;
	}
	
	// ------------------------------------------------------------
	void SetKeyboardHandle(UIScriptedMenu handler)
	{
		m_keyboard_handler = handler;
	}
	
	// ------------------------------------------------------------
	void LoadingShow()
	{
		#ifndef NO_GUI	
			m_loading.Show();
		#endif
	}
	
	void LoadingHide()
	{
		#ifndef NO_GUI	
			m_loading.Hide();
		#endif
	}	

	CashedObjectsParams m_CashedObjectsParams;
	CashedObjectsArrays m_CashedObjectsArrays;
};

DayZGame g_Game;

DayZGame GetDayZGame()
{
	return g_Game;
}
