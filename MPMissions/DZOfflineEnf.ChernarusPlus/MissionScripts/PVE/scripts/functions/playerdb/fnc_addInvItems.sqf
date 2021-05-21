private ["_obj","_items"];

_obj = _this select 0;

_items = _this select 1;

if (isNull _obj) exitWith {true};

if !(typeName _items == "ARRAY") exitWith {true};

{
		private["_class","_state","_inv","_item"];
		
		_class = _x select 0;
		
		_state = _x select 1;
		
		_inv = _x select 2;
				
		_isCargo = true;
		
		_objPOS = getpos _obj;


		//_item = _obj createInCargo _class;
		_item = createVehicle [_class, [(_objPOS select 0),(_objPOS select 1),(_objPOS select 2)], [], 0, "CAN_COLLIDE"];
		sleep 0.3;
		_obj moveToInventory _item;
		
		/*if( count (itemsInCargo _obj) > 0 ) then
		{
			moveToGround _item;
			_obj moveToCargo _item;
		};*/
		
		if (isNull _item) then
		{
			_isCargo = false;
			_item = _obj createInInventory _class;
				
		};		

		if ( typeName _state == "ARRAY" ) then 
		{
			if !( count _state <= 0 ) then 
			{ 
				null = [_item,_state] call fnc_addItemState;
			};
		};

		
		if ( typeName _inv == "ARRAY" ) then 
		{
			if !( count _inv <= 0 ) then 
			{ 
				null = [_item,_inv] call fnc_addInvItems;
			};
		};
		
		
		
		null = call 
		{
			if (_item isKindOf "FoodItemBase") exitWith {
				// _var = _item getVariable ["food_stage",['Raw',0,0,0]];
				// [_item,(_var select 0)] call fnc_changeFoodStage;
				_item call event_fnc_foodStage;
			};
		};
		
} forEach _items;

true
