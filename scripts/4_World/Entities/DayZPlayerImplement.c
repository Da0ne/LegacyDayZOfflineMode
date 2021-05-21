/* 
DayZPlayerImplement

this file is implemenation of dayzPlayer in script 
- logic of movement 
- camera switching logic

*/

class DayZPlayerImplement extends DayZPlayer
{
	// Timer 	m_UpdateTick;

	protected autoptr HumanMovementState 	m_MovementState = new HumanMovementState();	//!< cached ptr for movement state 

	
	//! constructor 
	void 	DayZPlayerImplement()
	{
		//Print("DayZPlayerImplement: Init");
	}	


	//! destructor 
	void 	~DayZPlayerImplement()
	{
	}

	
	//-------------------------------------------------------------
	//!
	//! This is main command logic
	//! 
	
	//! 
	void  CommandHandler(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)	
	{
		//! handle finished commands
		if (pCurrentCommandFinished)
		{
			if (pCurrentCommandID == DayZPlayerConstants.COMMANDID_ACTION)
			{
				// start moving
				StartCommand_Move();
				return;
			}
		}		
		
		// string a = "In Command Handler, delta time: " + ToString(pDt);
		// Print(a);
	}
	
	//-------------------------------------------------------------
	//!
	//! This is main camera selection logic
	//! 
	int 	CameraHandler(int pCamType)
	{
		// Print("DayZPlayerCfgBase: Camera Handler called");

		/* {
			int perItemCamUD = GetCurrentPerItemCameraUD();
			string 		a	= "Per Item Camera User Data: " + ToString(perItemCamUD);
			Print(a);
		} */
		
		if (pCamType == CamInternal)
		{
			return DayZPlayerCameras.DAYZCAMERA_1ST;
		}
		else if (pCamType == CamExternal)
		{
			//! get movement state 
			GetMovementState(m_MovementState);

			if (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_CROUCH)
			{
				return DayZPlayerCameras.DAYZCAMERA_3RD_CRO;
			}	

			else if (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDCROUCH)
			{
				return DayZPlayerCameras.DAYZCAMERA_3RD_CRO_RAISED;
			}
			
			if (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_PRONE)
			{
				return DayZPlayerCameras.DAYZCAMERA_3RD_PRO;		
			}
			else if (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDPRONE)
			{
				return DayZPlayerCameras.DAYZCAMERA_3RD_PRO_RAISED;	
			}
			
			if (m_MovementState.m_iStanceIdx == DayZPlayerConstants.STANCEIDX_RAISEDERECT)
			{
				return DayZPlayerCameras.DAYZCAMERA_3RD_ERC_RAISED;
			} 
						
			else if (m_MovementState.m_iMovement == 3) 
			{
				return DayZPlayerCameras.DAYZCAMERA_3RD_ERC_SPR;	 
			}
			
			return DayZPlayerCameras.DAYZCAMERA_3RD_ERC;

			// 
		}

		return DayZPlayerCameras.DAYZCAMERA_1ST;
	}	
}

