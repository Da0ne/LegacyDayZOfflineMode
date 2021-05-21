class Temperature: ModifierBase
{
	private const float	NONAFFECTING_HEAT_COMFORT_MIN_TRESHOLD = -10;
	private const float	NONAFFECTING_HEAT_COMFORT_MAX_TRESHOLD = 18;
	private const float	HEALTHY_TEMPERATURE = 36.8;
	private const float	TEMPERATURE_INCREMENT_PER_SEC = 0.008;
	private float		m_LastTemperatureLevel;
		
	void Init()
	{
		m_TrackActivatedTime				= false;
		m_ID 							= MDF_TEMPERATURE;
		float	m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		float	m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}

	private bool ActivateCondition(PlayerBase player)
	{
		return true;
	}

	private bool DeactivateCondition(PlayerBase player)
	{
		return false;
	}

	private bool OnTick(PlayerBase player, float deltaT)
	{	
		
		float heat_comfort = player.GetStatHeatComfort().Get();
		float temperature = player.GetStatTemperature().Get();
		
		float temperaturedelta  = Math.AbsInt(temperature - m_LastTemperatureLevel);
		if (temperature <  m_LastTemperatureLevel) temperaturedelta = -temperaturedelta;
		//if( player.m_NotifiersManager ) player.m_NotifiersManager.FindNotifier(NTF_WARMTH).DisplayTendency(temperaturedelta);
		m_LastTemperatureLevel = temperature;
		
		player.SetAnimVariableFloat("temperature",temperature);
		
		if ( heat_comfort > NONAFFECTING_HEAT_COMFORT_MAX_TRESHOLD || heat_comfort < NONAFFECTING_HEAT_COMFORT_MIN_TRESHOLD )
		{
			//  // temp decresase rate calculation
			float temperature_increment = 0.001 * heat_comfort * deltaT; //extremer the heatcomfort value, the bigger effect on temperature growth/decrease ,
			//if ( heat_comfort < 0 ) temperature_increment = temperature_increment *(5-(temperature/6))^2;  //higher the player temperature, faster the cooling effect if heatcomfort is below 0
			player.GetStatTemperature().Add( temperature_increment ); 
		}
		else
		{
			if ( temperature < HEALTHY_TEMPERATURE )
			{
				player.GetStatTemperature().Add( (TEMPERATURE_INCREMENT_PER_SEC*deltaT) );
			}
		}	
	}
};

