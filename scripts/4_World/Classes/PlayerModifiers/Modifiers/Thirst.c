class Thirst: ModifierBase
{
	private const float 	LOW_WATER_TRESHOLD 			= 1000;
	private const float 	WATER_DECREMENT_PER_SEC 	= -0.25;
	private float 			m_LastWaterLevel;
	
	void Init()
	{
		m_TrackActivatedTime				= false;
		m_ID 							= MDF_THIRST;
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
		float water = player.GetStatWater().Get();
		float waterdelta  = Math.AbsInt(water - m_LastWaterLevel);
		if (water <  m_LastWaterLevel) waterdelta = -waterdelta;
		//if( player.m_NotifiersManager ) player.m_NotifiersManager.FindNotifier(NTF_THIRSTY).DisplayTendency(waterdelta);
		m_LastWaterLevel = water;
		player.GetStatWater().Add( (WATER_DECREMENT_PER_SEC*deltaT) );
		if ( water <= LOW_WATER_TRESHOLD )
		{		
			float currenthealth = player.GetHealth("GlobalHealth", "Health");
			player.SetHealth("GlobalHealth", "Health" , ( currenthealth + ( ( -1 / ( 1 + water ) ) * deltaT / 50 ) ) );		//lower is your energy, more health is taken
			float currentblood = player.GetHealth("GlobalHealth", "Blood");
			player.SetHealth("GlobalHealth", "Blood", ( currentblood + ( ( -0.05 / ( 1 + water ) ) * deltaT ) ) );
		}
	}
};