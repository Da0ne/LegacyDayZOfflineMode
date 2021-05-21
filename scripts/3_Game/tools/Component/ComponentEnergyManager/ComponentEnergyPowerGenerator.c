class ComponentEnergyPowerGenerator : ComponentEnergyManager
{
	EntityAI m_SparkPlug;
	
	bool CanSwitchOn()
	{
		
		if ( !IsSwitchedOn()  &&  m_ThisEntityAI.GetDamage() < 1 )
		{
			// TO DO: Create UA for installing sparkplug when design for this action exists!
			//if ( HasSparkplug()  &&  m_SparkPlug.GetDamage() < 1 )
			//{
				if ( m_ThisEntityAI.GetCompEM().HasEnoughStoredEnergy() )
				{
					return true;
				}
			//}
		}
		
		return false;
	}

	bool HasSparkplug()
	{
		if ( m_SparkPlug )
		{
			return true;
		}
		
		return false;
	}

	void Event_OnItemAttached(EntityAI item, string slot_name)
	{
		super.Event_OnItemAttached(item, slot_name);
		
		string message;
		
		if ( item.IsKindOf("SparkPlug") )
		{
			message 		= "#user_action_elect_installed_sparkplug";
			m_SparkPlug		= item;
		}
		

		Log("Event_OnItemAttached");

		
		/* TODO: send message to player (currently we dont have player pointer)
		if ( player && message != "" )
		{
			player.MessageAction( message );
		}
		*/
	}

	void Event_OnItemDetached(EntityAI item, string slot_name)
	{
		super.Event_OnItemDetached(item, slot_name);
		
		string message;
		
		if ( item.IsKindOf("SparkPlug") )
		{
			message 		= "#user_action_elect_removed_sparkplug";
			m_SparkPlug		= NULL;
			
			if ( IsSwitchedOn() )
			{
				SwitchOff(); // Turn off the generator since the spark plug was removed
			}
		}
		
		Log("Event_OnItemDetached");
		
		/* TODO: send message to player (currently we dont have player pointer)
		if ( player && message != "" )
		{
			player.MessageAction( message );
		}
		*/
	}
}