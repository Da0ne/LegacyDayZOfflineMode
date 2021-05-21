class StaminaHandler
{	
	protected int 							m_PlayerSpeed;
	protected int 							m_PlayerStance;
	protected float 						m_PlayerLoad;
	protected float 						m_StaminaDelta;
	protected float 						m_Stamina = 0; 
	protected float 						m_StaminaCap = 100;
	protected float 						m_Time;
	protected autoptr Param2<float,float>	m_StaminaParams; 
	protected autoptr HumanMovementState	m_State;
	protected PlayerBase					m_Player;
	
	void StaminaHandler(PlayerBase player)
	{
		if ( GetGame().IsServer() && GetGame().IsMultiplayer() ) m_StaminaParams = new Param2<float,float>(0,0);		
		m_Player = player;
		m_State = new HumanMovementState();
		m_Stamina = 0; 
		m_StaminaCap = 100;
		m_Time = 0;
	}
	
	void Update(float deltaT)
	{
		if ( m_Player )
		{
			// Calculates actual max stamina based on player's load
			m_PlayerLoad = m_Player.GetPlayerLoad();
			if ( GetGame().IsServer() && GetGame().IsMultiplayer() )
			{
				m_Stamina = m_Player.m_PlayerStats.m_Stamina.Get();	
				m_StaminaCap =  Math.Max((m_Player.m_PlayerStats.m_Stamina.GetMax() - ((m_PlayerLoad/STAMINA_KG_TO_GRAMS) * STAMINA_KG_TO_STAMINAPERCENT_PENALTY )),STAMINA_MIN_CAP);
			}		
			
			m_StaminaCap =  Math.Max((100 - ((m_PlayerLoad/STAMINA_KG_TO_GRAMS) * STAMINA_KG_TO_STAMINAPERCENT_PENALTY )),STAMINA_MIN_CAP); //jtomasik - tohle pak smaz
			
			// Calculates stamina gain/loss based on movement and load
			m_Player.GetMovementState(m_State);
			m_PlayerSpeed = m_State.m_iMovement;
			m_PlayerStance = m_State.m_iStanceIdx;
			switch ( m_PlayerSpeed )
			{
				case 3/* DayZPlayerConstants.MOVEMENTIDX_SPRINT */: //sprint
					m_StaminaDelta = STAMINA_GAIN_IDLE_PER_SEC;
					if ( m_PlayerStance == DayZPlayerConstants.STANCEIDX_ERECT  || m_PlayerStance == DayZPlayerConstants.STANCEIDX_RAISEDERECT)  m_StaminaDelta = -STAMINA_DRAIN_STANDING_SPRINT_PER_SEC; 
					if ( m_PlayerStance == DayZPlayerConstants.STANCEIDX_CROUCH || m_PlayerStance == DayZPlayerConstants.STANCEIDX_RAISEDCROUCH) m_StaminaDelta = -STAMINA_DRAIN_CROUCHED_SPRINT_PER_SEC;
					if ( m_PlayerStance == DayZPlayerConstants.STANCEIDX_PRONE  || m_PlayerStance == DayZPlayerConstants.STANCEIDX_RAISEDPRONE)  m_StaminaDelta = -STAMINA_DRAIN_PRONE_SPRINT_PER_SEC;
					break;
					
				case 2/* DayZPlayerConstants.MOVEMENTIDX_RUN */: //jog
					m_StaminaDelta = (STAMINA_GAIN_JOG_PER_SEC + GetStaminaGainBonus());
					break;
					
				case 1/* DayZPlayerConstants.MOVEMENTIDX_WALK */: //walk
					m_StaminaDelta = (STAMINA_GAIN_WALK_PER_SEC + GetStaminaGainBonus());
					break;
					
				case 0/* DayZPlayerConstants.MOVEMENTIDX_IDLE */: //idle
					m_StaminaDelta = (STAMINA_GAIN_IDLE_PER_SEC + GetStaminaGainBonus());
					break;
					
				default:
					m_StaminaDelta = STAMINA_GAIN_IDLE_PER_SEC;
					break;
			}

			//Sets current stamina & stores + syncs data with client
			m_Stamina = Math.Max(0,Math.Min((m_Stamina + m_StaminaDelta*deltaT),m_StaminaCap));
			if ( GetGame().IsServer() && GetGame().IsMultiplayer() )	
			{
				m_Time += deltaT;
				if ( m_StaminaParams && m_Time >= STAMINA_SYNC_RATE )
				{
					m_Time = 0;
					m_Player.m_PlayerStats.m_Stamina.Set(m_Stamina);
					m_StaminaParams.param1 = m_Stamina;
					m_StaminaParams.param2 = m_StaminaCap;
					GetGame().RPCSingleParam(m_Player, RPC_STAMINA, m_StaminaParams, m_Player);
				}
			}
			else
			{
				DisplayStamina();
			}

			//debug
			//m_Player.MessageStatus(ToString(player) + " stamina: " + ToString(Math.Floor(m_Stamina)) + "  delta: w" + ToString(Math.Floor(m_StaminaDelta))  + "  cap: " + ToString(m_StaminaCap));
		}
	}
	
	void SyncStamina(float stamina, float stamina_cap) //Is called from RPC on playerbase.c syncs stamina values on server with client
	{
		m_Stamina = stamina;
		m_StaminaCap = stamina_cap;
	}
	
	bool EnoughStaminaToSprint() // Player can sprint if at least treshold of stamina is regained
	{
		if ( m_Stamina >= STAMINA_SPRINT_TRESHOLD ) return true; 
		return false;
	}
	
	float GetStaminaGainBonus()// Calulates stamina regain bonus based on current stamina level (So it's better to wait for stamina to refill completely and avoid overloading)
	{
		return Math.Min((m_Stamina/10),STAMINA_GAIN_BONUS_CAP);
	}
	
	void DisplayStamina()
	{
		if ( m_Player ) m_Player.GetDisplayStatus().SetBar(DELM_BAR_STAMINA,m_Stamina,m_StaminaCap);
	}
};