class WoodBase extends Plant
{
	bool IsWoodBase()
	{
		return true;
	}

	int GetAmountOfDrops(ItemBase item)
	{
		if ( IsTree() )
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
		else
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
				return 3;
			}
		}
	}

	string GetMaterial(ItemBase item)
	{
		Print("getmaterial: Treebase");
		if ( IsTree() )
		{
			if ( item && item.KindOf("Knife") )
			{
				return "EN5C_LongWoodenStick";
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
		else
		{
			if ( item && item.KindOf("Knife") )
			{
				return "EN5C_LongWoodenStick";
			}
			else if ( item && item.KindOf("Axe") )
			{
				return "EN5C_LongWoodenStick";
			}
			else
			{
				return "EN5C_WoodenStick";
			}
		}
	}

	int GetAmountOfMaterialPerDrop(ItemBase item)
	{
		if ( IsTree() )
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
		else
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
	}

	float GetDamageToMiningItemEachDrop(ItemBase item)
	{
		if ( IsTree() )
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
		else
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
	}
};