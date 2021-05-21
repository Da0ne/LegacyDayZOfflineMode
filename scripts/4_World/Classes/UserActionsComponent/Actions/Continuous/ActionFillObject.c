class ActionFillObjectCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 8;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionFillObject: ActionContinuousBase
{
	private const float MAXIMAL_TARGET_DISTANCE = 2;
	
	void ActionFillObject()
	{
		m_CallbackClass = ActionFillObjectCB;
		m_MessageSuccess = "I've filled the object.";
		m_MessageStartFail = "";
		m_MessageStart = "";
		m_MessageSuccess = "";
		m_MessageFail = "";
		m_MessageCancel = "";
		//m_Animation = "DIGGINGSHOVEL";
	}
	
	void CreateConditionComponents() 
	{	
		
		m_ConditionTarget = new CCTNonRuined(MAXIMAL_TARGET_DISTANCE);
		m_ConditionItem = new CCINonRuined;
	}

	int GetType()
	{
		return AT_FILL_OBJECT;
	}
		
	string GetText()
	{
		return "Fill the object";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		if ( target != NULL && target.IsInherited(EN5C_HescoBox) && item.GetDamage() < 1 )
		{
			if ( target.GetDamage() < 1 )
			{
				EN5C_HescoBox hesco = (EN5C_HescoBox)target;
				
				if ( hesco.GetState() == EN5C_HescoBox.UNFOLDED || hesco.GetState() == EN5C_HescoBox.FILLED )
				{
					return true;
				}
			}
			else
			{
				SendMessageToClient( player, "Hesco Box is ruined." );
			}
		}
		
		return false;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		if ( target != NULL && target.IsInherited(EN5C_HescoBox) )
		{
			item.AddDamage( 0.05 );
			
			EN5C_HescoBox hesco = (EN5C_HescoBox)target;
			
			if ( hesco.GetState() == EN5C_HescoBox.UNFOLDED )
			{
				hesco.Fill();
			}
			else if ( hesco.GetState() == EN5C_HescoBox.FILLED )
			{
				hesco.Unfold();
			}
		}
	}
};