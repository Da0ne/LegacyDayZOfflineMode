class ActionInjectMorphineTarget: ActionInjectTarget
{
	void ActionInjectMorphineTarget()
	{
		m_MessageStart = "Player started injecting you morphine.";
		m_MessageSuccess = "Player finished injecting you morphine.";
	//	m_Animation = "INJECTEPIPENT";
	}

	int GetType()
	{
		return AT_INJECT_MORPHINE_T;
	}
		
	void ApplyModifiers( PlayerBase player, Object target, ItemBase item )
	{
		PlayerBase ntarget = target;
		//target.m_ModifiersManager.ActivateModifier(MDF_CHEMICALPOISON);
	}
};