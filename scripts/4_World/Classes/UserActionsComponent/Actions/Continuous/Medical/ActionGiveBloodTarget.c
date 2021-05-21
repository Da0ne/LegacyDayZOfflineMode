class ActionGiveBloodTargetCB : ActionContinuousBaseCB
{
	private const float QUANTITY_USED_PER_SEC = 100;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousQuantity(QUANTITY_USED_PER_SEC);
	}
};

class ActionGiveBloodTarget: ActionContinuousBase
{
	private const float MAXIMAL_TARGET_DISTANCE = 1;
	
	void ActionGiveBloodTarget()
	{
		m_CallbackClass = ActionGiveBloodTargetCB;
		m_MessageStartFail = "It is empty.";
		m_MessageStart = "Player started giving you blood.";
		m_MessageSuccess = "Player finished giving you blood.";
		m_MessageFail = "Player moved and giving you blood was canceled.";
		m_MessageCancel = "You stopped giving blood.";
		//m_Animation = "giveblood";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionTarget = new CCTMan(MAXIMAL_TARGET_DISTANCE);
		m_ConditionItem = new CCINonRuined;
	}

	int GetType()
	{
		return AT_GIVE_BLOOD_T;
	}
		
	string GetText()
	{
		return "give blood to target";
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata  )
	{	
		float efficiency = player.GetQuantityEfficiency( GetType() );
		if ( efficiency == -1 )
		{
			efficiency = 1;
		}
		
		PlayerBase ntarget = target;
		item.TransferModifiers(ntarget);
		Param1<float> nacdata = acdata;
		//ntarget.m_PlayerStats.m_Blood.Add(nacdata.param1 * efficiency);//BLOOD_REPLACE
		float delta = nacdata.param1 * efficiency;
		player.SetHealth("GlobalHealth", "Blood", player.GetHealth("GlobalHealth", "Blood") + delta );

		int itembloodtype = 0; //item.GetVar(BloodType);
		int bloodtypetarget = ntarget.m_PlayerStats.m_BloodType.Get();
		bool bloodmatch = MiscGameplayFunctions.MatchBloodCompatibility(itembloodtype, bloodtypetarget);
		if ( !bloodmatch )
		{
			ntarget.m_ModifiersManager.ActivateModifier(MDF_HEMOLYTIC_REACTION);
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