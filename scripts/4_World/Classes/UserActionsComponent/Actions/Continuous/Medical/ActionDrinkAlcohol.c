class ActionDrinkAlcohol: ActionDrink
{
	int GetType()
	{
		return AT_DRINK_ALCOHOL;
	}
		
	void ApplyModifiers( PlayerBase player, Object target, ItemBase item )
	{
		player.m_ModifiersManager.ActivateModifier(MDF_POISONING);
	}
};