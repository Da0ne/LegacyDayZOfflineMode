class HemolyticReaction: ModifierBase
{
	private const float	BLOOD_DECREMENT_PER_SEC = 5;
	private float		m_BloodLoos;
	
	void Init()
	{
		m_TrackActivatedTime				= false;
		m_ID 							= MDF_HEMOLYTIC_REACTION;
		float	m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		float	m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}
	private bool ActivateCondition(PlayerBase player)
	{
		return false;
	}

	private void OnActivate(PlayerBase player)
	{
		m_BloodLoos = 0;
		if( player.m_NotifiersManager ) player.m_NotifiersManager.AttachByType(NTF_SICK);
	}

	private void OnDeactivate(PlayerBase player)
	{
		if( player.m_NotifiersManager ) player.m_NotifiersManager.DetachByType(NTF_SICK);
	}


	private bool DeactivateCondition(PlayerBase player)
	{
		return false;
	}

	private bool OnTick(PlayerBase player, float deltaT)
	{
		m_BloodLoos += BLOOD_DECREMENT_PER_SEC;
		float currentblood = player.GetHealth("GlobalHealth", "Blood");
		player.SetHealth("GlobalHealth", "Blood", ( currentblood + (m_BloodLoos*deltaT) ) );
	}
};