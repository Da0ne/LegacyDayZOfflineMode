class Cholera: ModifierBase
{
	private const float	 	TEMPERATURE_INCREMENT_PER_SEC = 0.1;
	
	void Init()
	{
		m_TrackActivatedTime				= false;
		m_ID 							= MDF_CHOLERA;
		float	m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		float	m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}
	
	private bool ActivateCondition(PlayerBase player)
	{
		if(player.GetSingleAgentCount(AGT_CHOLERA) > CHOLERA_AGENT_THRESHOLD_ACTIVATE) 
		{
			return true;
		}
		else return false;
	}

	private void OnActivate(PlayerBase player)
	{
	}

	private void OnDeactivate(PlayerBase player)
	{
	}

	private bool DeactivateCondition(PlayerBase player)
	{
		if(player.GetSingleAgentCount(AGT_CHOLERA) < CHOLERA_AGENT_THRESHOLD_DEACTIVATE) 
		{
			return true;
		}
		else return false;
	}

	private bool OnTick(PlayerBase player, float deltaT)
	{
		//player.GetStatTemperature().Add( (TEMPERATURE_INCREMENT_PER_SEC*deltaT) );
		//Log("Ticking OnTick influenza modifier "+ToString(player.GetSingleAgentCount(AGT_INFLUENZA)));
	}
};