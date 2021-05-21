class CAContinuousQuantityEdible : CAContinuousQuantity
{

	void CAContinuousQuantityEdible( float quantity_used_per_second )
	{
		m_ContinuousAction = true;
		m_QuantityUsedPerSecond = quantity_used_per_second;
	}
	
	void CalcAndSetQuantity( PlayerBase player, Object target, ItemBase item )
	{	
		if ( m_SpentUnits )
		{
			m_SpentUnits.param1 = m_SpentQuantity;
			SetACData(m_SpentUnits);
		}
		m_SpentQuantity = Math.Floor(m_SpentQuantity);
		if ( m_Action && m_Action.m_ConditionTarget.IsActionOnTarget() )
		{
			if ( target && target.IsMan() )
			{
			
				PlayerBase ntarget = target;
				item.Consume(target, m_SpentQuantity);
			}
		}
		else
		{
			item.Consume(player, m_SpentQuantity);
		}
	}
};