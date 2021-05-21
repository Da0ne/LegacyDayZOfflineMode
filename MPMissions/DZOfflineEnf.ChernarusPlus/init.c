void main()
{
	//INIT WEATHER BEFORE ECONOMY INIT------------------------
	g_Game.SetOvercast(Math.RandomFloatInclusive(0.1, 1.0), Math.RandomInt(60, 1800), Math.RandomInt(60, 1800));
	g_Game.SetRain(Math.RandomFloatInclusive(0.1, 1.0), Math.RandomInt(60, 1800), Math.RandomInt(60, 1800));
	g_Game.SetFog(Math.RandomFloatInclusive(0.1, 1.0), Math.RandomInt(60, 1800), Math.RandomInt(60, 1800));

	//DATE RESET AFTER ECONOMY INIT-------------------------
	int year, month, day, hour, minute;
	int reset_month = 9, reset_day = 20;
	GetGame().GetWorld().GetDate(year, month, day, hour, minute);

	if ((month == reset_month) && (day < reset_day))
	{
		GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
	}
	else
	{
		if ((month == reset_month + 1) && (day > reset_day))
		{
			GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
		}
		else
		{
			if ((month < reset_month) || (month > reset_month + 1))
			{
				GetGame().GetWorld().SetDate(year, reset_month, reset_day, hour, minute);
			}
		}
	}
	Print("0.62 Offline is alive! " + GetGame().GetMission());

	//Compile needed scripts
	GetGame().ExecuteSQF("dbSavePlayerPrep = compile preprocessFileLineNumbers \"MissionScripts\\PVE\\scripts\\functions\\playerdb\\dbSavePlayerPrep.sqf\";");
	GetGame().ExecuteSQF("dbSavePlayer = compile preprocessFileLineNumbers \"MissionScripts\\PVE\\scripts\\functions\\playerdb\\dbSavePlayer.sqf\";");
	GetGame().ExecuteSQF("fnc_reloadWeaponOnSpawn = compile preprocessFileLineNumbers \"MissionScripts\\PVE\\scripts\\functions\\playerdb\\fnc_reloadWeaponOnSpawn.sqf\";");
	GetGame().ExecuteSQF("fnc_addHandsItem = compile preprocessFileLineNumbers \"MissionScripts\\PVE\\scripts\\functions\\playerdb\\fnc_addHandsItem.sqf\";");
	GetGame().ExecuteSQF("fnc_addQuickBarItems = compile preprocessFileLineNumbers \"MissionScripts\\PVE\\scripts\\functions\\playerdb\\fnc_addQuickBarItems.sqf\";");
	GetGame().ExecuteSQF("fnc_addItemState = compile preprocessFileLineNumbers \"MissionScripts\\PVE\\scripts\\functions\\playerdb\\fnc_addItemState.sqf\";");
	GetGame().ExecuteSQF("fnc_getItemState = compile preprocessFileLineNumbers \"MissionScripts\\PVE\\scripts\\functions\\playerdb\\fnc_getItemState.sqf\";");
	GetGame().ExecuteSQF("fnc_getInvItems = compile preprocessFileLineNumbers \"MissionScripts\\PVE\\scripts\\functions\\playerdb\\fnc_getInvItems.sqf\";");
	GetGame().ExecuteSQF("fnc_addInvItems = compile preprocessFileLineNumbers \"MissionScripts\\PVE\\scripts\\functions\\playerdb\\fnc_addInvItems.sqf\";");
	GetGame().ExecuteSQF("fnc_dbDestroyProfile = compile preprocessFileLineNumbers \"MissionScripts\\PVE\\scripts\\functions\\playerdb\\fnc_dbDestroyProfile.sqf\";");
	GetGame().ExecuteSQF("fnc_dbFindInProfile = compile preprocessFileLineNumbers \"MissionScripts\\PVE\\scripts\\functions\\playerdb\\fnc_dbFindInProfile.sqf\";");
	GetGame().ExecuteSQF("fnc_dbLoadFromProfile = compile preprocessFileLineNumbers \"MissionScripts\\PVE\\scripts\\functions\\playerdb\\fnc_dbLoadFromProfile.sqf\";");
	GetGame().ExecuteSQF("fnc_dbSaveToProfile = compile preprocessFileLineNumbers \"MissionScripts\\PVE\\scripts\\functions\\playerdb\\fnc_dbSaveToProfile.sqf\";");
	GetGame().ExecuteSQF("fnc_dbCreateCharInProfile = compile preprocessFileLineNumbers \"MissionScripts\\PVE\\scripts\\functions\\playerdb\\fnc_dbCreateCharInProfile.sqf\";");

	string SQF = "";
	//check saved player
	SQF += "nul = [] spawn { ";
	SQF += "_savedChar = 0 call fnc_dbFindInProfile; ";
	SQF += "_isAlive   = _savedChar select 0; ";
	SQF += "_pos 	   = _savedChar select 2; ";

	SQF += "diag_log format[\"Saved player data: _isAlive: %1 _pos: %2 _savedChar: %3 \",_isAlive,_pos,_savedChar]; ";
	
	SQF += "if (_isAlive) then ";
	SQF += "{ ";
	SQF += 	"player = 0 call fnc_dbLoadFromProfile; ";
	SQF += 	"null = player call fnc_reloadWeaponOnSpawn; ";					
	SQF += 	"null = [player,call compile callFunction [\"DataBaseRead\",\"QUICKBAR\",format[\"UID_%1\",0]]] call fnc_addQuickBarItems; ";
	SQF +=  "player setPos _pos;";
	SQF += "}else{ ";
	SQF += "_top = getArray(configFile >> \"cfgCharacterCreation\" >> \"top\"); ";
	SQF += "_bottom = getArray(configFile >> \"cfgCharacterCreation\" >> \"bottom\"); ";
	SQF += "_shoe = getArray(configFile >> \"cfgCharacterCreation\" >> \"shoe\"); ";
	SQF += "_myTop = _top select floor(random(count _top)); ";
	SQF += "_myBottom = _bottom select floor(random(count _bottom)); ";
	SQF += "_myShoe = _shoe select floor(random(count _shoe)); ";
	SQF += "{null = player createInInventory _x} forEach [_myTop,_myBottom,_myShoe]; ";
	SQF += "_v = player createInInventory \"Consumable_Roadflare\"; ";
	SQF += "_v = player createInInventory \"Consumable_Rags\"; ";
	SQF += "_v setQuantity 1; ";
	SQF += "_pos = DZ_SPAWN_POINTS select (floor random (count DZ_SPAWN_POINTS)); player setPos _pos; ";
	SQF += "}; ";
	SQF += "}; ";

	GetGame().ExecuteSQF(SQF);

	//init part
	string intSQF = "";
	intSQF += "player call player_initialize; ";
	intSQF += "player call init_newPlayer; ";

	intSQF += "player addEventHandler ['HandleDamage',{_this call event_assessDamage} ]; ";
	intSQF += "player addEventHandler ['ItemAttached',{_this call event_conatinerAttachedToPlayer} ]; ";
	intSQF += "player addEventHandler ['ItemDetached',{_this call event_conatinerDetachedFromPlayer} ]; ";
	intSQF += "player addEventHandler ['InventoryIn',{_this call event_itemInContainer} ]; ";
	intSQF += "player addEventHandler ['InventoryOut',{_this call event_itemOutOfContainer} ]; ";
	intSQF += "player call calc_heatweight; ";
	intSQF += "myNotifiers = player getVariable ['myNotifiers',[]]; ";
	intSQF += "0 publicVariableClient 'myNotifiers'; ";
	intSQF += "selectPlayer player; "; 
	intSQF += "nul = player execFSM \"MissionScripts\\PVE\\scripts\\fsm\\brain_player_client.fsm\"; ";
	GetGame().ExecuteSQF(intSQF);

	if (!g_Game.m_OfflineCELoaded)
	{
		GetGame().ExecuteSQF("diag_log 'Load CLE';");
		GetGame().ExecuteSQF("dbInitEconomy [true];");
		g_Game.m_OfflineCELoaded = true;
	}
};