class EN5C_BurlapSackCover extends ItemBase
{
	void ~EN5C_BurlapSackCover()
	{
		GetGame().GetWorld().SetAperture(0);
	}
}
