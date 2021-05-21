class PluginDeveloper extends PluginBase
{	
	static const int MIN_SQF_DEBUG_WATCHERS_COUNT = 5;
	static const int MAX_SQF_DEBUG_WATCHERS_COUNT = 12;
	
	protected autoptr array<SqfDebugWatcher>		m_SqfDebugWatchers;
	protected bool									m_IsWinHolding;	
	protected bool 									m_GodModeEnabled;
	protected int									m_FeaturesMask;
	
	static PluginDeveloper GetInstance()
	{
		return GetPlugin( PluginDeveloper );
	}
	
	//! Set Player position at his cursor position in the world
	void TeleportAtCursor()
	{
		DeveloperTeleport.TeleportAtCursor();
	}
	
	//! Teleport player at position
	void Teleport(PlayerBase player, vector position)
	{
		DeveloperTeleport.SetPlayerPosition(player, position);
	}
	
	//! Enable / Disable Free camera (Fly mod)
	void ToggleFreeCameraBackPos()
	{
		DeveloperFreeCamera.FreeCameraToggle( GetGame().GetPlayer(), false );
	}
	
	//! Enable / Disable Free camera (Fly mod) - disable of camera will teleport player at current free camera position.
	void ToggleFreeCamera()
	{
		DeveloperFreeCamera.FreeCameraToggle( GetGame().GetPlayer(), true );
	}
	
	//! Enable / Disable Free camera (Fly mod) - disable of camera will teleport player at current free camera position.
	Camera GetFreeCamera()
	{
		PlayerBase pb = GetGame().GetPlayer();
		if (!pb.m_Cam)
				return NULL;

		return pb.m_Cam;
	}
	
	bool IsEnabledFreeCamera()
	{
		return DeveloperFreeCamera.IsFreeCameraEnabled();
	}	
	
	// System Public Events
	void PluginDeveloper()
	{
		
	}

	void ~PluginDeveloper()
	{
		int i;
		
		for ( i = 0; i < m_SqfDebugWatchers.Count(); i++ )
		{
			delete m_SqfDebugWatchers.Get( i );
		}
	}

	void OnInit()
	{
		super.OnInit();
		
		m_SqfDebugWatchers = new array<SqfDebugWatcher>;
		m_GodModeEnabled = false;
		
		DeveloperFreeCamera.OnInit();
		
		for ( int i = 1; i <= MIN_SQF_DEBUG_WATCHERS_COUNT; i++ )
		{
			m_SqfDebugWatchers.Insert( new SqfDebugWatcher(i, "") );
		}
	}
	
	void OnRPC(PlayerBase player, int rpc_type, ParamsReadContext ctx)
	{
		if ( !GetGame().IsDebug() )
		{
			return;
		}
		
		switch(rpc_type)
		{
			case DEV_RPC_SPAWN_ITEM:
			{
				OnRPCSpawnItem(player, ctx); break;
			}
			case DEV_RPC_CLEAR_INV:
			{
				OnRPCClearInventory(player); break;
			}
			case DEV_RPC_SEND_SERVER_LOG:
			{
				OnRPCServerLogRecieved( ctx ); break;
			}
			case DEV_RPC_TOGGLE_GOD_MODE:
			{
				OnRPCToggleGodMode( player ); break;
			}
			case RPC_SYNC_SCENE_OBJECT:
			{
				OnRPCSyncSceneObject( ctx ); break;
			}
		}
		
		DeveloperTeleport.OnRPC(player, rpc_type, ctx);
	}
	
	// Public API
	// Send Print to scripted console
	void PrintLogClient(string msg_log)
	{	
		if ( g_Game.GetUIManager().IsMenuOpen(MENU_SCRIPTCONSOLE) )
		{
			Param1<string> msg_p = new Param1<string>(msg_log);
			CallMethod(CALL_ID_SCR_CNSL_ADD_PRINT, msg_p);
		}
	}	
	
	// Server Log Synch: Server Side
	void SendServerLogToClient(string msg)
	{
		autoptr array<Man> players = new array<Man>;
		GetGame().GetPlayers( players );
			
		for ( int i = 0; i < players.Count(); ++i )
		{
			autoptr Param1<string> param = new Param1<string>( msg );
			players.Get(i).RPCSingleParam(DEV_RPC_SEND_SERVER_LOG, param, players.Get(i));
		}
	}
	
	void OnRPCSyncSceneObject( ParamsReadContext ctx )
	{
		Param3<string, EntityAI, Param> par = new Param3<string, EntityAI, Param>( "", NULL, new Param );

		if ( ctx.Read( par ) )
		{		
			PluginSceneManager module_scene_editor = ( PluginSceneManager ) GetPlugin( PluginSceneManager );
			SceneData	scene_data = module_scene_editor.GetLoadedScene();
			SceneObject scene_object = scene_data.GetSceneObjectByEntityAI( par.param2 );
			
			GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).Call( scene_object, par.param1, par.param3 );
		}
	}

	// RPC Events
	// Server Log Synch: Client Side
	void OnRPCServerLogRecieved(ParamsReadContext ctx)
	{
		autoptr Param1<string> param = new Param1<string>( "" );
		
		if ( ctx.Read(param) && param.param1 != "" )
		{
			Debug.ReceivedLogMessageFromServer(param.param1);
		}
	}
	
	void OnRPCToggleGodMode( PlayerBase player )
	{
		ToggleGodMode( player );
	}
	
	void OnRPCClearInventory(PlayerBase player)
	{
		ClearInventory(player);
	}
	
	// Client -> Server Spawning: Server Side
	void OnRPCSpawnItem(PlayerBase player, ParamsReadContext ctx)
	{
		autoptr Param5<string, int, float, float, float> p = new Param5<string, int, float, float, float>("", 0, 0, 0, 0);
		if ( ctx.Read(p) )
		{
			SpawnItem(player, p.param1, p.param2,  p.param3,  p.param4, p.param5);
		}
	}
	
	// God mode
	bool IsGodMode()
	{
		return m_GodModeEnabled;
	}

	void ToggleGodModeThisPlayer()
	{
		ToggleGodMode( GetGame().GetPlayer() );
	}

	void ToggleGodMode( PlayerBase player )
	{
		if ( GetGame().IsServer() )
		{
			if ( !m_GodModeEnabled )
			{
				player.SetAllowDamage( false );
				player.MessageStatus( "God mode ENABLED" );
				m_GodModeEnabled = true;
			}
			else
			{
				player.SetAllowDamage( true );
				player.MessageStatus( "God mode DISABLED" );
				m_GodModeEnabled = false;
			}
		}
		else
		{
			if ( !m_GodModeEnabled )
			{
				m_GodModeEnabled = true;
			}
			else
			{
				m_GodModeEnabled = false;
			}
			player.RPCSingleParam( DEV_RPC_TOGGLE_GOD_MODE, NULL );
		}
	}
	
	// Spawn item in palyer
	EntityAI SpawnItem(PlayerBase player, string item_name, int spawn_type, float damage, float quantity, float distance = 0, bool usePosition = false, vector pos = "0 0 0", EntityAI attachmentObject = NULL)
	{
		if ( quantity == -1 && !GetDayZGame().IsNewPlayer() )
		{
			quantity = GetGame().ConfigGetInt( CFG_VEHICLESPATH + " " + item_name + " stackedMax");
		}

		if ( GetGame().IsServer() )
		{		
			// Client -> Server Spawning: Server Side
			EntityAI entity = player.SpawnItem(item_name, spawn_type, true, distance, usePosition, pos, attachmentObject);
		
			if ( entity != NULL && entity.IsInherited(ItemBase) )
			{
				ItemBase item = entity;
				
				if ( damage > 0 )
				{
					item.SetDamage(damage);
				}
				if ( quantity > 0 )
				{
					item.SetQuantity(quantity);
				}
			}
			
			return entity;
		}
		else
		{		
			// Client -> Server Spawning: Client Side
			autoptr Param5<string, int, float, float, float> params = new Param5<string, int, float, float, float>(item_name, spawn_type, damage, quantity, distance);
			player.RPCSingleParam(DEV_RPC_SPAWN_ITEM, params);
		}
		
		return NULL;
	}
	
	EntityAI SpawnFromClipboard()
	{
		UIScriptedMenu menu_curr = GetGame().GetUIManager().GetMenu();

		if ( menu_curr == NULL )
		{			
			PlayerBase player = GetGame().GetPlayer();
			if ( player )
			{
				vector pos_player = player.GetPosition();
				
				// Get item from clipboard
				string		item_name;
				GetGame().CopyFromClipboard(item_name);
				
				// Find cursor position in the world
				vector pos_cursor = GetGame().GetCursorPos();
				
				// Spawn & Synch Item
				EntityAI object_spawned_from_clipboard = SpawnItem(player, item_name, SPAWNTYPE_GROUND, 0, -1, vector.Distance (pos_cursor, pos_player ));
				Print(object_spawned_from_clipboard);
				return object_spawned_from_clipboard;
			}
		}
		
		return NULL;
	}
	
	// Clear Player Inventory
	void ClearInventory(PlayerBase player)
	{
		if ( GetGame().IsServer() )
		{
			player.ClearInventory();
		}
		else
		{
			Param1<int> params = new Param1<int>(0);
			player.RPCSingleParam(DEV_RPC_CLEAR_INV, params);
		}
	}	
	
	// Debug Console: SQF Debug Watchers
	void ToggleHelpScreen()
	{
		if ( g_Game.GetUIManager().GetMenu() == NULL )
		{
			g_Game.GetUIManager().EnterScriptedMenu(MENU_HELP_SCREEN, NULL);
		}
		else if ( g_Game.GetUIManager().IsMenuOpen(MENU_HELP_SCREEN) )
		{
			g_Game.GetUIManager().Back();
		}
	}

	void ToggleScriptConsole()
	{
		if ( g_Game.GetUIManager().GetMenu() == NULL )
		{
			g_Game.GetUIManager().EnterScriptedMenu(MENU_SCRIPTCONSOLE, NULL);
		}
		else if ( g_Game.GetUIManager().IsMenuOpen(MENU_SCRIPTCONSOLE) )
		{
			g_Game.GetUIManager().Back();
		}
	}

	void AddSqfDebugWatcher( int id )
	{
		m_SqfDebugWatchers.Insert( new SqfDebugWatcher(id, "") );
	}

	bool RemoveLastSqfDebugWatcher()
	{
		if ( m_SqfDebugWatchers.Count() > MIN_SQF_DEBUG_WATCHERS_COUNT )
		{
			int last_sqf_debug_watcher = m_SqfDebugWatchers.Count() - 1;
			
			SqfDebugWatcher sqf_debug_watcher = m_SqfDebugWatchers.Get( last_sqf_debug_watcher );
			delete sqf_debug_watcher;
			
			m_SqfDebugWatchers.Remove( last_sqf_debug_watcher );
			
			return true;
		}
		
		return false;
	}

	void UpdateSqfDebugWatchers()
	{
		for ( int i = 0; i < m_SqfDebugWatchers.Count(); i++ )
		{
			SqfDebugWatcher sqf_debug_watcher = m_SqfDebugWatchers.Get( i );
				
			if ( sqf_debug_watcher != NULL && sqf_debug_watcher.IsRunning() )
			{
				if ( GetGame().GetUIManager().IsMenuOpen(MENU_SCRIPTCONSOLE) || GetGame().GetUIManager().IsMenuOpen(MENU_SCENE_EDITOR))
				{
						string sqf_command = GetSqfCommandForSqfDebugWatcher( m_SqfDebugWatchers.Get( i ).GetId() );
						m_SqfDebugWatchers.Get( i ).SetSqfCommand( sqf_command );
				}
					
				m_SqfDebugWatchers.Get( i ).Execute();
			}
		}
	}

	int GetSqfDebugWatchersCount()
	{
		return m_SqfDebugWatchers.Count();
	}

	SqfDebugWatcher GetSqfDebugWatcherById( int id )
	{
		for ( int i = 0; i < m_SqfDebugWatchers.Count(); i++ )
		{
			SqfDebugWatcher sqf_debug_watcher = m_SqfDebugWatchers.Get( i );
			if ( id == sqf_debug_watcher.GetId() )
			{
				return sqf_debug_watcher;
			}
		}
		
		return NULL;
	}

	string GetSqfCommandForSqfDebugWatcher( int id )
	{
		autoptr Param1<int> param = new Param1<int>(id);
		Param1<string> p = CallMethod( CALL_ID_SCR_CNSL_GET_SQF_WATCHER, param);
		string s = p.param1;
		delete p;
		return s;
	}

	void OnSqfDebugWatcherResult( int id, string result )
	{
		autoptr Param2<int, string> param = new Param2<int, string>(id, result);
		CallMethod( CALL_ID_SCR_CNSL_SET_SQF_WATCHER_RESULT, param);
	}
	
	// Mission Editor
	void ToggleMissionLoader()
	{
		g_Game.GetUIManager().OpenWindow( GUI_WINDOW_MISSION_LOADER );
	}
	
	// Script Editor History
	private void ScriptHistoryNext()
	{
		// Console key press
		if ( g_Game.GetUIManager().IsMenuOpen(MENU_SCRIPTCONSOLE) )
		{
			CallMethod(CALL_ID_SCR_CNSL_HISTORY_NEXT, NULL);
		}
	}

	private	void ScriptHistoryBack()
	{
		// Console key press
		if ( g_Game.GetUIManager().IsMenuOpen(MENU_SCRIPTCONSOLE) )
		{
			CallMethod(CALL_ID_SCR_CNSL_HISTORY_BACK, NULL);
		}
	}
	
	private bool IsIngame()
	{
		UIScriptedMenu menu_curr = GetGame().GetUIManager().GetMenu();

		if ( menu_curr == NULL )
		{			
			return true;
		}
		
		return false;
	}

	private bool IsInConsole()
	{
		UIScriptedMenu menu_curr = GetGame().GetUIManager().GetMenu();

		if ( menu_curr != NULL && menu_curr.GetID() == MENU_SCRIPTCONSOLE )
		{			
			return true;
		}
		
		return false;
	}
	
	// Tools
	int QuickSortPartition( out TStringArray arr, local int left, local int right )
	{
		local string pivot = arr.Get( left );
		local int i = left;
		local int j = right + 1;
		local string temp;
			
		while ( true )
		{
			while ( true )
			{ 
				i++; 
				if ( i > right || arr.Get(i) > pivot )
				{
					break;
				}
			}
			
			while ( true )
			{
				j--; 
				if ( arr.Get(j) <= pivot )
				{
					break;
				}
			}
			
			if ( i >= j ) 
			{
				break;
			}
			
			temp = arr.Get( i ); 
			arr.Set( i, arr.Get(j) ); 
			arr.Set( j, temp );
		}
		
		temp = arr.Get( left ); 
		arr.Set( left, arr.Get(j) );
		arr.Set( j, temp );
	   
		return j;
	}

	void QuickSort( out TStringArray arr, local int left, local int right )
	{
		local int j;

		if ( left < right ) 
		{
			j = QuickSortPartition( arr, left, right );
			QuickSort( arr, left, j - 1 );
			QuickSort( arr, j + 1, right );
		}
	}

	void SortStringArray( out TStringArray arr )
	{
		QuickSort( arr, 0, arr.Count() - 1 );
	}
}

void OnSqfDebugWatcherResult( int id, string result )
{
	PluginDeveloper.GetInstance().OnSqfDebugWatcherResult( id, result );
}
