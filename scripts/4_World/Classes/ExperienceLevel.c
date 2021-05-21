class ExperienceLevel
{
	int m_RequiredExp;
	
	map<string, float>	m_Numbers;
	map<string, string>	m_Strings;
	

	void ExperienceLevel()
	{
		int m_RequiredExp;

		m_RequiredExp	= -1;
		m_Numbers		= new map<string, float>;
		m_Strings		= new map<string, string>;
	}

	void ~ExperienceLevel()
	{
		delete m_Numbers;
		delete m_Strings;
	}

	void SetRequiredExp(int req_exp)
	{
		m_RequiredExp = req_exp;
	}

	int GetRequiredExp()
	{
		return m_RequiredExp;
	}

	void AddParamNumber(string param_name, float value)
	{
		m_Numbers.Set(param_name, value);
	}

	void AddParamString(string param_name, string value)
	{
		m_Strings.Set(param_name, value);
	}

	float GetParamNumber(string param_name)
	{
		return m_Numbers.Get(param_name);
	}

	string GetParamString(string param_name)
	{
		return m_Strings.Get(param_name);
	}
}