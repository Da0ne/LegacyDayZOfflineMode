class ActionWaterGardenSlotCB : ActionContinuousBaseCB
{
	private const float QUANTITY_USED_PER_SEC = 50;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousQuantity(QUANTITY_USED_PER_SEC);
	}
};

class ActionWaterGardenSlot: ActionContinuousBase
{
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	
	void ActionWaterGardenSlot()
	{
		m_CallbackClass = ActionWaterGardenSlotCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EMPTYBOTTLE;
		m_MessageSuccess = "I've watered slot.";
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
		return AT_WATER_GARDEN_SLOT;
	}
	
	string GetText()
	{
		return "Water slot";
	}
	
	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		if ( target.IsInherited(GardenBase) )
		{
			GardenBase garden_base = (GardenBase)target;
			
			string selection;
			GetGame().GetPlayerCursorObjectComponentName( player, selection );
			
			Slot slot = garden_base.GetSlotBySelection( selection );
		
			if ( slot  &&  !slot.GetPlant()  &&  (slot.IsDigged() || slot.IsPlanted())  &&  slot.NeedsWater() ) // !slot.GetPlant() is here because we have separate user action for watering plants
			{
				return true;
			}
			else
			{
				return false;
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
			
			GetGame().GetPlayerCursorObjectComponentName( player, selection );
			Param1<float> nacdata = acdata;
			Slot slot = garden_base.GetSlotBySelection( selection );
			slot.GiveWater ( item, nacdata.param1 );
		}
	}
};