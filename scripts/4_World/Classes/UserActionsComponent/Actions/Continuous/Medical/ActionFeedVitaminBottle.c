class ActionFeedVitaminBottle: ActionForceConsume
{
	int GetType()
	{
		return AT_FEED_VITAMIN_BOTTLE;
	}

	void ApplyModifiers( PlayerBase player, Object target, ItemBase item )
	{
		PlayerBase ntarget = target;
		//ntarget.m_ModifiersManager.ActivateModifier(MDF_CHEMICALPOISON);
	}
};