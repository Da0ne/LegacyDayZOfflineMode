class ActionTurnOnRoadFlare: ActionTurnOnWhileInHands
{
	void ActionTurnOnRoadFlare()
	{
		m_MessageSuccess = "I ignitied roadflare.";
		m_MessageFail = "It's out of energy";
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_LIGHTFLARE;
	}

	int GetType()
	{
		return AT_TURN_ON_ROADFLARE;
	}

	string GetText()
	{
		return "ignite";
	}

	void OnStart( PlayerBase player, Object target, ItemBase item )
	{
		if ( item != NULL && item.IsInherited(EN5C_Roadflare) )
		{
			GetGame().CreateSoundOnObject(item, "roadflareTurnOn", 50, false);
		}
	}
};