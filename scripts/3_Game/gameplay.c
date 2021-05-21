/** @file */


const int INDEX_NOT_FOUND = -1;
//-----------------------------------------------------------------------------
typedef Serializer ParamsReadContext;
typedef Serializer ParamsWriteContext;


//-----------------------------------------------------------------------------
class Action
{
	proto bool GetText(Man player, out string text);
	proto bool GetTextRich(Man player, out string text);
	proto native int AddRef();
	proto native int Release();
};

class TotalDamageResult
{
	proto native float GetDamage(string zoneName, string healthType);
	proto native float GetHighestDamage(string healthType);
};

/*class TRefArray<class T> extends array<T>
{
	void ~TRefArray()
	{
		for (int i = 0; i < Count(); i++)
		{
			T item = Get(i);
			item.Release();
		}
	}
};

typedef TRefArray<Action> TActionRefArray;*/

class TActionRefArray extends array<Action>
{
	void AddRef()
	{
		for (int i = 0; i < Count(); i++)
		{
			Action item = Get(i);
			item.AddRef();
		}
	}

	void Release()
	{
		for (int i = 0; i < Count(); i++)
		{
			Action item = Get(i);
			item.Release();
		}

		Clear();
	}

	void ~TActionRefArray()
	{
		Release();
	}
};

//-----------------------------------------------------------------------------
class ActionMenuContext
{
	proto void AddAction(string text, int action_id, Param params, float priority, bool showWindow, bool hideOnUse, array<int> memoryPoints = NULL);
	private void ~ActionMenuContext()
	{
	}
};

class ActionParams
{

};

const string NullStringArray[1] = { "" };

//-----------------------------------------------------------------------------
//! Selection class
class Selection
{
	proto native string GetName();
	proto native int GetVertexCount();
	proto native int GetLODVertexIndex(int sel_vertex_index);

	vector GetVertexPosition(LOD lod, int index)
	{
		int lodIndex = GetLODVertexIndex(index);
		return lod.GetVertexPosition(lodIndex);
	}
};

//-----------------------------------------------------------------------------
//! LOD class
class LOD
{
	proto native int GetSelectionCount();
	proto native bool GetSelections(notnull out array<Selection> selections);

	proto native vector GetVertexPosition(int vertex_index);
	
	proto native string GetName(Object myObject);
	
	Selection GetSelectionByName( string name )
	{
		array<Selection> selections = new array<Selection>;
		GetSelections( selections );
		
		for ( int i = 0; i < selections.Count(); ++i )
		{
			string selection_name = selections.Get( i ).GetName();
			if ( selection_name.ToLower() == name.ToLower() ) return selections.Get( i );
		}
		
		return NULL;
	}
}

//-----------------------------------------------------------------------------
//! Object class
class Object extends IEntity
{
	proto private void ~Object();
	proto private void Object();
	
	//! Retrieve all LODS	
	proto native bool GetLODS(notnull out array<LOD> lods);
	
	//! Retrieve LOD name
	proto native string GetLODName(LOD lod);
	
	//! Retrieve LOD by given name
	LOD GetLODByName( string name )
	{
		array<LOD> lods = new array<LOD>;
		GetLODS( lods );
		
		for ( int i = 0; i < lods.Count(); ++i )
		{
			string lod_name = GetLODName( lods.Get( i ) );
			if ( lod_name.ToLower() == name.ToLower() ) return lods.Get( i );	
		}
		
		return NULL;
	}
	
	//! Retrieve position
	proto native vector GetPosition();
	
	//! Set position
	proto native void SetPosition(vector vPos);
	
	/**
	\brief Place object on surface
	*/
	proto native void PlaceOnSurface();

	/**
	\brief Retrieve orientation (yaw, pitch, roll) in <b>degrees</b>
	\return vector of yaw, pitch and roll angles (x = yaw, y = pitch, z = roll)
	*/
	proto native vector GetOrientation();

	/**
	\brief Set orientation (yaw, pitch, roll) in <b>degrees</b>
	@param vOrientation, vector of yaw, pitch and roll angles (x = yaw, y = pitch, z = roll)
	*/
	proto native void SetOrientation(vector vOrientation);
	
	//! Retrieve direction vector
	proto native vector GetDirection();
	
	/**
	\brief Set direction of object
	@param vDirection, direction vector
	\note object up vector is always y-axis
	*/
	proto native void SetDirection(vector vPos);
	
	//! Calculate local position of other entity to this entity
	proto native vector GetLocalPos(vector vPos);
	//! Calculate global position to this entity from local position
	proto native vector GetGlobalPos(vector vPos);
	
	//! Retrieve object's speed in global space
	proto native vector GetSpeed();
	
	//! Retrieve object's speed in local space
	proto native vector GetModelSpeed();
	
	//! Returns true if object's geometry can affect pathgraph generation
	proto native bool CanAffectPathgraph();
	
	//! Sets if object's geometry affects pathgraph generation. If first parameter is true, it ignores second parameter and sets affecting pathgraph by parameter set in object's config
	proto native void SetAffectPathgraph(bool fromConfig, bool affect);
	

	/**
	\brief Get Clipping info
	@param minMax array[2] of vectors, into minMax[0] is returned min, into minMax[1] is returned max
	\return radius of bounding box
	@code
	vector minMax[2];
	float radius = GetGame().GetPlayer().ClippingInfo(minMax);
	Print(minMax);
	Print(radius);
	@endcode
	*/
	proto float ClippingInfo(out vector minMax[2]);
	
	/** 
	\brief Gets collision bounding box
	@param minMax array[2] of vectors, into minMax[0] is returned min, into minMax[1] is returned max
	\return true if collision box exists, false otherwise
	@code
	vector minMax[2];
	if(GetGame().GetPlayer().GetCollisionBox(minMax))
		Print("has box");
	@endcode
	*/
	proto native bool GetCollisionBox(out vector minMax[2]);
	
	//! Set object damage (high level, network aware), call only from server
	proto native void SetDamage(float damage);
	
	//! Get total damage (clipped to range 0..1)
	proto native float GetDamage();
	
	//! Get damage level (range 0..4, 0 = pristine, 1 = worn, 2 = damaged, 3 = badly damaged, 4 = ruined)
	proto native int GetDamageLevel();
	
	//! Add damage to total damage (clipped to range 0..1) auto_delete = true If damage is 1 or more, object will be deleted automaticly
	float AddDamage(float damage, bool auto_delete = false)
	{
		float obj_dmg = GetDamage();
		obj_dmg = obj_dmg + damage;
			
		SetDamage(obj_dmg);
		
		if ( auto_delete && obj_dmg >= 1 )
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(GetGame(), "ObjectDelete", new Param1<Object>(this)); 
		}
		
		return obj_dmg;
	}
	
	proto native void GetSelectionList(out TStringArray selectionList);
	
	//! Returns true if the given selection was found in the p3d file. False if otherwise.
	bool HasSelection( string selection )
	{
		autoptr TStringArray selections = new TStringArray;
		GetSelectionList( selections );
		
		for ( local int i = 0; i < selections.Count(); ++i )
		{
			if ( selections.Get( i ) == selection )
			{
				return true;
			}
		}
		
		return false;
	}
	
	//! Searches object's config for the given animation name. Returns true if it was found, or false if not.
	bool HasAnimation( string anim_name )
	{
		string cfg_path_vehicles = "CfgVehicles " + GetType() + " ";
		if ( GetGame().ConfigIsExisting (cfg_path_vehicles)  &&  GetGame().ConfigIsExisting (cfg_path_vehicles + "AnimationSources " + anim_name) )
		{
			return true;
		}
		
		string cfg_path_weapons = "CfgWeapons " + GetType() + " ";
		if ( GetGame().ConfigIsExisting (cfg_path_weapons)  &&  GetGame().ConfigIsExisting (cfg_path_weapons + "AnimationSources " + anim_name) )
		{
			return true;
		}
		
		string cfg_path_magazines = "CfgMagazines " + GetType() + " ";
		if ( GetGame().ConfigIsExisting (cfg_path_magazines)  &&  GetGame().ConfigIsExisting (cfg_path_magazines + "AnimationSources " + anim_name) )
		{
			return true;
		}
		
		return false;
	}
	
	//! If Returns true if this item has EnergyManager in its config. Otherwise returns false.
	bool HasEnergyManager()
	{
		return false;
	}
	
	proto native int GetMemoryPointIndex(string memoryPointName);

	/**
  \brief [Client] This function is called by engine to get list of scripted actions
	@param ctx action menu context
	@param player which is asking for actions
	\n usage :
	@code
	void OnGetActions(ActionMenuContext ctx, Man player)
	{
		super.OnGetActions(ctx, player);
		if (IsActionCopyAvalible()) 
		{
			autoptr Param4<string, int, EntityAI, EntityAI> p = new Param4<string, int, EntityAI, EntityAI>("ahoj", 5, this, player);
			ctx.AddAction("Copy", ACTION_ID_COPY, p, 1000, true, true);
		}
	}
	@endcode
	*/
	void OnGetActions(ActionMenuContext ctx, Man player)
	{
	}

	/**
  \brief [Server] Function called by engine to execute scripted action
	@param action_id ID of action given in OnGetActions
	@param player which is executing action
	@param params custom action params given in OnGetActions 
	\n usage :
	@code
	bool OnAction(int action_id, Man player, ParamsReadContext ctx)
	{
		switch (action_id)
		{
			case ACTION_ID_COPY:
				if (IsActionCopyAvalible())
				{
					autoptr Param4<string, int, EntityAI, EntityAI> copy_action_params = new Param4<string, int, EntityAI, EntityAI>("", 0, NULL, NULL);
					if (ctx.Read(copy_action_params))
					{
						PrintString("Copy test: " + copy_action_params.param1 + "  " + copy_action_params.param2.ToString());
						Print(copy_action_params.param3);
						Print(copy_action_params.param4);
						GetGame().CreateObject("Trap_Bear", player.GetPosition(), false);
						return true;
					}
				}
				return false;
		}

		return super.OnAction(action_id, player, ctx);
	}
	@endcode
	*/
	bool OnAction(int action_id, Man player, ParamsReadContext ctx)
	{
	}

	//! Get config class of object
	string GetType()
	{
		string ret;
		GetGame().ObjectGetType(this, ret);
	
		return ret;
	}
	
	//! Get display name of entity
	string GetName()
	{
		string tmp;
		GetGame().ObjectGetDisplayName(this, tmp);
		return tmp;
	}


	int Release()
	{
		return GetGame().ObjectRelease(this);
	}
	
	proto native int AddRef();
	
	//! Check config class name of the object
	bool IsKindOf(string type)
	{
		return GetGame().ObjectIsKindOf(this, type);
	}
		
	// Check alive state
	bool IsAlive()
	{
		if ( GetDamage() >= 1 ) 
		{
			return false; // Is dead
		}
		return true; // Is alive
	}

		
	//! Returns if this entity is Man
	bool IsMan()
	{
		return false;
	}
	
	//! Returns if this entity is EntityAI
	bool IsEntityAI()
	{
		return false;
	}
	
	//! Returns if this entity is Static Transmitter
	bool IsStaticTransmitter()
	{
		return false;
	}
	
	//! Returns if this entity is ItemBase
	bool IsItemBase()
	{
		return false;
	}
	
	bool IsContainer()
	{
		return false;
	}
	
	//! Returns if this entity is Magazine
	bool IsMagazine()
	{
		return false;
	}
	
	//! Returns if this entity is InventoryItem
	bool IsInventoryItem()
	{
		return false;
	}

	//! Returns if this entity is Clothing
	bool IsClothing()
	{
		return false;
	}

	//! Returns if this entity is fireplacebase
	bool IsFireplace()
	{	
		return false;
	}
	
	//! Returns if this entity is tree
	bool IsTree()
	{	
		return false;
	}
	
	//! Returns if this entity is rock
	bool IsRock()
	{	
		return false;
	}
	
	//! Returns whether object is PlanBase
	bool IsWoodBase()
	{
		return false;
	}

	//! Returns if this entity is bush
	bool IsBush()
	{
		return false;
	}
	
	//! Returns if this entity is Pelt_Base
	bool IsPeltBase()
	{
		return false;
	}

	//! Returns if this entity is Weapon
	bool IsWeapon()
	{
		return false;
	}
	//! Remote procedure call shortcut, see CGame.RPC / CGame.RPCSingleParam
	void RPC(int rpc_type, array<Param> params, Man recipient = NULL)
	{
		GetGame().RPC(this, rpc_type, params, recipient);
	}
	
	//! Remote procedure call shortcut, see CGame.RPCSingleParam / CGame.RPC
	void RPCSingleParam(int rpc_type, Param param, Man recipient = NULL)
	{
		GetGame().RPCSingleParam(this, rpc_type, param, recipient);
	}
	
	/**
  \brief Called after remote procedure call is recieved for this object
		@param rpc_type user defined identification of RPC
		@param ctx read context for params
	*/
	void OnRPC(int rpc_type, ParamsReadContext ctx);

	vector GetSelectionPosition(string name)
	{
		return GetGame().ObjectGetSelectionPosition(this, name);
	}
	
	vector ModelToWorld(vector modelPos)
	{
		return GetGame().ObjectModelToWorld(this, modelPos);
	}
	
	vector WorldToModel(vector worldPos)
	{
		return GetGame().ObjectWorldToModel(this, worldPos);
	}

	// config class API	
	
	proto string ConfigGetString(string entryName);
	proto int ConfigGetInt(string entryName);
	bool ConfigGetBool(string entryName)
	{
		if ( ConfigGetInt(entryName) == 1 )
		{
			return true;
		}
		
		return false;
	}

	proto float ConfigGetFloat(string entryName);
	proto vector ConfigGetVector(string entryName);
	//proto ParamEntry ConfigGetEntry(string entryName);

	/**
  \brief Get array of strings from config entry.
	@param entryName 
	@param value output
	*/
	proto native void		ConfigGetTextArray(string entryName, out TStringArray values);

	/**
  \brief Get array of floats from config entry.
	@param entryName 
	@param value output
	*/
	proto native void		ConfigGetFloatArray(string entryName, out TFloatArray values);
	
	/**
  \brief Get array of integers from config entry.
	@param entryName 
	@param value output
	*/
	proto native void		ConfigGetIntArray(string entryName, out TIntArray values);

	/**
  \brief Checks if given entry exists.
	@param entryName 
	*/
	proto native bool		ConfigIsExisting(string entryName);
	
	/**
  \brief Checks if object is destroyed.
	*/
	proto native bool   IsDamageDestroyed();
	
	/**
  \brief Returns current state of health.
	@param zoneName if empty string, returns state of global health
	@param healthType if empty string, returns state of main health
	*/
	proto native float  GetHealth(string zoneName, string healthType);
	
	/**
  \brief Returns maximum value of health.
	@param zoneName if empty string, returns state of global health
	@param healthType if empty string, returns state of main health
	*/
	proto native float  GetMaxHealth(string zoneName, string healthType);

	/**
  \brief Sets current state of health.
	@param zoneName if empty string, sets state of global health
	@param healthType if empty string, sets state of main health
	*/
	proto native void   SetHealth(string zoneName, string healthType, float value);
	
	/**
  \brief Adds health.
	@param zoneName if empty string, sets state of global health
	@param healthType if empty string, sets state of main health
	*/
	proto native void   AddHealth(string zoneName, string healthType, float value);

	/**
  \brief Decreases health.
	@param zoneName if empty string, sets state of global health
	@param healthType if empty string, sets state of main health
	*/
	proto native void   DecreaseHealth(string zoneName, string healthType, float value);

	/**
  \brief Applies damage on object.
	@param damageType DT_CLOSE_COMBAT/DT_FIRE_ARM/DT_EXPLOSION/DT_CUSTOM
	@param source source of damage
	@param compomentName which component was hit
	@param ammoName ammoType, which defines how much damage should be applied
	@param directHitModelPos local position of hit
	@param damageCoef multiplier of applied damage
	*/
	proto native void   ProcessDirectDamage(int damageType, EntityAI source, string componentName, string ammoName, vector modelPos, float damageCoef = 1.0);
	
	
	proto native string GetDamageZoneNameByComponentIndex(int componentIndex);
	
	
	//! Compares config class name to given string
	bool KindOf( string tag )
	{
		bool found = false;
		string item_name = this.GetType();	
		autoptr TStringArray item_tag_array = new TStringArray;
		ConfigGetTextArray("cfgVehicles " + item_name + " itemInfo", item_tag_array);	
		
		int array_size = item_tag_array.Count();
		for (int i = 0; i < array_size; i++)
		{
			if ( item_tag_array.Get(i) == tag )
			{
				found = true;
				break;
			}
		}
		return found;
	}
};

class Plant extends Object
{
};

/*
class ParamEntry
{
	proto string GetString(string entryName);
	proto int GetInt(string entryName);
	proto float GetFloat(string entryName);
	proto autoptr ParamEntry GetEntry(string entryName); 
	proto int GetNumChildren();
	proto autoptr ParamEntry GetNumChildren(int n);	
};
*/
//-----------------------------------------------------------------------------
class ObjectTyped extends Object
{
};

//-----------------------------------------------------------------------------
class Entity extends ObjectTyped
{
};

//-----------------------------------------------------------------------------
class Camera extends Entity
{
	void Camera()
	{
		Print("Camera()");
		SetEventMask( EntityEvent.FRAME );
	}
	
	override void EOnFrame( IEntity other, float timeSlice )
	{
		Print(timeSlice);
	}

	proto native void SetPos(vector pos); // set destination
	proto native void SetDir(vector dir); // set direction
	proto native void SetFOV(float fov);
	proto native void SetFOVRange(float minFov, float maxFov);
	proto native void SetFocus(float distance, float blur);
	proto native void SetTarget(vector target);
	proto native void PreparePos(vector pos);
	proto native void PrepareFOV(float fov);
	proto native void PrepareFOVRange(float minFov, float maxFov);
	proto native void PrepareFocus(float distance, float blur);
	proto native void PrepareTarget(vector target);
	proto native void Command(string mode);
	proto native void Commit(float time); // commit all deferred settings
	proto native void CommitPrepared(float time); // commit all prepared settings
	proto native void PreloadPrepared(float time); // commit all deferred settings
	proto native void SetBank(float bank); // set camera bank
	proto native void SetManual(bool manual);
	proto native bool GetManual();
	proto native void SetComfy(bool comfy);
	proto native bool GetComfy();
	proto native void SetIsMouseMoveDisabled(bool isMouseMoveDisabled);
	proto native bool GetIsMouseMoveDisabled();
	proto native bool GetCommitted();
	proto native bool GetPreloaded();
	
	/**
	\brief Sets target EntityAI around which camera will orbit. Camera will also 
		change from free mode to orbit mode. To change camera mode back to free
		mode set orbitTarget to NULL.
		\param orbitTarget \p EntityAI target object around which will camera orbit.
	*/
	proto native void SetOrbitTarget(EntityAI orbitTarget);
	
	/**
	\brief Set direction of object
	@param vDirection, direction vector
	\note { object up vector is always y-axis }
	\note { also commit(0) }
	*/
	/*void SetDirection(vector vDirection)
	{
		SetDir(vDirection);
		SetTarget(GetPosition() + vDirection);
		Commit(0);
	}*/
};

//-----------------------------------------------------------------------------
class ProxyInventory extends ObjectTyped
{
};

//-----------------------------------------------------------------------------
class ProxyCrew extends Entity
{
};

//-----------------------------------------------------------------------------
class ProxySubpart extends Entity
{
};

class EventParams
{
};

class EventParams1 extends EventParams
{
	Object obj;
	void EventParams1(Object o)
	{
		obj = o;
	}
};

class EntityAI;

class EntityEventHandler
{
	void OnEvent(EntityAI entity, int entity_event_type /* EE* */, Param params)
	{
	}
};

class EntityAnimEndEventHandler extends EntityEventHandler
{
	private string m_anim_name;
	private autoptr CallQueueContext m_call;

	void EntityAnimEndEventHandler(string anim_name, Class target, string fn, Param params = NULL)
	{
		m_anim_name = anim_name;
		m_call = new CallQueueContext(target, fn, params);
	}

	void OnEvent(EntityAI entity, int entity_event_type /* EE* */, Param params)
	{
		if (entity_event_type != EEAnimDone) return;
		if (!m_call) return;

		Param1<string> param = params;
		if (m_anim_name == "" || param.param1 == m_anim_name)
		{
			m_call.Call();
		}
	}
};

//-----------------------------------------------------------------------------
//! Reloads weapon with magazine
proto native bool TryReloadWeapon(EntityAI person, EntityAI weapon, EntityAI magazine);

proto native bool TrySetEntityToQuickBar(Man person, EntityAI item, int index);
//! Better to use Man::CanDropEntity and Man::DropEntity instead
proto native bool TryDropEntity(Man person, EntityAI item);
//! Better to use EntityAI::CanTakeEntityAsOwned and EntityAI::TakeEntityAsOwned instead
proto native bool TryTakeEntityAsOwned(EntityAI person, EntityAI item);
//! Better to use EntityAI::CanAddEntityInCargo and EntityAI::TakeEntityAsOwned instead
proto native bool TryTakeEntityToCargo(EntityAI person, EntityAI item);
//! Better to use EntityAI::CanAddEntityInCargoEx and EntityAI::TakeEntityAsOwnedAtPos instead
proto native bool TryTakeEntityToCargoEx(EntityAI person, EntityAI item, int row, int col);
//! Better to use EntityAI::CanAddEntityInCargoEx instead
proto native bool TryTakeEntityToProxyCargoEx(EntityAI owner, EntityAI item, int pxy, int row, int col);
//! Better to use EntityAI::CanTakeEntityAsAttachment  and EntityAI::TakeEntityAsAttachment instead
proto native bool TryTakeEntityAsAttachment(EntityAI person, EntityAI item, int slot);
//! Better to use EntityAI::CanAddEntityInHands and EntityAI::TakeEntityToHands instead
proto native bool TryTakeEntityToHands(Man person, EntityAI item);
//! Better to use EntityAI::MoveItemFromHandsToInventory  instead
proto native bool TryMoveItemFromHandsToInventory(Man person, EntityAI item);
//! Better to use EntityAI::RemoveItemFromHands  instead
proto native bool TryRemoveItemFromHands(Man person, EntityAI item);
//! Better to use EntityAI::SwapEntities  instead
proto native bool TrySwapEntities(Man person, EntityAI item1, EntityAI item2);
//! Check if entities can be swapped
proto native bool CanSwapEntities(EntityAI item1, EntityAI item2);


//-----------------------------------------------------------------------------
class EntityAI extends Entity
{
	
	private ComponentsBank m_ComponentsBank;
	
	//! CreateComponent
	Component CreateComponent(int comp_type, string extended_class_name="")
	{
		return GetComponent(comp_type, extended_class_name);	
	}
	
	//! GetComponent
	Component GetComponent(int comp_type, string extended_class_name="")
	{
		if ( m_ComponentsBank == NULL )
		{
			m_ComponentsBank = new ComponentsBank(this);
		}
		
		return m_ComponentsBank.GetComponent(comp_type, extended_class_name);
	}
	
	//! DeleteComponent
	bool DeleteComponent(int comp_type)
	{
		return m_ComponentsBank.DeleteComponent(comp_type);
	}
	
	//! IsComponentExist
	bool HasComponent(int comp_type)
	{
		if ( m_ComponentsBank )
		{
			return m_ComponentsBank.IsComponentAlreadyExist(comp_type);
		}
		
		return false;
	}
	
	//! Log
	void Log(string msg, string fnc_name = "n/a")
	{
		Debug.Log(msg, "Object", "n/a", fnc_name, this.GetType());
	}
	
	//! LogWarning
	void LogWarning(string msg, string fnc_name = "n/a")
	{
		Debug.LogWarning(msg, "Object", "n/a", fnc_name, this.GetType());
	}
	
	//! LogError
	void LogError(string msg, string fnc_name = "n/a")
	{
		Debug.LogError(msg, "Object", "n/a", fnc_name, this.GetType());
	}	
	
	private EntityEventHandler m_event_handler;

	void EntityAI()
	{
		// Set up the Energy Manager
		string type = GetType();
		string param_access_energy_sys = "CfgVehicles " + type + " EnergyManager ";
		bool is_electic_device	= GetGame().ConfigIsExisting(param_access_energy_sys);
		
		if (is_electic_device)
		{
			switch(type)
			{
				case "EN5C_PowerGenerator":
					CreateComponent(COMP_TYPE_ENERGY_MANAGER, "ComponentEnergyPowerGenerator");
				break;
				
				case "EN5C_Spotlight":
					CreateComponent(COMP_TYPE_ENERGY_MANAGER, "ComponentEnergySpotlight");
				break;
				 
				default:
					CreateComponent(COMP_TYPE_ENERGY_MANAGER);
				break;
			}
		}
	}

	void ~EntityAI()
	{
		delete m_ComponentsBank;
	}
	
	//! is this container empty or not, checks both cargo and attachments
	bool IsEmpty()
	{
		return (!HasAnyCargo() && AttachmentsCount() == 0);
	}

	//! is this container empty or not, checks only cargo
	bool HasAnyCargo()
	{
		Cargo cargo = this.GetCargo();
		
		if(!cargo) return false;//this is not a cargo container
		
		if( cargo.GetItemCount() > 0 )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	EntityEventHandler GetEventHandler()
	{ 
		return m_event_handler;
	}

	void SetEventHandler(EntityEventHandler event_handler)
	{
		m_event_handler = event_handler;
	}
	
	int GetAgents()
	{
		return 0;
	}		
	void RemoveAgent(int agent_id);
	void RemoveAllAgents();
	void InsertAgent(int agent, int count);
	
	bool IsEntityAI()
	{
		return true;
	}
	
	/**
\brief Delete this object in next frame
	\return \p void
	@code
		ItemBase item = GetGame().GetPlayer().CreateInInventory("GrenadeRGD5");
		
		item.Delete();
	@endcode
*/
	void Delete()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(GetGame(), "ObjectDelete", new Param1<Object>(this)); 
	}	

	//! Entity-variables can be used/shared with SQF scripts through EntityAI::GetVariable/SetVariable methods (the same as getVariable/setVariable on SQF side)
	proto bool GetVariable(string varName, out void var);
	//! Entity-variables can be used/shared with SQF scripts through EntityAI::GetVariable/SetVariable methods (the same as getVariable/setVariable on SQF side)
	proto void SetVariable(string varName, void var);
	
	//! Legacy event system can be used from enforce script through EntityAI::OnEvent method. (parameters are passed through Params class. Variables in params class must match with event handler parameters.). It's best way how to call SQF code from enforce-scripted entities
	proto native void OnEvent(int eventType, Param params);
	
	//! Returns player which owns this entity (top in ownership hierarchy)
	proto native Man GetOwnerPlayer();

	//! Returns direct owner of this entity (for example: if this entity is in Backpack, returns Backpack)
	proto native EntityAI GetOwner();
	
	string GetVariableString(string varName)
	{
		string ret = "";
		GetVariable(varName, ret);
		return ret;
	}
	int GetVariableInt(string varName)
	{
		int ret = 0;
		GetVariable(varName, ret);
		return ret;
	}
	float GetVariableFloat(string varName)
	{
		float ret = 0;
		GetVariable(varName, ret);
		return ret;
	}
	Object GetVariableObject(string varName)
	{
		Object ret = NULL;
		GetVariable(varName, ret);
		return ret;
	}

	//!Called upon object creation
	void EEInit()
	{
	}

	void EEAnimHook(int userType, string param)
	{
	}

	void EEAnimDone(string moveName)
	{
		if (m_event_handler) 
		{
			Param param = new Param1<string>(moveName);
			m_event_handler.OnEvent(this, EEAnimDone, param);
			delete param;
		}
	}

	/**
	@code
	// example of detecting getting item/dropping item from/to inventory
	void EEItemLocationChanged(EntityAI old_owner, EntityAI new_owner)
	{
		Man owner_player_old = NULL;
		Man owner_player_new = NULL;

		if (old_owner)
		{
			if (old_owner.IsPlayer())
			{
				owner_player_old = old_owner;
			}
			else
			{
				owner_player_old = old_owner.GetOwnerPlayer();
			}
		}

		if (new_owner)
		{
			if (new_owner.IsPlayer())
			{
				owner_player_new = new_owner;
			}
			else
			{
				owner_player_new = new_owner.GetOwnerPlayer();
			}
		}

		if ( !owner_player_new && owner_player_old )
		{
			Print("Item dropped from inventory");
		}

		if ( owner_player_new && !owner_player_old )
		{
			Print("Item taken to inventory");
		}
	}
	@endcode
*/	
	void EEItemLocationChanged(EntityAI old_owner, EntityAI new_owner)
	{
	}

	void EEAnimStateChanged(string moveName)
	{
	}
	
	void EEFired(string weaponType, string muzzleType, string mode, string type, string magazineType, Object missile)
	{
	}

	void EELocal(bool isLocal)
	{
	}

	void EEEngine(bool onOff)
	{
	}
	
	void EEFuel(bool hasFuel)
	{
	}

	void EEDammaged(string hitpointName, float hit)
	{
	}

	void EEKilled(Object killer)
	{
	}
	
	void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, string component, string ammo, vector modelPos)
	{
	}
	
	// !Called on PARENT when a child is attached to it.
	void EEItemAttached(EntityAI item, string slot_name)
	{
		if ( m_ComponentsBank != NULL )
		{
			for ( int comp_key = 0; comp_key < COMP_TYPE_COUNT; ++comp_key )
			{
				if ( m_ComponentsBank.IsComponentAlreadyExist(comp_key) )
				{
					m_ComponentsBank.GetComponent(comp_key).Event_OnItemAttached(item, slot_name);
				}
			}
		}
		
		// Energy Manager
		if ( GetGame().IsServer() )
		{
			if ( this.HasEnergyManager()  &&  item.HasEnergyManager() )
			{
				GetCompEM().OnAttachmentAdded(item);
			}
		}
		
		//item.OnWasAttached(this, slot_name); // Commented out since it's not being used in 0.62. Fix for: DAYZ-25234
	}
	
	// !Called on PARENT when a child is detached from it.
	void EEItemDetached(EntityAI item, string slot_name)
	{
		if ( m_ComponentsBank != NULL )
		{
			for ( int comp_key = 0; comp_key < COMP_TYPE_COUNT; ++comp_key )
			{
				if ( m_ComponentsBank.IsComponentAlreadyExist(comp_key) )
				{
					m_ComponentsBank.GetComponent(comp_key).Event_OnItemDetached(item, slot_name);
				}
			}
		}
		
		// Energy Manager
		if ( GetGame().IsServer() )
		{
			if ( this.HasEnergyManager()  &&  item.HasEnergyManager() )
			{
				GetCompEM().OnAttachmentRemoved(item);
			}
		}
		
		//item.OnWasDetached(this, slot_name); // Commented out since it's not being used in 0.62. Fix for: DAYZ-25234
	}

	void EECargoIn(EntityAI item)
	{
	}

	void EECargoOut(EntityAI item)
	{
	}

	void EECargoMove(EntityAI item)
	{
	}
	
	void EEOnWaterEnter()
	{
	}
	
	void EEOnWaterExit()
	{
	}

	void EEUsed(Man owner)
	{
	}

	//! Called on client (in multiplayer) when sqf synchronize variable is synchronized
	void EEVariableSynchronized(string variable_name)
	{
	}

	void EEOnAfterLoad()
	{
		// ENERGY MANAGER
		// Restore connections between devices which were connected before server restart
		if ( HasEnergyManager()  &&  GetCompEM().GetRestorePlugState() )
		{
			int low = GetCompEM().GetEnergySourceIDLow();
			int high = GetCompEM().GetEnergySourceIDHigh();
			
			// get pointer to EntityAI based on this ID
			EntityAI potential_energy_source = GetGame().GetEntityByPersitentID(low, high); // This function is available only in this event!
			
			// IMPORTANT!
			// Object IDs acquired here become INVALID when electric devices are transfered to another server while in plugged state (like Flashlight plugged into its attachment 9V battery)
			// These items must be excluded from this system of restoring plug state so they don't accidentally connect to incorrect objects through invalid IDs.
			// Therefore their plug state should be restored withing the EEItemAttached() event instead of here.
			
			bool IsAttachment = false;
			
			if (potential_energy_source)
				IsAttachment = this.HasAttachment(potential_energy_source);
			
			if ( !IsAttachment  &&  potential_energy_source )
				IsAttachment = potential_energy_source.HasAttachment(this);
			
			if ( potential_energy_source  &&  potential_energy_source.HasEnergyManager()  &&  !IsAttachment )
			{
				GetCompEM().PlugThisInto(potential_energy_source); // restore connection
			}
		}
		
	}
	
	/**
	\brief This event is called when player is marked captive.
		\param captive \p bool true when player is marked captive, false when it is unmarked captive.
	*/
	void EEOnSetCaptive(bool captive)
	{
	}

	// inventory conditions
	bool ConditionAttachEntityAI(EntityAI child)
	{
	}

	bool ConditionAttach(EntityAI parent)
	{
	}

	bool ConditionDetachEntityAI(EntityAI child)
	{
	}

	bool ConditionDetach(EntityAI parent)
	{
	}

	bool ConditionCargoInEntityAI(EntityAI child)
	{
	}

	bool ConditionCargoIn(EntityAI parent)
	{
	}

	bool ConditionCargoOutEntityAI(EntityAI child)
	{
	}

	bool ConditionCargoOut(EntityAI parent)
	{
	}
	
	bool ConditionIntoInventoryEntityAI(EntityAI entity_ai)
	{
	}

	bool ConditionIntoInventory(EntityAI player)
	{
	}

	bool ConditionIntoHandsEntityAI(EntityAI entity_ai)
	{
	}

	bool ConditionIntoHands(EntityAI player)
	{
	}

	bool ConditionOutOfHandsEntityAI(EntityAI entity_ai)
	{
	}

	bool ConditionOutOfHands(EntityAI player)	
	{
	}


	// !Called on CHILD when it's attached to parent.
	void OnWasAttached( EntityAI parent, string slot_name )
	{
		
	}
	
	// !Called on CHILD when it's detached from parent.
	void OnWasDetached( EntityAI parent, string slot_name )
	{
		
	}
	
	/**
	\brief Returns attachment slot where this item belongs
	*/
	proto native int GetSlotId();
	/**
	\brief Returns number of slots for attachments
	*/
	proto native int GetSlotsCount();
	/**
	\brief Returns number of slots for attachments corrected for weapons
	*/
	int GetSlotsCountCorrect()
	{
		return GetSlotsCount();
	};
	/**
	\brief Returns count of attachments attached to this item
	*/
	proto native int AttachmentsCount();
	/**
	\brief Returns attached entity by attachment index
	\note index is not slot! for getting attachment by slot use FindAttachment
	*/
	proto native EntityAI GetAttachmentFromIndex(int index);
	/**
	\brief Returns True if entity is attached to this
	*/
	proto native bool HasAttachment(EntityAI e);
	/**
	\brief Returns True if entity is attached to this in slot
	*/
	proto native bool HasAttachmentEx(EntityAI e, int slot);
	/**
	\brief Returns attached entity in slot (you can use GetGame().GetInventorySlot(name) to get slot id)
	*/
	proto native EntityAI FindAttachment(int slot);
	/**
	\brief Returns placeholder entity for slot (naked arms, legs etc)
	*/
	proto native EntityAI FindPlaceholderForSlot(int slot);

	//! Returns attachment by slot name.
	EntityAI FindAttachmentBySlotName(string slot_name)
	{
		if ( GetGame() )
		{
			int slot_id = GetGame().GetInventorySlot(slot_name);
			return this.FindAttachment(slot_id); 
		}
	}
	
	proto native bool CanRemoveAttachment(EntityAI attachment);
	proto native bool CanRemoveAttachmentEx(EntityAI attachment, int slot);

	/**
	\brief Returns if entity can be removed from its current location
	*/
	proto native bool CanRemoveEntity();
	
	/**
	\brief Remove attachment
	*/
	proto native bool RemoveAttachment(EntityAI attachment); 
	/**
	\brief Remove attachment from slot
	*/
	proto native bool RemoveAttachmentEx(EntityAI attachment, int slot); 

	/**
	\brief Create Entity of specified type inside this entity 
	\warning Low-level API, not synchronized over network!
	*/
	proto native EntityAI CreateOwnedEntity(string typeName);
	/**
	\brief Create Entity of specified type as attachment of entity
	*/
	proto native EntityAI CreateAttachment(string typeName);
	/**
	\brief Returns if item can be added anywhere into this entity (as attachment or into cargo, recursively)
	*/
	bool CanTakeEntityAsOwned(EntityAI item)
	{
		if (CanAddAttachment(item))
		{
			return true;
		}
	
		if (CanAddEntityInCargo(item))
		{
			return true;
		}
	
		// try add into one of attachments
		int attachments = AttachmentsCount();
		for (int i = 0; i < attachments; i++)
		{
			EntityAI attachment = GetAttachmentFromIndex(i);
			if (!attachment) continue;
	
			if (attachment.CanAddEntityInCargo(item))
			{
				return true;
			}
		}
	
		return false;
	}
	/**
	\brief Put item anywhere into this entity (as attachment or into cargo, recursively)
	*/
	bool TakeEntityAsOwned(EntityAI item)
	{
		return TryTakeEntityAsOwned(this, item);
	}
	/**
	\brief Put item into into cargo
	*/
	bool TakeEntityToCargo(EntityAI item)
	{
		return TryTakeEntityToCargo(this, item);
	}
	/**
	\brief Put item into into cargo on specific cargo location
	*/
	bool TakeEntityToCargoAtPos(EntityAI item, Cargo cargo, int row, int col)
	{
		if (HasProxyCargo() && cargo != GetCargo())
		{
			int i,c;
			c = GetProxyCargoCount();
			for (i = 0; i < c; i++)
			{
				if (cargo == GetProxyCargo(i))
				{
					return TryTakeEntityToProxyCargoEx(this, item, i, row, col);
				}
			}
		}
		return TryTakeEntityToCargoEx(this, item, row, col);
	}
	/**
	\brief Returns if item can be added as attachment on specific slot
	*/
	bool CanTakeEntityAsAttachmentToSlot(EntityAI item, int slot)
	{
		return CanAddAttachmentEx(item, slot);
	}
	/**
	\brief Put item into as attachment on specific slot
	*/
	bool TakeEntityAsAttachmentToSlot(EntityAI item, int slot)
	{
		return TryTakeEntityAsAttachment(this, item, slot);
	}
	/**
	\brief Returns if item can be added as attachment
	*/
	bool CanTakeEntityAsAttachment(EntityAI item)
	{
		return CanAddAttachmentEx(item, item.GetSlotId());
	}
	/**
	\brief Put item into as attachment
	*/
	bool TakeEntityAsAttachment(EntityAI item)
	{
		return TryTakeEntityAsAttachment(this, item, item.GetSlotId());
	}
	/**
	\brief Get attached entity by type
	*/
	EntityAI GetAttachmentByType(typename type)
	{
		for ( int i = 0; i < AttachmentsCount(); i++ )
		{
			EntityAI attachment = GetAttachmentFromIndex ( i );
			if ( attachment && attachment.IsInherited ( type ) )
			{
				return attachment;
			}
		}
		
		return NULL;
	}
	/**
	\brief Get attached entity by config type name
	*/
	EntityAI GetAttachmentByConfigTypeName(string type)
	{
		for ( int i = 0; i < AttachmentsCount(); i++ )
		{
			EntityAI attachment = GetAttachmentFromIndex ( i );
			if ( attachment.IsKindOf ( type ) )
			{
				return attachment;
			}
		}
		
		return NULL;
	}
	/**
	\brief Returns if item can be dropped out from this entity
	*/
	bool CanDropEntity(EntityAI item)
	{
		// this function will be implemented in future
		return true;
	}
	/**
	\brief Drop item out of this entity
	*/
	bool DropEntity(EntityAI item)
	{
		return TryDropEntity(this, item);
	}

	/**
	\brief LOW LEVEL: Check if attachment can be added to any slot
	*/	
	proto native bool CanAddAttachment(notnull EntityAI e); 
	/**
	\brief LOW LEVEL: Check if attachment can be added to slot
	*/
	proto native bool CanAddAttachmentEx(notnull EntityAI e, int slot); 

	/**
	\brief LOW LEVEL: Get Cargo class for this entity
	\warning Low-level API, use only if you know what you are doing
	*/
	proto native Cargo GetCargo();
	proto native bool HasProxyCargo();
	proto native int GetProxyCargoCount();
	proto native Cargo GetProxyCargo(int cargoIndex);

	proto native bool HasOwnedEntity(EntityAI item);
	proto native bool CanAddOwnedEntity(EntityAI item);
	proto native bool AddOwnedEntity(EntityAI item);
	proto native bool CanRemoveOwnedEntity(EntityAI item);
	proto native bool RemoveOwnedEntity(EntityAI item);

	proto native bool HasEntityInCargo(EntityAI e);
	proto native bool HasEntityInCargoEx(EntityAI e, int row, int col);
	proto native bool CanAddEntityInCargo(EntityAI e);
	proto native bool CanAddEntityInCargoEx(EntityAI e, int row, int col);
	proto native bool AddEntityInCargo(EntityAI owner, EntityAI cargo);
	proto native bool AddEntityInCargoEx(EntityAI owner, EntityAI e, int row, int col);
	proto native bool CanRemoveEntityInCargo(EntityAI e);
	proto native bool RemoveEntityInCargo(EntityAI owner, EntityAI e);

	proto native bool CanLockInventoryWithKey(EntityAI key);
	proto native bool CanUnlockInventoryWithKey(EntityAI key);
	proto native void LockInventoryWithKey(EntityAI key);
	proto native void UnlockInventoryWithKey(EntityAI key);
	proto native bool HasKeys();

	proto native void LockInventory(int lockType);
	proto native void UnlockInventory(int lockType);
	proto native bool IsInventoryUnlocked();
	proto native bool IsInventoryLocked();

	proto native void SwitchLight(bool isOn);
	
	//! Change texture in hiddenSelections
	proto native void SetObjectTexture(int index, string texture_name);
	//! Change material in hiddenSelections
	proto native void SetObjectMaterial(int index, string mat_name);

	//! Return animation phase of animation on object.
	proto native float GetAnimationPhase(string animation);
	//! Process animation on object. Animation is defined in config file. Wanted animation phase is set to phase.
	void SetAnimationPhase(string animation, float phase)
	{
		GetGame().ObjectSetAnimationPhase(this, animation, phase);
	}
	
	proto native bool	IsPilotLight();
	proto native void SetPilotLight(bool isOn);
	
	proto native bool IsWaterContact();
	
	//! Returns the vehicle in which the given unit is mounted. If there is none, the unit is returned.
	proto native EntityAI GetVehicle();
	
	/**
	\brief Checks whether the unit is a captive. If the unit is a vehicle, its commander is checked instead. 
	@code
		GetGame().GetPlayer().IsCaptive();
	@endcode
	*/
	proto native bool IsCaptive();

	proto native void SetAllowDamage(bool val);

	//! Get list of available actions
	proto native void GetActions(Man player, EntityAI with = NULL);
	
	//! Returns True if object has any actions
	proto native bool HasActions(Man player, EntityAI with = NULL);

	/**
	\brief Engine calls this function to collect data from entity to store to game database (on server side).
	@code
	void OnStoreSave(ParamsWriteContext ctx)
	{
		// dont forget to propagate this call trough class hierarchy!
		super.OnStoreSave(ctx);

		// write any data (using params) you want to store
		autoptr Param4<bool, int, float, string> p1 = new Param4<bool, int, float, string>(true, 56, 6.28, "Pepe");
		autoptr Param4<float, string, float, string> p2 = new Param4<float, string, float, string>(9.56, "ahoj", 6.28, "svet");
		ctx.Write(p1);
		ctx.Write(p2);
	}
	@endcode
	*/
	void OnStoreSave(ParamsWriteContext ctx)
	{
		// Saving of energy related states
		if ( HasEnergyManager() )
		{
			/*===========================
				Save int, float, bool
			===========================*/
			
			// Save energy amount
			autoptr Param1<float> par_f_energy = new Param1<float>( GetCompEM().GetEnergy() );
			ctx.Write( par_f_energy );
			
			// Save passive/active state
			autoptr Param1<bool> par_b_is_passive = new Param1<bool>( GetCompEM().IsPassive() );
			ctx.Write( par_b_is_passive );
			
			// Save ON/OFF state
			autoptr Param1<bool> par_b_is_on = new Param1<bool>( GetCompEM().IsSwitchedOn() );
			ctx.Write( par_b_is_on );
			
			// Save plugged/unplugged state
			autoptr Param1<bool> par_b_is_plugged = new Param1<bool>( GetCompEM().IsPlugged() );
			ctx.Write( par_b_is_plugged );
			
			
			/*===========================
					Save object IDs
			===========================*/
			
			// ENERGY SOURCE
			// Save energy source IDs
			EntityAI energy_source = GetCompEM().GetEnergySource();
			int ID_low = 0;
			int ID_high = 0;
			
			if (energy_source)
			{
				energy_source.GetPersistentID(ID_low, ID_high);
			}
			
			// Save energy source ID low
			autoptr Param1<int> p_i_energy_source_ID_low = new Param1<int>( ID_low );
			ctx.Write( p_i_energy_source_ID_low );
			
			// Save energy source ID high
			autoptr Param1<int> p_i_energy_source_ID_high = new Param1<int>( ID_high );
			ctx.Write( p_i_energy_source_ID_high );
		}
	}
	
	/**
	\brief Called when data is loaded from game database (on server side).
	@code
	void OnStoreLoad(ParamsReadContext ctx)
	{
		// dont forget to propagate this call trough class hierarchy!
		super.OnStoreLoad(ctx);

		// read data loaded from game database (format and order of reading must be the same as writing!)
		autoptr Param4<bool, int, float, string> p1 = new Param4<bool, int, float, string>(false, 0, 0, "");
		autoptr Param4<float, string, float, string> p2 = new Param4<float, string, float, string>(0, "", 0, "");
		if (ctx.Read(p1))
		{
		  Print(p1.param1);
		  Print(p1.param2);
		  Print(p1.param3);
		  Print(p1.param4);
		}
		else
		{
			Print("no data");
		}

		if (ctx.Read(p2))
		{
		  Print(p2.param1);
		  Print(p2.param2);
		  Print(p2.param3);
		  Print(p2.param4);
		}
		else
		{
			Print("no data");
		}
	}
	@endcode
	*/
	void OnStoreLoad (ParamsReadContext ctx)
	{
		// Restoring of energy related states
		if ( HasEnergyManager() )
		{
			/*=====================================
				Load int, float, bool variables
			=====================================*/
			
			// Load energy amount
			autoptr Param1<float> par_f_energy = new Param1<float>( 0 );
			ctx.Read( par_f_energy );
			GetCompEM().SetEnergy(par_f_energy.param1);
			
			// Load passive/active state
			autoptr Param1<bool> par_b_is_passive = new Param1<bool>( 0 );
			ctx.Read( par_b_is_passive );
			GetCompEM().SetPassiveState(par_b_is_passive.param1);
			
			// Load ON/OFF state
			autoptr Param1<bool> par_b_is_on = new Param1<bool>( false );
			ctx.Read( par_b_is_on );
			
			// Load plugged/unplugged state
			autoptr Param1<bool> par_b_is_plugged = new Param1<bool>( false );
			ctx.Read( par_b_is_plugged );
			
			
			/*=============================
					Load object IDs
			=============================*/
			
			// ENERGY SOURCE
			// Load energy source ID low
			autoptr Param1<int> p_i_energy_source_ID_low = new Param1<int>( 0 ); // Even 0 can be valid ID!
			ctx.Read( p_i_energy_source_ID_low );
			
			// Load energy source ID high
			autoptr Param1<int> p_i_energy_source_ID_high = new Param1<int>( 0 ); // Even 0 can be valid ID!
			ctx.Read( p_i_energy_source_ID_high );
			
			if ( par_b_is_plugged.param1 )
			{
				// Because function GetEntityByPersitentID() cannot be called here, ID values must be stored and used later.
				GetCompEM().StoreEnergySourceIDs( p_i_energy_source_ID_low.param1, p_i_energy_source_ID_high.param1 );
				GetCompEM().RestorePlugState(true);
			}
			
			if (par_b_is_on.param1)
			{
				GetCompEM().SwitchOn();
			}
		}
	}

	//! Sets object synchronization dirty flag, which signalize that object wants to be synchronized (take effect only in MP on server side)
	proto native void SetSynchDirty();

	/**
	\brief After synchronization dirty flag is set (SetSynchDirty function), engine calls this function to collect data to synchronize from entity (on server side).
	@code
	void OnSynchronize(ParamsWriteContext ctx)
	{
		// dont forget to propagate this call trough class hierarchy!
		super.OnSynchronize(ctx);

		// write any data (using params) you want to pass to clients instances
		autoptr Param2<int, float> p = new Param2<int, float>(m_counter, m_blood_preasure);
		ctx.Write(p);
	}
	@endcode
	*/
	void OnSynchronize(ParamsWriteContext ctx)
	{
	}
	
	/**
	\brief Called on clients after receiving synchronization data from server.
	@code
	void OnSynchronized(ParamsReadContext ctx)
	{
		// dont forget to propagate this call trough class hierarchy!
		super.OnSynchronized(ctx);

		// read data (format and order of reading on client must be the same as writing on server side)
		Param2<int, float> p = new Param2<int, float>(0, 0);
		if (ctx.Read(p))
		{
			m_counter = p.param1;
			m_blood_preasure = p.param2;
		}
	}
	@endcode
	*/
	void OnSynchronized (ParamsReadContext ctx)
	{
	}
	
	void EOnFrame(IEntity other, float timeSlice)
	{
		if ( m_ComponentsBank != NULL )
		{
			for ( int comp_key = 0; comp_key < COMP_TYPE_COUNT; ++comp_key )
			{
				if ( m_ComponentsBank.IsComponentAlreadyExist(comp_key) )
				{
					m_ComponentsBank.GetComponent(comp_key).Event_OnFrame(other, timeSlice);
				}
			}
		}
	}
	
	// -------------------------------------------------------------------------
	Shape DebugBBoxDraw()
	{
		return GetComponent(COMP_TYPE_ETITY_DEBUG).DebugBBoxDraw();
	}
	
	// -------------------------------------------------------------------------
	void DebugBBoxSetColor(int color)
	{
		GetComponent(COMP_TYPE_ETITY_DEBUG).DebugBBoxSetColor(color);
	}
	
	// -------------------------------------------------------------------------
	void DebugBBoxDelete()
	{
		GetComponent(COMP_TYPE_ETITY_DEBUG).DebugBBoxDelete();
	}
	
	// -------------------------------------------------------------------------
	Shape DebugDirectionDraw(float distance = 1)
	{
		return GetComponent(COMP_TYPE_ETITY_DEBUG).DebugDirectionDraw(distance);
	}
	
	// -------------------------------------------------------------------------
	void DebugDirectionSetColor(int color)
	{
		GetComponent(COMP_TYPE_ETITY_DEBUG).DebugDirectionSetColor(color);
	}
	
	// -------------------------------------------------------------------------
	void DebugDirectionDelete()
	{
		GetComponent(COMP_TYPE_ETITY_DEBUG).DebugDirectionDelete();
	}
	
	//! Hides selection of the given name. Must be configed in config.hpp and models.cfg
	void HideSelection( string selection_name )
	{
		SetAnimationPhase ( selection_name, 1 ); // 1 = hide, 0 = unhide!
	}
	
	//! Shows selection of the given name. Must be configed in config.hpp and models.cfg
	void ShowSelection( string selection_name )
	{
		SetAnimationPhase ( selection_name, 0 ); // 1 = hide, 0 = unhide!
	}

	//! Returns low and high bits of persistence id of this entity.
	//! This id stays the same event after server restart.
	proto void GetPersistentID( out int low, out int high );
	
	//! Get remaining economy lifetime (seconds)
	proto native float GetLifetime();
	//! Reset economy lifetime to default (seconds)
	proto native void IncreaseLifetime();
	
	/*===================================================================================================
												ENERGY MANAGER
	Documentation: Confluence >> Camping & Squatting >> Electricity >> Energy Manager functionalities
	===================================================================================================*/
	
	//! Use this to access Energy Manager component on your device. Returns NULL if the given object lacks such component.
	ComponentEnergyManager GetCompEM()
	{
		if ( HasComponent(COMP_TYPE_ENERGY_MANAGER) )
		{
			return GetComponent(COMP_TYPE_ENERGY_MANAGER);
		}
		
		return NULL;
	}
	
	//! If this item has class EnergyManager in its config then it returns true. Otherwise returns false. 
	bool HasEnergyManager()
	{
		return HasComponent(COMP_TYPE_ENERGY_MANAGER);
	}
	
	// ------ Public Events for Energy manager component------
	
	//! Energy manager event: Called only once when this device starts doing its work
	void OnWorkStart() {}
	
	//! Energy manager event: Called every device update if its supposed to do some work. The update can be every second or at random, depending on its manipulation.
	void OnWork( float consumed_energy ) {}
	
	//! Energy manager event: Called when the device stops working (was switched OFF or ran out of energy)
	void OnWorkStop() {}
	
	//! Energy manager event: Called when the device is switched on
	void OnSwitchOn() {}
	
	//! Energy manager event: Called when the device is switched OFF
	void OnSwitchOff() {}
	
	//! Energy manager event: Called when this device is plugged into some energy source
	void OnIsPlugged(EntityAI source_device) {}
	
	//! Energy manager event: Called when this device is UNPLUGGED from the energy source
	void OnIsUnplugged( EntityAI last_energy_source ) {}
	
	//! Energy manager event: When something is plugged into this device
	void OnOwnSocketTaken( EntityAI device ) {}
	
	//! Energy manager event: When something is UNPLUGGED from this device
	void OnOwnSocketReleased( EntityAI device ) {}
	
	//! Energy manager event: Object's initialization. Energy Manager is fully initialized by this point.
	void OnInitEnergy() {}
	
	// ------ End of Energy Manager Component ------
};

//-----------------------------------------------------------------------------
class ScriptedEntity extends EntityAI
{
	/**
	\brief Sets collision box for object
	@param mins min values of box
	@param maxs max values of box
	\note This function is obsolete, use rather SetCollisionBox()
	*/
	proto native void SetClippingInfo(vector mins, vector maxs, float radius);
	
	/**
	\brief Sets collision box for object
	@param mins min values of box
	@param maxs max values of box
	\n usage :
	@code
	vector mins = "-1 -1 -1";
	vector maxs = "1 1 1";
	SetCollisionBox(mins, maxs);
	@endcode
	*/
	proto native void SetCollisionBox(vector mins, vector maxs);
};

//-----------------------------------------------------------------------------
class Transport extends EntityAI
{
	proto native bool EngineInOperation();
	proto native float GetRPM();
	proto native float GetSpeedometerValue();
	proto native float GetFuelFraction();
	proto native float VisibleLights();
	proto native float VisibleMovement();
	proto native float VisibleMovementNoSize();
	proto native float Audible();

/*	bool TestEngineInOperation()
	{
		Print("Transport::EngineInOperation");
		float isRunning = EngineInOperation();
		return isRunning;
	}
	float TestGetRPM()
	{
		Print("Transport::GetRPM");
		float rpm = GetRPM();
		return rpm;
	}
	float TestGetSpeedometerValue()
	{
		Print("Transport::GetSpeedometerValue");
		float speedKmh = GetSpeedometerValue();
		return speedKmh;
	}
	float TestGetFuelFraction()
	{
		Print("Transport::GetFuelFraction");
		float fuel = GetFuelFraction();
		return fuel;
	}
	float TestVisibleLights()
	{
		Print("Transport::VisibleLights");
		return VisibleLights();
	}
	float TestVisibleMovement()
	{
		Print("Transport::VisibleMovement");
		return VisibleMovement();
	}
	float TestVisibleMovementNoSize()
	{
		Print("Transport::VisibleMovementNoSize");
		return VisibleMovementNoSize();
	}
	float TestAudible()
	{
		Print("Transport::Audible");
		return Audible();
	}*/
};

//-----------------------------------------------------------------------------
class TankOrCar extends Transport
{
	proto native int GetEngagedGear();
	proto native float GetMinRPM();
	proto native float GetMaxRPM();
	proto native float GetBrakeFluidFraction();
	proto native float GetOilFraction();
	proto native float GetCoolantFraction();
	proto native float GetElectricPowerFraction();

/*	int TestGetEngagedGear()
	{
		Print("GetEngagedGear");
		int gear = GetEngagedGear();
		return gear;
	}
	float TestGetMinRPM()
	{
		Print("TankOrCar::GetMinRPM");
		float minRpm = GetMinRPM();
		return minRpm;
	}
	float TestGetMaxRPM()
	{
		Print("TankOrCar::GetMaxRPM");
		float maxRpm = GetMaxRPM();
		return maxRpm;
	}
	float TestGetBrakeFluidFraction()
	{
		Print("Transport::GetBrakeFluidFraction");
		float fluid = GetBrakeFluidFraction();
		return fluid;
	}
	float TestGetOilFraction()
	{
		Print("Transport::GetOilFraction");
		float oil = GetOilFraction();
		return oil;
	}
	float TestGetCoolantFraction()
	{
		Print("Transport::GetCoolantFraction");
		float coolant = GetCoolantFraction();
		return coolant;
	}
	float TestGetElectricPowerFraction()
	{
		Print("Transport::GetElectricPowerFraction");
		float electricPower = GetElectricPowerFraction();
		return ElectricPower;
	}*/
};

//-----------------------------------------------------------------------------
class Car extends TankOrCar
{
	int testVar;
	
	proto native void TestMethod(int a);
/*
	void EOnFrame(IEntity other, float timeSlice)
	{
		TestMethod(1);
		
		vector pos = GetOrigin();
		Shape.Create(ShapeType.LINE, 0xffffffff, SS_ONCE, pos, pos + Vector(0,5,0));
		//Print(GetOrigin());
		//Print("frame");
		if(KeyState(KC_J))
		{
			Print("frame");
			SetOrigin(Vector(0, 1, 0) * timeSlice + GetOrigin()); 		
		}
	}
	
	void EOnSimulate(IEntity other, float timeSlice)
	{
		//Print("simulate");
	}

	void EOnInit(IEntity world, int extra)
	{
		Print("on init");
	}
*/		
	void Car()
	{
		SetFlags(EntityFlags.SOLID, false);
//		SetEventMask(EntityEvent.FRAME|EntityEvent.SIMULATE);
	}
};

//-----------------------------------------------------------------------------
class Thing extends EntityAI
{
};

//-----------------------------------------------------------------------------
class Man extends EntityAI
{
	proto native void						DisableSimulation(bool disable);

	proto native bool						PlayActionGlobal(string action);
	proto native bool						PlayActionNowGlobal(string action);
	proto native bool						PlayMoveGlobal(string move);
	proto native bool						PlayMoveNowGlobal(string move);
	proto native void						SwitchMoveGlobal(string move);
	proto native void						SwitchActionGlobal(string action);

	//! Returns vehicle which this Man object is driving. If this Man object isn't as driver of any vehicle it will return NULL.
	proto native EntityAI					GetDrivingVehicle();

	proto native bool						CanOpenInventory();
	
	proto native int						GetQuickBarSize();
	proto native bool						CanAddEntityToQuickBarEx(EntityAI item, int index);
	proto native EntityAI				GetEntityInQuickBar(int index);
	proto native EntityAI				GetEntityInHands();
	proto native bool						HasEntityInHands(EntityAI e);
	proto native bool						CanAddEntityInHands(EntityAI e);
	proto native bool 					CanRemoveEntityInHands();

	proto native void 					SetAnimVariableInt(string name, int value);
	proto native void 					SetAnimVariableFloat(string name, float value);
	proto native void 					SetAnimVariableBool(string name, bool value);
	proto native void 					SetAnimVariableString(string name, string value);
	
	proto native void 					SetShakeBodyFactor(float value);
	proto native void 					SetShakeHandsFactor(float value);

	proto native vector					GetCameraDirection();
	proto native vector					GetCameraPosition();
	
	proto native void						AddNoise(string noiseTypeName);
	proto native float					GetPresenceInAI();
	proto native string					GetCurrentWeaponMode();
	proto native float					GetCurrentZeroing();

	//! Returns Life State of player, possible values are LifeStateAlive, LifeStateDead, LifeStateAsleep, LifeStateUnconscious
	proto native int						GetLifeState(); 

	//! Set speech restriction
	proto native void                       SetSpeechRestricted(bool state);
	
	//! Check if player has resctricted speech
	proto native bool                       IsSpeechRestricted();
	
	//! Texture that is used on the Man's face and hands
	proto native void						SetFaceTexture(string texture_name);
	//! Material that is used on the Man's face and hands
	proto native void						SetFaceMaterial(string material_name);

	bool IsMan()
	{
		return true;
	}

	void EEItemIntoHands(EntityAI item)
	{
	}

	void EEItemOutOfHands(EntityAI item)
	{
	}

	// multiplayer safe inventory operations
	bool SetEntityShortcut(EntityAI item, int index)
	{
		return TrySetEntityToQuickBar(this, item, index);
	}
	
	// -------------------------------------------------------------------------
	bool CanDropEntity(EntityAI item)
	{
		// this function can be implemented in future
		return true;
	}
	
	// -------------------------------------------------------------------------
	bool DropEntity(EntityAI item)
	{
		return TryDropEntity(this, item);
	}
	
	// -------------------------------------------------------------------------
	bool TakeEntityToHands(EntityAI item)
	{
		EntityAI in_hands = GetEntityInHands();
		if (in_hands)
		{
			MoveItemFromHandsToInventory(in_hands);
		}
	
		return TryTakeEntityToHands(this, item);
	}
	
	// -------------------------------------------------------------------------
	bool MoveItemFromHandsToInventory(EntityAI item)
	{
		return TryMoveItemFromHandsToInventory(this, item);
	}
	
	// -------------------------------------------------------------------------
	bool RemoveItemFromHands(EntityAI item)
	{
		return TryRemoveItemFromHands(this, item);
	}
	
	// -------------------------------------------------------------------------
	bool SwapEntities(EntityAI item1, EntityAI item2)
	{
		return TrySwapEntities(this, item1, item2);
	}

//----------------------------------------------------------------
// Stats

	//! Registers new stat type for this player.
	/*!
		\param[in] name The name of the stat type.
	*/
	proto native bool StatRegister( string name );
	//! Gets counter value of the specified stat type.
	/*!
		\param[in] name The name of the stat type.
	*/
	proto native float StatGet( string name );
	//! Gets counter value as string of the specified stat type.
	/*!
		\param[in]  name  The name of the stat type.
		\param[out] value A formatted string containing stat value.
	*/
	proto void StatGetCounter( string name, out string value );
	//! Gets counter value as formatted time string of the specified stat type.
	/*!
		\param[in]  name  The name of the stat type.
		\param[out] value A formatted string containing stat value.
	*/
	proto void StatGetAsTime( string name, out string value );
	//! Updates stat counter with given value.
	/*!
		\param[in] name  The name of the stat type.
		\param[in] value The specified value.
	*/
	proto native void StatUpdate( string name, float value );
	//! Updates stat counter by time.
	/*!
		Use this to measure play time.

		\param[in] name The name of the stat type.
	*/
	proto native void StatUpdateByTime( string name );
	//! Updates stat counter by player's actual position.
	/*!
		Use this to measure player's travelling distance.

		\param[in] name The name of the stat type.
	*/
	proto native void StatUpdateByPosition( string name );
	//! Updates stat counter by given position.
	/*!
		\param[in] name The name of the stat type.
		\param[in] pos  The specified position.
	*/
	proto native void StatUpdateByGivenPos( string name, vector pos );
	//! Invokes read stats from server to client.
	proto native void StatInvokeUpdate();

//----------------------------------------------------------------
	
	void TestSQF(int number, string text, vector vec)
	{
		Print(number);
		Print(text);
		Print(vec);
	}

	void PerformAction(Action action)
	{
		GetGame().PerformAction(this, action);
	}

	void Man()
	{
		SetFlags(EntityFlags.TOUCHTRIGGERS, false);
	}
};


//-----------------------------------------------------------------------------
class InventoryItem extends EntityAI
{	
	//! Some inventoryItem devices can be switched on/off (radios, transmitters)
	proto native void SwitchOn(bool onOff);
	//! Some inventoryItem devices can be switched on/off (radios, transmitters)
	proto native bool	IsOn();	
	
	void EEUsed(Man owner)
	{
		if (GetGame().GetMission())
		{
			GetGame().GetMission().OnItemUsed(this, owner);
		}
	}
	
	void OnRightClick()
	{
	
	}

	event bool OnUseFromInventory(Man owner)
	{
		return false;
	}

	//! Get tooltip text
	string GetTooltip()
	{
		return ConfigGetString("descriptionShort");
	}

	bool IsInventoryItem()
	{
		return true;
	}
};

//-----------------------------------------------------------------------------
class Cargo
{
	proto native int GetWidth();
	proto native int GetHeight();
	proto native int GetItemCount();
	proto native EntityAI GetItem(int index);
	proto void GetItemSize(int index, out int w, out int h);
	proto void GetItemPos(int index, out int x, out int y);

	proto native EntityAI FindEntityInCargoOn(int row, int col);
	proto native int FindEntityInCargo(EntityAI e);

	proto native bool CanMoveEntityInCargoEx(EntityAI e, int row, int col); /// can move from current location in cargo to new one
	proto native bool CanSwapEntityInCargo(EntityAI oldItem, EntityAI newItem);

	proto native EntityAI GetParent();
};

//-----------------------------------------------------------------------------
class Building extends EntityAI
{
};

//-----------------------------------------------------------------------------
class SoundOnVehicle extends Entity
{
};

// custom widgets
//-----------------------------------------------------------------------------
class ItemPreviewWidget extends UIWidget
{
	proto native void SetItem(EntityAI object);
	proto native EntityAI GetItem();
	
	proto native bool GetCloseup();
	proto native void SetCloseup(bool enabled);

	proto native void		SetModelOrientation(vector vOrientation);
	proto native vector GetModelOrientation();
	proto native void		SetModelPosition(vector vPos);
	proto native vector GetModelPosition();
};

//-----------------------------------------------------------------------------
class PlayerPreviewWidget extends UIWidget
{
	proto native void		SetItemInHands(InventoryItem object);
	proto native InventoryItem		GetItemInHands();
	proto native void		SetPlayer(Man player);
	proto native Man		GetPlayer();
	proto native void		SwitchMove(string move);

	proto native void		SetModelOrientation(vector vOrientation);
	proto native vector GetModelOrientation();
	proto native void		SetModelPosition(vector vPos);
	proto native vector GetModelPosition();
};


//-----------------------------------------------------------------------------
class EntityScriptedActivity
{
	void EntityScriptedActivity()
	{
	}

	void ~EntityScriptedActivity()
	{
	}

	bool OnSimulate(float timeslice)
	{
		return false;
	}

	proto native int AddRef();
	proto native int Release();
};

//-----------------------------------------------------------------------------
//! Player description
class PlayerIdentity
{
	//! ping range estimation
	proto native int GetPingMin();
	//! ping range estimation
	proto native int GetPingMax();
	//! ping range estimation
	proto native int GetPingAvg();

	//! bandwidth estimation (in kbps)
	proto native int GetBandwidthMin();
	//! bandwidth estimation (in kbps)
	proto native int GetBandwidthMax();
	//! bandwidth estimation (in kbps)
	proto native int GetBandwidthAvg();

	//! current desync level (max. error of unsent messages)
	proto native int GetDesynch();
	
	//! nick (short) name of player
	proto native owned string GetName();
	//! full name of player
	proto native owned string GetFullName();
	//! unique id of player (derived from CD key, may include verified userId after a double colon)
	proto native owned string GetId();

	private void ~PlayerIdentity()
	{
	}
};

//-----------------------------------------------------------------------------
const int PROGRESS_START = 0;
const int PROGRESS_FINISH = 1;
const int PROGRESS_PROGRESS = 2;
const int PROGRESS_UPDATE = 3;

//-----------------------------------------------------------------------------
typedef	int	HiveEventPriority;
typedef int ChatChannel;

//-----------------------------------------------------------------------------
//! state, progress, title
typedef Param3<int, float, string> ProgressEventParams;
typedef Param1<string> ScriptLogEventParams;
typedef Param1<EntityAI> EntityNetworkUpdateEventParams;
//! channel, from, text, color config class
typedef Param4<int, string, string, string> ChatMessageEventParams;
typedef Param1<int> ChatChannelEventParams;
typedef Param1<int> SQFConsoleEventParams;
//! type (TitEffectName), show, duration, title  
typedef Param4<int, int, float, string> TileEffectEventParams; 
//! player, item1, item2, actions
typedef Param4<Man, EntityAI, EntityAI, array<Action>> InventoryActionsEventParams; 
//! PlayerIdentity, PlayerPos, Top, Bottom, Shoe, Skin
typedef Param6<PlayerIdentity, vector, int, int, int, int> ClientNewEventParams; 
//! PlayerIdentity
typedef Param1<PlayerIdentity> ClientRespawnEventParams; 
//! PlayerIdentity, Man
typedef Param2<PlayerIdentity, Man> ClientReadyEventParams; 
//! PlayerIdentity, Man
typedef Param3<PlayerIdentity, Man, int> ClientDisconnectedEventParams; 
//! PlayerIdentity, QueueTime, NewChar
typedef Param3<PlayerIdentity, int, bool> ClientQueuedEventParams; 
//! Width, Height, Windowed
typedef Param3<int, int, bool> WindowsResizeEventParams;
//! Enabled
typedef Param1<bool> VONStateEventParams;
//! Camera
typedef Param1<Camera> SetFreeCameraEventParams;


//-----------------------------------------------------------------------------
#ifdef DOXYGEN
// just because of doc

enum ChatChannel
{
	CCNone,
	CCGlobal,
	CCVehicle,
	CCItemTransmitter,
	CCPublicAddressSystem,
	CCItemMegaphone,
	CCDirect,
	CCCustom1,
	CCCustom2,
	CCCustom3,
	CCCustom4,
	CCCustom5,
	CCCustom6,
	CCCustom7,
	CCCustom8,
	CCCustom9,
	CCCustom10,
	CCCustomLast = CCCustom10,
	CCStatus,
	CCSystem,
	CCN
};

enum EventType
{
	//! no params
	StartupEventTypeID,
	//! no params
	WorldCleaupEventTypeID,
	//! no params
	MPSessionStartEventTypeID,
	//! no params
	MPSessionEndEventTypeID,
	//! no params
	MPSessionFailEventTypeID,
	//! params: \ref ProgressEventParams
	ProgressEventTypeID,
	//! no params
	NetworkManagerClientEventTypeID,
	//! no params
	NetworkManagerServerEventTypeID,
	//! no params
	DialogQueuedEventTypeID,
	//! params: \ref EntityNetworkUpdateEventParams
	EntityNetworkUpdateEventTypeID,
	//! params: \ref ChatMessageEventParams
	ChatMessageEventTypeID,
	//! params: \ref ChatChannelEventParams
	ChatChannelEventTypeID,
	//! params: \ref SQFConsoleEventParams
	SQFConsoleEventTypeID,
	//! params: \ref TileEffectEventParams
	TileEffectEventTypeID,
	//! no params
	ActionMenuChangedEventTypeID,
	//! params: \ref InventoryActionsEventParams
	InventoryActionsEventTypeID,
	//! params: \ref ClientNewEventParams
	ClientNewEventTypeID,
	//! params: \ref ClientRespawnEventParams
	ClientRespawnEventTypeID,
	//! params: \ref ClientReadyEventParams
	ClientReadyEventTypeID,
	//! params: \ref ClientDisconnectedEventParams
	ClientDisconnectedEventTypeID,
	//! params: \ref ClientQueuedEventParams
	ClientQueuedEventTypeID,
	//! params: \ref ScriptLogEventParams
	ScriptLogEventTypeID,
	//! params: \ref VONStateEventParams
	VONStateEventTypeID,
	//! params: \ref SetFreeCameraEventParams
	SetFreeCameraEventTypeID
	
	//possible in engine events not accessable from script
	//ReloadShadersEvent
	//LoadWorldProgressEvent
	
	//SignStatusEvent
	//SetPausedEvent
	//TerminationEvent
	//UserSettingsChangedEvent
	//StorageChangedEvent
	//BeforeResetEvent
	//AfterRenderEvent
	//AfterResetEvent
	//CrashLogEvent
	//ConsoleEvent
};

enum TitEffectName
{
	TitPlain,
	TitPlainDown,
	TitBlack,
	TitBlackFaded,
	TitBlackOut,
	TitBlackIn,
	TitWhiteOut,
	TitWhiteIn,
	TitPlainNoFade
};
#endif
/**
 *  Game Class provide most "world" or global engine API functions.
 */
class CGame
{
	// -enUserActions
	bool m_ParamUserActions;
	// -enDebugActions
	bool m_DebugActions;
	// -enNewPlayer
	bool m_ParamNewPlayer;
	// -enNewInventory
	bool m_ParamNewInventory;
	// -enPlayerDiag
	bool m_ParamPlayerDiag;
	// -enNoLogs
	bool m_ParamNoLogs
	// -enMainMenu2
	bool m_MainMenu2;
	
	ScriptModule GameScript;

	private autoptr array<Param> m_ParamCache;
	
	void CGame()
	{
		m_ParamCache = new array<Param>;
		m_ParamCache.Insert(NULL);
	}
	
	proto native WorkspaceWidget GetWorkspace();
	
	//! 
	/**
  \brief Called when some system event occur.
  @param eventTypeId event type.
	@param params Param object, cast to specific param class to get parameters for particular event.
	*/
	void OnEvent(EventType eventTypeId, Param params)
	{
	}

	/**
	\brief Called after creating of CGame instance
	*/
	void OnAfterCreate()
	{
	}
	
	/**
  \brief Called on World update
  @param doSim False when simulation is paused, True otherwise
	@param timeslice time elapsed from last call
	*/
	void OnUpdate(bool doSim, float timeslice) 
	{
	}

	/**
  \brief Called when key is pressed
  @param key direct input key code (DIK)
	*/
	void OnKeyPress(int key)
	{
	}

	/**
  \brief Called when key is released
  @param key direct input key code (DIK)
	*/
	void OnKeyRelease(int key)
	{
	}

	/**
  \brief Called when mouse button is pressed
  @param button - number of button \ref Mouse
	*/
	void OnMouseButtonPress(int button)
	{
	}
	
	/**
  \brief Called when mouse button is released
  @param button - number of button \ref Mouse
	*/
	void OnMouseButtonRelease(int button)
	{
	}
	
	/**
  \brief create custom main menu part (submenu)
	*/
	UIScriptedMenu			CreateScriptedMenu( int id ) { }

	/**
  \brief create custom window part
	*/
	UIScriptedWindow			CreateScriptedWindow( int id ) { }	
	
	/**
  \brief Called after remote procedure call is recieved for this object
		@param target object on which remote procedure is called, when NULL, RPC is evaluated by CGame as global
		@param rpc_type user defined identification of RPC
		@param ctx read context for params
	*/
	void	OnRPC(Object target, int rpc_type, ParamsReadContext ctx)
	{
	}
	
	/**
  \brief Sets exit code and quits in the right moment
	*/
	proto native void		RequestExit( int code );

	/**
  \brief Sets exit code and restart in the right moment
	*/
	proto native void		RequestRestart(int code);

	/**
  \brief Connects to last success network session
	\return true on success, false if there is no previous session
	*/
	proto native int		ConnectLastSession( UIScriptedMenu parent , int selectedCharacter = -1 );
	/**
  \brief Disconnects from current multiplayer session
	*/
	proto native void		DisconnectSession();

	// profile functions
	/**
  \brief Gets array of strings from profile variable
	@param name of the variable
	@param values output values
	\n usage:
	@code
	autoptr TStringArray lastInventoryArray = new TStringArray;
	GetGame().GetProfileStringList("lastInventory", lastInventoryArray);
	@endcode
	*/
	proto native void		GetProfileStringList(string name, out TStringArray values);

	/**
  \brief Gets string from profile variable
	@param name of the variable
	@param value output value
	\return true when successful
	*/
	proto bool					GetProfileString(string name, out string value);

	/**
  \brief Sets array of strings to profile variable
	@param name of the variable
	@param values to set
	*/
	proto native void		SetProfileStringList(string name, TStringArray values);

	/**
  \brief Sets string to profile variable
	@param name of the variable
	@param value to set
	*/
	proto native void		SetProfileString(string name, string value);

	/**
  \brief Saves profile on disk.
	*/
	proto native void		SaveProfile();

	/**
  \brief Gets current player name
	@param name output value
	*/
	proto void					GetPlayerName(out string name);

	/**
  \brief Sets current player name
	@param name
	*/
	proto native void		SetPlayerName(string name);
	
	/**
  	\brief Assign player entity to client (in multiplayer)
	\note Works only on server
	@param name
	*/	
	proto native Entity	CreateEntity(PlayerIdentity identity, string name, vector pos, float radius, string spec);
	
	/**
  	\brief Set camera for player
	\note Works only on server
	@param Player object
	*/	
	proto native void		SelectPlayer(PlayerIdentity identity, Object player);
	
	/**
  	\brief Sets client as disconnected after log-out timeout
	\note Works only on server
	@param name
	*/	
	proto native void		SetPlayerDisconnected(PlayerIdentity identity);
	
	/**
  	\brief Sets client to leave waiting screen after log-in time
	\note Works only on client
	*/
	proto native void		SetPlayerWaited();
	
	/**
  	\brief Returns current daytime on server
	*/
	proto native float		GetDayTime();
	
	// config functions
	/**
	\brief Get string value from config on path.
		@param path of value, classes are delimited by empty space. You can specify config file by using "configFile" or "missionConfigFile" as a first class name.
		@param value output
		\return true on success
	*/
	proto bool					ConfigGetText(string path, out string value);
	
	/**
	\brief Get string value from config on path.
		@param path of value, classes are delimited by empty space. You can specify config file by using "configFile" or "missionConfigFile" as a first class name.
		\return value output string
	*/
	string ConfigGetTextOut(string path)
	{
		string ret_s;
		ConfigGetText(path, ret_s);
		return ret_s;
	}
	
	/**
  \brief Get float value from config on path.
	@param path of value, classes are delimited by empty space. You can specify config file by using "configFile" or "missionConfigFile" as a first class name.
	\return value
	*/
	proto native float	ConfigGetFloat(string path);
	

	/**
  \brief Get vector value from config on path.
	@param path of value, classes are delimited by empty space. You can specify config file by using "configFile" or "missionConfigFile" as a first class name.
	\return value
	*/
	proto native vector ConfigGetVector(string path);

	/**
  \brief Get int value from config on path.
	@param path of value, classes are delimited by empty space. You can specify config file by using "configFile" or "missionConfigFile" as a first class name.
	\return value
	*/
	proto native int		ConfigGetInt(string path);

	/**
  \brief Returns type of config value
	@param path of value, classes are delimited by empty space. You can specify config file by using "configFile" or "missionConfigFile" as a first class name.
	\return one of constants CT_INT, CT_FLOAT, CT_STRING, CT_ARRAY, CT_CLASS, CT_OTHER
	*/
	proto native int		ConfigGetType(string path);

	/**
  \brief Get array of strings from config on path.
	@param path of value, classes are delimited by empty space. You can specify config file by using "configFile" or "missionConfigFile" as a first class name.
	@param value output
	\n usage :
	@code
	autoptr TStringArray characterAnimations = new TStringArray;
	GetGate().ConfigGetTextArray("CfgMovesMaleSdr2 States menu_idleUnarmed0 variantsPlayer", characterAnimations);
	@endcode
	*/
	proto native void		ConfigGetTextArray(string path, out TStringArray values);

	/**
  \brief Get array of floats from config on path.
	@param path of value, classes are delimited by empty space. You can specify config file by using "configFile" or "missionConfigFile" as a first class name.
	@param value output
	*/
	proto native void		ConfigGetFloatArray(string path, out TFloatArray values);
	
	/**
  \brief Get array of integers from config on path.
	@param path of value, classes are delimited by empty space. You can specify config file by using "configFile" or "missionConfigFile" as a first class name.
	@param value output
	*/
	proto native void		ConfigGetIntArray(string path, out TIntArray values);

	/**
  \brief Get name of subclass in config class on path.
	@param path of value, classes are delimited by empty space. You can specify config file by using "configFile" or "missionConfigFile" as a first class name.
	@param index of subclass in class
	@param name output
	\return true on success, false if path or child is not found
	*/
	proto bool					ConfigGetChildName(string path, int index, out string name);

	/**
  \brief Get name of base class of config class on path.
	@param path of value, classes are delimited by empty space. You can specify config file by using "configFile" or "missionConfigFile" as a first class name.
	@param name output
	\return true on success, false if path or base is not exists
	*/
	proto bool					ConfigGetBaseName(string path, out string base_name);

	/**
  \brief Get count of subclasses in config class on path.
	@param path of value, classes are delimited by empty space. You can specify config file by using "configFile" or "missionConfigFile" as a first class name.
	@param index of parameter in class
	@param name output
	\return true on success, false if path or child is not found
	*/
	proto native int		ConfigGetChildrenCount(string path);
	proto native bool		ConfigIsExisting(string path);

	proto native void		ConfigGetFullPath(string path, out TStringArray full_path);
	proto native void		ConfigGetObjectFullPath(Object obj, out TStringArray full_path);

/**
\brief Converts array of strings into single string.
	\param Array of strings like {"All", "AllVehicles", "Land"}
	\return Converts given array into something lile "All AllVehicles Land".
	@code
		???
	@endcode
*/
	string					ConfigPathToString(TStringArray array_path)
	{
		string return_path = "";
		int count = array_path.Count();
		
		for (int i = 0; i < count; i++)
		{
			return_path += array_path.Get(i);
			
			if ( i < count - 1 )
			{
				return_path += " ";
			}
		}
		
		return return_path;
	}

	/**
  \brief Get command line parameter value.
	@param name of parameter
	@param value output
	\return true when parameter exists, false otherwise
	\n usage :
	@code
	// you run e.g.: DayZInt.exe -scriptDebug=true
	string value;
	if (GetGame().CommandlineGetParam("scriptDebug", value) && value == "true")
	{
		Print("Script debugging on!");
	}
	@endcode
	*/
	proto bool					CommandlineGetParam(string name, out string value);
	
	proto native void CopyToClipboard(string text);
	proto void				CopyFromClipboard(out string text);

	proto native void 	BeginOptionsVideo();
	proto native void 	EndOptionsVideo();
	
	// entity functions
	/**
	\brief Preload objects with certain type in certain distance from camera.
	@param type of objects which should be preloaded
	@param distance from camera in which objects should be preloaded
	\return true on success
	*/
	proto native bool		PreloadObject( string type, float distance );

	/**
	\brief Creates object of certain type
	@param type of objects to create
	@param pos position where to create object
	@param create_local if True, object is not spawned on clients only on server
	@param init_ai if creating object is LightAI class, by this param is initialised AI or not
	\return new Object
	*/
	proto native Object CreateObject( string type, vector pos, bool create_local = false, bool init_ai = false );
	proto native SoundOnVehicle CreateSoundOnObject(Object source, string sound_name, float distance, bool looped);
	proto native void   ObjectDelete( Object obj );
	proto native int    ObjectRelease( Object obj );
	proto void          ObjectGetType( Object obj, out string type );
	proto void          ObjectGetDisplayName( Object obj, out string name );
	proto native void   ObjectSetAnimationPhase(Entity obj, string animation, float phase);
	proto native float  ObjectGetAnimationPhase(Entity obj, string animation);
	proto native vector ObjectGetSelectionPosition(Object obj, string name);
	proto native vector ObjectModelToWorld(Object obj, vector modelPos);
	proto native vector ObjectWorldToModel(Object obj, vector worldPos);
	//! returns true if player can access item's cargo/attachments (check only distance)
	proto native bool		IsObjectAccesible(EntityAI item, Man player);
	//! returns true if player can access item's cargo/attachments (check only inventory conditions)
	proto native bool		IsInventoryTreeLocked(EntityAI item, EntityAI player);	

	// input
	proto native Input	GetInput();
	
	// inventory stuff, MP safe
	/*proto native void		InventorySetItemShortcut(Man owner, InventoryItem item, int index);
	proto native void		InventoryDropItem(Man owner, InventoryItem item);
	proto native void		InventoryMoveItem(EntityAI target, int index, InventoryItem item);
	proto native int 		InventoryCanMoveItem(EntityAI target, int index, InventoryItem item);
	proto native void		InventoryTakeItemToHands(Man man, InventoryItem item);
	proto native int 		InventorySwapItems(Man owner, InventoryItem item1, InventoryItem item2);
	proto native int 		InventoryCanSwapItems(InventoryItem item1, InventoryItem item2);*/

	// support
	//! Executes SQF expression, if there is an error, is printed into the script console
	proto native void		ExecuteSQF(string expression);
	//! Executes Enforce Script expression, if there is an error, is printed into the script console
	proto native bool 	ExecuteEnforceScript(string expression, string mainFnName);

	proto native bool 	ScriptTest();
	//! Get list of all debug modes
	proto native void		GetDiagModeNames(out TStringArray diag_names);
	//! Set specific debug mode
	proto native void		SetDiagModeEnable(int diag_mode, bool enabled);
	//! Gets state of specific debug mode
	proto native bool		GetDiagModeEnable(int diag_mode);

	//! Get list of all debug draw modes
	proto native void		GetDiagDrawModeNames(out TStringArray diag_names);
	//! Set debug draw mode
	proto native void		SetDiagDrawMode(int diag_draw_mode);
	//! Gets current debug draw mode
	proto native int		GetDiagDrawMode();
	
	/**
	\brief Returns average FPS of last 16 frames
	@return fps in milliseconds
	*/
	proto native float		GetFps();
	
	/**
	\brief Returns current time from start of the game
	@return time in milliseconds
	*/
	proto native float		GetTickTime();

	//! return inventory slot id for given inventory slot name
	proto native int		GetInventorySlot(string slot_name);
	proto void					GetInventoryItemSize(InventoryItem item, out int width, out int height);
	/**
  \brief Returns list of all objects in radius "radius" around position "pos"
	@param pos
	@param radius
	@param inPlayerSight if inPlayerSight is not NULL, function raycast every object if is visible by player "inPlayerSight" (heavy performance load!)
	@param objects output array
	*/
	proto native void		GetObjectsAtPosition(vector pos, float radius, Man inPlayerSight, out array<Object> objects, out array<Cargo> proxyCargos);
	proto native World	GetWorld();
	proto void					GetWorldName( out string world_name );
	proto native vector	SnapToGround( vector pos );
	proto void					FormatString( string format, string params[], out string output);
	proto void					HtmlToPlain( string text );
	proto void					GetVersion( out string version );
	proto native UIManager	GetUIManager();
	proto native Man		GetPlayer();
	proto native void		GetPlayers( out array<Man> players );
	
	// actions & activities
	EntityScriptedActivity CreateActivity(Object target, Man man, int activity_type)
	{
		Print(activity_type);
		return NULL;
	}

	proto native void		PerformAction(Man player, Action action);
	proto native void		GetActionMenuActions(out TActionRefArray actions);
	//! Updates action menu items, call UpdateCursorTarget prior this to update aiming target
	proto native void		UpdateActionMenuActions();
	
	//! Updates target of player aiming
	proto native void		UpdateCursorTarget();
	//! Returns world position where player aims, call UpdateCursorTarget to refresh this data
	proto native vector		GetCursorPos();
	//! Returns object on which player aims, call UpdateCursorTarget to refresh this data
	proto native Object 	GetCursorObject();
	//! Returns name of component/selection on which player aims, call UpdateCursorTarget to refresh this data. The object must have View Geometry or Fire Geometry. View Geometry has priority.
	proto void 	            GetCursorObjectComponentName( out string component_name );
	//! Returns the direction where the mouse points, from the camera view
	proto native vector		GetPointerDirection();
	//! Transforms position in world to position in screen in pixels as x, y component of vector, z parameter represents distance between camera and world_pos
	proto native vector		GetScreenPos(vector world_pos);
	//! Transforms position in world to position in screen in percentage (0.0 - 1.0) as x, y component of vector, z parameter represents distance between camera and world_pos
	proto native vector		GetScreenPosRelative(vector world_pos);
	
	//! Returns world position where player aims (works only on server)
	proto native vector		GetPlayerCursorPos(Man man);
	//! Returns object on which player aims,(works only on server)
	proto native Object 	GetPlayerCursorObject(Man man);
	//! Returns name of component/selection on which player aims (works only on server)
	proto void 	            GetPlayerCursorObjectComponentName( Man man, out string component_name );
	//! Returns true if player is aiming on component with given name(works only on server)
	proto native bool       GetPlayerCursorObjectComponentNameQuery( Man man, string component_name );

	//! Returns position of crosshair in pixels
	proto void 	            GetCrosshairScreenPos( out float x, out float y );
	
	//! Return singleton of MenuData class - at main menu contains characters played with current profile.
	proto native MenuData	GetMenuData();
	/**
  \brief Initiate remote procedure call. When called on client, RPC is evaluated on server; When called on server, RPC is executed on all clients
	@param target object on which remote procedure is called, when NULL, RPC is evaluated by CGame as global
	@param rpc_type user defined identification of RPC
	@param params custom params array
	@param recipient specified client to send RPC to. If NULL, RPC will be send to all clients (specifying recipient increase security and decrease network traffic)
	@code
	const int RPC_LOW_BLOOD_PRESSURE_EFFECT = 0;
	...
	// on server side, after blood pressure of player is low...
	autoptr Param1<float> p = new Param1<float>(m_blood_pressure);
	autoptr array<param> params = new array<param>;
	params.Insert(p);
	GetGame().RPC(player, RPC_LOW_BLOOD_PRESSURE_EFFECT, params);
	// or shortcut
	GetGame().RPCSingleParam(player, RPC_LOW_BLOOD_PRESSURE_EFFECT, p);
	...
	// on client
	class PlayerBase
	{
		// override OnRPC function
		void OnRPC(int rpc_type, ParamsReadContext ctx)
		{
			// dont forget to propagate this call trough class hierarchy!
			super.OnRPC(rpc_type, ctx);

			switch(rpc_type)
			{
				case RPC_LOW_BLOOD_PRESSURE_EFFECT:
					autoptr Param1<float> p = Param1<float>(0);
					if (ctx.Read(p))
					{
						float blood_pressure = p.param1;
						float effect_intensity = 1.0 - blood_pressure;
						ShowFancyScreenEffect(effect_intensity);
					}
				break;
			}
		}
	}
	@endcode
	*/
	proto native void		RPC(Object target, int rpc_type, notnull array<Param> params, Man recipient = NULL);
	//! see CGame.RPC
	void					RPCSingleParam(Object target, int rpc_type, Param param, Man recipient = NULL)
	{
		m_ParamCache.Set(0, param);
		RPC(target, rpc_type, m_ParamCache, recipient);
	}

	//! Use for profiling code from start to stop, they must match have same name, look wiki pages for more info: https://confluence.bistudio.com/display/EN/Profiler
	proto native void		ProfilerStart(string name);
	//! Use for profiling code from start to stop, they must match have same name, look wiki pages for more info: https://confluence.bistudio.com/display/EN/Profiler
	proto native void		ProfilerStop(string name); 

	/**
	\brief Returns list of names of recipes for certain player and items combination.
	@param player
	@param item1
	@param item2
	@param recipe_names output, results are inserted here
	*/
	proto native void		RecipesGet(Man player, EntityAI item1, EntityAI item2, out TStringArray recipe_names);
	/**
	\brief Returns true, if items has any recipes for certain player 
	@param player
	@param item1
	@param item2
	*/
	proto native bool		RecipesHas(Man player, EntityAI item1, EntityAI item2);
	/**
	\brief Process recipe.
	@param recipe_name name of recipe (you can get recipe names using RecipesGet function)
	@param player
	@param item1
	@param item2
	*/
	proto native void		RecipeProcess(string recipe_name, Man player, EntityAI item1, EntityAI item2);
	/**
	\brief Generate description for recipe for certain player and items combination.
	@param recipe_name name of recipe (you can get recipe names using RecipesGet function)
	@param player
	@param item1
	@param item2
	@param text final description is outputted here
	*/
	proto void				RecipeGetDescription(string recipe_name, Man player, EntityAI item1, EntityAI item2, out string text);
/**
 \brief Prints text into game chat.
	\param text to print
	\param colorClass ??
	\n usage :
	@code
		GetGame().Chat("Item splitted", "colorAction");
	@endcode
*/
	proto native void		Chat(string text, string colorClass);
	proto native void		ChatMP(Man recipient, string text, string colorClass);
	proto native void		ChatPlayer(ChatChannel channel, string text);
	//! Returns current chat channel.
	proto native ChatChannel ChatGetChannel();

	// sound
	/**
 \brief Unit or object will say given sound in 3D Space. This allows broadcasting of positional music or sound from a source, without having to script a fade sound or music command. Sound is defined in CfgSound of the Description.ext.
 \param unit: Object - position of sound is taken from position of this object
 \param sound: String - classname. Defined in Description.ext
 \param distance: float - how far is dound hearable
 */
	proto native void		Say3D(Object source, string sound_name, float distance);

	// mission
	proto native Mission 	GetMission();
	proto native void		SetMission(Mission mission);
	
	//! Starts mission (equivalent for SQF playMission). You MUST use double slash \\ 
	proto native void		PlayMission(string path);
	
	//! Create only enforce script mission, used for mission script reloading 
	proto protected native void		CreateMission(string path);
	proto native void		RestartMission();
	proto native void		AbortMission();
	proto native void		RespawnPlayer();
	proto native bool		CanRespawnPlayer();
	proto native int		RetryMission();

	proto native bool		IsMultiplayer();
	proto native bool		IsClient();
	proto native bool		IsServer();
	
	// Interny build
	proto native bool		IsDebug();
	bool IsParamUserActions()
	{
		return m_ParamUserActions;
	}
	
	bool IsNewPlayer()
	{
		return m_ParamNewPlayer;
	}

	bool IsNewInventory()
	{
		return m_ParamNewInventory;
	}
	
	bool IsNoLogs()
	{
		return m_ParamNoLogs;
	}
	
	bool IsDebugActions()
	{
		return m_DebugActions;
	}
	
	bool IsNewUI()
	{
		return true;
	}
	
	bool IsPlayerDiag()
	{
		return m_ParamPlayerDiag;
	}

	bool IsMainMenu2()
	{
		return m_MainMenu2;
	}

	proto native void		GetPlayerIndentities( out array<PlayerIdentity> identities );
	
	proto native float		SurfaceY(float x, float z);
	proto native float		SurfaceRoadY(float x, float z);
	proto void				SurfaceGetType(float x, float z, out string type);
	proto native vector		SurfaceGetNormal(float x, float z);
	proto native float		SurfaceGetSeaLevel();
	proto native bool		SurfaceIsSea(float x, float z);
	proto native bool		SurfaceIsPond(float x, float z);
	
	proto native void 		UpdatePathgraphRegion(vector regionMin, vector regionMax);
	
	void 					UpdatePathgraphRegionByObject(Object object)
	{
		vector pos = object.GetPosition();
		vector min_max[2];
		float radius = object.ClippingInfo ( min_max );
		vector min = Vector ( pos[0] - radius, pos[1], pos[2] - radius );
		vector max = Vector ( pos[0] + radius, pos[1], pos[2] + radius );
		UpdatePathgraphRegion( min, max );
	}
	
	/**
	\brief Finds all objects that are in choosen oriented bounding box (OBB)
		\param center \p vector, center of OBB
		\param orientation \p vector, direction (front vector), used for calculation of OBB rotation
		\param edgeLength \p vector, sizes of whole box
		\param excludeObjects \p array<Object>, objects that should be excluded from collision check
		\param collidedObjects \p array<Object>, out parameter, objects that collided with OBB
		\returns \p bool, \p true if at least one object collided with OBB, \p false otherwise
		\note Object that doesn't have collision geometry will be ignored
		
		@code
			vector pos = GetPosition();
			vector orientation = GetOrientation();
			vector size = "10 4 8";
			autoptr array<Object> excluded_objects = new array<Object>;
			excluded_objects.Insert(this);
			autoptr array<Object> nearby_objects = new array<Object>;
			
			if(GetGame().IsBoxColliding( pos, orientation, size, excluded_objects, nearby_objects))
			{
				for (local int i = 0, c = nearby_objects.Count(); i < c; ++i)
				{
					PrintString("object " + i.ToString());
				}
			}
		@endcode
	*/
	proto native bool		IsBoxColliding(vector center, vector orientation, vector edgeLength, array<Object> excludeObjects, array<Object> collidedObjects = NULL); 
	
	/**
	\brief Raycasts world by given parameters
	\param begPos \p vector 
	\param endPos \p vector 
	\param contactPos \p vector out, world position of first contact
	\param contactDir \p vector out, direction of first contact (available only when object is hitted)
	\param contactComponent \p int out, object component index (available only when object is hitted)
	\param results \p set<Object> out, set of objects hitted by raycast. Can be NULL if not needed
	\param with \p Object
	\param ignore \p Object
	\param sorted \p bool
	\param ground_only \bool raycasts only ground (ignores all objects)
	\param iType \p int, type of intersection, possible values \ref ObjIntersect
	\param radius \p float

	\returns \p bool return true if raycast hits ground or object
	@code
	// raycast test
	if (player)
	{
		vector from = player.GetPosition();
		vector to = from + (player.GetDirection() * 100);
		vector contactPos;
		vector contactDir;
		int contactComponent;

		if ( GetGame().Raycast(from, to, contactPos, contactDir, contactComponent) )
		{
			Print(contactPos);
			Print(contactDir);
			Print(contactComponent);
		}
	}
	@endcode
	*/
	proto bool				Raycast(vector begPos, vector endPos, out vector contactPos, out vector contactDir, out int contactComponent, out set<Object> results = NULL, Object with = NULL, Object ignore = NULL,  bool sorted = false,  bool ground_only = false,  int iType = ObjIntersectView, float radius = 0.0);

//-----------------------------------------------------------------------------
// weather
//-----------------------------------------------------------------------------

	//! Returns actual sky overcast in range <0, 1> where 0 means no cloud in the sky at all.
	proto native float	GetOvercast();
	//! Returns overcast value towards the weather is heading.
	proto native float	GetOvercastForecast();
	/*!
		\brief Sets the overcast forecast.
		\param  overcast     Desired overcast value that should be met in given time.
		\param  time         A time of the next change (how long it takes in seconds to interpolate to given value).
		\param  minDuration  A minimal time in seconds the change will last.
	*/
	proto native void		SetOvercast( float overcast, float time = 0, float minDuration = 0 );
	//! Returns actual fog density in range <0, 1> where 0 means no fog.
	proto native float	GetFog();
	//! Returns fog density towards the weather is heading.
	proto native float	GetFogForecast();
	/*!
		\brief Sets the fog density forecast.
		\param  fog          Desired fog density that should be met in given time.
		\param  time         A time of the next change (how long it takes in seconds to get to given value).
		\param  minDuration  A minimal time in seconds the change will last. 
	*/
  proto native void		SetFog( float fog, float time = 0, float minDuration = 0 );
	//! Returns actual rain density in range <0, 1> where 0 means no rain.
  proto native float	GetRain();
	/*!
		\brief Returns rain density towards the weather is heading.
		\note Keep in mind that if the overcast value is small, rain values are not updated.
		      So rain forecast and its next time change can be misleading.
	*/
	proto native float	GetRainForecast();
	//! Returns remaining time in seconds until next rain forecast will be computed.
	proto native float	GetRainNextChange();
	/*!
		\brief Sets the rain forecast.
		\param  rain         Desired rain density that should be met in given time.
		\param  time         A time of the next change (how long it takes in seconds to get to given value).
		\param  minDuration  A minimal time in seconds the change will last. 
	*/
	proto native void		SetRain( float density, float time = 0, float minDuration = 0 );
	//! Sets the time in seconds from now when next rain forecast should be computed.
	proto native void		SetRainNextChange( float time );
	/*!
		\brief Sets the properties of thunderstorms.
		\param  density  A value in <0, 1> range where 0 means no thunderstorms at all
		                 and 1 means thunderstorm every time it gets cloudy.
		\param  timeOut  A minimal time in seconds before next thunder can be created.
	*/
	proto native void		SetStorm( float density, float timeOut );
	/*!
		\brief Returns actual wind force in range <0, 1> where 0 means no wind and 1 means strong wind.
		\note Wind is changing very often in the game, so the returned value may not stand for too long if not forced.
	*/
	proto native float	GetWindForce();
	//! Returns wind vector (direction and speed as length of the vector).
	proto native vector	GetWind();
	/*!
		\brief Sets the actual wind force in range <0, 1> where 0 means no wind and 1 means strong wind (current maximum wind speed is 35 m/s).
		\note Wind is changing very often in the game, so the set value may not stand for too long if not forced.
	*/
	proto native void		SetWindForce( float wind );
	/*!
		\brief Sets the wind vector (direction and speed as length of the vector).
		\param  direction  Wind vector (direction and speed as length of the vector).
		\param  force      If true forces the wind to keep the given props (turns off updating).
		\note Wind is changing very often in the game, so the set value may not stand for too long if not forced.
	*/
	proto native void		SetWind( vector direction, bool force = false );
	//! Returns actual time from start of a server (how many seconds elapsed from server start).
	proto native float	GetWeatherTime();
	//! Returns remaining time in seconds until next weather forecast will be computed.
	proto native float	GetNextWeatherChange();
	//! Sets the time in seconds from now when next weather forecast should be computed.
	proto native void		SetNextWeatherChange( float time );
	// reset
	proto native void	ResetWeather();
	//! Returns actual air temperature in degrees Celsius.
	proto native float	GetAirTemperature();
	//! Sets custom camera camera EV.
	proto native void	SetEVUser(float value);
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// persitence
//-----------------------------------------------------------------------------

	//! Returns EntityAI by its persistent ID parts
	//! or null if entity with given persistent ID does not exists.
	/*!
		This function returns valid data only inside AfterLoad event.
		Do not use this in anywhere else.

		Its main purpose is only for keep track on object connections
		after server restarts, all data related to this function are deleted
		when server initializtion is done.
	*/
	proto native EntityAI GetEntityByPersitentID( int low, int high );

//-----------------------------------------------------------------------------

#ifdef DOXYGEN
	// just because of doc
	enum HiveEventPriority
	{
		EVENT_PRIORITY_ERROR,
		EVENT_PRIORITY_WARNING,
		EVENT_PRIORITY_INFO
	};
#endif

	//! Add hive event with given data
	proto native bool AddEvent( HiveEventPriority priority, string msg_text, int timeout = -1 );
	//! Check if any hive event is pending
	proto native bool	CheckHiveEvents();
	//! Return text and seconds of the pending event
	proto void				GetLastHiveEvent(out string msg_text, out int value, out int priority);

/**
\brief Returns is class name inherited from parent class name
	\param cfg_class_name \p Config Class name ("Animal_CervusElaphus")
	\param cfg_parent_name \p Parent Config Class name ("DZ_LightAI")
	\returns \p bool is class name inherited from parent class name
	@code
		bool is_kind = GetGame().IsKindOf( "Animal_CervusElaphus", "DZ_LightAI");
		PrintString(ToString(is_kind));
		
		>> 1
	@endcode
*/
	bool IsKindOf(string cfg_class_name, string cfg_parent_name)
	{
		TStringArray full_path = new TStringArray;
		
		ConfigGetFullPath("CfgVehicles " + cfg_class_name, full_path);
	
		if (full_path.Count() == 0)
		{
			ConfigGetFullPath("CfgAmmo " + cfg_class_name, full_path);
		}
		
		if (full_path.Count() == 0)
		{
			ConfigGetFullPath("CfgMagazines " + cfg_class_name, full_path);
		}
		
		if (full_path.Count() == 0)
		{
			ConfigGetFullPath("cfgWeapons " + cfg_class_name, full_path);
		}
	
		if (full_path.Count() == 0)
		{
			ConfigGetFullPath("CfgNonAIVehicles " + cfg_class_name, full_path);
		}
		
		cfg_parent_name.ToLower();
		for (int i = 0; i < full_path.Count(); i++)
		{
			string tmp = full_path.Get(i);
			tmp.ToLower();
			if (tmp == cfg_parent_name)
			{
				delete full_path;
				return true;
			}
		}
	
		delete full_path;
		return false;
	}

/**
\brief Returns is object inherited from parent class name
	\param object \p Object
	\param cfg_parent_name \p Parent Config Class name ("DZ_LightAI")
	\returns \p bool is object inherited from parent class name
	@code
		bool is_kind = GetGame().IsKindOf( my_animal, "DZ_LightAI");
		PrintString(ToString(is_kind));
		
		>> 1
	@endcode
*/
	bool ObjectIsKindOf(Object object, string cfg_parent_name)
	{
		TStringArray full_path = new TStringArray;
		ConfigGetObjectFullPath(object, full_path);
	
		cfg_parent_name.ToLower();
	
		for (int i = 0; i < full_path.Count(); i++)
		{
			string tmp = full_path.Get(i);
			tmp.ToLower();
			if (tmp == cfg_parent_name)
			{
				delete full_path;
				return true;
			}
		}
	
		delete full_path;
		return false;
	}
	
	/**
	Searches given config path (config_path) for the given member (searched_member) and returns its index.
	\nReturns -1 if not found.
	\n usage:
	* @code
	* int skinning_class_index = g_Game.ConfigFindClassIndex(cfgPath_animal, "Skinning");
	* @endcode
	*/
	int ConfigFindClassIndex(string config_path, string searched_member)
	{
		int class_count = ConfigGetChildrenCount(config_path);
		for (int index = 0; index < class_count; index++)
		{
			string found_class = "";
			ConfigGetChildName(config_path, index, found_class);
			if (found_class == searched_member)
			{
				return index;
			}
		}
		return -1;
	}

	//!returns mission time in milliseconds	
	proto int GetTime();

	/**
  Returns CallQueue for certain category
  @param call_category call category, valid values are:
	\n CALL_CATEGORY_SYSTEM - calls & timers in this queue are processed every time without any restrictions
	\n CALL_CATEGORY_GUI - calls & timers in this queue are processed when GUI is enabled (even during pase game)
	\n CALL_CATEGORY_GAMEPLAY - calls & timers in this queue are processed only during mission, when game is not paused
	\n usage:
	* @code
	* GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(this, "Refresh"); // calls "Refresh" function on "this" with no arguments
	* GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(this, "Show", new Param1<bool>(true)); // calls "Show" function on "this" with one bool argument
	* GetGame().GetCallQueue(CALL_CATEGORY_GUI).Call(this, "SetPos", new Param2<float, float>(0.2, 0.5)); // calls "SetPos" function on "this" with two float arguments
	* @endcode
	*/
	CallQueue GetCallQueue(int call_category) {}

	/**
  Returns TimerQueue for certain category
  @param call_category call category, valid values are:
	\n CALL_CATEGORY_SYSTEM - calls & timers in this queue are processed every time without any restrictions
	\n CALL_CATEGORY_GUI - calls & timers in this queue are processed when GUI is enabled (even during pase game)
	\n CALL_CATEGORY_GAMEPLAY - calls & timers in this queue are processed only during mission, when game is not paused
	*/
	TimerQueue GetTimerQueue(int call_category) {}

	/**
  Returns DragQueue. Callbacks are called on mouse move until mouse button is released, then the queue is cleaned.
	*/
	DragQueue GetDragQueue() {}
	
	
	bool IsInventoryOpen()
	{
	}
};

// some defs for CGame::ShowDialog()
/*
const int DBB_NONE = 0;
const int DBB_OK = 1;
const int DBB_YES = 2;
const int DBB_NO = 3;
const int DBB_CANCEL = 4;

const int DBT_OK = 0;		//just OK button
const int DBT_YESNO = 1;	//Yes and No buttons
const int DBT_YESNOCANCEL = 2; //Yes, No, Cancel buttons

const int DMT_NONE = 0;
const int DMT_INFO = 1;
const int DMT_WARNING = 2;
const int DMT_QUESTION = 3;
const int DMT_EXCLAMATION = 4;
*/

proto native CGame GetGame();

class Hud
{
	autoptr Timer m_Timer;
	void Init( Widget hud_panel_widget ) {}
	void DisplayNotifier( int key, int tendency ) {}
	void DisplayBadge( int key, bool show ) {}
	void SetStamina( int value, int range ) {}
	void DisplayStance( int stance ) {}
	void DisplayPresence() {}
};

//-----------------------------------------------------------------------------
//! Mission class
class Mission
{
	ScriptModule MissionScript;

	void OnInit()	{}
	void OnMissionStart() {}
	void OnMissionFinish()	{}
	void OnUpdate(float timeslice) {}
	void OnKeyPress(int key) {}
	void OnKeyRelease(int key) {}
	void OnMouseButtonPress(int button){}
	void OnMouseButtonRelease(int button){}
	void OnEvent(EventType eventTypeId, Param params) {}
	void OnItemUsed(InventoryItem item, Man owner) {}	
	
	Hud GetHud()
	{ 
		return NULL;
	}
	
	EntityScriptedActivity	CreateActivity(Object target, Man man, int activity_type)
	{
		return NULL;
	}

	UIScriptedMenu	CreateScriptedMenu( int id ) 
	{ 
		return NULL;
	}
	
	UIScriptedWindow	CreateScriptedWindow( int id ) 
	{ 
		return NULL;
	}
	
	bool IsPaused() 
	{ 
		return false; 
	}
	
	bool IsGame()
	{
		return false;
	}
	
	bool IsServer()
	{
		return false;
	}

	void Pause() {}
	void Continue() {}
	
	void RefreshCrosshairVisibility() {}
	
	bool IsMissionGameplay()
	{
		return false;
	}
};

// -------------------------------------------------------------------------
/*
const int CamEffectTop = 0;
const int CamEffectLeft = 1;
const int CamEffectRight = 2;
const int CamEffectFront = 3;
const int CamEffectBack = 4;
const int CamEffectLeftFront = 5;
const int CamEffectRightFront = 6;
const int CamEffectLeftBack = 7;
const int CamEffectRightBack = 8;
const int CamEffectLeftTop = 9;
const int CamEffectRightTop = 10;
const int CamEffectFrontTop = 11;
const int CamEffectBackTop = 12;
const int CamEffectBottom = 13;
*/
// --------------------------------------------------------------------------

class MenuData
{
	proto native int	GetCharactersCount();
	proto native int 	GetLastPlayedCharacter();
	proto native Man 	CreateCharacterPerson(int index);

	//proto native void	GetCharacterStringList(int characterID, string name, out TStringArray values);
	//proto bool			GetCharacterString(int characterID,string name, out string value);
}

class World
{
	//proto private void ~World();
	//proto private void World();

	proto native void	GetPlayerList(out array<Man> players);
	
	/**
	\brief Get actual ingame world time
		\param year
		\param month in range <1, 12>
		\param day in range <1, 31>
		\param hour in range <0, 23>
		\param minute in range <0, 59>
		@code
			int year, month, day, hour, minute;
			GetGame().GetWorld().GetDate(year, month, day, hour, minute);
		@endcode
	*/
	proto void			GetDate(out int year, out int month, out int day, out int hour, out int minute);
	
	/**
	\brief Sets actual ingame world time
		\param year
		\param month in range <1, 12>
		\param day in range <1, 31>
		\param hour in range <0, 23>
		\param minute in range <0, 59>
		@code
			int year = 2016;
			int month = 5;
			int day = 4;
			int hour = 14;
			int minute = 57;
			GetGame().GetWorld().SetDate(year, month, day, hour, minute);
		@endcode
	*/
	proto native void	SetDate(int year, int month, int day, int hour, int minute);
	proto native float	GetLatitude();
	proto native float	GetLongitude();
	proto native float	GetMoonIntensity();
	proto native float	GetSunOrMoon();
	
	proto native float	GetEyeAccom();
	proto native void	SetEyeAccom(float eyeAccom);
	proto native void	SetCameraEffect( Object obj, string name, int pos, bool infinite );	
	proto native Camera	CreateFreeCamera();
	proto native void	DeleteFreeCamera();	
	proto native void	StartFpsBenchmark(int sectorsCount, float preloadDistance, float fpsLimitRed, float fpsLimitGreen, float fpsLimitBlue, float minX, float minY, float maxX, float maxY);
	proto native void	StartFpsBenchmarkWithMask(float preloadDistance, float fpsLimitRed, float fpsLimitGreen, float fpsLimitBlue);
	proto native void	StartFpsBenchmarkViews(string xmlFileName, float preloadDistance);
	
	/*!
	@code
	Material matColors = GetGame().GetWorld().GetMaterial("postprocess/colors");
	SetMaterialParam(matColors, "Saturation", 0.8);
	float color[4];
	color[0] = 0.5;
	color[1] = 0.8;
	color[2] = 0.7;
	color[3] = 0.6;
	SetMaterialParam(matColors, "OverlayColor", color);
	@endcode
	*/
	proto native Material GetMaterial(string materialName);

	/*!
	set new PP effect and its material
	\param camera					number of camera
	\param effectIndex		index of effect
	\param effectName			name of effect
	\param matName				name of material
	*/
	proto native void		SetCameraPostProcessEffect(int cam, int ppEffect, string effectName, string materialName);
	
	void SetAperture(float invDiameter)
	{
		if (invDiameter <= 0)
		{
			SetEyeAccom(-1);
		}
		else
		{
			float apertureArea = 1.0 / invDiameter;
			SetEyeAccom(apertureArea * apertureArea);
		}
	}
};

// -------------------------------------------------------------------------
/*
// Option Access Type
const int AT_UNKNOWN = 0;
const int AT_OBJECTS_DETAIL = 1;
const int AT_TEXTURE_DETAIL = 2;
const int AT_VRAM_VALUE = 3;
const int AT_HDR_DETAIL = 4;
const int AT_FSAA_DETAIL = 5;
const int AT_VSYNC_VALUE = 6;
const int AT_ANISO_DETAIL = 7;
const int AT_OPTIONS_FXAA_VALUE = 8;
const int AT_OPTIONS_PIP_VALUE = 9;
const int AT_OPTIONS_SW_VALUE = 10;
const int AT_POSTPROCESS_EFFECTS = 11;
const int AT_QUALITY_PREFERENCE = 12;
const int AT_ATOC_DETAIL = 13;
const int AT_AMBIENT_OCCLUSION = 14;
const int AT_BLOOM = 15;
const int AT_ROTATION_BLUR = 16;
const int AT_SHADING_DETAIL = 17;
const int AT_SHADOW_DETAIL = 18;
const int AT_OPTIONS_TERRAIN = 19;
const int AT_OPTIONS_RESOLUTION = 20;
const int AT_OPTIONS_SLIDER_FILLRATE = 21;
const int AT_OPTIONS_REFRESH = 22;
const int AT_OPTIONS_GAMMA_SLIDER = 23;
const int AT_OPTIONS_BRIGHT_SLIDER = 24;
const int AT_OPTIONS_VISIBILITY_SLIDER = 25;
const int AT_OPTIONS_OBJECT_VISIBILITY_SLIDER = 26;
const int AT_OPTIONS_TRAFFIC_VISIBILITY_SLIDER = 27;
const int AT_OPTIONS_SHADOW_VISIBILITY_SLIDER = 28;
const int AT_OPTIONS_DRAWDISTANCE_SLIDER = 29;
const int AT_OPTIONS_BLOOD = 30;
const int AT_OPTIONS_IMPERIALUNITS = 31;
const int AT_OPTIONS_VEHICLEFREELOOK = 32;
const int AT_OPTIONS_WBUFFER = 33;
const int AT_ASPECT_RATIO = 34;
const int AT_OPTIONS_IGUISIZE = 35;
const int AT_CONFIG_YREVERSED = 36;
const int AT_OPTIONS_PERSPECTIVE = 37;
const int AT_OPTIONS_FIELD_OF_VIEW = 38;
const int AT_OPTIONS_MUSIC_SLIDER = 39;
const int AT_OPTIONS_EFFECTS_SLIDER = 40;
const int AT_OPTIONS_VON_SLIDER = 41;
const int AT_OPTIONS_MASTER_VOLUME = 42;
const int AT_OPTIONS_VOICES_SLIDER = 43;
const int AT_OPTIONS_MIC_SENS_SLIDER = 44;
const int AT_OPTIONS_SAMPLES_SLIDER = 45;
const int AT_OPTIONS_HWACC = 46;
const int AT_OPTIONS_EAX = 47;
const int AT_OPTIONS_SINGLE_VOICE = 48;
const int AT_OPTIONS_LANGUAGE = 49;
const int AT_OPTIONS_SUBTITLES = 50;
const int AT_OPTIONS_RADIO = 51;
const int AT_CONFIG_XAXIS = 52;
const int AT_CONFIG_YAXIS = 53;
const int AT_CONFIG_FLOATING_ZONE = 54;
const int AT_CONFIG_MOUSE_FILTERING = 55;
const int AT_CONFIG_HEAD_BOB = 56;

// Option Access Control Type
const int OA_CT_NUMERIC = 0;
const int OA_CT_SWITCH = 1;
const int OA_CT_LIST = 2;

// Option Field of view constants
const float OPTIONS_FIELD_OF_VIEW_MIN = 0.75242724772f;
const float OPTIONS_FIELD_OF_VIEW_MAX = 1.30322025726f;
*/

class OptionsAccess
{
	//proto private void ~OptionsAccess();
	//proto private void OptionsAccess();

	proto native  int		GetAccessType();
	proto native  int		GetControlType();
	proto native  void	Apply();
	proto native  void	Test();
	proto native  void	Revert();
	proto native  int		IsChanged();
	proto native  int		NeedRestart();
	proto native  int		SetChangeImmediately();
};

// -------------------------------------------------------------------------
class NumericOptionsAccess extends OptionsAccess
{
	proto native  float	ReadValue();
	proto native  void	WriteValue(float value);
	proto native	float	GetMin();
	proto native	float	GetMax();
};

// -------------------------------------------------------------------------
class ListOptionsAccess extends OptionsAccess
{
	proto native int		GetIndex();
	proto native void		SetIndex(int index);
	proto native int		GetItemsCount();
	proto	void					GetItemText(int index, out string value);
};

// -------------------------------------------------------------------------
class SwitchOptionsAccess extends OptionsAccess
{
	proto native void		Switch();
	proto	void					GetItemText(out string value);
};

// -------------------------------------------------------------------------
class GameOptions
{
	proto native void	Apply();
	proto native void	Revert();
	proto native void	Test();
	proto native OptionsAccess GetOption(int index);
	proto native int GetOptionsCount();
	proto native int NeedRestart();
};

typedef Ref<Object> ORefT;
typedef Link<Object> OLinkT;

// #include "Scripts/Classes/Component/_include.c"