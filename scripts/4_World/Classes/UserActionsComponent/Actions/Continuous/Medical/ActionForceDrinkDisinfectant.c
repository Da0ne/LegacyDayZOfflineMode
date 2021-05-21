class ActionForceDrinkDisinfectant: ActionForceDrink
{
	int GetType()
	{
		return AT_FORCE_DRINK_DISINFECTANT;
	}

	void ApplyModifiers( PlayerBase player, Object target, ItemBase item )
	{
		PlayerBase ntarget = target;
		ntarget.m_ModifiersManager.ActivateModifier(MDF_POISONING);
	}
};