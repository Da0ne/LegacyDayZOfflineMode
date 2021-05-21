class HealthRegen: ModifierBase
{
	private const 	float	MINIMAL_WATER_TO_REGENRATE = 1500;
	private const 	float	MINIMAL_ENERGY_TO_REGENRATE = 2000;
	private const 	float	MINIMAL_BLOOD_REQUIRED_TO_REGENERATE_HEALTH = 5000;
	private const	float 	HEALTH_INCREMENT_PER_SEC = 0.02;
		
	void Init()
	{
		m_TrackActivatedTime				= false;
		m_ID 							= MDF_HEALTH_REGEN;
		float	m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		float	m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}
	private bool ActivateCondition(PlayerBase player)
	{
		if ( player.GetStatWater().Get() >= MINIMAL_WATER_TO_REGENRATE && player.GetStatEnergy().Get() >= MINIMAL_ENERGY_TO_REGENRATE )
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
	}

	private bool DeactivateCondition(PlayerBase player)
	{
		if ( player.GetStatWater().Get() < MINIMAL_WATER_TO_REGENRATE || player.GetStatEnergy().Get() < MINIMAL_ENERGY_TO_REGENRATE )
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
		if (  player.GetHealth("GlobalHealth", "Blood") == MINIMAL_BLOOD_REQUIRED_TO_REGENERATE_HEALTH )
		{
			float currenthealth = player.GetHealth("GlobalHealth", "Health");
			player.SetHealth("GlobalHealth", "Health" , ( currenthealth + (HEALTH_INCREMENT_PER_SEC*deltaT) ) );
		}
	}
};
