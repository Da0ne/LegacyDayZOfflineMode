class Trap_Bear extends TrapBase
{
	// Vertical raycast start positions:    Center,      North East,    North West,    South East,  South West
	static vector m_RaycastSources[5] = {"0.0 0.1 0.0", "0.2 0.1 0.2", "-.2 0.1 0.2", "0.2 0.1 -.2", "-.2 0.1 -.2"}; // Positions are local to model. Vertical offset prevents ground collision.
	
	void Trap_Bear()
	{
		m_DamagePlayers = 0.35; 		//How much damage player gets when caught
		m_InitWaitTime = 0; 			//After this time after deployment, the trap is activated
		m_AnimationPhaseGrounded = "placing";
		m_AnimationPhaseSet = "BearTrap_Set";
		m_AnimationPhaseTriggered = "placing";
		m_AttachmentEvent.Set("NoAttachment","BearTrapBite");
		m_CustomExplosion = true; // Allows calling of the 'BearTrapBite' event
	}
	
	void BearTrapBite()
	{
		vector 	contact_pos;
		vector 	contact_dir;
		int 	contactComponent;
		bool 	IsSteppedOn = false;
		ManBase	victim_MB = NULL;
		autoptr set<Object> victims = new set<Object>;
		
		for ( local int i = 0; i < 5; ++i )
		{
			local vector raycast_start_pos = ModelToWorld ( m_RaycastSources[i] );
			local vector raycast_end_pos = "0 0.5 0" + raycast_start_pos;
			
			//Debug.DrawArrow( raycast_start_pos, raycast_end_pos ); // Uncomment for debugging of raycast positions
			GetGame().Raycast ( raycast_start_pos, raycast_end_pos, contact_pos, contact_dir, contactComponent, victims , NULL, this, true, false, ObjIntersectIFire);
			
			for ( int i2 = 0; i2 < victims.Count(); ++i2 )
			{
				local Object contact_obj = victims.Get(i2);
				
				if ( contact_obj.IsMan() )
				{
					victim_MB = contact_obj;
					break;
				}
			}
			
			if (victim_MB)
			{
				IsSteppedOn = true;
				break;
			}
		}
		
		if (IsSteppedOn)
		{
			if (victim_MB.IsPlayer())
			{
				if ( victim_MB.IsKindOf("SurvivorENFANIMSYS") ) // Do not damage OLD player through new damage system! Otherwise the game might crash!
				{
					string damage_zone = victim_MB.GetDamageZoneNameByComponentIndex(contactComponent);
					victim_MB.ProcessDirectDamage(DT_CLOSE_COMBAT, this, damage_zone, "MeleeFist", "0 0 0", 1);
				}
				
				// Damage to old player or AI agents is done in the TrabBase system
			}
		}
	}
}
