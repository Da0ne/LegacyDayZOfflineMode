class ActionFertilizeSlotCB : ActionContinuousBaseCB
{
	private const float QUANTITY_USED_PER_SEC = 50;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousFertilizeGardenSlot(QUANTITY_USED_PER_SEC);
	}
};

class ActionFertilizeSlot: ActionContinuousBase
{
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	
	void ActionFertilizeSlot()
	{
		m_CallbackClass = ActionFertilizeSlotCB;
		m_MessageSuccess = "I've fertilized slot.";
		m_MessageStartFail = "There's not enough fertilizer.";
		m_MessageStart = "I've started fertilizing.";
		m_MessageFail = "There's not enough fertilizer.";
		m_MessageCancel = "I stoped fertilizing.";
	}
	
	void CreateConditionComponents() 
	{
		m_ConditionTarget = new CCTDummy;
		m_ConditionItem = new CCINonRuined;
	}

	int GetType()
	{
		return AT_FERTILIZE_SLOT;
	}
		
	string GetText()
	{
		return "Fertilize slot";
	}


	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		if ( target.IsInherited(GardenBase) )
		{
			GardenBase garden_base = (GardenBase)target;
			
			string selection;
			GetGame().GetPlayerCursorObjectComponentName( player, /*out*/ selection );
			
			if ( garden_base.IsCorrectFertilizer( item, selection ) && garden_base.NeedsFertilization( selection ) )
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
		if ( target  &&  target.IsInherited(GardenBase) )
		{
			GardenBase garden_base = (GardenBase)target;
			string selection;
			GetGame().GetPlayerCursorObjectComponentName( player, selection );
			Param1<float> nacdata = acdata;
			SendMessageToClient(player, garden_base.Fertilize( player, item, nacdata.param1, selection ));
			
			if ( garden_base.NeedsFertilization(selection) == false )
			{
				player.IncreaseSkill( PluginExperience.EXP_FARMER_FERTILIZATION, 1 );
			}
		}
	}
};