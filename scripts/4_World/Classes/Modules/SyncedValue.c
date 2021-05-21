class SyncedValue
{
	string m_Name;
	float m_Value;
	
	void SyncedValue( string name, float value )
	{
		m_Name = name;
		m_Value = value;
	}
	
	string GetName()
	{
		return m_Name;
	}
	
	float GetValue()
	{
		return m_Value;
	}
}
