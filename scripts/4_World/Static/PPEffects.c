class PPEffects
{
	//-------------------------------------------------------
	// BLUR START
	//-------------------------------------------------------
	
	static float m_BlurInventory;
	static float m_BlurDrunk;
	
	//-------------------------------------------------------
	static void SetBlur(float value)
	{
		Material mat_blur = GetGame().GetWorld().GetMaterial("graphics/materials/postprocess/gauss");
		
		if (mat_blur)
		{
			mat_blur.SetParam("Intensity", value);
		}
	}
	//-------------------------------------------------------
	static void UpdateBlur()
	{
		SetBlur( m_BlurInventory + m_BlurDrunk );//add here new blur effects
	}
	//-------------------------------------------------------
	static void SetBlurInventory(float value)
	{
		m_BlurInventory = value;
		UpdateBlur();
	}
	//-------------------------------------------------------
	static void SetBlurDrunk(float value)
	{
		m_BlurDrunk = value;
		UpdateBlur();
	}
	//-------------------------------------------------------
	// BLUR END
	//-------------------------------------------------------
	
};