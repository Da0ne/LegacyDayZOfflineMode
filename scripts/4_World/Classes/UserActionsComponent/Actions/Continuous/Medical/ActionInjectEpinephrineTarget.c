class ActionInjectEpinephrineTarget: ActionInjectTarget
{
	void ActionInjectEpinephrineTarget()
	{
		//m_Animation = "INJECTEPINENT";
		m_MessageStart = "Player started injecting you epinephrine.";
		m_MessageSuccess = "Player finished injecting you epinephrine.";
	}

	int GetType()
	{
		return AT_INJECT_EPINEPHRINE_T;
	}
		
	void ApplyModifiers( PlayerBase player, Object target, ItemBase item )
	{
		PlayerBase ntarget = target;
		ntarget.m_ModifiersManager.DeactivateModifier(MDF_HEART_ATTACK);
		ntarget.m_PlayerStats.m_Shock.Set(0);
	}
};