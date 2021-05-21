class CCINonRuined : CCIBase
{
	bool Can( PlayerBase player, ItemBase item )
	{
		if ( item && item.GetDamage() < 1 ) return true;
		return false;
	}
};