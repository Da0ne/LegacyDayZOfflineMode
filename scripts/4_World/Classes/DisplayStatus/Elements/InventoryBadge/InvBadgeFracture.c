class InvBadgeFracture extends DisplayElement
{
	void InvBadgeFracture()
	{
		m_Colors[1] = 0xFFFF9090;
		m_Colors[2] = 0xFFFF6060;
		m_Colors[3] = 0xFFFF1515;
		
		m_Labels[1] = "SPRAINED";
		m_Labels[2] = "CHIPPED";
		m_Labels[3] = "FRACTURED";

		m_Key 	= 	NTFKEY_FRACTURE;
		m_Pos 	= 	NTFP_FRACTURE;
		m_Type	=	TYPE_NTFR;
	}
}