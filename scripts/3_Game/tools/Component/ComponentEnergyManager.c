//-----------------------------
// ENERGY MANAGER
//-----------------------------
/*
Author: Boris Vacula

Documentation can be found at DayZ Confluence >> Camping & Squatting >> Electricity >> Energy Manager functionalities

This system controls storage, spending and sharing of energy between entities

Every EntityAI object which uses this API gains these functions:
	-It can store some amout of energy
	-It can use this amount of energy for any kind of functionality
	-It can share this energy with other devices plugged into it
	-It will have an ON/OFF switch
*/

// TO DO: Doxygen komentar ^

class ComponentEnergyManager : Component
{
	protected static bool 			m_EnableDebug = true; // Use this to toggle visualisation of plug connections
	protected 		Shape			m_DebugPlugArrow;
	
	protected 		bool			m_IsSwichedOn;
	protected 		bool			m_CfgAlwaysSwitchedOn;
	protected 		bool			m_IsPassiveDevice;
	protected 		bool			m_IsWorking;
	protected 		bool			m_CanStopWork;
	protected 		bool			m_RestorePlugState; // After server restart, this value reports if this device was plugged into something or not at the end of last session.
	protected 		bool			m_AutoSwitchOff;
	
	protected 		int				m_EnergySourceSocketID = -1;
	protected 		int				m_PlugType;
	protected 		int				m_EnergySourceIDLow;  // Persistent ID
	protected 		int				m_EnergySourceIDHigh; // persistent ID
	protected 		int				m_AttachmentActionType;
	
	protected 		float			m_EnergyUsage;
	protected 		float			m_Energy;
	protected 		float			m_EnergyStorageMax;
	protected 		float			m_ReduceMaxEnergyByDamageCoef;
	protected 		float			m_SocketsCount;
	protected 		float			m_CordLength;
	protected 		float			m_LastUpdateTime;
	
	protected 		string			m_CordTextureFile;
	
	// Concatenated strings for p3d selections
	protected static const string	SOCKET_ 			= "socket_";
	protected static const string	_PLUGGED 			= "_plugged";
	protected static const string	_AVAILABLE 			= "_available";
	
	// Selections
	static const string				SEL_CORD_PLUGGED 	= "cord_plugged";
	static const string				SEL_CORD_FOLDED		= "cord_folded";
	
	protected autoptr TIntArray		m_CompatiblePlugTypes;
	EntityAI						m_EnergySource; // Energy source can be any EntityAI object
	autoptr array<EntityAI> 		m_PluggedDevices;
	autoptr map<string,EntityAI>	m_DeviceByPlugSelection;
	
	autoptr Timer 					m_UpdateTimer;
	autoptr Timer 					m_DebugUpdate;
	
	const int MAX_SOCKETS_COUNT = 10;
	EntityAI m_Sockets[MAX_SOCKETS_COUNT];
	
	

	// Constructor
	void ComponentEnergyManager()
	{
		m_UpdateTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
		
		if (m_EnableDebug)
		{
			m_DebugUpdate = new Timer( CALL_CATEGORY_GAMEPLAY );
		}
	}

	// Destructor
	void ~ComponentEnergyManager()
	{
		delete m_UpdateTimer;
		
		if (m_EnableDebug)
		{
			m_DebugUpdate.Stop();
			delete m_DebugUpdate;	
		}
	}

	// Initialization. Energy Manager is ready.
	void Event_OnInit()
	{
		if (m_EnableDebug)
		{
			m_DebugUpdate.Run(0.01, this, "DebugUpdate", NULL, true);
		}
		
		GetGame().GameScript.CallFunction(m_ThisEntityAI, "OnInitEnergy", NULL, 0);
	}
	
	// Update debug arrows
	void DebugUpdate()
	{
		if (m_DebugPlugArrow)
		{
			Debug.RemoveShape(m_DebugPlugArrow);
			m_DebugPlugArrow = NULL; 
		}
		
		if ( IsPlugged() )
		{
			vector from = GetEnergySource().GetPosition() + "0 0.1 0";
			vector to = m_ThisEntityAI.GetPosition() + "0 0.1 0";
			
			if ( m_ThisEntityAI.GetType() == "EN5C_BarbedWire" ) // Special case for debugging of electric fences. Temporal code until offsets in fences are fixed.
			{
				EntityAI BBB = m_ThisEntityAI.GetOwner();
				
				if ( BBB  &&  BBB.GetType() == "EN5C_Fence" )
				{
					to = to + "0 -1.3 0";
				}
			}
			
			m_DebugPlugArrow = Debug.DrawArrow( from, to );
		}
	}

	// Prepare everything
	void Event_OnAwake()
	{
		if ( GetGame().IsServer() )
		{
			string param_access_energy_sys = "CfgVehicles " + m_ThisEntityAI.GetType() + " EnergyManager ";
			
			// Read all config parameters
			m_EnergyUsage					= GetGame().ConfigGetFloat (param_access_energy_sys + "energyUsagePerSecond");
			m_CfgAlwaysSwitchedOn			= GetGame().ConfigGetFloat (param_access_energy_sys + "alwaysOn");
			m_AutoSwitchOff					= GetGame().ConfigGetFloat (param_access_energy_sys + "autoSwitchOff");
			
			m_Energy						= GetGame().ConfigGetFloat (param_access_energy_sys + "energyAtSpawn");
			m_EnergyStorageMax				= GetGame().ConfigGetFloat (param_access_energy_sys + "energyStorageMax");
			m_ReduceMaxEnergyByDamageCoef	= GetGame().ConfigGetFloat (param_access_energy_sys + "reduceMaxEnergyByDamageCoef");
			m_SocketsCount					= GetGame().ConfigGetFloat (param_access_energy_sys + "powerSocketsCount");
			
			m_IsPassiveDevice				= GetGame().ConfigGetFloat (param_access_energy_sys + "isPassiveDevice");
			m_CordLength 					= GetGame().ConfigGetFloat (param_access_energy_sys + "cordLength");
			m_PlugType 						= GetGame().ConfigGetFloat (param_access_energy_sys + "plugType");
			
			m_AttachmentActionType			= GetGame().ConfigGetFloat (param_access_energy_sys + "attachmentAction");
			
			// If energyAtSpawn is present, then use its value for energyStorageMax if that cfg param is not present (for convenience's sake)
			string cfg_check_energy_limit = param_access_energy_sys + "energyStorageMax";
			
			if ( !GetGame().ConfigIsExisting (cfg_check_energy_limit)  &&  m_Energy > 0)
			{
				m_EnergyStorageMax = m_Energy;
			}
			
			// Fill m_CompatiblePlugTypes
			string cfg_check_plug_types = param_access_energy_sys + "compatiblePlugTypes";
			
			if ( GetGame().ConfigIsExisting (cfg_check_plug_types) )
			{
				m_CompatiblePlugTypes = new TIntArray;
				GetGame().ConfigGetIntArray(cfg_check_plug_types, m_CompatiblePlugTypes);	
			}
			
			m_PluggedDevices = new array<EntityAI>;
			
			if ( m_CordLength < 0 )
			{
				m_CordLength = 0;
				string error_message_cord = "Warning! " + m_ThisEntityAI.GetType() + ": config parameter 'cordLength' is less than 0! Cord length should not be negative!";
				DPrint(error_message_cord);
			}
			
			m_DeviceByPlugSelection = new map<string,EntityAI>;
			
			// Prepare the m_DeviceByPlugSelection
			string cfg_animation_sources = "cfgVehicles " + m_ThisEntityAI.GetType() + " " + "AnimationSources ";
			int animation_sources_count = GetGame().ConfigGetChildrenCount(cfg_animation_sources);
			
			for (int i_selection = 0; i_selection < animation_sources_count; i_selection++)
			{
				// TO DO: This could be optimized so not all selections on item are considered as plug/socket selections.
				local string selection;
				GetGame().ConfigGetChildName(cfg_animation_sources, i_selection, selection);
				m_DeviceByPlugSelection.Set(selection, NULL);
			}
			
			// Prepare sockets
			if ( m_SocketsCount > MAX_SOCKETS_COUNT )
			{
				m_SocketsCount = MAX_SOCKETS_COUNT;
				string error_message_sockets = "Error! " + m_ThisEntityAI.GetType() + ": config parameter 'powerSocketsCount' is higher than the current limit (" + MAX_SOCKETS_COUNT.ToString() + ")! Raise the limit (constant MAX_SOCKETS_COUNT) or decrease the powerSocketsCount parameter for this device!";
				DPrint(error_message_sockets);
			}
			
			m_Sockets[MAX_SOCKETS_COUNT]; // Handles selections for plugs in the sockets. Feel free to change the limit if needed.
			
			GetGame().ConfigGetText(param_access_energy_sys + "cordTextureFile", m_CordTextureFile); 
			
			if ( IsAlwaysSwitchedOn() )
			{
				StartUpdates();
			}
		}
		
		for ( int i = 0; i <= GetSocketsCount(); ++i )
		{
			m_ThisEntityAI.HideSelection ( SOCKET_ + i.ToString() + _PLUGGED );
		}
		
		m_ThisEntityAI.HideSelection ( SEL_CORD_PLUGGED );
	}

	// Returns the type of this component
	int GetCompType()
	{
		return COMP_TYPE_ENERGY_MANAGER;
	}
	
	
	
	

	//======================================================================================
	// 									 PUBLIC FUNCTIONS
	// 						Use these to control the Energy Manager
	// Functions are in order of their return value: void, bool, int, float, string, array.
	//======================================================================================

	//! Energy manager: Switches ON the device. If it has enough energy to consume, it will start doing its work. Otherwise it won't do any work, until it becomes powered.
	void SwitchOn()
	{
		if ( CanSwitchOn() )
		{		
			m_IsSwichedOn = true;
			
			DeviceUpdate(); // 'Wake up' this device now
			StartUpdates();
			
			// 'Wakes up' all connected devices
			WakeUpWholeBranch( m_ThisEntityAI );
			
			// Call event
			GetGame().GameScript.CallFunction(m_ThisEntityAI, "OnSwitchOn", NULL, 0);
		}
	}

	//! Energy manager: Switches OFF the device.
	void SwitchOff()
	{
		if ( CanTurnOff() )
		{
			m_IsSwichedOn = false;
			
			if ( IsWorking() )
			{
				StopUpdates();
				DeviceUpdate();
			}
			
			// 'Wakes up' all connected devices
			WakeUpWholeBranch( m_ThisEntityAI );
			
			// Call event
			GetGame().GameScript.CallFunction(m_ThisEntityAI, "OnSwitchOff", NULL, 0);
		}
	}

	//! Energy manager: Changes the status of this device. When it's passive (true), the main timer and OnWork events are not used.
	void SetPassiveState(bool state = true)
	{
		m_IsPassiveDevice = state;
		if ( !m_IsPassiveDevice )
		{
			DeviceUpdate();
		}
	}

	//! Energy manager: Unplugs the given device from this one.
	void UnplugDevice(EntityAI device_to_unplug)
	{
		if ( GetGame().IsServer() )
		{
			int index_start	= GetPluggedDevices().Count() - 1;
			bool device_found = false;
			
			for ( int i = index_start; i >= 0; --i )
			{
				EntityAI plugged_device = GetPluggedDevices().Get(i);
				
				if (plugged_device == device_to_unplug)
				{
					GetPluggedDevices().Remove(i);
					device_found = true;
					break;
				}
			}
			
			if (device_found)
			{
				int socket_ID = device_to_unplug.GetCompEM().GetEnergySourceSocketID();
				UnplugCordFromSocket( socket_ID );
				device_to_unplug.GetCompEM().SetEnergySource(NULL);
				device_to_unplug.GetCompEM().DeviceUpdate();
				device_to_unplug.GetCompEM().StartUpdates();
				device_to_unplug.GetCompEM().WakeUpWholeBranch( m_ThisEntityAI );
				
				if (m_EnableDebug)
				{
					Debug.DestroyAllShapes();
					m_DebugPlugArrow = NULL;
				}
				
				OnOwnSocketReleased(device_to_unplug);
				device_to_unplug.GetCompEM().OnIsUnplugged( m_ThisEntityAI );
				device_to_unplug.ShowSelection ( SEL_CORD_FOLDED );
				device_to_unplug.HideSelection ( SEL_CORD_PLUGGED );
			}
		}
	}
	
	// Used only for storing of the plug's state through server restart
	void RestorePlugState(bool state)
	{
		m_RestorePlugState = state;
	}

	//! Energy manager: Sets stored energy for this device. It ignores the min/max limit!
	void SetEnergy(float new_energy)
	{
		m_Energy = new_energy;
	}

	//! Energy manager: Unplugs this device from its power source
	void UnplugThis()
	{
		if ( GetGame().IsServer() )
		{
			if ( GetEnergySource() )
			{
				GetEnergySource().GetCompEM().UnplugDevice(m_ThisEntityAI);
			}
		}
	}
	
	//! Energy manager: Unplugs everything directly connected to this device
	void UnplugAllDevices()
	{
		if ( GetPluggedDevices() ) // This check is necesarry in case this function is called before initialization
		{
			int index_start	= GetPluggedDevices().Count() - 1;
			for ( int i = index_start; i >= 0; --i )
			{
				UnplugDevice( GetPluggedDevices().Get(i) );
			}
		}
	}

	//! Energy manager: Shows/Hides all selections this system works with. Call this if something is wrong with selections (like during Init and Restore event in config)
	void UpdateSelections()
	{
		// Lets update sockets, if there are any
		int slots_c = GetSocketsCount();
		
		for ( local int i = 0; i < slots_c; ++i )
		{
			local EntityAI plug_owner = m_Sockets[i];
			
			if ( plug_owner )
			{
				string plugged_selection = SOCKET_ + (i+1).ToString() + _PLUGGED;
				string available_selection = SOCKET_ + (i+1).ToString() + _AVAILABLE;
				m_ThisEntityAI.ShowSelection ( plugged_selection );
				m_ThisEntityAI.HideSelection ( available_selection );
				local string texture_path = plug_owner.GetCompEM().GetCordTextureFile();
				int selection_index = GetHiddenSelectionIndex( plugged_selection );
				m_ThisEntityAI.SetObjectTexture(selection_index, texture_path );
			}
			else
			{
				m_ThisEntityAI.ShowSelection ( SOCKET_ + (i+1).ToString() + _AVAILABLE );
				m_ThisEntityAI.HideSelection ( SOCKET_ + (i+1).ToString() + _PLUGGED );
			}
		}
		
		// Now lets update the cord/plug state
		if ( GetEnergySource() )
		{
			m_ThisEntityAI.ShowSelection ( SEL_CORD_PLUGGED );
			m_ThisEntityAI.HideSelection ( SEL_CORD_FOLDED );
		}
		else
		{
			m_ThisEntityAI.ShowSelection ( SEL_CORD_FOLDED );
			m_ThisEntityAI.HideSelection ( SEL_CORD_PLUGGED );
		}
	}

	//! Energy manager: Unplugs this device when it's necesarry
	void UpdatePlugState()
	{
		if ( m_ThisEntityAI.GetOwnerPlayer()  &&  m_ThisEntityAI.GetOwnerPlayer().IsKindOf("SurvivorBase") )
		{
			EntityAI player = m_ThisEntityAI.GetOwnerPlayer();
			
			// Measure distance from the player
			vector pos_player = player.GetPosition();
			if ( ! IsEnergySourceAtReach( pos_player , 5 )  )
			{
				UnplugThis();
			}
		}
		else
		{
			// Measure distance from the device
			vector pos_item = m_ThisEntityAI.GetPosition();
			
			if ( m_ThisEntityAI.GetOwner() )
			{
				pos_item = m_ThisEntityAI.GetOwner().GetPosition();
			}
			
			if ( ! IsEnergySourceAtReach( pos_item )  )
			{
				UnplugThis();
			}
		}
	}

	// Returns an array of plug types this device can accept
	void GetCompatiblePlugTypes(out TIntArray IDs)
	{
		IDs = m_CompatiblePlugTypes;
	}

	// Stores IDs of the energy source.
	void StoreEnergySourceIDs(int ID_low, int ID_high)
	{
		m_EnergySourceIDLow = ID_low;
		m_EnergySourceIDHigh = ID_high;
	}
	
	
	
	
	
	// Returns true if this device was plugged into something at the end of previous session
	bool GetRestorePlugState()
	{
		return m_RestorePlugState;
	}
	
	//! Energy manager: Plugs this device into the given one. Returns true if that was successfull, or false if not (due to plug incompatibility or no free socket on the receiver).
	bool PlugThisInto(EntityAI energy_source)
	{
		return energy_source.GetCompEM().PlugInDevice(m_ThisEntityAI);
	}
	
	//! Energy manager: Checks if the device can be turned ON
	bool CanSwitchOn()
	{
		if ( !IsSwitchedOn()  &&  m_ThisEntityAI.GetDamage() < 1 )
		{
			return true;
		}
		
		return false;
	}

	//! Energy manager: Predicts if this device will do work when it will be switched on. Use the optional parameter test_energy to overwrite the default amount of energy consumption.
	bool CanWork( float test_energy = -1)
	{
		// Check if the (serially connected) power source(s) can provide needed energy.
		local float 	energy_usage = test_energy;
		local float 	gathered_energy = GetEnergy();
		local EntityAI	energy_source = GetEnergySource();
		
		if (energy_usage == -1)
		{
			energy_usage = GetEnergyUsage();
		}
		
		while ( gathered_energy < energy_usage ) // Look for energy source if we don't have enough energy
		{
			if ( energy_source  &&  energy_source.GetDamage() < 1  &&  energy_source.GetCompEM().IsSwitchedOn() )
			{
				gathered_energy = gathered_energy + energy_source.GetCompEM().GetEnergy();
				energy_source = energy_source.GetCompEM().GetEnergySource();
			}
			else
			{
				// No power source, no energy.
				return false;
			}
		}
		
		// Enough energy was found
		return true;
	}

	//! Energy manager: Checks if the device can be turned OFF
	bool CanSwitchOff()
	{
		if ( IsAlwaysSwitchedOn() )
		{
			return false;
		}
		
		return IsSwitchedOn();
	}

	//! Energy manager: Returns state of the switch. Whenever the device is working is not accounted for. Use IsWorking() for that.
	bool IsSwitchedOn()
	{
		if ( IsAlwaysSwitchedOn() )
		{
			return true;
		}
		
		return m_IsSwichedOn;
	}

	//! Energy manager: Returns config value EnergyManager -> isAlwaysOn
	bool IsAlwaysSwitchedOn()
	{
		return m_CfgAlwaysSwitchedOn;
	}

	//! Energy manager: Returns true if the cord of this device is folded. Returns false if it's plugged.
	bool IsCordFolded()
	{
		if ( IsPlugged() )
			return false;
		
		return true;
	}
	
	//! Energy manager: Returns true if this device is set to be passive. False if otherwise.
	bool IsPassive()
	{
		return m_IsPassiveDevice;
	}

	//! Energy manager: Returns true if this device is plugged into some other device (even if they are OFF or ruined). Otherwise it returns false.
	bool IsPlugged()
	{
		if ( GetEnergySource() != NULL )
		{
			return true;
		}
		
		return false;	
	}
	
	
	//! Energy manager: Consumes the given amount of energy. If there is not enough of stored energy in this device, then it tries to take it from its power source, if any exists. Returns true if the requested amount of energy was consumed. Otherwise it returns false.
	bool ConsumeEnergy(local float amount)
	{
		return FindAndConsumeEnergy(amount, true);
	}

	//! Energy manager: Checks if the device can be turned OFF (Use this in User Actions as a condition)
	bool CanTurnOff()
	{
		if ( IsAlwaysSwitchedOn() )
		{
			return false;
		}
		
		return IsSwitchedOn();
	}

	//! Energy manager: Returns true if this device has consumed enough needed energy to do its work in this cycle.
	bool IsWorking()
	{
		return m_IsWorking;
	}

	//! Energy manager: Returns true if this device has enough of stored energy for its own use.
	bool HasEnoughStoredEnergy()
	{
		if ( GetEnergy() > GetEnergyUsage() )
		{
			return true;
		}
		
		return false;
	}

	//! Energy manager: Returns true if this device has a free socket to receive another plug
	bool HasFreeSocket()
	{
		int plugged_devices = GetPluggedDevices().Count();
		int plugged_devices_limit = GetSocketsCount();
		
		if ( plugged_devices < plugged_devices_limit )
		{
			return true;
		}
		
		return false;
	}

	//! Energy manager: Returns true if this device can receive power plug of the other device.
	bool CanReceivePlugFrom( EntityAI device_to_plug )
	{
		if ( HasFreeSocket()  &&  device_to_plug != m_ThisEntityAI  &&  device_to_plug.GetCompEM().GetEnergySource() != m_ThisEntityAI )
		{
			if ( device_to_plug.GetCompEM().GetPlugType() == PLUG_UNDEFINED )
			{
				return true; // When plugType is undefined in config then make it compatible.
			}
			
			if ( m_CompatiblePlugTypes )
			{
				for ( local int i = 0; i < m_CompatiblePlugTypes.Count(); i++ )
				{
					local int plugID = m_CompatiblePlugTypes.Get(i);
					if ( plugID == device_to_plug.GetCompEM().GetPlugType() )
					{
						return true;
					}
				}
			}
			else
			{
				// Since the config parameter compatiblePlugTypes is not present, then accept all plugs for simplicity's sake
				return true;
			}
		}
		
		return false;
	}

	/**
	\brief Energy manager: Returns true/false if this device's virtual power cord can/cannot reach its energy source at the given position.
		\param from_position \p vector position from where the measurement will be taken
		\param add_tolerance \p float parameter will add to the cord's length in meters (optional)
		\return \p bool True if device will be at reach. False if not, or is not plugged, or is not an electricity device.
		@code
			vector position_player = GetGame().GetPlayer().GetPosition();
			
			if ( my_device.GetCompEM().IsEnergySourceAtReach( position_player )  )
			{
				Print("device is at reach!");
			}else{
				Print("device is NOT at reach!");
			}
		@endcode
	*/
	bool IsEnergySourceAtReach( vector from_position, float add_tolerance = 0 )
	{
		if ( !IsPlugged() )
		{
			return false;
		}
		
		if ( GetCordLength() == 0 ) // 0 is an exception, which means infinitely long cable.
		{
			return true;
		}
		
		EntityAI energy_source = GetEnergySource();
		
		if (!energy_source)
			return false;
		
		vector source_pos = energy_source.GetPosition();
		float distance = vector.Distance( from_position, source_pos );
		
		if ( distance > GetCordLength() + add_tolerance )
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	
	
	
	// Returns index of the string found in cfg array 'hiddenSelections'
	int GetHiddenSelectionIndex( string selection )
	{
		autoptr array<string> config_selections	= new array<string>;
		string cfg_hidden_selections = "CfgVehicles" + " " + m_ThisEntityAI.GetType() + " " + "hiddenSelections";
		GetGame().ConfigGetTextArray ( cfg_hidden_selections, config_selections );
		
		for (int i = 0; i < config_selections.Count(); ++i)
		{
			if ( config_selections.Get ( i ) == selection )
			{
				return i;
			}
		}
		
		return -1;
	}
	
	//! Energy manager: Returns the count of power sockets (whenever used or not)
	int GetSocketsCount()
	{
		return m_SocketsCount;
	}

	//! Energy manager: Returns plug type. Check \DZ\data\basicDefines.hpp  OR  \Scripts\Classes\Component\_constants.h files for types of plugs
	int GetPlugType()
	{
		return m_PlugType;
	}

	// Returns the action ID which is supposed to be done upon receiving an attachment
	int GetAttachmentAction()
	{
		return m_AttachmentActionType;
	}

	// Returns persistent ID (low) of the energy source
	int GetEnergySourceIDLow()
	{
		return m_EnergySourceIDLow;
	}

	// Returns persistent ID (high) of the energy source
	int GetEnergySourceIDHigh()
	{
		return m_EnergySourceIDHigh;
	}
	
	//! Energy manager: Returns the number of devices plugged into this one.
	int GetPluggedDevicesCount()
	{
		return GetPluggedDevices().Count();
	}
	



	//! Energy manager: Returns the number of energy this device needs to run itself (See its config >> energyUsagePerSecond)
	float GetEnergyUsage()
	{
		return m_EnergyUsage;
	}

	//! Energy manager: Returns the amount of stored energy this device has
	float GetEnergy()
	{
		return m_Energy;
	}

	//! Energy manager: Adds energy to this device and clamps it within its min/max storage limits. Returns the amount of energy that was clamped. Negative value is supported, but you should still use ConsumeEnergy(...) for propper consumption of energy.
	float AddEnergy(float added_energy)
	{
		if ( GetGame().IsServer() )
		{
			float energy_to_clamp = GetEnergy() + added_energy;
			float clamped_energy = Math.Clamp( energy_to_clamp, 0, GetEnergyMax() );
			SetEnergy(clamped_energy);
			StartUpdates();
			return energy_to_clamp - clamped_energy;
		}
		
		return 0;
	}

	//! Energy manager: Returns the maximum amount of energy this device can store while taking its damage into account.
	float GetEnergyMax()
	{
		// Removed because in 0.62 we don't use the new damage system!
		/*
		if (GetGame().IsServer())
		{
			float max_health = m_ThisEntityAI.GetMaxHealth("","");
			float health = m_ThisEntityAI.GetHealth("","");
			
			if (max_health == 0  ||  m_ReduceMaxEnergyByDamageCoef == 0)
				return m_EnergyStorageMax;
			
			float damage_coef = 1 - (health / max_health);
			
			return m_EnergyStorageMax * (1 - ( damage_coef * m_ReduceMaxEnergyByDamageCoef ) );
		}
		*/
		
		return m_EnergyStorageMax;
		//return 0;
	}



	//! Energy manager: Returns the energy source this device is plugged into
	EntityAI GetEnergySource()
	{
		return m_EnergySource;
	}

	//! Energy manager: Returns the device to which the given plug selection belongs to
	EntityAI GetPlugOwner(string plug_selection_name)
	{
		if( m_DeviceByPlugSelection.Contains(plug_selection_name) )
		{
			return m_DeviceByPlugSelection.Get(plug_selection_name);
		}
		
		return NULL;
	}
	
	//! Energy manager: Returns a device which is plugged into this one. If there are more devices plugged into this, then it returns the first one that is found.
	EntityAI GetPluggedDevice()
	{
		if ( !GetGame().IsServer() )
			return NULL;
		
		if ( GetPluggedDevices().Count() > 0 )
			return GetPluggedDevices().Get(0);
		
		return NULL;
	}
	



	//! Energy manager: Returns path to the cord texture file.
	string GetCordTextureFile()
	{
		return m_CordTextureFile;
	}



	//! Energy manager: Returns an array of devices which are plugged into this one
	array<EntityAI> GetPluggedDevices()
	{
		return m_PluggedDevices;
	}



	//! Energy manager: Returns an array of devices which are plugged into this one and are turned on
	array<EntityAI> GetPoweredDevices()
	{
		array<EntityAI> return_array = new array<EntityAI>;
		int plugged_devices_c = GetPluggedDevices().Count();
		for ( local int i = 0; i < plugged_devices_c; ++i )
		{
			local EntityAI device = GetPluggedDevices().Get(i);
			if ( IsSwitchedOn() )
			{
				return_array.Insert(device);
			}
		}
		
		return return_array;
	}
	
	
	
	
	/*===================================
				PUBLIC EVENTS
	===================================*/

	// Called every device update if its supposed to do some work. The update can be every second or at random, depending on its manipulation.
	void OnWork( float consumed_energy ) {m_ThisEntityAI.OnWork(consumed_energy);}

	// Called when this device is plugged into some energy source
	void OnIsPlugged(EntityAI source_device) {m_ThisEntityAI.OnIsPlugged(source_device);}

	// Called when this device is UNPLUGGED from the energy source
	void OnIsUnplugged( EntityAI last_energy_source ) {m_ThisEntityAI.OnIsUnplugged( last_energy_source );}

	// When something is plugged into this device
	void OnOwnSocketTaken( EntityAI device ) {	m_ThisEntityAI.OnOwnSocketTaken(device);}

	// When something is UNPLUGGED from this device
	void OnOwnSocketReleased( EntityAI device ) {m_ThisEntityAI.OnOwnSocketReleased( device );}
	
	
	// Handles automatic attachment action
	void OnAttachmentAdded(EntityAI elec_device)
	{
		int attachment_action_type = GetAttachmentAction();
		
		if ( attachment_action_type == PLUG_THIS_INTO_ATTACHMENT )
		{
			if ( elec_device.GetCompEM().CanReceivePlugFrom( m_ThisEntityAI ) )
			{
				PlugThisInto(elec_device);
			}
		}
		else if ( attachment_action_type == PLUG_ATTACHMENTS_INTO_THIS )
		{
			elec_device.GetCompEM().PlugThisInto(m_ThisEntityAI);
		}
	}

	// Handles automatic detachment action
	void OnAttachmentRemoved(EntityAI elec_device)
	{
		int attachment_action_type = GetAttachmentAction();
		
		if ( attachment_action_type == PLUG_THIS_INTO_ATTACHMENT )
		{
			if ( elec_device == GetEnergySource() )
			{
				UnplugThis();
			}
		}
		else if ( attachment_action_type == PLUG_ATTACHMENTS_INTO_THIS )
		{
			elec_device.GetCompEM().UnplugThis();
		}
	}




	/*===================================
			 PROTECTED FUNCTIONS
	===================================*/

	// Starts the device's main cycle
	protected void StartUpdates()
	{
		if ( !m_IsPassiveDevice )
		{
			if ( !m_UpdateTimer.IsRunning() ) // Makes sure the timer is NOT running already
			{
				m_UpdateTimer.Run(CONSUME_ENERGY_INTERVAL, this, "DeviceUpdate", NULL, true);
			}
		}
	}

	// Stops the device's main cycle
	protected void StopUpdates()
	{
		m_UpdateTimer.Stop();
	}

	// 'Wakes up' all devices down the network so they start working, if they have enough power, and are switched ON
	protected void WakeUpWholeBranch( EntityAI original_caller )
	{
		if ( GetGame().IsServer() )
		{
			if ( GetSocketsCount() > 0 )
			{
				local array<EntityAI> plugged_devices = GetPluggedDevices();
				local int plugged_devices_c = plugged_devices.Count();
				
				for ( local int i = 0; i < plugged_devices_c; ++i )
				{
					local EntityAI device = plugged_devices.Get(i);
					if ( device != original_caller ) // original_caller check here prevents infinite loops
					{
						device.GetCompEM().DeviceUpdate();
						device.GetCompEM().StartUpdates();
						device.GetCompEM().WakeUpWholeBranch( original_caller );
					}
				}
			}
		}
	}

	// Finds an available socket and plugs the given device into it.
	// This is only about visualisation.
	protected void PlugCordIntoSocket( EntityAI device_to_plug )
	{
		int slots_c = GetSocketsCount();
		
		for ( int i = 0; i < slots_c; ++i )
		{
			local EntityAI plug_owner = m_Sockets[i];
			
			if ( !plug_owner ) // Check if this socket is available
			{
				// plug the consumptor into this socket!
				m_Sockets[i] = device_to_plug;
				
				local string plugged_selection = SOCKET_ + (i+1).ToString() + _PLUGGED;
				SetPlugOwner( plugged_selection, device_to_plug );
				m_ThisEntityAI.ShowSelection ( plugged_selection );
				
				local string unplugged_selection = SOCKET_ + (i+1).ToString() + _AVAILABLE;
				m_ThisEntityAI.HideSelection ( unplugged_selection );
				local string texture_path = device_to_plug.GetCompEM().GetCordTextureFile();
				int selection_index = GetHiddenSelectionIndex( plugged_selection );
				m_ThisEntityAI.SetObjectTexture( selection_index, texture_path );
				device_to_plug.GetCompEM().SetEnergySourceSocketID(i);
				break;
			}
		}
	}

	// Sets energy source for this device
	protected void SetEnergySource( EntityAI source )
	{	
		m_EnergySource = source;
		
		if (source)
		{		
			StartUpdates();
			OnIsPlugged(source);
		}
	}

	// Plugs the given device into this one
	protected bool PlugInDevice(EntityAI device_to_plug)
	{
		if ( GetGame().IsServer() )
		{
			if ( CanReceivePlugFrom(device_to_plug) )
			{
				if ( device_to_plug.GetCompEM().IsPlugged() )
					device_to_plug.GetCompEM().UnplugThis();
				
				GetPluggedDevices().Insert( device_to_plug );
				device_to_plug.GetCompEM().SetEnergySource(m_ThisEntityAI);
				WakeUpWholeBranch( m_ThisEntityAI );
				device_to_plug.HideSelection ( SEL_CORD_FOLDED );
				device_to_plug.ShowSelection ( SEL_CORD_PLUGGED );
				PlugCordIntoSocket( device_to_plug ); // Visualisation
				OnOwnSocketTaken( device_to_plug );
				
				return true;
			}
			else
			{ 
				// The receiving device cannot accept this plug. Figure out why:
				
				string this_str = m_ThisEntityAI.GetType();
				string device_str = device_to_plug.GetType();
				
				if ( GetPlugType() != device_to_plug.GetCompEM().GetPlugType() )
				{
					// Technically, plug compatibility is not a problem. However, having incompatible devices connected might cause issues elsewhere, so that's why it is forbidden here.
					DPrint("WARNING! Attempt at plugging " + device_str + " into " + this_str + " has failed due to incompatible plug! To make the plug compatible, include its type in the receiving device's config (array parameter 'compatiblePlugTypes')");
				}
				
				if ( m_ThisEntityAI == device_to_plug.GetCompEM().GetEnergySource() )
				{
					DPrint("WARNING! Attempt at plugging " + device_str + " into " + this_str + " has encountered a problem. These devices are already connected! Solution: Don't plug a device into its current power source twice.");
				}
				
				if ( GetPluggedDevicesCount() >= GetSocketsCount() )
				{
					DPrint("WARNING! Attempt at plugging " + device_str + " into " + this_str + " has failed! The receiver has no free socket for another plug!");
				}
				
				return false;
			}
		}
	}

	// Sets the device to which the given plug selection belongs to
	protected void SetPlugOwner(string selection_name, EntityAI device)
	{
		if( m_DeviceByPlugSelection.Contains(selection_name) )
		{
			m_DeviceByPlugSelection.Set(selection_name, device);
		}
	}
	
	// Frees the given socket.
	// This is only about visualisation.
	protected void UnplugCordFromSocket( int socket_to_unplug_ID )
	{
		EntityAI plug_owner = m_Sockets[socket_to_unplug_ID];
		
		if ( plug_owner )
		{
			m_Sockets[socket_to_unplug_ID] = NULL;
			local string unplugged_selection = SOCKET_ + (socket_to_unplug_ID+1).ToString() + _AVAILABLE;
			m_ThisEntityAI.ShowSelection ( unplugged_selection );
			
			local string plugged_selection = SOCKET_ + (socket_to_unplug_ID+1).ToString() + _PLUGGED;
			m_ThisEntityAI.HideSelection ( plugged_selection );
			SetPlugOwner( plugged_selection, NULL );
			plug_owner.GetCompEM().SetEnergySourceSocketID(-1);
		}
	}

	// Sets the state of the device
	protected void SetPowered( bool state )
	{
		m_IsWorking = state;
	}
	
	// Tries to consume the given amount of energy. If there is none in this device, then it tries to take it from some power source.
	protected bool FindAndConsumeEnergy(local float amount, local bool ignore_switch_state = false)
	{
		if ( GetGame().IsServer() )
		{
			if ( (ignore_switch_state  ||  IsSwitchedOn())  &&  m_ThisEntityAI.GetDamage() < 1)
			{
				local float available_energy = AddEnergy(-amount);
				
				if ( available_energy < 0  &&  GetEnergySource() )
				{
					// This devices does not has enough of stored energy, therefore it will take it from its power source (which can be a chain of cable reels)
					return GetEnergySource().GetCompEM().FindAndConsumeEnergy( -available_energy );
				}
				
				if ( available_energy >= 0)
				{
					return true;
				}
				
				return false;
			}
			else
			{
				return false;
			}
		}
	}
	
	// Gets the socket ID this device is powered from.
	protected int GetEnergySourceSocketID()
	{
		return m_EnergySourceSocketID;
	}



	// Returns the length of the device's virtual cord.
	protected float GetCordLength()
	{
		return m_CordLength;
	}



	// Sets the socket ID this device is plugged into.
	protected void SetEnergySourceSocketID( int slot_ID )
	{
		m_EnergySourceSocketID = slot_ID;
	}

	// Updates the device's state of power. This function is visualized in the diagram at DayZ Confluence >> Camping & Squatting >> Electricity >> Energy Manager functionalities
	protected void DeviceUpdate()
	{
		//vector pos = m_ThisEntityAI.GetPosition();
		//string debug_message = "Object " + m_ThisEntityAI.GetType() + "  |  Game time: " + ftoa(GetGame().GetTime()) + "  |  Energy: " + ftoa( GetEnergy() ) + "  |  IsAtReach: " + (IsEnergySourceAtReach(pos)).ToString();
		//Print(debug_message);
		
		if ( GetGame().IsServer()  &&  !m_IsPassiveDevice )
		{
			// 'm_ThisEntityAI' and 'this' must be checked because this method is caled from a timer
			if ( m_ThisEntityAI  &&  this  &&  IsSwitchedOn()  &&  m_ThisEntityAI.GetDamage() < 1 ) 
			{
				bool was_powered = IsWorking();
				
				// Make sure to use only as much % of energy as needed since this function can be called at random.
				float consumed_energy_coef = Math.Clamp(GetGame().GetTime() - m_LastUpdateTime, 0, CONSUME_ENERGY_INTERVAL * 1000 );
				consumed_energy_coef = consumed_energy_coef / (CONSUME_ENERGY_INTERVAL * 1000);
				
				if (consumed_energy_coef > 0) // Prevents calling of OnWork events when no energy is consumed
				{
					m_LastUpdateTime = GetGame().GetTime();
					bool has_consumed_enough = ConsumeEnergy( GetEnergyUsage() * consumed_energy_coef );
					SetPowered( has_consumed_enough );
					
					if ( has_consumed_enough )
					{
						if ( !was_powered )
						{
							m_CanStopWork = true;
							GetGame().GameScript.CallFunction(m_ThisEntityAI, "OnWorkStart", NULL, 0);
						}
						
						float consumed_energy = GetEnergyUsage() * consumed_energy_coef;
						OnWork( consumed_energy );
					}
					else
					{
						if ( was_powered )
						{
							if (m_CanStopWork)
							{
								m_CanStopWork = false;
								GetGame().GameScript.CallFunction(m_ThisEntityAI, "OnWorkStop", NULL, 0); // This event is called only once when the device STOPS being powered
								
								if (m_AutoSwitchOff)
								{
									SwitchOff(); 
								}
							}
						}
						
						StopUpdates();
					}
				}
			}
			else if(this  &&  m_ThisEntityAI)
			{
				SetPowered( false );
				
				if (m_CanStopWork)
				{
					m_CanStopWork = false;
					GetGame().GameScript.CallFunction(m_ThisEntityAI, "OnWorkStop", NULL, 0); // This event is called only once when the device STOPS being powered
				}
			}
		}
	}
}