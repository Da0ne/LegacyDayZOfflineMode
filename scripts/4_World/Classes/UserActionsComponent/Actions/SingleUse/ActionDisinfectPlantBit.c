class PluginHorticulture;
class ActionDisinfectPlantBitCB : ActionSingleUseBaseCB
{
	private const float QUANTITY_USED_PER_SEC = 0.06;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CASingleUseQuantity(QUANTITY_USED_PER_SEC);
	}
};


class ActionDisinfectPlantBit: ActionSingleUseBase
{
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	
	void ActionDisinfectPlantBit()
	{
		m_CallbackClass = ActionDisinfectPlantBitCB;
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
		return AT_DISINFECT_PLANT_BIT;
	}

	string GetText()
	{
		return "Disinfect plant a bit";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		if ( target != NULL && target.IsInherited(EN5C_PlantBase) && item.GetDamage() < 1 )
		{
			EN5C_PlantBase plant = (EN5C_PlantBase)target;
			
			if ( plant.IsGrowing() && plant.NeedsSpraying() )
			{	
				if ( item.GetQuantity2() > 0 )
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
			
			EN5C_PlantBase plant = (EN5C_PlantBase)target;
			Param1<float> nacdata = acdata;
			SendMessageToClient(player, plant.StopInfestation( player, item, nacdata.param1 ));
		}
	}
};