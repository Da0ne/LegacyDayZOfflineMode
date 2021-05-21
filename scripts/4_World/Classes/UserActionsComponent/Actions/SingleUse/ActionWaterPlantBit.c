//TO DO: Remove this user action!

class PluginHorticulture;
class ActionWaterPlantBitCB : ActionSingleUseBaseCB
{
	private const float QUANTITY_USED_PER_SEC = 50;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CASingleUseQuantity(QUANTITY_USED_PER_SEC);
	}
};

class ActionWaterPlantBit: ActionSingleUseBase
{
	
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	
	void ActionWaterPlantBit()
	{
		m_CallbackClass = ActionWaterPlantBitCB;
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
		return AT_WATER_PLANT_BIT;
	}

	string GetText()
	{
		return "Water plant a bit";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		return false; // This user action is obsolete!
		
		if ( target != NULL && target.IsInherited(EN5C_PlantBase) && item != NULL && item.GetDamage() < 1 )
		{
			EN5C_PlantBase plant = (EN5C_PlantBase)target;
			
			if ( plant.NeedsWater() )
			{
				if ( item.GetQuantity2() > 0 /* //m_QuantityUsedPerSecond*/ )
				{
					return true;
				}
			}
		}
		
		return false;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		if ( target != NULL && target.IsInherited(EN5C_PlantBase) )
		{
			Param1<float> nacdata = acdata;
			item.SetQuantity2( item.GetQuantity2() - nacdata.param1 );
			
			EN5C_PlantBase plant = (EN5C_PlantBase)target;
			//SendMessageToClient(player, plant.GiveWater( item ) ); Slot, not plant!
		}
	}
};
