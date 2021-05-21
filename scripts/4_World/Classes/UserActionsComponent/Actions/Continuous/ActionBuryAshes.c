class ActionBuryAshesCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 6;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionBuryAshes: ActionContinuousBase
{
	
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	
	void ActionBuryAshes()
	{
		m_CallbackClass = ActionBuryAshesCB;
		m_MessageStartFail = "I can't bury it here.";
		m_MessageStart = "I've started burying the ashes.";
		m_MessageSuccess = "I have buried the ashes.";
		m_MessageFail = "I couldn't bury the ashes.";
		m_MessageCancel = "I've stopped burying the ashes.";
		//m_Animation = "DIGGINGSHOVEL";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionTarget = new CCTNonRuined(MAXIMAL_TARGET_DISTANCE);
		m_ConditionItem = new CCINonRuined;
	}

	int GetType()
	{
		return AT_BURY_ASHES;
	}

	string GetText()
	{
		return "bury ashes";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		EN5C_FireplaceBase fireplace_target = ( EN5C_FireplaceBase ) target;
		
		if ( target  &&  target.IsFireplace() && fireplace_target.HasAshes() && !fireplace_target.IsBurning() && fireplace_target.GetTemperature() < fireplace_target.PARAM_MIN_FIRE_TEMPERATURE )
		{
			return true;
		}
		
		return false;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		EN5C_FireplaceBase fireplace_target = ( EN5C_FireplaceBase ) target;
		
		//check if fireplace cargo and attachments are empty
		if ( !fireplace_target.IsEmpty() )
		{
			SendMessageToClient(player, fireplace_target.MESSAGE_BURY_ASHES_FAILED_NOT_EMPTY );
		}
			
		//check hard surface
		string surface_type;
		vector position = target.GetPosition();
		GetGame().SurfaceGetType ( position[0], position[2], surface_type ); 
		if ( surface_type != "CRHlina"  &&  surface_type != "CRForest1"  &&  surface_type != "CRForest2"  &&  surface_type != "CRGrass1"  &&  surface_type != "CRGrass2" )
		{
			SendMessageToClient(player, fireplace_target.MESSAGE_BURY_ASHES_FAILED_SURFACE );
		}
		
		//destroy fireplace with ashes
		GetGame().ObjectDelete( target );	
	}
};