class ActionWaterPlantCB : ActionContinuousBaseCB
{
	private const float QUANTITY_USED_PER_SEC = 50;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousWaterPlant(QUANTITY_USED_PER_SEC);
	}
};

class ActionWaterPlant: ActionContinuousBase
{
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	
	void ActionWaterPlant()
	{
		m_CallbackClass = ActionWaterPlantCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EMPTYBOTTLE;
		m_MessageSuccess = "I've watered plant.";
		m_MessageStartFail = "";
		m_MessageStart = "";
		m_MessageSuccess = "";
		m_MessageFail = "";
		m_MessageCancel = "";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionTarget = new CCTNonRuined(MAXIMAL_TARGET_DISTANCE);
		m_ConditionItem = new CCINonRuined;
	}

	int GetType()
	{
		return AT_WATER_PLANT;
	}
		
	string GetText()
	{
		return "Water plant";
	}

	bool ActionCondition ( PlayerBase player, Object target, ItemBase item )
	{
		if ( target != NULL && target.IsInherited(EN5C_PlantBase) && item != NULL && item.GetDamage() < 1 )
		{
			EN5C_PlantBase plant = (EN5C_PlantBase)target;
			
			if ( plant.NeedsWater() && item.GetQuantity2() > 0 )
			{
				return true;
			}
		}
		
		return false;
	}

	void OnComplete ( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		if ( target != NULL && target.IsInherited(EN5C_PlantBase) )
		{
			EN5C_PlantBase plant = (EN5C_PlantBase)target;
			Param1<float> nacdata = acdata;
			Slot slot = plant.GetSlot();
			SendMessageToClient(player, slot.GiveWater( item, nacdata.param1 ) );
		}
	}
};