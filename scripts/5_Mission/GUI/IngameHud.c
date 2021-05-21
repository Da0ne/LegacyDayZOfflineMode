class IngameHud extends Hud
{
	private const float FADE_IN_TIME = 0.3;
	private const float FADE_OUT_TIME = 0.3;
	private const float HIDE_MENU_TIME = 5;

	private autoptr map<int, string>	m_StatesWidgetNames;
	private autoptr map<int, ImageWidget>	m_StatesWidgets;  // [key] ImageWidget

	private autoptr map<int, string>	m_BadgesWidgetNames;
	private autoptr map<int, bool>	m_BadgesWidgetDisplay;
	private autoptr map<int, ImageWidget>	m_BadgesWidgets;  // [key] ImageWidget

	private autoptr map<int, string>	m_VehicleGearTable;

	private Widget m_HudPanelWidget;
	private Widget	m_quickbar_widget;
	private autoptr InventoryQuickbar m_quickbar;
	
	private TextWidget VehiclePanelSpeedValue;
	private TextWidget VehiclePanelGearValue;
	private Widget m_VehiclePanel0;
	
	private Widget m_VehiclePanel;
	private Widget m_Notifiers;
	private Widget m_Badges;
	private autoptr Timer m_hide_timer;
	private autoptr Timer m_vehicle_timer;
	private autoptr Timer m_zeroing_and_weaponmode_timer;
	private autoptr WidgetFadeTimer m_fade_timer_weapon_mode;
	private autoptr WidgetFadeTimer m_fade_timer_zeroing;
	private autoptr WidgetFadeTimer m_fade_timer_zeroing_old;
	private autoptr WidgetFadeTimer m_fade_timer_magazine_name;
	private autoptr WidgetFadeTimer m_fade_timer_weapon_name;
	private autoptr WidgetFadeTimer m_fade_timer_weapon_stats;
	private autoptr WidgetFadeTimer m_fade_timer_quickbar;
	private ProgressBarWidget m_stamina;
	private Widget m_stamina_background;
	private Widget m_presence;
	private Widget m_stanceProne;
	private Widget m_stanceStand;
	private Widget m_stanceCrouch;
	private Widget m_stancePanel;
	private Widget m_PresenceLevel0;
	private Widget m_PresenceLevel1;
	private Widget m_PresenceLevel2;
	private Widget m_PresenceLevel3;
	private Widget m_PresenceLevel4;
	private TextWidget m_Zeroing;
	private Widget m_WeaponStats;
	private TextWidget m_WeaponMode;
	private TextWidget m_ZeroingOld;
	private TextWidget m_WeaponName;
	private TextWidget m_MagazineName;
	private autoptr Timer myTimer;
	private autoptr Timer m_quickbar_timer;
	private autoptr array<WidgetFadeTimer> m_fade_timers = new array<WidgetFadeTimer>;
	
	bool	m_quickbar_state;
	bool faded;
	bool zeroing_key_pressed;
		
	void IngameHud()
	{
		m_fade_timer_weapon_mode = new WidgetFadeTimer;
		m_fade_timer_zeroing = new WidgetFadeTimer;
		 m_fade_timer_zeroing_old = new WidgetFadeTimer;
		 m_fade_timer_magazine_name = new WidgetFadeTimer;
		 m_fade_timer_weapon_name = new WidgetFadeTimer;
		 m_fade_timer_weapon_stats = new WidgetFadeTimer;
		 m_fade_timer_quickbar = new WidgetFadeTimer;
		m_hide_timer = new Timer(CALL_CATEGORY_GUI);
		m_vehicle_timer = new Timer( CALL_CATEGORY_GAMEPLAY );
		m_zeroing_and_weaponmode_timer = new Timer( CALL_CATEGORY_GAMEPLAY );

		m_StatesWidgets = new map<int, ImageWidget>; // [key] widgetName
		m_StatesWidgetNames = new map<int, string>;

		m_BadgesWidgets = new map<int, ImageWidget>; // [key] widgetName
		m_BadgesWidgetNames = new map<int, string>;
		m_BadgesWidgetDisplay = new map<int, bool>;

		m_VehicleGearTable = new map<int, string>;
		m_VehicleGearTable.Set( -1, "" );
		m_VehicleGearTable.Set( GEAR_NEUTRAL, "N" );
		m_VehicleGearTable.Set( GEAR_FIRST, "1" );
		m_VehicleGearTable.Set( GEAR_SECOND, "2" );
		m_VehicleGearTable.Set( GEAR_THIRD, "3" );
		m_VehicleGearTable.Set( GEAR_FOURTH, "4" );
		m_VehicleGearTable.Set( GEAR_FIFTH, "5" );
		m_VehicleGearTable.Set( GEAR_SIXTH, "6" );
		m_VehicleGearTable.Set( GEAR_SEVENTH, "7" );
		m_VehicleGearTable.Set( GEAR_EIGTH, "8" );
		m_VehicleGearTable.Set( GEAR_REVERSE, "R" );
	}
	
	void Init( Widget hud_panel_widget )
	{
		m_HudPanelWidget = hud_panel_widget;
		m_HudPanelWidget.Show( true );

		m_quickbar_timer = new Timer( CALL_CATEGORY_GAMEPLAY );

		// quickbar
		m_quickbar_widget = m_HudPanelWidget.FindAnyWidget("QuickbarGrid");
		m_quickbar_widget.Show( false );
		
		// TEMPORARY HACK!!! player is not present when Hud is being initialized 
		myTimer = new Timer( CALL_CATEGORY_GAMEPLAY );
		myTimer.Run( 1, this, "InitQuickbar" );

		// panels
		m_stamina = m_HudPanelWidget.FindAnyWidget("StaminaBar");
		m_presence = m_HudPanelWidget.FindAnyWidget("PresencePanel");
		m_Badges = hud_panel_widget.FindAnyWidget("BadgesPanel");
		m_Notifiers = m_HudPanelWidget.FindAnyWidget("NotifiersPanel");
		m_VehiclePanel = m_HudPanelWidget.FindAnyWidget("VehiclePanel");
		VehiclePanelSpeedValue = m_HudPanelWidget.FindAnyWidget("SpeedValue0");
		VehiclePanelGearValue = m_HudPanelWidget.FindAnyWidget("SpeedValue1");
		m_VehiclePanel0 = m_HudPanelWidget.FindAnyWidget("VehiclePanel0");
		m_Zeroing = (TextWidget)m_HudPanelWidget.FindAnyWidget("Zeroing");
		m_WeaponMode = (TextWidget)m_HudPanelWidget.FindAnyWidget("WeaponMode");
		m_WeaponStats = (Widget)m_HudPanelWidget.FindAnyWidget("WeaponStats");
		m_ZeroingOld = (TextWidget)m_HudPanelWidget.FindAnyWidget("ZeroingOld");
		m_WeaponName = (TextWidget)m_HudPanelWidget.FindAnyWidget("WeaponName");
		m_MagazineName = (TextWidget)m_HudPanelWidget.FindAnyWidget("MagazineName");
		m_stamina_background = m_HudPanelWidget.FindAnyWidget("StaminaBackground");
		m_stamina_background.Show(true);
		m_stanceProne = m_HudPanelWidget.FindAnyWidget("StanceProne");
		m_stanceCrouch = m_HudPanelWidget.FindAnyWidget("StanceCrouch");
		m_stanceStand = m_HudPanelWidget.FindAnyWidget("StanceStand");
		m_stancePanel = m_HudPanelWidget.FindAnyWidget("StancePanel");
		
		// state notifiers
		m_StatesWidgetNames.Clear();
		m_StatesWidgets.Clear();
		m_StatesWidgetNames.Set( NTFKEY_THIRSTY, "Thirsty" );
		m_StatesWidgetNames.Set( NTFKEY_HUNGRY, "Hungry" );
		m_StatesWidgetNames.Set( NTFKEY_SICK, "Health" );
		m_StatesWidgetNames.Set( NTFKEY_BACTERIA, "Bacteria" );
		m_StatesWidgetNames.Set( NTFKEY_BLEEDISH, "Blood" );
		m_StatesWidgetNames.Set( NTFKEY_FEVERISH, "Temperature" );

		#ifndef NO_GUI
			delete(m_Timer);
			m_Timer = new Timer( CALL_CATEGORY_GAMEPLAY );
			m_Timer.Run(1, this, "RefreshQuickbar", NULL, true ); 
		#endif

		if ( GetDayZGame().IsNewPlayer() )
		{
			m_Notifiers.Show( true );
			// m_Notifiers.SetAlpha( 0 );
			m_Badges.Show( true );
			// m_Badges.SetAlpha( 0 );

			int i = 0;
			int key = 0;
			for ( i = 0; i < m_StatesWidgetNames.Count(); i++ )
			{
				string widget_name = m_StatesWidgetNames.GetElement(i);
				key = m_StatesWidgetNames.GetKey(i);
				ImageWidget w = m_Notifiers.FindAnyWidget( String( "Icon" + widget_name ) );
				m_StatesWidgets.Set( key, w );
				w.Show( true );
				// clear all arrows
				for ( int x = 1; x < 4; x++ )
				{
					w = m_Notifiers.FindAnyWidget( String( widget_name + "ArrowUp" + x.ToString() ) );
					w.Show( false );
					w = m_Notifiers.FindAnyWidget( String( widget_name + "ArrowDown" + x.ToString() ) );
					w.Show( false );
				}
			}

			// badges
			m_BadgesWidgetNames.Clear();
			m_BadgesWidgets.Clear();
			m_BadgesWidgetDisplay.Clear();
			m_BadgesWidgetNames.Set( NTFKEY_FRACTURE, "Fracture" );
			m_BadgesWidgetNames.Set( NTFKEY_STUFFED, "Stomach" );
			m_BadgesWidgetNames.Set( NTFKEY_SICK, "Pill" );
			m_BadgesWidgetNames.Set( NTFKEY_BLEEDISH, "Wetness" );
			m_BadgesWidgetNames.Set( NTFKEY_FEVERISH, "Skull" );
			// NTFKEY_SICK
			// NTFKEY_BLEEDISH
			// NTFKEY_FRACTURE
			// NTFKEY_STUFFED
			// NTFKEY_WETNESS iconDrops

			for ( i = 0; i < m_BadgesWidgetNames.Count(); i++ )
			{
				string badge_name = m_BadgesWidgetNames.GetElement(  i);
				key = m_BadgesWidgetNames.GetKey( i );
				ImageWidget badge_widget = m_Badges.FindAnyWidget( badge_name );
				m_BadgesWidgets.Set( key, badge_widget );
				badge_widget.Show( false );
				m_BadgesWidgetDisplay.Set( key, false );
			}

			m_PresenceLevel0 = hud_panel_widget.FindAnyWidget("Presence0");
			m_PresenceLevel1 = hud_panel_widget.FindAnyWidget("Presence1");
			m_PresenceLevel2 = hud_panel_widget.FindAnyWidget("Presence2");
			m_PresenceLevel3 = hud_panel_widget.FindAnyWidget("Presence3");
			m_PresenceLevel4 = hud_panel_widget.FindAnyWidget("Presence4");
			m_PresenceLevel0.Show( false );
			m_PresenceLevel1.Show( false );
			m_PresenceLevel2.Show( false );
			m_PresenceLevel3.Show( false );
			m_PresenceLevel4.Show( false );
		}
		else
		{
			m_stamina_background.Show( false );
			m_stamina.Show( false );
			m_presence.Show( false );
			m_stanceProne.Show( false );
			m_stanceStand.Show( false );
			m_stanceCrouch.Show( false );
			m_Badges.Show( false );
			m_Notifiers.Show( false );
		}

		#ifndef NO_GUI
			m_vehicle_timer.Run(0.5, this, "RefreshVehicleSpeedHud", NULL, true ); 
		#endif
		
		#ifndef NO_GUI
		if ( GetDayZGame().IsNewPlayer() )
		{
			m_zeroing_and_weaponmode_timer.Run(0.1, this, "RefreshZeroingAndWeaponMode", NULL, true );
		}
		else
		{
			m_zeroing_and_weaponmode_timer.Run(0.1, this, "RefreshZeroingAndWeaponModeOld", NULL, true );
		}
		#endif
	}
	
	void DisplayNotifier( int key, int tendency )
	{
		ImageWidget w = m_Notifiers.FindAnyWidget( String( "Icon" + m_StatesWidgetNames.Get( key ) ) );
		w.Show( true );

		// tendency arrows
		string arrow_name = "ArrowUp";
		if ( tendency < 0 )
		{
			arrow_name = "ArrowDown";
		}
		tendency = Math.AbsInt( tendency );

		for ( int x = 1; x < 4; x++ )
		{ 
			w = m_Notifiers.FindAnyWidget( String(  m_StatesWidgetNames.Get( key ) + "ArrowUp" + x.ToString() ) );
			w.Show( false );
			w = m_Notifiers.FindAnyWidget( String(  m_StatesWidgetNames.Get( key ) + "ArrowDown" + x.ToString() ) );
			w.Show( false );
		}

		for ( int i = 1; i < ( tendency + 1) ; i++ )
		{
			string widget_name = m_StatesWidgetNames.Get( key ) + arrow_name + i.ToString() ;
			w = m_Notifiers.FindAnyWidget( widget_name );
			w.Show( true );
		}
	}
	
	void DisplayBadge( int key, bool show )
	{
		m_BadgesWidgetDisplay.Set( key, show );

		int x = 0;
		for ( int i = 0; i < m_BadgesWidgetDisplay.Count(); i++ )
		{
			int badge_key = m_BadgesWidgetDisplay.GetKey( i );
			string badge_name = m_BadgesWidgetNames.Get( badge_key );
			ImageWidget badge_widget = m_Badges.FindAnyWidget( badge_name );
			if ( badge_widget )
			{
				if ( m_BadgesWidgetDisplay.Get( badge_key ) == true )
				{
					badge_widget.SetPos ( x*0.2, 0.0, true);
					badge_widget.Show( true );
					x = x + 1;
				}
				else
				{
					badge_widget.Show( false );
				}
			}
		}
	}
	
	void SetStamina( int value , int range )
	{
		PlayerBase player = GetGame().GetPlayer();
		if( !player.m_PlayerStats )
		{
			return;
		}
		float max = player.m_PlayerStats.m_Stamina.GetMax();
		float percentage =  range / max;
		m_stamina.SetCurrent( ( value / range ) * max );
		m_stamina.SetSize( percentage, 0.1 );
 		m_stamina_background.SetSize( 1-percentage, 0.1);
	}
	
	void RefreshZeroingAndWeaponModeOld()
	{
		PlayerBase player = GetGame().GetPlayer();
		
		if ( player != NULL )
		{
			EntityAI entity = player.GetEntityInHands();
			
			if ( entity != NULL && entity.IsWeapon() )
			{	
				float zeroing = player.GetCurrentZeroing();
				
				if( zeroing_key_pressed )
				{
					zeroing_key_pressed = false;
					faded = false;
				}
				
				if( !faded )
				{
					m_fade_timer_zeroing_old.FadeOut( m_WeaponStats, 5 );
					m_fade_timer_magazine_name.FadeOut( m_WeaponName, 5 );
					m_fade_timer_weapon_name.FadeOut( m_MagazineName, 5 );
					m_fade_timer_weapon_stats.FadeOut( m_ZeroingOld, 5 );
					faded = true;
				}
				
				if( entity.AttachmentsCount() == 0 )
				{
					m_MagazineName.SetText( "" );
				}
				
				for ( int i = 0; i < entity.AttachmentsCount(); i++ )
				{
					EntityAI attachment = entity.GetAttachmentFromIndex( i );
					if ( !attachment ) continue;
					
					if ( attachment.IsMagazine() )
					{
						m_MagazineName.SetText( attachment.GetName() );
					}
					else
					{
						m_MagazineName.SetText("");
					}
				}
				
				m_WeaponName.SetText( entity.GetName() );
				m_WeaponStats.Show( true );
				m_ZeroingOld.SetText( zeroing.ToString() );
			}
			else
			{
				faded = false;
				m_WeaponStats.Show( false );
			}
		}
	}
	
	void RefreshZeroingAndWeaponMode()
	{
		PlayerBase player = GetGame().GetPlayer();
		
		if ( player != NULL )
		{
			EntityAI entity = player.GetEntityInHands();
			
			if ( entity != NULL && entity.IsWeapon() )
			{	
				float zeroing = player.GetCurrentZeroing();
				string weaponMode = player.GetCurrentWeaponMode();
				
				if( zeroing_key_pressed )
				{
					zeroing_key_pressed = false;
					faded = false;
				}
				
				if( !faded )
				{
					m_fade_timer_weapon_mode.FadeOut(m_WeaponMode, 5);
					m_fade_timer_zeroing.FadeOut(m_Zeroing, 5);
					faded = true;
			}
				
				m_Zeroing.Show(true);
				m_WeaponMode.Show(true);
				m_Zeroing.SetText( zeroing.ToString() );
				m_WeaponMode.SetText( weaponMode );
			}
			else
			{
				faded = false;
				m_WeaponMode.Show(false);
				m_Zeroing.Show(false);
			}
		}
	}

	bool KeyPress(int key)
	{
	}
	
	bool ZeroingKeyPress()
	{
		zeroing_key_pressed = true;
	}
	
	void DisplayStance( int stance )
	{
		if( stance == 1 )
		{
			m_stanceStand.Show(true);
			m_stanceCrouch.Show(false);
			m_stanceProne.Show(false);
		}
		if( stance == 2 )
		{
			m_stanceStand.Show(false);
			m_stanceCrouch.Show(true);
			m_stanceProne.Show(false);
		}
		if( stance == 3 )
		{
			m_stanceStand.Show(false);
			m_stanceCrouch.Show(false);
			m_stanceProne.Show(true);
		}
	}
	
	void DisplayPresence()
	{
		// Log( String( "DisplayPresence" ), LogTemplates.TEMPLATE_JANOSIK );
		PlayerBase player = GetGame().GetPlayer();
		if ( player != NULL )
		{
			// Log( String( "DisplayPresence1" ), LogTemplates.TEMPLATE_JANOSIK );
			float presence_level = player.GetPresenceInAI();
			m_PresenceLevel0.Show( false );
			m_PresenceLevel1.Show( false );
			m_PresenceLevel2.Show( false );
			m_PresenceLevel3.Show( false );
			m_PresenceLevel4.Show( false );

			if ( presence_level > 0 )
			{
				m_PresenceLevel0.Show( true );
			}
			if ( presence_level > 0.2 )
			{
				m_PresenceLevel1.Show( true );
			}
			if ( presence_level > 0.4 )
			{
				m_PresenceLevel2.Show( true );
			}
			if ( presence_level > 0.6 )
			{
				m_PresenceLevel3.Show( true );
			}
			if ( presence_level > 0.8 )
			{
				m_PresenceLevel4.Show( true );
			}
		}
	}
	
	/*void RefreshVehicleHud()
	{
		PlayerBase player = GetGame().GetPlayer();
		if ( player != NULL )
		{
			Transport vehicle = player.GetDrivingVehicle();
			TankOrCar car = (TankOrCar) vehicle;
			if ( vehicle != NULL )
			{
				m_VehiclePanel.Show( true );
				// zatial zakomentovane - bude treba poriesit nejaky per frame refresh system
				// float rpm_value = -90 + ( vehicle.GetRPM() * 100 ) ;
				// Widget RPMPointer = m_HudPanelWidget.FindAnyWidget("rpm_pointer");
				// RPMPointer.SetRotation( 0, 0, rpm_value, true );
				// TextWidget VehiclePanelRPMValue = m_HudPanelWidget.FindAnyWidget("rpm_label");
				// VehiclePanelRPMValue.SetText( ftoa( floor( rpm_value * 100 ) ) );
				
				TextWidget VehiclePanelSpeedValue = m_HudPanelWidget.FindAnyWidget("SpeedValue");
				VehiclePanelSpeedValue.SetText( ftoa( floor( vehicle.GetSpeedometerValue() ) ) );
				
				int engaged_gear = car.GetEngagedGear();
				int prev_gear = engaged_gear - 1;
				int next_gear = engaged_gear + 1;

				if ( engaged_gear == GEAR_NEUTRAL )
				{
					prev_gear = GEAR_REVERSE;
				}
				else if ( engaged_gear == GEAR_REVERSE )
				{
					prev_gear = -1;
					next_gear = GEAR_NEUTRAL;
				}
				
				VehiclePanelSpeedValue = m_HudPanelWidget.FindAnyWidget("Current");
				VehiclePanelSpeedValue.SetText( m_VehicleGearTable.Get( engaged_gear ) );
							
				VehiclePanelSpeedValue = m_HudPanelWidget.FindAnyWidget("Next");
				VehiclePanelSpeedValue.SetText( m_VehicleGearTable.Get( next_gear ) );

				VehiclePanelSpeedValue = m_HudPanelWidget.FindAnyWidget("Prev");
				VehiclePanelSpeedValue.SetText( m_VehicleGearTable.Get( prev_gear ) );
			}
			else
			{
				m_VehiclePanel.Show( false );
			}
		}
	}*/
	
	void RefreshVehicleSpeedHud()
	{
		PlayerBase player = GetGame().GetPlayer();
		if ( player != NULL )
		{
			Transport vehicle = player.GetDrivingVehicle();
			if ( vehicle != NULL && vehicle.IsInherited( TankOrCar ) )
			{
				TankOrCar car = (TankOrCar) vehicle;
				m_VehiclePanel0.Show( true );				
				VehiclePanelSpeedValue.SetText( Math.Floor( vehicle.GetSpeedometerValue() ).ToString() );
				
				int engaged_gear = car.GetEngagedGear();
				VehiclePanelGearValue.SetText( m_VehicleGearTable.Get( engaged_gear ) );
			}
			else
			{
				m_VehiclePanel0.Show( false );
			}
		}
	}
	
	void InitQuickbar()
	{
		if (m_quickbar == NULL)
		{
			m_quickbar = new InventoryQuickbar( m_quickbar_widget );
			ShowQuickbar();
		}
	}
	
	InventoryQuickbar GetQuickbar()
	{
		return m_quickbar;
	}
	
	void ShowQuickbar()
	{
		if(!GetGame().GetPlayer())
		{
			return;
		}
		
		if ( m_quickbar_widget && GetGame().GetPlayer().GetQuickBarSize() != 0 )
		{
				InventoryGrid quickbarGrid;
				m_quickbar_widget.GetScript(quickbarGrid);
				Widget child = quickbarGrid.GetRoot().GetChildren();
				float alpha = quickbarGrid.GetRoot().GetAlpha();
				RefreshQuickbar();

				for ( int i = 0; i < m_fade_timers.Count(); i++)
				{
					WidgetFadeTimer timer = m_fade_timers.Get(i);
					timer.Stop();
					delete timer;
				}
				m_fade_timers.Clear();
				
				m_quickbar_widget.Show( true );
				m_quickbar_state = true;
				
				while (child)
				{
					child.Show(true);
					child.GetChildren().Show(true);
					child.SetAlpha(alpha);
					child.GetChildren().SetAlpha(1);
					child = child.GetSibling();
				}
				m_quickbar_timer.Stop();
				m_quickbar_timer.Run( 45, this, "HideQuickbar" );
		}
	}
	
	void HideQuickbar( bool ignore_state = false , bool instant_hide = false )
	{
		if ( m_quickbar_widget )
		{
			InventoryGrid quickbarGrid;
			m_quickbar_widget.GetScript(quickbarGrid);
			
			if( !instant_hide )
			{
				Widget child = quickbarGrid.GetRoot().GetChildren();
				while (child)
				{
					WidgetFadeTimer m_fade_timer_quickbar = new WidgetFadeTimer;
					m_fade_timer_quickbar.FadeOut(child, 5, true);
					m_fade_timers.Insert(m_fade_timer_quickbar);
					child = child.GetSibling();
				}
			}
			else
			{
				m_quickbar_widget.Show( false );
			}
			
			if ( ! ignore_state )
			{
				m_quickbar_state = false;
			}
			m_quickbar_timer.Stop();
		}
	}
	
	void SetLeftStatsVisibility(bool visible)
	{
		/*m_stamina_background.Show(visible);
		m_stamina.Show(visible);
		m_stancePanel.Show(visible);
		m_presence.Show(visible);*/
	}
	
	void RefreshQuickbar()
	{
		if ( m_quickbar ) 
		{
			m_quickbar.UpdateItems( m_quickbar_widget );
		}
	}
}
