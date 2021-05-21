class EN5C_Greenhouse extends GardenBase
{
	void EN5C_Greenhouse()
	{
		m_BaseFertility = 1.0;
		InitializeSlots();
		DigAllSlots();
	}
	
	int GetGardenSlotsCount()
	{
		return 11;
	}
}

/*
// Just testing something. Temporal code.
class Land_Misc_Greenhouse extends Building
{
	void Land_Misc_Greenhouse()
	{
		Print("GREENHOUSE CREATED!");
	}
	
	bool ConditionIntoInventory( EntityAI player )
	{
		return false;
	}

	bool ConditionIntoHands( EntityAI player )
	{
		return false;
	}

	bool ConditionOutOfHands( EntityAI player )
	{
		return false;
	}
}
*/