class UIPopupScriptSceneManager extends UIPopupScript
{
	private ButtonWidget 		m_BtnCancel;
	private ButtonWidget 		m_BtnSceneNew;
	private ButtonWidget 		m_BtnSceneLoad;
	private ButtonWidget 		m_BtnSceneRename;
	private ButtonWidget 		m_BtnSceneDuplicate;
	private ButtonWidget 		m_BtnSceneDelete;
	private TextListboxWidget	m_LstListScenes;
	
	private static const int m_DaysInMonth[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	
	private ButtonWidget	m_BtnSave;
	
	private TextWidget		m_TxtWeatherTime;
	private TextWidget		m_TxtAirTemperature;
	
	private SliderWidget 	m_SldStartTime;
	private TextWidget		m_TxtStartTimeValue;
	private SliderWidget 	m_SldStartDay;
	private TextWidget		m_TxtStartDayValue;
	private SliderWidget	m_SldOvercast;
	private TextWidget		m_TxtOvercastValue;
	private SliderWidget	m_SldRain;
	private TextWidget		m_TxtRainValue;
	private SliderWidget	m_SldFog;
	private TextWidget		m_TxtFogValue;
	private SliderWidget	m_SldWindForce;
	private TextWidget		m_TxtWindForceValue;
	
	private int				m_OrigYear;
	private int				m_OrigMonth;
	private int				m_OrigDay;
	private int				m_OrigHour;
	private int				m_OrigMinute;
	private float			m_OrigOvercast;
	private float			m_OrigRain;
	private float			m_OrigFog;
	private float			m_OrigWindForce;
	
	private int				m_CurrYear;
	private int				m_CurrMonth;
	private int				m_CurrDay;
	private int				m_CurrHour;
	private int				m_CurrMinute;
	private float			m_CurrOvercast;
	private float			m_CurrRain;
	private float			m_CurrFog;
	private float			m_CurrWindForce;
	
	private autoptr TimerUpdate	m_TimerUpdate;
	
	//================================================
	// UIPopupScriptSceneManager
	//================================================	
	void UIPopupScriptSceneManager(Widget wgt)
	{
		m_BtnCancel			= wgt.FindAnyWidget("btn_ppp_sm_cancel");
		m_BtnSceneNew		= wgt.FindAnyWidget("btn_ppp_sm_mission_new");
		m_BtnSceneLoad		= wgt.FindAnyWidget("btn_ppp_sm_mission_load");
		m_BtnSceneRename	= wgt.FindAnyWidget("btn_ppp_sm_mission_rename");
		m_BtnSceneDelete	= wgt.FindAnyWidget("btn_ppp_sm_mission_delete");
		m_BtnSceneDuplicate	= wgt.FindAnyWidget("btn_ppp_sm_mission_duplicate");
		
		m_LstListScenes	= wgt.FindAnyWidget("tls_ppp_sm_scene_list");
		
		m_BtnSave			= wgt.FindAnyWidget("btn_ppp_st_save");
		
		m_TxtWeatherTime	= wgt.FindAnyWidget("txt_ppp_st_w_time_value");
		m_TxtAirTemperature	= wgt.FindAnyWidget("txt_ppp_st_w_air_temp_value");
		
		m_SldStartTime		= wgt.FindAnyWidget("sld_ppp_st_start_time");
		m_TxtStartTimeValue	= wgt.FindAnyWidget("txt_ppp_st_start_time_value");
		
		m_SldStartDay		= wgt.FindAnyWidget("sld_ppp_st_start_day");
		m_TxtStartDayValue	= wgt.FindAnyWidget("txt_ppp_st_start_day_value");
		
		m_SldOvercast		= wgt.FindAnyWidget("sld_ppp_st_overcast");
		m_TxtOvercastValue	= wgt.FindAnyWidget("txt_ppp_st_overcast_value");
		
		m_SldRain			= wgt.FindAnyWidget("sld_ppp_st_rain");
		m_TxtRainValue		= wgt.FindAnyWidget("txt_ppp_st_rain_value");
		
		m_SldFog			= wgt.FindAnyWidget("sld_ppp_st_fog");
		m_TxtFogValue		= wgt.FindAnyWidget("txt_ppp_st_fog_value");
		
		m_SldWindForce		= wgt.FindAnyWidget("sld_ppp_st_wind_force");
		m_TxtWindForceValue	= wgt.FindAnyWidget("txt_ppp_st_wind_force_value");
	}

	//================================================
	// OnClick
	//================================================	
	bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		
		string scene_name;
		PluginSceneManager editor;
		
		if ( w == m_BtnCancel )
		{
			GetGame().GetWorld().SetDate( m_OrigYear, m_OrigMonth, m_OrigDay, m_OrigHour, m_OrigMinute );
			GetGame().SetOvercast( m_OrigOvercast, 0, 1000 );
			GetGame().SetRain( m_OrigRain, 0, 1000 );
			GetGame().SetFog( m_OrigFog, 0, 1000 );
			GetGame().SetWindForce( m_OrigWindForce );
			
			PopupBack();
			
			return true;
		}
		else if ( w == m_BtnSceneNew )
		{
			PopupOpen(SceneEditorMenu.POPUP_ID_SCENE_NEW, NULL);
			
			return true;
		}
		else if ( w == m_BtnSceneLoad )
		{
			if ( m_LstListScenes.GetSelectedRow() != -1 )
			{
				scene_name = m_LstListScenes.GetItem( m_LstListScenes.GetSelectedRow(), 0 );
				editor = GetPlugin(PluginSceneManager);
				
				editor.SceneLoad(scene_name);
				PopupBack();
				
				return true;
			}
		}
		else if ( w == m_BtnSceneRename )
		{
			scene_name = m_LstListScenes.GetItem( m_LstListScenes.GetSelectedRow(), 0 );
			
			UIPopupScriptSceneRename popup_rename = PopupOpen(SceneEditorMenu.POPUP_ID_SCENE_RENAME, NULL);
			popup_rename.SetRenameName(scene_name);
			
			return true;
		}
		else if ( w == m_BtnSceneDuplicate )
		{
			if ( m_LstListScenes.GetSelectedRow() != -1 )
			{
				scene_name = m_LstListScenes.GetItem( m_LstListScenes.GetSelectedRow(), 0 );
				editor = GetPlugin(PluginSceneManager);
				
				editor.SceneDuplicate(scene_name);
				UpdateSceneList();
				
				return true;
			}
		}
		else if ( w == m_BtnSceneDelete )
		{
			if ( m_LstListScenes.GetSelectedRow() != -1 )
			{
				scene_name = m_LstListScenes.GetItem( m_LstListScenes.GetSelectedRow(), 0 );
				editor = GetPlugin(PluginSceneManager);
				
				editor.SceneDelete(scene_name);
				
				if ( editor.SceneCanDelete(scene_name) )
				{
					UIPopupScriptSceneDelete popup_delete = PopupOpen(SceneEditorMenu.POPUP_ID_SCENE_DELETE, NULL);
					popup_delete.SetDeleteName(scene_name);
				}
				else
				{
					UIPopupScriptNotify popup_notify = PopupOpen(SceneEditorMenu.POPUP_ID_NOTIFY, NULL);
					popup_notify.SetLabelText("You cant delete current loaded scene!");
				}			
				
				UpdateSceneList();
				return true;
			}		
		}
		else if ( w == m_BtnSave )
		{		
			m_OrigYear = m_CurrYear;
			m_OrigMonth = m_CurrMonth;
			m_OrigDay = m_CurrDay;
			m_OrigHour = m_CurrHour;
			m_OrigMinute = m_CurrMinute;
			m_OrigOvercast = m_CurrOvercast;
			m_OrigRain = m_CurrRain;
			m_OrigFog = m_CurrFog;
			m_OrigWindForce = m_CurrWindForce;
			
			editor = GetPlugin(PluginSceneManager);
			editor.SetWeather(m_CurrOvercast, m_CurrRain, m_CurrFog, m_CurrWindForce);
			editor.SetDate(m_CurrYear, m_CurrMonth, m_CurrDay, m_CurrHour, m_CurrMinute);
			editor.SceneSave();
			
			PopupBack();
			
			return true;
		}
		
		return false;
	}
	
	bool OnChange(Widget w, int x, int y, bool finished)
	{
		if ( w == m_SldStartTime )
		{		
			float slider_value_start_time = m_SldStartTime.GetCurrent() * 0.01;		
			float start_time_f = slider_value_start_time * 1439;
			int start_time = start_time_f;
			m_CurrHour = start_time / 60;
			m_CurrMinute = start_time % 60;
			
			UpdateSliderStartTime( m_CurrHour, m_CurrMinute );
			
			GetGame().GetWorld().SetDate( m_CurrYear, m_CurrMonth, m_CurrDay, m_CurrHour, m_CurrMinute );
			
			return true;
		}
		else if ( w == m_SldStartDay )
		{		
			float slider_value_start_day = m_SldStartDay.GetCurrent();
			float start_day_f = slider_value_start_day * 3.64 + 1;
			int start_day = start_day_f;
			
			for ( int i = 0; i < 12; i++ )
			{
				int days = m_DaysInMonth[i];
				if ( start_day <= days )
				{
					m_CurrMonth = i+1;
					m_CurrDay = start_day;
					break;
				}
				else
				{
					start_day -= days;
				}
			}
			
			UpdateSliderStartDay( m_CurrMonth, m_CurrDay );
			
			GetGame().GetWorld().SetDate( m_CurrYear, m_CurrMonth, m_CurrDay, m_CurrHour, m_CurrMinute );
			
			return true;
		}
		else if ( w == m_SldOvercast )
		{		
			UpdateSliderOvercast();
			
			m_CurrOvercast = m_SldOvercast.GetCurrent() * 0.01;		
			GetGame().SetOvercast( m_CurrOvercast, 0, 1000 );
			
			return true;
		}
		else if ( w == m_SldRain )
		{		
			UpdateSliderRain();
			
			m_CurrRain = m_SldRain.GetCurrent() * 0.01;		
			GetGame().SetRain( m_CurrRain, 0, 1000 );			
			
			return true;
		}
		else if ( w == m_SldFog )
		{		
			UpdateSliderFog();
			
			m_CurrFog = m_SldFog.GetCurrent() * 0.01;		
			GetGame().SetFog( m_CurrFog, 0, 1000 );			
			
			return true;
		}
		else if ( w == m_SldWindForce )
		{		
			UpdateSliderWindForce();
			
			m_CurrWindForce = m_SldWindForce.GetCurrent() * 0.01;		
			GetGame().SetWindForce( m_CurrWindForce );			
			
			return true;
		}
		
		return false;
	}

	//================================================
	// UpdateSceneList
	//================================================	
	bool UpdateSceneList()
	{
		m_LstListScenes.ClearItems();
		
		PluginSceneManager editor = GetPlugin(PluginSceneManager);
		
		TStringArray scene_list = editor.GetSceneList();
		string loaded_scene_name = editor.SceneGetName();
		
		for ( int i = 0; i < scene_list.Count(); ++i )
		{
			string scene_name = scene_list.Get(i);
			
			m_LstListScenes.AddItem(scene_name, NULL, 0);
			
			if ( scene_name == loaded_scene_name )
			{
				m_LstListScenes.SelectRow(i);
			}
		}
	}

	//================================================
	// UpdateSceneList
	//================================================	
	void OnOpen(Param param)
	{
		//Log("Poup Scene Manager OnOpen");
		
		UpdateSceneList();
		GetGame().GetWorld().GetDate( m_OrigYear, m_OrigMonth, m_OrigDay, m_OrigHour, m_OrigMinute );
		m_OrigOvercast = GetGame().GetOvercast();
		m_OrigRain = GetGame().GetRain();
		m_OrigFog = GetGame().GetFog();
		m_OrigWindForce = GetGame().GetWindForce();
		
		m_CurrYear = m_OrigYear;
		m_CurrMonth = m_OrigMonth;
		m_CurrDay = m_OrigDay;
		m_CurrHour = m_OrigHour;
		m_CurrMinute = m_OrigMinute;
		m_CurrOvercast = m_OrigOvercast;
		m_CurrRain = m_OrigRain;
		m_CurrFog = m_OrigFog;
		m_CurrWindForce = m_OrigWindForce;
		
		Print( "Year" );
		Print( m_CurrYear );
		
		delete m_TimerUpdate;
		m_TimerUpdate = new TimerUpdate(this, "OnUpdate", CALL_CATEGORY_SYSTEM);
		
		ResetSliders();
	}
	
	//================================================
	// OnClose
	//================================================	
	void OnClose()
	{		
		m_TimerUpdate.Stop();
		delete m_TimerUpdate;
	}
	
	//================================================
	// OnUpdate
	//================================================	
	bool OnUpdate()
	{
		m_TxtWeatherTime.SetText(GetGame().GetWeatherTime().ToString());
		m_TxtAirTemperature.SetText(GetGame().GetAirTemperature().ToString());
	}
	
	//================================================
	// ResetSliders
	//================================================	
	void ResetSliders()
	{
		int year, month, day, hour, minute;
		GetGame().GetWorld().GetDate( year, month, day, hour, minute );
		m_SldStartTime.SetCurrent( ((hour * 60) + minute) / 14.39 );
		UpdateSliderStartTime( hour, minute );
		
		float start_day = day;
		int month_tmp = month;
		while ( month_tmp > 1 )
		{
			month_tmp--;
			start_day += m_DaysInMonth[month];
		}
		m_SldStartDay.SetCurrent( start_day / 3.64 );
		UpdateSliderStartDay( month, day );
		
		m_SldOvercast.SetCurrent(GetGame().GetOvercast() * 100);
		UpdateSliderOvercast();
		
		m_SldRain.SetCurrent(GetGame().GetRain() * 100);
		UpdateSliderRain();
		
		m_SldFog.SetCurrent(GetGame().GetFog() * 100);
		UpdateSliderFog();
		
		m_SldWindForce.SetCurrent(GetGame().GetWindForce() * 100);
		UpdateSliderWindForce();
	}
	
	void UpdateSliderStartTime( int hour, int minute )
	{
		string label_text = hour.ToStringLen(2) + ":" + minute.ToStringLen(2);
		m_TxtStartTimeValue.SetText( label_text );
	}

	void UpdateSliderStartDay( int month, int day )
	{
		string label_text = day.ToString() + "." + month.ToString() + ". " + m_CurrYear.ToString();
		m_TxtStartDayValue.SetText( label_text );
	}

	void UpdateSliderOvercast()
	{
		string label_text = m_SldOvercast.GetCurrent().ToString()+"%";
		m_TxtOvercastValue.SetText( label_text );
	}

	void UpdateSliderRain()
	{
		string label_text = m_SldRain.GetCurrent().ToString()+"%";
		m_TxtRainValue.SetText( label_text );
	}

	void UpdateSliderFog()
	{
		string label_text = m_SldFog.GetCurrent().ToString()+"%";
		m_TxtFogValue.SetText( label_text );
	}

	void UpdateSliderWindForce()
	{
		string label_text = m_SldWindForce.GetCurrent().ToString()+"%";
		m_TxtWindForceValue.SetText( label_text );
	}
}
