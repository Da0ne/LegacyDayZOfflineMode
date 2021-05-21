/** @file 

  this file is interface DayZPlayer classes 

*/


// *************************************************************************************
// ! DayZPlayerCameraResult - camera result -
// *************************************************************************************
class DayZPlayerCameraResult
{

	vector		m_CameraTM[4];		//!< transformation matrix - pos + orient of the camera
	float 		m_fFov;				//!< fov value - not used now 
	float 		m_fDistance;		//!< camera distance (external cameras only)


	//! cannot be instanced from script (always created from C++)
	private void DayZPlayerCameraResult()
	{
	}

	//! cannot be instanced from script (always created from C++)
	private void ~DayZPlayerCameraResult()
	{
	}

}

// *************************************************************************************
// ! DayZPlayerCamera - main dayz player camera implement 
// *************************************************************************************
class DayZPlayerCamera 
{

	void DayZPlayerCamera(DayZPlayer pPlayer, HumanInputController pInput)
	{
		m_pPlayer		= pPlayer;
		m_pInput 		= pInput;
	}

	//! virtual callback - called when camera is created
	void 		OnActivate (DayZPlayerCamera pPrevCamera, DayZPlayerCameraResult pPrevCameraResult)
	{
	}

	//!	virtual callback - called each frame
	void 		OnUpdate(float pDt, out DayZPlayerCameraResult pOutResult)
	{
	}

	//!	helper to blend between cameras 
	//! ret[0] - up down angle
	//! ret[1] - left right angle
	//! ret[2] - roll
	vector 		GetBaseAngles()
	{
		return	"0 0 0";
	}


	//! data 
	protected 	DayZPlayer 				m_pPlayer;		//!< player camera is attached to
	protected 	HumanInputController	m_pInput;		//!< human input 
}




// *************************************************************************************
// ! DayZPlayerType - DayZPlayer configuration 
// *************************************************************************************

//! class that configures static data for DayZPlayer
//! on each mission start DayZPlayerTypeCreate(DayZPlayerType pType) is called - can call methods

class DayZPlayerType
{
	//!----------------------------------------------------
	// item in hands config

	//! 
	proto native 	void SetDefaultItemInHandsProfile(string pAnimInstanceName, HumanItemBehaviorCfg pBehaviorCfg);


	//! reset profiles 
	proto native 	void ResetItemInHandsProfiles();

	//! adds setup profile for items of class pItemClass
	//! when item is in hands - pAnimInstanceName is used for animation sourcing 
	//! pCanRaise is whether weapon / item can be raised
	//! pRHandIK == false -> rhand ik is always turned off
	//! pLHandIK == false -> lhand ik is always turned off

	//! VKOSTIK: DO NOT FORGET !!! HumanItemBehaviorCfg.m_iPerItemCameraUD - per item camera user data - enables you setup different cameras for different items in hands

	proto native 	int AddItemInHandsProfile(string pItemClass, string pAnimInstanceName, HumanItemBehaviorCfg pBehaviorCfg);


	//!----------------------------------------------------
	// camera creators

	//! resets camera creators
	proto native 	void ResetCameraCreators();

	//! registers camera creator - camera type => camera type (must be descendant of DayZPlayerCamera)
	proto native 	void RegisterCameraCreator(int pCameraID, typename pCameraType);

}


// *************************************************************************************
// ! DayZPlayerConstants - dayz player constants 
// *************************************************************************************
//! defined in C++
enum DayZPlayerConstants
{
    //! ---------------------------------------------------------
    //! ---------------------- movement stances -----------------
    //! ---------------------------------------------------------

	STANCEIDX_ERECT,
	STANCEIDX_CROUCH,
	STANCEIDX_PRONE,
	STANCEIDX_RAISEDERECT,
	STANCEIDX_RAISEDCROUCH,
	STANCEIDX_RAISEDPRONE,


	//! stance masks 
	STANCEMASK_ERECT,
	STANCEMASK_CROUCH,
	STANCEMASK_PRONE,
	STANCEMASK_RAISEDERECT,
	STANCEMASK_RAISEDCROUCH,
	STANCEMASK_RAISEDPRONE,
	STANCEMASK_ALL,		// STANCEMASK_ERECT | STANCEMASK_CROUCH | STANCEMASK_PRONE | STANCEMASK_RAISEDERECT | STANCEMASK_RAISEDCROUCH | STANCEMASK_RAISEDPRONE
	STANCEMASK_NOTRAISED 	= STANCEMASK_ERECT | STANCEMASK_CROUCH | STANCEMASK_PRONE;
	STANCEMASK_RAISED 		= STANCEMASK_RAISEDERECT | STANCEMASK_RAISEDCROUCH | STANCEMASK_RAISEDPRONE;


	//! movements
	MOVEMENT_IDLE,
	MOVEMENT_WALK,
	MOVEMENT_RUN,
	MOVEMENT_SPRINT,
	MOVEMENT_RHANDIK,
	MOVEMENT_LHANDIK,



    //! ---------------------------------------------------------
    //! ---------------------- COMMAND IDs ----------------------
    //! ---------------------------------------------------------
	
	//! command ids - main movement commands 
	COMMANDID_NONE,    	// type is int - no command - invalid state 
	COMMANDID_MOVE,     // type is int (overridden from C++) - normal movement (idle, walk, run, sprint, ... )
	COMMANDID_ACTION,	// type is int (overridden from C++) - full body action


	//! modifier commands - additive behaviour 
	COMMANDID_MOD_LOOKAT,    	// look at - always on 
	COMMANDID_MOD_WEAPONS,   	// weapons - always on 
    COMMANDID_MOD_ACTION,		// action - additive action 




    //! ---------------------------------------------------------
    //! ---------------------- ACTIONS --------------------------
    //! ---------------------------------------------------------

	//! animations constants 

	//! note: (erc - standing, cro - crouching, pne - prone)    	anim pose it can start from
	//! note: (end, end2, action, loop2)							internal command which can be send in loop state 

	//! looping 	
	/* CMD_ACTION_DRINK = 0,		// erc,cro,pne 	[end, end2]
	CMD_ACTION_EAT = 1,         // erc,cro,pne  [end, end2]
	CMD_ACTION_DRINKPOND = 2,	// cro 	      	[end]
	CMD_ACTION_DRINKWELL = 3,   // cro 	      	[end]
	CMD_ACTION_EMPTYCANISTER = 4, 	// erc, cro [end]
	CMD_ACTION_DRINKCANISTER = 5, 	// erc, cro [end]
	CMD_ACTION_FILLBOTTLEWELL = 6,   // cro		[end]
	CMD_ACTION_CATCHRAIN = 7,   // erc,cro,pne  [end]
	CMD_ACTION_FIREESTINGUISHER = 8,   // erc   [end]
	CMD_ACTION_WRING = 9,   		// cro 		[end]
	CMD_ACTION_BERRIES = 10,   // erc,cro		[end]
	CMD_ACTION_CHOPTREE= 11,   // erc			[end]
	CMD_ACTION_DIGSHOVEL= 12,   // erc			[end]
	CMD_ACTION_DIGHOE = 13,   // erc			[end]
	CMD_ACTION_FISHING= 14,   // cro			[action, end(catch fish), end2(not catching anything) ]
	CMD_ACTION_CPR = 15,   // cro     			[loop(actually pumping), end]
	CMD_ACTION_BANDAGE = 16,   // cro4			[end]
	CMD_ACTION_EATPILLS = 17,   // erc,cro,pne	[end]
	CMD_ACTION_EATTABLETS = 18,   // erc,cro,pne[end]
	CMD_ACTION_MORPHINE  = 19,   // cro			[end]
	CMD_ACTION_INJECTION  = 20,   // cro        [end]
	CMD_ACTION_CRAFTING = 21,   // cro			[end]
	CMD_ACTION_EMPTYBOTTLE = 22,   // erc,cro	[!!!!!!!!!!!!!]
	CMD_ACTION_INTERRACTPLAYER = 23,   // cro 	[end]
	CMD_ACTION_OPENCOMPASS = 24,   // erc       [end]
	
	// onetime 
	CMD_ACTION_LICKBATTERY = 100,   // erc,cro,pne
	CMD_ACTION_PICKUP = 101,   // erc,cro,pne
	CMD_ACTION_LIGHTFLARE = 102,   // erc,cro,pne
	CMD_ACTION_LITCHEMLIGHT = 103,   // erc,cro,pne
	CMD_ACTION_UNPINGRENAGE = 104,   // erc,cro,pne
	CMD_ACTION_DEPLOY = 105,   // cro
	CMD_ACTION_VOMIT = 106,   // cro
	CMD_ACTION_DRINKCAN = 107,   // erc,cro,pne
	CMD_ACTION_OPENDOORFW = 108,   // erc,cro !!! DUPLICIT
	CMD_ACTION_OPENDOORBW = 109,   // erc,cro 
	CMD_ACTION_SWIPELDOOR = 110,   // erc,cro 
	CMD_ACTION_SWIPERDOOR = 111,   // erc,cro 
	CMD_ACTION_OPENLID = 112,   // erc,cro 
	CMD_ACTION_CLOSELID = 113,   // erc,cro 	
	CMD_ACTION_GIVEL = 114,   // erc,cro,pne
	CMD_ACTION_GIVER = 115,   // erc,cro,pne */
	
		
    //! --------------------------
    //! modifier (differential animation)
    CMD_ACTIONMOD_DRINK = 0,		    // erc,cro,pne      [end, end2]
	CMD_ACTIONMOD_EAT = 1,              // erc,cro,pne      [end, end2]
	CMD_ACTIONMOD_EMPTYCANISTER = 2, 	// erc, cro         [end]
	CMD_ACTIONMOD_CATCHRAIN = 3,        // erc,cro,pne      [end]
	CMD_ACTIONMOD_EATPILLS = 4,         // erc,cro,pne      [end]
	CMD_ACTIONMOD_EATTABLETS = 5,       // erc,cro,pne      [end]
	CMD_ACTIONMOD_EMPTYBOTTLE = 6,      // erc,cro          [end]
	CMD_ACTIONMOD_OPENCOMPASS = 7,      // erc              [end]
	
	// onetime 
	CMD_ACTIONMOD_LICKBATTERY = 100,    // erc,cro,pne
	CMD_ACTIONMOD_PICKUP = 101,         // erc,cro,pne
	CMD_ACTIONMOD_LIGHTFLARE = 102,     // erc,cro,pne
	CMD_ACTIONMOD_LITCHEMLIGHT = 103,   // erc,cro,pne
	CMD_ACTIONMOD_UNPINGRENAGE = 104,   // erc,cro,pne
	CMD_ACTIONMOD_DRINKCAN = 105,       // erc,cro,pne
	CMD_ACTIONMOD_OPENDOORFW = 106,     // erc,cro
	CMD_ACTIONMOD_OPENDOORBW = 107,     // erc,cro 
	CMD_ACTIONMOD_SWIPELDOOR = 108,     // erc,cro 
	CMD_ACTIONMOD_SWIPERDOOR = 109,     // erc,cro 
	CMD_ACTIONMOD_OPENLID = 110,        // erc,cro 
	CMD_ACTIONMOD_CLOSELID = 111,       // erc,cro 	
	CMD_ACTIONMOD_GIVEL = 112,          // erc,cro,pne
	CMD_ACTIONMOD_GIVER = 113,          // erc,cro,pne 

    //! --------------------------
    //! fb (full body)
	CMD_ACTIONFB_DRINKPOND = 0,	        // cro 	      	    [end]
	CMD_ACTIONFB_DRINKWELL = 1,         // cro 	            [end]
	CMD_ACTIONFB_FILLBOTTLEWELL = 2,    // cro	            [end]
	CMD_ACTIONFB_FIREESTINGUISHER = 3,  // erc              [end]
	CMD_ACTIONFB_WRING = 4,   		    // cro 		        [end]
	CMD_ACTIONFB_BERRIES = 5,           // erc,cro		    [end]
	CMD_ACTIONFB_CHOPTREE = 6,          // erc			    [end]
	CMD_ACTIONFB_DIGSHOVEL = 7,         // erc		        [end]
	CMD_ACTIONFB_DIGHOE = 8,            // erc			    [end]
	CMD_ACTIONFB_FISHING = 9,           // cro			    [action, end(catch fish), end2(not catching anything) ]
	CMD_ACTIONFB_CPR = 10,              // cro     		    [loop(actually pumping), end]
	CMD_ACTIONFB_BANDAGE = 11,          // cro		        [end]
	CMD_ACTIONFB_CRAFTING = 12,         // cro		        [end]
	CMD_ACTIONFB_INTERRACTPLAYER = 13,  // cro 	            [end]
	CMD_ACTIONFB_DRINKCANISTER = 14, 	// erc, cro         [end]
	CMD_ACTIONFB_FORCEFEED = 15, 	    // erc, cro         [end]
	CMD_ACTIONFB_BANDAGETARGET = 16, 	// erc, cro         [end]
	CMD_ACTIONFB_DISINFECTTARGET = 17, 	// erc, cro         [end]
	CMD_ACTIONFB_TRANSFUSETARGET = 18, 	// erc, cro         [end]
	
	// onetime 
	CMD_ACTIONFB_DEPLOY = 100,          // cro
	CMD_ACTIONFB_VOMIT = 101,           // cro
	CMD_ACTIONFB_MORPHINE = 102,        // erc, cro
	CMD_ACTIONFB_INJECTION = 103,       // erc, cro
	CMD_ACTIONFB_STARTFIREMATCH = 104,  // erc, cro
	CMD_ACTIONFB_SKINANIMAL = 105,      // cro
	CMD_ACTIONFB_WASHHANDS = 106,       // cro


	//! internal action commands 
	CMD_ACTIONINT_END2			= -1,	//!< secondary ending
	CMD_ACTIONINT_END			= 0, 	//!< end action
	CMD_ACTIONINT_ACTION		= 1, 	//!< one time secondary action within an action
	CMD_ACTIONINT_ACTIONLOOP	= 2, 	//!< loop secondary action within an action

	CMD_ACTIONINT_COMMITSUICIDE = CMD_ACTIONINT_END2,	//!< commiting suicide
	CMD_ACTIONINT_AVOIDSUCIDE 	= CMD_ACTIONINT_END, 	//!< not commiting suicide
    
    //! ---------------------------------------------------------
    //! ---------------------- GESTURES -------------------------
    //! ---------------------------------------------------------
    //! modifier (differential animation)
    CMD_GESTUREMOD_GREETING = 1000,		// erc,cro,pne 	    [end]
    CMD_GESTUREMOD_POINT = 1001,		// erc,cro,pne 	    [end]
    CMD_GESTUREMOD_THUMB = 1002,		// erc,cro,pne 	    [end]
    CMD_GESTUREMOD_SILENCE = 1003,		// erc,cro,pne 	    [end]
    CMD_GESTUREMOD_TAUNT = 1004,		// erc,cro,pne 	    [end]
    CMD_GESTUREMOD_TIMEOUT = 1005,		// erc,cro,pne 	    [end]
    CMD_GESTUREMOD_HEART = 1006,		// erc,cro,pne 	    [end]
    CMD_GESTUREMOD_FACEPALM = 1007,		// erc,cro   	    [end]
    CMD_GESTUREMOD_RPS = 1008,		    // erc 	            [SPECIAL, SEE BELOW]

    CMD_ACTIONINT_RPS_ROCK 	= 10,	    // RPS Internal Gesture
    CMD_ACTIONINT_RPS_PAPER 	= 11,	// RPS Internal Gesture
    CMD_ACTIONINT_RPS_SCISSORS = 12,	// RPS Internal Gesture


    // onetime 
    CMD_GESTUREMOD_THROAT = 1100,		// erc,cro,pne
    CMD_GESTUREMOD_CLAP = 1101,		    // erc,cro

    //! --------------------------
    //! fb (full body)
    CMD_GESTUREFB_SALUTE = 1000,	    // erc              [end]
    CMD_GESTUREFB_CAMPFIRE = 1001,	    // cro              [end]
    CMD_GESTUREFB_LYINGDOWN = 1002,	    // cro              [end]
    CMD_GESTUREFB_SOS = 1003,	        // erc              [end]
    CMD_GESTUREFB_SITA = 1004,	        // cro              [end]
    CMD_GESTUREFB_SITB = 1005,	        // cro              [end]

    // onetime 
    CMD_GESTUREFB_DANCE = 1100,	        // erc
    
    //! ---------------------------------------------------------
    //! ---------------------- SUICIDE --------------------------
    //! ---------------------------------------------------------
    //! fb (full body)
    CMD_SUICIDEFB_1HD = 2000,	        // cro              [end (cancel), end2 (commit)]
    CMD_SUICIDEFB_FIREAXE = 2001,	    // erc              [end (cancel), end2 (commit)]
    CMD_SUICIDEFB_PITCHFORK = 2002,	    // erc              [end (cancel), end2 (commit)]
    CMD_SUICIDEFB_PISTOL = 2003,	    // cro              [end (cancel), end2 (commit)]
    CMD_SUICIDEFB_RIFLE = 2004,	        // cro              [end (cancel), end2 (commit)]
    CMD_SUICIDEFB_SWORD = 2005,	        // erc              [end (cancel), end2 (commit)]
    CMD_SUICIDEFB_UNARMED = 2006,	    // cro              [end (cancel), end2 (commit)]
    CMD_SUICIDEFB_WOODAXE = 2007,	    // erc              [end (cancel), end2 (commit)]
    CMD_SUICIDEFB_SPEAR = 2008,	        // erc              [end (cancel), end2 (commit)]
	

};



// *************************************************************************************
// ! DayZPlayer - mainly for logic and camera 
// *************************************************************************************
class DayZPlayer extends Human
{
	
	
	//! ---------------- command handling -------------------------

	//! updated each tick 
	//! handles: starting new commands, canceling, interruption 
	//! pDt 						- delta time 
	//! pCurrentCommandID 			- actually running command's ID 
	//! pCurrentCommandFinished		- is current command finished 
	
	void 			CommandHandler(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished);	

	//! ---------------- camera handling -------------------------

	//! virtual 
	//! returns registered camera type
	//! pCamType - CamInternal, CamExternal ... 
	int 			CameraHandler(int pCamType);


	//! ---------------- camera additiona functions -------------------------

	//! gets current camera 
	proto native 	DayZPlayerCamera		GetCurrentCamera();

	//! gets current camera 
	proto native 	int 					GetCurrentPerItemCameraUD();

	
	//! ---------------- sync shadow -------------------------


	//! test if entity is DayZPlayer 
	static proto native bool				IsEntityDayZPlayer(IEntity pEntity);

	//! 
	proto native 	bool					DebugSyncShadowSetup(DayZPlayer pPlayer);



	//! ---------------- camera additiona functions -------------------------
	
	
	//! returns true if player is currently in one of the stances specified by stance mask 
	//! IsPlayerInStance(STANCEMASK_ERECT | STANCEMASK_CROUCH) returns true if player is standing or crouching and not raised (aimed)
	//! IsPlayerInStance(STANCEMASK_PRONE | STANCEIDX_RAISEDPRONE) returns true if player is in or in prone (both raised or nonraised)
	//! IsPlayerInStance(STANCEMASK_ALL) returns true always 
	//! IsPlayerInStance(STANCEMASK_RAISEDERECT | STANCEMASK_RAISEDCROUCH | STANCEMASK_RAISEDPRONE) returns true if player has raised hands
	
	// PSOVIS - move to native
	bool		IsPlayerInStance(int pStanceMask)		// STANCEMASK_ERECT | STANCEMASK_CROUCH 
	{
		HumanMovementState		state = new HumanMovementState;
		GetMovementState(state);
		bool ret = ((1 << state.m_iStanceIdx) & pStanceMask) != 0;
		delete state;
		return ret;
	}
}

