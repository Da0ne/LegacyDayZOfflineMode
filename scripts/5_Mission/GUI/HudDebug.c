// #include "Scripts\GUI\ScriptsHudDebug\_include.c"

class HudDebug extends Hud
{
	static const int HUD_WIN_UNDEFINED			= 0;
	static const int HUD_WIN_CHAR_STATS			= 1;
	static const int HUD_WIN_CHAR_MODIFIERS		= 2;
	static const int HUD_WIN_CHAR_AGENTS		= 3;
	static const int HUD_WIN_CHAR_DEBUG			= 4;
	
	Widget					m_WgtRoot;
	Widget					m_Crosshair;
	array<HudDebugWinBase>	m_Panels;
	Timer					m_TimerUpdate;
	
	//============================================
	// HudDebug
	//============================================
	void HudDebug()
	{
	}

	//============================================
	// ~HudDebug
	//============================================
	void ~HudDebug()
	{
		m_WgtRoot.Destroy();
		m_TimerUpdate.Stop();
		delete m_TimerUpdate;
		
		for ( int i = 0; i < m_Panels.Count(); ++i )
		{
			delete m_Panels.Get(i);
		}
		
		delete m_Panels;
	}
	
	//============================================
	// Init
	//============================================
	void Init(Widget hud_panel_widget)
	{
		m_WgtRoot = hud_panel_widget;
		m_WgtRoot.Show( true );
		
		// Crosshair widget root
		m_Crosshair = m_WgtRoot.FindAnyWidget("wdw_Crosshair");
		
		m_Panels = new array<HudDebugWinBase>;
		
		// Register Window Character Stats
		HudDebugWinCharStats win_char_stats = new HudDebugWinCharStats( m_WgtRoot.FindAnyWidget("wdw_CharacterStats") );
		m_Panels.Insert( win_char_stats );
		
		// register Window Chracter Modifiers
		HudDebugWinCharModifiers win_char_modifiers = new HudDebugWinCharModifiers( m_WgtRoot.FindAnyWidget("wdw_CharacterModifiers") );
		m_Panels.Insert( win_char_modifiers );
		
		// register Window Chracter Agents
		HudDebugWinCharAgents win_char_agents = new HudDebugWinCharAgents( m_WgtRoot.FindAnyWidget("wdw_CharacterAgents") );
		m_Panels.Insert( win_char_agents );
		
		// register Window Chracter Debug
		HudDebugWinCharDebug win_char_debug = new HudDebugWinCharDebug( m_WgtRoot.FindAnyWidget("wdw_CharacterDebug") );
		m_Panels.Insert( win_char_debug );
		
		RefreshByLocalProfile();
		RefreshCrosshairVisibility();
		
		m_TimerUpdate = new Timer();
		m_TimerUpdate.Run(0.3, this, "Update", NULL, true);
	}
	
	//============================================
	// Update
	//============================================
	void Update()
	{
		for ( int i = 0; i < m_Panels.Count(); ++i )
		{
			if ( m_Panels.Get(i).IsVisible() )
			{
				m_Panels.Get(i).Update();
			}
		}
	}

	//============================================
	// SetPanetVisible
	//============================================
	void SetPanelVisible(int panel_type, bool visible)
	{
		if ( visible )
		{
			PanelShow(panel_type);
		}
		else
		{
			PanelHide(panel_type);
		}
	}
	
	//============================================
	// PanelShow
	//============================================
	void PanelShow(int panel_type)
	{
		for ( int i = 0; i < m_Panels.Count(); ++i )
		{
			HudDebugWinBase panel = m_Panels.Get(i);
			
			if ( panel.GetType() == panel_type )
			{
				panel.Show();
			}
		}
	}

	//============================================
	// PanelHide
	//============================================
	void PanelHide(int panel_type)
	{
		for ( int i = 0; i < m_Panels.Count(); ++i )
		{
			HudDebugWinBase panel = m_Panels.Get(i);
			
			if ( panel.GetType() == panel_type )
			{
				panel.Hide();
			}
		}
	}

	//============================================
	// RefreshCrosshairVisibility
	//============================================
	void RefreshCrosshairVisibility()
	{
		PluginConfigDebugProfile module_cfg_profile = (PluginConfigDebugProfile)GetPlugin(PluginConfigDebugProfile);
		
		if ( module_cfg_profile )
		{
			PluginDeveloper modul_dev = (PluginDeveloper)GetPlugin(PluginDeveloper);

			if ( modul_dev.IsEnabledFreeCamera() )
			{
				m_Crosshair.Show( module_cfg_profile.GetFreeCameraCrosshairVisible() );
			}
			else
			{
				m_Crosshair.Show( false );
			}
		}
	}

	//============================================
	// RefreshByLocalProfile
	//============================================
	void RefreshByLocalProfile()
	{
		PluginConfigDebugProfile module_cfg_profile = (PluginConfigDebugProfile)GetPlugin(PluginConfigDebugProfile);
		
		if ( module_cfg_profile )
		{
			SetPanelVisible(HudDebug.HUD_WIN_CHAR_STATS, 		module_cfg_profile.GetCharacterStatsVisible());
			SetPanelVisible(HudDebug.HUD_WIN_CHAR_MODIFIERS, 	module_cfg_profile.GetCharacterModifiersVisible());
			SetPanelVisible(HudDebug.HUD_WIN_CHAR_AGENTS,	 	module_cfg_profile.GetCharacterAgentsVisible());
			SetPanelVisible(HudDebug.HUD_WIN_CHAR_DEBUG, 		module_cfg_profile.GetCharacterDebugVisible());
		}
	}
	
	//============================================
	// IsInitialized
	//============================================
	bool IsInitialized()
	{
		if ( m_WgtRoot == NULL )
		{
			return false;
		}
		
		return false;
	}	
}
