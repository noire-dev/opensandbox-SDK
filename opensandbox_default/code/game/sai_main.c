// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// OpenSandbox — GPLv2; see LICENSE for details.

#include "g_local.h"

#include "botlib.h"		//bot lib interface

#include "ai_main.h"
#include "ai_dmq3.h"
#include "ai_chat.h"
#include "ai_cmd.h"
#include "ai_dmnet.h"

#include "chars.h"
#include "inv.h"
#include "syn.h"

#define MAX_PATH		144

#include "sai_local.h"

sai_state_t	*sai_states[MAX_CLIENTS];

#define SAI_FRAMETIME 300
#define SAI_THINKTIME 300

float sai_floattime;
int sai_numbots;
int sai_lastframetime;
float sai_updatetime;

int sai_max_bspmodelindex;

void SAI_ThinkQueue(void) {
	int i, botnum = 0;

    G_Printf("SAI: ThinkQueue\n");

	for( i = 0; i < MAX_CLIENTS; i++ ) {
		if( !sai_states[i] || !sai_states[i]->inuse ) continue;
		sai_states[i]->botthink_residual = SAI_THINKTIME * botnum / sai_numbots;
		botnum++;
	}
}

int SAI_InitLibrary(void) {
	trap_BotLibVarSet("maxclients", va("%i", MAX_CLIENTS));
	trap_BotLibVarSet("maxentities", va("%i", MAX_GENTITIES));
	trap_BotLibVarSet("sv_mapChecksum", cvarString("sv_mapChecksum"));
	trap_BotLibVarSet("g_gametype", cvarString("g_gametype"));
	trap_BotLibVarSet("nochat", cvarString("bot_nochat"));
	trap_BotLibVarSet("basedir", cvarString("fs_basepath"));
	trap_BotLibDefine("MISSIONPACK");
	return trap_BotLibSetup();
}

int SAI_Setup( int restart ) {
	int			errnum;

	cvarRegister("bot_thinktime", "100", CVAR_CHEAT);
	cvarRegister("bot_memorydump", "0", CVAR_CHEAT);
	cvarRegister("bot_saveroutingcache", "0", CVAR_CHEAT);
	cvarRegister("bot_pause", "0", CVAR_CHEAT);
	cvarRegister("bot_report", "0", CVAR_CHEAT);
	cvarRegister("bot_testsolid", "0", CVAR_CHEAT);
	cvarRegister("bot_testclusters", "0", CVAR_CHEAT);
	cvarRegister("bot_developer", "0", CVAR_CHEAT);

	//if the game is restarted for a tournament
	if (restart) {
		return qtrue;
	}

	//initialize the bot states
	memset( sai_states, 0, sizeof(sai_states) );

	errnum = SAI_InitLibrary();
	if (errnum != BLERR_NOERROR) return qfalse;
	return qtrue;
}

void SAI_ResetState(sai_state_t *bs) {
	int client, entitynum, inuse;
	int movestate, goalstate, chatstate, weaponstate;
	bot_settings_t settings;
	int character;
	playerState_t ps;							//current player state
	float entergame_time;

	//save some things that should not be reset here
	memcpy(&settings, &bs->settings, sizeof(bot_settings_t));
	memcpy(&ps, &bs->cur_ps, sizeof(playerState_t));
	inuse = bs->inuse;
	client = bs->client;
	entitynum = bs->entitynum;
	character = bs->character;
	movestate = bs->ms;
	goalstate = bs->gs;
	chatstate = bs->cs;
	weaponstate = bs->ws;
	entergame_time = bs->entergame_time;

	//reset the whole state
	memset(bs, 0, sizeof(sai_state_t));
	//copy back some state stuff that should not be reset
	bs->ms = movestate;
	bs->gs = goalstate;
	bs->cs = chatstate;
	bs->ws = weaponstate;
	memcpy(&bs->cur_ps, &ps, sizeof(playerState_t));
	memcpy(&bs->settings, &settings, sizeof(bot_settings_t));
	bs->inuse = inuse;
	bs->client = client;
	bs->entitynum = entitynum;
	bs->character = character;
	bs->entergame_time = entergame_time;
	//reset several states
	if (bs->ms) trap_BotResetMoveState(bs->ms);
	if (bs->gs) trap_BotResetGoalState(bs->gs);
	if (bs->ws) trap_BotResetWeaponState(bs->ws);
	if (bs->gs) trap_BotResetAvoidGoals(bs->gs);
	if (bs->ms) trap_BotResetAvoidReach(bs->ms);
}

void SAI_SetupDeathmatchAI(void) {
	int ent, modelnum;
	char model[128];

	gametype = cvarInt("g_gametype");
	maxclients = cvarInt("g_maxClients");

	cvarRegister("bot_rocketjump", "1", 0);
	cvarRegister("bot_grapple", "0", 0);
	cvarRegister("bot_fastchat", "0", 0);
	cvarRegister("bot_nochat", "0", 0);
	cvarRegister("bot_testrchat", "0", 0);
	cvarRegister("bot_predictobstacles", "1", 0);

	sai_max_bspmodelindex = 0;
	for (ent = trap_AAS_NextBSPEntity(0); ent; ent = trap_AAS_NextBSPEntity(ent)) {
		if (!trap_AAS_ValueForBSPEpairKey(ent, "model", model, sizeof(model))) continue;
		if (model[0] == '*') {
			modelnum = atoi(model+1);
			if (modelnum > sai_max_bspmodelindex)
				sai_max_bspmodelindex = modelnum;
		}
	}
}

int SAI_LoadMap( int restart ) {
	int			i;

	if (!restart) {
		trap_BotLibLoadMap( cvarString("sv_mapname") );
	}

	for (i = 0; i < MAX_CLIENTS; i++) {
		if (sai_states[i] && sai_states[i]->inuse) {
			SAI_ResetState( sai_states[i] );
			sai_states[i]->setupcount = 4;
		}
	}

	SAI_SetupDeathmatchAI();

	return qtrue;
}

int SAI_SetupClient(int client, struct bot_settings_s *settings, qboolean restart) {
	char filename[MAX_PATH], name[MAX_PATH], gender[MAX_PATH];
	sai_state_t *bs;
	int errnum;
	char userinfo[MAX_INFO_STRING];

	if (!sai_states[client]) sai_states[client] = G_Alloc(sizeof(sai_state_t));
	bs = sai_states[client];

	if (bs && bs->inuse) {
		G_Printf("BotAISetupClient: client %d already setup\n", client);
		return qfalse;
	}

	if (!trap_AAS_Initialized()) {
		BotAI_Print(PRT_FATAL, "AAS not initialized\n");
		return qfalse;
	}

	//load the bot character
	bs->character = trap_BotLoadCharacter(settings->characterfile, settings->skill);
	if (!bs->character) {
		BotAI_Print(PRT_FATAL, "couldn't load skill %f from %s\n", settings->skill, settings->characterfile);
		return qfalse;
	}
	//copy the settings
	memcpy(&bs->settings, settings, sizeof(bot_settings_t));
	//allocate a goal state
	bs->gs = trap_BotAllocGoalState(client);
	//load the item weights
	trap_Characteristic_String(bs->character, CHARACTERISTIC_ITEMWEIGHTS, filename, MAX_PATH);
	errnum = trap_BotLoadItemWeights(bs->gs, filename);
	if (errnum != BLERR_NOERROR) {
		trap_BotFreeGoalState(bs->gs);
		return qfalse;
	}
	//allocate a weapon state
	bs->ws = trap_BotAllocWeaponState();
	//load the weapon weights
	trap_Characteristic_String(bs->character, CHARACTERISTIC_WEAPONWEIGHTS, filename, MAX_PATH);
	errnum = trap_BotLoadWeaponWeights(bs->ws, filename);
	if (errnum != BLERR_NOERROR) {
		trap_BotFreeGoalState(bs->gs);
		trap_BotFreeWeaponState(bs->ws);
		return qfalse;
	}
	//allocate a chat state
	bs->cs = trap_BotAllocChatState();
	//load the chat file
	trap_Characteristic_String(bs->character, CHARACTERISTIC_CHAT_FILE, filename, MAX_PATH);
	trap_Characteristic_String(bs->character, CHARACTERISTIC_CHAT_NAME, name, MAX_PATH);
	errnum = trap_BotLoadChatFile(bs->cs, filename, name);
	if (errnum != BLERR_NOERROR) {
		trap_BotFreeChatState(bs->cs);
		trap_BotFreeGoalState(bs->gs);
		trap_BotFreeWeaponState(bs->ws);
		return qfalse;
	}
	//get the gender characteristic
	trap_Characteristic_String(bs->character, CHARACTERISTIC_GENDER, gender, MAX_PATH);
	//set the chat gender
	if (*gender == 'f' || *gender == 'F') trap_BotSetChatGender(bs->cs, CHAT_GENDERFEMALE);
	else if (*gender == 'm' || *gender == 'M') trap_BotSetChatGender(bs->cs, CHAT_GENDERMALE);
	else trap_BotSetChatGender(bs->cs, CHAT_GENDERLESS);

	bs->inuse = qtrue;
	bs->client = client;
	bs->entitynum = client;
	bs->setupcount = 4;
	bs->entergame_time = FloatTime();
	bs->ms = trap_BotAllocMoveState();
	bs->walker = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_WALKER, 0, 1);

	trap_GetUserinfo(bs->client, userinfo, sizeof(userinfo));
	bs->npcType = atoi(Info_ValueForKey(userinfo, "npcType"));

	sai_numbots++;

	//NOTE: reschedule the bot thinking
	SAI_ThinkQueue();

	//bot has been setup succesfully
	return qtrue;
}

void SAI_RegularUpdate(void) {
	if (sai_updatetime < FloatTime()) {
		trap_BotUpdateEntityItems();
		sai_updatetime = FloatTime() + 0.3;
	}
}

void SAI_UpdateInventory(sai_state_t *bs) {
	gentity_t *ent;
	int i;
	
	//weapons
	for(i = 1; i < WEAPONS_NUM; i++) {
		ent = g_entities + bs->client;
		bs->swep_list[i] = ent->swep_list[i];
		bs->swep_ammo[i] = ent->swep_ammo[i];
	}
}

void SAI_InputToUserCommand(bot_input_t *bi, usercmd_t *ucmd, int delta_angles[3], int time) {
	vec3_t angles, forward, right;
	short temp;
	int j;

	//clear the whole structure
	memset(ucmd, 0, sizeof(usercmd_t));
	
	ucmd->serverTime = time;

	if (bi->actionflags & ACTION_DELAYEDJUMP) {
		bi->actionflags |= ACTION_JUMP;
		bi->actionflags &= ~ACTION_DELAYEDJUMP;
	}
	//set the buttons
	ucmd->buttons = BUTTON_ATTACK;
	if (bi->actionflags & ACTION_RESPAWN) ucmd->buttons = BUTTON_ATTACK;
	if (bi->actionflags & ACTION_ATTACK) ucmd->buttons |= BUTTON_ATTACK;
	if (bi->actionflags & ACTION_TALK) ucmd->buttons |= BUTTON_TALK;
	if (bi->actionflags & ACTION_GESTURE) ucmd->buttons |= BUTTON_GESTURE;
	if (bi->actionflags & ACTION_USE) ucmd->buttons |= BUTTON_USE_HOLDABLE;
	if (bi->actionflags & ACTION_WALK) ucmd->buttons |= BUTTON_WALKING;
	if (bi->actionflags & ACTION_AFFIRMATIVE) ucmd->buttons |= BUTTON_AFFIRMATIVE;
	if (bi->actionflags & ACTION_NEGATIVE) ucmd->buttons |= BUTTON_NEGATIVE;
	if (bi->actionflags & ACTION_GETFLAG) ucmd->buttons |= BUTTON_GETFLAG;
	if (bi->actionflags & ACTION_GUARDBASE) ucmd->buttons |= BUTTON_GUARDBASE;
	if (bi->actionflags & ACTION_PATROL) ucmd->buttons |= BUTTON_PATROL;
	if (bi->actionflags & ACTION_FOLLOWME) ucmd->buttons |= BUTTON_FOLLOWME;

	ucmd->weapon = bi->weapon;
	ucmd->angles[PITCH] = ANGLE2SHORT(bi->viewangles[PITCH]);
	ucmd->angles[YAW] = ANGLE2SHORT(bi->viewangles[YAW]);
	ucmd->angles[ROLL] = ANGLE2SHORT(bi->viewangles[ROLL]);
	//subtract the delta angles
	for (j = 0; j < 3; j++) {
		temp = ucmd->angles[j] - delta_angles[j];
		ucmd->angles[j] = temp;
	}

	if (bi->dir[2]) angles[PITCH] = bi->viewangles[PITCH];
	else angles[PITCH] = 0;
	angles[YAW] = bi->viewangles[YAW];
	angles[ROLL] = 0;
	AngleVectors(angles, forward, right, NULL);
	//bot input speed is in the range [0, 400]
	bi->speed = bi->speed * 127 / 400;
	//set the view independent movement
	ucmd->forwardmove = DotProduct(forward, bi->dir) * bi->speed;
	ucmd->rightmove = DotProduct(right, bi->dir) * bi->speed;
	ucmd->upmove = abs(forward[2]) * bi->dir[2] * bi->speed;
	//normal keyboard movement
	if (bi->actionflags & ACTION_MOVEFORWARD) ucmd->forwardmove += 127;
	if (bi->actionflags & ACTION_MOVEBACK) ucmd->forwardmove -= 127;
	if (bi->actionflags & ACTION_MOVELEFT) ucmd->rightmove -= 127;
	if (bi->actionflags & ACTION_MOVERIGHT) ucmd->rightmove += 127;
	//jump/moveup
	if (bi->actionflags & ACTION_JUMP) ucmd->upmove += 127;
	//crouch/movedown
	if (bi->actionflags & ACTION_CROUCH) ucmd->upmove -= 127;
}

void SAI_ChangeViewAngles(sai_state_t *bs, float thinktime) {
	float diff, anglespeed;
	int i;

	if (bs->ideal_viewangles[PITCH] > 180) bs->ideal_viewangles[PITCH] -= 360;

	for (i = 0; i < 2; i++) {
		//smooth slowdown view model
		diff = abs(AngleDifference(bs->viewangles[i], bs->ideal_viewangles[i]));
		anglespeed = diff * 0.05f;
		if (anglespeed > 360) anglespeed = 360;
		bs->viewangles[i] = BotChangeViewAngle(bs->viewangles[i], bs->ideal_viewangles[i], anglespeed);
	}
	//bs->viewangles[PITCH] = 0;
	if (bs->viewangles[PITCH] > 180) bs->viewangles[PITCH] -= 360;
	//elementary action: view
	trap_EA_View(bs->client, bs->viewangles);
}

void SAI_UpdateInput(sai_state_t *bs, int time, int elapsed_time) {
	bot_input_t bi;
	int j;

    G_Printf("SAI: UpdateInput \n");

	//add the delta angles to the bot's current view angles
	for (j = 0; j < 3; j++) {
		bs->viewangles[j] = AngleMod(bs->viewangles[j] + SHORT2ANGLE(bs->cur_ps.delta_angles[j]));
	}
	//change the bot view angles
	SAI_ChangeViewAngles(bs, (float) elapsed_time / 1000);
	//retrieve the bot input
	trap_EA_GetInput(bs->client, (float) time / 1000, &bi);
	//respawn hack
	if (bi.actionflags & ACTION_RESPAWN) {
		if (bs->lastucmd.buttons & BUTTON_ATTACK) bi.actionflags &= ~(ACTION_RESPAWN|ACTION_ATTACK);
	}
	//convert the bot input to a usercmd
	SAI_InputToUserCommand(&bi, &bs->lastucmd, bs->cur_ps.delta_angles, time);
	//subtract the delta angles
	for (j = 0; j < 3; j++) {
		bs->viewangles[j] = AngleMod(bs->viewangles[j] - SHORT2ANGLE(bs->cur_ps.delta_angles[j]));
	}
}

qboolean SAI_InLavaOrSlime(sai_state_t *bs) {
	vec3_t feet;

	VectorCopy(bs->origin, feet);
	feet[2] -= 23;
	return (trap_AAS_PointContents(feet) & (CONTENTS_LAVA|CONTENTS_SLIME));
}

void SAI_AimAtEnemy(sai_state_t *bs) {
	vec3_t dir;
	aas_entityinfo_t entinfo;
	
	if (bs->enemy < 0) return;
	
	trap_AAS_EntityInfo(bs->enemy, &entinfo);
	VectorSubtract(entinfo.origin, bs->eye, dir);
	vectoangles(dir, bs->ideal_viewangles);
	VectorCopy(bs->ideal_viewangles, bs->viewangles);
	trap_EA_View(bs->client, bs->viewangles);
}

qboolean SAI_InFieldOfVision(vec3_t viewangles, float fov, vec3_t angles)
{
	int i;
	float diff, angle;

	for (i = 0; i < 2; i++) {
		angle = AngleMod(viewangles[i]);
		angles[i] = AngleMod(angles[i]);
		diff = angles[i] - angle;
		if (angles[i] > angle) {
			if (diff > 180.0) diff -= 360.0;
		}
		else {
			if (diff < -180.0) diff += 360.0;
		}
		if (diff > 0) {
			if (diff > fov * 0.5) return qfalse;
		}
		else {
			if (diff < -fov * 0.5) return qfalse;
		}
	}
	return qtrue;
}

void SAI_Trace(bsp_trace_t *bsptrace, vec3_t start, vec3_t mins, vec3_t maxs, vec3_t end, int passent, int contentmask) {
	trace_t trace;

	trap_Trace(&trace, start, mins, maxs, end, passent, contentmask);
	//copy the trace information
	bsptrace->allsolid = trace.allsolid;
	bsptrace->startsolid = trace.startsolid;
	bsptrace->fraction = trace.fraction;
	VectorCopy(trace.endpos, bsptrace->endpos);
	bsptrace->plane.dist = trace.plane.dist;
	VectorCopy(trace.plane.normal, bsptrace->plane.normal);
	bsptrace->plane.signbits = trace.plane.signbits;
	bsptrace->plane.type = trace.plane.type;
	bsptrace->surface.value = trace.surfaceFlags;
	bsptrace->ent = trace.entityNum;
	bsptrace->exp_dist = 0;
	bsptrace->sidenum = 0;
	bsptrace->contents = 0;
}

void SAI_CheckAttack(sai_state_t *bs) {
	float fov;
	int attackentity;
	bsp_trace_t bsptrace;
	//float selfpreservation;
	vec3_t forward, right, start, end, dir, angles;
	weaponinfo_t wi;
	bsp_trace_t trace;
	aas_entityinfo_t entinfo;
	vec3_t mins = {-8, -8, -8}, maxs = {8, 8, 8};

	attackentity = bs->enemy;
	trap_AAS_EntityInfo(attackentity, &entinfo);
	VectorSubtract(bs->aimtarget, bs->eye, dir);

	if (gameInfoWeapons[bs->weaponnum].wType == WT_MELEE) {
		if (VectorLengthSquared(dir) > Square(100)) return;
	}
	if (VectorLengthSquared(dir) < Square(100)) fov = 360;
	else fov = 50;

	vectoangles(dir, angles);
	if (!SAI_InFieldOfVision(bs->viewangles, fov, angles)) return;
	SAI_Trace(&bsptrace, bs->eye, NULL, NULL, bs->aimtarget, bs->client, CONTENTS_SOLID|CONTENTS_PLAYERCLIP);
	if (bsptrace.fraction < 1 && bsptrace.ent != attackentity) return;

	//get the start point shooting from
	VectorCopy(bs->origin, start);
	start[2] += bs->cur_ps.viewheight;
	AngleVectors(bs->viewangles, forward, right, NULL);
	start[0] += forward[0] * right[0];
	start[1] += forward[1] * right[1];
	start[2] += forward[2] * right[2];
	//end point aiming at
	VectorMA(start, 1000, forward, end);
	//a little back to make sure not inside a very close enemy
	VectorMA(start, -12, forward, start);
	SAI_Trace(&trace, bs->origin, mins, maxs, end, bs->entitynum, MASK_SHOT);
	
	//if fire has to be release to activate weapon
	if (wi.flags & WFL_FIRERELEASED) {
		if (bs->flags & BFL_ATTACKED) {
			trap_EA_Attack(bs->client);
		}
	} else {
		trap_EA_Attack(bs->client);
	}
	bs->flags ^= BFL_ATTACKED;
}

void SAI_SetupMovement(sai_state_t *bs) {
	bot_initmove_t initmove;

	memset(&initmove, 0, sizeof(bot_initmove_t));
	VectorCopy(bs->cur_ps.origin, initmove.origin);
	VectorCopy(bs->cur_ps.velocity, initmove.velocity);
	VectorClear(initmove.viewoffset);
	initmove.viewoffset[2] += bs->cur_ps.viewheight;
	initmove.entitynum = bs->entitynum;
	initmove.client = bs->client;
	initmove.thinktime = bs->thinktime;
	
	//set the onground flag
	if (bs->cur_ps.groundEntityNum != ENTITYNUM_NONE) initmove.or_moveflags |= MFL_ONGROUND;
	
	//set the teleported flag
	if ((bs->cur_ps.pm_flags & PMF_TIME_KNOCKBACK) && (bs->cur_ps.pm_time > 0)) {
		initmove.or_moveflags |= MFL_TELEPORTED;
	}
	
	//set the waterjump flag
	if ((bs->cur_ps.pm_flags & PMF_TIME_WATERJUMP) && (bs->cur_ps.pm_time > 0)) {
		initmove.or_moveflags |= MFL_WATERJUMP;
	}
	
	//set presence type
	if (bs->cur_ps.pm_flags & PMF_DUCKED) initmove.presencetype = PRESENCE_CROUCH;
	else initmove.presencetype = PRESENCE_NORMAL;

	if (bs->walker > 0.5) initmove.or_moveflags |= MFL_WALK;
	
	VectorCopy(bs->viewangles, initmove.viewangles);
	trap_BotInitMoveState(bs->ms, &initmove);
}

void SAI_AttackMove(sai_state_t *bs, int tfl) {
	int attackentity;
	float dist;
	float attack_dist, attack_range;
	vec3_t forward, backward, sideward, up = {0, 0, 1};
	bot_moveresult_t moveresult;
	bot_goal_t goal;

	attackentity = bs->enemy;
	
	if (bs->npcType == NT_NEXTBOT) {
		//create the chase goal
		goal.entitynum = attackentity;
		goal.areanum = bs->lastenemyareanum;
		VectorCopy(bs->lastenemyorigin, goal.origin);
		VectorSet(goal.mins, -8, -8, -8);
		VectorSet(goal.maxs, 8, 8, 8);
		
		SAI_SetupMovement(bs);
        G_Printf("SAI: AttackMove after setup\n");
		trap_BotMoveToGoal(&moveresult, bs->ms, &goal, tfl);
		return;
	}
	G_Printf("SAI: AttackMove standard\n");
	memset(&moveresult, 0, sizeof(bot_moveresult_t));

	attack_dist = 140;
	attack_range = 40;
	
	SAI_SetupMovement(bs);

	VectorCopy(forward, sideward);
	VectorNormalize(sideward);

	//perform the movement
	if (trap_BotMoveInDirection(bs->ms, sideward, 400, MOVE_WALK)) {
	    G_Printf("SAI: AttackMove engine error\n");
	    return;
	}

	return;
}

int SAI_SameTeam(sai_state_t *bs, int entnum) {
	char info1[1024], info2[1024];

	if (bs->client < 0 || bs->client >= MAX_CLIENTS) return qfalse;
	if (entnum < 0 || entnum >= MAX_CLIENTS) return qfalse;

	trap_GetConfigstring(CS_PLAYERS+bs->client, info1, sizeof(info1));
	trap_GetConfigstring(CS_PLAYERS+entnum, info2, sizeof(info2));

	if ( gametype >= GT_TEAM && atoi(Info_ValueForKey(info1, "nt")) <= NT_PLAYER ) {
		if (atoi(Info_ValueForKey(info1, "t")) == atoi(Info_ValueForKey(info2, "t"))) return qtrue;
	} else {
		if(!BG_FactionShouldAttack(gameInfoNPCTypes[atoi(Info_ValueForKey(info1, "nt"))].faction, gameInfoNPCTypes[atoi(Info_ValueForKey(info2, "nt"))].faction)) return qtrue;
	}
	return qfalse;
}

float SAI_EntityVisible(int viewer, vec3_t eye, int ent) {
	bsp_trace_t trace;
	aas_entityinfo_t entinfo;

	trap_AAS_EntityInfo(ent, &entinfo);
	
	//trace from eye to ent origin
	SAI_Trace(&trace, eye, NULL, NULL, entinfo.origin, viewer, CONTENTS_SOLID|CONTENTS_PLAYERCLIP);

	if (trace.fraction >= 1 || trace.ent == ent) return 1.00;
	return 0.00;
}

int SAI_FindItem(sai_state_t *bs, int tfl, bot_goal_t *ltg, float range) {
	return trap_BotChooseNBGItem(bs->gs, bs->origin, bs->inventory, tfl, ltg, range);
}

int SAI_FindEnemy(sai_state_t *bs, int curenemy) {
	int i;
	float vis;
	float squaredist, cursquaredist;
	aas_entityinfo_t entinfo, curenemyinfo;
	vec3_t dir;

	if (curenemy >= 0) {
		trap_AAS_EntityInfo(curenemy, &curenemyinfo);
		VectorSubtract(curenemyinfo.origin, bs->origin, dir);
		cursquaredist = VectorLengthSquared(dir);
	} else {
		cursquaredist = 0;
	}

	for (i = 0; i < maxclients && i < MAX_CLIENTS; i++) {
		if (i == bs->client) continue;  //myself
		if (i == curenemy) continue;    //if it's the current enemy
		
		trap_AAS_EntityInfo(i, &entinfo);
		if (!entinfo.valid) continue;
		if (g_entities[entinfo.number].client->ps.pm_type == PM_DEAD || entinfo.number == bs->entitynum) continue;
		if (entinfo.powerups & (1 << PW_INVIS) && !(entinfo.flags & EF_FIRING)) continue;

		//calculate the distance towards the enemy
		VectorSubtract(entinfo.origin, bs->origin, dir);
		squaredist = VectorLengthSquared(dir);
		
		//if this enemy is further away than the current one
		if (curenemy >= 0 && squaredist > cursquaredist) continue;
		
		//if the bot has no
		if (squaredist > Square(16384.0)) continue;
		
		//if on the same team
		if (SAI_SameTeam(bs, i)) continue;
		
		//check if the enemy is visible
		if(bs->npcType != NT_NEXTBOT){
			vis = SAI_EntityVisible(bs->entitynum, bs->eye, i);
		} else {
			vis = 1;
		}
		if (vis <= 0) continue;
		
		//found an enemy
		bs->enemy = entinfo.number;
		if (curenemy >= 0) bs->enemysight_time = FloatTime() - 2;
		else bs->enemysight_time = FloatTime();
		bs->enemysuicide = qfalse;
		bs->enemydeath_time = 0;
		bs->enemyvisible_time = FloatTime();
		return qtrue;
	}
	return qfalse;
}

int sai_weapon[] = {
	WP_NONE,

	// Quake weapons here!
	WP_GRAPPLING_HOOK,
	WP_GAUNTLET,
	WP_GRENADE_LAUNCHER,
	WP_PROX_LAUNCHER,
	WP_MACHINEGUN,
	WP_LIGHTNING,
	WP_PLASMAGUN,
	WP_ROCKET_LAUNCHER,
	WP_SHOTGUN,
	WP_CHAINGUN,
	WP_NAILGUN,
	WP_RAILGUN,
	WP_BFG,

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
};

int SAI_ChooseWeapon(sai_state_t *bs) {
	int i, weaponID;
	for (i = WEAPONS_NUM - 1; i > 0; i--) {
		weaponID = sai_weapon[i];
		if(bs->swep_list[weaponID] == WS_HAVE && (bs->swep_ammo[weaponID] > 0 || bs->swep_ammo[weaponID] == -1)){
		if(weaponID != WP_NONE && weaponID != WP_GRAPPLING_HOOK && weaponID != WP_PHYSGUN && weaponID != WP_GRAVITYGUN && weaponID != WP_TOOLGUN && weaponID != WP_REGENERATOR){
			return weaponID;
		}
		}
	}
	return WP_NONE;
}

void SAI_SelectWeapon(sai_state_t *bs) {
	if (bs->weaponnum != SAI_ChooseWeapon(bs)) bs->weaponchange_time = FloatTime();
	bs->weaponnum = SAI_ChooseWeapon(bs);
	bs->cur_ps.weapon = bs->weaponnum;
	trap_EA_SelectWeapon(bs->client, bs->weaponnum);
}

void SAI_DeathmatchAI(sai_state_t *bs, float thinktime) {
	char buf[144];

	//if the bot has just been setup
	if (bs->setupcount > 0) {
		bs->setupcount--;
		if (bs->setupcount > 0) return;
		//set the team
		Com_sprintf(buf, sizeof(buf), "team %s", bs->settings.team);
		trap_EA_Command(bs->client, buf);
		bs->setupcount = 0;
		BotSetupAlternativeRouteGoals();
	}
	G_Printf("SAI: DeathmatchAI after cycle\n");
	//no ideal view set
	bs->flags &= ~BFL_IDEALVIEWSET;
	//
	if (!level.intermissiontime) SAI_UpdateInventory(bs);

	if(!bs->state) bs->state = AI_Seek;

	//execute AI node
	bs->state(bs);
}

int SAI_RunAI(int client, float thinktime) {
	sai_state_t *bs;
	int j;

	trap_EA_ResetInput(client);

	bs = sai_states[client];
	if (!bs || !bs->inuse) {
		BotAI_Print(PRT_FATAL, "BotAI: client %d is not setup\n", client);
		return qfalse;
	}

	//retrieve the current client state
	BotAI_GetClientState( client, &bs->cur_ps );

	//add the delta angles to the bot's current view angles
	for (j = 0; j < 3; j++) {
		bs->viewangles[j] = AngleMod(bs->viewangles[j] + SHORT2ANGLE(bs->cur_ps.delta_angles[j]));
	}
	//increase the local time of the bot
	bs->ltime += thinktime;
	//
	bs->thinktime = thinktime;
	//origin of the bot
	VectorCopy(bs->cur_ps.origin, bs->origin);
	//eye coordinates of the bot
	VectorCopy(bs->cur_ps.origin, bs->eye);
	bs->eye[2] += bs->cur_ps.viewheight;
	//get the area the bot is in
	bs->areanum = BotPointAreaNum(bs->origin);
	//the real AI
	G_Printf("SAI: DeathmatchAI\n");
	SAI_DeathmatchAI(bs, thinktime);
	//set the weapon selection every AI frame
	trap_EA_SelectWeapon(bs->client, bs->weaponnum);
	//subtract the delta angles
	for (j = 0; j < 3; j++) {
		bs->viewangles[j] = AngleMod(bs->viewangles[j] - SHORT2ANGLE(bs->cur_ps.delta_angles[j]));
	}
	//everything was ok
	return qtrue;
}

int SAI_Frame(int time) {
	int i;
	gentity_t	*ent;
	bot_entitystate_t state;
	int elapsed_time, thinktime;
	static int local_time;
	static int botlib_residual;
	
	elapsed_time = time - local_time;
	local_time = time;
	botlib_residual += elapsed_time;
	
	if (elapsed_time > SAI_THINKTIME) thinktime = elapsed_time;
	else thinktime = SAI_THINKTIME;

	// update bot state
	if ( botlib_residual >= thinktime ) {
		botlib_residual -= thinktime;

		trap_BotLibStartFrame((float) time / 1000);

		if (!trap_AAS_Initialized()) return qfalse;

		//update entities
		for (i = 0; i < MAX_GENTITIES; i++) {
			ent = &g_entities[i];
			//bypass ents
			if (!ent->inuse || !ent->r.linked || ent->r.svFlags & SVF_NOCLIENT || ent->s.eType == ET_MISSILE || ent->s.eType > ET_EVENTS || ent->r.contents == CONTENTS_TRIGGER) {
				trap_BotLibUpdateEntity(i, NULL);
				continue;
			}

			memset(&state, 0, sizeof(bot_entitystate_t));

			VectorCopy(ent->r.currentOrigin, state.origin);
			if (i < MAX_CLIENTS) {
				VectorCopy(ent->s.apos.trBase, state.angles);
			} else {
				VectorCopy(ent->r.currentAngles, state.angles);
			}
			VectorCopy(ent->s.origin2, state.old_origin);
			VectorCopy(ent->r.mins, state.mins);
			VectorCopy(ent->r.maxs, state.maxs);
			state.type = ent->s.eType;
			state.flags = ent->s.eFlags;
			if (ent->r.bmodel) state.solid = SOLID_BSP;
			else state.solid = SOLID_BBOX;
			state.groundent = ent->s.groundEntityNum;
			state.modelindex = ent->s.modelindex;
			state.modelindex2 = ent->s.modelindex2;
			state.frame = ent->s.frame;
			state.event = ent->s.event;
			state.eventParm = ent->s.eventParm;
			state.powerups = ent->s.powerups;
			state.legsAnim = ent->s.legsAnim;
			state.torsoAnim = ent->s.torsoAnim;
			state.weapon = ent->s.weapon;

			trap_BotLibUpdateEntity(i, &state);
		}

		SAI_RegularUpdate();
	}

	sai_floattime = trap_AAS_Time();

	// execute scheduled bot AI
	for( i = 0; i < MAX_CLIENTS; i++ ) {
		if( !sai_states[i] || !sai_states[i]->inuse ) continue;
		
		sai_states[i]->botthink_residual += elapsed_time;
		if ( sai_states[i]->botthink_residual >= thinktime ) {
			sai_states[i]->botthink_residual -= thinktime;

			if (!trap_AAS_Initialized()) return qfalse;

			if (g_entities[i].client->pers.connected == CON_CONNECTED) {
			    G_Printf("SAI: RunAI\n");
				SAI_RunAI(i, (float) thinktime / 1000);
				
				SAI_UpdateInput(sai_states[i], time, elapsed_time);
		        trap_BotUserCommand(sai_states[i]->client, &sai_states[i]->lastucmd);
			}
		}
	}

	return qtrue;
}
