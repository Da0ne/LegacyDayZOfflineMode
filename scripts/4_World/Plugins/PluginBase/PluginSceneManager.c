class PluginSceneManager extends PluginBase
{
	static string		SCENE_DEFAULT_NAME		= "default";
	static string		SCENE_SUFIX				= "scene";
	static PlayerBase	PLAYER;
	static PluginSceneManager instance;
	
	bool			m_RulerActivated;
	autoptr array<vector>	m_RulerPoints;
	autoptr array<Shape>	m_RulerLines;
	autoptr array<Shape>	m_RulerSpheres;
	
	static const int SCENE_EDITOR_CMD_REFRESH	= 0;
	static const int SCENE_EDITOR_CMD_POPUP		= 1;
	static const int SCENE_EDITOR_CMD_SAVE		= 2;
	
	//---------------------------------------------------------------------------------
	// >> Public scope

	// System Events
	void PluginSceneManager()
	{
		if ( instance == NULL )
		{
			instance = this;
		}
	}
	
	static PluginSceneManager GetInstance()
	{
		return GetPlugin( PluginSceneManager );
	}

	//==========================================
	// OnInit (System Event)
	//==========================================
	void OnInit()
	{
		m_ModuleDeveloper			= GetPlugin(PluginDeveloper);
		m_ModuleConfigScene			= GetPlugin(PluginConfigScene);
		m_ModuleConfigDebugProfile	= GetPlugin(PluginConfigDebugProfile);
		
		//Ruler
		m_RulerActivated = false;
		m_RulerPoints	= new array<vector>;
		m_RulerLines	= new array<Shape>;
		m_RulerSpheres	= new array<Shape>;
	}

	//==========================================
	// OnDestroy (destructor)
	//==========================================
	void OnDestroy()
	{
		SceneUnload();
	}
	
	//==========================================
	// OnMouseWheel
	//==========================================
	void OnMouseWheel(int wheel)
	{
		if ( !IsMouseInScene() )
		{
			return;
		}
		
		if ( GetSelectedSceneObject() )
		{
			GetSelectedSceneObject().AddRotation(GetRotationAngle() * wheel);
		}
		
		EditorUpdate();
	}

	//==========================================
	// OnUIEditorOpened
	//==========================================
	void OnUIEditorOpened()
	{	
		m_IsOpen = true;

		DeveloperFreeCamera.EnableFreeCameraSceneEditor(GetGame().GetPlayer());
		
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(this, "FreeCameraControlDisable", NULL);
		
		EditorUpdate();
	}

	//==========================================
	// OnUIEditorClosed
	//==========================================
	void OnUIEditorClosed()
	{
		m_IsOpen = false;
		
		FreeCameraControlEnable();
		
		m_ModuleDeveloper.ToggleFreeCameraBackPos();
		
		EditorUpdate();
	}
	
	//::::::::::::::::::::::::::::::::::::::::::
	// Event_OnClick
	//::::::::::::::::::::::::::::::::::::::::::
	Object Event_OnClick()
	{	
		if ( !IsMouseInScene() )
		{
			return NULL;
		}

		if (!m_SceneData)
			return NULL;
			
		// Direction Vector from current active Camera
		vector dir = GetGame().GetPointerDirection();
		
		PlayerBase pb = GetGame().GetPlayer();

		if (!pb.m_Cam)
			return NULL;

		// Raycast from
		vector from = pb.m_Cam.GetPosition(); 
		// Raycast to
		vector to = from + ( dir * 10000 );
		// Raycast out parameter
		vector contact_pos;
		vector contact_dir;
		int contact_component;
		set<Object> objects = new set<Object>;
		bool selected = false;
		
		if ( GetGame().Raycast(from, to, contact_pos, contact_dir, contact_component, objects) )   
		{
			for ( int i = 0; i < objects.Count(); ++i )
			{
				Object obj = objects.Get(i);
Print("Raycast obj: "+ obj.GetType());				
				if ( obj.IsInherited( EntityAI ) )
				{
					SceneObject sc_object = m_SceneData.GetSceneObjectByEntityAI(obj);
					
					if ( sc_object == NULL )
					{
						sc_object = m_SceneData.AddObject(obj);
						return obj;
					}
					
					SelectObject( sc_object );
					selected = true;
					
					break;
				}
			}
		}
		
		if ( !selected )
		{
			DeselectObject();
			
			//	Ruler
			if ( IsRulerActivated() && GetSelectedSceneObject() == NULL )
			{
				RulerAddPoint();
			}
		}
		
		EditorUpdate();
		if (m_SelectedObject && m_SelectedObject.GetObject())
			return m_SelectedObject.GetObject();

		return NULL;
	}

	//::::::::::::::::::::::::::::::::::::::::::
	// Event_OnClickMiddle
	//::::::::::::::::::::::::::::::::::::::::::
	void Event_OnClickMiddle()
	{
		if ( !IsMouseInScene() )
		{
			return;
		}

		if (!m_SceneData)
			return;
		
		if ( GetSelectedSceneObject() != NULL )
		{
			// Direction Vector from current active Camera
			vector dir = GetGame().GetPointerDirection();
			
			PlayerBase pb = GetGame().GetPlayer();

			if (!pb.m_Cam)
				return;

			// Raycast from
			vector from = pb.m_Cam.GetPosition(); 
			// Raycast to
			vector to = from + ( dir * 10000 );   
			// Raycast out parameter
			vector contact_pos;
			vector contact_dir;
			int contact_component;
			set<Object> objects = new set<Object>;
			bool selected = false;
			
			if ( GetGame().Raycast(from, to, contact_pos, contact_dir, contact_component, objects) )   
			{
				for ( int i = 0; i < objects.Count(); ++i )
				{
					Object obj = objects.Get(i);
					
					if ( obj.IsInherited( EntityAI ) )
					{
						EntityAI eai = obj;
						SceneObject scene_object = m_SceneData.GetSceneObjectByEntityAI(eai);
						
						if ( scene_object )
						{
							if ( !scene_object.IsPlayer() )
							{
								if ( scene_object != NULL && scene_object != GetSelectedSceneObject() )
								{
									if ( !AreSceneObjectsLinked(GetSelectedSceneObject(), scene_object) ) 
									{
										LinkSceneObjects( GetSelectedSceneObject(), scene_object );
									}
									else
									{
										UnlinkSceneObjects( GetSelectedSceneObject(), scene_object );
									}
								}
							}
							else
							{
								SceneEditorMenuCmdPopup("I am sorry. You cant create link on player!");
							}
						}
						else
						{
							SceneEditorMenuCmdPopup("I am sorry. Selected object is not Scene Object!");
						}	
					}
				}
			}
		}
		
		EditorUpdate();
	}

	//::::::::::::::::::::::::::::::::::::::::::
	// Event_OnDrag
	//::::::::::::::::::::::::::::::::::::::::::
	void Event_OnDrag()
	{
		if (!m_SceneData)
			return;

		if ( !IsMouseInScene() )
		{
			return;
		}
		
		if ( GetSelectedSceneObject() != NULL )
		{
			// Direction Vector from current active Camera
			vector dir = GetGame().GetPointerDirection();
			
			PlayerBase pb = GetGame().GetPlayer();
			if (!pb.m_Cam)
				return;

			// Raycast from
			vector from = pb.m_Cam.GetPosition(); 
			// Raycast to
			vector to = from + ( dir * 10000 );   
			// Raycast out parameter
			vector contact_pos;
			vector contact_dir;
			int contact_component;
			
			//Raycast(vector begPos, vector endPos, out vector contactPos, out vector contactDir, out int contactComponent, out set<Object> results = NULL, Object with = NULL, Object ignore = NULL,  bool sorted = false,  bool ground_only = false,  int iType = ObjIntersectView, float radius = 0.0);
			
			if ( GetGame().Raycast(from, to, contact_pos, contact_dir, contact_component, NULL, NULL, NULL, false, false) )   
			{
				GetSelectedSceneObject().SetPosition(contact_pos);
			}
			
			EditorUpdate();
		}
	}

	//::::::::::::::::::::::::::::::::::::::::::
	// Event_OnDoubleClick
	//::::::::::::::::::::::::::::::::::::::::::
	SceneObject Event_OnDoubleClick()
	{
		if (!m_SceneData)
			return NULL;

		if ( !IsMouseInScene() )
		{
			return NULL;
		}
		
		if ( m_ClassBrowserSelected == "" )
		{
			return NULL;
		}	
		
		// Direction Vector from current active Camera
		vector dir = GetGame().GetPointerDirection();
		
		PlayerBase pb = GetGame().GetPlayer();

		if (!pb.m_Cam)
				return NULL;

		// Raycast from
		vector from = pb.m_Cam.GetPosition(); 
		// Raycast to
		vector to = from + ( dir * 10000 );   
		// Raycast out parameter
		vector contact_pos;
		vector contact_dir;
		int contact_component;
		
		if ( GetGame().Raycast(from, to, contact_pos, contact_dir, contact_component) )   
		{
			SceneObject obj = m_SceneData.CreateSceneObject(m_ClassBrowserSelected, contact_pos);
			
			if ( obj != NULL )
			{
				SelectObject(obj);
				return obj;
			}			
		}
		
		return NULL;
	}
	
	//==========================================
	// Load
	//==========================================
	void InitLoad()
	{		
		Print("PluginSceneManager :: InitLoad()");
		PLAYER = GetGame().GetPlayer();
		
		SceneLoad( m_ModuleConfigDebugProfile.GetSceneEditorName() );
		
		RestoreSceneObjectsLinks();
		
		ExecuteInitScripts();
	}

	//==========================================
	// RestoreSceneObjectsLinks
	//==========================================
	void RestoreSceneObjectsLinks()
	{
		array<SceneObject> scene_objects = GetSceneObjects();
			
		for ( int i = 0; i < scene_objects.Count(); i++ )
		{
			SceneObject scene_object = scene_objects.Get(i);
			
			array<int> linked_scene_objects_indices = scene_object.m_LinkedSceneObjectsIndices;
			
			for ( int j = 0; j < linked_scene_objects_indices.Count(); j++ )
			{
				int index = linked_scene_objects_indices.Get(j);
				if ( index >= 0 && index < scene_objects.Count() )
				{
					scene_object.LinkSceneObject( scene_objects.Get(index), false );
					
//PrintString("Load link: "+ ToString(scene_object.GetObject()) +" <-> "+ ToString(scene_objects.Get(index).GetObject()) );
				}
			}
			
			delete scene_object.m_LinkedSceneObjectsIndices;
			scene_object.m_LinkedSceneObjectsIndices = NULL;
		}
	}

	//==========================================
	// ExecuteInitScripts
	//==========================================
	void ExecuteInitScripts()
	{
		array<SceneObject> scene_objects = GetSceneObjects();	
		for ( int i = 0; i < scene_objects.Count(); i++ )
		{
			SceneObject scene_object = scene_objects.Get(i);
			ExecuteEnforceScript( i, scene_object, scene_object.GetInitScript() );
		}
	}
	
	//==========================================
	// EditorToggle
	//==========================================
	void EditorToggle()
	{	
		//Log("EditorToggle");

		if ( GetGame().IsMultiplayer() )
		{
			return;
		}

		if ( IsOpened() )
		{
			// Close UI Scene Editor Menu
			GetGame().GetUIManager().Back();
		}
		else
		{
			// Open UI Scene Editor Menu
			GetGame().GetUIManager().EnterScriptedMenu(MENU_SCENE_EDITOR, NULL);
			EditorUpdate();
		}
	}
	
	//==========================================
	// IsOpened
	//==========================================
	bool IsOpened()
	{
		return m_IsOpen;
	}

	//==========================================
	// SelectClassName
	//==========================================
	bool SelectClassName(string class_name)
	{
		m_ClassBrowserSelected = class_name;
	}

	//==========================================
	// FreeCameraControlEnable
	//==========================================
	void FreeCameraControlEnable()
	{
		// Enable Camera control
		GetGame().GetInput().ChangeGameFocus(-1);
		
		// Disable Cursor
		GetGame().GetUIManager().ShowCursor(false);

		PlayerBase pb = GetGame().GetPlayer();
		pb.m_FreezCam = false;
	}

	//==========================================
	// FreeCameraControlDisable
	//==========================================
	void FreeCameraControlDisable()
	{
		// Disable Camera control
		GetGame().GetInput().ChangeGameFocus(1);
		
		// Enable Cursor
		GetGame().GetUIManager().ShowCursor(true);

		PlayerBase pb = GetGame().GetPlayer();
		pb.m_FreezCam = true;
	}
	
	//==========================================
	// DeleteSelectedObject
	//==========================================
	void DeleteSelectedObject()
	{
		SceneObject selected_obj = GetSelectedSceneObject();
		
		if ( selected_obj != NULL )
		{
			if ( !selected_obj.IsPlayer() )
			{
				array<SceneObject> scene_objects = GetSceneObjects();
				for ( int i = 0; i < scene_objects.Count(); i++ )
				{
					if ( AreSceneObjectsLinked(scene_objects.Get(i), selected_obj) )
					{
						UnlinkSceneObjects(scene_objects.Get(i), selected_obj);
					}
				}
				
				m_SceneData.DeleteSceneObject(selected_obj);
				
				scene_objects.RemoveItem(selected_obj);
				
				DeselectObject();
			}
			else
			{
				SceneEditorMenuCmdPopup("You cant delete player!");
			}
		}
		
		EditorUpdate();
	}
	
	//==========================================
	// SelectObject
	//==========================================
	void SelectObject(SceneObject obj)
	{
		if ( GetSelectedSceneObject() != NULL && GetSelectedSceneObject() == obj )
		{
			return;
		}
		
		DeselectObject();

		m_SelectedObject = obj;
		
		EditorUpdate();
	}
	
	//==========================================
	// GetSelectedSceneObject
	//==========================================
	SceneObject GetSelectedSceneObject()
	{
		return m_SelectedObject;
	}

	//==========================================
	// SelectedObjectDuplicate
	//==========================================
	void SelectedObjectDuplicate()
	{
		SceneObject selected_obj = GetSelectedSceneObject();
		
		if ( selected_obj )
		{
			if ( selected_obj.GetObject() == PLAYER )
			{
				return;
			}
			
			vector size = GetSelectedSceneObject().GetSize();
			
			vector new_pos = selected_obj.GetPosition() + Vector(1, 0, 1) + size;
			
			SceneObject obj = m_SceneData.CreateSceneObject(selected_obj.GetTypeName(), new_pos );
			obj.SetRotation(selected_obj.GetRotation());
			
			array<EntityAI> attachments = selected_obj.GetAttachments();
			for ( int i = 0; i < attachments.Count(); i++ )
			{
				obj.AddAttachment( attachments.Get(i).GetType() );
			}
			delete attachments;
			
			SelectObject(obj);
		}
	}

	//==========================================
	// SelectedObjectSetRot
	//==========================================
	void SelectedObjectSetRot(float dir)
	{
		if ( GetSelectedSceneObject() )
		{
			GetSelectedSceneObject().SetRotation(dir);
			
			EditorUpdate();
		}
	}

	//==========================================
	// SelectedObjectSetPosX
	//==========================================
	void SelectedObjectSetPosX(float value)
	{
		SceneObject selected_obj = GetSelectedSceneObject();
		
		if ( selected_obj )
		{
			vector v = selected_obj.GetPosition();
			v[0] = value;
			selected_obj.SetPosition(v);
			
			EditorUpdate();
		}
	}

	//==========================================
	// SelectedObjectSetPosY
	//==========================================
	void SelectedObjectSetPosY(float value)
	{
		SceneObject selected_obj = GetSelectedSceneObject();
		
		if ( selected_obj )
		{
			vector v = selected_obj.GetPosition();
			v[1] = value;
			selected_obj.SetPosition(v);
			
			EditorUpdate();
		}
	}

	//==========================================
	// SelectedObjectSetPosZ
	//==========================================
	void SelectedObjectSetPosZ(float value)
	{
		SceneObject selected_obj = GetSelectedSceneObject();
		
		if ( GetSelectedSceneObject() )
		{
			vector v = selected_obj.GetPosition();
			v[2] = value;
			selected_obj.SetPosition(v);
			
			EditorUpdate();
		}
	}

	//==========================================
	// SelectedObjectSetDamage
	//==========================================
	void SelectedObjectSetDamage(float value)
	{
		if ( GetSelectedSceneObject() )
		{
			GetSelectedSceneObject().SetDamage(Math.Clamp ( value, 0, 1 ));
			
			EditorUpdate();
		}
	}

	//==========================================
	// SelectedObjectSetDamage
	//==========================================
	void SelectedObjectFocus()
	{
		SceneObject selected_obj = GetSelectedSceneObject();
		
		PlayerBase pb = GetGame().GetPlayer();
		if (!pb.m_Cam)
				return;

		if ( selected_obj )
		{
			Camera		cam = pb.m_Cam;
			vector		cam_pos = cam.GetPosition();
			vector		obj_pos = selected_obj.GetPosition();
			vector		view_target = obj_pos;
			
			vector obj_size = selected_obj.GetSize();
			
			cam_pos = obj_pos;
			
			cam_pos[0] = cam_pos[0] + ( obj_size[0] * 2 );
			cam_pos[1] = cam_pos[1] + obj_size[1] + 0.5;
			cam_pos[2] = cam_pos[2] + ( obj_size[2] * 2 );		
			
			view_target[1] = view_target[1] + (obj_size[1] * 0.5);
			
			vector cam_dir = view_target - cam_pos;
			cam_dir.Normalize();
			
			cam.SetPosition(cam_pos);
			cam.SetDirection(cam_dir);
			
			//vector debug_pos = cam_pos + (cam_dir * 50);
			//Debug.DrawLine(cam_pos, debug_pos);
			//Debug.DrawSphere(cam_pos);
		}
	}

	//==========================================
	// SelectedObjectAddAttachment
	//==========================================
	void SelectedObjectAddAttachment(string att_name)
	{
		if ( GetSelectedSceneObject() )
		{
			GetSelectedSceneObject().AddAttachment(att_name);
		}
		
		EditorUpdate();
	}

	//==========================================
	// SelectedObjectRemoveAttachment
	//==========================================
	void SelectedObjectRemoveAttachment(EntityAI e)
	{
		if ( GetSelectedSceneObject() )
		{
			GetSelectedSceneObject().RemoveAttachment(e);
		}
		
		EditorUpdate();
	}

	//==========================================
	// LinkSceneObjects
	//==========================================
	void LinkSceneObjects(SceneObject scene_object1, SceneObject scene_object2)
	{
		scene_object1.LinkSceneObject(scene_object2);
		scene_object2.LinkSceneObject(scene_object1);
	}

	//==========================================
	// UnlinkSceneObjects
	//==========================================
	void UnlinkSceneObjects(SceneObject scene_object1, SceneObject scene_object2)
	{
		scene_object1.UnlinkSceneObject(scene_object2);
		scene_object2.UnlinkSceneObject(scene_object1);
	}

	//==========================================
	// AreSceneObjectsLinked
	//==========================================
	bool AreSceneObjectsLinked(SceneObject scene_object1, SceneObject scene_object2)
	{
		if ( scene_object1.IsLinkedWithSceneObject(scene_object2) && scene_object2.IsLinkedWithSceneObject(scene_object1) )
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	//==========================================
	// SetDrawSelection
	//==========================================
	void SetDrawSelection(bool draw)
	{
		m_ModuleConfigDebugProfile.SetSceneEditorDrawSelection(draw);
		
		EditorUpdate();
	}

	//==========================================
	// GetDrawSelection
	//==========================================
	bool GetDrawSelection()
	{
		return m_ModuleConfigDebugProfile.GetSceneEditorDrawSelection();
	}

	//==========================================
	// SetLoadPlayerPos
	//==========================================
	void SetLoadPlayerPos(bool save)
	{
		m_ModuleConfigDebugProfile.SetSceneEditorLoadPlayerPos(save);
	}

	//==========================================
	// SetLoadPlayerPos
	//==========================================
	bool GetLoadPlayerPos()
	{
		return m_ModuleConfigDebugProfile.GetSceneEditorLoadPlayerPos();
	}
	
	//==========================================
	// SetRotationAngle
	//==========================================
	void SetRotationAngle(int angle)
	{
		m_ModuleConfigDebugProfile.SetSceneEditorRotationAngle(angle);
	}

	//==========================================
	// GetRotationAngle
	//==========================================
	int	GetRotationAngle()
	{
		return m_ModuleConfigDebugProfile.GetSceneEditorRotationAngle();
	}

	//==========================================
	// SceneLoad
	//==========================================
	void SceneLoad(string scene_name)
	{
		SceneUnload();
		
Print("SceneManager - SceneLoad start...");
		
		m_ModuleConfigDebugProfile.SetSceneEditorName(scene_name);
		m_SceneData = m_ModuleConfigScene.SceneDataLoad(scene_name);

Print("SceneManager - SceneLoad finished...");
		if ( !(GetGame().IsMultiplayer() && GetGame().IsServer()) )
		{
			EditorUpdate();
Print("SceneManager - Editor update finished...");
		}
	}

	//==========================================
	// SceneUnload
	//==========================================
	void SceneUnload()
	{
		if ( m_SceneData )
		{
			delete m_SceneData;
			m_SceneData = NULL;
		}
		
		DeselectObject();
	}

	//==========================================
	// SceneSave
	//==========================================
	void SceneSave()
	{
		m_ModuleConfigScene.ClearParams();
		m_ModuleConfigScene.SceneDataSave(m_SceneData);
		SceneEditorMenuCmdSave();
		EditorUpdate();
	}

	//==========================================
	// SceneDelete
	//==========================================
	bool SceneCanDelete(string scene_name)
	{
		if ( scene_name == m_SceneData.GetNameScene() )
		{
			return false;
		}
		
		return true;
	}

	//==========================================
	// SceneDelete
	//==========================================
	bool SceneDelete(string scene_name)
	{
		if ( scene_name == m_SceneData.GetNameScene() )
		{
			return false;
		}
		
		m_ModuleConfigScene.SceneDelete(scene_name);
		
		EditorUpdate();
		
		return true;
	}	

	//==========================================
	// SceneDelete
	//==========================================
	void SceneDuplicate(string scene_name)
	{
		m_ModuleConfigScene.SceneDuplicate(scene_name);
		
		EditorUpdate();
	}

	//==========================================
	// SceneRename
	//==========================================
	void SceneRename(string scene_name, string new_name)
	{
		m_ModuleConfigScene.SceneRename(scene_name, new_name);
		
		if ( scene_name == m_SceneData.GetNameScene() )
		{
			m_SceneData.SetNameScene(new_name);
		}
		
		EditorUpdate();
	}

	//==========================================
	// SceneGetName
	//==========================================
	string SceneGetName()
	{
		if ( m_SceneData )
		{
			return m_SceneData.GetNameScene();
		}
		
		return STRING_EMPTY;
	}
	
	//==========================================
	// SetWeather
	//==========================================
	void SetWeather(float overcast, float rain, float fog, float wind_force)
	{
		m_SceneData.SetInitOvercast(overcast);
		m_SceneData.SetInitRain(rain);
		m_SceneData.SetInitFog(fog);
		m_SceneData.SetInitWindForce(wind_force);
	}

	//==========================================
	// SetDate
	//==========================================
	void SetDate( int year, int month, int day, int hour, int minute )
	{
		m_SceneData.SetInitYear(year);
		m_SceneData.SetInitMonth(month);
		m_SceneData.SetInitDay(day);
		m_SceneData.SetInitHour(hour);
		m_SceneData.SetInitMinute(minute);
	}

	//==========================================
	// SelectAndFocusPlayerObject
	//==========================================
	void SelectAndFocusPlayerObject()
	{
		array<SceneObject> scene_objects = GetSceneObjects();
		for ( int i = 0; i < scene_objects.Count(); i++ )
		{
			if ( scene_objects.Get(i).GetObject() == GetGame().GetPlayer() )
			{
				SelectObject( scene_objects.Get(i) );
				SelectedObjectFocus();
				break;
			}
		}
	}
		
	//------------------------------------------
	// ExecuteEnforceScript
	//------------------------------------------
	void ExecuteEnforceScript( int index, SceneObject scene_object, string script )
	{
		if ( script != "" )
		{
			string cls = scene_object.GetObject().ClassName();
			string wrapped_script = "void main() \n{\n PluginSceneManager se = GetPlugin(Type(\"PluginSceneManager\")); \n SceneObject this_obj = se.GetSceneObjectByIndex(" + index.ToString() + "); \n " + cls + " this_eai = this_obj.GetObject(); \n " + script + "\n}\n";
			
			GetGame().ExecuteEnforceScript(wrapped_script, "main");
		}
	}
	
	//==========================================
	// RulerToggle
	//==========================================
	void RulerToggle()
	{
		if ( IsRulerActivated() )
		{
			m_RulerActivated = false;
		}
		else
		{
			m_RulerActivated = true;
		}
		
		SceneEditorMenuCmdRefresh();
	}

	//==========================================
	// ActivateRuler
	//==========================================
	bool IsRulerActivated()
	{
		return m_RulerActivated;
	}

	//==========================================
	// RulerAddPoint
	//==========================================
	void RulerAddPoint()
	{
		vector point_pos = GetMousePositionInScene();
		
		m_RulerPoints.Insert( point_pos );
		
		EditorUpdateRuler();
	}

	//------------------------------------------
	// RulerClearVisuals
	//------------------------------------------
	void RulerClearVisuals()
	{
		for ( int i = 0; i < m_RulerLines.Count(); ++i )
		{
			m_RulerLines.Get(i).Destroy();
		}
		m_RulerLines.Clear();
		
		for ( int j = 0; j < m_RulerSpheres.Count(); ++j )
		{
			m_RulerSpheres.Get(j).Destroy();
		}
		m_RulerSpheres.Clear();
	}

	//------------------------------------------
	// RulerDelete
	//------------------------------------------
	void RulerDelete()
	{
		m_RulerPoints.Clear();
		
		EditorUpdateRuler();
	}

	//==========================================
	// GetLoadedScene
	//==========================================
	SceneData GetLoadedScene()
	{
		return m_SceneData;
	}

	//==========================================
	// GetSelectedSceneObjectIndex
	//==========================================
	int GetSelectedSceneObjectIndex()
	{
		array<SceneObject> scene_objects = GetSceneObjects();
		for ( int i = 0; i < scene_objects.Count(); i++ )
		{
			if ( GetSelectedSceneObject() == scene_objects.Get(i) )
			{
				return i;
			}
		}
		
		return -1;
	}

	//==========================================
	// GetSceneObjects
	//==========================================
	array<SceneObject> GetSceneObjects()
	{
		if ( m_SceneData )
		{
			return m_SceneData.GetSceneObjects();
		}
		
		return NULL;
	}
	
	//==========================================
	// GetScenePlayers
	//==========================================
	array<ScenePlayer> GetScenePlayers()
	{
		if ( m_SceneData )
		{
			return m_SceneData.GetScenePlayers();
		}
		
		return NULL;
	}
	
	//==========================================
	// GetSceneObjectsAll
	//==========================================
	array<SceneObject> GetSceneObjectsAll()
	{
		if ( m_SceneData )
		{
			return m_SceneData.GetSceneObjectsAll();
		}
		
		return NULL;
	}

	//==========================================
	// GetSceneObjectByIndex
	//==========================================
	SceneObject GetSceneObjectByIndex( int index )
	{
		array<SceneObject> scene_objects = GetSceneObjects();
		
		if ( scene_objects != NULL )
		{
			if ( index > -1 && index < scene_objects.Count() )
			{
				return scene_objects.Get( index );
			}
		}
		
		return NULL;
	}

	//==========================================
	// GetSceneObjectByEntity
	//==========================================
	SceneObject GetSceneObjectByEntity( EntityAI entity )
	{
		array<SceneObject> scene_objects = GetSceneObjects();
		
		if ( scene_objects != NULL )
		{
			for ( int i = 0; i <= scene_objects.Count(); ++i )
			{
				SceneObject obj = scene_objects.Get(i);
				EntityAI e = obj.GetObject();		
				
				if ( e == entity )
				{
					return obj;
				}
			}
		}
		
		return NULL;
	}
	
	//==========================================
	// GetSceneList
	//==========================================
	TStringArray GetSceneList()
	{
		return m_ModuleConfigScene.GetSceneList();
	}

	//==========================================
	// DeleteSceneObject
	//==========================================
	void DeleteSceneObject(SceneObject obj)
	{		
		if ( obj != NULL )
		{
			if ( obj.GetObject() != GetGame().GetPlayer() )
			{
				obj.UnlinkAll();
				
				if ( GetSelectedSceneObject() != NULL && GetSelectedSceneObject() == obj )
				{
					DeselectObject();
				}
				
				m_SceneData.DeleteSceneObject(obj);
			}
			else
			{
				SceneEditorMenuCmdPopup("You cant delete player!");
			}
		}
			
		EditorUpdate();
	}
	
	//---------------------------------------------------------------------------------
	// >> Protected	scope
	protected PluginDeveloper			m_ModuleDeveloper;
	protected PluginConfigScene			m_ModuleConfigScene;
	protected PluginConfigDebugProfile	m_ModuleConfigDebugProfile;

	//---------------------------------------------------------------------------------
	// >> protected Scope
	protected float					m_TimeLastClickRelease;
	protected float					m_TimeLastClickPress;
	protected int					m_PressedMouseID;
	protected bool					m_IsDragging;
	protected array<Shape>			m_ShapesSceneObjects;
	protected map<int, KeyBinding>	m_KeyBindings;
	protected SceneData				m_SceneData;
	protected SceneObject			m_SelectedObject;
	protected string				m_ClassBrowserSelected;
	protected bool					m_IsOpen;
	
	//------------------------------------------
	// DeselectObject
	//------------------------------------------
	protected void DeselectObject()
	{
		m_SelectedObject = NULL;
	}
	
	//------------------------------------------
	// EditorUpdate	
	//------------------------------------------
	protected void EditorUpdate()
	{
		if ( GetGame().IsMultiplayer() && GetGame().IsServer() )
		{
			return;
		}
		
		array<SceneObject> objects = GetSceneObjects();
		if (objects == NULL)
			return;

		for ( int i = 0; i < objects.Count(); ++i )
		{
			SceneObject obj = objects.Get(i);
			
			array<SceneObject> linked_scene_objects = obj.GetLinkedSceneObjects();
			int j;
			
			if ( IsOpened() )
			{
				obj.EditorShapeAdd();
				
				if ( GetSelectedSceneObject() && GetSelectedSceneObject() == obj )
				{
					if ( GetDrawSelection() )
					{
						obj.EditorShapeSelect();
					}
				}
				else
				{
					obj.EditorShapeDeselect();
				}
				
				for ( j = 0; j < linked_scene_objects.Count(); j++ )
				{					
					obj.EditorLineAdd( linked_scene_objects.Get(j) );
				}
			}
			else
			{
				obj.EditorShapeRemove();
				
				for ( j = 0; j < linked_scene_objects.Count(); j++ )
				{
					obj.EditorLineRemove( linked_scene_objects.Get(j) );
				}
			}
		}

		SceneEditorMenuCmdRefresh();
	}
	
	//------------------------------------------
	// SceneEditorMenuRefresh
	//------------------------------------------
	protected void SceneEditorMenuCmdRefresh()
	{
		CallMethod(CALL_ID_SCENE_EDITOR_COMMAND, new Param2<int, Param>(SCENE_EDITOR_CMD_REFRESH, NULL));
	}

	//------------------------------------------
	// SceneEditorMenuCmdPopup
	//------------------------------------------
	protected void SceneEditorMenuCmdPopup(string message)
	{
		CallMethod(CALL_ID_SCENE_EDITOR_COMMAND, new Param2<int, Param1<string>>(SCENE_EDITOR_CMD_POPUP, new Param1<string>(message)));
	}

	//------------------------------------------
	// SceneEditorMenuCmdSave
	//------------------------------------------
	protected void SceneEditorMenuCmdSave()
	{
		CallMethod(CALL_ID_SCENE_EDITOR_COMMAND, new Param2<int, Param>(SCENE_EDITOR_CMD_SAVE, NULL));
	}
	
	//------------------------------------------
	// IsMouseInScene
	//------------------------------------------
	protected bool IsMouseInScene()
	{
		Widget widget_under_mouse = GetWidgetUnderCursor();
		
		if ( widget_under_mouse != NULL && widget_under_mouse.GetName() == "pnl_SceneControl" )
		{
			return true;
		}
		
		return false;
	}
	
	//------------------------------------------
	// EditorUpdateRuler
	//------------------------------------------
	protected void EditorUpdateRuler()
	{
		RulerClearVisuals();
		
		for ( int i = 0; i < m_RulerPoints.Count(); ++i )
		{
			vector pos1 = m_RulerPoints.Get(i);
			
			m_RulerSpheres.Insert(Debug.DrawSphere(pos1, 0.1));
			
			if ( i < m_RulerPoints.Count() - 1 )
			{
				vector pos2 = m_RulerPoints.Get(i + 1);
				m_RulerLines.Insert(Debug.DrawLine(pos1, pos2));
			}
		}
	}
	
	//------------------------------------------
	// GetMousePositionInScene
	//------------------------------------------
	protected vector GetMousePositionInScene()
	{
		PlayerBase pb = GetGame().GetPlayer();

		if (!pb.m_Cam)
			return "0 0 0";
				
		vector dir = GetGame().GetPointerDirection();
		vector from = pb.m_Cam.GetPosition(); 
		vector to = from + ( dir * 10000 );   
		vector contact_pos;
		vector contact_dir;
		int contact_component;
		
		if ( GetGame().Raycast(from, to, contact_pos, contact_dir, contact_component) )   
		{
			return contact_pos;
		}
		
		return "0 0 0";
	}
}
