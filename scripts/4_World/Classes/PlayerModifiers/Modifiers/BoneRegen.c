class BoneRegen: ModifierBase
{
	private const 	float	MINIMAL_WATER_TO_REGENRATE = 2500;
	private const 	float	MINIMAL_ENERGY_TO_REGENRATE = 4000;
	private const 	float	MINIMAL_BLOOD_TO_REGENRATE = 5000;
	private const 	float	MINIMAL_BLOOD_REQUIRED_TO_REGENERATE_HEALTH = 5000;
	private const	float 	BONE_HEALTH_INCREMENT_PER_SEC = 0.25;
	
	void Init()
	{
		m_TrackActivatedTime				= false;
		m_ID 							= MDF_BONE_REGEN;
		float	m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		float	m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}

	private bool ActivateCondition(PlayerBase player)
	{
		if (  player.GetHealth("GlobalHealth", "Blood") >= MINIMAL_BLOOD_TO_REGENRATE && player.GetStatWater().Get() >= MINIMAL_WATER_TO_REGENRATE && player.GetStatEnergy().Get() >= MINIMAL_ENERGY_TO_REGENRATE )
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
	}

	private bool DeactivateCondition(PlayerBase player)
	{
		if (  player.GetHealth("GlobalHealth", "Blood") < MINIMAL_BLOOD_TO_REGENRATE && player.GetStatWater().Get() < MINIMAL_WATER_TO_REGENRATE && player.GetStatEnergy().Get() < MINIMAL_ENERGY_TO_REGENRATE )
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
		player.SetHealth("RightArm","Health",((player.GetHealth("RightArm","Health")+BONE_HEALTH_INCREMENT_PER_SEC)*deltaT));
		player.SetHealth("RightHand","Health",((player.GetHealth("RightHand","Health")+BONE_HEALTH_INCREMENT_PER_SEC)*deltaT));
		player.SetHealth("LeftArm","Health",((player.GetHealth("LeftArm","Health")+BONE_HEALTH_INCREMENT_PER_SEC)*deltaT));
		player.SetHealth("LeftHand","Health",((player.GetHealth("LeftHand","Health")+BONE_HEALTH_INCREMENT_PER_SEC)*deltaT));
		
		player.SetHealth("RightLeg","Health",((player.GetHealth("RightLeg","Health")+BONE_HEALTH_INCREMENT_PER_SEC)*deltaT));
		player.SetHealth("RightFoot","Health",((player.GetHealth("RightFoot","Health")+BONE_HEALTH_INCREMENT_PER_SEC)*deltaT));
		player.SetHealth("LeftLeg","Health",((player.GetHealth("LeftLeg","Health")+BONE_HEALTH_INCREMENT_PER_SEC)*deltaT));
		player.SetHealth("LeftFoot","Health",((player.GetHealth("LeftFoot","Health")+BONE_HEALTH_INCREMENT_PER_SEC)*deltaT));
	}
};