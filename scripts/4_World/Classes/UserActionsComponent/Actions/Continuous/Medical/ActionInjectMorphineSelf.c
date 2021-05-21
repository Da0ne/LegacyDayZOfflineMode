class ActionInjectMorphineSelf: ActionInjectSelf
{
	void ActionInjectMorphineSelf()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_MORPHINE;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
	}
	
	int GetType()
	{
		return AT_INJECT_MORPHINE_S;
	}
		
	void ApplyModifiers( PlayerBase player, Object target, ItemBase item )
	{
		//player.m_ModifiersManager.ActivateModifier(MDF_CHEMICALPOISON);
	}
};