class PowerGenerator extends ItemBase
{
	float						m_Fuel;
	private static float		m_FuelTankCapacity; // Capacity in ml.
	private static float		m_FuelToEnergyRatio; // Conversion ratio of 1 ml of fuel to X Energy
	
	SoundOnVehicle 	m_SoundLoopEntity;
	autoptr Timer 	m_SoundLoopStartTimer;
	Object 			m_SmokeParticle;
	
	ItemBase	m_SparkPlug; // Attached spark plug item
	
	// Constructor
	void PowerGenerator()
	{
		if ( !m_SoundLoopStartTimer ) // Memory leak prevention.
		{
			m_SoundLoopStartTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
		}
	}

	// Destructor
	void ~PowerGenerator()
	{
		delete m_SoundLoopStartTimer;
	}
	
	// Play the loop sound
	void StartLoopSound()
	{
		if ( GetCompEM().IsSwitchedOn() )
		{
			m_SoundLoopEntity = GetGame().CreateSoundOnObject(this, "powerGeneratorLoop", 50, true);
		}
	}
	
	// Taking item into inventory
	bool ConditionIntoInventory ( EntityAI  player ) 
	{
		return CanManipulate();
	}

	// Taking item into inventory
	bool ConditionIntoHands ( EntityAI player ) 
	{
		return CanManipulate();
	}

	// Returns true/false if this item can be moved into inventory/hands
	bool CanManipulate()
	{
		bool is_plugged = GetCompEM().IsPlugged();
		bool is_running = GetCompEM().IsWorking();
		int plugged_devices_count = 0;
		
		if ( GetCompEM().GetPluggedDevices() )
		{
			plugged_devices_count = GetCompEM().GetPluggedDevices().Count();
		}
		
		if ( !is_plugged  &&  !is_running  &&  plugged_devices_count == 0 )
		{
			return true;
		}
		
		return false;
	}
	
	/*===================================
					EVENTS
	===================================*/
	
	// Init
	void OnInitEnergy()
	{
		m_FuelTankCapacity = GetGame().ConfigGetFloat ("CfgVehicles " + GetType() + " fuelTankCapacity");
		m_FuelToEnergyRatio = GetCompEM().GetEnergyMax() / m_FuelTankCapacity; // Conversion ratio of 1 ml of fuel to X Energy
		m_SparkPlug = NULL;
		m_SoundLoopEntity = NULL;
		
		UpdateFuelMeter();
	}
	
	// Generator is working
	void OnWorkStart()
	{
		GetGame().CreateSoundOnObject(this, "powerGeneratorTurnOn", 50, false);
		if ( !m_SoundLoopStartTimer.IsRunning() ) // Makes sure the timer is NOT running already
		{
			m_SoundLoopStartTimer.Run(1.5, this, "StartLoopSound", NULL, false);
		}
		
		
		// Particle test
		// Function GetObject causes a crash on server
		
		vector pos = GetPosition();
		m_SmokeParticle = GetGame().CreateObject("#particlesourceEnf", pos, true); // TEST
		if (m_SmokeParticle)
		{
			vobject vobj = GetObject("graphics/particles/smoke_small_generator_01.ptc");
			m_SmokeParticle.SetObject(vobj, "");
			ReleaseObject(vobj);
		}
	}

	// Do work
	void OnWork( float consumed_energy )
	{
		UpdateFuelMeter();
	}

	// Turn off when this runs out of fuel
	void OnWorkStop()
	{
		GetGame().CreateSoundOnObject(this, "powerGeneratorTurnOff", 50, false);
		GetGame().ObjectDelete(m_SoundLoopEntity);
		m_SoundLoopEntity = NULL;
		
		GetGame().ObjectDelete(m_SmokeParticle); // Deletes particle
		UpdateFuelMeter();
	}
	
	// Called when this generator is picked up
	void EEItemLocationChanged  ( EntityAI old_owner, EntityAI new_owner ) 
	{
		super.EEItemLocationChanged(old_owner, new_owner);
		UpdateFuelMeter();
	}
	
	
	/*================================
				FUNCTIONS
	================================*/
	
	void UpdateFuelMeter()
	{
		float fuel_coef = GetCompEM().GetEnergy() / GetCompEM().GetEnergyMax();
		SetAnimationPhase("dial_fuel", fuel_coef);
	}
	
	// Reports generator's status to the player
	void ReportStatus(PlayerBase player)
	{
		float fuel_in_tank = GetFuel();
		float max_fuel_in_tank = GetMaxFuel();
		string message = "";
		bool can_run = true;
		
		if (GetDamage() < 1)
		{
			switch(fuel_in_tank)
			{
				case 0:
					message = "The tank is empty and it has room for " + Math.Round( max_fuel_in_tank/1000 ).ToString() + " litres of fuel.";
					can_run = false;
				break;
				
				case max_fuel_in_tank:
					message = "The tank is full and it has " + Math.Round( max_fuel_in_tank/1000 ).ToString() + " litres of fuel.";
				break;
				
				default:
					message = "There is " + Math.Round( (fuel_in_tank/100 )/10 ).ToString() + " litres of fuel in the tank.";
				break;
			}
			
			if (HasSparkplug())
			{
				if (m_SparkPlug.GetDamage() < 1)
				{
					message = message + " The engine has a spark plug installed.";
				}else{
					message = message + " The spark plug is ruined.";
					can_run = false;
				}
			}else{
				message = message + " The engine is missing a spark plug.";
				can_run = false;
			}
		}
		else
		{
			message = "The generator is ruined.";
			can_run = false;
		}
		
		if (can_run)
		{
			player.MessageFriendly( message );
		}else{
			player.MessageImportant( message );
		}
		
	}
	
	// Adds energy to the generator
	void SetFuel(float fuel_amount)
	{
		GetCompEM().SetEnergy(fuel_amount * m_FuelToEnergyRatio);
		UpdateFuelMeter();
	}

	// Adds fuel (energy) to the generator
	// Returns how much fuel was accepted
	float AddFuel(float available_fuel)
	{
		if ( available_fuel == 0 )
		{
			return 0;
		}
		
		float needed_fuel = GetMaxFuel() - GetFuel();
		
		if ( needed_fuel > available_fuel )
		{
			SetFuel(GetFuel() + available_fuel);
			return available_fuel; // Return used fuel amount
		}
		else
		{
			SetFuel(GetMaxFuel());
			return needed_fuel;
		}
	}

	// Check the bottle if it can be used to fill the tank
	bool CanAddFuel( ItemBase container )
	{
		if (container)
		{
			// Check the liquid
			string liquid_type;
			liquid_type	= container.GetVariableString("LiquidType");
			
			// Do all checks
			if ( container.GetQuantity() > 0  &&  GetCompEM().GetEnergy() < GetCompEM().GetEnergyMax()  &&  liquid_type == "\"gasoline\"" ) // In-string quotation marks must be here because the GetVariableString() function writes them into the output variable.
			{
				return true;
			}
		}
		return false;
	}

	// Returns fuel amount
	float GetFuel()
	{
		return GetCompEM().GetEnergy() / m_FuelToEnergyRatio;
	}

	// Returns max fuel amount
	float GetMaxFuel()
	{
		return m_FuelTankCapacity;
	}

	// Checks sparkplug
	bool HasSparkplug()
	{
		if ( m_SparkPlug )
		{
			return true;
		}else{
			return false;
		}
	}

	// Installs the sparkplug
	void InstallSparkplug(ItemBase item)
	{
		TakeEntityAsAttachment( item );
	}
}

// Temporal class for new User Actions
class EN5C_PowerGenerator extends PowerGenerator
{

}
