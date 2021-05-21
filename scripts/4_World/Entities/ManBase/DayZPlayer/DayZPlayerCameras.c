class DayZPlayerCameras
{
	static const int 	DAYZCAMERA_1ST 				= 1;		//!< 1st person camera
	static const int 	DAYZCAMERA_3RD_ERC 			= 2;		//!< 3rd - standing
	static const int 	DAYZCAMERA_3RD_ERC_SPR 		= 3;		//!< 3rd - standing sprint
	static const int 	DAYZCAMERA_3RD_ERC_RAISED 	= 4;		//!< 3rd - standing raised
	static const int 	DAYZCAMERA_3RD_CRO 			= 5;		//!< 3rd - crouch
	static const int 	DAYZCAMERA_3RD_CRO_RAISED 	= 6;		//!< 3rd - crouch
	static const int 	DAYZCAMERA_3RD_PRO 			= 7;		//!< 3rd - laying
	static const int 	DAYZCAMERA_3RD_PRO_RAISED   = 8; 		//!< 3rd - laying raised
	
	static const int 	PERITEMUD_INVALID 			= -1;		//! uninitialized / invalid per item camera user data
	static const int 	PERITEMUD_EMPTYHANDED 		= 20;		//! for empty hands
	static const int 	PERITEMUD_ONEHANDED 		= 25;		//! for empty hands
	static const int 	PERITEMUD_TWOHANDED 		= 30;		//! for two handed items
	


	//! called from DayZPlayerTypeCreate() fn	
	// temporary moved out of the 
	static void 	RegisterCameras(DayZPlayerType pType)
	{
		pType.ResetCameraCreators();

		pType.RegisterCameraCreator(DAYZCAMERA_1ST, DayZPlayerCamera1stPerson);
		pType.RegisterCameraCreator(DAYZCAMERA_3RD_ERC, DayZPlayerCamera3rdPersonErc);
		pType.RegisterCameraCreator(DAYZCAMERA_3RD_ERC_SPR, DayZPlayerCamera3rdPersonErcSpr);
		pType.RegisterCameraCreator(DAYZCAMERA_3RD_ERC_RAISED, DayZPlayerCamera3rdPersonErcRaised);
		pType.RegisterCameraCreator(DAYZCAMERA_3RD_CRO, DayZPlayerCamera3rdPersonCrouch);
		pType.RegisterCameraCreator(DAYZCAMERA_3RD_CRO_RAISED, DayZPlayerCamera3rdPersonCrouchRaised);
		pType.RegisterCameraCreator(DAYZCAMERA_3RD_PRO, DayZPlayerCamera3rdPersonProne);
		pType.RegisterCameraCreator(DAYZCAMERA_3RD_PRO_RAISED, DayZPlayerCamera3rdPersonProneRaised);
	}

}
