class CCTDummy : CCTBase
{
	void CCTDummy()
	{
		m_ActionOnTarget = true;
	}
	
	bool Can( PlayerBase player, Object target )
	{
		if( target ) return true;
		return false;
	}
};