class ActionDigSlotCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 4;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionDigSlot: ActionContinuousBase
{
	
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	private int m_FreeSlotIndex = -1;
		
	void ActionDigSlot()
	{
		m_CallbackClass = ActionDigSlotCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DIGSHOVEL;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
		m_MessageSuccess = "I have digged slot.";
		m_MessageStartFail = "";
		m_MessageStart = "";
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
		return AT_DIG_SLOT;
	}
		
	string GetText()
	{
		return "Dig slot";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		if ( !player.IsPlacingObject() && target != NULL && target.IsInherited(GardenBase) )
		{
			GardenBase garden = (GardenBase)target;

			string item_type = item.GetType();
			//GetGame().ConfigGetText( "cfgVehicles " + item_type + " Horticulture ToolAnim", m_Animation );
			
			PluginExperience module_exp = GetPlugin(PluginExperience);
			float soil_preparation_efficiency = module_exp.GetExpParamNumber(player, PluginExperience.EXP_FARMER_SOIL_PREPARATION, "efficiency");
			//m_TimeToCompleteAction = GetGame().ConfigGetFloat( "cfgVehicles " + item_type + " Horticulture DiggingTimeToComplete") / soil_preparation_efficiency;
			
			m_FreeSlotIndex = garden.GetFreeSlotIndex( player );
			
			if ( m_FreeSlotIndex > -1 )
			{
				return true;
			}
		}
		
		return false;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		if ( target != NULL && target.IsInherited(GardenBase) && item != NULL )
		{
			GardenBase garden = (GardenBase)target;
			SendMessageToClient(player, garden.DigSlot( player, item, m_FreeSlotIndex ) );
			player.IncreaseSkill( PluginExperience.EXP_FARMER_SOIL_PREPARATION, 1 );
		}
	}
};
