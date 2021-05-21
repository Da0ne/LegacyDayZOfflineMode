class InvBadgeBlood extends DisplayElement
{
	void InvBadgeBlood()
	{
		m_Colors[1] = 0xFFFF0000;
		
		m_Labels[1] = "BLEEDING";

		
		m_Key 	= 	NTFKEY_BLEEDISH;
		m_Pos 	= 	NTFP_BLEEDISH;
		m_Type	=	TYPE_NTFR;
	}
}