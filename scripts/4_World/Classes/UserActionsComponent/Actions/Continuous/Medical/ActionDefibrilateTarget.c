class ActionDefibrilateTargetCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 15;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionDefibrilateTarget: ActionContinuousBase
{
	
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	float m_EnergyUsage;
	
	void ActionDefibrilateTarget()
	{
		m_CallbackClass = ActionDefibrilateTargetCB;
		m_MessageStartFail = "It's out of juice.";
		m_MessageStart = "Player started defibrilating you.";
		m_MessageSuccess = "Player finished defibrilating you.";
		m_MessageFail = "Player moved and defibrilating was canceled.";
		m_MessageCancel = "You stopped defibrilating.";
		m_EnergyUsage = 5000; //wats
		//m_Animation = "defibrilate";
	}
	
	void CreateConditionComponents() 
	{	
		
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(MAXIMAL_TARGET_DISTANCE);
	}

	int GetType()
	{
		return AT_DEFIBRILATE_T;
	}
	
	string GetText()
	{
		return "defibrilate target";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{	
		if ( item.HasEnergyManager()  &&  item.GetCompEM().IsSwitchedOn() && item.GetDamage() < 1 ) 
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
		if ( item.HasEnergyManager()  &&  item.GetCompEM().CanWork() )
		{
			item.GetCompEM().ConsumeEnergy ( m_EnergyUsage );
		
			PlayerBase ntarget = target;
			ntarget.m_ModifiersManager.DeactivateModifier ( MDF_HEART_ATTACK );
		}
		else
		{
			player.MessageImportant ( m_MessageStartFail );		
		}
	}
};
