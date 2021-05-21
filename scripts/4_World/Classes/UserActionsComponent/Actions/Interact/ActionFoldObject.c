class ActionFoldObject: ActionInteractBase
{
	void ActionFoldObject()
	{
		m_MessageSuccess = "";
		m_MessageStartFail = "Hesco Box is ruined.";
		m_MessageStart = "Hesco Box is ruined.";
		m_MessageSuccess = "";
		m_MessageFail = "";
		m_MessageCancel = "";
		//m_Animation = "UNPINGRENADE";
	}

	int GetType()
	{
		return AT_FOLD_OBJECT;
	}

	string GetText()
	{
		return "Fold the object";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		if ( target.IsInherited(EN5C_HescoBox) )
		{
			EN5C_HescoBox hesco = (EN5C_HescoBox)target;
			
			if ( hesco.GetState() == EN5C_HescoBox.UNFOLDED )
			{
				return true;
			}
		}
		return false;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		EN5C_HescoBox hesco = (EN5C_HescoBox)target;
		if ( hesco.GetState() == EN5C_HescoBox.UNFOLDED )
		{
			hesco.Fold();
		}
	}
};