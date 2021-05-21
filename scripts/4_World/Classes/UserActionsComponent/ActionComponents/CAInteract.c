class CAInteract : CABase
{	
	int Execute( PlayerBase player, Object target, ItemBase item )
	{
		return UA_FINISHED;
	}
	
	int Interupt( PlayerBase player, Object target, ItemBase item )
	{
		return UA_CANCEL;
	}
};