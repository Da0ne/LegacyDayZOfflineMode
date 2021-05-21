private ["_obj","_itemInHand","_hand","_itemInHandState","_itemInHandItems"];

_obj = _this select 0;

_itemInHand = _this select 1;

_itemInHandState = _itemInHand select 1;

_itemInHandItems = _itemInHand select 2;


if (isNull _obj) exitWith {true};

if !(typeName _itemInHand == "ARRAY") exitWith {true}; 

if !( count _itemInHand > 0 ) exitWith {true}; 

_hand = _obj createInHands (_itemInHand select 0);

if (isNull _hand) then
{
	_hand = (_itemInHand select 0) createVehicle (getPosATL _agent);
	_hand setPosATL (getPosATL _agent);
};	


if !(isNull _hand) then 
{	
		
	if ( typeName _itemInHandState == "ARRAY" ) then 
	{
		if !( count _itemInHandState <= 0 ) then 
		{ 
			[_hand,_itemInHandState] call fnc_addItemState;
		};
	};

	
	if ( typeName _itemInHandItems == "ARRAY" ) then 
	{
		if !( count _itemInHandItems <= 0 ) then 
		{ 
			[_hand,_itemInHandItems] call fnc_addInvItems;
		};
	};
};


true
