class EN5C_BarbedWire extends ItemBase
{
	// Sounds lists
	const static int		SOUNDS_SPARK_COUNT							= 4;
	const static int		SOUNDS_CUT_COUNT							= 3;
	const static float		RANDOM_SPARK_INTERVAL						= 5; // TO DO! Currently not used.
	
	const static string 	m_SoundsSpark[SOUNDS_SPARK_COUNT] 			= {"electricFenceSpark1", "electricFenceSpark2", "electricFenceSpark3", "electricFenceSpark4"};
	const static string 	m_SoundsCut[SOUNDS_CUT_COUNT] 				= {"barbedFenceCut1", "barbedFenceCut2", "barbedFenceCut3"};
	const static string 	m_SoundBuzzLoop 							= "electricFenceBuzzLoop1";
	
	Object m_ParticleSpark;
	SoundOnVehicle m_BuzzSoundLoop;
	autoptr Timer m_SparkEvent;
	BarbedWireTrigger m_DamageTrigger;
	
	void BarbedWire()
	{
		m_ParticleSpark = NULL;
		m_SparkEvent = new Timer( CALL_CATEGORY_GAMEPLAY );
	}
	
	void ~BarbedWire()
	{
		if (m_ParticleSpark  &&  GetGame()) // GetGame() is null when the game is being shut down
		{
			GetGame().ObjectDelete(m_ParticleSpark);
		}
	}
	
	void OnWorkStart()
	{
		SoundBuzzLoopStart();
	}

	// Looped sound effect control
	void OnWorkStop()
	{
		SoundBuzzLoopStop();
	}

	void OnWork( float consumed_energy )
	{
		Spark();
	}

	void OnIsPlugged(EntityAI source_device)
	{
		SoundCut();
	}

	void OnIsUnplugged( EntityAI last_energy_source )
	{
		SoundCut();
	}
	
	void OnInventoryEnter(Man player)
	{
		super.OnInventoryEnter(player);
		HideSelection("placing");
		ShowSelection("zbytek");
		DestroyDamageTriger();
		GetCompEM().UnplugThis();
	}
	
	void OnPlacementFinished( PlayerBase player )
	{
		// TO DO: Unfold the barbe wire and make a barrier out of it!
		
		HideSelection("zbytek");
		ShowSelection("placing");
		
		if (!GetOwner())
		{
			CreateDamageTriger();
		}
	}
	
	BarbedWireTrigger CreateDamageTriger()
	{
		vector global_pos = GetPosition();
		m_DamageTrigger = GetGame().CreateObject( "BarbedWireTrigger", global_pos, false );
		vector mins = "-0.5 0 -0.5";
		vector maxs = "0.5 0.5 0.5";
		vector ori = GetOrientation();
		m_DamageTrigger.SetOrientation( ori );
		m_DamageTrigger.SetExtents(mins, maxs);
		m_DamageTrigger.SetParentObject( this );
	}
	
	void DestroyDamageTriger()
	{
		if ( m_DamageTrigger  &&  GetGame() ) // It's necesarry to check if the game exists. Otherwise a crash occurs while quitting.
		{
			GetGame().ObjectDelete( m_DamageTrigger );
			m_DamageTrigger = NULL;
		}
	}
	
	// Controls spawn of random sparks
	void TimerRandomSpark() // TO DO: Come up with randomized functionality.
	{
		if ( GetCompEM().IsWorking() )
		{
			Spark();
			//m_SparkEvent.Run(  frandom(0, RANDOM_SPARK_INTERVAL / (plugged_wires_c+1) )  , this, "TimerRandomSpark", NULL, false);
		}
	}

	// Spawns spark particle effect and plays sound.
	void Spark()
	{
		if (m_ParticleSpark  &&  GetGame()) // GetGame() is null when the game is being shut down
		{
			GetGame().ObjectDelete(m_ParticleSpark);
			m_ParticleSpark = NULL;
		}
		
		vector pos = GetPosition();
		m_ParticleSpark = GetGame().CreateObject("#particlesourceEnf", pos, true);
		
		if (m_ParticleSpark)
		{
			vobject vobj = GetObject("graphics/particles/electro_shortc2.ptc");
			m_ParticleSpark.SetObject(vobj, "");
			ReleaseObject(vobj);
		}
		
		SoundSpark();
	}
	
	
	// SOUNDS
	void SoundCut()
	{
		int random_index = Math.RandomInt(0, SOUNDS_CUT_COUNT);
		string sound_type = m_SoundsCut[random_index];
		GetGame().CreateSoundOnObject(this, sound_type, 50, false);
	}

	// Plays sound
	void SoundSpark()
	{
		int random_index = Math.RandomInt(0, SOUNDS_SPARK_COUNT);
		string sound_type = m_SoundsSpark[random_index];
		GetGame().CreateSoundOnObject(this, sound_type, 50, false);
	}

	// Plays sound
	void SoundBuzzLoopStart()
	{
		if (!m_BuzzSoundLoop)
		{
			m_BuzzSoundLoop = GetGame().CreateSoundOnObject(this, m_SoundBuzzLoop, 50, true);
		}
	}

	// Stops sound
	void SoundBuzzLoopStop()
	{
		if (m_BuzzSoundLoop)
		{
			GetGame().ObjectDelete(m_BuzzSoundLoop);
			m_BuzzSoundLoop = NULL;
		}
	}
}