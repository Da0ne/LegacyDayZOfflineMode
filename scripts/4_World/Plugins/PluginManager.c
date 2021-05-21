class PluginManager
{	
	private array<typename>				m_PluginRegister;	// list of modules for creation
	private map<typename, PluginBase>	m_PluginsPtrs;		// plugin, plugin pointer
	
	//=================================
	// Constructor
	//=================================
	void PluginManager()
	{
		m_PluginRegister	= new array<typename>;
		m_PluginsPtrs		= new map<typename, PluginBase>;
	}
	
	//=================================
	// Destructor
	//=================================
	void ~PluginManager()
	{
		int i;
		PluginBase plugin;
		
		for ( i = 0; i < m_PluginsPtrs.Count(); ++i)
		{
			plugin = m_PluginsPtrs.GetElement(i);
			if ( plugin != NULL )
			{
				plugin.OnDestroy();
			}
		}
		
		for ( i = 0; i < m_PluginsPtrs.Count(); ++i)
		{
			plugin = m_PluginsPtrs.GetElement(i);
			if ( plugin != NULL )
			{
				delete plugin;
			}
		}
		
		delete m_PluginsPtrs;
		delete m_PluginRegister;
	}
	
	//=================================
	// Init
	//=================================
	void Init()
	{
		//----------------------------------------------------------------------
		// Register modules
		//----------------------------------------------------------------------
		//				Module Class Name 				Client	Server	Release(false = Only In Debug )
		//----------------------------------------------------------------------
		if (g_Game.m_OfflineMissions == 2)
		{
			RegisterPlugin( "PluginConfigViewer",				true, 	true, 	true );
			RegisterPlugin( "PluginHorticulture",				true, 	true, 	true );
			RegisterPlugin( "PluginRepairing",					true, 	true, 	true );
			RegisterPlugin( "PluginExperience",					true, 	true, 	true );
			RegisterPlugin( "PluginLocalSqfHistory",			true, 	true, 	true );
			RegisterPlugin( "PluginLocalEnscriptHistory",		true, 	true, 	true );
			RegisterPlugin( "PluginPlayerStatus",				true, 	true, 	true );
			RegisterPlugin( "PluginMessageManager",				true, 	true, 	true );
			RegisterPlugin( "PluginLifespan",					true, 	true, 	true );
			RegisterPlugin( "PluginVariables",					true, 	true, 	true );

			RegisterPlugin( "PluginKeyBinding",					true, 	false, 	true );
			RegisterPlugin( "PluginDeveloper",					true, 	true, 	true );
			RegisterPlugin( "PluginDeveloperSync",				true, 	true, 	true );
			RegisterPlugin( "PluginRecipesManager",				true, 	true, 	true );
			RegisterPlugin( "PluginSceneManager",				true, 	true, 	true );
			RegisterPlugin( "PluginConfigScene",				true, 	true, 	true );
			RegisterPlugin( "PluginConfigDebugProfile",			true, 	true, 	true );
			RegisterPlugin( "PluginConfigDebugProfileFixed",	true, 	true, 	true );
			RegisterPlugin( "PluginObjectsInteractionManager",	false, 	true, 	true );

			RegisterPlugin( "PluginTransmissionAgents",			true, 	true, 	false );
			RegisterPlugin( "PluginItemDiagnostic",				true, 	true, 	false );
			RegisterPlugin( "PluginDayzPlayerDebug",			true, 	true, 	false );
			RegisterPlugin( "PluginHologramDebug",				true, 	true, 	false );
		}
	}
	
	//=================================
	// PluginsInit
	//=================================
	void PluginsInit()
	{
		int i = 0;
		
		for ( i = 0; i < m_PluginRegister.Count(); ++i)
		{
			typename plugin_type = m_PluginRegister.Get(i);
			
			PluginBase module_exist = GetPluginByType( plugin_type );
			if ( module_exist )
			{
				m_PluginsPtrs.Remove( plugin_type );
				delete module_exist;
			}
			
			Class module_new = plugin_type.Spawn();
			m_PluginsPtrs.Set(plugin_type, module_new);
		}
		
		for ( i = 0; i < m_PluginsPtrs.Count(); ++i)
		{
			PluginBase plugin = m_PluginsPtrs.GetElement(i);
			if ( plugin != NULL )
			{
				plugin.OnInit();
			}
		}
	}
	
	/**
	\brief Returns registred plugin by class type, better is to use global funtion GetPlugin(typename plugin_type)
		\param module_tpye \p typename class type of plugin
		\return \p PluginBase
		@code
			PluginRepairing plugin = GetPluginManager().GetPluginByType(PluginRepairing);
		@endcode
	*/
	//=================================
	// GetPluginByType
	//=================================
	PluginBase GetPluginByType( typename plugin_type )
	{
		if ( m_PluginsPtrs.Contains( plugin_type ) )
		{
			return m_PluginsPtrs.Get( plugin_type );
		}
		
		return NULL;
	}
	
	/**
	\brief Register new PluginBase to PluginManager for storing and handling plugin.
		\param module_tpye \p typename class type of plugin
		\return \p void
		@code
			class PluginRepairing extends PluginBase
			{
			...
			}
			
			RegisterPlugin(PluginRepairing);
		@endcode
	*/
	//=================================
	// RegisterPlugin
	//=================================
	protected void RegisterPlugin( string plugin_class_name, bool reg_on_client, bool reg_on_server, bool reg_on_release )
	{
		if ( !reg_on_client )
		{
			if ( GetGame().IsClient() )
			{
				return;
			}
		}
		
		if ( !reg_on_server )
		{
			if ( GetGame().IsMultiplayer() )
			{
				if ( GetGame().IsServer() )
				{
					return;
				}
			}
		}
		
		if ( !reg_on_release )
		{
			if ( !GetGame().IsDebug() )
			{
				return;
			}
		}
		
		m_PluginRegister.Insert( plugin_class_name.ToType() );
	}
	
	//---------------------------------
	// UnregisterPlugin
	//---------------------------------
	protected bool UnregisterPlugin( string plugin_class_name )
	{
		typename plugin_type = plugin_class_name.ToType();
		
		if ( m_PluginRegister.Find( plugin_type ) != -1 )
		{
			m_PluginRegister.RemoveItem( plugin_type );
			return true;
		}
		
		return false;
	}
}

PluginManager g_Plugins;

/**
\brief Returns registred plugin by class type, better is to use global funtion GetPlugin(typename plugin_type)
	\param module_tpye \p typename class type of plugin
	\return \p PluginBase
	@code
		PluginRepairing plugin = GetPluginManager().GetPluginByType(PluginRepairing);
	@endcode
*/
PluginManager GetPluginManager()
{	
	if ( g_Plugins == NULL )
	{
		PluginManagerInit();
	}
	
	return g_Plugins;
}


void PluginManagerInit()
{	
	if (g_Plugins == NULL)
	{
		g_Plugins = new PluginManager;
		g_Plugins.Init();
		g_Plugins.PluginsInit();
	}
}

bool IsPluginManagerExists()
{
	if ( g_Plugins != NULL )
	{
		return true;
	}	
	
	return false;
}

PluginBase GetPlugin(local typename plugin_type)
{
	PluginBase plugin = GetPluginManager().GetPluginByType(plugin_type);
	
	if ( plugin == NULL )
	{
		PrintString("Module " + plugin_type.ToString() + " is not Registred in PluginManager.c!");
		DumpStack();
		return NULL;
	}
	
	return plugin;
}

bool IsModuleExist(local typename plugin_type)
{
	if ( IsPluginManagerExists() )
	{
		return ( GetPlugin(plugin_type) != NULL );
	}
	
	return false;
}

void DeletePlugins()
{
	delete g_Plugins;
	g_Plugins = NULL;
}