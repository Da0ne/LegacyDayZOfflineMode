class ActionRemovePlant: ActionInteractBase
{
	void ActionRemovePlant()
	{
		m_MessageSuccess = "";
		m_MessageStartFail = "";
		m_MessageStart = "";
		m_MessageSuccess = "";
		m_MessageFail = "";
		m_MessageCancel = "";
	}

	int GetType()
	{
		return AT_REMOVE_PLANT;
	}

	string GetText()
	{
		return "remove plant";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		if ( target != NULL && target.IsInherited(EN5C_PlantBase) )
		{
			EN5C_PlantBase plant = (EN5C_PlantBase)target;
			
			if ( plant.IsGrowing()  ||  plant.IsDry()  ||  !plant.HasCrops() )
			{
				return true;
			}
		}
		
		return false;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		if ( target != NULL && target.IsInherited(EN5C_PlantBase) )
		{
			EN5C_PlantBase plant = (EN5C_PlantBase)target;
			m_MessageSuccess = plant.Remove( player );
		}
	}
};