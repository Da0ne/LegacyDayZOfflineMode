class CfgParamBool extends CfgParamType
{	
	private bool m_Value;
	
	void CfgParamBool(string param_name)
	{
		m_Value = false;
	}
	
	void SetValue(bool value)
	{
		m_Value = value;
	}
	
	bool GetValue()
	{
		return m_Value;
	}
	
	int GetType()
	{
		return CFG_TYPE_BOOL;
	}
}