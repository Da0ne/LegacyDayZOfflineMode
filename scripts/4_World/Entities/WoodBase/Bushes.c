class BushHard_b_canina2s: BushHard 
{
	int GetAmountOfDrops(ItemBase item)
	{
		if ( item && item.KindOf("Knife") )
		{
			return 3;
		}
		else if ( item && item.KindOf("Axe") )
		{
			return 1;
		}
		else
		{
			return 10;
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
			return "EN5C_LongWoodenStick";
		}
		else
		{
			return "EN5C_CaninaBerry"; 
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

class BushHard_b_sambucus: BushHard 
{
	int GetAmountOfDrops(ItemBase item)
	{
		if ( item && item.KindOf("Knife") )
		{
			return 3;
		}
		else if ( item && item.KindOf("Axe") )
		{
			return 1;
		}
		else
		{
			return 10;
		}
	}	

	string GetMaterial(ItemBase item)
	{
		Print("getmaterial: Sambucus");
		if ( item && item.KindOf("Knife") )
		{
			return "EN5C_WoodenStick";
		}
		else if ( item && item.KindOf("Axe") )
		{
			return "EN5C_LongWoodenStick";
		}
		else
		{
			return "EN5C_SambucusBerry"; 
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