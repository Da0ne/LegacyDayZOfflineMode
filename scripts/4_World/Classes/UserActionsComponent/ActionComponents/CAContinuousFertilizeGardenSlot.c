class CAContinuousFertilizeGardenSlot : CAContinuousQuantity
{
	protected float m_SlotFertilizerNeed;

	void CAContinuousWaterPlant( float quantity_used_per_second )
	{
		m_ContinuousAction = true;
		m_QuantityUsedPerSecond = quantity_used_per_second;
	}
	
	void Setup( PlayerBase player, Object target, ItemBase item )
	{
		if (target.IsKindOf("GardenBase"))
		{
			GardenBase target_GB = (GardenBase) target;
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
			
			string selection;
			GetGame().GetPlayerCursorObjectComponentName( player, selection );
			Slot slot = target_GB.GetSlotBySelection( selection );
			
			
			string item_type = item.GetType();
			float consumed_quantity = GetGame().ConfigGetFloat( "cfgVehicles " + item_type + " Horticulture ConsumedQuantity" );
			
			m_SlotFertilizerNeed = slot.GetFertilizerQuantityMax() - consumed_quantity;
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
			if ( m_SpentQuantity < m_ItemQuantity  &&  m_SpentQuantity < m_SlotFertilizerNeed )
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