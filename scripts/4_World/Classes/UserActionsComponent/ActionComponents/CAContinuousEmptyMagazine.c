class CAContinuousEmptyMagazine : CABase
{
	protected float 				m_TargetUnits;
	protected float 				m_SpentQuantity;
	protected float 				m_Time;
	protected float 				m_TimeToEjectOneBullet;
	protected autoptr Param1<float>	m_SpentUnits;
	
	void CAContinuousEmptyMagazine( float eject_time )
	{
		m_ContinuousAction = true;
		m_TimeToEjectOneBullet = eject_time;
		m_Time = 0;
	}
	
	void Setup( PlayerBase player, Object target, ItemBase item  )
	{
		if ( !m_SpentUnits )
		{ 
			m_SpentUnits = new Param1<float>(0);
		}
		else
		{	
			m_SpentUnits.param1 = 0;
		}
		m_SpentQuantity = 0;
		Magazine itm = (Magazine)item;
		m_TargetUnits = itm.GetAmmoCount();	
	}
	
	int Execute( PlayerBase player, Object target, ItemBase item  )
	{
		if ( !player || !item )
		{
			return UA_ERROR;
		}
		
		if ( m_TargetUnits == 0 )
		{
			return UA_FAILED;
		}
		else
		{
			if ( m_SpentQuantity < m_TargetUnits )
			{
				m_Time += GetDeltaT();
				if ( m_Time > m_TimeToEjectOneBullet )
				{
					m_Time = 0;
					m_SpentQuantity += 1;
				}
				return UA_PROCESSING;
			}
			else
			{
				CalcAndSetQuantity(player,target,item);
				return UA_FINISHED;
			}
		}	
	}

	int Interupt( PlayerBase player, Object target, ItemBase item  )
	{
		if ( !player || !item )
		{
			return UA_ERROR;
		}
		
		CalcAndSetQuantity(player, target, item);
		return UA_FINISHED;
	}
	
	//---------------------------------------------------------------------------
	
	void CalcAndSetQuantity( PlayerBase player, Object target, ItemBase item )
	{
		if ( item )
		{
			Magazine itm = (Magazine)item;
			if ( m_SpentUnits )
			{
				m_SpentUnits.param1 = m_SpentQuantity;
				SetACData(m_SpentUnits);
			}		
			itm.AddAmmoCount(-m_SpentQuantity);
		}
	}
};