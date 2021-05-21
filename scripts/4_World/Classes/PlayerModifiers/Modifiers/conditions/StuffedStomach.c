class StuffedStomach: ModifierBase
{
	private const int	 	FATALLY_STUFFED_TRESHOLD 	= 8000;
	
	void Init()
	{
		m_TrackActivatedTime				= false;
		m_ID 							= MDF_STUFFED;
		float	m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		float	m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}
	
	private bool ActivateCondition(PlayerBase player)
	{
		float stomachs = player.GetStatStomach().Get() + player.GetStatStomachWater().Get();
		if ( stomachs >= FATALLY_STUFFED_TRESHOLD )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	private void OnActivate(PlayerBase player)
	{
		//Print("Vomiting");
		player.GetStateManager().QueueUpPrimaryState( StateIDs.STATE_VOMIT );
	}


	private bool DeactivateCondition(PlayerBase player)
	{
		float stomachs = player.GetStatStomach().Get() + player.GetStatStomachWater().Get();
		if ( stomachs < FATALLY_STUFFED_TRESHOLD )
		{
			return false;
		}
		else
		{
			return true;
		}
	}

};