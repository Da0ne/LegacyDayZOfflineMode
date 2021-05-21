// This is just a test action!
class ActionRepairWithToolFromHands: ActionSingleUseBase
{
	private const float MAXIMAL_TARGET_DISTANCE = 2;

	void ActionRepairWithToolFromHands()
	{
		Print("ActionRepairWithToolFromHands");
		m_MessageSuccess = "";
		m_MessageStartFail = "";
		m_MessageStart = "";
		m_MessageSuccess = "";
		m_MessageFail = "";
		m_MessageCancel = "";
		
		//m_Animation = "open";
	}
	
	void CreateConditionComponents() 
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined(MAXIMAL_TARGET_DISTANCE);
	}

	int GetType()
	{
		return 0; //AT_REPAIR;
	}

	string GetText()
	{
		return "repair";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		Print("ActionRepairWithToolFromHands::ActionCondition");
		Print(target);
		if ( target  /*&&  target.IsKindOf("ItemBase")*/ )
		{
			ItemBase item_to_repair = (ItemBase) target;
			bool can_repair = item_to_repair.CanRepair(item);
			Print(can_repair);
			return can_repair;
		}
		
		return false;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		Print("ActionRepairWithToolFromHands::OnComplete");
		if ( target  /* &&  target.IsKindOf("ItemBase")*/ )
		{
			ItemBase item_to_repair = (ItemBase) target;
			Param1<float> nacdata = acdata;
			bool was_repaired = item_to_repair.Repair(player, item, nacdata.param1);
			Print(was_repaired);
		}
	}
};
