// private["_array"];
_agent = _this;
//save lifestate
switch (lifeState _agent) do
{
	case "UNCONSCIOUS": {
		_agent setVariable ["unconscious",true];
	};
	case "ALIVE": {
		_agent setVariable ["unconscious",false];
	};
	default {
		_agent setVariable ["unconscious",false];
	};
};

true
