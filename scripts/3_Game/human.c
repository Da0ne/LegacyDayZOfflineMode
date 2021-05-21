/** @file 

  this file is interface to Human 

*/



// *************************************************************************************
// ! HumanItemBehaviorCfg - per item config of stances / movements   
// *************************************************************************************
class HumanItemBehaviorCfg
{

	// static int	DEFMOVEMENT				= MOVEMENT_IDLE | MOVEMENT_WALK | MOVEMENT_RUN | SPRINT;

	int		m_iStanceMask;					//!< combinations of STANCEMASK_
	int		m_StanceMovements[6];			//! 6 stances -> all has movement mask, STANCEIDX_ ... is index
	float	m_StanceRaisedXAimOffsets[6];	//! Erected Walk,Run,Sprint, Crouched Walk,Run,Sprint X(LeftRight) aiming offsets
	float	m_StanceRaisedYAimOffsets[6];	//! Erected Walk,Run,Sprint, Crouched Walk,Run,Sprint Y(UpDown) aiming offsets
	int 	m_iPerItemCameraUserData;		//! per item camera user data - can be obtained in runtime by DayZPlayer.GetPerItemCameraUD()
}



// *************************************************************************************
// ! HumanInputController - what we know about the input - human.GetInputController()
// *************************************************************************************
class HumanInputController
{
	//! returns per tick aim change (in radians) 
	proto native vector 		GetAimChange();

	//! returns per tickaim change (in radians) 
	proto native bool			CameraIsFreeLook();
	
	//! returns true if camera is on right/ false-left shoulder
	proto native bool			Camera3rdIsRightShoulder();


	//! never created by script
	private void HumanInputController()
	{
	}

	//! never created by script
	private void ~HumanInputController()
	{
	}	
}


// *************************************************************************************
// ! HumanCommandActionCallback - called as action callback 
// *************************************************************************************
class HumanCommandActionCallback
{
	//! cancels action 
	proto native 	void 	Cancel();	

	//! calls internal command 
	//! pInternalCommandId is one of CMD_ACTIONINT_ ... 
	proto native 	void 	InternalCommand(int pInternalCommandId);	

	//! enables calling cancel condition
	//! default is disabled
	//! when enabled - there must be a function "bool CancelCondition()" implemented
	proto native 	void 	EnableCancelCondition(bool pEnable);

	//! system implemented cancel condition (now raise or sprint cancels action)
	proto native 	bool	DefaultCancelCondition();

	//! registers animation event, when event registered and received - it sends OnAnimationEvent with registered id 
	proto native void 		RegisterAnimationEvent(string pAnimationEventStr, int pId);

	//! example of implement of CancelCondition
	//! returns true it it should CANCEL the action
	//	bool				CancelCondition()	{ return false; }

	//! undef state - not running anymore
	static const int 	STATE_NONE 			= 0;

	//! looped state 
	static const int 	STATE_LOOP_IN 		= 1;
	static const int 	STATE_LOOP_LOOP 	= 2;
	static const int 	STATE_LOOP_END 		= 3;
	static const int 	STATE_LOOP_END2		= 4;
	static const int 	STATE_LOOP_LOOP2	= 5;
	static const int 	STATE_LOOP_ACTION	= 6;

	//! one time state 
	static const int 	STATE_NORMAL 		= 7;

	//! returns one of STATE_...
	proto native int 	GetState();


	//----------------------------------
	// debug helper

	//! returns debug string of current state 
	string	GetStateString()
	{
		int		state = GetState();
		switch (state)
		{
		case STATE_NONE:			return "NONE";

		case STATE_LOOP_IN:			return "LOOP_IN";
		case STATE_LOOP_LOOP: 		return "LOOP_LOOP";
		case STATE_LOOP_END:		return "LOOP_END";
		case STATE_LOOP_END2:		return "LOOP_END2";
		case STATE_LOOP_LOOP2:		return "LOOP_LOOP2";
		case STATE_LOOP_ACTION:		return "LOOP_ACTION";

			//! one time state 
		case STATE_NORMAL:			return "ONE TIME";
		}

		return "Undef";
	}



	//----------------------------------
	// callbacks

	//! called when activated
	// void 	OnActivate()	{};


	//! calls animation event -> must be registered with RegisterAnimationEvent
	//!
	void 	OnAnimationEvent(int pEventID)	{};

	//! called ALWAYS! when action is finished (normal finish or cancel -> pCanceled)
	void 	OnFinish(bool pCanceled)	{};		
	
	
	// helps identify type of callback
	bool IsUserActionCallback()
	{
		return false;
	}
	
	bool IsSymptomCallback()
	{
		return false;
	}
}



// *************************************************************************************
// ! HumanMovementState - movement state, command, stance, movement, human.GetMovementState(movementState)
// *************************************************************************************
class HumanMovementState
{
	int 		m_CommandTypeId;	//! current command's id 
	int 		m_iStanceIdx;		//! current stance (DayZPlayer.STANCEIDX_ERECT, ...), only if the command has a stance
	int 		m_iMovement;		//! current movement (0 idle, 1 walk, 2-run, 3-sprint), only if the command has a movement 
}



// *************************************************************************************
// ! Human - human script interface 
// *************************************************************************************
class Human extends Man
{
	//---------------------------------------------------------
	// bone transforms 

	//! returns bone index for a name (-1 if pBoneName doesn't exist)
	proto native 	int 		GetBoneIndexByName(string pBoneName);

	//! returns local space, model space, world space position of the bone 
	proto native	vector		GetBonePositionLS(int pBoneIndex);
	proto native 	vector		GetBonePositionMS(int pBoneIndex);
	proto native 	vector		GetBonePositionWS(int pBoneIndex);

	//! returns local space, model space, world space orientation (quaternion) of a bone 
	proto native	void 		GetBoneRotationLS(int pBoneIndex, out float pQuat[4]);
	proto native 	void 		GetBoneRotationMS(int pBoneIndex, out float pQuat[4]);
	proto native 	void 		GetBoneRotationWS(int pBoneIndex, out float pQuat[4]);

	//! returns local space, model space, world space orientation (quaternion) of a bone 
	proto native	void 		GetBoneTransformLS(int pBoneIndex, out vector pTm[4]);
	proto native 	void 		GetBoneTransformMS(int pBoneIndex, out vector pTm[4]);
	proto native 	void 		GetBoneTransformWS(int pBoneIndex, out vector pTm[4]);


	//---------------------------------------------------------
	// controller 
			
	//! returns human input controller
	proto native 	HumanInputController	GetInputController();


	//---------------------------------------------------------
	// commands 

	//! returns movement state (current command id, )
	proto native 	void 	GetMovementState(HumanMovementState pState);

	//! returns current command ID
	// proto native 	int 	GetCurrentCommandID();


	//!----- MOVE -----

	//! starts command - Move
	proto native 	void 						StartCommand_Move();

	//!----- ACTIONS ----- 

	//! starts command - Action 
	//! pStanceMask is mix of flags STANCEMASK_ERECT, ... 
	proto native 	HumanCommandActionCallback 	StartCommand_Action(int pActionID, typename pCallbackClass, int pStanceMask);

	//! is human is in command action - returns its callback, if current command is action 
	proto native	HumanCommandActionCallback 	GetCommand_Action();

	
	//---------------------------------------------------------
	// command modifiers

	//!
	proto native 	int 		GetCommandModifierCount();

	//! returns COMMANDID_ .. type id of command modifier on index pIndex
	proto native 	int 		GetCommandModifierId(int pIndex);	



	// --- modifier/additive actions - played on COMMANDID_MOVE command

	//! adds action command modifier, creates callback instance for you 
	proto native 	HumanCommandActionCallback	AddCommandModifier_Action(int pActionID, typename pCallbackClass);

	//! force remove - normally if action is ended or interrupted - this is not needed to call 
	proto native 	void 						RemoveCommandModifier_Action(HumanCommandActionCallback pCallback);

	//! returns callback for action if any is active, null if nothing runs 
	proto native 	HumanCommandActionCallback	GetCommandModifier_Action();


	//---------------------------------------------------------
	// debug info for Item Accessor


	//! returns current item's class name
	proto native   string  						DebugGetItemClass();

	//! returns current item's class that is found in config
	proto native   string  						DebugGetItemSuperClass();

	//! returns current item's animation instance 
	proto native   string  						DebugGetItemAnimInstance();


}
