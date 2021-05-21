

class DayzPlayerItemBehaviorCfg extends HumanItemBehaviorCfg
{
	void DayzPlayerItemBehaviorCfg()
	{
		m_iStanceMask = DayZPlayerConstants.STANCEMASK_ALL;

		m_StanceMovements[DayZPlayerConstants.STANCEIDX_ERECT] = DayZPlayerConstants.MOVEMENT_IDLE | DayZPlayerConstants.MOVEMENT_WALK | DayZPlayerConstants.MOVEMENT_RUN | DayZPlayerConstants.MOVEMENT_SPRINT;
		m_StanceMovements[DayZPlayerConstants.STANCEIDX_CROUCH] = DayZPlayerConstants.MOVEMENT_IDLE | DayZPlayerConstants.MOVEMENT_RUN | DayZPlayerConstants.MOVEMENT_SPRINT;
		m_StanceMovements[DayZPlayerConstants.STANCEIDX_PRONE] = DayZPlayerConstants.MOVEMENT_IDLE | DayZPlayerConstants.MOVEMENT_RUN | DayZPlayerConstants.MOVEMENT_SPRINT;
		m_StanceMovements[DayZPlayerConstants.STANCEIDX_RAISEDERECT] = DayZPlayerConstants.MOVEMENT_IDLE | DayZPlayerConstants.MOVEMENT_WALK | DayZPlayerConstants.MOVEMENT_RUN | DayZPlayerConstants.MOVEMENT_SPRINT;
		m_StanceMovements[DayZPlayerConstants.STANCEIDX_RAISEDCROUCH] = DayZPlayerConstants.MOVEMENT_IDLE | DayZPlayerConstants.MOVEMENT_RUN | DayZPlayerConstants.MOVEMENT_SPRINT;
		m_StanceMovements[DayZPlayerConstants.STANCEIDX_RAISEDPRONE] = DayZPlayerConstants.MOVEMENT_IDLE;

		m_iPerItemCameraUserData = DayZPlayerCameras.PERITEMUD_INVALID;		//! per item camera user data to be uninitialized
	}

	void	SetEmptyHanded()
	{
		m_iStanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_PRONE | DayZPlayerConstants.STANCEMASK_RAISEDERECT;
		m_iPerItemCameraUserData = DayZPlayerCameras.PERITEMUD_EMPTYHANDED;
	}
	
	void	SetTwoHanded()
	{
	  SetEmptyHanded();
	  m_iPerItemCameraUserData = DayZPlayerCameras.PERITEMUD_TWOHANDED;
	}
	
	void	SetToolsOneHanded()
	{
	  m_iStanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH | DayZPlayerConstants.STANCEMASK_PRONE | DayZPlayerConstants.STANCEMASK_RAISEDERECT;
	  m_iPerItemCameraUserData = DayZPlayerCameras.PERITEMUD_ONEHANDED;
	}
	
	void	SetHeavyItems()
	{
	  m_iStanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
	  m_StanceMovements[DayZPlayerConstants.STANCEIDX_ERECT] = DayZPlayerConstants.MOVEMENT_IDLE | DayZPlayerConstants.MOVEMENT_WALK;
	  m_iPerItemCameraUserData = DayZPlayerCameras.PERITEMUD_TWOHANDED;
	}
}



void DayZPlayerTypeCreate(DayZPlayerType pType)
{
	Print("DayZPlayerTypeCreate called");

	//! behaviour 
	DayzPlayerItemBehaviorCfg 	emptyHanded = new DayzPlayerItemBehaviorCfg;
	emptyHanded.SetEmptyHanded();
	
	DayzPlayerItemBehaviorCfg 	twoHanded = new DayzPlayerItemBehaviorCfg;
	twoHanded.SetTwoHanded();
	
	DayzPlayerItemBehaviorCfg 	toolsOneHanded = new DayzPlayerItemBehaviorCfg;
	toolsOneHanded.SetToolsOneHanded();
	
	DayzPlayerItemBehaviorCfg 	heavyItemBehaviour = new DayzPlayerItemBehaviorCfg;
	heavyItemBehaviour.SetHeavyItems();
	
	//! set the default - use default instance set + default behaviour - empty hands
	pType.SetDefaultItemInHandsProfile("", emptyHanded);

	//! reset previous setup 
	pType.ResetItemInHandsProfiles();


	//! Pistols (RH IK)
	{
		DayzPlayerItemBehaviorCfg 	pistolItemBehaviour = new DayzPlayerItemBehaviorCfg;
		
		pType.AddItemInHandsProfile("EN5C_1911_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_1911.asi", pistolItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_CZ75_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_cz75.asi", pistolItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_DartGun", "dz/anims/workspaces/player/player_main/weapons/player_main_x2dartgun.asi", pistolItemBehaviour);
		//! pType.AddItemInHandsProfile("DE_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_pistol.asi", pistolItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_Derringer_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_derringer.asi", pistolItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_Flaregun_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_flaregun.asi", pistolItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_FNX_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_fnp45.asi", pistolItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_Glock19_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_glock.asi", pistolItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_LongHorn_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_longhorn.asi", pistolItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_Magnum_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_magnum.asi", pistolItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_MKII_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_rugermk2.asi", pistolItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_pb6p9", "dz/anims/workspaces/player/player_main/weapons/player_main_pb6p9.asi",pistolItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_MakarovIJ70_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_ij70.asi", pistolItemBehaviour);
		//! pType.AddItemInHandsProfile("PMM_Makarov_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_PMM_Makarov.asi", pistolItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_P1_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_p1.asi", pistolItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_Red9_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_mauserred9.asi", pistolItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_Shockpistol_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_taser.asi", pistolItemBehaviour);
		
		delete pistolItemBehaviour;
	}

	//! Firearms (RH,LH IK)		
	{
		DayzPlayerItemBehaviorCfg 	fireArmsItemBehaviour = new DayzPlayerItemBehaviorCfg;

		pType.AddItemInHandsProfile("EN5C_AK74_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_ak74u.asi", fireArmsItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_AK101_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_ak74u.asi", fireArmsItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_AKM_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_akm.asi", fireArmsItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_AugSteyr_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_aug.asi", fireArmsItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_B95_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_b95.asi", fireArmsItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_CZ61_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_cz61.asi", fireArmsItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_CZ527_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_cz527.asi", fireArmsItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_FAL_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_fnfal.asi", fireArmsItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_Izh18_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_izh18.asi", fireArmsItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_M4A1_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_m4a1.asi", fireArmsItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_M249_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_m249.asi",fireArmsItemBehaviour);
		pType.AddItemInHandsProfile("mini_uzi", "dz/anims/workspaces/player/player_main/weapons/player_main_miniuzi.asi",fireArmsItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_SawedoffMosin9130_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_mosin1903.asi", fireArmsItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_MP5K_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_mp5.asi", fireArmsItemBehaviour);
		pType.AddItemInHandsProfile("ots14", "dz/anims/workspaces/player/player_main/weapons/player_main_ots14.asi",fireArmsItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_PM73Rak_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_pm73.asi", fireArmsItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_Repeater_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_repeater.asi", fireArmsItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_Ruger1022_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_ruger1022.asi", fireArmsItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_ScoutBase", "dz/anims/workspaces/player/player_main/player_main_scout.asi",fireArmsItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_SKS_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_sks.asi", fireArmsItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_SVD_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_svd.asi", fireArmsItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_Trumpet_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_trumpet.asi", fireArmsItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_UMP45_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_ump45.asi", fireArmsItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_VSS_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_vss.asi", fireArmsItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_Winchester70_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_winchester70.asi", fireArmsItemBehaviour);

		//! Shotguns
		pType.AddItemInHandsProfile("Izh43Shotgun_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_izh43.asi", fireArmsItemBehaviour);
		pType.AddItemInHandsProfile("Mp133Shotgun_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_mp133.asi", fireArmsItemBehaviour);
		pType.AddItemInHandsProfile("Mp133Shotgun_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_mp133grip.asi", fireArmsItemBehaviour);
		pType.AddItemInHandsProfile("Saiga12KShotgun_Base", "dz/anims/workspaces/player/player_main/weapons/player_main_izh43.asi", fireArmsItemBehaviour);

		delete fireArmsItemBehaviour;
	}
	
	//! Archery (RH,LH IK)
	{
		DayzPlayerItemBehaviorCfg 	archeryItemBehaviour = new DayzPlayerItemBehaviorCfg;

		pType.AddItemInHandsProfile("EN5C_PVCBow", "dz/anims/workspaces/player/player_main/player_main_bow.asi", archeryItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_QuickieBow", "dz/anims/workspaces/player/player_main/weapons/player_main_bow_quicky.asi", archeryItemBehaviour);
		pType.AddItemInHandsProfile("RecurveBow", "dz/anims/workspaces/player/player_main/weapons/player_main_bow_recurve.asi", archeryItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_Crossbow", "dz/anims/workspaces/player/player_main/weapons/player_main_crossbow.asi", archeryItemBehaviour);

		delete archeryItemBehaviour;
	}
	
	//! Launchers
	{
		DayzPlayerItemBehaviorCfg 	launcherItemBehaviour = new DayzPlayerItemBehaviorCfg;

		pType.AddItemInHandsProfile("GP25Base", "dz/anims/workspaces/player/player_main/weapons/player_main_m72law.asi", launcherItemBehaviour);	
		pType.AddItemInHandsProfile("LAWBase", "dz/anims/workspaces/player/player_main/weapons/player_main_m72law.asi", launcherItemBehaviour);	
		pType.AddItemInHandsProfile("M203Base", "dz/anims/workspaces/player/player_main/weapons/player_main_m72law.asi", launcherItemBehaviour);
		pType.AddItemInHandsProfile("RPG7Base", "dz/anims/workspaces/player/player_main/weapons/player_main_m72law.asi", launcherItemBehaviour);

		delete launcherItemBehaviour;
	}

	//! Ammunition
	{
		pType.AddItemInHandsProfile("EN5C_Ammunition_Base", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Ammo_LAW_HE", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	

	}

	//! Attachments
	{	
		//! light
		pType.AddItemInHandsProfile("EN5C_UniversalLight", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_TLRLight", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
	
		//! magazine
		pType.AddItemInHandsProfile("EN5C_Magazine_Base", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
	
		//! muzzle
		pType.AddItemInHandsProfile("EN5C_AK_Bayonet", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_M9A1_Bayonet", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Mosin_Bayonet", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_SKS_Bayonet", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Mosin_Compensator", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_MP5_Compensator", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_ImprovisedSuppressor", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_M4_Suppressor", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_AK_Suppressor", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_PistolSuppressor", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Groza_Barrel_Short", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Groza_Barrel_Grip", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Groza_Barrel_Suppressor", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
	
		//! optic
		pType.AddItemInHandsProfile("EN5C_M4_CarryHandleOptic", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_BUISOptic", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_M68Optic", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_M4_T3NRDSOptic", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_FNP45_MRDOptic", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Crossbow_RedpointOptic", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_ReflexOptic", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_ACOGOptic", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_PUScopeOptic", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_KashtanOptic", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_LongrangeOptic", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_HuntingOptic", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_PistolOptic", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_PSO1Optic", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_PSO11Optic", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_GrozaOptic", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
	
		//! support
		pType.AddItemInHandsProfile("EN5C_M4_OEBttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_M4_MPBttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_M4_CQBBttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_AK_WoodBttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_AK74_WoodBttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_AK_FoldingBttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_AK_PlasticBttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_AKS74U_Bttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_MP5k_StockBttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Red9Bttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Fal_OeBttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Fal_FoldingBttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Saiga_Bttstck", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_M4_PlasticHndgrd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_M4_RISHndgrd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_M4_MPHndgrd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_AK_WoodHndgrd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_AK74_Hndgrd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_AK_RailHndgrd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_AK_PlasticHndgrd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_MP5_PlasticHndgrd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_MP5_RailHndgrd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_M249_Hndgrd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_M249_RisHndgrd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_AtlasBipod", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_M249_Bipod", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_GhillieAtt_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Groza_LowerReceiver", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_GrozaGL_LowerReceiver", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
	}
	
	//! Explosion
	{
		pType.AddItemInHandsProfile("EN5C_RGD5Grenade", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_M67Grenade", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_M18SmokeGrenade_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Grenade_Base", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_FlashGrenade", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_RDG2SmokeGrenade_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
	}

	//! Tools
	{
		pType.AddItemInHandsProfile("EN5C_Flashlight", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Pen_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_CanOpener", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Heatpack", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_MessTin", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Pliers", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Lockpick", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_WeaponCleaningKit", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_SewingKit", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_LeatherSewingKit", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Handcuffs", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_HandcuffKeys", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Cassette", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_FieldShovel", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Crowbar", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Hammer", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_MeatTenderizer", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Wrench", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_LugWrench", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Pipe", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Screwdriver", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Sickle", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Hacksaw", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_KitchenKnife", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_HuntingKnife", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)
		pType.AddItemInHandsProfile("EN5C_CombatKnife", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)		
		pType.AddItemInHandsProfile("EN5C_SteakKnife", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_HayHook", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_StoneKnife", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Iceaxe", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_PipeWrench", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)
		pType.AddItemInHandsProfile("EN5C_Hatchet", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)
		pType.AddItemInHandsProfile("EN5C_Machete", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_BrassKnuckles_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_TelescopicBaton", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_CattleProd", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_StunBaton", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Cleaver", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		
		//! Tools two handed
		pType.AddItemInHandsProfile("EN5C_Shovel", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);
		pType.AddItemInHandsProfile("EN5C_Mace", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);
		pType.AddItemInHandsProfile("EN5C_FarmingHoe", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);
		pType.AddItemInHandsProfile("EN5C_ImprovisedFishingRod", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);
		pType.AddItemInHandsProfile("EN5C_ObsoleteFishingRod", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);
		pType.AddItemInHandsProfile("EN5C_SledgeHammer", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);
		pType.AddItemInHandsProfile("EN5C_FishingRod", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);
		pType.AddItemInHandsProfile("EN5C_Broom", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);
		pType.AddItemInHandsProfile("EN5C_Paddle", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);
		pType.AddItemInHandsProfile("EN5C_FirefighterAxe", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);
		pType.AddItemInHandsProfile("EN5C_WoodAxe", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);
		pType.AddItemInHandsProfile("EN5C_Pitchfork", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);		
		pType.AddItemInHandsProfile("EN5C_Pickaxe", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);		
		pType.AddItemInHandsProfile("EN5C_Sword", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);		
		pType.AddItemInHandsProfile("EN5C_BaseballBat", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);		
		pType.AddItemInHandsProfile("EN5C_HockeyStick", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);		
		pType.AddItemInHandsProfile("EN5C_Chainsaw", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);
		pType.AddItemInHandsProfile("EN5C_FireExtinguisher", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);
	}
	
	//! Camping
	{
		//! Camping 2handed
		pType.AddItemInHandsProfile("EN5C_Spotlight", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);	
		pType.AddItemInHandsProfile("EN5C_CableReel", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);	

		//! Camping heavy
		pType.AddItemInHandsProfile("EN5C_MediumTent", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_CarTent", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_PartyTent", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_CamoNet", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_CamoNetShelter", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_BatteryCharger", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour);
	}

	//! Consumables
	{
		pType.AddItemInHandsProfile("EN5C_Paper", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);//!< use default (1 handed)
		pType.AddItemInHandsProfile("EN5C_DuctTape", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);//!< use default (1 handed)
		pType.AddItemInHandsProfile("EN5C_Bandana_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);//!< use default (1 handed)
		pType.AddItemInHandsProfile("EN5C_Rag", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);//!< use default (1 handed)
		pType.AddItemInHandsProfile("EN5C_BurlapStrip", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);//!< use default (1 handed)
		pType.AddItemInHandsProfile("EN5C_Stone", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);//!< use default (1 handed)
		pType.AddItemInHandsProfile("EN5C_SmallStone", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);//!< use default (1 handed)
		pType.AddItemInHandsProfile("EN5C_BatteryD", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);//!< use default (1 handed)
		pType.AddItemInHandsProfile("EN5C_Battery9V", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);//!< use default (1 handed)
		pType.AddItemInHandsProfile("EN5C_SmallGasCanister", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);//!< use default (1 handed)
		pType.AddItemInHandsProfile("EN5C_Spraycan_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);//!< use default (1 handed)
		pType.AddItemInHandsProfile("EN5C_Matchbox", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);//!< use default (1 handed)
		pType.AddItemInHandsProfile("EN5C_NailBox", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);//!< use default (1 handed)
		pType.AddItemInHandsProfile("EN5C_Roadflare", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);//!< use default (1 handed)
		pType.AddItemInHandsProfile("EN5C_Chemlight_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);//!< use default (1 handed)
		pType.AddItemInHandsProfile("EN5C_Hook", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);//!< use default (1 handed)
		pType.AddItemInHandsProfile("EN5C_BoneHook", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);//!< use default (1 handed)
		pType.AddItemInHandsProfile("EN5C_Nail", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);//!< use default (1 handed)
		pType.AddItemInHandsProfile("EN5C_Bone", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);//!< use default (1 handed)
		pType.AddItemInHandsProfile("EN5C_Bait", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);//!< use default (1 handed)
		pType.AddItemInHandsProfile("EN5C_BoneBait", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);//!< use default (1 handed)
		pType.AddItemInHandsProfile("EN5C_Netting", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);//!< use default (1 handed)
		
		//! Consumables two handed
		pType.AddItemInHandsProfile("EN5C_Firewood", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);	
		pType.AddItemInHandsProfile("EN5C_LargeGasCanister", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);	
		pType.AddItemInHandsProfile("EN5C_MediumGasCanister", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);	
		pType.AddItemInHandsProfile("EN5C_ButaneCanister", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);	
		pType.AddItemInHandsProfile("EN5C_Pelt_Base", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);	
		pType.AddItemInHandsProfile("EN5C_TannedLeather", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);	
		pType.AddItemInHandsProfile("EN5C_WoodenPlank", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);
		pType.AddItemInHandsProfile("EN5C_PileOfWoodenPlanks", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);
		pType.AddItemInHandsProfile("EN5C_MetalPlate", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);
		pType.AddItemInHandsProfile("EN5C_GardenLime", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);
		pType.AddItemInHandsProfile("EN5C_Bark_ColorBase", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);
	}

	//! Containers
	{
		//! pType.AddItemInHandsProfile("EN5C_Refrigerator", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		//! pType.AddItemInHandsProfile("EN5C_RefrigeratorMinsk", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_FirstAidKit", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_SmallProtectorCase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_CarrierPouches", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_AmmoBox", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Bear_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	

		//! Containers - 2handed	
		pType.AddItemInHandsProfile("EN5C_WaterproofBag_ColorBase", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);

		//! Containers - Heavy
		pType.AddItemInHandsProfile("Container_BarrelBase", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_Barrel_ColorBase", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour);
		pType.AddItemInHandsProfile("EN5C_TrashCan", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour);
	}

	//! Cooking
	{
		pType.AddItemInHandsProfile("EN5C_ClutterCutterFireplace", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_FireplaceBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_BarrelHoles_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Tripod", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_FryingPan", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_PortableGasStove", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_PortableGasLamp", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Candle", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_PetrolLighter", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		
		//! Cooking two handed
		pType.AddItemInHandsProfile("EN5C_Pot", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);
	}

	//! Crafting
	{
		pType.AddItemInHandsProfile("EN5C_Rope", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_ImprovisedRope", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_MetalWire", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Torch", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_ChickenFeather", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_GorkaHelmetVisor", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_HandDrillKit", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	

		//! Crafting 2hd
		pType.AddItemInHandsProfile("EN5C_Spear", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);
		pType.AddItemInHandsProfile("EN5C_BurlapSack", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);
		pType.AddItemInHandsProfile("EN5C_WoodenStick", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);
		pType.AddItemInHandsProfile("EN5C_LongWoodenStick", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);

	}

	//! Cultivation
	{
		pType.AddItemInHandsProfile("EN5C_TomatoSeedsPack", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_SeedBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_PepperSeedsPack", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_PumpkinSeedsPack", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_ZucchiniSeedsPack", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_CannabisSeedsPack", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_AntiPestsSpray", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_ParticleTest", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		
		//! Cultivation two handed
		pType.AddItemInHandsProfile("EN5C_PlantMaterial", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);

	}
	
	//! Drinks
	{
		pType.AddItemInHandsProfile("EN5C_Bottle_Base", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_RiverWater", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_WellWater", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Canteen", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_WaterBottle", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Vodka", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_WaterPouch_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_SodaCan_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_SodaCan_Empty", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
	}
	
	//! Food
	{
		//! Food animal corpses
		pType.AddItemInHandsProfile("EN5C_Carp", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Sardines", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Mackerel", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Worm", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
	
		//! Food cans
		pType.AddItemInHandsProfile("EN5C_BakedBeansCan", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_BakedBeansCan_Opened", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_PeachesCan", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_PeachesCan_Opened", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_TacticalBaconCan", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_TacticalBaconCan_Opened", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_SpaghettiCan", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_SpaghettiCan_Opened", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_SardinesCan", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_SardinesCan_Opened", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_TunaCan", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_TunaCan_Opened", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
	
		//! Food fruits
		pType.AddItemInHandsProfile("EN5C_Apple", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Banana", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Orange", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Tomato", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_GreenBellPepper", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Zucchini", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Pumpkin", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_SlicedPumpkin", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Potato", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Kiwi", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_SambucusBerry", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_CaninaBerry", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Cannabis", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Plum", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Pear", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)
	
		//! Food meats
		pType.AddItemInHandsProfile("EN5C_HumanSteakMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_GoatSteakMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_MouflonSteakMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_BoarSteakMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_PigSteakMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_DeerSteakMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_WolfSteakMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_BearSteakMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_CowSteakMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_SheepSteakMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_FoxSteakMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_ChickenBreastMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_RabbitLegMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_CarpFilletMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_MackerelFilletMeat", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Lard", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
	
		//! Food mushrooms
		pType.AddItemInHandsProfile("EN5C_AgaricusMushroom", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_AmanitaMushroom", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_MacrolepiotaMushroom", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_LactariusMushroom", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_PsilocybeMushroom", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_AuriculariaMushroom", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
	
		//! Food basic
		pType.AddItemInHandsProfile("EN5C_Edible_Base", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_BoxCerealCrunchin", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_PowderedMilk", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_SmallGuts", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Guts", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Rice", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Marmalade", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
	}
	
	//! Medicial 
	{
		pType.AddItemInHandsProfile("EN5C_Bottle_Base", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_BandageDressing", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_DisinfectantSpray", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_DisinfectantAlcohol", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_PurificationTablets", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_CharcoalTablets", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_PainkillerTablets", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_VitaminBottle", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_TetracyclineAntibiotics", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Epinephrine", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Morphine", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Syringe", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_ClearSyringe", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_BloodSyringe", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_InjectionVial", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_SalineBag", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_StartKit", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_SalineBagIV", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_BloodBagEmpty", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_BloodBagFull", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_BloodBagIV", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_BloodTestKit", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Splint", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Thermometer", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_CigarettePack_ColorBase", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		
		//! Medicial two handed
		pType.AddItemInHandsProfile("EN5C_Defibrillator", "dz/anims/workspaces/player/player_main/player_main_2h.asi", twoHanded);
	}

	//! Navigation
	{
		pType.AddItemInHandsProfile("EN5C_ChernarusMap", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Compass", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_OrienteeringCompass", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
	}
	
	//! Optic
	{
		pType.AddItemInHandsProfile("EN5C_Binoculars", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
	}

	//! Radio
	{
		pType.AddItemInHandsProfile("EN5C_Transmitter_Base", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_StaticRadio", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Megaphone", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_Radio", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
       
	   //! Radio heavy
		pType.AddItemInHandsProfile("EN5C_CarBattery", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour);	//!< use default (1 handed)	
	}

	//! Traps
	{
		pType.AddItemInHandsProfile("EN5C_LandMine", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_SmallFishTrap", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_FishNetTrap", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_TripwireTrap", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
		pType.AddItemInHandsProfile("EN5C_RabbitSnareTrap", "dz/anims/workspaces/player/player_main/player_main_1h.asi", toolsOneHanded);	//!< use default (1 handed)	
    
		//! Traps heavy
		pType.AddItemInHandsProfile("EN5C_BearTrap", "dz/anims/workspaces/player/player_main/player_main_heavy.asi", heavyItemBehaviour);
	}
	
	delete toolsOneHanded;
	delete heavyItemBehaviour;
	delete twoHanded;
	delete emptyHanded;
	
	//! register camera creators
	DayZPlayerCameras.RegisterCameras(pType);
}
