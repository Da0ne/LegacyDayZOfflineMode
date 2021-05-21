private ["_uid","_char","_key","_agent"];

_uid = _this;

_char = _uid call fnc_dbFindInProfile;

_key = format["UID_%1",_uid];

if !(_char select 0) exitWith {objNull};

_agent = player;//createAgent [(_char select 1),(_char select 2),[],0,"NONE"];

_agent setPosATL (_char select 2); // _agent setPos (_char select 2);	_agent setposASL (_char select 2);

_agent setDir (_char select 3);

_agent switchMove (_char select 6); 

// _agent setVectorUp (_char select 4);

// _agent setVectorDir (_char select 5);

null = _agent takeInHands objNull;

// STATE
{
	private ["_name","_value"];
	
	_name = _x select 0;
	
	_value =  _x select 1;
	
	if (_name == "bleedingsources" ) then 
	{	
		_value = str(_x select 1);
	};

	_agent setVariable[_name,_value];
	_agent setHit ["legs", 0];
	_agent setHit ["feet", 0];
	_agent setHit ["hands", 0];

} forEach (call compile callFunction ["DataBaseRead","STATE",_key]);


// HANDS
null = [_agent,(call compile callFunction ["DataBaseRead","HANDS",_key])] call fnc_addHandsItem;


// INVENTORY
_inventory = [];
{
	_re = call compile callFunction ["DataBaseRead",_x,_key];
	_inventory set [(count _inventory),_re];
} forEach ( call compile callFunction ["DataBaseRead","SLOTS",_key] );
null = [_agent,_inventory] call fnc_addInvItems;


diag_log format[":::: dbLoadFromProfile: %1",_char];

_agent
