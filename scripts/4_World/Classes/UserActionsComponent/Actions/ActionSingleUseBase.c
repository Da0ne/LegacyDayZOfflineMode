class ActionSingleUseBaseCB : ActionBaseCB
{
	void CreateActionComponent()
	{
		m_ActionComponent = new CASingleUse;
	}
};

class ActionSingleUseBase : ActionBase
{
	void ActionSingleUseBase()
	{
		m_CallbackClass = ActionSingleUseBaseCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EAT;
	}
};