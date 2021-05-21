class BatteryCharger extends ItemBase
{
	// Model selections
	static protected const string CLIPS_CAR			= "clips_car_battery";
	static protected const string CLIPS_TRUCK		= "clips_truck_battery";
	static protected const string CLIPS_DETACHED	= "clips_detached";
	static protected const string CLIPS_FOLDED		= "clips_folded";
	static protected const string SWITCH_ON			= "switch_on";
	static protected const string SWITCH_OFF		= "switch_off";
	static protected const string CORD_PLUGGED		= "cord_plugged";
	static protected const string CORD_FOLDED		= "cord_folded";
	static protected const string LIGHT_STATE_1		= "light_stand_by";
	static protected const string LIGHT_STATE_2		= "light_charging";
	static protected const string LIGHT_STATE_3		= "light_charged";
	
	// glow materials
	static protected const string RED_LIGHT_GLOW	= "dz\\gear\\camping\\data\\battery_charger_light_r.rvmat";
	static protected const string GREEN_LIGHT_GLOW	= "dz\\gear\\camping\\data\\battery_charger_light_g.rvmat";
	static protected const string YELLOW_LIGHT_GLOW	= "dz\\gear\\camping\\data\\battery_charger_light_y.rvmat";
	static protected const string SWITCH_LIGHT_GLOW	= "dz\\gear\\camping\\data\\battery_charger_light_switch_on.rvmat";
	static protected const string DEFAULT_MATERIAL 	= "dz\\gear\\camping\\data\\battery_charger.rvmat";
	
	protected const string ATTACHED_CLIPS_STATES[] 	= {CLIPS_CAR, CLIPS_TRUCK}; // TO DO: Add helicopter battery
	protected const int ATTACHED_CLIPS_STATES_COUNT	= 2;
	
	protected float m_ChargeEnergyPerSecond;
	
	static protected 	float 	m_BlinkingStatusLightInterval = 0.2; // How often the the lights blink
	autoptr 			Timer 	m_UpdateStatusLightsTimer;
	protected 			bool 	m_BlinkingStatusLightIsOn = false;	// Status of one blinking light
	
	void BatteryCharger()
	{
		m_ChargeEnergyPerSecond = GetGame().ConfigGetFloat ("CfgVehicles " + GetType() + " ChargeEnergyPerSecond");
		m_UpdateStatusLightsTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
		SwitchLightOff();
	}
	
	void OnWork( float consumed_energy )
	{
		// Charging functionality
		ItemBase battery = GetCompEM().GetPluggedDevice();
		
		if ( battery )
		{
			float battery_capacity = battery.GetCompEM().GetEnergyMax();
			
			if ( battery.GetCompEM().GetEnergy() < battery_capacity )
			{
				// Heat up the items so players know they are working.
				this.SetTemperature2(60); 
				battery.SetTemperature2(60);
				
				float charger_damage = GetDamage();
				float energy_add;
				energy_add = m_ChargeEnergyPerSecond * ( consumed_energy / GetCompEM().GetEnergyUsage() );
				
				if ( GetCompEM().ConsumeEnergy(energy_add) ) // consumes energy from the power source
				{
					// There is enough of energy to use
					energy_add = energy_add * ( 1 - (charger_damage/2) ); // Damaged charger works less efficiently
				}
				else
				{
					// There is NOT enough of energy to use
					energy_add = 0;
				}
				
				battery.GetCompEM().AddEnergy( energy_add );
			}
			else
			{
				battery.GetCompEM().SetEnergy( battery_capacity );
			}
		}
		else
		{
			// Nothing for now
		}
	}

	void OnWorkStart()
	{
		UpdateStatusLights();
		m_UpdateStatusLightsTimer.Run(m_BlinkingStatusLightInterval, this, "UpdateStatusLights", NULL, true);
	}
		
	void OnWorkStop()
	{
		UpdateStatusLights();
		m_UpdateStatusLightsTimer.Stop();
	}
	
	void UpdateStatusLights()
	{
		if (GetCompEM().IsWorking())
		{
			SwitchLightOn();
			
			ItemBase battery = GetCompEM().GetPluggedDevice();
			
			if (battery)
			{
				RedLightOff();
				
				float battery_capacity = battery.GetCompEM().GetEnergyMax();
				float battery_energy = battery.GetCompEM().GetEnergy();
				
				if (battery_energy < battery_capacity*0.33)
				{
					// Less than 1/3 charged, yellow status light must repeatedly blink
					
					if (m_BlinkingStatusLightIsOn)
						YellowLightOn();
					else
						YellowLightOff();
					
					m_BlinkingStatusLightIsOn = !m_BlinkingStatusLightIsOn;
				}
				else if (battery_energy > battery_capacity*0.33  &&  battery_energy < battery_capacity*0.66)
				{
					// Less than 2/3 charged, yellow status light must glow
					
					YellowLightOn();
				}
				else if (battery_energy > battery_capacity*0.66  &&  battery_energy < battery_capacity)
				{
					// Less than 3/3 charged, yellow status light must glow, green light must blink
					
					YellowLightOn();
					
					if (m_BlinkingStatusLightIsOn)
						GreenLightOn();
					else
						GreenLightOff();
					
					m_BlinkingStatusLightIsOn = !m_BlinkingStatusLightIsOn;
				}
				else if (battery_energy >= battery_capacity)
				{
					// Fully charged, green light must glow
					YellowLightOff();
					GreenLightOn();
				}
			}
			else
			{
				if (m_BlinkingStatusLightIsOn)
					RedLightOn();
				else
					RedLightOff();
				
				m_BlinkingStatusLightIsOn = !m_BlinkingStatusLightIsOn;
				
				GreenLightOff();
				YellowLightOff();
			}
		}
		else
		{
			SwitchLightOff();
			GreenLightOff();
			RedLightOff();
			YellowLightOff();
		}
	}
	
	bool ConditionIntoInventory ( EntityAI  player ) 
	{
		// No "Take" action if the item is connected
		if ( !GetCompEM().IsPlugged()  &&  !GetCompEM().GetPluggedDevice() )
		{
			return true;
		}
		
		return false;
	}

	bool ConditionIntoHands ( EntityAI player ) 
	{
		// No "Take into hands" action if the item is connected
		if ( !GetCompEM().IsPlugged()  &&  !GetCompEM().GetPluggedDevice() )
		{
			return true;
		}
		
		return false;
	}

	void OnOwnSocketTaken( EntityAI device )
	{
		string att_type = GetCompEM().GetPluggedDevice().GetType();
		
		if ( att_type == "CarBattery"  ||  att_type == "EN5C_CarBattery" )
		{
			HideAttachedClipsStates();
			ShowSelection(CLIPS_CAR);
		}
		
		if ( att_type == "TruckBattery"  ||  att_type == "EN5C_TruckBattery" )
		{
			HideAttachedClipsStates();
			ShowSelection(CLIPS_TRUCK);
		}
		
		HideSelection(CLIPS_DETACHED);
		HideSelection(CLIPS_FOLDED);
	}

	void OnOwnSocketReleased( EntityAI device )
	{
		HideAttachedClipsStates();
		ShowSelection(CLIPS_DETACHED);
	}

	bool CanReceiveAttachment( ItemBase item )
	{
		// No attaching if the charger is in inventory!
		PlayerBase charger_owner = GetOwnerPlayer();
		if ( charger_owner )
		{
			return false;
		}
		
		// Only one attachment allowed
		if ( GetCompEM().GetPluggedDevice() )
		{
			return false;
		}
		
		if ( item.HasEnergyManager()  &&  item.GetCompEM().GetPluggedDevicesCount() >= 1 ) // Make sure nothing is plugged into the battery
		{
			return false;
		}
		
		return true;
	}
	
	void HideAttachedClipsStates()
	{
		for ( int i = 0; i < ATTACHED_CLIPS_STATES_COUNT; i++ )
		{
			string selection = ATTACHED_CLIPS_STATES[i];
			HideSelection(selection);
		}
	}
	
	
	// Control of status lights
	// ON
	void RedLightOn()
	{
		SetObjectMaterial( 0, RED_LIGHT_GLOW );
	}
	void GreenLightOn()
	{
		SetObjectMaterial( 2, GREEN_LIGHT_GLOW );
	}
	void YellowLightOn()
	{
		SetObjectMaterial( 1, YELLOW_LIGHT_GLOW );
	}
	void SwitchLightOn()
	{
		SetObjectMaterial( 3, SWITCH_LIGHT_GLOW );
	}
	// OFF
	void RedLightOff()
	{
		SetObjectMaterial( 0, DEFAULT_MATERIAL );
	}
	void GreenLightOff()
	{
		SetObjectMaterial( 2, DEFAULT_MATERIAL );
	}
	void YellowLightOff()
	{
		SetObjectMaterial( 1, DEFAULT_MATERIAL );
	}
	void SwitchLightOff()
	{
		SetObjectMaterial( 3, DEFAULT_MATERIAL );
	}
	
	
	void OnSwitchOn()
	{
		HideSelection(SWITCH_OFF);
		ShowSelection(SWITCH_ON);
	}
	
	void OnSwitchOff()
	{
		HideSelection(SWITCH_ON);
		ShowSelection(SWITCH_OFF);
	}
	
	// Inventory manipulation
	void OnInventoryExit(Man player)
	{
		super.OnInventoryExit(player);
		
		HideAttachedClipsStates();
		HideSelection(CLIPS_FOLDED);
		ShowSelection(CLIPS_DETACHED);
	}
	
	void OnInventoryEnter(Man player)
	{
		super.OnInventoryEnter(player);
		
		HideAttachedClipsStates();
		HideSelection(CLIPS_DETACHED);
		ShowSelection(CLIPS_FOLDED);
	}
}

// Temporal class for new User Actions
class EN5C_BatteryCharger extends BatteryCharger
{
	void OnHologramBeingPlaced( PlayerBase player )
	{
		ItemBase item_in_hands = player.GetHologram().GetParentObject();
		string SEL_CORD_PLUGGED = item_in_hands.GetCompEM().SEL_CORD_PLUGGED;
		string SEL_CORD_FOLDED = item_in_hands.GetCompEM().SEL_CORD_FOLDED;

		SetAnimationPhase( CLIPS_DETACHED, 0);
		SetAnimationPhase( CLIPS_FOLDED, 1);
	
		//TO DO toto sa mi nepaci, treba to urobit nejako aby bol argument arrray/set

		player.GetHologram().RefreshVisual( SEL_CORD_PLUGGED );
		player.GetHologram().RefreshVisual( SEL_CORD_FOLDED );
		player.GetHologram().RefreshVisual( CLIPS_DETACHED );
		player.GetHologram().RefreshVisual( CLIPS_FOLDED );
	}
}
