DZ_TotalZombies = 0;
DZ_TotalAnimals = 0;
DZ_TotalEvents = 0;

playerBodies = [];

windChill = 0;
worldLightScale = 0;
windSpeed = 0;

debug = false;

DZ_PlayerHitpoints = [];
_hitPoints = configfile >> "CfgVehicles" >> "ManBase" >> "HitPoints";
for "_i" from 0 to (count _hitPoints - 1) do { 
	DZ_PlayerHitpoints set [count DZ_PlayerHitpoints,configName(_hitPoints select _i)];
};

//events
event_saySound =		compile preprocessFileLineNumbers "\dz\server\scripts\events\event_saySound.sqf";
event_weaponFired =		compile preprocessFileLineNumbers "\dz\server\scripts\events\event_weaponFired.sqf";
event_craftMessage = 	compile preprocessFileLineNumbers "\dz\server\scripts\events\event_craftMessage.sqf";
event_modifier = 		compile preprocessFileLineNumbers "\dz\server\scripts\events\event_modifier.sqf";
event_assessDamage = 	compile preprocessFileLineNumbers "MissionScripts\PVE\scripts\events\event_assessDamage.sqf";
event_transferModifiers = 	compile preprocessFileLineNumbers "\dz\server\scripts\events\event_transferModifiers.sqf";
event_igniteObject = 	compile preprocessFileLineNumbers "\dz\server\scripts\events\event_igniteObject2.sqf";
event_clearModifiers = 	compile preprocessFileLineNumbers "\dz\server\scripts\events\event_clearModifiers.sqf";
event_playerKilled = 	compile preprocessFileLineNumbers "\dz\server\scripts\events\event_playerKilled.sqf";
event_hitZombie =		compile preprocessFileLineNumbers "\dz\server\scripts\events\event_hitZombie.sqf";
event_bloodTransfusion = compile preprocessFileLineNumbers "\dz\server\scripts\events\event_bloodTransfusion.sqf";
event_killedWildAnimal = compile preprocessFileLineNumbers "\dz\server\scripts\events\event_killedWildAnimal.sqf";
event_gasLight = compile preprocessFileLineNumbers "\dz\server\scripts\events\event_gasLight.sqf";
event_openCan = compile preprocessFileLineNumbers "\dz\server\scripts\events\event_openCan.sqf";
event_vehicleDamaged = compile preprocessFileLineNumbers "\dz\server\scripts\events\event_vehicleDamaged.sqf";

//Custom zombie spawn system
[] execVM "\dz\server\scripts\events\zombieSpawns.sqf";  // Dont touch

//CUSTOM DATABASE PLAYER SAVE FUNCTIONS
player_initialize = 	compile preprocessFileLineNumbers "MissionScripts\PVE\scripts\functions\player_initialize.sqf";

//players
player_colorpelt = compile preprocessFileLineNumbers "\dz\server\scripts\players\player_colorpelt.sqf";
player_barrelcheck = compile preprocessFileLineNumbers "\dz\server\scripts\players\player_barrelcheck.sqf";
player_craftfertilizer = compile preprocessFileLineNumbers "\dz\server\scripts\players\player_craftfertilizer.sqf";
player_bleachitem = compile preprocessFileLineNumbers "\dz\server\scripts\players\player_bleachitem.sqf";
player_tanpelt = 	compile preprocessFileLineNumbers "\dz\server\scripts\players\player_tanpelt.sqf";
player_coloritem = 	compile preprocessFileLineNumbers "\dz\server\scripts\players\player_coloritem.sqf";
player_combineQuantity = 	compile preprocessFileLineNumbers "\dz\server\scripts\players\player_combineQuantity.sqf";
player_splitQuantity = 	compile preprocessFileLineNumbers "\dz\server\scripts\players\player_splitQuantity.sqf";
player_useItem =		compile preprocessFileLineNumbers "\dz\server\scripts\players\player_useItem.sqf";
player_useItemEnd =		compile preprocessFileLineNumbers "\dz\server\scripts\players\player_useItemEnd.sqf";
player_fillBottle =		compile preprocessFileLineNumbers "\dz\server\scripts\players\player_fillBottle.sqf";
player_stripMagazine = 	compile preprocessFileLineNumbers "\dz\server\scripts\players\player_stripMagazine.sqf";
player_loadMagazine = 	compile preprocessFileLineNumbers "\dz\server\scripts\players\player_loadMagazine.sqf";
player_addInventory = 	compile preprocessFileLineNumbers "\dz\server\scripts\players\player_addInventory.sqf";
player_actionOnTarget = compile preprocessFileLineNumbers "\dz\server\scripts\players\player_actionOnTarget.sqf";
player_actionOnSelf = 	compile preprocessFileLineNumbers "\dz\server\scripts\players\player_actionOnSelf.sqf";
player_actionOnItem = 	compile preprocessFileLineNumbers "\dz\server\scripts\players\player_actionOnItem.sqf";
player_combineMoney = 	compile preprocessFileLineNumbers "\dz\server\scripts\players\player_combineMoney.sqf";
player_applyBandage = 	compile preprocessFileLineNumbers "\dz\server\scripts\players\player_applyBandage.sqf";
player_chamberRound = 	compile preprocessFileLineNumbers "\dz\server\scripts\players\player_chamberRound.sqf";
player_loadWeapon = 	compile preprocessFileLineNumbers "\dz\server\scripts\players\player_loadWeapon.sqf";
player_pickBerry =		compile preprocessFileLineNumbers "\dz\server\scripts\players\player_pickBerry.sqf";
player_moveToInventory =	compile preprocessFileLineNumbers "\dz\server\scripts\players\player_moveToInventory.sqf";
player_applyDefibrillator = 	compile preprocessFileLineNumbers "\dz\server\scripts\players\player_applyDefibrillator.sqf";
player_mendItem = compile preprocessFileLineNumbers "\dz\server\scripts\players\player_mendItem.sqf";
player_igniteFireplace = compile preprocessFileLineNumbers "\dz\server\scripts\players\player_igniteFireplace.sqf";
player_RabbitSnareTrap = compile preprocessFileLineNumbers "\dz\server\scripts\players\player_RabbitSnareTrap.sqf";
player_bearTrap		 = compile preprocessFileLineNumbers "\dz\server\scripts\players\player_bearTrap.sqf";
player_landMine		 = compile preprocessFileLineNumbers "\dz\server\scripts\players\player_landMine.sqf";
player_plantStages = 		compile preprocessFileLineNumbers "\dz\server\scripts\players\player_plantStages.sqf";

player_digGreenhouse =	compile preprocessFileLineNumbers "\dz\server\scripts\players\player_digGreenhouse.sqf";

player_dryClothes = compile preprocessFileLineNumbers "\dz\server\scripts\players\player_dryClothes.sqf";
player_gutDeadBodyCheck = compile preprocessFileLineNumbers "\dz\server\scripts\players\player_gutDeadBodyCheck.sqf";
player_gutDeadBodyStart = compile preprocessFileLineNumbers "\dz\server\scripts\players\player_gutDeadBodyStart.sqf";
player_gutDeadBodyEnd = compile preprocessFileLineNumbers "\dz\server\scripts\players\player_gutDeadBodyEnd.sqf";
player_gutDeadBodyCancel = compile preprocessFileLineNumbers "\dz\server\scripts\players\player_gutDeadBodyCancel.sqf";
player_useDuctTapeOnItem = compile preprocessFileLineNumbers "\dz\server\scripts\players\player_useDuctTapeOnItem.sqf";

player_DigGardenPlot = compile preprocessFileLineNumbers "\dz\server\scripts\players\player_DigGardenPlot.sqf";

player_liquidSource = compile preprocessFileLineNumbers "\dz\server\scripts\players\player_liquidSource.sqf";
player_liquidConsumed = compile preprocessFileLineNumbers "\dz\server\scripts\players\player_liquidConsumed.sqf";
player_tanPeltInAction = compile preprocessFileLineNumbers "\dz\server\scripts\players\player_tanPeltInAction.sqf";
player_sewLeatherClothing = compile preprocessFileLineNumbers "\dz\server\scripts\players\player_sewLeatherClothing.sqf"; 
player_catchRain = compile preprocessFileLineNumbers "\dz\server\scripts\players\player_catchRain.sqf"; 

//cooking
cooking_cookingProcess = compile preprocessFileLineNumbers 	"\dz\server\scripts\cooking\cooking_cookingProcess.sqf";
cooking_processFood = compile preprocessFileLineNumbers 	"\dz\server\scripts\cooking\cooking_processFood.sqf";
cooking_manageActions = compile preprocessFileLineNumbers	"\dz\server\scripts\cooking\cooking_manageActions.sqf";
//fireplace
fireplace_manageFire = compile preprocessFileLineNumbers 	"\dz\server\scripts\cooking\fireplace_manageFire.sqf";
fireplace_manageFuel = compile preprocessFileLineNumbers 	"\dz\server\scripts\cooking\fireplace_manageFuel.sqf";
fireplace_manageState = compile preprocessFileLineNumbers 	"\dz\server\scripts\cooking\fireplace_manageState.sqf";
fireplace_manageActions = compile preprocessFileLineNumbers	"\dz\server\scripts\cooking\fireplace_manageActions.sqf";
//gascooker
gascooker_manageActions = compile preprocessFileLineNumbers	"\dz\server\scripts\cooking\gascooker_manageActions.sqf";
gascooker_manageFire = compile preprocessFileLineNumbers	"\dz\server\scripts\cooking\gascooker_manageFire.sqf";

//weapons
weapon_swapHandguard =compile preprocessFileLineNumbers "\dz\server\scripts\weapons\weapon_swapHandguard.sqf";

//server
building_spawnLoot =	compile preprocessFileLineNumbers "\dz\server\scripts\server\building_spawnLoot.sqf";
init_spawnLoot = 		compile preprocessFileLineNumbers "\dz\server\scripts\init\spawnLoot.sqf";
init_spawnZombies = 	compile preprocessFileLineNumbers "\dz\server\scripts\init\spawnZombies.sqf";
init_spawnWildAnimals = 	compile preprocessFileLineNumbers "\dz\server\scripts\init\spawnWildAnimals.sqf";
//init_spawnServerEvent = 	compile preprocessFileLineNumbers "\dz\server\scripts\init\spawnServerEvent.sqf";
player_queued = 		compile preprocessFileLineNumbers "\dz\modulesDayZ\scripts\player_queued.sqf";

//functions
fnc_generateTooltip = 	compile preprocessFileLineNumbers "\dz\modulesDayZ\scripts\fn_generateTooltip.sqf";
fnc_inString = 			compile preprocessFileLineNumbers "\dz\server\scripts\functions\fn_inString.sqf";
fnc_inAngleSector =  	compile preprocessFileLineNumbers "\dz\server\scripts\functions\fn_inAngleSector.sqf";
fnc_isMaxQuantity = 	compile preprocessFileLineNumbers "\dz\server\scripts\functions\isMaxQuantity.sqf";
BIS_fnc_findSafePos =	compile preprocessFileLineNumbers "\dz\server\scripts\functions\fn_findSafePos.sqf";
fnc_generateQuantity =	compile preprocessFileLineNumbers "\dz\server\scripts\functions\fn_generateQuantity.sqf";
dayz_losCheck =			compile preprocessFileLineNumbers "\dz\server\scripts\functions\dayz_losCheck.sqf";
dayz_losChance = 		compile preprocessFileLineNumbers "\dz\server\scripts\functions\dayz_losChance.sqf";
dayz_bulletHit = 		compile preprocessFileLineNumbers "\dz\modulesDayZ\scripts\dayz_bulletHit.sqf";
fnc_playerMessage =		compile preprocessFileLineNumbers "\dz\modulesDayZ\scripts\fn_playerMessage.sqf";
runZombieBrain =		compile preprocessFileLineNumbers "\dz\server\scripts\functions\runZombieBrain.sqf";
tick_modifiers =		compile preprocessFileLineNumbers "\dz\server\scripts\functions\tick_modifiers.sqf";
tick_states =			compile preprocessFileLineNumbers "\dz\server\scripts\functions\tick_states.sqf";
tick_environment = 		compile preprocessFileLineNumbers "\dz\server\scripts\functions\tick_environment.sqf";
randomValue =			compile preprocessFileLineNumbers "\dz\modulesDayZ\scripts\randomValue.sqf";
world_surfaceNoise = 	compile preprocessFileLineNumbers "\dz\server\scripts\functions\fn_surfaceNoise.sqf";
farmingFunctions =		[] execVM "\dz\server\scripts\functions\farming.sqf";
cutoutSeedsFunctions = 	[] execVM "\dz\server\scripts\functions\cutoutSeedsFunctions.sqf";

//initialize
zombie_initialize = 	compile preprocessFileLineNumbers "\dz\server\scripts\init\zombie_initialize.sqf";
init_battery = 		compile preprocessFileLineNumbers "\dz\server\scripts\init\battery_initialize.sqf";
init_flare = 		compile preprocessFileLineNumbers "\dz\server\scripts\init\flare_initialize.sqf";
init_torch = 		compile preprocessFileLineNumbers "\dz\server\scripts\init\torch_initialize.sqf";
init_wreck = 		compile preprocessFileLineNumbers "\dz\server\scripts\init\wreck_initialize.sqf";
init_lamp =
{
	if (isServer) then
	{
		_this spawn {
			//_this synchronizeVariable ["light",1,{_this call event_fnc_gasLight}];	//not needed
		};
	};
};

init_newPlayer =
{
	//establish default variables
_this setVariable ["health",DZ_HEALTH];
_this setVariable ["blood",DZ_BLOOD];
_this setVariable ["energy",DZ_ENERGY];
_this setVariable ["water",DZ_WATER];
_this setVariable ["stomach",DZ_STOMACH];
_this setVariable ["diet",DZ_DIET];
_this setVariable ["bodytemperature",DZ_TEMPERATURE];
_this setVariable ["heatComfort",DZ_HEATCOMFORT];
_this setVariable ["wet",0];
_this setVariable ["musclecramp",DZ_MUSCLECRAMP];
_this setVariable ["restrainedwith",0];
_this setVariable ["totalHeatIsolation",DZ_TOTALHEATISOLATION];
_this setVariable ["totalWeight",DZ_TOTALWEIGHT];
_this setVariable ["totalKills",0];
[0,_this,'HitLegs'] call event_modifier; //leg fracture check

//get blood type
_bloodTypes = getArray (configFile >> "cfgSolutions" >> "bloodTypes"); 
_rand = random 1; 
_newType = ""; 
{ 
	_chance = getNumber (configFile >> "cfgSolutions" >> _x >> "probabilityScale"); 
	if (_rand < _chance) exitWith  
	{ 
		_newType = _x; 
	}; 
} forEach _bloodTypes;	

_this setVariable ["bloodtype",_newType];

};

init_newBody =
{
	//manage player body
playerBodies set [count playerBodies,_agent];
_agent setVariable ["UID",_uid];
diag_log format["Recording player %1 with %2",name _agent,_uid];

//move player into body
_id selectPlayer _agent;

_agent call player_initialize;

[_id] spawnForClient compile "player execFSM '\dz\modulesDayZ\fsm\brain_player_client.fsm'";

_agent addEventHandler ["HandleDamage",{_this call event_assessDamage} ];
	
//attach inventory handlers
_agent addEventHandler ["ItemAttached",{_this call event_conatinerAttachedToPlayer} ];
_agent addEventHandler ["ItemDetached",{_this call event_conatinerDetachedFromPlayer} ];

_agent addEventHandler ["InventoryIn",{_this call event_itemInContainer} ];
_agent addEventHandler ["InventoryOut",{_this call event_itemOutOfContainer} ];

_agent call calc_heatweight;

myNotifiers = _agent getVariable ["myNotifiers",[]];

_id publicVariableClient "myNotifiers";	


};

//calculates total weight of items and heat isolation for character loaded from db
calc_heatweight = {
	
	_agent = _this;
	_totalWeight = 0;
	_totalHeatIsolation = 0;	
	_inventorylist = itemsInInventory _agent;
	
	{
		_totalWeight = _totalWeight + (getNumber (configFile >> "cfgVehicles" >> typeOf _x >> "weight"));
		
	} forEach _inventorylist;
	
	_totalHeatIsolation= _totalHeatIsolation + (getNumber (configFile >> "cfgVehicles" >> typeOf (_agent itemInSlot 'Back') >> "heatIsolation"));
	_totalHeatIsolation= _totalHeatIsolation + (getNumber (configFile >> "cfgVehicles" >> typeOf (_agent itemInSlot 'Eyewear') >> "heatIsolation"));
	_totalHeatIsolation= _totalHeatIsolation + (getNumber (configFile >> "cfgVehicles" >> typeOf (_agent itemInSlot 'Headgear') >> "heatIsolation"));
	_totalHeatIsolation= _totalHeatIsolation + (getNumber (configFile >> "cfgVehicles" >> typeOf (_agent itemInSlot 'Mask') >> "heatIsolation"));
	_totalHeatIsolation= _totalHeatIsolation + (getNumber (configFile >> "cfgVehicles" >> typeOf (_agent itemInSlot 'Body') >> "heatIsolation"));
	_totalHeatIsolation= _totalHeatIsolation + (getNumber (configFile >> "cfgVehicles" >> typeOf (_agent itemInSlot 'Legs') >> "heatIsolation"));
	_totalHeatIsolation= _totalHeatIsolation + (getNumber (configFile >> "cfgVehicles" >> typeOf (_agent itemInSlot 'Feet') >> "heatIsolation"));
	_totalHeatIsolation= _totalHeatIsolation + (getNumber (configFile >> "cfgVehicles" >> typeOf (_agent itemInSlot 'Vest') >> "heatIsolation"));
	_totalHeatIsolation= _totalHeatIsolation + (getNumber (configFile >> "cfgVehicles" >> typeOf (_agent itemInSlot 'Gloves') >> "heatIsolation"));
	_totalHeatIsolation= _totalHeatIsolation + (getNumber (configFile >> "cfgVehicles" >> typeOf (_agent itemInSlot 'Armband') >> "heatIsolation"));
	
	_agent setVariable ["totalWeight",_totalWeight];
	_agent setVariable ["totalHeatIsolation",_totalHeatIsolation];
};





event_conatinerAttachedToPlayer = {
	_agent = _this select 0;
	_item = _this select 1;
	
	//CALCULATE PLAYER'S TOTAL LOAD
	_totalWeight = _agent getVariable ["totalWeight",0];
	_insideCargo = itemsInCargo _item;
	{
		_totalWeight = _totalWeight + (getNumber (configFile >> "cfgVehicles" >> typeOf _x >> "weight"));
	}forEach _insideCargo;
	_agent setVariable ["totalWeight",_totalWeight];

	if(_item isKindOf 'ClothingBase')then{
		//CALCULATE PLAYER'S TOTAL HEAT COMFORT
		_totalHeatIsolation = _agent getVariable ["totalHeatIsolation",0];	//_totalHeatIsolation = _totalHeatIsolation + ((1-(damage _item)/2)*getNumber (configFile >> "cfgVehicles" >> typeOf _item >> "heatIsolation")); cannot include damage because of costs for update
		_totalHeatIsolation = _totalHeatIsolation + (getNumber (configFile >> "cfgVehicles" >> typeOf _item >> "heatIsolation"));
		_agent setVariable ["totalHeatIsolation",_totalHeatIsolation];
		//CALCULATE PLAYER'S TOTAL WETNESS
		_itemwet = _item getVariable ["wet",0];
		if(_itemwet > 0)then{
			_totalWetness = _agent getVariable ["wet",0];
			_totalWetness = _totalWetness + _itemwet/10;
			_item setVariable ["wet",0];
			_agent setVariable ["wet",_totalWetness];
		};
	};	
};

event_conatinerDetachedFromPlayer = {
	_agent = _this select 0;
	_item = _this select 1;
	
	//CALCULATE PLAYER'S TOTAL LOAD
	_totalWeight = _agent getVariable ["totalWeight",0];
	_insideCargo = itemsInCargo _item;
	{
		_totalWeight = _totalWeight - (getNumber (configFile >> "cfgVehicles" >> typeOf _x >> "weight"));
	}forEach _insideCargo;
	_agent setVariable ["totalWeight",_totalWeight];

	if(_item isKindOf 'ClothingBase')then{
		//CALCULATE PLAYER'S TOTAL HEAT COMFORT
		_totalHeatIsolation = _agent getVariable ["totalHeatIsolation",0];
		_totalHeatIsolation = _totalHeatIsolation - (getNumber (configFile >> "cfgVehicles" >> typeOf _item >> "heatIsolation"));
		_agent setVariable ["totalHeatIsolation",_totalHeatIsolation];
		//CALCULATE ITEM'S AND PLAYER'S WETNESS
		_totalWetness = _agent getVariable ["wet",0];
		if(_totalWetness > 0)then{
			_absorbf = getNumber(configFile >> "cfgVehicles" >> typeOf _item >> "absorbency");
			_absorb = ((_absorbf min _totalWetness)/10);
			//WET EVEN ITMES INSIDE
			if(_absorb > 0)then{
				_item setVariable ["wet",_absorb*10];
				_insideCargo = itemsInCargo _item;
				{
					if(_x isKindOf 'ClothingBase')then{
						_x setVariable ["wet",_absorb];
					};
				}forEach _insideCargo;
			};
			
			_totalWetness = ((_totalWetness - _absorb) max 0); 
			_agent setVariable ["wet",_totalWetness];
		};
	};
};



event_itemInContainer = {
	_agent = _this select 0;
	_item = _this select 1;
	//CALCULATE PLAYER'S TOTAL LOAD
	_totalWeight = _agent getVariable ["totalWeight",0];
	_totalWeight = _totalWeight + (getNumber (configFile >> "cfgVehicles" >> typeOf _item >> "weight"));
	_agent setVariable ["totalWeight",_totalWeight];

};

event_itemOutOfContainer = {

	_agent = _this select 0;
	_item = _this select 1;
	//CALCULATE PLAYER'S TOTAL LOAD
	_totalWeight = _agent getVariable ["totalWeight",0];
	_totalWeight = _totalWeight - (getNumber (configFile >> "cfgVehicles" >> typeOf _item >> "weight"));
	_agent setVariable ["totalWeight",_totalWeight];
};



player_warningMessage = {
	private["_messages","_color","_client","_output"];
	_messages = _this select 0;
	_color = _this select 1;
	_client = _this select 2;
	_output = (_messages select floor(random(count _messages)));
	if (isDedicated) then {
		_client statusChat [_output,_color];
	} else {
		statusChat [_output,_color];
	};
};

player_checkPulse = 
{
	_target = _this select 0;
	_person = _this select 1;
	/*
	_blood = _target getVariable ["blood",DZ_BLOOD];
	_pressure = switch (true) do
	{
		case (_blood < (DZ_BLOOD * 0.2)): {"very fast and "};
		case (_blood < (DZ_BLOOD * 0.4)): {"fast and "};
		case (_blood < (DZ_BLOOD * 0.8)): {"regular and "};
		default {""};
	};
	*/
	_health = _target getVariable ["health",DZ_HEALTH];
	_state = switch (true) do
	{
		case (_health < (DZ_HEALTH * 0.1)): {"extremely weak"};
		case (_health < (DZ_HEALTH * 0.2)): {"very weak"};
		case (_health < (DZ_HEALTH * 0.4)): {"weak"};
		case (_health < (DZ_HEALTH * 0.8)): {"steady"};
		default {"strong"};
	};
	
	switch (true) do
	{
		case (!alive _target): //dead
		{
			_time = diag_tickTime - (_target getVariable ["timeOfDeath",diag_tickTime]);
			_state = switch (true) do
			{
				case (_time > 600): {"cold"};
				case (_time > 400): {"lukewarm"};
				default {"warm"};
			};
			[_person,format["%1 has no pulse and is %2 to touch",name _target,_state],""] call fnc_playerMessage;
		};
		case (_target getVariable["fibrillation",false]): //fibrillation
		{
			[_person,format["%1 has a %2 but irregular pulse",name _target,_state],""] call fnc_playerMessage;
		};		
		default //others
		{
			[_person,format["%1 has a %2 pulse",name _target,_state],""] call fnc_playerMessage;
		};
	};
};

player_coverHead = {
	_agent = _this select 0;
	_hide = _this select 1;
	if (_hide) then
	{
		if (lifeState _agent == "UNCONSCIOUS") then
		{
			_agent spawnForPlayer compile "setEVUser -5; 1 fadeSound 0; 1 fadeSpeech 0; 1 fadeRadio 0.4;"; //setAperture 10000;
		}
		else
		{
			//diag_log format['CoverTargetHead Effect -> _agent: %1',_agent];
			_agent spawnForPlayer compile "setEVUser -5; 1 fadeSound 0.4; 1 fadeSpeech 0.4; 1 fadeRadio 0.4;"; //setAperture 10000;
		};
	}
	else
	{
		//diag_log format['UncoverSelfHead Effect -> _agent: %1',_agent];		
		_agent spawnForPlayer compile "setEVImpuls 5; setEVUser 0; 1 fadeSound 1; 1 fadeSpeech 1; 1 fadeRadio 1;"; //setAperture -1;
	};
};

fnc_generateRscQuantity = {
	private["_object","_resource","_qty ","_max","_str"];
	_object = _this select 0;
	_resource = _this select 1;
	_qty = _object getVariable [_resource,1];
	_max = getNumber (configFile >> "CfgVehicles" >> typeOf _object >> "Resources" >> _resource);
	_qty = round ((_qty / _max) * 100);
	_str = format ["%1%%",_qty];
	_str
};
/*
player_suicide = {
	_fsm = [_person,_this] execFSM "\dz\server\scripts\fsm\player_suicide.fsm";
	_person setVariable ["fsm_suicide",_fsm];
};

event_animHook =
{
	//[SurvivorPartsFemaleWhite:0:0,3,"fired"]
	_agent = _this select 0;
	_value = _this select 1;
	_type = _this select 2;
	
	hint "test!";
	
	switch (_type) do
	{
		case "fired":
		{
			_fsm = _agent getVariable ["fsm_suicide",-1];
			hint "bang!";
			if (_fsm < 0) exitWith {};
			hint "bang bang!";
			_fsm setFSMVariable ["_fired",true];
		};
	};
};
*/
player_transferWater = {
	_receiverQty = quantity _tool2;
	_senderQty = quantity _tool1;
	_name2 = displayName _tool2;
	_name1 = displayName _tool1;
	_type = _tool1 getVariable ["liquidType", ""];
	
	if (_receiverQty >= maxQuantity _tool2) exitWith 
	{
		[_owner,format["The %1 is already full",_name2],""] call fnc_playerMessage;
	};
	if (quantity _tool1 == 0) exitWith 
	{
		[_owner,format["The %1 is empty",_name1],""] call fnc_playerMessage;
	};
	if (damage _tool2 >= 1 || damage _tool1 >= 1) exitWith 
	{
		[_owner,format["The %1 is too badly damaged",_name2],""] call fnc_playerMessage;
	};
	
	if ( quantity _tool2 > 0 && (quantity _tool2 < (maxQuantity _tool2) && _tool2 getVariable ["liquidType",""] != _type ) ) exitWith 
	{
		[_owner,format["There is already something in the %1",_name2],""] call fnc_playerMessage;
	};
	
	_exchanged = ((_receiverQty + _senderQty) min (maxQuantity _tool2)) - _receiverQty;
	_receiverQty = _receiverQty + _exchanged;
	_senderQty = _senderQty - _exchanged;
	_tool2 setQuantity _receiverQty;
	_tool1 setQuantity _senderQty;
	
	_temperature1 = _tool1 getVariable ['temperature',0];
	_temperature2 = _tool2 getVariable ['temperature',0];
	_temperature = (_temperature1 + _temperature2) / 2 min 100; //max temp for liduids is set to 100 degree Celsius
	_tool2 setVariable ['temperature', _temperature];
	
	_tool2 setVariable [ "liquidType", _type];
	
	[_tool1,_tool2,"Direct",1] call event_transferModifiers;
	[_owner,format["You have poured the %2 into the %1",_name2,_name1],"colorAction"] call fnc_playerMessage;
};

player_canUseDrinkAction =
{
	_canUse = true;
	_item =  _this select 0;
	_person = _this select 1;

	if ( _person getVariable ['isUsingSomething',0] != 0 ) then {_canUse = false;};
	
	if ( quantity _item <= 0 ) then {_canUse = false;};
	
	if ( !(isNull itemInHands _person) && (isNull itemParent _item)) then {_canUse = false;};
	
	if (((itemParent _item) isKindOf 'FireplaceBase')) then {_canUse = false;}; //is attached to fireplace
	
	_canUse
};

player_paintItem = {
	_baseClass = _this select 0;
	_type = _this select 1;
	_name = displayName _tool1;
	_damage = damage _tool1;
	if (quantity _tool2 < 0.25) exitWith 
	{
		[_owner,format["Not enough paint left in the %1",displayName _tool2],""] call fnc_playerMessage;
	};

	//make new weapon
	_parent = itemParent _tool1;
	_color = getText(configFile >> "cfgVehicles" >> typeOf _tool2 >> "color");
	_newItemType = format ["%1_%2",_baseClass,_color];
	_newItemCfg = configFile >> _type >> _newItemType;
	
	if (!isClass _newItemCfg) exitWith
	{
		_msg = "You cannot paint the %1 to %2";
		if (_baseClass == typeOf _tool1) then {_msg = "You cannot add %2 paint to the %1"};
		[_owner,format[_msg,_name,_color],""] call fnc_playerMessage;
	};
	
	_qty = _tool1 getVariable ["quantity",-1];	//LEAVE THIS, TO DETECT NULL
	if (_tool1 isKindOf "DefaultMagazine") then
	{
		_qty = magazineAmmo _tool1;
	};
	
	_newItem = _tool1;
	_qb1 = _owner getQuickBarIndexOfEntity _tool1;
	
	//check if has attachments, if not just create inside
	if (count itemsInInventory _tool1 > 0)  then
	{	
		_oldItem = moveToGround _tool1;
		_newItem = [_newItemType,_parent,_owner] call player_addInventory;
		_newItem setDamage _damage;
		_light = nil;
		_hasLight = false;
		//move attachments to new weapon
		_array = getArray(_newItemCfg >> "attachments");
		{
			_hattn = (_tool1 itemInSlot _x);
			//prevents removing of flashlight on m4
			if (_hattn isKindOf "HandguardBase") then {
				_hatt = _newItem createInInventory (typeOf _hattn);
				_hatt setDamage (damage _hattn);
			}else{
				if (_hattn isKindOf "Att_Light_Universal") then {
					_light = _hattn;
					_hasLight = true;
				} else {
					_newItem moveToInventory _hattn;
				};
			};
		} forEach _array;
		if (_hasLight) then {
			_newItem moveToInventory _light;
		};
		_newItem moveToInventory (_tool1 itemInSlot "magazine");
		//remove old
		deleteVehicle _tool1;
		//switch in new
		//_parent moveToInventory _newItem;	
	} 
	else
	{	
		deleteVehicle _tool1;
		_newItem = [_newItemType,_parent,_owner] call player_addInventory;
		_newItem setDamage _damage;
	};
	
	//set quantities
	if (_qty >= 0) then
	{
		if (_newItem isKindOf "DefaultMagazine") then
		{
			_qty = _newItem setMagazineAmmo _qty;
		}
		else
		{
			_newItem setQuantity _qty;
		};
	};
	_tool2 addQuantity -0.25;
	_owner setEntityToQuickBarIndex [ _newItem, _qb1 ];
	
	[_owner,format["You have painted the %1 %2",_name,_color],"colorAction"] call fnc_playerMessage;
};



/*
checkbarrelcraft = {
//made by: jan tomasik 
	
	_limeperpelt = 0.5; //how many lime is consumed for generating one taned piece of pelt (1=10%)

	_barrel = _this;
	_cargo = itemsInCargo _barrel;
	_peltcount = 0;
	_limecount = 0;
	_counter = 0;
	_totalnewpeltcount = 0;
	// if you gonna add new skin amount make sure that it will be ordered properly (from highest to lowest)
	_tancAr = [8,6,4,2]; //table of taned pieces animal pelt gives. it would be better to have it stored in pelt config itself, but guys did not do that that way so I continued in same way
	_peltAr = [0,0,0,0];
	_minimumlime = 10;
	//fill database with amount of pelts
	{
		if((_x isKindOf 'Consumable_GardenLime') and ((damage _x) < 1))then{
			_limecount = _limecount+round((quantity _x)*10);
		}else{
						
			//wasnt able to use switch case properly ask programmers whats the deal
			if((_x isKindOf 'Consumable_PeltBase') and ((damage _x) < 1))then{
				if(_x isKindOf 'Consumable_PeltBear')then		{if((_tancAr select 0) < _minimumlime)then{_minimumlime = (_tancAr select 0);};_peltAr set [0,((_peltAr select 0)+1)];_peltcount = (_peltcount+(_tancAr select 0));};
				if(_x isKindOf 'Consumable_PeltDeer')then		{if((_tancAr select 1) < _minimumlime)then{_minimumlime = (_tancAr select 1);};_peltAr set [1,((_peltAr select 1)+1)];_peltcount = (_peltcount+(_tancAr select 1));};
				if(_x isKindOf 'Consumable_PeltCow')then 		{if((_tancAr select 1) < _minimumlime)then{_minimumlime = (_tancAr select 1);};_peltAr set [1,((_peltAr select 1)+1)];_peltcount = (_peltcount+(_tancAr select 1));};
				if(_x isKindOf 'Consumable_PeltSheep')then		{if((_tancAr select 2) < _minimumlime)then{_minimumlime = (_tancAr select 2);};_peltAr set [2,((_peltAr select 2)+1)];_peltcount = (_peltcount+(_tancAr select 2));};
				if(_x isKindOf 'Consumable_PeltWildboar')then	{if((_tancAr select 2) < _minimumlime)then{_minimumlime = (_tancAr select 2);};_peltAr set [2,((_peltAr select 2)+1)];_peltcount = (_peltcount+(_tancAr select 2));};
				if(_x isKindOf 'Consumable_PeltPig')then		{if((_tancAr select 2) < _minimumlime)then{_minimumlime = (_tancAr select 2);};_peltAr set [2,((_peltAr select 2)+1)];_peltcount = (_peltcount+(_tancAr select 2));};
				if(_x isKindOf 'Consumable_PeltGoat')then 		{if((_tancAr select 2) < _minimumlime)then{_minimumlime = (_tancAr select 2);};_peltAr set [2,((_peltAr select 2)+1)];_peltcount = (_peltcount+(_tancAr select 2));};
				if(_x isKindOf 'Consumable_PeltRabbit')then		{if((_tancAr select 3) < _minimumlime)then{_minimumlime = (_tancAr select 3);};_peltAr set [3,((_peltAr select 3)+1)];_peltcount = (_peltcount+(_tancAr select 3));};				
			};
		};
	} forEach _cargo;
	
	//simple material check
	if((_peltcount==0))exitWith{
		[_owner,'I need to add pelts to tan.',""] call fnc_playerMessage;
	};
	if((_limecount<_minimumlime*_limeperpelt))exitWith{
		[_owner,'Not enough lime for even one pelt.',""] call fnc_playerMessage;
	};
	
	//wait certain amount of time till its ready
	_currentTime = time;   
	while {time < _currentTime+10000} do {  
		
	}; 
	//calculate amount of pelts that could be taned
	{
		if(_x > 0)then{
			_maxpelts = floor (_limecount / ((_tancAr select _counter)*_limeperpelt));
			_usedcount = _maxpelts min (_peltAr select _counter);
			_newpeltcount = _usedcount * (_tancAr select _counter);
			_peltAr set [_counter,_usedcount];
			_limecount = _limecount - _newpeltcount*_limeperpelt;
			_totalnewpeltcount = _totalnewpeltcount + _newpeltcount;
		};
		_counter = _counter + 1;
	}forEach _peltAr;
	
	//delete used pelts
	{
		if(_x isKindOf 'Consumable_GardenLime')then{
			deleteVehicle _x;
		}else{
			if(_x isKindOf 'Consumable_PeltBase')then{
				if((_x isKindOf 'Consumable_PeltBear') and ((_peltAr select 0) > 0))then		{_peltAr set [0,((_peltAr select 0)-1)];deleteVehicle _x;};
				if((_x isKindOf 'Consumable_PeltDeer') and ((_peltAr select 1) > 0))then		{_peltAr set [1,((_peltAr select 1)-1)];deleteVehicle _x;};
				if((_x isKindOf 'Consumable_PeltCow') and ((_peltAr select 1) > 0))then 		{_peltAr set [1,((_peltAr select 1)-1)];deleteVehicle _x;};
				if((_x isKindOf 'Consumable_PeltSheep') and ((_peltAr select 2) > 0))then		{_peltAr set [2,((_peltAr select 2)-1)];deleteVehicle _x;};
				if((_x isKindOf 'Consumable_PeltWildboar') and ((_peltAr select 2) > 0))then	{_peltAr set [2,((_peltAr select 2)-1)];deleteVehicle _x;};
				if((_x isKindOf 'Consumable_PeltPig') and ((_peltAr select 2) > 0))then			{_peltAr set [2,((_peltAr select 2)-1)];deleteVehicle _x;};
				if((_x isKindOf 'Consumable_PeltGoat') and ((_peltAr select 2) > 0))then 		{_peltAr set [2,((_peltAr select 2)-1)];deleteVehicle _x;};
				if((_x isKindOf 'Consumable_PeltRabbit') and ((_peltAr select 3) > 0))then		{_peltAr set [3,((_peltAr select 3)-1)];deleteVehicle _x;};				
			};
		};
	} forEach _cargo;
	
	//generate taned skins in proper piles
	_pilescount = floor(_totalnewpeltcount/7);	
	for [{_i=0},{_i<_pilescount},{_i=_i+1}] do
	{	
		_item = _barrel createInCargo 'Consumable_TannedLeather';
		_item setQuantity 7;
	};
	_rest = (_totalnewpeltcount-(_pilescount*7));
	if(_rest > 0)then{
		_item = _barrel createInCargo 'Consumable_TannedLeather';
		_item setQuantity _rest;
	};
	//generate rest of lime in proper piles
	if(_limecount>0)then{
		_pileslime = floor(_limecount/10);	
		for [{_i=0},{_i<_pileslime},{_i=_i+1}] do
		{	
			_item = _barrel createInCargo 'Consumable_GardenLime';
			_item setQuantity 1;
			_item setDamage 0.85;
		};
		_rest = (_limecount-(_pileslime*10));
		if(_rest > 0)then{
			_item = _barrel createInCargo 'Consumable_GardenLime';
			_item setQuantity (_rest/10);
			_item setDamage 0.85;
		};
	};
};
*/

doZombieHit = {
	private["_agent","_fsm"];
	_agent = _this;
	_fsm = _agent getVariable["fsm",-1];
	if (_fsm < 0) exitWith{};
	_fsm setFSMVariable["_attacking",false];
};

building_getProxyName = {
	//Take string and return truncated proxy name
	_array = toArray(_this);
	_newArray = [];
	for "_i" from 17 to ((count _array) - 1) do 
	{
		_newArray set[(_i - 17),(_array select _i)];
	};
	_string = toString _newArray;
	_string
};

fnc_randomPlacement = {
	private["_pos","_max","_val"];
	_pos = _this select 0;
	_max = _this select 1;
	_val = 0;
	
	for "_i" from 0 to 1 do 
	{	
		_val = _max - (random (_max * 2));
		_pos set [_i,((_pos select _i) + _val)];
	};
	_pos
};

/*
ENVIRONMENT FUNCTIONS
*/
fnc_processItemWetness = {
	/*
		Processes wetness change for an item
		_this refers to whether item is parented to player or not
	*/
	
	//if there are no clothes on that part of body, dry/wet player instead
	if (isNull _item)exitWith{
		_playerWet = ((_playerWet + (_delta * 0.2 * _scale)) min 1) max 0;
	};
	
	//if clothes are already dry, dry player instead
	_wetness = _item getVariable ["wet",0];
	if(_wetness == 0 and _isDrying)exitWith{
	_playerWet = ((_playerWet + (_delta * 0.01 * _scale)) min 1) max 0;
	};
	
	//if clothes are already soaked, wet player instead
	_absorbancy = getNumber(configFile >> "cfgVehicles" >> typeOf _item >> "absorbency");
	if((_wetness == _absorbancy or _absorbancy == 0) and !_isDrying)exitWith{
		//if player has waterproof clothes and is in water he gets wet, if its just raining, then not
		if(_isWater)then{
			_playerWet = ((_playerWet + (_delta * (_absorbancy+0.2) * _scale)) min 1) max 0;
		};
	};
	
	//dry/wet clothes
	//_wetness = (((_wetness + (_delta * _scale)) min 1) max 0) * _absorbancy;
	_wetness = ((_wetness + (_delta * _scale)) min _absorbancy) max 0;
	_item setVariable ["wet",_wetness];
};

/*
DAMAGE CAlC
*/
damage_fnc_generateSlots =
{
	_slots = getArray (_config >> "inventorySlots");
	{
		private["_item"];
		//find out what is on the player
		_item = _agent itemInSlot _x;
		if (!isNull _item) then
		{
			_items set [count _items,_item];
		};
	} forEach _slots;
};
damage_fnc_applyStateChange =
{
	private["_v","_i"];
	_v = getNumber(_ammoConfig >> format["hit%1",_this]);
	//statusChat[format["Result: %1",_v],""];
	_i = _agent getVariable [_this,0];
	_agent setVariable [_this,(_i + _v)];
};
damage_unconscious = 
{
	private["_agent","_case"];
	_agent = _this select 0;
	_case = _this select 1;
	if _case then {
		_agent playAction "KnockDownStayB";
		_agent setUnconscious true;
	}
	else
	{
		//null = _agent spawn {sleep 1;_this playAction "KnockUp"};
		_agent setUnconscious false;
	};
};
damage_publishBleeding =
{
	// don't do damage for dead bodies
	//if(!alive _person) exitWith {};
	if(!alive _agent) exitWith {};
	
	_chance = getNumber(_ammoConfig >> "bleedChance");
	if (_isMelee) then
	{
		{
			_i = _agent itemInSlot _x;
			_chance = _chance * (1 - (getNumber(configFile >> "cfgVehicles" >> typeOf _i >> "DamageArmor" >> "bleed") * (1 - damage _i)));
		} forEach _slots;
	};
	
	_doBleed = (random 1) < _chance;
	if (_doBleed) then 
	{
		_bleeding = call compile (_agent getVariable ["bleedingsources","[]"]);
		_bleeding set [count _bleeding,_this select 5];
		_agent setVariable ["bleedingsources",str(_bleeding)];
	};
	//statusChat [format["chance: %1 // result: %2 // bone: %3 // damage: %4",_chance,_doBleed,_bone,_damage],""];
};
medical_playerBandage =
{
	_agent = _this select 1;
	if (_this select 0) then
	{
		_infectionChance = random 1;
		_wounds = _agent getVariable ['bleedingLevel',0];
		if (_wounds > 0) then {
			_item = _agent getVariable ['inUseItem',objNull];
			_condition = damage _item;
			if ((_condition >= 0.5) and (_infectionChance < 0.3)) then 
			{
				[0,_agent,'WoundInfection'] call event_modifier;
			};
		};
		_agent setVariable ['bleedingsources','[]'];
		_agent setVariable ['bleedingLevel',0];
		//_agent setDamage 0; //commented out as we dont want to fix broken leg by bandaging
	}
	else
	{
		_agent setVariable ['bleedingsources','[]'];
		_agent setVariable ['bleedingLevel',0];
		//_agent setDamage 0; //commented out as we dont want to fix broken leg by bandaging
	};
};
/*
EVENT MANAGEMENT
*/
fnc_speedVector = {
	private["_speed"];
	_speed = [0,0,0] distance (velocity _this);
	_speed
};

weapon_toggleOptics = {
//[(_this select 0),0] call 
	_optic = _this select 0;
	_powerOn = _this select 1;
	
	switch (_powerOn) do
	{
		case 0:
		{
			_optic setObjectTexture [0,"#(rgb,8,8,3)color(1,0,0,0)"];
		};
		case 1:
		{
			//set scope texture if required
			_opticTexture = getText (configFile >> "CfgVehicles" >> typeOf _optic >> "OpticsInfo" >> "opticSightTexture");
			if (_opticTexture != "") then
			{
				_optic setObjectTexture [0,_opticTexture];
			};
		};
	};
};

player_fnc_useItemStart =
{
	_person setVariable ["inUseItem",_item];
	if (_quantity > 0) then
	{
		//Use part of stack
		_item addQuantity -_use;
	};
};

player_fnc_useItemEnd =
{
	_oldItem = _person getVariable ["previousItem",objNull];
	_item = _person getVariable ["inUseItem",objNull];
	if (!_keepEmpty) then
	{		
		if (quantity _item <= 0) exitWith
		{
			deleteVehicle _item;
		};
	};
	if (_oldItem != _item) then 
	{
		_person moveItemFromHandsToInventory _item;

		//@TODO: wait for async.. stejne jako v RocketHellu (cti player_actionOnSelf.sqf)

		if (!isNull _oldItem) then
		{
			_person moveToHands _oldItem;
		};
	};
	_person setVariable ["inUseItem",objNull];
	_person setVariable ["previousItem",objNull];
};

event_fnc_sendActvMessage = {
	//send an activation message
	private["_messages","_style","_output","_statement","_print"];
	//_stages = _cfgModifier >> "Stages";
	//_cfgStage = _stages select _stage;

	// don't do damage for dead bodies
	if(!alive _person) exitWith {};

	_postpone = getNumber (_cfgStage >> "postponeMessageUntilCooldown");
	
	if(_postpone == 0) then {
	
		_messages = getArray (_cfgStage >> "messages");
		if (count _messages > 0) then
		{
			_style = getText (_cfgStage >> "messageStyle");
			_output = (_messages select (floor random(count _messages)));
			[_person,_output,_style] call fnc_playerMessage;
		};
		
	};

	_myNotifiers = _person getVariable ["myNotifiers",[]];
	_publishNotifiers = false;
	
	if (count _oldNotifier > 0) then	//blank old notifier
	{
		diag_log format ["notifier>>>before: %1", _myNotifiers];
		_myNotifiers set [_oldNotifier select 0,[]];
		_publishNotifiers = true;
		diag_log format ["notifier>>>after: %1", _myNotifiers];
	};
	
	_notifier = getArray (_cfgStage >> "notifier");
	if (count _notifier > 2) then	//send new notifier
	{	
		_myNotifiers set [_notifier select 0,[_notifier select 1,_notifier select 2]];
		_person setVariable ["myNotifiers",_myNotifiers];		
		_publishNotifiers = true;
	};
	
	if (_publishNotifiers) then
	{
		myNotifiers = _myNotifiers;
		(owner _person) publicVariableClient "myNotifiers";
	};
	
	//execute entry statement
	_statement = getText(_cfgStage >> "statementEnter");
	call compile _statement;
};

event_fnc_endModifier = {	
	private["_statement"];
	//end the modifier
	_modifiers set [_i,0];
	_modstates set [_i,0];
	_cleanup = true;
	_skipSave = true;
	
	//send a state exit message
	private["_messages","_style","_output"];
	_messages = getArray (_cfgModifier >> "messagesExit");
	if (count _messages > 0) then
	{
		_style = getText (_cfgModifier >> "messageExitStyle");
		_output = (_messages select (floor random(count _messages)));
		[_person,_output,_style] call fnc_playerMessage;
	};
	
	_notifier = getArray (_cfgStage >> "notifier");
	if (count _notifier > 0) then
	{
		_myNotifiers = _person getVariable ["myNotifiers",[]];
		_myNotifiers set [_notifier select 0,[]];
		myNotifiers = _myNotifiers;
		(owner _person) publicVariableClient "myNotifiers";
	};
	
	//run ending statement
	_statement = getText (_cfgStage >> "statementExit");	//_person
	call compile _statement;
};

event_fnc_addModifier = {
	/*
		important! this must not have its variables set to private!
		
		This script is used within others, parenting off their variables, to
		allow the addition of a modifier to the mod array
	*/
	//check if allowed to add
	private["_stages","_cfgStage"];
	_stages = _cfgModifier >> "Stages";
	_cfgStage = _stages select _stage;
	_condition = getText (_cfgStage >> "condition");
	
	//save old notifier
	_oldNotifier = [];
	
	_runEvent = _person call compile _condition;
	if (!_runEvent) exitWith {};
	
	//load additional variables			
	_remaining = getArray (_cfgStage >> "duration") call randomValue;
	_reminder = getArray (_cfgStage>> "cooldown") call randomValue;
	
	//send an activation message
	call event_fnc_sendActvMessage;
	
	//save results
	_modifiers set [count _modifiers,_modifier];
	if (_person isKindOf "SurvivorBase") then
	{
		_stageArray = [_stage];
		_modstates set [count _modstates,[_stageArray,_remaining,_reminder]];
	};
	//call event_fnc_saveModifiers;
};

event_fnc_saveModifiers = {
	/*
	important! don't declare these values as private, as it uses its
	parents variables
	
	This function processes an array save for modifiers where needed
	*/
	if (_cleanup) then 
	{
		_modifiers = _modifiers - [0];
		_modstates = _modstates - [0];
	};
	_person setVariable["modifiers",_modifiers];
	if (_person isKindOf "SurvivorBase") then
	{
		_person setVariable["modstates",_modstates];
	};
};

/*
event_fnc_attachFireplace = {
	_fireplace = _this select 0;
	_item = _this select 1;
	_type = _this select 2;
	_stage = 0;
	if(_fireplace animationPhase "Wood" == 0) then {_stage = 1};
	if(_fireplace animationPhase "BurntWood" == 0) then {_stage = 2};
	
	switch (ToLower(_type)) do {
		case "firewood": {
			if(_stage == 0) then {
				_fireplace animate['Wood',0];
				if(_item getVariable ["fuel",0] <= 0) then {
					_item setVariable ["fuel",500];
				};
			};
		};
		case "stones": {
			_master = _fireplace itemInSlot "Stones";
			if(quantity _master >= 8) then {
				_fireplace animate["Stones",0];
				_master addQuantity -8;
				if(quantity _master == 0) then {
					deleteVehicle _master;
				};
			}
		}
	};
};

event_fnc_detachFireplace = {
	_fireplace = _this select 0;
	_item = _this select 1;
	_type = _this select 2;
	_stage = 0;
	if(_fireplace animationPhase "Wood" == 0) then {_stage = 1};
	if(_fireplace animationPhase "BurntWood" == 0) then {_stage = 2};
	
	if(isOn _fireplace) then {
		_fireplace powerOn false;
		_fireplace setVariable ['fire',false];
	};
	
	switch (ToLower(_type)) do {
		case "firewood": {
			if(_stage == 1) then {
				_fireplace animate ["Wood",1];
			};
			if(_stage == 2) then {
				_fireplace animate ["BurntWood",1];
			};
		};
	};
};

event_fnc_powerOutFireplace = {
	_fireplace = _this select 0;
	_firewood = _fireplace itemInSlot "firewood";
	if(!isNull _firewood and (_fireplace getVariable ["fire",false])) then {
		_firewood addQuantity -1;
		if(quantity _firewood == 0) then {
			deleteVehicle _firewood;
			_fireplace switchLight 'OFF';
			_fireplace setVariable ['fire',false];
		} else {
			_firewood setVariable ["fuel",500];
			_fireplace powerOn true;
		};
	} else {
		_fireplace switchLight 'OFF';
		_fireplace setVariable ['fire',false];
	}
};
*/

event_fnc_advanceModifier = {
	private["_newStage"];
	_newStage = _checkStages select _cStage;
	/*
	important! don't declare these values as private, as it uses its
	parents variables
	*/
	//is condition met?
	_condition = getText (_newStage >> "condition");
	_runEvent = _person call compile _condition;
	if (_runEvent) then
	{
		//save old notifier
		_oldNotifier = getArray (_cfgStage >> "notifier");
		
		//load new stage
		_cfgStage = _newStage;
		_cfgStages = _checkStages;
		//statusChat[format["Advancing too...%1 in ; %2",configName _cfgStage,_cfgStages],"colorFriendly"];
		//diag_log format["Stage change to: %1; duration: %2",(_cfgStage),getArray (_cfgStage >> "duration")];
		_remaining = getArray (_cfgStage >> "duration") call randomValue;
		_reminder = getArray (_cfgStage >> "cooldown") call randomValue;
		_stage = _cStage;
		_stageArray set [_cSerial,_stage];
		_modstates set [_i,[_stageArray,_remaining,_reminder]];
		
		//send an activation message
		call event_fnc_sendActvMessage;
		
		//save
		//call event_fnc_saveModifiers;
	};
	_runEvent
};

/*
	PLAYER FUNCTIONS
*/
player_fnc_roundsDistribute = {
	/*
	Used when distributing rounds amongst piles
	uses the _quantity , _person , _ammo, _parent parent variables
	*/
	private["_pile","_receiverQty","_exchanged","_max"];
	/*
	_max = 	getNumber (configFile >> "CfgVehicles" >> _ammo >> "count");
	if (_quantity <= 0) exitWith {};
	_pile = objNull;
	{
		if (_x isKindOf _ammo) then
		{
			//has a pile
			if (!(_x call fnc_isMaxQuantity)) then
			{
				_pile = _x;
				_receiverQty = quantity _pile;
				//process changes
				_exchanged = ((_receiverQty + _quantity) min _max) - _receiverQty;
				_receiverQty = _receiverQty + _exchanged;
				
				_pile addQuantity _exchanged;				
			};
		};
	} forEach itemsInCargo _parent;
	if (_quantity > 0) then 
	{
		_pile = [_ammo,_parent,_person] call player_addInventory;
		_pile setMagazineAmmo _quantity;
	};
	*/
	_magdmg = _this;
	_max = 	getNumber (configFile >> "CfgMagazines" >> _ammo >> "count");
	_sound = getText (configFile >> "CfgMagazines" >> _ammo >> "emptySound");
	if (_quantity > _max)then{
		_amam = floor (_quantity/_max);
		if(_amam < 8)then{
			for [{_x=0},{_x<_amam},{_x=_x+1}] do{
				_pile = [_ammo,_parent,_person] call player_addInventory;
				_pile setMagazineAmmo _max;
			};
			if(_amam*_max != _quantity)then{
				_pile = [_ammo,_parent,_person] call player_addInventory;
				_pile setMagazineAmmo (_quantity-(_amam*_max));
			};
			[_person,_sound] call event_saySound;
		};
	}else{
		_pile = [_ammo,_parent,_person] call player_addInventory;
		_pile setMagazineAmmo _quantity;
		[_person,_sound] call event_saySound;
	};
	
};

player_fnc_knockdown = {
	/*
		Used within the damage functionality to knock a player down
		can be easily commented out to be disabled
	*/
	private["_agent","_source","_pos1","_pos2"];
	_agent = _this select 0;
	_source = _this select 1;
	_pos1 = getPos _agent;
	_pos2 = getPos _source;
	_dir = ((_pos2 select 0) - (_pos1 select 0)) atan2 ((_pos2 select 1) - (_pos1 select 1));
	//_dir = _dir % 360;
	_dir = _dir - (getdir _agent); //subtract direction of unit
	
	if (_dir < 0) then {_dir = _dir + 360};
	if (_dir > 360) then {_dir = _dir - 360};
	
	_dir = _dir + 45;

	switch true do 
	{
		case (_dir < 90):
		{
			_agent playAction "KnockDownF";
		};
		case (_dir < 180):
		{
			_agent playAction "KnockDownR";
		};
		case (_dir < 270):
		{
			_agent playAction "KnockDownB";
		};
		case (_dir < 360):
		{
			_agent playAction "KnockDownL";
		};
	};
};
 
player_fnc_tickBlood = {
	private["_regen","_hungerOk","_thirstOk","_result"];
	/*
	_regen = _this;
	_hungerOk = _person getVariable ["hunger",0] < 0;
	_thirstOk = _person getVariable ["thirst",0] < 0;
	_result = 0;
	
	if (_hungerOk and _thirstOk) then
	{
		_result = (_person getVariable ["blood",5000]) + _regen;
		_result = _result min (_person getVariable ["health",5000]);
		_result = _result - (_person getVariable ["blood",5000]);
	};
	_result
	*/
};

player_fnc_tickHealth = {
	private["_regen","_hungerOk","_thirstOk","_result","_bloodOk"];
	/*
		Calculates how much health the player should regen
	*/
	_regen = _this;
	_hungerOk = _person getVariable ["hunger",0] < 0;
	_thirstOk = _person getVariable ["thirst",0] < 0;
	_health = _person getVariable ["health",5000];
	_bloodOk = _person getVariable ["blood",5000] >= _health;
	_result = 0;
	
	if (_hungerOk and _thirstOk and _bloodOk) then
	{
		_result = _regen;
	};
	_result
};

player_fnc_tickExposure = {
	private["_wci","_result","_items","_situations","_situation"];
	/*
		Calculates how much exposure should change
	*/	
	_result = 0;
	
	_items = itemsInInventory _agent;
	_damper = 0;
	_situations = ["heatReduction","coldReduction"];
	_situation = switch true do
	{
		case (windChill <= DZ_COOLING_POINT):	{1};		//cooling
		case (windChill):	{0};		//warming
	};
	
	//tally up damper based on clothing
	{
		private["_isClothing","_wetness","_change"];
		_isClothing = _x isKindOf "ClothingBase";
		if (_isClothing) then
		{
			_wetness = _x getVariable ["wet",0];
			
			_change = getNumber(configFile >> "cfgVehicles" >> typeOf _x >> (_situations select _situation));
			_damper = _damper + _change;
		};
	} forEach _items;
	
	_damper = 1 - ((_damper min 1) max 0);
	_wci = (windChill * _damper);
	
	//diag_log format["WCI: %1",(_wci)];
	//hintSilent str(_wci);
	_result
};

player_fnc_processStomach = {
	private["_person","_itemCfg","_itemClass","_energy","_water","_stomach","_scale"];
	/*
		Calculates how much total volume in the stomach should change based on energy/water used by player for actions or food/drink consumed by player
	*/	
	_person = _this select 0;
	_item = _this select 1;	 
	
	if (typename _item == "STRING") then
	{
		_itemClass = _item;
	}
	else
	{
		_itemClass = typeOf _item;
	};
	
	_itemCfg = configFile >> "CfgVehicles" >> _itemClass;
	
	if (count _this > 2) then
	{
		_scale = _this select 2;
	}
	else
	{
		_scale = 1;
	};
	
	if (!isClass _itemCfg) exitWith {};
	
	//if (isNil "_scale") then {_scale = 1;hint "bing!";};
	
	_energy = _person getVariable ["energy",DZ_ENERGY]; // actual energy from all food and drink consumed
	_water = _person getVariable ["water",DZ_WATER]; // actual water from all food and drink consumed
	_stomach = _person getVariable ["stomach",DZ_STOMACH]; // actual volume in stomach
	_diet = _person getVariable ["diet",DZ_DIET]; // actual diet 
	
	_isFood = _itemClass isKindOf "FoodItemBase";
	_isDrink = _itemClass isKindOf "DrinksItemBase";
	_isMedicine = _itemClass isKindOf "MedicalItemBase";
	_randomLiquid = _itemClass isKindOf "BottleBase";
	
	switch true do
	{
		case (_isFood || _isDrink || _randomLiquid):
		{
			// pull food and drink nutritions parameters from Nutrition class
			_nutritionConfig = _itemCfg >> "Nutrition";
/*
			if ( _randomLiquid ) then
			{
				_liquidType = (itemInHands _person) getVariable ["liquidType", ""];
				
				if ( _liquidType != "") then
				{
					_nutritionConfig = configFile >> "cfgLiquidTypes" >> _liquidType >> "Nutrition";
				};
			};
*/
			_totalVolume = 0;
			_consumableWater = 0;
			_consumableEnergy = 0;
			_consumableNutriIndex = 0;
			
			if ( !( isNumber (_nutritionConfig >> 'totalVolume'))) then
			{
				// pull food and drink nutritions parameters from Stages class
				//_item = _person getVariable "inUseItem";
				
				_food_stage = _item getVariable 'food_stage';
				_food_stage_name = _food_stage select 0;
				_food_stage_params = getArray (_itemCfg >> "Stages" >> _food_stage_name);
				_nutrition_values = _food_stage_params select 0;
				
				_totalVolume = _nutrition_values select 0;
				_consumableWater = _nutrition_values select 2;
				_consumableEnergy = _nutrition_values select 1;
				_consumableNutriIndex = _nutrition_values select 3;
			}
			else
			{
				_totalVolume = getNumber (_nutritionConfig >> "totalVolume");
				_consumableWater = getNumber (_nutritionConfig >> "water");
				_consumableEnergy = getNumber (_nutritionConfig >> "energy");
				_consumableNutriIndex = getNumber (_nutritionConfig >> "nutritionalIndex");
			};

			//statusChat [format ["D> energy:%1, water:%2, stomach:%3, scale:%4 (%5)",_energy,_water,_stomach,_scale,isNil "_scale"],""]; // debug: actual values of states
			//statusChat [format ["D> _totalVolume:%1, _consumableWater:%2, _consumableEnergy:%3, _consumableNutriIndex: %4",_totalVolume,_consumableWater,_consumableEnergy, _consumableNutriIndex],""]; 

			// volume of portion actually eaten/drunk/used
			_portionVolume = _totalVolume * _scale; // ??Am I sure to get proper scale from actionOnSelf??

			// change energy
			_energyGathered = _consumableEnergy * _scale; // energy actually gathered from serving		
			_energy = _energy + _energyGathered;
			_person setVariable ["energy",_energy];
			//dbStats ["calories",_person,_energyGathered];

			// change water
			_waterGathered = _consumableWater * _scale; // water actually gathered from serving
			_water = _water + _waterGathered;
			_person setVariable ["water",_water];
			//dbStats ["water",_person,_waterGathered];
			
			// change diet
			_dietGathered = _consumableNutriIndex * _scale * 0.01; // nutrients actually gathered from serving	
			_diet = (((9 * _diet) + _dietGathered) / 10) min 1; // diet formula (will be probably changed)
			_person setVariable ["diet",_diet];
			
			// change stomach volume
			_stomach = _stomach + _portionVolume;
			_person setVariable ["stomach",_stomach];
			
			/*
			//transfer item modifiers
			if (typename _item == "STRING") then
			{
				
				_modifiers = getArray (configFile >> "CfgVehicles" >> _b >> "diseases");				
			}
			else
			{
				_modifiers = _item getVariable ['modifiers',[]];
			};
			
			{
				[0,_person,_x] call event_modifier;
			} foreach _modifiers;
			*/
		};
		case _isMedicine:
		{
			_medicineConfig = _itemCfg >> "Medicine";
			_consumablePrevention = getNumber (_medicineConfig >> "prevention");
			
			// change diet
			_dietGathered = _consumablePrevention; // nutrients actually gathered from medicine
			_diet = (((4 * _diet) + _dietGathered) / 5) min 1; // diet formula (will be probably changed)
			_person setVariable ["diet",_diet];
		};
	};
};

player_vomit = {
	_agent = _this;
	if (_agent getVariable["vomiting",false]) exitWith {};
	_agent playAction ["PlayerCrouch",{_agent setVariable["vomiting",true]}];
	waitUntil {_agent getVariable["vomiting",true]};
	_agent playAction ["PlayerVomit",{_agent setVariable["vomiting",false]}];
	_energy = _agent getVariable ["energy",DZ_ENERGY]; // actual energy from all food and drink consumed
	_water = _agent getVariable ["water",DZ_WATER]; // actual water from all food and drink consumed
	_stomach = _agent getVariable ["stomach",DZ_STOMACH]; // actual volume in stomach
	
	if (_stomach > 100) then
	{
		_agent setVariable ["vomit",diag_tickTime];	//broadcasts vomit
		
		[_agent,"action_vomit"] call event_saySound;
		
		//remove contents
		
		_energy = (_energy - 600) max -1;
		_agent setVariable ["energy",_energy];		

		_stomach = 0;
		_agent setVariable ["stomach",_stomach];
			
		_water = (_water - 1000);
		_agent setVariable ["water",_water];
	}
	else
	{
		//dry wretch
		
		//remove contents
		_agent setVariable ["stomach",0];
	};
};

/*

FISHING 

*/
//Checks whether player left fishing place and stops fishing minigame
fishing_event_add={
	_owner = _this select 0;
	fishingpos =  _this select 1;
	_owner addEventHandler ['AnimChanged','_owner=_this select 0;if(abs (((getPosATL _owner) select 0) - fishingpos) > 0.1)then{_owner call fishing_event_remove;itemInHands _owner powerOn false;}'];
};

fishing_event_remove={
	[_this,'You have moved and pulled the bait out of the water.','colorImportant'] call fnc_playerMessage;
	_this playAction 'cancelAction';
	_this removeAllEventHandlers 'AnimChanged';
};

build_TentContainer = 
{
	if ( isServer ) then
	{
		_tent = (_this select 0);
		_person = (_this select 1);
		_tentType = (_this select 2) select 0;
		_tentOri = (_this select 2) select 1;
		_dist = (_this select 2) select 2;
		_tentSize = (_this select 3);
		_tentX = (_tentSize select 0) * 0.5;
		_tentY = (_tentSize select 1) * 0.5;
		_tentZ = (_tentSize select 2) * 0.5;

		_hp = damage (_this select 0);

		_pos = getPosASL _person;
		_ori = direction _person;
		_cosOri = cos _ori;
		_sinOri = sin _ori; 
		_xPos = (_pos select 0) + (sin _ori * _dist);
		_yPos = (_pos select 1) + (cos _ori * _dist);
		_zPos = (_pos select 2);
		
		_waterCheck = true;
		_heightCheck = true;

		_posTentX = _xPos + _cosOri * _tentX - _sinOri * _tentY;
		_posTentY = _yPos + _cosOri * _tentY + _sinOri * _tentX;
		
		_height = surfaceHeightASL [_posTentX, _posTentY, _zPos + 0.2];
		_sur = surfaceTypeASL [_posTentX, _posTentY, _zPos + 0.2];

		if ( _height > 1.5 || _height < -0.2 ) then
		{
			_heightCheck = false;
		};

		if ( _sur == "FreshWater" || _sur == "sea") then
		{
			_waterCheck = false;
		};

		if ( _heightCheck && _waterCheck ) then
		{
			_posTentX = _xPos + _cosOri * _tentX - _sinOri * -_tentY;
			_posTentY = _yPos + _cosOri * -_tentY + _sinOri * _tentX;
			
			_height = surfaceHeightASL [_posTentX, _posTentY, _zPos + 0.2];
			_sur = surfaceTypeASL [_posTentX, _posTentY, _zPos + 0.2];
			
			if ( _height > 1.5 || _height < -0.2 ) then
			{
				_heightCheck = false;
			};

			if ( _sur == "FreshWater" || _sur == "sea") then
			{
				_waterCheck = false;
			};	
		};

		if ( _heightCheck && _waterCheck ) then
		{
			_posTentX = _xPos + _cosOri * -_tentX - _sinOri * -_tentY;
			_posTentY = _yPos + _cosOri * -_tentY + _sinOri * -_tentX;
			
			_height = surfaceHeightASL [_posTentX, _posTentY, _zPos + 0.2];
			_sur = surfaceTypeASL [_posTentX, _posTentY, _zPos + 0.2];
			
			if ( _height > 1.5 || _height < -0.2 ) then
			{
				_heightCheck = false;
			};
			
			if ( _sur == "FreshWater" || _sur == "sea") then
			{
				_waterCheck = false;
			};	
		};

		if ( _heightCheck && _waterCheck ) then
		{
			_posTentX = _xPos + _cosOri * -_tentX - _sinOri * _tentY;
			_posTentY = _yPos + _cosOri * _tentY + _sinOri * -_tentX;
			
			_height = surfaceHeightASL [_posTentX, _posTentY, _zPos + 0.2];
			_sur = surfaceTypeASL [_posTentX, _posTentY, _zPos + 0.2];

			if ( _height > 1.5 || _height < -0.2 ) then
			{
				_heightCheck = false;
			};

			if ( _sur == "FreshWater" || _sur == "sea") then
			{
				_waterCheck = false;
			};	
		};
		
		//boundingBox
		//_bCenter = boundingCenter TentMedium_Pitched;

		if ( _heightCheck ) then
		{
			if ( _waterCheck ) then
			{
				//check slope
				_terrainNormal = RoadWayNormalAsl [_xPos, _yPos, _pos select 2];
				_terrainSlope = atg(sqrt((_terrainNormal select 0)*(_terrainNormal select 0) + (_terrainNormal select 1)*(_terrainNormal select 1))/(_terrainNormal select 2) );

				if ( _terrainSlope < 20 ) then
				{
					 //test collision
					 _bbox = (collisionBox [[_xPos, _yPos, _zPos + _tentZ + 0.2 ], _tentSize ,[vectorDir _person, RoadWayNormalAsl [_xPos, _yPos, _zPos]], [_person, _tent]]);
					 if ( !_bbox ) then
					 {
						_tent = _tentType createVehicle [_xPos, _yPos];
						//_tent setPosASL [_xPos, _yPos, _pos select 2];
						_tent setDir _ori - _tentOri;
						_tent setPosASL [_xPos, _yPos, _pos select 2];
						_tent setDamage _hp;
						increaseLifetime _tent;
						
						_cutter = objNull;
						if (_tentType == "TentLarge_Pitched" ) then
						{
							_cutter = "Tent_Large_ClutterCutter" createVehicle [_xPos, _yPos]; // it can be better always on a ground 
						};
						if (_tentType == "TentMedium_Pitched" ) then
						{
							_cutter = "Tent_ClutterCutter" createVehicle [_xPos, _yPos]; // it can be better always on a ground 
						};

						_cutter setDir getDir _tent;

						deleteVehicle (_this select 0);
					 }
					 else
					 {
						[_person,'I can not pitch the tent, there is not enough space!','colorImportant'] call fnc_playerMessage;
					 };
				}
				else
				{
					[_person,'I can not pitch the tent, the slope is too steep!','colorImportant'] call fnc_playerMessage;		
				};
			}
			else
			{
				[_person,'I can not pitch the tent in the water!','colorImportant'] call fnc_playerMessage;
			};
		}
		else
		{
			[_person,'I can not pitch the tent on place like this!','colorImportant'] call fnc_playerMessage;		
		};
	};
};

pack_TentContainer = 
{
	if ( isServer ) then
	{
		deleteVehicle nearestobject [_this select 0, "Tent_ClutterCutter"]; // delete cutter

		_hp = damage (_this select 0);
		deleteVehicle (_this select 0);

		_person = (_this select 1);
		_tent = [(_this select 2),_person] call player_addInventory;
		_tent setDamage _hp;
	};
};

player_drown = {
	_agent = _this select 0;
	_branch = _this select 1;
	_dmg = _agent getVariable ["underwater",0];
	if(_branch == 0)then{
		if(_dmg > 0)then{
			_agent setVariable ["underwater",0];
		};
	}else{
		_dmg = _dmg + 1;
		if(_dmg > 30)then{_agent setDamage 1;}else{
			if(_dmg > 27)then{[_agent,"I am drowning","colorImportant"] call fnc_playerMessage;
				if (isPlayer _agent) then
				{
					effectDazed = true;
					call dayz_bulletHit;
					effectDazed = false;
				};
			}else{
				if(_dmg > 23)then{[_agent,"I am going to drown","colorImportant"] call fnc_playerMessage;}else{
					if(_dmg > 15)then{[_agent,"I am running out of air",""] call fnc_playerMessage;};
				};
			};
		};
		_agent setVariable ["underwater",_dmg];
	};
};


//Returns global X,Y pos according to given offset and direction
fnc_getRelativeXYPos = {
	_x = (_this select 0) select 0;
	_y = (_this select 0) select 1;
	_offsetX = (_this select 1) select 0;
	_offsetY = (_this select 1) select 1;
	_dir = _this select 2;
	
	_xPos = _x + (sin _dir * _offsetX);
	_yPos = _y + (cos _dir * _offsetX);
	_xPos = _xPos + (sin (_dir+90) * _offsetY);
	_yPos = _yPos + (cos (_dir+90) * _offsetY);
	[_xPos, _yPos];
};

/*
	Adds quantity to given item and keeps it within its limits.
	Negative parameter is supported and the item is automatically deleted when its quantity is <= 0
	Return value is resulted quantity
	[_item, _addedQuantity] call fnc_addQuantity;
*/
fnc_addQuantity = {
	_item = _this select 0;
	_amount = _this select 1;
	_resultedQuantity = quantity _item + _amount;
	if (_resultedQuantity > 0) then {
		if (_resultedQuantity > maxQuantity _item) then {
			_resultedQuantity = maxQuantity _item;
		};
		_item setQuantity _resultedQuantity;
		_resultedQuantity;
	}else{ //Delete empty items
		deleteVehicle _item;
		_resultedQuantity;
	};
};

/*
	Adds multiple items of the same type into the user's inventory.
	Returns array of those items.
	[_itemType, _itemCount, _user] call fnc_addItemCount;
*/
fnc_addItemCount = {
	_itemType = _this select 0;
	_itemCount = _this select 1;
	_user = _this select 2;
	_listOfCreatedItems = [];
	while {_itemCount > 0} do
	{
		_itemCount = _itemCount - 1;
		_item = [_itemType, _user] call player_addInventory;
		_item setQuantity maxQuantity _item;
		_listOfCreatedItems = _listOfCreatedItems + [_item];
	};
	_listOfCreatedItems;
};

/*
	Used in cutting firewood into smaller sticks.
	[_user, _tool, _axedFirewood, _typeOfResultedObject, _quantityOfResultedObject] call fnc_axeFirewoodIntoSticks;
*/
fnc_axeFirewoodIntoSticks = {
	_user = _this select 0;
	_tool = _this select 1;
	_wood = _this select 2;
	_resultObject = [_this select 3, _user] call player_addInventory;
	_resultQuantity = _this select 4;
	
	_resultObject setQuantity _resultQuantity;
	_tool setDamage (damage _tool + 0.001);
	[_user, format['I have made %1 wooden sticks out of 1 %2.',quantity _resultObject, displayName _wood],'colorAction'] call fnc_playerMessage;
	[_wood, -1] call fnc_addQuantity;
};

/*
	Changes food stage with visual representation.
	[_food, _food_stage_name_new] call fnc_changeFoodStage;
*/
fnc_changeFoodStage =
{
	//clear modifiers
	_food_clear_modifiers = ['FoodPoisoning_MediumImpact','FoodPoisoning_LightImpact','Cholera','Salmonellosis']; 	//list of modifiers to be cleared
	_clear_modifiers =
	{
		_food_object =  _this;
		
		{
			_modifier = _x;
			
			[1, _food_object, _modifier] call event_modifier;
		
		} foreach _food_clear_modifiers;
	};
	
	_can_change_to_stage = 
	{
		private["_food_stage","_action_and_stage","_can_change","_food","_config_name","_food_stage_params","_config_food","_new_stage","_stage","_food_stages"];
		_food = _this select 0;
		_new_stage = _this select 1;
		_config_name = "CfgVehicles";
		_config_food = configFile >> _config_name >> typeOf _food;
		_food_stage = _food getVariable ['food_stage', ['Raw',0,0,0]];
		_food_stage_name = _food_stage select 0;
		_food_stage_params = getArray (_config_food >> "Stages" >> _food_stage_name);
		_food_stages = _food_stage_params select 4;
		
		_can_change = false;
		for [{_i = 0}, {_i < (count _food_stages)}, {_i = _i + 1}] do
		{
			_action_and_stage = _food_stages select _i;
			_stage = _action_and_stage select 1;
			
			if (_stage == _new_stage) then
			{
				_can_change = true;
			};
		};
		
		_can_change
	};
	
	//transfer modifiers
	_transfer_food_modifiers =
	{
		private["_food_object","_food_stage_name","_food_stage_params","_modifiers"];
		_food_object = _this select 0;
		_food_stage_name = _this select 1;
		
		//food params (new)
		_food_stage_params = getArray (_config_food >> "Stages" >> _food_stage_name);
		_modifiers = _food_stage_params select 2;	
		
		{
			_modifier = _x;
			
			//debugLog
			//diag_log format["Modifier %1 transfered to %2.", _modifier, _food_object];
			
			[0, _food_object, _modifier] call event_modifier;
		
		} foreach _modifiers;
		
		//debugLog
		//diag_log format["Modifiers transfer done! (modifiers count %1).", count _modifiers];
	};	
	
	private["_food","_food_stage","_food_stage_name_new","_food_stage_params","_food_stage_set","_food_selection_index","_food_texture_index","_food_material_index"];
	_food = _this select 0;
	_food_stage_name_new = _this select 1;
	
	//food stage transitional condition - can change from-to?
	//except for init phase
	_food_stage = _food getVariable 'food_stage';
	_can_change_stage = [_food, _food_stage_name_new] call _can_change_to_stage;
	if (!(isNil "_food_stage") and 
		!_can_change_stage ) exitWith {};
	
	//get food params (new stage)
	_config_name = "CfgVehicles";
	_config_food = configFile >> _config_name >> typeOf _food;
	_food_stage_params = getArray (_config_food >> "Stages" >> _food_stage_name_new);
	_food_appearance = _food_stage_params select 3;
	
	_food_selection_index = _food_appearance select 0;	
	_food_texture_index = _food_appearance select 1;
	_food_material_index = _food_appearance select 2;
	
	_food_stage = _food getVariable ['food_stage', ['Raw',0,0,0]];
	_food_stage_name = _food_stage select 0;
	
	//TRANSFER MODIFIERS
	//clear modifiers 
	_food_min_temp_to_cook = (_food_stage_params select 1) select 0;
	_food_temperature = _food getVariable ['temperature', 0];
	if ( _food_temperature >= _food_min_temp_to_cook ) then
	{	
		_food call _clear_modifiers;  //clear modifiers only if the food changed stage because of heat
	};
	
	//transfer new modifiers
	[_food, _food_stage_name_new] call _transfer_food_modifiers;
	
	//merge stages
	//merge food stage name
	_food_stage set [0, _food_stage_name_new];
	
	//merge selection
	if (_food_selection_index >= 0) then 
	{
		_food_stage set [1, _food_selection_index];
	};
	
	//merge texture
	if (_food_texture_index >= 0) then 
	{
		_food_stage set [2, _food_texture_index];
	};
	
	//merge material
	if (_food_material_index >= 0) then 
	{
		_food_stage set [3, _food_material_index];
	};
	
	//set food stage
	_food setVariable ["food_stage", _food_stage]; 
	
	//refresh visual		
	_food call event_fnc_foodStage;
};

//checks if given surface is soft
fnc_isSoftSurface = 
{
	_surCheck = _this;
	_surfaces = ["cp_dirt", "cp_grass", "cp_grass_tall", 
				"cp_conifer_common1", "cp_conifer_common2", "cp_conifer_moss1", "cp_conifer_moss2",
				"cp_broadleaf_dense1", "cp_broadleaf_dense2", "cp_broadleaf_sparse1", "cp_broadleaf_sparse2",
				"cp_gravel"];
	if (_surCheck in _surfaces) exitWith
	{
		true;
	};
	false;
};

// Temporary function for send notifiers to Enfusion Script
fnc_sendNotifiersToEnfusion = 
{	
	private["_myNtfrs","_color","_item","_i","_name"];
	_myNtfrs = [];
	_color = [];
	
	_myNtfrs = _this getVariable ["myNotifiers",[]];	
		
	for [{_i=0},{ _i < count _myNtfrs},{_i=_i+1}] do
	{
		_item = _myNtfrs select _i;		
		
		if !( isNil "_item" ) then
		{		
			if (typeName _item == "ARRAY") then
			{
				_name = "";
				_color = [0,0,0];
				_alpha = 0;
			
				if ( count _item > 0 ) then
				{	
					_name = _item select 0;
					_color = _item select 1;
					_alpha = 0.5;
				};
				
				_this callMethod ["SQF_PlayerNotifierAdd", _name, _i, _color select 0, _color select 1, _color select 2, _alpha];				
			};
		};
	};
	
	_this callMethod ["SQF_PlayerNotifierSend"];
};
