class ActionPackTent: ActionInteractBase
{
	void ActionPackTent()
	{
		m_MessageSuccess = "I've packed tent.";
		m_MessageFail = "I cannot pack tent.";
		//m_Animation = "open";
	}

	int GetType()
	{
		return AT_PACK_TENT;
	}

	string GetText()
	{
		return "pack tent";
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
			
			if (  distance <= max_action_distance )	
			{
				if ( target.IsInherited(TentBase) ) 
				{
					TentBase tent = (TentBase)target;
					if ( tent.CanBePacked() )
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		if ( target != NULL && target.IsInherited(TentBase) ) 
		{
			TentBase tent = (TentBase)target;
			tent.Pack();
		}
	}
};