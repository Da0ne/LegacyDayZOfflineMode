
// *************************************************************************************
// ! DayZPlayerCamera3rdPerson - base for 3rd person cameras
// *************************************************************************************
class DayZPlayerCamera3rdPerson extends DayZPlayerCameraBase
{
	static const float 	CONST_UD_MIN	= -85.0;		//!< down limit
	static const float 	CONST_UD_MAX	= 85.0;			//!< up limit

	static const float 	CONST_LR_MIN	= -160.0;		//!< down limit
	static const float 	CONST_LR_MAX	= 160.0;			//!< up limit


	
	void 	DayZPlayerCamera3rdPerson(DayZPlayer pPlayer, HumanInputController pInput)
	{
		if (pInput.Camera3rdIsRightShoulder())
		{
			m_fCameraLRShoulder		= 1.0;
		}
		else
		{
			m_fCameraLRShoulder		= -1.0;
		}

		m_fCameraLRShoulderVel[0]	= 0.0;
		m_fRoll						= 0.0;
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
		m_fLeftRightAngle	= UpdateLRAngle(m_fLeftRightAngle, CONST_LR_MIN, CONST_LR_MAX, pDt);

		// update l/r offsets and set it as 
		if (m_pInput.Camera3rdIsRightShoulder())
		{
			m_fCameraLRShoulder = Math.SmoothCD(m_fCameraLRShoulder, 1.0, m_fCameraLRShoulderVel, 0.14, 1000, pDt);
		}
		else
		{
			m_fCameraLRShoulder = Math.SmoothCD(m_fCameraLRShoulder, -1.0, m_fCameraLRShoulderVel, 0.14, 1000, pDt);
		}

		float 	shoulderDist = m_fCameraLRShoulder * m_fShoulderWidth;
		
		//! 
		vector rot;
		rot[0] = m_fLeftRightAngle;
		rot[1] = m_fUpDownAngle;
		rot[2] = m_fRoll;
		
		Math3D.YawPitchRollMatrix(rot, pOutResult.m_CameraTM);


		//! base bone pos 
		if (m_iBoneIndex != -1)
		{
			pOutResult.m_CameraTM[3]	= m_pPlayer.GetBonePositionMS(m_iBoneIndex);
		}
		else
		{
			pOutResult.m_CameraTM[3]	= "0 0 0";
		}

		//! apply shoulder dist
		vector msOffset = m_CameraOffsetMS;
		msOffset[0] = msOffset[0] + shoulderDist;

		// ls offset + ms offset + shoulder width			
		pOutResult.m_CameraTM[3]	= pOutResult.m_CameraTM[3] + pOutResult.m_CameraTM[0] * m_CameraOffsetLS[0] + pOutResult.m_CameraTM[1] * m_CameraOffsetLS[1] + pOutResult.m_CameraTM[2] * m_CameraOffsetLS[2] + msOffset;

		//! store distance 
		pOutResult.m_fDistance 		= m_fDistance;
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


	//! runtime config
	protected	int 	m_iBoneIndex = -1;	//!< main bone 
	protected 	vector  m_CameraOffsetMS;	//!< model space offset
	protected 	vector  m_CameraOffsetLS;	//!< local space offset
	protected 	float 	m_fDistance;		//!< distance from start
	protected 	float 	m_fShoulderWidth;	//!< shoulder camera widths
	protected 	float 	m_fRoll;			//!< camera roll


	//! runtime values 
	protected 	float 	m_fUpDownAngle;		//!< up down angle in rad
	protected 	float 	m_fLeftRightAngle;	//!< left right angle in rad (in freelook only)

	//! shoulder offsets
	protected float 	m_fCameraLRShoulder;			// -1..1 shoulderness :)
	protected float 	m_fCameraLRShoulderVel[1];		// 0
}




// *************************************************************************************
// ! DayZPlayerCamera3rdPersonErc - 3rd person erected
// *************************************************************************************
class DayZPlayerCamera3rdPersonErc extends DayZPlayerCamera3rdPerson
{
	void DayZPlayerCamera3rdPersonErc(DayZPlayer pPlayer, HumanInputController pInput)
	{
		m_fDistance 		= 2.2;
		m_CameraOffsetMS	= "0.0 1.25 0.1";
		m_CameraOffsetLS	= "0.0 0.3 0.0";
		m_fShoulderWidth	= 0.4;
	}
}


// *************************************************************************************
// ! DayZPlayerCamera3rdPersonErc - 3rd person erected sprint
// *************************************************************************************
class DayZPlayerCamera3rdPersonErcSpr extends DayZPlayerCamera3rdPersonErc
{
	void DayZPlayerCamera3rdPersonErcSpr(DayZPlayer pPlayer, HumanInputController pInput)
	{
		m_fDistance 		= 1.55;
		m_CameraOffsetMS    = "0.0 1.2 0.2";
		m_CameraOffsetLS	= "0.0 0.3 0.0";

		m_iPelvisBone	= pPlayer.GetBoneIndexByName("Pelvis");
		if (m_iPelvisBone == -1)
		{
			Print("DayZPlayerCamera3rdPersonErcSpr: hips bone not found");
		}

		m_fTime = 0.0;
	}



	//	
	void 		OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
	{
		m_fTime += pDt * 5;
		
		vector 	tm[4];

		m_pPlayer.GetBoneTransformLS(m_iPelvisBone, tm);

		//! basically -> transform up vector (0,1,0) and read x coord -> and set is as roll
		float 	xShift = tm[1][0];
		m_fRoll = xShift * 3.0;	// 3 is just made up value i like :)

		// m_fRoll = (sin(1.5 * m_fTime)); //  + 0.3 * sin(3.6 * m_fTime) + 0.2 * sin(0.7 * m_fTime)) / 1.3 * 3;

		//! prev update 
		super.OnUpdate(pDt, pOutResult);

	}
	

	protected int 	m_iPelvisBone;
	protected float m_fTime;	

}


// *************************************************************************************
// ! DayZPlayerCamera3rdPersonErcRaised - 3rd person erected raised
// *************************************************************************************
class DayZPlayerCamera3rdPersonErcRaised extends DayZPlayerCamera3rdPersonErc
{
	void DayZPlayerCamera3rdPersonErcRaised(DayZPlayer pPlayer, HumanInputController pInput)
	{
		if ( pPlayer.GetCurrentPerItemCameraUD () == DayZPlayerCameras.PERITEMUD_EMPTYHANDED )
		{
			// no item in hands 
			m_fDistance 		= 2.0;
			m_fShoulderWidth	= 0.4;
			m_CameraOffsetMS    = "0.0 1.25 0.2";
			m_CameraOffsetLS	= "0.0 0.3 0.0";
			
		}		
				else if ( pPlayer.GetCurrentPerItemCameraUD () == DayZPlayerCameras.PERITEMUD_TWOHANDED )
				{
					// two handed
					m_fDistance 		= 1.8;
					m_fShoulderWidth	= 0.2;
			
				}
		
				else if ( pPlayer.GetCurrentPerItemCameraUD () == DayZPlayerCameras.PERITEMUD_ONEHANDED )
				{
					// one handed
					m_fDistance 		= 2.0;
					m_fShoulderWidth	= 0.4;
					m_CameraOffsetMS    = "0.0 1.25 0.2";
					m_CameraOffsetLS	= "0.0 0.3 0.0";
			
				}
		
				
		else
		{
			// weapons in hands 
			m_fDistance 		= 1.6;
			m_CameraOffsetMS    = "0.0 1.25 0.2";
			m_CameraOffsetLS	= "0.0 0.3 0.0";
			m_fShoulderWidth	= 0.6;
		}
		
	}
}


// *************************************************************************************
// ! DayZPlayerCamera3rdPersonCrouch - 3rd person crouch
// *************************************************************************************
class DayZPlayerCamera3rdPersonCrouch extends DayZPlayerCamera3rdPerson
{
	static const float 	CONST_CAMERAMOVEMENTHEIGH = 0.4;	// 0 idle, 0.15 walk 0.3 run/sprint 

	void DayZPlayerCamera3rdPersonCrouch (DayZPlayer pPlayer, HumanInputController pInput)
	{
		m_fDistance 		= 1.6;
		m_CameraOffsetMS	= "0.0 0.55 0.1";
		m_CameraOffsetLS	= "0.0 0.3 0.0";
		m_fShoulderWidth	= 0.4;
	}

	//	
	void 		OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
	{
		autoptr HumanMovementState		state 	= new HumanMovementState();		
		m_pPlayer.GetMovementState(state);

		//! movement height 
		float 	movement 	= (Limit(state.m_iMovement, 1, 3) - 1.0) * 0.5;
		movement 			*= CONST_CAMERAMOVEMENTHEIGH;
		m_fCameraHeight		= Math.SmoothCD(m_fCameraHeight, movement, m_fCameraHeightVel, 0.2, 1000, pDt);
		m_CameraOffsetMS[1] = 0.7 + m_fCameraHeight;

		//! prev update 
		super.OnUpdate(pDt, pOutResult);
	}


	protected float 	m_fCameraHeight;			// -1..1 shoulderness :)
	protected float 	m_fCameraHeightVel[1];		// 0
}


// *************************************************************************************
// ! DayZPlayerCamera3rdPersonCrouchRaised- 3rd person crouch raised
// *************************************************************************************
class DayZPlayerCamera3rdPersonCrouchRaised extends DayZPlayerCamera3rdPersonCrouch
{
	void DayZPlayerCamera3rdPersonCrouchRaised(DayZPlayer pPlayer, HumanInputController pInput)
	{
		m_fDistance 		= 1.2;
		m_CameraOffsetMS	= "0.0 0.75 0.1";
	}
}


// *************************************************************************************
// ! DayZPlayerCamera3rdPersonProne - 3rd person prone
// *************************************************************************************
class DayZPlayerCamera3rdPersonProne extends DayZPlayerCamera3rdPerson
{
	void DayZPlayerCamera3rdPersonProne(DayZPlayer pPlayer, HumanInputController pInput)
	{
		m_fDistance 		= 1.0;
		m_CameraOffsetMS	= "0.0 0.2 0.1";
		m_CameraOffsetLS	= "0.0 0.3 0.0";
		m_fShoulderWidth	= 0.5;
	}

}

// *************************************************************************************
// ! DayZPlayerCamera3rdPersonProneRaised - 3rd person prone raised
// *************************************************************************************
class DayZPlayerCamera3rdPersonProneRaised extends DayZPlayerCamera3rdPersonProne
{
	void DayZPlayerCamera3rdPersonProneRaised(DayZPlayer pPlayer, HumanInputController pInput)
	{
		m_fDistance 		= 0.6;
		m_CameraOffsetMS	= "0.0 0.225 0.1";
		m_fShoulderWidth	= 0.5;
	}
}
