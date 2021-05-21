// #include "Scripts\Classes\PlayerModifiers\Modifiers\_constants.c"
class ModifierBase
{
	int					m_ID = 0;
	ModifiersManager 	m_Manager;//the manager instance
	string 				m_System = "Modifiers";
	float 				m_ActivatedTime;//overall time this modifier was active
	bool				m_TrackActivatedTime;//should this modifier track overall time it was active ?
	PlayerBase			m_Player;
	float				m_TickIntervalInactive = 5000;
	float				m_TickIntervalActive = 3000;
	bool				m_IsActive;
	float				m_LastTickedActive;
	float				m_LastTickedInactive;
	
	PluginPlayerStatus 		m_ModulePlayerStatus;

	void ModifierBase()
	{
		m_ModulePlayerStatus 	= GetPlugin(PluginPlayerStatus);
	}
	
	void InitBase(PlayerBase player, ModifiersManager manager)
	{
		m_Manager 				= manager;
		m_Player 				= player;
		Init();
	}
	
	void Init(){}


	PlayerBase GetPlayer()
	{
		return m_Player;
	}

	void Tick(float delta_time)
	{

		if( IsActive() )
		{
			if(GetGame().GetTime() > (m_LastTickedActive + m_TickIntervalActive) )
			{
				if( DeactivateCondition(m_Player) )
				{
					Deactivate();
					return;
				}
				
				float deltaT;
				if( m_LastTickedActive != 0 )
				{
					deltaT = (GetGame().GetTime() - m_LastTickedActive) / 1000;		
				}
				else
				{
					deltaT = 0;
				}
				m_ActivatedTime += deltaT;
				m_LastTickedActive = GetGame().GetTime();
				
				OnTick(m_Player, deltaT);
			}
		}
		else
		{
			if( GetGame().GetTime() > (m_LastTickedInactive + m_TickIntervalInactive) )
			{
				if( ActivateCondition(m_Player) )
				{
					m_LastTickedInactive = GetGame().GetTime();
					Activate();
					return;
				}
			}
		}
	}
	
	bool IsActive()
	{
		return m_IsActive;
	}
	
	bool IsTrackAttachedTime()
	{
		return m_TrackActivatedTime;
	}
	
	int GetAttachedTime()
	{
		return m_ActivatedTime;
	}
	
	void SetAttachedTime(int time)
	{
		m_ActivatedTime = time;
	}
	
	int GetModifierID()
	{
		return m_ID;
	}
	
	string GetName()
	{
		return this.ClassName() + " modifier";
	}

	bool ActivateCondition(PlayerBase player)
	{
		return false;
	}

	bool DeactivateCondition(PlayerBase player)
	{
		return false;
	}

	void OnActivate(PlayerBase player)
	{

	}

	void OnDeactivate(PlayerBase player)
	{

	}

	void Activate(bool trigger = true)
	{
		m_IsActive = true;
		if(trigger) OnActivate(m_Player);
	}

	void Deactivate(bool trigger = true)
	{
		m_IsActive = false;
		m_ActivatedTime = 0;
		if(trigger) OnDeactivate(m_Player);
	}


	void OnStoreSave( ParamsWriteContext ctx )
	{
	}


	private bool OnTick(PlayerBase player, float deltaT)
	{
		
	}
}
