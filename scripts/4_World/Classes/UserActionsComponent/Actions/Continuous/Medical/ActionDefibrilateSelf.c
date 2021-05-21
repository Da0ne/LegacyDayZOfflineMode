class ActionDefibrilateSelfCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 15;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionDefibrilateSelf: ActionContinuousBase
{
	float m_EnergyUsage;
	
	void ActionDefibrilateSelf()
	{
		m_CallbackClass = ActionDefibrilateSelfCB;
		m_MessageStartFail = "It's out of juice.";
		m_MessageStart = "I have started defibrilating myself";
		m_MessageSuccess = "I have defibrilated myself.";
		m_MessageFail = "I have moved and defibrilating was canceled.";
		m_MessageCancel = "I stopped defibrilating.";
		m_EnergyUsage = 5000; //wats
		//m_Animation = "defibrilate";
	}
	
	void CreateConditionComponents() 
	{	
		
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTSelf;
	}

	int GetType()
	{
		return AT_DEFIBRILATE_S;
	}
		
	string GetText()
	{
		return "defibrilate";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{	
		if ( item.HasEnergyManager()  &&  item.GetCompEM().IsSwitchedOn() && item.GetDamage() < 1 ) 
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{	
		//has external energy source
		if ( item.GetCompEM().GetEnergySource() != NULL )
		{
			//has enough energy
			if ( item.GetCompEM().GetEnergySource().GetCompEM().GetEnergy() >= m_EnergyUsage ) // TODO: create function CanWork in elect comp
			{
				//spend energy
				item.GetCompEM().AddEnergy ( -m_EnergyUsage );
			
				player.m_ModifiersManager.DeactivateModifier ( MDF_HEART_ATTACK );
			}
		} 
		else
		{
			player.MessageImportant ( m_MessageStartFail );		
		}
	}
};
