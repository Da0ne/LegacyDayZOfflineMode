class ActionMineBushCB : ActionContinuousBaseCB
{
	private const float TIME_BETWEEN_MATERIAL_DROPS = 3;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousMineWood(TIME_BETWEEN_MATERIAL_DROPS);
	}
};

class ActionMineBush: ActionContinuousBase
{
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	
	void ActionMineBush()
	{
		m_CallbackClass = ActionMineBushCB;
		m_MessageStartFail = "My tool is too damaged to cut.";
		m_MessageStart = "I started cutting the bush down.";
		m_MessageSuccess = "I have cut the bush down.";
		m_MessageFail = "My tool is ruined.";
		//m_AnimationOneHanded = "DRINK";
		//m_AnimationTwoHanded = "DIGGINGSHOVEL";
	}
	
	void CreateConditionComponents() 
	{
		m_ConditionTarget = new CCTCursor(MAXIMAL_TARGET_DISTANCE);
		m_ConditionItem = new CCINonRuined;
	}

	int GetType()
	{
		return AT_MINE_BUSH;
	}
		
	string GetText()
	{
		return "cut the bush down";
	}

		
	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{	
		if ( target.IsBush() )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
	}
};