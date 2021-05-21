class UIPopupScriptSQFWatchers extends UIPopupScript
{
	private ButtonWidget m_BtnOk;
	private ButtonWidget m_BtnCancel;
	private ButtonWidget m_AddSqfDebugWatcherButton;
	private ButtonWidget m_RemoveSqfDebugWatcherButton;
	
	autoptr map<int, Widget> m_SqfDebugWatchersRows;
	autoptr map<int, EditBoxWidget> m_SqfDebugWatchers;
	autoptr map<int, EditBoxWidget> m_SqfDebugWatchersResults;
	autoptr map<int, ButtonWidget> m_SqfDebugWatchersButtons;
	autoptr map<int, ButtonWidget> m_SqfDebugWatchersClearButtons;
	autoptr TStringArray m_SqfConsoleHistory;
	
	Widget m_SqfDebugWatchersPanel;
	
	MultilineEditBoxWidget m_SQFEditBox;
	Widget m_SQFEditBoxBackground;
	Widget m_SQFEditBoxEdge;
	
	PluginLocalSqfHistory m_ModuleLocalSqfHistory;
	
	int m_SqfHistoryRow;
	
	void UIPopupScriptSQFWatchers( Widget wgt )
	{
		m_BtnOk		= wgt.FindAnyWidget("btn_ppp_sw_ok");
		m_BtnCancel	= wgt.FindAnyWidget("btn_ppp_sw_cancel");
		m_SQFEditBox = wgt.FindAnyWidget("txt_ppp_sw_editbox");
		m_SQFEditBoxBackground = wgt.FindAnyWidget("txt_ppp_sw_editbox_background");
		m_SQFEditBoxEdge = wgt.FindAnyWidget("txt_ppp_sw_editbox_edge");
		m_AddSqfDebugWatcherButton = wgt.FindAnyWidget("btn_ppp_sw_add_watch");
		m_RemoveSqfDebugWatcherButton = wgt.FindAnyWidget("btn_ppp_sw_remove_watch");
		
		m_SqfDebugWatchersRows = new map<int, Widget>;
		m_SqfDebugWatchers = new map<int, EditBoxWidget>;
		m_SqfDebugWatchersResults = new map<int, EditBoxWidget>;
		m_SqfDebugWatchersButtons = new map<int, ButtonWidget>;
		m_SqfDebugWatchersClearButtons = new map<int, ButtonWidget>;
	
		m_SqfDebugWatchersPanel = wgt;
		
		m_ModuleLocalSqfHistory = GetPlugin(PluginLocalSqfHistory);
		
		m_SqfConsoleHistory = new TStringArray;
		m_SqfConsoleHistory = m_ModuleLocalSqfHistory.GetAllLines();
		
		SqfHistoryBack();
		
		PluginDeveloper module_dev = (PluginDeveloper)GetPlugin( PluginDeveloper );
		int sqf_debug_watchers_count = module_dev.GetSqfDebugWatchersCount();
		for ( int i = 1; i <= sqf_debug_watchers_count; i++ )
		{
			SqfDebugWatcher sqf_debug_watcher = module_dev.GetSqfDebugWatcherById( i );

			if ( sqf_debug_watcher != NULL )
			{
				AddSqfDebugWatcher();
			}
		}
	}
	
	void OnSqfDebugWatcherResult( int id, string result )
	{
		EditBoxWidget sqf_debug_watcher_result = m_SqfDebugWatchersResults.Get( id );

		if ( sqf_debug_watcher_result != NULL )
		{
			sqf_debug_watcher_result.SetText( result );
		}
	}
	
	void ApplySQF()
	{
		string sqf_commands = "";
		m_SQFEditBox.GetText( sqf_commands );
		GetGame().ExecuteSQF( sqf_commands );
		m_SqfConsoleHistory.Insert( sqf_commands );
		m_ModuleLocalSqfHistory.AddNewLine( sqf_commands );
	}
	
	void SqfHistoryBack()
	{
		if ( m_SQFEditBox )
		{
			int history_index = m_SqfConsoleHistory.Count() - m_SqfHistoryRow - 1;
			if ( history_index > -1 )
			{
				m_SQFEditBox.SetText( m_SqfConsoleHistory.Get( history_index ) );
				m_SqfHistoryRow = m_SqfHistoryRow + 1;
			}
		}
	}

	void SqfHistoryForward()
	{
		if ( m_SQFEditBox )
		{
			int history_index = m_SqfConsoleHistory.Count() - m_SqfHistoryRow + 1;
			if ( history_index < m_SqfConsoleHistory.Count() )
			{
				m_SQFEditBox.SetText( m_SqfConsoleHistory.Get( history_index ) );
				m_SqfHistoryRow = m_SqfHistoryRow - 1;
			}
		}
	}
	
	string GetSqfCommandForSqfDebugWatcher( int id )
	{
		EditBoxWidget sqf_debug_watcher = m_SqfDebugWatchers.Get( id );

		if ( sqf_debug_watcher != NULL )
		{
			return sqf_debug_watcher.GetText();
		}
		else
		{
			return "";
		}
	}
	
	void AddSqfDebugWatcher()
	{
		PluginDeveloper module_dev = (PluginDeveloper)GetPlugin( PluginDeveloper );

		int next_id = m_SqfDebugWatchers.Count() + 1;
		if ( next_id <= PluginDeveloper.MAX_SQF_DEBUG_WATCHERS_COUNT )
		{
			Widget root = GetGame().GetWorkspace().CreateWidgets( "gui/layouts/scene_editor_sqf_debug_watcher_row.layout", m_SqfDebugWatchersPanel );
			root.SetPos( 10, 30+(m_SqfDebugWatchers.Count() * 25) );

			EditBoxWidget new_sqf_debug_watcher_input = root.FindAnyWidget( "SqfDebugWatcher" );
			EditBoxWidget new_sqf_debug_watcher_result = root.FindAnyWidget( "SqfDebugWatcherResult" );
			ButtonWidget new_sqf_debug_watcher_button = root.FindAnyWidget( "SqfDebugWatcherButton" );
			ButtonWidget new_sqf_debug_watcher_clear_buggon = root.FindAnyWidget( "SqfDebugWatcherClearButton" );

			new_sqf_debug_watcher_input.SetName( "SqfDebugWatcher" + next_id.ToString() );
			new_sqf_debug_watcher_result.SetName( "SqfDebugWatcher" + next_id.ToString() + "Result" );
			new_sqf_debug_watcher_button.SetName( "SqfDebugWatcher" + next_id.ToString() + "Button" );
			new_sqf_debug_watcher_clear_buggon.SetName( "SqfDebugWatcher" + next_id.ToString() + "ClearButton" );

			m_SqfDebugWatchersRows.Set( next_id, root );
			m_SqfDebugWatchers.Set( next_id, new_sqf_debug_watcher_input );
			m_SqfDebugWatchersResults.Set( next_id, new_sqf_debug_watcher_result );
			m_SqfDebugWatchersButtons.Set( next_id, new_sqf_debug_watcher_button );
			m_SqfDebugWatchersClearButtons.Set( next_id, new_sqf_debug_watcher_clear_buggon );

			SqfDebugWatcher sqf_debug_watcher = module_dev.GetSqfDebugWatcherById( next_id );

			if ( sqf_debug_watcher != NULL )
			{
				new_sqf_debug_watcher_input.SetText( sqf_debug_watcher.GetSqfCommand() );

				if ( sqf_debug_watcher.IsRunning() )
				{
					new_sqf_debug_watcher_button.SetText( "STOP" );
				}
				else
				{
					new_sqf_debug_watcher_button.SetText( "RUN" );
				}
			}
			else
			{
				module_dev.AddSqfDebugWatcher( next_id );
			}
			float m_orginal_width;
			float m_orginal_height;
			m_SqfDebugWatchersPanel.GetSize( m_orginal_width, m_orginal_height );
			m_SqfDebugWatchersPanel.SetSize( m_orginal_width, m_orginal_height + 25 );
			
			m_SQFEditBox.GetPos(m_orginal_width, m_orginal_height);
			m_SQFEditBox.SetPos(m_orginal_width, m_orginal_height + 25);
			m_SQFEditBoxBackground.GetPos(m_orginal_width, m_orginal_height);
			m_SQFEditBoxBackground.SetPos(m_orginal_width, m_orginal_height + 25);
			m_SQFEditBoxEdge.GetPos(m_orginal_width, m_orginal_height);
			m_SQFEditBoxEdge.SetPos(m_orginal_width, m_orginal_height + 25);
		}
	}
	
	bool RemoveLastSqfDebugWatcher()
	{
		PluginDeveloper module_dev = (PluginDeveloper)GetPlugin( PluginDeveloper );
		int last_id = module_dev.GetSqfDebugWatchersCount();

		if ( module_dev.RemoveLastSqfDebugWatcher() )
		{
			Widget w = m_SqfDebugWatchersRows.Get( last_id );
			w.Destroy();

			m_SqfDebugWatchersRows.Remove( last_id );
			m_SqfDebugWatchers.Remove( last_id );
			m_SqfDebugWatchersResults.Remove( last_id );
			m_SqfDebugWatchersButtons.Remove( last_id );
			m_SqfDebugWatchersClearButtons.Remove( last_id );
			
			float m_orginal_width;
			float m_orginal_height;
			m_SqfDebugWatchersPanel.GetSize( m_orginal_width, m_orginal_height );
			m_SqfDebugWatchersPanel.SetSize( m_orginal_width, m_orginal_height - 25 );
			
			m_SQFEditBox.GetPos(m_orginal_width, m_orginal_height);
			m_SQFEditBox.SetPos(m_orginal_width, m_orginal_height - 25);
			m_SQFEditBoxBackground.GetPos(m_orginal_width, m_orginal_height);
			m_SQFEditBoxBackground.SetPos(m_orginal_width, m_orginal_height - 25);
			m_SQFEditBoxEdge.GetPos(m_orginal_width, m_orginal_height);
			m_SQFEditBoxEdge.SetPos(m_orginal_width, m_orginal_height - 25);
		}
	}
	
	bool OnClick( Widget w, int x, int y, int button )
	{
		super.OnClick( w, x, y, button );
		
		PluginDeveloper module_dev = (PluginDeveloper)GetPlugin( PluginDeveloper );
		
		int id;
		SqfDebugWatcher sqf_debug_watcher;
		EditBoxWidget sqf_debug_watcher_input;
		EditBoxWidget sqf_debug_watcher_result;
		
		for ( int i = 0; i < m_SqfDebugWatchersButtons.Count(); i++ )
		{
			ButtonWidget sqf_debug_watcher_button = m_SqfDebugWatchersButtons.GetElement( i );

			if ( w == sqf_debug_watcher_button )
			{
				id = m_SqfDebugWatchersButtons.GetKey( i );
				sqf_debug_watcher = module_dev.GetSqfDebugWatcherById( id );

				if ( sqf_debug_watcher != NULL )
				{
					if ( sqf_debug_watcher.IsRunning() )
					{
						sqf_debug_watcher.Stop();
						sqf_debug_watcher_button.SetText( "RUN" );
						sqf_debug_watcher_result = m_SqfDebugWatchersResults.GetElement( i );

						if ( sqf_debug_watcher_result != NULL )
						{
							sqf_debug_watcher_result.SetText( "" );
						}
					}
					else
					{
						sqf_debug_watcher.Run();
						sqf_debug_watcher_button.SetText( "STOP" );
					}
				}

				return true;
			}
		}

		for ( i = 0; i < m_SqfDebugWatchersClearButtons.Count(); i++ )
		{
			ButtonWidget sqf_debug_watcher_clear_button = m_SqfDebugWatchersClearButtons.GetElement( i );

			if ( w == sqf_debug_watcher_clear_button )
			{
				id = m_SqfDebugWatchersClearButtons.GetKey( i );
				sqf_debug_watcher_button = m_SqfDebugWatchersButtons.GetElement( i );
				sqf_debug_watcher = module_dev.GetSqfDebugWatcherById( id );

				if ( sqf_debug_watcher != NULL )
				{
					if ( sqf_debug_watcher.IsRunning() )
					{
						sqf_debug_watcher.Stop();
						sqf_debug_watcher_button.SetText( "RUN" );
					}

					sqf_debug_watcher.SetSqfCommand( "" );

					sqf_debug_watcher_input = m_SqfDebugWatchers.GetElement( i );
					sqf_debug_watcher_result = m_SqfDebugWatchersResults.GetElement( i );

					sqf_debug_watcher_input.SetText( "" );
					sqf_debug_watcher_result.SetText( "" );
				}

				return true;
			}
		}
		
		if ( w == m_BtnOk )
		{		
			ApplySQF();
			PopupBack();
			return true;
		}
		else if ( w == m_BtnCancel )
		{
			PopupBack();
			return true;
		}
		else if ( w == m_AddSqfDebugWatcherButton )
		{
			AddSqfDebugWatcher();
			return true;
		}
		else if ( w == m_RemoveSqfDebugWatcherButton )
		{
			RemoveLastSqfDebugWatcher();
			return true;
		}
		
		return false;
	}
}
