// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

#include "g_local.h"
#include "../qcommon/q_shared.h"
#include "botlib.h"		//bot lib interface

#include "ai_main.h"

#include "chars.h"
#include "inv.h"
#include "syn.h"

#define MAX_PATH		144
#define AI_THINKTIME    300

int gametype;		//game type
int maxclients;		//maximum number of clients

qboolean EntityIsDead(aas_entityinfo_t *entinfo) {
	playerState_t ps;

	if (entinfo->number >= 0 && entinfo->number < MAX_CLIENTS) {
		BotAI_GetClientState( entinfo->number, &ps );
		if (ps.pm_type != PM_NORMAL) return qtrue;
	}
	return qfalse;
}

qboolean EntityIsInvisible(aas_entityinfo_t *entinfo) {
	if (entinfo->powerups & (1 << PW_INVIS)) return qtrue;
	return qfalse;
}

qboolean EntityIsShooting(aas_entityinfo_t *entinfo) {
	if (entinfo->flags & EF_FIRING) return qtrue;
	return qfalse;
}

int BotPointAreaNum(vec3_t origin) {
	int areanum, numareas, areas[10];
	vec3_t end;

	areanum = trap_AAS_PointAreaNum(origin);
	if (areanum) return areanum;
	VectorCopy(origin, end);
	end[2] += 10;
	numareas = trap_AAS_TraceAreas(origin, end, areas, NULL, 10);
	if (numareas > 0) return areas[0];
	return 0;
}

int weaponOrder[] = {
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

int BotSelectOpenSandboxWeapon(bot_state_t *bs) {
	int i, weaponID;
	for (i = WEAPONS_NUM - 1; i > 0; i--) {
		weaponID = weaponOrder[i];
		if(bs->swep_list[weaponID] == WS_HAVE && (bs->swep_ammo[weaponID] > 0 || bs->swep_ammo[weaponID] == -1)){
		if(weaponID != WP_NONE && weaponID != WP_GRAPPLING_HOOK && weaponID != WP_PHYSGUN && weaponID != WP_GRAVITYGUN && weaponID != WP_TOOLGUN && weaponID != WP_REGENERATOR){
			return weaponID;
		}
		}
	}
	return WP_NONE;
}

/*
==================
BotChooseWeapon
==================
*/
void BotChooseWeapon(bot_state_t *bs) {
	if (bs->weaponnum != BotSelectOpenSandboxWeapon(bs)) bs->weaponchange_time = FloatTime();
	bs->weaponnum = BotSelectOpenSandboxWeapon(bs);
	bs->cur_ps.weapon = bs->weaponnum;
	trap_EA_SelectWeapon(bs->client, bs->weaponnum);
}

/*
==================
BotSetupForMovement
==================
*/
void BotSetupForMovement(bot_state_t *bs) {
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
	//
	if (bs->walker > 0.5) initmove.or_moveflags |= MFL_WALK;
	//
	VectorCopy(bs->viewangles, initmove.viewangles);
	//
	trap_BotInitMoveState(bs->ms, &initmove);
}

/*
==================
BotUpdateInventory
==================
*/
void BotUpdateInventory(bot_state_t *bs) {
	gentity_t *ent;
	int i;

	//weapons
	for(i = 1; i < WEAPONS_NUM; i++){
		ent = g_entities + bs->client;
		bs->swep_list[i] = ent->swep_list[i];
		bs->swep_ammo[i] = ent->swep_ammo[i];
	}
}

qboolean BotIsDead(bot_state_t *bs) {
	return (bs->cur_ps.pm_type == PM_DEAD);
}

qboolean BotIsObserver(bot_state_t *bs) {
	char buf[MAX_INFO_STRING];
	if (bs->cur_ps.pm_type == PM_SPECTATOR) return qtrue;
	trap_GetConfigstring(CS_PLAYERS+bs->client, buf, sizeof(buf));
	if (atoi(Info_ValueForKey(buf, "t")) == TEAM_SPECTATOR) return qtrue;
	return qfalse;
}

qboolean BotIntermission(bot_state_t *bs) {
	//NOTE: we shouldn't be looking at the game code...
	if (level.intermissiontime) return qtrue;
	return (bs->cur_ps.pm_type == PM_FREEZE || bs->cur_ps.pm_type == PM_INTERMISSION);
}

qboolean BotInLavaOrSlime(bot_state_t *bs) {
	vec3_t feet;

	VectorCopy(bs->origin, feet);
	feet[2] -= 23;
	return (trap_AAS_PointContents(feet) & (CONTENTS_LAVA|CONTENTS_SLIME));
}

void BotAttackMove(bot_state_t *bs, int tfl) {
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

int BotSameTeam(bot_state_t *bs, int entnum) {
	char info1[1024], info2[1024];

	if (bs->client < 0 || bs->client >= MAX_CLIENTS)
		return qfalse;

	if (entnum < 0 || entnum >= MAX_CLIENTS)
		return qfalse;

	trap_GetConfigstring(CS_PLAYERS+bs->client, info1, sizeof(info1));
	trap_GetConfigstring(CS_PLAYERS+entnum, info2, sizeof(info2));

	if ( gametype >= GT_TEAM && atoi(Info_ValueForKey(info1, "nt")) <= NT_PLAYER ) {
		if (atoi(Info_ValueForKey(info1, "t")) == atoi(Info_ValueForKey(info2, "t"))) return qtrue;
	} else {
		if(!BG_FactionShouldAttack(gameInfoNPCTypes[atoi(Info_ValueForKey(info1, "nt"))].faction, gameInfoNPCTypes[atoi(Info_ValueForKey(info2, "nt"))].faction)) return qtrue;
	}
	return qfalse;
}

qboolean InFieldOfVision(vec3_t viewangles, float fov, vec3_t angles) {
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

float BotEntityVisible(int viewer, vec3_t eye, vec3_t viewangles, float fov, int ent) {
	bsp_trace_t trace;
	aas_entityinfo_t entinfo;

	BotEntityInfo(ent, &entinfo);
	
	BotAI_Trace(&trace, eye, NULL, NULL, entinfo.origin, viewer, CONTENTS_SOLID|CONTENTS_PLAYERCLIP);

	if (trace.fraction >= 1 || trace.ent == ent) return 1.00;
	return 0.00;
}

int BotFindEnemy(bot_state_t *bs, int curenemy) {
	int i;
	float vis;
	float squaredist, cursquaredist;
	aas_entityinfo_t entinfo, curenemyinfo;
	vec3_t dir;

	if (curenemy >= 0) {
		BotEntityInfo(curenemy, &curenemyinfo);
		VectorSubtract(curenemyinfo.origin, bs->origin, dir);
		cursquaredist = VectorLengthSquared(dir);
	} else {
		cursquaredist = 0;
	}
	
	for (i = 0; i < maxclients && i < MAX_CLIENTS; i++) {
		if (i == bs->client) continue;
		if (i == curenemy) continue;
		BotEntityInfo(i, &entinfo);
		if (!entinfo.valid) continue;
		if (EntityIsDead(&entinfo) || entinfo.number == bs->entitynum) continue;
		if (EntityIsInvisible(&entinfo) && !EntityIsShooting(&entinfo)) continue;

		//calculate the distance towards the enemy
		VectorSubtract(entinfo.origin, bs->origin, dir);
		squaredist = VectorLengthSquared(dir);
		if (curenemy >= 0 && squaredist > cursquaredist) continue;

		if (squaredist > Square(16384.0)) continue;
		if (BotSameTeam(bs, i)) continue;
		
		if(bs->npcType != NT_NEXTBOT){
			vis = BotEntityVisible(bs->entitynum, bs->eye, bs->viewangles, 360, i);
		} else {
			vis = 1.00;
		}
		if (vis <= 0.00) continue;
		
		//found an enemy
		bs->enemy = entinfo.number;
		return qtrue;
	}
	return qfalse;
}

void BotAimAtEnemy(bot_state_t *bs) {
	vec3_t dir;
	aas_entityinfo_t entinfo;
	
	if (bs->enemy < 0) return;
	
	BotEntityInfo(bs->enemy, &entinfo);
	VectorSubtract(entinfo.origin, bs->eye, dir);
	vectoangles(dir, bs->ideal_viewangles);
	VectorCopy(bs->ideal_viewangles, bs->viewangles);
	trap_EA_View(bs->client, bs->viewangles);
}

void BotCheckAttack(bot_state_t *bs) { //TODO: make from enemy origin, not from aimtarget
	float fov;
	int attackentity;
	bsp_trace_t bsptrace;
	vec3_t dir, angles;
	aas_entityinfo_t entinfo;
	vec3_t mins = {-8, -8, -8}, maxs = {8, 8, 8};

	attackentity = bs->enemy;
	BotEntityInfo(attackentity, &entinfo);
	VectorSubtract(bs->aimtarget, bs->eye, dir);

	if (gameInfoWeapons[bs->weaponnum].wType == WT_MELEE) {
		if (VectorLengthSquared(dir) > Square(100)) return;
	}
	if (VectorLengthSquared(dir) < Square(100)) fov = 360;
	else fov = 90;

	vectoangles(dir, angles);
	if (!InFieldOfVision(bs->viewangles, fov, angles)) return;
	BotAI_Trace(&bsptrace, bs->eye, NULL, NULL, bs->aimtarget, bs->client, CONTENTS_SOLID|CONTENTS_PLAYERCLIP);
	if (bsptrace.fraction < 1 && bsptrace.ent != attackentity) return;

	trap_EA_Attack(bs->client);
}

int AINode_Nothing(bot_state_t *bs) {
	return qtrue;
}

int AINode_Respawn(bot_state_t *bs) {
	if (!BotIsDead(bs)) {
		bs->ainode = AINode_Seek_LTG;
	} else {
	    trap_BotResetMoveState(bs->ms);
	    trap_BotResetGoalState(bs->gs);
	    trap_BotResetAvoidGoals(bs->gs);
	    trap_BotResetAvoidReach(bs->ms);
		trap_EA_Respawn(bs->client);
	}

	return qtrue;
}

void BotAimAtMovementDirection(bot_state_t *bs) {
    bot_input_t bi;
    vec3_t temp_dir;
    
    trap_EA_GetInput(bs->client, (float) bs->thinktime / 1000, &bi);
    
    if (VectorLength(bi.dir) < 0.1f) return;
    VectorCopy(bi.dir, temp_dir);
    temp_dir[2] = 0;
    
    if (VectorNormalize(temp_dir) > 0) {
        vectoangles(temp_dir, bs->ideal_viewangles);
        
        bs->ideal_viewangles[PITCH] = bs->viewangles[PITCH];
        bs->ideal_viewangles[ROLL] = 0;
        
        VectorCopy(bs->ideal_viewangles, bs->viewangles);
        trap_EA_View(bs->client, bs->viewangles);
    }
}

bot_goal_t AI_GoalFromRandomItem(bot_state_t *bs){
    bot_goal_t result;
    gentity_t* item;
    
    memset(&result, 0, sizeof(bot_goal_t));
    
    item = FindRandomItem();
    if (item) {
        G_Printf("Goal finded: %s \n", item->classname);
        VectorCopy(item->r.currentOrigin, result.origin);
        result.areanum = BotPointAreaNum(item->r.currentOrigin);
        VectorSet(result.mins, -8, -8, -8);
		VectorSet(result.maxs, 8, 8, 8);
        result.entitynum = item->s.number;
    }
    
    return result;
}

int AINode_Seek_LTG(bot_state_t *bs) {
	bot_moveresult_t moveresult;

	if (BotIsObserver(bs)) {
		bs->ainode = AINode_Nothing;
		return qfalse;
	}
	if (BotIntermission(bs)) {
		bs->ainode = AINode_Nothing;
		return qfalse;
	}
	if (BotIsDead(bs)) {
		bs->ainode = AINode_Respawn;
		return qfalse;
	}

	bs->tfl = TFL_DEFAULT;
	
	if (BotInLavaOrSlime(bs)) bs->tfl |= TFL_LAVA|TFL_SLIME;
	
	bs->enemy = -1;     //no enemy
	if (BotFindEnemy(bs, -1)) {
		bs->ainode = AINode_Battle_Fight;
		return qfalse;
	}
	
    if (level.time > bs->item_searchtime || trap_BotTouchingGoal(bs->origin, &bs->maingoal)) {
        bs->maingoal = AI_GoalFromRandomItem(bs);
        bs->item_searchtime = level.time + 30000;
    }
	
	if(bs->maingoal.entitynum){
	    BotSetupForMovement(bs);
	    trap_BotMoveToGoal(&moveresult, bs->ms, &bs->maingoal, bs->tfl);
	    BotAimAtMovementDirection(bs);
	}
	return qtrue;
}

int AINode_Battle_Fight(bot_state_t *bs) {
	int areanum;
	vec3_t target;
	aas_entityinfo_t entinfo;

	BotFindEnemy(bs, bs->enemy);

	if (bs->enemy < 0) {
		bs->ainode = AINode_Seek_LTG;
		return qfalse;
	}

	BotEntityInfo(bs->enemy, &entinfo);

	//if the enemy is invisible and not shooting the bot looses track easily
	if (EntityIsInvisible(&entinfo) && !EntityIsShooting(&entinfo)) {
		if (random() < 0.2) {
			bs->ainode = AINode_Seek_LTG;
			return qfalse;
		}
	}
	
	VectorCopy(entinfo.origin, target);
	
	//update the reachability area and origin if possible
	areanum = BotPointAreaNum(target);
	if (areanum && trap_AAS_AreaReachability(areanum)) {
		VectorCopy(target, bs->lastenemyorigin);
		bs->lastenemyareanum = areanum;
	}

	//if the enemy is not visible
	if (!BotEntityVisible(bs->entitynum, bs->eye, bs->viewangles, 360, bs->enemy) && bs->npcType != NT_NEXTBOT) {
		bs->ainode = AINode_Seek_LTG;
		return qfalse;
	}

	bs->tfl = TFL_DEFAULT;

	if (BotInLavaOrSlime(bs)) bs->tfl |= TFL_LAVA|TFL_SLIME;

	BotChooseWeapon(bs);
	BotAttackMove(bs, bs->tfl);
	BotAimAtEnemy(bs);
	BotCheckAttack(bs);
	return qtrue;
}

void BotDeathmatchAI(bot_state_t *bs, float thinktime) {
	char buf[144];

	//if the bot has just been setup
	if (bs->setupcount > 0) {
		bs->setupcount--;
		if (bs->setupcount > 0) return;
		//set the team
		Com_sprintf(buf, sizeof(buf), "team %s", bs->settings.team);
		trap_EA_Command(bs->client, buf);
		
		bs->setupcount = 0;
	}

	if (!BotIntermission(bs)) BotUpdateInventory(bs);
	if (!bs->ainode) bs->ainode = AINode_Seek_LTG;
	bs->ainode(bs);	//execute AI node
	
	if (!bs->inuse) return;
}

void BotSetupDeathmatchAI(void) {
	gametype = cvarInt("g_gametype");
	maxclients = cvarInt("g_maxClients");
}

bot_state_t	*botstates[MAX_CLIENTS];
int numbots;
float floattime;
float regularupdate_time;

void BotAI_Trace(bsp_trace_t *bsptrace, vec3_t start, vec3_t mins, vec3_t maxs, vec3_t end, int passent, int contentmask) {
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

int BotAI_GetClientState( int clientNum, playerState_t *state ) {
	gentity_t	*ent;

	ent = &g_entities[clientNum];
	if ( !ent->inuse ) return qfalse;
	if ( !ent->client ) return qfalse;

	memcpy( state, &ent->client->ps, sizeof(playerState_t) );
	return qtrue;
}

void BotEntityInfo(int entnum, aas_entityinfo_t *info) {
	trap_AAS_EntityInfo(entnum, info);
}

float BotChangeViewAngle(float angle, float ideal_angle, float speed) {
	float move;

	angle = AngleMod(angle);
	ideal_angle = AngleMod(ideal_angle);
	if (angle == ideal_angle) return angle;
	move = ideal_angle - angle;
	if (ideal_angle > angle) {
		if (move > 180.0) move -= 360.0;
	}
	else {
		if (move < -180.0) move += 360.0;
	}
	if (move > 0) {
		if (move > speed) move = speed;
	} else {
		if (move < -speed) move = -speed;
	}
	return AngleMod(angle + move);
}

void BotChangeViewAngles(bot_state_t *bs, float thinktime) {
	float diff, factor, maxchange, anglespeed;
	int i;

	if (bs->ideal_viewangles[PITCH] > 180) bs->ideal_viewangles[PITCH] -= 360;
	//
	if (bs->enemy >= 0) {
		factor = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_VIEW_FACTOR, 0.01f, 1);
		maxchange = trap_Characteristic_BFloat(bs->character, CHARACTERISTIC_VIEW_MAXCHANGE, 1, 1800);
	}
	else {
		factor = 0.05f;
		maxchange = 360;
	}
	if (maxchange < 240) maxchange = 240;
	maxchange *= thinktime;
	for (i = 0; i < 2; i++) {
		//smooth slowdown view model
		diff = abs(AngleDifference(bs->viewangles[i], bs->ideal_viewangles[i]));
		anglespeed = diff * factor;
		if (anglespeed > maxchange) anglespeed = maxchange;
		bs->viewangles[i] = BotChangeViewAngle(bs->viewangles[i], bs->ideal_viewangles[i], anglespeed);
	}
	//bs->viewangles[PITCH] = 0;
	if (bs->viewangles[PITCH] > 180) bs->viewangles[PITCH] -= 360;
	//elementary action: view
	trap_EA_View(bs->client, bs->viewangles);
}

void BotInputToUserCommand(bot_input_t *bi, usercmd_t *ucmd, int delta_angles[3], int time) {
	vec3_t angles, forward, right;
	short temp;
	int j;

	memset(ucmd, 0, sizeof(usercmd_t));

	ucmd->serverTime = time;

	if (bi->actionflags & ACTION_DELAYEDJUMP) {
		bi->actionflags |= ACTION_JUMP;
		bi->actionflags &= ~ACTION_DELAYEDJUMP;
	}

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

	for (j = 0; j < 3; j++) {
		temp = ucmd->angles[j] - delta_angles[j];
		ucmd->angles[j] = temp;
	}
	
	if (bi->dir[2]) angles[PITCH] = bi->viewangles[PITCH];
	else angles[PITCH] = 0;
	
	angles[YAW] = bi->viewangles[YAW];
	angles[ROLL] = 0;
	AngleVectors(angles, forward, right, NULL);

	bi->speed = bi->speed * 127 / 400;
	ucmd->forwardmove = DotProduct(forward, bi->dir) * bi->speed;
	ucmd->rightmove = DotProduct(right, bi->dir) * bi->speed;
	ucmd->upmove = abs(forward[2]) * bi->dir[2] * bi->speed;

	if (bi->actionflags & ACTION_MOVEFORWARD) ucmd->forwardmove += 127;
	if (bi->actionflags & ACTION_MOVEBACK) ucmd->forwardmove -= 127;
	if (bi->actionflags & ACTION_MOVELEFT) ucmd->rightmove -= 127;
	if (bi->actionflags & ACTION_MOVERIGHT) ucmd->rightmove += 127;
	if (bi->actionflags & ACTION_JUMP) ucmd->upmove += 127;
	if (bi->actionflags & ACTION_CROUCH) ucmd->upmove -= 127;
}

void BotUpdateInput(bot_state_t *bs, int time, int elapsed_time) {
	bot_input_t bi;
	int j;

	for (j = 0; j < 3; j++) {
		bs->viewangles[j] = AngleMod(bs->viewangles[j] + SHORT2ANGLE(bs->cur_ps.delta_angles[j]));
	}

	BotChangeViewAngles(bs, (float) elapsed_time / 1000);
	trap_EA_GetInput(bs->client, (float) time / 1000, &bi);

	if (bi.actionflags & ACTION_RESPAWN) {
		if (bs->lastucmd.buttons & BUTTON_ATTACK) bi.actionflags &= ~(ACTION_RESPAWN|ACTION_ATTACK);
	}

	BotInputToUserCommand(&bi, &bs->lastucmd, bs->cur_ps.delta_angles, time);

	for (j = 0; j < 3; j++) {
		bs->viewangles[j] = AngleMod(bs->viewangles[j] - SHORT2ANGLE(bs->cur_ps.delta_angles[j]));
	}
}

void BotAIRegularUpdate(void) {
	if (regularupdate_time < FloatTime()) {
		trap_BotUpdateEntityItems();
		regularupdate_time = FloatTime() + 0.3;
	}
}

int BotAI(int client, float thinktime) {
	bot_state_t *bs;
	char buf[1024];
	int i;

	trap_EA_ResetInput(client);
	
	while( trap_BotGetServerCommand(client, buf, sizeof(buf)) ) {
        //NOTHING, WE JUST ANSWER TO EVERY COMMAND
	}
	
	bs = botstates[client];
	if (!bs || !bs->inuse) {
		G_Printf("BotAI: client %d is not setup\n", client);
		return qfalse;
	}

	//retrieve the current client state
	BotAI_GetClientState( client, &bs->cur_ps );

	//add the delta angles to the bot's current view angles
	for (i = 0; i < 3; i++) {
		bs->viewangles[i] = AngleMod(bs->viewangles[i] + SHORT2ANGLE(bs->cur_ps.delta_angles[i]));
	}
	
	//increase the local time of the bot
	bs->ltime += thinktime;
	bs->thinktime = thinktime;
	VectorCopy(bs->cur_ps.origin, bs->origin);
	VectorCopy(bs->cur_ps.origin, bs->eye);
	bs->eye[2] += bs->cur_ps.viewheight;
	bs->areanum = BotPointAreaNum(bs->origin);
	
	BotDeathmatchAI(bs, thinktime);
	trap_EA_SelectWeapon(bs->client, bs->weaponnum);

	//subtract the delta angles
	for (i = 0; i < 3; i++) {
		bs->viewangles[i] = AngleMod(bs->viewangles[i] - SHORT2ANGLE(bs->cur_ps.delta_angles[i]));
	}

	return qtrue;
}

void BotScheduleBotThink(void) {
	int i, botnum;

	botnum = 0;

	for( i = 0; i < MAX_CLIENTS; i++ ) {
		if( !botstates[i] || !botstates[i]->inuse ) {
			continue;
		}
		//initialize the bot think residual time
		botstates[i]->botthink_residual = AI_THINKTIME * botnum / numbots;
		botnum++;
	}
}

int BotAISetupClient(int client, struct bot_settings_s *settings, qboolean restart) {
	char filename[MAX_PATH], name[MAX_PATH], gender[MAX_PATH];
	bot_state_t *bs;
	int errnum;
	char userinfo[MAX_INFO_STRING];

	if (!botstates[client]) botstates[client] = G_Alloc(sizeof(bot_state_t));
	bs = botstates[client];

	if (bs && bs->inuse) {
		G_Printf("BotAISetupClient: client %d already setup\n", client);
		return qfalse;
	}

	if (!trap_AAS_Initialized()) {
		G_Printf("AAS not initialized\n");
		return qfalse;
	}

	//load the bot character
	bs->character = trap_BotLoadCharacter(settings->characterfile, settings->skill);
	if (!bs->character) {
		G_Printf("couldn't load skill %f from %s\n", settings->skill, settings->characterfile);
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

	numbots++;

	//NOTE: reschedule the bot thinking
	BotScheduleBotThink();

	//bot has been setup succesfully
	return qtrue;
}

int BotAIShutdownClient(int client, qboolean restart) {
	bot_state_t *bs;

	bs = botstates[client];
	if (!bs || !bs->inuse) return qfalse;

	trap_BotFreeMoveState(bs->ms);
	trap_BotFreeGoalState(bs->gs);
	trap_BotFreeChatState(bs->cs);
	trap_BotFreeWeaponState(bs->ws);
	trap_BotFreeCharacter(bs->character);

	memset(bs, 0, sizeof(bot_state_t));
	bs->inuse = qfalse;
	numbots--;

	return qtrue;
}

void BotResetState(bot_state_t *bs) {
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
	memset(bs, 0, sizeof(bot_state_t));
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

int BotAILoadMap( int restart ) {
	int			i;

	if (!restart) trap_BotLibLoadMap( cvarString("sv_mapname") );

	for (i = 0; i < MAX_CLIENTS; i++) {
		if (botstates[i] && botstates[i]->inuse) {
			BotResetState( botstates[i] );
			botstates[i]->setupcount = 4;
		}
	}

	BotSetupDeathmatchAI();

	return qtrue;
}

int AI_Frame(int time) {
	int i;
	gentity_t	*ent;
	bot_entitystate_t state;
	int elapsed_time, thinktime;
	static int local_time;
	static int botlib_residual;

	elapsed_time = time - local_time;
	local_time = time;
	botlib_residual += elapsed_time;

	if (elapsed_time > AI_THINKTIME) thinktime = elapsed_time;
	else thinktime = AI_THINKTIME;

	// update the bot library
	if ( botlib_residual >= thinktime ) {
		botlib_residual -= thinktime;

		trap_BotLibStartFrame((float) time / 1000);

		if (!trap_AAS_Initialized()) return qfalse;

		//update entities in the botlib
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
		BotAIRegularUpdate();
	}

	floattime = trap_AAS_Time();

	// execute scheduled bot AI
	for( i = 0; i < MAX_CLIENTS; i++ ) {
		if( !botstates[i] || !botstates[i]->inuse ) continue;
		botstates[i]->botthink_residual += elapsed_time;
		
		if ( botstates[i]->botthink_residual >= thinktime ) {
			botstates[i]->botthink_residual -= thinktime;
			if (!trap_AAS_Initialized()) return qfalse;
			if (g_entities[i].client->pers.connected == CON_CONNECTED) BotAI(i, (float) thinktime / 1000);
		}
	}

	// execute bot commands
	for( i = 0; i < MAX_CLIENTS; i++ ) {
		if( !botstates[i] || !botstates[i]->inuse ) continue;
		if( g_entities[i].client->pers.connected != CON_CONNECTED ) continue;

		BotUpdateInput(botstates[i], time, elapsed_time);
		trap_BotUserCommand(botstates[i]->client, &botstates[i]->lastucmd);
	}

	return qtrue;
}

int BotInitLibrary(void) {
	trap_BotLibVarSet("maxclients", va("%i", MAX_CLIENTS));
	trap_BotLibVarSet("maxentities", va("%i", MAX_GENTITIES));
	trap_BotLibVarSet("sv_mapChecksum", cvarString("sv_mapChecksum"));
	trap_BotLibVarSet("g_gametype", cvarString("g_gametype"));
	trap_BotLibDefine("MISSIONPACK");
	return trap_BotLibSetup();
}

int BotAISetup( int restart ) {
	int			errnum;

	if (restart) return qtrue;

	//initialize the bot states
	memset( botstates, 0, sizeof(botstates) );

	errnum = BotInitLibrary();
	if (errnum != BLERR_NOERROR) return qfalse;
	return qtrue;
}

int BotAIShutdown( int restart ) {
	int i;

	//if the game is restarted for a tournament
	if ( restart ) {
		//shutdown all the bots in the botlib
		for (i = 0; i < MAX_CLIENTS; i++) {
			if (botstates[i] && botstates[i]->inuse) {
				BotAIShutdownClient(botstates[i]->client, restart);
			}
		}
		//don't shutdown the bot library
	}
	else {
		trap_BotLibShutdown();
	}
	return qtrue;
}
