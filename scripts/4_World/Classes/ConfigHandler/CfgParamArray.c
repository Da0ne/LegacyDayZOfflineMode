class CfgParamArray extends CfgParam
{
	private array<CfgParam> m_Values;
	
	void CfgParamArray(string param_name)
	{
		m_Values = new array<CfgParam>;
	}
	
	void ~CfgParamArray()
	{
		if ( m_Values != NULL )
		{
			Clear();
			delete m_Values;
		}
	}
	
	void Clear()
	{
		for ( local int i = 0; i < m_Values.Count(); ++i )
		{
			delete m_Values.Get(i);
		}
		
		m_Values.Clear();
	}
	
	array<CfgParam> GetValues()
	{
		return m_Values;
	}
	
	CfgParam GetValue(local int i)
	{
		return m_Values.Get(i);
	}
	
	void InsertValue(local CfgParam value)
	{
		if ( m_Values == NULL )
		{
			m_Values = new array<CfgParam>;
		}
		
		m_Values.Insert(value);
	}
	
	CfgParam GetValueByName(local string name, local int cfg_type)
	{
		if ( m_Values == NULL )
		{
			m_Values = new array<CfgParam>;
		}
		
		for ( local int i = 0; i < m_Values.Count(); ++i )
		{
			local CfgParam p = m_Values.Get(i);
			
			if ( name == p.GetName() )
			{
				return p;
			}
		}
		
		CfgParam param = CreateParam(name, cfg_type);
		m_Values.Insert(param);
		
		return param;
	}
	
	void SetParams(array<CfgParam> value_array)
	{
		if ( m_Values != NULL )
		{
			delete m_Values;
		}
		m_Values = value_array;
	}	
	
	bool IsValueArray()
	{
		return true;
	}
	
	int GetType()
	{
		return CFG_TYPE_ARRAY;
	}
}