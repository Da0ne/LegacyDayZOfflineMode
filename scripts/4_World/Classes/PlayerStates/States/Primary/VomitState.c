class VomitState extends StateBase
{
	//this is just for the state parameters set-up and is called even if the state doesn't execute, don't put any gameplay code in here
	void OnInit()
	{
		m_StateType = StateTypes.PRIMARY;
		m_Priority = 100;
		m_ID = StateIDs.STATE_VOMIT;
		m_QuitOnAnimFinish = true;
	}
	
	//!gets called every frame
	void OnUpdateServer(PlayerBase player)
	{

	}

	void OnUpdateClient(PlayerBase player)
	{

	}
	
	//!gets called once on an state which is being activated
	void OnGetActivatedServer(PlayerBase player)
	{
		//timer.Run(10, this, "Destroy");
		Debug.Log("OnGetActivated VomitState called", "PlayerState");
		player.GetStatStomach().Set(0);
		player.GetStatStomachWater().Set(0);
		player.GetStatEnergy().Set(0);
		//PlayAnimation(DayZPlayerConstants.CMD_ACTIONFB_VOMIT);
	}

	void OnGetActivatedClient(PlayerBase player)
	{
		
	}

	//!only gets called once on an active state that is being deactivated
	void OnGetDeactivatedServer(PlayerBase player)
	{
		Debug.Log("OnGetDeactivated VomitState called", "PlayerState");
	}

	void OnGetDeactivatedClient(PlayerBase player)
	{
		Debug.Log("OnGetDeactivated VomitState called", "PlayerState");
	}
}
