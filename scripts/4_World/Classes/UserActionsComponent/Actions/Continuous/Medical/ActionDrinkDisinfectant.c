class ActionDrinkDisinfectant: ActionDrink
{
	int GetType()
	{
		return AT_DRINK_DISINFECTANT;
	}
		
	void ApplyModifiers( PlayerBase player, Object target, ItemBase item )
	{
		player.m_ModifiersManager.ActivateModifier(MDF_POISONING);
	}
};