class ActionEatVitaminBottle: ActionConsume
{
	void ActionEatVitaminBottle()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EATPILLS;
	}

	int GetType()
	{
		return AT_EAT_VITAMIN_BOTTLE;
	}

	void ApplyModifiers( PlayerBase player, Object target, ItemBase item )
	{
		//player.m_ModifiersManager.ActivateModifier(MDF_CHEMICALPOISON);
}
};