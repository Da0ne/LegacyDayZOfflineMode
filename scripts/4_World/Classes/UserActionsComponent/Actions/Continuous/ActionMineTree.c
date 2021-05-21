class ActionMineTreeCB : ActionContinuousBaseCB
{
	private const float TIME_BETWEEN_MATERIAL_DROPS = 4;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousMineWood(TIME_BETWEEN_MATERIAL_DROPS);
	}
};

class ActionMineTree: ActionContinuousBase
{		
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	
	void ActionMineTree()
	{
		m_CallbackClass = ActionMineTreeCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CHOPTREE;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
		m_MessageStartFail = "My tool is too damaged to cut.";
		m_MessageStart = "I started cutting down the tree.";
		m_MessageSuccess = "I have cut down the tree.";
		m_MessageFail = "My tool is ruined.";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionTarget = new CCTCursor(MAXIMAL_TARGET_DISTANCE);
		m_ConditionItem = new CCINonRuined;
	}

	int GetType()
	{
		return AT_MINE_TREE;
	}
		
	string GetText()
	{
		return "cut tree down";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{	
		if ( target.IsTree() )
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