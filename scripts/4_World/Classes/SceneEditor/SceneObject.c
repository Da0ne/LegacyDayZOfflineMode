class SceneObject
{
	static const int	COLOR_OBJ_BBOX_NORMAL	= 0x00000000;
	static const int	COLOR_OBJ_BBOX_SELECT	= 0x1f007C00;
	
	protected autoptr Link<EntityAI>			m_ObjectPtr;
	protected Shape								m_DebugShapeBBox;
	protected string							m_InitScript;
	
	protected autoptr array<SceneObject>		m_LinkedSceneObjects;
	protected autoptr map<SceneObject, Shape>	m_LinkedSceneObjectsShapes;
	
	autoptr array<int>							m_LinkedSceneObjectsIndices;
	
	//========================================
	// SceneObject
	//========================================
	SceneObject Init(string obj_name, vector pos)
	{
		if ( obj_name != STRING_EMPTY )
		{
			bool is_ai = GetGame().IsKindOf( obj_name, "DZ_LightAI");
			
			PluginDeveloper 	module_dev = GetPlugin(PluginDeveloper);
			EntityAI e = module_dev.SpawnItem(PluginSceneManager.PLAYER, obj_name, SPAWNTYPE_GROUND, 0, 0, 0, true, pos);
			
			if ( e != NULL )
			{
				if ( e.IsInherited(ItemBase) ) 
				{		
					ItemBase item = (ItemBase)e;
					item.SetQuantity(item.GetQuantityMax());
				}
				
				LinkEntityAI(e);
			}
			else if ( obj_name != "player" )
			{
				return NULL;
			}
		}
		
		m_LinkedSceneObjects = new array<SceneObject>;
		m_LinkedSceneObjectsShapes = new map<SceneObject, Shape>;
		m_LinkedSceneObjectsIndices = new array<int>;
	
		return this;
	}
	
	//----------------------------------------
	// ~EditorShapeDeselect
	//----------------------------------------
	void ~EditorShapeDeselect()
	{
		EditorShapeDeselect();
	}
	
	//----------------------------------------
	// GetObject
	//----------------------------------------
	EntityAI GetObject()
	{
		if (!m_ObjectPtr) return NULL;
		return m_ObjectPtr.Ptr();
	}
	
	//----------------------------------------
	// IsPlayer
	//----------------------------------------
	bool IsPlayer()
	{
		return false;
	}
	
	//----------------------------------------
	// GetInitScript
	//----------------------------------------
	string GetInitScript()
	{
		return m_InitScript;
	}
	
	//----------------------------------------
	// SetInitScript
	//----------------------------------------
	void SetInitScript( string init_script )
	{
		m_InitScript = init_script;
	}
	
	//========================================
	// EditorShapeUpdatePos
	//========================================
	void EditorShapeUpdatePos()
	{
		if ( m_DebugShapeBBox != NULL )
		{		
			vector mat[4];
			GetObject().GetTransform(mat);
		
			if ( m_DebugShapeBBox != NULL )
			{
				m_DebugShapeBBox.SetMatrix(mat);
			}
		}
	}
	
	//========================================
	// EditorShapeSetColor
	//========================================
	void EditorShapeSetColor(int color)
	{
		if ( m_DebugShapeBBox )
		{
			m_DebugShapeBBox.SetColor(color);
		}
	}
	
	//========================================
	// EditorShapeSelect
	//========================================
	void EditorShapeSelect()
	{
		EditorShapeSetColor(COLOR_OBJ_BBOX_SELECT);
	}
	
	//========================================
	// EditorShapeDeselect
	//========================================
	void EditorShapeDeselect()
	{
		EditorShapeSetColor(COLOR_OBJ_BBOX_NORMAL);
	}
	
	//========================================
	// GetSize
	//========================================
	vector GetSize()
	{
		vector size = Vector(0,0,0);
		vector min_max[2];
			
		GetObject().GetCollisionBox(min_max);
			
		size[0] = min_max[1][0] - min_max[0][0];
		size[2] = min_max[1][2] - min_max[0][2];
		size[1] = min_max[1][1] - min_max[0][1];
		
		return size;
	}
	
	//========================================
	// EditorShapeAdd
	//========================================
	void EditorShapeAdd()
	{	
		if ( m_DebugShapeBBox != NULL )
		{
			return;
		}
		
		vector min		= "0 0 0";
		vector max		= "0 0 0";
	
		vector size = GetSize();
		
		float width		= size[0];
		float height	= size[1];
		float length	= size[2];		
		
		float width_h = width*0.5;
		float lenght_h = length*0.5;
		
		min[0] = -width_h;
		min[1] = 0;
		min[2] = -lenght_h;
		
		max[0] = width_h;
		max[1] = height;
		max[2] = lenght_h;
		
		//Log("EditorShapeAdd -> "+m_ObjectPtr.Ptr().GetType());
		
		m_DebugShapeBBox = Debug.DrawBox(min, max);
		EditorShapeUpdatePos();
		EditorShapeDeselect();
	}
	
	//========================================
	// EditorShapeRemove
	//========================================
	void EditorShapeRemove()
	{
		if ( m_DebugShapeBBox != NULL )
		{
			m_DebugShapeBBox.Destroy();
			m_DebugShapeBBox = NULL;
		}
	}
	
	//========================================
	// EditorLineRemove
	//========================================
	void EditorLineRemove(SceneObject obj)
	{
		for ( int i = 0; i < m_LinkedSceneObjectsShapes.Count(); i++ )
		{
			if ( m_LinkedSceneObjectsShapes.GetKey(i) == obj )
			{
				m_LinkedSceneObjectsShapes.GetElement(i).Destroy();
				m_LinkedSceneObjectsShapes.Remove( obj );
				break;
			}
		}
	}
	
	//========================================
	// EditorLineAdd
	//========================================
	void EditorLineAdd(SceneObject obj)
	{
		if ( obj.GetObject() != NULL && GetObject() != NULL )
		{
			if ( m_LinkedSceneObjectsShapes.Contains(obj) )
			{
				EditorLineRemove(obj);
			}
			
			vector pos1 = obj.GetSize();
			pos1[0] = 0; pos1[1] = pos1[1] / 2; pos1[2] = 0;
			pos1 = pos1 + obj.GetObject().GetPosition();
			
			vector pos2 = GetSize();
			pos2[0] = 0; pos2[1] = pos2[1] / 2; pos2[2] = 0;
			pos2 = pos2 + GetObject().GetPosition();
			
			m_LinkedSceneObjectsShapes.Insert( obj, Debug.DrawArrow( pos1, pos2 ) );
		}
	}
	
	//========================================
	// LinkEntityAI
	//========================================
	void LinkEntityAI(EntityAI e)
	{
		delete m_ObjectPtr;
		m_ObjectPtr = new Link<EntityAI>(e);
	}
	
	//========================================
	// IsLinkedWithSceneObject
	//========================================
	bool IsLinkedWithSceneObject(SceneObject scene_object)
	{
		int index = m_LinkedSceneObjects.Find(scene_object);
		if ( index >= 0 )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	//========================================
	// LinkEntityAI
	//========================================
	void LinkSceneObject(SceneObject scene_object, bool draw_line = true)
	{
		if ( !IsLinkedWithSceneObject(scene_object) )
		{
			if ( draw_line )
			{
				EditorLineAdd(scene_object);
			}
			m_LinkedSceneObjects.Insert(scene_object);
		}
	}
	
	//========================================
	// UnlinkSceneObject
	//========================================
	void UnlinkSceneObject(SceneObject scene_object)
	{
		int index = m_LinkedSceneObjects.Find(scene_object);
		if ( index >= 0 && index < m_LinkedSceneObjects.Count() )
		{
			EditorLineRemove(scene_object);
			m_LinkedSceneObjects.Remove(index);
		}
	}
	
	//========================================
	// UnlinkAll
	//========================================
	void UnlinkAll()
	{
		int link_count = GetLinkedSceneObjectsCount();
			
		if ( link_count > 0 )
		{
			for ( int i = 0; i < link_count; ++i )
			{
				PluginSceneManager.GetInstance().UnlinkSceneObjects(this, GetLinkedSceneObject(0));
			}			
		}
	}
		
	//========================================
	// GetLinkedSceneObjects
	//========================================
	array<SceneObject> GetLinkedSceneObjects()
	{
		return m_LinkedSceneObjects;
	}
	
	//========================================
	// GetLinkedSceneObjectsCount
	//========================================
	int GetLinkedSceneObjectsCount()
	{
		return m_LinkedSceneObjects.Count();
	}
	
	//========================================
	// GetLinkedSceneObject
	//========================================
	SceneObject GetLinkedSceneObject( int i )
	{
		return m_LinkedSceneObjects.Get(i);
	}
	
	//========================================
	// GetLinkedObject
	//========================================
	EntityAI GetLinkedObject( int i )
	{
		return GetLinkedSceneObject(i).GetObject();
	}
	
	//========================================
	// Destructor
	//========================================
	SceneObject ~SceneObject()
	{
		if ( m_ObjectPtr && m_ObjectPtr.Ptr() != GetGame().GetPlayer() )
		{
			EntityAI obj = m_ObjectPtr.Ptr();
			delete m_ObjectPtr;
			GetGame().ObjectDelete( obj );
		}
		
		for ( int i = 0; i < m_LinkedSceneObjects.Count(); i++ )
		{
			EditorLineRemove( m_LinkedSceneObjects.Get(i) );
		}
		
		EditorShapeRemove();
	}
	
	//========================================
	// GetTypeName
	//========================================
	string GetTypeName()
	{
		return m_ObjectPtr.Ptr().GetType();
	}
	
	//========================================
	// PlaceOnSurface
	//========================================
	void PlaceOnSurface()
	{
		if (m_ObjectPtr)
		{
			if ( GetGame().IsClient() && GetGame().IsMultiplayer() )
			{
				Param par = new Param3<string, EntityAI, Param>( "PlaceOnSurface" , m_ObjectPtr.Ptr(), NULL );
				SceneObjectSynch( par );			
			}
			else
			{
				m_ObjectPtr.Ptr().PlaceOnSurface();
			}
		}
	}
	
	//========================================
	// SetPosition
	//========================================
	void SetPosition(vector pos)
	{
		if (m_ObjectPtr) 
		{
			if ( GetGame().IsClient() && GetGame().IsMultiplayer() )
			{
				Param par = new Param3<string, EntityAI, Param>( "SetPosition" , m_ObjectPtr.Ptr(), new Param1<vector>( pos ) );
				SceneObjectSynch( par );			
			}
			else
			{
				m_ObjectPtr.Ptr().SetPosition(pos);
			}
		}
		PlaceOnSurface();
		EditorShapeUpdatePos();
	}
	
	//========================================
	// GetPosition
	//========================================
	vector GetPosition()
	{
		if (m_ObjectPtr) return m_ObjectPtr.Ptr().GetPosition();
		return Vector(0,0,0);
	}
	
	//========================================
	// GetDamage
	//========================================
	float GetDamage()
	{
		if (m_ObjectPtr) return m_ObjectPtr.Ptr().GetDamage();
		return 0;
	}
	
	//========================================
	// SetDamage
	//========================================
	void SetDamage(float value)
	{
		if (m_ObjectPtr)
		{
			if ( GetGame().IsClient() && GetGame().IsMultiplayer() )
			{
				Param par = new Param3<string, EntityAI, Param>( "SetDamage" , m_ObjectPtr.Ptr(), new Param1<float>( value ) );
				SceneObjectSynch( par );			
			}
			else
			{
				m_ObjectPtr.Ptr().SetDamage(value);
			}
		}
	}
		
	//========================================
	// GetPositionAsString
	//========================================
	string GetPositionAsString()
	{
		if (m_ObjectPtr) return m_ObjectPtr.Ptr().GetPosition().ToString(false);
		return Vector(0,0,0).ToString(false);
	}
	
	//========================================
	// SetPositionAsString
	//========================================
	void SetPositionAsString(string string_pos)
	{
		SetPosition(string_pos.ToVector());
	}
	
	//========================================
	// GetRotation
	//========================================
	float GetRotation()
	{
		if (m_ObjectPtr)
		{
			vector v = m_ObjectPtr.Ptr().GetOrientation();
			return v[0];
		}

		return 0;
	}
	
	//========================================
	// SetRotation
	//========================================
	void SetRotation(float rot)
	{
		if (m_ObjectPtr)
		{
			vector v = m_ObjectPtr.Ptr().GetOrientation();
			v[0] = rot;
			m_ObjectPtr.Ptr().SetOrientation(v);
			EditorShapeUpdatePos();
		}
	}
	
	void SceneObjectSynch( Param p )
	{
		GetGame().RPCSingleParam( GetGame().GetPlayer(), RPC_SYNC_SCENE_OBJECT, p, NULL );
	}
	
	//========================================
	// AddRotation
	//========================================
	void AddRotation( float add_rot )
	{
		if ( m_ObjectPtr )
		{
			if ( GetGame().IsClient() && GetGame().IsMultiplayer() )
			{
				Param par = new Param3<string, EntityAI, Param>( "AddRotation" , m_ObjectPtr.Ptr(), new Param1<float>( add_rot ) );
				SceneObjectSynch( par );			
			}
			else
			{
				vector v = m_ObjectPtr.Ptr().GetOrientation();
				v[0] = v[0] + add_rot;
				m_ObjectPtr.Ptr().SetOrientation( v );
			
				EditorShapeUpdatePos();
			}
		}		
	}
	
	//========================================
	// AddRotation
	//========================================
	void AddAttachment(string att_name)
	{
		GetObject().CreateAttachment(att_name);
	}
	
	//========================================
	// CanAttachment
	//========================================
	bool CanAttachment(EntityAI e)
	{
		return GetObject().CanAddAttachment(e);
	}
	
	//========================================
	// AddRotation
	//========================================
	void RemoveAttachment(EntityAI e)
	{
		GetObject().RemoveAttachment(e);
	}
	
	//========================================
	// GetAttachments
	//========================================
	array<EntityAI> GetAttachments()
	{
		array<EntityAI> ret = new array<EntityAI>;
		
		for ( int i = 0; i < GetObject().AttachmentsCount(); ++i )
		{
			ret.Insert(GetObject().GetAttachmentFromIndex(i));
		}
		
		return ret;
	}
	
	//========================================
	// GetConfigAttachments
	//========================================
	TStringArray GetConfigAttachments()
	{
		string			type_name = GetTypeName();
		TStringArray	cfg_attachments = new TStringArray;
		
		string cfg_path;
		
		if ( GetGame().ConfigIsExisting(CFG_VEHICLESPATH+" "+type_name) )
		{
			cfg_path = CFG_VEHICLESPATH+" "+type_name+" attachments";
		}
		else if ( GetGame().ConfigIsExisting(CFG_WEAPONSPATH+" "+type_name) )
		{
			cfg_path = CFG_WEAPONSPATH+" "+type_name+" attachments";
		}
		else if ( GetGame().ConfigIsExisting(CFG_MAGAZINESPATH+" "+type_name) )
		{
			cfg_path = CFG_MAGAZINESPATH+" "+type_name+" attachments";
		}
		
		GetGame().ConfigGetTextArray(cfg_path, cfg_attachments);
		
		return cfg_attachments;
	}
}