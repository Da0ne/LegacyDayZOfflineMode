//! limit function
float 	Limit(float pV, float pMin, float pMax)
{
	if (pV >= pMin && pV <= pMax)
	{
		return pV;
	}
	else if (pV < pMin)
	{
		return pMin;
	}
	else
	{
		return pMax;
	}	
}



class DayZPlayerCameraBase extends DayZPlayerCamera
{
	//! constructor must be same 
	void 	DayZPlayerCameraBase(DayZPlayer pPlayer, HumanInputController pInput)
	{
		m_fLRAngleVel[0] = 0;

		//!
		{
			int perItemCamUD = pPlayer.GetCurrentPerItemCameraUD();
			string 		a	= "Per Item Camera User Data: " + perItemCamUD.ToString();
			Print(a);
		}
	};


	float UpdateUDAngle(float pAngle, float pMin, float pMax)
	{
		//! update it in degrees
		pAngle 	+= m_pInput.GetAimChange()[1] * Math.RAD2DEG;
		pAngle 	= Limit(pAngle, pMin, pMax);

		return pAngle;
	}


	float UpdateLRAngle(float pAngle, float pMin, float pMax, float pDt)
	{
		//! lr angle
		if (m_pInput.CameraIsFreeLook())	
		{
			pAngle	+= m_pInput.GetAimChange()[0] * Math.RAD2DEG;
			pAngle	= Limit(pAngle, pMin, pMax);

			m_fLRAngleVel[0]	= 0;	// reset filter

		}
		else
		{
			// smooth value back to 0 
			pAngle		= Math.SmoothCD(pAngle, 0, m_fLRAngleVel, 0.14, 1000, pDt);
			// m_fLeftRightAngle	= 0.9 * (1.0 - pDt);			
		}

		return pAngle;
	}


	protected 	float 	m_fLRAngleVel[1];

}
