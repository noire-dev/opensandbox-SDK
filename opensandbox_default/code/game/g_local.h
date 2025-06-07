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
//
// g_local.h -- local definitions for game module

#include "../qcommon/q_shared.h"
#include "bg_public.h"
#include "g_public.h"

#define BODY_QUEUE_SIZE		8

#define INFINITE			1000000

#define	FRAMETIME			100					// msec
#define	CARNAGE_REWARD_TIME	3000
#define REWARD_SPRITE_TIME	2000

#define	INTERMISSION_DELAY_TIME	1000
#define	SP_INTERMISSION_DELAY_TIME	5000

// gentity->flags
#define	FL_GODMODE				1
#define	FL_TEAMSLAVE			2
#define FL_NO_KNOCKBACK			4
#define FL_DROPPED_ITEM			8
#define FL_NO_BOTS				16
#define FL_NO_HUMANS			32
#define FL_NO_SPAWN				64 

// target_effect spawnflags
#define SF_EFFECT_EXPLOSION				1
#define SF_EFFECT_PARTICLES_GRAVITY		2
#define SF_EFFECT_PARTICLES_LINEAR		4
#define SF_EFFECT_PARTICLES_LINEAR_UP	8
#define SF_EFFECT_PARTICLES_LINEAR_DOWN	16
#define SF_EFFECT_OVERLAY				32
#define SF_EFFECT_FADE					64
#define SF_EFFECT_SMOKEPUFF				128
#define SF_EFFECT_ACTIVATOR				256

// movers are things like doors, plats, buttons, etc
typedef enum {
	MOVER_POS1,
	MOVER_POS2,
	MOVER_1TO2,
	MOVER_2TO1,
	
	ROTATOR_POS1,
	ROTATOR_POS2,
	ROTATOR_1TO2,
	ROTATOR_2TO1
} moverState_t;

#define MAX_LOGIC_ENTITIES	256 //maximum number of entities that can target a target_logic
#define MAX_NETNAME			36

#define MAX_UNDO_STACK		1024

typedef struct gentity_s gentity_t;
typedef struct gclient_s gclient_t;

struct gentity_s {
	entityState_t	s;				// communicated by server to clients
	entityShared_t	r;				// shared by both the server system and game

	// DO NOT MODIFY ANYTHING ABOVE THIS, THE SERVER
	// EXPECTS THE FIELDS IN THAT ORDER!
	//================================

	struct gclient_s	*client;			// NULL if not a client

	qboolean	inuse;

	char		*classname;			// set in QuakeEd
	int			spawnflags;			// set in QuakeEd

	qboolean	neverFree;			// if true, FreeEntity will only unlink
									// bodyque uses this

	int			flags;				// FL_* variables

	char		*model;
	char		*model2;
	int			freetime;			// level.time when the object was freed
	
	int			eventTime;			// events will be cleared EVENT_VALID_MSEC after set
	qboolean	freeAfterEvent;
	qboolean	unlinkAfterEvent;

	qboolean	physicsObject;		// if true, it can be pushed by movers and fall off edges
									// all game items are physicsObjects, 
	float		physicsBounce;		// 1.0 = continuous bounce, 0.0 = no bounce
	int			clipmask;			// brushes with this content value will be collided against
									// when moving.  items and corpses do not collide against
									// players, for instance

	// movers
	moverState_t moverState;
	int			soundPos1;
	int			sound1to2;
	int			sound2to1;
	int			soundPos2;
	int			soundLoop;
	gentity_t	*parent;
	gentity_t	*nextTrain;
	gentity_t	*prevTrain;
	vec3_t		pos1, pos2;

	char		*message;

	int			timestamp;		// body queue sinking, etc

	float		angle;			// set in editor, -1 = up, -2 = down
	char		*target;
	char		*targetname;
	char		*team;
	gentity_t	*target_ent;

	float		speed;
	vec3_t		movedir;

	int			nextthink;
	void		(*think)(gentity_t *self);
	void		(*reached)(gentity_t *self);	// movers call this when hitting endpoint
	void		(*blocked)(gentity_t *self, gentity_t *other);
	void		(*touch)(gentity_t *self, gentity_t *other, trace_t *trace);
	void		(*use)(gentity_t *self, gentity_t *other, gentity_t *activator);
	void		(*pain)(gentity_t *self, gentity_t *attacker, int damage);
	void		(*die)(gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod);

	int			pain_debounce_time;
	int			fly_sound_debounce_time;	// wind tunnel
	int			last_move_time;

	int			health;

	qboolean	takedamage;

	int			damage;
	int			splashDamage;	// quad will increase this without increasing radius
	int			splashRadius;
	int			methodOfDeath;
	int			splashMethodOfDeath;

	int			count;

	gentity_t	*chain;
	gentity_t	*enemy;
	gentity_t	*activator;
	gentity_t	*teamchain;		// next entity in team
	gentity_t	*teammaster;	// master of the team

	int			kamikazeTime;
	int			kamikazeShockTime;

	int			watertype;
	int			waterlevel;

	int			noise_index;

	// timing variables
	float		wait;
	float		random;

	gitem_t		*item;			// for bonus items

	//NEW VARIABLES
	qboolean	flashlight;
	gentity_t	*owner;
	int			sandboxObject;
	
	int			sb_coltype;
	float		sb_gravity;
	char		*sb_class;
	char		*sb_sound;
	int			sb_material;
	int			sb_phys;
	int			sb_coll;
	int			sb_isnpc;
	int			sb_red;
	int			sb_green;
	int			sb_blue;
	int			sb_radius;

	float		lip;
	float		height;
	float		phase;

	int			npcType;
	int			tool_id;
	gentity_t	*tool_entity;
	int			botskill;
	
	float		distance;
	int			type;
	int			vehicle;
	int			objectType;

	//entityplus variables
	gentity_t	*botspawn;
	char		*clientname;			// name of the bot to spawn for target_botspawn
	char		*mapname;				// name of the map to switch to for target_mapchange
	char		*teleporterTarget;		// forces a client to be teleported to the entity with this targetname when using holdable_teleporter. Also used as key for holdable_teleporter itself.
	int			logicEntities[MAX_LOGIC_ENTITIES];	//keeping track of entities targeting a target_logic
	char		*target2;	//second target
	char		*damagetarget;	//second target
	char		*targetname2; //second targetname
	char		*deathTarget;	// target to trigger when bot from target_botspawn dies
	char		*lootTarget;	//item to drop when bot from target_botspawn dies
	float		skill; // skill level set by target_skill
	char		*overlay; // reference to overlay texture for target_effect
	char		*key;	// key for target_modify to change
	char		*value; // value for target_modify to change to
	int			armor; // armor for the target_playerstats entity
	char		*music; //path to music file(s) for target_music
	vec4_t		rgba1; //start color for target_effect fade
	vec4_t		rgba2; //end color for target_effect fade
	int			mtype;
	int			mtimeout;
	int			mhoming;
	int			mspeed;
	int			mbounce;
	int			mdamage;
	int			msdamage;
	int			msradius;
	int			mgravity;
	int			mnoclip;
	int			allowuse;
	
	int			swep_list[WEAPONS_NUM];
	int			swep_ammo[WEAPONS_NUM];
	int			swep_id;

	//Physgun and Gravitygun
	gentity_t 	*grabbedEntity;		//physgun object for player
	qboolean	isGrabbed;			//object is grabbed by player for prop
	float		grabDist;			//physgun distance for player
	vec3_t		grabOffset;			//physgun offset for player
	vec3_t		grabOldOrigin;		//physgun old origin for prop
	int			grabNewPhys;		//for freeze prop for prop

	//Props
	gentity_t 	*lastPlayer;		//for damage and killfeed

	//Phys state (Trace)
	qboolean	phys_inAir;
	qboolean	phys_inSolid;

	//Phys state (Point)
	qboolean	phys_inWater;

	//Phys settings
	int			phys_weldedObjectsNum;
	gentity_t	*phys_parent;

	//Saved info (save)
	vec3_t		phys_relativeOrigin;

	//Rotate vectors (save)
	vec3_t		phys_rv_0;
	vec3_t		phys_rv_1;
	vec3_t		phys_rv_2;

	//Phys think
	int			phys_nextthink;
	void		(*phys_think)(gentity_t *self);

	//Save weld (sync)
	int			sb_phys_welded; //welded
	int			sb_phys_parent;	//master

};


typedef enum {
	CON_DISCONNECTED,
	CON_CONNECTING,
	CON_CONNECTED
} clientConnected_t;

typedef enum {
	SPECTATOR_NOT,
	SPECTATOR_FREE,
	SPECTATOR_FOLLOW,
	SPECTATOR_SCOREBOARD
} spectatorState_t;

typedef enum {
	TEAM_BEGIN,		// Beginning a team game, spawn at base
	TEAM_ACTIVE		// Now actively playing
} playerTeamStateState_t;

typedef struct {
	playerTeamStateState_t	state;

	int			location;

	int			captures;
	int			basedefense;
	int			carrierdefense;
	int			flagrecovery;
	int			fragcarrier;
	int			assists;

	float		lasthurtcarrier;
	float		lastreturnedflag;
	float		flagsince;
	float		lastfraggedcarrier;
} playerTeamState_t;

// the auto following clients don't follow a specific client
// number, but instead follow the first two active players
#define	FOLLOW_ACTIVE1	-1
#define	FOLLOW_ACTIVE2	-2

// client data that stays across multiple levels or tournament restarts
// this is achieved by writing all the data to cvar strings at game shutdown
// time and reading them back at connection time.  Anything added here
// MUST be dealt with in G_InitSessionData() / G_ReadSessionData() / G_WriteSessionData()
typedef struct {
	team_t				sessionTeam;
	int					spectatorNum;		// for determining next-in-line to play
	spectatorState_t	spectatorState;
	int					spectatorClient;	// for chasecam and follow mode
	qboolean			teamLeader;			// true when this client is a team leader
} clientSession_t;

#define	MAX_VOTE_COUNT		3

//unlagged - true ping
#define NUM_PING_SAMPLES 64
//unlagged - true ping

// client data that stays across multiple respawns, but is cleared
// on each level change or team change at ClientBegin()
typedef struct {
	clientConnected_t	connected;
	usercmd_t	cmd;				// we would lose angles if not persistant
	char		netname[MAX_NETNAME];
	int			maxHealth;
	int			enterTime;			// level.time the client entered the game
	playerTeamState_t teamState;	// status in teamplay games

	int			pingsamples[NUM_PING_SAMPLES];
	int			samplehead;
	
    int         oldmoney;
} clientPersistant_t;

typedef struct {
    int id;
    int type;
    qboolean isRemoved;
} undo_stack_t;

//unlagged - backward reconciliation #1
// the size of history we'll keep
#define NUM_CLIENT_HISTORY 17

// everything we need to know to backward reconcile
typedef struct {
	vec3_t		mins, maxs;
	vec3_t		currentOrigin;
	int			leveltime;
} clientHistory_t;
//unlagged - backward reconciliation #1

// this structure is cleared on each ClientSpawn(),
// except for 'client->pers' and 'client->sess'
struct gclient_s {
	// ps MUST be the first element, because the server expects it
	playerState_t	ps;				// communicated by server to clients

	// the rest of the structure is private to game
	clientPersistant_t	pers;
	clientSession_t		sess;

	qboolean	readyToExit;		// wishes to leave the intermission
	qboolean	noclip;

	int			lastCmdTime;		// level.time of last usercmd_t, for EF_CONNECTION
									// we can't just use pers.lastCommand.time, because
									// of the g_sycronousclients case
	int			buttons;
	int			oldbuttons;
	int			latched_buttons;

	vec3_t		oldOrigin;

	// sum up damage over an entire frame, so
	// shotgun blasts give a single big kick
	int			damage_armor;		// damage absorbed by armor
	int			damage_blood;		// damage taken out of health
	int			damage_knockback;	// impact damage
	vec3_t		damage_from;		// origin for vector calculation
	qboolean	damage_fromWorld;	// if true, don't use the damage_from vector

	int			lasthurt_client;	// last client that damaged this client
	int			lasthurt_mod;		// type of damage the client did

	// timers
	int			respawnTime;		// can respawn when time > this, force after g_forcerespwan
	int			rewardTime;			// clear the EF_AWARD_IMPRESSIVE, etc when time > this

	int			airOutTime;

	qboolean	fireHeld;			// used for hook
	gentity_t	*hook;				// grapple hook if out
	gentity_t	*lasersight;		// flashlight

	// timeResidual is used to handle events that happen every second
	// like health / armor countdowns and regeneration
	int			timeResidual;

	int			timeEntityInfo;

	gentity_t	*persistantPowerup;
	int			portalID;
	int			invulnerabilityTime;

	char		*areabits;

	// unlagged - backward reconciliation #1
	// the serverTime the button was pressed
	int			attackTime;
	// the head of the history queue
	int			historyHead;
	// the history queue
	clientHistory_t	history[NUM_CLIENT_HISTORY];
	// the client's saved position
	clientHistory_t	saved;			// used to restore after time shift
	// an approximation of the actual server time we received this
	// command (not in 50ms increments)
	int			frameOffset;

	int			vehiclenum;

	// unlagged - smooth clients #1
	// the last frame number we got an update from this client
	int			lastUpdateFrame;
	qboolean    spawnprotected;

	undo_stack_t undoStack[MAX_UNDO_STACK];
};

//
// this structure is cleared as each map is entered
//
#define	MAX_SPAWN_VARS			64
#define	MAX_SPAWN_VARS_CHARS	4096

typedef struct {
	struct gclient_s	*clients;		// [maxclients]

	struct gentity_s	*gentities;
	int			gentitySize;
	int			num_entities;		// current number, <= MAX_GENTITIES

	// store latched cvars here that we want to get at often
	int			maxclients;

	int			framenum;
	int			time;					// in msec
	int			previousTime;			// so movers can back up when blocked

	int			startTime;				// level.time the map was started

	int			teamScores[TEAM_NUM_TEAMS];
	int			lastTeamLocationTime;		// last time of client team location update

	qboolean	newSession;				// don't use any old session data, because
										// we changed gametype

	qboolean	restarted;				// waiting for a map_restart to fire

	int			numConnectedClients;
	int			numNonSpectatorClients;	// includes connecting clients
	int			numPlayingClients;		// connected, non-spectators
	int			sortedClients[MAX_CLIENTS];		// sorted by score
	int			follow1, follow2;		// clientNums for auto-follow spectators

	int			snd_fry;				// sound index for standing in lava

	// spawn variables
	qboolean	spawning;				// the G_Spawn*() functions are valid
	int			numSpawnVars;
	char		*spawnVars[MAX_SPAWN_VARS][2];	// key / value pairs
	int			numSpawnVarChars;
	char		spawnVarChars[MAX_SPAWN_VARS_CHARS];

	// intermission state
	int			intermissionQueued;		// intermission was qualified, but
										// wait INTERMISSION_DELAY_TIME before
										// actually going there so the last
										// frag can be watched.  Disable future
										// kills during this delay
	int			intermissiontime;		// time the intermission was started
	qboolean	readyToExit;			// at least one client wants to exit
	int			exitTime;
	vec3_t		intermission_origin;	// also used for spectator spawns
	vec3_t		intermission_angle;

	qboolean	locationLinked;			// target_locations get linked
	gentity_t	*locationHead;			// head of the location list
	int			bodyQueIndex;			// dead bodies
	gentity_t	*bodyQue[BODY_QUEUE_SIZE];
	int			portalSequence;

	int			frameStartTime;

    //Obelisk tell
    int healthRedObelisk; //health in percent
    int healthBlueObelisk; //helth in percent
    qboolean MustSendObeliskHealth; //Health has changed
	gentity_t	*player;				// refers to the player in SP mode. Provides quick access to the player entity
	char		scoreLevelName[64];
	int			secretCount;			// number of target_secret entities in map
} level_locals_t;

extern int				SourceTechEntityList[MAX_GENTITIES];

//
// g_spawn.c
//

qboolean	G_SpawnString( const char *key, const char *defaultString, char **out );
// spawn string returns a temporary reference, you must CopyString() if you want to keep it
qboolean	G_SpawnFloat( const char *key, const char *defaultString, float *out );
qboolean	G_SpawnInt( const char *key, const char *defaultString, int *out );
qboolean	G_SpawnVector( const char *key, const char *defaultString, float *out );
qboolean	G_SpawnVector4( const char *key, const char *defaultString, float *out );
void		G_SpawnEntitiesFromString( void );
char 		*G_NewString( const char *string );

typedef struct {
	char	*name;
	void	(*spawn)(gentity_t *ent);
} spawn_t;

void SP_info_player_start (gentity_t *ent);
void SP_info_player_deathmatch (gentity_t *ent);

void SP_func_plat (gentity_t *ent);
void SP_func_static (gentity_t *ent);
void SP_func_rotating (gentity_t *ent);
void SP_func_bobbing (gentity_t *ent);
void SP_func_pendulum( gentity_t *ent );
void SP_func_button (gentity_t *ent);
void SP_func_door (gentity_t *ent);
void SP_func_train (gentity_t *ent);

void SP_trigger_always (gentity_t *ent);
void SP_trigger_multiple (gentity_t *ent);
void SP_trigger_push (gentity_t *ent);
void SP_trigger_teleport (gentity_t *ent);
void SP_trigger_hurt (gentity_t *ent);

void SP_target_give (gentity_t *ent);
void SP_target_delay (gentity_t *ent);
void SP_target_speaker (gentity_t *ent);
void SP_target_print (gentity_t *ent);
void SP_target_teleporter( gentity_t *ent );
void SP_target_relay (gentity_t *ent);
void SP_target_kill (gentity_t *ent);
void SP_target_position (gentity_t *ent);
void SP_target_location (gentity_t *ent);
void SP_target_push (gentity_t *ent);

void SP_path_corner (gentity_t *self);

void SP_misc_portal_camera(gentity_t *ent);
void SP_misc_portal_surface(gentity_t *ent);

void SP_shooter_rocket( gentity_t *ent );
void SP_shooter_plasma( gentity_t *ent );
void SP_shooter_grenade( gentity_t *ent );

void SP_team_CTF_redplayer( gentity_t *ent );
void SP_team_CTF_blueplayer( gentity_t *ent );
void SP_team_CTF_redspawn( gentity_t *ent );
void SP_team_CTF_bluespawn( gentity_t *ent );
void SP_team_blueobelisk( gentity_t *ent );
void SP_team_redobelisk( gentity_t *ent );
void SP_team_neutralobelisk( gentity_t *ent );

void SP_info_player_dd (gentity_t *ent);
void SP_info_player_dd_red (gentity_t *ent);
void SP_info_player_dd_blue (gentity_t *ent);

void SP_script_variable (gentity_t *ent);
void SP_script_cmd (gentity_t *ent);

void SP_sandbox_prop (gentity_t *ent);
void SP_sandbox_npc (gentity_t *ent);

extern spawn_t spawns_table[];

//
// g_cmds.c
//

void Cmd_Score_f (gentity_t *ent);
void StopFollowing( gentity_t *ent );
void BroadcastTeamChange( gclient_t *client, int oldTeam );
void SetTeam( gentity_t *ent, char *s );
void Cmd_FollowCycle_f( gentity_t *ent, int dir );  //KK-OAX Changed to match definition
char *ConcatArgs( int start );  //KK-OAX This declaration moved from g_svccmds.c

//
// g_items.c
//

void G_CheckTeamItems( void );
void G_RunItem( gentity_t *ent );
void RespawnItem( gentity_t *ent );
void UseHoldableItem( gentity_t *ent );
void PrecacheItem (gitem_t *it);
gentity_t *Drop_Item( gentity_t *ent, gitem_t *item );
gentity_t *LaunchItem( gitem_t *item, vec3_t origin, vec3_t velocity );
void SetRespawn (gentity_t *ent, float delay);
void G_SpawnItem (gentity_t *ent, gitem_t *item);
void FinishSpawningItem( gentity_t *ent );
void Think_Weapon (gentity_t *ent);
int ArmorIndex (gentity_t *ent);
void	Add_Ammo (gentity_t *ent, int weapon, int count);
void	Set_Ammo (gentity_t *ent, int weapon, int count);
void	Set_Weapon (gentity_t *ent, int weapon, int count);
void Touch_Item (gentity_t *ent, gentity_t *other, trace_t *trace );
void Touch_Item2(gentity_t *ent, gentity_t *other, trace_t *trace, qboolean allowBot);

void ClearRegisteredItems( void );
void RegisterItem( gitem_t *item );
void SaveRegisteredItems( void );

//
// g_utils.c
//

int G_ModelIndex( char *name );
int		G_SoundIndex( char *name );
void	G_TeamCommand( team_t team, char *cmd );
void	G_KillBox (gentity_t *ent);
gentity_t *G_Find (gentity_t *from, int fieldofs, const char *match);
gentity_t *G_PickTarget (char *targetname);
void G_PickAllTargets ( gentity_t *ent );
void	G_UseTargets (gentity_t *ent, gentity_t *activator);
void	G_SetMovedir ( vec3_t angles, vec3_t movedir);
void G_BotSandboxCheck (gentity_t *self);
void VehiclePhys( gentity_t *self );
gentity_t *FindEntityForPhysgun( gentity_t *ent, int range );
gentity_t *FindEntityForGravitygun( gentity_t *ent, int range );
void CrosshairPointPhys(gentity_t *ent, int range, vec3_t outPoint);
void CrosshairPointGravity(gentity_t *ent, int range, vec3_t outPoint);
gentity_t *G_FindEntityForEntityNum(int entityn);
gentity_t *G_FindEntityForClientNum(int entityn);

qboolean G_PlayerIsOwner(gentity_t *player, gentity_t *ent);
gentity_t *G_FindWeldEntity(gentity_t *ent);

void	G_InitGentity( gentity_t *e );
gentity_t	*G_Spawn (void);
gentity_t *G_TempEntity( vec3_t origin, int event );
void	G_Sound( gentity_t *ent, int channel, int soundIndex );

void	    G_FreeEntity( gentity_t *e );

void	G_TouchTriggers (gentity_t *ent);

float	*tv (float x, float y, float z);
char	*vtos( const vec3_t v );

void G_AddPredictableEvent( gentity_t *ent, int event, int eventParm );
void G_AddEvent( gentity_t *ent, int event, int eventParm );
void G_SetOrigin( gentity_t *ent, vec3_t origin );


void target_finish_think(gentity_t* self);
void target_finish_use (gentity_t *self, gentity_t *other, gentity_t *activator);

//
// g_combat.c
//

qboolean CanDamage (gentity_t *targ, vec3_t origin);
void G_Damage (gentity_t *targ, gentity_t *inflictor, gentity_t *attacker, vec3_t dir, vec3_t point, int damage, int dflags, int mod);
void G_ExitVehicle (int num);
void G_RadiusDamage (vec3_t origin, gentity_t *attacker, float damage, float radius, gentity_t *ignore, int mod);
int G_InvulnerabilityEffect( gentity_t *targ, vec3_t dir, vec3_t point, vec3_t impactpoint, vec3_t bouncedir );
void body_die( gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int meansOfDeath );
void TossClientItems( gentity_t *self );
void TossClientPersistantPowerups( gentity_t *self );
void TossClientCubes( gentity_t *self );

// damage flags
#define DAMAGE_RADIUS				0x00000001	// damage was indirect
#define DAMAGE_NO_ARMOR				0x00000002	// armour does not protect from this damage
#define DAMAGE_NO_KNOCKBACK			0x00000004	// do not affect velocity, just view angles
#define DAMAGE_NO_PROTECTION		0x00000008  // armor, shields, invulnerability, and godmode have no effect
#define DAMAGE_NO_TEAM_PROTECTION	0x00000010  // armor, shields, invulnerability, and godmode have no effect

//
// g_missile.c
//

void G_RunMissile( gentity_t *ent );

gentity_t *fire_plasma (gentity_t *self, vec3_t start, vec3_t aimdir);
gentity_t *fire_grenade (gentity_t *self, vec3_t start, vec3_t aimdir);
gentity_t *fire_rocket (gentity_t *self, vec3_t start, vec3_t dir);
gentity_t *fire_bfg (gentity_t *self, vec3_t start, vec3_t dir);
gentity_t *fire_grapple (gentity_t *self, vec3_t start, vec3_t dir);
gentity_t *fire_nail( gentity_t *self, vec3_t start, vec3_t forward, vec3_t right, vec3_t up );
gentity_t *fire_prox( gentity_t *self, vec3_t start, vec3_t aimdir );
gentity_t *fire_flame (gentity_t *self, vec3_t start, vec3_t aimdir);
gentity_t *fire_antimatter (gentity_t *self, vec3_t start, vec3_t aimdir);
gentity_t *fire_thrower( gentity_t *self, vec3_t start, vec3_t forward, vec3_t right, vec3_t up );
gentity_t *fire_bouncer( gentity_t *self, vec3_t start, vec3_t forward, vec3_t right, vec3_t up );
gentity_t *fire_exploder (gentity_t *self, vec3_t start, vec3_t dir);
gentity_t *fire_knocker( gentity_t *self, vec3_t start, vec3_t forward, vec3_t right, vec3_t up );
gentity_t *fire_regenerator (gentity_t *self, vec3_t start, vec3_t dir);
gentity_t *fire_propgun( gentity_t *self, vec3_t start, vec3_t forward, vec3_t right, vec3_t up );
gentity_t *fire_nuke( gentity_t *self, vec3_t start, vec3_t forward, vec3_t right, vec3_t up );

//
// g_mover.c
//

void G_RunMover( gentity_t *ent );
void Touch_DoorTrigger( gentity_t *ent, gentity_t *other, trace_t *trace );

//
// g_trigger.c
//

void trigger_teleporter_touch (gentity_t *self, gentity_t *other, trace_t *trace );
void lock_touch( gentity_t *self, gentity_t *other, trace_t *trace );

//
// g_misc.c
//

void TeleportPlayer( gentity_t *player, vec3_t origin, vec3_t angles, qboolean noKnockback );
void DropPortalSource( gentity_t *ent );
void DropPortalDestination( gentity_t *ent );

//
// g_weapon.c
//
void CalcMuzzlePoint ( gentity_t *ent, vec3_t forward, vec3_t right, vec3_t up, vec3_t muzzlePoint );
//unlagged - attack prediction #3
// we're making this available to both games
void SnapVectorTowards( vec3_t v, vec3_t to );
//unlagged - attack prediction #3
qboolean CheckGauntletAttack( gentity_t *ent );
void Weapon_HookFree (gentity_t *ent);
void Weapon_HookThink (gentity_t *ent);
void Laser_Gen (gentity_t *ent);
void Laser_Think( gentity_t *self );

void Weapon_Toolgun_Info( gentity_t *ent );

//unlagged - g_unlagged.c
void G_ResetHistory( gentity_t *ent );
void G_StoreHistory( gentity_t *ent );
void G_TimeShiftAllClients( int time, gentity_t *skip );
void G_UnTimeShiftAllClients( gentity_t *skip );
void G_DoTimeShiftFor( gentity_t *ent );
void G_UndoTimeShiftFor( gentity_t *ent );
void G_UnTimeShiftClient( gentity_t *client );
void G_PredictPlayerMove( gentity_t *ent, float frametime );
//unlagged - g_unlagged.c

//
// g_client.c
//

team_t TeamCount( int ignoreClientNum, int team );
int TeamLeader( int team );
team_t PickTeam( int ignoreClientNum );
void SetClientViewAngle( gentity_t *ent, vec3_t angle );
gentity_t *SelectSpawnPoint ( vec3_t avoidPoint, vec3_t origin, vec3_t angles );
void CopyToBodyQue( gentity_t *ent );
void ClientRespawn(gentity_t *ent);
void BeginIntermission (void);
void InitClientPersistant (gclient_t *client);
void InitClientResp (gclient_t *client);
void InitBodyQue (void);
void ClientSpawn( gentity_t *ent );
void player_die (gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod);
void AddScore( gentity_t *ent, int score );
void CalculateRanks( void );
qboolean SpotWouldTelefrag( gentity_t *spot );
qboolean SpawnPointIsActive( gentity_t *spot );

//
// g_svcmds.c
//

qboolean	ConsoleCommand( void );
gclient_t	*ClientForString( const char *s );

//
// g_weapon.c
//

void FireWeapon( gentity_t *ent );
void KamikazeDamage( gentity_t *self );
void CarExplodeDamage( gentity_t *self );
void KamikazeRadiusDamage( vec3_t origin, gentity_t *attacker, float damage, float radius, int mod );
void KamikazeShockWave( vec3_t origin, gentity_t *attacker, float damage, float push, float radius, int mod );
void G_StartKamikaze( gentity_t *ent );
void G_StartCarExplode( gentity_t *ent );
void G_StartNukeExplode( gentity_t *ent );

//
// p_hud.c
//

void MoveClientToIntermission (gentity_t *client);
void G_SetStats (gentity_t *ent);
void DeathmatchScoreboardMessage (gentity_t *client);

//
// g_cmds.c
//

void DoubleDominationScoreTimeMessage( gentity_t *ent );
void ObeliskHealthMessage( void );
void DeathmatchScoreboardMessage (gentity_t *client);
void RespawnTimeMessage(gentity_t *ent, int time);
void DominationPointNamesMessage (gentity_t *client);
void DominationPointStatusMessage( gentity_t *ent );

//
// g_main.c
//

void FindIntermissionPoint( void );
void SetLeader(int team, int client);
void CheckTeamLeader( int team );
void G_RunThink (gentity_t *ent);
void ExitLevel( void );
void SendScoreboardMessageToAllClients( void );
void QDECL G_Printf( const char *fmt, ... );
void QDECL G_Error( const char *fmt, ... ) __attribute__((noreturn));
void CheckTeamVote( int team );
qboolean G_NpcFactionProp(int prop, gentity_t* ent);

//
// g_client.c
//

char *ClientConnect( int clientNum, qboolean firstTime, qboolean isBot );
void ClientUserinfoChanged( int clientNum );
void ClientDisconnect( int clientNum );
void ClientBegin( int clientNum );
void ClientCommand( int clientNum );
void DropClientSilently( int clientNum );
void LinkBotSpawn( gentity_t *bot, char parentid[] );
void SetupCustomBot( gentity_t *bot );
void SetUnlimitedWeapons( gentity_t *ent );
void SetSandboxWeapons( gentity_t *ent );
void SetCustomWeapons( gentity_t *ent );

//
// g_active.c
//

void ClientThink( int clientNum );
void ClientEndFrame( gentity_t *ent );
void G_RunClient( gentity_t *ent );
qboolean G_CheckSwep( int clientNum, int wp, int finish );
int G_CheckSwepAmmo( int clientNum, int wp );
void G_DefaultSwep( int clientNum, int wp );

//
// g_team.c
//

qboolean OnSameTeam( gentity_t *ent1, gentity_t *ent2 );
void Team_CheckDroppedItem( gentity_t *dropped );
qboolean CheckObeliskAttack( gentity_t *obelisk, gentity_t *attacker );
//KK-OAX Added for Command Handling Changes (r24)
team_t G_TeamFromString( char *str );

//
// g_session.c
//

void G_ReadSessionData( gclient_t *client );
void G_InitSessionData( gclient_t *client, char *userinfo );
void G_InitWorldSession( void );
void G_WriteSessionData( void );

//
// g_bot.c
//

char *G_GetBotInfoByNumber( int num );
char *G_GetBotInfoByName( const char *name );
void G_CheckBotSpawn( void );
void G_RemoveQueuedBotBegin( int clientNum );
qboolean G_BotConnect( int clientNum, qboolean restart );
void Svcmd_AddBot_f( void );

//
// g_physics.c
//

void Phys_CheckCarCollisions(gentity_t *ent);

void Phys_HoldDropStatic(gentity_t *player, vec3_t velocity);
void Phys_HoldDropDynamic(gentity_t *player, vec3_t velocity, qboolean isPhysgun);
void Phys_HoldSetup(gentity_t *player, qboolean isPhysgun);
void Phys_HoldFrame(gentity_t *player, vec3_t velocity, qboolean isPhysgun);

void Phys_Disable( gentity_t *ent, vec3_t origin );
void Phys_Enable( gentity_t *ent );
void Phys_Unweld( gentity_t *ent );

void Phys_Frame( gentity_t *ent );
void Phys_Smoke( gentity_t *ent, float impact );
void Phys_CarDamage (gentity_t *targ, gentity_t *attacker, int damage);


//
// g_playerstore.c
//

void PlayerStoreInit( void );
void PlayerStore_store(char* guid, playerState_t ps);
void PlayerStore_restore(char* guid, playerState_t *ps);

//
// g_sandbox.c
//

void G_BuildPropSL( char *arg02, char *arg03, vec3_t xyz, gentity_t *player, char *arg04, char *arg05, char *arg06, char *arg07, char *arg08, char *arg09, char *arg10, char *arg11, char *arg12, char *arg13, char *arg14, char *arg15, char *arg16, char *arg17, char *arg18, char *arg19, char *arg20, char *arg21, char *arg22);
void G_ModProp( gentity_t *targ, gentity_t *attacker, char *arg01, char *arg02, char *arg03, char *arg04, char *arg05, char *arg06, char *arg07, char *arg08, char *arg09, char *arg10, char *arg11, char *arg12, char *arg13, char *arg14, char *arg15, char *arg16, char *arg17, char *arg18, char *arg19 );
void G_DieProp (gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod);

void Undo_ShiftStack(gentity_t *ent);
void Undo_AddElement(gentity_t *ent, int id, int type);
qboolean Undo_LastElement(gentity_t *ent, int *id, int *type, qboolean *isRemoved);
void Undo_RemoveElement(gentity_t *ent);

//
// g_mapfiles.c
//

qboolean G_ClassnameAllowed( char *input, qboolean danger );
void G_WriteMapfile_f( void );
void G_DeleteMapfile_f( void );
void G_ClearMap_f( void );
void G_ClearSandboxMap_f( void );
void G_LoadMapfile( char *filename );
void G_LoadMapfile_f( void );
qboolean SkippedChar ( char in );

// ai_main.c
#define MAX_FILEPATH			144

//bot settings
typedef struct bot_settings_s
{
	char characterfile[MAX_FILEPATH];
	float skill;
	char team[MAX_FILEPATH];
	char waypoint[MAX_TOKEN_CHARS];
} bot_settings_t;

int BotAISetup( int restart );
int BotAIShutdown( int restart );
int BotAILoadMap( int restart );
int BotAISetupClient(int client, struct bot_settings_s *settings, qboolean restart);
int BotAIShutdownClient( int client, qboolean restart );
int BotAIStartFrame( int time );
void BotTestAAS(vec3_t origin);

#include "g_team.h" // teamplay specific stuff


extern	level_locals_t	level;
extern	gentity_t		g_entities[MAX_GENTITIES];

#define	FOFS(x) ((size_t)&(((gentity_t *)0)->x))

//CVARS
void 	G_SendGameCvars( gentity_t *ent );
void 	G_SendSwepWeapons( gentity_t *ent );
void 	G_SendSpawnSwepWeapons( gentity_t *ent );
void 	plasma_think( gentity_t *ent );
void 	rocket_think( gentity_t *ent );
void 	grenade_think( gentity_t *ent );
void 	bfg_think( gentity_t *ent );
void 	nailgun_think( gentity_t *ent );
void 	UpdateGameCvars( void );
extern 	int 		mod_jumpheight;
extern	int			mod_gravity;

#define CMD_CHEAT           0x0001
#define CMD_CHEAT_TEAM      0x0002 // is a cheat when used on a team
#define CMD_MESSAGE         0x0004 // sends message to others (skip when muted)
#define CMD_TEAM            0x0008 // must be on a team
#define CMD_NOTEAM          0x0010 // must not be on a team
#define CMD_LIVING          0x0020
#define CMD_INTERMISSION    0x0040 // valid during intermission

typedef struct
{
    char    *cmdName;
    int     cmdFlags;
    void    ( *cmdHandler )( gentity_t *ent );
} commands_t;

//
// g_svcmds_ext.c
//

//Noire.Script
void Svcmd_NS_OpenScript_f( void );
void Svcmd_NS_Interpret_f( void );
void Svcmd_NS_VariableList_f( void );
void Svcmd_NS_ThreadList_f( void );
void Svcmd_NS_SendVariable_f( void );

float cvar_VariableValue( const char *var_name );

//SYSCALLS
void		trap_Printf( const char *fmt );
void		trap_Error( const char *fmt ) __attribute__((noreturn));
int			trap_Milliseconds( void );
void		trap_Cvar_Register( vmCvar_t *cvar, const char *var_name, const char *value, int flags );
void		trap_Cvar_Update( vmCvar_t *cvar );
void		trap_Cvar_Set( const char *var_name, const char *value );
int			trap_Cvar_VariableIntegerValue( const char *var_name );
void		trap_Cvar_VariableStringBuffer( const char *var_name, char *buffer, int bufsize );
int			trap_Argc( void );
void		trap_Argv( int n, char *buffer, int bufferLength );
int			trap_FS_FOpenFile( const char *qpath, fileHandle_t *f, fsMode_t mode );
void		trap_FS_Read( void *buffer, int len, fileHandle_t f );
void		trap_FS_Write( const void *buffer, int len, fileHandle_t f );
void		trap_FS_FCloseFile( fileHandle_t f );
void		trap_SendConsoleCommand( int exec_when, const char *text );
void		trap_LocateGameData( gentity_t *gEnts, int numGEntities, int sizeofGEntity_t, playerState_t *gameClients, int sizeofGameClient );
void		trap_DropClient( int clientNum, const char *reason );
void		trap_SendServerCommand( int clientNum, const char *text );
void		trap_SetConfigstring( int num, const char *string );
void		trap_GetConfigstring( int num, char *buffer, int bufferSize );
void		trap_GetUserinfo( int num, char *buffer, int bufferSize );
void		trap_SetUserinfo( int num, const char *buffer );
void		trap_GetServerinfo( char *buffer, int bufferSize );
void		trap_SetBrushModel( gentity_t *ent, const char *name );
void		trap_Trace( trace_t *results, const vec3_t start, const vec3_t mins, const vec3_t maxs, const vec3_t end, int passEntityNum, int contentmask );
int			trap_PointContents( const vec3_t point, int passEntityNum );
qboolean 	trap_InPVS( const vec3_t p1, const vec3_t p2 );
void		trap_AdjustAreaPortalState( gentity_t *ent, qboolean open );
void		trap_LinkEntity( gentity_t *ent );
void		trap_UnlinkEntity( gentity_t *ent );
int			trap_EntitiesInBox( const vec3_t mins, const vec3_t maxs, int *entityList, int maxcount );
qboolean 	trap_EntityContact( const vec3_t mins, const vec3_t maxs, const gentity_t *ent );
int			trap_BotAllocateClient( void );
void		trap_GetUsercmd( int clientNum, usercmd_t *cmd );
qboolean	trap_GetEntityToken( char *buffer, int bufferSize );
int			trap_FS_GetFileList( const char *path, const char *extension, char *listbuf, int bufsize );
int     	trap_RealTime( qtime_t *qtime );
void 		trap_System( const char *command );

int			trap_BotLibSetup( void );
int			trap_BotLibShutdown( void );
int			trap_BotLibVarSet(char *var_name, char *value);
int			trap_BotLibDefine(char *string);
int			trap_BotLibStartFrame(float time);
int			trap_BotLibLoadMap(const char *mapname);
int			trap_BotLibUpdateEntity(int ent, void /* struct bot_updateentity_s */ *bue);
int			trap_BotGetSnapshotEntity( int clientNum, int sequence );
int			trap_BotGetServerCommand(int clientNum, char *message, int size);
void		trap_BotUserCommand(int client, usercmd_t *ucmd);

int			trap_AAS_EnableRoutingArea( int areanum, int enable );
int			trap_AAS_BBoxAreas(vec3_t absmins, vec3_t absmaxs, int *areas, int maxareas);
int			trap_AAS_AreaInfo( int areanum, void /* struct aas_areainfo_s */ *info );
void		trap_AAS_EntityInfo(int entnum, void /* struct aas_entityinfo_s */ *info);
int			trap_AAS_Initialized(void);
void		trap_AAS_PresenceTypeBoundingBox(int presencetype, vec3_t mins, vec3_t maxs);
float		trap_AAS_Time(void);
int			trap_AAS_PointAreaNum(vec3_t point);
int			trap_AAS_TraceAreas(vec3_t start, vec3_t end, int *areas, vec3_t *points, int maxareas);
int			trap_AAS_PointContents(vec3_t point);
int			trap_AAS_NextBSPEntity(int ent);
int			trap_AAS_ValueForBSPEpairKey(int ent, char *key, char *value, int size);
int			trap_AAS_VectorForBSPEpairKey(int ent, char *key, vec3_t v);
int			trap_AAS_FloatForBSPEpairKey(int ent, char *key, float *value);
int			trap_AAS_IntForBSPEpairKey(int ent, char *key, int *value);
int			trap_AAS_AreaReachability(int areanum);
int			trap_AAS_AreaTravelTimeToGoalArea(int areanum, vec3_t origin, int goalareanum, int travelflags);
int			trap_AAS_Swimming(vec3_t origin);
int			trap_AAS_PredictClientMovement(void /* aas_clientmove_s */ *move, int entnum, vec3_t origin, int presencetype, int onground, vec3_t velocity, vec3_t cmdmove, int cmdframes, int maxframes, float frametime, int stopevent, int stopareanum, int visualize);

void		trap_EA_Say(int client, char *str);
void		trap_EA_SayTeam(int client, char *str);
void		trap_EA_Command(int client, char *command);
void		trap_EA_Action(int client, int action);
void		trap_EA_Gesture(int client);
void		trap_EA_Talk(int client);
void		trap_EA_Attack(int client);
void		trap_EA_Use(int client);
void		trap_EA_Respawn(int client);
void		trap_EA_Crouch(int client);
void		trap_EA_SelectWeapon(int client, int weapon);
void		trap_EA_View(int client, vec3_t viewangles);
void		trap_EA_GetInput(int client, float thinktime, void /* struct bot_input_s */ *input);
void		trap_EA_ResetInput(int client);

int			trap_BotLoadCharacter(char *charfile, float skill);
void		trap_BotFreeCharacter(int character);
float		trap_Characteristic_BFloat(int character, int index, float min, float max);
int			trap_Characteristic_BInteger(int character, int index, int min, int max);
void		trap_Characteristic_String(int character, int index, char *buf, int size);
int			trap_BotAllocChatState(void);
void		trap_BotFreeChatState(int handle);
void		trap_BotQueueConsoleMessage(int chatstate, int type, char *message);
void		trap_BotRemoveConsoleMessage(int chatstate, int handle);
int			trap_BotNextConsoleMessage(int chatstate, void /* struct bot_consolemessage_s */ *cm);
int			trap_BotNumConsoleMessages(int chatstate);
void		trap_BotInitialChat(int chatstate, char *type, int mcontext, char *var0, char *var1, char *var2, char *var3, char *var4, char *var5, char *var6, char *var7 );
int			trap_BotNumInitialChats(int chatstate, char *type);
int			trap_BotReplyChat(int chatstate, char *message, int mcontext, int vcontext, char *var0, char *var1, char *var2, char *var3, char *var4, char *var5, char *var6, char *var7 );
void		trap_BotEnterChat(int chatstate, int client, int sendto);
void		trap_BotGetChatMessage(int chatstate, char *buf, int size);
int			trap_BotFindMatch(char *str, void /* struct bot_match_s */ *match, unsigned long int context);
void		trap_BotMatchVariable(void /* struct bot_match_s */ *match, int variable, char *buf, int size);
void		trap_UnifyWhiteSpaces(char *string);
void		trap_BotReplaceSynonyms(char *string, unsigned long int context);
int			trap_BotLoadChatFile(int chatstate, char *chatfile, char *chatname);
void		trap_BotSetChatGender(int chatstate, int gender);
void		trap_BotSetChatName(int chatstate, char *name, int client);
void		trap_BotResetGoalState(int goalstate);
void		trap_BotRemoveFromAvoidGoals(int goalstate, int number);
void		trap_BotResetAvoidGoals(int goalstate);
void		trap_BotPushGoal(int goalstate, void /* struct bot_goal_s */ *goal);
void		trap_BotPopGoal(int goalstate);
void		trap_BotEmptyGoalStack(int goalstate);
void		trap_BotDumpAvoidGoals(int goalstate);
void		trap_BotDumpGoalStack(int goalstate);
void		trap_BotGoalName(int number, char *name, int size);
int			trap_BotGetTopGoal(int goalstate, void /* struct bot_goal_s */ *goal);
int			trap_BotGetSecondGoal(int goalstate, void /* struct bot_goal_s */ *goal);
int			trap_BotChooseLTGItem(int goalstate, vec3_t origin, int *inventory, int travelflags);
int			trap_BotChooseNBGItem(int goalstate, vec3_t origin, int *inventory, int travelflags, void /* struct bot_goal_s */ *ltg, float maxtime);
int			trap_BotTouchingGoal(vec3_t origin, void /* struct bot_goal_s */ *goal);
int			trap_BotItemGoalInVisButNotVisible(int viewer, vec3_t eye, vec3_t viewangles, void /* struct bot_goal_s */ *goal);
int			trap_BotGetNextCampSpotGoal(int num, void /* struct bot_goal_s */ *goal);
int			trap_BotGetLevelItemGoal(int index, char *classname, void /* struct bot_goal_s */ *goal);
float		trap_BotAvoidGoalTime(int goalstate, int number);
void		trap_BotSetAvoidGoalTime(int goalstate, int number, float avoidtime);
void		trap_BotUpdateEntityItems(void);
int			trap_BotLoadItemWeights(int goalstate, char *filename);
int			trap_BotAllocGoalState(int state);
void		trap_BotFreeGoalState(int handle);
void		trap_BotResetMoveState(int movestate);
void		trap_BotMoveToGoal(void /* struct bot_moveresult_s */ *result, int movestate, void /* struct bot_goal_s */ *goal, int travelflags);
int			trap_BotMoveInDirection(int movestate, vec3_t dir, float speed, int type);
void		trap_BotResetAvoidReach(int movestate);
void		trap_BotResetLastAvoidReach(int movestate);
int			trap_BotMovementViewTarget(int movestate, void /* struct bot_goal_s */ *goal, int travelflags, float lookahead, vec3_t target);
int			trap_BotPredictVisiblePosition(vec3_t origin, int areanum, void /* struct bot_goal_s */ *goal, int travelflags, vec3_t target);
int			trap_BotAllocMoveState(void);
void		trap_BotFreeMoveState(int handle);
void		trap_BotInitMoveState(int handle, void /* struct bot_initmove_s */ *initmove);
void		trap_BotAddAvoidSpot(int movestate, vec3_t origin, float radius, int type);
int			trap_BotLoadWeaponWeights(int weaponstate, char *filename);
int			trap_BotAllocWeaponState(void);
void		trap_BotFreeWeaponState(int weaponstate);
void		trap_BotResetWeaponState(int weaponstate);
int			trap_AAS_AlternativeRouteGoals(vec3_t start, int startareanum, vec3_t goal, int goalareanum, int travelflags, void /*struct aas_altroutegoal_s*/ *altroutegoals, int maxaltroutegoals, int type);
int			trap_AAS_PredictRoute(void /*struct aas_predictroute_s*/ *route, int areanum, vec3_t origin, int goalareanum, int travelflags, int maxareas, int maxtime, int stopevent, int stopcontents, int stoptfl, int stopareanum);
int			trap_AAS_PointReachabilityAreaIndex(vec3_t point);
