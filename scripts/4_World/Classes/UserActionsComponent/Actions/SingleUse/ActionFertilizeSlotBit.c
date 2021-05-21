class PluginHorticulture;
class ActionFertilizeSlotBit: ActionSingleUseBase
{
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	
	void ActionFertilizeSlotBit()
	{
		m_MessageSuccess = "";
		m_MessageStartFail = "";
		m_MessageStart = "";
		m_MessageSuccess = "";
		m_MessageFail = "";
		m_MessageCancel = "";
	}
	
	void CreateConditionComponents() 
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined(MAXIMAL_TARGET_DISTANCE);
	}

	int GetType()
	{
		return AT_FERTILIZE_SLOT_BIT;
	}

	string GetText()
	{
		return "Fertilize slot a bit";
	}


	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		if ( target != NULL && target.IsInherited(GardenBase) )
		{
			GardenBase garden_base = (GardenBase)target;
			
			string selection;
			GetGame().GetPlayerCursorObjectComponentName( player, /*out*/ selection );
			
			if ( garden_base.IsCorrectFertilizer(item, selection) && garden_base.NeedsFertilization(selection) )
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
		if ( target != NULL && target.IsInherited(GardenBase) )
		{
			GardenBase garden_base = (GardenBase)target;
			string selection;
			GetGame().GetPlayerCursorObjectComponentName( player, /*out*/ selection );
			SendMessageToClient(player, garden_base.FertilizeBit( player, item, selection ) );
			if ( garden_base.NeedsFertilization(selection) == false )
			{
				player.IncreaseSkill( PluginExperience.EXP_FARMER_FERTILIZATION, 1 );
			}
		}
	}
};