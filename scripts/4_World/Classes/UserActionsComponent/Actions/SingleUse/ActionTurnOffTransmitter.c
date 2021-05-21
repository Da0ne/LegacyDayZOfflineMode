class ActionTurnOffTransmitterCB : ActionSingleUseBaseCB
{
	void CreateActionComponent()
	{
		m_ActionComponent = new CASingleUse;
	}
};

class ActionTurnOffTransmitter: ActionTurnOffWhileInHands
{
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	
	void ActionTurnOffTransmitter()
	{
		m_CallbackClass = ActionTurnOffTransmitterCB;
		m_MessageSuccess = "I have turned it off.";
		m_MessageFail = "I have failed to turn it off.";
		//m_Animation = "";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(MAXIMAL_TARGET_DISTANCE);
	}

	int GetType()
	{
		return AT_TURN_OFF_TRANSMITTER;
	}

	string GetText()
	{
		return "turn off";
	}
};