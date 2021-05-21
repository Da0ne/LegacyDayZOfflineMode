class BloodRegen: ModifierBase
{
	private const float	BLOOD_INCREMENT_PER_SEC = 5;
	
	void Init()
	{
		m_TrackActivatedTime				= false;
		m_ID 							= MDF_BLOOD_REGEN;
		float	m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		float	m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}

	private bool ActivateCondition(PlayerBase player)
	{
		float m_MaxBlood = player.GetMaxHealth("GlobalHealth", "Blood");
		if ( player.GetHealth("GlobalHealth", "Blood") <= m_MaxBlood )
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
		float m_MaxBlood = player.GetMaxHealth("GlobalHealth", "Blood");
		if ( player.GetHealth("GlobalHealth", "Blood") >= m_MaxBlood )
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
		float currentblood = player.GetHealth("GlobalHealth", "Blood");
		player.SetHealth("GlobalHealth", "Blood", ( currentblood + (BLOOD_INCREMENT_PER_SEC*deltaT) ) );
	}
};