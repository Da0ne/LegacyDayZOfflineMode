class Bleedish: NotifierBase
{
	void Bleedish(NotifiersManager manager)
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
		return NTF_BLEEDISH;
	}


	private void DisplayBadge()
	{
		
		GetDisplayStatus().SetStatus(DELM_BADGE_BLEEDING,true);
		GetDisplayStatus().SetStatus(DELM_NTFR_BLOOD,DELM_LVL_1);
	}

	private void HideBadge()
	{
		
		GetDisplayStatus().SetStatus(DELM_BADGE_BLEEDING,false);
		GetDisplayStatus().SetStatus(DELM_NTFR_BLOOD,DELM_LVL_0);
	}

	private void DisplayMessage()
	{
		m_Player.MessageImportant("I feel warm blood on my clothes");
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