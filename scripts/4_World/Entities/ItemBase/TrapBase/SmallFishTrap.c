class Trap_SmallFish;

class EN5C_SmallFishTrap extends Trap_SmallFish 
{
	autoptr RainProcurementManager m_RainProcurement;

	// ----------------------------------------------------------------------------------------
	void OnPlacementFinished( PlayerBase player )
	{
		// fill the bottle when placed in water
		vector position = this.GetPosition();

		if ( GetGame().SurfaceIsSea( position[0], position[2] ) || GetGame().SurfaceIsPond( position[0], position[2] ) )
		{
			this.SetQuantity2( this.GetQuantityMax2() );
			SetupTrapPlayer( player, false );
		}	
		else
		{
			m_RainProcurement = new RainProcurementManager( this );
			m_RainProcurement.InitRainProcurement();
		}
	}
	
	// ----------------------------------------------------------------------------------------
	// When the item is picked up by a player, stop the rain procurement
	void EEItemLocationChanged( EntityAI old_owner, EntityAI new_owner ) 
	{
		if (m_RainProcurement != NULL) 
		{
			super.EEItemLocationChanged(old_owner, new_owner);
		
			if (new_owner)
			{
				m_RainProcurement.StopRainProcurement();
				delete m_RainProcurement;
			}
		}
	}
}