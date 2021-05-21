class ActionBiteCharcoalTablets: ActionConsumeSingle
{
	void ActionBiteCharcoalTablets()
	{
		//m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EATTABLETS;
	}

	int GetType()
	{
		return AT_BITE_CHARCOAL_TABLETS;
	}

	void ApplyModifiers( PlayerBase player, Object target, ItemBase item )
	{
		player.m_ModifiersManager.DeactivateModifier(MDF_POISONING);
	}
};