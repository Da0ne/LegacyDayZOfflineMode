private ["_agent","_items","_arr"];

_agent = _this select 0;
_items = _this select 1;
_arr = [];
	
	if ( typeName _items != "ARRAY" ) exitWith {true};

	{
		private["_in"];
		
		_in = _items find (typeOf _x);
		if !( (typeOf _x) in _arr ) then
		{
			if ( _in > -1) then 
			{
				null = _agent setEntityToQuickBarIndex [_x,_in];
				_arr set [count _arr, (typeOf _x)];
			};
		};
	} forEach (itemsInInventory _agent);
	
true
