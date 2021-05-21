class ActionWorldCraftCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 2;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionWorldCraft: ActionContinuousBase
{
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	autoptr Param2<int,string> m_Param;
	EntityAI m_Target_C;
	EntityAI m_Item_C;
	Man m_Player_C;
		
	void ActionWorldCraft()
	{
		m_CallbackClass = ActionWorldCraftCB;		
		m_MessageStartFail = "Ooops";
		m_MessageStart = "I have started crafting.";
		m_MessageSuccess = "I crafted something!";
		m_MessageFail = "Oops again..";
		m_Param = new Param2<int,string>(0,"");
		
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined(MAXIMAL_TARGET_DISTANCE);
	}

	int GetType()
	{
		return AT_WORLD_CRAFT;
	}
		
	string GetText()
	{
		return "craft";
	}
		
	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{	
		if ( item && target && target.IsItemBase() )
		{
			m_Item_C = item;
			m_Target_C = target;
			if ( player.m_ModuleRecipesManager.GetValidAction(m_Item_C,m_Target_C,m_Param, player) )
			{
				return true;
			}
		}
		return false;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		if ( m_Item_C && m_Target_C && m_Param )
		{
			m_Player_C = player;
			player.m_ModuleRecipesManager.PerformRecipeServer(m_Param.param1,m_Item_C,m_Target_C,m_Player_C);
		}
	}
};

