class ActionBiteTetracyclineAntibiotics: ActionConsumeSingle
{
	void ActionBiteTetracyclineAntibiotics()
	{
		//m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EATTABLETS;
	}

	int GetType()
	{
		return AT_BITE_TETRACYCLINE_ANTIBIOTICS;
	}

	void ApplyModifiers( PlayerBase player, Object target, ItemBase item )
	{
		//player.m_ModifiersManager.ActivateModifier(MDF_CHEMICALPOISON);
	}
};