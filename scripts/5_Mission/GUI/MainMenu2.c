class MainMenu2 extends UIScriptedMenu
{
	private autoptr WidgetFadeTimer m_fade_timer;
	private autoptr PlayerNameHandler m_player_name_handler;
	private DayZIntroScene m_scene;

	private TextListboxWidget m_GlobalStatsTextListBox;
	private autoptr Timer m_statisticsUpdateTimer;
	private Widget m_connection_warning_widget;
	private bool m_statisticsLoadedSuccessfully;

	void MainMenu2()
	{
		m_fade_timer = new WidgetFadeTimer;
		m_statisticsUpdateTimer = new Timer(CALL_CATEGORY_GUI);
		m_statisticsLoadedSuccessfully = false;
	}

	void ~MainMenu2()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).RemoveCalls(this);
	}
	
	Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_MainMenu2.layout");
		
		string version;
		string player_name;
		g_Game.GetVersion(version);
		g_Game.GetPlayerName(player_name);

		TextWidget version_widget = layoutRoot.FindAnyWidget("VersionText");
		version_widget.SetText("#main_menu_version " + version);

		EditBoxWidget name_widget = (EditBoxWidget)layoutRoot.FindAnyWidget("CharacterNameText");
		name_widget.SetText(player_name);

		m_GlobalStatsTextListBox = (TextListboxWidget)layoutRoot.FindAnyWidget("GlobalStatisticsTextListboxWidget");

		m_player_name_handler = new PlayerNameHandler;
		name_widget.SetHandler(m_player_name_handler);

		MissionMainMenu mission = g_Game.GetMission();
		m_scene = mission.GetIntroScene();

		// NotificationMessage m_notification_widget = new NotificationMessage( layoutRoot ); 

		return layoutRoot;
	}
	
	void OnShow()
	{
		g_Game.EarlyAccessDialog(this);

		if (!m_statisticsLoadedSuccessfully && m_statisticsUpdateTimer.IsRunning() == false) UpdateStats(); // hotfix: on steamversion calling of StatInvokeUpdate freeze the game for 15+ seconds everytime player enters main menu
	}
	
	void OnHide()
	{
	}
	
	bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick( w, x, y, button);
		
		switch (w.GetUserID())
		{
		case IDC_MAIN_NEXT_CHARACTER:
			m_scene.ChangeCharacter(m_scene.NextCharacterID());
			return true;
		case IDC_MAIN_PREV_CHARACTER:
			m_scene.ChangeCharacter(m_scene.PrevCharacterID());
			return true;
		case IDC_MAIN_PLAY:
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(this, "ConnectLastSession");
			return true;
			
		case IDC_MAIN_CHARACTER:
			if (m_scene)
			{
				EnterScriptedMenu(MENU_CHARACTER);
			}
			return true;
		
		case IDC_MAIN_MULTIPLAYER: 
			g_Game.GetUIManager().EnterServerBrowser(this);
			return true;
			break;

		case IDC_MAIN_OPTIONS:
			EnterScriptedMenu(MENU_OPTIONS);
			return true;

		case IDC_MAIN_QUIT:
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(g_Game, "RequestExit", new Param1<int>(IDC_MAIN_QUIT));
			break;
		}

		return false;
	}
	
	bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		super.OnMouseButtonDown(w, x, y, button);
		
		if (w.GetName() == "CharacterRotationFrame")
		{
			if (m_scene) m_scene.CharacterRotationStart();
			return true;
		}

		return false;
	}

	bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		super.OnMouseButtonUp(w, x, y, button);
		
		if (m_scene) m_scene.CharacterRotationStop();
		return true;
	}
	
	void FadeIn(float time)
	{
		//m_fade_timer.FadeIn(layoutRoot, time);
	}
	
	void ConnectLastSession()
	{
		if (g_Game.ConnectLastSession(this) == 0)
		{
			g_Game.GetUIManager().EnterServerBrowser(this);
		}
	}

	void UpdateStats()
	{
		m_GlobalStatsTextListBox.ClearItems();

		// NEW STATS API
		if ( m_scene && m_scene.m_demoUnit )
		{
			string lvalue = "";
			string lname = "";
			
			lname = "Play time";
			m_scene.m_demoUnit.StatGetAsTime("playtime", lvalue);
			int line_index = m_GlobalStatsTextListBox.AddItem(lname, NULL, 0);
			m_GlobalStatsTextListBox.SetItem(line_index, lvalue, NULL, 1);
			
			string debug_string = "";
			float fvalue = m_scene.m_demoUnit.StatGet("playtime");
			debug_string = "playtime: " + fvalue.ToString();
			//Print(debug_string);
			
			lname = "Distance traveled";
			m_scene.m_demoUnit.StatGetCounter("dist", lvalue);
			line_index = m_GlobalStatsTextListBox.AddItem(lname, NULL, 0);
			m_GlobalStatsTextListBox.SetItem(line_index, lvalue, NULL, 1);
			
			debug_string = "dist: " + lvalue;
			//Print(debug_string);
			
			m_statisticsLoadedSuccessfully = true;
		}
	}
}
