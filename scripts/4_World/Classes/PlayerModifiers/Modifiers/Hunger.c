class Hunger: ModifierBase
{
	private float			m_EnergyDelta;
	private float			m_LastEnergyLevel;
	private const float 	LOW_ENERGY_TRESHOLD = 1000;
	private const float 	ENERGY_DECREMENT_PER_SEC = -1;
	
	void Init()
	{
		m_TrackActivatedTime				= false;
		m_ID 							= MDF_HUNGER;
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
		float energy_delta  = Math.AbsInt(player.GetStatEnergy().Get() - m_LastEnergyLevel);
		if (player.GetStatEnergy().Get() <  m_LastEnergyLevel) energy_delta = -energy_delta;
		m_LastEnergyLevel = player.GetStatEnergy().Get();
		//if( player.m_NotifiersManager ) player.m_NotifiersManager.FindNotifier(NTF_HUNGRY).DisplayTendency(energy_delta);
		player.GetStatEnergy().Add(ENERGY_DECREMENT_PER_SEC*deltaT);
		if ( player.GetStatEnergy().Get() <= LOW_ENERGY_TRESHOLD )
		{
			float currenthealth = player.GetHealth("GlobalHealth", "Health");
			player.SetHealth("GlobalHealth", "Health" , ( currenthealth + ( ( -1 / ( 1 + player.GetStatEnergy().Get() ) ) * deltaT / 50 ) ) );		//lower is your energy, more health is taken
			float currentblood = player.GetHealth("GlobalHealth", "Blood");
			player.SetHealth("GlobalHealth", "Blood", ( currentblood + ( ( -0.05 / ( 1 + player.GetStatEnergy().Get() ) ) * deltaT ) ) );
		}
	}
};