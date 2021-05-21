class RockBase: Object 
{
	bool IsRock()
	{
		return true;
	}

	int GetAmountOfDrops(ItemBase item)
	{
		if ( item && item.IsKindOf("EN5C_Pickaxe") )
		{
			return 10;
		}
		else if ( item && item.IsKindOf("EN5C_SledgeHammer") )
		{
			return 5;
		}
		else
		{
			return 100;
		}
	}	

	string GetMaterial(ItemBase item)
	{
		if ( item && item.IsKindOf("EN5C_Pickaxe") )
		{
			return "EN5C_Stone";
		}
		else if ( item && item.IsKindOf("EN5C_SledgeHammer") )
		{
			return "EN5C_Stone";
		}
		else
		{
			return "EN5C_SmallStone"; 
		}
	}

	int GetAmountOfMaterialPerDrop(ItemBase item)
	{
		if ( item && item.IsKindOf("EN5C_Pickaxe") )
		{
			return 1;
		}
		else if ( item && item.IsKindOf("EN5C_SledgeHammer") )
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
		if ( item && item.IsKindOf("EN5C_Pickaxe") )
		{
			return 0.001;
		}
		else if ( item && item.IsKindOf("EN5C_SledgeHammer") )
		{
			return 0.05;
		}
		else
		{
			return 0.1;
		}
	}
};

//jtomasik - it would be blast being able just to inherit it from RockBase, but the way static objects are handled most likely don't allow it? ask p cimo
class Static_r2_boulder1: RockBase
{
};
class Static_r2_boulder2: RockBase
{
};
class Static_small_stone_01_f: RockBase
{
};
class Static_small_stone_02_f: RockBase
{
};
class Static_stone_small_f: RockBase
{
};
class Static_stone_small_w: RockBase
{
};
class Static_bluntstone_01: RockBase
{
};
class Static_bluntstone_01_lc: RockBase
{
};
class Static_bluntstone_02: RockBase
{
};
class Static_bluntstone_02_lc: RockBase
{
};
class Static_bluntstone_03: RockBase
{
};
class Static_SharpStone_01: RockBase
{
};
class Static_sharpstone_01_lc: RockBase
{
};
class Static_sharpstone_02: RockBase
{
};
class Static_sharpstone_02_lc: RockBase
{
};
class Static_sharpstone_03: RockBase
{
};
class Static_sharpstone_03_lc: RockBase
{
};