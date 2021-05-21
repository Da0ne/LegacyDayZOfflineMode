class ActionFillGeneratorTankCB : ActionContinuousBaseCB
{
	private const float QUANTITY_FILLED_PER_SEC = 50;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousFill(QUANTITY_FILLED_PER_SEC);
	}
};

class ActionFillGeneratorTank: ActionContinuousBase
{	
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	
	void ActionFillGeneratorTank()
	{
		m_CallbackClass = ActionFillGeneratorTankCB;
		m_MessageStartFail = "There's nothing left.";
		m_MessageStart = "I am filling the tank.";
		m_MessageSuccess = "I've poured fuel into generator.";
		m_MessageFail = "I've stopped filling the tank.";
		m_MessageCancel = "I've stopped filling the tank.";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined(MAXIMAL_TARGET_DISTANCE);
	}

	int GetType()
	{
		return AT_FILL_GENERATOR_TANK;
	}
		
	string GetText()
	{
		return "fill the tank";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		if ( target.IsKindOf("EN5C_PowerGenerator") )
		{
			EN5C_PowerGenerator pg = (EN5C_PowerGenerator) target;
			
			if ( pg.CanAddFuel( item ) )
			{
				return true;
			}
		}
		
		return false;
	}
	

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		EN5C_PowerGenerator generator = (EN5C_PowerGenerator) target;
		Param1<float> nacdata = acdata;
		int consumed_fuel = generator.AddFuel( nacdata.param1 );
	}
};