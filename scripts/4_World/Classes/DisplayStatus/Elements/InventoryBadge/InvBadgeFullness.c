class InvBadgeFullness extends DisplayElement
{
	void InvBadgeFullness()
	{
		m_Colors[1] = 0xFFCCCC00;
		m_Colors[2] = 0xFF999900;
		m_Colors[3] = 0xFF666600;
		
		m_Labels[1] = "TOO STUFFED";
		m_Labels[2] = "VERY STUFFED";
		m_Labels[3] = "STUFFED";
		
		m_Key 	= 	NTFKEY_STUFFED;
		m_Pos 	= 	NTFP_STUFFED;
		m_Type	=	TYPE_NTFR;
	}
}