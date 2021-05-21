class ActionEatPainkillerTablets: ActionConsume
{
	void ActionEatPainkillerTablets()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EATTABLETS;
	}
	
	int GetType()
	{
		return AT_EAT_PAINKILLER_TABLETS;
	}

	void ApplyModifiers( PlayerBase player, Object target, ItemBase item )
	{
		//player.m_ModifiersManager.DeactivateModifier(MDF_POISONING);
	}
};