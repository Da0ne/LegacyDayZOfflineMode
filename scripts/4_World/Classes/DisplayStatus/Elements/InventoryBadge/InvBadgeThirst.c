class InvBadgeThirst extends DisplayElement
{
	void InvBadgeThirst()
	{
		m_Colors[1] = 0xFF0000FF;
		m_Colors[2] = 0xFF3030FF;
		m_Colors[3] = 0xFF6060FF;
		m_Colors[4] = 0xFF9090FF;
		
		m_Labels[1] = "HYDRATED";
		m_Labels[2] = "THIRSTY";
		m_Labels[3] = "VERY THIRSTY";
		m_Labels[4] = "FATALLY THIRSTY";
		
		m_Key 	= 	NTFKEY_THIRSTY;
		m_Pos 	= 	NTFP_THIRSTY;
		m_Type	=	TYPE_NTFR;
	}
}