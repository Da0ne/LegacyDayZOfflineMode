class ActionBandageTargetCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 2;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionBandageTarget: ActionContinuousBase
{
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	
	void ActionBandageTarget()
	{
		m_CallbackClass = ActionBandageTargetCB;
		m_MessageStartFail = "There's no bandage left.";
		m_MessageStart = "Player started bandaging you.";
		m_MessageSuccess = "Player finished bandaging you.";
		m_MessageFail = "Player moved and bandaging was canceled.";
		m_MessageCancel = "You stopped bandaging.";
		//m_Animation = "BANDAGET";
		m_Sounds.Insert("bandage_0");
		m_Sounds.Insert("bandage_1");
		m_Sounds.Insert("bandage_2");
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(MAXIMAL_TARGET_DISTANCE);
	}

	int GetType()
	{
		return AT_BANDAGE_T;
	}
		
	string GetText()
	{
		return "bandage target";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{	
		if ( item.GetQuantity2() > 0 ) 
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		PlayerBase ntarget = target;
		ntarget.m_ModifiersManager.DeactivateModifier(MDF_BLEEDING);
		item.AddQuantity2(-1,true);
		
		int increase_value = 1;
		if ( item.IsKindOf("EN5C_Bandage") )
		{
			increase_value = 2;
		}
		
		player.IncreaseSkill(PluginExperience.EXP_MEDIC_BANDAGING, increase_value);
	}
};
