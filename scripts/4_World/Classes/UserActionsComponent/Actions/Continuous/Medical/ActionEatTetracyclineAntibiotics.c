class ActionEatTetracyclineAntibiotics: ActionConsume
{
	void ActionEatTetracyclineAntibiotics()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EATPILLS;
	}

	int GetType()
	{
		return AT_EAT_TETRACYCLINE_ANTIBIOTICS;
	}

	void ApplyModifiers( PlayerBase player, Object target, ItemBase item )
	{
		//player.m_ModifiersManager.ActivateModifier(MDF_CHEMICALPOISON);
	}
};