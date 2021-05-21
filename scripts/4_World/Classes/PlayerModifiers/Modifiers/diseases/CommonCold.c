class CommonCold: ModifierBase
{
	private const float	 	TOO_WEAK_IMMUNITY_TRESHOLD = 0.5;
	private const float	 	FEVER_DEADLY_TIME = 15; //seconds
	private float			m_Time;
		
	void Init()
	{
		m_TrackActivatedTime				= false;
		m_ID 							= MDF_COMMON_COLD;
		float	m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		float	m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}
	bool ActivateCondition(PlayerBase player)
	{
		if( player.GetImmunity() < TOO_WEAK_IMMUNITY_TRESHOLD )
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
		m_Time = 0;
		if( player.m_NotifiersManager ) player.m_NotifiersManager.AttachByType(NTF_SICK);
	}


	private bool DeactivateCondition(PlayerBase player)
	{
		if( player.GetImmunity() >= TOO_WEAK_IMMUNITY_TRESHOLD )
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
		if( m_Time > FEVER_DEADLY_TIME )
		{
			player.GetModifiersManager().ActivateModifier(MDF_FEVER);
			if( player.m_NotifiersManager ) player.m_NotifiersManager.DetachByType(NTF_SICK);
			//m_Immunity.Add(IMMUNITY_INCREMENT_PER_SEC);//temporary
		}
		else
		{
			m_Time = m_Time + deltaT;
		}
	}
};