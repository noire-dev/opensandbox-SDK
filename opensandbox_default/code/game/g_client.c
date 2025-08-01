// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

#include "g_local.h"

static vec3_t playerMins = {-15, -15, -24};
static vec3_t playerMaxs = {15, 15, 32};

#define MAX_SPAWN_POINTS 128

void SP_info_player_deathmatch(gentity_t *ent) {
	int i;

	G_SpawnInt("nobots", "0", &i);
	if(i) ent->flags |= FL_NO_BOTS;
	G_SpawnInt("nohumans", "0", &i);
	if(i) ent->flags |= FL_NO_HUMANS;
}

void SP_info_player_start(gentity_t *ent) {
	ent->classname = "info_player_deathmatch";
	SP_info_player_deathmatch(ent);
}

qboolean SpotWouldTelefrag(gentity_t *spot) {
	int i, num;
	gentity_t *hit;
	vec3_t mins, maxs;

	VectorAdd(spot->s.origin, playerMins, mins);
	VectorAdd(spot->s.origin, playerMaxs, maxs);
	num = trap_EntitiesInBox(mins, maxs, SourceTechEntityList, MAX_GENTITIES);

	for(i = 0; i < num; i++) {
		hit = &g_entities[SourceTechEntityList[i]];
		// if ( hit->client && hit->client->ps.stats[STAT_HEALTH] > 0 ) {
		if(hit->client) {
			return qtrue;
		}
	}

	return qfalse;
}

static gentity_t *SelectRandomFurthestSpawnPoint(vec3_t avoidPoint, vec3_t origin, vec3_t angles) {
	gentity_t *spot;
	vec3_t delta;
	float dist;
	float list_dist[64];
	gentity_t *list_spot[64];
	int numSpots, rnd, i, j;

	numSpots = 0;
	spot = NULL;

	while((spot = G_Find(spot, FOFS(classname), "info_player_deathmatch")) != NULL) {
		if(SpotWouldTelefrag(spot)) continue;
		VectorSubtract(spot->s.origin, avoidPoint, delta);
		dist = VectorLength(delta);
		for(i = 0; i < numSpots; i++) {
			if(dist > list_dist[i]) {
				if(numSpots >= 64) numSpots = 64 - 1;
				for(j = numSpots; j > i; j--) {
					list_dist[j] = list_dist[j - 1];
					list_spot[j] = list_spot[j - 1];
				}
				list_dist[i] = dist;
				list_spot[i] = spot;
				numSpots++;
				if(numSpots > 64) numSpots = 64;
				break;
			}
		}
		if(i >= numSpots && numSpots < 64) {
			list_dist[numSpots] = dist;
			list_spot[numSpots] = spot;
			numSpots++;
		}
	}
	if(!numSpots) {
		spot = G_Find(NULL, FOFS(classname), "info_player_deathmatch");
		if(!spot) G_Error("Couldn't find a spawn point");
		VectorCopy(spot->s.origin, origin);
		origin[2] += 9;
		VectorCopy(spot->s.angles, angles);
		return spot;
	}

	// select a random spot from the spawn points furthest away
	rnd = random() * (numSpots / 2);

	VectorCopy(list_spot[rnd]->s.origin, origin);
	origin[2] += 9;
	VectorCopy(list_spot[rnd]->s.angles, angles);

	return list_spot[rnd];
}

gentity_t *SelectSpawnPoint(vec3_t avoidPoint, vec3_t origin, vec3_t angles) { return SelectRandomFurthestSpawnPoint(avoidPoint, origin, angles); }

static gentity_t *SelectSpectatorSpawnPoint(vec3_t origin, vec3_t angles) {
	FindIntermissionPoint();

	VectorCopy(level.intermission_origin, origin);
	VectorCopy(level.intermission_angle, angles);

	return NULL;
}

void InitBodyQue(void) {
	int i;
	gentity_t *ent;

	level.bodyQueIndex = 0;
	for(i = 0; i < BODY_QUEUE_SIZE; i++) {
		ent = G_Spawn();
		ent->classname = "bodyque";
		ent->neverFree = qtrue;
		level.bodyQue[i] = ent;
	}
}

static void BodySink(gentity_t *ent) {
	if(level.time - ent->timestamp > 6500) {
		trap_UnlinkEntity(ent);
		ent->physicsObject = qfalse;
		return;
	}
	ent->nextthink = level.time + 100;
	ent->s.pos.trBase[2] -= 1;
}

/*
=============
CopyToBodyQue

A player is respawning, so make an entity that looks
just like the existing corpse to leave behind.
=============
*/
void CopyToBodyQue(gentity_t *ent) {
	gentity_t *e;
	int i;
	gentity_t *body;
	int contents;

	trap_UnlinkEntity(ent);

	// if client is in a nodrop area, don't leave the body
	contents = trap_PointContents(ent->s.origin, -1);
	if(contents & CONTENTS_NODROP) return;

	// grab a body que and cycle to the next one
	body = level.bodyQue[level.bodyQueIndex];
	level.bodyQueIndex = (level.bodyQueIndex + 1) % BODY_QUEUE_SIZE;

	trap_UnlinkEntity(body);

	body->s = ent->s;
	body->s.eFlags = EF_DEAD;  // clear EF_TALK, etc
	if(ent->s.eFlags & EF_KAMIKAZE) {
		body->s.eFlags |= EF_KAMIKAZE;

		// check if there is a kamikaze timer around for this owner
		for(i = 0; i < MAX_GENTITIES; i++) {
			e = &g_entities[i];
			if(!e->inuse) continue;
			if(e->activator != ent) continue;
			if(strcmp(e->classname, "kamikaze timer")) continue;
			e->activator = body;
			break;
		}
	}
	body->s.powerups = 0;   // clear powerups
	body->s.loopSound = 0;  // clear lava burning
	body->s.number = body - g_entities;
	body->timestamp = level.time;
	body->physicsObject = qtrue;
	body->phys_bounce = 0;  // don't bounce
	if(body->s.groundEntityNum == ENTITYNUM_NONE) {
		body->s.pos.trType = TR_GRAVITY;
		body->s.pos.trTime = level.time;
		VectorCopy(ent->client->ps.velocity, body->s.pos.trDelta);
	} else {
		body->s.pos.trType = TR_STATIONARY;
	}
	body->s.event = 0;

	// change the animation to the last-frame only, so the sequence
	// doesn't repeat anew for the body
	switch(body->s.legsAnim & ~ANIM_TOGGLEBIT) {
		case BOTH_DEATH1:
		case BOTH_DEAD1: body->s.torsoAnim = body->s.legsAnim = BOTH_DEAD1; break;
		case BOTH_DEATH2:
		case BOTH_DEAD2: body->s.torsoAnim = body->s.legsAnim = BOTH_DEAD2; break;
		case BOTH_DEATH3:
		case BOTH_DEAD3:
		default: body->s.torsoAnim = body->s.legsAnim = BOTH_DEAD3; break;
	}

	body->r.svFlags = ent->r.svFlags;
	VectorCopy(ent->r.mins, body->r.mins);
	VectorCopy(ent->r.maxs, body->r.maxs);
	VectorCopy(ent->r.absmin, body->r.absmin);
	VectorCopy(ent->r.absmax, body->r.absmax);

	body->clipmask = CONTENTS_SOLID | CONTENTS_PLAYERCLIP;
	body->r.contents = CONTENTS_CORPSE;
	body->r.ownerNum = ent->s.number;

	body->nextthink = level.time + 5000;
	body->think = BodySink;

	body->die = body_die;

	// don't take more damage if already gibbed
	if(ent->health <= GIB_HEALTH) {
		body->takedamage = qfalse;
	} else {
		body->takedamage = qtrue;
	}

	VectorCopy(body->s.pos.trBase, body->r.currentOrigin);
	trap_LinkEntity(body);
}

void SetClientViewAngle(gentity_t *ent, vec3_t angle) {
	int i;

	// set the delta angle
	for(i = 0; i < 3; i++) {
		int cmdAngle;

		cmdAngle = ANGLE2SHORT(angle[i]);
		ent->client->ps.delta_angles[i] = cmdAngle - ent->client->pers.cmd.angles[i];
	}
	VectorCopy(angle, ent->s.angles);
	VectorCopy(ent->s.angles, ent->client->ps.viewangles);
}

void ClientRespawn(gentity_t *ent) {
	gentity_t *tent;

	if(ent->npcType > NT_PLAYER) {
		DropClientSilently(ent->client->ps.clientNum);
		return;
	}

	CopyToBodyQue(ent);
	ClientSpawn(ent);

	// add a teleportation effect
	tent = G_TempEntity(ent->client->ps.origin, EV_PLAYER_TELEPORT_IN);
	tent->s.clientNum = ent->s.clientNum;
}

/*
================
TeamCount

Returns number of players on a team
================
*/
static team_t TeamCount(int ignoreClientNum, int team) {
	int i;
	int count = 0;

	for(i = 0; i < level.maxclients; i++) {
		if(i == ignoreClientNum) continue;
		if(level.clients[i].pers.connected == CON_DISCONNECTED) continue;
		if(level.clients[i].pers.connected == CON_CONNECTING) continue;
		if(level.clients[i].sess.sessionTeam == team) count++;
	}

	return count;
}

/*
================
TeamLeader

Returns the client number of the team leader
================
*/
int TeamLeader(int team) {
	int i;

	for(i = 0; i < level.maxclients; i++) {
		if(level.clients[i].pers.connected == CON_DISCONNECTED) {
			continue;
		}
		if(level.clients[i].sess.sessionTeam == team) {
			if(level.clients[i].sess.teamLeader) return i;
		}
	}

	return -1;
}

team_t PickTeam(int ignoreClientNum) {
	int counts[TEAM_NUM_TEAMS];

	counts[TEAM_BLUE] = TeamCount(ignoreClientNum, TEAM_BLUE);
	counts[TEAM_RED] = TeamCount(ignoreClientNum, TEAM_RED);

	if(counts[TEAM_BLUE] > counts[TEAM_RED]) return TEAM_RED;
	if(counts[TEAM_RED] > counts[TEAM_BLUE]) return TEAM_BLUE;
	if(level.teamScores[TEAM_BLUE] > level.teamScores[TEAM_RED]) return TEAM_RED;
	return TEAM_BLUE;
}

static void ClientCleanName(const char *in, char *out, int outSize) {
	int len, colorlessLen;
	char ch;
	char *p;
	int spaces;

	// save room for trailing null byte
	outSize--;

	len = 0;
	colorlessLen = 0;
	p = out;
	*p = 0;
	spaces = 0;

	while(1) {
		ch = *in++;
		if(!ch) break;

		// don't allow leading spaces
		if(!*p && ch == ' ') {
			continue;
		}

		// check colors
		if(ch == Q_COLOR_ESCAPE) {
			// solo trailing carat is not a color prefix
			if(!*in) break;

			// don't allow black in a name, period
			if(ColorIndex(*in) == 0) {
				in++;
				continue;
			}

			// make sure room in dest for both chars
			if(len > outSize - 2) break;

			*out++ = ch;
			*out++ = *in++;
			len += 2;
			continue;
		}

		// don't allow too many consecutive spaces
		if(ch == ' ') {
			spaces++;
			if(spaces > 3) {
				continue;
			}
		} else {
			spaces = 0;
		}

		if(len > outSize - 1) break;

		*out++ = ch;
		colorlessLen++;
		len++;
	}
	*out = 0;

	// don't allow empty names
	if(*p == 0 || colorlessLen == 0) {
		Q_strncpyz(p, "noname", outSize);
	}
}

/*
===========
ClientUserInfoChanged

Called from ClientConnect when the player first connects and
directly by the server system when the player updates a userinfo variable.

The game can override any of the settings and call trap_SetUserinfo if desired.
============
*/
void ClientUserinfoChanged(int clientNum) {
	gentity_t *ent;
	int teamTask, teamLeader, team;
	int npcType, botskill;
	char *s;
	char model[MAX_QPATH], headModel[MAX_QPATH], legsModel[MAX_QPATH];
	char headR[16], headG[16], headB[16];
	char modelR[16], modelG[16], modelB[16];
	char legsR[16], legsG[16], legsB[16];
	char physR[16], physG[16], physB[16];
	char swep_id[16];
	char oldname[MAX_STRING_CHARS];
	gclient_t *client;
	char userinfo[MAX_INFO_STRING];

	ent = g_entities + clientNum;
	client = ent->client;

	trap_GetUserinfo(clientNum, userinfo, sizeof(userinfo));

	// check for malformed or illegal info strings
	if(!Info_Validate(userinfo)) strcpy(userinfo, "\\name\\badinfo");

	// set name
	Q_strncpyz(oldname, client->pers.netname, sizeof(oldname));
	s = Info_ValueForKey(userinfo, "name");
	ClientCleanName(s, client->pers.netname, sizeof(client->pers.netname));

	ent->tool_id = atoi(Info_ValueForKey(userinfo, "toolgun_tool"));
	ent->tool_entity = NULL;

	if(client->pers.connected == CON_CONNECTED) {
		if(strcmp(oldname, client->pers.netname)) {
			trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " renamed to %s\n\"", oldname, client->pers.netname));
		}
	}

	// set model
	if(g_gametype.integer >= GT_TEAM) {
		Q_strncpyz(model, Info_ValueForKey(userinfo, "team_model"), sizeof(model));
		Q_strncpyz(headModel, Info_ValueForKey(userinfo, "team_headmodel"), sizeof(headModel));
		Q_strncpyz(legsModel, Info_ValueForKey(userinfo, "team_legsmodel"), sizeof(legsModel));
	} else {
		Q_strncpyz(model, Info_ValueForKey(userinfo, "model"), sizeof(model));
		Q_strncpyz(headModel, Info_ValueForKey(userinfo, "headmodel"), sizeof(headModel));
		Q_strncpyz(legsModel, Info_ValueForKey(userinfo, "legsmodel"), sizeof(legsModel));
	}

	if(ent->r.svFlags & SVF_BOT) {
		if(g_gametype.integer >= GT_TEAM) {
			Q_strncpyz(legsModel, Info_ValueForKey(userinfo, "team_model"), sizeof(legsModel));
		} else {
			Q_strncpyz(legsModel, Info_ValueForKey(userinfo, "model"), sizeof(legsModel));
		}

		botskill = atoi(Info_ValueForKey(userinfo, "skill"));
		ent->skill = botskill;
	}

	npcType = atoi(Info_ValueForKey(userinfo, "npcType"));
	if(npcType && ent->r.svFlags & SVF_BOT){
		ent->npcType = npcType;
	} else {
		ent->npcType = NT_PLAYER;
	}

	strcpy(legsR, Info_ValueForKey(userinfo, "legsR"));
	strcpy(legsG, Info_ValueForKey(userinfo, "legsG"));
	strcpy(legsB, Info_ValueForKey(userinfo, "legsB"));
	strcpy(modelR, Info_ValueForKey(userinfo, "modelR"));
	strcpy(modelG, Info_ValueForKey(userinfo, "modelG"));
	strcpy(modelB, Info_ValueForKey(userinfo, "modelB"));
	strcpy(headR, Info_ValueForKey(userinfo, "headR"));
	strcpy(headG, Info_ValueForKey(userinfo, "headG"));
	strcpy(headB, Info_ValueForKey(userinfo, "headB"));
	strcpy(physR, Info_ValueForKey(userinfo, "physR"));
	strcpy(physG, Info_ValueForKey(userinfo, "physG"));
	strcpy(physB, Info_ValueForKey(userinfo, "physB"));

	// set team
	if(g_gametype.integer >= GT_TEAM && g_entities[clientNum].r.svFlags & SVF_BOT) {
		s = Info_ValueForKey(userinfo, "team");
		if(!Q_stricmp(s, "red") || !Q_stricmp(s, "r")) {
			team = TEAM_RED;
		} else if(!Q_stricmp(s, "blue") || !Q_stricmp(s, "b")) {
			team = TEAM_BLUE;
		} else if(!Q_stricmp(s, "free") && ent->npcType > NT_PLAYER) {  // FREE_TEAM
			team = TEAM_FREE;
		} else {
			// pick the team with the least number of players
			team = PickTeam(clientNum);
		}
		client->sess.sessionTeam = team;
	} else {
		team = client->sess.sessionTeam;
	}

	// team task (0 = none, 1 = offence, 2 = defence)
	teamTask = atoi(Info_ValueForKey(userinfo, "teamtask"));
	// team Leader (1 = leader, 0 is normal player)
	teamLeader = client->sess.teamLeader;

	strcpy(swep_id, va("%i", ent->swep_id));

	if(ent->r.svFlags & SVF_BOT) {
		s = va("n\\%s\\t\\%i\\m\\%s\\hm\\%s\\lm\\%s\\si\\%s\\vn\\%i\\nt\\%i\\s\\%s\\tt\\%d\\tl\\%d", client->pers.netname, team, model, headModel, legsModel, swep_id, client->vehicleNum, ent->npcType, Info_ValueForKey(userinfo, "skill"), teamTask, teamLeader);
	} else {
		s = va("n\\%s\\t\\%i\\m\\%s\\hm\\%s\\lm\\%s\\hr\\%s\\hg\\%s\\hb\\%s\\mr\\%s\\mg\\%s\\mb\\%s\\lr\\%s\\lg\\%s\\lb\\%s\\pr\\%s\\pg\\%s\\pb\\%s\\si\\%s\\vn\\%i\\nt\\%i\\tt\\%d\\tl\\%d\\f\\%i", client->pers.netname, client->sess.sessionTeam, model, headModel, legsModel, headR, headG, headB, modelR, modelG, modelB, legsR, legsG, legsB, physR, physG, physB, swep_id, client->vehicleNum, ent->npcType, teamTask, teamLeader, ent->flashlight);
	}

	trap_SetConfigstring(CS_PLAYERS + clientNum, s);
}

/*
===========
ClientConnect

Called when a player begins connecting to the server.
Called again for every map change.
============
*/
char *ClientConnect(int clientNum, qboolean firstTime, qboolean isBot) {
	char *value;
	gclient_t *client;
	char userinfo[MAX_INFO_STRING];
	gentity_t *ent;

	if(clientNum >= g_maxClients.integer) return "Server is full, increase g_maxClients.";

	ent = &g_entities[clientNum];

	trap_GetUserinfo(clientNum, userinfo, sizeof(userinfo));

	if(!isBot) {
		// check for a password
		value = Info_ValueForKey(userinfo, "password");
		if(g_password.string[0] && Q_stricmp(g_password.string, "none") && strcmp(g_password.string, value) != 0) {
			return "Invalid password";
		}
	}

	// they can connect
	ent->client = level.clients + clientNum;
	client = ent->client;

	memset(client, 0, sizeof(*client));

	client->pers.connected = CON_CONNECTING;

	// read or initialize the session data
	if(firstTime || level.newSession) G_InitSessionData(client, userinfo);
	G_ReadSessionData(client);

	if(isBot) {
		ent->r.svFlags |= SVF_BOT;
		ent->inuse = qtrue;
		if(!G_BotConnect(clientNum, !firstTime)) return "BotConnectfailed";

		SandboxBotSpawn(ent, Info_ValueForKey(userinfo, "spawnid"));
	}

	ClientUserinfoChanged(clientNum);

	// don't do the "xxx connected" messages if they were caried over from previous level
	if(firstTime && (ent->npcType <= NT_PLAYER)) trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " connected\n\"", client->pers.netname));

	if(g_gametype.integer >= GT_TEAM && client->sess.sessionTeam != TEAM_SPECTATOR) BroadcastTeamChange(client, -1);

	// count current clients and rank for scoreboard
	CalculateRanks();

	return NULL;
}

void ClientBegin(int clientNum) {
	gentity_t *ent;
	gclient_t *client;
	gentity_t *tent;
	int flags;

	ent = g_entities + clientNum;

	client = level.clients + clientNum;

	if(ent->r.linked) trap_UnlinkEntity(ent);
	G_InitGentity(ent);
	ent->touch = 0;
	ent->pain = 0;
	ent->client = client;

	client->pers.connected = CON_CONNECTED;
	client->pers.teamState.state = TEAM_BEGIN;

	flags = client->ps.eFlags;
	memset(&client->ps, 0, sizeof(client->ps));
	client->ps.eFlags = flags;

	// locate ent at a spawn point
	ClientSpawn(ent);

	if(client->sess.sessionTeam != TEAM_SPECTATOR) {
		// send event
		tent = G_TempEntity(ent->client->ps.origin, EV_PLAYER_TELEPORT_IN);
		tent->s.clientNum = ent->s.clientNum;

		trap_SendServerCommand(-1, va("print \"%s" S_COLOR_WHITE " entered the game\n\"", client->pers.netname));
	}

	// count current clients and rank for scoreboard
	CalculateRanks();
}

static void SetupCustomBot(gentity_t *bot) {
	if(!bot->botspawn) return;

	// give bot weapons
	if(bot->botspawn->weapon <= 1) {
		Set_Weapon(bot, WP_GAUNTLET, WS_HAVE);
	}
	Set_Weapon(bot, bot->botspawn->weapon, WS_HAVE);
	if(bot->swep_ammo[bot->botspawn->weapon] != -1) {
		Set_Ammo(bot, bot->botspawn->weapon, 9999);
	}

	bot->health = bot->client->ps.stats[STAT_HEALTH] = bot->client->ps.stats[STAT_MAX_HEALTH] = bot->botspawn->health;

	CopyAlloc(bot->target, bot->botspawn->target);  // noire.dev bot->target
}

void SetUnlimitedWeapons(gentity_t *ent) {
	Set_Ammo(ent, WP_GAUNTLET, -1);
	Set_Ammo(ent, WP_GRAPPLING_HOOK, -1);
	Set_Ammo(ent, WP_TOOLGUN, -1);
	Set_Ammo(ent, WP_PHYSGUN, -1);
	Set_Ammo(ent, WP_GRAVITYGUN, -1);
}

static void SetSandboxWeapons(gentity_t *ent) {
	if(g_gametype.integer == GT_SANDBOX) {
		Set_Weapon(ent, WP_TOOLGUN, WS_HAVE);
		Set_Weapon(ent, WP_PHYSGUN, WS_HAVE);
		Set_Weapon(ent, WP_GRAVITYGUN, WS_HAVE);
	}
}

static void SetCustomWeapons(gentity_t *ent) {
	int i;
	Set_Ammo(ent, WP_GAUNTLET, -1);
	Set_Ammo(ent, WP_GRAPPLING_HOOK, -1);
	Set_Ammo(ent, WP_TOOLGUN, -1);
	Set_Ammo(ent, WP_PHYSGUN, -1);
	Set_Ammo(ent, WP_GRAVITYGUN, -1);
	Set_Weapon(ent, WP_GAUNTLET, WS_HAVE);
	if(g_spawn_machinegun.integer > 0) {
		Set_Weapon(ent, WP_MACHINEGUN, WS_HAVE);
		Set_Ammo(ent, WP_MACHINEGUN, g_spawn_machinegun.integer);
	}
	if(g_spawn_shotgun.integer > 0) {
		Set_Weapon(ent, WP_SHOTGUN, WS_HAVE);
		Set_Ammo(ent, WP_SHOTGUN, g_spawn_shotgun.integer);
	}
	if(g_spawn_grenade.integer > 0) {
		Set_Weapon(ent, WP_GRENADE_LAUNCHER, WS_HAVE);
		Set_Ammo(ent, WP_GRENADE_LAUNCHER, g_spawn_grenade.integer);
	}
	if(g_spawn_rocket.integer > 0) {
		Set_Weapon(ent, WP_ROCKET_LAUNCHER, WS_HAVE);
		Set_Ammo(ent, WP_ROCKET_LAUNCHER, g_spawn_rocket.integer);
	}
	if(g_spawn_lightning.integer > 0) {
		Set_Weapon(ent, WP_LIGHTNING, WS_HAVE);
		Set_Ammo(ent, WP_LIGHTNING, g_spawn_lightning.integer);
	}
	if(g_spawn_railgun.integer > 0) {
		Set_Weapon(ent, WP_RAILGUN, WS_HAVE);
		Set_Ammo(ent, WP_RAILGUN, g_spawn_railgun.integer);
	}
	if(g_spawn_plasmagun.integer > 0) {
		Set_Weapon(ent, WP_PLASMAGUN, WS_HAVE);
		Set_Ammo(ent, WP_PLASMAGUN, g_spawn_plasmagun.integer);
	}
	if(g_spawn_bfg.integer > 0) {
		Set_Weapon(ent, WP_BFG, WS_HAVE);
		Set_Ammo(ent, WP_BFG, g_spawn_bfg.integer);
	}
	if(g_spawn_grapple.integer) {
		Set_Weapon(ent, WP_GRAPPLING_HOOK, WS_HAVE);
	}
	if(g_spawn_nailgun.integer > 0) {
		Set_Weapon(ent, WP_NAILGUN, WS_HAVE);
		Set_Ammo(ent, WP_NAILGUN, g_spawn_nailgun.integer);
	}
	if(g_spawn_prox.integer > 0) {
		Set_Weapon(ent, WP_PROX_LAUNCHER, WS_HAVE);
		Set_Ammo(ent, WP_PROX_LAUNCHER, g_spawn_prox.integer);
	}
	if(g_spawn_chaingun.integer > 0) {
		Set_Weapon(ent, WP_CHAINGUN, WS_HAVE);
		Set_Ammo(ent, WP_CHAINGUN, g_spawn_chaingun.integer);
	}

	ent->health = ent->client->ps.stats[STAT_ARMOR] = g_spawn_armor.integer;
	ent->health = ent->client->ps.stats[STAT_HEALTH] = g_spawn_health.integer;
	// Set spawnweapon
	if(g_gametype.integer == GT_SANDBOX) {
		ent->swep_id = WP_PHYSGUN;
		ent->client->ps.weapon = WP_PHYSGUN;
		ClientUserinfoChanged(ent->s.clientNum);
		return;
	} else {
		for(i = WEAPONS_NUM; i > 1; i--) {
			if(ent->swep_list[i] == WS_HAVE) {
				ent->swep_id = i;
				ent->client->ps.weapon = i;
				ClientUserinfoChanged(ent->s.clientNum);
				return;
			}
		}
		ent->swep_id = 1;
		ent->client->ps.weapon = 1;
		ClientUserinfoChanged(ent->s.clientNum);
	}
}

void ClientSpawn(gentity_t *ent) {
	int index;
	vec3_t spawn_origin, spawn_angles;
	gclient_t *client;
	int i;
	clientPersistant_t saved;
	clientSession_t savedSess;
	int persistant[MAX_PERSISTANT];
	gentity_t *spawnPoint;
	int flags;
	int savedPing;
	int eventSequence;
	char userinfo[MAX_INFO_STRING];

	index = ent - g_entities;
	client = ent->client;

	// find a spawn point
	if(ent->botspawn) {
		spawnPoint = ent->botspawn;
		VectorCopy(ent->botspawn->s.origin, spawn_origin);
		VectorCopy(ent->botspawn->s.angles, spawn_angles);
	} else if(client->sess.sessionTeam == TEAM_SPECTATOR) {
		spawnPoint = SelectSpectatorSpawnPoint(spawn_origin, spawn_angles);
	} else if(g_gametype.integer >= GT_CTF) {
		// all base oriented team games use the CTF spawn points
		spawnPoint = SelectCTFSpawnPoint(client->sess.sessionTeam, client->pers.teamState.state, spawn_origin, spawn_angles);
	} else {
		do {
			// don't spawn near existing origin if possible
			spawnPoint = SelectSpawnPoint(client->ps.origin, spawn_origin, spawn_angles);

			if((spawnPoint->flags & FL_NO_BOTS) && (ent->r.svFlags & SVF_BOT)) continue;

			if((spawnPoint->flags & FL_NO_HUMANS) && !(ent->r.svFlags & SVF_BOT)) continue;

			break;

		} while(1);
	}
	client->pers.teamState.state = TEAM_ACTIVE;

	// always clear the kamikaze flag
	ent->s.eFlags &= ~EF_KAMIKAZE;

	// toggle the teleport bit so the client knows to not lerp
	flags = ent->client->ps.eFlags & (EF_TELEPORT_BIT);
	flags ^= EF_TELEPORT_BIT;

	// clear everything but the persistant data
	G_ResetHistory(ent);

	saved = client->pers;
	savedSess = client->sess;
	savedPing = client->ps.ping;
	for(i = 0; i < MAX_PERSISTANT; i++) {
		persistant[i] = client->ps.persistant[i];
	}
	eventSequence = client->ps.eventSequence;

	memset(client, 0, sizeof(*client));

	client->pers = saved;
	client->sess = savedSess;
	client->ps.ping = savedPing;

	for(i = 0; i < MAX_PERSISTANT; i++) {
		client->ps.persistant[i] = persistant[i];
	}
	client->ps.eventSequence = eventSequence;
	// increment the spawncount so the client will detect the respawn
	client->ps.persistant[PERS_SPAWN_COUNT]++;
	client->ps.persistant[PERS_TEAM] = client->sess.sessionTeam;
	client->airOutTime = level.time + 12000;

	trap_GetUserinfo(index, userinfo, sizeof(userinfo));
	// set max health
	client->pers.maxHealth = 100;
	// clear entity values
	client->ps.stats[STAT_MAX_HEALTH] = client->pers.maxHealth;
	client->ps.eFlags = flags;

	ent->s.groundEntityNum = ENTITYNUM_NONE;
	ent->client = &level.clients[index];
	ent->takedamage = qtrue;
	ent->inuse = qtrue;
	ent->classname = "player";
	ent->r.contents = CONTENTS_BODY;
	ent->clipmask = MASK_PLAYERSOLID;
	ent->die = player_die;
	ent->waterlevel = 0;
	ent->watertype = 0;
	ent->flags = 0;

	VectorCopy(playerMins, ent->r.mins);
	VectorCopy(playerMaxs, ent->r.maxs);

	client->ps.clientNum = index;

	// health will count down towards max_health
	ent->health = client->ps.stats[STAT_HEALTH] = client->ps.stats[STAT_MAX_HEALTH] + 25;

	G_SetOrigin(ent, spawn_origin);
	VectorCopy(spawn_origin, client->ps.origin);

	// the respawned flag will be cleared after the attack and jump keys come up
	client->ps.pm_flags |= PMF_RESPAWNED;

	trap_GetUsercmd(client - level.clients, &ent->client->pers.cmd);
	SetClientViewAngle(ent, spawn_angles);

	if(ent->client->sess.sessionTeam != TEAM_SPECTATOR) {
		G_KillBox(ent);
		trap_LinkEntity(ent);

		client->ps.weaponstate = WEAPON_READY;
		for(i = 1; i < WEAPONS_NUM; i++) {
			ent->swep_list[i] = WS_NONE;
			ent->swep_ammo[i] = 0;
		}
		SetUnlimitedWeapons(ent);
		SetSandboxWeapons(ent);
		if(ent->botspawn) {
			SetupCustomBot(ent);
		} else {
			SetCustomWeapons(ent);
		}
	}

	G_SendSpawnSwepWeapons(ent);

	// don't allow full run speed for a bit
	client->ps.pm_flags |= PMF_TIME_KNOCKBACK;
	client->ps.pm_time = 100;

	client->respawnTime = level.time;
	client->latched_buttons = 0;

	// set default animations
	client->ps.torsoAnim = TORSO_STAND;
	client->ps.legsAnim = LEGS_IDLE;

	if(level.intermissiontime) {
		MoveClientToIntermission(ent);
	} else {
		G_UseTargets(spawnPoint, ent);
	}

	// run a client frame to drop exactly to the floor,
	// initialize animations and other things
	client->ps.commandTime = level.time - 100;
	ent->client->pers.cmd.serverTime = level.time;
	ClientThink(ent - g_entities);

	// positively link the client, even if the command times are weird
	if(ent->client->sess.sessionTeam != TEAM_SPECTATOR) {
		BG_PlayerStateToEntityState(&client->ps, &ent->s, qtrue);
		VectorCopy(ent->client->ps.origin, ent->r.currentOrigin);
		trap_LinkEntity(ent);
	}

	// run the presend to set anything else
	ClientEndFrame(ent);

	// clear entity state values
	BG_PlayerStateToEntityState(&client->ps, &ent->s, qtrue);

	RespawnTimeMessage(ent, 0);

	G_SendGameCvars(ent);
}

void ClientDisconnect(int clientNum) {
	gentity_t *ent;
	int i;

	ent = g_entities + clientNum;
	if(!ent->client) return;

	// stop any following clients
	for(i = 0; i < level.maxclients; i++) {
		if(level.clients[i].sess.sessionTeam == TEAM_SPECTATOR && level.clients[i].sess.spectatorState == SPECTATOR_FOLLOW && level.clients[i].sess.spectatorClient == clientNum) {
			StopFollowing(&g_entities[i]);
		}
	}

	// send effect if they were completely connected
	if(ent->client->pers.connected == CON_CONNECTED && ent->client->sess.sessionTeam != TEAM_SPECTATOR) {
		// They don't get to take powerups with them!
		// Especially important for stuff like CTF flags
		TossClientItems(ent);
		TossClientPersistantPowerups(ent);
		if(g_gametype.integer == GT_HARVESTER) TossClientCubes(ent);
	}

	trap_UnlinkEntity(ent);
	ent->s.modelindex = 0;
	ent->inuse = qfalse;
	ent->classname = "disconnected";
	ent->client->pers.connected = CON_DISCONNECTED;
	ent->client->ps.persistant[PERS_TEAM] = TEAM_FREE;
	ent->client->sess.sessionTeam = TEAM_FREE;

	trap_SetConfigstring(CS_PLAYERS + clientNum, "");

	CalculateRanks();

	if(ent->r.svFlags & SVF_BOT) BotAIShutdownClient(clientNum, qfalse);
}

void DropClientSilently(int clientNum) { 
	if(g_entities[clientNum].npcType > NT_PLAYER) trap_DropClient(clientNum, "DR_SILENT_DROP");
}
