class ActionReigniteFireplace: ActionInteractBase
{
	void ActionReigniteFireplace()
	{
		m_MessageStart = "I have started fire by blowing air into fireplace.";
		m_MessageSuccess = "I have started fire by blowing air into fireplace.";
		m_MessageFail = "I have failed to start fire by blowing air into fireplace.";
		m_MessageCancel = "I have stopped blowing air into fireplace.";
		//m_Animation = "";
	}

	int GetType()
	{
		return AT_REIGNITE_FIREPLACE;
	}

	string GetText()
	{
		return "blow air into fireplace";
	}
		
	bool ActionCondition ( PlayerBase player, Object target, ItemBase item )
	{
		EN5C_FireplaceBase fireplace_target = target;
		
		if ( !fireplace_target.IsBurning() && fireplace_target.GetTemperature() >= fireplace_target.PARAM_MIN_TEMP_TO_REIGNITE )
		{
			return true;
		}
		
		return false;
	}
		
	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		EN5C_FireplaceBase fireplace_target = target;
		
		//check kindling
		if ( !fireplace_target.CheckKindling() )
		{
			player.MessageImportant ( fireplace_target.MESSAGE_REIGNITE_NO_KINDLING );
			return;
		}
		
		//check wetness
		if ( !fireplace_target.CheckWetness() )
		{
			player.MessageImportant ( fireplace_target.MESSAGE_REIGNITE_TOO_WET );
			return;
		}
		
		//check rain
		if ( !fireplace_target.CheckRain() )
		{
			player.MessageImportant ( fireplace_target.MESSAGE_REIGNITE_RAIN );
			return;
		}
		
		//start fire
		fireplace_target.StartFire();
	}
};