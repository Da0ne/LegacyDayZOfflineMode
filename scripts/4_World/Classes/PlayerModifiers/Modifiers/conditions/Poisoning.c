class Poisoning: ModifierBase
{
	private const float 	WATER_DECREMENT_PER_SEC = -50;
	private const float 	BLOOD_DECREMENT_PER_SEC = -5;
	private const float 	POISONING_LASTS_SEC	 = -5;
	private float			m_Time;
	
	void Init()
	{
		m_TrackActivatedTime				= false;
		m_ID 							= MDF_POISONING;
		float	m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		float	m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}
	private bool ActivateCondition(PlayerBase player)
	{
		return false;
	}

	private void OnActivate(PlayerBase player)
	{
		m_Time = 0;
		if( player.m_NotifiersManager ) player.m_NotifiersManager.AttachByType(NTF_SICK);
	}

	private bool DeactivateCondition(PlayerBase player)
	{
		return false;
		if( player.m_NotifiersManager ) player.m_NotifiersManager.DetachByType(NTF_SICK);
	}

	private bool OnTick(PlayerBase player, float deltaT)
	{	
		player.GetStatWater().Add((WATER_DECREMENT_PER_SEC*deltaT));
		
		float currentblood = player.GetHealth("GlobalHealth", "Blood");
		player.SetHealth("GlobalHealth", "Blood", ( currentblood + (BLOOD_DECREMENT_PER_SEC*deltaT) ) );

		if ( m_Time > POISONING_LASTS_SEC )
		{
			player.GetModifiersManager().DeactivateModifier(MDF_POISONING);
		}
		else
		{
			m_Time += deltaT;
		}
	}
};