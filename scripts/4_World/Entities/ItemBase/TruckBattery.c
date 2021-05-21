/*
class TruckBattery : ItemBase
{
	bool ConditionAttach ( EntityAI parent )
	{
		string obj_type = parent.GetType()
		if ( obj_type == "EN5C_BatteryCharger"  ||  obj_type == "BatteryCharger" )
		{
			BatteryCharger charger = (BatteryCharger) parent;
			return charger.CanReceiveAttachment(this);
		}
		
		return true;
	}

	bool ConditionDetach ( EntityAI parent )
	{
		//string obj_type = parent.GetType()
		//if ( obj_type == "EN5C_BatteryCharger"  ||  obj_type == "BatteryCharger" )
		//{
			//return true;
		//}
		
		return true;
	}
}
*/