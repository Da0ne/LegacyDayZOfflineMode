class CCTNone : CCTBase
{
	void CCTNone()
	{
		m_ActionOnTarget = false;
	}
	
	bool Can( PlayerBase player, Object target )
	{
		if( player ) return true;
		return false;
	}
};