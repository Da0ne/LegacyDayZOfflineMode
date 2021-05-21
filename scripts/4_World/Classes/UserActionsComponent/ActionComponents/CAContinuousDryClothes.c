class CAContinuousDryClothes : CABase
{
	protected float 				m_TargetUnits;
	protected float 				m_SpentQuantity;
	protected float 				m_ItemQuantity;
	protected float 				m_QuantityWringledPerSecond;
	protected autoptr Param1<float>	m_SpentUnits;
	
	void CAContinuousDryClothes( float quantity_wringled_per_second )
	{
		m_ContinuousAction = true;
		m_QuantityWringledPerSecond = quantity_wringled_per_second;
	}
	
	void Setup( PlayerBase player, Object target, ItemBase item  )
	{
		m_SpentQuantity = 0;
		if ( !m_SpentUnits )
		{ 
			m_SpentUnits = new Param1<float>(0);
		}
		else
		{	
			m_SpentUnits.param1 = 0;
		}
		m_ItemQuantity = item.GetWet2();
		m_TargetUnits = item.GetWetMax2();
	}
	
	int Execute( PlayerBase player, Object target, ItemBase item  )
	{
		if ( !player || !item  )
		{
			return UA_ERROR;
		}
		
		if ( item.GetWet2() >= item.GetWetMax2() )
		{
			return UA_FAILED;
		}
		else
		{
			if ( m_SpentQuantity < m_TargetUnits )
			{
				m_SpentQuantity -= m_QuantityWringledPerSecond * GetDeltaT();
				float val = m_ItemQuantity-m_SpentQuantity;
				if ( m_Action ) m_Action.SendMessageToClient(player, val.ToString());
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
		if ( m_SpentUnits )
		{
			m_SpentUnits.param1 = m_SpentQuantity;
			SetACData(m_SpentUnits);
		}
		item.AddWet2(m_SpentQuantity); // although we are using AddWet2 function, we are adding negative number, which effectively means subtracting
	}
};