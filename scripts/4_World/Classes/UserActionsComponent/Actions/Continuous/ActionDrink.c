class ActionDrinkCB : ActionContinuousBaseCB
{
	private const float QUANTITY_USED_PER_SEC2 = 50;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousQuantityEdible(QUANTITY_USED_PER_SEC2);
	}
};

class ActionDrink: ActionConsume
{
	void ActionDrink()
	{
		m_CallbackClass = ActionDrinkCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_DRINK;
		m_MessageStartFail = "There's nothing to drink.";
		m_MessageStart = "I have started drinking.";
		m_MessageSuccess = "I have finished drinking.";
		m_MessageFail = "Player moved and drinking was canceled.";
		m_MessageCancel = "I stopped drinking.";
		m_Sounds.Insert("DrinkBottle_0");
		m_Sounds.Insert("DrinkBottle_1");
		//m_Animation = "DRINK";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}

	int GetType()
	{
		return AT_DRINK;
	}
		
	string GetText()
	{
		return "drink";
	}
};