class ActionIgniteFireplaceByLighterCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 4;

	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionIgniteFireplaceByLighter: ActionContinuousBase
{
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	
	void ActionIgniteFireplaceByLighter()
	{
		m_CallbackClass = ActionIgniteFireplaceByLighterCB;
		m_MessageStartFail = "There's nothing to ignite.";
		m_MessageStart = "I have started igniting the fireplace with a petrol lighter.";
		m_MessageSuccess = "I have ignited the fireplace with a petrol lighter.";
		m_MessageFail = "I have canceled the igniting action.";
		m_MessageCancel = "I have stopped igniting the fireplace.";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionTarget = new CCTDummy;
		m_ConditionItem = new CCINonRuined;
	}

	int GetType()
	{
		return AT_IGNITE_FIREPLACE_BY_LIGHTER;
	}
		
	string GetText()
	{
		return "ignite by lighter";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{	
		if ( target && target.IsFireplace() )
		{	
			EN5C_FireplaceBase fireplace_target = target;
			
			if ( !fireplace_target.IsBurning() && item.GetDamage() < 1 ) 
			{
				if ( fireplace_target.IsBarrelHoles() ) 		//if barrel with holes
				{
					EN5C_BarrelHoles_ColorBase barrel = ( EN5C_BarrelHoles_ColorBase ) fireplace_target;
					if ( !barrel.IsOpened() )
					{
						return false;
					}
				}
				
				return true;
			}
		}
		
		return false;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		EN5C_FireplaceBase fireplace_target = target;

		//check kindling
		if ( !fireplace_target.CheckKindling() )
		{
			SendMessageToClient(player, fireplace_target.MESSAGE_IGNITE_NO_KINDLING );
		}
		
		//check wetness
		if ( !fireplace_target.CheckWetness() )
		{
			SendMessageToClient(player, fireplace_target.MESSAGE_IGNITE_TOO_WET );
		}
		
		//check roof
		if ( fireplace_target.IsBaseFireplace() || fireplace_target.IsBarrelHoles() )	//fireplace and barrel with Holes only
		{
			if ( !fireplace_target.CheckRoof() )
			{
				SendMessageToClient(player, fireplace_target.MESSAGE_IGNITE_UNDER_ROOF );
			}
		}
		
		//check rain
		if ( fireplace_target.IsBaseFireplace() || fireplace_target.IsBarrelHoles() )	//fireplace and barrel with Holes only
		{
			if ( !fireplace_target.CheckRain() )
			{
				SendMessageToClient(player, fireplace_target.MESSAGE_IGNITE_RAIN );
			}
		}
		
		//check surface
		if ( !fireplace_target.CheckSurface() )
		{
			SendMessageToClient(player, fireplace_target.MESSAGE_IGNITE_IN_WATER );
		}	
		
		//complete action
		//remove grass
		ItemBase clutter_cutter = GetGame().CreateObject ( fireplace_target.OBJECT_CLUTTER_CUTTER , target.GetPosition(), true, false );
		clutter_cutter.SetOrientation ( target.GetOrientation() );
		
		//start fire
		fireplace_target.StartFire();
	}
}