class Lives: NotifierBase
{
	private const float 	DEC_TRESHOLD_LOW 			= -0.5;
	private const float 	DEC_TRESHOLD_MED 			= -1;
	private const float 	DEC_TRESHOLD_HIGH			= -2;
	private const float 	INC_TRESHOLD_LOW 			= 0.5;
	private const float 	INC_TRESHOLD_MED 			= 1;
	private const float 	INC_TRESHOLD_HIGH			= 2;
	
	void Lives(NotifiersManager manager)
	{
	}

	int GetNotifierType()
	{
		return NTF_LIVES;
	}

	
	void DisplayTendency(float delta)
	{
		float tendency = CalculateTendency(delta, INC_TRESHOLD_LOW, INC_TRESHOLD_MED, INC_TRESHOLD_HIGH, DEC_TRESHOLD_LOW, DEC_TRESHOLD_MED, DEC_TRESHOLD_HIGH);
		
	}

	private void DisplayBadge()
	{
	}

	private void HideBadge()
	{
	}

	private void DisplayMessage()
	{
	}

	private void PlayAnimation()
	{
	}

	private void PlaySound()
	{
	}
	
		
	protected float GetObservedValue()
	{
		return m_Player.GetHealth("","Health");
	}
};