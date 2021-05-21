class EN5C_MetalWire extends ItemBase
{
	void OnIsUnplugged(EntityAI last_energy_source)
	{
		GetCompEM().UnplugAllDevices();
	}
}