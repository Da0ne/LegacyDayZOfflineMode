class ActionPlugIn: ActionSingleUseBase
{
	private const float MAXIMAL_TARGET_DISTANCE = 2;

	void ActionPlugIn()
	{
		
		m_MessageStartFail = "m_MessageStartFail";
		m_MessageStart = "m_MessageStart";
		m_MessageSuccess = "m_MessageSuccess";
		m_MessageFail = "m_MessageFail";
		m_MessageCancel = "m_MessageCancel";
		
		//m_TimeToCompleteAction = 4;
		////m_MaximalActionDistance = 1;
		//m_Animation = "eat";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined(MAXIMAL_TARGET_DISTANCE);
	}
	

	int GetType()
	{
		return AT_PLUG_IN;
	}
		
	string GetText()
	{
		return "plug in";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		if ( target  &&  item )
		{
			ItemBase target_IB = (ItemBase) target;
			
			if ( item.HasEnergyManager()  &&  !item.GetCompEM().IsPlugged()  &&  target_IB.HasEnergyManager()  &&  target_IB.GetCompEM().CanReceivePlugFrom(item) )
			{
				return true;
			}
			
			ItemBase attached_device = GetAttachedDevice(target_IB);
			
			if (attached_device)
			{
				return true;
			}
		}
		
		return false;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		ItemBase target_IB = (ItemBase) target;
		
		if ( target_IB.HasEnergyManager() )
		{
			ItemBase attached_device = GetAttachedDevice(target_IB);
			
			if (attached_device)
			{
				target_IB = attached_device;
			}
			
			item.GetCompEM().PlugThisInto(target_IB);
			
			if ( !player.IsPlacingObject() )
			{
				ActionTogglePlaceObject toggle_action = (ActionTogglePlaceObject) player.m_ActionManager.GetAction ( AT_TOGGLE_PLACE_OBJECT );
				toggle_action.TogglePlacing ( player, target, item );
			}
		}
	}
	
	ItemBase GetAttachedDevice(ItemBase parent)
	{
		string parent_type = parent.GetType();
		
		if ( parent.IsInherited(EN5C_CarBattery)/*  ||  parent.IsInherited(TruckBattery)*/ ) // TO DO: Add truck battery when it's registered in script. Same for helicopter battery.
		{
			ItemBase parent_attachment = parent.GetAttachmentByType(EN5C_MetalWire);
			
			if (!parent_attachment)
				parent_attachment = parent.GetAttachmentByType(EN5C_BarbedWire);
			
			return parent_attachment;
		}
		
		return NULL;
	}
};