class EN5C_Refridgerator extends ItemBase
{	
	SoundOnVehicle 	m_SoundLoopEntity;
	
	void OnInitEnergy()
	{
		m_SoundLoopEntity = NULL;
	}
	
	//--- POWER EVENTS
	void OnWorkStart()
	{
		// TO DO: Slow down or stop the aging of food inside of the fridge when such functionality is created.
		
		m_SoundLoopEntity = GetGame().CreateSoundOnObject(this, "powerGeneratorLoop", 50, true); // powerGeneratorLoop is a placeholder sound
	}
	
	void OnWork( float consumed_energy )
	{
		
	}
	
	void OnWorkStop()
	{
		// TO DO: Allow food inside the fridge to start aging again.
		
		GetGame().ObjectDelete(m_SoundLoopEntity);
		m_SoundLoopEntity = NULL;
	}
	
	//--- ACTION EVENTS
	void OnSwitchOn()
	{
		
	}

	void OnSwitchOff()
	{
		
	}
}