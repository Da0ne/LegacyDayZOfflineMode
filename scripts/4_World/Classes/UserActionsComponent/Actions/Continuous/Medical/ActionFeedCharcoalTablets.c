class ActionFeedCharcoalTablets: ActionForceConsume
{
	int GetType()
	{
		return AT_FEED_CHARCOAL_TABLETS;
	}

	void ApplyModifiers( PlayerBase player, Object target, ItemBase item )
	{
		PlayerBase ntarget = target;
		ntarget.m_ModifiersManager.DeactivateModifier(MDF_POISONING);
	}
};