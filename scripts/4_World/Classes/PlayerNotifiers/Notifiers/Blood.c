class Blood: NotifierBase
{
	private const float 	DEC_TRESHOLD_LOW 	= -0.5;
	private const float 	DEC_TRESHOLD_MED 	= -1;
	private const float 	DEC_TRESHOLD_HIGH	= -2;
	private const float 	INC_TRESHOLD_LOW 	= 0.5;
	private const float 	INC_TRESHOLD_MED 	= 1;
	private const float 	INC_TRESHOLD_HIGH	= 2;

	void Blood(NotifiersManager manager)
	{
	}

	int GetNotifierType()
	{
		return NTF_BLOOD;
	}

	protected void DisplayTendency(float delta)
	{
		float tendency = CalculateTendency(delta, INC_TRESHOLD_LOW, INC_TRESHOLD_MED, INC_TRESHOLD_HIGH, DEC_TRESHOLD_LOW, DEC_TRESHOLD_MED, DEC_TRESHOLD_HIGH);
		GetDisplayStatus().SetStatus(DELM_TDCY_BLOOD,tendency);
		
	}

	protected float GetObservedValue()
	{
		float blood = m_Player.GetHealth("GlobalHealth","Blood");
		return blood;
	}
};