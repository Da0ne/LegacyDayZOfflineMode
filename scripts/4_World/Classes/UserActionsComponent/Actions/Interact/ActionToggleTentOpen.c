class ActionToggleTentOpen: ActionInteractBase
{
	void ActionToggleTentOpen()
	{
		m_MessageSuccess = "I've performed action.";
		m_MessageFail = "I cannot perform action.";
		//m_Animation = "open";
	}

	int GetType()
	{
		return AT_TOGGLE_TENT_OPEN;
	}

	string GetText()
	{
		return "toggle";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{	
		if ( player && target )
		{
			float max_action_distance = 1; //m_MaximalActionDistance;
			
			if ( target.IsInherited(EN5C_CarTent) ) max_action_distance = 5.0;
			else if ( target.IsInherited(EN5C_LargeTent) ) max_action_distance = 4.0;
			else if ( target.IsInherited(EN5C_MediumTent) ) max_action_distance = 3.0;
			
			float distance = Math.AbsInt(vector.Distance(target.GetPosition(),player.GetPosition()));
			
			if (  distance <= max_action_distance /*&& player.IsFacingTarget(target)*/ && ActionCondition(player,target,item) )	
			{
				if ( target.IsInherited(TentBase) ) 
				{
					string selection;
					GetGame().GetPlayerCursorObjectComponentName( player, /*out*/ selection );
					
					TentBase tent = (TentBase)target;
					
					if ( tent.CanToggleSelection(selection) )
					{
						return true;
					}
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		if ( target != NULL && target.IsInherited(TentBase) ) 
		{
			string selection;
			GetGame().GetPlayerCursorObjectComponentName( player, /*out*/ selection );
			
			TentBase tent = (TentBase)target;
			tent.ToggleSelection( selection );
			
			//regenerate pathgraph
			tent.SetAffectPathgraph( true, false );
			
			if (item.CanAffectPathgraph())
			{
				//Start update
				Timer update_timer = new Timer ( CALL_CATEGORY_SYSTEM );
				update_timer.Run ( 0.1, GetGame(), "UpdatePathgraphRegionByObject", new Param1<Object>(tent), false );
			}
		}
	}
};