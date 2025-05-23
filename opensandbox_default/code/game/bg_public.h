// 
// OpenSandbox
// 
// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2008-2012 OpenArena Team
// Copyright (C) 2023-2024 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// 
// This file is part of OpenSandbox.
// 
// OpenSandbox is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License, version 2,
// as published by the Free Software Foundation.
// 
// This modified code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this project. If not, see <http://www.gnu.org/licenses/>.
// 
// Contact: opensandboxteam@gmail.com
//

#if defined(BG_PUBLIC_H)
#else
#define BG_PUBLIC_H 1

#define	GAME_VERSION		"game"

#define	DEFAULT_GRAVITY		800
#define	GIB_HEALTH			-66
#define	ARMOR_PROTECTION	0.66

#define	MAX_ITEMS			1024

#define	RANK_TIED_FLAG		0x4000

#define DEFAULT_SHOTGUN_SPREAD	700
#define DEFAULT_SHOTGUN_COUNT	11

#define	ITEM_RADIUS			15		// item sizes are needed for client side pickup detection

#define	TOOLGUN_RANGE		4096
#define	PHYSGUN_RANGE		8192
#define	GRAVITYGUN_RANGE	1024
#define	GRAVITYGUN_DIST		128

#define	SCORE_NOT_PRESENT	-9999	// for the CS_SCORES[12] when only one player is present

#define	VOTE_TIME			30000	// 30 seconds before vote times out

#define	MINS_Z				-24
#define	DEFAULT_VIEWHEIGHT	26
#define CROUCH_VIEWHEIGHT	12
#define	DEAD_VIEWHEIGHT		-16

//Domination points
#define MAX_DOMINATION_POINTS 8
#define MAX_DOMINATION_POINTS_NAMES 20

// physics engine
#define		PHYS_ROTATING 0.020
#define		PHYS_DAMAGESENS 30

#define		VEHICLE_SENS 30
#define		VEHICLE_DAMAGE 0.05
#define		VEHICLE_DAMAGESENS 30

#ifdef GAME
#define		PHYS_PROP_IMPACT g_physimpact.value
#define		PHYS_SENS g_physimpulse.integer
#define		PHYS_DAMAGE g_physdamage.value
#define		VEHICLE_PROP_IMPACT g_physimpact.value
#endif

//entity info
#define MAX_ENTITYINFO 	4

//objects properties
#define OBJ_DEFAULT					0
#define OBJ_SANDBOX					1
#define OBJ_EDITOR					2

//npc properties
#define NP_CHAT						1
#define NP_CHATLISTEN				2
#define NP_ATTACK					3
#define NP_CONTROL					4
#define NP_SCORE					5
#define NP_GOAL						6
#define NP_JUMP						7
#define NP_PICKUP					8
#define NP_HARM						9

//object type
#define OT_VANILLAQ3				0
#define OT_BASIC					1
#define OT_VEHICLE					2
#define OT_TNT						3
#define OT_NUKE						4

//phys type
#define PHYS_STATIC					1
#define PHYS_DYNAMIC				2

//phys check type
#define PHYSCHECK_SOLID				0
#define PHYSCHECK_INAIR				1

//angles2 storage
#define A2_MASS						0
//#define A2_UNUSED					1
//#define A2_UNUSED					2

//factions
#define NPC_PLAYER					0
#define NPC_ENEMY					1
#define NPC_CITIZEN					2
#define NPC_GUARD					3
#define NPC_PARTNER					4
#define NPC_PARTNERENEMY			5

//tools
#define TL_CREATE					0
#define TL_MATERIAL					1
#define TL_DELETE					2
#define TL_MODEL					3
#define TL_PHYSICS					4
#define TL_PRIVATE					5
#define TL_COLLISION				6
#define TL_HEALTH					7
#define TL_COLOR					8
#define TL_ANGLE					9
#define TL_SCALE					10
#define TL_REPLACEITEM				11
#define TL_COUNT					12
#define TL_WELD						13
#define TL_BIND						14

//undo
#define UNDO_PROPSPAWN				1
#define UNDO_NPCSPAWN				2

// CS_SERVERINFO and CS_SYSTEMINFO are defined in q_shared.h
#define	CS_MUSIC				2
#define	CS_WARMUP				3
#define	CS_SCORES1				4
#define	CS_SCORES2				5
#define CS_VOTE_TIME			6
#define CS_VOTE_STRING			7
#define	CS_VOTE_YES				8
#define	CS_VOTE_NO				9
#define CS_TEAMVOTE_TIME		10 // 2 slots
#define CS_TEAMVOTE_STRING		12 // 2 slots
#define	CS_TEAMVOTE_YES			14 // 2 slots
#define	CS_TEAMVOTE_NO			16 // 2 slots
#define	CS_GAME_VERSION			18
#define	CS_LEVEL_START_TIME		19
#define	CS_INTERMISSION			20
#define CS_FLAGSTATUS			21
#define CS_SHADERSTATE			22
#define CS_BOTINFO				23 // 2 slots
#define	CS_ITEMS				25
#define CS_PRIMARYOBJECTIVE		26
#define CS_SECONDARYOBJECTIVE	27
#define	CS_OVERLAY				28
#define CS_SCOREBOARDMUSIC		29
#define CS_DEATHMUSIC			30
#define CS_CUTSCENE				31
#define CS_PLAYERMODEL			32
#define CS_PLAYERHEADMODEL		33
#define CS_OBJECTIVESOVERLAY	34
#define	CS_MODELS				35
#define	CS_SOUNDS				(CS_MODELS+MAX_MODELS)
#define	CS_PLAYERS				(CS_SOUNDS+MAX_SOUNDS)
#define CS_LOCATIONS			(CS_PLAYERS+MAX_CLIENTS)
#define CS_MAX					(CS_PLAYERS+MAX_CLIENTS)

#if (CS_MAX) > MAX_CONFIGSTRINGS
#error overflow: (CS_MAX) > MAX_CONFIGSTRINGS
#endif

typedef enum {
	GT_SANDBOX,
	GT_MAPEDITOR,
	GT_SINGLE, //SHOULD BE 3 FOR ENGINE 
	GT_FFA,
	GT_TOURNAMENT,
	GT_TEAM,
	GT_CTF,
	GT_1FCTF,
	GT_OBELISK,
	GT_HARVESTER,	
	GT_ELIMINATION,
	GT_CTF_ELIMINATION,
	GT_LMS,
	GT_DOUBLE_D,
	GT_DOMINATION,
	GT_MAX_GAME_TYPE
} gametype_t;

typedef enum { GENDER_MALE, GENDER_FEMALE, GENDER_NEUTER } gender_t;

/*
===================================================================================

PMOVE MODULE

The pmove code takes a player_state_t and a usercmd_t and generates a new player_state_t
and some other output data.  Used for local prediction on the client game and true
movement on the server game.
===================================================================================
*/

typedef enum {
	PM_NORMAL,		// can accelerate and turn
	PM_NOCLIP,		// noclip movement
	PM_SPECTATOR,	// still run into walls
	PM_DEAD,		// no acceleration or turning, but free falling
	PM_FREEZE,		// stuck in place with no control
	PM_INTERMISSION,	// no movement or status bar
	PM_SPINTERMISSION,	// no movement or status bar
	PM_CUTSCENE		// stuck in place with no control and fixed view angles
} pmtype_t;

typedef enum {
	WEAPON_READY, 
	WEAPON_RAISING,
	WEAPON_DROPPING,
	WEAPON_FIRING
} weaponstate_t;

// pmove->pm_flags
#define	PMF_DUCKED			1
#define	PMF_JUMP_HELD		2
#define	PMF_BACKWARDS_JUMP	8		// go into backwards land
#define	PMF_BACKWARDS_RUN	16		// coast down to backwards run
#define	PMF_TIME_LAND		32		// pm_time is time before rejump
#define	PMF_TIME_KNOCKBACK	64		// pm_time is an air-accelerate only time
#define	PMF_TIME_WATERJUMP	256		// pm_time is waterjump
#define	PMF_RESPAWNED		512		// clear after attack and jump buttons come up
#define	PMF_USE_ITEM_HELD	1024
#define PMF_GRAPPLE_PULL	2048	// pull towards grapple location
#define PMF_FOLLOW			4096	// spectate following another player
#define PMF_SCOREBOARD		8192	// spectate as a scoreboard
#define PMF_INVULEXPAND		16384	// invulnerability sphere set to full size
//Elimination players cannot fire in warmup
#define PMF_ELIMWARMUP		32768	//I hope this is more than 16 signed bits! (it's not but it just works anyway...)
//Don't add anymore, I have already set the sign bit :-(
#define PMF_FORCE_WALK		65536	// bot always walks 
#define PMF_ATTACK_RUN		131072	// bot is allowed to run when attacking

#define	PMF_ALL_TIMES	(PMF_TIME_WATERJUMP|PMF_TIME_LAND|PMF_TIME_KNOCKBACK)

#define	MAXTOUCH	32
typedef struct {
	// state (in / out)
	playerState_t	*ps;
	entityState_t	*s;

	// command (in)
	usercmd_t	cmd;
	int			tracemask;			// collide against these types of surfaces
	qboolean	gauntletHit;		// true if a gauntlet attack would actually hit something

	int			framecount;

	// results (out)
	int			numtouch;
	int			touchents[MAXTOUCH];

	vec3_t		mins, maxs;			// bounding box size

	int			watertype;
	int			waterlevel;

	float		xyspeed;

	// callbacks to test the world
	// these will be different functions during game and cgame
	void		(*trace)( trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentMask );
	int			(*pointcontents)( const vec3_t point, int passEntityNum );
} pmove_t;

// if a full pmove isn't done on the client, you can just update the angles
void PM_UpdateViewAngles( playerState_t *ps, const usercmd_t *cmd );
void Pmove (pmove_t *pmove);

// player_state->stats[] indexes
// NOTE: may not have more than 16
typedef enum {
	STAT_HEALTH,
	STAT_HOLDABLE_ITEM,
	STAT_PERSISTANT_POWERUP,
	STAT_WEAPONS,					// 16 bit fields
	STAT_ARMOR,				
	STAT_DEAD_YAW,					// look this direction when dead (FIXME: get rid of?)
	STAT_CLIENTS_READY,				// bit mask of clients wishing to exit the intermission (FIXME: configstring?)
	STAT_MAX_HEALTH,				// health / armor limit
	STAT_NO_PICKUP,					// for dropped ammo
	STAT_MONEY,						// for buy activator funcs
	STAT_SWEPAMMO,					// for SWEP WEAPONS ammo
	STAT_VEHICLE,					// for vehicle system
	STAT_VEHICLEHP					// for vehicle system
} statIndex_t;


// player_state->persistant[] indexes
// these fields are the only part of player_state that isn't
// cleared on respawn
// NOTE: may not have more than 16
typedef enum {
	PERS_SCORE,						// !!! MUST NOT CHANGE, SERVER AND GAME BOTH REFERENCE !!!
	PERS_HITS,						// total number of hits inflicted so damage beeps can sound on change
	PERS_RANK,						// player rank or team rank
	PERS_TEAM,						// player team
	PERS_SPAWN_COUNT,				// incremented every respawn
	PERS_PLAYEREVENTS,				// 16 bits that can be flipped for events
	PERS_ATTACKER,					// clientnum of last damage inflicter
	PERS_ATTACKEE_ARMOR,			// health/armor of last person we attacked
	PERS_KILLED,					// count of the number of times you died
	// player awards tracking
	PERS_IMPRESSIVE_COUNT,			// two railgun hits in a row
	PERS_EXCELLENT_COUNT,			// two successive kills in a short amount of time
	PERS_DEFEND_COUNT,				// defend awards
	PERS_ASSIST_COUNT,				// assist awards
	PERS_GAUNTLET_FRAG_COUNT,		// kills with the guantlet
	PERS_CAPTURES,					// captures
	// entityplus
	PERS_SECRETS					// first 7 bits is number of secrets found, 7 bits after that is total number of secrets in level(s)
} persEnum_t;


// entityState_t->eFlags
#define	EF_DEAD				0x00000001		// don't draw a foe marker over players with EF_DEAD
#define EF_TICKING			0x00000002		// used to make players play the prox mine ticking sound
#define	EF_TELEPORT_BIT		0x00000004		// toggled every time the origin abruptly changes
#define	EF_AWARD_EXCELLENT	0x00000008		// draw an excellent sprite
#define EF_PLAYER_EVENT		0x00000010
#define	EF_BOUNCE			0x00000010		// for missiles
#define	EF_BOUNCE_HALF		0x00000020		// for missiles
#define	EF_AWARD_GAUNTLET	0x00000040		// draw a gauntlet sprite
#define	EF_NODRAW			0x00000080		// may have an event, but no model (unspawned items)
#define	EF_FIRING			0x00000100		// for lightning gun
#define	EF_KAMIKAZE			0x00000200
#define	EF_MOVER_STOP		0x00000400		// will push otherwise
#define EF_AWARD_CAP		0x00000800		// draw the capture sprite
#define	EF_TALK				0x00001000		// draw a talk balloon
#define	EF_CONNECTION		0x00002000		// draw a connection trouble sprite
#define	EF_VOTED			0x00004000		// already cast a vote
#define	EF_AWARD_IMPRESSIVE	0x00008000		// draw an impressive sprite
#define	EF_AWARD_DEFEND		0x00010000		// draw a defend sprite
#define	EF_AWARD_ASSIST		0x00020000		// draw a assist sprite
#define EF_AWARD_DENIED		0x00040000		// denied
#define EF_TEAMVOTED		0x00080000		// already cast a team vote
#define EF_HEARED           0x00100000      // heard gunfire or sounds

// NOTE: may not have more than 16
typedef enum {
	PW_NONE,

	PW_QUAD,
	PW_BATTLESUIT,
	PW_HASTE,
	PW_INVIS,
	PW_REGEN,
	PW_FLIGHT,

	PW_REDFLAG,
	PW_BLUEFLAG,
	PW_NEUTRALFLAG,

	PW_SCOUT,
	PW_GUARD,
	PW_DOUBLER,
	PW_AMMOREGEN,
	PW_INVULNERABILITY,

	PW_NUM_POWERUPS

} powerup_t;

typedef enum {
	HI_NONE,

	//can only hold one of the items below. Adding items here requires editing of GetPlayerHoldable() in g_utils
	HI_TELEPORTER,
	HI_MEDKIT,
	HI_KAMIKAZE,
	HI_PORTAL,
	HI_INVULNERABILITY,

	HI_HOLDABLE_SPLIT,	//not an actual item, only a marker to differentiate between different types of holdables. Do not 'give' this to players.
	
	//can hold multiple of the items below
	HI_KEY_RED,
	HI_KEY_GREEN,
	HI_KEY_BLUE,
	HI_KEY_YELLOW,
	HI_KEY_MASTER,
	HI_KEY_GOLD,
	HI_KEY_SILVER,
	HI_KEY_IRON,

	HI_NUM_HOLDABLE
} holdable_t;

typedef enum {
	DD_NONE,

	DD_POINTARED,
	DD_POINTABLUE,
	DD_POINTAWHITE,
	DD_POINTBRED,
	DD_POINTBBLUE,
	DD_POINTBWHITE
} doubled_t;

typedef enum {
	DOM_NONE,

	DOM_POINTRED,
	DOM_POINTBLUE,
	DOM_POINTWHITE,
} domination_t;


typedef enum {
	WP_NONE,

	WP_GAUNTLET,
	WP_MACHINEGUN,
	WP_SHOTGUN,
	WP_GRENADE_LAUNCHER,
	WP_ROCKET_LAUNCHER,
	WP_LIGHTNING,
	WP_RAILGUN,
	WP_PLASMAGUN,
	WP_BFG,
	WP_GRAPPLING_HOOK,
	WP_NAILGUN,
	WP_PROX_LAUNCHER,
	WP_CHAINGUN,
	WP_FLAMETHROWER,
	WP_ANTIMATTER, //Vanilla q3 set

	// New weapons here!
	WP_THROWER,
	WP_BOUNCER,
	WP_THUNDER,
	WP_EXPLODER,
	WP_KNOCKER,
	WP_PROPGUN,
	WP_REGENERATOR,
	WP_NUKE,

	WP_PHYSGUN,
	WP_GRAVITYGUN,
	WP_TOOLGUN, //Sandbox set
	
	WEAPONS_NUM		//look for this to add new ones - WEAPONS_HYPER
} weapon_t;	

// reward sounds (stored in ps->persistant[PERS_PLAYEREVENTS])
#define	PLAYEREVENT_DENIEDREWARD		0x0001
#define	PLAYEREVENT_GAUNTLETREWARD		0x0002
#define PLAYEREVENT_HOLYSHIT			0x0004

// entityState_t->event values
// entity events are for effects that take place reletive
// to an existing entities origin.  Very network efficient.

// two bits at the top of the entityState->event field
// will be incremented with each change in the event so
// that an identical event started twice in a row can
// be distinguished.  And off the value with ~EV_EVENT_BITS
// to retrieve the actual event number
#define	EV_EVENT_BIT1		0x00000100
#define	EV_EVENT_BIT2		0x00000200
#define	EV_EVENT_BITS		(EV_EVENT_BIT1|EV_EVENT_BIT2)

#define	EVENT_VALID_MSEC	300

typedef enum {
	EV_NONE,

	EV_FOOTSTEP,
	EV_FOOTSTEP_METAL,
	EV_FOOTSPLASH,
	EV_FOOTWADE,
	EV_SWIM,

	EV_STEP_4,
	EV_STEP_8,
	EV_STEP_12,
	EV_STEP_16,

	EV_FALL_SHORT,
	EV_FALL_MEDIUM,
	EV_FALL_FAR,

	EV_JUMP_PAD,			// boing sound at origin, jump sound on player

	EV_JUMP,                        //Event 14
	EV_WATER_TOUCH,	// foot touches
	EV_WATER_LEAVE,	// foot leaves
	EV_WATER_UNDER,	// head touches
	EV_WATER_CLEAR,	// head leaves

	EV_ITEM_PICKUP,			// normal item pickups are predictable
	EV_GLOBAL_ITEM_PICKUP,	// powerup / team sounds are broadcast to everyone

	EV_NOAMMO,
	EV_CHANGE_WEAPON,
	EV_FIRE_WEAPON,

	EV_USE_ITEM0,                   //Event 24
	EV_USE_ITEM1,
	EV_USE_ITEM2,
	EV_USE_ITEM3,
	EV_USE_ITEM4,
	EV_USE_ITEM5,
	EV_USE_ITEM6,
	EV_USE_ITEM7,
	EV_USE_ITEM8,
	EV_USE_ITEM9,
	EV_USE_ITEM10,
	EV_USE_ITEM11,
	EV_USE_ITEM12,
	EV_USE_ITEM13,
	EV_USE_ITEM14,
	EV_USE_ITEM15,

	EV_ITEM_RESPAWN,                //Event 40
	EV_ITEM_POP,
	EV_PLAYER_TELEPORT_IN,
	EV_PLAYER_TELEPORT_OUT,

	EV_GRENADE_BOUNCE,		// eventParm will be the soundindex

	EV_GENERAL_SOUND,
	EV_GLOBAL_SOUND,		// no attenuation
	EV_GLOBAL_TEAM_SOUND,

	EV_BULLET_HIT_FLESH,
	EV_BULLET_HIT_WALL,

	EV_MISSILE_HIT,                 //Event 50
	EV_MISSILE_MISS,
	EV_MISSILE_MISS_METAL,
	EV_RAILTRAIL,
	EV_SHOTGUN,
	EV_BULLET,				// otherEntity is the shooter

	EV_PAIN,
	EV_PAINVEHICLE,
	EV_DEATH1,
	EV_DEATH2,
	EV_DEATH3,
	EV_OBITUARY,                    //Event 60

	EV_POWERUP_QUAD,
	EV_POWERUP_BATTLESUIT,
	EV_POWERUP_REGEN,

	EV_GIB_PLAYER,			// gib a previously living player
	EV_SCOREPLUM,			// score plum

	EV_PROXIMITY_MINE_STICK,
	EV_PROXIMITY_MINE_TRIGGER,
	EV_KAMIKAZE,			// kamikaze explodes
	EV_OBELISKEXPLODE,		// obelisk explodes
	EV_OBELISKPAIN,			// obelisk is in pain
	EV_INVUL_IMPACT,		// invulnerability sphere impact
	EV_JUICED,				// invulnerability juiced effect
	EV_LIGHTNINGBOLT,		// lightning bolt bounced of invulnerability sphere

	EV_DEBUG_LINE,
	EV_STOPLOOPINGSOUND,
	EV_TAUNT,
	EV_TAUNT_YES,
	EV_TAUNT_NO,
	EV_TAUNT_FOLLOWME,
	EV_TAUNT_GETFLAG,
	EV_TAUNT_GUARDBASE,
	EV_TAUNT_PATROL,

	EV_EMIT_DEBRIS_LIGHT,		// emit light concrete chunks
	EV_EMIT_DEBRIS_DARK,		// emit dark concrete chunks
	EV_EMIT_DEBRIS_LIGHT_LARGE,	// emit light large concrete chunks
	EV_EMIT_DEBRIS_DARK_LARGE,	// emit dark large concrete chunks
	EV_EMIT_DEBRIS_WOOD,		// emit wooden chunks
	EV_EMIT_DEBRIS_FLESH,		// emit gibs
	EV_EMIT_DEBRIS_GLASS,		// emite shards of glass
	EV_EMIT_DEBRIS_STONE,		// emit chunks of stone

	EV_EARTHQUAKE,
	EV_EXPLOSION,
	EV_PARTICLES_GRAVITY,
	EV_PARTICLES_LINEAR,
	EV_PARTICLES_LINEAR_UP,
	EV_PARTICLES_LINEAR_DOWN,
	EV_OVERLAY,
	EV_SMOKEPUFF,
	EV_WATERPUFF,
	
	EV_SILENT_ITEM_PICKUP,		// item pickup without pickup sound

	EV_FOOTSTEP_FLESH,
	
	EV_HORN,		//VEHICLES
	EV_CRASH25,
	EV_OT1_IMPACT,
	EV_GRAVITYSOUND

} entity_event_t;


typedef enum {
	GTS_RED_CAPTURE,
	GTS_BLUE_CAPTURE,
	GTS_RED_RETURN,
	GTS_BLUE_RETURN,
	GTS_RED_TAKEN,
	GTS_BLUE_TAKEN,
	GTS_REDOBELISK_ATTACKED,
	GTS_BLUEOBELISK_ATTACKED,
	GTS_REDTEAM_SCORED,
	GTS_BLUETEAM_SCORED,
	GTS_REDTEAM_TOOK_LEAD,
	GTS_BLUETEAM_TOOK_LEAD,
	GTS_TEAMS_ARE_TIED,
	GTS_KAMIKAZE
} global_team_sound_t;

// animations
typedef enum {
	BOTH_DEATH1,
	BOTH_DEAD1,
	BOTH_DEATH2,
	BOTH_DEAD2,
	BOTH_DEATH3,
	BOTH_DEAD3,

	TORSO_GESTURE,

	TORSO_ATTACK,
	TORSO_ATTACK2,

	TORSO_DROP,
	TORSO_RAISE,

	TORSO_STAND,
	TORSO_STAND2,

	LEGS_WALKCR,
	LEGS_WALK,
	LEGS_RUN,
	LEGS_BACK,
	LEGS_SWIM,

	LEGS_JUMP,
	LEGS_LAND,

	LEGS_JUMPB,
	LEGS_LANDB,

	LEGS_IDLE,
	LEGS_IDLECR,

	LEGS_TURN,

	TORSO_GETFLAG,
	TORSO_GUARDBASE,
	TORSO_PATROL,
	TORSO_FOLLOWME,
	TORSO_AFFIRMATIVE,
	TORSO_NEGATIVE,

	MAX_ANIMATIONS,

	LEGS_BACKCR,
	LEGS_BACKWALK,
	FLAG_RUN,
	FLAG_STAND,
	FLAG_STAND2RUN,

	MAX_TOTALANIMATIONS
} animNumber_t;


typedef struct animation_s {
	int		firstFrame;
	int		numFrames;
	int		loopFrames;			// 0 to numFrames
	int		frameLerp;			// msec between frames
	int		initialLerp;		// msec to get to first frame
	int		reversed;			// true if animation is reversed
	int		flipflop;			// true if animation should flipflop back to base
} animation_t;


// flip the togglebit every time an animation
// changes so a restart of the same anim can be detected
#define	ANIM_TOGGLEBIT		128


typedef enum {
	TEAM_FREE,
	TEAM_RED,
	TEAM_BLUE,
	TEAM_SPECTATOR,

	TEAM_NUM_TEAMS
} team_t;

typedef enum {
	VCLASS_NONE,
	VCLASS_CAR,

	VCLASS_NUM
} vehclass_t;

typedef enum {
	VSET_SPEED,
	VSET_GRAVITY,
	VSET_WEAPON,
	VSET_WEAPONRATE,

	VSET_NUM
} vehsetting_t;

// This is a fair assumption for Double Domination:
#define TEAM_NONE TEAM_SPECTATOR

// Time between location updates
#define TEAM_LOCATION_UPDATE_TIME		1000

// How many players on the overlay
#define TEAM_MAXOVERLAY		32

//team task
typedef enum {
	TEAMTASK_NONE,
	TEAMTASK_OFFENSE, 
	TEAMTASK_DEFENSE,
	TEAMTASK_PATROL,
	TEAMTASK_FOLLOW,
	TEAMTASK_RETRIEVE,
	TEAMTASK_ESCORT,
	TEAMTASK_CAMP
} teamtask_t;

// means of death
typedef enum {
	MOD_UNKNOWN,
	MOD_SHOTGUN,
	MOD_GAUNTLET,
	MOD_MACHINEGUN,
	MOD_GRENADE,
	MOD_GRENADE_SPLASH,
	MOD_ROCKET,
	MOD_ROCKET_SPLASH,
	MOD_PLASMA,
	MOD_PLASMA_SPLASH,
	MOD_RAILGUN,
	MOD_LIGHTNING,
	MOD_BFG,
	MOD_BFG_SPLASH,
	MOD_FLAME,
	MOD_FLAME_SPLASH,
	MOD_ANTIMATTER,
	MOD_ANTIMATTER_SPLASH,
	MOD_TOOLGUN,
	MOD_WATER,
	MOD_SLIME,
	MOD_LAVA,
	MOD_CRUSH,
	MOD_TELEFRAG,
	MOD_FALLING,
	MOD_SUICIDE,
	MOD_TARGET_LASER,
	MOD_TRIGGER_HURT,
	MOD_NAIL,
	MOD_CHAINGUN,
	MOD_PROXIMITY_MINE,
	MOD_KAMIKAZE,
	MOD_JUICED,
	MOD_GRAPPLE,
	MOD_CAR,
	MOD_CAREXPLODE,
	MOD_PROP,
	MOD_SWEP,
	MOD_KNOCKER,
	MOD_REGENERATOR,
	MOD_NUKE,
	MOD_BREAKABLE_SPLASH
} meansOfDeath_t;


//---------------------------------------------------------

// gitem_t->type
typedef enum {
	IT_BAD,
	IT_WEAPON,				// EFX: rotate + upscale + minlight
	IT_AMMO,				// EFX: rotate
	IT_ARMOR,				// EFX: rotate + minlight
	IT_HEALTH,				// EFX: static external sphere + rotating internal
	IT_POWERUP,				// instant on, timer based
							// EFX: rotate + external ring that rotates
	IT_HOLDABLE,			// single use, holdable item
							// EFX: rotate + bob
	IT_PERSISTANT_POWERUP,
	IT_TEAM,
	IT_BACKPACK				// Backpack dropped by player when dying in SP mode
} itemType_t;

#define MAX_ITEM_MODELS 4

typedef struct gitem_s {
	char		*classname;	// spawning name
	char		*pickup_sound;
	char		*world_model[MAX_ITEM_MODELS];

	char		*icon;
	char		*pickup_name;	// for printing on pickup
	char		*pickup_nameru;	// for printing on pickup

	int			quantity;		// for ammo how much, or duration of powerup
	itemType_t  giType;			// IT_* flags

	int			giTag;

	char		*precaches;		// string of all models and images this item will use
	char		*sounds;		// string of all sounds this item will use
} gitem_t;

// included in both the game dll and the client
extern	gitem_t	bg_itemlist[];
extern	int		bg_numItems;

gitem_t	*BG_FindItem( const char *pickupName );
gitem_t	*BG_FindSwep( int id );
gitem_t	*BG_FindSwepAmmo( int id );
gitem_t	*BG_FindItemForWeapon( weapon_t weapon );
gitem_t	*BG_FindItemForPowerup( powerup_t pw );
gitem_t	*BG_FindItemForHoldable( holdable_t pw );
gitem_t	*BG_FindItemForBackpack( );
#define	ITEM_INDEX(x) ((x)-bg_itemlist)

qboolean	BG_CanItemBeGrabbed( int gametype, const entityState_t *ent, const playerState_t *ps );

// g_elimflags->integer
//This is used to signal the client that it cannot go to free spectator:
#define EF_ONEWAY                       1
#define EF_NO_FREESPEC                  2

// content masks
#define	MASK_ALL				(-1)
#define	MASK_SOLID				(CONTENTS_SOLID)
#define	MASK_SELECT				(CONTENTS_SOLID|CONTENTS_BODY|CONTENTS_CORPSE|CONTENTS_TRIGGER)
#define	MASK_OBJECTS			(CONTENTS_SOLID|CONTENTS_BODY|CONTENTS_CORPSE)
#define	MASK_PLAYERSOLID		(CONTENTS_SOLID|CONTENTS_PLAYERCLIP|CONTENTS_BODY)
#define	MASK_DEADSOLID			(CONTENTS_SOLID|CONTENTS_PLAYERCLIP)
#define	MASK_WATER				(CONTENTS_WATER|CONTENTS_LAVA|CONTENTS_SLIME)
#define	MASK_OPAQUE				(CONTENTS_SOLID|CONTENTS_SLIME|CONTENTS_LAVA)
#define	MASK_SHOT				(CONTENTS_SOLID|CONTENTS_BODY|CONTENTS_CORPSE)
#define	MASK_NOCSHOT			(CONTENTS_BODY|CONTENTS_CORPSE)

//
// entityState_t->eType
//
typedef enum {
	ET_GENERAL,
	ET_PLAYER,
	ET_ITEM,
	ET_MISSILE,
	ET_MOVER,
	ET_BEAM,
	ET_PORTAL,
	ET_SPEAKER,
	ET_PUSH_TRIGGER,
	ET_TELEPORT_TRIGGER,
	ET_INVISIBLE,
	ET_GRAPPLE,				// grapple hooked on wall
	ET_LASER,				// lasersight entity type
	ET_TEAM,
    ET_WEATHER,             // used to specify per area weather

	ET_EVENTS				// any of the EV_* events can be added freestanding
							// by setting eType to ET_EVENTS + eventNum
							// this avoids having to set eFlags and eventNum
} entityType_t;

//KK-OAX Using this now instead of g_mem.c
// bg_alloc.c
//
qboolean BG_CanAlloc( unsigned int size );
void    *BG_Alloc( unsigned int size );
void    BG_InitMemory( void );
void    BG_Free( void *ptr );
void    BG_DefragmentMemory( void );


void	BG_EvaluateTrajectory( const trajectory_t *tr, int atTime, vec3_t result );
void	BG_EvaluateTrajectoryDelta( const trajectory_t *tr, int atTime, vec3_t result );

void	ST_EvaluateTrajectory( const trajectory_t *tr, int atTime, vec3_t result, float mass );
void	ST_EvaluateTrajectoryDelta( const trajectory_t *tr, int atTime, vec3_t result, float mass );

void	BG_AddPredictableEventToPlayerstate( int newEvent, int eventParm, playerState_t *ps );

void	BG_TouchJumpPad( playerState_t *ps, entityState_t *jumppad, qboolean playsound );

void	BG_PlayerStateToEntityState( playerState_t *ps, entityState_t *s, qboolean snap );
void	BG_PlayerStateToEntityStateExtraPolate( playerState_t *ps, entityState_t *s, int time, qboolean snap );

qboolean	BG_PlayerTouchesItem( playerState_t *ps, entityState_t *item, int atTime );

int BG_VehicleCheckClass (int id);
float BG_GetVehicleSettings (int id, int set);


#define ARENAS_PER_TIER		4
#define MAX_ARENAS			8192
#define	MAX_ARENAS_TEXT		25000

#define MAX_BOTS			8192
#define MAX_BOTS_TEXT		25000


// Kamikaze

// 1st shockwave times
#define KAMI_SHOCKWAVE_STARTTIME		0
#define KAMI_SHOCKWAVEFADE_STARTTIME	1500
#define KAMI_SHOCKWAVE_ENDTIME			2000
// explosion/implosion times
#define KAMI_EXPLODE_STARTTIME			250
#define KAMI_IMPLODE_STARTTIME			2000
#define KAMI_IMPLODE_ENDTIME			2250
// 2nd shockwave times
#define KAMI_SHOCKWAVE2_STARTTIME		2000
#define KAMI_SHOCKWAVE2FADE_STARTTIME	2500
#define KAMI_SHOCKWAVE2_ENDTIME			3000
// radius of the models without scaling
#define KAMI_SHOCKWAVEMODEL_RADIUS		88
#define KAMI_BOOMSPHEREMODEL_RADIUS		72
// maximum radius of the models during the effect
#define KAMI_SHOCKWAVE_MAXRADIUS		1320
#define KAMI_BOOMSPHERE_MAXRADIUS		720
#define KAMI_SHOCKWAVE2_MAXRADIUS		704

//KK-OAX
//bg_misc.c
char *BG_TeamName( team_t team );

#endif
