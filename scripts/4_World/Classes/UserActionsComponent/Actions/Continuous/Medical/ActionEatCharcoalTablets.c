class ActionEatCharcoalTablets: ActionConsume
{
	void ActionEatCharcoalTablets()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EATTABLETS;
	}
	
	int GetType()
	{
		return AT_EAT_CHARCOAL_TABLETS;
	}

	void ApplyModifiers( PlayerBase player, Object target, ItemBase item )
	{
		player.m_ModifiersManager.DeactivateModifier(MDF_POISONING);
	}
};