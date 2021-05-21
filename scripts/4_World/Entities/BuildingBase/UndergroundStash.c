class UndergroundStash extends Building
{
	ItemBase m_StashedItem;
	
	void SetStashedItem(ItemBase item)
	{
		m_StashedItem = item;
	}

	ItemBase GetStashedItem()
	{
		return m_StashedItem;
	}
}