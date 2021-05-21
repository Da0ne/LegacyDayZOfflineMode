class CAContinuousLoadMagazine : CABase
{
	protected float 				m_TargetUnits;
	protected float 				m_SpentQuantity;
	protected float 				m_ItemQuantity;
	protected float 				m_SourceQuantity;
	protected float 				m_QuantityFilledPerSecond;
	protected autoptr Param1<float>	m_SpentUnits;
	
	void CAContinuousLoadMagazine( float quantity_filled_per_second )
	{
		m_ContinuousAction = true;
		m_QuantityFilledPerSecond = quantity_filled_per_second;
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
		
		Magazine itm = (Magazine)item;
		m_ItemQuantity = itm.GetAmmoCount();
		if ( target.IsItemBase() )
		{
			Magazine trg = (Magazine)target;
			m_SourceQuantity = trg.GetAmmoCount();
		}
		m_TargetUnits = itm.GetAmmoMax();	
	}
	
	int Execute( PlayerBase player, Object target, ItemBase item  )
	{
		if ( !player || !item || !target || (target && !target.IsItemBase()) )
		{
			return UA_ERROR;
		}
		Magazine trg = (Magazine)target;
		Magazine itm = (Magazine)item;
		if ( itm.GetAmmoCount() >= itm.GetAmmoMax() || trg.GetAmmoCount() == 0 )
		{
			return UA_FAILED;
		}
		else
		{
			if ( m_SpentQuantity < m_TargetUnits && m_SpentQuantity < m_SourceQuantity )
			{
				m_SpentQuantity += m_QuantityFilledPerSecond * GetDeltaT();
				float val = m_ItemQuantity-m_SpentQuantity;
				if ( m_Action ) m_Action.SendMessageToClient(player, val.ToString());
				return UA_PROCESSING;
			}
			else
			{
				CalcAndSetQuantity(player,trg,itm);
				return UA_FINISHED;
			}
		}	
	}

	int Interupt( PlayerBase player, Object target, ItemBase item  )
	{
		if ( !player || !item || !target )
		{
			return UA_ERROR;
		}
		
		CalcAndSetQuantity(player, target, item);
		return UA_FINISHED;
	}
	
	//---------------------------------------------------------------------------
	
	void CalcAndSetQuantity( PlayerBase player, Magazine target, Magazine item )
	{
		if ( m_SpentUnits )
		{
			m_SpentUnits.param1 = m_SpentQuantity;
			SetACData(m_SpentUnits);
		}		
		item.AddAmmoCount(m_SpentQuantity);
		target.AddAmmoCount(-m_SpentQuantity);
	}
};