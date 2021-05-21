class EN5C_Compass : ItemCompass
{
	void EN5C_Compass()
	{
		m_Opened = false;
	}

	void Open()
	{
		m_Opened = true;
		GetGame().ObjectSetAnimationPhase(this,"cover",1);
		//super.Open();
	}

	void Close()
	{
		m_Opened = false;
		GetGame().ObjectSetAnimationPhase(this,"cover",0);
		//super.Close();
	}
}
