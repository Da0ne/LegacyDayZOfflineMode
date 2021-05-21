class TrapTrigger extends Trigger
{
	TrapBase m_ParentObj;

	void OnEnter( Object obj )
	{
		if ( g_Game.IsServer() )
		{
			if ( m_ParentObj && m_ParentObj.IsActive() == true ) 
			{
				m_ParentObj.AddDamageToVictim(obj);
			}
		}
	}
	
	void SetParentObject( TrapBase obj )
	{
		if ( g_Game.IsServer() )
		{
			m_ParentObj = obj;
		}
	}
}
