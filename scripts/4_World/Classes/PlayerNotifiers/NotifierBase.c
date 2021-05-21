class NotifierBase
{
	
	float				m_DeltaT; // time in seconds since the last tick
	autoptr Timer 		m_Timer1; // timer which can be used for whatever
	PlayerBase			m_Player; //the player this Notifier belongs to
	int					m_Type;
	NotifiersManager	m_Manager;
	const int 			TENDENCY_BUFFER_SIZE = 9;//for best results, this should be somewhat aligned with modifier frequency
	
	bool				m_Active;
	private float		m_SecsSinceLastMessage; //internal counter
	private float		m_SecsToMessage; //internal counter
	protected float		m_MinPauseBetweenMessages; //minimal amount of seconds that needs to pass till message is displayed
	protected float		m_MaxPauseBetweenMessages; //maximal amount of seconds that can pass till message is displayed
	private float		m_SecsSinceLastSound; //internal counter
	private float		m_SecsToSound; //internal counter
	protected float		m_MinPauseBetweenSounds; //minimal amount of seconds that needs to pass till sound is played
	protected float		m_MaxPauseBetweenSounds; //maximal amount of seconds that can pass till sound is played
	private float		m_SecsSinceLastAnimation; //internal counter
	private float		m_SecsToAnimation; //internal counter
	protected float		m_MinPauseBetweenAnimations; //minimal amount of seconds that needs to pass till animation played
	protected float		m_MaxPauseBetweenAnimations; //maximal amount of seconds that can pass till animation is splayed
	float				m_TickInterval;
	float				m_TickIntervalLastTick;
	float				m_TendencyBuffer[TENDENCY_BUFFER_SIZE];
	int					m_TendencyBuffer_WriteIterator;
	int					m_TendencyBufferWriteTimes;
	//int					m_TendencyID;
	
	PlayerStat<float>	m_MaxStamina;

	PluginPlayerStatus 	m_ModulePlayerStatus;
		
	void NotifierBase(NotifiersManager manager)
	{

		//m_Timer1 = new Timer();
		m_ModulePlayerStatus = GetPlugin(PluginPlayerStatus);
		m_Active = true; 
		m_Manager = manager;
		m_Player = manager.GetPlayer();
		
		m_MaxStamina = m_Player.m_PlayerStats.m_MaxStamina;
		m_MinPauseBetweenMessages = -1;
		m_MinPauseBetweenSounds = m_MinPauseBetweenMessages;
		m_MinPauseBetweenAnimations = m_MinPauseBetweenMessages;
		m_TickInterval = 1000;

		
	}

	bool IsTimeToTick(float current_time)
	{
		if( current_time > m_TickIntervalLastTick + m_TickInterval )
		{
			return true;
		}
		else 
		{
			return false;
		}
	}
	
	
	DisplayStatus GetDisplayStatus()
	{
		return m_Player.GetDisplayStatus();
	}

	int GetNotifierType()
	{
		return m_Type;
	}

	string GetName()
	{
		return this.ClassName() + " Notifier";
	}

	bool IsActive()
	{
		if ( m_Active )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void SetActive( bool state )
	{
		m_Active = state;
		if ( !state ) HideBadge();
	}

	void DisplayTendency(float delta)
	{
	}

	void AddToCyclicBuffer(float value)//for tendency
	{
		m_TendencyBuffer[m_TendencyBuffer_WriteIterator] = value;
		m_TendencyBuffer_WriteIterator++;
		m_TendencyBufferWriteTimes++;
		if( m_TendencyBuffer_WriteIterator == TENDENCY_BUFFER_SIZE )
		{
			m_TendencyBuffer_WriteIterator = 0;
		}
	}
	
	float ReadFromCyclicBuffer(int index)
	{
		int indx = m_TendencyBuffer_WriteIterator + index;
		if( indx >= TENDENCY_BUFFER_SIZE)
		{
			indx = indx - TENDENCY_BUFFER_SIZE;
		}
		return m_TendencyBuffer[indx];
	}
	
	float GetDeltaAvaraged()//for tendency
	{
		float tendency_sum = 0;
		
		for(int i = 1; i < TENDENCY_BUFFER_SIZE; i++)
		{
			int previous_index = i - 1;
			float tendency = ReadFromCyclicBuffer(i) - ReadFromCyclicBuffer(previous_index);
			//PrintString("index "+previous_index.ToString()+" = " + ReadFromCyclicBuffer(previous_index).ToString() );
			//PrintString("index "+i.ToString()+" = " + ReadFromCyclicBuffer(i).ToString() );
			//PrintString("tendency = " + tendency.ToString() );
			tendency_sum += tendency;
		}
		
		return tendency_sum / (TENDENCY_BUFFER_SIZE - 1);
	}

	void OnTick(float current_time)
	{
		m_TickIntervalLastTick = current_time;
		DisplayBadge();
		if ( m_MinPauseBetweenMessages != -1 )
		{
			if ( m_SecsSinceLastMessage >= m_SecsToMessage )
			{
				DisplayMessage();
				m_SecsToMessage = Math.RandomInt(m_MinPauseBetweenMessages,m_MaxPauseBetweenMessages);
				m_SecsSinceLastMessage = 0;
			}
			else
			{
				m_SecsSinceLastMessage+=m_DeltaT;
			}
		}
		if ( m_MinPauseBetweenSounds != -1 )
		{
			if ( m_SecsSinceLastSound >= m_SecsToSound )
			{
				PlaySound();
				m_SecsToSound = Math.RandomInt(m_MinPauseBetweenSounds,m_MaxPauseBetweenSounds);
				m_SecsSinceLastSound = 0;
			}
			else
			{
				m_SecsSinceLastSound+=m_DeltaT;
			}
		}
		if ( m_MinPauseBetweenAnimations != -1 )
		{
			if ( m_SecsSinceLastAnimation >= m_SecsToAnimation )
			{
				PlayAnimation();
				m_SecsToAnimation = Math.RandomInt(m_MinPauseBetweenAnimations,m_MaxPauseBetweenAnimations);
				m_SecsSinceLastAnimation = 0;
			}
			else
			{
				m_SecsSinceLastAnimation+=m_DeltaT;
			}
		}
		AddToCyclicBuffer( GetObservedValue() );
		DisplayTendency( GetDeltaAvaraged() );

	}
	
	protected int CalculateTendency(float delta, float inctresholdlow, float inctresholdmed, float inctresholdhigh, float dectresholdlow, float dectresholdmed, float dectresholdhigh)
	{                                        	
		int ndelta = TENDENCY_STABLE;
		if ( delta > inctresholdlow )  		ndelta = TENDENCY_INC_LOW;
		if ( delta > inctresholdmed )  ndelta = TENDENCY_INC_MED;
		if ( delta > inctresholdhigh ) ndelta = TENDENCY_INC_HIGH;	
		if ( delta < dectresholdlow )  ndelta = TENDENCY_DEC_LOW;
		if ( delta < dectresholdmed )  ndelta = TENDENCY_DEC_MED;
		if ( delta < dectresholdhigh ) ndelta = TENDENCY_DEC_HIGH;
		return ndelta;
	}                                        	

	protected void DisplayBadge()
	{
	}

	protected void HideBadge()
	{
	}

	protected void DisplayMessage()
	{
	}

	protected void PlaySound()
	{
	}
	
	protected void PlayAnimation()
	{
	}
	
	protected float GetObservedValue()
	{
		return 0;
	}
}