class CAContinuousMineRock : CAContinuousMineWood
{	
	void CAContinuousMineRock(float time_between_drops)
	{
		m_ContinuousAction = true;
		m_TimeBetweenEachMaterialDrop = time_between_drops;
	}
	
	bool GetMiningData(PlayerBase player, Object target, ItemBase item )
	{
		if ( target && target.IsRock() )
		{
			RockBase ntarget = target;
			m_AmountOfDrops = Math.Max(1,ntarget.GetAmountOfDrops(item));
			m_Material = ntarget.GetMaterial(item);
			m_AmountOfMaterialPerDrop = Math.Max(1,ntarget.GetAmountOfMaterialPerDrop(item));
			m_DamageToMiningItemEachDrop = ntarget.GetDamageToMiningItemEachDrop(item);
			return true;
		}
		return false;
	}
};