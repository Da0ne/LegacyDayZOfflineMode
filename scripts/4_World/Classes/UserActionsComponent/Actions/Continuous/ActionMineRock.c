class ActionMineRockCB : ActionContinuousBaseCB
{
	private const float TIME_BETWEEN_MATERIAL_DROPS = 8;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousMineRock(TIME_BETWEEN_MATERIAL_DROPS);
	}
};

class ActionMineRock: ActionContinuousBase
{
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	
	void ActionMineRock()
	{
		m_CallbackClass = ActionMineRockCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CHOPTREE;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
		m_CallbackClass = ActionMineRockCB;
		m_MessageStartFail = "My tool is too damaged to mine.";
		m_MessageStart = "I started mining the rock.";
		m_MessageSuccess = "I have mined the rock.";
		m_MessageFail = "My tool is ruined.";
		//m_AnimationOneHanded = "DRINK";
		//m_AnimationTwoHanded = "DIGGINGSHOVEL";
	}
	
	void CreateConditionComponents() 
	{		
		m_ConditionTarget = new CCTCursor(MAXIMAL_TARGET_DISTANCE);
		m_ConditionItem = new CCINonRuined;
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{	
		if ( target.IsRock() )
		{
			return true;
		}
		return false;
	}

	int GetType()
	{
		return AT_MINE_ROCK;
	}
		
	string GetText()
	{
		return "mine";
	}


	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
	}
};