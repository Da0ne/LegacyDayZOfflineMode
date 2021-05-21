class StateBase
{
	int m_Priority;
	SoundOnVehicle m_SoundObject;
	bool m_PlayedSound;
	bool m_IsActivated;
	PlayerBase m_Player;
	
	int m_ID;
	
	bool m_QuitOnAnimFinish = true;
	bool m_DestroyRequested = false;
	bool m_StateType = false;
	StateManager m_Manager;
	
	void StateBase()
	{
	}
	
	void ~StateBase()
	{
		OnDestructed();
	}
	
	int GetID()
	{
		return m_ID;
	}

	bool IsPrimary()
	{
		if( m_StateType == StateTypes.PRIMARY)
		return true;
		else return false;
	}
	
	PlayerBase GetPlayer()
	{
		return m_Player;		
	}

	int GetPriority()
	{
		return m_Priority;
	}
	
	bool OnConstructed(StateManager manager)
	{
		
	}
	
	bool OnDestructed()
	{
		if( IsActivated() ) 
		{
			Deactivate();
		}
		m_Manager.OnStateExit(this);
	}

	void Activate()
	{
		m_IsActivated = true;
		if( GetGame() && GetGame().IsServer() )
		{
			OnGetActivatedServer(m_Player);
			if( GetGame().IsMultiplayer() ) SyncToClientActivated( GetID() );
		}
		else
		{
			OnGetActivatedClient(m_Player);
		}
	}
	
	void Deactivate()
	{
		if( !GetGame() ) return;
		m_IsActivated = false;
		if( GetGame().IsServer() ) 
		{
			OnGetDeactivatedServer(m_Player);
			if( GetGame().IsMultiplayer() ) SyncToClientDeactivated( GetID() );
		}
		else OnGetDeactivatedClient(m_Player);
		
	}
	
	bool IsActivated()
	{
		return m_IsActivated;
	}
	
	void Update()
	{
		if( !IsActivated() ) 
		{
			Activate();
		}
		if( GetGame().IsServer() && GetGame().IsMultiplayer() ) OnUpdateServer(m_Player);
		else OnUpdateClient(m_Player);
		CheckDestroy();
	}
	
	void Init(StateManager manager, PlayerBase player)
	{
		m_Manager = manager;
		m_Player = player;
		OnInit();
	}
	
	void PlayAnimation(int animation, bool destroy_on_finish = true)
	{
		StateCB callback = GetPlayer().StartCommand_Action(animation, StateCB, 0);
		callback.Init(this, destroy_on_finish);
	}
	
	void PlaySound(string sound, bool destroy_on_finish = true)
	{
		m_SoundObject = GetGame().CreateSoundOnObject(GetPlayer(), sound, 0, false);
		m_PlayedSound = true;
	}

	void SyncToClientActivated( int state_id )
	{
		if( !GetGame().IsServer() ) return;
		
		CashedObjectsParams.PARAM1_INT.param1 = state_id;
		GetGame().RPCSingleParam(GetPlayer(), RPC_PLAYER_STATES_ON, CashedObjectsParams.PARAM1_INT,GetPlayer() );
	}
	
	void SyncToClientDeactivated( int state_id )
	{
		if( !GetGame().IsServer() ) return;
		CashedObjectsParams.PARAM1_INT.param1 = state_id;
		GetGame().RPCSingleParam(GetPlayer(), RPC_PLAYER_STATES_OFF, CashedObjectsParams.PARAM1_INT,GetPlayer() );
	}

	void CheckSoundFinished()
	{
		if (m_PlayedSound && !m_SoundObject)
		{
			m_DestroyRequested = true;		
		}
	}
	void CheckDestroy()
	{
		CheckSoundFinished();
		if (m_DestroyRequested) Destroy();
	}
	
	void Destroy()
	{
		delete this;
	}
	
	//!gets called upon animation state exit
	void OnAnimationFinish(bool canceled, bool destroy)
	{
		if( destroy ) m_DestroyRequested = true;
	}
	//!this is just for the state parameters set-up and is called even if the state doesn't execute, don't put any gameplay code in here
	void OnInit(){}
	
	//!gets called every frame
	void OnUpdateServer(PlayerBase player){}
	
	//!gets called every frame
	void OnUpdateClient(PlayerBase player){}
	//!gets called once on an state which is being activated
	void OnGetActivatedServer(PlayerBase player){}
	void OnGetActivatedClient(PlayerBase player){}
	//!only gets called once on an active state that is being deactivated
	void OnGetDeactivatedServer(PlayerBase player){}
	void OnGetDeactivatedClient(PlayerBase player){}
}
