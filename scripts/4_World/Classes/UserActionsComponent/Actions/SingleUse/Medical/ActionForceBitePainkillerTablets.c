class ActionForceBitePainkillerTablets: ActionForceConsumeSingle
{
	int GetType()
	{
		return AT_FORCE_BITE_PAINKILLER_TABLETS;
	}

	void ApplyModifiers( PlayerBase player, Object target, ItemBase item )
	{
		PlayerBase ntarget = target;
		//target.m_ModifiersManager.ActivateModifier(MDF_CHEMICALPOISON);
	}
};