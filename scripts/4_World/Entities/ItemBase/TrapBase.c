class TrapBase extends ItemBase
{
	int   m_InitWaitTime; 			//After this time after deployment, the trap is activated
	bool m_NeedActivation;			//If activation of trap is needed
	float m_DefectRate; 			//Added damage after trap activation
	float m_DamagePlayers; 			//How much damage player gets when caught
	float m_DamageOthers; 			//How much damage player gets when caught
	float m_WrongManipulation;		//Chance for damage when manipulating (activate/deactivate) with trap
	bool  m_Reactivable;			//whether trap can be re-activated 
	bool  m_CustomExplosion;		//explosion yes/no
	string  m_CustomExplosionEvent;	//explosion event
	int m_Radius;  					//TODO: Minimal allowed distance to the closest player for the trap to work

	bool m_AddActivationDefect;		// Damage trap after activation
	bool m_AddDeactivationDefect;	// Damage trap after deactivation

	bool m_WasActivatedOrDeactivated;

	autoptr array<string> m_SurfaceForSetup;
	string m_AnimationPhaseGrounded;
	string m_AnimationPhaseSet;
	string m_AnimationPhaseTriggered;
	autoptr map<string, string> m_AnimationPhaseAttachment; //item_name, [animation_phase, event_on_enter]
	autoptr map<string, string> m_AttachmentEvent; //item_name, [animation_phase, event_on_enter]

	string m_InfoSetup;
	string m_InfoDeactivated;
	string m_InfoDamageManipulation;
	string m_InfoDamage;
	string m_InfoActivationTime;

	autoptr EntityAnimEndEventHandler m_anim_end_event_handler;
	
	protected autoptr Timer m_Timer;
	protected TrapTrigger m_TrapTrigger;
	protected ItemBase m_ExplosionObject;

	void TrapBase()
	{
		m_IsInProgress = false;
		m_NeedActivation = true;
		m_InitWaitTime = 5; 			//After this time after deployment, the trap is activated
		m_DefectRate = 0.15; 			//Added damage after trap activation
		m_DamagePlayers = 0.25; 		//How much damage player gets when caught
		m_DamageOthers = 1.0; 			//How much damage player gets when caught
		m_WrongManipulation = 0.05;		//Chance for damage when manipulating (activate/deactivate) with trap
		m_Reactivable = true;			//If trap can be reactivated
		m_CustomExplosion = false;		//make explosion
		m_CustomExplosionEvent = "";
		m_Radius = 1;  					//TODO: Minimal allowed distance to the closest player for the trap to work
		
		m_AddActivationDefect = false;
		m_AddDeactivationDefect = false;

		m_WasActivatedOrDeactivated = false;

		m_SurfaceForSetup = new array<string>;

		m_AnimationPhaseGrounded = "";
		m_AnimationPhaseSet = "";
		m_AnimationPhaseTriggered = "";
		m_AnimationPhaseAttachment = new map<string, string>;
		m_AttachmentEvent = new map<string, string>;
		
		m_InfoSetup = 				"I've placed the trap";
		m_InfoDeactivated = 		"I've deactivated trap";
		m_InfoDamageManipulation =  "You've been hurt when manipulating with trap";
		m_InfoDamage =				"You've been hurt by trap";
		m_InfoActivationTime = 		"Trap will be active in " + m_InitWaitTime.ToString() + " seconds";
	}
	
	void ~TrapBase()
	{
		//GetGame() can be sometimes NULL when turning off server
		if ( GetGame() && m_TrapTrigger )
		{
			GetGame().ObjectDelete( m_TrapTrigger );
			m_TrapTrigger = NULL;
		}		
	}

	bool IsActive()
	{
		if ( m_IsActive && m_IsInProgress == false && this.GetOwnerPlayer() == NULL )
		{
			return true;
		}
		return false;
	}

	bool IsInactive()
	{
		if ( m_IsActive == false && m_IsInProgress == false && this.GetOwnerPlayer() == NULL )
		{
			return true;
		}
		return false;
	}

	// trap cannot be taken when is activated
	bool IsTakeable()
	{
		// if ( g_Game.IsServer() )  // so starym GUI to vracia isServer=false, s novym GUI je to true!!!
		// {
			if ( m_IsInProgress == false && !IsActive() )
			{
				return true;
			}
		// }
		return false;
	}

	bool IsActivable()
	{
		if ( g_Game.IsServer() )
		{
			if ( m_IsActive == false && this.GetOwnerPlayer() == NULL && this.GetOwner() == NULL && m_IsInProgress == false && this.GetDamage() < 1 && m_NeedActivation )
			{
				return true;
			}
		}
		return false;
	}

	bool IsPlaceable()
	{
		if ( g_Game.IsServer() )
		{
			if ( this.GetOwnerPlayer() != NULL && this.GetOwnerPlayer().GetEntityInHands() == this && m_SurfaceForSetup != NULL )
			{
				if ( m_SurfaceForSetup.Count() > 0 )
				{
					PlayerBase player = (PlayerBase)this.GetOwnerPlayer();
					
					vector player_pos = player.GetPosition();
					vector aim_pos = player.GetAimPosition();
									
					if ( vector.Distance(player_pos, aim_pos) <= 1.5 )
					{
						return IsPlaceableAtPosition( aim_pos );
					}
				}
				else
				{
					return true;
				}
			}
		}
		
		return false;
	}

	bool IsPlaceableAtPosition( vector position )
	{
		string surface_type;
		GetGame().SurfaceGetType( position[0], position[2], surface_type );

		if ( m_SurfaceForSetup.Find(surface_type) > -1 )
		{
			return true;
		}
		
		return false;
	}
	
	void AddDamageToVictim( Object victim )
	{
		if ( g_Game.IsServer() )
		{
			if ( m_Timer )
			{
				m_Timer.Stop();
			}
			
			this.RefreshState();

			if ( m_CustomExplosion )
			{
				if ( m_AttachmentEvent.Count() > 0 )
				{
					int attachments = AttachmentsCount();
					string event_to_call = "";
					ItemBase attachment;

					if ( attachments > 0 )
					{		
						attachment = GetAttachmentFromIndex(0);
			
						if (attachment)
						{
							event_to_call = m_AttachmentEvent.Get( attachment.GetType() );
						}
					}
					else
					{
						event_to_call = m_AttachmentEvent.Get( "NoAttachment" );
					};
					
					if ( event_to_call != "" )
					{
						delete m_Timer;
						m_Timer = new Timer( CALL_CATEGORY_GAMEPLAY );
						m_Timer.Run(0.1, this, event_to_call, new Param2<TrapBase, ItemBase>(this, attachment) );
					}
				}

				if ( m_CustomExplosionEvent != "" )
				{
					delete m_Timer;
					m_Timer = new Timer( CALL_CATEGORY_GAMEPLAY );
					m_Timer.Run(0.1, this, m_CustomExplosionEvent, new Param1<TrapBase>(this) ); 
				}

			}
			
			if (m_DamagePlayers > 0)
			{
				if ( victim.ClassName() == "SurvivorBase" )
				{
					if ( !victim.IsKindOf("SurvivorPartsNewAnimSystem") ) // ignore new player
					{
						victim.AddDamage( m_DamagePlayers, false );
						PlayerBase player = victim;
						player.MessageStatus( m_InfoDamage );
					}
				}
				else
				{
					victim.AddDamage( m_DamageOthers );
				}
			}
			
			SetInactive( false );
			AddDefect();
		}
	}

	void RefreshState()
	{
		if ( !m_WasActivatedOrDeactivated )
		{
			return;
		}
		
		if ( g_Game.IsServer() )
		{
			// item has attachments defined
			if ( m_AnimationPhaseAttachment.Count() > 0 )
			{
				// zisti aky attachment mas a podla toho zapni animacnu fazu
				int attachments = AttachmentsCount();
				// hide all attachment animation phases
				// no attachments are attached, so set all attachments animation phases to invisible
				for ( int i = 0; i < m_AnimationPhaseAttachment.Count(); i++ )
				{
					// Print( m_AnimationPhaseAttachment.GetByIndex(i) );
					this.SetAnimationPhase( m_AnimationPhaseAttachment.GetElement(i), 1 );
				}
				for ( int ii = 0; ii < attachments; ii++ )
				{
					EntityAI attachment = GetAttachmentFromIndex(ii);
					string animation_phase = m_AnimationPhaseAttachment.Get( attachment.GetType() );
					if ( animation_phase != "" && IsActive() )
					{
						this.SetAnimationPhase( animation_phase, 0 );
					}
				}
			}

			// item is owned by player
			if ( this.GetOwnerPlayer() != NULL && m_AnimationPhaseGrounded != "" )
			{
				this.SetAnimationPhase( m_AnimationPhaseSet, 1 );
				if ( m_AnimationPhaseTriggered != m_AnimationPhaseGrounded ) 
				{
					this.SetAnimationPhase( m_AnimationPhaseTriggered, 1 );
				}
				this.SetAnimationPhase( m_AnimationPhaseGrounded, 0 );
			}
			// item is set active
			else if ( this.IsActive() )
			{
				if ( m_AnimationPhaseGrounded != "" )
				{
					this.SetAnimationPhase( m_AnimationPhaseGrounded, 1 );
				}
				if ( m_AnimationPhaseSet != "" && m_AnimationPhaseTriggered != "" )
				{
					this.SetAnimationPhase( m_AnimationPhaseTriggered, 1 );
					this.SetAnimationPhase( m_AnimationPhaseSet, 0 );
				}
			}
			// item is inactive and not owned by player (on the ground)
			else if ( this.IsInactive() )
			{
				if ( m_AnimationPhaseGrounded != "" &&  m_AnimationPhaseTriggered != m_AnimationPhaseGrounded )
				{
					this.SetAnimationPhase( m_AnimationPhaseGrounded, 1 );
				}
				if ( m_AnimationPhaseSet != "" && m_AnimationPhaseTriggered != "" )
				{
					this.SetAnimationPhase( m_AnimationPhaseSet, 1 );
					this.SetAnimationPhase( m_AnimationPhaseTriggered, 0 );
				}
			}
		}
	}

	void SetupTrap()
	{ 
		if ( GetGame().IsServer() )
		{
			if ( this.GetOwnerPlayer() && this.GetOwnerPlayer().CanDropEntity( this) )  // kvoli desyncu
			{
				SetupTrapPlayer( this.GetOwnerPlayer() );
			}
		}
	}

	void SetupTrapPlayer( PlayerBase player, bool set_position = true )
	{ 
		if ( GetGame().IsServer() )
		{
			player.PlayActionGlobal( "deployItem" );
			
			if ( set_position )
			{
				player.DropEntity( this );
				
				vector trapPos = ( player.GetDirection() ) * 1.5;
				trapPos[1] = 0;
				this.SetPosition( player.GetPosition() + trapPos );
			}
					
			if ( m_NeedActivation == false )
			{
				this.SetActive();
			}
			player.MessageStatus( m_InfoSetup );
		}
	}

	bool AddDefect()
	{
		if ( g_Game.IsServer() )
		{
			this.AddDamage( m_DefectRate );
		}
	}
	
	void SetActive()
	{
		if ( g_Game.IsServer() )
		{
			m_WasActivatedOrDeactivated = true;
			
			m_IsInProgress = false;
			m_IsActive = true;
			CreateTrigger();
			if ( m_AddActivationDefect )
			{
				this.AddDefect();
			}
			RefreshState();
		}
	}

	void StartActivate( PlayerBase player )
	{
		if ( g_Game.IsServer() )
		{
			m_Timer = new Timer( CALL_CATEGORY_GAMEPLAY );
			if ( m_InitWaitTime > 0 )
			{
				m_IsInProgress = true;
				m_Timer.Run( m_InitWaitTime, this, "SetActive" );
				player.MessageStatus( m_InfoActivationTime );
			}
			else
			{
				SetActive();
			}
		}
	}

	void StartDeactivate( PlayerBase player )
	{
		if ( g_Game.IsServer() )
		{
			float rnd = Math.RandomFloat( 0,1 );
			
			if ( rnd < m_WrongManipulation )
			{
				OnDeactivateWrongManipulation( player );
			}
			else
			{
				player.MessageStatus( m_InfoDeactivated );
				this.SetInactive();
			}
		}
	}

	void SetInactive( bool stop_timer = true )
	{
		if ( g_Game.IsServer() )
		{
			m_WasActivatedOrDeactivated = true;
			
			m_IsActive = false;
			if ( m_Timer && stop_timer )
			{
				m_Timer.Stop();
			}
			g_Game.ObjectDelete( m_TrapTrigger );
			m_TrapTrigger = NULL;
			
			if ( m_AddDeactivationDefect )
			{
				this.AddDefect();
			}

			// de-attach attachments
			int attachments = AttachmentsCount();
			if ( attachments > 0 )
			{
				EntityAI attachment = GetAttachmentFromIndex(0);
				if (attachment)
				{
					this.DropEntity( attachment );
				}
			}

			this.RefreshState();
		}
	}

	void OnDeactivateWrongManipulation( PlayerBase player )
	{
		player.AddDamage( m_DamagePlayers );
		player.MessageStatus( m_InfoDamageManipulation );
	}

	void CreateTrigger()
	{
		m_TrapTrigger = g_Game.CreateObject( "TrapTrigger", this.GetPosition(), false );
		vector mins = "-0.15 -0.5 -0.15";
		vector maxs = "0.15 0.5 0.15";
		m_TrapTrigger.SetOrientation( this.GetOrientation() );
		m_TrapTrigger.SetExtents(mins, maxs);	
		m_TrapTrigger.SetParentObject( this );
	}

	void EEItemLocationChanged  ( EntityAI old_owner, EntityAI new_owner ) 
	{
		if ( g_Game.IsServer() )
		{
			this.RefreshState();

			// TAKE ACTIVE TRAP FROM VICINITY 
			if ( old_owner == NULL && new_owner != NULL && IsActive() )  // !!! lebo nefunguju zlozene podmienky v if-e
			{
				// TAKE INTO HANDS
				if ( new_owner.ClassName() == "PlayerBase" )
				{
					AddDamageToVictim( new_owner );
				}
				else // TAKE INTO BACKPACK, ETC ...  // !!! lebo nefunguje elseif
				{	
					if ( new_owner.GetOwnerPlayer().ClassName() == "PlayerBase" )
					{
						AddDamageToVictim( new_owner.GetOwnerPlayer() );
					}
				}
			}
		}
		
	}

	void EEItemAttached(EntityAI item, string slot_name)
	{
		super.EEItemAttached(item, slot_name);
		
		if ( g_Game.IsServer() )
		{
			// Print(item);
			// Print(slot_name);
			// Print( item.GetType() );
			// Print( ClassName(item) );
			this.RefreshState();
		}
	}	
	
	void EEItemDetached (EntityAI item, string slot_name)
	{
		super.EEItemDetached(item, slot_name);
		
		if ( g_Game.IsServer() )
		{
			// Print(item);
			// Print(slot_name);
			this.RefreshState();
		}
	}

	protected bool m_IsActive;
	protected bool m_IsInProgress;
	
	// ITEM CANNOT BE TAKEN WHEN IN PROGRESS
	bool ConditionIntoInventory ( EntityAI  player ) 
	{
		return IsTakeable();
	}
	
	bool ConditionIntoHands ( EntityAI player ) 
	{
		return IsTakeable();
	}

	bool ConditionOutOfHands ( EntityAI player ) 
	{
		return IsTakeable();
	}
	
	void OnPlacementFinished( PlayerBase player )
	{
		SetupTrapPlayer( player, false );
	}
	
	bool CanBePlaced( PlayerBase player, vector position )
	{
		return IsPlaceableAtPosition( position );
	}

	string CanBePlacedFailMessage( PlayerBase player, vector position )
	{
		return "Trap can't be placed on this surface type.";
	}
}

/*
TODO:
damage pasce sa nevyuziva
nastavit public, private
damage pre hraca do NOHY, HLAVY, atd
SET RADIUS
SERVER SYNC ?
custom radius
*/