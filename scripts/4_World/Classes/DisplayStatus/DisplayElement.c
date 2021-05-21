class DisplayElement extends MessageReceiverBase
{

	bool 	m_Blinking;
	int		m_Value;
	int		m_Type = 0;
	int 	m_Range = 0;
	int		m_Colors[7];
	string 	m_Labels[7];
	int		m_Key = 0;
	int		m_Pos = 1;
	PluginPlayerStatus 	m_ModulePlayerStatus;

	void DisplayElement()
	{
		m_System = "DisplayStatus";
		m_ModulePlayerStatus = GetPlugin(PluginPlayerStatus);
		//SetMaxValue();
	}
	
	void ImmediateUpdate()
	{
		if ( m_Type == TYPE_BAR )//<<<<--------- bar( hardcoded for stamina and stamina range, no generic Bar UI support)
		{
			m_ModulePlayerStatus.SetStamina(m_Value, m_Range);
		}
		if ( m_Type == TYPE_STANCE )
		{
			m_ModulePlayerStatus.SetStance( m_Value );
		}
	}
	
	void UpdateStatus()
	{
		if ( !IsPluginManagerExists() )
		{
			return;
		}
		if( m_Type == TYPE_TDCY )//<<<<--------- tendency
		{
			int i;
			if(m_Value > 3) 
			{
				i = 0 - (m_Value - 3);
			}
			else
			{
				i = m_Value;
			}
			//Log("m_Value: " +ToString(m_Value));
			//Log("i value: " +ToString(i));
			m_ModulePlayerStatus.DisplayTendency(m_Key, i);
		}

		else if (m_Type == TYPE_NTFR )//<<<<--------- inventory badge
		{
			string message 	= m_Labels[m_Value];
			int color 		= m_Colors[m_Value];
			
			if(m_Value>0) 
			{
				m_ModulePlayerStatus.SetNotifier(m_Key, m_Pos, message, color);
			}
			else
			{
				m_ModulePlayerStatus.SetNotifier(m_Key); //hiding
			}
		}
		
		else if (m_Type == TYPE_BADGE )//<<<<--------- badge
		{
			if(m_Value == 1) 
			{
				m_ModulePlayerStatus.SetBadge(m_Key, true);
			}
			else
			{
				m_ModulePlayerStatus.SetBadge(m_Key, false);
			}
		}
	}


	void SetValue(int value)
	{
		if (m_Type != TYPE_BAR )//<<<<--------- bar( hardcoded for stamina and stamina range, no generic Bar UI support)
		{
			m_Value = value;
		}
	}

	int GetValue()
	{
		return m_Value;
	}

	void SetBar(int value,int range)
	{
		if (m_Type == TYPE_BAR )//<<<<--------- bar( hardcoded for stamina and stamina range, no generic Bar UI support)
		{
			m_Value = value;
			m_Range = range;
		}
	}

	int BitToDec(int mask, int index, int length)
	{
		int value = mask & (GetCompareMask() << index);
		value = value >> index;
		return value;
	}

	int GetCompareMask()
	{
		switch(m_Type)
		{
			case TYPE_BADGE:
				return 1;
			case TYPE_TDCY:
				return 7;
			case TYPE_NTFR:
				return 7;
			case TYPE_BAR:
				return 63;
			case TYPE_STANCE:
				return 4;
		}
	}

	int GetType()
	{
		return m_Type;
	}

	int GetOffset()
	{
		switch(m_Type)
		{
			case TYPE_BADGE:
				return 1;
			case TYPE_TDCY:
				return 3;
			case TYPE_NTFR:
				return 3;
			case TYPE_BAR:
				return 6;
			case TYPE_STANCE:
				return 4;
		}
	}

	void SetMaxValue()
	{
		if( m_Type == TYPE_BADGE )
		{
			//Log("offset value:"+ToString(1));
			m_Value = 1;
		}
		if( m_Type == TYPE_TDCY )
		{
			//Log("offset value:"+ToString(m_Value));
			m_Value = 7;
		}
		if( m_Type == TYPE_NTFR )
		{
			//Log("offset value:"+ToString(m_Value));
			m_Value = 7;
		}
	}
}