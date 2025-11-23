// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// OpenSandbox — GPLv2; see LICENSE for details.

#include "../qcommon/q_shared.h"
#include "g_local.h"
#include "ai_main.h"

#define AI_THINKTIME 300

bot_state_t* botstates[MAX_CLIENTS];
int numbots;
float floattime;
float regularupdate_time;

int gametype;    // game type
int maxclients;  // maximum number of clients

static void BotAI_Trace(bsp_trace_t* bsptrace, vec3_t start, vec3_t mins, vec3_t maxs, vec3_t end, int passent, int contentmask) {
	trace_t trace;

	trap_Trace(&trace, start, mins, maxs, end, passent, contentmask);
	// copy the trace information
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

static int BotAI_GetClientState(int clientNum, playerState_t* state) {
	gentity_t* ent;

	ent = &g_entities[clientNum];
	if(!ent->inuse) return qfalse;
	if(!ent->client) return qfalse;

	memcpy(state, &ent->client->ps, sizeof(playerState_t));
	return qtrue;
}

static qboolean EntityIsDead(aas_entityinfo_t* entinfo) {
	playerState_t ps;

	if(entinfo->number >= 0 && entinfo->number < MAX_CLIENTS) {
		BotAI_GetClientState(entinfo->number, &ps);
		if(ps.pm_type != PM_NORMAL) return qtrue;
	}
	return qfalse;
}

static qboolean EntityIsInvisible(aas_entityinfo_t* entinfo) {
	if(entinfo->powerups & (1 << PW_INVIS)) return qtrue;
	return qfalse;
}

static qboolean EntityIsShooting(aas_entityinfo_t* entinfo) {
	if(entinfo->flags & EF_FIRING) return qtrue;
	return qfalse;
}

static int BotPointAreaNum(vec3_t origin) {
	int areanum, numareas, areas[10];
	vec3_t end;

	areanum = trap_AAS_PointAreaNum(origin);
	if(areanum) return areanum;
	VectorCopy(origin, end);
	end[2] += 10;
	numareas = trap_AAS_TraceAreas(origin, end, areas, NULL, 10);
	if(numareas > 0) return areas[0];
	return 0;
}

static int weaponOrder[] = {
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

static int BotSelectWeapon(bot_state_t* bs) {
	int i, weaponID;
	for(i = WEAPONS_NUM - 1; i > 0; i--) {
		weaponID = weaponOrder[i];
		if(bs->swep_list[weaponID] == WS_HAVE && (bs->swep_ammo[weaponID] > 0 || bs->swep_ammo[weaponID] == -1)) {
			if(weaponID != WP_NONE && weaponID != WP_GRAPPLING_HOOK && weaponID != WP_PHYSGUN && weaponID != WP_GRAVITYGUN && weaponID != WP_TOOLGUN && weaponID != WP_REGENERATOR) {
				return weaponID;
			}
		}
	}
	return WP_NONE;
}

static void BotChooseWeapon(bot_state_t* bs) {
	bs->weaponnum = BotSelectWeapon(bs);
	bs->cur_ps.weapon = bs->weaponnum;
	trap_EA_SelectWeapon(bs->client, bs->weaponnum);
}

static void BotSetupForMovement(bot_state_t* bs) {
	bot_initmove_t initmove;

	memset(&initmove, 0, sizeof(bot_initmove_t));
	VectorCopy(bs->cur_ps.origin, initmove.origin);
	VectorCopy(bs->cur_ps.velocity, initmove.velocity);
	VectorClear(initmove.viewoffset);
	initmove.viewoffset[2] += bs->cur_ps.viewheight;
	initmove.entitynum = bs->entitynum;
	initmove.client = bs->client;
	initmove.thinktime = bs->thinktime;
	// set the onground flag
	if(bs->cur_ps.groundEntityNum != ENTITYNUM_NONE) initmove.or_moveflags |= MFL_ONGROUND;
	// set the waterjump flag
	if((bs->cur_ps.pm_flags & PMF_TIME_WATERJUMP) && (bs->cur_ps.pm_time > 0)) {
		initmove.or_moveflags |= MFL_WATERJUMP;
	}
	// set presence type
	if(bs->cur_ps.pm_flags & PMF_DUCKED)
		initmove.presencetype = PRESENCE_CROUCH;
	else
		initmove.presencetype = PRESENCE_NORMAL;

	VectorCopy(bs->viewangles, initmove.viewangles);
	trap_BotInitMoveState(bs->ms, &initmove);
}

static void BotUpdateInventory(bot_state_t* bs) {
	gentity_t* ent;
	int i;

	// weapons
	for(i = 1; i < WEAPONS_NUM; i++) {
		ent = g_entities + bs->client;
		bs->swep_list[i] = ent->swep_list[i];
		bs->swep_ammo[i] = ent->swep_ammo[i];
	}
}

static qboolean BotIsDead(bot_state_t* bs) { return (bs->cur_ps.pm_type == PM_DEAD); }

static qboolean BotIsObserver(bot_state_t* bs) {
	char buf[MAX_INFO_STRING];
	if(bs->cur_ps.pm_type == PM_SPECTATOR) return qtrue;
	trap_GetConfigstring(CS_PLAYERS + bs->client, buf, sizeof(buf));
	if(atoi(Info_ValueForKey(buf, "t")) == TEAM_SPECTATOR) return qtrue;
	return qfalse;
}

static qboolean BotIntermission(bot_state_t* bs) {
	if(level.intermissiontime) return qtrue;
	return (bs->cur_ps.pm_type == PM_FREEZE || bs->cur_ps.pm_type == PM_INTERMISSION);
}

static void BotAttackMove(bot_state_t* bs, int tfl) {
	bot_moveresult_t moveresult;
	bot_goal_t goal;

	goal.entitynum = bs->enemy;
	goal.areanum = bs->lastenemyareanum;
	VectorCopy(bs->lastenemyorigin, goal.origin);
	VectorSet(goal.mins, -8, -8, -8);
	VectorSet(goal.maxs, 8, 8, 8);

	BotSetupForMovement(bs);
	trap_BotMoveToGoal(&moveresult, bs->ms, &goal, tfl);
}

static int BotSameTeam(bot_state_t* bs, int entnum) {
	char info1[1024], info2[1024];

	if(bs->client < 0 || bs->client >= MAX_CLIENTS) return qfalse;

	if(entnum < 0 || entnum >= MAX_CLIENTS) return qfalse;

	trap_GetConfigstring(CS_PLAYERS + bs->client, info1, sizeof(info1));
	trap_GetConfigstring(CS_PLAYERS + entnum, info2, sizeof(info2));

	if(gametype >= GT_TEAM && atoi(Info_ValueForKey(info1, "nt")) <= NT_PLAYER) {
		if(atoi(Info_ValueForKey(info1, "t")) == atoi(Info_ValueForKey(info2, "t"))) return qtrue;
	} else {
		if(!BG_FactionShouldAttack(gameInfoNPCTypes[atoi(Info_ValueForKey(info1, "nt"))].faction, gameInfoNPCTypes[atoi(Info_ValueForKey(info2, "nt"))].faction)) return qtrue;
	}
	return qfalse;
}

static float BotEntityVisible(int viewer, vec3_t eye, int ent) {
	bsp_trace_t trace;
	aas_entityinfo_t entinfo;

	trap_AAS_EntityInfo(ent, &entinfo);
	BotAI_Trace(&trace, eye, NULL, NULL, entinfo.origin, viewer, CONTENTS_SOLID | CONTENTS_PLAYERCLIP);

	if(trace.fraction >= 1 || trace.ent == ent) return 1.00;
	return 0.00;
}

static int BotFindEnemy(bot_state_t* bs, int curenemy) {
	int i;
	float vis;
	float squaredist, cursquaredist;
	aas_entityinfo_t entinfo, curenemyinfo;
	vec3_t dir;

	if(curenemy >= 0) {
		trap_AAS_EntityInfo(curenemy, &curenemyinfo);
		VectorSubtract(curenemyinfo.origin, bs->origin, dir);
		cursquaredist = VectorLengthSquared(dir);
	} else {
		cursquaredist = 0;
	}

	for(i = 0; i < maxclients && i < MAX_CLIENTS; i++) {
		if(i == bs->client) continue;
		if(i == curenemy) continue;
		trap_AAS_EntityInfo(i, &entinfo);
		if(!entinfo.valid) continue;
		if(EntityIsDead(&entinfo) || entinfo.number == bs->entitynum) continue;
		if(EntityIsInvisible(&entinfo) && !EntityIsShooting(&entinfo)) continue;

		// calculate the distance towards the enemy
		VectorSubtract(entinfo.origin, bs->origin, dir);
		squaredist = VectorLengthSquared(dir);
		if(curenemy >= 0 && squaredist > cursquaredist) continue;

		if(squaredist > Square(16384.0)) continue;
		if(BotSameTeam(bs, i)) continue;

		if(bs->npcType != NT_NEXTBOT) {
			vis = BotEntityVisible(bs->entitynum, bs->eye, i);
		} else {
			vis = 1.00;
		}
		if(vis <= 0.00) continue;

		bs->enemy = entinfo.number;
		return qtrue;
	}
	return qfalse;
}

static void BotAimAtEnemy(bot_state_t* bs) {
	vec3_t dir;
	aas_entityinfo_t entinfo;

	if(bs->enemy < 0) return;

	trap_AAS_EntityInfo(bs->enemy, &entinfo);
	VectorSubtract(entinfo.origin, bs->eye, dir);
	vectoangles(dir, bs->ideal_viewangles);
	VectorCopy(bs->ideal_viewangles, bs->viewangles);
	trap_EA_View(bs->client, bs->viewangles);
}

static void BotAimAtMovementDirection(bot_state_t* bs) {
	bot_input_t bi;
	vec3_t temp_dir;

	trap_EA_GetInput(bs->client, (float)bs->thinktime / 1000, &bi);

	if(VectorLength(bi.dir) < 0.1f) return;
	VectorCopy(bi.dir, temp_dir);
	temp_dir[2] = 0;

	if(VectorNormalize(temp_dir) > 0) {
		vectoangles(temp_dir, bs->ideal_viewangles);

		bs->ideal_viewangles[PITCH] = bs->viewangles[PITCH];
		bs->ideal_viewangles[ROLL] = 0;
	}
}

static bot_goal_t AI_GoalFromRandomItem(bot_state_t* bs) {
	bot_goal_t result;
	gentity_t* item;

	memset(&result, 0, sizeof(bot_goal_t));

	item = FindRandomItem();
	if(item) {
		VectorCopy(item->r.currentOrigin, result.origin);
		result.areanum = BotPointAreaNum(item->r.currentOrigin);
		VectorSet(result.mins, -8, -8, -8);
		VectorSet(result.maxs, 8, 8, 8);
		result.entitynum = item->s.number;
	}

	return result;
}

static int AI_Battle(bot_state_t* bs) {
	int areanum;
	vec3_t target;
	aas_entityinfo_t entinfo;

	BotFindEnemy(bs, bs->enemy);

	if(bs->enemy < 0) return qfalse;

	trap_AAS_EntityInfo(bs->enemy, &entinfo);

	if(EntityIsInvisible(&entinfo) && !EntityIsShooting(&entinfo)) {
		if(random() < 0.2) return qfalse;
	}

	VectorCopy(entinfo.origin, target);

	areanum = BotPointAreaNum(target);
	VectorCopy(target, bs->lastenemyorigin);
	if(areanum) bs->lastenemyareanum = areanum;

	if(!BotEntityVisible(bs->entitynum, bs->eye, bs->enemy) && bs->npcType != NT_NEXTBOT) return qfalse;

	BotChooseWeapon(bs);
	BotAttackMove(bs, bs->tfl);
	BotAimAtEnemy(bs);
	trap_EA_Attack(bs->client);
	return qtrue;
}

static int AINode_Default(bot_state_t* bs) {
	bot_moveresult_t moveresult;

	if(BotIsObserver(bs) || BotIntermission(bs)) return qfalse;

	if(BotIsDead(bs)) {
		trap_BotResetMoveState(bs->ms);
		trap_EA_Attack(bs->client);
		return qfalse;
	}

	bs->tfl = TFL_DEFAULT;

	bs->enemy = -1;  // no enemy
	if(BotFindEnemy(bs, -1)) {
		AI_Battle(bs);
		return qfalse;
	}

	if(level.time > bs->item_searchtime || trap_BotTouchingGoal(bs->origin, &bs->maingoal)) {
		bs->maingoal = AI_GoalFromRandomItem(bs);
		bs->item_searchtime = level.time + 30000;
	}

	if(bs->maingoal.entitynum) {
		BotSetupForMovement(bs);
		trap_BotMoveToGoal(&moveresult, bs->ms, &bs->maingoal, bs->tfl);
		BotAimAtMovementDirection(bs);
	}
	return qtrue;
}

static void BotDeathmatchAI(bot_state_t* bs, float thinktime) {
	char buf[144];

	if(bs->setupcount > 0) {
		bs->setupcount--;
		if(bs->setupcount > 0) return;

		// set the team
		Com_sprintf(buf, sizeof(buf), "team %s", bs->settings.team);
		trap_EA_Command(bs->client, buf);
		bs->setupcount = 0;
	}

	if(!BotIntermission(bs)) BotUpdateInventory(bs);
	if(!bs->ainode) bs->ainode = AINode_Default;
	bs->ainode(bs);  // execute AI node

	if(!bs->inuse) return;
}

static void BotSetupDeathmatchAI(void) {
	gametype = cvarInt("g_gametype");
	maxclients = cvarInt("g_maxClients");
}

static float BotChangeViewAngle(float angle, float ideal_angle, float speed) {
	float move;

	angle = AngleMod(angle);
	ideal_angle = AngleMod(ideal_angle);
	if(angle == ideal_angle) return angle;
	move = ideal_angle - angle;
	if(ideal_angle > angle) {
		if(move > 180.0) move -= 360.0;
	} else {
		if(move < -180.0) move += 360.0;
	}
	if(move > 0) {
		if(move > speed) move = speed;
	} else {
		if(move < -speed) move = -speed;
	}
	return AngleMod(angle + move);
}

static void BotChangeViewAngles(bot_state_t* bs, float thinktime) {
	float diff, factor, maxchange, anglespeed;
	int i;

	if(bs->ideal_viewangles[PITCH] > 180) bs->ideal_viewangles[PITCH] -= 360;

	factor = 0.80f;
	maxchange = 200;

	maxchange *= thinktime;
	for(i = 0; i < 2; i++) {
		// smooth slowdown view model
		diff = abs(AngleDifference(bs->viewangles[i], bs->ideal_viewangles[i]));
		anglespeed = diff * factor;
		if(anglespeed > maxchange) anglespeed = maxchange;
		bs->viewangles[i] = BotChangeViewAngle(bs->viewangles[i], bs->ideal_viewangles[i], anglespeed);
	}
	if(bs->viewangles[PITCH] > 180) bs->viewangles[PITCH] -= 360;

	// elementary action: view
	trap_EA_View(bs->client, bs->viewangles);
}

static void BotInputToUserCommand(bot_input_t* bi, usercmd_t* ucmd, int delta_angles[3], int time) {
	vec3_t angles, forward, right;
	short temp;
	int j;

	memset(ucmd, 0, sizeof(usercmd_t));

	ucmd->serverTime = time;

	if(bi->actionflags & ACTION_DELAYEDJUMP) {
		bi->actionflags |= ACTION_JUMP;
		bi->actionflags &= ~ACTION_DELAYEDJUMP;
	}

	if(bi->actionflags & ACTION_ATTACK) ucmd->buttons |= BUTTON_ATTACK;
	if(bi->actionflags & ACTION_TALK) ucmd->buttons |= BUTTON_TALK;
	if(bi->actionflags & ACTION_GESTURE) ucmd->buttons |= BUTTON_GESTURE;
	if(bi->actionflags & ACTION_USE) ucmd->buttons |= BUTTON_USE_HOLDABLE;
	if(bi->actionflags & ACTION_WALK) ucmd->buttons |= BUTTON_WALKING;

	ucmd->weapon = bi->weapon;

	ucmd->angles[PITCH] = ANGLE2SHORT(bi->viewangles[PITCH]);
	ucmd->angles[YAW] = ANGLE2SHORT(bi->viewangles[YAW]);
	ucmd->angles[ROLL] = ANGLE2SHORT(bi->viewangles[ROLL]);

	for(j = 0; j < 3; j++) {
		temp = ucmd->angles[j] - delta_angles[j];
		ucmd->angles[j] = temp;
	}

	if(bi->dir[2])
		angles[PITCH] = bi->viewangles[PITCH];
	else
		angles[PITCH] = 0;

	angles[YAW] = bi->viewangles[YAW];
	angles[ROLL] = 0;
	AngleVectors(angles, forward, right, NULL);

	bi->speed = bi->speed * 127 / 400;
	ucmd->forwardmove = DotProduct(forward, bi->dir) * bi->speed;
	ucmd->rightmove = DotProduct(right, bi->dir) * bi->speed;
	ucmd->upmove = abs(forward[2]) * bi->dir[2] * bi->speed;

	if(bi->actionflags & ACTION_MOVEFORWARD) ucmd->forwardmove += 127;
	if(bi->actionflags & ACTION_MOVEBACK) ucmd->forwardmove -= 127;
	if(bi->actionflags & ACTION_MOVELEFT) ucmd->rightmove -= 127;
	if(bi->actionflags & ACTION_MOVERIGHT) ucmd->rightmove += 127;
	if(bi->actionflags & ACTION_JUMP) ucmd->upmove += 127;
	if(bi->actionflags & ACTION_CROUCH) ucmd->upmove -= 127;
}

static void BotUpdateInput(bot_state_t* bs, int time, int elapsed_time) {
	bot_input_t bi;
	int j;

	for(j = 0; j < 3; j++) {
		bs->viewangles[j] = AngleMod(bs->viewangles[j] + SHORT2ANGLE(bs->cur_ps.delta_angles[j]));
	}

	BotChangeViewAngles(bs, (float)elapsed_time / 1000);
	trap_EA_GetInput(bs->client, (float)time / 1000, &bi);

	if(bi.actionflags & ACTION_RESPAWN) {
		if(bs->lastucmd.buttons & BUTTON_ATTACK) bi.actionflags &= ~(ACTION_RESPAWN | ACTION_ATTACK);
	}

	BotInputToUserCommand(&bi, &bs->lastucmd, bs->cur_ps.delta_angles, time);

	for(j = 0; j < 3; j++) {
		bs->viewangles[j] = AngleMod(bs->viewangles[j] - SHORT2ANGLE(bs->cur_ps.delta_angles[j]));
	}
}

static void BotAIRegularUpdate(void) {
	if(regularupdate_time < floattime) {
		regularupdate_time = floattime + 0.3;
	}
}

static int BotAI(int client, float thinktime) {
	bot_state_t* bs;
	char buf[1024];
	int i;

	trap_EA_ResetInput(client);

	while(trap_BotGetServerCommand(client, buf, sizeof(buf))) {
		// NOTHING, WE JUST ANSWER TO EVERY COMMAND
	}

	bs = botstates[client];
	if(!bs || !bs->inuse) {
		G_Printf("BotAI: client %d is not setup\n", client);
		return qfalse;
	}

	// retrieve the current client state
	BotAI_GetClientState(client, &bs->cur_ps);

	// add the delta angles to the bot's current view angles
	for(i = 0; i < 3; i++) {
		bs->viewangles[i] = AngleMod(bs->viewangles[i] + SHORT2ANGLE(bs->cur_ps.delta_angles[i]));
	}

	// increase the local time of the bot
	bs->thinktime = thinktime;
	VectorCopy(bs->cur_ps.origin, bs->origin);
	VectorCopy(bs->cur_ps.origin, bs->eye);
	bs->eye[2] += bs->cur_ps.viewheight;
	bs->areanum = BotPointAreaNum(bs->origin);

	BotDeathmatchAI(bs, thinktime);
	trap_EA_SelectWeapon(bs->client, bs->weaponnum);

	// subtract the delta angles
	for(i = 0; i < 3; i++) {
		bs->viewangles[i] = AngleMod(bs->viewangles[i] - SHORT2ANGLE(bs->cur_ps.delta_angles[i]));
	}

	return qtrue;
}

static void BotScheduleBotThink(void) {
	int i, botnum;

	botnum = 0;

	for(i = 0; i < MAX_CLIENTS; i++) {
		if(!botstates[i] || !botstates[i]->inuse) continue;
		botstates[i]->botthink_residual = AI_THINKTIME * botnum / numbots;
		botnum++;
	}
}

int BotAISetupClient(int client, struct bot_settings_s* settings) {
	bot_state_t* bs;
	char userinfo[MAX_INFO_STRING];

	if(!botstates[client]) botstates[client] = G_Alloc(sizeof(bot_state_t));
	bs = botstates[client];

	if(bs && bs->inuse) {
		G_Printf("BotAISetupClient: client %d already setup\n", client);
		return qfalse;
	}

	if(!trap_AAS_Initialized()) {
		G_Printf("AAS not initialized\n");
		return qfalse;
	}

	bs->inuse = qtrue;
	bs->client = client;
	bs->entitynum = client;
	bs->setupcount = 4;
	bs->ms = trap_BotAllocMoveState();

	trap_GetUserinfo(bs->client, userinfo, sizeof(userinfo));
	bs->npcType = atoi(Info_ValueForKey(userinfo, "npcType"));

	numbots++;

	BotScheduleBotThink();  // NOTE: reschedule the bot thinking

	return qtrue;
}

int BotAIShutdownClient(int client, qboolean restart) {
	bot_state_t* bs;

	bs = botstates[client];
	if(!bs || !bs->inuse) return qfalse;

	trap_BotFreeMoveState(bs->ms);

	memset(bs, 0, sizeof(bot_state_t));
	bs->inuse = qfalse;
	numbots--;

	return qtrue;
}

void BotResetState(bot_state_t* bs) {
	int client, entitynum, inuse;
	int movestate;
	bot_settings_t settings;
	playerState_t ps;  // current player state

	// save some things that should not be reset here
	memcpy(&settings, &bs->settings, sizeof(bot_settings_t));
	memcpy(&ps, &bs->cur_ps, sizeof(playerState_t));
	inuse = bs->inuse;
	client = bs->client;
	entitynum = bs->entitynum;
	movestate = bs->ms;

	// reset the whole state
	memset(bs, 0, sizeof(bot_state_t));
	// copy back some state stuff that should not be reset
	bs->ms = movestate;
	memcpy(&bs->cur_ps, &ps, sizeof(playerState_t));
	memcpy(&bs->settings, &settings, sizeof(bot_settings_t));
	bs->inuse = inuse;
	bs->client = client;
	bs->entitynum = entitynum;
	// reset several states
	if(bs->ms) trap_BotResetMoveState(bs->ms);
}

int BotAILoadMap(int restart) {
	int i;

	if(!restart) trap_BotLibLoadMap(cvarString("sv_mapname"));

	for(i = 0; i < MAX_CLIENTS; i++) {
		if(botstates[i] && botstates[i]->inuse) {
			BotResetState(botstates[i]);
			botstates[i]->setupcount = 4;
		}
	}

	BotSetupDeathmatchAI();

	return qtrue;
}

int AI_Frame(int time) {
	int i;
	gentity_t* ent;
	bot_entitystate_t state;
	int elapsed_time, thinktime;
	static int local_time;
	static int botlib_residual;

	elapsed_time = time - local_time;
	local_time = time;
	botlib_residual += elapsed_time;

	if(elapsed_time > AI_THINKTIME)
		thinktime = elapsed_time;
	else
		thinktime = AI_THINKTIME;

	// update the bot library
	if(botlib_residual >= thinktime) {
		botlib_residual -= thinktime;

		trap_BotLibStartFrame((float)time / 1000);

		if(!trap_AAS_Initialized()) return qfalse;

		// update entities in the botlib
		for(i = 0; i < MAX_GENTITIES; i++) {
			ent = &g_entities[i];
			// bypass ents
			if(!ent->inuse || !ent->r.linked || ent->r.svFlags & SVF_NOCLIENT || ent->s.eType == ET_MISSILE || ent->s.eType > ET_EVENTS || ent->r.contents == CONTENTS_TRIGGER) {
				trap_BotLibUpdateEntity(i, NULL);
				continue;
			}

			memset(&state, 0, sizeof(bot_entitystate_t));

			VectorCopy(ent->r.currentOrigin, state.origin);
			if(i < MAX_CLIENTS) {
				VectorCopy(ent->s.apos.trBase, state.angles);
			} else {
				VectorCopy(ent->r.currentAngles, state.angles);
			}
			VectorCopy(ent->s.origin2, state.old_origin);
			VectorCopy(ent->r.mins, state.mins);
			VectorCopy(ent->r.maxs, state.maxs);
			state.type = ent->s.eType;
			state.flags = ent->s.eFlags;
			if(ent->r.bmodel)
				state.solid = SOLID_BSP;
			else
				state.solid = SOLID_BBOX;
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
		BotAIRegularUpdate();
	}

	floattime = trap_AAS_Time();

	// execute scheduled bot AI
	for(i = 0; i < MAX_CLIENTS; i++) {
		if(!botstates[i] || !botstates[i]->inuse) continue;
		botstates[i]->botthink_residual += elapsed_time;

		if(botstates[i]->botthink_residual >= thinktime) {
			botstates[i]->botthink_residual -= thinktime;
			if(!trap_AAS_Initialized()) return qfalse;
			if(g_entities[i].client->pers.connected == CON_CONNECTED) BotAI(i, (float)thinktime / 1000);
		}
	}

	// execute bot commands
	for(i = 0; i < MAX_CLIENTS; i++) {
		if(!botstates[i] || !botstates[i]->inuse) continue;
		if(g_entities[i].client->pers.connected != CON_CONNECTED) continue;

		BotUpdateInput(botstates[i], time, elapsed_time);
		trap_BotUserCommand(botstates[i]->client, &botstates[i]->lastucmd);
	}

	return qtrue;
}

int BotAISetup(int restart) {
	int errnum;

	if(restart) return qtrue;

	// initialize the bot states
	memset(botstates, 0, sizeof(botstates));

	errnum = trap_BotLibSetup();
	if(errnum != 0) return qfalse;
	return qtrue;
}

int BotAIShutdown(int restart) {
	int i;

	// if the game is restarted for a tournament
	if(restart) {
		// shutdown all the bots in the botlib
		for(i = 0; i < MAX_CLIENTS; i++) {
			if(botstates[i] && botstates[i]->inuse) {
				BotAIShutdownClient(botstates[i]->client, restart);
			}
		}
		// don't shutdown the bot library
	} else {
		trap_BotLibShutdown();
	}
	return qtrue;
}
