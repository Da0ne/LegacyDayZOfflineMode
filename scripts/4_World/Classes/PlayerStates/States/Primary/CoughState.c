class CoughState extends StateBase
{
	//this is just for the state parameters set-up and is called even if the state doesn't execute, don't put any gameplay code in here
	void OnInit()
	{
		m_StateType = StateTypes.PRIMARY;
		m_Priority = 0;
		m_ID = StateIDs.STATE_COUGH;
		m_QuitOnAnimFinish = true;
	}
	
	//!gets called every frame
	void OnUpdateServer(PlayerBase player)
	{

	}

	void OnUpdateClient(PlayerBase player)
	{
		int i;
		i = i + 0;
	}
	
	//!gets called once on an state which is being activated
	void OnGetActivatedServer(PlayerBase player)
	{
		PlaySound("Action_Vomitting_0");
		player.MessageStatus("Coughing placeholder sound playing");
	}

	//!gets called once on an state which is being activated
	void OnGetActivatedClient(PlayerBase player)
	{
	}

	//!only gets called once on an active state that is being deactivated
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
