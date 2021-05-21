class ActionLoadMagazineCB : ActionContinuousBaseCB
{
	private const float QUANTITY_FILLED_PER_SEC = 1;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousLoadMagazine(QUANTITY_FILLED_PER_SEC);
	}
};

class ActionLoadMagazine: ActionContinuousBase
{
	void ActionLoadMagazine()
	{
		m_CallbackClass = ActionLoadMagazineCB;
		m_MessageStartFail = "It's ruined.";
		m_MessageStart = "I have started filling magazine.";
		m_MessageSuccess = "I have finished filling the magazine.";
		m_MessageFail = "It broke.";
		m_MessageCancel = "I stopped filling the magazine.";
		//m_Animation = "EATTABLETS";		
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}
	/*
	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{	
		if ( item.GetQuantity2() <= item.GetQuantityMax2() ) 
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	*/
	int GetType()
	{
		return AT_LOAD_MAGAZINE;
	}
		
	string GetText()
	{
		return "load magazine";
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		if ( target )
		{
			Magazine trg = (Magazine)target;
			if ( trg.GetAmmoCount() == 0 ) GetGame().ObjectDelete(trg);
		}
	}
};