class PlayerAgentPool
{
	autoptr map<int,float> m_VirusPool = new map<int,float>;
	autoptr array<int> m_VirusPoolArray = new array<int>;
	float m_LastTicked = 0;
	float m_TotalAgentCount;
	PlayerBase m_Player;
	
	PluginTransmissionAgents m_PluginTransmissionAgents = GetPlugin(PluginTransmissionAgents);
	
	void PlayerAgentPool(PlayerBase player)
	{
		m_Player = player;
	}

	void ~PlayerAgentPool()
	{
		
	}
	
	void PrintAgents()
	{
		if( m_VirusPool )
		{
			for(int i = 0; i < m_VirusPool.Count(); i++)
			{
				Debug.Log("Agent: "+ m_VirusPool.GetKey(i).ToString(), "Agents");
				Debug.Log("Count: "+ m_VirusPool.GetElement(i).ToString(), "Agents");
			}
		
		}
	}
	
	void ImmuneSystemTick(float value)//this is a regular tick induced in the the player's immune system
	{
		GrowAgents(value);
	}

	void OnStoreSave(ParamsWriteContext ctx)
	{
		Debug.Log("PlayerAgentPool OnStoreSave called", "Agents");
		CashedObjectsParams.PARAM1_INT.param1 = m_VirusPool.Count();
		
		ctx.Write( CashedObjectsParams.PARAM1_INT );
		
		for(int i = 0; i < m_VirusPool.Count();i++)
		{
			CashedObjectsParams.PARAM2_INT_INT.param1 = m_VirusPool.GetKey(i);
			CashedObjectsParams.PARAM2_INT_INT.param2 = m_VirusPool.GetElement(i);
			//Debug.Log("Writing agent to DB id: "+ ToString(CashedObjectsParams.PARAM2_INT_INT.param1), "Agents");
			//Debug.Log("Writing agent to DB count: "+ ToString(CashedObjectsParams.PARAM2_INT_INT.param2), "Agents");
			ctx.Write( CashedObjectsParams.PARAM2_INT_INT );
		}
	}
	
	void OnStoreLoad(ParamsReadContext ctx)
	{
		Debug.Log("PlayerAgentPool OnStoreLoad called", "Agents");
		ctx.Read(CashedObjectsParams.PARAM1_INT);
		int count = CashedObjectsParams.PARAM1_INT.param1;
		
		for(int i = 0; i < count;i++)
		{
			ctx.Read(CashedObjectsParams.PARAM2_INT_INT);
			
			int key = CashedObjectsParams.PARAM2_INT_INT.param1;
			float value = CashedObjectsParams.PARAM2_INT_INT.param2;
			
			Debug.Log("Reading agent from DB id: "+ key.ToString(), "Agents");
			
			m_VirusPool.Insert( key,value );
		}
	
	}
		
	void AddAgent(int agent_id, int count)
	{
		int max_count = m_PluginTransmissionAgents.GetAgentMaxCount(agent_id);

		if(	!m_VirusPool.Contains(agent_id) )//if it contains, maybe add count only ?
		{
			m_VirusPool.Insert( agent_id, Math.Clamp(count,0,max_count) );
		}
		else
		{
			int new_value = m_VirusPool.Get(agent_id) + count;
			m_VirusPool.Set(agent_id,Math.Clamp(new_value,0,max_count) );
		}
	}


	void RemoveAgent(int agent_id)
	{
		if(	m_VirusPool.Contains(agent_id) )
		{
			m_VirusPool.Remove( agent_id );
		}
	}
	
	void RemoveAllAgents()
	{
		m_VirusPool.Clear();
	}
	
	array<int> GetAgents()
	{
		m_VirusPoolArray.Clear();
		
		for(int i = 0; i < m_VirusPool.Count();i++)
		{
			m_VirusPoolArray.Insert( m_VirusPool.GetKey(i) );
		}
		
		return m_VirusPoolArray;
	}

	int GetSingleAgentCount(int agent_id)
	{
		if(	m_VirusPool.Contains(agent_id) )//if it contains, maybe add count only ?
		{
			return m_VirusPool.Get( agent_id );
		}
	}
	
	int GetTotalAgentCount()
	{
		int agent_count;
		for(int i = 0; i < m_VirusPool.Count(); i++)
		{
			agent_count += m_VirusPool.GetElement(i);
		}
		return agent_count;
	}
	
	
	
	void GrowAgents(float immune_attack_value)
	{
		if ( !IsPluginManagerExists() )//check if modules are running
		{
			return;
		}
		Debug.Log("GrowAgents called, immune_attack_value = " + immune_attack_value.ToString(), "Agents");
	
		float m_DeltaT = (GetGame().GetTime() - m_LastTicked) / 1000;
		m_LastTicked = GetGame().GetTime();
		m_TotalAgentCount = 0;
		for(int i = 0; i < m_VirusPool.Count(); i++)
		{
			int agent_id = m_VirusPool.GetKey(i);
			float invasibility = m_PluginTransmissionAgents.GetAgentInvasibility( agent_id );
			float grow_delta = invasibility * m_DeltaT;
			float old_count = m_VirusPool.Get( agent_id );
			float new_count = old_count + grow_delta - immune_attack_value;
			m_TotalAgentCount += new_count;//count the overall num of agents
			Debug.Log("old_count"+ old_count.ToString(), "Agents");
			Debug.Log("new_count"+ new_count.ToString(), "Agents");

			
			if(new_count > 0)
			{
				Debug.Log("+ growing agent"+ agent_id.ToString() +"to count: "+new_count.ToString(), "Agents");
				m_VirusPool.Set( agent_id, new_count);
			}
			else
			{
				Debug.Log("- REMOVING agent"+ agent_id.ToString(), "Agents");
				m_VirusPool.Remove( agent_id );
			}	
		}
		//m_TotalAgentDelta = m_TotalAgentCount - m_TotalAgentDelta;
		//if( m_Player.GetNotifiersManager() ) m_Player.GetNotifiersManager().FindNotifier(NTF_AGENT_INFECTION).DisplayTendency(m_TotalAgentDelta);
		
	}
	
	void GetDebugObject(array<Param> object_out)
	{
	
		int total_items = 0;
		for(int i = 0;i < m_VirusPool.Count(); i++)
		{
			int 	item_key 	= m_VirusPool.GetKey(i);
			float 	item_value 	= m_VirusPool.Get(item_key);
			
			string agent_name = m_PluginTransmissionAgents.GetNameByID(item_key);
			
			object_out.Insert( new Param2<string,string>(agent_name, item_value.ToString()) );
			total_items++;
		}
		object_out.InsertAt(new Param1<int>(total_items) ,0);
	}
}