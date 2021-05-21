class InvBadgeSick extends DisplayElement
{
	void InvBadgeSick()
	{
		m_Colors[1] = 0xFFFFFF33;
		
		m_Labels[1] = "SICK";

		m_Key 	= 	NTFKEY_SICK;
		m_Pos 	= 	NTFP_SICK;
		m_Type	=	TYPE_NTFR;
	}
}