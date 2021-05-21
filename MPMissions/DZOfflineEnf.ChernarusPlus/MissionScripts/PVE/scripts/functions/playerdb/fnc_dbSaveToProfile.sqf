private ["_uid","_agent","_vars","_char","_quickbar","_state","_items","_itemInHands","_hands","_slots"];

_uid = _this select 0;
_agent = _this select 1;
_vars = [["unconscious",false],["modifiers",[]],["modstates",[]],["myNotifiers",[]],["damageArray",[]],["exposure",0],["bloodtype","BloodONeg"],["blood",0],["health",0],["shock",0],["feet",0],["energy",0],["water",0],["stomach",0],["restrainedwith",0],["bodytemperature",0],["heatcomfort",0],["diet",0],["bleedingsources","[]"],["wet",0],["musclecramp",0],["penalties",0],["totalWeight",0],["totalHeatIsolation",0],["bleedingLevel",0],["underwater",0]];
_char = [false,"",[0,0,0]];
_quickbar = ["","","","","","","","",""];
_state = [];
_items = []; 
_hands = []; 
_slots = [];

if (isNull _agent) exitWith {true};

// alive 0 | model/type 1 | pos 2 | dir 3 
_char set [0,((lifeState _agent == "ALIVE")&&(not captive _agent))];
_char set [1,(typeOf _agent)];
_char set [2,(getPosATL _agent)];
_char set [3,(getDir _agent)];
_char set [4,0]; //_char set [4,(vectorUp _agent)];
_char set [5,0]; // _char set [5,(vectorDir _agent)];
_char set [6,(animationState _agent)];

// Save CHAR 
null = [_uid, _char] spawn 
{
	null = callFunction ["DataBaseWrite","CHAR",format["UID_%1",_this select 0],format["%1",_this select 1]];
	// if (DB_DEBUG) then {diag_log format ["Save CHAR: %2  | UID: %1",_this select 0,_this select 1]};
};


// Save STATE
{ 

	_value = _agent getVariable [(_x select 0),(_x select 1)];
	
	if ((_x select 0) == "bleedingsources" ) then 
	{
		_value = call compile _value;
	};

	_state set [(count _state),[(_x select 0),_value]]; 

} forEach _vars;

null = [_uid, _state] spawn 
{
	null = callFunction ["DataBaseWrite","STATE",format["UID_%1",_this select 0],format["%1",_this select 1]];
	// if (DB_DEBUG) then {diag_log format ["Save STATE: %2  | UID: %1",_this select 0,_this select 1]};
};


// Save QUICKBAR
{
	private ["_quickIdx"];
	_quickIdx =	_agent getQuickBarIndexOfEntity _x;
	if ( _quickIdx > -1 ) then { _quickbar set [_quickIdx,(typeOf _x)] };
} forEach (itemsInInventory _agent);

null = [_uid, _quickbar] spawn 
{
	null = callFunction ["DataBaseWrite","QUICKBAR",format["UID_%1",_this select 0],format["%1",_this select 1]];
	// if (DB_DEBUG) then {diag_log format ["Save STATE: %2  | UID: %1",_this select 0,_this select 1]};
};


// Save ITEMS 
// Inventory Items [ SLOT 0 | TYPE 1 | STATE 2 [dmg 0| qty/ammo 1 | wet 2] | ITEMS 3]
{
	private ["_slotItem","_items"];
	_slotItem = _agent itemInSlot _x;
	if !(isNull _slotItem) then 
	{
		_slots set [count _slots, toUpper _x];
		// diag_log format ["_slotItem: %1 | %2",_slotItem,_x];
		// _items set [ (count _items), [(typeOf _slotItem),(_slotItem call fnc_getItemState),([_agent,_slotItem] call fnc_getInvItems)]];
		_items = [(typeOf _slotItem),(_slotItem call fnc_getItemState),([_agent,_slotItem] call fnc_getInvItems)];
		
		null = [_x,_uid,_items] spawn 
		{
			null = callFunction ["DataBaseWrite",format["%1",toUpper (_this select 0)],format["UID_%1",(_this select 1)],format["%1",(_this select 2)]];
			// if (DB_DEBUG) then {diag_log format ["Save ITEMS: %2  | UID: %1",_this select 0,_this select 1]};
		};
	};
} forEach (itemEnumSlots _agent);

null = [_uid,_slots] spawn 
{
	null = callFunction ["DataBaseWrite","SLOTS",format["UID_%1",(_this select 0)],format["%1",(_this select 1)]];
};
		
		
// Save HANDS 
_itemInHands = itemInHands _agent;
if !(isNull _itemInHands) then 
{
	_hands = [(typeOf _itemInHands),(_itemInHands call fnc_getItemState),([_agent,_itemInHands] call fnc_getInvItems)];
};

null = [_uid, _hands] spawn 
{
	null = callFunction ["DataBaseWrite","HANDS",format["UID_%1",_this select 0],format["%1",_this select 1]];
	// if (DB_DEBUG) then {diag_log format ["Save ITEMS: %2  | UID: %1",_this select 0,_this select 1]};
};

if (DB_DEBUG) then {diag_log format [":::: dbSaveToProfile: %1 DONE!",_char]};

true
