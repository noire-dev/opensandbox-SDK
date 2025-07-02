// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

#if defined(BG_PUBLIC_H)
#else
#define BG_PUBLIC_H 1

#define	GIB_HEALTH			-66
#define	MAX_ITEMS			256
#define	RANK_TIED_FLAG		0x4000
#define	ITEM_RADIUS			15
#define	SCORE_NOT_PRESENT	-9999	// for the CS_SCORES[12] when only one player is present
#define	MINS_Z				-24
#define	DEFAULT_VIEWHEIGHT	26
#define CROUCH_VIEWHEIGHT	12
#define	DEAD_VIEWHEIGHT		-16

// physics engine
#define		PHYS_ROTATING 0.020
#define		PHYS_DAMAGESENS 30
#define		VEHICLE_SENS 30
#define		VEHICLE_DAMAGE 0.05
#define		VEHICLE_DAMAGESENS 30

#ifdef GAME
#define		PHYS_PROP_IMPACT 	0.80
#define		PHYS_SENS 			450
#define		PHYS_DAMAGE 		0.60
#endif

//entity info
#define MAX_ENTITYINFO 	4

//weapon status
#define WS_NONE						0
#define WS_HAVE						1
#define WS_NOAMMO					2

typedef enum {
	OT_VANILLAQ3,
	OT_BASIC,
	OT_VEHICLE,
	OT_TNT,
	OT_NUKE,
	
	OBJECTTYPES_NUM
} objectType_t;

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

//tools
#define TL_CREATE					0
#define TL_MATERIAL					1
#define TL_DELETE					2
#define TL_PHYSICS					3
#define TL_PRIVATE					4
#define TL_COLLISION				5
#define TL_HEALTH					6
#define TL_COLOR					7
#define TL_ANGLE					8
#define TL_SCALE					9
#define TL_REPLACEITEM				10
#define TL_COUNT					11
#define TL_WELD						12
#define TL_BIND						13

// CS_SERVERINFO and CS_SYSTEMINFO are defined in q_shared.h
#define	CS_MUSIC				2
#define	CS_SCORES1				3
#define	CS_SCORES2				4
#define	CS_LEVEL_START_TIME		5
#define	CS_INTERMISSION			6
#define CS_FLAGSTATUS			7
#define CS_BOTINFO				8 // 2 slots
#define	CS_MODELS				MAX_CUSTOMSTRINGS
#define	CS_SOUNDS				(CS_MODELS+MAX_MODELS)
#define	CS_PLAYERS				(CS_SOUNDS+MAX_SOUNDS)
#define CS_LOCATIONS			(CS_PLAYERS+MAX_CLIENTS)
#define CS_MAX					(CS_LOCATIONS+MAX_LOCATIONS)

#if (CS_MAX) > MAX_CONFIGSTRINGS
#error overflow: (CS_MAX) > MAX_CONFIGSTRINGS
#endif

typedef enum {
	//FFA GAMEMODES
	GT_SANDBOX,
	GT_FFA,
	//TEAM GAMEMODES
	GT_TEAM,
	GT_CTF,
	GT_1FCTF,
	GT_OBELISK,
	GT_HARVESTER,	

	GT_MAX_GAME_TYPE
} gametype_t;

extern char* gametypes_names[];
extern char* gametypes_mapnames[];

typedef enum { 
	GENDER_MALE, 
	GENDER_FEMALE, 
	GENDER_NEUTER
} gender_t;

typedef enum {
	IT_NULL,
	IT_WEAPON,
	IT_AMMO,
	IT_ARMOR,
	IT_HEALTH,
	IT_POWERUP,
	IT_HOLDABLE,
	IT_RUNE,
	IT_TEAM,
} itemType_t;

typedef struct item_s {
	char			*classname;
	char			*world_model;
	char			*icon;
	char			*pickup_name;
	int				quantity;
	itemType_t  	giType;
	int				giTag;
} item_t;

extern item_t	gameInfoItems[];
extern int		gameInfoItemsNum;

typedef enum {
	PM_NORMAL,		// can accelerate and turn
	PM_NOCLIP,		// noclip movement
	PM_SPECTATOR,	// still run into walls
	PM_DEAD,		// no acceleration or turning, but free falling
	PM_FREEZE,		// stuck in place with no control
	PM_INTERMISSION,	// no movement or status bar
	PM_SPINTERMISSION,	// no movement or status bar
} pmtype_t;

typedef enum {
	WEAPON_READY, 
	WEAPON_RAISING,
	WEAPON_DROPPING,
	WEAPON_FIRING,
} weaponstate_t;

#define	PMF_DUCKED			1
#define	PMF_JUMP_HELD		2
#define	PMF_BACKWARDS_JUMP	4		// go into backwards land
#define	PMF_BACKWARDS_RUN	8		// coast down to backwards run
#define	PMF_TIME_LAND		16		// pm_time is time before rejump
#define	PMF_TIME_KNOCKBACK	32		// pm_time is an air-accelerate only time
#define	PMF_TIME_WATERJUMP	64		// pm_time is waterjump
#define	PMF_RESPAWNED		128		// clear after attack and jump buttons come up
#define	PMF_USE_ITEM_HELD	256
#define PMF_GRAPPLE_PULL	512		// pull towards grapple location
#define PMF_FOLLOW			1024	// spectate following another player
#define PMF_SCOREBOARD		2048	// spectate as a scoreboard
#define PMF_INVULEXPAND		4096	// invulnerability sphere set to full size
#define	PMF_ALL_TIMES		(PMF_TIME_WATERJUMP|PMF_TIME_LAND|PMF_TIME_KNOCKBACK)

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

typedef enum {
	STAT_HEALTH,
	STAT_ARMOR,
	STAT_AMMO,
	STAT_MAX_HEALTH,
	STAT_HOLDABLE_ITEM,
	STAT_PERSISTANT_POWERUP,
	STAT_VEHICLE,
	STAT_VEHICLEHP,
} statIndex_t;

typedef enum {
	PERS_SCORE,						// !!! MUST NOT CHANGE, SERVER AND GAME BOTH REFERENCE !!!
	PERS_HITS,						// total number of hits inflicted so damage beeps can sound on change
	PERS_RANK,						// player rank or team rank
	PERS_TEAM,						// player team
	PERS_SPAWN_COUNT,				// incremented every respawn
	PERS_ATTACKEE_ARMOR,			// health/armor of last person we attacked
} persEnum_t;

#define	EF_DEAD				1			// don't draw a foe marker over players with EF_DEAD
#define EF_TICKING			2			// used to make players play the prox mine ticking sound
#define	EF_TELEPORT_BIT		4			// toggled every time the origin abruptly changes
#define EF_PLAYER_EVENT		8
#define	EF_BOUNCE			16			// for missiles
#define	EF_NODRAW			32			// may have an event, but no model (unspawned items)
#define	EF_FIRING			64			// for lightning gun
#define	EF_KAMIKAZE			128
#define	EF_TALK				256			// draw a talk balloon

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

	HI_TELEPORTER,
	HI_MEDKIT,
	HI_KAMIKAZE,
	HI_PORTAL,
	HI_INVULNERABILITY,

	HI_NUM_HOLDABLE
} holdable_t;

typedef enum {
	WP_NONE,

	// Quake weapons here!
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

	// New weapons here!
	WP_FLAMETHROWER,
	WP_ANTIMATTER,
	WP_THROWER,
	WP_BOUNCER,
	WP_THUNDER,
	WP_EXPLODER,
	WP_KNOCKER,
	WP_PROPGUN,
	WP_REGENERATOR,
	WP_NUKE,

	// Sandbox weapons here!
	WP_PHYSGUN,
	WP_GRAVITYGUN,
	WP_TOOLGUN,
	
	WEAPONS_NUM
} weapon_t;

typedef enum {
	WT_NONE,

	WT_MELEE,
	WT_BULLET,
	WT_SHOTGUN,
	WT_LIGHTNING,
	WT_RAILGUN,
	WT_EMPTY,
	WT_TOOLGUN,
	WT_MISSILE,
	
	WEAPONTYPES_NUM
} weaponType_t;

typedef enum {
	MT_NONE,

	MT_GENERAL,
	MT_HOOK,
	MT_MINE,

	MT_PROPS,

	MT_PROPGUN,
	MT_NUKE,
	
	MISSILETYPES_NUM
} missileType_t;

typedef struct weaponPropeties_s {
	weaponType_t	wType;
	missileType_t	mType;
	weapon_t		mEffect;
	char			*classname;
	int				delay;
	int				count;
	int				damage;
	int				splashDamage;
	int				splashRadius;
	int				range;
	int				mod;
	int				speed;
	int				speedRandom;
	int				spread;
	int				timeout;
	qboolean		guided;
	qboolean		gravity;
	qboolean		bounce;
	float			bounceModifier;
} weaponProperties_t;

extern	weaponProperties_t	gameInfoWeapons[];
extern	int					gameInfoWeaponsNum;

typedef struct wPropPropeties_s {
	objectType_t	oType;
	char			*modelname;
	int				mtMin;
	int				mtMax;
	int				health;
	float			gravity;
	float			scale;
	float			colSize;
	qboolean		solid;
} wPropProperties_t;

extern	wPropProperties_t	gameInfoWProps[];
extern	int					gameInfoWPropsNum;

typedef enum {
	NT_NONE,

	NT_PLAYER,
	NT_ENEMY,
	NT_CITIZEN,
	NT_GUARD,
	NT_PARTNER,
	NT_NEXTBOT,
	
	NPCTYPES_NUM
} NPCType_t;

typedef enum {
    FRAC_NONE,

    FRAC_PLAYER			= 1 << 0,
    FRAC_ENEMY			= 1 << 1,
    FRAC_CITIZEN		= 1 << 2,
    FRAC_GUARD			= 1 << 3,
    FRAC_PARTNER		= 1 << 4,
    FRAC_NEXTBOT		= 1 << 5,
} factionBits_t;

typedef struct NPCTypes_s {
	char			*name;
    factionBits_t 	faction;
	float			gravity;
	float			damage;
	float			speed;
	qboolean		dropItems;
    qboolean 		canPush;
    qboolean 		friendlyFire;
    qboolean 		canPickup;
    qboolean 		canChat;
} NPCTypes_t;

extern	NPCTypes_t			gameInfoNPCTypes[];
extern	int					gameInfoNPCTypesNum;

typedef struct NPCFactions_s {
    factionBits_t 	faction;
    unsigned int	attackMask;
} NPCFactions_t;

extern	NPCFactions_t		gameInfoFactions[];
extern	int					gameInfoFactionsNum;

extern	char*				gameInfoSandboxSpawns[];
extern	int					gameInfoSandboxSpawnsNum;

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
	EV_FOOTSTEP_FLESH,

	EV_FOOTSPLASH,

	EV_SWIM,

	EV_STEP_4,
	EV_STEP_8,
	EV_STEP_12,
	EV_STEP_16,

	EV_FALL_SHORT,
	EV_FALL_MEDIUM,
	EV_FALL_FAR,

	EV_JUMP_PAD,
	EV_JUMP,

	EV_WATER_TOUCH,
	EV_WATER_LEAVE,
	EV_WATER_UNDER,
	EV_WATER_CLEAR,

	EV_ITEM_PICKUP,
	EV_NOAMMO,
	EV_CHANGE_WEAPON,
	EV_FIRE_WEAPON,

	EV_USE_ITEM0,
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

	EV_ITEM_RESPAWN,
	EV_ITEM_POP,

	EV_PLAYER_TELEPORT_IN,
	EV_PLAYER_TELEPORT_OUT,

	EV_GRENADE_BOUNCE,

	EV_GENERAL_SOUND,
	EV_GLOBAL_SOUND,
	EV_GLOBAL_TEAM_SOUND,

	EV_BULLET_HIT_FLESH,
	EV_BULLET_HIT_WALL,

	EV_MISSILE_HIT,
	EV_MISSILE_MISS,
	EV_MISSILE_MISS_METAL,

	EV_RAILTRAIL,
	EV_SHOTGUN,
	EV_BULLET,

	EV_PAIN,
	EV_DEATH1,
	EV_DEATH2,
	EV_DEATH3,

	EV_OBITUARY,
	EV_GIB_PLAYER,

	EV_PROXIMITY_MINE_STICK,
	EV_PROXIMITY_MINE_TRIGGER,
	EV_KAMIKAZE,
	EV_OBELISKEXPLODE,
	EV_OBELISKPAIN,
	EV_INVUL_IMPACT,
	EV_JUICED,
	EV_LIGHTNINGBOLT,
	EV_STOPLOOPINGSOUND,
	EV_TAUNT,

	EV_EXPLOSION,
	EV_PARTICLES_GRAVITY,
	EV_PARTICLES_LINEAR,
	EV_PARTICLES_LINEAR_UP,
	EV_PARTICLES_LINEAR_DOWN,
	EV_SMOKEPUFF,
	EV_WATERPUFF,

	EV_HORN,
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

	MAX_ANIMATIONS,

	LEGS_BACKCR,		//automatic generated
	LEGS_BACKWALK,		//automatic generated

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

#define TEAM_NONE TEAM_SPECTATOR

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

typedef enum {
	MOD_UNKNOWN,
	MOD_WEAPONS = WEAPONS_NUM, // Weapons (check weapon_t)

	// Other
	MOD_WATER,
	MOD_SLIME,
	MOD_LAVA,
	MOD_CRUSH,
	MOD_TELEFRAG,
	MOD_FALLING,
	MOD_SUICIDE,
	MOD_TRIGGER_HURT,
	MOD_KAMIKAZE,
	MOD_JUICED,
	MOD_CAR,
	MOD_CAREXPLODE,
	MOD_PROP
} meansOfDeath_t;

#define	ITEM_INDEX(x) ((x)-gameInfoItems)

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

typedef enum {
	ET_GENERAL,
	ET_PLAYER,
	ET_ITEM,
	ET_MISSILE,
	ET_MOVER,
	ET_PORTAL,
	ET_SPEAKER,
	ET_PUSH_TRIGGER,
	ET_TELEPORT_TRIGGER,
	ET_INVISIBLE,
	ET_GRAPPLE,
	ET_TEAM,

	ET_EVENTS
} entityType_t;

#define MAX_ARENAS			8192
#define	MAX_ARENAS_TEXT		25000

#define MAX_BOTS			8192
#define MAX_BOTS_TEXT		25000

#define KAMI_SHOCKWAVE_STARTTIME		0
#define KAMI_SHOCKWAVEFADE_STARTTIME	1500
#define KAMI_SHOCKWAVE_ENDTIME			2000
#define KAMI_EXPLODE_STARTTIME			250
#define KAMI_IMPLODE_STARTTIME			2000
#define KAMI_IMPLODE_ENDTIME			2250
#define KAMI_SHOCKWAVE2_STARTTIME		2000
#define KAMI_SHOCKWAVE2FADE_STARTTIME	2500
#define KAMI_SHOCKWAVE2_ENDTIME			3000
#define KAMI_SHOCKWAVEMODEL_RADIUS		88
#define KAMI_BOOMSPHEREMODEL_RADIUS		72
#define KAMI_SHOCKWAVE_MAXRADIUS		1320
#define KAMI_BOOMSPHERE_MAXRADIUS		720
#define KAMI_SHOCKWAVE2_MAXRADIUS		704

// bg_alloc.c
void *G_Alloc(int size);
void G_InitMemory(void);

// bg_misc.c
int BG_FindNPCTypeID(const char *name);
qboolean BG_FactionShouldAttack(int attackerFaction, int targetFaction);
item_t *BG_FindItemForPowerup(powerup_t pw);
item_t *BG_FindItemForHoldable(holdable_t pw);
item_t *BG_FindItemForWeapon(weapon_t weapon);
item_t *BG_FindItem(const char *pickupName);
qboolean BG_CheckClassname(const char *classname);
item_t *BG_FindWeapon(int id);
item_t *BG_FindAmmo(int id);
qboolean BG_PlayerTouchesItem(playerState_t *ps, entityState_t *item, int atTime);
qboolean BG_CanItemBeGrabbed(int gametype, const entityState_t *ent, const playerState_t *ps);
void BG_EvaluateTrajectory(const trajectory_t *tr, int atTime, vec3_t result);
void BG_EvaluateTrajectoryDelta(const trajectory_t *tr, int atTime, vec3_t result);
void ST_EvaluateTrajectory(const trajectory_t *tr, int atTime, vec3_t result, float mass);
void ST_EvaluateTrajectoryDelta(const trajectory_t *tr, int atTime, vec3_t result, float mass);
void BG_AddPredictableEventToPlayerstate(int newEvent, int eventParm, playerState_t *ps);
void BG_TouchJumpPad(playerState_t *ps, entityState_t *jumppad);
void BG_PlayerStateToEntityState(playerState_t *ps, entityState_t *s, qboolean snap);
qboolean BG_InVehicle(int id);

// bg_pmove.c
void PM_UpdateViewAngles(playerState_t *ps, const usercmd_t *cmd);
void Pmove(pmove_t *pmove);

#endif
