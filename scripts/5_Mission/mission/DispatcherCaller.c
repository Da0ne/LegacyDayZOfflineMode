class DispatcherCaller extends Dispatcher
{
	private void SendLogAtClient(Param1<string> p)
	{
		PluginDeveloper m = GetPlugin(PluginDeveloper);
		
		if ( m != NULL )
		{
			m.SendServerLogToClient(p.param1);
		}
	}
	
	private void ScriptConsoleAddPrint(Param1<string> p)
	{
		UIScriptedMenu ui_menu = GetGame().GetUIManager().GetMenu();
		if ( ui_menu.GetID() == MENU_SCRIPTCONSOLE )
		{
			ScriptConsole scripted_console = ui_menu;
			scripted_console.Add(p.param1);
		}
	}
	
	private void ScriptConsoleHistoryBack()
	{
		UIScriptedMenu ui_menu = GetGame().GetUIManager().GetMenu();
		if ( ui_menu.GetID() == MENU_SCRIPTCONSOLE )
		{
			ScriptConsole scripted_console = ui_menu;
			scripted_console.HistoryBack();
		}
	}
	
	private void ScriptConsoleHistoryForward()
	{
		UIScriptedMenu ui_menu = GetGame().GetUIManager().GetMenu();
		if ( ui_menu.GetID() == MENU_SCRIPTCONSOLE )
		{
			ScriptConsole scripted_console = ui_menu;
			scripted_console.HistoryForward();
		}
	}
	
	private Param ScriptConsoleGetSqfCommandWatcher( Param params )
	{
		Param1<int> p = params;
		
		int watcher_id = p.param1;
		
		UIScriptedMenu ui_menu = GetGame().GetUIManager().GetMenu();
		Param1<string> param;
		
		if ( ui_menu.GetID() == MENU_SCENE_EDITOR )
		{
			SceneEditorMenu scene_editor_menu = ui_menu;
			param = new Param1<string>(scene_editor_menu.GetSqfCommandForSqfDebugWatcher(watcher_id));
		}
		else if( ui_menu.GetID() == MENU_SCRIPTCONSOLE )
		{
			ScriptConsole scripted_console = ui_menu;
			param = new Param1<string>(scripted_console.GetSqfCommandForSqfDebugWatcher(watcher_id));
		}
		
		return param;
	}
	
	private void ScriptConsoleGetSqfCommandWatcherResult( Param params )
	{
		Param2<int, string> p = params;
		
		int watcher_id			= p.param1;
		string watcher_result	= p.param2;
		
		UIScriptedMenu ui_menu = GetGame().GetUIManager().GetMenu();
		if ( ui_menu )
		{
			if( ui_menu.GetID() == MENU_SCENE_EDITOR )
			{
				SceneEditorMenu scene_editor_menu = ui_menu;
				scene_editor_menu.OnSqfDebugWatcherResult( watcher_id, watcher_result );
			}
			else if( ui_menu.GetID() == MENU_SCRIPTCONSOLE )
			{
				ScriptConsole scripted_console = ui_menu;
				scripted_console.OnSqfDebugWatcherResult( watcher_id, watcher_result );
			}
		}
	}
	
	private void SceneEditorCommand(Param params)
	{
		UIScriptedMenu ui_menu = GetGame().GetUIManager().GetMenu();
		if ( ui_menu && ui_menu.GetID() == MENU_SCENE_EDITOR )
		{
			SceneEditorMenu editor_menu = ui_menu;
			editor_menu.SceneEditorCommand( params );
		}
	}

	private void MissionGameplayHideInventory()
	{
		MissionGameplay mission = (MissionGameplay)GetGame().GetMission();
		if ( mission )
		{
			mission.HideInventory();
		}
	}
	
	Param CallMethod(CallID call_id, Param params)
	{				
		switch(call_id)
		{
		case CALL_ID_SEND_LOG:
			SendLogAtClient(params);
			break;
		case CALL_ID_SCR_CNSL_ADD_PRINT:
			ScriptConsoleAddPrint(params);
			break;
		case CALL_ID_SCR_CNSL_HISTORY_BACK:
			ScriptConsoleHistoryBack();
			break;		
		case CALL_ID_SCR_CNSL_HISTORY_NEXT:
			ScriptConsoleHistoryForward();
			break;
		case CALL_ID_SCR_CNSL_GET_SQF_WATCHER:
			return ScriptConsoleGetSqfCommandWatcher( params );
			break;
		case CALL_ID_SCR_CNSL_SET_SQF_WATCHER_RESULT:
			ScriptConsoleGetSqfCommandWatcherResult( params );
			break;
		case CALL_ID_SCENE_EDITOR_COMMAND:
			SceneEditorCommand( params );
			break;
		case CALL_ID_HIDE_INVENTORY:
			MissionGameplayHideInventory();
			break;
		}
	}
}
