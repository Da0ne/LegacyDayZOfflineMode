class CCTSelf : CCTBase
{
	void CCTSelf()
	{
		m_ActionOnTarget = false;
	}
	
	bool Can( PlayerBase player, Object target )
	{
		if ( player && player.IsAlive() ) return true;
		return false;
	}
};