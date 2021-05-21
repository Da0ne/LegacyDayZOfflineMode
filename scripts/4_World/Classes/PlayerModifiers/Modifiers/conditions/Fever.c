class Fever: ModifierBase
{
	private const float	 	TEMPERATURE_INCREMENT_PER_SEC = 0.1;
		
	void Init()
	{
		m_TrackActivatedTime				= false;
		m_ID 							= MDF_FEVER;
		float	m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		float	m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}
	private bool ActivateCondition(PlayerBase player)
	{
		//Print(GetGame().GetTime());
		return false;
	}

	private void OnActivate(PlayerBase player)
	{
		//Print(GetGame().GetTime());
		if( player.m_NotifiersManager ) player.m_NotifiersManager.AttachByType(NTF_FEVERISH);
	}

	private void OnDeactivate(PlayerBase player)
	{
		if( player.m_NotifiersManager ) player.m_NotifiersManager.DetachByType(NTF_FEVERISH);
	}


	private bool DeactivateCondition(PlayerBase player)
	{
		return false;
	}

	private bool OnTick(PlayerBase player, float deltaT)
	{
		player.GetStatTemperature().Add( (TEMPERATURE_INCREMENT_PER_SEC*deltaT) );
	}
};