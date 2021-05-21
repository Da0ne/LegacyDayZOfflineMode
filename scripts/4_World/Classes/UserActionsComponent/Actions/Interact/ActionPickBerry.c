class ActionPickBerry: ActionInteractBase
{
	private const float MAXIMAL_TARGET_DISTANCE2 = 1.5;
	
	void ActionPickBerry()
	{
		m_MessageSuccess = "I have harvested it.";
		m_MessageStartFail = "Fail..";
		//m_MaximalActionDistance = 1;
		
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_BERRIES;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT;
	}

	void CreateConditionComponents() 
	{
		m_ConditionTarget = new CCTCursor(MAXIMAL_TARGET_DISTANCE2);
		m_ConditionItem = new CCINone;
	}

	int GetType()
	{
		return AT_PICK_BERRY;
	}

	string GetText()
	{
		return "harvest";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{	
		if ( player && target && target.IsWoodBase() )
		{
			return true;
		}
		return false;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		WoodBase ntarget = target;
		ntarget.AddDamage((1 / Math.Max(1,ntarget.GetAmountOfDrops(item))), true);
		ItemBase drop = GetGame().CreateObject(ntarget.GetMaterial(NULL),player.GetPosition(), false);
		drop.SetQuantity2(ntarget.GetAmountOfMaterialPerDrop(item),false);
	}
};