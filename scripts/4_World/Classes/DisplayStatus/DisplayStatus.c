class DisplayStatus
{
	const int FLOAT_USABLE_SIZE = 31;
	const int NUMBER_OF_MASKS = 2;//how many INT numbers we need to accommodate all elements(ideally, this number should be calculated in run-time and transmitted via rpc with the masks)
	autoptr map<int, DisplayElement> m_Elements;
	autoptr array<int> m_MaskArray;
	autoptr Timer myTimer1;
	Mission mission;
	Hud hud;
	int m_LastTick;
	PlayerBase m_Player;
	string m_System 		= "DisplayStatus";
	
	autoptr array<Param> rpcParams;
	
	void DisplayStatus(PlayerBase player)
	{

		m_Player 		= player;
		m_Elements 		= new map<int,DisplayElement>;
		m_MaskArray 	= new array<int>;
		m_LastTick		= 0;
		rpcParams 		= new array<Param>;
		
		for(int i = 0; i < NUMBER_OF_MASKS; i++)
		{
			rpcParams.Insert( new Param1<int>(0) );
		}

		//myTimer2.Run(3, this, "Debug", NULL, true);
		//myTimer2.Run(10, this, "PrintElements", NULL, true);
		
		// don't add new stuff unless you really really know what you're doing
		AddElement(DELM_BADGE_STUFFED, new BadgeStuffed);// size 1
		AddElement(DELM_BADGE_WET, new BadgeWet);// size 1
		AddElement(DELM_BADGE_SICK, new BadgeSick);// size 1
		AddElement(DELM_BADGE_POISONED, new BadgePoisoned);// size 1
		AddElement(DELM_BADGE_FRACTURE, new BadgeFracture);// size 1
		AddElement(DELM_TDCY_HEALTH, new TendencyHealth);// size 3
		AddElement(DELM_TDCY_BLOOD, new TendencyBlood);// size 3
		AddElement(DELM_TDCY_TEMPERATURE, new TendencyTemperature);// size 3
		AddElement(DELM_TDCY_ENERGY,new TendencyHunger);// size 3
		AddElement(DELM_TDCY_WATER, new TendencyThirst);// size 3
		AddElement(DELM_NTFR_BLOOD,new InvBadgeBlood);// size 3
		AddElement(DELM_NTFR_FULLNESS, new InvBadgeFullness);// size 3
		AddElement(DELM_NTFR_HUNGER, new InvBadgeHunger);// size 3
		// sum 29/31
		AddElement(DELM_NTFR_FEVER, new InvBadgeFever);// size 3
		AddElement(DELM_NTFR_FRACTURE, new InvBadgeFracture);// size 3
		AddElement(DELM_NTFR_HEALTHY, new InvBadgeSick);// size 3
		AddElement(DELM_NTFR_WETNESS, new InvBadgeWetness);// size 3
		AddElement(DELM_NTFR_THIRST, new InvBadgeThirst);// size 3
		AddElement(DELM_BAR_STAMINA, new ElementStamina);// size 6
		AddElement(DELM_TDCY_BACTERIA, new TendencyBacteria);
		AddElement(DELM_STANCE, new ElementStance);
		//sum 21/31
		mission = GetGame().GetMission();
		if ( mission )
		{
			hud = mission.GetHud();
		}
	}
	
	void ~DisplayStatus()
	{
		while( m_Elements.Count() > 0 )
		{
			delete m_Elements.GetElement(0);
			m_Elements.RemoveElement(0);
		}
		
		for(int i = 0; i < rpcParams.Count();i++)
		{
			delete rpcParams.Get(i);
		}
	}

	void OnScheduledTick()
	{
		if(GetGame().GetTime() > m_LastTick + DISPLAY_STATUS_UPDATE_INTERVAL)
		{
			SendRPC();
			m_LastTick = GetGame().GetTime();
		}
		
		ImmediateUpdate();
		DisplayPresence();
	}

	void AddElement(int id, DisplayElement element)
	{
		Log("adding element:"+id.ToString());
		m_Elements.Set(id, element );
	}

	void SetBlinking(int id, bool blink)
	{
		Log("for element: "+id.ToString()+"setting blinking:"+blink.ToString());
		m_Elements.Get(id).m_Blinking = blink;
	}

	void SetStatus(int id, int level)
	{
		if(level<0) level = Math.AbsInt(level)+3;//REWORK.V: hacking, remove in the final version !!!!
		//Log("for element: "+ToString(id)+" setting level:"+ToString(level));
		//Log(m_Elements.Get(id).m_Value);
		//PrintString("m_Elements> ");
		//PrintElements();
		//Log(ToString(m_Elements));
		//Log(ToString(id));
		//Log(ToString(m_Elements.Get(id)));
		
		if( m_Elements.Get(id) ) m_Elements.Get(id).m_Value = level;
	}

	void SetBar(int id, int value, int range)
	{
			m_Elements.Get(id).SetBar(value,range);
	}

	void DebugRandValues()
	{
		for(int i = 0; i < m_Elements.Count();i++)
		{
			if(m_Elements.GetElement(i).GetType() == TYPE_BADGE)
			{
				m_Elements.GetElement(i).m_Value = Math.RandomInt(0,2);
			}
			else if(m_Elements.GetElement(i).GetType() == TYPE_BAR)
			{
				m_Elements.GetElement(i).m_Value = Math.RandomInt(0,65);
			}
			else
			{
				m_Elements.GetElement(i).m_Value = Math.RandomInt(0,7);
			}
		}
	}

	void PopulateMaskArray()
	//this will serialize all elements and "compresses" them into integer(s) through bit shifting, these integers are placed into an array
	{
		m_MaskArray.Clear();
		int offset = 0;
		int mask = 0;
		
		for(int i = 0; i < m_Elements.Count(); i++)
		{
			if( (m_Elements.GetElement(i).GetOffset() + offset) > FLOAT_USABLE_SIZE )
			{
				m_MaskArray.Insert(mask);
				offset = 0;
				mask = 0;
			}
			mask = mask | (m_Elements.GetElement(i).GetValue() << offset);
			offset = offset + m_Elements.GetElement(i).GetOffset();
		}
		m_MaskArray.Insert(mask);
	}



	void PopulateElements()//extracts elements from mask
	{
		int maskArrayIndex = 0;
		int offset = 0;
		int mask = 0;
		
		for(int i = 0; i < m_Elements.Count();i++)
		{
			//Log("entity> " + ToString(m_Elements.GetElement(i)) );
			if(offset + m_Elements.GetElement(i).GetOffset() > FLOAT_USABLE_SIZE)
			{
				maskArrayIndex++;
				offset = 0;
			}
			mask = m_MaskArray.Get(maskArrayIndex);
			int value = BitToDec( mask, offset, m_Elements.GetElement(i).GetCompareMask() );
			offset = offset + m_Elements.GetElement(i).GetOffset();
			m_Elements.GetElement(i).SetValue( value );
		}
	}


	int BitToDec(int mask, int index, int compareMask)
	{
		int value = mask & (compareMask << index);
		value = value >> index;
		return value;
	}


	void PrintMasks()
	{
		for(int i = 0; i < m_MaskArray.Count();i++)
		{
			Log("mask"+i.ToString()+">"+m_MaskArray.Get(i).ToString() );
		}
	}

	void PrintElements()
	{
		for(int i = 0; i < m_Elements.Count();i++)
		{
			PrintString(i.ToString() +": "+ m_Elements.GetElement(i).m_Value.ToString() );
		}
	}

	void SendRPC()//on Server
	{
		//DebugRandValues();
		//Log("SendRPC called");
		PopulateMaskArray();
		Debug.LogArrayInt(m_MaskArray,"DisplayStatus");
		
		//PrintMasks();
		Param1<int> paramTmp;
		for(int i = 0; i < m_MaskArray.Count();i++)
		{
			//Debug.Log("rpcParams count >" + ToString(rpcParams.Count()));
			//Debug.Log("rpcParams i value >" + ToString(i));
			paramTmp = rpcParams.Get(i);
			paramTmp.param1 = m_MaskArray.Get(i);
		}
		GetGame().RPC(m_Player, RPC_SYNC_DISPLAY_STATUS, rpcParams,m_Player);
	}
	
	void ImmediateUpdate()
	{
		for(int i = 0; i < m_Elements.Count();i++)
		{
			m_Elements.GetElement(i).ImmediateUpdate();
		}
	}
	
	void DisplayPresence()
	{
		if ( hud )
		{
			hud.DisplayPresence();
		}
	}

	void UpdateStatus()
	{
		//Log("UpdateStatus called for entity: "+ToString(m_Player));
		for(int i = 0; i < m_Elements.Count();i++)
		{
			m_Elements.GetElement(i).UpdateStatus();
		}
	}

	void OnRPC(ParamsReadContext ctx)//on Client 			REWORK.V: NUMBER_OF_MASKS should ideally be transmitted with the masks, ie. saved when sending, and then read on receive as first item
	{
		//Log("OnRPC called");
		m_MaskArray.Clear();
		for(int i = 0; i < NUMBER_OF_MASKS;i++)
		{
			ctx.Read(CashedObjectsParams.PARAM1_INT); 
			m_MaskArray.Insert(CashedObjectsParams.PARAM1_INT.param1);
		}
		//Debug.LogArrayInt(m_MaskArray,"DisplayStatus");
		PopulateElements();
		UpdateStatus();
	}


	void Debug()
	{
		Log("debug");
		PluginPlayerStatus 	m_ModulePlayerStatus = GetPlugin(PluginPlayerStatus);
		m_ModulePlayerStatus.DisplayTendency(NTFKEY_HUNGRY, 2);
	}
}