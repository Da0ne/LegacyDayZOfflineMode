private["_state","_poisonChance","_randnum","_result","_quant","_newItem"];
/*
	Function handling searching bushes for berries

	Author: Jan Tomasik
*/
_state = _this select 0;
switch _state do 
{
	case 0:
	{
		private["_person","_berry"];
		_person = _this select 1;
		_berry = _this select 2;
		_chance = _this select 3;
		_result = format["[1,_this,%1,%2] call player_pickBerry",str(_berry),_chance];	
		_person setVariable ["isUsingSomething",1];
		_person setSlotLock ["Hands", true];
		_person say3D ["ZombieAttack_10", 50, 111450];
		_person playAction ["ItemUseLong",compile _result];
	};
	case 1:
	{
		private["_person","_berry"];
		_person = _this select 1;
		
		_actionCanceled = _person getVariable ["isUsingSomething",0];
		if (_actionCanceled == 2) exitWith
		{
			[_person,"Current action was canceled",""] call fnc_playerMessage;	
			_person setVariable ["isUsingSomething",0];
			_person setSlotLock ["Hands", false];
			diag_log format ["PickBerry:::Lock::: %1", _person getSlotLock "Hands" ];
		};
		
		_berry = _this select 2;
		_chance = _this select 3;
		_name =  getText(configFile >> "CfgVehicles" >> _berry >> "displayName");
		_quant =  getNumber(configFile >> "CfgVehicles" >> _berry >> "stackedMax");
		_randnum = round(random _chance);
		if (_randnum<1) then {
			_person setSlotLock ["Hands", false];
			diag_log format ["PickBerry:::Lock::: %1", _person getSlotLock "Hands" ];
			_newItem = [_berry,_person] call player_addInventory;
			if(_quant == 0)then{
				_newItem setQuantity 0;
			}else{
				_newItem setQuantity 1;
			};
			[_person,format["I have found something!",_name],""] call fnc_playerMessage;
		} else {
			[_person,"I have not found a thing.",""] call fnc_playerMessage;
			_person setSlotLock ["Hands", false];
			diag_log format ["PickBerry:::Lock::: %1", _person getSlotLock "Hands" ];
		};

		_person setVariable ["isUsingSomething",0];
		_person setSlotLock ["Hands", false];
		diag_log format ["PickBerry:::Lock::: %1", _person getSlotLock "Hands" ];

	};
}
