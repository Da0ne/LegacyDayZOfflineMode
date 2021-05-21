class PluginLifespan extends PluginBase
{
	private static const int LIFESPAN_MIN = 0;
	private static const int LIFESPAN_MAX = 5; // value in minutes when player achieved maximum age in order to have full beard
	
	private static const int HANDS_OBJECT_SLOT = 7; // hands object slot index for setting and unsetting bloody hands
	
	private autoptr map<PlayerBase, LifespanLevel> m_PlayerCurrentLevel;
	private autoptr map<string, array<LifespanLevel>> m_LifespanLevels;
	private autoptr map<string, BloodyHands> m_BloodyHands;

	void PluginLifespan()
	{
		m_PlayerCurrentLevel = new map<PlayerBase, LifespanLevel>;
		LoadFromCfg();
	}

	void ~PluginLifespan()
	{
		int i;
		
		for ( i = 0; i < m_LifespanLevels.Count(); i++ )
		{
			array<LifespanLevel> levels = m_LifespanLevels.GetElement( i );
			for ( int j = 0; j < levels.Count(); j++ )
			{
				delete levels.Get( j );
			}
			delete levels;
		}
		
		for ( i = 0; i < m_BloodyHands.Count(); i++ )
		{
			BloodyHands bloody_hands = m_BloodyHands.GetElement( i );
			delete bloody_hands;
		}
	}
	
	void LoadFromCfg()
	{
		m_LifespanLevels = new map<string, array<LifespanLevel>>;
		m_BloodyHands = new map<string, BloodyHands>;
		
		const int LIFESPAN_LENGTH = LIFESPAN_MAX - LIFESPAN_MIN;

		string cfg_name = "CfgVehicles";
		int cfg_class_count = GetGame().ConfigGetChildrenCount( cfg_name );
		
		int i, j, k, l, m;
		for ( i = 0; i < cfg_class_count; i++ )
		{
			string cfg_class_name = "";
			GetGame().ConfigGetChildName( cfg_name, i, cfg_class_name );
			
			if ( cfg_class_name != "" && cfg_class_name != "access" )
			{
				if ( GetGame().IsKindOf(cfg_class_name, "SurvivorMale") || GetGame().IsKindOf(cfg_class_name, "SurvivorFemale") )
				{
					string cfg_class_fullname = cfg_name + " " + cfg_class_name;
					int cfg_class_member_count = GetGame().ConfigGetChildrenCount( cfg_class_fullname );

					for ( j = 0; j < cfg_class_member_count; j++ )
					{
						string cfg_class_member_name = "";
						GetGame().ConfigGetChildName( cfg_class_fullname, j, cfg_class_member_name );
						
						string cfg_class_member_fullname = cfg_class_fullname + " " + cfg_class_member_name;

						if ( cfg_class_member_name == "Lifespan" )
						{
							int cfg_class_member_member_count = GetGame().ConfigGetChildrenCount( cfg_class_member_fullname );
							
							for ( k = 0; k < cfg_class_member_member_count; k++ )
							{
								string cfg_class_member_member_name = "";
								GetGame().ConfigGetChildName( cfg_class_member_fullname, k, cfg_class_member_member_name );
								
								string cfg_class_member_member_fullname = cfg_class_member_fullname + " " + cfg_class_member_member_name;
								
								//Print( cfg_class_member_member_name );
								
								if ( cfg_class_member_member_name == "Beard" )
								{
									//Print( cfg_class_name );
									
									autoptr TStringArray materials = new TStringArray;
									int cfg_class_member_member_variable_count = GetGame().ConfigGetChildrenCount( cfg_class_member_member_fullname );
									//Print( cfg_class_member_member_variable_count );
									
									for ( l = 0; l < cfg_class_member_member_variable_count; l++ )
									{
										string cfg_class_member_member_variable_name = "";
										GetGame().ConfigGetChildName( cfg_class_member_member_fullname, l, cfg_class_member_member_variable_name );
										string cfg_class_member_member_variable_fullname = cfg_class_member_member_fullname + " " + cfg_class_member_member_variable_name;
										
										if ( cfg_class_member_member_variable_name == "mat" )
										{
											GetGame().ConfigGetTextArray( cfg_class_member_member_variable_fullname, materials );

											array<LifespanLevel> lifespan_levels = new array<LifespanLevel>;

											int level_count = materials.Count() / 2;
											for ( m = 0; m < level_count; m++ )
											{
												int tex = m*2;
												int mat = ((m*2)+1);
												if ( mat < materials.Count() )
												{
													float threshold = (((float)m / (float)(level_count - 1)) * (float)LIFESPAN_LENGTH) + (float)LIFESPAN_MIN;
													lifespan_levels.Insert( new LifespanLevel(m, threshold, materials.Get(tex), materials.Get(mat)) );
												}
											}
											
											if ( lifespan_levels.Count() > 0 )
											{
												m_LifespanLevels.Set( cfg_class_name, lifespan_levels );
											}
											else 
											{
												delete lifespan_levels;
											}
										}
									}
								}	
							}
						}
						else if ( cfg_class_member_name == "Skinning" )
						{
							int cfg_skin_class_member_member_count = GetGame().ConfigGetChildrenCount( cfg_class_member_fullname );
							
							for ( k = 0; k < cfg_skin_class_member_member_count; k++ )
							{
								string cfg_skin_class_member_member_name = "";
								GetGame().ConfigGetChildName( cfg_class_member_fullname, k, cfg_skin_class_member_member_name );
								
								string cfg_skin_class_member_member_fullname = cfg_class_member_fullname + " " + cfg_skin_class_member_member_name;

								if ( cfg_skin_class_member_member_name == "BloodyHands" )
								{
									autoptr TStringArray skin_materials = new TStringArray;
									int cfg_skin_class_member_member_variable_count = GetGame().ConfigGetChildrenCount( cfg_skin_class_member_member_fullname );
									
									string bloody_material = "";
									string normal_material = "";
									
									for ( l = 0; l < cfg_skin_class_member_member_variable_count; l++ )
									{
										string cfg_skin_class_member_member_variable_name = "";
										GetGame().ConfigGetChildName( cfg_skin_class_member_member_fullname, l, cfg_skin_class_member_member_variable_name );
										string cfg_skin_class_member_member_variable_fullname = cfg_skin_class_member_member_fullname + " " + cfg_skin_class_member_member_variable_name;
										
										if ( cfg_skin_class_member_member_variable_name == "mat_normal" )
										{
											normal_material = GetGame().ConfigGetTextOut( cfg_skin_class_member_member_variable_fullname );
										}
										else if ( cfg_skin_class_member_member_variable_name == "mat_blood" )
										{
											bloody_material = GetGame().ConfigGetTextOut( cfg_skin_class_member_member_variable_fullname );
										}
									}
									
									m_BloodyHands.Set( cfg_class_name, new BloodyHands(normal_material, bloody_material) );
								}
							}
						}
					}
				}
			}
		}
		
		/*for ( int cl = 0; cl < m_LifespanLevels.Count(); cl++ )
		{
			string class_name = m_LifespanLevels.GetKeyByIndex( cl );
			array<LifespanLevel> levels = m_LifespanLevels.GetByIndex( cl );

			Print( class_name );

			for ( int ll = 0; ll < levels.Count(); ll++)
			{
				Print( levels.Get( ll ).GetLevel() );
				Print( levels.Get( ll ).GetThreshold() );
				Print( levels.Get( ll ).GetTextureName() );
				Print( levels.Get( ll ).GetMaterialName() );
			}
		}*/
	}

	void UpdateLifespanLevelAuto( PlayerBase player, bool force_update = false )
	{
		if ( player != NULL )
		{
			
			// NEW STATS API
			float player_playtime = player.StatGet("playtime");
			float player_cached_playtime = player.GetCachedPlaytime();
			
			if ( player_cached_playtime > 0 && player_cached_playtime > player_playtime)
			{
				player_playtime = player_cached_playtime;
			}
			else 
			{
				player.SetCachedPlaytime( player_playtime );
			}
			
			float player_beard = player_playtime - player.GetLastShavedSeconds();
			player_beard = player_beard / 60.0;
			
			UpdateLifespanLevel( player, player_beard, force_update );
		}
	} 

	void UpdateLifespanLevelParam( PlayerBase player, string hours_survived, bool force_update = false )
	{
		if ( player != NULL )
		{
			autoptr TStringArray splitted = new TStringArray;
			hours_survived.Split( ":", splitted );

			if ( splitted.Count() > 1 )
			{
				int hours = splitted.Get(0).ToInt();
				int minutes = splitted.Get(1).ToInt();
				
				float player_playtime = ((hours * 60.0) + minutes) * 60.0;
				float player_cached_playtime = player.GetCachedPlaytime();
				
				if ( player_cached_playtime > 0 && player_cached_playtime > player_playtime)
				{
					player_playtime = player_cached_playtime;
				}
				else 
				{
					player.SetCachedPlaytime( player_playtime );
				}
				
				float player_beard = player_playtime - player.GetLastShavedSeconds();
				player_beard = player_beard / 60.0;
				
				UpdateLifespanLevel( player, player_beard, force_update );
			}
		}
	}

	private void UpdateLifespanLevel( PlayerBase player, float player_beard, bool force_update = false )
	{
		// Lifespan updates only for developer
		if ( GetGame().IsDebug() ) 
		{
			if ( m_PlayerCurrentLevel.Contains(player) )
			{
				LifespanLevel current_level = m_PlayerCurrentLevel.Get( player );

				if ( player_beard > current_level.GetThreshold() || force_update )
				{
					LifespanLevel next_level = GetLifespanLevel( player.GetPlayerClass(), player_beard );

					if ( next_level != NULL )
					{
						SetPlayerLifespanLevel( player, next_level );
						m_PlayerCurrentLevel.Set( player, next_level );
					}
				}
			}
			else
			{
				if ( m_LifespanLevels.Contains( player.GetPlayerClass() ) )
				{
					LifespanLevel level = GetLifespanLevel( player.GetPlayerClass(), player_beard );

					if ( level != NULL )
					{
						SetPlayerLifespanLevel( player, level );
						m_PlayerCurrentLevel.Set( player, level );
					}
				}
			}
		}
	}

	void SetBloodyHands( PlayerBase player )
	{
		SetHandsMaterial( player, BloodyHands.MATERIAL_TYPE_BLOODY );
		player.SetHasBloodyHands( true );
	}

	void UnSetBloodyHands( PlayerBase player )
	{
		SetHandsMaterial( player, BloodyHands.MATERIAL_TYPE_NORMAL );
		player.SetHasBloodyHands( false );
	}

	private void SetHandsMaterial( PlayerBase player, int material_type )
	{
		string player_class = player.GetPlayerClass();

		if ( m_BloodyHands.Contains(player_class) )
		{
			EntityAI eai = player.FindPlaceholderForSlot( HANDS_OBJECT_SLOT );
			eai.SetObjectMaterial( 0, m_BloodyHands.Get(player_class).GetMaterial(material_type) );
		}
	}

	private LifespanLevel GetLifespanLevel( string player_class, float age )
	{
		array<LifespanLevel> lifespan_levels = m_LifespanLevels.Get( player_class );
		
		if ( lifespan_levels != NULL )
		{
			for ( int i = lifespan_levels.Count() - 1; i >= 0; i-- )
			{
				LifespanLevel level = lifespan_levels.Get( i );
				
				if ( age >= level.GetThreshold() )
				{
					return level;
				}
			}
			
			if ( lifespan_levels.Count() > 0 )
			{
				return lifespan_levels.Get( 0 );
			}
		}
		
		return NULL;
	}

	private void SetPlayerLifespanLevel( PlayerBase player, LifespanLevel level )
	{
		/*Print( "set level" );
		
		Print( level.GetLevel() );
		Print( level.GetThreshold() );
		Print( level.GetTextureName() );
		Print( level.GetMaterialName() );
		player.MessageStatus( "Level: " + level.GetLevel().ToString() );*/
		
		player.SetFaceTexture( level.GetTextureName() );
		player.SetFaceMaterial( level.GetMaterialName() );
	}
}
