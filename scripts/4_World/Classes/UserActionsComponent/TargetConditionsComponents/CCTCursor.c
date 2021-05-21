class CCTCursor : CCTBase
{
	protected float m_MaximalActionDistance;
	
	void CCTCursor ( float maximal_target_distance )
	{
		m_ActionOnTarget = true;
		m_MaximalActionDistance = maximal_target_distance;
	}
	
	bool Can( PlayerBase player, Object target )
	{	
		if ( player && target && target.GetDamage() < 1 )
		{
			float distance = Math.AbsInt(vector.Distance(GetGame().GetCursorPos(),player.GetPosition()));
			if ( distance <= m_MaximalActionDistance )
			{
				return true;
			}	
		}
		return false;
	}
};