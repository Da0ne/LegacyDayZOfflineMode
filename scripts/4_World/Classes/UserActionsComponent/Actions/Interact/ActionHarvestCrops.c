class ActionHarvestCrops: ActionInteractBase
{
	void ActionHarvestCrops()
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
		return AT_HARVEST_CROPS;
	}

	string GetText()
	{
		return "harvest crops";
	}

	bool ActionCondition( PlayerBase player, Object target, ItemBase item )
	{
		if ( target.IsInherited(EN5C_PlantBase) )
		{
			EN5C_PlantBase plant = (EN5C_PlantBase)target;
			
			if (  ( plant.IsMature()  ||  plant.IsSpoiled() )  &&  plant.HasCrops()  )
			{
				return true;
			}
		}
		
		return false;
	}

	void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata )
	{
		EN5C_PlantBase plant = (EN5C_PlantBase)target;
		m_MessageSuccess = plant.Harvest( player );
		player.IncreaseSkill( PluginExperience.EXP_FARMER_HARVESTING, 3 );
	}
};