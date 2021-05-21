class ImmuneSystem: ModifierBase
{
	void Init()
	{
		m_TrackActivatedTime				= false;
		m_ID 							= MDF_IMMUNE_SYSTEM;
		float	m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		float	m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}
	private bool ActivateCondition(PlayerBase player)
	{
		return true;
	}

	private void OnActivate(PlayerBase player)
	{
	}

	private bool DeactivateCondition(PlayerBase player)
	{
		return false;
	}

	private bool OnTick(PlayerBase player, float deltaT)
	{
		Debug.Log("ticking immune system", "agent");
		float result 	= player.GetImmunity() * deltaT;
		
		
		player.ImmuneSystemTick(result);
		Debug.Log("result: "+result.ToString(), "agent");
	}
};
