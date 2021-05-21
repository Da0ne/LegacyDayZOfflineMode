class ActionDrinkDisinfectant2: ActionDrink
{
	int GetType()
	{
		return AT_DRINK_DISINFECTANT2;
	}
		
	void ApplyModifiers( PlayerBase player, Object target, ItemBase item )
	{
		player.m_ModifiersManager.ActivateModifier(MDF_POISONING);
	}
};