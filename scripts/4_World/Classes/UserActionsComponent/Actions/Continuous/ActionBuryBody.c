class ActionBuryBodyCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 6;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionBuryBody: ActionContinuousBase
{
	
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	
	void ActionBuryBody()
	{
		m_CallbackClass = ActionBuryBodyCB;
		m_MessageStartFail = "There's nothing to ignite.";
		m_MessageStart = "I have started igniting with hand drill.";
		m_MessageSuccess = "I have ignited the fireplace with a hand drill.";
		m_MessageFail = "I have canceled the igniting action.";
		m_MessageCancel = "I have stopped igniting the fireplace.";
		//m_Animation = "DIGGINGSHOVEL";
	}
	
	void CreateConditionComponents() 
	{	
		
		m_ConditionTarget = new CCTDummy;
		m_ConditionItem = new CCINonRuined;
	}

	int GetType()
	{
		return AT_BURY_BODY;
	}
		
	string GetText()
	{
		return "bury the body";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		EntityAI body_EAI = (EntityAI) target;
		if ( !body_EAI.IsAlive() )
		{
			string surface_type;
			vector position = body_EAI.GetPosition();
			GetGame().SurfaceGetType ( position[0], position[2], surface_type );
			
			if ( surface_type == "CRHlina"  ||  surface_type == "CRForest1"  ||  surface_type == "CRForest2"  ||  surface_type == "CRGrass1"  ||  surface_type == "CRGrass2" )
			{
				return true;
			}
		}
		
		return false;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		g_Game.ObjectDelete(target); 
	}
};
