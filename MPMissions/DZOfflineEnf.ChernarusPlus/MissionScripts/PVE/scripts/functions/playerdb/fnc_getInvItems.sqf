private ["_agent","_item","_arr","_itemsInInventory","_itemsInCargo","_isIn"];

_agent = _this select 0;

_item = _this select 1;

if (isNull _agent) exitWith {true};

_arr = [];

_itemsInInventory = itemsInInventory _item;

_itemsInCargo = itemsInCargo _item;

// diag_log format ["_item: %1",_item];
// diag_log format ["_itemsInInventory: %1",_itemsInInventory];
// diag_log format ["_itemsInCargo: %1",_itemsInCargo];

_isIn = call 
{
	if ( (count _itemsInCargo) < (count _itemsInInventory) && !( (count _itemsInCargo) <= 0) ) exitWith 
	{
		_itemsInCargo
	};
	_itemsInInventory
};

if !( count _isIn <= 0) then 
{
	{
		_arr set [(count _arr),[(typeOf _x),(_x call fnc_getItemState),([_agent,_x] call fnc_getInvItems)]];
	} forEach _isIn; 
};

_arr	

