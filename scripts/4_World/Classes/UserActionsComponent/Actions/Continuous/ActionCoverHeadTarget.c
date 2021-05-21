class ActionCoverHeadTargetCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 2;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionCoverHeadTarget: ActionContinuousBase
{
	
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	
	void ActionCoverHeadTarget()
	{
		m_CallbackClass = ActionCoverHeadTarget;
		m_MessageStartFail = "There's nothing left.";
		m_MessageStart = "Player started putting sack on your head.";
		m_MessageSuccess = "Player finished putting sack on your head.";
		m_MessageFail = "Player moved and putting sack on was canceled.";
		m_MessageCancel = "You stopped putting sack on.";
		//m_Animation = "INJECTEPIPENS";
	}
	
	void CreateConditionComponents() 
	{	
		
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTMan(MAXIMAL_TARGET_DISTANCE);		
	}

	int GetType()
	{
		return AT_COVER_HEAD_T;
	}
		
	string GetText()
	{
		return "put on targets head";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{	
		if ( !IsWearingHeadgear(target,item) && item.GetDamage() < 1 ) 
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
		PlayerBase ntarget = target;
		ntarget.CreateInInventory("EN5C_BurlapSackCover","cargo_headgear");
		item.TransferModifiers(ntarget);
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
					return true;
				}
			}
		}
		return false;
	}
};