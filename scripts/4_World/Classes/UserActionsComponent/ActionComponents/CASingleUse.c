class CASingleUse : CABase
{	
	int Execute( PlayerBase player, Object target, ItemBase item )
	{
		if ( !item )
		{
			return UA_ERROR;
		}
		if ( item.GetDamage() == 1 )
		{
			return UA_FAILED;
		}
		return UA_FINISHED;
	}
	
	int Interupt( PlayerBase player, Object target, ItemBase item )
	{
		return UA_CANCEL;
	}
};