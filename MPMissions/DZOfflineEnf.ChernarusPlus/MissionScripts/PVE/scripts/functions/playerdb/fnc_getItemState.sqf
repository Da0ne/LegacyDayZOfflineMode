private ["_itemState","_itemVars","_storeVariables","_mid"];

_itemState = [];

_itemVars = [];

if (isNull _this) exitWith {true};

_storeVariables = _this call 
	{	
		if (_this isKindOf "CfgVehicles" ) exitWith 
		{
			getArray (configFile >> "CfgVehicles" >> typeOf _this >> "storeVariables")
		};

		if (_this isKindOf "cfgWeapons" ) exitWith 
		{
			getArray (configFile >> "cfgWeapons" >> typeOf _this >> "storeVariables")
		};
		
		if (_this isKindOf "CfgMagazines" ) exitWith 
		{
			getArray (configFile >> "CfgMagazines" >> typeOf _this >> "storeVariables")
		};
		
		["RVMAT","CustomTexture","ammo","power","wet","internalenergy","liquidType","modifiers","food_stage","message","color","butane","fire","note","ropemat","lidopen","busy","filledWith","ison","temperature","used","state"]
	};
	
	
{
	_var = _this getVariable _x;
	
	 if !(isNil "_var") then 
	 {
			null = call {
					
					if ( _x == "message") exitWith 
					{
							
							_mid = _this getVariable ["message-id",(floor random 999)+(floor random 999)];
							
							null = callFunction ["DataBaseWrite",format ["MESSAGE-%1",_mid],format ["UID_%1",_uid],_var];

							_this setVariable ["message-id",_mid];
							
							_itemVars set [(count _itemVars),[_x,_mid]];
					};
					
					_itemVars set [(count _itemVars),[_x,_var]];
				};
		};
} forEach _storeVariables;


_itemState set [0,(damage _this)];

null = call 
{

	if ( _this isKindOf "MagazineBase") exitWith 
	{
		_itemState set [1,(magazineAmmo _this)];
	};
	
	_itemState set [1,(quantity _this)];	
};

_itemState set [2,_itemVars];

_itemState


