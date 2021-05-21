class ActionTuneFrequencyOnGround : ActionInteractBase
{
	void ActionTuneFrequencyOnGround()
	{
		m_MessageSuccess = "I have tuned the frequency.";
		m_MessageFail = "Cannot tune the frequency on the device without power.";
		//m_Animation = "";
	}

	int GetType()
	{
		return AT_TUNE_FREQUENCY_ON_GROUND;
	}

	string GetText()
	{
		return "tune frequency";
	}
		
	bool ActionCondition ( PlayerBase player, Object target, ItemBase item )
	{
		EN5C_StaticRadio transmitter = ( EN5C_StaticRadio ) target;
		string selection;
		GetGame().GetPlayerCursorObjectComponentName( player, selection );
		
		if ( transmitter.CanOperate() && transmitter.GetDamage() < 1 && selection == "control_panel" )
		{
			return true;
		}
		
		return false;
	}
		
	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		EN5C_StaticRadio transmitter = ( EN5C_StaticRadio ) target;
		
		transmitter.SetNextFrequency ( player );	
	}
};