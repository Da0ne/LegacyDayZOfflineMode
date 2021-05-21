class NotifiersManager
{

	autoptr array<NotifierBase> 	m_Notifiers;
	PlayerBase						m_Player;
	autoptr DisplayStatus			m_DisplayStatus;
	float							m_MinTickTime;
	string 							m_System = "Notifiers";
	void NotifiersManager(PlayerBase player)
	{
		m_Player = player;

		//this may be commented out on server
		m_Notifiers = new array<NotifierBase>;
		
		m_Notifiers.Insert(new Hungry(this));
		m_Notifiers.Insert(new Thirsty(this));
		m_Notifiers.Insert(new Warmth(this));
		m_Notifiers.Insert(new Wetness(this));
		m_Notifiers.Insert(new Fracture(this));
		m_Notifiers.Insert(new Healthy(this));
		m_Notifiers.Insert(new Feverish(this));
		m_Notifiers.Insert(new Sick(this));
		m_Notifiers.Insert(new Bleedish(this));
		m_Notifiers.Insert(new Stuffed(this));
		m_Notifiers.Insert(new Blood(this));
		m_Notifiers.Insert(new Lives(this));
		m_Notifiers.Insert(new AgentInfectionTendency(this));
		
		m_MinTickTime = MIN_TICK_NOTIFIERS;
	}

	void ~NotifiersManager()
	{
		for(int i = 0;i < m_Notifiers.Count(); i++)
		{
			delete m_Notifiers.Get(i);
		}
	}

	PlayerBase GetPlayer()
	{
		return m_Player;
	}

	DisplayStatus GetDisplayStatus()
	{
		return m_DisplayStatus;
	}


	NotifierBase FindNotifier( int type )
	{
		for(int i = 0;i < m_Notifiers.Count(); i++)
		{
			if ( m_Notifiers.Get(i).GetNotifierType() == type )
			{
				return m_Notifiers.Get(i);
			}
		}
	}

	void AttachByType(int notifier, bool triggerEvent = true)
	{
		if ( GetGame().IsServer() || !m_Player.IsPlayer() )
		{
			return;
		}
		FindNotifier(notifier).SetActive(true);
	}

	void DetachByType(int notifier, bool triggerEvent = true)
	{
		if ( GetGame().IsServer() || !m_Player.IsPlayer() )
		{
			return;
		}
		FindNotifier(notifier).SetActive(false);
	}

	void OnScheduledTick()
	{
		TickNotifiers();
	}

	void TickNotifiers()
	{
		Log("Ticking notifiers for player: "+m_Player.ToString());
		float current_time = GetGame().GetTime();

		for(int i = 0;i < m_Notifiers.Count(); i++)
		{
			if ( m_Notifiers.Get(i).IsActive() && m_Notifiers.Get(i).IsTimeToTick(current_time) )
			{
				m_Notifiers.Get(i).OnTick(current_time);
			}
		}
	}

}