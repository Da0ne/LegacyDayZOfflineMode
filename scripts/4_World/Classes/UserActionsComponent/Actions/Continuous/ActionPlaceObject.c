class ActionPlaceObjectCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 4;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionPlaceObject: ActionContinuousBase
{
	//SELECTIONS
	
	const string SELECTION_PLACING 		= "placing";
	const string SELECTION_INVENTORY 	= "inventory";
	
	vector m_Min;
	vector m_Max;
		
	void ActionPlaceObject()
	{
		m_CallbackClass = ActionPlaceObjectCB;
		/*
		m_MessageStart = "Player started placing object.";
		m_MessageSuccess = "Player placed the object."
		m_MessageFail = "Player failed to place the object.";
		m_MessageCancel = "You canceled action.";
		*/
		m_MessageFail = "Placing of object failed because object is colliding with environment.";
		//m_Animation = "EAT";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	bool ActionCondition ( PlayerBase player, Object target, ItemBase item )
	{
		if ( player.IsPlacingObject() )
		{
			return true;
		}	
		
		return false;
	}

	int GetType()
	{
		return AT_PLACE_OBJECT;
	}
		
	string GetText()
	{
		return "Place object";
	}

	void OnStart ( PlayerBase player, Object target, ItemBase item )
	{
		//disable hologram position update
		player.GetHologram().SetUpdatePosition( false );
	}


	void OnCancel ( PlayerBase player, Object target, ItemBase item, Param acdata  )
	{
		//enable hologram position update
		if ( player.IsPlacingObject() ) 
		{
			player.GetHologram().SetUpdatePosition ( true );
		}
		
		return true;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		if ( !player.GetHologram().IsColliding() )
		{
			if ( item.CanBePlaced(player, player.GetHologram().GetProjectionObject().GetPosition()) )
			{
				//place object
				player.PlacingFinish( player.GetItemInHands() );
			}
			else
			{
				//enable hologram position update
				player.GetHologram().SetUpdatePosition( true );
				
				//message player
				SendMessageToClient(player, item.CanBePlacedFailMessage(player, player.GetHologram().GetProjectionObject().GetPosition()) );
			}
		}
		else
		{
			//enable hologram position update
			player.GetHologram().SetUpdatePosition ( true );
			
			//message player
			InformPlayers( player,target,UA_FAILED);
		}
	}
};
