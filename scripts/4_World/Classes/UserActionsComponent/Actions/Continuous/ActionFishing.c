class ActionFishingCB : ActionContinuousBaseCB
{
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousFish;
	}
};

class ActionFishing: ActionContinuousBase
{
	void ActionFishing()
	{
		m_CallbackClass = ActionFishingCB;
		m_Sounds.Insert("FishStruggling_0");
		m_Sounds.Insert("FishStruggling_1");
		m_Sounds.Insert("FishStruggling_2");
	}
	
	void CreateConditionComponents() 
	{
		
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}
	
	int GetType()
	{
		return AT_FISHING;
	}
		
	string GetText()
	{
		return "Pull";
	}
	
	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{	
		EN5C_FishingRod_Base nitem = item;
		if ( nitem.IsFishingActive() )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
};

	