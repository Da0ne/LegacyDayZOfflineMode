class ActionManager
{
	autoptr array<ActionBase>			m_Actions;
	autoptr map<int, ActionBase>		m_ActionsMap;		
	autoptr TIntArray					m_PlayerInteractActions;
	autoptr ActionConstructor			m_ActionConstructor;
	PlayerBase							m_Player;
	autoptr Param2<int,int> 			m_UserActionParams;
	autoptr HumanCommandActionCallback	m_Command;
	
	ActionBase							m_CurrentPrimaryAction;
	ActionBase							m_CurrentSecondaryAction;
	ActionBase							m_CurrentTertiaryAction;
	string 								m_CurrentTertiaryActionText;
	int 								m_CurrentPrimaryActionID;
	int									m_CurrentSecondaryActionID; 
	int 								m_CurrentTertiaryActionID;
	int 								m_LastPrimaryActionID;
	int									m_LastSecondaryActionID; 
	int 								m_LastTertiaryActionID;	
	int									m_InjectedActionID;
	bool 								m_ActionSet;
	Object 								m_CurrentTarget;
	ItemBase							m_CurrentItemInHands;
	Object								m_InjectedActionTarget;
	
	const int 							AT_CRAFT = 179;
	const int 							AT_WORLD_CRAFT= 181;
	
	void ActionManager(PlayerBase player)
	{
		m_Player = player;
		if ( m_Player )
		{
			m_Actions = new array<ActionBase>;
			m_PlayerInteractActions = new TIntArray;
			m_ActionsMap = new map<int, ActionBase>;
			m_ActionConstructor = new ActionConstructor;
			m_InjectedActionID = 0;
			
			if( GetGame().IsServer() )
			{
				m_UserActionParams = new Param2<int,int>(0,0);
			}
			m_ActionConstructor.ConstructActions(m_Actions,m_PlayerInteractActions);
			for (int i = 0; i < m_Actions.Count(); i++)
			{	
				m_Actions.Get(i).CreateConditionComponents();
				m_ActionsMap.Set(m_Actions.Get(i).GetType(), m_Actions.Get(i));
			}
		}
	}

	void ~ActionManager()
	{
		for (int i = 0; i < m_Actions.Count(); i++)
		{
			delete m_Actions.Get(i);
		}
	}

	ActionBase GetAction(int type)
	{		
		return m_ActionsMap.Get(type);
	}

	TIntArray GetPlayerActions()
	{
		return m_PlayerInteractActions;
	}
	
		
	void ActionSetter()
	{
		GetUserActions();
		
		// check placing	
		if ( m_Player.IsPlacingObject() ) 
		{
			if ( m_Player.GetItemInHands() == NULL || m_Player.GetItemInHands() != m_Player.GetHologram().GetParentObject() )
			{
				GetGame().ObjectDelete ( m_Player.GetHologram().GetProjectionObject() );
			}
			return true; 
		}
	}
	
	void InjectAction(int action_id, Object action_target = NULL )
	{
		m_InjectedActionID = action_id;
		m_InjectedActionTarget = action_target;
	}
	
	void EjectAction()
	{
		m_InjectedActionID = 0;
		m_InjectedActionTarget = NULL;
	}

	//suitable action pick------------------------------
	void GetUserActions()
	{
		if ( m_Player && !m_Player.GetCommandModifier_Action() && !m_Player.GetCommand_Action() ) 
		{
			ClearUserActions();
			autoptr TIntArray primary_action_ids = new TIntArray;
			autoptr TIntArray secondary_action_ids = new TIntArray;
			autoptr TIntArray tertiary_action_ids = new TIntArray;
			bool is_target_free = true;
			ActionBase picked_action;
			
			// Gather inputs
			if ( m_Player.GetItemInHands() && m_Player.GetItemInHands().IsItemBase() ) m_CurrentItemInHands = m_Player.GetItemInHands();
			if ( m_Player.GetCursorObject() && ( m_Player.GetCursorObject().IsEntityAI() || m_Player.GetCursorObject().IsWoodBase() || m_Player.GetCursorObject().IsRock() ) ) //jtomasik - ty ORy by tu nemely byt, ale obejcty z managed prestavly vracet IsEntityAI jako true
			{
				m_CurrentTarget = m_Player.GetCursorObject();
				if ( m_Player.m_ModuleObjectsInteractionManager ) is_target_free = m_Player.m_ModuleObjectsInteractionManager.IsFree(m_CurrentTarget);				
			}
			else
			{
				m_CurrentTarget = NULL;
			}
			// Overrides contextual actions
			if ( m_InjectedActionID != 0 )
			{
				m_CurrentPrimaryActionID = m_InjectedActionID;
				picked_action = GetAction(m_CurrentPrimaryActionID);
				if ( m_InjectedActionTarget ) m_CurrentTarget = m_InjectedActionTarget;
				if ( picked_action && picked_action.Can(m_Player, m_CurrentTarget, m_CurrentItemInHands) ) 
				{
					m_CurrentPrimaryAction = picked_action;
					m_UserActionParams.param1 =  m_CurrentPrimaryActionID;
				}
			}
			else
			{	
				if ( is_target_free )
				{
					// if there is item in hand, check its actions
					if ( m_CurrentItemInHands )
					{	
						// Continuous actions get
						if ( primary_action_ids )
						{
							g_Game.ConfigGetIntArray("cfgVehicles " + m_CurrentItemInHands.GetType() + " ContinuousActions", primary_action_ids);	
							if ( primary_action_ids.Count() == 0 ) g_Game.ConfigGetIntArray("cfgMagazines " + m_CurrentItemInHands.GetType() + " ContinuousActions", primary_action_ids); //jtomasik - a tohle bys se melo dodelat i pro zbrane, vehicles a dalsi config classy	
							primary_action_ids.InsertAt(AT_WORLD_CRAFT,0); // adds default crafting interaction check
							// Continuous action set
							if ( primary_action_ids && primary_action_ids.Count() > 0  )
							{
								for ( int i = 0; i < primary_action_ids.Count(); i++ )
								{
									m_CurrentPrimaryActionID = primary_action_ids.Get(i);
									picked_action = GetAction(m_CurrentPrimaryActionID);
									if ( picked_action && picked_action.Can(m_Player,m_CurrentTarget, m_CurrentItemInHands) ) 
									{
										m_CurrentPrimaryAction = picked_action;
										m_UserActionParams.param1 =  m_CurrentPrimaryActionID;
										break;
									}
								}
							}
							
						}
						// Single use actions get
						if ( secondary_action_ids )
						{
							g_Game.ConfigGetIntArray("cfgVehicles " +m_CurrentItemInHands.GetType() + " SingleUseActions", secondary_action_ids);	
							// Single use action set
							if ( secondary_action_ids  && secondary_action_ids.Count() > 0 )
							{
								for ( i = 0; i < secondary_action_ids.Count(); i++ )
								{
									m_CurrentSecondaryActionID = secondary_action_ids.Get(i);
									picked_action = GetAction(m_CurrentSecondaryActionID);		
									if ( picked_action && picked_action.Can(m_Player, m_CurrentTarget, m_CurrentItemInHands) )
									{
										m_CurrentSecondaryAction = picked_action;
										m_UserActionParams.param2 =  m_CurrentSecondaryActionID;
										break;
									}
								}
							}
						}
					}
					//check Interact actions get
					if ( tertiary_action_ids )
					{
						if ( m_CurrentTarget )
						{
							// Adding interact actions of items in the world
							g_Game.ConfigGetIntArray("cfgVehicles " + m_CurrentTarget.GetType() + " InteractActions", tertiary_action_ids);			
						}
						// Adding default interact actions of player
						if ( GetPlayerActions().Count() > 0 )
						{
							for ( i = 0; i < GetPlayerActions().Count(); i++ )
							{
								int uauid = GetPlayerActions().Get(i);
								tertiary_action_ids.Insert(uauid);
							}
						}	
						// Interact action set
						if ( tertiary_action_ids.Count() > 0 )
						{
							for ( i = 0; i < tertiary_action_ids.Count(); i++ )
							{
								m_CurrentTertiaryActionID = tertiary_action_ids.Get(i);
								picked_action = GetAction(m_CurrentTertiaryActionID);
								if ( picked_action && picked_action.Can(m_Player, m_CurrentTarget, NULL) )
								{
									m_CurrentTertiaryAction = picked_action;
									//m_UserActionParams.param3 =  m_CurrentTertiaryActionID;
									m_CurrentTertiaryActionText = picked_action.GetText();
									break;
								}
							}
						}
					}
				}
			}
		}
		//SYNC ACTIONS
		if ( m_CurrentPrimaryAction || m_CurrentSecondaryAction || m_CurrentTertiaryAction )
		{
			if ( m_LastPrimaryActionID != m_CurrentPrimaryActionID || m_LastSecondaryActionID != m_CurrentSecondaryActionID ||  m_LastTertiaryActionID != m_CurrentTertiaryActionID )
			{
				m_LastPrimaryActionID = m_CurrentPrimaryActionID;
				m_LastSecondaryActionID = m_CurrentSecondaryActionID;
				m_LastTertiaryActionID = m_CurrentTertiaryActionID;
				GetGame().RPCSingleParam(m_Player, RPC_USER_ACTION_SYNCH, m_UserActionParams, m_Player);
			}
		}
	}

	void ClearUserActions()
	{
		if ( m_CurrentPrimaryAction )
		{ 
			m_CurrentPrimaryAction = NULL;
		}
		if ( m_CurrentSecondaryAction ) 
		{
			m_CurrentSecondaryAction = NULL;
		}
		if ( m_CurrentTertiaryAction )
		{ 
			m_CurrentTertiaryAction = NULL;
		}
		if ( m_UserActionParams )
		{
			m_UserActionParams.param1 = 0;
			m_UserActionParams.param2 = 0;
		//m_UserActionParams.param3 = 0;
		}
	}
	
	bool IsSprinting() //jtomasik - Actions cannot be performed while sprinting, later should be parameter of action itself
	{
		autoptr HumanMovementState	state = new HumanMovementState();
		m_Player.GetMovementState(state);
		if (state.m_iMovement == 3) return true;	//  jtomasik - DayZPlayerConstants.MOVEMENT_SPRINT ?
		return false;
	}
	
	// Interact----------------------------------------------------
	
	void OnGetActions (ActionMenuContext ctx)
	{  	
		if ( m_CurrentTertiaryAction && m_CurrentTertiaryActionText ) ctx.AddAction("Press MMB to "+m_CurrentTertiaryActionText, 109, NULL, 1000, true, false);
	}
	
	void OnAction() //Interact
	{
		if ( !m_Player.GetCommandModifier_Action() && !m_Player.GetCommand_Action() && !IsSprinting() )
		{
			if ( m_CurrentTertiaryAction && ( m_CurrentTarget && m_CurrentTarget == m_Player.GetCursorObject() || !m_CurrentTarget ) )
			{
				m_CurrentTertiaryAction.Start(m_Player, m_CurrentTarget, NULL);
			}
		}
	}

	// Continuous----------------------------------------------------
	void OnContinuousStart()
	{
		if ( !m_Player.GetCommandModifier_Action() && !m_Player.GetCommand_Action() && !IsSprinting() )
		{
			if ( m_CurrentPrimaryAction && m_CurrentItemInHands && m_CurrentItemInHands == m_Player.GetItemInHands() && ( m_CurrentTarget && m_CurrentTarget == m_Player.GetCursorObject() || !m_CurrentTarget || m_InjectedActionTarget) ) 
			{
				/*
				PrintString("UADBG Player: " + ToString(m_Player));
				PrintString("UADBG Target: " + ToString(m_CurrentTarget) + " act: " + ToString(m_Player.GetCursorObject()));
				PrintString("UADBG Item: " + ToString(m_CurrentItemInHands) + " act: " + ToString(m_Player.GetItemInHands()));
				*/
				m_CurrentPrimaryAction.Start(m_Player, m_CurrentTarget, m_CurrentItemInHands);
			}
		}
	}
	
	
	void OnContinuousCancel()
	{
	//	Print("Actionmanager.c | OnContinuousCancel | called");
	
		ActionBaseCB callback = m_Player.GetCommandModifier_Action();
		if (!callback)
		{
			callback = m_Player.GetCommand_Action();
			if ( !callback ) return;
			if ( !callback.IsUserActionCallback() ) return;
			if ( callback.GetState() == callback.STATE_LOOP_END || callback.GetState() == callback.STATE_LOOP_END2 ) return;
		}
		callback.EndAction(true);
	}
	
	//Single----------------------------------------------------
	void OnSingleUse()
	{
		if ( !m_Player.GetCommandModifier_Action() && !m_Player.GetCommand_Action() && !IsSprinting() )
		{
			if ( m_CurrentSecondaryAction && m_CurrentItemInHands && m_CurrentItemInHands == m_Player.GetItemInHands() && ( m_CurrentTarget && m_CurrentTarget == m_Player.GetCursorObject() || !m_CurrentTarget ) )
			{
				/*PrintString("UADBG Player: " + ToString(m_Player));
				PrintString("UADBG Target: " + ToString(m_CurrentTarget) + " act: " + ToString(m_Player.GetCursorObject()));
				PrintString("UADBG Item: " + ToString(m_CurrentItemInHands) + " act: " + ToString(m_Player.GetItemInHands()));
				*/
				m_CurrentSecondaryAction.Start(m_Player, m_CurrentTarget, m_CurrentItemInHands);	
			}
		}		
	}
};
