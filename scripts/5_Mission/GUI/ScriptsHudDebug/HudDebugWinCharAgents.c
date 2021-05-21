class HudDebugWinCharAgents extends HudDebugWinBase
{	
	protected TextListboxWidget	m_WgtAgents;

	//============================================
	// HudDebugWinCharAgents
	//============================================
	void HudDebugWinCharAgents( Widget widget_root )
	{	
		m_WgtRoot = widget_root;
		m_WgtAgents = ( TextListboxWidget ) m_WgtRoot.FindAnyWidget( "txl_CharAgents_Values" );
		
		FitWindowByContent( m_WgtAgents );
	}

	void ~HudDebugWinCharAgents()
	{	
		SetUpdate( false );
	}

	//============================================
	// GetWinType
	//============================================
	int GetType()
	{
		return HudDebug.HUD_WIN_CHAR_AGENTS;
	}
	
	//============================================
	// Update
	//============================================
	void SetUpdate( bool state )
	{
		//Disable update on server (PluginDeveloperSync)
		PlayerBase player = ( PlayerBase ) GetGame().GetPlayer();
		PluginDeveloperSync developer_sync = GetPlugin( PluginDeveloperSync );
		
		//if client, send RPC to deactivate
		if ( GetGame().IsClient() )
		{
			autoptr Param1<int> params = new Param1<int>( (int) state );
			if ( player )
			{
				player.RPCSingleParam( DEV_AGENTS_UPDATE, params );
			}
		}
		//else deactive directly
		else
		{
			if ( developer_sync )
			{
				developer_sync.EnableUpdate( state, DEV_AGENTS_UPDATE, player );	
			}
		}
	}

	void Update()
	{
		super.Update();
		
		//Print("Update()");
		
		//refresh notifiers
		SetAgents();
	}
	
	//============================================
	// Show / Hide
	//============================================
	void Show()
	{
		super.Show();
		
		//Print("Show()");
		
		SetUpdate( true );
	}

	void Hide()
	{
		super.Hide();
		
		//Print("Hide()");
		
		SetUpdate( false );
	}
	
	void SetAgents()
	{
		PluginDeveloperSync developer_sync = GetPlugin( PluginDeveloperSync );
		
		//clear window
		ClearAgents();
			
		//set agents	
		if ( GetDayZGame().IsNewPlayer() && developer_sync.m_PlayerAgentsSynced.Count() > 0 )
		{
			for ( int i = 0; i < developer_sync.m_PlayerAgentsSynced.Count(); i++ )
			{
				AddAgent( developer_sync.m_PlayerAgentsSynced.Get( i ).GetName(), developer_sync.m_PlayerAgentsSynced.Get( i ).GetValue().ToString() );
			}
		}
		
		//fit to screen
		FitWindow();
	}
	
	void AddAgent( string title, string value )
	{
		int index = m_WgtAgents.AddItem( title, NULL, 0 );
		m_WgtAgents.SetItem( index, value, NULL, 1 );
	}

	void ClearAgents()
	{
		m_WgtAgents.ClearItems();
	}

	void FitWindow()
	{
		FitWindowByContent( m_WgtAgents );
	}
}
