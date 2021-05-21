class ActionAttachToCharger: ActionSingleUseBase
{
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	
	void ActionAttachToCharger()
	{
		m_MessageSuccess = "I've attached the battery to the charger";
	}
	
	void CreateConditionComponents() 
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined(MAXIMAL_TARGET_DISTANCE);
	}

	int GetType()
	{
		return AT_ATTACH_TO_CHARGER;
	}
		
	string GetText()
	{
		return "attach battery to charger";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		if ( target  &&  item )
		{
			if ( target.HasEnergyManager() )
			{
				EntityAI target_EAI = (EntityAI) target;
				ItemBase battery = target_EAI.GetCompEM().GetPluggedDevice();
				
				if (battery)
					return false; // Something is already being charged from battery charger
				else
					return true;
				
			}
		}
		
		return false;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		ItemBase target_IB = (ItemBase) target; // cast to ItemBase
		target_IB.TakeEntityAsAttachment (item);
	}
};