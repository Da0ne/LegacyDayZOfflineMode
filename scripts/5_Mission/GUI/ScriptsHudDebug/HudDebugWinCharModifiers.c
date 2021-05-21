class HudDebugWinCharModifiers extends HudDebugWinBase
{	
	protected TextListboxWidget	m_WgtModifiers;

	//============================================
	// HudDebugWinCharModifiers
	//============================================
	void HudDebugWinCharModifiers( Widget widget_root )
	{	
		m_WgtRoot = widget_root;
		m_WgtModifiers = ( TextListboxWidget ) m_WgtRoot.FindAnyWidget( "txl_CharModifiers_Values" );
		
		FitWindowByContent( m_WgtModifiers );
	}

	void ~HudDebugWinCharModifiers()
	{	
		SetUpdate( false );
	}

	//============================================
	// GetWinType
	//============================================
	int GetType()
	{
		return HudDebug.HUD_WIN_CHAR_MODIFIERS;
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
				player.RPCSingleParam( DEV_MODS_UPDATE, params );
			}
		}
		//else deactive directly
		else
		{
			if ( developer_sync )
			{
				developer_sync.EnableUpdate( state, DEV_MODS_UPDATE, player );	
			}
		}
	}

	void Update()
	{
		super.Update();
		
		//Print("Update()");
		
		//refresh notifiers
		SetModifiers();
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
	
	void SetModifiers()
	{
		PluginDeveloperSync developer_sync = GetPlugin( PluginDeveloperSync );
		
		//clear window
		ClearModifiers();
		
		if ( GetDayZGame().IsNewPlayer() && developer_sync.m_PlayerModsSynced.Count() > 0 )
		{
			//get modifiers data
			autoptr array<string> active_mods = new array<string>;
			FilterActiveModifiers( developer_sync.m_PlayerModsSynced, active_mods );
			autoptr array<string> inactive_mods = new array<string>;
			FilterInactiveModifiers( developer_sync.m_PlayerModsSynced, inactive_mods );
			
			//set active mods
			for ( int i = 0; i < active_mods.Count(); i++ )
			{
				AddModifier( active_mods.Get( i ), "ACTIVE" );
			}
			
			//add empty line (separator)
			if ( active_mods.Count() > 0 )
			{
				AddModifier( "", "" );
			}

			//set inactive mods
			for ( int j = 0; j < inactive_mods.Count(); j++ )
			{
				AddModifier( inactive_mods.Get( j ), "INACTIVE" );
			}
		}
		
		//fit to screen
		FitWindow();		
	}
	
	void AddModifier( string title, string value )
	{
		int index = m_WgtModifiers.AddItem( title, NULL, 0 );
		m_WgtModifiers.SetItem( index, value, NULL, 1 );
	}

	void ClearModifiers()
	{
		m_WgtModifiers.ClearItems();
	}

	void FitWindow()
	{
		FitWindowByContent( m_WgtModifiers );
	}
	
	//filter modifiers data
	void FilterActiveModifiers( array<SyncedValue> in_data, array<string> out_data )
	{
		for ( int i = 0; i < in_data.Count(); i++ )
		{
			if ( in_data.Get( i ).GetValue() > 0 )
			{
				out_data.Insert( in_data.Get( i ).GetName() );
			}
		}
	}

	void FilterInactiveModifiers( array<SyncedValue> in_data, array<string> out_data )
	{
		for ( int i = 0; i < in_data.Count(); i++ )
		{
			if ( in_data.Get( i ).GetValue() < 1 )
			{
				out_data.Insert( in_data.Get( i ).GetName() );
			}
		}
	}
}
