// TO DO: Merge EN5C_Roadflare from DayZ! This is old version without particles!

class EN5C_Roadflare : ItemBase
{
	private SoundOnVehicle 	m_LoopSoundEntity = NULL;
	
	void ~EN5C_Roadflare()
	{
		if ( m_LoopSoundEntity != NULL && GetGame() != NULL )
		{
			GetGame().ObjectDelete( m_LoopSoundEntity );
		}
	}
	
	void OnWorkStart()
	{
		SwitchLight(true);
		m_LoopSoundEntity = GetGame().CreateSoundOnObject(this, "roadflareLoop", 50, true);
	}
	
	void OnWork(float consumed_energy)
	{
		
	}
	
	void OnWorkStop()
	{
		SwitchLight(false);
		
		if ( m_LoopSoundEntity != NULL && GetGame() != NULL )
		{
			GetGame().ObjectDelete( m_LoopSoundEntity );
		}
	}
};
