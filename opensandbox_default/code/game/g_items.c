// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

#include "g_local.h"

#define RESPAWN_ARMOR 25
#define RESPAWN_HEALTH 35
#define RESPAWN_AMMO 40
#define RESPAWN_HOLDABLE 60
#define RESPAWN_MEGAHEALTH 35
#define RESPAWN_POWERUP 120
#define RESPAWN_WEAPON 5

static int Pickup_Powerup(gentity_t *ent, gentity_t *other) {
	int quantity;

	if(!other->client->ps.powerups[ent->item->giTag]) {
		other->client->ps.powerups[ent->item->giTag] = level.time - (level.time % 1000);
	}

	if(ent->count) {
		quantity = ent->count;
	} else {
		quantity = ent->item->quantity;
		other->client->ps.powerups[ent->item->giTag] += ent->count * 1000;
	}

	other->client->ps.powerups[ent->item->giTag] += quantity * 1000;

	return RESPAWN_POWERUP;
}

static int Pickup_PersistantPowerup(gentity_t *ent, gentity_t *other) {
	other->client->ps.stats[STAT_PERSISTANT_POWERUP] = ent->item - gameInfoItems;
	other->client->persistantPowerup = ent;

	switch(ent->item->giTag) {
		case PW_GUARD:
			other->health = 200;
			other->client->ps.stats[STAT_HEALTH] = 200;
			other->client->ps.stats[STAT_MAX_HEALTH] = 200;
			other->client->ps.stats[STAT_ARMOR] = 200;
			other->client->pers.maxHealth = 200;
			break;

		case PW_SCOUT:
			other->client->pers.maxHealth = 100;
			other->client->ps.stats[STAT_ARMOR] = 0;
			break;

		case PW_DOUBLER: other->client->pers.maxHealth = 100; break;
		case PW_AMMOREGEN: other->client->pers.maxHealth = 100; break;
		default: other->client->pers.maxHealth = 100; break;
	}

	return -1;
}

static int Pickup_Holdable(gentity_t *ent, gentity_t *other) {
	other->client->ps.stats[STAT_HOLDABLE_ITEM] = ent->item - gameInfoItems;

	if(ent->item->giTag == HI_KAMIKAZE) {
		other->client->ps.eFlags |= EF_KAMIKAZE;
	}

	return RESPAWN_HOLDABLE;
}

void Set_Weapon(gentity_t *ent, int weapon, int status) {
	if(status == 1) {
		ent->swep_list[weapon] = 1;
	} else {
		ent->swep_list[weapon] = 0;
	}
}

static void Add_Ammo(gentity_t *ent, int weapon, int count) {
	if(count == 9999) {
		ent->swep_ammo[weapon] = 9999;
	} else {
		ent->swep_ammo[weapon] += count;
		if(ent->swep_ammo[weapon] > 9000) {
			ent->swep_ammo[weapon] = 9000;
		}
	}
}

void Set_Ammo(gentity_t *ent, int weapon, int count) { ent->swep_ammo[weapon] = count; }

static int Pickup_Ammo(gentity_t *ent, gentity_t *other) {
	int quantity;

	if(ent->count) {
		quantity = ent->count;
	} else {
		quantity = ent->item->quantity;
	}

	Add_Ammo(other, ent->item->giTag, quantity);

	return RESPAWN_AMMO;
}

static int Pickup_Weapon(gentity_t *ent, gentity_t *other) {
	int quantity;

	if(ent->count) {
		quantity = ent->count;
	} else {
		quantity = ent->item->quantity;
	}

	// add the weapon
	Set_Weapon(other, ent->item->giTag, 1);
	Add_Ammo(other, ent->item->giTag, quantity);

	SetUnlimitedWeapons(other);

	return RESPAWN_WEAPON;
}

static int Pickup_Health(gentity_t *ent, gentity_t *other) {
	int max;
	int quantity;

	// small and mega healths will go over the max
	if(other->client && gameInfoItems[other->client->ps.stats[STAT_PERSISTANT_POWERUP]].giTag == PW_GUARD) {
		max = other->client->ps.stats[STAT_MAX_HEALTH];
	} else if(ent->item->quantity != 5 && ent->item->quantity != 100) {
		max = other->client->ps.stats[STAT_MAX_HEALTH];
	} else {
		max = other->client->ps.stats[STAT_MAX_HEALTH] * 2;
	}

	if(ent->count) {
		quantity = ent->count;
	} else {
		quantity = ent->item->quantity;
	}

	other->health += quantity;

	if(other->health > max) {
		other->health = max;
	}
	other->client->ps.stats[STAT_HEALTH] = other->health;

	if(ent->item->quantity == 100) {  // mega health respawns slow
		return RESPAWN_MEGAHEALTH;
	}

	return RESPAWN_HEALTH;
}

static int Pickup_Armor(gentity_t *ent, gentity_t *other) {
	int upperBound;

	other->client->ps.stats[STAT_ARMOR] += ent->item->quantity;

	if(other->client && gameInfoItems[other->client->ps.stats[STAT_PERSISTANT_POWERUP]].giTag == PW_GUARD) {
		upperBound = other->client->ps.stats[STAT_MAX_HEALTH];
	} else {
		upperBound = other->client->ps.stats[STAT_MAX_HEALTH] * 2;
	}

	if(other->client->ps.stats[STAT_ARMOR] > upperBound) {
		other->client->ps.stats[STAT_ARMOR] = upperBound;
	}

	return RESPAWN_ARMOR;
}

/*
===============
RespawnItem
===============
*/
void RespawnItem(gentity_t *ent) {
	int spawn_item;
	item_t *item;
	int i = 1;

	if(g_randomItems.integer && ent->item->giType != IT_RUNE && ent->item->giType != IT_TEAM) {
		char* randomitem[] = {
			"none",
			"weapon_machinegun",
			"weapon_shotgun",
			"weapon_grenadelauncher",
			"weapon_rocketlauncher",
			"weapon_lightning",
			"weapon_railgun",
			"weapon_plasmagun",
			"weapon_bfg",
			"weapon_grapplinghook",
			"weapon_nailgun",
			"weapon_prox_launcher",
			"weapon_chaingun",
			"weapon_flamethrower",
			"weapon_antimatter",
			"weapon_thrower",
			"weapon_bouncer",
			"weapon_thunder",
			"weapon_exploder",
			"weapon_knocker",
			"weapon_propgun",
			"weapon_regenerator",
			"weapon_gravitygun",
			"ammo_bullets",
			"ammo_shells",
			"ammo_grenades",
			"ammo_cells",
			"ammo_lightning",
			"ammo_rockets",
			"ammo_slugs",
			"ammo_bfg",
			"ammo_nails",
			"ammo_mines",
			"ammo_belt",
			"ammo_flame",
			"item_armor_shard",
			"item_armor_combat",
			"item_armor_body",
			"item_health_small",
			"item_health",
			"item_health_large",
			"item_health_mega",
			"item_quad",
			"item_enviro",
			"item_haste",
			"item_invis",
			"item_regen",
			"item_flight",
			"holdable_teleporter",
			"holdable_medkit",
			"holdable_kamikaze",
			"holdable_invulnerability",
			"holdable_portal",
			0
		};

		spawn_item = rq3_random(1, 55);

		for(item = gameInfoItems + 1, i = 1; item->classname; item++, i++) {
			if(!strcmp(item->classname, randomitem[spawn_item])) {
				ent->item = &gameInfoItems[i];
				ent->item->classname = gameInfoItems[i].classname;
				ent->s.modelindex = i;
			}
		}
	}

	// randomly select from teamed entities
	if(ent->team) {
		gentity_t *master;
		int count;
		int choice;

		if(!ent->teammaster) {
			G_Error("RespawnItem: bad teammaster");
		}
		master = ent->teammaster;

		for(count = 0, ent = master; ent; ent = ent->teamchain, count++);

		choice = rand() % count;

		for(count = 0, ent = master; count < choice; ent = ent->teamchain, count++);
	}

	ent->r.contents = CONTENTS_TRIGGER;
	ent->s.eFlags &= ~EF_NODRAW;
	ent->r.svFlags &= ~SVF_NOCLIENT;
	trap_LinkEntity(ent);

	// play the normal respawn sound only to nearby clients
	G_AddEvent(ent, EV_ITEM_RESPAWN, 0);

	ent->nextthink = 0;
}

/*
===============
Touch_Item
===============
*/
void Touch_Item(gentity_t *ent, gentity_t *other, trace_t *trace) {
	int respawn;
	qboolean predict;

	if(!other->client) return;
	if(other->health < 1) return;  // dead people can't pickup

	if(ent->npcType) {
		if(!gameInfoNPCTypes[ent->npcType].canPickup) return;  // npc can't pickup
	}

	// the same pickup rules are used for client side and server side
	if(!BG_CanItemBeGrabbed(g_gametype.integer, &ent->s, &other->client->ps)) {
		return;
	}

	// call the item-specific pickup function
	switch(ent->item->giType) {
		case IT_WEAPON: respawn = Pickup_Weapon(ent, other); break;
		case IT_AMMO: respawn = Pickup_Ammo(ent, other); break;
		case IT_ARMOR: respawn = Pickup_Armor(ent, other); break;
		case IT_HEALTH: respawn = Pickup_Health(ent, other); break;
		case IT_POWERUP:
			respawn = Pickup_Powerup(ent, other);
			predict = qfalse;
			break;
		case IT_RUNE: respawn = Pickup_PersistantPowerup(ent, other); break;
		case IT_TEAM: respawn = Pickup_Team(ent, other); break;
		case IT_HOLDABLE: respawn = Pickup_Holdable(ent, other); break;
		default: return;
	}

	if(!respawn) {
		return;
	}

	// play the normal pickup sound
	if(predict) {
		G_AddPredictableEvent(other, EV_ITEM_PICKUP, ent->s.modelindex);
	} else {
		G_AddEvent(other, EV_ITEM_PICKUP, ent->s.modelindex);
	}

	// fire item targets
	G_UseTargets(ent, other);

	// wait of -1 will not respawn
	if(ent->wait == -1) {
		ent->r.svFlags |= SVF_NOCLIENT;
		ent->s.eFlags |= EF_NODRAW;
		ent->r.contents = 0;
		ent->unlinkAfterEvent = qtrue;
		return;
	}

	// non zero wait overrides respawn time
	if(ent->wait) {
		respawn = ent->wait;
	}

	// random can be used to vary the respawn time
	if(ent->random) {
		respawn += crandom() * ent->random;
		if(respawn < 1) {
			respawn = 1;
		}
	}

	// dropped items will not respawn
	if(ent->flags & FL_DROPPED_ITEM) {
		ent->freeAfterEvent = qtrue;
	}

	ent->r.svFlags |= SVF_NOCLIENT;
	ent->s.eFlags |= EF_NODRAW;
	ent->r.contents = 0;

	if(respawn <= 0) {
		ent->nextthink = 0;
		ent->think = 0;
	} else {
		ent->nextthink = level.time + respawn * 1000;
		ent->think = RespawnItem;
	}
	trap_LinkEntity(ent);
}

/*
================
LaunchItem

Spawns an item and tosses it forward
================
*/
gentity_t *LaunchItem(item_t *item, vec3_t origin, vec3_t velocity) {
	gentity_t *dropped;

	dropped = G_Spawn();

	dropped->s.eType = ET_ITEM;
	dropped->s.modelindex = item - gameInfoItems;  // store item number in modelindex
	dropped->s.modelindex2 = 1;                    // This is non-zero is it's a dropped item

	dropped->classname = item->classname;
	dropped->item = item;
	VectorSet(dropped->r.mins, -ITEM_RADIUS, -ITEM_RADIUS, -ITEM_RADIUS);
	VectorSet(dropped->r.maxs, ITEM_RADIUS, ITEM_RADIUS, ITEM_RADIUS);
	dropped->r.contents = CONTENTS_TRIGGER;

	dropped->touch = Touch_Item;

	G_SetOrigin(dropped, origin);
	dropped->s.pos.trType = TR_GRAVITY;
	dropped->s.pos.trTime = level.time;
	VectorCopy(velocity, dropped->s.pos.trDelta);

	dropped->s.eFlags |= EF_BOUNCE;
	if((g_gametype.integer == GT_CTF || g_gametype.integer == GT_1FCTF) && item->giType == IT_TEAM) {  // Special case for CTF flags
		dropped->think = Team_DroppedFlagThink;
		dropped->nextthink = level.time + 30000;
		Team_CheckDroppedItem(dropped);
	} else {  // auto-remove after 30 seconds
		dropped->think = G_FreeEntity;
		dropped->nextthink = level.time + 30000;
	}

	dropped->flags = FL_DROPPED_ITEM;

	trap_LinkEntity(dropped);

	return dropped;
}

/*
================
Drop_Item

Spawns an item and tosses it forward
================
*/
gentity_t *Drop_Item(gentity_t *ent, item_t *item) {
	vec3_t velocity;
	vec3_t angles;

	VectorCopy(ent->s.apos.trBase, angles);
	angles[YAW] += random() * 360.0f;
	angles[PITCH] = 0;  // always forward

	AngleVectors(angles, velocity, NULL, NULL);
	VectorScale(velocity, 320, velocity);
	velocity[2] += 200 + crandom() * 50;

	return LaunchItem(item, ent->s.pos.trBase, velocity);
}

/*
================
Use_Item

Respawn the item
================
*/
static void Use_Item(gentity_t *ent, gentity_t *other, gentity_t *activator) { RespawnItem(ent); }

/*
================
FinishSpawningItem

Traces down to find where an item should rest, instead of letting them
free fall from their spawn points
================
*/
void FinishSpawningItem(gentity_t *ent) {
	trace_t tr;
	vec3_t dest;

	VectorSet(ent->r.mins, -ITEM_RADIUS, -ITEM_RADIUS, -ITEM_RADIUS);
	VectorSet(ent->r.maxs, ITEM_RADIUS, ITEM_RADIUS, ITEM_RADIUS);

	ent->s.eType = ET_ITEM;
	ent->s.modelindex = ent->item - gameInfoItems;  // store item number in modelindex
	ent->s.modelindex2 = 0;                         // zero indicates this isn't a dropped item

	ent->r.contents = CONTENTS_TRIGGER;
	ent->touch = Touch_Item;
	// useing an item causes it to respawn
	ent->use = Use_Item;

	if(ent->spawnflags & 1) {
		// suspended
		G_SetOrigin(ent, ent->s.origin);
	} else {
		// drop to floor
		VectorSet(dest, ent->s.origin[0], ent->s.origin[1], ent->s.origin[2] - 4096);
		trap_Trace(&tr, ent->s.origin, ent->r.mins, ent->r.maxs, dest, ent->s.number, MASK_SOLID);
		if(tr.startsolid) {
			G_Printf("FinishSpawningItem: %s startsolid at %s\n", ent->classname, vtos(ent->s.origin));
			G_FreeEntity(ent);
			return;
		}

		// allow to ride movers
		ent->s.groundEntityNum = tr.entityNum;

		G_SetOrigin(ent, tr.endpos);
	}

	// team slaves and targeted items aren't present at start
	if((ent->flags & FL_TEAMSLAVE) || ent->targetname) {
		ent->s.eFlags |= EF_NODRAW;
		ent->r.contents = 0;
		return;
	}

	// powerups don't spawn in for a while
	if(ent->item->giType == IT_POWERUP) {
		float respawn;

		respawn = 45 + crandom() * 15;
		ent->s.eFlags |= EF_NODRAW;
		ent->r.contents = 0;
		ent->nextthink = level.time + respawn * 1000;
		ent->think = RespawnItem;
		return;
	}

	trap_LinkEntity(ent);
}

/*
==================
G_CheckTeamItems
==================
*/
void G_CheckTeamItems(void) {
	// Set up team stuff
	Team_InitGame();

	if(g_gametype.integer == GT_CTF) {
		item_t *item;

		// check for the two flags
		item = BG_FindItem("Red Flag");
		if(!item) {
			G_Printf(S_COLOR_YELLOW "WARNING: No team_CTF_redflag in map\n");
		}
		item = BG_FindItem("Blue Flag");
		if(!item) {
			G_Printf(S_COLOR_YELLOW "WARNING: No team_CTF_blueflag in map\n");
		}
	}

	if(g_gametype.integer == GT_1FCTF) {
		item_t *item;

		// check for all three flags
		item = BG_FindItem("Red Flag");
		if(!item) {
			G_Printf(S_COLOR_YELLOW "WARNING: No team_CTF_redflag in map\n");
		}
		item = BG_FindItem("Blue Flag");
		if(!item) {
			G_Printf(S_COLOR_YELLOW "WARNING: No team_CTF_blueflag in map\n");
		}
		item = BG_FindItem("Neutral Flag");
		if(!item) {
			G_Printf(S_COLOR_YELLOW "WARNING: No team_CTF_neutralflag in map\n");
		}
	}

	if(g_gametype.integer == GT_OBELISK) {
		gentity_t *ent;

		// check for the two obelisks
		ent = NULL;
		ent = G_Find(ent, FOFS(classname), "team_redobelisk");
		if(!ent) {
			G_Printf(S_COLOR_YELLOW "WARNING: No team_redobelisk in map\n");
		}

		ent = NULL;
		ent = G_Find(ent, FOFS(classname), "team_blueobelisk");
		if(!ent) {
			G_Printf(S_COLOR_YELLOW "WARNING: No team_blueobelisk in map\n");
		}
	}

	if(g_gametype.integer == GT_HARVESTER) {
		gentity_t *ent;

		// check for all three obelisks
		ent = NULL;
		ent = G_Find(ent, FOFS(classname), "team_redobelisk");
		if(!ent) {
			G_Printf(S_COLOR_YELLOW "WARNING: No team_redobelisk in map\n");
		}

		ent = NULL;
		ent = G_Find(ent, FOFS(classname), "team_blueobelisk");
		if(!ent) {
			G_Printf(S_COLOR_YELLOW "WARNING: No team_blueobelisk in map\n");
		}

		ent = NULL;
		ent = G_Find(ent, FOFS(classname), "team_neutralobelisk");
		if(!ent) {
			G_Printf(S_COLOR_YELLOW "WARNING: No team_neutralobelisk in map\n");
		}
	}
}

/*
============
G_ItemDisabled
============
*/
static int G_ItemDisabled(item_t *item) {
	char name[128];

	Com_sprintf(name, sizeof(name), "disable_%s", item->classname);
	return trap_Cvar_VariableIntegerValue(name);
}

/*
============
G_SpawnItem

Sets the clipping size and plants the object on the floor.
Items can't be immediately dropped to floor, because they might
be on an entity that hasn't spawned yet.
============
*/
void G_SpawnItem(gentity_t *ent, item_t *item) {
	G_SpawnFloat("random", "0", &ent->random);
	G_SpawnFloat("wait", "0", &ent->wait);

	if(G_ItemDisabled(item)) return;

	ent->item = item;
	// some movers spawn on the second frame, so delay item
	// spawns until the third frame so they can ride trains
	ent->nextthink = level.time + FRAMETIME * 2;
	ent->think = FinishSpawningItem;
	ent->physicsBounce = 0.50;  // items are bouncy

	if(item->giType == IT_RUNE) {
		ent->s.generic1 = ent->spawnflags;
	}
}

/*
================
G_BounceItem
================
*/
static void G_BounceItem(gentity_t *ent, trace_t *trace) {
	vec3_t velocity;
	float dot;
	int hitTime;

	// reflect the velocity on the trace plane
	hitTime = level.previousTime + (level.time - level.previousTime) * trace->fraction;
	BG_EvaluateTrajectoryDelta(&ent->s.pos, hitTime, velocity);
	dot = DotProduct(velocity, trace->plane.normal);
	VectorMA(velocity, -2 * dot, trace->plane.normal, ent->s.pos.trDelta);

	// cut the velocity to keep from bouncing forever
	VectorScale(ent->s.pos.trDelta, ent->physicsBounce, ent->s.pos.trDelta);

	// check for stop
	if(trace->plane.normal[2] > 0 && ent->s.pos.trDelta[2] < 40) {
		trace->endpos[2] += 1.0;  // make sure it is off ground
		SnapVector(trace->endpos);
		G_SetOrigin(ent, trace->endpos);
		ent->s.groundEntityNum = trace->entityNum;
		return;
	}

	VectorAdd(ent->r.currentOrigin, trace->plane.normal, ent->r.currentOrigin);
	VectorCopy(ent->r.currentOrigin, ent->s.pos.trBase);
	ent->s.pos.trTime = level.time;
}

/*
================
G_RunItem
================
*/
void G_RunItem(gentity_t *ent) {
	vec3_t origin;
	trace_t tr;
	int contents;
	int mask;

	// if groundentity has been set to -1, it may have been pushed off an edge
	if(ent->s.groundEntityNum == -1) {
		if(ent->s.pos.trType != TR_GRAVITY) {
			ent->s.pos.trType = TR_GRAVITY;
			ent->s.pos.trTime = level.time;
		}
	}

	if(ent->s.pos.trType == TR_STATIONARY) {
		// check think function
		G_RunThink(ent);
		return;
	}

	// get current position
	BG_EvaluateTrajectory(&ent->s.pos, level.time, origin);

	// trace a line from the previous position to the current position
	if(ent->clipmask) {
		mask = ent->clipmask;
	} else {
		mask = MASK_PLAYERSOLID & ~CONTENTS_BODY;  // MASK_SOLID;
	}
	trap_Trace(&tr, ent->r.currentOrigin, ent->r.mins, ent->r.maxs, origin, ent->r.ownerNum, mask);

	VectorCopy(tr.endpos, ent->r.currentOrigin);

	if(tr.startsolid) {
		tr.fraction = 0;
	}

	trap_LinkEntity(ent);  // FIXME: avoid this for stationary?

	// check think function
	G_RunThink(ent);

	if(tr.fraction == 1) {
		return;
	}

	// if it is in a nodrop volume, remove it
	contents = trap_PointContents(ent->r.currentOrigin, -1);
	if(contents & CONTENTS_NODROP) {
		if(ent->item && ent->item->giType == IT_TEAM) {
			Team_FreeEntity(ent);
		} else {
			G_FreeEntity(ent);
		}
		return;
	}

	G_BounceItem(ent, &tr);
}
