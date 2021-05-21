class ActionWringClothesCB : ActionContinuousBaseCB
{
	private const float QUANTITY_WRINGLED_PER_SECOND = 0.03;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousDryClothes(QUANTITY_WRINGLED_PER_SECOND);
	}
};

class ActionWringClothes: ActionContinuousBase
{
	void ActionWringClothes()
	{
		m_CallbackClass = ActionWringClothesCB;
		//m_CommandUID = DayZPlayerConstants.CMD_ACTION_WRING;
		m_MessageStartFail = "It's ruined.";
		m_MessageStart = "I have started wringing clothes.";
		m_MessageSuccess = "I have finished wringing clothes.";
		m_MessageFail = "Player moved and stopped wringing clothes.";
		m_MessageCancel = "I stopped wringing clothes.";
		
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_WRING;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
	}
	
	void CreateConditionComponents() 
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	int GetType()
	{
		return AT_WRING_CLOTHES;
	}
		
	string GetText()
	{
		return "wring the clothes";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{	
		if( item.GetWet2() >= 0.1 && item.GetWet2() <= item.GetWetMax2())
		{
			return true;	
		}
		else
		{	
			return false;		
		}	
	}		
};