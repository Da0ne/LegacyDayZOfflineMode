class ActionContinuousBaseCB : ActionBaseCB
{};

class ActionContinuousBase : ActionBase
{
	void ActionContinuousBase() 
	{
		m_CallbackClass = ActionContinuousBaseCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EAT;
	}
};