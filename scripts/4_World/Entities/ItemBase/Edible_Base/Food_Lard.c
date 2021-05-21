class Food_Lard extends EN5C_Edible_Base
{
	bool CanEat()
	{
		EntityAI entity_owner = GetOwner();
		
		if ( entity_owner && entity_owner.HasAttachment(this) )
		{
			return false;
		}
		
		return true;
	}
}
