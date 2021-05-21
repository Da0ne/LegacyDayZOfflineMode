// #include "Scripts\Classes\Gui\OptionsAccessUI.c"
// #include "Scripts\Classes\Gui\SliderOptionsAccessUI.c"
// #include "Scripts\Classes\Gui\ComboBoxOptionsAccessUI.c"
// #include "Scripts\Classes\Gui\ButtonOptionsAccessUI.c"


typedef array<OptionsAccessUI> TOptionsAccessUIArray;

class OptionsMenu extends UIScriptedMenu
{
	int m_selected_tab = 0;
	
	void OptionsMenu()
	{
		m_action_indices = new TIntArray;
		m_keys = new TIntArray;
		m_optionsUI = new TOptionsAccessUIArray;
	}

	void ~OptionsMenu()
	{
		m_Options.Revert();
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).RemoveCalls(this);
		
		for (int i = 0; i < m_optionsUI.Count(); i++)
		{
			OptionsAccessUI optionUI = m_optionsUI.Get(i);
			delete optionUI;
		}
		GetGame().EndOptionsVideo();
	}
	
	Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_options.layout");
		
		GetGame().BeginOptionsVideo();	
		
		m_tabs[TAB_GAME] = layoutRoot.FindAnyWidget("GamePanelWidget");
		m_tabs[TAB_SOUND] = layoutRoot.FindAnyWidget("SoundPanelWidget");
		m_tabs[TAB_VIDEO] = layoutRoot.FindAnyWidget("VideoPanelWidget");
		m_tabs[TAB_CONTROLS] = layoutRoot.FindAnyWidget("ControlsPanelWidget");

		m_tab_buttons[TAB_GAME] = (ButtonWidget)layoutRoot.FindAnyWidget("GameButtonWidget");
		m_tab_buttons[TAB_SOUND] = (ButtonWidget)layoutRoot.FindAnyWidget("SoundButtonWidget");
		m_tab_buttons[TAB_VIDEO] = (ButtonWidget)layoutRoot.FindAnyWidget("VideoButtonWidget");
		m_tab_buttons[TAB_CONTROLS] = (ButtonWidget)layoutRoot.FindAnyWidget("ControlsButtonWidget");

#ifdef X1_TODO_TEMP_GUI
		m_tab_buttons[TAB_VIDEO].Show(false);
		m_tab_buttons[TAB_VIDEO] = NULL;
		m_tab_buttons[TAB_CONTROLS].Show(false);
		m_tab_buttons[TAB_CONTROLS] = NULL;
		m_tabs[TAB_VIDEO].Show(false);
		m_tabs[TAB_VIDEO] = NULL;
		m_tabs[TAB_CONTROLS].Show(false);
		m_tabs[TAB_CONTROLS] = NULL;
#endif

		m_action_groups = (XComboBoxWidget)layoutRoot.FindAnyWidget("ActionGroupsComboBoxWidget");
		m_action_groups.ClearAll();

		m_actions = (TextListboxWidget)layoutRoot.FindAnyWidget("ActionsListboxWidget");
		m_actions.ClearItems();

		m_devices = (TextListboxWidget)layoutRoot.FindAnyWidget("DevicesListboxWidget");
		m_devices.ClearItems();

		m_presets_button = (ButtonWidget)layoutRoot.FindAnyWidget("PresetsBtn");
		m_assign_button = (ButtonWidget)layoutRoot.FindAnyWidget("AssignBtn");

		m_device_button = (ButtonWidget)layoutRoot.FindAnyWidget("EnableDeviceBtn");
		m_device_custom_scheme = (CheckBoxWidget)layoutRoot.FindAnyWidget("CustomSchemeCheckBoxWidget");

		m_test_button = (ButtonWidget)layoutRoot.FindAnyWidget("TestBtn");
		m_quality_set_button = (ButtonWidget)layoutRoot.FindAnyWidget("QualitySetBtn");
		m_default_video_button = (ButtonWidget)layoutRoot.FindAnyWidget("DefaultVideoBtn");

		m_system_cm_check = layoutRoot.FindAnyWidget("SystemCMCheckBox");
		m_global_cm_check = layoutRoot.FindAnyWidget("GlobalCMCheckBox");
		m_direct_cm_check = layoutRoot.FindAnyWidget("DirectCMCheckBox");
		m_vehicle_cm_check = layoutRoot.FindAnyWidget("VehicleCMCheckBox");
		m_radio_cm_check = layoutRoot.FindAnyWidget("RadioCMCheckBox");
		
		int i = 0;
		int c = g_Game.GetInput().GetActionGroupsCount();
		string name;

		for (i = 0; i < c ;i++)
		{
			g_Game.GetInput().GetActionGroupName(i, name);
			m_action_groups.AddItem(name);
		}
		SelectActionGroup(0);
		SelectTab(m_selected_tab);

		InitOptions();	
		UpdateDevices();
		UpdateChatMessages();
		return layoutRoot;
	}
	
	void InitOptions()
	{
		int i;
		int c;
		OptionsAccessUI optionUI;
		OptionsAccess optionAccess;
		
		// cleanup first
		c = m_optionsUI.Count();
		for (i = 0; i < c; i++)
		{
			optionUI = m_optionsUI.Get(i);
			delete optionUI;
		}
		m_optionsUI.Clear();

		if (m_Options)
		{
			delete m_Options;
		}
		
		m_Options = new GameOptions;

		c = m_Options.GetOptionsCount();
		for (i = 0; i < c; i++)
		{
			optionUI = NULL;
			optionAccess = m_Options.GetOption(i);
			
			switch (optionAccess.GetControlType())
			{
			case OA_CT_NUMERIC:
			optionUI = new SliderOptionsAccessUI;
			break;
			case OA_CT_SWITCH:
			optionUI = new ButtonOptionsAccessUI;
			break;
			case OA_CT_LIST:
			optionUI = new ComboBoxOptionsAccessUI;
			break;
			}

			switch (optionAccess.GetAccessType())
			{
			case AT_OPTIONS_LANGUAGE:
			m_language_option = optionUI;
			break;
			case AT_OPTIONS_SLIDER_FILLRATE:
			m_rend_res_option = optionUI;
			break;
			case AT_QUALITY_PREFERENCE:
			m_overall_quality_option = optionUI;
			break;
			case AT_OPTIONS_VIDEO_DEFAULT:
			m_default_video_access = optionAccess;
			break;
			case AT_FSAA_DETAIL:
			m_anitialiasing_option = optionUI;
			break;
			case AT_POSTPROCESS_EFFECTS:
			m_postprocess_quality_option = optionUI;
			break;
			case AT_ATOC_DETAIL:
			m_atoc_option = optionUI;
			break;
			case AT_AMBIENT_OCCLUSION:
			m_ao_option = optionUI;
			break;
			case AT_BLOOM:
			m_bloom_option = optionUI;
			break;
			case AT_ROTATION_BLUR:
			m_rot_blur_option = optionUI;
			break;
			}

			if (optionUI)
			{
				optionUI.Attach(optionAccess, layoutRoot.FindAnyWidgetById(WIDGET_USER_ID_OFFSET + optionAccess.GetAccessType()));
				m_optionsUI.Insert(optionUI);
			}
		}

		CheckOptionsDependency();
	}
	
	bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		
		int i;
		int selected;
		OptionsAccessUI item;
		
		switch (w.GetUserID())
		{
		case IDC_OK:
			if (m_Options.NeedRestart())
			{
				g_Game.GetUIManager().ShowDialog("#main_menu_configure", "#menu_restart_needed", 0, DBT_YESNO, DBB_YES, DMT_QUESTION, this);
			}
			else
			{
				GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(this, "ApplyFn");
			}
			return true;
			
		case IDC_CANCEL:
			Close();
			return true;
		}

		if (w == m_device_button)
		{
			selected = m_devices.GetSelectedRow();
			if (selected > -1)
			{
				if (g_Game.GetInput().IsDeviceEnabled(selected))
				{
					g_Game.GetInput().SetDeviceEnabled(selected, false);
				}
				else
				{
					g_Game.GetInput().SetDeviceEnabled(selected, true);
				}
			}

			UpdateDevices();
			return true;
		}

		if (w == m_device_custom_scheme)
		{
			if (g_Game.GetInput().IsCustomSchemeEnabled())
			{
				g_Game.GetInput().SetCustomSchemeEnabled(false);
			}
			else
			{
				g_Game.GetInput().SetCustomSchemeEnabled(true);
			}

			UpdateDevices();
			return true;
		}

		if (w == m_action_groups)
		{
			SelectActionGroup(m_action_groups.GetCurrentItem());
			return true;
		}
		
		if (w == m_test_button)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(this, "TestFn");
			return true;
		}

		if (w == m_quality_set_button)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(this, "OverallQualityChanged");
			return true;
		}

		if (w == m_default_video_button)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(this, "DefaultVideoSettings");
			return true;
		}

		if (w == m_anitialiasing_option.GetWidget())
		{
			CheckOptionsDependency();
		}

		if (w == m_postprocess_quality_option.GetWidget())
		{
			CheckOptionsDependency();
		}

		if (w == m_presets_button)
		{
			EnterScriptedMenu(MENU_CONTROLS_PRESET);
		}

		if (w == m_assign_button)
		{
			selected = m_actions.GetSelectedRow();
			if (selected > -1)
			{
				int index = m_action_indices.Get(selected);

				AssignmentMenu assignmentsMenu = (AssignmentMenu)EnterScriptedMenu(MENU_ASSIGNMENT);
				if (assignmentsMenu)
				{
					assignmentsMenu.InitAssignment(index);
				}

				return true;
			}
		}

		for (i = 0; i < TABS_COUNT; i++)
		{
			if (w == m_tab_buttons[i])
			{
				SelectTab(i);
				return true;
			}
		}

		return false;
	}
	
	bool OnController(Widget w, int control, int value)
	{
		super.OnController(w, control, value);
		
		if(control == ControlID.CID_TABLEFT && value != 0)
		{
			SelectPreviousTab();
		}
		
		if(control == ControlID.CID_TABRIGHT && value != 0)
		{
			SelectNextTab();
		}
	}
	
	bool OnChange(Widget w, int x, int y, bool finished)
	{
		super.OnChange(w, x, y, finished);
		
		if (w == m_system_cm_check)
		{
			m_system_cm = m_system_cm_check.IsChecked();
		}
		else if (w == m_global_cm_check)
		{
			m_global_cm  = m_global_cm_check.IsChecked();
		}
		else if (w == m_direct_cm_check)
		{
			m_direct_cm = m_direct_cm_check.IsChecked();
		}
		else if (w == m_vehicle_cm_check)
		{
			m_vehicle_cm = m_vehicle_cm_check.IsChecked();
		}
		else if (w == m_radio_cm_check)
		{
			m_radio_cm = m_radio_cm_check.IsChecked();
		}
		return false;
	}
	
	bool OnDoubleClick(Widget w, int x, int y, int button)
	{
		super.OnDoubleClick(w, x, y, button);
		
		int selected;

		if (w == m_actions)
		{
			selected = m_actions.GetSelectedRow();
			if (selected > -1)
			{
				int item = m_action_indices.Get(selected);

				AssignmentMenu assignmentsMenu = (AssignmentMenu)EnterScriptedMenu(MENU_ASSIGNMENT);
				if (assignmentsMenu)
				{
					assignmentsMenu.InitAssignment(item);
				}

				return true;
			}
		}

		if (w == m_devices)
		{
			selected = m_devices.GetSelectedRow();
			if (selected > -1)
			{
				m_device_button.Enable(g_Game.GetInput().IsCustomSchemeEnabled());

				if (g_Game.GetInput().IsDeviceEnabled(selected))
				{
					m_device_button.SetText("#menu_disable");
				}
				else
				{
					m_device_button.SetText("#menu_enable");
				}
			}
			else
			{
				m_device_button.Show(false);
			}
		}
	}
	
	bool OnModalResult(Widget w, int x, int y, int code, int result)
	{
		super.OnModalResult(w, x, y, code, result);
		
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(this, "ApplyFn");
		
		if (result == DBB_YES)
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(this, "RestartFn");
		}
	}

	void OnShow()
	{
		super.OnShow();
		SelectActionGroup(m_action_groups.GetCurrentItem());
	}
	
	void SelectPreviousTab()
	{
		int currTab = m_selected_tab;
		currTab = (currTab - 1) % TABS_COUNT;
		if (currTab < 0)
		{
			currTab = TABS_COUNT - 1;
		}

		while(currTab != m_selected_tab)
		{
			if (m_tab_buttons[currTab] != NULL)
			{
				SelectTab(currTab);
				return;
			}

			currTab = (currTab - 1) % TABS_COUNT;
			if (currTab < 0)
			{
				currTab = TABS_COUNT - 1;
			}
		}
	}
	
	void SelectNextTab()
	{
		int currTab = m_selected_tab;
		currTab = (currTab + 1) % TABS_COUNT;
		
		while(currTab != m_selected_tab)
		{
			if (m_tab_buttons[currTab] != NULL)
			{
				SelectTab(currTab);	
				return;		
			}
			
			currTab = (currTab + 1) % TABS_COUNT;
		}
	}

	void SelectTab(int tab_id)
	{
		int i = 0;
		for (i = 0; i < TABS_COUNT; i++)
		{
			Widget tab = m_tabs[i];
			if(tab != NULL)
				tab.Show(i == tab_id);
		}

		for (i = 0; i < TABS_COUNT; i++)
		{
			ButtonWidget button = m_tab_buttons[i];
			if(button != NULL)
				button.SetState(i == tab_id);		
		}

		m_selected_tab = tab_id;
	}
	
	void SelectActionGroup(int group_index)
	{
		Print("group_index: " + group_index);
		m_actions.ClearItems();
		g_Game.GetInput().GetActionGroupItems(group_index, m_action_indices);
		string desc;

		for (int i = 0; i < m_action_indices.Count(); i++)
		{
			int action_index = m_action_indices.Get(i);
			g_Game.GetInput().GetActionDesc(action_index, desc);
			int row = m_actions.AddItem(desc, NULL, 0);
			Print(GetActionKeysString(action_index) + " index -> " + action_index);
			m_actions.SetItem(row, GetActionKeysString(action_index), NULL, 1);
		}
	}
	
	string GetActionKeysString(int action_index)
	{
		int key;
		string name;
		string output;
		output = "";
		g_Game.GetInput().GetActionKeys(action_index, m_keys);

		for (int i = 0; i < m_keys.Count(); i++)
		{
			key = m_keys.Get(i);
			g_Game.GetInput().GetActionKeyName(key, name);
			if (i > 0)
			{
				output += ",";
			}

			output += "'";
			output += name;
			output += "'";

			int conflictAction = g_Game.GetInput().CheckKeyCollision(action_index, key);
			if (conflictAction != -1)
			{
				output += "!";
			}
		}

		return output;
	}
	
	void UpdateChatMessages()
	{
		m_system_cm = GetProfileValueBool(SYSTEM_CHAT_MSG, true);
		m_system_cm_check.SetChecked(m_system_cm);
		
		m_global_cm = GetProfileValueBool(GLOBAL_CHAT_MSG, true);
		m_global_cm_check.SetChecked(m_global_cm);
		
		m_direct_cm = GetProfileValueBool(DIRECT_CHAT_MSG, true);
		m_direct_cm_check.SetChecked(m_direct_cm);
		
		m_vehicle_cm = GetProfileValueBool(VEHICLE_CHAT_MSG, true);
		m_vehicle_cm_check.SetChecked(m_vehicle_cm);

		m_radio_cm = GetProfileValueBool(RADIO_CHAT_MSG, true);	
		m_radio_cm_check.SetChecked(m_radio_cm);
	}
	
	void UpdateDevices()
	{
		// devices
		int i;
		int c;
		int lastSelection = m_devices.GetSelectedRow();
		string name;

		m_devices.ClearItems();
		
		c = g_Game.GetInput().GetDevicesCount();
		for (i = 0; i < c; i++)
		{
			g_Game.GetInput().GetDeviceName(i, name);
			m_devices.AddItem(name, NULL, 0);
			int enabled = g_Game.GetInput().IsDeviceEnabled(i);

			if (enabled)
			{
				m_devices.SetItem(i, "#menu_enabled", NULL, 1);
			}
			else
			{
				m_devices.SetItem(i, "#menu_disabled", NULL, 1);
			}
		}

		if (c)
		{
			if (lastSelection >= 0 && lastSelection < c)
			{
				m_devices.SelectRow(lastSelection);
			}
			else
			{
				m_devices.SelectRow(0);
			}

			m_device_button.Enable(true);

			if (g_Game.GetInput().IsDeviceEnabled(m_devices.GetSelectedRow()))
			{
				m_device_button.SetText("#menu_disable");
			}
			else
			{
				m_device_button.SetText("#menu_enable");
			}
		}
		else
		{
			m_device_button.Enable(false);
		}

		int IsCustomSchemeEnabled = g_Game.GetInput().IsCustomSchemeEnabled();
		
		if (g_Game.GetInput().IsCustomSchemeEnabled())
		{
			m_device_custom_scheme.SetChecked(false);
			m_device_button.Show(true);
		}
		else
		{
			m_device_custom_scheme.SetChecked(true);
			m_device_button.Show(false);
		}
	}
	
	autoptr GameOptions m_Options;

	void TestFn()
	{
		m_Options.Test();

		// reload fillrate option
		m_rend_res_option.Init();
		m_rend_res_option.Update();
	}

	void ApplyFn()
	{
		SetProfileValueBool(SYSTEM_CHAT_MSG, m_system_cm);
		SetProfileValueBool(GLOBAL_CHAT_MSG, m_global_cm);
		SetProfileValueBool(DIRECT_CHAT_MSG, m_direct_cm);
		SetProfileValueBool(VEHICLE_CHAT_MSG, m_vehicle_cm);
		SetProfileValueBool(RADIO_CHAT_MSG, m_radio_cm);
		
		string value;
		GetGame().GetProfileString("usenewui", value);
		
		GetGame().SaveProfile();
		m_Options.Apply();
		Close();
	}

	void RestartFn()
	{
		g_Game.RequestRestart(IDC_MAIN_QUIT);
	}
	
	void OverallQualityChanged()
	{
		m_overall_quality_option.GetOptionAccess().Test();
		m_overall_quality_option.GetOptionAccess().Apply();
		InitOptions();
	}
	
	void DefaultVideoSettings()
	{
		if (m_default_video_access)
		{
			m_default_video_access.Switch();
			InitOptions();
		}
	}

	void CheckOptionsDependency()
	{
		ListOptionsAccess listAccess;

		{
			listAccess = m_anitialiasing_option.GetOptionAccess();
			if (listAccess.GetIndex() == 0)
			{
				m_atoc_option.GetWidget().SetFlags(WidgetFlags.DISABLED);
			}
			else
			{
				m_atoc_option.GetWidget().ClearFlags(WidgetFlags.DISABLED);
			}
		}

		{
			listAccess = m_postprocess_quality_option.GetOptionAccess();
			if (listAccess.GetIndex() == 0)
			{
				m_bloom_option.GetWidget().SetFlags(WidgetFlags.DISABLED);
				m_rot_blur_option.GetWidget().SetFlags(WidgetFlags.DISABLED);
			}
			else
			{
				m_bloom_option.GetWidget().ClearFlags(WidgetFlags.DISABLED);
				m_rot_blur_option.GetWidget().ClearFlags(WidgetFlags.DISABLED);
			}

			if (listAccess.GetIndex() < 2)
			{
				m_ao_option.GetWidget().SetFlags(WidgetFlags.DISABLED);
			}
			else
			{
				m_ao_option.GetWidget().ClearFlags(WidgetFlags.DISABLED);
			}
		}
	}

	const int TAB_GAME = 0;
	const int TAB_SOUND = 1;
	const int TAB_VIDEO = 2;
	const int TAB_CONTROLS = 3;
	const int TABS_COUNT = 4;

	const int WIDGET_USER_ID_OFFSET = 10000;
	
	Widget m_tabs[TABS_COUNT];
	
	autoptr TIntArray m_action_indices;
	autoptr TIntArray m_keys;
	
	bool	m_system_cm;
	bool	m_global_cm;
	bool	m_direct_cm;
	bool	m_vehicle_cm;
	bool	m_radio_cm;

	CheckBoxWidget	m_system_cm_check;
	CheckBoxWidget	m_global_cm_check;
	CheckBoxWidget	m_direct_cm_check;
	CheckBoxWidget	m_vehicle_cm_check;
	CheckBoxWidget	m_radio_cm_check;
	
	ButtonWidget m_tab_buttons[TABS_COUNT];
	XComboBoxWidget m_action_groups;
	TextListboxWidget m_actions;
	TextListboxWidget m_devices;
	ButtonWidget m_device_button;
	ButtonWidget m_presets_button;
	ButtonWidget m_assign_button;
	ButtonWidget m_test_button;
	ButtonWidget m_quality_set_button;
	ButtonWidget m_default_video_button;
	CheckBoxWidget m_device_custom_scheme;

	OptionsAccessUI m_rend_res_option;
	OptionsAccessUI m_language_option;
	OptionsAccessUI m_overall_quality_option;
	SwitchOptionsAccess m_default_video_access;
	ComboBoxOptionsAccessUI m_anitialiasing_option; // AT_FSAA_DETAIL
	ComboBoxOptionsAccessUI m_postprocess_quality_option; // AT_POSTPROCESS_EFFECTS

	OptionsAccessUI m_atoc_option; // AT_ATOC_DETAIL
	OptionsAccessUI m_ao_option; // AT_AMBIENT_OCCLUSION
	OptionsAccessUI m_bloom_option; // AT_BLOOM
	OptionsAccessUI m_rot_blur_option; // AT_ROTATION_BLUR

	autoptr TOptionsAccessUIArray m_optionsUI;
}
