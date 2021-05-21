class AgentInfectionTendency: NotifierBase
{

	private const float 	DEC_TRESHOLD_LOW 	= -0.5;
	private const float 	DEC_TRESHOLD_MED 	= -1;
	private const float 	DEC_TRESHOLD_HIGH	= -2;
	private const float 	INC_TRESHOLD_LOW 	= 0.5;
	private const float 	INC_TRESHOLD_MED 	= 1;
	private const float 	INC_TRESHOLD_HIGH	= 2;
	
	void AgentInfectionTendency(NotifiersManager manager)
	{
		SetActive(true);
	}

	int GetNotifierType()
	{
		return NTF_AGENT_INFECTION;
	}
	
	bool ActivateCondition()
	{
		return true;
	}

	private void DisplayTendency(float delta)
	{
		float tendency = CalculateTendency(delta, INC_TRESHOLD_LOW, INC_TRESHOLD_MED, INC_TRESHOLD_HIGH, DEC_TRESHOLD_LOW, DEC_TRESHOLD_MED, DEC_TRESHOLD_HIGH);
		GetDisplayStatus().SetStatus(DELM_TDCY_BACTERIA,tendency);
		
	}
	
	protected float GetObservedValue()
	{
		int count = m_Player.GetTotalAgentCount();
		//Debug.Log( "GetObservedValue:" + count.ToString(),"Tendency");
		return count;
	}
};