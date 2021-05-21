class EN5C_Watchtower extends BaseBuildingBase
{	
	void EN5C_Watchtower()
	{
		// Electric watchtower functionality is WIP!
		
		m_DmgTrgLocalPos				= { "0 0.50 2.6" , "-2.6 0.50 0" , "2.6 0.50 0" }; // {"<right> <up> <forward>", "<right> <up> <forward>", "<right> <up> <forward>"} model coordinates
		m_DmgTrgLocalDir 				= {0, -90, 90};
		
		SetAnimationPhase ( "Base_down_right", 0 );
	}
	
	void ~EN5C_Watchtower()
	{
		
	}
	
	// --- ATTACHMENTS
	void OnItemAttached ( EntityAI item ) 
	{
		
	}
	
	void OnItemDetached ( EntityAI item ) 
	{
		
	}
}