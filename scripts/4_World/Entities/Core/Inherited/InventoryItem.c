
//-----------------------------------------------------------------------------
class Weapon extends InventoryItemSuper
{
	bool IsWeapon()
	{
		return true;
	}

	proto native int GetMuzzlesCount();

	/**
	\brief Returns number of slots for attachments corrected for weapons
	*/
	int GetSlotsCountCorrect()
	{
		int ac = AttachmentsCount();
		int	sc = GetSlotsCount() + GetMuzzlesCount();
		if (ac > sc) sc = ac; // fix of some weapons which has 1 attachments but 0 slots...
		return sc;
	};
};

//-----------------------------------------------------------------------------
class ItemSuppressor extends InventoryItemSuper
{
};

typedef ItemSuppressor SuppressorBase;

//-----------------------------------------------------------------------------
class ItemWatch extends InventoryItemSuper
{
};

//-----------------------------------------------------------------------------
class ItemTransmitter extends InventoryItemSuper
{
	proto native void SetNextChannel();
	proto native void SetPrevChannel();
	proto native float GetTunedFrequency();
	proto native void EnableBroadcast(bool state);
	proto native void EnableReceive(bool state);
	proto native bool IsBroadcasting();
	proto native bool IsReceiving();
};

//-----------------------------------------------------------------------------
class ItemMegaphone extends InventoryItemSuper
{
}

//-----------------------------------------------------------------------------
class ItemRadio extends InventoryItemSuper
{
	proto native void TuneNext();
	proto native void TunePrev();
};

//-----------------------------------------------------------------------------
class ItemOptics extends InventoryItemSuper
{
};

typedef ItemOptics OpticBase;

//-----------------------------------------------------------------------------
class ItemGPS extends InventoryItemSuper
{
};

//-----------------------------------------------------------------------------
class ItemCompass extends InventoryItemSuper
{
};

//-----------------------------------------------------------------------------
class Clothing extends InventoryItemSuper
{
	bool IsClothing()
	{
		return true;
	}
};

typedef Clothing ClothingBase;

//-----------------------------------------------------------------------------
class ItemBook extends InventoryItemSuper
{
	void EEUsed(Man owner)
	{
		if ( GetGame().IsServer() )
		{
			 RPCSingleParam(RPC_READ_A_BOOK, NULL, owner);
		}
	}

	event bool OnUseFromInventory(Man owner)
	{
		return false;
	}
	
	void OnRPC( int rpc_type, ParamsReadContext  ctx)
	{
		super.OnRPC(rpc_type, ctx);
		
		if (rpc_type == RPC_READ_A_BOOK)
		{
			super.EEUsed(NULL);
		}
	}
};

//-----------------------------------------------------------------------------
class ItemGrenade extends InventoryItemSuper
{
};

typedef ItemGrenade GrenadeBase;

//-----------------------------------------------------------------------------
class ItemMap extends InventoryItemSuper
{
};
