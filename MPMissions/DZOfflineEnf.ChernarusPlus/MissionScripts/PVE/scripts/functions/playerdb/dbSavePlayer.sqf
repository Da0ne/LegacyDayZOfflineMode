private ["_uid","_agent"];
_first = _this select 0;
_uid   = _this select 1;
_agent = _this select 2;

_killed = false;

if ( _first > 0 ) then {
		
		_agent call dbSavePlayerPrep;
		[_uid, _agent] call fnc_dbSaveToProfile;
		diag_log format ["dbSavePlayer: Player [%2] [%1] Saved as alive",_uid,(_agent getVariable["Cli_Name",0])];
} else {

		_killed = true;
		_agent setDamage 1;
		[_uid, _agent] call fnc_dbDestroyProfile;
		diag_log format ["dbSavePlayer: Player [%2] [%1] Saved as dead",_uid,(_agent getVariable["Cli_Name",0])];

};

_killed