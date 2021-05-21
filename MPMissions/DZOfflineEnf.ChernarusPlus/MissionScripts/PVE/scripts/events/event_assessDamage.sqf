ProfileStart "event_assessDamage.sqf";
//if ((_this select 1)!="") exitWith {0};
private["_agent","_damage","_source","_bone","_base","_items","_slots"];

//select bone to damage
_agent = _this select 0;
_selection = _this select 1;
_damage = _this select 2;
_source = _this select 3;
_ammo = _this select 4;

_ammoConfig = configFile >> "cfgAmmo" >> _ammo;
_isMelee = _ammo isKindOf "MeleeDamage";
_isZombie = _source isKindOf "ZombieEngine";
_final = 0;

//ignore very low damage
if (_damage < 0.1 and !_isMelee) exitWith {0; ProfileStop "event_assessDamage.sqf";};

if (_isZombie) then
{
	//_damage = _damage * 3;
	_diseaseChance = random 1;
	switch true do 
	{
		case (_diseaseChance < 0.01): {[0,_agent,"BrainFlu"] call event_modifier;};	//brain flu
		case (_diseaseChance < 0.2): {[0,_agent,"WoundInfection"] call event_modifier;};	//infection
	};
};
//else
//{
//	admin_log format ['Source is:: %1.',_source];
//};
//hint(str _damage);
if (count (_this select 5) > 1) then
{
	_bone = (((_this select 5) select 1) select 0);
}
else
{
	//undefined, so fall damage
	//_bone = "Pelvis";
	_bone = "";
	_agent setVariable ['falldamage', true];	
	//handle damage script is probably called for each hit selection separately, so if some actions should be done only once during hit we need if below
	if(_selection == "feet") then
	{
		_legdmg = _damage*0.7;
		_legactdmg = (_agent getHitPointDamage 'HitLegs');
		//if(_legactdmg < _legdmg)then{ //in case I dont want to cummulate damage in leg, but damage leg based on height of each fall independently
		_agent setHit ['legs',(_legactdmg+_legdmg)];
		
		//};
		
		//remove blood for falldamage
		if(_damage > 0.25) then
		{
			_admg = ((_agent getVariable ['blood',2500]) - _damage*1500);
			_agent setVariable ['blood',_admg];
		};
		//kill player instantly
		if(_damage > 1.4) then
		{
			_agent setDamage 1;
		};
		
		//damage gear
		_shoes = (_agent itemInSlot "Feet");
		_currfdmg = (damage _shoes) + _damage*0.9;
		_shoes setDamage _currfdmg;	
		
		_pants = (_agent itemInSlot "Legs");
		_currpdmg = (damage _pants) + _damage/2;
		_pants setDamage _currpdmg;	
		if(_currpdmg > 0.6) then
		{	//damage items in pants
			{
				_x setDamage ((damage _x) + _damage/2);
			} forEach itemsInCargo _pants;
		};
	};
};

_config = configFile >> "cfgBody" >> _bone;
_base = configName inheritsFrom _config;

//ensure correct parented bone
if (_base != "") then
{
	_bone = configName inheritsFrom _config;
};

if (_bone == "") exitWith {0; ProfileStop "event_assessDamage.sqf";};

if (!_isZombie) then
{
	_item = itemInHands _source;
	
	if (!isNull _item) then
	{
		_v = damage _item;
		_apply = getNumber(configFile >> "cfgVehicles" >> typeOf _item >> "fragility");

		//change damage based on melee object damage
		if (_isMelee) then
		{		
			_damage = _damage * (1 - _v);	//TODO: Peter is going to write log scale :)
		};	
		
		//apply damage to item
		if (_apply > 0) then
		{
			_v = _v + (_apply * _damage);
			_item setDamage _v;
		};
	};
};

//ensure correct damage is applied (for now)
_hitPoint = getText(configFile >> "cfgBody" >> _bone >> "hitPoint");

//process pp effect

effectDazed = true;
call dayz_bulletHit;
effectDazed = false;

//slot items
_items = [];
call damage_fnc_generateSlots;

/*
	Process Item Damage
*/
{
	private["_item","_itemConfig","_condition"];
	_item = _x;
	_itemConfig = configFile >> "cfgVehicles" >> typeOf _item >> "DamageArmor";
	_condition = 1 - (damage _item);	//current condition
	
	//diag_log format["DAMAGING ITEM %1",typeOf _item];
	
	//Damage Item Itself
	_resolved = false;
	if (_damage > 0.05) then 
	{
		if (isClass _itemConfig) then
		{
			//Apply damage to an item that has armor
			_armor = getNumber(_itemConfig >> "ballistic") * _condition;	//get current damage state		
			if (_armor > 0) then
			{
				//Remove armor from object
				_armorResult = ((_armor - _damage) max 0) / _armor;
				_damage = (_damage - _armor) max 0;
				_item setDamage (1 - _armorResult);	//apply damage to armored item
				_resolved = true;
				//diag_log format["DAMAGED ITEM %1 to %2",typeOf _item,_armorResult];
			};
		};
		if (!_resolved) then
		{
			//Apply damage to an item that has no armor
			if ((damage _item) < 1) then
			{
				_item setDamage ((damage _item) + _damage);
				_damage = (_damage - (_condition / 3)) max 0;
				//diag_log format["DAMAGED ITEM %1 with %2 to %3",typeOf _item,_damage,((damage _item) + _damage)];
			};
		};
	};
	_priorDamage = _damage;
	
	//Damage Interior Items
	if (_damage > 0.05) then 
	{
		{
			if ((damage _x) < 1) then
			{
				_condition = 1 - (damage _x);
				_x setDamage ((damage _x) + _damage);
				_damage = (_damage - (_condition / 4)) max 0;
			};
		} forEach itemsInCargo _item;
	};	
} forEach _items;

// don't do damage for dead bodies
if(!alive _agent) exitWith {0; ProfileStop "event_assessDamage.sqf";};



/*
APPLY BODY DAMAGE
*/
"shock" call damage_fnc_applyStateChange;
"blood" call damage_fnc_applyStateChange;
"health" call damage_fnc_applyStateChange;
_shock = 0;

//Cattle prod damage transfer

if ((itemInHands _source) isKindOf "CattleProd")then{
	_battery = (itemInHands _source) itemInSlot "BatteryD";
	_batterypower = _battery getVariable ["power",0];
	if (_batterypower >= 5000) then
	{
		_r = random 4;
		if (_r < 1) then
		{
			_agent setVariable["fibrillation",true];
		};
		_shock = _agent getVariable ["shock",0];
		_agent setVariable ["shock",(_shock+2000)];
		_batterypower=_batterypower-2000;
		[_agent,"I feel a hot burst of electricity running through my body.","colorImportant"] call fnc_playerMessage;
		_battery setVariable ["power",_batterypower];
	};
};

if ((itemInHands _source) isKindOf "StunBaton")then
{
	_battery = (itemInHands _source) itemInSlot "BatteryD"; 
	_batterypower = _battery getVariable ["power",0];
	if (_batterypower >= 5000)then
	{
		_r = random 3;
		if (_r < 1) then
		{
			_agent setVariable["fibrillation",true];
		};
		_shock = _agent getVariable ["shock",0];
		_agent setVariable ["shock",(_shock+4000)];
		_batterypower=_batterypower-2000;
		[_agent,"I feel a hot burst of electricity running through my body.","colorImportant"] call fnc_playerMessage;
		_battery setVariable ["power",_batterypower];
	};
};


if ((itemInHands _source) isKindOf "Chainsaw") then
{
	if ((isOn (itemInHands _source))) then
	{
		_shock = _agent getVariable ["shock",0];
		_agent setVariable ["shock",(_shock+1000)];
		_chainblood = _agent getVariable ["blood",5000];
		_agent setVariable ["blood",_chainblood-1000];
		_chainhealth = _agent getVariable ["health",5000];
		_agent setVariable ["health",_chainhealth-500];
		//[_agent,"Wrrrrummm","colorImportant"] call fnc_playerMessage;
	};
};



switch (toLower _bone) do 
{
	case "head":
	{
		_shock = _agent getVariable ["shock",0];
		_totalShock = (random (getNumber(_ammoConfig >> "hitShockHead")) + _shock);
		
		//deduct protective shock
		_headwear = _agent itemInSlot "headgear";
		_shockProtection = getNumber(configFile >> "cfgVehicles" >> typeOf _headwear >> "DamageArmor" >> "shock") * (1 - damage _headwear);
		_totalShock = (_totalShock - _shockProtection) max 0;
		
		_agent setVariable ["shock",_totalShock];
		if (_totalShock > (_agent getVariable["blood",0])) then
		{
			//will happen anyway but make it happen fast
			[0,_agent,"Unconscious"] call event_modifier;
			admin_log format ['%1(uid=%2) has fallen into unconsciousness. HARD HIT Total S::%3', name _agent, getPlayerUID _agent, _totalShock];
		};
	};
	case "hands":
	{
	};
	default
	{
	};
};

//make bleeding occur
if (_damage <= 0) exitWith {0; ProfileStop "event_assessDamage.sqf";};

//set hitpoint damage
_hit = _agent getHitpointDamage _hitpoint;
_total = (_hit + _damage) min 1;
_agent setHitPointDamage [_hitpoint,_total];

_array = [];
_array resize 0;
{
	_v = _agent getHitPointDamage _x;
	//diag_log format ["Saving hitpoint %1 with %2",_x,_v];
	if (_v > 0) then
	{
		_array set [count _array,[_x,_v]];
	};
} forEach DZ_PlayerHitpoints;
_agent setVariable ["damageArray",_array];


//_strc = format["Hit: %1; Selection: %2; Damage: %3; Slots: %4; Items: %5; Hitpoint: %6 (total: %7)",_bone,_selection,_damage,_slots,_items,_hitpoint,_total];
//statusChat [_strc,""];
//copyToclipboard _strc;

if (_total > 0.5 and isClass(configFile >> "cfgModifiers" >> _hitpoint)) then
{
	[0,_agent,_hitpoint] call event_modifier;
	if (_total == 1) then
	{
		[2,_agent,_hitpoint,1] call event_modifier;
	};
};


//death
if (_agent getVariable ["health",DZ_HEALTH] <= 0) then
{
	_final = 1;
	//dbStats ["murder",_source,1];
};
if (_agent getVariable ["blood",DZ_BLOOD] <= 0) then
{
	_final = 1;
	//dbStats ["murder",_source,1];
};

if ( isPlayer _source && isPlayer _agent ) then
{
	_itemInHands = typeOf (itemInHands _source);
	if ( _isMelee ) then
	{
		if ( _itemInHands == "" ) then
		{
			admin_log format ['%1(uid=%2) HIT %3(uid=%4) by %5 into %6.',name _source, getPlayerUID _source, name _agent, getPlayerUID _agent, "fist", _bone];
		}
		else
		{
			admin_log format ['%1(uid=%2) HIT %3(uid=%4) by %5 into %6.',name _source, getPlayerUID _source, name _agent, getPlayerUID _agent, _itemInHands, _bone];
		};
	}
	else
	{
		admin_log format ['%1(uid=%2) SHOT %3(uid=%4) by %5 into %6.',name _source, getPlayerUID _source, name _agent, getPlayerUID _agent, _itemInHands, _bone];
	};

	admin_log format ['%1(uid=%2) STATUS S::%3 B::%4 H::%5 HP::%6.',name _agent, getPlayerUID _agent, _agent getVariable 'shock', _agent getVariable 'blood', _agent getVariable 'health', damage _agent];
};

ProfileStop "event_assessDamage.sqf";

call damage_publishBleeding;

_final