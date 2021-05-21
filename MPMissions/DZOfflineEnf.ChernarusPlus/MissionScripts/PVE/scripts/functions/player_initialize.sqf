private["_state","_healthCheck","_fsm"];
/*
	Run on initialization of a player

	Author: Rocket
*/
_agent = _this;

//if (!isPlayer _player) exitWith {};

if (isServer) then
{
	//check if server task manager is running
	if (isNil "DZ_ServerBrain") then
	{
		DZ_ServerBrain = [] execFSM "MissionScripts\PVEscripts\fsm\server_taskmanager.fsm";
	};

	_agent setVariable ["p1",getPosATL _agent];
	_agent setVariable ["prevDamage",0];
	
	//_eh = _agent addEventHandler ["AnimHook",{_this call event_animHook}];
		
	_agent synchronizeVariable ["bleedingsources",0.5,{_this call event_playerBleed}];
	_agent synchronizeVariable ["vomit",1,{null = _this spawn effect_playerVomit}];
	_agent synchronizeVariable ["blood",0.5];
	_agent synchronizeVariable ["health",0.5];
	_agent synchronizeVariable ["shock",0.5];
	_agent synchronizeVariable ["bodytemperature",0.5];
	_agent synchronizeVariable ["heatcomfort",0.5];
	_agent synchronizeVariable ["wet",0.5];
	_agent synchronizeVariable ["prevDamage",05];
	
	//set start time
	_agent setVariable ["starttime",diag_tickTime];
	
	//assign damage values
	{
		_agent setHitPointDamage [(_x select 0),(_x select 1)];
	} forEach (_agent getVariable ["damageArray",[]]);
	
	//assign unconscious state
	if (_agent getVariable ["unconscious",false]) then
	{
		_agent setUnconscious true;
	};
	
	//assign blindness
	/*
	//diag_log [format ["headgear slot: %1, type of: %2",(_agent itemInSlot "headgear"),typeOf (_agent itemInSlot "headgear")],""]; // DEBUG
	if (typeOf (_agent itemInSlot "headgear") == "Cover_BurlapSack") then
	{	
		//diag_log [format ["setting the aperture, fading sound, fading speech"],""]; // DEBUG
		_agent spawnForPlayer {setAperture 10000;1 fadeSound 0.4;1 fadeSpeech 0.4;};
		//_agent spawnForPlayer compile "setAperture 10000;1 fadeSound 0.4;1 fadeSpeech 0.4;";
	};
	*/
	
	//count bleeding
	_coords = call compile (_agent getVariable ["bleedingsources","[]"]);
	_agent setVariable ["bleedingLevel",(count _coords)];
	
	//run player brain
	//_fsm = _agent execFSM "\dz\server\scripts\fsm\brain_player_server.fsm";
	
	_agent setTargetCategory "player";
	
	null = _agent spawn 
	{
		_agent = _this;
		
		while {alive _this} do 
		{
			if (debug) then
			{
				//if (_agent != player) exitWith {};
				_str = format["MODIFIERS:\n\n%1\n\n",_agent];
				_modStates = _agent getVariable ["modstates",[]];
				_i = 0;
				{
					_v = _modStates select 0;
					_stage = _v select 0;
					_dur = _v select 1;
					_cool = _v select 2;
					_str = _str + format["%1: %2, %3, %4",_x,_stage,_dur,_cool];
					_i = _i + 1;
				} forEach (_agent getVariable ["modifiers",[]]);
//oldui 				hintSilent _str;
			};
			
			//Ticks
			//_agent call tick_modifiers;	//records changes in any modifiers
			//_agent call tick_states;		//records changes in any states
			//_agent call tick_environment; //records changes due to environment
			
			//death
			if (_agent getVariable ["health",DZ_HEALTH] <= 0) then
			{
				admin_log format ['%1(uid=%2) DIED Health <= 0', name _agent, getPlayerUID _agent];
				_agent setDamage 1;
			};
			if (_agent getVariable ["blood",DZ_BLOOD] <= 0) then
			{
				admin_log format ['%1(uid=%2) DIED Blood <= 0', name _agent, getPlayerUID _agent];
				_agent setDamage 1;
			};
			
			//exit
			if (isNull _agent) exitWith {};
			
			//wait
			sleep DZ_TICK;
		};
	};
	
	
	//set muscle cramp
	_musclec = _agent getVariable["musclecramp",0];
	_agent SetMuscleCramp _musclec;
	
	//event handlers
	_firedEH = _agent addEventHandler ["fired", {_this spawn event_weaponFired}];

	// EnScript player init function
	_agent callMethod ["OnSqfPlayerInit"];
};
