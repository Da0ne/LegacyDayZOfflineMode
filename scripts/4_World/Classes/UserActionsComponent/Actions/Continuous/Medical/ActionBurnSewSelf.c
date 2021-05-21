class ActionBurnSewSelfCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 2;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionBurnSewSelf: ActionContinuousBase
{
	void ActionBurnSewSelf()
	{
		m_CallbackClass = ActionBurnSewSelfCB;
		m_MessageStartFail = "It's not possible.";
		m_MessageStart = "I have started closing my wounds";
		m_MessageSuccess = "I have closed my wounds.";
		m_MessageFail = "I have moved and healing was canceled.";
		m_MessageCancel = "I stopped closing my wounds.";
		//m_Animation = "bandage";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}

	int GetType()
	{
		return AT_BURN_SEW_S;
	}
		
	string GetText()
	{
		return "close wounds";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{	
		if ( item.GetTemperature2() > 80 ) 
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
		item.AddDamage(0.05);
		player.m_ModifiersManager.DeactivateModifier(MDF_BLEEDING);
		player.m_PlayerStats.m_Shock.Add(1000);
	}
};