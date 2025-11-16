// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// OpenSandbox — GPLv2; see LICENSE for details.

typedef struct sai_state_s {
	int inuse;										//true if this state is used by a bot client
	int botthink_residual;							//residual for the bot thinks
	int client;										//client number of the bot
	int entitynum;									//entity number of the bot
	int npcType;
	playerState_t cur_ps;							//current player state
	int last_eFlags;								//last ps flags
	usercmd_t lastucmd;								//usercmd from last frame
	int entityeventTime[MAX_GENTITIES];				//last entity event time
	//
	bot_settings_t settings;						//several bot settings
	int (*state)(struct sai_state_s *bs);			//current AI node
	float thinktime;								//time the bot thinks this frame
	vec3_t origin;									//origin of the bot
	vec3_t velocity;								//velocity of the bot
	int presencetype;								//presence type of the bot
	vec3_t eye;										//eye coordinates of the bot
	int areanum;									//the number of the area the bot is in
	int inventory[MAX_ITEMS];						//string with items amounts the bot has
	int tfl;										//the travel flags the bot uses
	int flags;										//several flags
	int respawn_wait;								//wait until respawned
	int lasthealth;									//health value previous frame
	int lastkilledplayer;							//last killed player
	int lastkilledby;								//player that last killed this bot
	int botdeathtype;								//the death type of the bot
	int enemydeathtype;								//the death type of the enemy
	int botsuicide;									//true when the bot suicides
	int enemysuicide;								//true when the enemy of the bot suicides
	int setupcount;									//true when the bot has just been setup
	int map_restart;									//true when the map is being restarted
	int entergamechat;								//true when the bot used an enter game chat
	int num_deaths;									//number of time this bot died
	int num_kills;									//number of kills of this bot
	int revenge_enemy;								//the revenge enemy
	int revenge_kills;								//number of kills the enemy made
	int lastframe_health;							//health value the last frame
	int lasthitcount;								//number of hits last frame
	int chatto;										//chat to all or team
	float walker;									//walker charactertic
	float ltime;									//local bot time
	float entergame_time;							//time the bot entered the game
	float ltg_time;									//long term goal time
	float nbg_time;									//nearby goal time
	float respawn_time;								//time the bot takes to respawn
	float respawnchat_time;							//time the bot started a chat during respawn
	float chase_time;								//time the bot will chase the enemy
	float enemyvisible_time;						//time the enemy was last visible
	float check_time;								//time to check for nearby items
	float stand_time;								//time the bot is standing still
	float lastchat_time;							//time the bot last selected a chat
	float kamikaze_time;							//time to check for kamikaze usage
	float invulnerability_time;						//time to check for invulnerability usage
	float standfindenemy_time;						//time to find enemy while standing
	float attackstrafe_time;						//time the bot is strafing in one dir
	float attackcrouch_time;						//time the bot will stop crouching
	float attackchase_time;							//time the bot chases during actual attack
	float attackjump_time;							//time the bot jumped during attack
	float enemysight_time;							//time before reacting to enemy
	float enemydeath_time;							//time the enemy died
	float enemyposition_time;						//time the position and velocity of the enemy were stored
	float defendaway_time;							//time away while defending
	float defendaway_range;							//max travel time away from defend area
	float rushbaseaway_time;						//time away from rushing to the base
	float attackaway_time;							//time away from attacking the enemy base
	float harvestaway_time;							//time away from harvesting
	float ctfroam_time;								//time the bot is roaming in ctf
	float killedenemy_time;							//time the bot killed the enemy
	float arrive_time;								//time arrived (at companion)
	float lastair_time;								//last time the bot had air
	float teleport_time;							//last time the bot teleported
	float camp_time;								//last time camped
	float camp_range;								//camp range
	float weaponchange_time;						//time the bot started changing weapons
	float firethrottlewait_time;					//amount of time to wait
	float firethrottleshoot_time;					//amount of time to shoot
	float notblocked_time;							//last time the bot was not blocked
	float blockedbyavoidspot_time;					//time blocked by an avoid spot
	float predictobstacles_time;					//last time the bot predicted obstacles
	int predictobstacles_goalareanum;				//last goal areanum the bot predicted obstacles for
	vec3_t aimtarget;
	vec3_t enemyvelocity;							//enemy velocity 0.5 secs ago during battle
	vec3_t enemyorigin;								//enemy origin 0.5 secs ago during battle
	//
	int kamikazebody;								//kamikaze body
	int proxmines[MAX_PROXMINES];
	int numproxmines;
	//
	int character;									//the bot character
	int ms;											//move state of the bot
	int gs;											//goal state of the bot
	int cs;											//chat state of the bot
	int ws;											//weapon state of the bot
	//
	int enemy;										//enemy entity number
	int lastenemyareanum;							//last reachability area the enemy was in
	vec3_t lastenemyorigin;							//last origin of the enemy in the reachability area
	int weaponnum;									//current weapon number
	vec3_t viewangles;								//current view angles
	vec3_t ideal_viewangles;						//ideal view angles
	vec3_t viewanglespeed;
	//
	int ltgtype;									//long term goal type
	// team goals
	int teammate;									//team mate involved in this team goal
	int decisionmaker;								//player who decided to go for this goal
	int ordered;									//true if ordered to do something
	float order_time;								//time ordered to do something
	int owndecision_time;							//time the bot made it's own decision
	bot_goal_t teamgoal;							//the team goal
	bot_goal_t altroutegoal;						//alternative route goal
	float reachedaltroutegoal_time;					//time the bot reached the alt route goal
	float teammessage_time;							//time to message team mates what the bot is doing
	float teamgoal_time;							//time to stop helping team mate
	float teammatevisible_time;						//last time the team mate was NOT visible
	int teamtaskpreference;							//team task preference
	// last ordered team goal
	int lastgoal_decisionmaker;
	int lastgoal_ltgtype;
	int lastgoal_teammate;
	bot_goal_t lastgoal_teamgoal;
	// for leading team mates
	int lead_teammate;								//team mate the bot is leading
	bot_goal_t lead_teamgoal;						//team goal while leading
	float lead_time;								//time leading someone
	float leadvisible_time;							//last time the team mate was visible
	float leadmessage_time;							//last time a messaged was sent to the team mate
	float leadbackup_time;							//time backing up towards team mate
	//
	char teamleader[32];							//netname of the team leader
	float askteamleader_time;						//time asked for team leader
	float becometeamleader_time;					//time the bot will become the team leader
	float teamgiveorders_time;						//time to give team orders
	float lastflagcapture_time;						//last time a flag was captured
	int numteammates;								//number of team mates
	int redflagstatus;								//0 = at base, 1 = not at base
	int blueflagstatus;								//0 = at base, 1 = not at base
	int neutralflagstatus;							//0 = at base, 1 = our team has flag, 2 = enemy team has flag, 3 = enemy team dropped the flag
	int flagstatuschanged;							//flag status changed
	int forceorders;								//true if forced to give orders
	int flagcarrier;								//team mate carrying the enemy flag
	int ctfstrategy;								//ctf strategy
	char subteam[32];								//sub team name
	float formation_dist;							//formation team mate intervening space
	char formation_teammate[16];					//netname of the team mate the bot uses for relative positioning
	float formation_angle;							//angle relative to the formation team mate
	vec3_t formation_dir;							//the direction the formation is moving in
	vec3_t formation_origin;						//origin the bot uses for relative positioning
	bot_goal_t formation_goal;						//formation goal

	bot_activategoal_t *activatestack;				//first activate goal on the stack
	bot_activategoal_t activategoalheap[MAX_ACTIVATESTACK];	//activate goal heap

	bot_waypoint_t *checkpoints;					//check points
	bot_waypoint_t *patrolpoints;					//patrol points
	bot_waypoint_t *curpatrolpoint;					//current patrol point the bot is going for
	int patrolflags;								//patrol flags
	int	swep_list[WEAPONS_NUM];						//OpenSandbox weapon system
	int	swep_ammo[WEAPONS_NUM];						//OpenSandbox ammo system
} sai_state_t;

// sai_state.c
int AI_Intermission(sai_state_t *bs);
int AI_Observer(sai_state_t *bs);
int AI_Respawn(sai_state_t *bs);
int AI_Battle(sai_state_t *bs);
int AI_Seek(sai_state_t *bs);
int AI_Stand(sai_state_t *bs);
