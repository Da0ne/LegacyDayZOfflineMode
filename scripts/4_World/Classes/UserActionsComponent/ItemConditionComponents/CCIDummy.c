class CCIDummy : CCIBase
{
	bool Can( PlayerBase player, ItemBase item )
	{
		if( item ) return true;
		return false;
	}
};