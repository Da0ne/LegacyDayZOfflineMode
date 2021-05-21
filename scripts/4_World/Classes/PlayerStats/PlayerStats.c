class PlayerStats
{

	autoptr array<PlayerStatBase> m_PlayerStats;
	autoptr Timer m_SyncTimer;
	Man m_Player;
	string m_System = "Stats"; //debuging tag
	
	autoptr PlayerStat<float> m_Shock;
	autoptr PlayerStat<float> m_Unconscious;
	autoptr PlayerStat<float> m_Temperature;
	autoptr PlayerStat<float> m_HeatComfort;
	autoptr PlayerStat<float> m_HeatIsolation;
	autoptr PlayerStat<float> m_Tremor;
	autoptr PlayerStat<float> m_Wet;
	autoptr PlayerStat<float> m_Energy;
	autoptr PlayerStat<float> m_Water;
	autoptr PlayerStat<float> m_Stomach;
	autoptr PlayerStat<float> m_StomachEnergy;
	autoptr PlayerStat<float> m_StomachWater;
	autoptr PlayerStat<float> m_Diet;
	autoptr PlayerStat<float> m_Stamina;
	autoptr PlayerStat<float> m_MaxStamina;
	autoptr PlayerStat<int>	m_BloodType;

	void PlayerStats(Man player)
	{
		m_PlayerStats 	= new array<PlayerStatBase>;
		m_Player 		= player;
		
		/*	VARIABLES TO BE ADDED
		"feet",
		"restrainedwith", 
		"kzombies",
		"kplayers",	
		"bleedingsources",	
		"myNotifiers",
		"damageArray",
		"musclecramp",
		"penalties",	
		"totalWeight"
	*/
		//											min						max						initial					"name" 				is_synced(obsolete) ?
		m_Shock 			= new PlayerStat<float>	(0,						5000,					0,						"Shock",			true);
		m_Unconscious 		= new PlayerStat<float>	(0,						1,						0,						"Unconscious",		true);
		m_Temperature 		= new PlayerStat<float>	(30,					45,						36.3,					"Temperature",		true);
		m_HeatComfort		= new PlayerStat<float>	(-100,					300,					-20,					"HeatComfort",		true);
		m_HeatIsolation 	= new PlayerStat<float>	(0,						5000,					0,						"HeatIsolation",	true);
		m_Tremor 			= new PlayerStat<float>	(0,						1,						0,						"Tremor",			true);
		m_Wet 				= new PlayerStat<float>	(0,						1,						0,						"Wet",				true);
		m_Energy 			= new PlayerStat<float>	(0,						20000,					20000,					"Energy",			true);
		m_Water 			= new PlayerStat<float>	(0,						5000,					5000,					"Water",			true);
		m_Stomach 			= new PlayerStat<float>	(0,						5000,					0,						"Stomach",			true);
		m_StomachEnergy 	= new PlayerStat<float>	(0,						40000,					0,						"StomachEnergy",	true);
		m_StomachWater		= new PlayerStat<float>	(0,						5000,					0,						"StomachWater",		true);
		m_Diet 				= new PlayerStat<float>	(0,						5000,					2500,					"Diet",				false);
		m_Stamina			= new PlayerStat<float>	(0,						100,					100,					"Stamina",			true);
		m_BloodType			= new PlayerStat<int>	(0,						7,						MiscGameplayFunctions.GenerateBloodType(),		"BloodType",		false);
		
		m_PlayerStats.Insert(m_Shock);
		m_PlayerStats.Insert(m_Unconscious);
		m_PlayerStats.Insert(m_Temperature);
		m_PlayerStats.Insert(m_HeatComfort);
		m_PlayerStats.Insert(m_HeatIsolation);
		m_PlayerStats.Insert(m_Tremor);
		m_PlayerStats.Insert(m_Wet);
		m_PlayerStats.Insert(m_Energy);
		m_PlayerStats.Insert(m_Water);
		m_PlayerStats.Insert(m_Stomach);
		m_PlayerStats.Insert(m_StomachEnergy);
		m_PlayerStats.Insert(m_StomachWater);
		m_PlayerStats.Insert(m_Diet);
		m_PlayerStats.Insert(m_Stamina);
		m_PlayerStats.Insert(m_BloodType);
	}

	array<PlayerStatBase> Get()
	{
		return m_PlayerStats;
	}

	void SaveStats ( ParamsWriteContext ctx )
	{
		for ( int i = 0; i < m_PlayerStats.Count(); i++ )
		{
			m_PlayerStats.Get(i).OnStoreSave(ctx);
		}
	}

	void LoadStats ( ParamsReadContext ctx )
	{
		for ( int i = 0; i < m_PlayerStats.Count(); i++ )
		{
			m_PlayerStats.Get(i).OnStoreLoad(ctx);
		}
	}

}