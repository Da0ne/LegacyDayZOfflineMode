class ActionShaveCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 2;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionShave: ActionContinuousBase
{
	void ActionShave()
	{
		m_CallbackClass = ActionShaveCB;
		m_MessageStartFail = "I have no beard to be shaved.";
		m_MessageStart = "I have started shaving myself.";
		m_MessageSuccess = "I have shaved myself.";
		m_MessageFail = "I have failed to shave myself.";
		m_MessageCancel = "I stopped shaving myself.";
		//m_Animation = "ignite";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}

	int GetType()
	{
		return AT_SHAVE;
	}
		
	string GetText()
	{
		return "Shave myself";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		if ( item.GetDamage() < 1 )
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
		player.ShavePlayer();
	}
};