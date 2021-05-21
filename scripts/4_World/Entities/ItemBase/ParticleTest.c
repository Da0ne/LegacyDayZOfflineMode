// Particle test for Maxman

class EN5C_ParticleTest extends ItemBase
{
	Object m_Particle;
	autoptr Timer 	m_StartParticleTimer;
	
	// Constructor
	void EN5C_ParticleTest()
	{
		m_Particle = NULL;
		m_StartParticleTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
		m_StartParticleTimer.Run(0.01, this, "StartParticle", NULL, true);
	}
	
	void StartParticle()
	{
		vector pos = GetPosition();
		
		if (!m_Particle)
		{
			m_Particle = GetGame().CreateObject("#particlesourceEnf", pos, true);
			Print(m_Particle);
			if (m_Particle)
			{
				vobject vobj = GetObject("graphics/particles/smoke_small_generator_01.ptc");
				m_Particle.SetObject(vobj, "");
				ReleaseObject(vobj);
			}
		}
		else
		{
			m_Particle.SetPosition(pos);
		}
		
	}

	// Destructor
	void ~EN5C_ParticleTest()
	{
		Print(m_Particle);
		if (m_Particle  &&  GetGame()) // GetGame() is null when the game is being shut down
		{
			GetGame().ObjectDelete(m_Particle);
		}
		
		if (m_StartParticleTimer)
		{
			delete m_StartParticleTimer;
		}
	}
}