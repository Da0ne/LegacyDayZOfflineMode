class DeveloperFreeCamera
{
	protected static autoptr Link<Camera>					m_FreeCamera;
	
	static void OnInit()
	{
		m_FreeCamera			= NULL;
	}
	
	//! Enable / Disable Free camera (Fly mod) - disable of camera will teleport player at current free camera position.
	static void FreeCameraToggle( PlayerBase player, bool teleport_player = false )
	{
		if ( !IsFreeCameraEnabled() )
		{
			EnableFreeCamera( player );
		}
		else 
		{
			DisableFreeCamera( player, teleport_player );
		}
	}
	
	static void EnableFreeCameraSceneEditor( PlayerBase player )
	{
		EnableFreeCamera(player);
		
		Camera free_camera = GetFreeCamera();
		
		if ( free_camera )
		{
			free_camera.Command( "MOUSE MOVE OFF" );
			free_camera.Command( "INERTIA OFF" );
		}
	}
	
	//! Is free camera enabled
	static bool IsFreeCameraEnabled()
	{
		if ( m_FreeCamera != NULL )
		{
			return true;
		}
		
		return false;
	}
	
	//! Get current free camera
	static Camera GetFreeCamera()
	{
		if ( m_FreeCamera != NULL )
		{
			return m_FreeCamera.Ptr();
		}
		
		return NULL;
	}

	//---------------------------------------------
	// EnableFreeCamera
	//---------------------------------------------
	private static void SetFreeCamera( Camera camera )
	{
		if ( camera )
		{
			m_FreeCamera = new Link<Camera>( camera );
		}
		else
		{
			if ( m_FreeCamera != NULL )
			{
				delete m_FreeCamera;
			}
			m_FreeCamera = NULL;
		}
	}

	//---------------------------------------------
	// EnableFreeCamera
	//---------------------------------------------
	protected static void EnableFreeCamera( PlayerBase player )
	{
		PlayerBase pb = (PlayerBase)GetGame().GetPlayer();
		if (pb)
		{
			if (pb.m_Cam == NULL){
				GetGame().ExecuteSQF("nul = [] spawn { DZ_FREE_CAM = \"Camera\" camCreate position player; DZ_FREE_CAM cameraEffect [\"internal\",\"back\"]; DZ_FREE_CAM camCommit 0; player callMethod ['CreateFreeCamInstance', DZ_FREE_CAM]; }; ");
				SetFreeCamera( pb.m_Cam );
			}else{
				GetGame().ExecuteSQF("DZ_FREE_CAM cameraEffect [\"terminate\",\"back\"]; camDestroy DZ_FREE_CAM;"); 
			}
		}
	}

	//---------------------------------------------
	// DisableFreeCamera
	//---------------------------------------------
	protected static void DisableFreeCamera( PlayerBase player, bool teleport_player = false )
	{
		if ( player )
		{
			if ( m_FreeCamera != NULL )
			{
				if ( teleport_player )
				{
					Camera camera = m_FreeCamera.Ptr();
					if ( camera == NULL )
					{
						GetGame().GetWorld().DeleteFreeCamera();
						SetFreeCamera( NULL );

						GetGame().GetMission().RefreshCrosshairVisibility();

						return;
					}
					vector from = camera.GetPosition(); 
					vector to = from + ( Vector(0,-1,0) * 10000 );   
					vector contact_pos;   
					vector contact_dir;   
					int contact_component; 
					
					if ( GetGame().Raycast(from, to, contact_pos, contact_dir, contact_component) )   
					{
						DeveloperTeleport.SetPlayerPosition( player, contact_pos );
						DeveloperTeleport.SetPlayerDirection( player, camera.GetDirection() );
					}
				}
				
				GetGame().ExecuteSQF("DZ_FREE_CAM cameraEffect [\"terminate\",\"back\"]; camDestroy DZ_FREE_CAM;");
				SetFreeCamera( NULL );

				GetGame().GetMission().RefreshCrosshairVisibility();
				//GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(GetGame().GetMission(), "RefreshCrosshairVisibility", NULL);
			}
		}
		else
		{
			Print("DisableFreeCamera( PlayerBase player, bool teleport_player ) -> player is NULL");
			Debug.LogError("DisableFreeCamera( PlayerBase player, bool teleport_player ) -> player is NULL", "DeveloperFreeCamera");
		}
	}
	
	//---------------------------------------------
	// OnRPCFreeCameraEnable
	//---------------------------------------------
	static void OnSetFreeCameraEvent( PlayerBase player, Camera camera )
	{
		if ( player )
		{
			if ( !GetGame().IsMultiplayer() )
			{
				PluginSceneManager plugin_scene_manager = (PluginSceneManager)GetPlugin(PluginSceneManager);
				
				if ( plugin_scene_manager.IsOpened() )
				{
					plugin_scene_manager.EditorToggle();
				}
			}
			
			SetFreeCamera( camera );
			
			GetGame().GetMission().RefreshCrosshairVisibility();
			//GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).Call(GetGame().GetMission(), "RefreshCrosshairVisibility", NULL);
		}
	}
}