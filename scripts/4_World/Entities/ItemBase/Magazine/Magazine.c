class Magazine extends InventoryItemSuper
{
	//! Gets magazine ammo count
	proto native int GetAmmoCount();
	//! Sets magazine ammo count, MP safe
	proto native void SetAmmoCount(int ammoCount);
	//! Adds magazine ammo, MP safe
	void AddAmmoCount(int ammoCount)
	{
		SetAmmoCount(GetAmmoCount() + ammoCount);
	}
	//! returns max rounds for this mag (returns "count" config value)
	int GetAmmoMax()
	{
		return ConfigGetInt("count");
	}
	//! set max rounds for this mag
	void SetAmmoMax()
	{
		SetAmmoCount( GetAmmoMax() );
	}
	//! Returns if this entity is Magazine
	bool IsMagazine()
	{
		return true;
	}
	
	
	bool CanBeSplit()
	{
		if(!GetGame().IsNewPlayer() ) return false;
	
		if( ConfigGetBool("canBeSplit") )
		{
			if( this.GetAmmoCount() > 1 )
			{
				return true;
			}
			else
			{
				return false;			
			}
		}
	}
	
	void SplitItem()
	{
		if ( !CanBeSplit() )
		{
			return;
		}
		
		float quantity = this.GetAmmoCount();
		float split_quantity_new = Math.Floor( quantity / 2 );
		
		this.AddAmmoCount(-split_quantity_new);
		
		PlayerBase player = this.GetOwnerPlayer();
		Magazine new_item;
		if( player )
		{
			new_item = player.CopyInventoryItem( this );
		}
		else
		{
			new_item = GetGame().CreateObject(this.GetType(), this.GetPosition() );
		}
			
		new_item.SetAmmoCount( split_quantity_new );
	}
	
	
	void CombineItems( ItemBase other_item )
	{
		if( !CanBeCombined(other_item) ) return;
		if( other_item.GetType() != this.GetType() ) return;
		
		Magazine other_magazine = other_item;
		float other_item_quantity = other_magazine.GetAmmoCount();
		float this_free_space = this.GetAmmoMax() - this.GetAmmoCount();
		float quantity_used = 0;
		
		if( other_item_quantity > this_free_space )
		{
			quantity_used = this_free_space;
		}
		else
		{
			quantity_used = other_item_quantity;
		}
		if( quantity_used!= 0 )
		{
			this.AddAmmoCount(quantity_used);
			other_magazine.AddAmmoCount(-quantity_used);
			if(other_magazine.GetAmmoCount() <= 0) other_magazine.Delete();
		}
	}
	
	
	
};