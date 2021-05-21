class PlayerStatBase
{
	
	void 	OnStoreSave( ParamsWriteContext ctx );
	void 	OnStoreLoad( ParamsReadContext ctx );
	void 	OnRPC(ParamsReadContext ctx);
	float 	Get();
	string 	GetLabel();
	void 	SetByFloat(float value);
	void 	SetByParam(Param param);
	bool	IsSynced();
	
	bool	m_IsSynced;
};

class PlayerStat<Class T> extends PlayerStatBase
{
	protected T 		m_MinValue;
	protected T 		m_MaxValue;
	protected T 		m_Value;
	protected string 	m_ValueLabel;
	Param1<T> 			p1;
	
	void PlayerStat(T min, T max,T init, string label,bool synced)
	{
		m_IsSynced		= synced;
		m_MinValue 		= min;
		m_MaxValue 		= max;
		m_Value			= init;
		m_ValueLabel 	= label;
	}
	
	bool IsSynced()
	{
		return m_IsSynced;
	}
	
	void Set( T value )
	{
		if( value > m_MaxValue)
		{
			m_Value = m_MaxValue;
		}
		else if(value < m_MinValue)
		{
			m_Value = m_MinValue;
		}
		else
		{
			m_Value = value;
		}
	}
	
	void SetByFloat(float value)
	{
		T f = value;
		Set(f);
	}
	
	void SetByParam(Param param)
	{
		p1 = param;
		T v = p1.param1;
		Set(v);
	}
	
	void Add( T value )
	{
		Set(m_Value+value);
	}

	float Get()
	{
		return m_Value;
	}
	
	string GetLabel()
	{
		return m_ValueLabel;
	}

	float GetMax()
	{
		return m_MaxValue;
	}

	float GetMin()
	{
		return m_MinValue;
	}
	
	float GetNormalized()
	{
		return ((Get()- GetMin()) / (GetMax() - GetMin()));
	}
	
	void OnStoreSave( ParamsWriteContext ctx )
	{   
		autoptr Param2<string, T> p1 = new Param2<string, T>(m_ValueLabel, m_Value);
		ctx.Write(p1);
	}

	void OnStoreLoad( ParamsReadContext ctx )
	{
		autoptr Param2<string, T> p1 = new Param2<string, T>("",0);

		if ( ctx.Read(p1) )
		{
			m_Value = p1.param2;
		}
	}
}