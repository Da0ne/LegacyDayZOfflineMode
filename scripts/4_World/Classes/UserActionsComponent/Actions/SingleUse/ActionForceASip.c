class ActionForceASip: ActionForceABite
{
	int GetType()
	{
		return AT_FORCE_A_SIP;
	}

	string GetText()
	{
		return "give a sip";
	}

	void ActionForceASip()
	{
		m_MessageStart = "Player started giving you a sip.";
		m_MessageSuccess = "Player gave you a sip.";
		m_MessageFail = "Player moved and giving a sip was canceled.";
		//m_Animation = "drink";
		m_Sounds.Insert("DrinkBottle_0");
		m_Sounds.Insert("DrinkBottle_1");
	}
};