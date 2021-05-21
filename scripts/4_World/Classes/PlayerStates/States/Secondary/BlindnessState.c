class BlindnessState extends StateBase
{
	Material m_MatGauss;
	const int BLUR_DURATION = 3000;
	//this is just for the state parameters set-up and is called even if the state doesn't execute, don't put any gameplay code in here
	void OnInit()
	{
		m_StateType = StateTypes.SECONDARY;
		m_Priority = 0;
		m_ID = StateIDs.STATE_BLINDNESS;
		m_QuitOnAnimFinish = true;
	}
	
	//!gets called every frame
	void OnUpdateServer(PlayerBase player)
	{
	}

	void OnUpdateClient(PlayerBase player)
	{
		int effect_mid_point = BLUR_DURATION / 2;
		float value = (int)((Math.AbsInt((GetGame().GetTime() % BLUR_DURATION) - effect_mid_point)));
		value =  value / BLUR_DURATION;
		PPEffects.SetBlurDrunk(value);
	}
	
	//!gets called once on an state which is being activated
	void OnGetActivatedServer(PlayerBase player)
	{

	}

	void OnGetActivatedClient(PlayerBase player)
	{

	}

	void OnGetDeactivatedServer(PlayerBase player)
	{
		Debug.Log("OnGetDeactivated CoughState called", "PlayerState");
	}
	
	//!only gets called once on an active state that is being deactivated
	void OnGetDeactivatedClient(PlayerBase player)
	{
		Debug.Log("OnGetDeactivated CoughState called", "PlayerState");
	}
	
}
