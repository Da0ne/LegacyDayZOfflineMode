class PluginDeveloperSync extends PluginBase
{
	autoptr Timer m_UpdateTimer;
	autoptr array<PlayerBase> m_RegisteredPlayers;
	
	autoptr array<SyncedValue> m_PlayerStatsSynced;
	autoptr array<SyncedValue> m_PlayerModsSynced;
	autoptr array<SyncedValue> m_PlayerAgentsSynced;
	
	bool m_StatsUpdateStatus;
	bool m_ModsUpdateStatus;
	bool m_AgentsUpdateStatus;
	
	void PluginDeveloperSync()
	{
		m_RegisteredPlayers 	= new array<PlayerBase>;
		m_PlayerStatsSynced 	= new array<SyncedValue>;
		m_PlayerModsSynced 		= new array<SyncedValue>;
		m_PlayerAgentsSynced 	= new array<SyncedValue>;
		
		m_StatsUpdateStatus 	= false;
		m_ModsUpdateStatus 		= false;
		m_AgentsUpdateStatus 	= false;
	}
	
	//================================================================
	// Init / Update
	//================================================================
	void OnInit()
	{
		super.OnInit();
		
		if ( GetGame().IsServer() )
		{
			//create timer
			m_UpdateTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
		}
	}

	void EnableUpdate( bool state, int type, PlayerBase player )
	{
		//set update by type
		switch ( type )
		{
			case DEV_STATS_UPDATE: 
			{
				m_StatsUpdateStatus = state; 
				break;	
			}
			
			case DEV_MODS_UPDATE: 
			{
				m_ModsUpdateStatus = state; 
				break;	
			}
			
			case DEV_AGENTS_UPDATE: 
			{
				m_AgentsUpdateStatus = state; 
				break;	
			}
		}	
		
		if ( !GetDayZGame().IsNewPlayer() )
		{
			return;
		}
		
		if ( m_UpdateTimer )
		{
			if ( state && !m_UpdateTimer.IsRunning() )
			{
				//Print("ToggleUpdate() - RUN");
				
				if ( m_StatsUpdateStatus || m_ModsUpdateStatus || m_AgentsUpdateStatus )
				{
					m_UpdateTimer.Run( 1, this, "Update", NULL, true );
					
					//register player
					RegisterPlayer( player );	
				}
			}
			else
			{
				//Print("ToggleUpdate() - STOP");
				
				if ( !m_StatsUpdateStatus && !m_ModsUpdateStatus && !m_AgentsUpdateStatus )
				{
					m_UpdateTimer.Stop();
				
					//unregister player
					UnregisterPlayer( player );
				}
			}
		}
	}
	
	void Update()
	{
		//Stats
		if ( m_StatsUpdateStatus )
		{
			//Multiplayer server
			if ( GetDayZGame().IsMultiplayer() && GetDayZGame().IsServer() )
			{
				for ( int i = 0; i < m_RegisteredPlayers.Count(); i++ )
				{
					if ( m_RegisteredPlayers.Get( i ) )
					{
						SendRPCStats( m_RegisteredPlayers.Get( i ) );		
					}
				}
			}	
			//Local server
			else
			{
				if ( GetDayZGame().IsServer() )
				{
					//update local
					UpdateStatsLocal();	
				}
			}
		}
		
		//Mods
		if ( m_ModsUpdateStatus )
		{
			//Multiplayer server
			if ( GetDayZGame().IsMultiplayer() && GetDayZGame().IsServer() )
			{
				for ( int j = 0; j < m_RegisteredPlayers.Count(); j++ )
				{
					if ( m_RegisteredPlayers.Get( j ) )
					{
						SendRPCMods( m_RegisteredPlayers.Get( j ) );		
					}
				}
			}	
			//Local server
			else
			{
				if ( GetDayZGame().IsServer() )
				{
					//update local
					UpdateModsLocal();	
				}
			}
		}
		
		//Agents
		if ( m_AgentsUpdateStatus )
		{
			//Multiplayer server
			if ( GetDayZGame().IsMultiplayer() && GetDayZGame().IsServer() )
			{
				for ( int k = 0; k < m_RegisteredPlayers.Count(); k++ )
				{
					if ( m_RegisteredPlayers.Get( k ) )
					{
						SendRPCAgents( m_RegisteredPlayers.Get( k ) );		
					}
				}
			}	
			//Local server
			else
			{
				if ( GetDayZGame().IsServer() )
				{
					//update local
					UpdateAgentsLocal();	
				}
			}
		}
	}
	
	//--- Register / Unregister player
	void RegisterPlayer( PlayerBase player )
	{
		if ( !IsPlayerRegistered( player ) ) 
		{
			m_RegisteredPlayers.Insert( player );
		}
	}

	void UnregisterPlayer( PlayerBase player )
	{
		if ( IsPlayerRegistered( player ) ) 
		{
			m_RegisteredPlayers.RemoveItem( player );
		}
	}

	bool IsPlayerRegistered( PlayerBase player )
	{
		if ( m_RegisteredPlayers.Find( player ) > 0 )
		{
			return true;
		}
		
		return false;
	}
	
	//================================================================
	// RPC
	//================================================================ 
	void OnRPC( PlayerBase player, int rpc_type, ParamsReadContext ctx )
	{
		if ( !GetGame().IsDebug() )
		{
			return;
		}
		
		switch( rpc_type )
		{
			case DEV_STATS_UPDATE:
			{
				EnableUpdate( GetRPCUpdateState( ctx ), DEV_STATS_UPDATE, player ); break;
			}
			
			case DEV_MODS_UPDATE:
			{
				EnableUpdate( GetRPCUpdateState( ctx ), DEV_MODS_UPDATE, player ); break;
			}
			
			case DEV_AGENTS_UPDATE:
			{
				EnableUpdate( GetRPCUpdateState( ctx ), DEV_AGENTS_UPDATE, player ); break;
			}
			
			case DEV_RPC_STATS_DATA:
			{
				OnRPCStats( ctx ); break;
			}
			
			case DEV_RPC_MODS_DATA:
			{
				OnRPCMods( ctx ); break;
			}
			
			case DEV_RPC_AGENTS_DATA:
			{
				OnRPCAgents( ctx ); break;
			}
		}
	}
	
	//get update status from ctx data
	bool GetRPCUpdateState( ParamsReadContext ctx )
	{
		autoptr Param1<int> p = new Param1<int>(0);
		if ( ctx.Read(p) )
		{
			return (bool) p.param1;
		}
		
		return false;
	}

	//STATS
	//send player stats through RPC
	void SendRPCStats( PlayerBase player )
	{
		//write and send values
		if ( player )
		{
			array<Param> rpc_params = new array<Param>;
			
			//param count
			float param_count = (float) player.m_PlayerStats.Get().Count();
			rpc_params.Insert( new Param2<string, float>( "PARAM_COUNT", param_count ) );
			
			for ( int i = 0; i < player.m_PlayerStats.Get().Count(); i++ ) 
			{
				string label = player.m_PlayerStats.Get().Get( i ).GetLabel();
				float value = (float) player.m_PlayerStats.Get().Get( i ).Get();
				rpc_params.Insert( new Param2<string, float>( label, value ) );
			}
			
			//send params
			GetDayZGame().RPC( player, DEV_RPC_STATS_DATA, rpc_params );
		}
	}

	//Display player stats
	void OnRPCStats( ParamsReadContext ctx )
	{
		//clear values
		m_PlayerStatsSynced.Clear();
		
		autoptr Param2<string, float> p = new Param2<string, float>( "", 0 );
		
		//get param count
		int param_count = 0;
		if ( ctx.Read(p) )
		{
			param_count = (int) p.param2;
		}
		
		//read values and set 
		for ( int i = 0; i < param_count; i++ )
		{
			ctx.Read(p);
			m_PlayerStatsSynced.Insert( new SyncedValue( p.param1, p.param2 ) );
		}
	}

	//Update data on local
	void UpdateStatsLocal()
	{
		PlayerBase player = ( PlayerBase ) GetDayZGame().GetPlayer();
		
		if ( player && GetDayZGame().IsNewPlayer() )
		{
			//clear values
			m_PlayerStatsSynced.Clear();
			
			//set values
			for ( int i = 0; i < player.m_PlayerStats.Get().Count(); i++ ) 
			{
				string label = player.m_PlayerStats.Get().Get( i ).GetLabel();
				float value = player.m_PlayerStats.Get().Get( i ).Get();
				m_PlayerStatsSynced.Insert( new SyncedValue( label, value ) );
			}
		}
	}

	//MODS
	//send player mods through RPC
	void SendRPCMods( PlayerBase player )
	{
		//write and send values
		if ( player )
		{
			array<Param> rpc_params = new array<Param>;
			
			//get active/inactive modifiers
			ModifiersManager mods_manager = player.GetModifiersManager();
			
			autoptr array<string> active_mods = new array<string>;
			mods_manager.DbgGetActiveModifiers( active_mods );
			
			autoptr array<string> inactive_mods = new array<string>;
			mods_manager.DbgGetInactiveModifiers( inactive_mods );
			
			//param count
			float active_mods_count = (float) active_mods.Count();
			float inactive_mods_count = (float) inactive_mods.Count();
			float param_count = active_mods_count + inactive_mods_count;
			rpc_params.Insert( new Param2<string, float>( "PARAM_COUNT", param_count ) );
			
			//set active modifiers
			for ( int i = 0; i < active_mods.Count(); i++ ) 
			{
				rpc_params.Insert( new Param2<string, float>( active_mods.Get( i ), 1 ) );
			}
			
			//set inactive modifiers
			for ( int j = 0; j < inactive_mods.Count(); j++ ) 
			{
				rpc_params.Insert( new Param2<string, float>( inactive_mods.Get( j ), 0 ) );
			}
			
			//send params
			GetDayZGame().RPC( player, DEV_RPC_MODS_DATA, rpc_params );
		}
	}

	//Display player modifiers
	void OnRPCMods( ParamsReadContext ctx )
	{
		//clear values
		m_PlayerModsSynced.Clear();
		
		autoptr Param2<string, float> p = new Param2<string, float>( "", 0 );
		
		//get param count
		int param_count = 0;
		if ( ctx.Read(p) )
		{
			param_count = (int) p.param2;
		}
		
		//read values and set 
		for ( int i = 0; i < param_count; i++ )
		{
			ctx.Read(p);
			m_PlayerModsSynced.Insert( new SyncedValue( p.param1, p.param2 ) );
		}
	}

	//Update data on local
	void UpdateModsLocal()
	{
		PlayerBase player = ( PlayerBase ) GetDayZGame().GetPlayer();
		
		if ( player && GetDayZGame().IsNewPlayer() )
		{
			//clear values
			m_PlayerModsSynced.Clear();
			
			//get active/inactive modifiers
			ModifiersManager mods_manager = player.GetModifiersManager();
			
			autoptr array<string> active_mods = new array<string>;
			mods_manager.DbgGetActiveModifiers( active_mods );
			
			autoptr array<string> inactive_mods = new array<string>;
			mods_manager.DbgGetInactiveModifiers( inactive_mods );
			
			//set active modifiers
			for ( int i = 0; i < active_mods.Count(); i++ ) 
			{
				m_PlayerModsSynced.Insert( new SyncedValue( active_mods.Get( i ), 1 ) );
			}
			
			//set inactive modifiers
			for ( int j = 0; j < inactive_mods.Count(); j++ ) 
			{
				m_PlayerModsSynced.Insert( new SyncedValue( inactive_mods.Get( j ), 0 ) );
			}
		}
	}
	
	//AGENTS
	//send player mods through RPC
	void SendRPCAgents( PlayerBase player )
	{
		//write and send values
		if ( player )
		{
			array<Param> rpc_params = new array<Param>;
			
			//get agent pool data
			array<Param> agent_pool = new array<Param>;
			player.m_AgentPool.GetDebugObject( agent_pool );
			
			//agents count
			autoptr Param1<int> p1 = agent_pool.Get( 0 );
			int agents_count = p1.param1;
			//param count
			float param_count = (float) agents_count;
			rpc_params.Insert( new Param2<string, float>( "PARAM_COUNT", param_count ) );
			
			//set synced data
			for ( int i = 0; i < agents_count; i++ ) 
			{
				autoptr Param2<string, string> p2 = agent_pool.Get( i + 1 );
				rpc_params.Insert( new Param2<string, float>( p2.param1, p2.param2.ToFloat() ) );
			}
			
			//send params
			GetDayZGame().RPC( player, DEV_RPC_AGENTS_DATA, rpc_params );
		}
	}

	//Display player modifiers
	void OnRPCAgents( ParamsReadContext ctx )
	{
		//clear values
		m_PlayerAgentsSynced.Clear();
		
		autoptr Param2<string, float> p = new Param2<string, float>( "", 0 );
		
		//get param count
		int param_count = 0;
		if ( ctx.Read(p) )
		{
			param_count = (int) p.param2;
		}
		
		//read values and set 
		for ( int i = 0; i < param_count; i++ )
		{
			ctx.Read(p);
			m_PlayerAgentsSynced.Insert( new SyncedValue( p.param1, p.param2 ) );
		}
	}

	//Update data on local
	void UpdateAgentsLocal()
	{
		PlayerBase player = ( PlayerBase ) GetDayZGame().GetPlayer();
		
		if ( player && GetDayZGame().IsNewPlayer() )
		{
			//clear values
			m_PlayerAgentsSynced.Clear();
			
			//get agent pool data
			array<Param> agent_pool = new array<Param>;
			player.m_AgentPool.GetDebugObject( agent_pool );
			
			//get agents count 
			autoptr Param1<int> p1 = agent_pool.Get( 0 );
			int agents_count = p1.param1;
			
			//set synced data
			for ( int i = 0; i < agents_count; i++ ) 
			{
				autoptr Param2<string, string> p2 = agent_pool.Get( i + 1 );
				m_PlayerAgentsSynced.Insert( new SyncedValue( p2.param1, p2.param2.ToFloat() ) );
			}
		}
	}
}
