private ["_uid","_agent","_key","_slots","_files"];

_uid = _this select 0;

_agent = _this select 1;

_key = format["UID_%1",_uid];

_slots = call compile callFunction ["DataBaseRead","SLOTS",_key];

_files = ["CHAR","STATE","HANDS","QUICKBAR","SLOTS"];

if ( count _slots > 0 ) then {_files = _files + _slots};

{ null = callFunction ["DataBaseDelete",_x,_key]; sleep 0.01} forEach _files;

if (DB_DEBUG) then {diag_log format [":::: dbDestroyProfile: %1", _uid]};

true
