class InvBadgeHunger extends DisplayElement
{
	void InvBadgeHunger()
	{
		m_Colors[1] = 0xFF50FF50;
		m_Colors[2] = 0xFF309030;
		m_Colors[3] = 0xFF609060;
		m_Colors[4] = 0xFF906060;
		
		m_Labels[1] = "ENERGIZED";
		m_Labels[2] = "HUNGRY";
		m_Labels[3] = "VERY HUNGRY";
		m_Labels[4] = "STARVING";
		
		m_Key 	= 	NTFKEY_HUNGRY;
		m_Pos 	= 	NTFP_HUNGRY;
		m_Type	=	TYPE_NTFR;
	}
}