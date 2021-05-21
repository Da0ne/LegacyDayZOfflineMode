
//! this is main camera class 
class DayZPlayerCamera1stPerson extends DayZPlayerCameraBase
{
	static const float 	CONST_UD_MIN	= -85.0;		//!< down limit
	static const float 	CONST_UD_MAX	= 85.0;			//!< up limit

	static const float 	CONST_LR_MIN	= -160.0;		//!< down limit
	static const float 	CONST_LR_MAX	= 160.0;			//!< up limit


	void 	DayZPlayerCamera1stPerson(DayZPlayer pPlayer, HumanInputController pInput)
	{
		m_iBoneIndex = pPlayer.GetBoneIndexByName("Head");
		if (m_iBoneIndex == -1)
		{
			Print("DayZPlayerCamera1stPerson: main bone not found");
		}

		Print("DayZPlayerCamera1stPerson");
		Print(m_iBoneIndex);
	}
	

	//
	void 		OnActivate (DayZPlayerCamera pPrevCamera, DayZPlayerCameraResult pPrevCameraResult)
	{
		if (pPrevCamera)
		{
			vector 	f = pPrevCamera.GetBaseAngles();
			m_fUpDownAngle	= f[0]; 
		}
	}

	//	
	void 		OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
	{
		//! update angles from input 
		m_fUpDownAngle 		= UpdateUDAngle(m_fUpDownAngle, CONST_UD_MIN, CONST_UD_MAX);
		m_fLeftRightAngle 	= UpdateLRAngle(m_fLeftRightAngle, CONST_LR_MIN, CONST_LR_MAX, pDt);

		
		//! 
		vector rot;
		rot[0] = m_fLeftRightAngle;
		rot[1] = m_fUpDownAngle;
		rot[2] = 0;
		
		Math3D.YawPitchRollMatrix(rot, pOutResult.m_CameraTM);

		if (m_iBoneIndex != -1)
		{
			//! 
			pOutResult.m_CameraTM[3]	= m_pPlayer.GetBonePositionMS(m_iBoneIndex);
	
		}

		else
		{
			pOutResult.m_CameraTM[3]	= "0 0 0";
		}
	}

	//
	vector GetBaseAngles()
	{
		vector a;
		a[0] = m_fUpDownAngle;
		a[1] = m_fLeftRightAngle;
		a[2] = 0;
		return a;
	}


	protected	int 	m_iBoneIndex;		//!< main bone 

	protected 	float 	m_fUpDownAngle;		//!< up down angle in rad
	protected 	float 	m_fLeftRightAngle;	//!< left right angle in rad (in freelook only)
}


