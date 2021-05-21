class CCTBase
{
	protected bool m_ActionOnTarget = true; //necessary to set, informs action base checks wheter target is required for action or not
	
	bool Can( PlayerBase player, Object target )
	{
		return true;
	}
	
	bool IsActionOnTarget()
	{
		return m_ActionOnTarget;
	}
};