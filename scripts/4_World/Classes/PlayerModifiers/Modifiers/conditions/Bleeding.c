class Bleeding: ModifierBase
{
	private const float		BLOOD_DECREMENT_PER_SEC = -10;
		
	void Init()
	{
		m_TrackActivatedTime				= false;
		m_ID 							= MDF_BLEEDING;
		float	m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		float	m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}

	private bool ActivateCondition(PlayerBase player)
	{
		return false;
	}

	private void OnActivate(PlayerBase player)
	{
		if( player.m_NotifiersManager ) player.m_NotifiersManager.AttachByType(NTF_BLEEDISH);
	}

	private void OnDeactivate(PlayerBase player)
	{
		if( player.m_NotifiersManager ) player.m_NotifiersManager.DetachByType(NTF_BLEEDISH);
	}

	private bool DeactivateCondition(PlayerBase player)
	{
		return false;
	}

	private bool OnTick(PlayerBase player, float deltaT)
	{
		float currentblood = player.GetHealth("GlobalHealth", "Blood");
		player.SetHealth("GlobalHealth", "Blood", ( currentblood + (BLOOD_DECREMENT_PER_SEC * deltaT ) ) );
	}
};