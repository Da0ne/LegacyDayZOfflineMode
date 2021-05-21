class ActionDigStashCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 10;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionDigStash: ActionContinuousBase
{
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	
	void ActionDigStash()
	{
		m_CallbackClass = ActionDigStashCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DIGSHOVEL;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
		
		m_MessageStartFail = "I can't dig here.";
		m_MessageStart = "I've started digging.";
		m_MessageSuccess = "I have stashed the chest.";
		m_MessageFail = "I couldn't dig the hole.";
		m_MessageCancel = "I've stopped digging.";
	}
	
	void CreateConditionComponents() 
	{	
		
		m_ConditionTarget = new CCTNonRuined(MAXIMAL_TARGET_DISTANCE);
		m_ConditionItem = new CCINonRuined;
	}

	int GetType()
	{
		return AT_DIG_STASH;
	}

	string GetText()
	{
		return "dig stash";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		if ( target  )
		{
			ItemBase target_IB = (ItemBase) target;
			
			// Check surface
			string surface_type;
			vector position = target.GetPosition();
			GetGame().SurfaceGetType ( position[0], position[2], surface_type );
			
			if ( surface_type == "CRHlina"  ||  surface_type == "CRForest1"  ||  surface_type == "CRForest2"  ||  surface_type == "CRGrass1"  ||  surface_type == "CRGrass2" )
			{
				if ( target_IB.ConfigGetBool("canBeDigged") )
				{
					return true;
				}
			}
		}
		
		return false;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		string target_type = target.GetType();
		
		if ( target_type == "UndergroundStash" )
		{
			UndergroundStash target_stash = (UndergroundStash) target;
			ItemBase chest = target_stash.GetStashedItem();
			
			if (chest)
			{
				chest.SetPosition( target_stash.GetPosition() );
				target_stash.SetStashedItem(NULL);
			}
			
			g_Game.ObjectDelete( target_stash );
		}
		else
		{
			UndergroundStash stash = GetGame().CreateObject("UndergroundStash", target.GetPosition(), false);
			target.SetPosition( target.GetPosition() + "0 -500 0" ); // TO DO: Do not use teleportation hack if possible!
			stash.SetStashedItem(target);
		}
	}
};