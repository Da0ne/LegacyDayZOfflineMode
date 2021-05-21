// *************************************************************************************
// ! PluginDayzPlayerDebugUserData
// *************************************************************************************
class PluginDayzPlayerDebugUserData
{
	void PluginDayzPlayerDebugUserData(int pUserData, bool pFullBody, int pStanceMask = -1)
	{
		m_iUserData 	= pUserData;
		m_bIsFullBody	= pFullBody;
		m_iStanceMask	= pStanceMask;
	}
		

	int		GetInt()
	{
		return m_iUserData;
	}

	bool	IsFullBody()
	{
			return m_bIsFullBody;
	}
	
	int 	GetStanceMask()
	{
			return m_iStanceMask;
	}

	protected int 	m_iUserData;
	protected bool	m_bIsFullBody;
	protected int 	m_iStanceMask;
}

// *************************************************************************************
// ! PluginDayzPlayerDebugUI 
// *************************************************************************************
class PluginDayzPlayerDebugUIHandler extends ScriptedWidgetEventHandler
{
	bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		return m_pPluginPlayerDebug.OnClick(w, x, y, button);
	}


	PluginDayzPlayerDebug		m_pPluginPlayerDebug;
}


// *************************************************************************************
// ! PluginDayzPlayerDebugUI 
// *************************************************************************************
class PluginDayzPlayerActionCallback extends HumanCommandActionCallback
{
	void 	PluginDayzPlayerActionCallback()
	{
		Print("Action callback created");
		EnableCancelCondition(true);
		RegisterAnimationEvent("ActionExec", 1);	// really weird id 

		m_fTimeStart = GetWorldTime();
	}
	
	void 	~PluginDayzPlayerActionCallback()
	{
		Print("Action callback deleted");
	}

	void 	OnFinish(bool pCanceled)
	{
		if (pCanceled)
		{
			m_pStateWidget.SetText("Result: canceled");
		}
		else
		{
			m_pStateWidget.SetText("Result: ended");
		}
	}


	bool				CancelCondition()	
	{ 
		string a = GetStateString();
		m_pStateWidget.SetText(a);

		// Print("Action cancel condition");
		return DefaultCancelCondition(); 
	}

	void 				OnAnimationEvent(int pEventID)
	{
		// only one event id is registered - 1 so nothing else can be delivered
		string a = "ActionExec event at: ";
		float  tFromSt = GetWorldTime() - m_fTimeStart;
		a += tFromSt.ToString();

		m_pAnimEventWidget.SetText(a);
	}



	//! 
	float 								m_fTimeStart;
	EditBoxWidget 						m_pStateWidget;
	TextWidget 							m_pAnimEventWidget;
}


// *************************************************************************************
// ! PluginDayzPlayerDebug  
// *************************************************************************************
class PluginDayzPlayerDebug extends PluginBase
{
	autoptr Timer						m_TickTimer;
	bool 								m_IsActive	= false;
	bool								m_HasFocus  = false;
	
	Widget 								m_MainWnd;
	EditBoxWidget 						m_PlayerStatusV;
	TextListboxWidget					m_ActionsSelector;
	ButtonWidget						m_ActionsStart;
	ButtonWidget						m_ActionsEnd;
	ButtonWidget						m_ActionsEnd2;
	ButtonWidget						m_ActionsCancel;
	ButtonWidget						m_ActionsOTA;
	ButtonWidget						m_ActionsOTL;
	EditBoxWidget 						m_ActionStateV;
	TextWidget 							m_ActionEventV;
	TextWidget 							m_ActionsGlobalStatus;

	ButtonWidget 						m_PlayerSyncCreateShadow;
	ButtonWidget 						m_PlayerSyncDelete;

	TextWidget 							m_ItemAccClass;
	TextWidget 							m_ItemAccSClass;
	TextWidget 							m_ItemAccInst;



	PluginDayzPlayerDebugUIHandler		m_pUIHandler;

	Entity 								m_pPlayerShadow;


	//---------------------------------------------------
    // gui stuff

	void PluginDayzPlayerDebug()
	{
		#ifndef NO_GUI
		CreateModuleWidgets();
		#endif
	}
	
	
	void ~PluginDayzPlayerDebug()
	{
		PlayerSyncDelete();

		#ifndef NO_GUI
		DestroyModuleWidgets();
		#endif		
	}

	void RegisterDebugItem(ItemBase item, PlayerBase player)
	{
	}
	
	
	bool IsActive()
	{
		return m_IsActive;
	}

	void ShowWidgets(bool show)
	{
		if(m_MainWnd) 
		{
			m_MainWnd.Show(show);
		}
	}

	//!
	void Show()
	{
		ShowWidgets(true);
		m_IsActive = true;

		if (!m_TickTimer)
		{
			m_TickTimer = new Timer();
			m_TickTimer.Run(0.1, this, "Tick", NULL, true);			
		}
	}

	//!
	void Hide()
	{
		ShowWidgets(false);
		m_IsActive = false;
			
		delete m_TickTimer;
		m_TickTimer = NULL;
	}

	//!
	void 	CaptureFocus()
	{
		if (!m_HasFocus)
		{
			GetGame().GetInput().ChangeGameFocus(1);
			GetGame().GetUIManager().ShowCursor(true);
			m_HasFocus = true;
		}
	}

	//!
	void	ReleaseFocus()
	{
		if (m_HasFocus)
		{
			GetGame().GetInput().ChangeGameFocus(-1);
			GetGame().GetUIManager().ShowCursor(false);
			m_HasFocus = false;
		}	
	}

	//!
	void ToggleDebugWindowEvent()
	{
		if(m_IsActive)
		{	
			ReleaseFocus();	
			Hide();
			
		}
		else
		{
			Show();
		}
	}


	void ToggleDebugWindowEventP()
	{
		if (m_IsActive && !m_HasFocus)
		{
			CaptureFocus();
			return;
		}
		else if (!m_IsActive)
		{
			Show();
			CaptureFocus();
			return;
		}

		ToggleDebugWindowEvent();
	}


	void CreateModuleWidgets()
	{
		if (!m_pUIHandler)
		{
			m_pUIHandler = new PluginDayzPlayerDebugUIHandler();
			m_pUIHandler.m_pPluginPlayerDebug = this;
		}

		if (!m_MainWnd)
		{
			m_MainWnd = GetGame().GetWorkspace().CreateWidgets("gui/layouts/day_z_playerdebug/day_z_playerdebug.layout");
			m_MainWnd.SetHandler(m_pUIHandler);
			m_MainWnd.Show(false);
		}
		
		m_PlayerStatusV 	= m_MainWnd.FindAnyWidget("PlayerStatusV");
		m_ActionsSelector	= m_MainWnd.FindAnyWidget("ActionsSelector");
		m_ActionsStart		= m_MainWnd.FindAnyWidget("ActionsStart");
		m_ActionsEnd		= m_MainWnd.FindAnyWidget("ActionsEnd");
		m_ActionsEnd2		= m_MainWnd.FindAnyWidget("ActionsEnd2");
		m_ActionsCancel		= m_MainWnd.FindAnyWidget("ActionsCancel");
		m_ActionsOTA			= m_MainWnd.FindAnyWidget("ActionsOTA");
		m_ActionsOTL			= m_MainWnd.FindAnyWidget("ActionsOTL");
		m_ActionStateV		= m_MainWnd.FindAnyWidget("ActionStateV");
		m_ActionEventV		= m_MainWnd.FindAnyWidget("ActionEventV");
		m_ActionsGlobalStatus	= m_MainWnd.FindAnyWidget("ActionGlobalStatus");

		m_PlayerSyncCreateShadow 	= m_MainWnd.FindAnyWidget("PlayerSyncCreate");
		m_PlayerSyncDelete 			= m_MainWnd.FindAnyWidget("PlayerSyncDelete");

		m_ItemAccClass			= m_MainWnd.FindAnyWidget("ItemAccessClass");
		m_ItemAccSClass			= m_MainWnd.FindAnyWidget("ItemAccessSClass");
		m_ItemAccInst				= m_MainWnd.FindAnyWidget("ItemAccessInst");

		ActionsInit();
		
		// m_DebugAgentListWidget = m_DebugRootWidget.FindAnyWidget("w_Agents");
		// m_DebugFloatsListWidget = m_DebugRootWidget.FindAnyWidget("w_Floats");
		// m_DebugStringsListWidget = m_DebugRootWidget.FindAnyWidget("w_Strings");
		// m_DebugFloatsProperListWidget = m_DebugRootWidget.FindAnyWidget("w_FloatsProper");
		// m_ItemPreviewWidget = m_DebugRootWidget.FindAnyWidget("w_ItemPreview");
		// m_ClassNameWidget = m_DebugRootWidget.FindAnyWidget("w_ClassName");
	}
	
	void DestroyModuleWidgets()
	{
		m_MainWnd.Destroy();
		m_MainWnd = NULL;

		delete m_pUIHandler;
		m_pUIHandler = NULL;
	}
	

	//---------------------------------------------------
    // main update

	
	void Tick()
	{
		DayZPlayer							player 	= (DayZPlayer) GetGame().GetPlayer();
		if (!player)
		{
			return;
		}
				
		// Print("PluginDayzPlayerDebug tick");	
		UpdatePlayerStatus(player);
		
		// DbgShowBoneName(player);			
		UpdateCommandModifiers(player);

		//!
		UpdateItemAccessor(player);
	}
	
	
	//---------------------------------------------------
    // Player main

	
	void UpdatePlayerStatus(DayZPlayer pPlayer)
	{
		
		autoptr HumanMovementState		state 	= new HumanMovementState();
		
		
		pPlayer.GetMovementState(state);
				
		string a = "Cmd:"; 
		
		if (state.m_CommandTypeId == DayZPlayerConstants.COMMANDID_MOVE)
		{
			a += "MOVE:";
			a += " st:" + state.m_iStanceIdx.ToString() + ", mv:" + state.m_iMovement.ToString();
		}
		else if (state.m_CommandTypeId == DayZPlayerConstants.COMMANDID_ACTION)
		{
			a += "FB ACTION:";
			a += " st:" + state.m_iStanceIdx.ToString();
		}
		else
		{
			a += "UNKNOWN";
		}
		
		m_PlayerStatusV.SetText(a);
	}


	void DbgShowBoneName(DayZPlayer pPlayer)
	{
		int 	boneIndex = pPlayer.GetBoneIndexByName("Head");

		// vector 	pos			= pPlayer.GetBonePositionMS(boneIndex);
		vector 	tm[4];
		
		pPlayer.GetBoneTransformMS(boneIndex, tm);

	

		string a = "Bone index:";
		a += boneIndex.ToString();
		a += ","; 
		a += tm.ToString();
		Print(a);
	}


	void UpdateCommandModifiers(DayZPlayer pPlayer)
	{
		Actions_UpdateGlobals(pPlayer);
	
		// int nMods = pPlayer.GetCommandModifierCount();
		// Print("Number of modifiers");
		// Print(nMods);

		/*
		for (int i = 0; i < nMods; i ++)
		{
			// int a = pPlayer.GetCommandModifierId(i);

			if (pPlayer.GetCommandModifierId(i) == DayZPlayerConstants.COMMANDID_MOD_ACTION)
			{
			}
			
			// Print(a);
			
		}
		*/

		HumanCommandActionCallback clbk = pPlayer.GetCommandModifier_Action();
		if (clbk)
		{
			Print(clbk);
		};
	}


	//---------------------------------------------------
    // Actions

	void ActionsInit()
	{
			
		m_ActionsSelector.ClearItems();
		
        //! ---------------------------------------------------------
        //! ---------------------- ACTIONS --------------------------
        //! ---------------------------------------------------------
		//! looping 
		m_ActionsSelector.AddItem("L CMD_ACTION_DRINK", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_DRINK, false), 0);
		m_ActionsSelector.AddItem("L CMD_ACTION_EAT", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_EAT, false), 0);
		m_ActionsSelector.AddItem("L CMD_ACTION_EMPTYCANISTER", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_EMPTYCANISTER, false), 0);
		m_ActionsSelector.AddItem("L CMD_ACTION_CATCHRAIN", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_CATCHRAIN, false), 0);
		m_ActionsSelector.AddItem("L CMD_ACTION_EATPILLS", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_EATPILLS, false), 0);
		m_ActionsSelector.AddItem("L CMD_ACTION_EATTABLETS", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_EATTABLETS, false), 0);
		m_ActionsSelector.AddItem("L CMD_ACTION_EMPTYBOTTLE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_EMPTYBOTTLE, false), 0);
		m_ActionsSelector.AddItem("L CMD_ACTION_OPENCOMPASS", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_OPENCOMPASS, false), 0);
        
        //! one time
		m_ActionsSelector.AddItem("O CMD_ACTION_LICKBATTERY", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_LICKBATTERY, false), 0);
		m_ActionsSelector.AddItem("O CMD_ACTION_PICKUP", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_PICKUP, false), 0);
		m_ActionsSelector.AddItem("O CMD_ACTION_LIGHTFLARE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_LIGHTFLARE, false), 0);
		m_ActionsSelector.AddItem("O CMD_ACTION_LITCHEMLIGHT", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_LITCHEMLIGHT, false), 0);
		m_ActionsSelector.AddItem("O CMD_ACTION_UNPINGRENAGE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_UNPINGRENAGE, false), 0);
		m_ActionsSelector.AddItem("O CMD_ACTION_DRINKCAN", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_DRINKCAN, false), 0);
		m_ActionsSelector.AddItem("O CMD_ACTION_OPENDOORFW", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORFW, false), 0);
		m_ActionsSelector.AddItem("O CMD_ACTION_OPENDOORBW", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_OPENDOORBW, false), 0);
		m_ActionsSelector.AddItem("O CMD_ACTION_SWIPELDOOR", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_SWIPELDOOR, false), 0);
		m_ActionsSelector.AddItem("O CMD_ACTION_SWIPERDOOR", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_SWIPERDOOR, false), 0);
		m_ActionsSelector.AddItem("O CMD_ACTION_OPENLID", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_OPENLID, false), 0);
		m_ActionsSelector.AddItem("O CMD_ACTION_CLOSELID", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_CLOSELID, false), 0);
		m_ActionsSelector.AddItem("O CMD_ACTION_GIVEL", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_GIVEL, false), 0);
		m_ActionsSelector.AddItem("O CMD_ACTION_GIVER", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONMOD_GIVER, false), 0);


		//! fullbody looping
		m_ActionsSelector.AddItem("FB L CMD_ACTION_DRINKPOND", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_DRINKPOND, true, DayZPlayerConstants.STANCEMASK_CROUCH), 0);
		m_ActionsSelector.AddItem("FB L CMD_ACTION_DRINKWELL", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_DRINKWELL, true, DayZPlayerConstants.STANCEMASK_CROUCH), 0);
		m_ActionsSelector.AddItem("FB L CMD_ACTION_FILLBOTTLEWELL", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_FILLBOTTLEWELL, DayZPlayerConstants.STANCEMASK_CROUCH, true), 0);
		m_ActionsSelector.AddItem("FB L CMD_ACTION_FIREESTINGUISHER", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_FIREESTINGUISHER, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
		m_ActionsSelector.AddItem("FB L CMD_ACTION_WRING", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_WRING, true, DayZPlayerConstants.STANCEMASK_CROUCH), 0);
		m_ActionsSelector.AddItem("FB L CMD_ACTION_BERRIES", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_BERRIES, DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT), 0);
		m_ActionsSelector.AddItem("FB L CMD_ACTION_CHOPTREE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_CHOPTREE, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
		m_ActionsSelector.AddItem("FB L CMD_ACTION_DIGSHOVEL", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_DIGSHOVEL, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
		m_ActionsSelector.AddItem("FB L CMD_ACTION_DIGHOE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_DIGHOE, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
		m_ActionsSelector.AddItem("FB L CMD_ACTION_FISHING", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_FISHING, true, DayZPlayerConstants.STANCEMASK_CROUCH ), 0);
		m_ActionsSelector.AddItem("FB L CMD_ACTION_CPR", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_CPR, true, DayZPlayerConstants.STANCEMASK_CROUCH ), 0);
		m_ActionsSelector.AddItem("FB L CMD_ACTION_BANDAGE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_BANDAGE, true, DayZPlayerConstants.STANCEMASK_CROUCH ), 0);
		m_ActionsSelector.AddItem("FB L CMD_ACTION_CRAFTING", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_CRAFTING, true, DayZPlayerConstants.STANCEMASK_CROUCH ), 0);
		m_ActionsSelector.AddItem("FB L CMD_ACTION_INTERRACTPLAYER", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_INTERRACTPLAYER, true, DayZPlayerConstants.STANCEMASK_CROUCH ), 0);
		m_ActionsSelector.AddItem("FB L CMD_ACTION_DRINKCANISTER", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_DRINKCANISTER, true), 0);
		m_ActionsSelector.AddItem("FB L CMD_ACTION_FORCEFEED", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_FORCEFEED, DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT), 0);
        m_ActionsSelector.AddItem("FB L CMD_ACTION_BANDAGETARGET", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_BANDAGETARGET, DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT), 0);
        m_ActionsSelector.AddItem("FB L CMD_ACTION_DISINFECTTARGET", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_DISINFECTTARGET, DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT), 0);
        m_ActionsSelector.AddItem("FB L CMD_ACTION_TRANSFUSETARGET", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_TRANSFUSETARGET, DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT), 0);

		//! fullbody one time
		m_ActionsSelector.AddItem("FB O CMD_ACTION_DEPLOY", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_DEPLOY, true, DayZPlayerConstants.STANCEMASK_CROUCH ), 0);
		m_ActionsSelector.AddItem("FB O CMD_ACTION_VOMIT", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_VOMIT,true, DayZPlayerConstants.STANCEMASK_CROUCH ), 0);
		m_ActionsSelector.AddItem("FB O CMD_ACTION_MORPHINE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_MORPHINE, DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT), 0);
		m_ActionsSelector.AddItem("FB O CMD_ACTION_INJECTION", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_INJECTION, DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT), 0);
		m_ActionsSelector.AddItem("FB O CMD_ACTION_STARTFIREMATCH", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_STARTFIREMATCH, DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_ERECT), 0);
		m_ActionsSelector.AddItem("FB O CMD_ACTION_WASHHANDS", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_ACTIONFB_WASHHANDS,true, DayZPlayerConstants.STANCEMASK_CROUCH ), 0);
        
        
        //! ---------------------------------------------------------
        //! ---------------------- GESTURES -------------------------
        //! ---------------------------------------------------------
		//! looping 
		m_ActionsSelector.AddItem("L CMD_GESTURE_GREETING", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_GREETING, false), 0);
		m_ActionsSelector.AddItem("L CMD_GESTURE_POINT", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_POINT, false), 0);
		m_ActionsSelector.AddItem("L CMD_GESTURE_THUMB", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_THUMB, false), 0);
		m_ActionsSelector.AddItem("L CMD_GESTURE_SILENCE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_SILENCE, false), 0);
		m_ActionsSelector.AddItem("L CMD_GESTURE_TAUNT", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_TAUNT, false), 0);
		m_ActionsSelector.AddItem("L CMD_GESTURE_TIMEOUT", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_TIMEOUT, false), 0);
		m_ActionsSelector.AddItem("L CMD_GESTURE_HEART", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_HEART, false), 0);
		m_ActionsSelector.AddItem("L CMD_GESTURE_FACEPALM", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_FACEPALM, false), 0);
		m_ActionsSelector.AddItem("L CMD_GESTURE_RPS", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_RPS, false), 0);
        
        //! one time
		m_ActionsSelector.AddItem("O CMD_GESTURE_THROAT", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_THROAT, false), 0);
		m_ActionsSelector.AddItem("O CMD_GESTURE_CLAP", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREMOD_CLAP, false), 0);
        
		//! fullbody looping
		m_ActionsSelector.AddItem("FB L CMD_GESTURE_SALUTE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_SALUTE, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
		m_ActionsSelector.AddItem("FB L CMD_GESTURE_CAMPFIRE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_CAMPFIRE, true, DayZPlayerConstants.STANCEMASK_CROUCH), 0);
		m_ActionsSelector.AddItem("FB L CMD_GESTURE_LYINGDOWN", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_LYINGDOWN, true, DayZPlayerConstants.STANCEMASK_CROUCH), 0);
		m_ActionsSelector.AddItem("FB L CMD_GESTURE_SOS", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_SOS, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
		m_ActionsSelector.AddItem("FB L CMD_GESTURE_SITA", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_SITA, true, DayZPlayerConstants.STANCEMASK_CROUCH), 0);
		m_ActionsSelector.AddItem("FB L CMD_GESTURE_SITB", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_SITB, true, DayZPlayerConstants.STANCEMASK_CROUCH), 0);
        
		//! fullbody one time
		m_ActionsSelector.AddItem("FB O CMD_GESTURE_DANCE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_GESTUREFB_DANCE, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
        
        
        //! ---------------------------------------------------------
        //! ---------------------- SUICIDE --------------------------
        //! ---------------------------------------------------------
		//! fullbody looping
		m_ActionsSelector.AddItem("FB L CMD_SUICIDE_1HD", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_SUICIDEFB_1HD, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
		m_ActionsSelector.AddItem("FB L CMD_SUICIDE_FIREAXE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_SUICIDEFB_FIREAXE, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
		m_ActionsSelector.AddItem("FB L CMD_SUICIDE_PITCHFORK", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_SUICIDEFB_PITCHFORK, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
		m_ActionsSelector.AddItem("FB L CMD_SUICIDE_PISTOL", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_SUICIDEFB_PISTOL, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
		m_ActionsSelector.AddItem("FB L CMD_SUICIDE_RIFLE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_SUICIDEFB_RIFLE, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
		m_ActionsSelector.AddItem("FB L CMD_SUICIDE_SWORD", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_SUICIDEFB_SWORD, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
		m_ActionsSelector.AddItem("FB L CMD_SUICIDE_UNARMED", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_SUICIDEFB_UNARMED, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
		m_ActionsSelector.AddItem("FB L CMD_SUICIDE_WOODAXE", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_SUICIDEFB_WOODAXE, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
		m_ActionsSelector.AddItem("FB L CMD_SUICIDE_SPEAR", new PluginDayzPlayerDebugUserData(DayZPlayerConstants.CMD_SUICIDEFB_SPEAR, true, DayZPlayerConstants.STANCEMASK_ERECT), 0);
        
	}


	//!
	void	Actions_Start()
	{
		int		row = m_ActionsSelector.GetSelectedRow();
		if (row == -1)
		{
			return;
		}

		//!
		DayZPlayer							player 	= (DayZPlayer)GetGame().GetPlayer();
		if (!player)
		{
			return;
		}

		PluginDayzPlayerDebugUserData	userData;
		m_ActionsSelector.GetItemData(row, 0, userData);

		//! start action 
		PluginDayzPlayerActionCallback a;
		
		if (userData.IsFullBody())
		{
			a = player.StartCommand_Action(userData.GetInt(), PluginDayzPlayerActionCallback, userData.GetStanceMask());
		}
		else
		{
			a = player.AddCommandModifier_Action(userData.GetInt(), PluginDayzPlayerActionCallback);
		}
		a.m_pStateWidget 		= m_ActionStateV;
		a.m_pAnimEventWidget	= m_ActionEventV;
	}

	//!
	void	Actions_DoInternalCommand(int pCommandId)
	{
		//!
		DayZPlayer							player 	= (DayZPlayer)GetGame().GetPlayer();
		if (!player)
		{
			return;
		}

		HumanCommandActionCallback clbk = player.GetCommand_Action();
		if (!clbk)
		{
			clbk = player.GetCommandModifier_Action();
			if (!clbk)
			{
				return;
			}
		}
		
		clbk.InternalCommand(pCommandId);		
	}

	//!
	void	Actions_DoCancel()
	{
		//!
		DayZPlayer							player 	= (DayZPlayer)GetGame().GetPlayer();
		if (!player)
		{
			return;
		}

		HumanCommandActionCallback clbk = player.GetCommand_Action();
		if (!clbk)
		{
			clbk = player.GetCommandModifier_Action();
			if (!clbk)
			{
				return;
			}
		}
		clbk.Cancel();			
	}


	//! 
	void 	Actions_UpdateGlobals(DayZPlayer pPlayer)
	{
		HumanCommandActionCallback clbk = pPlayer.GetCommandModifier_Action();
		if (!clbk)
		{
			m_ActionsGlobalStatus.SetText("No Action");
		}

		m_ActionsGlobalStatus.SetText(clbk.ToString());
	}


        
    //! ---------------------------------------------------------


	void 	PlayerSyncCreate()
	{
		PlayerSyncDelete();

		vector item_position = 	GetGame().GetPlayer().GetPosition() + ( GetGame().GetPlayer().GetDirection() * 2.0);

		m_pPlayerShadow = GetGame().CreateObject("SurvivorENFANIMSYS", item_position, false, false);

		DayZPlayer 	pl = ((DayZPlayer)GetGame().GetPlayer());		
		pl.DebugSyncShadowSetup(m_pPlayerShadow);
	}

	void 	PlayerSyncDelete()
	{
		if (m_pPlayerShadow)
		{
			GetGame().ObjectDelete(m_pPlayerShadow);
			m_pPlayerShadow = NULL;
		}

		DayZPlayer 	pl = ((DayZPlayer)GetGame().GetPlayer());		
		if (pl)
		{
			pl.DebugSyncShadowSetup(NULL);
		}

	}


	//---------------------------------------------------
	// item accessor

	void UpdateItemAccessor(DayZPlayer pPlayer)
	{
		m_ItemAccClass.SetText(pPlayer.DebugGetItemClass());
		m_ItemAccSClass.SetText(pPlayer.DebugGetItemSuperClass());
		m_ItemAccInst.SetText(pPlayer.DebugGetItemAnimInstance());
	}


	//---------------------------------------------------
    // window ui clicks

	//! buttons clicks
	bool	OnClick(Widget w, int x, int y, int button)
	{
		if (w == m_ActionsStart)
		{
			Print("PluginPlayerDebug: Action Start");
			Actions_Start();
			return true;
		}
		else if (w == m_ActionsEnd)
		{
			Print("PluginPlayerDebug: Action End");
			Actions_DoInternalCommand(DayZPlayerConstants.CMD_ACTIONINT_END);
			return true;
		}
		else if (w == m_ActionsEnd2)
		{
			Print("PluginPlayerDebug: Action End 2");
			Actions_DoInternalCommand(DayZPlayerConstants.CMD_ACTIONINT_END2);
			return true;
		}
		else if (w == m_ActionsOTA)
		{
			Print("PluginPlayerDebug: Action One Time Action");
			Actions_DoInternalCommand(DayZPlayerConstants.CMD_ACTIONINT_ACTION);
			return true;
		}
		else if (w == m_ActionsOTL)
		{
			Print("PluginPlayerDebug: Action Loop");
			Actions_DoInternalCommand(DayZPlayerConstants.CMD_ACTIONINT_ACTIONLOOP);
			return true;
		}
		else if (w == m_ActionsCancel)
		{
			Print("PluginPlayerDebug: Action Cancel");
			Actions_DoCancel();
			return true;
		}
		else if (w == m_PlayerSyncCreateShadow)
		{
			PlayerSyncCreate();
			return true;
		}
		else if (w == m_PlayerSyncDelete)
		{
			PlayerSyncDelete();
			return true;
		}
		
		return false;
	}

			
}
