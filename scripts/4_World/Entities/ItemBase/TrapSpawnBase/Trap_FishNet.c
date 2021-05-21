class Trap_FishNet extends TrapSpawnBase
{
	void Trap_FishNet()
	{
		m_InitWaitTime = Math.RandomFloat(45,90);
		m_BaitNeeded = false;
		m_IsFoldable = true;

		m_AnimationPhaseSet = "inventory";
		m_AnimationPhaseTriggered = "placing";
		
		m_WaterSurfaceForSetup = true;

		m_CatchesPond = new multiMap<string, float>;
		m_CatchesPond.Insert("Food_Carp",1);
		m_CatchesPond.Insert("Food_Carp",1);
		m_CatchesPond.Insert("Food_Carp",2);
		
		m_CatchesSea = new multiMap<string, float>;
		m_CatchesSea.Insert("Food_Sardines",1);
		m_CatchesSea.Insert("Food_Sardines",1);
		m_CatchesSea.Insert("Food_Sardines",2);

		m_CatchesSea.Insert("Food_Sardines",1);
		m_CatchesSea.Insert("Food_Sardines",1);
		m_CatchesSea.Insert("Food_Sardines",2);

		m_CatchesSea.Insert("Food_Mackerel",1);
		m_CatchesSea.Insert("Food_Mackerel",1);
		m_CatchesSea.Insert("Food_Mackerel",2);
		
		m_CatchesGroundAnimal = new multiMap<string, float>;
	}
	
	// ITEM CANNOT BE TAKEN WHEN CONTAINS CARGO
	bool ConditionIntoInventory ( EntityAI  player ) 
	{
		
		return IsTakeable();
	}
	
	bool ConditionIntoHands ( EntityAI player ) 
	{
		return IsTakeable();
	}

	bool ConditionOutOfHands ( EntityAI player ) 
	{
		return IsTakeable();
	}

	bool ConditionCargoInEntityAI( EntityAI child )
	{
		if ( GetOwnerPlayer() == NULL )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	bool ConditionCargoOutEntityAI( EntityAI child )
	{
		if ( GetOwnerPlayer() == NULL )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}
