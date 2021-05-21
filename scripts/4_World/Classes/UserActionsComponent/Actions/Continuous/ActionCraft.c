class ActionCraftCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 4;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionCraft: ActionContinuousBase
{
	bool m_HasStarted;
		
	ItemBase m_Item1;
	ItemBase m_Item2;
		
	int m_RecipeID;
	
	void ActionCraft()
	{
		m_MessageStartFail = "Ooops";
		m_MessageStart = "I have started crafting.";
		m_MessageSuccess = "I crafted something!";
		m_MessageFail = "Oops again..";
		
		m_Sound = "craft_universal_0";
		
		m_CallbackClass = ActionCraftCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
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
		return AT_CRAFT;
	}
		
	string GetText()
	{
		return "craft";
	}

		
		
	void OnStart( PlayerBase player, Object target, ItemBase item )
	{
		if( player.m_Item1 ) 			m_Item1 = player.m_Item1;
		if( player.m_Item2 ) 			m_Item2 = player.m_Item2;
		if( player.m_RecipeID > -1 )	m_RecipeID = player.m_RecipeID;
		m_HasStarted = true;

	}
	
	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{	
		if( !m_HasStarted )
		{
			if ( player.IsCraftingSetUp() )
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return true;
		}
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		player.PerformRecipe(m_RecipeID, m_Item1,m_Item2);
		m_HasStarted = false;
	}

	void OnCancel ( PlayerBase player, Object target, ItemBase item, Param acdata  )
	{
		m_HasStarted = false;
	}

	void CancelCraft(PlayerBase player)
	{
		player.SetCrafting(false);
	}
};