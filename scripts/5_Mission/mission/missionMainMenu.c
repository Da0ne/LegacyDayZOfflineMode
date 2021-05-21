class MissionMainMenu extends MissionBase
{
	private UIScriptedMenu m_mainmenu;
	private DayZIntroScene m_intro_scene;
	bool m_NoCutscene;

	DayZIntroScene GetIntroScene()
	{
		return m_intro_scene;
	}

	void MissionMainMenu()
	{
	}

	void OnInit()
	{
		if (!m_NoCutscene)
		{
			CreateIntroScene();
		}

		if (!m_mainmenu)
		{
			if ( GetDayZGame().IsMainMenu2() )
			{
				m_mainmenu = (MainMenu2)g_Game.GetUIManager().EnterScriptedMenu(MENU_MAIN, NULL);
			}
			else
			{
				m_mainmenu = (MainMenu)g_Game.GetUIManager().EnterScriptedMenu(MENU_MAIN, NULL);
			}
		}
	}

	void CreateIntroScene()
	{
		DestroyIntroScene();
		m_intro_scene = new DayZIntroScene;
		//m_intro_scene.CreateDefaultCharacter();
	}

	void DestroyIntroScene()
	{
		if (m_intro_scene) delete m_intro_scene;
		m_intro_scene = NULL;
	}
	
	void OnMissionStart()
	{
		if (m_mainmenu)
		{
			//m_mainmenu.FadeIn(2.0);	//Fade in method is currently commented in MainMenu class
		}
#ifndef PLATFORM_XBOX
		g_Game.GetUIManager().ShowCursor(true);
#endif
		g_Game.SetMissionState( DayZGame.MISSION_STATE_MAINMENU );
	}
	
	void OnMissionFinish()
	{
		m_mainmenu.Cleanup();
		GetGame().GetUIManager().CloseAll();
		m_mainmenu = NULL;
		
		DestroyIntroScene();
		g_Game.GetUIManager().ShowCursor(false);
	}

	void OnUpdate(float timeslice)
	{
		if (g_Game.IsLoading()) return; 
		
		if (m_intro_scene)
		{
			m_intro_scene.Update();
		}

		if( m_mainmenu )
		{
			m_mainmenu.Update(timeslice);
		}

		if (GetGame().GetInput().GetActionDown(UAUIBack, false, true))
		{
			if (GetGame().GetUIManager().GetMenu() != m_mainmenu)
			{
				GetGame().GetUIManager().Back();
			}		
		}
		
		if (GetGame().GetUIManager().GetMenu() == m_mainmenu)
		{
			if ((!g_Game.GetUIManager().GetLoginQueueDialog()) && (g_Game.GetUIManager().GetLoginQueuePosition() > 0))
			{
				g_Game.GetUIManager().SetLoginQueueDialog(true);
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(g_Game.GetUIManager(), "EnterLoginQueueScript", new Param1<UIScriptedMenu>(m_mainmenu));
			}
		}
	}
}
