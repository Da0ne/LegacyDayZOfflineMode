class BrokenLegs: ModifierBase
{
	private const 	float	HEALTHY_LEG = 80;

	void Init()
	{
		m_TrackActivatedTime				= false;
		m_ID 							= MDF_BROKEN_LEGS;
		float	m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		float	m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}	

	private bool  ActivateCondition(PlayerBase player)
	{
		if ( player.GetHealth("RightLeg", "Health") <= 0 || player.GetHealth("LeftLeg", "Health") <= 0 )
		{
			return true;
		}
		return false;
	}

	private void  OnActivate(PlayerBase player)
	{
		// player.setBrokenLimbs("Legs",true);
	}

	private void OnDeactivate(PlayerBase player)
	{
		// player.setBrokenLimbs("Legs",false);
	}

	private bool  DeactivateCondition(PlayerBase player)
	{
		if ( player.GetHealth("RightLeg", "Health") >= HEALTHY_LEG && player.GetHealth("LeftLeg", "Health") >= HEALTHY_LEG )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};