//-----------------------------------------------------------------------------
class Fountain extends BuildingSuper
{
};

//-----------------------------------------------------------------------------
class Church extends BuildingSuper
{
};

class PASBroadcaster extends EntityAI
{
	proto native void SwitchOn(bool onOff);
	proto native bool IsOn();
};

class StaticTransmitter extends EntityAI
{
	proto native void SwitchOn(bool onOff);
	proto native bool IsOn();
	proto native void SetNextChannel();
	proto native void SetPrevChannel();
	proto native float GetTunedFrequency();
	proto native void EnableBroadcast(bool state);
	proto native void EnableReceive(bool state);
	proto native bool IsBroadcasting();
	proto native bool IsReceiving();
};