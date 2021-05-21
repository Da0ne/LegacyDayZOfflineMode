class ActionGiveBloodSelfCB : ActionContinuousBaseCB
{
	private const float QUANTITY_USED_PER_SEC = 100;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousQuantity(QUANTITY_USED_PER_SEC);
	}
};

class ActionGiveBloodSelf: ActionContinuousBase
{
	void ActionGiveBloodSelf()
	{
		m_CallbackClass = ActionGiveBloodSelfCB;
		m_MessageStartFail = "There's no blood left.";
		m_MessageStart = "I have started giving blood myself";
		m_MessageSuccess = "I gave myself blood.";
		m_MessageFail = "I have moved and giving blood was canceled.";
		m_MessageCancel = "I stopped giving self a blood.";
		//m_Animation = "GIVEBLOODS";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}

	int GetType()
	{
		return AT_GIVE_BLOOD_S;
	}
		
	string GetText()
	{
		return "give blood";
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		float efficiency = player.GetQuantityEfficiency( GetType() );
		if ( efficiency == -1 )
		{
			efficiency = 1;
		}
		Param1<float> nacdata = acdata;
		//player.m_PlayerStats.m_Blood.Add(nacdata.param1 * efficiency);//BLOOD_REPLACE
		float delta = nacdata.param1 * efficiency;
		player.SetHealth("GlobalHealth", "Blood", player.GetHealth("GlobalHealth", "Blood") + delta );
		
		int itembloodtype = 0; //item.GetVar(BloodType);
		int bloodtypetarget = player.m_PlayerStats.m_BloodType.Get();
		bool bloodmatch = MiscGameplayFunctions.MatchBloodCompatibility(itembloodtype, bloodtypetarget);
		if ( !bloodmatch )
		{
			player.m_ModifiersManager.ActivateModifier(MDF_HEMOLYTIC_REACTION);
		}	
		if ( item.IsKindOf("EN5C_BloodSyringe") )
		{
			ItemBase syringe = GetGame().CreateObject("EN5C_Syringe", player.GetPosition());
			MiscGameplayFunctions.TransferItemProperties(item, syringe, true, false, true);
			item.Delete();
			player.TakeEntityToHands(syringe);
		}
	}
};