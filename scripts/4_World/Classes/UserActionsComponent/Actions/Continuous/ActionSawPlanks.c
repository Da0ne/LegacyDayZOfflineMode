class ActionSawPlanksCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 3;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionSawPlanks: ActionContinuousBase
{
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	
	void ActionSawPlanks()
	{
		m_CallbackClass = ActionSawPlanksCB;
		m_MessageStartFail = "The tool is ruined.";
		m_MessageStart = "I have started sawing the planks.";
		m_MessageSuccess = "I have sawn 3 planks.";
		m_MessageFail = "I've stopped sawing the planks.";
		m_MessageCancel = "I've stopped sawing the planks.";
		//m_Animation = "startFire";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionTarget = new CCTNonRuined(MAXIMAL_TARGET_DISTANCE);
		m_ConditionItem = new CCINonRuined;
	}

	int GetType()
	{
		return AT_SAW_PLANKS;
	}

	string GetText()
	{
		return "saw planks";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		if ( item )
		{
			string item_type = item.GetType();
			
			switch(item_type)
			{
				case "EN5C_Hacksaw":
					//m_TimeToCompleteAction = 6;
				break;
		  
				case "EN5C_Chainsaw":
					//m_TimeToCompleteAction = 2;
				break;
			}
			
			return true;
		}
		
		return false;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		PileOfWoodenPlanks item_POWP = (PileOfWoodenPlanks) target;
		item_POWP.RemovePlanks(1);
		
		vector pos = player.GetPosition();
		ItemBase planks = GetGame().CreateObject("EN5C_WoodenPlank", pos);
		planks.AddQuantity( 3, true );
		
		item.AddDamage(0.01);
	}
};