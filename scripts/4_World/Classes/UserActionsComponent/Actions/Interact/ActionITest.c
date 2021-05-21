class ActionITest: ActionInteractBase
{
	void ActionITest()
	{
		m_MessageSuccess = "I have taken one.";
		m_MessageStart = "I started taking one.";
		m_MessageStartFail = "Its' empty";
	}
	
	int GetType()
	{
		return AT_ITEST;
	}

	string GetText()
	{
		return "take one";
	}
};