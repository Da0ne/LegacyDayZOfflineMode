class InfluenzaAgent extends AgentBase
{
	void Init()
	{
		m_Type 					= AGT_INFLUENZA;
		m_Invasibility 			= 0.1;//increase the agent count by this number per second
		m_TransferabilityIn		= 1;//to the player
		m_TransferabilityOut	= 1;//from the player
		m_MaxCount 				= 1000;
	}

}