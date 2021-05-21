class ActionEatMeat: ActionEat
{
	int GetType()
	{
		return AT_EAT_MEAT;
	}
			
	void ApplyModifiers( PlayerBase player, Object target, ItemBase item )
	{
		if ( item.IsInherited( EN5C_Edible_Base ) )
		{
			EN5C_Edible_Base food_item = ( EN5C_Edible_Base ) item;
			
			if ( food_item.IsMeat() && FoodManager.IsFoodRaw( food_item ) )
			{
				PluginLifespan module_lifespan = GetPlugin( PluginLifespan );
				module_lifespan.SetBloodyHands( player );
			}
		}
	}
}
