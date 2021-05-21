class ActionFeedPainkillerTablets: ActionForceConsume
{
	int GetType()
	{
		return AT_FEED_PAINKILLER_TABLETS;
	}

	void ApplyModifiers( PlayerBase player, Object target, ItemBase item )
	{
		PlayerBase ntarget = target;
		//ntarget.m_ModifiersManager.ActivateModifier(MDF_CHEMICALPOISON);
	}
};