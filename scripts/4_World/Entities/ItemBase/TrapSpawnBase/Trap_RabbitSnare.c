class Trap_RabbitSnare extends TrapSpawnBase
{
	void Trap_RabbitSnare()
	{
		m_InitWaitTime = 120 + Math.RandomInt( 0, 300 );
		m_UpdateWaitTime = 15;
		m_BaitNeeded = false;
		m_IsFoldable = true;
		m_IsUsable = true;
		m_MinimalDistanceFromPlayersToCatch = 100;

		m_AnimationPhaseSet = "inventory";
		m_AnimationPhaseTriggered = "placing";
		m_AnimationPhaseUsed = "rabbit_snare_used";

		m_WaterSurfaceForSetup = false;

		m_SurfaceForSetup = new map<string, float>;
		m_SurfaceForSetup.Set( "CRBoulders", 0.10 );
		m_SurfaceForSetup.Set( "CRHlina", 0.15 ); 
		m_SurfaceForSetup.Set( "CRForest1", 0.50 ); 
		m_SurfaceForSetup.Set( "CRForest2", 0.50 ); 
		m_SurfaceForSetup.Set( "CRGrass1", 0.35 ); 
		m_SurfaceForSetup.Set( "CRGrass2", 0.35 ); 
		m_SurfaceForSetup.Set( "CRGrit1", 0.10 ); 
		m_SurfaceForSetup.Set( "cesta", 0.25 ); 
		
		m_CatchesPond = new multiMap<string, float>;
		
		m_CatchesSea = new multiMap<string, float>;
		
		m_CatchesGroundAnimal = new multiMap<string, float>;
		m_CatchesGroundAnimal.Insert("Animal_LepusEuropaeus",1);
		m_CatchesGroundAnimal.Insert("Animal_LepusEuropaeus",1);
		m_CatchesGroundAnimal.Insert("Animal_LepusEuropaeus",1);
	}
	
	void SetupTrap()
	{
		if ( g_Game.IsServer() )
		{
			if ( this.GetOwnerPlayer().CanDropEntity( this) )
			{
				if ( this.GetDamage() >= 1 ) 
				{
					PlayerBase player = this.GetOwnerPlayer();
					player.MessageImportant( "The trap is ruined." );
				}
				else
				{
					PlayerBase owner_player = this.GetOwnerPlayer();
					owner_player.PlayActionGlobal( "PlayerCraft" );
					owner_player.DropEntity( this );

					//GetDirection
					vector trapPos = ( owner_player.GetDirection() ); // * 0.5;
					trapPos[1] = 0;
					this.SetPosition( owner_player.GetPosition() + trapPos );
					
					owner_player.MessageStatus( m_InfoSetup );

					SetActive();
					
					this.SetOrientation( owner_player.GetOrientation() );
				}
			}
		}
	}
	
	void AlignCatch( ItemBase obj, string catch_name )
	{
		if ( catch_name == "Animal_LepusEuropaeus" )
		{
			obj.SetOrientation( this.GetOrientation() );
			
			vector forward_vec = this.GetDirection();
			vector side_vec = forward_vec.Perpend(  ) * -0.22;
			forward_vec = forward_vec * -0.3;
			
			vector chatch_pos = obj.GetPosition() + forward_vec + side_vec;
			obj.SetPosition( chatch_pos );
		}
	}
}
