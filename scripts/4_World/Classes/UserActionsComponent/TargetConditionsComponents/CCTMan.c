class CCTMan : CCTBase
{
	protected float m_MaximalActionDistance;
	
	void CCTMan ( float maximal_target_distance )
	{
		m_ActionOnTarget = true;
		m_MaximalActionDistance = maximal_target_distance;		
	}
	
	bool Can( PlayerBase player, Object target )
	{	
		if ( player && target && target != player && target.IsMan() )
		{
			PlayerBase man = target;
			float distance = Math.AbsInt(vector.Distance(man.GetPosition(),player.GetPosition()));
			if ( man.IsAlive() && distance <= m_MaximalActionDistance && player.IsFacingTarget(target) )
			{
				return true;
			}	
		}
		return false;
	}
};