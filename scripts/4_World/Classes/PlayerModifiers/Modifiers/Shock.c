class Shock: ModifierBase
{
	private const float	 	UNCONSCIOUS_LIMIT = 0;
	private const float 	SHOCK_INCREMENT_PER_SEC = 1;
	
	void Init()
	{
		m_TrackActivatedTime				= false;
		m_ID 							= MDF_SHOCK;
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
		if ( player.GetHealth("GlobalHealth","Shock") <= UNCONSCIOUS_LIMIT )
		{
			//player.setUnconscious();
		}
		else
		{
			player.AddHealth("GlobalHealth","Shock", (SHOCK_INCREMENT_PER_SEC * deltaT) );
		}
	}
};