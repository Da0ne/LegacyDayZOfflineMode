private ["_itemInHands","_itemsInInventory"];

_agent = _this;

_itemInHands = itemInHands _agent;

_itemsInInventory = itemsInInventory _agent;

	{

		if(isClass(configFile >> "CfgMagazines" >> typeOf _x)) then 
		{	
			_parent = itemParent _x;
			
			if(isClass(configFile >> "cfgWeapons" >> typeOf _parent)) then 
			{
				_a = moveToGround _x; 
				_b = moveToGround _parent;
				waitUntil {_a && _b};
				null = _parent moveToInventory _x;
				null = _agent moveToInventory _parent;
				// _agent playAction 'ReloadMagazine';
			};		
		};

	} forEach  _itemsInInventory;


/*
if !(isNull _itemInHands) then 
{
		if !(isClass(configFile >> "cfgWeapons" >> typeOf _itemInHands)) exitWith{};	
		{
			
			if(isClass(configFile >> "CfgMagazines" >> typeOf _x)) then 
			{	
				_a = moveToGround _x;
				_b = moveToGround _itemInHands;
				waitUntil {_a && _b};
				null =_itemInHands moveToInventory _x;
				
				// null = _agent takeInHands _itemInHands;
				// _agent moveInHands _parent;
			};
		} forEach (itemsInInventory _itemInHands);
};	
*/

true
