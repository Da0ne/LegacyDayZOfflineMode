class ActionGiveSalineSelfCB : ActionContinuousBaseCB
{
	private const float QUANTITY_USED_PER_SEC = 100;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousQuantity(QUANTITY_USED_PER_SEC);
	}
};

class ActionGiveSalineSelf: ActionContinuousBase
{
	
	void ActionGiveSalineSelf()
	{
		m_CallbackClass = ActionGiveSalineSelfCB;
		m_MessageStartFail = "There's nothing in it.";
		m_MessageStart = "I have started giving me saline";
		m_MessageSuccess = "I have salined myself.";
		m_MessageFail = "I have moved and giving saline was canceled.";
		m_MessageCancel = "I stopped giving saline myself.";
		//m_Animation = "givesaline";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}

	int GetType()
	{
		return AT_GIVE_SALINE_S;
	}
		
	string GetText()
	{
		return "give saline";
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		float efficiency = player.GetQuantityEfficiency( GetType() );
		if ( efficiency == -1 )
		{
			efficiency = 1;
		}	
		
		item.TransferModifiers(player);
		Param1<float> nacdata = acdata;
		//player.m_PlayerStats.m_Blood.Add(nacdata.param1 * efficiency);//BLOOD_REPLACE
		float delta = nacdata.param1 * efficiency;
		player.SetHealth("GlobalHealth", "Blood", player.GetHealth("GlobalHealth", "Blood") + delta );
	}
};