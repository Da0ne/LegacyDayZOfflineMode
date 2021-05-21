class ActionCookOnStickCB : ActionContinuousBaseCB
{
	private const float REPEAT_AFTER_SEC = 1;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousRepeat(REPEAT_AFTER_SEC);
	}
};

class ActionCookOnStick: ActionContinuousBase
{
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	
	void ActionCookOnStick()
	{
		m_CallbackClass = ActionCookOnStickCB;
		m_MessageStartFail = "I have failed to start the action.";
		m_MessageStart = "I have started the action.";
		m_MessageFail = "I have failed the action.";
		m_MessageCancel = "I have stopped the action.";
		//m_Animation = "DRINK";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionTarget = new CCTNonRuined(MAXIMAL_TARGET_DISTANCE);
		m_ConditionItem = new CCINonRuined;
	}

	int GetType()
	{
		return AT_COOK_ON_STICK;
	}
		
	string GetText()
	{
		return "cook on stick";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{	
		if ( target && target.IsFireplace() )
		{
			EN5C_FireplaceBase fireplace_target = target;

			//barrel with holes
			if ( fireplace_target.IsBarrelHoles() )
			{
				EN5C_BarrelHoles_ColorBase barrel = ( EN5C_BarrelHoles_ColorBase ) fireplace_target;
				if ( barrel.IsOpened() )
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			
			//fireplace
			if ( fireplace_target.IsBaseFireplace() )
			{
				if ( !fireplace_target.IsInAnimPhase( fireplace_target.ANIMATION_OVEN ) && item.AttachmentsCount() > 0 && item.GetDamage() < 1 ) 
				{
					return true;
				}	
			}
			
			//TODO  add cooking in houses
			//fireplace indoor
		}
		
		return false;
	}

	void OnStart ( PlayerBase player, Object target, ItemBase item )
	{
		super.OnStart ( player, target, item );
		
		//add item as ingredient to fireplace
		EN5C_FireplaceBase fireplace_target = ( EN5C_FireplaceBase ) target;
		ItemBase food_on_stick = ( ItemBase ) item.GetAttachmentByType ( EN5C_Edible_Base );
		fireplace_target.AddToItemsOnStick ( food_on_stick );
	}

	void OnCancel ( PlayerBase player, Object target, ItemBase item, Param acdata  )
	{
		super.OnCancel ( player, target, item, acdata );
		
		//remove item as ingredient to fireplace
		EN5C_FireplaceBase fireplace_target = ( EN5C_FireplaceBase ) target;
		ItemBase food_on_stick = ( ItemBase ) item.GetAttachmentByType ( EN5C_Edible_Base );
		fireplace_target.RemoveFromItemsOnStick ( food_on_stick );
	} 
};