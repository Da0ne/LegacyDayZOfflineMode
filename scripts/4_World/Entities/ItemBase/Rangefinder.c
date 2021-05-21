class EN5C_Rangefinder extends EN5C_Inventory_Base
{
	void EN5C_Rangefinder()
	{
		
	}
	
	// How frequently the measurement should be taken
	float GetMeasurementUpdateInterval()
	{
		return 0.1;
	}
	
	// Measures the distance and returns result in formated string
	string DoMeasurement(PlayerBase player)
	{
		string 		message			= "";
		const float	MAX_RANGE 		= 9999;
		float 		energy_needed 	= GetCompEM().GetEnergyUsage() * GetMeasurementUpdateInterval();
		vector 		from 			= player.GetCameraPosition();
		vector 		to 				= from + (player.GetCameraDirection() * MAX_RANGE);
		vector 		contact_pos;
		vector 		contact_dir;
		int 		contactComponent;
		
		// Fire raycast
		GetGame().Raycast ( from, to, contact_pos, contact_dir, contactComponent, NULL , NULL, player, false, false, ObjIntersectIFire);
		//Debug.DrawArrow( from, contact_pos ); // Uncomment for debugging of raycast positions
		
		// Use energy
		bool is_device_working = GetCompEM().ConsumeEnergy( energy_needed );
		
		// Generate result
		if (is_device_working)
		{
			float dist = vector.Distance( from, contact_pos );
			dist = Math.Round(dist);
			message = "#range_finder_distance" + ": " + dist.ToString() + " #meters";
		}
		else
		{
			message = "#range_finder_distance" + ": ----" + " #meters";
		}
		
		
		return message;
	}
}