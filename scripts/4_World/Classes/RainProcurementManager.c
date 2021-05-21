class RainProcurementManager
{
	protected 			ItemBase 	m_ProcuringItem;	
	protected 			int 		m_IsUnderRoof;
	protected	autoptr	Timer 		m_UpdateTimer;
	protected 	const 	int 		RAIN_COEFFICIENT = 10;

	void RainProcurementManager( ItemBase procuring_item )
	{
		m_ProcuringItem = procuring_item;
	}
	
	void ~RainProcurementManager()
	{
		delete m_UpdateTimer;
	}

	// ----------------------------------------------------------------------------------------
	void InitRainProcurement()
	{
		m_IsUnderRoof = IsUnderRoof();

		//m_ProcuringItem.SetQuantity2(0); /*set to 0 for debug purposses*/

		if( !m_IsUnderRoof )
		{
			m_UpdateTimer = new Timer();
			m_UpdateTimer.Run( 10, this, "RainProcurementCheck", NULL, true ); 
		}
	}

	// ----------------------------------------------------------------------------------------
	void RainProcurementCheck()
	{		
		float rain_intensity = GetGame().GetRain();
		float fill_per_update = RAIN_COEFFICIENT * rain_intensity;

		if ( GetGame().GetRain() > 0 ) 
		{
			if ( m_ProcuringItem.GetQuantity2() < m_ProcuringItem.GetQuantityMax2() )
			{
				m_ProcuringItem.AddQuantity2( fill_per_update );

				//PrintString( m_ProcuringItem.GetName() + " quantity: " + m_ProcuringItem.GetQuantity2().ToString() );
			}
			else
			{		
				//Print("vesel full");		
				StopRainProcurement();				
			}
		}
	}

	// ----------------------------------------------------------------------------------------
	bool IsUnderRoof() 
	{
		vector from = m_ProcuringItem.GetPosition(); 
		vector ceiling = "0 20 0";
		vector to = from + ceiling;
		vector contact_pos;
		vector contact_dir;
		int contact_component;	
	
		return GetGame().Raycast( from, to, contact_pos, contact_dir, contact_component, NULL, NULL, m_ProcuringItem );	
	}

	// ----------------------------------------------------------------------------------------
	void StopRainProcurement()
	{
		if( !m_IsUnderRoof )
		{
			m_UpdateTimer.Stop();	
		}
		
	}
};