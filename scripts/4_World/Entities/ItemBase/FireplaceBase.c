class EN5C_FireplaceBase extends ItemBase
{
	//State params
	protected bool m_IsBurning;   
	protected bool m_HasAshes; 	
	protected int m_FireIntensity; //0 - small, 1 - normal
	
	//TODO - add to heating and cooling processes !!!
	//Fireplace params
	protected float m_TemperatureLossMP;		//determines how fast will fireplace loose its temperature when cooling (lower is better)
	protected float m_FuelBurnRateMP;			//determines how fast will the fuel item burn before spending (lower is better)
	
	//cooking
	protected autoptr Cooking m_CookingProcess;
	
	//
	const float PARAM_SMALL_FIRE_TEMPERATURE 		= 150;		//maximum fireplace temperature of a small fire (degree Celsius)
	const float PARAM_NORMAL_FIRE_TEMPERATURE 		= 1000;		//maximum fireplace temperature of a normal fire (degree Celsius)
	const float PARAM_MIN_FIRE_TEMPERATURE 			= 40;		//minimum fireplace temperature under which the fireplace is inactive (degree Celsius)
	const float	PARAM_TEMPERATURE_INCREASE 			= 20;		//how much will temperature increase when fireplace is burning (degree Celsius)
	const float	PARAM_TEMPERATURE_DECREASE 			= 20;		//how much will temperature decrease when fireplace is cooling (degree Celsius)
	const float	PARAM_MAX_WET_TO_IGNITE 			= 0.2;		//maximum wetness value when the fireplace can be ignited
	const float PARAM_MIN_TEMP_TO_REIGNITE 			= 200;		//minimum fireplace temperature under which the fireplace can be reignited using air only (degree Celsius)
	const float	PARAM_IGNITE_RAIN_THRESHOLD 		= 0.05;		//maximum rain value when the fireplace can be ignited
	const float	PARAM_BURN_WET_THRESHOLD 			= 0.5;		//maximum wetness value when the fireplace is able to burn
	const float	PARAM_WET_INCREASE_COEF 			= 0.02;		//value for calculating of  wetness that fireplace gain when raining
	const float	PARAM_FIRE_CONSUM_RATE_AMOUNT		= 1.0;		//base value of fire consumption rate (how many base energy will be spent on each update)
	const float	PARAM_BURN_DAMAGE_COEF				= 0.05;		//value for calculating damage on items located in fireplace cargo
	const float	PARAM_CARGO_TEMP_INCREASE_COEF		= 10;		//value for calculating temperature increase on each heat update interval (degree Celsius)
	const float PARAM_MAX_CARGO_TEMP_INCREASE		= 200;		//maximum value of temperature of items in fireplace cargo when heating (degree Celsius)
	const float PARAM_HEAT_RADIUS 					= 2.5;		//radius in which objects are heated by fire
	const float PARAM_HEAT_WET_DECREASE_COEF		= 0.02;		//value for calculating wet loss when heating by fire
	
	//cooking
	const float PARAM_COOKING_TEMP_THRESHOLD		= 100;		//temperature threshold for starting coooking process (degree Celsius)
	const float PARAM_COOKING_EQUIP_MAX_TEMP		= 250;		//maximum temperature of attached cooking equipment (degree Celsius)
	const float PARAM_COOKING_EQUIP_TEMP_INCREASE	= 10;		//how much will temperature increase when attached on burning fireplace (degree Celsius)
	//
	const int 	TIMER_HEATING_UPDATE_INTERVAL 		= 2;		//update interval duration of heating process (seconds)
	const int 	TIMER_COOLING_UPDATE_INTERVAL 		= 2;		//update interval duration of cooling process (seconds)
	
	//Items on a stick (cooking)
	protected autoptr array<ItemBase> m_ItemsOnStick;			//items that are impaled on a stick and will be cooking if right conditions are met
	
	//Timers
	protected autoptr Timer m_HeatingTimer;
	protected autoptr Timer m_CoolingTimer;
	
	
	//Attachments
	protected ItemBase m_CookingEquipment;
	protected autoptr FireConsumable m_ItemToConsume;
	
	//Fireplace indoor effects
	protected string m_SmokeParticlePoint;
	protected string m_IndoorSlotName;
	
	//Particles
	protected Object m_SmallFire;
	protected Object m_NormalFire;
	const string PARTICLE_SMALL_FIRE 		= "graphics/particles/fire_small_camp_01.ptc";
	const string PARTICLE_NORMAL_FIRE		= "graphics/particles/fire_middle_camp_01.ptc";
	
	//Fuel
	//Kindling / Fuel item types
	protected autoptr array<FireConsumable> m_FireConsumables;
	protected autoptr array<FireConsumableType> m_KindlingTypes; 	
	protected autoptr array<FireConsumableType> m_FuelTypes; 		
	
	//Player action / status messages
	const string MESSAGE_IGNITE_IGNIT_ITEM_DAMAGED 		= "You cannot start fire. %1 is too damaged.";
	const string MESSAGE_IGNITE_NO_KINDLING 			= "There needs to be some kindling to start a fire.";
	const string MESSAGE_IGNITE_TOO_WET 				= "The fireplace is too wet to be ignited.";
	const string MESSAGE_IGNITE_RAIN	 				= "The fire went out because of the rain.";
	const string MESSAGE_IGNITE_WIND	 				= "The fire went out because of the wind.";
	const string MESSAGE_IGNITE_IN_WATER 				= "I cannot ignite the fireplace in the water.";
	const string MESSAGE_IGNITE_UNDER_ROOF 				= "I cannot ignite the fireplace here, it's not safe.";
	const string MESSAGE_IGNITE_FLARE_NOT_LIT 			= "I cannot ignite the fireplace because the flare is not lit.";
	const string MESSAGE_EXTINGUISH_NOT_ENOUGH_WATER 	= "There needs to be more water to extinguish the fireplace.";
	const string MESSAGE_REIGNITE_NO_KINDLING 			= "There needs to be some kindling to start a fire.";
	const string MESSAGE_REIGNITE_RAIN	 				= "Cannot reignite fireplace in rain.";
	const string MESSAGE_REIGNITE_TOO_WET 				= "The fireplace is too wet to be reignited.";
	const string MESSAGE_BURY_ASHES_FAILED_NOT_EMPTY	= "Cannot bury ashes. There are sill items in it.";
	const string MESSAGE_BURY_ASHES_FAILED_SURFACE		= "Cannot bury ashes on hard surface.";
	
	//Visual animations
	const string ANIMATION_ASHES 			= "Ashes";
	const string ANIMATION_KINDLING			= "Kindling";
	const string ANIMATION_STICKS			= "Sticks";
	const string ANIMATION_WOOD 			= "Wood";
	const string ANIMATION_BURNT_WOOD 		= "BurntWood";
	const string ANIMATION_STONES 			= "Stones";
	const string ANIMATION_TRIPOD 			= "Tripod";
	const string ANIMATION_OVEN 			= "Oven";
	const string ANIMATION_COOKWARE_HANDLE 	= "handleRotate";
	
	//Visual textures and materials
	const string MATERIAL_FIREPLACE_GLOW 		= "dz\\gear\\cooking\\data\\stoneground.rvmat";
	const string MATERIAL_FIREPLACE_NOGLOW 		= "dz\\gear\\cooking\\data\\stonegroundnoemit.rvmat";
	
	//Attachment types
	//Fuel
	const string ATTACHMENT_STICKS 		= "EN5C_WoodenStick";
	const string ATTACHMENT_FIREWOOD 	= "EN5C_Firewood";
	//Kindling
	const string ATTACHMENT_RAGS 		= "EN5C_Rag";
	const string ATTACHMENT_BANDAGE 	= "EN5C_Bandage";
	const string ATTACHMENT_BOOK	 	= "ItemBook";	//TODO
	const string ATTACHMENT_BARK_OAK 	= "EN5C_Bark_Oak";
	const string ATTACHMENT_BARK_BIRCH 	= "EN5C_Bark_Birch";
	const string ATTACHMENT_PAPER		= "Consumable_Paper"; //TODO
	//Other
	const string ATTACHMENT_TRIPOD 		= "EN5C_Tripod";
	const string ATTACHMENT_STONES 		= "EN5C_Stone";
	const string ATTACHMENT_COOKING_POT	= "EN5C_Pot";
	//
	const string OBJECT_CLUTTER_CUTTER 	= "Fireplace_ClutterCutter";
	
	//METHODS
	//Constructor
	protected void EN5C_FireplaceBase()
	{
		//define kindling types
		m_KindlingTypes = new array<FireConsumableType>; 	
		m_KindlingTypes.Insert( new FireConsumableType ( ATTACHMENT_RAGS, 7 ) );
		m_KindlingTypes.Insert( new FireConsumableType ( ATTACHMENT_BANDAGE, 7 ) );
		m_KindlingTypes.Insert( new FireConsumableType ( ATTACHMENT_BOOK, 10 ) );
		m_KindlingTypes.Insert( new FireConsumableType ( ATTACHMENT_BARK_OAK, 10 ) );
		m_KindlingTypes.Insert( new FireConsumableType ( ATTACHMENT_BARK_BIRCH, 7 ) );
		m_KindlingTypes.Insert( new FireConsumableType ( ATTACHMENT_PAPER, 5 ) );
		
		//define fuel types
		m_FuelTypes = new array<FireConsumableType>; 	
		m_FuelTypes.Insert( new FireConsumableType ( ATTACHMENT_STICKS, 13 ) );
		m_FuelTypes.Insert( new FireConsumableType ( ATTACHMENT_FIREWOOD, 50 ) );
		
		//define fuel / kindling items (fire consumables)
		m_FireConsumables = new array<FireConsumable>;
		
		//cooking
		//m_CookingProcess = NULL;
		m_ItemsOnStick = new array<ItemBase>;
		
		//init values
		SetBurningState( false );   
		SetAshesState( false );
		
		//default burning parameters
		SetTemperatureLossMP( 1.0 );
		SetFuelBurnRateMP( 1.0 );
	}

	protected void ~EN5C_FireplaceBase()
	{
		//stop timers
		delete m_HeatingTimer;
		delete m_CoolingTimer;
		
		//delete kindling types
		DeleteFireConsumableTypesInArray( m_KindlingTypes );
		
		//delete fuel types
		DeleteFireConsumableTypesInArray( m_FuelTypes );
		
		//delete fire consumables
		DeleteFireConsumablesInArray( m_FireConsumables );
		
		//stop particles
		NormalFireStop( false );
		SmallFireStop( false );
	}
	
	//on store save/load
	void OnStoreSave( ParamsWriteContext ctx )
	{   
		super.OnStoreSave(ctx);
		
		//Save ashes state
		autoptr Param1<bool> has_ashes = new Param1<bool>( m_HasAshes );
		ctx.Write( has_ashes.param1 );
		
		//Save temperature loss MP
		autoptr Param1<float> temp_loss_mp = new Param1<float>( GetTemperatureLossMP() );
		ctx.Write( temp_loss_mp.param1 );

		//Save fuel burn rate MP
		autoptr Param1<float> fuel_burn_rate_mp = new Param1<float>( GetFuelBurnRateMP() );
		ctx.Write( fuel_burn_rate_mp.param1 );
	}

	void OnStoreLoad( ParamsReadContext ctx )
	{
		super.OnStoreLoad(ctx);
		
		//Load ashes state
		autoptr Param1<bool> has_ashes = new Param1<bool>( false );
		ctx.Read( has_ashes );
		SetAshesState( has_ashes.param1 );	//set loaded value
		
		//Load temperature loss MP
		autoptr Param1<float> temp_loss_mp = new Param1<float>( 1.0 );
		ctx.Read( temp_loss_mp );
		SetTemperatureLossMP( temp_loss_mp.param1 );
		
		//Load fuel burn rate MP
		autoptr Param1<float> fuel_burn_rate_mp = new Param1<float>( 1.0 );
		ctx.Read( fuel_burn_rate_mp );
		SetFuelBurnRateMP( fuel_burn_rate_mp.param1 );
		
		//refresh visual after load
		RefreshFireplaceVisual();
	}
	
	//check fireplace types
	bool IsFireplace()
	{
		return true;
	}
	
	bool IsBaseFireplace()
	{
		return false;
	}
	
	bool IsBarrelHoles()
	{
		return false;
	}
	
	bool IsFireplaceIndoor()
	{
		return false;
	}

	void DestroyFireplace()
	{
		//delete object
		GetGame().ObjectDelete ( this );
	}

	//attachments conditions
	bool CanAttachItem ( ItemBase item )
	{
		return false;
	}

	bool CanDetachItem ( ItemBase item )
	{
		return false;
	}
	
	//cooking equipment
	ItemBase GetCookingEquipment()
	{
		return m_CookingEquipment;
	}
	
	void SetCookingEquipment( ItemBase equipment )
	{
		m_CookingEquipment = equipment;
	}
	
	void ClearCookingEquipment()
	{
		SetCookingEquipment( NULL );
	}
	
	// --- VISUAL
	//Fireplace visual
	protected void RefreshFireplaceVisual()
	{
		//ANIMATION_STICKS
		if ( IsItemAttached ( ATTACHMENT_STICKS ) )
		{
			SetAnimationPhase ( ANIMATION_STICKS, 0 );
		}
		else
		{
			SetAnimationPhase ( ANIMATION_STICKS, 1 );
		}
		
		//ANIMATION_WOOD / ANIMATION_BURNT_WOOD
		if ( IsItemAttached ( ATTACHMENT_FIREWOOD ) )
		{
			if ( IsBurning() ) 
			{
				SetAnimationPhase ( ANIMATION_BURNT_WOOD, 0 );
			}
			else
			{
				SetAnimationPhase ( ANIMATION_WOOD, 0 );
			}
		}
		else
		{
			SetAnimationPhase ( ANIMATION_WOOD, 1 );
			SetAnimationPhase ( ANIMATION_BURNT_WOOD, 1 );		
		}
		
		//kindling states
		if ( HasAshes() )
		{
			SetAnimationPhase ( ANIMATION_KINDLING, 1 );
			SetAnimationPhase ( ANIMATION_ASHES, 0 );
		}
		else
		{
			SetAnimationPhase ( ANIMATION_ASHES, 1 );
			
			int  kindling_items_count = GetKindlingCount();
			
			if ( kindling_items_count == 0 )
			{
				SetAnimationPhase ( ANIMATION_KINDLING, 1 );
			}
			else
			{
				SetAnimationPhase ( ANIMATION_KINDLING, 0 );
			}
		}
		
		//ANIMATION_STONES
		if ( IsItemAttached ( ATTACHMENT_STONES ) )
		{
			ItemBase attached_item = ( ItemBase ) GetAttachmentByConfigTypeName ( ATTACHMENT_STONES );
			float item_quantity = attached_item.GetQuantity2();
			
			//hide all
			if ( item_quantity < 4 )
			{
				SetAnimationPhase ( ANIMATION_STONES, 1 );
				SetAnimationPhase ( ANIMATION_OVEN, 1 );
			}
			
			//show stones
			if ( item_quantity >=4 && item_quantity < 8 )
			{
				SetAnimationPhase ( ANIMATION_OVEN, 1 );
				SetAnimationPhase ( ANIMATION_STONES, 0 );
			}
			
			//show oven
			if ( item_quantity >= 8 )
			{
				SetAnimationPhase ( ANIMATION_STONES, 1 );
				SetAnimationPhase ( ANIMATION_OVEN, 0 );
			}
		}
		else
		{
			SetAnimationPhase ( ANIMATION_STONES, 1 );
			SetAnimationPhase ( ANIMATION_OVEN, 1 );		
		}
		
		//ANIMATION_TRIPOD
		if ( IsItemAttached ( ATTACHMENT_TRIPOD ) )
		{
			SetAnimationPhase ( ANIMATION_TRIPOD, 0 );
		}
		else
		{
			SetAnimationPhase ( ANIMATION_TRIPOD, 1 );
		}
	}

	protected void RefreshFireVisual()
	{
		int fire_intensity = GetFireIntensity();
		
		if ( IsBurning() )
		{
			if ( fire_intensity == 0 )
			{
				SmallFireStart();
				NormalFireStop();
			}
			else
			{
				NormalFireStart();
				SmallFireStop();
			}
		}
		else
		{
			NormalFireStop();
			SmallFireStop();
		}
	}
	
	//Fireplace fire intensity
	protected void RefreshFireIntensity()
	{
		float temperature = GetTemperature();
		int fire_intensity = GetFireIntensity();
		
		//if is not burning
		if ( !IsBurning() ) return;
		
		//Small fire
		if ( temperature <= PARAM_SMALL_FIRE_TEMPERATURE && fire_intensity == 1 )
		{
			SetFireIntensity ( 0 );
			RefreshFireVisual();
		}
		
		//Big fire
		if ( temperature > PARAM_SMALL_FIRE_TEMPERATURE && fire_intensity == 0 )
		{
			SetFireIntensity ( 1 );
			RefreshFireVisual();
		}
	}

	//Particles
	//normal fire
	protected void NormalFireStart( bool enable_log = true )
	{
		vector pos = GetPosition();
		m_NormalFire = GetGame().CreateObject( "#particlesourceEnf", pos, true );
		
		if ( m_NormalFire )
		{
			vobject vobj = GetObject( PARTICLE_NORMAL_FIRE );
			m_NormalFire.SetObject( vobj, "" );
			ReleaseObject( vobj );
		}
				
		//debug
		if ( enable_log && GetGame().GetPlayer() )
		{
			PlayerBase player = ( PlayerBase ) GetGame().GetPlayer();
			if ( player ) player.MessageStatus( "Normal Fire Start" );	
		}
	}
	
	protected void NormalFireStop( bool enable_log = true )
	{
		if ( m_NormalFire && GetGame() )
		{
			GetGame().ObjectDelete( m_NormalFire );
		}
		
		//debug
		if ( enable_log && GetGame().GetPlayer() )
		{
			PlayerBase player = ( PlayerBase ) GetGame().GetPlayer();
			if ( player ) player.MessageStatus( "Normal Fire Stop" );	
		}
	}
	
	//small fire
	protected void SmallFireStart( bool enable_log = true )
	{
		vector pos = GetPosition();
		m_SmallFire = GetGame().CreateObject( "#particlesourceEnf", pos, true );
		
		if ( m_SmallFire )
		{
			vobject vobj = GetObject( PARTICLE_SMALL_FIRE );
			m_SmallFire.SetObject( vobj, "" );
			ReleaseObject( vobj );
		}		
				
		if ( enable_log && GetGame().GetPlayer() )
		{
			PlayerBase player = ( PlayerBase ) GetGame().GetPlayer();
			if ( player ) player.MessageStatus( "Small Fire Start" );	
		}
	}
	
	protected void SmallFireStop( bool enable_log = true )
	{
		if ( m_SmallFire && GetGame() )
		{
			GetGame().ObjectDelete( m_SmallFire );
		}
				
		if ( enable_log && GetGame().GetPlayer() )
		{
			PlayerBase player = ( PlayerBase ) GetGame().GetPlayer();
			if ( player ) player.MessageStatus( "Small Fire Stop" );	
		}
	}	
	
	
	// --- FUEL / KINDLING
	//Add to fire consumables
	protected void AddToFireConsumables ( ItemBase item )
	{
		float energy = GetFireConsumableEnergy ( item );
		m_FireConsumables.Insert ( new FireConsumable ( item, energy ) );
	}

	protected float GetFireConsumableEnergy ( ItemBase item )
	{
		//Kindling
		for ( int i = 0; i < m_KindlingTypes.Count(); i++ )
		{
			if ( item.IsKindOf ( m_KindlingTypes.Get ( i ).m_ItemType ) )
			{
				return m_KindlingTypes.Get ( i ).m_Energy;
			}
		}
		
		//Fuel
		for ( int j = 0; j < m_FuelTypes.Count(); j++ )
		{
			if ( item.IsKindOf ( m_FuelTypes.Get ( j ).m_ItemType ) )
			{
				return m_FuelTypes.Get ( j ).m_Energy;
			}
		}
		
		return 0;
	}

	//Remove from fire consumables
	protected void RemoveFromFireConsumables ( ItemBase item )
	{
		m_FireConsumables.RemoveItem ( GetFireConsumableByItem( item ) );
	}

	protected FireConsumable GetFireConsumableByItem( ItemBase item )
	{
		for ( int i = 0; i < m_FireConsumables.Count(); i++ )
		{
			if ( m_FireConsumables.Get( i ).m_Item == item )
			{
				return m_FireConsumables.Get( i );
			}
		}
		
		return NULL;
	}

	//Set fuel / kindling to consume
	//Sets the item with the lowest energy value as item that will be consumed next
	protected void SetItemToConsume()
	{
		float energy = 0;
		int fire_consumables_count = m_FireConsumables.Count();
		
		if ( fire_consumables_count == 0 )
		{
			m_ItemToConsume = NULL;	
			return;
		}
		
		for ( int i = 0; i < fire_consumables_count; i++ )
		{
			if ( i == 0 )
			{
				energy = m_FireConsumables.Get ( i ).m_Energy;		//Set first value
				
				m_ItemToConsume = m_FireConsumables.Get ( i );
			}
			else
			{
				if ( m_FireConsumables.Get ( i ).m_Energy < energy )
				{
					energy = m_FireConsumables.Get ( i ).m_Energy;
					
					m_ItemToConsume = m_FireConsumables.Get ( i );
				}	
			}
		}
	}

	protected FireConsumable GetItemToConsume()
	{
		return m_ItemToConsume;
	}

	//Spend item that is used as consumable for fire (fuel, kindling)
	//if 'amount == 0', the whole quantity will be consumed (quantity -= 1 )
	protected void SpendFireConsumable ( float amount )
	{
		//spend item
		FireConsumable fire_consumable = GetItemToConsume();
		
		if ( fire_consumable == NULL )
		{
			return;
		}
		
		ItemBase item = fire_consumable.m_Item;
		
		//Debug
		/*
		PlayerBase player = ( PlayerBase ) GetGame().GetPlayer();
		string message = "item type = " + item.GetType() + " m_RemainingEnergy = " + ftoa ( fire_consumable.m_RemainingEnergy ) + " quantity = " + ftoa ( item.GetQuantity2() ) + " amount = " + ftoa ( amount );
		player.MessageAction ( message );
		Print(message);
		*/
		
		//set remaining energy
		fire_consumable.m_RemainingEnergy = fire_consumable.m_RemainingEnergy - amount;
		
		float quantity = item.GetQuantity2();
		if ( fire_consumable.m_RemainingEnergy <= 0 || amount == 0 )
		{
			quantity = item.GetQuantity2() - 1;
			item.SetQuantity2 ( quantity, false );
			fire_consumable.m_RemainingEnergy = fire_consumable.m_Energy;
		}
		
		//if there is nothing left, delete and remove old consumable, set new fire consumable
		if ( quantity <= 0 )
		{
			//remove item from fuel items
			RemoveFromFireConsumables ( fire_consumable.m_Item );
				
			//delete item object and empty fire consumable item
			GetGame().ObjectDelete ( item );
			delete fire_consumable;
			
			//Set new item to consume
			SetItemToConsume();
		}
		
		//refresh visual
		RefreshFireplaceVisual();
	}

	//GetKindlingCount
	//Returns count of all kindling type items (define in 'm_KindlingTypes') attached to fireplace
	protected int GetKindlingCount()
	{
		int attachments_count = AttachmentsCount();
		int kindling_types_count = m_KindlingTypes.Count();
		int kindling_count = 0;
		
		for ( int i = 0; i < attachments_count; i++ )
		{
			ItemBase item = GetAttachmentFromIndex ( i );
			
			for ( int j = 0; j < kindling_types_count; j++ )
			{
				if ( item.IsKindOf ( m_KindlingTypes.Get ( j ).m_ItemType ) )
				{
					kindling_count++;
					break;
				}
			}
		}
		
		return kindling_count;
	}

	//GetFuelCount
	//Returns count of all fuel type items (define in 'm_FuelTypes') attached to fireplace
	protected int GetFuelCount()
	{
		int attachments_count = AttachmentsCount();
		int fuel_types_count = m_FuelTypes.Count();
		int fuel_count = 0;
		
		for ( int i = 0; i < attachments_count; i++ )
		{
			ItemBase item = GetAttachmentFromIndex ( i );
			
			for ( int j = 0; j < fuel_types_count; j++ )
			{
				if ( item.IsKindOf ( m_FuelTypes.Get ( j ).m_ItemType ) )
				{
					fuel_count++;
					break;
				}
			}
		}
		
		return fuel_count;
	}

	//returns if item attached to fireplace is kindling
	protected bool IsKindling ( ItemBase item )
	{
		int kindling_types_count = m_KindlingTypes.Count();
		
		for ( int i = 0; i < kindling_types_count; i++ )
		{
			if ( item.IsKindOf ( m_KindlingTypes.Get ( i ).m_ItemType ) ) 
			{
				return true;
			}
		}
		
		return false;
	}

	//returns if item attached to fireplace is fuel
	protected bool IsFuel ( ItemBase item )
	{
		int fuel_types_count = m_FuelTypes.Count();
		
		for ( int i = 0; i < fuel_types_count; i++ )
		{
			if ( item.IsKindOf ( m_FuelTypes.Get ( i ).m_ItemType ) ) 
			{
				return true;
			}
		}
		
		return false;
	}

	//Has attachments
	bool IsItemAttached ( string item_type )
	{
		int attachments_count = AttachmentsCount();
		
		for ( int i = 0; i < attachments_count; i++ )
		{
			ItemBase item = GetAttachmentFromIndex ( i );
			
			if ( item.IsKindOf ( item_type ) ) 
			{
				return true;
			}
		}
		
		return false;
	}

	//Has attachments of required quantity
	bool IsItemAttachedQuantity ( string item_type, float quantity )
	{
		int attachments_count = AttachmentsCount();
		
		for ( int i = 0; i < attachments_count; i++ )
		{
			ItemBase item = GetAttachmentFromIndex ( i );
			
			if ( item.IsKindOf ( item_type ) && item.GetQuantity2() >= quantity ) 
			{
				return true;
			}
		}
		
		return false;
	}

	//Has last attached item
	bool HasLastAttachment()
	{
		if ( AttachmentsCount() == 1 )
		{
			return true;
		}
		
		return false;
	}

	//Has last fuel/kindling attached
	bool HasLastFuelKindlingAttached()
	{
		int fuel_kindling_count = GetFuelCount() + GetKindlingCount();

		if ( fuel_kindling_count == 1 )
		{
			return true;
		}
		
		return false;
	}

	
	//	--- FIRE
	//Is fireplace burning?
	bool IsBurning()
	{
		return m_IsBurning;
	}
	
	//Set burning state
	protected void SetBurningState ( bool is_burning )
	{
		m_IsBurning = is_burning;	
	}

	//Are ashes present in fireplace?
	bool HasAshes()
	{
		return m_HasAshes;
	}

	//Set ashes state
	protected void SetAshesState ( bool has_ashes )
	{
		m_HasAshes = has_ashes;
	}

	//Temperature loss MP
	protected float GetTemperatureLossMP()
	{
		return m_TemperatureLossMP;
	}	
	
	protected void SetTemperatureLossMP( float value )
	{
		m_TemperatureLossMP = value;
	}
	
	//Fuel burn rate MP
	protected float GetFuelBurnRateMP()
	{
		return m_FuelBurnRateMP;
	}	
	
	protected void SetFuelBurnRateMP( float value )
	{
		m_FuelBurnRateMP = value;
	}
	
	//Get fire intensity 
	//0 - small, 1 - normal
	protected int GetFireIntensity()
	{
		return m_FireIntensity;
	}

	//Sets fire intensity value
	protected void SetFireIntensity ( int value )
	{
		m_FireIntensity = value;
	}

	//Start the fire process
	// 1. start heating
	// 2. heating
	// 3. stop heating
	void StartFire()
	{
		//Print("Starting fire...");
		
		if ( !IsBurning() )	
		{
			//set fuel to consume
			SetItemToConsume();
			
			//set fireplace paramas
			SetBurningState ( true );
			SetAshesState ( true );
			
			//turn light on
			SetPilotLight ( true );
			
			//start heating
			StartHeating();
		}
		
		//Refresh fire visual
		RefreshFireVisual();
	}

	//Start heating
	protected void StartHeating()
	{
		//Print("Starting heat...");
		
		//visual
		SetObjectMaterial ( 0, MATERIAL_FIREPLACE_GLOW );
		
		m_HeatingTimer = new Timer ( CALL_CATEGORY_GAMEPLAY );
		m_HeatingTimer.Run ( TIMER_HEATING_UPDATE_INTERVAL, this, "Heating", NULL, true );	
	}

	//Do heating
	protected void Heating()
	{
		//Debug
		/*
		PlayerBase player = ( PlayerBase ) GetGame().GetPlayer();
		player.MessageAction ( "Heating..." );
		Print("Heating...");
		*/
		
		float wetness = GetWet();
		float temperature = GetTemperature();
		int fire_consumables_count = m_FireConsumables.Count();
		float temperature_modifier = 0;
		
		//are all conditions met?
		if ( wetness > PARAM_BURN_WET_THRESHOLD || fire_consumables_count == 0 )
		{
			StopFire();
			return; 
		}

		//DEBUG
		/*
		string s_message = "-> fire amount = " + ftoa(PARAM_FIRE_CONSUM_RATE_AMOUNT) + " burn rate = " + ftoa(GetFuelBurnRateMP());
		player.MessageAction (s_message);
		Print ( s_message );
		*/
		
		//spend actual fire consumable
		float amount = PARAM_FIRE_CONSUM_RATE_AMOUNT * GetFuelBurnRateMP();
		SpendFireConsumable ( amount );
		
		//set wetness if raining and alter temperature modifier (which will lower temperature increase because of rain)
		bool is_raining_over = LineHit ( 100 );		//sky height threshold is set to 100 meters
		bool rain = GetGame().GetRain();
		if ( rain >= PARAM_BURN_WET_THRESHOLD && is_raining_over )
		{
			//set wet to firplace
			wetness = wetness + ( PARAM_WET_INCREASE_COEF * rain );
			wetness = Math.Clamp ( wetness, 0, 1 );  //wetness <0-1>
			SetWet ( wetness );
			
			//set temperature modifier
			temperature_modifier = PARAM_TEMPERATURE_INCREASE * rain;
		}
		
		//calculate and set temperature
		temperature = temperature + PARAM_TEMPERATURE_INCREASE - temperature_modifier;		//calculate new temperature 
		if ( GetFuelCount() == 0 )
		{
			temperature = Math.Clamp ( temperature, 0, PARAM_SMALL_FIRE_TEMPERATURE );		//small fire
		}
		else
		{
			temperature = Math.Clamp ( temperature, 0, PARAM_NORMAL_FIRE_TEMPERATURE );		//normal fire
		}
		SetTemperature ( temperature );
		
		//check fire intensity
		RefreshFireIntensity();
		
		//damage cargo items
		BurnItemsInCargo();
		
		//transfer heat to near players, cook food on stick
		TransferHeatToNearObjects();
		
		//manage cooking equipment
		if ( m_CookingEquipment )
		{
			float cook_equip_temp = m_CookingEquipment.GetTemperature();
			
			if ( cook_equip_temp >= PARAM_COOKING_TEMP_THRESHOLD )
			{
				//start cooking
				CookWithEquipment();
			}
			
			//DEBUG
			/*
			PlayerBase player = ( PlayerBase ) GetGame().GetPlayer();
			string s_message = "cooking equipment temp = " + ftoa(cook_equip_temp) + " | PARAM_COOKING_TEMP_THRESHOLD = " + ftoa(PARAM_COOKING_TEMP_THRESHOLD);
			if (player != NULL) {player.MessageAction (s_message);}
			Print ( s_message );
			*/
			
			//set temperature to cooking equipment
			cook_equip_temp = cook_equip_temp + PARAM_COOKING_EQUIP_TEMP_INCREASE;
			if ( temperature <= PARAM_SMALL_FIRE_TEMPERATURE )
			{
				cook_equip_temp = Math.Clamp ( cook_equip_temp, 0, PARAM_SMALL_FIRE_TEMPERATURE );
			}
			else
			{
				cook_equip_temp = Math.Clamp ( cook_equip_temp, 0, PARAM_COOKING_EQUIP_MAX_TEMP );
			}
			m_CookingEquipment.SetTemperature ( cook_equip_temp );
		}
		
		//manage cooking on stick
		if ( m_ItemsOnStick.Count() > 0 )
		{
			CookOnStick();
		}
	}
	
	//Stop the fire process
	// 1. start cooling
	// 2. cooling
	// 3. stop cooling
	void StopFire()
	{
		//Debug
		//PlayerBase player = ( PlayerBase ) GetGame().GetPlayer();
		//if (player != NULL) {player.MessageAction ( "Stop fire..." );}
		//Print("Stop fire...");
		
		//Stop heating
		StopHeating();
		
		//spend item
		SpendFireConsumable ( 0 );
		
		//turn fire off
		SetBurningState ( false );
		
		//start cooling
		StartCooling();
		
		//Refresh fire visual
		RefreshFireVisual();
	}
	
	
	//Stop heating
	protected void StopHeating()
	{
		//Debug
		//PlayerBase player = ( PlayerBase ) GetGame().GetPlayer();
		//if (player != NULL) {player.MessageAction ( "Stop heating..." );}
		//Print("Stop heating...");
		
		//Stop heating
		m_HeatingTimer.Stop();
		//delete m_HeatingTimer;
		delete m_HeatingTimer;
	}	

	//Start cooling
	protected void StartCooling()
	{
		//Debug
		//PlayerBase player = ( PlayerBase ) GetGame().GetPlayer();
		//if (player != NULL) {player.MessageAction ( "Start cooling..." );}
		//Print("Start cooling...");
		
		//Start cooling
		m_CoolingTimer = new Timer ( CALL_CATEGORY_GAMEPLAY );
		m_CoolingTimer.Run ( TIMER_COOLING_UPDATE_INTERVAL, this, "Cooling", NULL, true );		
	}

	//Start cooling       
	protected void Cooling()
	{
		//Debug
		/*
		PlayerBase player = ( PlayerBase ) GetGame().GetPlayer();
		player.MessageAction ( "Cooling..." );
		Print("Cooling...");
		*/
		
		float wetness = GetWet();
		float temperature = GetTemperature();
		float temperature_modifier = 0;
		
		if ( this && !IsBurning() && temperature >= PARAM_MIN_FIRE_TEMPERATURE )
		{
			//check wetness
			//set wetness if raining and alter temperature modifier (which will lower temperature increase because of rain)
			bool is_raining_over = LineHit ( 100 );		//sky height threshold is set to 100 meters
			bool rain = GetGame().GetRain();
			if ( is_raining_over )
			{
				//set wet to firplace
				wetness = wetness + ( PARAM_WET_INCREASE_COEF * rain );
				wetness = Math.Clamp ( wetness, 0, 1 );  //wetness <0-1>
				SetWet ( wetness );
				
				//set temperature modifier
				temperature_modifier = temperature_modifier + ( PARAM_TEMPERATURE_DECREASE * rain );
			}
			
			//calculate already obtained wetness (e.g. extinguished by water)
			temperature_modifier = temperature_modifier + ( PARAM_TEMPERATURE_DECREASE * wetness );
			
			//calculate and set temperature (decrease)
			temperature = temperature - PARAM_TEMPERATURE_DECREASE - temperature_modifier * GetTemperatureLossMP();		//calculate new temperature 
			temperature = Math.Clamp ( temperature, 0, PARAM_NORMAL_FIRE_TEMPERATURE );		//cannot be bigger than normal fire temperature and lower than zero
			SetTemperature ( temperature );		
			
			//cook with equipment if temperature of equipment is still high
			if ( m_CookingEquipment )
			{
				float cook_equip_temp = m_CookingEquipment.GetTemperature();
				
				if ( cook_equip_temp >= PARAM_COOKING_TEMP_THRESHOLD )
				{
					//cook
					CookWithEquipment();
				}
			}
			
			//cook on a stick if temperature of fireplace is still high
			if ( temperature >= PARAM_COOKING_TEMP_THRESHOLD && m_ItemsOnStick.Count() > 0 )
			{
				CookOnStick();
			}
			
			//turn off glow effect
			if ( temperature <= PARAM_MIN_TEMP_TO_REIGNITE && IsPilotLight() )
			{
				//visual
				SetObjectMaterial ( 0, MATERIAL_FIREPLACE_NOGLOW );
				
				//turn light off
				SetPilotLight ( false );
			}
		}
		else
		{
			//stop cooling
			StopCooling();
		}
	}

	protected void StopCooling()
	{
		//Debug
		//PlayerBase player = ( PlayerBase ) GetGame().GetPlayer();
		//if (player != NULL) {player.MessageAction ( "Stop cooling..." );}
		//Print("Stop cooling...");
		
		//Stop cooling
		m_CoolingTimer.Stop();
		
		//delete m_CoolingTimer;
		delete m_CoolingTimer;
	}

	//Cooking
	protected void CookWithEquipment()
	{
		if ( m_CookingProcess == NULL )
		{
			m_CookingProcess = new Cooking;
		}
		
		m_CookingProcess.CookWithEquipment ( m_CookingEquipment );
	}

	protected void CookOnStick()
	{
		if ( m_CookingProcess == NULL )
		{
			m_CookingProcess = new Cooking;
		}
		 
		m_CookingProcess.CookOnStick ( m_ItemsOnStick );
	}

	protected void BurnItemsInCargo()
	{
		Cargo cargo = GetCargo();
		
		for ( int i = 0; i < cargo.GetItemCount(); i++ )
		{
			ItemBase item = cargo.GetItem ( i );
			//set damage
			item.AddDamage ( PARAM_BURN_DAMAGE_COEF );
			
			//set temperature
			float temperature = item.GetTemperature() + PARAM_CARGO_TEMP_INCREASE_COEF;
			temperature = Math.Clamp ( temperature, 0, PARAM_MAX_CARGO_TEMP_INCREASE );
			item.SetTemperature ( temperature );
		}
	}

	protected void TransferHeatToNearObjects()
	{
		autoptr array<Object> nearest_objects = new array<Object>;
		autoptr array<Cargo> proxy_cargos = new array<Cargo>;		//mandatory parameter
		GetGame().GetObjectsAtPosition ( GetPosition(), PARAM_HEAT_RADIUS, NULL, nearest_objects, proxy_cargos ); 
		
		for ( int i = 0; i < nearest_objects.Count(); i++ )
		{
			if ( nearest_objects.Get ( i ).IsKindOf ( "PlayerBase" ) )
			{
				PlayerBase player = nearest_objects.Get ( i );
				
				//TODO - player set/get wet
				//float wetness = player.GetWet();
				float wetness = player.GetVariableFloat ( "wet" );
				float distance = vector.Distance ( player.GetPosition(), GetPosition() );
				
				//wet
				if ( wetness > 0 )
				{
					float heat = PARAM_HEAT_WET_DECREASE_COEF * ( PARAM_HEAT_RADIUS - distance );
					wetness = wetness - heat;
					wetness = Math.Clamp ( wetness, 0, 1 );
					//player.SetWet ( wetness );
					player.SetVariable ( "wet", wetness );
				}
			}
		}
		
	}

	
	// --- SUPPORT
	//Check if object is under a roof (height check)
	bool LineHit ( float height )
	{
		vector from = GetPosition();
		vector to = GetPosition();
		to[1] = to[1] + height;
		vector contactPos;
		vector contactDir;
		int contactComponent;
		
		return GetGame().Raycast ( from, to, contactPos, contactDir, contactComponent, NULL, NULL, this );
	}

	//Is player facing fireplace
	bool IsFacingFireplace ( PlayerBase player )
	{
		vector fireplace_pos = GetPosition();
		vector player_pos = player.GetPosition();
		vector dir_vec = vector.Direction( fireplace_pos, player_pos );	//calculate direction vector
		dir_vec.Normalize();										//normalize direction vector
		
		//check zero vector
		if ( dir_vec.Length() == 0 )
		{
			return false;
		}
		
		float dot = vector.Dot ( player.GetOrientation(), dir_vec );
		if ( Math.Acos ( dot ) < 0.436332313 ) 		//0.436332313 => 25 degrees 
		{
			return true;
		}
		
		return false;
	}
	
	//! Delete all objects of type FireConsumable in array (script not game objects).
	static void DeleteFireConsumablesInArray( array<FireConsumable> objects )
	{
		for ( int i = 0; i < objects.Count(); i++ )
		{
			if ( objects.Get( i ) )
			{
				delete objects.Get( i );
			}
		}
	}
	
	//! Delete all objects of type FireConsumableType in array (script not game objects).
	static void DeleteFireConsumableTypesInArray( array<FireConsumableType> objects )
	{
		for ( int i = 0; i < objects.Count(); i++ )
		{
			if ( objects.Get( i ) )
			{
				delete objects.Get( i );
			}
		}
	}
	
	// --- ACTIONS
	// --- Actions / Action conditions
	//Check if there is some kindling attached
	bool CheckKindling()
	{
		if ( GetKindlingCount() > 0 )
		{
			return true;
		}
		
		return false;
	}

	bool CheckWind()
	{
		//check wind
		vector wind = GetGame().GetWind();
		float wind_strength = wind[0] + wind [1] + wind[2];
		
		//calculate probability
		//fail
		float probability_fail = (( wind_strength / 14 ) * 0.5);
		probability_fail = Math.Clamp ( probability_fail, 0, 1 ); //clamp 0-1
		//success
		float probability_success = Math.RandomFloat01();
		
		if ( probability_fail < probability_success )
		{
			return true;
		}
		
		return false;
	}

	//Check if there is some kindling attached
	bool CheckWetness()
	{
		if ( GetWet() < PARAM_MAX_WET_TO_IGNITE )
		{
			return true;
		}
		
		return false;
	}

	//Check if there is any roof above fireplace
	bool CheckRoof()
	{
		//check roof - 4 meters above
		return !LineHit ( 4 );
	}

	//Check if it's raining and there is only sky above fireplace
	bool CheckRain()
	{
		//check rain
		if ( GetGame().GetRain() < PARAM_IGNITE_RAIN_THRESHOLD && !LineHit ( 100 ) )
		{
			return true;			
		}
		
		return false;
	}

	//Check there is no water surface bellow fireplace
	bool CheckSurface()
	{
		//check surface
		string surface_type;
		vector fireplace_pos = GetPosition();
		GetGame().SurfaceGetType ( fireplace_pos[0], fireplace_pos[2], surface_type ); 
		if ( surface_type != "FreshWater" && surface_type != "sea" )
		{
			return true;
		}
		
		return false;
	}

	//Checks if has not additional items in it
	bool IsEmpty()
	{
		//check cargo and attachments
		Cargo cargo = GetCargo();
		if ( cargo.GetItemCount() == 0 && AttachmentsCount() == 0 )
		{
			return true;			
		}
		
		return false;
	}
	
	//Checks if it has no items in cargo
	bool IsCargoEmpty()
	{
		//check cargo
		Cargo cargo = GetCargo();
		if ( cargo.GetItemCount() == 0 )
		{
			return true;			
		}
		
		return false;
	}

	//
	//Add item to items-on-stick array
	void AddToItemsOnStick ( ItemBase item )
	{
		m_ItemsOnStick.Insert ( item );
	}

	//Remove item from items-on-stick array
	void RemoveFromItemsOnStick ( ItemBase item )
	{
		m_ItemsOnStick.RemoveItem ( item );
	}
	
	//Check if object is in animation phase
	bool IsInAnimPhase( string anim_phase )
	{
		if ( GetAnimationPhase ( anim_phase ) == 0 )
		{
			return true;
		}
		
		return false;
	}
}
