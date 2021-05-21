class BulletHitState extends StateBase
{
	Material m_MatColors;
	const int BLUR_DURATION = 3000;
	float color[4];
	//this is just for the state parameters set-up and is called even if the state doesn't execute, don't put any gameplay code in here
	void OnInit()
	{
		m_StateType = StateTypes.SECONDARY;
		m_Priority = 0;
		m_ID = StateIDs.STATE_BULLET_HIT;
		m_QuitOnAnimFinish = true;
		m_MatColors = GetGame().GetWorld().GetMaterial("graphics/materials/postprocess/colors");
	}
	
	//!gets called every frame
	void OnUpdateServer(PlayerBase player)
	{
		
	}

	void OnUpdateClient(PlayerBase player)
	{
		color[0] = 0.5;
		color[1] = 0.8;
		color[2] = 0.7;
		color[3] = 0.6;
		int effect_mid_point = BLUR_DURATION / 2;
		float value = (int)((Math.AbsInt((GetGame().GetTime() % BLUR_DURATION) - effect_mid_point)));
		value =  value / BLUR_DURATION;
		color[0] = value;
		if ( m_MatColors )
		{
			m_MatColors.SetParam("OverlayFactor", 1);
			m_MatColors.SetParam("OverlayColor", color);
		}
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
