class ActionBaseCB : HumanCommandActionCallback
{	
	protected ActionBase				m_ActionData;//ref to static part
	protected int						m_State; //action module returns here its current state
	protected Object					m_Target; //target of action
	protected ItemBase					m_Item; //item in players hands being used for action
	protected PlayerBase				m_Player;//performer of actions
	protected autoptr CABase			m_ActionComponent;//action component
	protected SoundOnVehicle 			m_SoundObject;//object of sound playing on entity
	protected bool 						m_Canceled;//helps prevent doubled calling of actionbase End method
		
	//Command events
	void OnAnimationEvent(int pEventID)	
	{
		if ( pEventID == UA_ANIM_EVENT ) 
		{
			ProgressAction(); 
			ProgressAction();
		}
		//Print("ActionBase.c | ActionBaseCB | OnAnimationEvent called");
	}
	
	bool CancelCondition()
	{
		if ( GetState() == STATE_LOOP_LOOP )
		{
			ProgressAction();
			//Print("ActionBase.c | ActionBaseCB| CancelCondition called ");
		}
		return DefaultCancelCondition(); 
	}
	
	void OnFinish(bool pCanceled)	
	{
		if ( m_SoundObject ) GetGame().ObjectDelete(m_SoundObject);
		if ( m_Target ) m_ActionData.UnlockTarget(m_Player,m_Target);
		if( m_Player.m_ActionManager ) m_Player.m_ActionManager.EjectAction();//jtomasik- pokud byla akce injectnuta, tak tohle se postara aby actionmanager zase zacal misto ni nabizet kontextualni akce, ale mozna neni uplne idealni actionmanager nastavovat odtud
		EndAction(pCanceled);
	}
	
	bool SetCommand( int command_uid )
	{		
		InternalCommand(command_uid);
	}
		 	
	//Action handlers
	void ProgressAction()
	{
		m_ActionData.Do(this,m_State,m_ActionComponent,m_Player,m_Target,m_Item);
	}	
	
	void EndAction(bool cancel)
	{
		//Print("ActionBase.c | ActionBaseCB| EndAction called ");
		if ( cancel && !m_Canceled )
		{
			m_State = UA_CANCEL;
			m_ActionData.End(this,m_State,m_ActionComponent,m_Player,m_Target,m_Item);
			m_Canceled = true;
		}
	}
	
	//Action Component handlers
	void CreateActionComponent() 
	{
		m_ActionComponent = new CADummy;
	}
	
	void InitActionComponent()
	{
		m_Canceled = false;
		if ( m_Target ) m_ActionData.LockTarget(m_Player,m_Target);
		CreateActionComponent();
		m_ActionComponent.Init(m_ActionData, m_Player, m_Target, m_Item);
		m_State = UA_PROCESSING;
		if ( m_ActionComponent.IsContinuousAction() ) 
		{ 
			SetCommand(DayZPlayerConstants.CMD_ACTIONINT_ACTIONLOOP);
			EnableCancelCondition(true);
		}
		else
		{	
			SetCommand(DayZPlayerConstants.CMD_ACTIONINT_ACTION);
			RegisterAnimationEvent("ActionExec", UA_ANIM_EVENT);
		}
		m_SoundObject = m_ActionData.PlayActionSound(m_Player);
	}
		
	int ProgressActionComponent()
	{
		m_State = m_ActionComponent.Execute(m_Player, m_Target, m_Item);
		return m_State;
	}	
	
	int EndActionComponent()
	{
		m_State = m_ActionComponent.Interupt(m_Player, m_Target, m_Item);
		return m_State;
	}	
	
	//data
	void SetActionData(ActionBase actionBase, PlayerBase player, Object target, ItemBase item )
	{
		m_ActionData = actionBase;
		m_Player = player;
		m_Target = target;
		m_Item = item;
	}
	
	bool IsUserActionCallback()
	{
		return true;
	}
};

class ActionBase
{	
	//STATIC DATA
	// Configurable action parameters
	protected string					m_MessageStartFail; //called from ontick method of quantity and time baseclasses, usually informing player that item is empty
	protected string					m_MessageStart; //called from onstart method, usually informing player that he started the action
	protected string					m_MessageSuccess; //called from ontick method, ussualy informing player that he succesfully finished the action
	protected string 					m_MessageFail; //called from cancel or ontick methods,  ussualy informing player that he or target moved and thus was action canceled
	protected string					m_MessageCancel; //called from cancel method, ussualy informing player that he stoped holding RMB and canceled action
	protected string					m_MessageSuccessTarget; //called from ontick method, ussualy informing other player that he succesfully finished the action
	protected string 					m_MessageStartTarget; //called from cancel or ontick methods,  ussualy informing other player that he or target moved and thus was action canceled
	protected string					m_MessageCancelTarget; //called from cancel method, ussualy informing other player that he stoped holding RMB and canceled action
	protected string					m_Sound; //sound played at the beggining of action

	protected bool						m_LockTargetOnUse;	//this parameter sets wheter player can perform actions on target while other player is already performing action on it. defaulted as true
	protected bool 						m_FullBody; //tells whether action is full body or additive
	protected int						m_StanceMask;
	protected int 						m_CommandUID;	//tells which command should be used for callback
	protected typename 					m_CallbackClass; //tells which callback should be instantiated
	protected autoptr TStringArray		m_Sounds;			//User action sound is picked from this array randomly
	autoptr CCIBase 					m_ConditionItem;	//Condition Component
	autoptr CCTBase						m_ConditionTarget; 	//Condition Component	

	//RUNTIME DATA
	protected autoptr Param1<string> 	m_MessageParam; //used for passing messages from server to client
	protected autoptr Param2<int,int>	m_MessagesParam; 	
	
	//-----------------------------------------------------
	// 	Action events and methods
	//-----------------------------------------------------
	void ActionBase() 
	{
		// definable
		m_CallbackClass = ActionBaseCB;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_PRONE;
		m_FullBody = false;
		m_Sound = "";
		m_LockTargetOnUse = true;
		m_MessageStartFail = "The action failed";
		m_MessageStart = "I have started an action.";
		m_MessageSuccess = "The action succesfully finished.";
		m_MessageFail = "The action failed.";
		m_MessageCancel = "I have canceled the action.";
		m_MessageStartTarget = "Other player started performing action on you.";
		m_MessageSuccessTarget = "Other player succesfuly performed action on you.";
		m_MessageCancelTarget = "Other player canceled the action.";
		
		// dont override
		m_MessageParam = new Param1<string>("");
		m_MessagesParam = new Param2<int,int>(0,0);
		m_Sounds = new TStringArray;
	}
			
	void CreateConditionComponents() //Instantiates components, called once from ActionManager on actions construction
	{
		m_ConditionItem = new CCIDummy;
		m_ConditionTarget = new CCTDummy;
	}
	
	int GetType()  //returns action uid
	{
		return 0;
	}

	string GetText() //text game dispalys in HUD hint 
	{
		return "default action text";
	}
	
	protected void OnStart( PlayerBase player, Object target, ItemBase item ) //setup of action parameters, called before performing action
	{
	}

	protected void OnCancel( PlayerBase player, Object target, ItemBase item, Param acdata  ) //method called when player stops inputing
	{
	}
	
	protected void OnFail( PlayerBase player ) //method called when action has not met conditions in action component
	{
	}

	protected void OnComplete( PlayerBase player, Object target, ItemBase item, Param acdata ) //method called on succesful finishing of action
	{
	}
	
	protected void OnRepeat( PlayerBase player, Object target, ItemBase item, Param acdata ) //method called on succesful finishing of action
	{
	}
	
	protected bool ActionCondition( PlayerBase player, Object target, ItemBase item ) //condition for action
	{
		return true;
	}
	
	void ApplyModifiers( PlayerBase player, Object target, ItemBase item ) // method that is planned to be called after every succesful completion of action to transfer diseases and other modifiers, now is called before oncompletes
	{
		if ( target && target.IsMan() )
		{
			if ( item ) item.TransferModifiers(target);
		}
	}


	//----------------------------------------------------------------------------------------------
	// Core methods don't override unless you know what you are doing
	//----------------------------------------------------------------------------------------------		
	// COMMANDS -----------------------------------------------------------------------		
	protected int GetActionCommand()
	{
		return m_CommandUID;
	}
	
	protected int GetStanceMask()
	{
		return m_StanceMask;
	}
	
	protected bool IsFullBody()
	{
		return m_FullBody;
	}
	
	protected typename GetCallbackClassTypename()
	{
		return m_CallbackClass; 
	}
	
	protected ActionBaseCB CreateAndSetupActionCallback( PlayerBase player, Object target, ItemBase item )
	{
		ActionBaseCB callback;
		if (  IsFullBody() )
		{
			callback = player.StartCommand_Action(GetActionCommand(),GetCallbackClassTypename(),GetStanceMask());			
		}
		else
		{
			callback = player.AddCommandModifier_Action(GetActionCommand(),GetCallbackClassTypename());
		}
		callback.SetActionData(this, player, target, item); 
		callback.InitActionComponent(); //jtomasik - tohle mozna patri do constructoru callbacku?
		return callback;
	}
		
		
		
	//	ACTION LOGIC -------------------------------------------------------------------
	// called from actionmanager.c
	void Start( PlayerBase player, Object target, ItemBase item ) //Setup on start of action
	{
		PlayActionSound(player);
		OnStart(player, target, item);
		InformPlayers(player,target,UA_START);	
		CreateAndSetupActionCallback(player, target, item);
		
		/*Debug
		SendMessageToClient( player, "ActionBase.c : Start");
		Print("ActionBase.c : Start");
		*/
	}

	bool Can( PlayerBase player, Object target, ItemBase item )
	{
		if ( !m_FullBody && (!player || !player.IsPlayerInStance(GetStanceMask())) )
		{
			return false;
		}
		if ( m_ConditionItem && m_ConditionItem.Can(player, item) && m_ConditionTarget && m_ConditionTarget.Can(player, target) && ActionCondition(player, target, item) ) return true;
		return false;
	}
	
	// called from ActionBaseCB.c
	void Do( ActionBaseCB callback, int state, CABase action_component, PlayerBase player, Object target, ItemBase item )
	{
		if ( state == UA_ERROR || !callback || !player || !action_component || !m_ConditionItem || !m_ConditionTarget ||  ( item && item != player.GetItemInHands() ) || ( m_ConditionTarget.IsActionOnTarget() && !target ) ) //jtomasik - tohle mozna muze byt v CancelCondtion metodu callbacku?
		{
			Print("Do - ABRUPT CANCEL, CONDITIONS NOT MET");
			if ( callback && player )
			{			
				callback.Cancel();
			}
			else
			{
				Print("Do - PLAYER LOST");
			}	
		}
		else
		{
			switch ( state )
			{
				case UA_FAILED:
					OnFail(player);
					InformPlayers(player,target,UA_ERROR);
					callback.SetCommand(DayZPlayerConstants.CMD_ACTIONINT_END2);
					//callback.Cancel();//jtomasik - ted prehraje END2 (a tim to i ukonci), pozdeji by mozna piti prazdne lahve atp. mela byt samostatna akce
					break;
				
				case UA_PROCESSING:	
					if ( Can(player, target, item) )
					{
						callback.ProgressActionComponent(); //jtomasik- nevim jestli tohle volani tam a zpet je uplne ok
						InformPlayers(player,target,UA_PROCESSING);
					}
					else
					{
						InformPlayers(player,target,UA_FAILED);
						callback.Cancel();
					}
					break;
					
				case UA_REPEAT:
					OnRepeat(player,target,item,action_component.GetACData());
					ApplyModifiers(player,target,item);
					InformPlayers(player,target,UA_REPEAT);
					break;
					
				case UA_FINISHED:
					OnComplete(player,target,item,action_component.GetACData());
					ApplyModifiers(player,target,item);//mozna volat z modulu?
					InformPlayers(player,target,UA_FINISHED);
					callback.SetCommand(DayZPlayerConstants.CMD_ACTIONINT_END);
					break;
					
				default:
					Print("Do - ACTION COMPONENT RETURNED WRONG VALUE");
					callback.Cancel();
					break;
			}
		}
	}
	
	// called from ActionBaseCB.c 
	void End( ActionBaseCB callback, int state, CABase action_component, PlayerBase player, Object target, ItemBase item )
	{
		if ( action_component && player && item && m_ConditionTarget && ( !m_ConditionTarget.IsActionOnTarget() || ( m_ConditionTarget.IsActionOnTarget() && target)) ) 
		{
			int action_component_result = callback.EndActionComponent(); //jtomasik- nevim jestli tohle volani tam a zpet je uplne ok
			switch ( action_component_result )
			{
				case UA_FINISHED:
					
					OnComplete(player,target,item,action_component.GetACData());
					ApplyModifiers(player,target,item);
					InformPlayers(player,target,UA_FINISHED);
					callback.SetCommand(DayZPlayerConstants.CMD_ACTIONINT_END);
					break;
					
				case UA_CANCEL:
					OnCancel(player, target, item,action_component.GetACData());
					InformPlayers(player,target,UA_CANCEL);
					callback.Cancel();
					break;
					
				default:
					Print("Cancel - ACTION COMPONENT RETURNED WRONG VALUE");
					callback.Cancel();
				break;
			}
		}
	}
	
	void LockTarget( PlayerBase player, Object target )
	{
		if ( m_LockTargetOnUse && player.m_ModuleObjectsInteractionManager ) player.m_ModuleObjectsInteractionManager.Lock(target);
	}
	
	void UnlockTarget( PlayerBase player, Object target )
	{
		if ( m_LockTargetOnUse && player.m_ModuleObjectsInteractionManager ) player.m_ModuleObjectsInteractionManager.Release(target); 
	}

		
	// MESSAGES --------------------------------------------------------------------
	string GetMessageText( int state ) //returns text of action based on given id
	{
		string message = "";
		switch ( state )
		{
			case UA_ERROR:
				message = GetMessageStartFail();
				break;
				
			case UA_FAILED:
				message = GetMessageFail();
				break;
				
			case UA_PROCESSING:
				message = "Processing";
				break;
				
			case UA_REPEAT:
				message = "loop end";
				break;
				
			case UA_START:
				message = GetMessageStart();
				break;
				
			case UA_FINISHED:
				message = GetMessageSuccess();
				break;
				
			case UA_CANCEL:
				message = GetMessageCancel();
				break;
				
			case UA_STARTT:
				message = GetMessageStartTarget();
				break;
				
			case UA_FINISHEDT:
				message = GetMessageSuccessTarget();
				break;
				
			case UA_CANCELT:
				message = GetMessageCancelTarget();
				break;
				
			default:
				Print("Informplayers - CALLED WITH WRONG STATE PARAMETER");
				break;
		}
		if ( message != "" )
		{
			return message;
		}
	}
	
	protected void InformPlayers( PlayerBase player, Object target, int state ) //delivers message ids to clients based on given context
	{				
		player.MessageAction(GetMessageText(state)); //jtomasik - tohle se smaze az to pujde ven, ted je to jako debug pro lokalni testovani
		
		if( GetGame().IsServer() && player && m_MessagesParam && state > 0 && state < 12 )
		{
			int message_target = 0;
			switch ( state )
			{
				case UA_START:
					message_target = UA_STARTT;
					break;
					
				case UA_FINISHED:
					message_target = UA_FINISHEDT;
					break;
					
				case UA_CANCEL:
					message_target = UA_CANCELT;
					break;
			}
			if ( player.IsAlive() )
			{
				m_MessagesParam.param1 = GetType();
				m_MessagesParam.param2 = state;
				GetGame().RPCSingleParam(player, RPC_USER_ACTION_MESSAGES, m_MessagesParam, player);
			}
			if ( message_target > 0 && m_ConditionTarget && m_ConditionTarget.IsActionOnTarget() && target && target.IsMan() && target.IsAlive() )
			{
				m_MessagesParam.param1 = GetType();
				m_MessagesParam.param2 = message_target;
				GetGame().RPCSingleParam(target, RPC_USER_ACTION_MESSAGES, m_MessagesParam, target);
			}
		}
	}
		
	void SendMessageToClient( Object reciever, string message ) //sends given string to client, don't use if not nescessary
	{
		if( GetGame().IsServer() && reciever && reciever.IsMan() && m_MessageParam && reciever.IsAlive() && message != "" )
		{
			m_MessageParam.param1 = message;
			PlayerBase man = reciever;
			GetGame().RPCSingleParam(man, RPC_USER_ACTION_MESSAGE, m_MessageParam, man);
		}
	}
	
	protected string GetMessageStartFail()
	{
		return m_MessageStartFail;
	}
	
	protected string GetMessageStart()
	{
		return m_MessageStart;
	}
	
	protected string GetMessageSuccess() 
	{
		return m_MessageSuccess;
	}
	
	protected string GetMessageFail()
	{
		return m_MessageFail;
	}
	
	protected string GetMessageCancel() 
	{
		return m_MessageCancel;
	}
	
	protected string GetMessageSuccessTarget() 
	{
		return m_MessageSuccessTarget;
	}
	
	protected string GetMessageStartTarget()
	{
		return m_MessageStartTarget;
	}
	
	protected string GetMessageCancelTarget()
	{
		return m_MessageCancelTarget;
	}
	
	// SOUNDS ------------------------------------------------------
	SoundOnVehicle PlayActionSound( PlayerBase player )
	{
		if ( player ) 
		{
			if ( m_Sound != "" )
			{
				return GetGame().CreateSoundOnObject(player, m_Sound, 6, false);
			}
			if ( m_Sounds && m_Sounds.Count() > 0 )
			{
				int rand_num =  Math.RandomInt(0, m_Sounds.Count());
				return GetGame().CreateSoundOnObject(player, m_Sounds.Get(rand_num), 6, false);
			}
		}
	}
};