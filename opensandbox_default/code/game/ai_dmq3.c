// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

#include "g_local.h"
#include "botlib.h"

#include "ai_main.h"
#include "ai_dmq3.h"
#include "ai_chat.h"
#include "ai_cmd.h"
#include "ai_dmnet.h"
#include "ai_team.h"

#include "chars.h"				//characteristics
#include "inv.h"				//indexes into the inventory
#include "syn.h"				//synonyms
#include "match.h"				//string matching types and vars

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

bot_moveresult_t BotAttackMove(bot_state_t *bs, int tfl) {
	bot_moveresult_t moveresult;
	bot_goal_t goal;
	
	goal.entitynum = bs->enemy;
	goal.areanum = bs->lastenemyareanum;
	VectorCopy(bs->lastenemyorigin, goal.origin);
	VectorSet(goal.mins, -8, -8, -8);
	VectorSet(goal.maxs, 8, 8, 8);
		
	BotSetupForMovement(bs);
	trap_BotMoveToGoal(&moveresult, bs->ms, &goal, tfl);
	return;
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
	int i, healthdecrease;
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
	vec3_t forward, right, start, end, dir, angles;
	weaponinfo_t wi;
	bsp_trace_t trace;
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
	int ent, modelnum;
	char model[128];

	gametype = cvarInt("g_gametype");
	maxclients = cvarInt("g_maxClients");
}
