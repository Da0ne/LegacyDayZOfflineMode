class Healthy: NotifierBase
{
	private const float	 	HEALTHY_BLOOD_TRESHOLD	= 5000;
	private const float	 	HEALTHY_TRESHOLD		= 5000;
	private const float	 	HEALING_ENERGY_TRESHOLD	= 4000;
	private const float	 	HEALING_WATER_TRESHOLD	= 2500;
	private const float	 	HEALING_BLOOD_TRESHOLD	= 5000;
		
	void Healthy(NotifiersManager manager)
	{
		m_MinPauseBetweenMessages 	= 3;
		m_MaxPauseBetweenMessages 	= 5;
		m_MinPauseBetweenSounds 	= 10;
		m_MaxPauseBetweenSounds 	= 12;
		m_MinPauseBetweenAnimations	= 10;
		m_MaxPauseBetweenAnimations	= 12;
	}

	int GetNotifierType()
	{
		return NTF_HEALTHY;
	}

	private void DisplayBadge()
	{
		float energy = m_Player.GetStatEnergy().Get();
		float water = m_Player.GetStatWater().Get();
		float blood = m_Player.GetHealth("", "Blood");
		float health = m_Player.GetHealth("", "Health");
		if ( energy >= HEALING_ENERGY_TRESHOLD && water >= HEALING_WATER_TRESHOLD && blood >= HEALING_BLOOD_TRESHOLD && health < HEALTHY_TRESHOLD ) 
		{
			GetDisplayStatus().SetStatus(DELM_NTFR_HEALTHY,DELM_LVL_1);
			
		}
		else if ( blood >= HEALTHY_BLOOD_TRESHOLD && health >= HEALTHY_TRESHOLD )
		{
			GetDisplayStatus().SetStatus(DELM_NTFR_HEALTHY,DELM_LVL_2);
		}
	}

	private void HideBadge()
	{
		
		GetDisplayStatus().SetStatus(DELM_NTFR_HEALTHY,DELM_LVL_0);
	}

	private void DisplayMessage()
	{
	}

	private void PlayAnimation()
	{
	//	PrintString(GetName() + " animation");
	}	

	private void PlaySound()
	{
	//	PrintString(GetName() + " sound");
	}

};