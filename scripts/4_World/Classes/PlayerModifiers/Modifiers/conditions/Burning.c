class Burning: ModifierBase
{
	private const float	 	BURNING_TRESHOLD = 199;
	private const float	 	HEALTH_DECREMENT_PER_SEC = -5;
	
	void Init()
	{
		m_TrackActivatedTime				= false;
		m_ID 							= MDF_BURNING;
		float	m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		float	m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}
	private bool ActivateCondition(PlayerBase player)
	{
		float hc = player.GetStatHeatComfort().Get();
		if ( hc > BURNING_TRESHOLD )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	private void OnActivate(PlayerBase player)
	{
		//Print("My face is melting!");
	}


	private bool DeactivateCondition(PlayerBase player)
	{
		if ( player.GetStatHeatComfort().Get() <= BURNING_TRESHOLD )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	private bool OnTick(PlayerBase player, float deltaT)
	{
		float currenthealth = player.GetHealth("GlobalHealth", "Health");
		player.SetHealth("GlobalHealth", "Health" , ( currenthealth + (HEALTH_DECREMENT_PER_SEC*deltaT) ) );	
		player.GetStatTemperature().Set( player.GetStatTemperature().GetMax() );
	}
};