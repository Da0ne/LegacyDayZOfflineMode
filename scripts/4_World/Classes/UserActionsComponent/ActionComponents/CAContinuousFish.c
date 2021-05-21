class CAContinuousFish : CABase
{
	protected EN5C_FishingRod_Base m_Rod;
	
	void CAContinuousFish()
	{
		m_ContinuousAction = true;
	}

	void Setup( PlayerBase player, Object target, ItemBase item  )
	{
		m_Rod = item;
	}
	
	int Execute( PlayerBase player, Object target, ItemBase item  )
	{
		if ( m_Rod != item )
		{
			return UA_ERROR;
		}
		
		m_Rod.AddPull(GetDeltaT());
		return UA_PROCESSING;	
	}
};