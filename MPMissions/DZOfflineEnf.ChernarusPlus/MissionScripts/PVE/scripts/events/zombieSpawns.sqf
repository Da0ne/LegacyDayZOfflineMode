DZ_InfectedKilled = {
private["_agent","_uid","_waitTime"];
//diag_log format ["Infected Handel: called"];
_agent = _this select 0;
_killer = _this select 1;
_waitTime = diag_tickTime;

_nearBy = nearestObjects [_killer, ["ZombieBase"], 250];
{

if ( (vehicle _killer) distance [(getpos _x select 0),(getpos _x select 1)] >= 175 ) then {
    _pre = _x getVariable ["lifetime",0];
	_new = _pre - 30;
	_x setVariable ["lifetime",_new];
};

if ( (_x getVariable ["lifetime",0]) <= 0 ) then {
 diag_log format ["Infected Handel: Zombie lifetime ended at [%1]",(getpos _agent)];
 deleteVehicle _x;
};


}forEach _nearBy;


waitUntil {(diag_tickTime - _waitTime) > 10};
deleteVehicle _agent;
};

"Deploy_ZombieFunc" addPublicVariableEventHandler
{
	    private["_person","_tentOri","_dist","_tentSize","_tentX","_tentY","_tentZ","_pos","_ori","_xPos","_yPos","_zPos","_waterCheck","_heightCheck"];
	    _array = _this select 1;

		_person = (_array select 0);
		_tentOri = (_array select 1) select 0;
		_dist = (_array select 1) select 1;
		_tentSize = (_array select 2);
		_tentX = (_tentSize select 0) * 0.5;
		_tentY = (_tentSize select 1) * 0.5;
		_tentZ = (_tentSize select 2) * 0.5;

		if ( _dist <= 50 ) then {
			_dist = 80 + (floor(random 45));
		};

		_pos = getPosASL _person;
		_ori = ((direction _person) - (floor(random 180)));
		_cosOri = cos _ori;
		_sinOri = sin _ori; 
		_xPos = (_pos select 0) + (sin _ori * _dist);
		_yPos = (_pos select 1) + (cos _ori * _dist);
		_zPos = (_pos select 2);
		
		_waterCheck = true;
		_heightCheck = true;

		_posTentX = _xPos + _cosOri * _tentX - _sinOri * _tentY;
		_posTentY = _yPos + _cosOri * _tentY + _sinOri * _tentX;
		
		_height = surfaceHeightASL [_posTentX, _posTentY, _zPos + 0.2];
		_sur = surfaceTypeASL [_posTentX, _posTentY, _zPos + 0.2];

		if ( _height > 5 || _height < -0.2 ) then
		{
			_heightCheck = false;
		};

		if ( _sur == "FreshWater" || _sur == "sea") then
		{
			_waterCheck = false;
		};

		if ( _heightCheck && _waterCheck ) then
		{
			_posTentX = _xPos + _cosOri * _tentX - _sinOri * -_tentY;
			_posTentY = _yPos + _cosOri * -_tentY + _sinOri * _tentX;
			
			_height = surfaceHeightASL [_posTentX, _posTentY, _zPos + 0.2];
			_sur = surfaceTypeASL [_posTentX, _posTentY, _zPos + 0.2];
			
			if ( _height > 5 || _height < -0.2 ) then
			{
				_heightCheck = false;
			};

			if ( _sur == "FreshWater" || _sur == "sea") then
			{
				_waterCheck = false;
			};	
		};

		if ( _heightCheck && _waterCheck ) then
		{
			_posTentX = _xPos + _cosOri * -_tentX - _sinOri * -_tentY;
			_posTentY = _yPos + _cosOri * -_tentY + _sinOri * -_tentX;
			
			_height = surfaceHeightASL [_posTentX, _posTentY, _zPos + 0.2];
			_sur = surfaceTypeASL [_posTentX, _posTentY, _zPos + 0.2];
			
			if ( _height > 5 || _height < -0.2 ) then
			{
				_heightCheck = false;
			};
			
			if ( _sur == "FreshWater" || _sur == "sea") then
			{
				_waterCheck = false;
			};	
		};

		if ( _heightCheck && _waterCheck ) then
		{
			_posTentX = _xPos + _cosOri * -_tentX - _sinOri * _tentY;
			_posTentY = _yPos + _cosOri * _tentY + _sinOri * -_tentX;
			
			_height = surfaceHeightASL [_posTentX, _posTentY, _zPos + 0.2];
			_sur = surfaceTypeASL [_posTentX, _posTentY, _zPos + 0.2];

			if ( _height > 5 || _height < -0.2 ) then
			{
				_heightCheck = false;
			};

			if ( _sur == "FreshWater" || _sur == "sea") then
			{
				_waterCheck = false;
			};	
		};
		
		//boundingBox
		//_bCenter = boundingCenter TentMedium_Pitched;

		if ( _heightCheck ) then
		{
			if ( _waterCheck ) then
			{
				//check slope
				_terrainNormal = RoadWayNormalAsl [_xPos, _yPos, _pos select 2];
				_terrainSlope = atg(sqrt((_terrainNormal select 0)*(_terrainNormal select 0) + (_terrainNormal select 1)*(_terrainNormal select 1))/(_terrainNormal select 2) );

				if ( _terrainSlope < 80 ) then
				{
					 //test collision
					 _bbox = (collisionBox [[_xPos, _yPos, _zPos + _tentZ + 0.2 ], _tentSize ,[vectorDir _person, RoadWayNormalAsl [_xPos, _yPos, _zPos]], [_person, _person]]);
					 if ( !_bbox ) then
					 {
                       //[_person,'depoly zombie!','colorAction'] call fnc_playerMessage;

				        _cfg = configFile >> "CfgSpawns";
				        _Arr = getArray(_cfg >> "types");
				        _type = _Arr select floor random count _Arr;

                       _vehicleAI = createAgent [_type, [_xPos, _yPos], [], (random 125), "CAN_COLLIDE"]; 
					   _vehicleAI setPosASL [_xPos+floor(random 25), _yPos+floor(random 30), _pos select 2];
					   _vehicleAI addeventhandler ["HandleDamage",{_this call event_hitZombie} ];
					   _vehicleAI addeventhandler ["killed",{ null = _this spawn DZ_InfectedKilled; } ];
					   _vehicleAI setDir floor(random 360);
					   _vehicleAI setVariable ["lifetime",320];

					 }
					 else
					 {
						//[_person,'Not Enough Space to depoly zombie!','colorAction'] call fnc_playerMessage;
					 };
				}
				else
				{
					//[_person,'the slope is too steep!','colorAction'] call fnc_playerMessage;		
				};
			}
			else
			{
				//[_person,'Water Surface','colorAction'] call fnc_playerMessage;
			};
		}
		else
		{
			//[_person,'Terrian too high for zeds!','colorAction'] call fnc_playerMessage;		
		};
};
