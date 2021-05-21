class ActionAttachWheels: ActionAttach
{
	int m_wheel_hub;
	
	void ActionAttachWheels()
	{
		//m_MessageStartFail = "m_MessageStartFail";
		//m_MessageStart = "m_MessageStart";
		m_MessageSuccess = "I've attached the object.";
		//m_MessageFail = "m_MessageFail";
		//m_MessageCancel = "m_MessageCancel";
		
		m_wheel_hub = -1;
	}

	int GetType()
	{
		return AT_ATTACH_WHEELS;
	}
		
	string GetText()
	{
		return "attach wheels";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		string test = player.GetName();
		string test2 = item.GetType();
		string test3 = target.GetName();
		
		if ( player && target && item )
		{
			string v_component;
			//g_Game.GetPlayerCursorObjectComponentName( player,v_component );
			
			if ( g_Game.GetPlayerCursorObjectComponentNameQuery(player,"wheel_1_1"))
			{
				Print( "Predni leve" );
				m_wheel_hub = 0;
				return true;
			}

			if ( g_Game.GetPlayerCursorObjectComponentNameQuery(player,"wheel_1_2"))
			{
				Print( "zadni leve" );
				m_wheel_hub = 1;
				return true;
			}

			if ( g_Game.GetPlayerCursorObjectComponentNameQuery(player,"wheel_1_3"))
			{
				Print( "prostredni leve" );
				m_wheel_hub = 2;
				return true;
			}

			if ( g_Game.GetPlayerCursorObjectComponentNameQuery(player,"wheel_1_4"))
			{
				Print( "druhe predni leve" );
				m_wheel_hub = 3;
				return true;
			}
			if ( g_Game.GetPlayerCursorObjectComponentNameQuery(player,"wheel_2_1"))
			{
				Print( "Predni prave" );
				m_wheel_hub = 4;
				return true;
			}

			if ( g_Game.GetPlayerCursorObjectComponentNameQuery(player,"wheel_2_2"))
			{
				Print( "zadni prave" );
				m_wheel_hub = 5;
				return true;
			}

			if ( g_Game.GetPlayerCursorObjectComponentNameQuery(player,"wheel_2_3"))
			{
				Print( "prostredni prave" );
				m_wheel_hub = 6;
				return true;
			}

			if ( g_Game.GetPlayerCursorObjectComponentNameQuery(player,"wheel_2_4"))
			{
				Print( "druhe predni prave" );
				m_wheel_hub = 7;
				return true;
			}		
		}
		
 		return false;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		EntityAI target_entity = (EntityAI) target; // cast to ItemBase
		int slot_index	= item.GetSlotId();
		int test	= m_wheel_hub;
		target_entity.TakeEntityAsAttachmentToSlot( item, slot_index );	
		target_entity.TakeEntityAsAttachment( item );
	}
};