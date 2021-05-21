class ActionEmptyMagazineCB : ActionContinuousBaseCB
{
	private const float TIME_TO_EMPTY_ONE_BULLET = 1;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousEmptyMagazine(TIME_TO_EMPTY_ONE_BULLET);
	}
};

class ActionEmptyMagazine: ActionContinuousBase
{
	void ActionEmptyMagazine()
	{
		m_CallbackClass = ActionEmptyMagazineCB;
		m_MessageStartFail = "It's empty.";
		m_MessageStart = "I have started emptying magazine.";
		m_MessageSuccess = "I have finished emptying the magazine.";
		m_MessageFail = "It broke.";
		m_MessageCancel = "I stopped emptying the magazine.";
		//m_Animation = "EATTABLETS";		
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}
	
	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{	
		if ( item )
		{
			Magazine itm = (Magazine)item;
			if ( itm.GetAmmoCount() > 0 ) 
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	
	int GetType()
	{
		return AT_EMPTY_MAGAZINE;
	}
		
	string GetText()
	{
		return "empty magazine";
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		autoptr TStringArray ammo = new TStringArray;
		g_Game.ConfigGetTextArray("cfgMagazines " + item.GetType() + " ammoItems", ammo);
		if ( ammo.Count() > 0 ) 
		{
			if ( acdata )
			{ 
				Param1<float> bullet_count = acdata;
				MiscGameplayFunctions.CreateMagazinePiles(ammo.Get(0),player.GetPosition(),bullet_count.param1,item.GetDamage());
			}
		}
	}
};