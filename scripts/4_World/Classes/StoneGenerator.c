class StoneGenerator
{	
	private PlayerBase 		m_Player;
	private array<ItemBase> m_StoneArray;
	private float			m_Time;
	private const float		STONEGEN_TICK = 10; //in seconds
	private const float		STONEGEN_MIN_DISTANCE_TO_REPOSITION = 80; //in meters
	private const float		STONEGEN_SPAWN_DISTANCE = 60; //in meters
	private const float		STONEGEN_MAX_SPREAD = 15; //in meters
	private const int		STONEGEN_STONE_AMOUNT = 5;
	
	private array<Man>		m_Players;
	
	
	//Makes sure there is always one small stone near player if he has none in inventory
	void StoneGenerator(PlayerBase player)
	{
		m_Player = player;
		m_StoneArray = new array<ItemBase>;
		m_Players = new array<Man>;
	}
	
	void ~StoneGenerator()
	{
		CleanUp();
	}
		
	void Update(float deltaT)
	{
		m_Time += deltaT;	
		if ( m_Time > STONEGEN_TICK )
		{
			m_Time = 0;
			if ( m_Player )
			{
				if ( m_StoneArray && m_StoneArray.Count() == STONEGEN_STONE_AMOUNT )
				{
					vector player_pos = m_Player.GetPosition();
					for ( int i = 0; i < m_StoneArray.Count(); i++ )
					{
						ItemBase stone = m_StoneArray.Get(i);
						if ( stone && !stone.GetOwner() )
						{
							if ( vector.Distance(stone.GetPosition(),player_pos) > STONEGEN_MIN_DISTANCE_TO_REPOSITION )
							{
								vector pos = GeneratePos(stone);
								GetGame().SnapToGround(pos);
								stone.SetPosition(pos);
							}
						}
						else
						{
							m_StoneArray.Remove(i);
						}
					}
				}
				else
				{
					CreateStone();
				}
			}
		}
	}	
	
	vector GeneratePos(ItemBase stone)
	{
		vector new_pos = stone.GetPosition();
		if ( m_Player )
		{
			vector player_pos = m_Player.GetPosition();
			player_pos[0] = player_pos[0] + Math.RandomInt(-STONEGEN_MAX_SPREAD,STONEGEN_MAX_SPREAD);
			player_pos[2] = player_pos[2] + Math.RandomInt(-STONEGEN_MAX_SPREAD,STONEGEN_MAX_SPREAD);;
			vector player_dir = m_Player.GetDirection()*STONEGEN_SPAWN_DISTANCE;
			vector possible_pos = player_pos+player_dir;	
			if ( !GetGame().SurfaceIsSea(possible_pos[0], possible_pos[2]) && !GetGame().SurfaceIsPond(possible_pos[0], possible_pos[2]) ) 
			{
				autoptr array<Object> excluded_objects = new array<Object>;
				excluded_objects.Insert(stone);
				if( !GetGame().IsBoxColliding(possible_pos, stone.GetOrientation(), "0.25 0.25 0.25", excluded_objects) )
				{
					bool can_translate = true;
					if ( m_Players )
					{
						GetGame().GetPlayers(m_Players);
						if ( m_Players.Count() > 1)
						{
							vector other_player_pos;
							for ( int i = 0; i < m_Players.Count(); i++ )
							{
								other_player_pos = m_Players.Get(i).GetPosition();
								if ( vector.Distance(new_pos,other_player_pos) < STONEGEN_SPAWN_DISTANCE || vector.Distance(possible_pos,other_player_pos) < STONEGEN_SPAWN_DISTANCE )
								{
									can_translate = false;
									break;
								}
							}
						}
					}
					if (can_translate) new_pos = possible_pos;
				}
			}
			//Debug 
			//m_Player.MessageAction("pos vec: " + ToString(player_pos) + " dir vec: " + ToString(m_Player.GetDirection()) + " dv mult: " + ToString(player_dir) + " fin vec: " + ToString(new_pos));
		}
		return new_pos;
	}
	
	void CreateStone()
	{
		if ( m_StoneArray )
		{
			ItemBase stone = GetGame().CreateObject("EN5C_SmallStone","0 0 0", false);
			stone.SetQuantity2(1);
			m_StoneArray.Insert(stone);
			vector pos = GeneratePos(stone);
			GetGame().SnapToGround(pos);
			stone.SetPosition(pos);
		}
	}
	
	void CleanUp()
	{
		if ( GetGame() && m_StoneArray )
		{
			for ( int i = 0; i < m_StoneArray.Count(); i++ )
			{
				ItemBase stone = m_StoneArray.Get(i);
				if ( stone && !stone.GetOwner() ) GetGame().ObjectDelete(stone);
			}
			delete m_StoneArray;
		}
	}
};