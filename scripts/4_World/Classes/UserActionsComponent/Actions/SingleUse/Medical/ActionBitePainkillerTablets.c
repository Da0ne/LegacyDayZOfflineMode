class ActionBitePainkillerTablets: ActionConsumeSingle
{
	void ActionBitePainkillerTablets()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EATTABLETS;
	}

	int GetType()
	{
		return AT_BITE_PAINKILLER_TABLETS;
	}

	void ApplyModifiers( PlayerBase player, Object target, ItemBase item )
	{
		//player.m_ModifiersManager.ActivateModifier(MDF_CHEMICALPOISON);
	}
};