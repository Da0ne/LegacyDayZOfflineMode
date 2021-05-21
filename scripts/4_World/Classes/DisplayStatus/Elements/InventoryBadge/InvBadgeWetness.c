class InvBadgeWetness extends DisplayElement
{
	void InvBadgeWetness()
	{
		m_Colors[1] = 0xFF9090FF;
		m_Colors[2] = 0xFF6060FF;
		m_Colors[3] = 0xFF3030FF;
		m_Colors[3] = 0xFF0000FF;
		
		m_Labels[1] = "DAMP";
		m_Labels[2] = "WET";
		m_Labels[3] = "SOAKED";
		m_Labels[3] = "DRENCHED";
		

		m_Key 	= 	NTFKEY_WETNESS;
		m_Pos 	= 	NTFP_WETNESS;
		m_Type	=	TYPE_NTFR;
	}
}