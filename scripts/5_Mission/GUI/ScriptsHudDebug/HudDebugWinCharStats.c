class HudDebugWinCharStats extends HudDebugWinBase
{
	TextListboxWidget				m_WgtValues;
	
	//============================================
	// HudDebugWinCharStats
	//============================================
	void HudDebugWinCharStats(Widget widget_root)
	{	
		m_WgtValues = (TextListboxWidget)widget_root.FindAnyWidget("txl_CharStats_Values");
		
		FitWindow();
	}

	void ~HudDebugWinCharStats()
	{	
		SetUpdate( false );
	}


	//============================================
	// GetWinType
	//============================================
	int GetType()
	{
		return HudDebug.HUD_WIN_CHAR_STATS;
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
				player.RPCSingleParam( DEV_STATS_UPDATE, params );
			}
		}
		//else deactive directly
		else
		{
			if ( developer_sync )
			{
				developer_sync.EnableUpdate( state, DEV_STATS_UPDATE, player );	
			}
		}
	}


	void Update()
	{
		super.Update();
		
		//Print("Update()");
		
		//refresh notifiers
		SetValues();
	}
	
	void SetValues()
	{
		PluginDeveloperSync developer_sync = GetPlugin( PluginDeveloperSync );
		
		//clear window
		ClearValues();
			
		if ( GetDayZGame().IsNewPlayer() && developer_sync.m_PlayerStatsSynced.Count() > 0 )
		{
			//set
			for ( int i = 0; i < developer_sync.m_PlayerStatsSynced.Count(); i++ )
			{
				AddValue( developer_sync.m_PlayerStatsSynced.Get( i ).GetName(), developer_sync.m_PlayerStatsSynced.Get( i ).GetValue().ToString() );
			}
		}
		
		//fit to screen
		FitWindow();		
	}

	void AddValue( string title, string value )
	{
		int index = m_WgtValues.AddItem( title, NULL, 0 );
		m_WgtValues.SetItem( index, value, NULL, 1 );
	}

	void ClearValues()
	{
		m_WgtValues.ClearItems();
	}

	void FitWindow()
	{
		FitWindowByContent( m_WgtValues );
	}
}
