class ActionPlugIntoFence: ActionSingleUseBase
{
	private const float MAXIMAL_TARGET_DISTANCE = 6;

	void ActionPlugIntoFence()
	{
		m_MessageStartFail = "m_MessageStartFail";
		m_MessageStart = "m_MessageStart";
		m_MessageSuccess = "m_MessageSuccess";
		m_MessageFail = "m_MessageFail";
		m_MessageCancel = "m_MessageCancel";
	}
	
	void CreateConditionComponents() 
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined(MAXIMAL_TARGET_DISTANCE);
	}

	int GetType()
	{
		return AT_PLUG_INTO_FENCE;
	}
	
	string GetText()
	{
		return "plug in";
	}
	
	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		if ( target  &&  item  &&  target.IsInherited(BaseBuildingBase))
		{
			BaseBuildingBase target_BBB = (BaseBuildingBase) target;
			
			EN5C_BarbedWire bw = target_BBB.GetAttachmentByType(EN5C_BarbedWire);
			
			if ( bw  &&  bw.GetCompEM().CanReceivePlugFrom(item) )
			{
				return true;
			}
		}
		
		return false;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		ItemBase target_IB = (ItemBase) target;
		EN5C_BarbedWire bw = target_IB.GetAttachmentByType(EN5C_BarbedWire);
		item.GetCompEM().PlugThisInto(bw);
		
		if ( !player.IsPlacingObject() )
		{
			ActionTogglePlaceObject toggle_action = (ActionTogglePlaceObject) player.m_ActionManager.GetAction ( AT_TOGGLE_PLACE_OBJECT );
			toggle_action.TogglePlacing ( player, target, item );
		}
	}
};