class TreeHard_t_quercus2f: TreeHard 
{
	int GetAmountOfDrops(ItemBase item)
	{
		if ( item && item.KindOf("Knife") )
		{
			return 10;
		}
		else if ( item && item.KindOf("Axe") )
		{
			return 3;
		}
		else
		{
			return 100;
		}
	}	

	string GetMaterial(ItemBase item)
	{
		if ( item && item.KindOf("Knife") )
		{
			return "EN5C_Bark_Oak";
		}
		else if ( item && item.KindOf("Axe") )
		{
			return "EN5C_FireWood";
		}
		else
		{
			return "EN5C_WoodenStick"; 
		}
	}

	int GetAmountOfMaterialPerDrop(ItemBase item)
	{
		if ( item && item.KindOf("Knife") )
		{
			return 1;
		}
		else if ( item && item.KindOf("Axe") )
		{
			return 1;
		}
		else
		{
			return 1;
		}
	}

	float GetDamageToMiningItemEachDrop(ItemBase item)
	{
		if ( item && item.KindOf("Knife") )
		{
			return 0.1;
		}
		else if ( item && item.KindOf("Axe") )
		{
			return 0.05;
		}
		else
		{
			return 0;
		}
	}
};

class TreeHard_t_quercus3s: TreeHard_t_quercus2f 
{
};

class TreeHard_t_betula2f: TreeHard_t_quercus3s 
{
	string GetMaterial(ItemBase item)
	{
		if ( item && item.KindOf("Knife") )
		{
			return "EN5C_Bark_Birch";
		}
		else if ( item && item.KindOf("Axe") )
		{
			return "EN5C_FireWood";
		}
		else
		{
			return "EN5C_WoodenStick"; 
		}
	}
};

class TreeHard_t_betula2s: TreeHard_t_betula2f
{
}; 

class TreeHard_t_betula2w: TreeHard_t_betula2f
{
}; 

class TreeSoft_t_malus1s: TreeSoft
{
	int GetAmountOfDrops(ItemBase item)
	{
		if ( item && item.KindOf("Knife") )
		{
			return 10;
		}
		else if ( item && item.KindOf("Axe") )
		{
			return 3;
		}
		else
		{
			return 20;
		}
	}	

	string GetMaterial(ItemBase item)
	{
		if ( item && item.KindOf("Knife") )
		{
			return "EN5C_WoodenStick";
		}
		else if ( item && item.KindOf("Axe") )
		{
			return "EN5C_FireWood";
		}
		else
		{
			return "EN5C_Apple"; 
		}
	}

	int GetAmountOfMaterialPerDrop(ItemBase item)
	{
		if ( item && item.KindOf("Knife") )
		{
			return 1;
		}
		else if ( item && item.KindOf("Axe") )
		{
			return 1;
		}
		else
		{
			return 1;
		}
	}

	float GetDamageToMiningItemEachDrop(ItemBase item)
	{
		if ( item && item.KindOf("Knife") )
		{
			return 0.1;
		}
		else if ( item && item.KindOf("Axe") )
		{
			return 0.05;
		}
		else
		{
			return 0;
		}
	}
};