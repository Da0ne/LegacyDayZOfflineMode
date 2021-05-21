class ConstructionMaterial
{
	string m_Type;
	float m_Quantity;
	
	void ConstructionMaterial ( string type, float quantity )
	{
		m_Type = type;
		m_Quantity = quantity;
	}
	
	void ~ContructionMaterial()
	{
	}
}
