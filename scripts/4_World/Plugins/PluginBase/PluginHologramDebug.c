class PluginHologramDebug extends PluginBase
{	
	static PluginHologramDebug GetInstance()
	{
		return GetPlugin( PluginHologramDebug );
	}

	void ~PluginHologramDebug()
	{
		
	}

	// Client side
	void TogglePlacingDebug()
	{
		PlayerBase player = GetGame().GetPlayer();
		//ItemBase obj_hands = player.GetItemInHands();
		
		if ( player.GetItemInHands() != NULL )
		{
			player.RPCSingleParam(RPC_PLACING_DEBUG, NULL);
		}
	}
	
	// Client side	
	void PlaceHologramDebug()
	{
		PlayerBase player = GetGame().GetPlayer();
		//ItemBase obj_hands = player.GetItemInHands();		
		
		if ( player.GetItemInHands() != NULL && player.GetHologram() != NULL )
		{
			player.RPCSingleParam(RPC_PLACING_FINISH, NULL);
		}
	}
	
	void OnRPC(int rpc_type, ParamsReadContext ctx, PlayerBase player)
	{
		// Server side
		if ( rpc_type == RPC_PLACING_DEBUG )
		{	
			if( player.GetHologram() == NULL )
			{	
				player.PlacingStart( player.GetItemInHands().GetType() );
			}
			else
			{
				player.PlacingStop();
			}
			
		}
	}
}