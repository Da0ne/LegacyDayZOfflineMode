class EN5C_GardenPlot extends GardenBase
{
	void EN5C_GardenPlot()
	{
		m_BaseFertility = 0.8;
		InitializeSlots();
		DigAllSlots(); // TO DO: Slots should be digged by default, so remove this function when that change is made.
	}
	
	int GetGardenSlotsCount()
	{
		return 9;
	}
	
	void ~EN5C_GardenPlot()
	{
		
	}
}

class GardenPlot extends ItemBase
{
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
};