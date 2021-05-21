class ActionInjectEpinephrineSelf: ActionInjectSelf
{
	int GetType()
	{
		return AT_INJECT_EPINEPHRINE_S;
	}
		
	void ApplyModifiers( PlayerBase player, Object target, ItemBase item )
	{
		player.m_ModifiersManager.DeactivateModifier(MDF_HEART_ATTACK);
		player.m_PlayerStats.m_Shock.Set(0);
	}
};