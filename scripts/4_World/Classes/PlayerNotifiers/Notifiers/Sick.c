class Sick: NotifierBase
{
	void Sick(NotifiersManager manager)
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
		return NTF_SICK;
	}

	private void DisplayBadge()
	{
		
		GetDisplayStatus().SetStatus(DELM_BADGE_SICK,true);
		GetDisplayStatus().SetStatus(DELM_NTFR_SICK,DELM_LVL_1);
	}

	private void HideBadge()
	{
		
		GetDisplayStatus().SetStatus(DELM_BADGE_SICK,false);
		GetDisplayStatus().SetStatus(DELM_NTFR_SICK,DELM_LVL_0);
	}

	private void DisplayMessage()
	{
		m_Player.MessageStatus("I feel sick");
	}

	private void PlayAnimation()
	{
		//PrintString(GetName() + " animation");
	}	

	private void PlaySound()
	{
		//PrintString(GetName() + " sound");
	}
};