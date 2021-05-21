class ActionTurnOnTorch: ActionTurnOnWhileInHands
{
	void ActionTurnOnTorch()
	{
		m_MessageSuccess = "I ignitied torch.";
		m_MessageFail = "It's burnt out.";
		//m_Animation = "ignite";
	}

	int GetType()
	{
		return AT_TURN_ON_TORCH;
	}

	string GetText()
	{
		return "ignite";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{	
		if ( target != NULL && target.IsInherited(EN5C_Torch) && target.GetDamage() < 1 && item != NULL && item.GetDamage() < 1 )
		{
			EN5C_Torch target_torch = (EN5C_Torch)target;
			
			if ( target_torch.GetCompEM().CanWork() )
			{
				if ( item.IsKindOf( "EN5C_Matchbox" ) && item.GetQuantity() > 0 )
				{
					return true;
				}
				else if ( item.IsInherited( EN5C_Torch) )
				{
					EN5C_Torch item_torch = (EN5C_Torch)item;
					
					if ( item_torch.GetCompEM().IsWorking() )
					{
						return true;
					}
				}
			}
		}
		
		return false;
	}

	void OnStart( PlayerBase player, Object target, ItemBase item )
	{
		if ( item != NULL && item.IsKindOf( "EN5C_Matchbox" ) )
		{
			GetGame().CreateSoundOnObject(item, "matchboxStrike", 50, false);
		}
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		if ( target != NULL && target.IsInherited(EN5C_Torch) )
		{
			if ( item.IsKindOf( "EN5C_Matchbox" ) )
			{
				item.AddQuantity2(-1,true);
			}
			
			EN5C_Torch torch = (EN5C_Torch)target;
			torch.GetCompEM().SwitchOn();
		}
	}
};