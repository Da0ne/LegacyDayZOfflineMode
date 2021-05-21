class CAContinuousRepeat : CABase
{
	protected float					m_TimeElpased;
	protected float					m_TimeToComplete;
	protected float					m_DefaultTimeToComplete;
	protected autoptr Param1<float>	m_SpentUnits;
	
	void CAContinuousRepeat( float time_to_complete_action )
	{
		m_ContinuousAction = true;
		m_DefaultTimeToComplete = time_to_complete_action;
	}
	
	void Setup( PlayerBase player, Object target, ItemBase item  )
	{
		m_TimeElpased = 0;
		if ( !m_SpentUnits )
		{ 
			m_SpentUnits = new Param1<float>(0);
		}
		else
		{	
			m_SpentUnits.param1 = 0;
		}
		m_TimeToComplete = GetTimeToComplete(player, target, item);
	}
	
	int Execute( PlayerBase player, Object target, ItemBase item  )
	{
		if ( !player || !item )
		{
			return UA_ERROR;
		}
		
		if ( item.GetDamage() == 1 )
		{
			return UA_FAILED;
		}
		else
		{
			if ( m_TimeElpased < m_TimeToComplete )
			{
				m_TimeElpased += GetDeltaT();
				return UA_PROCESSING;
			}
			else
			{
				Setup(player,target,item);
				return UA_REPEAT;	
			}
		}	
	}

	int Interupt( PlayerBase player, Object target, ItemBase item )
	{
		if ( m_SpentUnits )
		{
			m_SpentUnits.param1 = m_TimeElpased;
			SetACData(m_SpentUnits);
		}
		return UA_FINISHED;
	}
	
	//---------------------------------------------------------------------------
	
	float GetTimeToComplete( PlayerBase player, Object target, ItemBase item )
	{
		float result = m_DefaultTimeToComplete;
		if ( m_Action )
		{
			float efficiency = player.GetTimeToCompleteEfficiency( m_Action.GetType() );
			if ( efficiency != -1 )
			{
				result = m_DefaultTimeToComplete / efficiency;
			}
		}
		return result;
	}
};