class ActionBandageSelfCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 2;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionBandageSelf: ActionContinuousBase
{
	void ActionBandageSelf()
	{
		m_CallbackClass = ActionBandageSelfCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_BANDAGE;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
		
		m_MessageStartFail = "There's nothing to bandage.";
		m_MessageStart = "I have started bandaging myself";
		m_MessageSuccess = "I have bandaged myself.";
		m_MessageFail = "I have moved and bandaging was canceled.";
		m_MessageCancel = "I stopped bandaging.";
		m_Sounds.Insert("bandage_0");
		m_Sounds.Insert("bandage_1");
		m_Sounds.Insert("bandage_2");
	}
	
	void CreateConditionComponents() 
	{		
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}

	int GetType()
	{
		return AT_BANDAGE_S;
	}
		
	string GetText()
	{
		return "bandage";
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
		

		player.m_ModifiersManager.ActivateModifier(MDF_BLEEDING);
		item.AddQuantity2(-1,true);
		int increase_value = 1;
		if ( item.IsKindOf("EN5C_Bandage") )
		{
			increase_value = 2;
		}
		
		player.IncreaseSkill(PluginExperience.EXP_MEDIC_BANDAGING, increase_value);
	}
};