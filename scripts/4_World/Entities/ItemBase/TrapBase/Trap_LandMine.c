class Trap_LandMine extends TrapBase
{
	void Trap_LandMine()
	{
		m_Reactivable = false;
		m_CustomExplosion = true;
		m_DefectRate = 0.15;
		m_WrongManipulation = 0.05;
		m_CustomExplosionEvent = "MakeExplosion";
		m_DamagePlayers = 0.0; 			//How much damage player gets when caught
		m_InitWaitTime = 3; 			//After this time after deployment, the trap is activated
		m_InfoActivationTime = "Trap will be active in " + m_InitWaitTime.ToString() + " seconds";  // nefunguje dynamicke vyrazy mimo funkcii
		
		m_AddDeactivationDefect = true;
	}
		
	void MakeExplosion( TrapBase obj )
	{
		this.Explode();
	}
	
	void OnDeactivateWrongManipulation( PlayerBase player )
	{
		if ( GetDamage() >= 0.5 )
		{
			MakeExplosion( this );
		}
		else 
		{
			player.MessageStatus( m_InfoDeactivated );
			this.SetInactive();
		}
	}
}
