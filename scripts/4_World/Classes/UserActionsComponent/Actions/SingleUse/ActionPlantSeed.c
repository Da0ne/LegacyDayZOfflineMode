class ActionPlantSeed: ActionSingleUseBase
{
	void ActionPlantSeed()
	{
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
		m_ConditionTarget = new CCTDummy;
	}

	int GetType()
	{
		return AT_PLANT_SEED;
	}

	string GetText()
	{
		return "Plant seed";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		if ( target != NULL && target.IsInherited( GardenBase) )
		{
			GardenBase garden_base = (GardenBase)target;
			string selection;
			GetGame().GetPlayerCursorObjectComponentName( player, /*out*/ selection );
			
			if ( item != NULL && item.GetQuantity2() > 0 && garden_base.CanPlantSeed( selection ) )
			{
				return true;
			}
		}
		
		return false;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		if ( target != NULL && target.IsInherited(GardenBase) )
		{
			GardenBase garden_base = (GardenBase)target;
			string selection;
			GetGame().GetPlayerCursorObjectComponentName( player, /*out*/ selection );
			//SendMessageToClient(player, garden_base.PlantSeed( player, item, selection ));
			garden_base.TakeEntityAsAttachment(item);
		}
	}
};