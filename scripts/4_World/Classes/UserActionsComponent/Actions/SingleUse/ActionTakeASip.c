class ActionTakeASip: ActionTakeABite
{
	void ActionTakeASip()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_DRINKCAN;
		m_MessageSuccess = "I took a sip.";
	}
	
	int GetType()
	{
		return AT_TAKE_A_SIP;
	}

	string GetText()
	{
		return "take a sip";
	}
};