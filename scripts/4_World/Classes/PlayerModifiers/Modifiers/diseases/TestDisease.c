class TestDisease: ModifierBase
{
	float m_Interval;

	void Init()
	{
		m_TrackActivatedTime				= false;
		m_ID 							= 9999;
		float	m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		float	m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}
	
	bool ActivateCondition(PlayerBase player)
	{
		return true;
	}

	void OnActivate(PlayerBase player)
	{
		//player.GetStateManager().QueueUpSecondaryState( StateIDs.STATE_BLINDNESS );
		//player.GetStateManager().QueueUpSecondaryState( StateIDs.STATE_BULLET_HIT );
	}

	void OnDeactivate(PlayerBase player)
	{
	}

	bool DeactivateCondition(PlayerBase player)
	{
		return false;
	}

	bool OnTick(PlayerBase player, float deltaT)
	{
		m_Interval = m_Interval + deltaT;
		/*
		if( m_Interval > 10 )
		{
			player.GetStateManager().QueueUpPrimaryState( StateIDs.STATE_COUGH );
			player.GetStateManager().QueueUpPrimaryState( StateIDs.STATE_COUGH );
			//if( Math.RandomInt(0,3) == 0 ) player.GetStateManager().QueueUpPrimaryState( StateIDs.STATE_VOMIT );
			m_Interval = 0;
		}
		*/
	}
};