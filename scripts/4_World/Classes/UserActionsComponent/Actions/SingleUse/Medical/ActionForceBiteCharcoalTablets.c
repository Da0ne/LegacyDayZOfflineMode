class ActionForceBiteCharcoalTablets: ActionForceConsumeSingle
{
	int GetType()
	{
		return AT_FORCE_BITE_CHARCOAL_TABLETS;
	}

	void ApplyModifiers( PlayerBase player, Object target, ItemBase item )
	{
		PlayerBase ntarget = target;
		ntarget.m_ModifiersManager.DeactivateModifier(MDF_POISONING);
	}
};