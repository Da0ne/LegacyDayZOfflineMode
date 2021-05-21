class Health: ModifierBase
{
	private float	m_LastHealthLevel;
	private float	m_LastBloodLevel;
	
	
	void Init()
	{
		m_TrackActivatedTime				= false;
		m_ID 							= MDF_HEALTH;
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
		
		float blood =  player.GetHealth("GlobalHealth", "Blood");
		float health = player.GetHealth("GlobalHealth", "Health");	

		float healthdelta  = Math.AbsInt(health - m_LastHealthLevel);
		if (health <  m_LastHealthLevel) healthdelta = -healthdelta;
		//if( player.m_NotifiersManager ) player.m_NotifiersManager.FindNotifier(NTF_LIVES).DisplayTendency(healthdelta);
		m_LastHealthLevel = health;
		
		float blooddelta  = Math.AbsInt(blood - m_LastBloodLevel);
		if (blood <  m_LastBloodLevel) blooddelta = -blooddelta;
		//if( player.m_NotifiersManager ) player.m_NotifiersManager.FindNotifier(NTF_BLOOD).DisplayTendency(blooddelta);
		m_LastBloodLevel = blood;
		
	}
};