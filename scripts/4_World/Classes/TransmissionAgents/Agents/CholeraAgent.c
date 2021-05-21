class CholeraAgent extends AgentBase
{
	void Init()
	{
		m_Type 					= AGT_CHOLERA;
		m_Invasibility 			= 0.5;//increase the agent count by this number per second
		m_TransferabilityIn		= 0.1;//to the player
		m_TransferabilityOut	= 0;//from the player
		m_MaxCount 				= 1000;
	}
}