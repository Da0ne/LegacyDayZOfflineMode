class ActionInteractBaseCB : ActionBaseCB
{
	void CreateActionComponent()
	{
		m_ActionComponent = new CAInteract;
	}
};

class ActionInteractBase : ActionBase
{
	private const float MAXIMAL_TARGET_DISTANCE = 1.5;
	
	void ActionInteractBase() 
	{
		m_CallbackClass = ActionInteractBaseCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EAT;
	}
	
	void CreateConditionComponents() 
	{
		m_ConditionItem = new CCINone;
		m_ConditionTarget = new CCTNonRuined(MAXIMAL_TARGET_DISTANCE);
	}
};