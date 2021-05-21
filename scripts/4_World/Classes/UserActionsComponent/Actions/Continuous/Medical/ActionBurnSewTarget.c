class ActionBurnSewTargetCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 2;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionBurnSewTarget: ActionContinuousBase
{
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	
	void ActionBurnSewTarget()
	{
		m_CallbackClass = ActionBurnSewTargetCB;
		m_MessageStartFail = "It's not possible.";
		m_MessageStart = "Player started closing your wounds.";
		m_MessageSuccess = "Player finished closing your wounds.";
		m_MessageFail = "Player moved and closing wounds was canceled.";
		m_MessageCancel = "You stopped closing target's wounds.";
	//	m_Animation = "sew";
	}
	
	void CreateConditionComponents() 
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(MAXIMAL_TARGET_DISTANCE);
	}

	int GetType()
	{
		return AT_BURN_SEW_T;
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
		
	string GetText()
	{
		return "close target's wounds";
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		PlayerBase ntarget = target;
		ntarget.m_ModifiersManager.DeactivateModifier(MDF_BLEEDING);
		ntarget.m_PlayerStats.m_Shock.Add(1000);
		item.AddDamage(0.05);
	}
};