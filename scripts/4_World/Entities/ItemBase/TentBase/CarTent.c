class EN5C_CarTent extends TentBase
{
	void EN5C_CarTent()
	{
		m_ToggleSelections.Insert( new ToggleSelections("door_open", "door_closed"), 0 );
		
		m_ShowSelectionsWhenPitched.Insert( "door_open" );
		
		m_HideSelectionsWhenPacked.Insert( "door_open" );
		m_HideSelectionsWhenPacked.Insert( "door_closed" );
		
		Init();
	}
};
