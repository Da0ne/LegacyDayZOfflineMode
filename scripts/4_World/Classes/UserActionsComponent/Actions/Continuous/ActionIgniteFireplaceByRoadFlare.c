class ActionIgniteFireplaceByRoadFlareCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 4;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionIgniteFireplaceByRoadFlare: ActionContinuousBase
{
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	
	void ActionIgniteFireplaceByRoadFlare()
	{
		m_CallbackClass = ActionIgniteFireplaceByRoadFlareCB;
		m_MessageStartFail = "There's nothing to ignite.";
		m_MessageStart = "I have started igniting the fireplace with a road flare.";
		m_MessageSuccess = "I have ignited the fireplace with a road flare.";
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
		return AT_IGNITE_FIREPLACE_BY_ROADFLARE;
	}
		
	string GetText()
	{
		return "ignite by road flare";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{	
		if ( target && target.IsFireplace() )
		{
			EN5C_FireplaceBase fireplace_target = target;
			
			if ( !fireplace_target.IsBurning()  &&  item.HasEnergyManager()  &&  item.GetCompEM().IsWorking() ) 
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
		Print("OnComplete for ActionIgniteFireplaceByRoadFlare");
		EN5C_FireplaceBase fireplace_target = target;
		
		//check if road flare is lit
		if ( !item.GetCompEM().IsWorking() )
		{
			SendMessageToClient(player, fireplace_target.MESSAGE_IGNITE_FLARE_NOT_LIT );
			return;
		}
		
		//check kindling
		if ( !fireplace_target.CheckKindling() )
		{
			SendMessageToClient(player, fireplace_target.MESSAGE_IGNITE_NO_KINDLING );
			return;
		}
		
		//check wetness
		if ( !fireplace_target.CheckWetness() )
		{
			SendMessageToClient(player, fireplace_target.MESSAGE_IGNITE_TOO_WET );
			return;
		}
		
		//check roof
		if ( fireplace_target.IsBaseFireplace() || fireplace_target.IsBarrelHoles() )	//fireplace and barrel with Holes only
		{
			if ( !fireplace_target.CheckRoof() )
			{
				SendMessageToClient(player, fireplace_target.MESSAGE_IGNITE_UNDER_ROOF );
				return;
			}
		}
		
		//check rain
		if ( fireplace_target.IsBaseFireplace() || fireplace_target.IsBarrelHoles() )	//fireplace and barrel with Holes only
		{
			if ( !fireplace_target.CheckRain() )
			{
				SendMessageToClient(player, fireplace_target.MESSAGE_IGNITE_RAIN );
				return;
			}
		}
		
		//check surface
		if ( !fireplace_target.CheckSurface() )
		{
			SendMessageToClient(player, fireplace_target.MESSAGE_IGNITE_IN_WATER );
			return;
		}	
		
		//complete action
		//remove grass
		ItemBase clutter_cutter = GetGame().CreateObject ( fireplace_target.OBJECT_CLUTTER_CUTTER , target.GetPosition(), true, false );
		clutter_cutter.SetOrientation ( target.GetOrientation() );
		
		//start fire
		fireplace_target.StartFire();
	}
};