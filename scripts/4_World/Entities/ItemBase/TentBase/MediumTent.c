class EN5C_MediumTent extends TentBase
{
	void EN5C_MediumTent()
	{
		m_ToggleSelections.Insert( new ToggleSelections("entrance_open", "entrance_closed"), 0 );
		
		m_ShowSelectionsWhenPitched.Insert( "entrance_open" );
		
		m_HideSelectionsWhenPacked.Insert( "entrance_open" );
		m_HideSelectionsWhenPacked.Insert( "entrance_closed" );
		
		Init();
	}
};
