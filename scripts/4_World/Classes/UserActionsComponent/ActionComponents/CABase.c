class CABase 
{	
	protected ActionBase	m_Action;
	protected float 		m_DeltaT;
	protected float 		m_LastTick;
	protected Param			m_ACData;
	protected bool			m_ContinuousAction = false;	
	
	void Init( ActionBase action, PlayerBase player, Object target, ItemBase item )
	{
		m_Action = action;
		if ( !m_ACData ) m_ACData = new Param;
		m_LastTick = GetGame().GetTime();
		Setup(player,target,item);
	}
	
	void Setup( PlayerBase player, Object target, ItemBase item  )
	{
	}

	int Execute( PlayerBase player, Object target, ItemBase item  )
	{
	}

	int Interupt( PlayerBase player, Object target, ItemBase item  )
	{
	}
	
	float GetDeltaT() //delta time for action ticks
	{
		m_DeltaT = (GetGame().GetTime() - m_LastTick) / 1000;
		m_LastTick = GetGame().GetTime();
		return m_DeltaT;
	}
	
	void SetACData(Param units) //overload this method if you want to send more than one parameter out of the action component
	{
		if ( m_ACData )	m_ACData = units;
	}
	
	Param GetACData() //delta time for action ticks
	{
		if ( m_ACData ) return m_ACData;
		return NULL;
	}
	
	bool IsContinuousAction() //informs command callback whether action is looped or oneshot
	{
		return m_ContinuousAction;
	}
};