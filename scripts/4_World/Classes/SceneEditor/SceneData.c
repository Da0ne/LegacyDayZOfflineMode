class SceneData
{
	protected string	m_NameScene;
	protected string	m_NameMission;
	protected float 	m_InitTime;
	protected int 		m_DateInitYear;
	protected int		m_DateInitMonth;
	protected int		m_DateInitDay;
	protected int		m_DateInitHour;
	protected int		m_DateInitMinute;
	protected float 	m_WeaterInitOvercast;
	protected float 	m_WeaterInitRain;
	protected float 	m_WeaterInitFog;
	protected float 	m_WeaterInitWindForce;
	
	protected array<SceneObject>	m_Objects;
	protected array<ScenePlayer>	m_Players;
	protected array<SceneObject>	m_AllObjs;
	
	//========================================
	// Constructor -> SceneData
	//========================================
	void SceneData()
	{
		m_Objects = new array<SceneObject>;
		m_Players = new array<ScenePlayer>;
		m_AllObjs = new array<SceneObject>;
	}
	
	//========================================
	// Constructor -> SceneData
	//========================================
	void ~SceneData()
	{
		for ( int i = 0; i < m_Objects.Count(); ++i )
		{
			delete m_Objects.Get(i);
		}
		
		delete m_Objects;
		
		for ( int j = 0; j < m_Players.Count(); ++j )
		{
			delete m_Players.Get(j);
		}
		
		delete m_Players;
	}
	
	//========================================
	// GetSceneObjects
	//========================================
	array<SceneObject> GetSceneObjects()
	{
		return m_Objects;
	}
	
	//========================================
	// GetScenePlayers
	//========================================
	array<ScenePlayer> GetScenePlayers()
	{
		return m_Players;
	}
	
	//========================================
	// GetSceneObjectsAll
	//========================================
	array<SceneObject> GetSceneObjectsAll()
	{
		array<ScenePlayer> scene_players = GetScenePlayers();
		
		m_AllObjs.Clear();		
		m_AllObjs.Copy( GetSceneObjects() );
		
		for ( int x = 0; x < scene_players.Count(); ++x )
		{
			m_AllObjs.InsertAt(scene_players.Get(x), 0);
		}
		
		return m_AllObjs;
	}
	
	//========================================
	// GetNameScene
	//========================================
	string GetNameScene()
	{
		return m_NameScene;
	}
	
	//========================================
	// SetNameScene
	//========================================
	void SetNameScene(string name)
	{
		m_NameScene = name;
	}
	
	//========================================
	// GetNameMission
	//========================================
	string GetNameMission()
	{
		return m_NameMission;
	}
	
	//========================================
	// SetNameMission
	//========================================
	void SetNameMission(string name)
	{
		m_NameMission = name;
	}
	
	//========================================
	// GetInitTime
	//========================================
	float GetInitTime()
	{
		return m_InitTime;
	}
	
	//========================================
	// SetInitTime
	//========================================
	void SetInitTime(float time)
	{
		m_InitTime = time;
	}
	
	
	//========================================
	// GetInitYear
	//========================================
	int GetInitYear()
	{
		return m_DateInitYear;
	}
	
	//========================================
	// SetInitYear
	//========================================
	void SetInitYear(int value)
	{
		m_DateInitYear = value;
		GetGame().GetWorld().SetDate( m_DateInitYear, m_DateInitMonth, m_DateInitDay, m_DateInitHour, m_DateInitMinute );
	}
	
	//========================================
	// GetInitMonth
	//========================================
	int GetInitMonth()
	{
		return m_DateInitMonth;
	}
	
	//========================================
	// SetInitMonth
	//========================================
	void SetInitMonth(int value)
	{
		m_DateInitMonth = value;
		GetGame().GetWorld().SetDate( m_DateInitYear, m_DateInitMonth, m_DateInitDay, m_DateInitHour, m_DateInitMinute );
	}
	
	//========================================
	// GetInitDay
	//========================================
	int GetInitDay()
	{
		return m_DateInitDay;
	}
	
	//========================================
	// SetInitDay
	//========================================
	void SetInitDay(int value)
	{
		m_DateInitDay = value;
		GetGame().GetWorld().SetDate( m_DateInitYear, m_DateInitMonth, m_DateInitDay, m_DateInitHour, m_DateInitMinute );
	}
	
	//========================================
	// GetInitHour
	//========================================
	int GetInitHour()
	{
		return m_DateInitHour;
	}
	
	//========================================
	// SetInitHour
	//========================================
	void SetInitHour(int value)
	{
		m_DateInitHour = value;
		GetGame().GetWorld().SetDate( m_DateInitYear, m_DateInitMonth, m_DateInitDay, m_DateInitHour, m_DateInitMinute );
	}
	
	//========================================
	// GetInitMinute
	//========================================
	int GetInitMinute()
	{
		return m_DateInitMinute;
	}
	
	//========================================
	// SetInitMinute
	//========================================
	void SetInitMinute(int value)
	{
		m_DateInitMinute = value;
		GetGame().GetWorld().SetDate( m_DateInitYear, m_DateInitMonth, m_DateInitDay, m_DateInitHour, m_DateInitMinute );
	}
	
	//========================================
	// GetInitOvercast
	//========================================
	float GetInitOvercast()
	{
		return m_WeaterInitOvercast;
	}
	
	//========================================
	// SetInitOvercast
	//========================================
	void SetInitOvercast(float value)
	{
		m_WeaterInitOvercast = value;
		GetGame().SetOvercast( value, 0, 1000 );
	}
	
	//========================================
	// GetInitRain
	//========================================
	float GetInitRain()
	{
		return m_WeaterInitRain;
	}
	
	//========================================
	// SetInitRain
	//========================================
	void SetInitRain(float value)
	{
		m_WeaterInitRain = value;
		GetGame().SetRain( value, 0, 1000 );
	}
	
	//========================================
	// GetInitFog
	//========================================
	float GetInitFog()
	{
		return m_WeaterInitFog;
	}
	
	//========================================
	// SetInitFog
	//========================================
	void SetInitFog(float value)
	{
		m_WeaterInitFog = value;
		GetGame().SetFog( value, 0, 1000 );
	}
	
	//========================================
	// GetInitWindForce
	//========================================
	float GetInitWindForce()
	{
		return m_WeaterInitWindForce;
	}
	
	//========================================
	// SetInitWindForce
	//========================================
	void SetInitWindForce(float value)
	{
		m_WeaterInitWindForce = value;
		GetGame().SetWindForce( value );
	}
	
	//========================================
	// CreateSceneObject
	//========================================
	SceneObject CreateSceneObject(string obj_name, vector pos)
	{
		SceneObject sc_obj = new SceneObject();
		sc_obj.Init(obj_name, pos);
		
		if ( sc_obj != NULL )
		{
			m_Objects.Insert(sc_obj);
				
			return sc_obj;
		}
		
		return NULL;
	}
	
	//========================================
	// CreateScenePlayer
	//========================================
	ScenePlayer CreateScenePlayer()
	{
		ScenePlayer sc_ply = new ScenePlayer();
		sc_ply.Init(STRING_EMPTY, Vector(0,0,0));
		
		m_Players.Insert(sc_ply);
PrintString("CreateScenePlayer Add => "+ m_Players.Count().ToString() );
		return sc_ply;
	}
	
	//========================================
	// Clear
	//========================================
	SceneObject Clear()
	{
		for ( int i = 0; i < m_Objects.Count(); ++i )
		{
			delete m_Objects.Get(i);
		}
		
		for ( int j = 0; j < m_Players.Count(); ++j )
		{
			delete m_Players.Get(j);
		}
	}
	
	//========================================
	// AddObject
	//========================================
	SceneObject AddObject(EntityAI e)
	{
		SceneObject sc_obj = new SceneObject();
		sc_obj.Init("player", e.GetPosition());
		
		sc_obj.LinkEntityAI( e );
		
		if ( sc_obj.IsPlayer() )
		{
			m_Players.Insert( sc_obj );
		}
		else
		{
			m_Objects.Insert( sc_obj );
		}		
		
		return sc_obj;
	}
	
	//========================================
	// GetSceneObjectByEntityAI
	//========================================
	SceneObject GetSceneObjectByEntityAI(EntityAI e)
	{
		for ( int i = 0; i < m_Players.Count(); ++i )
		{
			EntityAI ply = m_Players.Get(i).GetObject();
			
			if ( ply != NULL && ply == e )
			{
				return m_Players.Get(i);
			}	
		}
		
		for ( int j = 0; j < m_Objects.Count(); ++j )
		{
			EntityAI obj = m_Objects.Get(j).GetObject();
			
			if ( obj != NULL && obj == e )
			{
				return m_Objects.Get(j);
			}	
		}
		
		return NULL;	
	}
	
	//========================================
	// DeleteSceneObject
	//========================================
	void DeleteSceneObject(SceneObject scene_object)
	{
		int index_p = m_Players.Find(scene_object);
		
		if ( index_p != -1 )
		{
			delete m_Players.Get(index_p);
			m_Players.Remove(index_p);
		}
		
		
		int index_o = m_Objects.Find(scene_object);
		
		if ( index_o != -1 )
		{
			delete m_Objects.Get(index_o);
			m_Objects.Remove(index_o);
		}
	}
}