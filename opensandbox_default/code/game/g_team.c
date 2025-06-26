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

#include "g_local.h"

#define OBELISK_HEALTH 2500
#define OBELISK_REGEN 2500
#define OBELISK_REGENTIME 2500
#define OBELISK_RESPAWN 2500

#define CTF_CAPTURE_BONUS 100
#define CTF_TEAM_BONUS 25
#define CTF_RECOVERY_BONUS 10
#define CTF_FLAG_BONUS 10
#define CTF_RETURN_FLAG_ASSIST_BONUS 10
#define CTF_FRAG_CARRIER_ASSIST_BONUS 10
#define CTF_FRAG_CARRIER_ASSIST_TIMEOUT 10000
#define CTF_RETURN_FLAG_ASSIST_TIMEOUT 10000

#define OVERLOAD_ATTACK_BASE_SOUND_TIME 20000

typedef struct teamgame_s {
	float last_flag_capture;
	int last_capture_team;
	flagStatus_t redStatus;   // CTF
	flagStatus_t blueStatus;  // CTF
	flagStatus_t flagStatus;  // One Flag CTF
	int redTakenTime;
	int blueTakenTime;
	int redObeliskAttackedTime;
	int blueObeliskAttackedTime;
} teamgame_t;

teamgame_t teamgame;

gentity_t *neutralObelisk;

static char ctfFlagStatusRemap[] = {'0', '1', '*', '*', '2'};
static char oneFlagStatusRemap[] = {'0', '1', '2', '3', '4'};

static void Team_SetFlagStatus(int team, flagStatus_t status) {
	qboolean modified = qfalse;

	switch(team) {
		case TEAM_RED:  // CTF
			if(teamgame.redStatus != status) {
				teamgame.redStatus = status;
				modified = qtrue;
			}
			break;

		case TEAM_BLUE:  // CTF
			if(teamgame.blueStatus != status) {
				teamgame.blueStatus = status;
				modified = qtrue;
			}
			break;

		case TEAM_FREE:  // One Flag CTF
			if(teamgame.flagStatus != status) {
				teamgame.flagStatus = status;
				modified = qtrue;
			}
			break;
	}

	if(modified) {
		char st[4];

		if(g_gametype.integer == GT_CTF) {
			st[0] = ctfFlagStatusRemap[teamgame.redStatus];
			st[1] = ctfFlagStatusRemap[teamgame.blueStatus];
			st[2] = 0;
		} else {  // GT_1FCTF
			st[0] = oneFlagStatusRemap[teamgame.flagStatus];
			st[1] = 0;
		}

		trap_SetConfigstring(CS_FLAGSTATUS, st);
	}
}

void Team_InitGame(void) {
	memset(&teamgame, 0, sizeof teamgame);

	switch(g_gametype.integer) {
		case GT_CTF:
			teamgame.redStatus = teamgame.blueStatus = -1;  // Invalid to force update
			Team_SetFlagStatus(TEAM_RED, FLAG_ATBASE);
			Team_SetFlagStatus(TEAM_BLUE, FLAG_ATBASE);
			break;
		case GT_1FCTF:
			teamgame.flagStatus = -1;  // Invalid to force update
			Team_SetFlagStatus(TEAM_FREE, FLAG_ATBASE);
			break;
		default: break;
	}
}

static int OtherTeam(int team) {
	if(team == TEAM_RED)
		return TEAM_BLUE;
	else if(team == TEAM_BLUE)
		return TEAM_RED;
	return team;
}

static const char *TeamName(int team) {
	if(team == TEAM_RED)
		return "RED";
	else if(team == TEAM_BLUE)
		return "BLUE";
	else if(team == TEAM_SPECTATOR)
		return "SPECTATOR";
	return "FREE";
}

// NULL for everyone
static void QDECL PrintMsg(gentity_t *ent, const char *fmt, ...) {
	char msg[1024];
	va_list argptr;
	char *p;

	va_start(argptr, fmt);
	if(Q_vsnprintf(msg, sizeof(msg), fmt, argptr) >= sizeof(msg)) {
		G_Error("PrintMsg overrun");
	}
	va_end(argptr);

	// double quotes are bad
	while((p = strchr(msg, '"')) != NULL) *p = '\'';

	trap_SendServerCommand(((ent == NULL) ? -1 : ent - g_entities), va("print \"%s\"", msg));
}

/*
==============
AddTeamScore
 used for gametype > GT_TEAM
 for gametype GT_TEAM the level.teamScores is updated in AddScore in g_combat.c
==============
*/
static void AddTeamScore(vec3_t origin, int team, int score) {
	gentity_t *te;

	te = G_TempEntity(origin, EV_GLOBAL_TEAM_SOUND);
	te->r.svFlags |= SVF_BROADCAST;

	if(team == TEAM_RED) {
		if(level.teamScores[TEAM_RED] + score == level.teamScores[TEAM_BLUE]) {
			// teams are tied sound
			te->s.eventParm = GTS_TEAMS_ARE_TIED;
		} else if(level.teamScores[TEAM_RED] <= level.teamScores[TEAM_BLUE] && level.teamScores[TEAM_RED] + score > level.teamScores[TEAM_BLUE]) {
			// red took the lead sound
			te->s.eventParm = GTS_REDTEAM_TOOK_LEAD;
		} else {
			// red scored sound
			te->s.eventParm = GTS_REDTEAM_SCORED;
		}
	} else {
		if(level.teamScores[TEAM_BLUE] + score == level.teamScores[TEAM_RED]) {
			// teams are tied sound
			te->s.eventParm = GTS_TEAMS_ARE_TIED;
		} else if(level.teamScores[TEAM_BLUE] <= level.teamScores[TEAM_RED] && level.teamScores[TEAM_BLUE] + score > level.teamScores[TEAM_RED]) {
			// blue took the lead sound
			te->s.eventParm = GTS_BLUETEAM_TOOK_LEAD;
		} else {
			// blue scored sound
			te->s.eventParm = GTS_BLUETEAM_SCORED;
		}
	}
	level.teamScores[team] += score;
}

/*
==============
OnSameTeam
==============
*/
qboolean OnSameTeam(gentity_t *ent1, gentity_t *ent2) {
	if(!ent1->client || !ent2->client) {
		return qfalse;
	}

	if(g_gametype.integer < GT_TEAM) {
		return qfalse;
	}

	if(ent1->client->sess.sessionTeam == ent2->client->sess.sessionTeam) {
		return qtrue;
	}

	return qfalse;
}

void Team_CheckDroppedItem(gentity_t *dropped) {
	if(dropped->item->giTag == PW_REDFLAG) {
		Team_SetFlagStatus(TEAM_RED, FLAG_DROPPED);
	} else if(dropped->item->giTag == PW_BLUEFLAG) {
		Team_SetFlagStatus(TEAM_BLUE, FLAG_DROPPED);
	} else if(dropped->item->giTag == PW_NEUTRALFLAG) {
		Team_SetFlagStatus(TEAM_FREE, FLAG_DROPPED);
	}
}

static gentity_t *Team_ResetFlag(int team) {
	char *c;
	gentity_t *ent, *rent = NULL;

	switch(team) {
		case TEAM_RED: c = "team_CTF_redflag"; break;
		case TEAM_BLUE: c = "team_CTF_blueflag"; break;
		case TEAM_FREE: c = "team_CTF_neutralflag"; break;
		default: return NULL;
	}

	ent = NULL;
	while((ent = G_Find(ent, FOFS(classname), c)) != NULL) {
		if(ent->flags & FL_DROPPED_ITEM)
			G_FreeEntity(ent);
		else {
			rent = ent;
			RespawnItem(ent);
		}
	}

	Team_SetFlagStatus(team, FLAG_ATBASE);

	return rent;
}

static void Team_ResetFlags(void) {
	if(g_gametype.integer == GT_CTF) {
		Team_ResetFlag(TEAM_RED);
		Team_ResetFlag(TEAM_BLUE);
	} else if(g_gametype.integer == GT_1FCTF) {
		Team_ResetFlag(TEAM_FREE);
	}
}

static void Team_ReturnFlagSound(gentity_t *ent, int team) {
	gentity_t *te;

	if(ent == NULL) {
		G_Printf("Warning:  NULL passed to Team_ReturnFlagSound\n");
		return;
	}

	te = G_TempEntity(ent->s.pos.trBase, EV_GLOBAL_TEAM_SOUND);
	if(team == TEAM_BLUE) {
		te->s.eventParm = GTS_RED_RETURN;
	} else {
		te->s.eventParm = GTS_BLUE_RETURN;
	}
	te->r.svFlags |= SVF_BROADCAST;
}

static void Team_TakeFlagSound(gentity_t *ent, int team) {
	gentity_t *te;

	if(ent == NULL) {
		G_Printf("Warning:  NULL passed to Team_TakeFlagSound\n");
		return;
	}

	// only play sound when the flag was at the base
	// or not picked up the last 10 seconds
	switch(team) {
		case TEAM_RED:
			if(teamgame.blueStatus != FLAG_ATBASE) {
				if(teamgame.blueTakenTime > level.time - 10000) return;
			}
			teamgame.blueTakenTime = level.time;
			break;

		case TEAM_BLUE:  // CTF
			if(teamgame.redStatus != FLAG_ATBASE) {
				if(teamgame.redTakenTime > level.time - 10000) return;
			}
			teamgame.redTakenTime = level.time;
			break;
	}

	te = G_TempEntity(ent->s.pos.trBase, EV_GLOBAL_TEAM_SOUND);
	if(team == TEAM_BLUE) {
		te->s.eventParm = GTS_RED_TAKEN;
	} else {
		te->s.eventParm = GTS_BLUE_TAKEN;
	}
	te->r.svFlags |= SVF_BROADCAST;
}

static void Team_CaptureFlagSound(gentity_t *ent, int team) {
	gentity_t *te;

	if(ent == NULL) {
		G_Printf("Warning:  NULL passed to Team_CaptureFlagSound\n");
		return;
	}

	te = G_TempEntity(ent->s.pos.trBase, EV_GLOBAL_TEAM_SOUND);
	if(team == TEAM_BLUE) {
		te->s.eventParm = GTS_BLUE_CAPTURE;
	} else {
		te->s.eventParm = GTS_RED_CAPTURE;
	}
	te->r.svFlags |= SVF_BROADCAST;
}

void Team_ReturnFlag(int team) {
	Team_ReturnFlagSound(Team_ResetFlag(team), team);
	if(team == TEAM_FREE) {
		PrintMsg(NULL, "The flag has returned!\n");
	} else {
		PrintMsg(NULL, "The %s flag has returned!\n", TeamName(team));
	}
}

void Team_FreeEntity(gentity_t *ent) {
	if(ent->item->giTag == PW_REDFLAG) {
		Team_ReturnFlag(TEAM_RED);
	} else if(ent->item->giTag == PW_BLUEFLAG) {
		Team_ReturnFlag(TEAM_BLUE);
	} else if(ent->item->giTag == PW_NEUTRALFLAG) {
		Team_ReturnFlag(TEAM_FREE);
	}
}

/*
==============
Team_DroppedFlagThink
Automatically set in Launch_Item if the item is one of the flags
Flags are unique in that if they are dropped, the base flag must be respawned when they time out
==============
*/
void Team_DroppedFlagThink(gentity_t *ent) {
	int team = TEAM_FREE;

	if(ent->item->giTag == PW_REDFLAG) {
		team = TEAM_RED;
	} else if(ent->item->giTag == PW_BLUEFLAG) {
		team = TEAM_BLUE;
	} else if(ent->item->giTag == PW_NEUTRALFLAG) {
		team = TEAM_FREE;
	}

	Team_ReturnFlagSound(Team_ResetFlag(team), team);
}

/*
==============
Team_DroppedFlagThink
==============
*/
static int Team_TouchOurFlag(gentity_t *ent, gentity_t *other, int team) {
	int i;
	gentity_t *player;
	gclient_t *cl = other->client;
	int enemy_flag;

	if(g_gametype.integer == GT_1FCTF) {
		enemy_flag = PW_NEUTRALFLAG;
	} else {
		if(cl->sess.sessionTeam == TEAM_RED) {
			enemy_flag = PW_BLUEFLAG;
		} else {
			enemy_flag = PW_REDFLAG;
		}

		if(ent->flags & FL_DROPPED_ITEM) {
			// hey, its not home.  return it by teleporting it back
			PrintMsg(NULL, "%s" S_COLOR_WHITE " returned the %s flag!\n", cl->pers.netname, TeamName(team));
			AddScore(other, CTF_RECOVERY_BONUS);
			other->client->pers.teamState.flagrecovery++;
			other->client->pers.teamState.lastreturnedflag = level.time;
			// ResetFlag will remove this entity!  We must return zero
			Team_ReturnFlagSound(Team_ResetFlag(team), team);
			return 0;
		}
	}

	// the flag is at home base.  if the player has the enemy
	// flag, he's just won!
	if(!cl->ps.powerups[enemy_flag]) return 0;  // We don't have the flag
	if(g_gametype.integer == GT_1FCTF) {
		PrintMsg(NULL, "%s" S_COLOR_WHITE " captured the flag!\n", cl->pers.netname);
	} else {
		PrintMsg(NULL, "%s" S_COLOR_WHITE " captured the %s flag!\n", cl->pers.netname, TeamName(OtherTeam(team)));
	}

	cl->ps.powerups[enemy_flag] = 0;

	teamgame.last_flag_capture = level.time;
	teamgame.last_capture_team = team;

	// Increase the team's score
	AddTeamScore(ent->s.pos.trBase, other->client->sess.sessionTeam, 1);

	other->client->pers.teamState.captures++;

	// other gets another 10 frag bonus
	AddScore(other, CTF_CAPTURE_BONUS);

	Team_CaptureFlagSound(ent, team);

	// Ok, let's do the player loop, hand out the bonuses
	for(i = 0; i < g_maxClients.integer; i++) {
		player = &g_entities[i];
		if(!player->inuse) continue;

		if(player->client->sess.sessionTeam != cl->sess.sessionTeam) {
			player->client->pers.teamState.lasthurtcarrier = -5;
		} else if(player->client->sess.sessionTeam == cl->sess.sessionTeam) {
			if(player != other) AddScore(player, CTF_TEAM_BONUS);
			// award extra points for capture assists
			if(player->client->pers.teamState.lastreturnedflag + CTF_RETURN_FLAG_ASSIST_TIMEOUT > level.time) {
				AddScore(player, CTF_RETURN_FLAG_ASSIST_BONUS);
				other->client->pers.teamState.assists++;

			} else if(player->client->pers.teamState.lastfraggedcarrier + CTF_FRAG_CARRIER_ASSIST_TIMEOUT > level.time) {
				AddScore(player, CTF_FRAG_CARRIER_ASSIST_BONUS);
				other->client->pers.teamState.assists++;
			}
		}
	}
	Team_ResetFlags();

	CalculateRanks();

	return 0;  // Do not respawn this automatically
}

static int Team_TouchEnemyFlag(gentity_t *ent, gentity_t *other, int team) {
	gclient_t *cl = other->client;

	if(g_gametype.integer == GT_1FCTF) {
		PrintMsg(NULL, "%s" S_COLOR_WHITE " got the flag!\n", other->client->pers.netname);

		cl->ps.powerups[PW_NEUTRALFLAG] = INT_MAX;  // flags never expire

		if(team == TEAM_RED) {
			Team_SetFlagStatus(TEAM_FREE, FLAG_TAKEN_RED);
		} else {
			Team_SetFlagStatus(TEAM_FREE, FLAG_TAKEN_BLUE);
		}
	} else {
		PrintMsg(NULL, "%s" S_COLOR_WHITE " got the %s flag!\n", other->client->pers.netname, TeamName(team));

		if(team == TEAM_RED)
			cl->ps.powerups[PW_REDFLAG] = INT_MAX;  // flags never expire
		else
			cl->ps.powerups[PW_BLUEFLAG] = INT_MAX;  // flags never expire

		Team_SetFlagStatus(team, FLAG_TAKEN);
	}

	AddScore(other, CTF_FLAG_BONUS);
	cl->pers.teamState.flagsince = level.time;
	Team_TakeFlagSound(ent, team);

	return -1;  // Do not respawn this automatically, but do delete it if it was FL_DROPPED
}

int Pickup_Team(gentity_t *ent, gentity_t *other) {
	int team;
	gclient_t *cl = other->client;

	if(g_gametype.integer == GT_OBELISK) {
		// there are no team items that can be picked up in obelisk
		G_FreeEntity(ent);
		return 0;
	}

	if(g_gametype.integer == GT_HARVESTER) {
		// the only team items that can be picked up in harvester are the cubes
		if(ent->spawnflags != cl->sess.sessionTeam) {
			cl->ps.generic1 += 1;
		}
		G_FreeEntity(ent);
		return 0;
	}
	// figure out what team this flag is
	if(strcmp(ent->classname, "team_CTF_redflag") == 0) {
		team = TEAM_RED;
	} else if(strcmp(ent->classname, "team_CTF_blueflag") == 0) {
		team = TEAM_BLUE;
	} else if(strcmp(ent->classname, "team_CTF_neutralflag") == 0) {
		team = TEAM_FREE;
	} else {
		PrintMsg(other, "Don't know what team the flag is on.\n");
		return 0;
	}
	if(g_gametype.integer == GT_1FCTF) {
		if(team == TEAM_FREE) {
			return Team_TouchEnemyFlag(ent, other, cl->sess.sessionTeam);
		}
		if(team != cl->sess.sessionTeam) {
			return Team_TouchOurFlag(ent, other, cl->sess.sessionTeam);
		}
		return 0;
	}
	// GT_CTF
	if(team == cl->sess.sessionTeam) {
		return Team_TouchOurFlag(ent, other, team);
	}
	return Team_TouchEnemyFlag(ent, other, team);
}

/*
===========
Team_GetLocation
Report a location for the player. Uses placed nearby target_location entities
============
*/
static gentity_t *Team_GetLocation(gentity_t *ent) {
	gentity_t *eloc, *best;
	float bestlen, len;
	vec3_t origin;

	best = NULL;
	bestlen = 3 * 8192.0 * 8192.0;

	VectorCopy(ent->r.currentOrigin, origin);

	for(eloc = level.locationHead; eloc; eloc = eloc->nextTrain) {
		len = (origin[0] - eloc->r.currentOrigin[0]) * (origin[0] - eloc->r.currentOrigin[0]) + (origin[1] - eloc->r.currentOrigin[1]) * (origin[1] - eloc->r.currentOrigin[1]) + (origin[2] - eloc->r.currentOrigin[2]) * (origin[2] - eloc->r.currentOrigin[2]);

		if(len > bestlen) {
			continue;
		}

		if(!trap_InPVS(origin, eloc->r.currentOrigin)) {
			continue;
		}

		bestlen = len;
		best = eloc;
	}

	return best;
}

/*
===========
Team_GetLocation
Report a location for the player. Uses placed nearby target_location entities
============
*/
qboolean Team_GetLocationMsg(gentity_t *ent, char *loc, int loclen) {
	gentity_t *best;

	best = Team_GetLocation(ent);

	if(!best) return qfalse;

	if(best->count) {
		if(best->count < 0) best->count = 0;
		if(best->count > 7) best->count = 7;
		Com_sprintf(loc, loclen, "%c%c%s" S_COLOR_WHITE, Q_COLOR_ESCAPE, best->count + '0', best->message);
	} else
		Com_sprintf(loc, loclen, "%s", best->message);

	return qtrue;
}

/*
================
SelectRandomDeathmatchSpawnPoint
go to a random point that doesn't telefrag
================
*/
#define MAX_TEAM_SPAWN_POINTS 32
static gentity_t *SelectRandomTeamSpawnPoint(int teamstate, team_t team) {
	gentity_t *spot;
	int count;
	int selection;
	gentity_t *spots[MAX_TEAM_SPAWN_POINTS];
	char *classname;

	if(teamstate == TEAM_BEGIN) {
		if(team == TEAM_RED)
			classname = "team_CTF_redplayer";
		else if(team == TEAM_BLUE)
			classname = "team_CTF_blueplayer";
		else
			return NULL;
	} else {
		if(team == TEAM_RED)
			classname = "team_CTF_redspawn";
		else if(team == TEAM_BLUE)
			classname = "team_CTF_bluespawn";
		else
			return NULL;
	}
	count = 0;

	spot = NULL;

	while((spot = G_Find(spot, FOFS(classname), classname)) != NULL) {
		if(SpotWouldTelefrag(spot)) {
			continue;
		}
		spots[count] = spot;
		if(++count == MAX_TEAM_SPAWN_POINTS) break;
	}

	if(!count) {  // no spots that won't telefrag
		return G_Find(NULL, FOFS(classname), classname);
	}

	selection = rand() % count;
	return spots[selection];
}

/*
===========
SelectCTFSpawnPoint
============
*/
gentity_t *SelectCTFSpawnPoint(team_t team, int teamstate, vec3_t origin, vec3_t angles) {
	gentity_t *spot;

	spot = SelectRandomTeamSpawnPoint(teamstate, team);

	if(!spot) {
		return SelectSpawnPoint(vec3_origin, origin, angles);
	}

	VectorCopy(spot->s.origin, origin);
	origin[2] += 9;
	VectorCopy(spot->s.angles, angles);

	return spot;
}

/*
================
Obelisks
================
*/
static void ObeliskHealthChange(int team, int health) {
	int currentPercentage;
	int percentage = (health * 100) / 2500;
	if(percentage < 0) percentage = 0;
	currentPercentage = level.healthRedObelisk;
	if(team != TEAM_RED) currentPercentage = level.healthBlueObelisk;
	if(percentage != currentPercentage) {
		if(team == TEAM_RED) {
			level.healthRedObelisk = percentage;
		} else {
			level.healthBlueObelisk = percentage;
		}
		level.MustSendObeliskHealth = qtrue;
		ObeliskHealthMessage();
	}
}

static void ObeliskRegen(gentity_t *self) {
	self->nextthink = level.time + OBELISK_REGENTIME * 1000;
	ObeliskHealthChange(self->spawnflags,self->health);
	if(self->health >= OBELISK_HEALTH) {
		return;
	}

	self->health += OBELISK_REGEN;
	if(self->health > OBELISK_HEALTH) {
		self->health = OBELISK_HEALTH;
	}

	self->activator->s.modelindex2 = self->health * 0xff / OBELISK_HEALTH;
	self->activator->s.frame = 0;
}

static void ObeliskRespawn(gentity_t *self) {
	self->takedamage = qtrue;
	self->health = OBELISK_HEALTH;

	self->think = ObeliskRegen;
	self->nextthink = level.time + OBELISK_REGENTIME * 1000;

	self->activator->s.frame = 0;
}

static void ObeliskDie(gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod) {
	int otherTeam;

	otherTeam = OtherTeam(self->spawnflags);
	AddTeamScore(self->s.pos.trBase, otherTeam, 1);

	CalculateRanks();

	self->takedamage = qfalse;
	self->think = ObeliskRespawn;
	self->nextthink = level.time + OBELISK_RESPAWN * 1000;

	self->activator->s.modelindex2 = 0xff;
	self->activator->s.frame = 2;

	G_AddEvent(self->activator, EV_OBELISKEXPLODE, 0);

	AddScore(attacker, CTF_CAPTURE_BONUS);

	ObeliskHealthChange(self->spawnflags,self->health);
	teamgame.redObeliskAttackedTime = 0;
	teamgame.blueObeliskAttackedTime = 0;
}

static void ObeliskTouch(gentity_t *self, gentity_t *other, trace_t *trace) {
	int tokens;

	if(!other->client) {
		return;
	}

	if(OtherTeam(other->client->sess.sessionTeam) != self->spawnflags) {
		return;
	}

	tokens = other->client->ps.generic1;
	if(tokens <= 0) {
		return;
	}

	PrintMsg(NULL, "%s" S_COLOR_WHITE " brought in %i skull%s.\n", other->client->pers.netname, tokens, tokens ? "s" : "");

	AddTeamScore(self->s.pos.trBase, other->client->sess.sessionTeam, tokens);

	AddScore(other, CTF_CAPTURE_BONUS * tokens);

	other->client->ps.generic1 = 0;
	CalculateRanks();

	Team_CaptureFlagSound(self, self->spawnflags);
}

static void ObeliskPain(gentity_t *self, gentity_t *attacker, int damage) {
	int actualDamage = damage / 10;
	if(actualDamage <= 0) {
		actualDamage = 1;
	}
	self->activator->s.modelindex2 = self->health * 0xff / OBELISK_HEALTH;
	if(!self->activator->s.frame) {
		G_AddEvent(self, EV_OBELISKPAIN, 0);
	}
	self->activator->s.frame = 1;
	AddScore(attacker, actualDamage);
}

static gentity_t *SpawnObelisk(vec3_t origin, int team, int spawnflags) {
	trace_t tr;
	vec3_t dest;
	gentity_t *ent;

	ent = G_Spawn();

	VectorCopy(origin, ent->s.origin);
	VectorCopy(origin, ent->s.pos.trBase);
	VectorCopy(origin, ent->r.currentOrigin);

	VectorSet(ent->r.mins, -15, -15, 0);
	VectorSet(ent->r.maxs, 15, 15, 87);

	ent->s.eType = ET_GENERAL;
	ent->flags = FL_NO_KNOCKBACK;

	if(g_gametype.integer == GT_OBELISK) {
		ent->r.contents = CONTENTS_SOLID;
		ent->takedamage = qtrue;
		ent->health = OBELISK_HEALTH;
		ent->die = ObeliskDie;
		ent->pain = ObeliskPain;
		ent->think = ObeliskRegen;
		ent->nextthink = level.time + OBELISK_REGENTIME * 1000;
	}
	if(g_gametype.integer == GT_HARVESTER) {
		ent->r.contents = CONTENTS_TRIGGER;
		ent->touch = ObeliskTouch;
	}

	if(spawnflags & 1) {
		// suspended
		G_SetOrigin(ent, ent->s.origin);
	} else {
		// mappers like to put them exactly on the floor, but being coplanar
		// will sometimes show up as starting in solid, so lif it up one pixel
		ent->s.origin[2] += 1;

		// drop to floor
		VectorSet(dest, ent->s.origin[0], ent->s.origin[1], ent->s.origin[2] - 4096);
		trap_Trace(&tr, ent->s.origin, ent->r.mins, ent->r.maxs, dest, ent->s.number, MASK_SOLID);
		if(tr.startsolid) {
			ent->s.origin[2] -= 1;
			G_Printf("SpawnObelisk: %s startsolid at %s\n", ent->classname, vtos(ent->s.origin));

			ent->s.groundEntityNum = ENTITYNUM_NONE;
			G_SetOrigin(ent, ent->s.origin);
		} else {
			// allow to ride movers
			ent->s.groundEntityNum = tr.entityNum;
			G_SetOrigin(ent, tr.endpos);
		}
	}

	ent->spawnflags = team;

	trap_LinkEntity(ent);

	return ent;
}

void SP_team_redobelisk(gentity_t *ent) {
	gentity_t *obelisk;

	if(g_gametype.integer <= GT_TEAM) {
		G_FreeEntity(ent);
		return;
	}
	ent->s.eType = ET_TEAM;
	if(g_gametype.integer == GT_OBELISK) {
		obelisk = SpawnObelisk(ent->s.origin, TEAM_RED, ent->spawnflags);
		obelisk->activator = ent;
		// initial obelisk health value
		ent->s.modelindex2 = 0xff;
		ent->s.frame = 0;
	}
	if(g_gametype.integer == GT_HARVESTER) {
		obelisk = SpawnObelisk(ent->s.origin, TEAM_RED, ent->spawnflags);
		obelisk->activator = ent;
	}
	ent->s.modelindex = TEAM_RED;
	trap_LinkEntity(ent);
}

void SP_team_blueobelisk(gentity_t *ent) {
	gentity_t *obelisk;

	if(g_gametype.integer <= GT_TEAM) {
		G_FreeEntity(ent);
		return;
	}
	ent->s.eType = ET_TEAM;
	if(g_gametype.integer == GT_OBELISK) {
		obelisk = SpawnObelisk(ent->s.origin, TEAM_BLUE, ent->spawnflags);
		obelisk->activator = ent;
		// initial obelisk health value
		ent->s.modelindex2 = 0xff;
		ent->s.frame = 0;
	}
	if(g_gametype.integer == GT_HARVESTER) {
		obelisk = SpawnObelisk(ent->s.origin, TEAM_BLUE, ent->spawnflags);
		obelisk->activator = ent;
	}
	ent->s.modelindex = TEAM_BLUE;
	trap_LinkEntity(ent);
}

void SP_team_neutralobelisk(gentity_t *ent) {
	if(g_gametype.integer != GT_1FCTF && g_gametype.integer != GT_HARVESTER) {
		G_FreeEntity(ent);
		return;
	}
	ent->s.eType = ET_TEAM;
	if(g_gametype.integer == GT_HARVESTER) {
		neutralObelisk = SpawnObelisk(ent->s.origin, TEAM_FREE, ent->spawnflags);
		neutralObelisk->spawnflags = TEAM_FREE;
	}
	ent->s.modelindex = TEAM_FREE;
	trap_LinkEntity(ent);
}

qboolean CheckObeliskAttack(gentity_t *obelisk, gentity_t *attacker) {
	gentity_t *te;

	// if this really is an obelisk
	if(obelisk->die != ObeliskDie) {
		return qfalse;
	}

	// if the attacker is a client
	if(!attacker->client) {
		return qfalse;
	}

	// if the obelisk is on the same team as the attacker then don't hurt it
	if(obelisk->spawnflags == attacker->client->sess.sessionTeam) {
		return qtrue;
	}

	// obelisk may be hurt

	// if not played any sounds recently
	if((obelisk->spawnflags == TEAM_RED && teamgame.redObeliskAttackedTime < level.time - OVERLOAD_ATTACK_BASE_SOUND_TIME) || (obelisk->spawnflags == TEAM_BLUE && teamgame.blueObeliskAttackedTime < level.time - OVERLOAD_ATTACK_BASE_SOUND_TIME)) {
		// tell which obelisk is under attack
		te = G_TempEntity(obelisk->s.pos.trBase, EV_GLOBAL_TEAM_SOUND);
		if(obelisk->spawnflags == TEAM_RED) {
			te->s.eventParm = GTS_REDOBELISK_ATTACKED;
			teamgame.redObeliskAttackedTime = level.time;
		} else {
			te->s.eventParm = GTS_BLUEOBELISK_ATTACKED;
			teamgame.blueObeliskAttackedTime = level.time;
		}
		te->r.svFlags |= SVF_BROADCAST;
	}

	return qfalse;
}
