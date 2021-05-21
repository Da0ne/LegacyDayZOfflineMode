
class DayZIntroScene: Managed
{
	string m_lastCharacter;
	int m_LastShavedSeconds;
	int m_CachedPlaytime;
	int m_currentCharacterID;
	autoptr TStringArray m_lastInventory;
	autoptr TStringArray m_characterAnimations;
	autoptr EntityAnimEndEventHandler m_anim_end_event_handler;

	Camera m_camera;
	PlayerBase m_demoUnit;
	vector m_demoPos;
	vector m_demoRot;
	vector m_cameraTrans[4];
	bool m_rotatingCharacter;
	int m_rotatingCharacterMouseX;
	int m_rotatingCharacterMouseY;
	float m_rotatingCharacterRot;

	protected autoptr Timer m_timer;
	protected EntityAI m_entity_to_take;	


	void DayZIntroScene()
	{
		m_timer = new Timer();
		m_currentCharacterID = -1;
		m_demoPos = "0 0 0";
		m_demoRot = "0 0 0";
		m_lastCharacter = "";
		m_LastShavedSeconds = 0;
		m_CachedPlaytime = 0;
		m_lastInventory = new TStringArray;
		m_characterAnimations = new TStringArray;
		m_rotatingCharacter = 0;
		
		int character_to_select = - 1;
		string cached_playtime_str = "";
		g_Game.GetProfileString("cachedPlaytime", cached_playtime_str);
		if ( cached_playtime_str != "" )
		{
			m_CachedPlaytime = cached_playtime_str.ToInt();
		}
		World w = g_Game.GetWorld();
		MenuData data = g_Game.GetMenuData();
		
		character_to_select = data.GetLastPlayedCharacter();
	/*	//NEW
		string cached_selectedCharacter_str = "";
		g_Game.GetProfileString("selectedCharacter", cached_selectedCharacter_str);
	
		if ( cached_selectedCharacter_str == "" )
		{
			if( data.GetCharactersCount() > 0)
			{
				character_to_select = 0;
			}
		}else
		{
			character_to_select = atoi(cached_selectedCharacter_str);
			
			if( data.GetCharactersCount() >= character_to_select)
			{
				character_to_select = data.GetCharactersCount() - 1;
			}
		}
	*/
		string worldName;
		g_Game.GetWorldName(worldName);
	
		string root_path = "cfgCharacterScenes " + worldName;
		
		int count = g_Game.ConfigGetChildrenCount(root_path);
		int index = Math.RandomInt(0, count - 1);
		string childName;
		g_Game.ConfigGetChildName(root_path, index, childName);
	
		string scene_path = root_path + " " + childName;
		vector target = SwapYZ(g_Game.ConfigGetVector(scene_path + " target"));
		vector position = SwapYZ(g_Game.ConfigGetVector(scene_path + " position"));
		TIntArray date = new TIntArray;
		g_Game.ConfigGetIntArray(scene_path + " date", date);
		float fov = g_Game.ConfigGetFloat(scene_path + " fov");
		float aperture = g_Game.ConfigGetFloat(scene_path + " aperture");
		float humidity = g_Game.ConfigGetFloat(scene_path + " humidity");
		float overcast = g_Game.ConfigGetFloat(scene_path + " overcast");
		
		if (humidity == 0) humidity = 0.2;
		if (overcast == 0) overcast = 0.2;
		
		World world = g_Game.GetWorld();
	
		if (world && date.Count() >= 5)
		{
			world.SetDate(date.Get(0), date.Get(1), date.Get(2), date.Get(3), date.Get(4));
			//world.SetWeather(overcast, -1, -1, 0);
			//world.SetAperture(aperture);
			/*
			// todo
			simulSetHumidity _humidity;
			*/
		}
	
		delete date;
		GetGame().ObjectDelete( m_camera );
		m_camera = g_Game.CreateObject("camera", SnapToGround(position), true);
		
		Math3D.MatrixIdentity4(m_cameraTrans);
	
		if (m_camera)
		{
			world.SetCameraEffect(m_camera, "internal", CamEffectBack, true);
			m_camera.PrepareTarget(target);
			m_camera.PreparePos(m_camera.GetPosition());
			m_camera.PrepareFOV(fov);
			m_camera.PrepareFocus(5.0, 1.0);
			m_camera.CommitPrepared(0.0);
			m_camera.PreloadPrepared(1.0);
			
			Math3D.DirectionAndUpMatrix(target - position, "0 1 0", m_cameraTrans);
			m_cameraTrans[3] = m_camera.GetPosition();
			
			m_demoPos = g_Game.SnapToGround(Vector(0.685547, -0.988281, 3.68823).Multiply4(m_cameraTrans));
			m_demoRot = "0 0 0";
			vector to_cam_dir = m_camera.GetPosition() - m_demoPos;
			m_demoRot[0] = Math.Atan2(to_cam_dir[0], to_cam_dir[2]) * Math.RAD2DEG;
		}
	
		
		
		m_anim_end_event_handler = new EntityAnimEndEventHandler("", this, "AnimateCharacter", NULL);
	
		ChangeCharacter(character_to_select);
	}
	
	//--------------------------------------------------------------
	void TryTakeToHands()
	{
		if (!m_entity_to_take)
		{
			m_timer.Stop();
			return;
		}
		
		if (!m_demoUnit) return;		
		
		EntityAI item_in_hands = m_demoUnit.GetEntityInHands();
		
		if (item_in_hands) return; // wait longer
		
		// horay!
		m_demoUnit.TakeEntityToHands(m_entity_to_take);
		InitCharacterAnimations(m_entity_to_take);
		
		m_entity_to_take = NULL;
		m_timer.Stop();
	}
	
	//--------------------------------------------------------------
	void TakeToHands(InventoryItem item)
	{
		m_timer.Stop();
		m_entity_to_take = item;
		
		if (!m_demoUnit) return;
		
		EntityAI item_in_hands = m_demoUnit.GetEntityInHands();
		if (item_in_hands) 
		{
			m_demoUnit.MoveItemFromHandsToInventory(item_in_hands);
		}
		
		if (item != NULL)
		{
			if (item_in_hands)
			{
				m_timer.Run(0.5, this, "TryTakeToHands", NULL, true);
			}
			else
			{
				TryTakeToHands();
			}
		}
	}
	
	// ------------------------------------------------------------
	void ChangeCharacter(int characterID)
	{
		if ( m_currentCharacterID == characterID )
		{
			return;
		}
		
		if (m_demoUnit)
		{
			//Print(String("Release character: " + m_demoUnit.GetType()));
			g_Game.ObjectDelete(m_demoUnit);
			m_demoUnit = NULL;
		}
		m_currentCharacterID = characterID;
		if(characterID == - 1)
		{
			return;
		}
	
		m_demoUnit = g_Game.GetMenuData().CreateCharacterPerson(characterID);
	
		//Print(String("Load character: " + type));
		
		//g_Game.PreloadObject(type, 1.0);
		
		if (m_demoUnit)
		{
			m_demoUnit.SetPosition(m_demoPos);
			m_demoUnit.SetOrientation(m_demoRot);
			m_demoUnit.SetEventHandler(m_anim_end_event_handler);
			m_demoUnit.SetLastShavedSeconds(m_LastShavedSeconds);
			m_demoUnit.SetCachedPlaytime(m_CachedPlaytime);
	
			InitCharacterAnimations(m_demoUnit.GetEntityInHands());
			//m_timer.Run(0.5, this, "InitItemInHands", NULL, false);
		}
		
		//Change statics for character HERE
		//counter = m_demoUnit.get("kill");
	}
	//-------------------------------------------------------------
	int NextCharacterID()
	{
		int count = g_Game.GetMenuData().GetCharactersCount();
		if(count < 2 )
		{
			return m_currentCharacterID;
		}
		if( m_currentCharacterID + 1 < count)
		{
			return m_currentCharacterID + 1;
		}
		return 0;
	}
	//-------------------------------------------------------------
	int PrevCharacterID()
	{
	//	int count = g_Game.GetMenuData().GetCharactersCount();
		
		if( m_currentCharacterID > 0 )
		{
			return m_currentCharacterID - 1;
		}
		return g_Game.GetMenuData().GetCharactersCount() - 1;
	}
	//-------------------------------------------------------------
	void CreateSelectedCharacter()
	{
		int selChar = m_currentCharacterID;
		m_currentCharacterID = -1;
		ChangeCharacter(selChar);
	}
	// ------------------------------------------------------------
	void DestroyDefaultCharacter()
	{
		g_Game.ObjectDelete(m_demoUnit);
		m_demoUnit = NULL;
	}
	// ------------------------------------------------------------
	void CreateCharacter(string type)
	{
		if (m_demoUnit)
		{
			//Print(String("Release character: " + m_demoUnit.GetType()));
			g_Game.ObjectDelete(m_demoUnit);
			m_demoUnit = NULL;
		}
		//HideSelectedCharacter();
		//Print(String("Load character: " + type));
		
		g_Game.PreloadObject(type, 1.0);
	
		m_demoUnit = g_Game.CreateObject(type, SnapToGround(m_demoPos), true);
		
		if (m_demoUnit)
		{
			m_demoUnit.SetPosition(m_demoPos);
			m_demoUnit.SetOrientation(m_demoRot);
			m_demoUnit.SetEventHandler(m_anim_end_event_handler);
			m_demoUnit.SetLastShavedSeconds(m_LastShavedSeconds);
			m_demoUnit.SetCachedPlaytime(m_CachedPlaytime);

			// NEW STATS API
			string lvalue = "";
			m_demoUnit.StatGetCounter("playtime", lvalue);

			PluginLifespan module_lifespan = (PluginLifespan)GetPlugin( PluginLifespan );
			module_lifespan.UpdateLifespanLevelParam( m_demoUnit, lvalue, true );
		}
	}
	
	// ------------------------------------------------------------
	void AnimateCharacter()
	{
		if (m_demoUnit && m_characterAnimations.Count() > 0)
		{
			int rnd = Math.RandomInt(0, (m_characterAnimations.Count() / 2) - 1 ) * 2;
			//Print("DayZIntroScene::AnimateCharacter()");
			//Print(m_characterAnimations.Get(rnd));
			m_demoUnit.PlayMoveNowGlobal(m_characterAnimations.Get(rnd));
		}
	}
	
	// ------------------------------------------------------------
	void GetAllItemsInInventory(out array<InventoryItem> items)
	{
		items.Clear();
		
		if (!m_demoUnit) return;
		
		for (int i = 0; i < IS_COUNT; i++)
		{
			InventoryItem item = m_demoUnit.FindAttachment(i);
	
			if (!item) continue;
	
			items.Insert(item);
	
			if (item.GetCargo())
			{
				Cargo cargo = item.GetCargo();
	
				for (int j = 0; j < cargo.GetItemCount(); j++)
				{
					items.Insert(cargo.GetItem(j));
				}
			}
	
			for (j = 0; j < item.AttachmentsCount(); j++)
			{
				items.Insert(item.GetAttachmentFromIndex(j));
			}
		}
		
		item = m_demoUnit.GetEntityInHands();
		if (item) items.Insert(item);
	}
	
	// ------------------------------------------------------------
	void InitItemInHands()
	{
		if (!m_demoUnit) return;
		
		InventoryItem rifle = NULL;
		InventoryItem chainsaw = NULL;
		InventoryItem pistol = NULL;
		InventoryItem knife = NULL;
		InventoryItem baseballBat = NULL;
		InventoryItem melee = NULL;
		
		autoptr array<InventoryItem> itemsInInventory = new array<InventoryItem>;
		GetAllItemsInInventory(itemsInInventory);
	
		for (int i = 0; i < itemsInInventory.Count(); i++)
		{
			InventoryItem item = itemsInInventory.Get(i);
	
			if (!item) continue;
	
			if (item.IsKindOf("Chainsaw"))
			{
				chainsaw = item;
			}
	
			if (item.IsKindOf("Rifle"))
			{
				rifle = item;
			}
			
			if (item.IsKindOf("Pistol"))
			{
				pistol = item;
			}
	
			if (item.IsKindOf("KnifeBase"))
			{
				knife = item;
			}
	
			if (item.IsKindOf("BaseballBat"))
			{
				baseballBat = item;
			}
			
			if (item.IsKindOf("MeleeItemBase"))
			{
				melee = item;
			}
		}
	
/*
		Print(m_demoUnit.GetEntityInHands());*/
		
		if (rifle && m_demoUnit.GetEntityInHands() != rifle)
		{
			TakeToHands(rifle);
		}
		else if (baseballBat && m_demoUnit.GetEntityInHands() != baseballBat)
		{
			TakeToHands(baseballBat);
		}
		else if (melee && m_demoUnit.GetEntityInHands() != melee)
		{
			TakeToHands(melee);
		}
		else if (knife && m_demoUnit.GetEntityInHands() != knife)
		{
			TakeToHands(knife);
		}
		else if (pistol && m_demoUnit.GetEntityInHands() != pistol)
		{
			TakeToHands(pistol);
		}
		else if (chainsaw && m_demoUnit.GetEntityInHands() != chainsaw)
		{
			TakeToHands(chainsaw);
		}
		else
		{
			InitCharacterAnimations(m_demoUnit.GetEntityInHands());
		}
	}
	
	// ------------------------------------------------------------
	void InitCharacterAnimations(EntityAI item)
	{
		m_characterAnimations.Clear();
	
		InventoryItem chainsaw = NULL;
		InventoryItem pistol = NULL;
		InventoryItem rifle = NULL;
		InventoryItem knife = NULL;
		InventoryItem melee = NULL;
		InventoryItem baseballBat = NULL;
		
		if (!m_demoUnit) return;
		
		if (item)	
		{
			if (item.IsKindOf("Chainsaw"))
			{
				chainsaw = item;
			}
			
			if (item.IsKindOf("Rifle"))
			{
				rifle = item;
			}
	
			if (item.IsKindOf("Pistol"))
			{
				pistol = item;
			}
	
			if (item.IsKindOf("KnifeBase"))
			{
				knife = item;
			}
	
			if (item.IsKindOf("BaseballBat"))
			{
				baseballBat = item;
			}
			
			if (item.IsKindOf("MeleeItemBase"))
			{
				melee = item;
			}
		}
		
		if (rifle)
		{
			g_Game.ConfigGetTextArray("CfgMovesMaleSdr2 States menu_idlerifleLong0 variantsPlayer", m_characterAnimations);
		}
		else if (baseballBat)
		{
			g_Game.ConfigGetTextArray("CfgMovesMaleSdr2 States menu_idleBat0 variantsPlayer", m_characterAnimations);
		}
		else if (melee)
		{
			g_Game.ConfigGetTextArray("CfgMovesMaleSdr2 States menu_idleHatchet0 variantsPlayer", m_characterAnimations);
		}
		else if (knife)
		{
			g_Game.ConfigGetTextArray("CfgMovesMaleSdr2 States menu_idleKnife0 variantsPlayer", m_characterAnimations);
		}
		else if (pistol)
		{
			g_Game.ConfigGetTextArray("CfgMovesMaleSdr2 States menu_idlePistol0 variantsPlayer", m_characterAnimations);
		}
		else if (chainsaw)
		{
			g_Game.ConfigGetTextArray("CfgMovesMaleSdr2 States menu_idleChainsaw0 variantsPlayer", m_characterAnimations);
		}
		else
		{
			g_Game.ConfigGetTextArray("CfgMovesMaleSdr2 States menu_idleUnarmed0 variantsPlayer", m_characterAnimations);
		}
	
		if (m_characterAnimations.Count() > 0)
		{
			int rnd = Math.RandomInt(0, (m_characterAnimations.Count() / 2) - 1) * 2;
			m_demoUnit.SwitchMoveGlobal(m_characterAnimations.Get(rnd));
		}
	}
	
	// ------------------------------------------------------------
	void ~DayZIntroScene()
	{
		//g_Game.GetMenuData().UnloadCharacters();
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).RemoveCalls(this);
	}
	
	// ------------------------------------------------------------
	void CharacterRotationStart()
	{
		m_rotatingCharacter = true;
		g_Game.GetMousePos(m_rotatingCharacterMouseX, m_rotatingCharacterMouseY);
		if (m_demoUnit) 
		{
			m_rotatingCharacterRot = m_demoRot[0];
			//m_demoUnit.DisableSimulation(true);
		}
	}
	
	// ------------------------------------------------------------
	void CharacterRotationStop()
	{
		if (m_rotatingCharacter)
		{
			m_rotatingCharacter = false;
			if (m_demoUnit)
			{
				//m_demoUnit.DisableSimulation(false);
			}
		}
	}
	
	// ------------------------------------------------------------
	void CharacterRotate()
	{
		int actual_mouse_x;
		int actual_mouse_y;
		g_Game.GetMousePos(actual_mouse_x, actual_mouse_y);
	
		int diff_x = m_rotatingCharacterMouseX - actual_mouse_x;
		
		if (m_demoUnit)
		{
			m_demoRot[0] = m_rotatingCharacterRot + (diff_x * 0.5);
			m_demoUnit.SetOrientation(m_demoRot);
		}	
	}
	
	// ------------------------------------------------------------
	void SaveDefaultCharacter()
	{
		if (m_demoUnit)
		{
			//Print(String(m_demoUnit.GetType()));
			g_Game.SetProfileString("defaultCharacter", m_demoUnit.GetType());
	
			InventoryItem item = NULL;
			TStringArray inventory = new TStringArray;
			
			for (int i = 0; i < IS_COUNT; i++)
			{
				item = m_demoUnit.FindAttachment(i);
				if (item)
				{
					inventory.Insert(item.GetType());
				}
			}
	
			g_Game.SetProfileStringList("defaultInventory", inventory);
			g_Game.SaveProfile();
	
			delete inventory;
		}	
	}
	
	// ------------------------------------------------------------
	void Update()
	{
		if (m_rotatingCharacter)
		{
			CharacterRotate();
		}
	}
	
	// ------------------------------------------------------------
	vector SwapYZ(vector vec)
	{
		vector tmp;
		tmp[0] = vec[0];
		tmp[1] = vec[2];
		tmp[2] = vec[1];
	
		return tmp;
	}
	
	// ------------------------------------------------------------
	vector SnapToGround(vector pos)
	{
		vector tmp_pos = g_Game.SnapToGround(pos);
		tmp_pos[1] = tmp_pos[1] + pos[1];
	
		return tmp_pos;
	}
	bool IsRotatingCharacter() {
		return m_rotatingCharacter;
	}
};
