class ActionActivateTrap: ActionInteractBase
{	
	void ActionActivateTrap()
	{
		m_MessageSuccess = "I've activated trap.";
		m_MessageFail = "I cannot activate trap.";
		//m_Animation = "open";
	}

	int GetType()
	{
		return AT_ACTIVATE_TRAP;
	}

	string GetText()
	{
		return "Activate the trap";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		if ( target != NULL && target.IsInherited(TrapBase) ) 
		{
			TrapBase trap = (TrapBase)target;
			
			if ( trap.IsActivable() )
			{
				return true;
			}
		}
		
		return false;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		if ( target != NULL && target.IsInherited(TrapBase) ) 
		{
			TrapBase trap = (TrapBase)target;
			trap.StartActivate( player );
		}
	}
};