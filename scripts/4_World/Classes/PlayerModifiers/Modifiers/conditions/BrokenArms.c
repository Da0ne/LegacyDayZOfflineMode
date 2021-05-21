class BrokenArms: ModifierBase
{
	private const 	float	HEALTHY_ARM = 80;
	
	void Init()
	{
		m_TrackActivatedTime				= false;
		m_ID 							= MDF_BROKEN_ARMS;
		float	m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		float	m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}

	private bool ActivateCondition(PlayerBase player)
	{
		if ( player.GetHealth("RightArm", "Health") <= 0 || player.GetHealth("LeftArm", "Health") <= 0 )
		{
			return true;
		}
		return false;
	}

	private void OnActivate(PlayerBase player)
	{
		// player.setBrokenLimbs("Arms",true);
	}

	private void OnDeactivate(PlayerBase player)
	{
		// player.setBrokenLimbs("Arms",false);
	}

	private bool DeactivateCondition(PlayerBase player)
	{
		if ( player.GetHealth("RightArm", "Health") >= HEALTHY_ARM && player.GetHealth("LeftArm", "Health") >= HEALTHY_ARM )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};