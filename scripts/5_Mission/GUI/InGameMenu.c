class InGameMenu extends UIScriptedMenu
{
	void InGameMenu()
	{
		//nic
	}

	void ~InGameMenu()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_GUI).RemoveCalls(this);
	}
	
	Widget Init()
	{
		layoutRoot = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_ingamemenu.layout");
		
		string version;
		GetGame().GetVersion(version);
		
		TextWidget version_widget = layoutRoot.FindAnyWidget("VersionTextWidget");
		version_widget.SetText("#main_menu_version " + version);

		ButtonWidget restart_btn = layoutRoot.FindAnyWidgetById(IDC_INT_RETRY);
		
		Man player = GetGame().GetPlayer();
		bool player_is_alive = false;
		if (player)
		{
			int life_state = player.GetLifeState();

			if (life_state == LifeStateAlive)
			{
				player_is_alive = true;
			}
		}
		//respawn
		restart_btn.SetText("#main_menu_respawn");
		restart_btn.Enable(GetGame().GetPlayer().GetLifeState() == LifeStateUnconscious || !player_is_alive);
		
		return layoutRoot;
	}

	bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		
		Mission mission = GetGame().GetMission();

		switch (w.GetUserID())
		{
		case IDC_MAIN_CONTINUE:
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(GetGame().GetMission(), "Continue");
			return true;
			
		case IDC_MAIN_OPTIONS:
			EnterScriptedMenu(MENU_OPTIONS);
			return true;

		case IDC_MAIN_QUIT:
			GetGame().GetUIManager().ShowDialog("EXIT", "Are you sure you want to exit?", IDC_INT_EXIT, DBT_YESNO, DBB_YES, DMT_QUESTION, NULL);
			//Save player incase exit
			Man player = GetGame().GetPlayer();
			int life_state = 0;
			if (player)
			{
				if (player.GetLifeState() == LifeStateAlive)
				{
					life_state = 1;
				}else{
					life_state = 0;
				}
			}
			GetGame().ExecuteSQF("[" + life_state +", 0, player] call dbSavePlayer;"); //lifestate, ID, player
			GetGame().ExecuteSQF("[player,\"Player progress saved to file!\",\"ColorFriendly\"] call fnc_playerMessage;");
			return true;

		case IDC_INT_RETRY:
			if (!GetGame().IsMultiplayer())
			{
				//Kill older char
				GetGame().ExecuteSQF("[0, 0, player] call dbSavePlayer;"); //lifestate, ID, player

				string SQF = "player setDamage 1; ";
				SQF += "player = createAgent [DZ_SkinsArray select floor(random(count DZ_SkinsArray)), position player, [], 0, 'NONE']; ";
				SQF += "selectPlayer player; ";
				SQF += "player call player_initialize; ";
				SQF += "player call init_newPlayer; ";
				SQF += "nul = player execFSM \"MissionScripts\\PVE\\scripts\\fsm\\brain_player_client.fsm\"; ";

				SQF += "player addEventHandler ['HandleDamage',{_this call event_assessDamage} ]; ";
				SQF += "player addEventHandler ['ItemAttached',{_this call event_conatinerAttachedToPlayer} ]; ";
				SQF += "player addEventHandler ['ItemDetached',{_this call event_conatinerDetachedFromPlayer} ]; ";
				SQF += "player addEventHandler ['InventoryIn',{_this call event_itemInContainer} ]; ";
				SQF += "player addEventHandler ['InventoryOut',{_this call event_itemOutOfContainer} ]; ";
				SQF += "player call calc_heatweight; ";
				SQF += "myNotifiers = player getVariable ['myNotifiers',[]]; ";
				SQF += "0 publicVariableClient 'myNotifiers'; ";

				string sSQF = "_top = getArray(configFile >> \"cfgCharacterCreation\" >> \"top\"); ";
				sSQF += "_bottom = getArray(configFile >> \"cfgCharacterCreation\" >> \"bottom\"); ";
				sSQF += "_shoe = getArray(configFile >> \"cfgCharacterCreation\" >> \"shoe\"); ";
				sSQF += "_myTop = _top select floor(random(count _top)); ";
				sSQF += "_myBottom = _bottom select floor(random(count _bottom)); ";
				sSQF += "_myShoe = _shoe select floor(random(count _shoe)); ";
				sSQF += "{null = player createInInventory _x} forEach [_myTop,_myBottom,_myShoe]; ";
				sSQF += "_v = player createInInventory \"Consumable_Roadflare\"; ";
				sSQF += "_v = player createInInventory \"Consumable_Rags\"; ";
				sSQF += "_v setQuantity 1; ";

				GetGame().ExecuteSQF(SQF);
				GetGame().ExecuteSQF(sSQF);
				//Position
				GetGame().ExecuteSQF("_pos = DZ_SPAWN_POINTS select (floor random (count DZ_SPAWN_POINTS)); player setPos _pos;");
				
				MissionGameplay ms = (MissionGameplay)GetGame().GetMission();
				ms.OnMissionFinish();
				ms.OnInit();
				ms.OnMissionStart();
			}
			else
			{
				GetGame().GetUIManager().ShowDialog("#main_menu_respawn", "#main_menu_respawn_question", 0, DBT_YESNO, DBB_YES, DMT_QUESTION, this);
			}
			return true;
		}

		return false;
	}
	
	bool OnModalResult(Widget w, int x, int y, int code, int result)
	{
		super.OnModalResult(w, x, y, code, result);
		
		if ( code == IDC_INT_EXIT && result == DBB_YES )
		{
			GetGame().RequestExit(0);
		}
		else if (result == DBB_YES && GetGame().IsMultiplayer())
		{
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(GetGame(), "RespawnPlayer");
			GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(GetGame().GetMission(), "Continue");
		}
	}
}
