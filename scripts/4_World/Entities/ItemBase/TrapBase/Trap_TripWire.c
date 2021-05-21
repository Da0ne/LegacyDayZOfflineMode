class Trap_TripWire : TrapBase
{
	private int m_StringType; // Type of string from which this TripWire is crafted: 0 - Crafting_MetalWire, 1 - Fence_BarbedWire
	
	void Trap_TripWire()
	{
		m_Reactivable = true;
		m_WrongManipulation = 0.0;
		m_CustomExplosion = true;
		m_DamagePlayers = 0.0; 			//How much damage player gets when caught
		m_InitWaitTime = 3; 			//After this time after deployment, the trap is activated
		m_NeedActivation = false;
		m_AnimationPhaseGrounded = "inventory";
		m_AnimationPhaseSet = "placing";
		m_AnimationPhaseTriggered = "TripWire_triggered";
		m_AnimationPhaseAttachment.Set("Drink_SodaCanEmpty","TripWire_set_can");
		m_AnimationPhaseAttachment.Set("GrenadeRGD5","TripWire_set_grenade");
		m_AnimationPhaseAttachment.Set("Grenade","TripWire_set_grenade");
		m_AttachmentEvent.Set("Drink_SodaCanEmpty","MakeNoise");
		m_AttachmentEvent.Set("GrenadeRGD5","MakeExplosion");
		m_AttachmentEvent.Set("Grenade","MakeExplosion");
		m_InfoActivationTime = "Trap will be active in " + m_InitWaitTime.ToString() + " seconds";  // nefunguje dynamicke vyrazy mimo funkcii
		
		m_SurfaceForSetup.Insert("CRBoulders");
		m_SurfaceForSetup.Insert("CRHlina");
		m_SurfaceForSetup.Insert("CRForest1");
		m_SurfaceForSetup.Insert("CRForest2");
		m_SurfaceForSetup.Insert("CRGrass1");
		m_SurfaceForSetup.Insert("CRGrass2");
		m_SurfaceForSetup.Insert("CRGrit1");
		m_SurfaceForSetup.Insert("Cesta");
	}

	// ITEM CANNOT BE TAKEN WHEN CONTAINS CARGO
	bool ConditionAttach ( EntityAI  parent ) 
	{
		return true;
	}
	
	void MakeNoise( TrapBase obj )
	{
		GetGame().Say3D( obj, "trapWireCans_0", 0 );
	}
	
	void MakeExplosion( TrapBase obj, ItemBase attachment )
	{
		attachment.SetPosition( obj.GetPosition() ); 
		attachment.Explode();
		GetGame().ObjectDelete( this );
	}

	void CreateTrigger()
	{
		m_TrapTrigger = GetGame().CreateObject( "TrapTrigger", this.GetPosition(), false );
		vector mins = "-0.75 -0.5 -0.01";
		vector maxs = "0.75 0.5 0.01";
		m_TrapTrigger.SetOrientation( this.GetOrientation() );
		m_TrapTrigger.SetExtents(mins, maxs);	
		m_TrapTrigger.SetParentObject( this );
	}
	
	void SetStringType( int string_type )
	{
		m_StringType = string_type;
	}
	
	int GetStringType()
	{
		return m_StringType;
	}
}
