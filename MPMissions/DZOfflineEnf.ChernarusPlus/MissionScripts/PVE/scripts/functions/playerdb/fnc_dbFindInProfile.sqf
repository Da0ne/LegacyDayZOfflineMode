private ["_re","_uid","_key"];

_uid = _this;

_key = format["UID_%1",_uid];

_re = callFunction ["DataBaseRead","CHAR",_key];

if (_re == "") exitWith {[false,"",[0,0,0]]};

diag_log format [":::: dbFindCharInProfile: %1",_key];

call compile _re
