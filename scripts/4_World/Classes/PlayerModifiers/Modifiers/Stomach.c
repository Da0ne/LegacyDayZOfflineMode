class Stomach: ModifierBase
{
	private const float	 	ENERGY_TRANSFERED_PER_SEC = 100;
	private const float	 	WATER_TRANSFERED_PER_SEC = 8;
	private const float		STOMACH_EMPTIED_SOLID_PER_SEC = 2;
	private const float		STOMACH_EMPTIED_LIQUID_PER_SEC = WATER_TRANSFERED_PER_SEC;
	
	void Init()
	{
		m_TrackActivatedTime				= false;
		m_ID 							= MDF_STOMACH;
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
		float stomach_volume = player.GetStatStomach().Get();
		float stomach_water = player.GetStatStomachWater().Get();
		
		if ( player.GetStatStomachEnergy().Get() >= ENERGY_TRANSFERED_PER_SEC*deltaT )
		{
			player.GetStatEnergy().Add(ENERGY_TRANSFERED_PER_SEC*deltaT);
			player.GetStatStomachEnergy().Add(-ENERGY_TRANSFERED_PER_SEC*deltaT);
		}
		
		if (  stomach_water >= WATER_TRANSFERED_PER_SEC*deltaT )
		{
			player.GetStatStomachWater().Add(-WATER_TRANSFERED_PER_SEC*deltaT);
			player.GetStatWater().Add(WATER_TRANSFERED_PER_SEC*deltaT);
		}
		
		if ( stomach_volume >= 0 )
		{
			float stomach_decrement = 1;
			if ( (stomach_volume - stomach_water) >= STOMACH_EMPTIED_SOLID_PER_SEC ) stomach_decrement += STOMACH_EMPTIED_SOLID_PER_SEC;
			if ( stomach_water >= STOMACH_EMPTIED_LIQUID_PER_SEC ) stomach_decrement += STOMACH_EMPTIED_LIQUID_PER_SEC;
			player.GetStatStomach().Add(-stomach_decrement*deltaT);
		}
	}
};