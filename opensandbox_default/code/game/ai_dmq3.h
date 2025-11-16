// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

//setup the deathmatch AI
void BotSetupDeathmatchAI(void);
//shutdown the deathmatch AI
void BotShutdownDeathmatchAI(void);
//let the bot live within it's deathmatch AI net
void BotDeathmatchAI(bot_state_t *bs, float thinktime);
//choose a weapon
void BotChooseWeapon(bot_state_t *bs);
//setup movement stuff
void BotSetupForMovement(bot_state_t *bs);
//update the inventory
void BotUpdateInventory(bot_state_t *bs);
//update the inventory during battle
void BotUpdateBattleInventory(bot_state_t *bs, int enemy);
//use holdable items during battle
void BotBattleUseItems(bot_state_t *bs);
//return true if the bot is dead
qboolean BotIsDead(bot_state_t *bs);
//returns true if the bot is in observer mode
qboolean BotIsObserver(bot_state_t *bs);
//returns true if the bot is in the intermission
qboolean BotIntermission(bot_state_t *bs);
//returns true if the bot is in lava or slime
qboolean BotInLavaOrSlime(bot_state_t *bs);
//returns true if the entity is dead
qboolean EntityIsDead(aas_entityinfo_t *entinfo);
//returns true if the entity is invisible
qboolean EntityIsInvisible(aas_entityinfo_t *entinfo);
//returns true if the entity is shooting
qboolean EntityIsShooting(aas_entityinfo_t *entinfo);
//returns true if this entity has the kamikaze
qboolean EntityHasKamikaze(aas_entityinfo_t *entinfo);
// set a user info key/value pair
void BotSetUserInfo(bot_state_t *bs, char *key, char *value);
// set the team status (offense, defense etc.)
void BotSetTeamStatus(bot_state_t *bs);
//returns the name of the client
char *ClientName(int client, char *name, int size);
//returns an simplyfied client name
char *EasyClientName(int client, char *name, int size);
//returns the skin used by the client
char *ClientSkin(int client, char *skin, int size);
// returns the appropriate synonym context for the current game type and situation
int BotSynonymContext(bot_state_t *bs);
// set last ordered task
int BotSetLastOrderedTask(bot_state_t *bs);
// selection of goals for teamplay
void BotTeamGoals(bot_state_t *bs, int retreat);
//returns true if the bot wants to retreat
int BotWantsToRetreat(bot_state_t *bs);
//returns true if the bot wants to chase
int BotWantsToChase(bot_state_t *bs);
//returns true if the bot wants to help
int BotWantsToHelp(bot_state_t *bs);
//returns true if the bot can and wants to rocketjump
int BotCanAndWantsToRocketJump(bot_state_t *bs);
//returns true if the bot wants to and goes camping
int BotWantsToCamp(bot_state_t *bs);
//the bot will perform attack movements
bot_moveresult_t BotAttackMove(bot_state_t *bs, int tfl);
//returns true if the bot and the entity are in the same team
int BotSameTeam(bot_state_t *bs, int entnum);
//returns true if teamplay is on
int TeamPlayIsOn(void);
// returns the client number of the team mate flag carrier (-1 if none)
int BotTeamFlagCarrier(bot_state_t *bs);
//returns visible team mate flag carrier if available
int BotTeamFlagCarrierVisible(bot_state_t *bs);
//returns visible enemy flag carrier if available
int BotEnemyFlagCarrierVisible(bot_state_t *bs);
//get the number of visible teammates and enemies
void BotVisibleTeamMatesAndEnemies(bot_state_t *bs, int *teammates, int *enemies, float range);
//returns true if within the field of vision for the given angles
qboolean InFieldOfVision(vec3_t viewangles, float fov, vec3_t angles);
//returns true and sets the .enemy field when an enemy is found
int BotFindEnemy(bot_state_t *bs, int curenemy);
//returns a roam goal
void BotRoamGoal(bot_state_t *bs, vec3_t goal);
//returns entity visibility in the range [0, 1]
float BotEntityVisible(int viewer, vec3_t eye, vec3_t viewangles, float fov, int ent);
//the bot will aim at the current enemy
void BotAimAtEnemy(bot_state_t *bs);
//check if the bot should attack
void BotCheckAttack(bot_state_t *bs);
int ClientOnSameTeamFromName(bot_state_t *bs, char *name);
int BotPointAreaNum(vec3_t origin);

//ctf flags
#define CTF_FLAG_NONE		0
#define CTF_FLAG_RED		1
#define CTF_FLAG_BLUE		2
//CTF skins
#define CTF_SKIN_REDTEAM	"red"
#define CTF_SKIN_BLUETEAM	"blue"

extern int gametype;		//game type
extern int maxclients;		//maximum number of clients

extern cvar_t bot_grapple;
extern cvar_t bot_rocketjump;
extern cvar_t bot_fastchat;
extern cvar_t bot_nochat;
extern cvar_t bot_testrchat;

extern bot_goal_t ctf_redflag;
extern bot_goal_t ctf_blueflag;
extern bot_goal_t ctf_neutralflag;
extern bot_goal_t redobelisk;
extern bot_goal_t blueobelisk;
extern bot_goal_t neutralobelisk;
