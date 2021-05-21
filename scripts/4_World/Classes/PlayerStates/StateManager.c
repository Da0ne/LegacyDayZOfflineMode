enum StateIDs {

	STATE_COUGH,
	STATE_VOMIT,
	STATE_BLINDNESS,
	STATE_BULLET_HIT
};

enum StateTypes 
{
	PRIMARY,
	SECONDARY
};

const int MAX_QUEUE_SIZE = 5; 

class StateManager
{
	
	PlayerBase m_Player;
	
	autoptr map<int, StateBase> m_AvailableStates;
	
	autoptr array<StateBase> m_StateQueuePrimary;
	autoptr array<StateBase> m_StateQueueSecondary;
	autoptr Timer m_Timer;
	bool m_ShowDebug = false;
	
	
	void Init()
	{
		RegisterState(new CoughState);
		RegisterState(new VomitState);
		RegisterState(new BlindnessState);
		RegisterState(new BulletHitState);
	}
	
	void StateManager(PlayerBase player)
	{
		m_StateQueuePrimary = new array<StateBase>;
		m_StateQueueSecondary = new array<StateBase>;
		m_AvailableStates = new map<int, StateBase>;
		m_Player = player;
		Init();
		m_Timer = new Timer();
		//m_Timer.Run(3, this, "PrintDebug", NULL, true);
	}
	
	void ~StateManager()
	{
		for(int i = 0; i < m_StateQueuePrimary.Count(); i++)
		{
			delete m_StateQueuePrimary.Get(i);
		}
		
		for(int z = 0; z < m_StateQueueSecondary.Count(); z++)
		{
			delete m_StateQueueSecondary.Get(z);
		}
	}

	
	void RegisterState(StateBase state)
	{
		state.Init(this, m_Player);
		int id = state.GetID();
		
		if( m_AvailableStates.Contains(id) )
		{
			Error("StateBase state already registered !");
			return;
		}
		
		m_AvailableStates.Insert(id, state);
		//PrintString("inserting id: "+ToString(id));
		
	
	}
	void OnScheduledTick()
	{
		UpdateActiveStates();
		if( m_ShowDebug ) DisplayDebug();
	}
	
	void UpdateActiveStates()
	{	
		//primary
		if( GetCurrentPrimaryActiveState() ) GetCurrentPrimaryActiveState().Update();
		
		//secondary
		for(int i = 0; i < m_StateQueueSecondary.Count(); i++)
		{
			m_StateQueueSecondary.Get(i).Update();
		}
	}
	
	
	void OnStateExit(StateBase state)
	{
		for(int i = 0; i < m_StateQueuePrimary.Count(); i++)
		{
			if( m_StateQueuePrimary.Get(i) == state )
			{
				m_StateQueuePrimary.RemoveOrdered(i);
				return;
			}
		}
	}

	StateBase SpawnState( int state_id )
	{
		if( m_AvailableStates.Get(state_id) )
		{
			string classname = ;
			StateBase state = m_AvailableStates.Get(state_id).ClassName().ToType().Spawn();
			state.Init(this, m_Player);
			return state;
		}
		
		if( !state ) 
		{
			Error("state not registered");
		}
		
		return NULL;
	}
	
	void CleanUpPrimaryQueue()
	{
		for(int i = 0; i < m_StateQueuePrimary.Count();i++)
		{
			delete m_StateQueuePrimary.Get(i);
		}
		m_StateQueuePrimary.Clear();
	}
	
	bool QueueUpPrimaryState(int state_id)
	{
		for(int i = 0; i < m_StateQueuePrimary.Count(); i++)
		{
			if( ComparePriority( GetStatePriority(state_id), m_StateQueuePrimary.Get(i).GetPriority() ) == 1 )
			{
				StateBase state = SpawnState( state_id );
				m_StateQueuePrimary.InsertAt(state,i);

				if( m_StateQueuePrimary.Count() > MAX_QUEUE_SIZE )
				{
					delete m_StateQueuePrimary.Get(MAX_QUEUE_SIZE);//delete the last element in the queue, no need to remove from the array, that's done via state callback on destruct
				}
				
				return true;
			}
		}
		if( m_StateQueuePrimary.Count() < MAX_QUEUE_SIZE) 
		{
			StateBase state_b = SpawnState( state_id );
			m_StateQueuePrimary.Insert(state_b);
			return true;
		}
		else
		{
			return false;
		}
		
	}
	
	void QueueUpSecondaryState(int state_id)
	{
		StateBase state = SpawnState( state_id );
		
		if( state.IsPrimary() ) 
		{
			Error("trying to queue up a primary state into a secondary state queue");
			delete state;
			return;
		}
		m_StateQueueSecondary.Insert(state);
	}
	
	void RemoveSecondaryState(int state_id)
	{
		for(int i = 0; i < m_StateQueueSecondary.Count();i++)
		{
			if( m_StateQueueSecondary.Get(i).GetID() == state_id )
			{
				delete m_StateQueueSecondary.Get(i);
				m_StateQueueSecondary.Remove(i);
				return;
			}
		}
	}
	

	StateBase GetCurrentPrimaryActiveState()
	{
		if(m_StateQueuePrimary.Count() > 0 ) return m_StateQueuePrimary.Get(0);
		else return NULL;
	}
	
	private int ComparePriority( int prio1, int prio2 )
	{
		if( prio1 > prio2 )
		{
			return 1;
		}
		else if ( prio2 > prio1 )
		{
			return 2;
		}
		return 0;
	}
	
	int GetStatePriority(int state_id)
	{
		return m_AvailableStates.Get(state_id).GetPriority();
	}
	
	void OnRPC(int rpc_type, ParamsReadContext ctx)
	{
		ctx.Read(CashedObjectsParams.PARAM1_INT);
		int state_id = CashedObjectsParams.PARAM1_INT.param1;
		bool is_primary = m_AvailableStates.Get(state_id).IsPrimary();
		
		if( rpc_type == RPC_PLAYER_STATES_ON )
		{
			if( is_primary )
			{
				CleanUpPrimaryQueue();
				QueueUpPrimaryState(state_id);
			}
			else
			{
				QueueUpSecondaryState(state_id,);
			}
		}
		else
		{
			if( is_primary )
			{
				CleanUpPrimaryQueue();
			}
			else
			{
				RemoveSecondaryState( state_id );
			}
		}
	
	}
	
	void PrintDebug()
	{
		//if( !GetGame().IsServer() ) return;
		Debug.Log("_____________________________________________", "PlayerStates");	
		for(int i = 0; i < m_StateQueuePrimary.Count(); i++)
		{
			string primary;
			primary += m_StateQueuePrimary.Get(i).ClassName();
			primary += " :: ";
		}
		Debug.Log(primary, "PlayerStates");
		Debug.Log("_____________________________________________", "PlayerStates");	
		for(int z = 0; z < m_StateQueueSecondary.Count(); z++)
		{
			Debug.Log(m_StateQueueSecondary.Get(z).ClassName(), "PlayerStates");

		}
		Debug.Log("_____________________________________________", "PlayerStates");	
	
	}

	void DisplayDebug()
	{
		if( GetGame().IsMultiplayer() && GetGame().IsServer() ) return;
		string primary;
		for(int i = 0; i < m_StateQueuePrimary.Count(); i++)
		{
			primary += m_StateQueuePrimary.Get(i).ClassName();
			primary += " | ";
		}

		DbgUI.BeginCleanupScope();     
        DbgUI.Begin("events", 50, 50);
		DbgUI.Text("Player States Debug Window");
        DbgUI.Text("Primary: " + primary);
		
		for(int z = 0; z < m_StateQueueSecondary.Count(); z++)
		{
			DbgUI.Text("Secondary "+ z.ToString() +" "+m_StateQueueSecondary.Get(z).ClassName());

		}
        DbgUI.End();
        DbgUI.EndCleanupScope();
		
	}

	
}