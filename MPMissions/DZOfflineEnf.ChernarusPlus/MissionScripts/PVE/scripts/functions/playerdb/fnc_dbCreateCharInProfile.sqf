private ["_uid","_key"];

_uid = _this;

_key = format["UID_%1",_uid];

null = callFunction ["DataBaseWrite","CHAR",_key,format["%1",[false,"",[0,0,0]]]];

if (DB_DEBUG) then {diag_log format [":::: dbCreateCharInProfile: %1",_key]};

true
