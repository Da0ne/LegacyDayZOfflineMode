/*
class ActionAttachCB
{
	void CreateActionComponent()
	{
		
	}
}
*/
class ActionAttach: ActionSingleUseBase
{
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	
	void ActionAttach()
	{
		//m_MessageStartFail = "m_MessageStartFail";
		//m_MessageStart = "m_MessageStart";
		m_MessageSuccess = "I've attached the object.";
		//m_MessageFail = "m_MessageFail";
		//m_MessageCancel = "m_MessageCancel";
		

	}

	void CreateConditionComponents()
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined(MAXIMAL_TARGET_DISTANCE);
	}
	
	int GetType()
	{
		return AT_ATTACH;
	}
		
	string GetText()
	{
		return "attach";
	}
	
	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		if ( player && target && item )
		{
			string type = item.GetType();
			autoptr TStringArray slots = new TStringArray;
			autoptr TStringArray attachments = new TStringArray;
			
			g_Game.ConfigGetTextArray("cfgVehicles " + item.GetType() + " inventorySlot", slots);
			
			if ( slots.Count() <= 0 )
			{
				string one_slot;
				g_Game.ConfigGetText("cfgVehicles " + item.GetType() + " inventorySlot", one_slot);
				slots.Insert(one_slot);
			}
			
			g_Game.ConfigGetTextArray("cfgVehicles " + target.GetType() + " attachments", attachments);
			
			if ( attachments.Count() <= 0 || slots.Count() <= 0)
			{
				return false;
			}

			for ( int i = 0; i < slots.Count(); i++ )
			{
				string slot = slots.Get(i);
				for ( int y = 0; y < attachments.Count(); y++ )
				{
					string att = attachments.Get(y);

					if ( slot == att )
					{
						ItemBase item_in_slot;
						EntityAI target_entity = (EntityAI) target;
						EntityAI item_entity = (EntityAI) item;
						
						int slot_index = item_entity.GetSlotId();
						
						item_in_slot = target_entity.FindAttachment( slot_index );
						if ( !item_in_slot )
						{
							return true;
						}
					}
				}
			}
		}
		
		return false;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		EntityAI target_entity = (EntityAI) target; // cast to ItemBase
		EntityAI item_entity = (EntityAI) item;
		//target_entity.TakeEntityAsAttachment( item );
		int slot_index = item_entity.GetSlotId();
		target_entity.TakeEntityAsAttachmentToSlot( item, slot_index  );
		
	 }
};