class ActionExtinguishFireplaceCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 6;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionExtinguishFireplace: ActionContinuousBase
{
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	
	float m_QuantityToExtinguish;
	
	void ActionExtinguishFireplace()
	{
		m_CallbackClass = ActionExtinguishFireplaceCB;
		//m_CommandUID = DayZPlayerConstants.CMD_ACTION_FIREESTINGUISHER
		m_MessageStartFail = "There's nothing to extinguish.";
		m_MessageStart = "I have started extinguishing the fireplace.";
		m_MessageSuccess = "I have extinguished the fireplace.";
		m_MessageFail = "I have canceled the extinguish action.";
		m_MessageCancel = "I have stopped extinguishing the fireplace.";
		m_QuantityToExtinguish = 500; //ml
		
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_FIREESTINGUISHER;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
	}
	
	void CreateConditionComponents() 
	{
		
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTDummy;
	}

	int GetType()
	{
		return AT_EXTINGUISH_FIREPLACE;
	}
		
	string GetText()
	{
		return "extinguish";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{	
		if ( target.IsFireplace() )
		{
			EN5C_FireplaceBase fireplace_target = target;
			
			if ( fireplace_target.IsBurning() && item.GetDamage() < 1 ) 
			{
				return true;
			}		
		}
		
		return false;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		EN5C_FireplaceBase fireplace_target = target;

		//check enough water
		if ( item.GetQuantity2() < m_QuantityToExtinguish )
		{
			player.MessageImportant ( fireplace_target.MESSAGE_EXTINGUISH_NOT_ENOUGH_WATER );
			return true;
		}
		
		//complete action
		//remove quantity
		item.AddQuantity ( -m_QuantityToExtinguish );
		
		//add wetness to fireplace
		float wetness = fireplace_target.GetWet();
		wetness = Math.Clamp ( wetness, fireplace_target.PARAM_BURN_WET_THRESHOLD, 1 );
		fireplace_target.SetWet ( wetness );
		
		//stop fire
		fireplace_target.StopFire();
	}
};