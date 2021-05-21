class ActionCoverHeadSelfCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 2;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionCoverHeadSelf: ActionContinuousBase
{
	
	
	void ActionCoverHeadSelf()
	{
		m_CallbackClass = ActionCoverHeadSelfCB;
		m_MessageStartFail = "It's too torn.";
		m_MessageStart = "I have started putting sack on";
		m_MessageSuccess = "I have put sack on.";
		m_MessageFail = "I have moved and putting sack on was canceled.";
		m_MessageCancel = "I stopped putting sack on.";
		//m_Animation = "INJECTEPIPENS";
	}
	
	void CreateConditionComponents() 
	{	
		
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}

	int GetType()
	{
		return AT_COVER_HEAD_S;
	}
		
	string GetText()
	{
		return "put on head";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{	
		if ( IsWearingHeadgear(player,item) && item.GetDamage() < 1 ) 
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
		//setaperture will be called from here, or from statemachine
		GetGame().GetWorld().SetAperture(10000);
		player.CreateInInventory("EN5C_BurlapSackCover","cargo_headgear");
		item.TransferModifiers(player);
		item.Delete();
	}


	bool IsWearingHeadgear( PlayerBase player, ItemBase item )
	{
		EntityAI attachment;
		int attcount = player.AttachmentsCount();
		for (int att = 0; att < attcount; att++)
		{	
			attachment = player.GetAttachmentFromIndex(att);
			if ( attachment.IsItemBase() )
			{
				ItemBase attitem = attachment;
				if( attitem.KindOf("Headgear") )
				{
					Print("headgear");
					return false;
				}
			}
		}
		return true;
	}
};