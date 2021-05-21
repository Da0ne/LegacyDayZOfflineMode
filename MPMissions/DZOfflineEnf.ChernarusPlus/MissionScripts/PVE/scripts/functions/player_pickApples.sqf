private["_task","_randnum","_result","_DoSpawn"];
/*
	Function handling searching trees for apples

	Author: Jan Tomasik
*/
_person = _this select 0;
_task = "";

_DoSpawn = {
	_person = _this select 0;
	_randnum = round(random _chance);
	_person setSlotLock ["Hands", false];
	_newItem = ['Fruit_Apple',_person] call player_addInventory;
	if(_quant == 0)then{
		_newItem setQuantity 0;
	}else{
		_newItem setQuantity 1;
	};
   [_person,format["I have found a few apples!"],"colorFriendly"] call fnc_playerMessage;
};

if ( (typeOf( itemInHands _person )) == "AxeBase" ) then {
	_task = "SHAKE"
} else {
	_task = "NO_TOOLS";
};

switch (_task) do {
	case "SHAKE" : {
		_result = format["['Fruit_Apple',_this] call _DoSpawn"];
		_person playAction ["GestureMeleeAxeSlash",compile _result];
	}; 
	case "NO_TOOLS" : {
		[_person,"I Can Not Shake the Tree without a tool!","colorImportant"] call fnc_playerMessage;
	}; 
};
