class ActionDigTileCB : ActionContinuousBaseCB
{
	private const float TIME_TO_COMPLETE = 4;
	
	void CreateActionComponent()
	{
		m_ActionComponent = new CAContinuousTime(TIME_TO_COMPLETE);
	}
};

class ActionDigTile: ActionContinuousBase
{	
	void ActionDigTile()
	{
		m_CallbackClass = ActionDigTileCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_DIGSHOVEL;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
		/*
		m_MessageStart = "Player started placing object.";
		m_MessageSuccess = "Player placed the object."
		m_MessageFail = "Player failed to place the object.";
		m_MessageCancel = "You canceled action.";
		m_MessageStartFail = "Failed to start action: Object collides with environment.";
		*/
		m_MessageFail = "Digging tile failed because tile is colliding with environment.";
	}
	
	void CreateConditionComponents() 
	{	
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	bool ActionCondition ( PlayerBase player, Object target, ItemBase item )
	{
		if ( player.IsPlacingObject() )
		{
			//string item_type = item.GetType();
			//GetGame().ConfigGetText( "cfgVehicles " + item_type + " Horticulture ToolAnim", m_Animation );
			
			PluginExperience module_exp = (PluginExperience)GetPlugin(PluginExperience);
			float soil_preparation_efficiency = module_exp.GetExpParamNumber(player, PluginExperience.EXP_FARMER_SOIL_PREPARATION, "efficiency");
			//m_TimeToCompleteAction = GetGame().ConfigGetFloat( "cfgVehicles " + item_type + " Horticulture DiggingTimeToComplete") / soil_preparation_efficiency;
			
			return true;
		}
		
		return false;
	}

	int GetType()
	{
		return AT_DIG_TILE;
	}
		
	string GetText()
	{
		return "Dig tile";
	}

	void OnStart ( PlayerBase player, Object target, ItemBase item )
	{
		//disable hologram position update
		player.GetHologram().SetUpdatePosition( false );
	}


	void OnCancel ( PlayerBase player, Object target, ItemBase item, Param acdata  )
	{
		//enable hologram position update
		if ( player.IsPlacingObject() ) 
		{
			player.GetHologram().SetUpdatePosition ( true );
		}

		return true;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		if ( !player.GetHologram().IsColliding() )
		{
			//place object
			player.PlacingFinish( player.GetHologram().GetProjectionObject() );
					
			//manage skill
			player.IncreaseSkill( PluginExperience.EXP_FARMER_SOIL_PREPARATION, 1 );
		}
		else
		{
			//enable hologram position update
			player.GetHologram().SetUpdatePosition ( true );
			
			//message player
			InformPlayers(player,target,UA_FAILED);
		}
	}
};
