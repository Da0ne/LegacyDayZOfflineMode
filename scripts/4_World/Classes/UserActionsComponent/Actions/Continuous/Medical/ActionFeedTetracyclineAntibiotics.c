class ActionFeedTetracyclineAntibiotics: ActionForceConsume
{
	int GetType()
	{
		return AT_FEED_TETRACYCLINE_ANTIBIOTICS;
	}

	void ApplyModifiers( PlayerBase player, Object target, ItemBase item )
	{
		PlayerBase ntarget = target;
		//ntarget.m_ModifiersManager.ActivateModifier(MDF_CHEMICALPOISON);
	}
};