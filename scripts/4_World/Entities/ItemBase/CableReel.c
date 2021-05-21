class CableReel extends ItemBase
{
	bool m_ForceIntoHands;

	static const string SEL_CORD_FOLDED 			= "cord_folded";
	static const string SEL_CORD_PLUGGED 			= "cord_plugged";
	
	void CableReel () 
	{
		m_ForceIntoHands = false;
	}

	void ForceIntoHandsNow ( PlayerBase player ) 
	{
		m_ForceIntoHands = true;
		player.TakeEntityToHands(this);
		m_ForceIntoHands = false;
	}
	
	bool ConditionIntoInventory ( EntityAI  player ) 
	{
		EntityAI owner_EAI = GetOwner();
		if ( owner_EAI  &&  owner_EAI.IsKindOf("EN5C_Fence"))
		{
			return true;
		}
		
		/*if ( GetCompEM().GetPluggedDevices()  &&  GetCompEM().GetPluggedDevices().Count() > 0 )
		{
			EntityAI powered_device = GetCompEM().GetPluggedDevices().Get(0).Ptr();
			
			if ( powered_device  &&  powered_device.IsKindOf ( "EN5C_BarbedWire" ) )
			{
				return true;
			}
			else
			{
				return false;
			}
		}*/
		
		bool allow_into_inv = !GetCompEM().IsPlugged();
		return allow_into_inv;
	}

	bool ConditionIntoHands ( EntityAI player ) 
	{
		if ( m_ForceIntoHands )
		{
			return true;
		}
		else
		{
			EntityAI owner_EAI = GetOwner();
			if ( owner_EAI  &&  owner_EAI.IsKindOf("EN5C_Fence"))
			{
				return true;
			}
			
			/*if ( GetCompEM().GetPluggedDevices()  &&  GetCompEM().GetPluggedDevices().Count() > 0 )
			{
				EntityAI powered_device = GetCompEM().GetPluggedDevices().Get(0).Ptr();
				
				if ( powered_device  &&  powered_device.IsKindOf ( "EN5C_BarbedWire" ) )
				{
					return true;
				}
				else
				{
					return false;
				}
			}*/
		}
		
		bool allow_into_inv = !GetCompEM().IsPlugged();
		return allow_into_inv;
	}

	bool ConditionOutOfHands ( EntityAI player ) 
	{
		return true;
	}
}

// Temporal class for new User Actions
class EN5C_CableReel extends CableReel
{
	void OnHologramBeingPlaced( PlayerBase player )
	{
		ItemBase item_in_hands = player.GetHologram().GetParentObject();
	
		//TO DO toto sa mi nepaci, treba to urobit nejako aby bol argument arrray/set
		player.GetHologram().RefreshVisual( SEL_CORD_PLUGGED );
		player.GetHologram().RefreshVisual( SEL_CORD_FOLDED );
	}
}
