class EN5C_Torch : ItemBase
{
	private SoundOnVehicle	m_LoopSoundEntity;
	Object 					m_FireParticle;
	
	void ~EN5C_Torch()
	{
		if ( m_LoopSoundEntity != NULL && GetGame() != NULL )
		{
			GetGame().ObjectDelete( m_LoopSoundEntity );
		}
	}
	
	bool ConditionCargoIn(EntityAI parent)
	{
		return CanBeTakenAsCargo();
	}
	
	bool ConditionCargoOut(EntityAI parent)
	{
		return CanBeTakenAsCargo();
	}
	
	bool CanBeTakenAsCargo()
	{
		// Don't let players burn their pockets!
		return !GetCompEM().IsWorking();
	}
	
	void OnWorkStart()
	{
		SetPilotLight(true);
		m_LoopSoundEntity = GetGame().CreateSoundOnObject(this, "torchLoop", 50, true);
		
		vector pos = GetPosition();
		m_FireParticle = GetGame().CreateObject("#particlesourceEnf", pos, true); // TEST
		
		if (m_FireParticle)
		{
			vobject vobj = GetObject("graphics/particles/fire_small_torch_yellow_01.ptc");
			m_FireParticle.SetObject(vobj, "");
			ReleaseObject(vobj);
			
			//this.AddChild(m_FireParticle, 1);
		}
	}
	
	void OnWorkStop()
	{
		SetPilotLight(false);
		if ( m_LoopSoundEntity != NULL )
		{
			GetGame().ObjectDelete( m_LoopSoundEntity );
		}
		
		GetGame().ObjectDelete(m_FireParticle); // Deletes particle
	}
};




 








