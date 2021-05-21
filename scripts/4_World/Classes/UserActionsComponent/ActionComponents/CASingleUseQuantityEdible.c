class CASingleUseQuantityEdible : CASingleUseQuantity
{
	void CASingleUseQuantityEdible( float quantity_used_per_action )
	{
		m_QuantityUsedPerAction = quantity_used_per_action;
	}
	
	void CalcAndSetQuantity(PlayerBase player, Object target, ItemBase item)
	{	
		if ( m_SpentUnits )
		{
			m_SpentUnits.param1 = m_QuantityUsedPerAction;
			SetACData(m_SpentUnits);	
		}
		if ( m_Action && m_Action.m_ConditionTarget.IsActionOnTarget() )
		{
			if ( target && target.IsMan() )
			{
				PlayerBase ntarget = target;
				item.Consume(ntarget, m_QuantityUsedPerAction);
			}
		}
		else
		{
			item.Consume(player, m_QuantityUsedPerAction);
		}
	}
};