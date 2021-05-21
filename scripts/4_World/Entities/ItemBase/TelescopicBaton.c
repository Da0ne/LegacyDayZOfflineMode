class EN5C_TelescopicBaton : ItemBase
{
	void EN5C_TelescopicBaton()
	{
		m_Opened = false;
	}

	void Open()
	{
		m_Opened = true;
		GetGame().ObjectSetAnimationPhase(this,"Doors1",0);
		//super.Open();
	}

	void Close()
	{
		m_Opened = false;
		GetGame().ObjectSetAnimationPhase(this,"Doors1",1);
		//super.Close();
	}
}
