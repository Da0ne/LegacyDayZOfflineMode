class Testing: ModifierBase
{
	float stuff;
	bool swch;
	void Init()
	{
		m_TrackActivatedTime				= true;
		m_ID 							= MDF_TESTING;
		float	m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		float	m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}

	private void OnActivate(PlayerBase player)
	{
		//m_Timer1.Run(1, this, "BadaBang");
		
	}



	// ------------------------------------------------------------------------------

	// ------------------------------------------------------------------------------

	private bool ActivateCondition(PlayerBase player)
	{
		return swch;
		if(stuff < 40)
		{
			return true;
		}
		else return false;
	}

	private bool DeactivateCondition(PlayerBase player)
	{
		if (GetAttachedTime() > 100) {swch = true; return true;}
		else return false;
		if(stuff > 40)
		{
			return true;
		}
		else return false;
	}
	// ------------------------------------------------------------------------------

	private bool OnTick(PlayerBase player, float deltaT)
	{
		stuff += deltaT;
	}
};