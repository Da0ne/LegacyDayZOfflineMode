class Tremor: ModifierBase
{
	private const float	 	TREMOR_DECREMENT_PER_SEC = 0.008;

	void Init()
	{
		m_TrackActivatedTime				= false;
		m_ID 							= MDF_TREMOR;
		float	m_TickIntervalInactive 	= DEFAULT_TICK_TIME_INACTIVE;
		float	m_TickIntervalActive 	= DEFAULT_TICK_TIME_ACTIVE;
	}

	private bool ActivateCondition(PlayerBase player)
	{
		if ( player.GetStatTremor().Get() > player.GetStatTremor().GetMin() )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	private bool DeactivateCondition(PlayerBase player)
	{
		if ( player.GetStatTremor().Get() == player.GetStatTremor().GetMin() )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	private bool OnTick(PlayerBase player, float deltaT)
	{
		player.GetStatTremor().Add( (TREMOR_DECREMENT_PER_SEC*deltaT) );
		player.SetShakeBodyFactor(player.GetStatTremor().Get());
		//_person SetBodyShaking tremor; ASK GAMEPLAY PROGRAMMERS TO EXPOSE THIS ENGINE FUNCTION IN ENSCRIPT
		//PrintString( "Tremor:" + ToString(tremor) );
	}
};