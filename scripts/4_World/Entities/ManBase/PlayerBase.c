class AnimalBase;

class PlayerBase extends ManBase
{
	bool m_ToggleSpecialActions = false;
	bool m_FreezCam = false;
	//---
	private int m_LastShavedSeconds;
	private int m_CachedPlaytime;
	private bool m_HasBloodyHands;

	private PluginExperience	m_ModuleExperience;
	private PluginPlayerStatus	m_ModulePlayerStatus;
	PluginObjectsInteractionManager m_ModuleObjectsInteractionManager;
	
	autoptr PlayerStats 		m_PlayerStats;
	PluginRecipesManager 		m_ModuleRecipesManager;
	autoptr ModifiersManager	m_ModifiersManager;
	autoptr NotifiersManager 	m_NotifiersManager = NULL;
	autoptr ActionManager	 	m_ActionManager;
	autoptr PlayerAgentPool		m_AgentPool;
	autoptr Environment			m_Environment;
	autoptr StateManager		m_StateManager;
	autoptr DisplayStatus 		m_DisplayStatus = NULL;
	autoptr StaminaHandler		m_StaminaHandler;
	autoptr StanceIndicator		m_StanceIndicator;
	const int 					BLEEDING_TRESHOLD = 100;
	const int 					SHOCK_TRESHOLD = 1000;
	const int 					OVERLOAD_LIMIT = 30000;
	float 						m_CargoLoad;
	autoptr Param2<int,int> 	m_UAParam;
	autoptr Param2<float,float>	m_StaminaParam;
	autoptr Param1<string> 		m_UAParamMessage;
	autoptr StoneGenerator 		m_StoneGenerator; 
	
	
	//hologram
	Hologram m_Hologram = NULL;
	
	private autoptr map<int, float> m_Skills;		// int - skill id, int - skill experience points

	// Temporary functions
	// NTF_COUNT
	Param3<string, int, bool> m_MyNotifiers[NTF_COUNT];
	
	//Sheduler
	float m_LastTick;
	
	//crafting
	int m_RecipeID = -1;
	EntityAI m_Item1;
	EntityAI m_Item2;
	bool m_Crafting;
	vector m_CraftingInitialPos;
	
	autoptr map<int,string> m_Recipes;

	Camera m_Cam;
	//---------
	protected float CAM_SPEED = 5.0;
	protected float MAX_CAM_SPEED = 50.0;
	protected float CAM_BOOST = 10.0;
	protected float CAM_MOVE_DRAG = 0.7;
	protected float CAM_MOUSE_SENSE = 0.4;
	protected float CAM_SMOOTHNESS = 0.8;
	protected float CAM_FOV = 1.0;	
	protected vector localVelocity;
	protected float  localyawVelocity;
	protected float  localfovVelocity = 1.0;
	protected float  localpitchVelocity;
	protected float  local_camspeed;
	//--------

	void CreateFreeCamInstance(Camera _cam)
	{
		m_Cam = _cam;
		m_Cam.SetFOVRange(0.005, 8.0);
		m_Cam.SetFOV(CAM_FOV);
		m_Cam.Commit(0);
	}

	override void EOnFrame( IEntity other, float timeSlice )
	{
		if (!m_Cam || GetGame().GetPlayer() != this)
			return;

		if (m_FreezCam)
			return;

		if (GetGame().GetMission() && ((MissionGameplay)GetGame().GetMission()).IsPaused())
			return;

		vector OldOrientation = m_Cam.GetOrientation();
		vector oldPos    = m_Cam.GetPosition();
		vector oldDir    = m_Cam.GetDirection();

		Input input = GetGame().GetInput();
		
		float forward  = input.GetAction(UAMoveForward, false) - input.GetAction(UAMoveBack, false);
		float strafe   = input.GetAction(3, false ) - input.GetAction(2, false);
		float altitude = input.GetAction(UALeanLeft, false ) - input.GetAction(14, false );
		float yawDiff = input.GetAction(UAAimLeft, false ) - input.GetAction(UAAimRight, false);
		float pitchDiff = input.GetAction(UAAimDown, false ) - input.GetAction(UAAimUp, false);
		float speedInc = input.GetAction(42, false) - input.GetAction(43,false);
		bool increaseSpeeds = input.GetAction(UATurbo, false);

		//Keyboard Control
		local_camspeed = CAM_SPEED;
		if ( CAM_BOOST > 0 )
		{
			CAM_SPEED = CAM_SPEED + ( CAM_SPEED * speedInc / CAM_BOOST );
			
			if ( CAM_SPEED < 0.01 ) {
				CAM_SPEED = 0.01;
			}
			
			local_camspeed = CAM_SPEED;

			if( increaseSpeeds )
				local_camspeed = local_camspeed * CAM_BOOST;
		}
		
		if (local_camspeed >= MAX_CAM_SPEED && !increaseSpeeds)
			CAM_SPEED = MAX_CAM_SPEED;
		
		vector up = vector.Up;
		vector direction = m_Cam.GetDirection();
		vector directionAside = vector.Up * direction;
		
		up = up * altitude * local_camspeed;
		direction = direction * forward * local_camspeed;
		directionAside = directionAside * strafe * local_camspeed;

		localVelocity = localVelocity * CAM_MOVE_DRAG;
		localVelocity = localVelocity + (direction + directionAside + up) * timeSlice;

		vector newPos = oldPos + localVelocity;

		float surfaceY = GetGame().SurfaceY( newPos[0], newPos[2] ) + 0.15; //0.25
		if ( newPos[1] < surfaceY )
			newPos[1] = surfaceY;

		//FOV
		if (input.GetActionDown(107, false, false))
		{
			CAM_MOUSE_SENSE = 0.01;
			float camFov = input.GetAction(34, false) - input.GetAction(59, false);
			float fovVelocity =  (fovVelocity + camFov * 0.01);

			CAM_FOV += fovVelocity;
			if (CAM_FOV > 8.0)
				CAM_FOV = 0.005;
			else if (CAM_FOV < 0.005)
				CAM_FOV = 1.0;

			m_Cam.SetFOV(CAM_FOV);
			m_Cam.Commit(0);
		}else{
			CAM_MOUSE_SENSE = 0.4;
		}

		//Mouse Control
		localyawVelocity =  (localyawVelocity + yawDiff * CAM_MOUSE_SENSE);
		localpitchVelocity = (localpitchVelocity + pitchDiff * CAM_MOUSE_SENSE);

		vector OrientNew = OldOrientation;
		OrientNew[0] = OldOrientation[0] - Math.RAD2DEG * localyawVelocity * timeSlice;
		OrientNew[1] = OldOrientation[1] - Math.RAD2DEG * localpitchVelocity * timeSlice;

		localyawVelocity   *= CAM_SMOOTHNESS;
		localpitchVelocity *= CAM_SMOOTHNESS;

		if( OrientNew[1] < -89 )
			OrientNew[1] = -89;
			
		if( OrientNew[1] > 89 )
			OrientNew[1] = 89;
			
		//Update Position & rotation
		m_Cam.SetPosition(newPos);
		m_Cam.SetOrientation(OrientNew);
	}

	void PlayerBase()
	{		
		//Print("PSOVIS: new player");

		SetEventMask(EntityEvent.INIT|EntityEvent.FRAME);
		//m_Agents = new array<int>; //transmission agents
		m_Crafting = false;
		m_Recipes = new map<int,string>;
		m_LastShavedSeconds = 0;
		m_CachedPlaytime = 0;
		m_HasBloodyHands = false;
		m_Skills = new map<int, float>;
		m_CargoLoad = 0;
		//PrintString( "isServer " + GetGame().IsServer().ToString() );
		//PrintString( "isClient " + GetGame().IsClient().ToString() );

		if ( GetDayZGame().IsDebug() )
		{
			if ( GetDayZGame().IsNewPlayer() )
			{
				m_UAParamMessage = new Param1<string>("");
				m_UAParam = new Param2<int,int>(0,0);
				m_DisplayStatus = new DisplayStatus(this);
				m_ModuleRecipesManager = (PluginRecipesManager)GetPlugin(PluginRecipesManager);
				m_ModuleObjectsInteractionManager = (PluginObjectsInteractionManager)GetPlugin(PluginObjectsInteractionManager);
				m_ActionManager = new ActionManager(this);//handler of user actions - instancuju ho i na clientu abych mel odkud tahat popisky akce pro ui, mohl bych si posilat samotne texty a nemit je tu vubec, ale bude to pak vetsi traffic
				m_StateManager = new StateManager(this); // state manager
				m_StaminaHandler = new StaminaHandler(this);//current stamina calculation
				/*if( !GetGame().IsServer() ) - jtomasik - pak to odcommentuj*/ m_StanceIndicator = new StanceIndicator(this);//gets current players stance and communicate it to HUD
				
				if( GetGame().IsServer() )
				{
					m_PlayerStats = new PlayerStats(this);//player stats
					m_Environment = new Environment(this);//environment effects on player
					m_ModifiersManager = new ModifiersManager(this); // player modifiers 
					m_NotifiersManager = new NotifiersManager(this); // player notifiers 
					m_AgentPool = new PlayerAgentPool(this); // agent pool manager
					m_StoneGenerator = new StoneGenerator(this);//generaters stones around player
					//m_ModifiersManager = new ModifiersManager(this); // player modifiers 
				}
			}
			
			PluginConfigDebugProfileFixed m_ConfigDebugProfileFixed = (PluginConfigDebugProfileFixed)GetPlugin(PluginConfigDebugProfileFixed);
			PluginConfigDebugProfile m_ConfigDebugProfile = (PluginConfigDebugProfile)GetPlugin(PluginConfigDebugProfile);
			PluginDeveloper m_Developer = (PluginDeveloper)GetPlugin(PluginDeveloper);
			PlayerBase player = this;
				
			string default_preset = m_ConfigDebugProfile.GetDefaultPreset();
			if ( player && default_preset != "" )
			{
				TStringArray preset_array = new TStringArray;
				m_ConfigDebugProfileFixed.GetPresetItems( default_preset, preset_array );
				
				bool is_preset_fixed = true;
				if ( preset_array.Count() == 0 )
				{
					m_ConfigDebugProfile.GetPresetItems( default_preset, preset_array );
					is_preset_fixed = false;
				}
				
				if ( preset_array.Count() > 0 )
				{
					m_Developer.ClearInventory(player);
					
					for (int i = 0; i < preset_array.Count(); i++)
					{
						int damage = -1;
						int quantity = -1;
						if ( is_preset_fixed )
						{
							damage = m_ConfigDebugProfileFixed.GetItemQuantity( default_preset, i );
							quantity = m_ConfigDebugProfileFixed.GetItemQuantity( default_preset, i );
						}
						else
						{
							damage = m_ConfigDebugProfile.GetItemQuantity( default_preset, i );
							quantity = m_ConfigDebugProfile.GetItemQuantity( default_preset, i );
						}
						m_Developer.SpawnItem(player, preset_array.Get(i), SPAWNTYPE_INVENTORY, damage, quantity );
					}
				}
				delete preset_array;
			}
		}
		
		m_ModuleExperience		= (PluginExperience)GetPlugin(PluginExperience);
		m_ModulePlayerStatus	= (PluginExperience)GetPlugin(PluginPlayerStatus);

		if ( GetGame().IsServer() )
		{
			PlayerNotifiersInit();
		}
		
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(this, "OnPlayerLoaded");
	}
	
	//--------------------------------------------------------------------------
	// PLAYER DAMAGE EVENT HANDLING
	//--------------------------------------------------------------------------
	void EEOnSetCaptive(bool captive)
	{
		if ( GetGame().IsMultiplayer() && GetGame().IsServer() && captive )
		{
			GetGame().RPCSingleParam(this, RPC_ON_SET_CAPTIVE, NULL, this);
		}
	}

	void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, string component, string ammo, vector modelPos)
	{
		PrintString("PLayerBase -> EEHitBy");
		
		DamagePlayer(damageResult, source);
		PrintString(component);
		PrintString(modelPos.ToString());
	}

	void DamagePlayer(TotalDamageResult damageResult, EntityAI source)
	{
		if ( damageResult.GetDamage("","Blood") > BLEEDING_TRESHOLD )
		{
			Print("server BLEEDING");
			if ( m_ModifiersManager ) m_ModifiersManager.ActivateModifier(MDF_BLEEDING);
		}
		
		if ( source.IsMan() )
		{
			ManBase man = source;
			ItemBase item = man.GetEntityInHands();
			
			if ( item  &&  item.HasEnergyManager() )
			{
				// Prototyping of energy based weapons. 
				
				bool energy_consumed = item.GetCompEM().ConsumeEnergy( item.GetCompEM().GetEnergyUsage() );
				
				if (energy_consumed)
				{
					if ( item.KindOf("EN5C_CattleProd") )
					{
						DecreaseHealth("GlobalHealth","Shock",2000);
					}
					if ( item.KindOf("EN5C_StunBaton") )
					{
						DecreaseHealth("GlobalHealth","Shock",4000);
					}
					if ( item.KindOf("EN5C_Chainsaw") )
					{
						DecreaseHealth("GlobalHealth","Shock",1000);
						DecreaseHealth("GlobalHealth","Blood",1000);
						DecreaseHealth("GlobalHealth","Health",500);
					}
				}
			}
		}
	}


	EntityAI FindCargoByBaseType(string searched_item)
	{
		EntityAI attachment;
		string item_name;
		int attcount = this.AttachmentsCount();
		
		for (int att = 0; att < attcount; att++)
		{	
			attachment = this.GetAttachmentFromIndex(att);
			if ( attachment.IsItemBase() )
			{
				item_name = attachment.GetType();
				if ( GetGame().IsKindOf(item_name, searched_item) )
				{
					return attachment;
				}
			}
		}
		return NULL;
	}

	//---------------------------------------------------------------------------------------------------------------------------
	void OnPlayerLoaded()
	{
		if ( GetGame().IsDebug() )
		{
			if ( !GetGame().IsMultiplayer()  &&  GetGame().GetPlayer()  &&  GetGame().GetPlayer().GetID() == this.GetID() )
			{
				PluginSceneManager scene_editor = GetPlugin(PluginSceneManager);
				scene_editor.InitLoad();
			}
		}
	}

	//Placing 
	
	//get hologram
	Hologram GetHologram()
	{
		return m_Hologram;
	}

	void PlacingStart( string obj_name )
	{
		// Server side
		if ( GetGame().IsMultiplayer() && GetGame().IsServer())
		{
			RPCSingleParam(RPC_PLACING_START, NULL, this);
		}
			
		// Server/Client side set hologram
		if( GetItemInHands().GetType() == obj_name )
		{
			m_Hologram = new Hologram( GetItemInHands().GetType(), this);
			m_Hologram.GetProjectionObject().OnHologramItemCreated();
		}
		else
		{
			m_Hologram = new Hologram( obj_name, this);
			m_Hologram.GetProjectionObject().OnHologramItemCreated();
		}
	}
	
	void PlacingStop()
	{
		// Server side
		if ( GetGame().IsMultiplayer() && GetGame().IsServer())
		{
			RPCSingleParam(RPC_PLACING_STOP, NULL, this);
		}
		
		// Server/Client side
		delete m_Hologram;
	}
	
	void PlacingFinish( ItemBase item )
	{
		// Server side
		if ( GetGame().IsMultiplayer() && GetGame().IsServer())
		{
			m_Hologram.Update();
 
			if ( m_Hologram.IsColliding() ) 
			{
				PrintString("IsColliding: "+ m_Hologram.IsColliding().ToString());	
				Print("Placing of object failed because object is colliding with environment.");
				return;	
			}
			else
			{
				m_Hologram.PlaceObject( item, this );
				RPCSingleParam( RPC_PLACING_FINISH, NULL, this );
				Print("Item Placed.");
			}
		}
		// local debug version, no multiplayer
		else if( !GetGame().IsMultiplayer() )
		{
			if ( m_Hologram.IsColliding() ) 
			{
				PrintString("IsColliding: "+ m_Hologram.IsColliding().ToString());	
				Print("Placing of object failed because object is colliding with environment.");
				return;	
			}
			else
			{
				m_Hologram.PlaceObject(item, this );
				Print("Item Placed.");
			}
		}

		// Server/Client side
			delete m_Hologram;

		item.OnPlacementFinished( this );
	}

	//Delete
	void TogglePlacingFromPlayer()
	{
		if ( m_ActionManager ) 
		{
			ActionTogglePlaceObject toggle_action = (ActionTogglePlaceObject) m_ActionManager.GetAction ( AT_TOGGLE_PLACE_OBJECT );
			toggle_action.TogglePlacing ( this, NULL, GetItemInHands() );
		}
	}

	//hologram is being placed
	bool IsPlacingObject()
	{
		if ( m_Hologram != NULL )
		{
			return true;
		}
		else
		{
			return false;			
		}
	}


	void EEOnWaterEnter()
	{
		if ( GetDayZGame().IsNewPlayer() && GetGame().IsServer() )
		{
			m_Environment.ProcessItemsWetness();
		}
	}



	// -------------------------------------------------------------------------
	void ~PlayerBase()
	{
		PlayerNotifiersDelete();
	}
	
	StateManager GetStateManager()
	{
		if( m_StateManager ) return m_StateManager;
		else return NULL;
	}
	DisplayStatus GetDisplayStatus()
	{
		if(m_DisplayStatus) return m_DisplayStatus;
		else return NULL;
	}

	void SetCrafting(bool state)
	{
		m_Crafting = state;
		if ( m_ActionManager ) 
		{
			if (state) 
			{
				m_ActionManager.InjectAction(AT_CRAFT);
			}
			else
			{
				m_ActionManager.EjectAction();
			}
		}
	}
	// -------------------------------------------------------------------------
	bool  IsCraftingSetUp()
	{
		return m_Crafting;
	}
	// -------------------------------------------------------------------------
	void  PerformRecipe(int id, EntityAI item1, EntityAI item2)
	{
		/*
		Debug.Log("Ingredient1: "+ToString(m_Item1.Ptr().GetType()),"recipes");
		Debug.Log("Ingredient2: "+ToString(m_Item2.Ptr().GetType()),"recipes");
		*/
		
		Debug.Log("PerformRecipe called on player: "+ string.ToString(this),"recipes");
		
		if ( m_ModuleRecipesManager )
		{
			if( !item1 || !item2 )
			{
				Debug.Log("PerformRecipe: At least one of the object links is now null, not performing the recipe","recipes");
				return;
			}
			m_ModuleRecipesManager.PerformRecipeServer(id, item1,item2,this);
			SetCrafting(false);
		}
	}
	// -------------------------------------------------------------------------
	float GetCraftingSetUpDistance()
	{
		return Math.AbsFloat( vector.Distance( m_CraftingInitialPos, GetPosition() ) );
	}
	// -------------------------------------------------------------------------
	void SetUpCrafting( int id, EntityAI item1, EntityAI item2 )
	{
		m_CraftingInitialPos = GetPosition();
		Debug.Log("SetUpCrafting called for id: "+ id.ToString()+ " on player: "+ this.ToString(),"recipes");

		SetCrafting(false);
		
		m_Item1 = item1;
		m_Item2 = item2;
		
		SetCrafting(true);
		
		m_RecipeID = id;
	}
	//--------------------------------------------------------------------------
	void OnScheduledTick(float deltaTime)
	{
		if ( GetDayZGame().IsNewPlayer() )
		{
			if( m_ModifiersManager ) m_ModifiersManager.OnScheduledTick(deltaTime);
			if( m_NotifiersManager ) m_NotifiersManager.OnScheduledTick();
			if( m_DisplayStatus ) m_DisplayStatus.OnScheduledTick();
			if( GetStateManager() ) GetStateManager().OnScheduledTick();
		}
	}

	
	bool m_ShowDbgUI = true;
	int m_DbgListSelection = 0;
	float m_DbgSliderValue = 0.0;
	autoptr array<string> m_DbgOptions = {"jedna", "dva", "tri"};
	
	NotifiersManager GetNotifiersManager()
	{
		return m_NotifiersManager;
	}
	//--------------------------------------------------------------------------
	void OnTick()//this gets called from the scheduler, adds deltaTime, and calls OnScheduledTick()
	{
		float deltaT = (GetGame().GetTime() - m_LastTick) / 1000;
		m_LastTick = GetGame().GetTime();
		OnScheduledTick(deltaT);		
		//-----------------------------------		
		/*
		DbgUI.Begin("Test");
		DbgUI.Check("Show DbgUI", m_ShowDbgUI);
		if (m_ShowDbgUI)
		{
			DbgUI.Text("DbgUI Test");		
	
			string name = "";
			DbgUI.InputText("name", name);		
			
			if (DbgUI.Button("Print name"))
			{
				Print(name);		
			}
			
			DbgUI.List("test list", m_DbgListSelection, m_DbgOptions);
			
			DbgUI.Text("Choice = " + m_DbgListSelection.ToString());	
			
			DbgUI.Spacer(10);
			DbgUI.SliderFloat("slider", m_DbgSliderValue, 0, 100);
			DbgUI.Text("Slider value = " + ftoa(m_DbgSliderValue));	
		}
		DbgUI.End(); */
		
		if( IsCraftingSetUp() && GetCraftingSetUpDistance() > 1 ) SetCrafting(false);
	}
	// -------------------------------------------------------------------------
	void EEItemIntoHands(EntityAI item)
	{				
		if (this == GetGame().GetPlayer())
		{
			UIScriptedMenu menu = GetGame().GetUIManager().FindMenu(MENU_INVENTORY);
			if (menu)
			{
				menu.Refresh();
			}
		}
	}

	void EEItemOutOfHands(EntityAI item)
	{	
		Print("PSOVIS: EEItemOutOfHands");

		if (this == GetGame().GetPlayer())
		{
			UIScriptedMenu menu = GetGame().GetUIManager().FindMenu(MENU_INVENTORY);
			if (menu)
			{
				menu.Refresh();
			}
		}
	}

	void CommandHandler(float pDt, int pCurrentCommandID, bool pCurrentCommandFinished)	
	{	
		super.CommandHandler(pDt,pCurrentCommandID,pCurrentCommandFinished);
		if ( m_StanceIndicator ) m_StanceIndicator.Update();
		if ( m_StaminaHandler ) m_StaminaHandler.Update(pDt);
		if ( GetGame().IsServer() )
		{
			if ( m_Environment ) m_Environment.Update(pDt);
			if ( m_StoneGenerator ) m_StoneGenerator.Update(pDt); 
			if ( m_ActionManager && (pCurrentCommandID == DayZPlayerConstants.COMMANDID_ACTION || pCurrentCommandID == DayZPlayerConstants.COMMANDID_MOVE) ) m_ActionManager.ActionSetter(); // checks for suitable action and sets it
		}
	}
	
	bool CanSprint()
	{
		if ( m_StaminaHandler && (!m_StaminaHandler.EnoughStaminaToSprint() || IsOverloaded()) ) return false;
		return true;
	}
	
	// -------------------------------------------------------------------------
	// 		USER ACTIONS
	// -------------------------------------------------------------------------
		
	// Events called from player input
	void OnGetActions (ActionMenuContext ctx, Man player)
	{  	
		super.OnGetActions(ctx, player);

		//ctx.AddAction("-=Close Menu=-", 696969, NULL, 1, true, true);//string text, int action_id, Param params, float priority, bool showWindow, bool hideOnUse, array<int> memoryPoints = NULL

		if ( GetDayZGame().IsNewPlayer() && GetGame().IsServer() && m_ActionManager ) m_ActionManager.OnGetActions(ctx);
	}

	bool OnAction(int action_id, Man player, ParamsReadContext ctx)
	{
		super.OnAction(action_id, player, ctx);

		/*if (action_id == 696969)
		{
			Print("Toggle Actions");
		}*/

		if( GetDayZGame().IsNewPlayer() && action_id == 109 && GetGame().IsServer() &&  m_ActionManager) m_ActionManager.OnAction();
	}

	void OnContinuousStart() 
	{
		Print("PlayerBase.c OnContinuousStart");
		if( GetDayZGame().IsNewPlayer() && GetGame().IsServer() && m_ActionManager ) m_ActionManager.OnContinuousStart();
	}

	void OnContinuousCancel()
	{
			if( GetDayZGame().IsNewPlayer() && GetGame().IsServer() && m_ActionManager ) m_ActionManager.OnContinuousCancel();
	}

	void OnSingleUse()
	{
		Print("PSOVIS: OnSingleUse");

		if( GetDayZGame().IsNewPlayer() && GetGame().IsServer() && m_ActionManager ) m_ActionManager.OnSingleUse();
	}



	void ShowHintForUserActions(Param2<int,int> uaparam)
	{
		//tahle cast bude client side sahat na promenne akci setu
		if ( GetDayZGame().IsNewPlayer() && m_ActionManager )
		{
			string actiontext;
			if ( uaparam.param1 > 0 )
			{
				actiontext = "HOLD RIGHT mouse button to " + m_ActionManager.GetAction(uaparam.param1).GetText();
				MessageAction(actiontext);
			}
			if ( uaparam.param2 > 0 )
			{			
				 actiontext = "PRESS RIGHT mouse button to " + m_ActionManager.GetAction(uaparam.param2).GetText();	
				MessageAction(actiontext);
			}
			/*
			if ( uaparam.param3 > 0 )
			{
				m_CurrentTertiaryActionText = GetAction(uaparam.param3).GetText();
			}
			else
			{
				m_CurrentTertiaryActionText = "Interact";
			}*/
		}
	}


	//-----------------------



	bool IsFacingTarget( Object target )
	{
		vector pdir = GetDirection();
		vector ptv = target.GetPosition() - GetPosition();
		pdir.Normalize();
		ptv.Normalize();
		//MessageAction(ToString(pdir)+"  "+ToString(ptv)+"  ");
		if (Math.AbsFloat(pdir[0]-ptv[0]) < 0.5 && Math.AbsFloat(pdir[2]-ptv[2]) < 0.5 )
		{
			return true;
		}
		return false;
	}

	//---------------------------------------------------------

	float GetPlayerLoad()
	{
		return m_CargoLoad;
	}
	
	bool IsOverloaded()
	{
		if ( GetPlayerLoad() >= OVERLOAD_LIMIT ) return true;
		return false;
	}

	void SetPlayerLoad(float load)
	{
		m_CargoLoad = load;
		
		//Log(ToString(this) + "'s load weight is " + ftoa(m_CargoLoad) + " g.", LogTemplates.TEMPLATE_PLAYER_WEIGHT);
	}

	void AddPlayerLoad(float addedload)
	{
		float newload = GetPlayerLoad() + addedload;
		SetPlayerLoad(newload);
		
		// Do not need -> Log is in SetPlayerLoad
		//PrintString(ToString(this) + "'s load weight is " + ToString(m_CargoLoad) + " g.");
	}

	bool IsItemInInventory(EntityAI entity)
	{
		
		EntityAI attachment;
		EntityAI in_item;
		int attcount = this.AttachmentsCount();
		
		for (int i = 0; i < attcount; i++)
		{	
			attachment = this.GetAttachmentFromIndex(i);
			if( attachment.HasEntityInCargo(entity) ) return true;
		}
		return false;
	}


	void CalculatePlayerLoad()
	{
		EntityAI attachment;
		EntityAI in_item;
		string item_name;
		int attcount = this.AttachmentsCount();
		float total_load = 0;
		
		for (int att = 0; att < attcount; att++)
		{	
			attachment = this.GetAttachmentFromIndex(att);
			if ( attachment.IsItemBase() )
			{
				item_name = attachment.GetType();
				total_load += g_Game.ConfigGetInt("cfgVehicles " + item_name + " weight");	
				if (  this.GetAttachmentFromIndex(att).GetCargo() )
				{
					int icount = this.GetAttachmentFromIndex(att).GetCargo().GetItemCount();
					for (int i = 0; i < icount; i++)
					{
						in_item = attachment.GetCargo().GetItem(i);
						if ( in_item.IsItemBase() )
						{
							item_name = in_item.GetType();
							total_load += g_Game.ConfigGetInt("cfgVehicles " + item_name + " weight");	
						}
					}
				}
			}
		}
		SetPlayerLoad(total_load);
	}


	void ChangePlayerLoad(EntityAI attachment, bool decrease)
	{
		EntityAI in_item;
		string item_name;
		float total_load = 0;
		if ( attachment.IsItemBase() )
		{
			item_name = attachment.GetType();
			total_load += g_Game.ConfigGetInt("cfgVehicles " + item_name + " weight");	
			if (attachment.GetCargo() != NULL)
			{
				int icount = attachment.GetCargo().GetItemCount();
				if ( icount > 0 )
				{
					for (int i = 0; i < icount; i++)
					{
						in_item = attachment.GetCargo().GetItem(i);
						if ( in_item.IsItemBase() )
						{
							item_name = in_item.GetType();
							total_load += g_Game.ConfigGetInt("cfgVehicles " + item_name + " weight");	
						}
					}
				}
			}
			if ( decrease )
			{
				AddPlayerLoad(-total_load);
			} 
			else
			{
				AddPlayerLoad(total_load);
			}
		}
	}
	
	float GetImmunity()
	{
		if(	m_PlayerStats ) 
		{
			float max_health = GetMaxHealth("GlobalHealth", "Health") + 0.01;//addition to prevent divisioin by zero in case of some messup
			float max_blood = GetMaxHealth("GlobalHealth", "Blood") + 0.01;//addition to prevent divisioin by zero in case of some messup
			return ( m_PlayerStats.m_Diet.GetNormalized() + m_PlayerStats.m_Energy.GetNormalized() + (this.GetHealth("GlobalHealth", "Health") / max_health) + (this.GetHealth("GlobalHealth", "Blood") / max_blood) ) / 4;
		}
		return 0;
	}
	
	// -------------------------------------------------------------------------
	void RPC(int rpc_type, array<Param> params, Man recipient = NULL)
	{
		super.RPC(rpc_type, params, recipient);
	}

	// -------------------------------------------------------------------------
	void OnRPC(int rpc_type, ParamsReadContext ctx)
	{
		super.OnRPC(rpc_type, ctx);
		
		if ( GetDayZGame().IsNewPlayer() )
		{
			if( rpc_type == RPC_SYNC_DISPLAY_STATUS )
			{
				if( GetDisplayStatus() ) 
				{
					GetDisplayStatus().OnRPC(ctx);
				}
			}
			
			else if( rpc_type == RPC_PLAYER_STATES_ON )
			{
				if( GetStateManager() ) 
				{
					GetStateManager().OnRPC(RPC_PLAYER_STATES_ON, ctx);
				}
			}
			
			else if( rpc_type == RPC_PLAYER_STATES_OFF )
			{
				if( GetStateManager() ) 
				{
					GetStateManager().OnRPC(RPC_PLAYER_STATES_OFF, ctx);
				}
			}
		
			else if ( GetGame().IsClient() )
			{
				if( rpc_type == RPC_USER_ACTION_SYNCH )
				{	
					if ( ctx.Read(m_UAParam) )
					{
						ShowHintForUserActions(m_UAParam);
					}
				}	
				
				else if( rpc_type == RPC_USER_ACTION_MESSAGE )
				{	
					if ( ctx.Read(m_UAParamMessage) )
					{
						string actiontext = m_UAParamMessage.param1;
						MessageAction(actiontext);
					}
				}
				
				else if( rpc_type == RPC_USER_ACTION_MESSAGES )
				{	
					if ( ctx.Read(m_UAParam) && m_ActionManager )
					{
						string actiontext2 = m_ActionManager.GetAction(m_UAParam.param1).GetMessageText(m_UAParam.param2);
						MessageAction(actiontext2);
					}
				}		

				if( rpc_type == RPC_STAMINA )
				{	
					if ( ctx.Read(m_StaminaParam) && m_StaminaHandler )
					{
						m_StaminaHandler.SyncStamina(m_StaminaParam.param1,m_StaminaParam.param2);
					}
				}	
	
			}
			
		}
		else
		{
			if( rpc_type == RPC_SQF_PLAYER_NOTIFI_SYNCH )
			{
				//Log("OnRPC => RPC_SQF_PLAYER_NOTIFI_SYNCH");
				
				autoptr array<Param> params = new array<Param>;
				autoptr Param1<int> p_count = new Param1<int>(0);
				
				if ( ctx.Read( p_count ) )
				{
					//Log( "OnRPC Reding Count: " + p_count.param1.ToString() );
					
					for ( int i = 0; i < p_count.param1; ++i )
					{
						Param3<int, string, int> p = new Param3<int, string, int>(0, "", 0);
						
						if ( ctx.Read( p ) )
						{
							//Log( "OnRPC Reding: " + p.param2 );
							params.Insert(p);
						}
					}			
				}
				
				if ( params.Count() > 0 )
				{
					UpdatePlayerNotifier(params);
				}
			}
		}
		
		if( rpc_type == RPC_PLACING_START)
		{
			PlacingStart( GetItemInHands().GetType() );
		}

		if( rpc_type == RPC_PLACING_STOP)
		{
			PlacingStop();
		}

		if( rpc_type == RPC_PLACING_FINISH)
		{
			PlacingFinish( GetItemInHands() );
		}
		
		if( rpc_type == RPC_RECIPE_SEND)
		{
			if ( m_ModuleRecipesManager )
			{
				m_ModuleRecipesManager.OnRPC(ctx,this);
			}
		}

		if( rpc_type == RPC_ON_SET_CAPTIVE)
		{
			CallMethod( CALL_ID_HIDE_INVENTORY, NULL );
		}

		
		if ( GetDayZGame().IsDebug() )
		{
			PluginDeveloper module_rc = GetPlugin(PluginDeveloper);
			module_rc.OnRPC(this, rpc_type, ctx);
			
			PluginDeveloperSync module_rcs = GetPlugin(PluginDeveloperSync);
			module_rcs.OnRPC(this, rpc_type, ctx);
			
			// TODO: remove after UA work
			PluginHologramDebug.GetInstance().OnRPC(rpc_type, ctx, this);
		}
	}
	
	// -------------------------------------------------------------------------
	void OnSynchronize(ParamsWriteContext ctx)
	{
		super.OnSynchronize(ctx);

		autoptr Param1<int> last_shaved_param = new Param1<int>( m_LastShavedSeconds );
		ctx.Write( last_shaved_param );
		
		autoptr Param1<int> cached_playtime_param = new Param1<int>( m_CachedPlaytime );
		ctx.Write( cached_playtime_param );
	}

	// -------------------------------------------------------------------------
	void OnSynchronized(ParamsReadContext ctx)
	{
		super.OnSynchronized(ctx);

		autoptr Param1<int> last_shaved_param = new Param1<int>( 0 );
		if ( ctx.Read(last_shaved_param) )
		{
			m_LastShavedSeconds = last_shaved_param.param1;
		}
		
		autoptr Param1<int> cached_playtime_param = new Param1<int>( 0 );
		if ( ctx.Read(cached_playtime_param) )
		{
			m_CachedPlaytime = cached_playtime_param.param1;
		}
	}

	// -------------------------------------------------------------------------
	ItemBase GetItemInHands()
	{
		ItemBase item = GetEntityInHands();
		return item;
	}

	//--------------------------------------------------------------------------
	/**
	\brief Spawn item on server side
		\param object_name \p string Name of item class
		\param spawn_type \int(const) Type of spawning const: SPAWNTYPE_GROUND, SPAWNTYPE_HANDS, SPAWNTYPE_INVENTORY
		\param full_quantity \bool Spawn with full quantity of item
		\param distance \float Distance of spawn position from player (Only if spawn_type = SPAWNTYPE_GROUND)
		\returns \p ItemBase Spawned item
		@code
			PlayerBase player = GetGame().GetPlayer();
			
			player.SpawnItem("Fruit_Apple", SPAWNTYPE_GROUND, true, 5);
		@endcode
	*/
	EntityAI SpawnItem(string object_name, int spawn_type, bool full_quantity, float distance = 0, bool usePosition = false, vector pos = "0 0 0", EntityAI attachmentObject = NULL)
	{
		EntityAI entity = NULL;

		// Creat Object
		bool is_ai = GetGame().IsKindOf( object_name, "DZ_LightAI");
		vector item_position;
		if( usePosition )
		{
			item_position = pos;
		}
		else
		{
			item_position = GetPosition() + ( GetDirection() * distance );
		}
		entity = GetGame().CreateObject(object_name, item_position, false, is_ai);

		if ( !entity )
		{
			string s = "Cannot spawn entity: "+object_name;
			Print(s);
			//g_Game.GetUIManager().ShowDialog("Spawn error", "Cannot spawn item: "+object_name, 0, DBT_OK, DBB_YES, DMT_EXCLAMATION, this);
			return NULL;
		}

		if ( entity.IsInherited( ItemBase ) )
		{
			ItemBase item = entity;

			// Set full quantity
			if ( full_quantity )
			{
				item.SetQuantity(item.GetQuantityMax());
			}

			// Spawn In Inventory
			if ( spawn_type == SPAWNTYPE_INVENTORY )
			{
				TakeItemToInventory(item);
			}
			// Spawn In Hands
			if ( spawn_type == SPAWNTYPE_HANDS )
			{
				TakeItemToHands(item);
			}
			// Spawn As Attachment
			if ( spawn_type == SPAWNTYPE_ATTACHMENT )
			{
				Object cursor_obj = GetCursorObject();
				if ( cursor_obj != NULL && cursor_obj.IsInherited(EntityAI) )
				{
					EntityAI eai = (EntityAI)cursor_obj;
					eai.TakeEntityAsAttachment(item);
				}
				else if ( attachmentObject != NULL)
				{
					attachmentObject.TakeEntityAsAttachment(item);
				}
			}
		}

		return entity;
	}

	//--------------------------------------------------------------------------
	bool DropItem(ItemBase item)
	{
		bool is_dropped = DropEntity(item);

		vector 	pos_spawn	= GetPosition() + GetDirection();
		pos_spawn[0]		= pos_spawn[0] + Math.RandomFloat(-0.2, 0.2);
		pos_spawn[2]		= pos_spawn[2] + Math.RandomFloat(-0.2, 0.2);
		pos_spawn		 	= GetGame().SnapToGround(pos_spawn);

		item.SetPosition(pos_spawn);

		return is_dropped;
	}

	bool TakeItemToInventory(ItemBase item)
	{
		if ( GetItemInHands() == item )
		{
			MoveItemFromHandsToInventory(item);
			return true;
		}

		if ( CanTakeEntityAsOwned(item) )
		{
			TakeEntityAsOwned(item);
			return true;
		}

		//g_Game.GetUIManager().ShowDialog("Spawn error", "Cannot take item into inventory! ("+object_name+")", 0, DBT_OK, DBB_YES, DMT_EXCLAMATION, this);
		return false;
	}

	bool TakeItemToHands(ItemBase item)
	{
		if ( GetItemInHands() )
		{
			RemoveItemFromHands(item);
		}

		return TakeEntityToHands(item);
	}

	// -------------------------------------------------------------------------
	/**
	/brief Creates \p ItemBase in players inventory. If player dont have enough slots for new item, item will be created on the ground under player.
		\param item_name \p string Class name of ItemBase in configs
		\returns \p ItemBase Created item
		@code
			ItemBase item = g_Game.GetPlayer().CreateInInventory("Consumable_GardenLime", "cargo_weapon");
		@endcode
	*/
	ItemBase CreateInInventory( string item_name, string cargo_type = "" ) // TODO: Create item in cargo
	{
		return SpawnItem(item_name, SPAWNTYPE_INVENTORY, true);
	}

	// -------------------------------------------------------------------------
	/**
	/brief Creates copy of \p ItemBase in players inventory. If player dont have enough slots for new item, item will be created on the ground under player.
		\param item_name \p string Class name of ItemBase in configs
		\returns \p ItemBase Created item
		@code
			ItemBase item = g_Game.GetPlayer().CreateInInventory("Consumable_GardenLime", "cargo_weapon");
		@endcode
	*/
	ItemBase CopyInventoryItem( ItemBase orig_item )
	{
		ItemBase item = CreateInInventory( orig_item.GetType() );
		if ( item == NULL )
		{
			return NULL;
		}

		// Copy of quantity
		item.SetQuantity( orig_item.GetQuantity() );

		// Copy of damage
		item.SetDamage( orig_item.GetDamage() );

		return item;
	}

	// -------------------------------------------------------------------------
	/**
	colorStatusChannel
	colorAction
	colorFriendly
	colorImportant
	*/
	void Message( string text, string style )
	{
		if (GetGame().IsMultiplayer() && GetGame().IsServer())
		{
			GetGame().ChatMP(this, text, style );
		}
		else
		{
			GetGame().Chat( text, style );
		}
	}

	// -------------------------------------------------------------------------
	void MessageStatus( string text )
	{
		Message( text, "colorStatusChannel" );
	}

	// -------------------------------------------------------------------------
	void MessageAction( string text )
	{
		Message( text, "colorAction" );
	}

	// -------------------------------------------------------------------------
	void MessageFriendly( string text )
	{
		Message( text, "colorFriendly" );
	}

	// -------------------------------------------------------------------------
	void MessageImportant( string text )
	{
		Message( text, "colorImportant" );
	}

	// -------------------------------------------------------------------------
	/**
	/brief Delete all items in inventory
		\returns \p void
		@code
			PlayerBase player = GetGame().GetPlayer();
			player.ClearInventory();
		@endcode
	*/
	void ClearInventory( )
	{
		if ( GetGame().IsServer() )
		{
			ItemBase item;
			for ( int i = 0; i < GetSlotsCount(); ++i )
			{
				item = FindAttachment(i);
				if ( item )
				{
					GetGame().ObjectDelete(item);
				}
			}

			ItemBase item_in_hands = GetEntityInHands();

			if ( item_in_hands )
			{
				GetGame().ObjectDelete(item_in_hands);
			}
		}
	}

	string GetPlayerClass()
	{
		string type;
		GetGame().ObjectGetType( this, type );
		return type;
	}

	void ShavePlayer()
	{
		
		// NEW STATS API
		//m_LastShavedSeconds = StatGet("playtime");
		
		PluginLifespan module_lifespan = (PluginLifespan)GetPlugin( PluginLifespan );
		module_lifespan.UpdateLifespanLevelAuto( this, true );
		
		SetSynchDirty();
	}

	int GetLastShavedSeconds()
	{
		return m_LastShavedSeconds;
	}

	void SetLastShavedSeconds( int last_shaved_seconds )
	{
		m_LastShavedSeconds = last_shaved_seconds;
	}

	int GetCachedPlaytime()
	{
		return m_CachedPlaytime;
	}

	void SetCachedPlaytime( int playtime )
	{
		m_CachedPlaytime = playtime;
	}

	bool GetHasBloodyHands()
	{
		return m_HasBloodyHands;
	}

	void SetHasBloodyHands( bool has_bloody_hands )
	{
		m_HasBloodyHands = has_bloody_hands;
	}

	void WashHands()
	{
		PluginLifespan module_lifespan = (PluginLifespan)GetPlugin( PluginLifespan );
		module_lifespan.UnSetBloodyHands( this );
	}

	void SetSkill( int exp_id, float exp_value )
	{
		m_Skills.Set( exp_id, exp_value );
	}

	float GetSkill( int exp_id )
	{
		return m_Skills.Get( exp_id );
	}

	float GetTimeToCompleteEfficiency( int exp_id )
	{
		return m_ModuleExperience.GetTimeToCompleteEfficiency(this, exp_id);
	}

	float GetQuantityEfficiency( int exp_id )
	{
		return m_ModuleExperience.GetQuantityEfficiency(this, exp_id);
	}

	void IncreaseSkill( int exp_id, float increase_value /*= 1*/ )
	{
		m_ModuleExperience.Increase(this, exp_id, increase_value);
	}

	void OnStoreSave( ParamsWriteContext ctx )
	{
		super.OnStoreSave( ctx );
		
		OnStoreSaveLifespan( ctx );

		if ( GetDayZGame().IsDebug() && GetDayZGame().IsServer() )
		{
			if( GetDayZGame().IsNewPlayer() )
			{
				if( m_PlayerStats ) m_PlayerStats.SaveStats(ctx);// save stats
				if( m_ModifiersManager ) m_ModifiersManager.OnStoreSave(ctx);// save modifiers
				if( m_AgentPool ) m_AgentPool.OnStoreSave(ctx);//save agents
			}
		}
	}

	void OnStoreLoad( ParamsReadContext ctx )
	{
		super.OnStoreLoad( ctx );
		
		OnStoreLoadLifespan( ctx );
		
		if ( GetDayZGame().IsDebug() && GetDayZGame().IsServer() && GetDayZGame().IsMultiplayer() )
		{
			if( GetDayZGame().IsNewPlayer() )
			{
				if( m_PlayerStats ) m_PlayerStats.LoadStats(ctx); // load stats
				if( m_ModifiersManager ) m_ModifiersManager.OnStoreLoad(ctx);// load modifiers
				if( m_AgentPool ) m_AgentPool.OnStoreLoad(ctx);//load agents
			}
		}
		
		SetSynchDirty();
	}

	void OnStoreSaveLifespan( ParamsWriteContext ctx )
	{
		autoptr Param1<int> skills_count = new Param1<int>( m_Skills.Count() );
		ctx.Write( skills_count );
		
		autoptr Param2<int, float> params = new Param2<int, float>( 0, 0.0 );
		
		for ( int i = 0; i < skills_count.param1; i++ )
		{
			params.param1 = m_Skills.GetKey( i );
			params.param2 = m_Skills.GetElement( i );
			ctx.Write( params );
		}

		autoptr Param1<int> param_int = new Param1<int>( m_LastShavedSeconds );
		ctx.Write( param_int );
		
		param_int.param1 = m_CachedPlaytime;
		ctx.Write( param_int );
		
		autoptr Param1<bool> param_bool = new Param1<bool>( m_HasBloodyHands );
		ctx.Write( param_bool );
	}

	void OnStoreLoadLifespan( ParamsReadContext ctx )
	{
		autoptr Param1<int> skills_count = new Param1<int>( 0 );
		ctx.Read( skills_count );
		
		autoptr Param2<int, float> params = new Param2<int, float>( 0, 0.0 );

		for ( int i = 0; i < skills_count.param1; i++ )
		{
			ctx.Read( params );
			SetSkill( params.param1, params.param2 );
		}
		
		autoptr Param1<int> param_int = new Param1<int>( 0 );
		ctx.Read( param_int );
		if ( param_int.param1 > 0 )
		{
			m_LastShavedSeconds = param_int.param1;
		}
		
		param_int.param1 = 0;
		ctx.Read( param_int );
		if ( param_int.param1 > 0 )
		{
			m_CachedPlaytime = param_int.param1;
		}
		
		autoptr Param1<bool> param_bool = new Param1<bool>( false );
		ctx.Read( param_bool );
		m_HasBloodyHands = param_bool.param1;
	}

	void OnSqfPlayerInit()
	{
		PluginLifespan module_lifespan = (PluginLifespan)GetPlugin( PluginLifespan );
		module_lifespan.UpdateLifespanLevelAuto( this, true );
		
		if ( m_HasBloodyHands )
		{
			module_lifespan.SetBloodyHands( this );
		}
		else 
		{
			module_lifespan.UnSetBloodyHands( this );
		}
		Print("OnSqfPlayerInit()");
	}

	void OnConnect()
	{
		Debug.Log("Player connected:"+this.ToString(),"Connect");

		// NEW STATS API
		StatRegister("playtime");
		StatRegister("dist");
	}

	void OnDisconnect()
	{
		Debug.Log("Player disconnected:"+this.ToString(),"Connect");
	}


	// agent transfer

	bool IsInfected()
	{
		if( m_AgentPool && m_AgentPool.GetAgents().Count() != 0) 
		{
			return true;			
		}
		else
		{
			return false;
		}
	}
	 
	//--------------------------------------------------------------------------------------------
	int GetAgents()
	{
		return PluginTransmissionAgents.MakeMaskFromArray( m_AgentPool.GetAgents() );
	}

	//--------------------------------------------------------------------------------------------
	void RemoveAgent(int agent_id) //removes a single agent type to the player agent pool
	{
		m_AgentPool.RemoveAgent(agent_id);
	}

	//--------------------------------------------------------------------------------------------
	void RemoveAllAgents()
	{
		m_AgentPool.RemoveAllAgents();
	}


	//--------------------------------------------------------------------------------------------
	void InsertAgent(int agent, int count) //adds a single agent type to the player agent pool
	{
		m_AgentPool.AddAgent(agent,count);
	}

	//--------------------------------------------------------------------------------------------
	int GetSingleAgentCount(int agent_id)
	{
		return m_AgentPool.GetSingleAgentCount(agent_id);
	}

	int GetTotalAgentCount()
	{
		return m_AgentPool.GetTotalAgentCount();
	}

	void PrintAgents()
	{
		m_AgentPool.PrintAgents();
	}

	void ImmuneSystemTick(float value)
	{
		m_AgentPool.ImmuneSystemTick(value);
	}


	//--------------------------------------------------------------------------------------------
	// TEMP for stable 0.60 (februar 2016) old player inventory notifiers
	// Server Side
	// calling from SQF server/scripts/init.sqf
	// collecting datas from SQF
	void SQF_PlayerNotifierAdd(string notifi_name, int notifi_id, float r, float g, float b, float a)
	{
		if ( GetGame().IsNewPlayer() )
		{
			return;
		}
		
		int color = ARGBF(a,r,g,b);
		
		Param3<string, int, bool> my_notifier = m_MyNotifiers[notifi_id];
		
		//Log("SQF_PlayerNotifierAdd: "+notifi_name);
		
		if ( my_notifier.param2 != color || my_notifier.param3 == true )
		{
			my_notifier.param1 = notifi_name;
			my_notifier.param2 = color;
			my_notifier.param3 = true; // tag for sychnronizing
			
			//Log("SQF_PlayerNotifierAdd: "+notifi_name+" => Synch");
		}
		
		m_MyNotifiers[notifi_id] = my_notifier;
	}

	//--------------------------------------------------------------------------------------------
	// TEMP for stable 0.60 (februar 2016) old player inventory notifiers
	// Server Side
	// calling from SQF server/scripts/init.sqf
	// Send on Client collected datas from SQF
	void SQF_PlayerNotifierSend()
	{
		if ( GetGame().IsNewPlayer() )
		{
			return;
		}
		
		autoptr array<Param> params = new array<Param>;
		
		int count = 0;
		
		for ( int i = 0; i < NTF_COUNT; ++i )
		{
			Param3<string, int, bool> my_notifier = m_MyNotifiers[i];
			
			if ( my_notifier != NULL && my_notifier.param3 )
			{
				count++;
				Param3<int, string, int> p = new Param3<int, string, int>(i, my_notifier.param1, my_notifier.param2);
				params.Insert( p );
				my_notifier.param3 = false;
			}
		}
			
		//Log("SQF_PlayerNotifierSend: Synch ("+itoa(params.Count())+")...");
		
		if ( count > 0 )
		{
			if ( GetGame().IsServer() && GetGame().IsMultiplayer() )
			{
				autoptr Param1<int> p_count = new Param1<int>(count);
				params.InsertAt (p_count, 0);
				
				//Log("SQF_PlayerNotifierSend => RPC sending data ("+itoa(params.Count())+")...");
				RPC(RPC_SQF_PLAYER_NOTIFI_SYNCH, params, this);
			}
			else
			{
				//Log("SQF_PlayerNotifierSend => local update data ("+itoa(params.Count())+")...");
				UpdatePlayerNotifier(params);
			}
		}
	}

	//--------------------------------------------------------------------------------------------
	// TEMP for stable 0.60 (februar 2016) old player inventory notifiers
	// Client Side from OnRPC
	//void	UpdatePlayerNotifier(array<Param3<int, string, int>> params);
	void UpdatePlayerNotifier(array<Param> params)
	{
		if ( !GetGame().IsNewUI() || GetGame().IsNewPlayer() )
		{
			return;
		}
		
		//Log("UpdatePlayerNotifier");
		
		if ( m_ModulePlayerStatus )
		{		
			for ( int i = 0; i < params.Count(); ++i )
			{
				Param3<int, string, int> p = params.Get(i);
				//Log("UpdatePlayerNotifier => ["+itoa(i)+"] = "+p.param2);
				m_ModulePlayerStatus.SetNotifier( p.param1, 9, p.param2, p.param3 );
				delete p;
			}
		}
	}

	//--------------------------------------------------------------------------------------------
	// TEMP for stable 0.60 (februar 2016) old player inventory notifiers
	// Client Side from UpdatePlayerNotifier
	void PlayerNotifierClear()
	{
		for ( int i = 0; i < NTF_COUNT; ++i )
		{
			if ( m_ModulePlayerStatus )
			{
				m_ModulePlayerStatus.SetNotifier( i, 9, "", 0x000000 );
			}
		}
	}

	void PlayerNotifiersInit()
	{
		PlayerNotifiersDelete();

		for ( int i = 0; i < NTF_COUNT; ++i )
		{
			m_MyNotifiers[i] = new Param3<string, int, bool>("", 0, true);
		}	
	}

	void PlayerNotifiersDelete()
	{
		for ( int i = 0; i < NTF_COUNT; ++i )
		{
			delete m_MyNotifiers[i];
		}
	}
	array<PlayerStatBase> GetStatsArray()
	{
		return m_PlayerStats.Get();
	}

	//Get aim (player crosshair) position
	vector GetAimPosition()
	{
		float min_distance = 0.5;		//min distance, default = 5m
		
		vector from = GetCameraPosition();
		vector to = from + (GetCameraDirection() * min_distance);
		vector contactPos;
		vector contactDir;
		int contactComponent;
		
		GetGame().Raycast ( from, to, contactPos, contactDir, contactComponent, NULL, NULL, NULL, false, true );
		
		return contactPos;
	}

	//get modifier manager
	ModifiersManager GetModifiersManager()
	{
		return m_ModifiersManager;
	}

	PlayerStat<float> GetStatStomach()
	{
		if( m_PlayerStats ) return m_PlayerStats.m_Stomach;
	}

	PlayerStat<float> GetStatWater()
	{
		if( m_PlayerStats ) return m_PlayerStats.m_Water;
	}

	PlayerStat<float> GetStatStomachWater()
	{
		if( m_PlayerStats ) return m_PlayerStats.m_StomachWater;
	}

	PlayerStat<float> GetStatEnergy()
	{
		if( m_PlayerStats ) return m_PlayerStats.m_Energy;
	}
	
	PlayerStat<float> GetStatShock()
	{
		if( m_PlayerStats ) return m_PlayerStats.m_Shock;
	}
	
	PlayerStat<float> GetStatUnconscious()
	{
		if( m_PlayerStats ) return m_PlayerStats.m_Unconscious;
	}
	
	PlayerStat<float> GetStatTemperature()
	{
		if( m_PlayerStats ) return m_PlayerStats.m_Temperature;
	}
	
	PlayerStat<float> GetStatHeatComfort()
	{
		if( m_PlayerStats ) return m_PlayerStats.m_HeatComfort;
	}
	
	PlayerStat<float> GetStatHeatIsolation()
	{
		if( m_PlayerStats ) return m_PlayerStats.m_HeatIsolation;
	}
	
	PlayerStat<float> GetStatTremor()
	{
		if( m_PlayerStats ) return m_PlayerStats.m_Tremor;
	}
	
	PlayerStat<float> GetStatWet()
	{
		if( m_PlayerStats ) return m_PlayerStats.m_Wet;
	}
	
	PlayerStat<float> GetStatStomachEnergy()
	{
		if( m_PlayerStats ) return m_PlayerStats.m_StomachEnergy;
	}
	
	PlayerStat<float> GetStatDiet()
	{
		if( m_PlayerStats ) return m_PlayerStats.m_Diet;
	}
	
	PlayerStat<float> GetStatStamina()
	{
		if( m_PlayerStats ) return m_PlayerStats.m_Stamina;
	}
	
	PlayerStat<int> GetStatBloodType()
	{
		if( m_PlayerStats ) return m_PlayerStats.m_BloodType;
	}
}