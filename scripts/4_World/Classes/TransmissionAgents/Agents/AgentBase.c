class AgentBase extends MessageReceiverBase
{
	float			m_Type = 0;
	float			m_Invasibility;//increase the agent count by this number per second
	float 			m_TransferabilityIn;//to the player
	float			m_TransferabilityOut;//from the player
	int				m_MaxCount = 1;

	void AgentBase()
	{
		Init();
	}
	
	void Init();
	
	int GetAgentType()
	{
		return m_Type;
	}
	float GetInvasibility()
	{
		return m_Invasibility;
	}

	float GetTransferabilityIn()
	{
		return m_TransferabilityIn;
	}

	float GetTransferabilityOut()
	{
		return m_TransferabilityOut;
	}

	int GetMaxCount()
	{
		return m_MaxCount;
	}

	string GetName()
	{
		return ClassName();
	}

}