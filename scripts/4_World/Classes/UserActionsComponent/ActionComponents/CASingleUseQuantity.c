class CASingleUseQuantity : CABase
{	
	protected float 				m_QuantityUsedPerAction;
	protected float 				m_ItemQuantity;
	protected autoptr Param1<float>	m_SpentUnits;

	void CASingleUseQuantity( float quantity_used_per_action )
	{
		m_QuantityUsedPerAction = quantity_used_per_action;	
	}
	
	void Setup( PlayerBase player, Object target, ItemBase item )
	{
		if ( !m_SpentUnits )
		{ 
			m_SpentUnits = new Param1<float>(0);
		}
		else
		{	
			m_SpentUnits.param1 = 0;
		}
		m_ItemQuantity = item.GetQuantity2();
	}
	
	int Execute( PlayerBase player, Object target, ItemBase item )
	{
		if ( !player || !item )
		{
			return UA_ERROR;
		}
		
		if ( m_ItemQuantity <= 0 )
		{
			return UA_FAILED;
		}
		CalcAndSetQuantity(player, target, item);
		return UA_FINISHED;	
	}
	
	int Interupt( PlayerBase player, Object target, ItemBase item )
	{
		return UA_CANCEL;
	}
	
	//---------------------------------------------------------------------------
	
	void CalcAndSetQuantity( PlayerBase player, Object target, ItemBase item )
	{
		if ( m_SpentUnits )
		{
			m_SpentUnits.param1 = m_QuantityUsedPerAction;
			SetACData(m_SpentUnits);	
		}
		item.AddQuantity2(-m_QuantityUsedPerAction,false);
	}
};