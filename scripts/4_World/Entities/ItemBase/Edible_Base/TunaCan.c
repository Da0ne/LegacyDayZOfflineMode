class EN5C_TunaCan : EN5C_Edible_Base
{
	void EN5C_TunaCan()
	{
		m_Opened = false;
	}
	
	void Open()
	{
		PlayerBase player = this.GetOwnerPlayer();
		m_Opened = true;
		ItemBase can = GetGame().CreateObject("EN5C_TunaCan_Opened", player.GetPosition());
		can.SetDamage( this.GetDamage() );
		player.RemoveItemFromHands(this);
		player.TakeEntityToHands(can);
		this.Delete();
	}
}
