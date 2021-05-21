class ActionGiveSalineTargetCB : ActionContinuousBaseCB
{
	private const float QUANTITY_USED_PER_SEC = 100;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousQuantity(QUANTITY_USED_PER_SEC);
	}
};

class ActionGiveSalineTarget: ActionContinuousBase
{
	private const float MAXIMAL_TARGET_DISTANCE = 1;
	
	void ActionGiveSalineTarget()
	{
		m_CallbackClass = ActionGiveSalineTargetCB;
		m_MessageStartFail = "Bag is empty.";
		m_MessageStart = "Player started giving you saline.";
		m_MessageSuccess = "Player finished giving you saline.";
		m_MessageFail = "Player moved and giving you saline was canceled.";
		m_MessageCancel = "You stopped giving saline.";
		//m_Animation = "givesaline";
	}
	
	void CreateConditionComponents() 
	{	
		
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(MAXIMAL_TARGET_DISTANCE);	
	}

	int GetType()
	{
		return AT_GIVE_SALINE_T;
	}
		
	string GetText()
	{
		return "give saline to target";
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		float efficiency = player.GetQuantityEfficiency( GetType() );
		if ( efficiency == -1 )
		{
			efficiency = 1;
		}
		PlayerBase ntarget = target;
		Param1<float> nacdata = nacdata;
		//ntarget.m_PlayerStats.m_Blood.Add(nacdata.param1 * efficiency);//BLOOD_REPLACE
		float delta = nacdata.param1 * efficiency;
		player.SetHealth("GlobalHealth", "Blood", player.GetHealth("GlobalHealth", "Blood") + delta );
	}
};