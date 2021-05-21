class Stuffed: NotifierBase
{
	private const int	 	STUFFED_TRESHOLD			= 1800;
	private const int	 	VERY_STUFFED_TRESHOLD		= 2800;
	private const int	 	FATALLY_STUFFED_TRESHOLD 	= 3300;

	void Stuffed(NotifiersManager manager)
	{
		m_MinPauseBetweenMessages 	= 3;
		m_MaxPauseBetweenMessages 	= 5;
		m_MinPauseBetweenSounds 	= 10;
		m_MaxPauseBetweenSounds 	= 12;
		m_MinPauseBetweenAnimations	= 10;
		m_MaxPauseBetweenAnimations	= 12;
		m_Active = false; 
	}

	int GetNotifierType()
	{
		return NTF_STUFFED;
	}

	private void DisplayBadge()
	{
		GetDisplayStatus().SetStatus(DELM_BADGE_STUFFED,true);
		
		float stomach = m_Player.GetStatStomach().Get();
		if (stomach >= FATALLY_STUFFED_TRESHOLD) 
		{
			GetDisplayStatus().SetStatus(DELM_NTFR_FULLNESS,DELM_LVL_3);
		}
		else if (stomach >= VERY_STUFFED_TRESHOLD)
		{
			GetDisplayStatus().SetStatus(DELM_NTFR_FULLNESS,DELM_LVL_2);
		}
		else if (stomach >= STUFFED_TRESHOLD)
		{
			GetDisplayStatus().SetStatus(DELM_NTFR_FULLNESS,DELM_LVL_1);
		}
	}

	private void HideBadge()
	{
		
		GetDisplayStatus().SetStatus(DELM_BADGE_STUFFED,false);
		GetDisplayStatus().SetStatus(DELM_NTFR_FULLNESS,DELM_LVL_0);
	}

	private void DisplayMessage()
	{
		float stomach = m_Player.GetStatStomach().Get();
		if (stomach >= FATALLY_STUFFED_TRESHOLD) 
		{
			m_Player.MessageStatus("I am going to throw up");
		}
		else if (stomach >= VERY_STUFFED_TRESHOLD)
		{
			m_Player.MessageStatus("I am SO full");
		}
		else if (stomach >= STUFFED_TRESHOLD)
		{

			m_Player.MessageStatus("I am so full");
		}
	}

	private void PlayAnimation()
	{
		float stomach = m_Player.GetStatStomach().Get();
		if (stomach >= FATALLY_STUFFED_TRESHOLD) 
		{
		//	PrintString(GetName() + " animation");
		}
	}	

	private void PlaySound()
	{
		float stomach = m_Player.GetStatStomach().Get();
		if (stomach >= FATALLY_STUFFED_TRESHOLD) 
		{
		//	PrintString(GetName() + " sound");
		}
	}
};