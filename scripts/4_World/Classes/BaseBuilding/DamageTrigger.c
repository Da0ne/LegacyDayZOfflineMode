class BarbedWireTrigger extends Trigger
{
	ItemBase 			m_ParentBarbedWire;
	const static int	SOUNDS_COLLISION_COUNT						= 4;
	const static int	SOUNDS_SHOCK_COUNT							= 4;
	const static string m_SoundsCollision[SOUNDS_COLLISION_COUNT]	= {"barbedFenceCollision1", "barbedFenceCollision2", "barbedFenceCollision3", "barbedFenceCollision4"};
	const static string m_SoundsShock[SOUNDS_SHOCK_COUNT] 			= {"electricFenceShock1", "electricFenceShock2", "electricFenceShock3", "electricFenceShock4"};

	// When a player / AI touches the Barbed Wire
	void OnEnter( Object obj )
	{
		if ( g_Game.IsServer() )
		{
			if ( m_ParentBarbedWire )
			{
				if ( obj.IsInherited(PlayerBase) )
				{
					// When a player touches the barbed wire
					bool energy_consumed = m_ParentBarbedWire.GetCompEM().ConsumeEnergy(1);
					PlayerBase player = (PlayerBase) obj;
					
					if ( energy_consumed )
					{
						// TO DO: 
						//		-Do electrical damage.
						//		-Cause bleeding?
						//		-Do some damage!
						
						player.MessageImportant( "*ELECTROCUTION*" );
						
						// Play sound
						SoundCollision();
						SoundElectricShock();
					}
					else
					{
						// TO DO: 
						//		-Cause bleeding?
						//		-Do some damage!
						
						player.MessageImportant( "*SCRATCH*" );
						
						// Play sound
						SoundCollision();
					}
				}
				else
				{
					// When an AI Agent touches the barbed wire
					if ( obj.IsInherited(ManBase) )
					{
						ManBase AI_unit = (ManBase) obj;
						AI_unit.SetDamage( 1 );
					}
				}
			}
		}
	}
	
	// Sets parent object for this trigger
	void SetParentObject( ItemBase wire )
	{
		if ( g_Game.IsServer() )
		{
			m_ParentBarbedWire = wire;
		}
	}

	// Plays an electric shock sound
	void SoundElectricShock()
	{
		int random_index = Math.RandomInt(0, SOUNDS_SHOCK_COUNT);
		string sound_type = m_SoundsShock[random_index];
		GetGame().CreateSoundOnObject(this, sound_type, 50, false);
	}
	
	// Plays a collision sound
	void SoundCollision()
	{
		int random_index = Math.RandomInt(0, SOUNDS_COLLISION_COUNT);
		string sound_type = m_SoundsCollision[random_index];
		GetGame().CreateSoundOnObject(this, sound_type, 50, false);
	}
}
