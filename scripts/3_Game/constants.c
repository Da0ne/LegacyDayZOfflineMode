 /**
 * \defgroup Various timers
 * \desc constants for various timers
 * @{
 */ 
const int DISPLAY_STATUS_UPDATE_INTERVAL = 1000;
/** @}*/

/**
 * \defgroup Character attachment slots
 * \desc Character attachment slots
 * @{
 */
const int CHAR_SLOT_SHOULDER	= 0;
const int CHAR_SLOT_MELEE		= 1;
const int CHAR_SLOT_HEADGEAR    = 2;
const int CHAR_SLOT_MASK      	= 3;
const int CHAR_SLOT_EYEWEAR     = 4;
const int CHAR_SLOT_HANDS      	= 5;
const int CHAR_SLOT_GLOVES      = 6;
const int CHAR_SLOT_ARMBAND     = 7;
const int CHAR_SLOT_VEST		= 8;
const int CHAR_SLOT_BODY		= 9;
const int CHAR_SLOT_BACK      	= 10;
const int CHAR_SLOT_LEGS      	= 11;
const int CHAR_SLOT_FEET      	= 12;
const int CHAR_SLOT_MAGAZINE    = 13;
/** @}*/


/**
 * \defgroup UI
 * \desc constants User Interface
 * @{
 */
const int IDC_OK								= 1;
const int IDC_CANCEL						= 2;
const int IDC_RESTART						= 5;

const int IDC_MAIN_NEXT_CHARACTER	= 50;
const int IDC_MAIN_PREV_CHARACTER	= 51;

const int IDC_MAIN_OPTIONS      = 102;
const int IDC_MAIN_MULTIPLAYER  = 105;
const int IDC_MAIN_QUIT         = 106;
const int IDC_MAIN_CONTINUE			= 114;
const int IDC_MAIN_PLAY         = 142;
const int IDC_MAIN_CHARACTER		= 143;
const int IDC_MULTI_REFRESH			= 123;

const int IDC_BOOK_VIEWER_PREV  = 102;
const int IDC_BOOK_VIEWER_NEXT  = 103;

//! ingame menu
const int IDC_INT_RETRY					= 105;
const int IDC_INT_EXIT					= 107;
/** @}*/

/**
 * \defgroup MenuID
 * \desc constants for menu pages
 * @{
 */
const int MENU_ANY									= 1;
const int MENU_NONE									= 2;
const int MENU_UNKNOWN								= 4;
const int MENU_ASSIGNMENT							= 8;
const int MENU_CHARACTER							= 16;
const int MENU_CHAT									= 32;
const int MENU_EARLYACCESS							= 64;
const int MENU_INGAME								= 128;
const int MENU_INSPECT								= 256;
const int MENU_INVENTORY							= 512;
const int MENU_LOADING								= 1024;
const int MENU_MAIN									= 2048;
const int MENU_OPTIONS								= 4096;
const int MENU_SCENE_EDITOR							= 8192;
const int MENU_SCRIPTCONSOLE						= 16384;
const int MENU_STARTUP								= 32768;
const int MENU_CHAT_INPUT							= 65536;
const int MENU_SCRIPTCONSOLE_DIALOG_PRESET_NAME		= 131072;
const int MENU_SCRIPTCONSOLE_DIALOG_PRESET_RENAME 	= 262144;
const int MENU_CONTROLS_PRESET						= 524288;
const int MENU_NOTE									= 1048576;
const int MENU_MAP									= 2097152;
const int MENU_BOOK									= 4194304;
const int MENU_HELP_SCREEN      					= 8388608;

const int GUI_WINDOW_MISSION_LOADER = 1;

const int SPAWNTYPE_GROUND		= 0;
const int SPAWNTYPE_HANDS		= 1;
const int SPAWNTYPE_INVENTORY	= 2;
const int SPAWNTYPE_ATTACHMENT	= 3;

const string CFG_VEHICLESPATH = "CfgVehicles";
const string CFG_WEAPONSPATH = "CfgWeapons";
const string CFG_MAGAZINESPATH = "CfgMagazines";
const string CFG_RECIPESPATH = "CfgRecipes";

/** @}*/

/**
 * \defgroup Local data saving to files
 * \desc constants for saving local datas
 * @{
 */
const int		CFG_ARRAY_ITEMS_MAX		= 64;
const string	CFG_FILE_FIXED_PROFILE	= "Scripts/profile_fixed.cfg";
const string	CFG_FILE_USER_PROFILE	= "$profile:profile.cfg";
const string	CFG_FILE_DEBUG_PROFILE	= "$profile:debugProfile.cfg";
const string	CFG_FILE_SEARCH_HISTORY	= "$profile:search_history.history";
const string	CFG_FILE_SQF_HISTORY	= "$profile:script_sqf.history";
const string	CFG_FILE_ENS_HISTORY	= "$profile:script_enscript.history";
const string	CFG_FILE_SCRIPT_LOG		= "$profile:script.log";
const string	CFG_FILE_SCRIPT_LOG_EXT	= "$profile:scriptExt.log";
/** @}*/

/**
 * \defgroup RPC commands
 * \desc Constants for sending RPC commands on server
 * @{
 */
const int RPC_SYNC_ITEM_VAR                     = 1;
const int RPC_SYNC_STAT                         = 2;
const int RPC_WRITE_NOTE                        = 3;
const int RPC_SYNC_DISPLAY_STATUS               = 4;
const int RPC_SQF_PLAYER_NOTIFI_SYNCH           = 5;
const int RPC_SQF_PLAYER_NOTIFI_SYNCH_CLEAN     = 6;
const int RPC_RECIPE_SEND    					= 7;
const int RPC_ON_SET_CAPTIVE					= 8;
const int RPC_SYNC_SCENE_OBJECT					= 9;
const int RPC_READ_A_BOOK               		= 10;	
const int RPC_USER_ACTION_SYNCH 				= 11;
const int RPC_USER_ACTION_MESSAGE 				= 12;
const int RPC_ITEM_DIAG							= 13;
const int RPC_ITEM_DIAG_CLOSE					= 14;
const int RPC_SET_OBJECT_POSITION 				= 15;
const int RPC_USER_ACTION_MESSAGES 				= 16;
const int RPC_ITEM_SPLIT						= 17;
const int RPC_ITEM_COMBINE						= 18;
const int RPC_PLAYER_STATES_ON					= 19;
const int RPC_PLAYER_STATES_OFF					= 20;
const int RPC_PLACING_DEBUG						= 21;
const int RPC_PLACING_START						= 22;
const int RPC_PLACING_STOP						= 23;
const int RPC_PLACING_FINISH					= 24;
const int RPC_STAMINA							= 25;
/** @}*/

/**
 * \defgroup Developer RPC commands
 * \desc Constants for sending developer RPC commands on server
 * @{
 */
const int DEV_RPC_SPAWN_ITEM                    = 101;
const int DEV_RPC_CLEAR_INV                     = 102;
const int DEV_RPC_TELEPORT                      = 103;
const int DEV_RPC_SET_PLAYER_DIRECTION          = 104;
const int DEV_RPC_SEND_SERVER_LOG               = 105;
const int DEV_RPC_TOGGLE_GOD_MODE               = 106;
const int DEV_RPC_STATS_DATA					= 107;
const int DEV_RPC_MODS_DATA						= 108;
const int DEV_RPC_AGENTS_DATA					= 109;
const int DEV_RPC_CREATE_SCENE_OBJECT 			= 110;
const int DEV_RPC_SCENE_LOAD 					= 111;
/** @}*/

/**
 * \defgroup Sync update commands
 * \desc Constants for synchronization update commands
 * @{
 */
const int DEV_STATS_UPDATE						= 1;
const int DEV_MODS_UPDATE						= 2;
const int DEV_AGENTS_UPDATE						= 3;
/** @}*/

/**
 * \defgroup Message commands
 * \desc Messaging System - the numbers must be 0 or higher, and the highest number should not be bigger than NUM_OF_CHANNELS-1
 * @{
 */
const int NUM_OF_CHANNELS 		= 100;
const int MSG_TEST_CHANNEL		= 0;
const int MSG_IMMUNE_REACTION	= 1;
const int MSG_STATS_SYNCED		= 2;
const int MSG_NOTIFIERS_TICK	= 3;
/** @}*/
	
/**
 * \defgroup Modifier pools minimum tick times in ms
 * \desc Modifier pools minimum tick times
 * @{
 */
const int MIN_TICK_MDFR_POOL_MAIN = 10000;//in ms
const int MIN_TICK_MDFR_POOL_PLAYER = 3000;
const int MIN_TICK_MDFR_POOL_ENVIRONMENT = 3000;
const int MIN_TICK_MDFR_POOL_DISEASE = 30000;
const int MIN_TICK_NOTIFIERS = 1000;
/** @}*/

/**
 * \defgroup Configurations for Environment class
 * \desc Configurations for Environment class
 * @{
 */
const float ENVIRO_TICK_RATE = 2; //in secs. how often should enviro effet process
const float	ENVIRO_TICKS_TO_WETNESS_CALCULATION = 30; // each X (ticks) is processed wetness on items on player
const float ENVIRO_WET_PENALTY_EFFECT = 5;//how much wetness of clothes affects players heatcomfort
const float ENVIRO_WET_INCREMENT = 0.01; // amount of wetness added to items wet value each tick if is raining
const float ENVIRO_DRY_INCREMENT = 0.0005; // amount of wetness subtracted from items wet value each tick if is not raining due to player heat
const float ENVIRO_SUN_INCREMENT = 0.002; // amount of wetness subtracted from items wet value each tick if is not raining due to sun
const float ENVIRO_CLOUD_DRY_EFFECT = 0.1; // how many % of ENVIRO_SUN_INCREMENT is reduced by cloudy sky
const float ENVIRO_CLOUDS_TEMP_EFFECT = 0.1; // how many % of environment temperature can be lowered by clouds
const float ENVIRO_FOG_TEMP_EFFECT = 0.2; // how many % of environment temperature can be lowered by fog
const float ENVIRO_WET_PENALTY = 0.5; //at which state of item wetness (0-1) will heat isolation start having negative effect on heat comfort of item
const float ENVIRO_WET_PASSTHROUGH_KOEF = 0.1; //how many times slower is wetting/drying items in backpacks
const float ENVIRO_ITEM_HEAT_TRANSFER_KOEF = 0.01; //converts temperature of items to entities heatcomfort gain
const float ENVIRO_WATER_TEMPERATURE_KOEF = 0.5; //how many time is water colder than air
const float ENVIRO_DEFAULT_ENTITY_HEAT = 3; //heat entity generates if not moving
const float ENVIRO_TEMPERATURE_HEIGHT_REDUCTION = 0.0065; //amount of ?C reduced for each 100 meteres of height above water level
const float ENVIRO_WIND_EFFECT = 1; //amount of % wind affect drying/wetting
const float ENVIRO_HIGH_NOON = 12; //when is sun highest on sky
const float	ENVIRO_HEAT_COMFORT_MAX = 210;
/** @}*/

/**
 * \defgroup Configurations for StaminaHandler class
 * \desc Configurations for StaminaHandler class
 * @{
 */
	// unit = currently percent (stamina max is 100)
const int 	STAMINA_DRAIN_STANDING_SPRINT_PER_SEC = 5; //in units (how much sprint depletes stamina)
const int 	STAMINA_DRAIN_CROUCHED_SPRINT_PER_SEC = 1; //in units (how much sprint in crouch depletes stamina)
const int 	STAMINA_DRAIN_PRONE_SPRINT_PER_SEC = 3; //in units (how much sprint in prone depletes stamina)
const int 	STAMINA_GAIN_JOG_PER_SEC = 2; //in units (how much of stamina units is gained while jogging)
const int 	STAMINA_GAIN_WALK_PER_SEC = 4; //in units (how much of stamina unitsis gained while walking)
const int 	STAMINA_GAIN_IDLE_PER_SEC = 6; //in units (how much of stamina unitsis gained while iddling)
const float STAMINA_GAIN_BONUS_CAP = 7.5; //in units (tells how much extra units can be added at best to stamina regain)
const float STAMINA_KG_TO_STAMINAPERCENT_PENALTY = 2.5; //in units (by how many  units is max stamina bar reduced for each 1 kg of load weight)
const float STAMINA_MIN_CAP = 25; //in units (overload won't reduce max stamina bar under this value)
const float STAMINA_SPRINT_TRESHOLD = 25; //in units (how many units of stamina you need regained in order to be able to start sprinting)
const float STAMINA_KG_TO_GRAMS = 1000; //for kg to g conversion
const float STAMINA_SYNC_RATE = 1; //in secs
/** @}*/

/**
 * \defgroup User action internal state machine states
 * \desc User action internal state machine states
 * @{
 */
const int 		UA_FAILED = 1;
const int 		UA_PROCESSING = 2;
const int 		UA_REPEAT = 3;
const int 		UA_FINISHED = 4;
const int		UA_CANCEL = 5;
const int		UA_ERROR = 6;
const int       UA_START = 7;
const int       UA_STARTT = 8;
const int       UA_CANCELT = 9;
const int       UA_FINISHEDT = 10;
const int    	UA_ANIM_EVENT = 11;
/** @}*/


/**
 * \defgroup Barel related functions constants
 * \desc Barel related functions constants
 * @{
 */
const int BAREL_LIME_PER_PELT = 100; //grams per pelt 
const int BAREL_BLEACH_PER_CLOTH = 50; //ml per item
const float BAREL_LIME_PER_PLANT = 50; //grams per gram
/** @}*/



/**

 * \defgroup Agent transmission system
 * \desc Agent transmission system
 * @{
 */
const int AGT_INV_IN	 		= 1;
const int AGT_INV_OUT	 		= 2;
const int AGT_UACTION_CONSUME	= 3;
const int AGT_TRANSFER_LIQUID	= 4;
const int AGT_UACTION_TOUCH		= 5;

const int DEF_BIOLOGICAL		= 1;

const int INFLUENZA_AGENT_THRESHOLD = 100;
const int CHOLERA_AGENT_THRESHOLD_ACTIVATE = 100;
const int CHOLERA_AGENT_THRESHOLD_DEACTIVATE = 20;
//agent list
const int AGT_CHOLERA 		= 1;
const int AGT_INFLUENZA 	= 2;



/** @}*/
			
const int QUANTITY_HIDDEN = 0;
const int QUANTITY_COUNT = 1;
const int QUANTITY_PROGRESS = 2;


			
/**
 * \defgroup LiquidTypes
 * \desc Constants for liquid types
 * @{
 */
// BEWARE ALL INDIVIDUAL LIQUID TYPES ARE ALSO REPRESENTED CONFIG-SIDE AND MUST MATCH(all changes must be made on both sides)
// NOTE ANY NUMBER HERE MUST BE A 0 OR ANY POWER OF TWO, THERE IS A MAXIMUM OF 32 INDIVIDUAL LIQUID TYPES POSSIBLE
const int LIQUID_BLOOD_0_P	= 1;
const int LIQUID_BLOOD_0_N	= 2;
const int LIQUID_BLOOD_A_P	= 4;
const int LIQUID_BLOOD_A_N	= 8;
const int LIQUID_BLOOD_B_P	= 16;
const int LIQUID_BLOOD_B_N	= 32;
const int LIQUID_BLOOD_AB_P	= 64;
const int LIQUID_BLOOD_AB_N = 128;

const int LIQUID_WATER = 256;
const int LIQUID_RIVERWATER = 512;
const int LIQUID_VODKA = 1024;
const int LIQUID_BEER = 2048;
const int LIQUID_GASOLINE = 4096;
const int LIQUID_DIESEL = 8192;
const int LIQUID_DISINFECTANT = 16384;
// these are groups which do not have to correspond with configs
const int GROUP_LIQUID_BLOOD = 255;
const int GROUP_LIQUID_ALL = -1;//-1 = all bits to 1
/** @}*/
	
/**
 * \defgroup ChatMessagesVisibility
 * \desc Constants for toggle chat messages type visibility
 * @{
 */
const string SYSTEM_CHAT_MSG = "system_chat_msg";
const string GLOBAL_CHAT_MSG = "global_chat_msg";
const string DIRECT_CHAT_MSG = "direct_chat_msg";
const string VEHICLE_CHAT_MSG = "vehicle_chat_msg";
const string RADIO_CHAT_MSG = "radio_chat_msg";	
/** @}*/

/**
 * \defgroup Repair Types
 * \desc Constants for Repair Types
 * @{
 */
const int REPAIR_TYPE_SHARPEN = 0;
const int REPAIR_TYPE_CLEAN = 1;
const int REPAIR_TYPE_TAILOR = 2;
const int REPAIR_TYPE_DUCTTAPE = 3;
/** @}*/

/**
 * \defgroup Repair Types
 * \desc Constants for Repair Types
 * @{
 */
const int VARIABLE_QUANTITY 	= 1;
const int VARIABLE_ENERGY 		= 2;
const int VARIABLE_TEMPERATURE 	= 4;
const int VARIABLE_WET 			= 8;
const int VARIABLE_BLOODTYPE	= 16;
const int VARIABLE_LIQUIDTYPE	= 32;
const int VARIABLE_ISLIT		= 64;
/** @}*/

