class ActionCollectSampleSelfCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 2;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionCollectSampleSelf: ActionContinuousBase
{
	void ActionCollectSampleSelf()
	{
		m_CallbackClass = ActionCollectSampleSelfCB;
		m_MessageStartFail = "It's full";
		m_MessageStart = "I have started collecting sample";
		m_MessageSuccess = "I have collected sample.";
		m_MessageFail = "I have moved and fixing was canceled.";
		m_MessageCancel = "I stopped fixing.";
		//m_Animation = "bandage";
	}
	
	void CreateConditionComponents() 
	{		
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}

	int GetType()
	{
		return AT_COLLECT_SAMPLE_S;
	}
		
	string GetText()
	{
		return "collect sample";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{	
		if ( item.GetDamage() < 1 ) 
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		ItemBase syringe = GetGame().CreateObject("EN5C_BloodSyringe", player.GetPosition());
		MiscGameplayFunctions.TransferItemProperties(item, syringe, true, false, true);
		syringe.SetLiquidType2(player.m_PlayerStats.m_BloodType.Get());
		item.Delete();
		player.TakeEntityToHands(syringe);
		
	}
};