class CAContinuousWaterPlant : CAContinuousQuantity
{
	protected float m_PlantThirstyness;

	void CAContinuousWaterPlant( float quantity_used_per_second )
	{
		m_ContinuousAction = true;
		m_QuantityUsedPerSecond = quantity_used_per_second;
	}
	
	void Setup( PlayerBase player, Object target, ItemBase item )
	{
		if (target.IsKindOf("EN5C_PlantBase"))
		{
			EN5C_PlantBase target_PB = (EN5C_PlantBase) target;
			m_SpentQuantity = 0;
			if ( !m_SpentUnits )
			{ 
				m_SpentUnits = new Param1<float>(0);
			}
			else
			{	
				m_SpentUnits.param1 = 0;
			}
			if ( item ) m_ItemQuantity = item.GetQuantity2();
			if ( target_PB ) m_PlantThirstyness = target_PB.GetWaterMax() - target_PB.GetWater();
		}
	}
	
	
	int Execute( PlayerBase player, Object target, ItemBase item  )
	{
		if ( !player || !item || !target )
		{
			return UA_ERROR;
		}
		
		if ( m_ItemQuantity <= 0 )
		{
			return UA_FAILED;
		}
		else
		{
			if ( m_SpentQuantity < m_ItemQuantity  &&  m_SpentQuantity < m_PlantThirstyness )
			{
				m_SpentQuantity += m_QuantityUsedPerSecond * GetDeltaT();
				float val = m_ItemQuantity-m_SpentQuantity;
				if ( m_Action ) m_Action.SendMessageToClient(player, val.ToString());
				return UA_PROCESSING;
			}
			else
			{
				CalcAndSetQuantity(player, target, item);
				return UA_FINISHED;
			}
		}	
	}
};