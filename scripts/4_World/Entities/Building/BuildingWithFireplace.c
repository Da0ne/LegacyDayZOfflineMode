class BuildingWithFireplace extends Building
{
	//fireplace indoor
	array<FireplacePoint> m_FireplacePoints;
	
	void BuildingWithFireplace()
	{
		m_FireplacePoints = new array<FireplacePoint>;
		m_FireplacePoints.Insert( new FireplacePoint( "fireplace_action1", "fireplace_point1", "fireplace_smoke1", NULL ) );
		m_FireplacePoints.Insert( new FireplacePoint( "fireplace_action1", "fireplace_point2", "fireplace_smoke2", NULL ) );
	}
	
	//fireplace indoor
	bool IsPointAvailable( string action_selection )
	{
		for ( int i = 0; i < m_FireplacePoints.Count(); i++ )
		{
			if ( m_FireplacePoints.Get( i ).GetActionSelection() == action_selection )
			{
				if ( m_FireplacePoints.Get( i ).GetObject() )	//is not NULL
				{
					return false;
				}
				else
				{
					return true;	
				}
			}
		}
		
		return false;
	}
	
	//get fireplace point by action selection
	FireplacePoint GetFirePointByActionSelection( string action_selection )
	{
		for ( int i = 0; i < m_FireplacePoints.Count(); i++ )
		{
			if ( m_FireplacePoints.Get( i ).GetActionSelection() == action_selection )
			{
				return m_FireplacePoints.Get( i );
			}
		}
		
		return NULL;
	}
	
	//returns true if given selection is define in m_FireplacePoints array
	bool IsFireplaceActionSelection( string selection )
	{
		for ( int i = 0; i < m_FireplacePoints.Count(); i++ )
		{
			if ( m_FireplacePoints.Get( i ).GetActionSelection() == selection )
			{
				return true;
			}
		}
		
		return false;
	}
}
