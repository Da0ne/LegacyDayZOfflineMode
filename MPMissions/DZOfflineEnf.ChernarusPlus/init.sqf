diag_log "init.sqf";
setTimeForScripts 90;
call compile preprocessFileLineNumbers "MissionScripts\PVE\init.sqf";

DZ_MP_CONNECT = true;
DZ_MAX_ZOMBIES = 0;
DZ_MAX_ANIMALS = 150;
DZ_PLAYER_COUNT = 10; //fake player max cap so the stats work 10 is sweet spot, not too fast or slow
//=====================================================================================================

connectedPlayers = [];
waitUntil { player == player };
connectedPlayers set [0,player];

//---------------------------------
_humidity = random 0.8;
[0,0] setOvercast _humidity;
simulWeatherSync;
setTimeForScripts 0.03;

"actionReleased" addPublicVariableEventHandler
{
	private["_agent"];
	_agent = _this select 1;
	[_agent,"You have broken free of your restraints!","ColorFriendly"] call fnc_playerMessage;
	_agent setCaptive false;
	_agent playAction 'PlayerRestrainOut';
	if (count _this > 2) then
	{
		null = (_this select 2) call player_addInventory;
	};
};

_WindMaxSpeed = floor random 7.5;
_WindSpeedOfChange = floor random 50;
_WindFnMin = floor random 0.5;
_WindFnMax = floor random 3.5;

setWindMaxSpeed _WindMaxSpeed;
setWindSpeedOfChange _WindSpeedOfChange;
setWindFnMin _WindFnMin;
setWindFnMax _WindFnMax;

index = 0;
indexTarget = DZ_PLAYER_COUNT;
onEachFrame {	
	if (index < 1) then {	
		if (alive player) then
		{
			player call tick_modifiers;	
			player call tick_states;		
			player call tick_environment;
			player call fnc_sendNotifiersToEnfusion;
		};
	};
	index = index + 1;		
	if (index == indexTarget) then {
		indexTarget = (round(diag_fps * 2)) max DZ_PLAYER_COUNT; //indexTarget = (round(diag_fps * 2)) max DZ_PLAYER_COUNT; //ideal is 25fps to get 2s ticks
		index = 0;
	};
};