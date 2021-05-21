class PluginExperience extends PluginBase
{
	const int EXP_CATEGORY_MEDIC				= 100;
	const int EXP_MEDIC_BANDAGING				= 101;
	const int EXP_MEDIC_FIXATING				= 102;
	const int EXP_MEDIC_TRANSFUSION_TEST		= 103;
	const int EXP_MEDIC_TRANSFUSION_COLLECT		= 104;
	const int EXP_MEDIC_TRANSFUSION_GIVE		= 105;
	const int EXP_MEDIC_SALINE_GIVE				= 106;
	
	const int EXP_CATEGORY_HUNTER				= 200;
	const int EXP_HUNTER_SKINNING				= 201;
	
	const int EXP_CATEGORY_TAILOR				= 300;
	const int EXP_TAILOR_REPAIRING				= 301;
	
	const int EXP_CATEGORY_COOK					= 400;
	
	const int EXP_CATEGORY_MECHANIC				= 500;
	
	const int EXP_CATEGORY_FARMER				= 600;
	const int EXP_FARMER_SOIL_PREPARATION		= 601;
	const int EXP_FARMER_FERTILIZATION			= 602;
	const int EXP_FARMER_HARVESTING				= 603;
	
	const int EXP_CATEGORY_BUILDER				= 700;
	
	float m_AntiGrind0[7];
	float m_AntiGrind1[7];
	float m_AntiGrind2[7];
	float m_AntiGrind3[7];
	float m_AntiGrind4[7];
	float m_AntiGrind5[7];
	float m_AntiGrind6[7];
	
	autoptr map<int, string>		m_ExpAccessName;
	autoptr map<int, Experience>	m_Experiences;		// int - exp type / Experience - experience class
	
	// PROTOTYPES
	
	void PluginExperience()
	{
		float anti_grind0[7] = { 0    ,  0.100,  0.143,  0.143,  0.333,  0.333,  0.333  }; m_AntiGrind0 = anti_grind0;
		float anti_grind1[7] = { 0.100,  0    ,  0.100,  0.100,  0.333,  0.200,  0.143  }; m_AntiGrind1 = anti_grind1;
		float anti_grind2[7] = { 0.143,  0.100,  0    ,  0.200,  0.333,  0.333,  0.200  }; m_AntiGrind2 = anti_grind2;
		float anti_grind3[7] = { 0.143,  0.100,  0.200,  0    ,  0.200,  0.143,  0.333  }; m_AntiGrind3 = anti_grind3;
		float anti_grind4[7] = { 0.333,  0.333,  0.333,  0.200,  0    ,  0.200,  0.100  }; m_AntiGrind4 = anti_grind4;
		float anti_grind5[7] = { 0.333,  0.200,  0.333,  0.143,  0.200,  0    ,  0.143  }; m_AntiGrind5 = anti_grind5;
		float anti_grind6[7] = { 0.333,  0.143,  0.200,  0.333,  0.100,  0.143,  0      }; m_AntiGrind6 = anti_grind6;

		// Map of class names for every experience
		m_ExpAccessName		= new map<int, string>;
		m_Experiences 		= new map<int, Experience>;

		// These are class names used in cfgSoftSkills.cpp and also access strings for <player.GetVariable(...)>
		// Make sure to register new skills here!
		m_ExpAccessName.Set(EXP_HUNTER_SKINNING, "Hunter Skinning");
		
		m_ExpAccessName.Set(EXP_TAILOR_REPAIRING, "Tailor Repairing");
		
		m_ExpAccessName.Set(EXP_MEDIC_BANDAGING, "Medic Bandaging");
		m_ExpAccessName.Set(EXP_MEDIC_FIXATING, "Medic Fixating");
		m_ExpAccessName.Set(EXP_MEDIC_TRANSFUSION_TEST, "Medic TransfusionTest");
		m_ExpAccessName.Set(EXP_MEDIC_TRANSFUSION_COLLECT, "Medic TransfusionCollect");
		m_ExpAccessName.Set(EXP_MEDIC_TRANSFUSION_GIVE, "Medic TransfusionGive");
		m_ExpAccessName.Set(EXP_MEDIC_SALINE_GIVE, "Medic SalineGive");
		
		m_ExpAccessName.Set(EXP_FARMER_SOIL_PREPARATION, "Farmer SoilPreparation");
		m_ExpAccessName.Set(EXP_FARMER_FERTILIZATION, "Farmer Fertilization");
		m_ExpAccessName.Set(EXP_FARMER_HARVESTING, "Farmer Harvesting");
		
		LoadFromCfg();
	}

	void ~PluginExperience()
	{
		for ( int i = 0; i < m_Experiences.Count(); i++ )
		{
			Experience exp = m_Experiences.GetElement( i );
			
			for ( int j = 0; j < exp.m_Levels.Count(); j++ )
			{
				delete exp.m_Levels.GetElement( j );
			}
			
			delete exp;
		}
	}
	
	// Preload cfgSoftSkills.cpp into memory (m_Experiences) for faster read/write
	void LoadFromCfg()
	{
		
		string cfg_access = "CfgExperiences";
		int cfg_exp_count = g_Game.ConfigGetChildrenCount(cfg_access);

		for ( int i = 0; i < cfg_exp_count; ++i )
		{
			string cfg_exp_profession_name = "";
			GetGame().ConfigGetChildName(cfg_access, i, /*out*/ cfg_exp_profession_name);
			string cfg_profession_access = cfg_access + " " + cfg_exp_profession_name;

			int cfg_profession_count = g_Game.ConfigGetChildrenCount(cfg_profession_access);
			
			for ( int c = 0; c < cfg_profession_count; c++ )
			{
				// Inside of cfgExperience. Here we can access all exp classes
				string cfg_exp_class_name = "";
				GetGame().ConfigGetChildName(cfg_profession_access, c, /*out*/ cfg_exp_class_name);

				Experience exp_class = new Experience;
				int exp_type = GetExpTypeByName(cfg_exp_profession_name + " " + cfg_exp_class_name);
				m_Experiences.Set(exp_type, exp_class);
				string cfg_exp_access = cfg_profession_access + " " + cfg_exp_class_name;

				int cfg_exp_lvl_count = g_Game.ConfigGetChildrenCount(cfg_exp_access);

				for ( int j = 0; j < cfg_exp_lvl_count; ++j )
				{
					// Inside of cfgExperience >> cfg_exp_class_name. Here we can access all levels for this experience.
					string cfg_exp_level_class_name;
					GetGame().ConfigGetChildName(cfg_exp_access, j, /*out*/ cfg_exp_level_class_name);

					string cfg_exp_lvl_access = cfg_exp_access + " " + cfg_exp_level_class_name;

					ExperienceLevel exp_lvl = new ExperienceLevel;
					exp_class.AddLevel(cfg_exp_level_class_name, exp_lvl);

					int cfg_exp_lvl_mbrs_count = g_Game.ConfigGetChildrenCount(cfg_exp_lvl_access);

					for ( int k = 0; k < cfg_exp_lvl_mbrs_count; ++k )
					{
						// Inside of cfgExperience >> cfg_exp_class_name >> cfg_exp_lvl_mbrs_count. Here we can access all parameters.
						string cfg_exp_level_member_name;
						GetGame().ConfigGetChildName(cfg_exp_lvl_access, k, /*out*/ cfg_exp_level_member_name);

						string cfg_exp_lvl_member_access = cfg_exp_lvl_access + " " + cfg_exp_level_member_name;

						if ( cfg_exp_level_member_name == "requiredExp" )
						{
							// Mandatory parameter in config
							int exp_req = GetGame().ConfigGetInt(cfg_exp_lvl_member_access);
							exp_lvl.SetRequiredExp(exp_req);
						}
						else
						{
							int type = GetGame().ConfigGetType(cfg_exp_lvl_member_access);

							switch(type)
							{
								case CT_INT: 
									int int_param = GetGame().ConfigGetInt(cfg_exp_lvl_member_access);
									exp_lvl.AddParamNumber(cfg_exp_level_member_name, int_param);
								break;
								
								case CT_FLOAT:
									float float_param = GetGame().ConfigGetFloat(cfg_exp_lvl_member_access);
									exp_lvl.AddParamNumber(cfg_exp_level_member_name, float_param);
								break;

								case CT_STRING:
									string string_param = "";
									GetGame().ConfigGetText(cfg_exp_lvl_member_access, /*out*/ string_param);
									exp_lvl.AddParamString(cfg_exp_level_member_name, string_param);
								break;
							}
						}
					}

					if ( exp_lvl.GetRequiredExp() == -1 )
					{
						string warning_message = "Error! Number parameter requiredExp not found in <" + cfg_exp_access + " " + cfg_exp_level_class_name + ">! Add it!";
						DPrint(warning_message);
					}
				}
			}
		}
	}
	
	void Store( PlayerBase player, int exp_id, int exp_value )
	{
		player.SetSkill( GetExperienceCategory(exp_id), exp_value );
	}

	int GetExperienceCategory( int exp_id )
	{
		int exp_category = exp_id / 100;
		return exp_category * 100;
	}
	
	void DoAntiGrindDecrease( PlayerBase player, int current_category, float increase_value )
	{
		if ( current_category != EXP_CATEGORY_MEDIC ) Decrease( player, EXP_CATEGORY_MEDIC, increase_value * GetAntiGrindDecrease(current_category, EXP_CATEGORY_MEDIC) );
		if ( current_category != EXP_CATEGORY_HUNTER ) Decrease( player, EXP_CATEGORY_HUNTER, increase_value * GetAntiGrindDecrease(current_category, EXP_CATEGORY_HUNTER) );
		if ( current_category != EXP_CATEGORY_TAILOR ) Decrease( player, EXP_CATEGORY_TAILOR, increase_value * GetAntiGrindDecrease(current_category, EXP_CATEGORY_TAILOR) );
		if ( current_category != EXP_CATEGORY_COOK ) Decrease( player, EXP_CATEGORY_COOK, increase_value * GetAntiGrindDecrease(current_category, EXP_CATEGORY_COOK) );
		if ( current_category != EXP_CATEGORY_MECHANIC ) Decrease( player, EXP_CATEGORY_MECHANIC, increase_value * GetAntiGrindDecrease(current_category, EXP_CATEGORY_MECHANIC) );
		if ( current_category != EXP_CATEGORY_FARMER ) Decrease( player, EXP_CATEGORY_FARMER, increase_value * GetAntiGrindDecrease(current_category, EXP_CATEGORY_FARMER) );
		if ( current_category != EXP_CATEGORY_BUILDER ) Decrease( player, EXP_CATEGORY_BUILDER, increase_value * GetAntiGrindDecrease(current_category, EXP_CATEGORY_BUILDER) );
	}
	
	float GetAntiGrindDecrease( int category1, int category2 )
	{
		float anti_grind[7];
		
		if ( category1 == EXP_CATEGORY_MEDIC || category2 == EXP_CATEGORY_MEDIC )
		{
			if ( category2 == EXP_CATEGORY_MEDIC ) category2 = category1;
			anti_grind = m_AntiGrind0;
		}
		else if ( category1 == EXP_CATEGORY_HUNTER || category2 == EXP_CATEGORY_HUNTER )
		{
			if ( category2 == EXP_CATEGORY_HUNTER ) category2 = category1;
			anti_grind = m_AntiGrind1;
		}
		else if ( category1 == EXP_CATEGORY_TAILOR || category2 == EXP_CATEGORY_TAILOR )
		{
			if ( category2 == EXP_CATEGORY_TAILOR ) category2 = category1;
			anti_grind = m_AntiGrind2;
		}
		else if ( category1 == EXP_CATEGORY_COOK || category2 == EXP_CATEGORY_COOK )
		{
			if ( category2 == EXP_CATEGORY_COOK ) category2 = category1;
			anti_grind = m_AntiGrind3;
		}
		else if ( category1 == EXP_CATEGORY_MECHANIC || category2 == EXP_CATEGORY_MECHANIC )
		{
			if ( category2 == EXP_CATEGORY_MECHANIC ) category2 = category1;
			anti_grind = m_AntiGrind4;
		}
		else if ( category1 == EXP_CATEGORY_FARMER || category2 == EXP_CATEGORY_FARMER )
		{
			if ( category2 == EXP_CATEGORY_FARMER ) category2 = category1;
			anti_grind = m_AntiGrind5;
		}
		else if ( category1 == EXP_CATEGORY_BUILDER || category2 == EXP_CATEGORY_BUILDER )
		{
			if ( category2 == EXP_CATEGORY_BUILDER ) category2 = category1;
			anti_grind = m_AntiGrind6;
		}
		else
		{
			return 0;
		}
		
		category2 = (category2 / 100) - 1;

		return anti_grind[category2];
	}
	
	int GetExpTypeByName(string exp_name)
	{
		int exp_type = m_ExpAccessName.GetKeyByValue( exp_name );

		return exp_type;
	}
	
	/**
	\brief Returns exp points for that skill.
		\param player User's character.
		\param exp_id Experience ID (specific constant)
		\returns \p int Number of skill points for that experience type
		@code
			PluginExperience module_exp = (PluginExperience)GetPlugin(PluginExperience);
			int exp_value = module_exp.GetExpPoints(module_exp.EXP_HUNTER_SKINNING);
			Print(exp_value);
			
			>> exp_value = 0
		@endcode
	*/
	float GetExpPoints(PlayerBase player, int exp_id) // To DO: Rename!
	{
		float exp_points = player.GetSkill( GetExperienceCategory(exp_id) );

		if ( exp_points > 0.0 )
		{
			return exp_points;
		}
		else
		{
			// Value was not found. Register it!
			Store( player, exp_id, 0.0 );
			return 0;
		}
	}

	/**
	\brief Increases experience for the given skill by +1.
		\param player User's character.
		\param exp_id Experience ID (specific constant)
		\returns \p Increased experience value
		@code
			PluginExperience module_exp = (PluginExperience)GetPlugin(PluginExperience);
			module_exp.Increase(player, module_exp.EXP_HUNTER_SKINNING);
		@endcode
	*/
	float Increase(PlayerBase player, int exp_id, float increase_value = 1)
	{
		DoAntiGrindDecrease( player, GetExperienceCategory(exp_id), increase_value );
		
		float exp_value = GetExpPoints( player, exp_id );
		exp_value += increase_value;
		Store( player, exp_id, exp_value );
		return exp_value;
	}
	
	float Decrease(PlayerBase player, int exp_id, float decrease_value = -1)
	{
		float exp_value = GetExpPoints( player, exp_id );
		exp_value -= decrease_value;
		if ( exp_value < 0.0 ) exp_value = 0.0;
		Store( player, exp_id, exp_value );
		return exp_value;
	}
	
	/**
	\brief Converts experience value into levels for the given skill.
		\param player User's character.
		\param exp_id Experience ID (specific constant)
		\returns \p Increased experience value
		@code
			PluginExperience module_exp = (PluginExperience)GetPlugin(PluginExperience);
			module_exp.Increase(player, module_exp.EXP_HUNTER_SKINNING);
		@endcode
	*/
	int GetLevel(float exp_value, int exp_id)
	{
		int level = 0;
		string cfg_exp_name = m_ExpAccessName.Get(exp_id);
		string cfg_exp_class = "CfgExperiences " + cfg_exp_name + " ";
		int levels_count = g_Game.ConfigGetChildrenCount(cfg_exp_class);
		autoptr TIntArray required_exp_arr = new TIntArray; // For remembering level requirements
		int i_level = 0;

		for ( i_level = 0; i_level < levels_count; i_level++ )
		{
			string member_name;
			GetGame().ConfigGetChildName( cfg_exp_class, i_level, member_name );
			int type = GetGame().ConfigGetType( String(cfg_exp_class + member_name) );
			if (type == CT_CLASS) // Check if the member is some class
			{
				string class_name = "Level" + i_level.ToString();
				string cfg_path = cfg_exp_class + class_name + " " + "requiredExp";
				int required_exp = g_Game.ConfigGetInt( cfg_path );
				
				required_exp_arr.Insert( required_exp );
			}
		}

		int array_count = required_exp_arr.Count();
		level = array_count - 1;
		i_level = 0;
		for ( i_level = 0; i_level < array_count; i_level++ )
		{
			int vi = required_exp_arr.Get( i_level );
			float v = (float)vi;
			if ( v <= exp_value )
			{
				level = i_level;
			}
			else
			{
				break;
			}
		}

		return level;
	}
	
	float GetExpParamNumber(PlayerBase player, int exp_type, string param)
	{
		float exp_value = GetExpPoints( player, exp_type );
		int skillLevel = GetLevel( exp_value, exp_type );
		string level_name = "level" + skillLevel.ToString();

		Experience exp = m_Experiences.Get( exp_type );
		ExperienceLevel exp_lvl = exp.m_Levels.Get( level_name );
		
		return exp_lvl.GetParamNumber( param );
	}

	string GetExpParamString(PlayerBase player, int exp_type, string param)
	{
		float exp_value = GetExpPoints( player, exp_type );
		int skillLevel = GetLevel( exp_value, exp_type );
		string level_name = "level" + skillLevel.ToString();

		Experience exp = m_Experiences.Get( exp_type );
		ExperienceLevel exp_lvl = exp.m_Levels.Get( level_name );
		string return_val = exp_lvl.GetParamString( param );
		return return_val;
	}
	
	float GetTimeToCompleteEfficiency( PlayerBase player, int action_type )
	{
		if ( GetGame().IsDebug() )
		{	
			if ( action_type == AT_BANDAGE_S || action_type == AT_BANDAGE_T )
			{
				return GetExpParamNumber(player, EXP_MEDIC_BANDAGING, "efficiency");
			}
			else if ( action_type == AT_SPLINT_S || action_type == AT_SPLINT_T )
			{
				return GetExpParamNumber(player, EXP_MEDIC_FIXATING, "efficiency");
			}
			else if ( action_type == AT_TEST_BLOOD_S || action_type == AT_TEST_BLOOD_T )
			{
				return GetExpParamNumber(player, EXP_MEDIC_TRANSFUSION_TEST, "efficiency");
			}
			else if ( action_type == AT_COLLECT_BLOOD_T || action_type == AT_COLLECT_BLOOD_S )
			{
				return GetExpParamNumber(player, EXP_MEDIC_TRANSFUSION_COLLECT, "efficiency");
			}
			else
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}
	}

	float GetQuantityEfficiency( PlayerBase player, int action_type )
	{
		if ( GetGame().IsDebug() )
		{	
			if ( action_type == AT_GIVE_BLOOD_S || action_type == AT_GIVE_BLOOD_T )
			{
				return GetExpParamNumber(player, EXP_MEDIC_TRANSFUSION_GIVE, "efficiency");
			}
			else if ( action_type == AT_GIVE_SALINE_S || action_type == AT_GIVE_SALINE_T )
			{
				return GetExpParamNumber(player, EXP_MEDIC_SALINE_GIVE, "efficiency");
			}
			else 
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}
	}
}
