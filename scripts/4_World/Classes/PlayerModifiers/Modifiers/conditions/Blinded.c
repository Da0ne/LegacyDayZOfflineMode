class Blinded: ModifierBase
{
	private const float		BLIND_TIME = 7;
	private float	 		m_Timer;

	void Init()
	{
		m_TrackActivatedTime				= false;
		m_ID 							= MDF_BLINDED;
		float	m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		float	m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}
	private bool ActivateCondition(PlayerBase player)
	{
		return false;
	}

	private void OnActivate(PlayerBase player)
	{	
		m_Timer = 0;
		
	}

	private void OnDeactivate(PlayerBase player)
	{	
		g_Game.GetWorld().SetAperture(0);
	}


	private bool DeactivateCondition(PlayerBase player)
	{
		if ( m_Timer > BLIND_TIME )
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
		m_Timer++;
		g_Game.GetWorld().SetAperture(m_Timer);
	}
};