class Spotlight extends ItemBase
{
	private bool	m_IsFolded;
	
	static const string SEL_REFLECTOR 				= "reflector";
	static const string SEL_CORD_FOLDED 			= "cord_folded";
	static const string SEL_CORD_PLUGGED 			= "cord_plugged";
	
	static const string SEL_INVENTORY 				= "inventory";
	static const string SEL_PLACING 				= "placing";
	static const string SEL_GLASS_F 				= "glass_folded";
	static const string SEL_GLASS_U 				= "glass_unfolded";
	static const string SEL_REFLECTOR_F				= "reflector_folded";
	static const string SEL_REFLECTOR_FAR_F			= "reflector_far_folded";
	static const string SEL_REFLECTOR_U				= "reflector_unfolded";
	static const string SEL_REFLECTOR_FAR_U			= "reflector_far_unfolded";
	
	/*
	Spotlight, folded and unfolded.
	*/
	
	void Spotlight()
	{
		Fold();
	}

	void OnWorkStart()
	{
		SetPilotLight(true);
	}

	void OnWorkStop()
	{
		SetPilotLight(false);
	}
	
	// Called when this device is picked up
	void EEItemLocationChanged( EntityAI old_owner, EntityAI new_owner ) 
	{
		super.EEItemLocationChanged(old_owner, new_owner);
		
		// When the item is picked up by a player
		if (new_owner)
		{
			Fold();
			GetCompEM().SwitchOff();
		}
	}

	void OnIsPlugged(EntityAI source_device)
	{
		PlayerBase player = GetOwnerPlayer();
		
		if (!player)
		{
			HideSelection( SEL_CORD_FOLDED );
			ShowSelection( SEL_CORD_PLUGGED );
		}
	}

	void OnIsUnplugged( EntityAI last_energy_source )
	{
		PlayerBase player = GetOwnerPlayer();
		
		if ( !player )
		{
			ShowSelection( SEL_CORD_FOLDED );
			HideSelection( SEL_CORD_PLUGGED );
		}
	}

	void OnPlacementFinished( PlayerBase player )
	{
		Unfold();
	}

	void Fold()
	{
		m_IsFolded = true;
		
		ShowSelection( SEL_INVENTORY );
		ShowSelection( SEL_GLASS_F );
		ShowSelection( SEL_REFLECTOR_F );
		ShowSelection( SEL_REFLECTOR_FAR_F );
		
		HideSelection( SEL_PLACING );
		HideSelection( SEL_REFLECTOR );
		HideSelection( SEL_CORD_FOLDED );
		HideSelection( SEL_CORD_PLUGGED );
		HideSelection( SEL_GLASS_U );
		HideSelection( SEL_REFLECTOR_U );
		HideSelection( SEL_REFLECTOR_FAR_U );
	}

	void Unfold()
	{
		m_IsFolded = false;
		
		ShowSelection( SEL_PLACING );
		ShowSelection( SEL_REFLECTOR_U );
		ShowSelection( SEL_REFLECTOR_FAR_U );
		ShowSelection( SEL_GLASS_U );
		ShowSelection( SEL_REFLECTOR );
		
		HideSelection( SEL_INVENTORY );
		HideSelection( SEL_GLASS_F );
		HideSelection( SEL_REFLECTOR_F );
		HideSelection( SEL_REFLECTOR_FAR_F );
		
		if ( GetCompEM().IsPlugged() )
		{
			HideSelection( SEL_CORD_FOLDED );
			ShowSelection( SEL_CORD_PLUGGED );
		}
		else
		{
			ShowSelection( SEL_CORD_FOLDED );
			HideSelection( SEL_CORD_PLUGGED );
		}
	}

	void OnStoreSave(ParamsWriteContext ctx)
	{   
		super.OnStoreSave(ctx);
		
		// Save folded/unfolded state
		autoptr Param1<bool> par_b_is_folded = new Param1<bool>( m_IsFolded );
		ctx.Write( par_b_is_folded );
	}

	void OnStoreLoad(ParamsReadContext ctx)
	{   
		super.OnStoreLoad(ctx);
		
		// Load folded/unfolded state
		autoptr Param1<bool> par_b_is_folded = new Param1<bool>( false );
		ctx.Read( par_b_is_folded );
		
		if (par_b_is_folded.param1)
		{
			Fold();
		}
		else
		{
			Unfold();
		}
	}

	bool ConditionIntoInventory ( EntityAI  player ) 
	{
		// Commented out so Reposition action is possible to execute
		return true; //GetCompEM().IsCordFolded(); 
	}

	bool ConditionIntoHands ( EntityAI player ) 
	{
		// Commented out so Reposition action is possible to execute
		return true; //GetCompEM().IsCordFolded();
	}

	bool IsFolded()
	{
		return m_IsFolded;
	}
}

// Temporal class for new User Actions
class EN5C_Spotlight extends Spotlight
{
	void OnHologramBeingPlaced( PlayerBase player )
	{
		ItemBase item_in_hands = player.GetHologram().GetParentObject();
	
		//TO DO toto sa mi nepaci, treba to urobit nejako aby bol argument arrray/set
		player.GetHologram().RefreshVisual( SEL_CORD_PLUGGED );
		player.GetHologram().RefreshVisual( SEL_CORD_FOLDED );
	}
}