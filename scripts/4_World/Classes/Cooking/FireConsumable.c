class FireConsumable
{
	ItemBase m_Item;
	float m_Energy;
	float m_RemainingEnergy;
	
	void FireConsumable ( ItemBase item, float energy )
	{
		m_Item = item;
		m_Energy = energy;
		m_RemainingEnergy = energy;
	}
}
