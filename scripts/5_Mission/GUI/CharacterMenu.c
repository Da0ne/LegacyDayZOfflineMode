class TAdvanceArray<Class T> extends array<string>
{
	void InsertArray(array<T> other)
	{
		for (int i = 0; i < other.Count(); i++)
		{
			T item = other.Get(i);
			Insert(item);
		}
	}
}

/*
typedef TAdvanceArray<int> TIntAdvanceArray;
typedef TAdvanceArray<float> TFloatAdvanceArray;
typedef TAdvanceArray<vector> TVectorAdvanceArray;
typedef TAdvanceArray<class> TClassAdvanceArray;
*/
typedef TAdvanceArray<string> TStringAdvanceArray;

class CharacterMenu extends UIScriptedMenu
{
	XComboBoxWidget m_genderComboBox;
	XComboBoxWidget m_skinComboBox;
	XComboBoxWidget m_topComboBox;
	XComboBoxWidget m_pantsComboBox;
	XComboBoxWidget m_shoesComboBox;

	TStringAdvanceArray m_genderList;
	TStringAdvanceArray m_raceMaleList;
	TStringAdvanceArray m_raceFemaleList;
	TStringAdvanceArray m_skinMaleList;
	TStringAdvanceArray m_skinFemaleList;
	TStringAdvanceArray m_shirtList;
	TStringAdvanceArray m_pantsList;
	TStringAdvanceArray m_shoeList;
	
	string m_format;
	array<Man> m_preloaded;
	DayZIntroScene m_scene;

	void CharacterMenu()
	{
		m_genderList = new TStringAdvanceArray;
		m_raceMaleList = new TStringAdvanceArray;
		m_raceFemaleList = new TStringAdvanceArray;
		m_skinMaleList = new TStringAdvanceArray;
		m_skinFemaleList = new TStringAdvanceArray;
		m_shirtList = new TStringAdvanceArray;
		m_pantsList = new TStringAdvanceArray;
		m_shoeList = new TStringAdvanceArray;
		
		m_preloaded = new array<Man>;
	}

	void ~CharacterMenu()
	{
		if (m_scene)
		{
			m_scene.CreateSelectedCharacter();
		}
		
		delete m_genderList;
		delete m_raceMaleList;
		delete m_raceFemaleList;
		delete m_skinMaleList;
		delete m_skinFemaleList;
		delete m_shirtList;
		delete m_pantsList;
		delete m_shoeList;
		
		// unload all possible characters
		/*for (int i = 0; i < m_preloaded.Count(); i++)
		{
			Man obj = m_preloaded.Get(i);
			
			if (obj)
			{
				//obj.Release();
				//g_Game.ObjectDelete(obj);
			}
		}*/

		delete m_preloaded;
	}

	Widget Init()
	{
		Widget layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_character.layout");

		m_genderComboBox = layoutRoot.FindAnyWidget("GenderComboBoxWidget");
		m_skinComboBox = layoutRoot.FindAnyWidget("SkinComboBoxWidget");
		m_topComboBox = layoutRoot.FindAnyWidget("TopComboBoxWidget");
		m_pantsComboBox = layoutRoot.FindAnyWidget("PantsComboBoxWidget");
		m_shoesComboBox = layoutRoot.FindAnyWidget("ShoesComboBoxWidget");
		
		MissionMainMenu mission = g_Game.GetMission();
		m_scene = mission.GetIntroScene();
		DefaultCharacterScreen();
		return layoutRoot;
	}
	
	void OnShow()
	{
	}
		
	bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		
		switch (w.GetUserID())
		{
		case IDC_OK:
			m_scene.SaveDefaultCharacter();
			Close();
			return true;
			
		case IDC_CANCEL:
			Close();
			return true;

		case IDC_RESTART:
			RandomCharacter();
			return true;
		}

		if (w == m_genderComboBox)
		{
			if (m_genderComboBox.GetCurrentItem() == 0)
			{
				CharacterComboBoxInsertList(m_skinComboBox, m_skinFemaleList, false);
			}
			else
			{
				CharacterComboBoxInsertList(m_skinComboBox, m_skinMaleList, false);
			}

			SetInventory();
		}
			
		if (w == m_skinComboBox)
		{
			SetInventory();
		}

		if (m_scene.m_demoUnit)
		{
			if (w == m_topComboBox)
			{
				SetAttachment(m_shirtList.Get(m_topComboBox.GetCurrentItem()), IS_BODY);
			}

			if (w == m_pantsComboBox)
			{
				SetAttachment(m_pantsList.Get(m_pantsComboBox.GetCurrentItem()), IS_LEGS);
			}

			if (w == m_shoesComboBox)
			{
				SetAttachment(m_shoeList.Get(m_shoesComboBox.GetCurrentItem()), IS_FEET);
			}
		}

		return false;
	}
	
	bool OnMouseButtonDown(Widget w, int x, int y, int button)
	{
		super.OnMouseButtonDown(w, x, y, button);
		
		if (w.GetName() == "CharacterRotationFrame")
		{
			m_scene.CharacterRotationStart();
			return true;
		}

		return false;
	}
	
	bool OnMouseButtonUp(Widget w, int x, int y, int button)
	{
		super.OnMouseButtonUp(w, x, y, button);
		
		m_scene.CharacterRotationStop();
		return true;
	}
	
	// tools
	void DefaultCharacterScreen()
	{
		int i = 0;
		int j = 0;
		string root_path = "cfgCharacterCreation";
		TStringArray race = new TStringArray;
		TStringArray skin = new TStringArray;
		TStringArray tmp_list = new TStringArray;
		TStringArray defInv = new TStringArray;
		
		g_Game.ConfigGetTextArray(root_path + " skin", skin);
		g_Game.ConfigGetTextArray(root_path + " gender", m_genderList);
		g_Game.ConfigGetTextArray(root_path + " race", race);
		g_Game.ConfigGetTextArray(root_path + " top", m_shirtList);
		g_Game.ConfigGetTextArray(root_path + " bottom", m_pantsList);
		g_Game.ConfigGetTextArray(root_path + " shoe", m_shoeList);
		g_Game.ConfigGetTextArray(root_path + " " + m_genderList.Get(0) + "custom", tmp_list);
		m_skinFemaleList.Copy(skin);
		m_raceFemaleList.Copy(race);
		
		m_skinFemaleList.InsertArray(tmp_list);
		m_raceFemaleList.InsertArray(tmp_list);

		g_Game.ConfigGetTextArray(root_path + " " + m_genderList.Get(1) + "custom", tmp_list);
		m_skinMaleList.Copy(skin);
		m_raceMaleList.Copy(race);

		m_skinMaleList.InsertArray(tmp_list);
		m_raceMaleList.InsertArray(tmp_list);
		
		g_Game.ConfigGetText(root_path + " format", m_format);
		CharacterComboBoxInsertList(m_genderComboBox, m_genderList, false);
		CharacterComboBoxInsertList(m_skinComboBox, skin, false);
		CharacterComboBoxInsertList(m_topComboBox, m_shirtList, true);
		CharacterComboBoxInsertList(m_pantsComboBox, m_pantsList, true);
		CharacterComboBoxInsertList(m_shoesComboBox, m_shoeList, true);
		
		string defChar;
		string params[2];
		params[0] = m_genderList.Get(0);
		params[1] = race.Get(0);

		// default character
		g_Game.FormatString(m_format, params, defChar);
		g_Game.GetProfileStringList("defaultInventory", defInv);
		g_Game.GetProfileString("defaultCharacter", defChar);
		
		if (defInv.Count() == 0)
		{
			defInv.Insert(m_shirtList.Get(0));
			defInv.Insert(m_pantsList.Get(0));
			defInv.Insert(m_shoeList.Get(0));
		}

		// todo: startLoadingScreen ["Loading Character Creation...."];

		

		if (m_scene.m_demoUnit)
		{
			InventoryItem item = NULL;

			for (i = 0; i < IS_COUNT; i++)
			{
				item = m_scene.m_demoUnit.FindAttachment(i);
				if (item)
				{
					g_Game.ObjectDelete(item);
				}
			}
		}

		m_scene.CreateCharacter(defChar);

		if (!m_scene.m_demoUnit) return;
		
		int index = -1;
		int last_preloaded_index;
		string tmp;
		vector preloaded_pos;
		Man preloaded_object = NULL;

		// preload all possible characters
		preloaded_pos = Vector(0,0,-2).Multiply4(m_scene.m_cameraTrans);
		
		for (i = 0; i < m_raceMaleList.Count(); i++)
		{
			params[0] = m_genderList.Get(1);
			params[1] = m_raceMaleList.Get(i);

			g_Game.FormatString(m_format, params, tmp);
			
			g_Game.PreloadObject(tmp, 1.0);		
			preloaded_object = g_Game.CreateObject(tmp, preloaded_pos, true);

			if (preloaded_object)
			{
				preloaded_object.SetPosition(preloaded_pos);
				preloaded_object.DisableSimulation(true);

				m_preloaded.Insert(preloaded_object);	
			}
		}

		for (i = 0; i < m_raceFemaleList.Count(); i++)
		{
			params[0] = m_genderList.Get(0);
			params[1] = m_raceFemaleList.Get(i);

			g_Game.FormatString(m_format, params, tmp);

			g_Game.PreloadObject(tmp, 1.0);
			preloaded_object = g_Game.CreateObject(tmp, preloaded_pos, true);

			if (preloaded_object)
			{
				preloaded_object.SetPosition(preloaded_pos);
				preloaded_object.DisableSimulation(true);

				m_preloaded.Insert(preloaded_object);
			}
		}

		// set currently used items
		tmp_list.Clear();

		if (m_scene.m_demoUnit.IsKindOf("SurvivorMale"))
		{
			CharacterComboBoxInsertList(m_skinComboBox, m_skinMaleList, false);
			m_genderComboBox.SetCurrentItem(1);

			params[0] = m_genderList.Get(m_genderComboBox.GetCurrentItem());

			for (i = 0; i < m_raceMaleList.Count(); i++)
			{
				params[1] = m_raceMaleList.Get(i);
				g_Game.FormatString(m_format, params, tmp);
				tmp_list.Insert(tmp);
			}
		}
		else
		{
			CharacterComboBoxInsertList(m_skinComboBox, m_skinFemaleList, false);
			m_genderComboBox.SetCurrentItem(0);

			params[0] = m_genderList.Get(m_genderComboBox.GetCurrentItem());
			for (i = 0; i < m_raceFemaleList.Count(); i++)
			{
				params[1] = m_raceFemaleList.Get(i);
				g_Game.FormatString(m_format, params, tmp);
				tmp_list.Insert(tmp);
			}

		}

		index = tmp_list.Find(m_scene.m_demoUnit.GetType());

		if (index == -1)
		{
			index = 0;
		}
		m_skinComboBox.SetCurrentItem(index);

		for (i = 0; i < 3; i++)
		{
			if (g_Game.IsKindOf(defInv.Get(i), "TopwearBase"))
			{
				index = m_shirtList.Find(defInv.Get(i));
				if (index == -1)
				{
					index = 0;
				}

				m_topComboBox.SetCurrentItem(index);
				continue;
			}

			if (g_Game.IsKindOf(defInv.Get(i), "BottomwearBase"))
			{
				index = m_pantsList.Find(defInv.Get(i));
				if (index == -1)
				{
					index = 0;
				}
				m_pantsComboBox.SetCurrentItem(index);
				continue;
			}

			if (g_Game.IsKindOf(defInv.Get(i), "FootwearBase"))
			{
				index = m_shoeList.Find(defInv.Get(i));
				if (index == -1)
				{
					index = 0;
				}
				m_shoesComboBox.SetCurrentItem(index);
				continue;
			}
		}

		SetInventory();
		// todo: endLoadingScreen;

		delete race;
		delete skin;
		delete tmp_list;
		delete defInv;
	}
	
	void SetInventory()
	{
		string character;
		string params[2];

		if (m_genderComboBox.GetCurrentItem() == 0)
		{
			params[0] = m_genderList.Get(0);
			params[1] = m_raceFemaleList.Get(m_skinComboBox.GetCurrentItem());
		}
		else
		{
			params[0] = m_genderList.Get(1);
			params[1] = m_raceMaleList.Get(m_skinComboBox.GetCurrentItem());
		}	
		
		g_Game.FormatString(m_format, params, character);
		m_scene.CreateCharacter(character);
		if (m_scene.m_demoUnit)
		{
			SetAttachment(m_shirtList.Get(m_topComboBox.GetCurrentItem()), IS_BODY);
			SetAttachment(m_pantsList.Get(m_pantsComboBox.GetCurrentItem()), IS_LEGS);
			SetAttachment(m_shoeList.Get(m_shoesComboBox.GetCurrentItem()), IS_FEET);
			m_scene.InitCharacterAnimations(m_scene.m_demoUnit.GetEntityInHands());
		}
	}
	
	void RandomCharacter()
	{
		// make random selection
		RandomSelectComboBox(m_genderComboBox);

		if (m_genderComboBox.GetCurrentItem() == 0)
		{
			CharacterComboBoxInsertList(m_skinComboBox, m_skinFemaleList, false);
		}
		else
		{
			CharacterComboBoxInsertList(m_skinComboBox, m_skinMaleList, false);
		}

		RandomSelectComboBox(m_skinComboBox);
		RandomSelectComboBox(m_topComboBox);
		RandomSelectComboBox(m_pantsComboBox);
		RandomSelectComboBox(m_shoesComboBox);

		// update character
		SetInventory();
		
		
		if (m_scene.m_demoUnit)
		{
			SetAttachment(m_shirtList.Get(m_topComboBox.GetCurrentItem()), IS_BODY);
			SetAttachment(m_pantsList.Get(m_pantsComboBox.GetCurrentItem()), IS_LEGS);
			SetAttachment(m_shoeList.Get(m_shoesComboBox.GetCurrentItem()), IS_FEET);
		}
	}
	
	void RandomSelectComboBox(XComboBoxWidget combo)
	{
		if (combo)
		{
			int rnd = Math.RandomInt(0, combo.GetNumItems());
			combo.SetCurrentItem(rnd);
		}
	}
	
	void SetAttachment(string type, int slot)
	{
		if (!m_scene.m_demoUnit) return;
		g_Game.ObjectDelete(m_scene.m_demoUnit.FindAttachment(slot));
		Object item = g_Game.CreateObject(type, "0 0 0", true);
		if (item) m_scene.m_demoUnit.TakeEntityAsAttachmentToSlot(item, slot);
	}

	void CharacterComboBoxInsertList(XComboBoxWidget combo, TStringArray items, bool search_display_name)
	{
		if (!combo) return;

		string display_name;

		combo.SetCurrentItem(0);
		combo.ClearAll();
		
		for (int i = 0; i < items.Count(); i++)
		{
			if (search_display_name)
			{
				 g_Game.ConfigGetText("cfgVehicles " + items.Get(i) + " displayName", display_name);
			}
			else
			{
				display_name = items.Get(i);
			}

			combo.AddItem(display_name);
		}
	}
	
	int ListLoad(string path, out TStringArray list);
}
