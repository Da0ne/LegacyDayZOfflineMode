class ActionPlaceFireplaceIndoor: ActionSingleUseBase
{
	private FireplacePoint m_ActualFireplacePoint;
	
	void ActionPlaceFireplaceIndoor()
	{
		m_MessageStartFail = "";
		m_MessageStart = "";
		m_MessageSuccess = "I placed the fireplace inside.";
		m_MessageFail = "I was unable to place the fireplace inside.";
		
		//m_TimeToCompleteAction = 4;
		//m_MaximalActionDistance = 1;
		//m_Animation = "eat";
	}
	
	int GetType()
	{
		return AT_PLACE_FIREPLACE_INDOOR;
	}
		
	string GetText()
	{
		return "place";
	}
	
	void CreateConditionComponents() 
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined( 10 );
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		string action_selection;
		GetDayZGame().GetPlayerCursorObjectComponentName( player, /*out*/ action_selection );
		
		if ( target && target.IsInherited( BuildingWithFireplace ) )
		{
			BuildingWithFireplace building = ( BuildingWithFireplace ) target;
			//TODO - add point-player distance restriction (~1.5m)
			if ( item && building.IsFireplaceActionSelection( action_selection ) )
			{
				if ( building.IsPointAvailable( action_selection ) )
				{
					return true;
				}
			}			
		}
		
		return false;
	}
	
	void OnStart( PlayerBase player, Object target, ItemBase item )
	{
		//get actual action_selection
		string action_selection;
		GetDayZGame().GetPlayerCursorObjectComponentName( player, /*out*/ action_selection );
		
		//get and set FireplacePoint reference
		BuildingWithFireplace building = ( BuildingWithFireplace ) target;
		m_ActualFireplacePoint = building.GetFirePointByActionSelection( action_selection );
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item /* fireplace in hands */, Param acdata )
	{	
		EN5C_FireplaceBase fireplace_in_hands = ( EN5C_FireplaceBase ) item;
		
		//get 'fire point pos' from 'fire point' memory point
		string fire_point = m_ActualFireplacePoint.GetFirePoint();
		BuildingWithFireplace building = ( BuildingWithFireplace ) target;
		//
		//create fireplace indoor
		EN5C_FireplaceIndoor fireplace_indoor = GetDayZGame().CreateObject( "EN5C_FireplaceIndoor", "0 0 0", false );

		//transfer all required parameters to this object (damage, wetness)
		fireplace_indoor.SetDamage( fireplace_in_hands.GetDamage() );
		fireplace_indoor.SetWet( fireplace_in_hands.GetWet() );
		
		//set position and direction to fireplace_pointX
		vector fire_point_pos = building.GetSelectionPosition( fire_point );
		vector fire_point_pos_world = building.ModelToWorld( fire_point_pos );
		fireplace_indoor.SetPosition( fire_point_pos_world );
		fireplace_indoor.SetOrientation( "0 1 0" );
		
		//transfer all cargo items to this object, position of all items must be preserved
		//fireplace in hands cannot have any items in it but this can be changed in the future
		Cargo cargo = fireplace_in_hands.GetCargo();
		for ( int i = 0; i < cargo.GetItemCount(); i++ )
		{
			int col;
			int row;
			cargo.GetItemPos( i, col, row );
			fireplace_indoor.TakeEntityToCargoAtPos( cargo.GetItem ( i ), fireplace_indoor.GetCargo(), row, col );
		}
		
		//transfer all attachments to this object
		int item_attachments_count = fireplace_in_hands.AttachmentsCount();
		for ( int j = 0; j < item_attachments_count; j++ )
		{
			EntityAI entity = fireplace_in_hands.GetAttachmentFromIndex( 0 );
			fireplace_indoor.TakeEntityAsAttachment( entity );
		}
		
		//set building settings (m_FireplacePoints) and fireplaceindoor settings
		m_ActualFireplacePoint.SetObject( fireplace_indoor );
		fireplace_indoor.SetFireplacePoint( m_ActualFireplacePoint );
	}
}