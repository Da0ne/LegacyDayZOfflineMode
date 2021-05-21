class ActionBiteVitaminBottle: ActionConsumeSingle
{
	void ActionBiteVitaminBottle()
	{
		//m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EATPILLS;
	}

	int GetType()
	{
		return AT_BITE_VITAMIN_BOTTLE;
	}

	void ApplyModifiers( PlayerBase player, Object target, ItemBase item )
	{
		//player.m_ModifiersManager.ActivateModifier(MDF_CHEMICALPOISON);
	}
};