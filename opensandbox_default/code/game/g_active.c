// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

#include "g_local.h"

/*
===============
G_DamageFeedback
Called just before a snapshot is sent to the given player.
Totals up all damage and generates both the player_state_t
damage values to that client for pain blends and kicks, and
global pain sound events for all clients.
===============
*/
static void P_DamageFeedback(gentity_t *player) {
	gclient_t *client;
	float count;
	vec3_t angles;

	client = player->client;
	if(client->ps.pm_type == PM_DEAD) return;

	// total points of damage shot at the player this frame
	count = client->damage_blood + client->damage_armor;
	if(count == 0) return;

	if(count > 255) count = 255;

	// send the information to the client
	// world damage (falling, slime, etc) uses a special code
	// to make the blend blob centered instead of positional
	if(client->damage_fromWorld) {
		client->ps.damagePitch = 255;
		client->ps.damageYaw = 255;

		client->damage_fromWorld = qfalse;
	} else {
		vectoangles(client->damage_from, angles);
		client->ps.damagePitch = angles[PITCH] / 360.0 * 256;
		client->ps.damageYaw = angles[YAW] / 360.0 * 256;
	}

	// play an apropriate pain sound
	if((level.time > player->pain_debounce_time) && !(player->flags & FL_GODMODE)) {
		player->pain_debounce_time = level.time + 700;
		G_AddEvent(player, EV_PAIN, player->health);
		client->ps.damageEvent++;
	}

	client->ps.damageCount = count;

	//
	// clear totals
	//
	client->damage_blood = 0;
	client->damage_armor = 0;
	client->damage_knockback = 0;
}

/*
=============
P_WorldEffects
Check for lava / slime contents and drowning
=============
*/
static void P_WorldEffects(gentity_t *ent) {
	qboolean envirosuit;
	int waterlevel;

	if(ent->client->noclip) {
		ent->client->airOutTime = level.time + 12000;  // don't need air
		return;
	}

	waterlevel = ent->waterlevel;

	envirosuit = ent->client->ps.powerups[PW_BATTLESUIT] > level.time;

	//
	// check for drowning
	//
	if(waterlevel == 3) {
		// envirosuit give air
		if(envirosuit) {
			ent->client->airOutTime = level.time + 10000;
		}

		// if out of air, start drowning
		if(ent->client->airOutTime < level.time) {
			// drown!
			ent->client->airOutTime += 1000;
			if(ent->health > 0) {
				// take more damage the longer underwater
				ent->damage += 2;
				if(ent->damage > 15) ent->damage = 15;

				// play a gurp sound instead of a normal pain sound
				if(ent->health <= ent->damage) {
					G_Sound(ent, CHAN_VOICE, G_SoundIndex("*drown.wav"));
				} else if(rand() & 1) {
					G_Sound(ent, CHAN_VOICE, G_SoundIndex("sound/player/gurp1.wav"));
				} else {
					G_Sound(ent, CHAN_VOICE, G_SoundIndex("sound/player/gurp2.wav"));
				}

				// don't play a normal pain sound
				ent->pain_debounce_time = level.time + 200;

				G_Damage(ent, NULL, NULL, NULL, NULL, ent->damage, DAMAGE_NO_ARMOR, MOD_WATER);
			}
		}
	} else {
		ent->client->airOutTime = level.time + 12000;
		ent->damage = 2;
	}

	//
	// check for sizzle damage (move to pmove?)
	//
	if(waterlevel && (ent->watertype & (CONTENTS_LAVA | CONTENTS_SLIME))) {
		if(ent->health > 0 && ent->pain_debounce_time <= level.time) {
			if(!envirosuit) {
				if(ent->watertype & CONTENTS_LAVA) {
					G_Damage(ent, NULL, NULL, NULL, NULL, 30 * waterlevel, 0, MOD_LAVA);
				}

				if(ent->watertype & CONTENTS_SLIME) {
					G_Damage(ent, NULL, NULL, NULL, NULL, 10 * waterlevel, 0, MOD_SLIME);
				}
			}
		}
	}
}

static void G_SetClientSound(gentity_t *ent) {
	if(ent->s.eFlags & EF_TICKING) {
		ent->client->ps.loopSound = G_SoundIndex("sound/weapons/proxmine/wstbtick.wav");
	} else if(ent->waterlevel && (ent->watertype & (CONTENTS_LAVA | CONTENTS_SLIME))) {
		ent->client->ps.loopSound = level.snd_fry;
	} else {
		if(ent->npcType > NT_PLAYER) {
			ent->client->ps.loopSound = G_SoundIndex(va("bots/%s", ent->target));
		} else {
			ent->client->ps.loopSound = 0;
		}
	}
}

static void ClientImpacts(gentity_t *ent, pmove_t *pm) {
	int i, j;
	trace_t trace;
	gentity_t *other;

	memset(&trace, 0, sizeof(trace));
	for(i = 0; i < pm->numtouch; i++) {
		for(j = 0; j < i; j++) {
			if(pm->touchents[j] == pm->touchents[i]) {
				break;
			}
		}
		if(j != i) {
			continue;  // duplicated
		}
		other = &g_entities[pm->touchents[i]];

		if((ent->r.svFlags & SVF_BOT) && (ent->touch)) {
			ent->touch(ent, other, &trace);
		}

		if(!other->touch) {
			continue;
		}

		other->touch(other, ent, &trace);
	}
}

/*
============
G_TouchTriggers
Find all trigger entities that ent's current position touches.
Spectators will only interact with teleporters.
============
*/
static void G_TouchTriggers(gentity_t *ent) {
	int i, num;
	int touch[MAX_GENTITIES];
	gentity_t *hit;
	trace_t trace;
	vec3_t mins, maxs;
	static vec3_t range = {40, 40, 52};

	if(!ent->client) {
		return;
	}

	// dead clients don't activate triggers!
	if(ent->client->ps.stats[STAT_HEALTH] <= 0) {
		return;
	}

	VectorSubtract(ent->client->ps.origin, range, mins);
	VectorAdd(ent->client->ps.origin, range, maxs);

	num = trap_EntitiesInBox(mins, maxs, touch, MAX_GENTITIES);

	// can't use ent->absmin, because that has a one unit pad
	VectorAdd(ent->client->ps.origin, ent->r.mins, mins);
	VectorAdd(ent->client->ps.origin, ent->r.maxs, maxs);

	for(i = 0; i < num; i++) {
		hit = &g_entities[touch[i]];

		if(!hit->touch && !ent->touch) {
			continue;
		}
		if(!(hit->r.contents & CONTENTS_TRIGGER)) {
			continue;
		}

		// ignore most entities if a spectator
		if(ent->client->sess.sessionTeam == TEAM_SPECTATOR) {
			if(hit->s.eType != ET_TELEPORT_TRIGGER &&
			   // this is ugly but adding a new ET_? type will
			   // most likely cause network incompatibilities
			   hit->touch != Touch_DoorTrigger) {
				continue;
			}
		}

		// use seperate code for determining if an item is picked up
		// so you don't have to actually contact its bounding box
		if(hit->s.eType == ET_ITEM) {
			if(!BG_PlayerTouchesItem(&ent->client->ps, &hit->s, level.time)) {
				continue;
			}
		} else {
			if(!trap_EntityContact(mins, maxs, hit)) {
				continue;
			}
		}

		memset(&trace, 0, sizeof(trace));

		if(hit->touch) {
			hit->touch(hit, ent, &trace);
		}

		if((ent->r.svFlags & SVF_BOT) && (ent->touch)) {
			ent->touch(ent, hit, &trace);
		}
	}

	// if we didn't touch a jump pad this pmove frame
	if(ent->client->ps.jumppad_frame != ent->client->ps.pmove_framecount) {
		ent->client->ps.jumppad_frame = 0;
		ent->client->ps.jumppad_ent = 0;
	}
}

static void SpectatorThink(gentity_t *ent, usercmd_t *ucmd) {
	pmove_t pm;
	gclient_t *client;

	client = ent->client;

	if(client->sess.spectatorState != SPECTATOR_FOLLOW) {
		client->ps.pm_type = PM_SPECTATOR;
		client->ps.speed = 900;  // faster than normal

		// set up for pmove
		memset(&pm, 0, sizeof(pm));
		pm.ps = &client->ps;
		pm.cmd = *ucmd;
		pm.tracemask = MASK_PLAYERSOLID & ~CONTENTS_BODY;  // spectators can fly through bodies
		pm.trace = trap_Trace;
		pm.pointcontents = trap_PointContents;

		// perform a pmove
		Pmove(&pm);
		// save results of pmove
		VectorCopy(client->ps.origin, ent->s.origin);

		G_TouchTriggers(ent);
		trap_UnlinkEntity(ent);
	}

	client->oldbuttons = client->buttons;
	client->buttons = ucmd->buttons;

	// attack button cycles through spectators
	if((client->buttons & BUTTON_ATTACK) && !(client->oldbuttons & BUTTON_ATTACK)) Cmd_FollowCycle_f(ent, 1);
}

static void G_KillVoid(gentity_t *ent) {
	vec3_t orig;

	if(!ent->client && ent->client->ps.stats[STAT_HEALTH] <= 0 && ent->client->ps.pm_type != PM_SPECTATOR) return;

	VectorCopy(ent->client->ps.origin, orig);
	if(orig[2] <= -520000) G_Damage(ent, NULL, NULL, NULL, NULL, 1000, 0, MOD_UNKNOWN);
}

static void G_MakeUnlimitedAmmo(gentity_t *ent) {
	int i;
	for(i = 1; i < WEAPONS_NUM; i++) {
		ent->swep_ammo[i] = 9999;
	}
	SetUnlimitedWeapons(ent);
}

static void ClientTimerActions(gentity_t *ent, int msec) {
	gclient_t *client;
	int maxHealth;

	client = ent->client;
	client->timeResidual += msec;

	while(client->timeResidual >= 1000) {
		client->timeResidual -= 1000;

		if(gameInfoItems[client->ps.stats[STAT_PERSISTANT_POWERUP]].giTag == PW_AMMOREGEN) G_MakeUnlimitedAmmo(ent);

		// regenerate
		if(gameInfoItems[client->ps.stats[STAT_PERSISTANT_POWERUP]].giTag == PW_GUARD) {
			maxHealth = client->ps.stats[STAT_MAX_HEALTH] / 2;
		} else if(client->ps.powerups[PW_REGEN]) {
			maxHealth = client->ps.stats[STAT_MAX_HEALTH];
		} else {
			maxHealth = 0;
		}
		if(maxHealth) {
			if(ent->health < maxHealth) {
				ent->health += 15;
				if(ent->health > maxHealth * 1.1) {
					ent->health = maxHealth * 1.1;
				}
			} else if(ent->health < maxHealth * 2) {
				ent->health += 5;
				if(ent->health > maxHealth * 2) {
					ent->health = maxHealth * 2;
				}
			}
		} else {
			if(ent->health < client->ps.stats[STAT_MAX_HEALTH]) {
				ent->health += 1;
				if(ent->health > client->ps.stats[STAT_MAX_HEALTH]) ent->health = client->ps.stats[STAT_MAX_HEALTH];
			}
		}

		G_SendGameCvars(ent);    // send game setting to client for sync
		G_SendSwepWeapons(ent);  // send sweps list to client for sync
	}
}

static void SendEntityInfoToClient(gentity_t *ent, int msec) {
	gclient_t *client;

	client = ent->client;
	client->timeEntityInfo += msec;

	while(client->timeEntityInfo >= 100) {
		client->timeEntityInfo -= 100;
		Weapon_Toolgun_Info(ent); // send entity info to client for sync
	}
}

static void ClientIntermissionThink(gclient_t *client) {
	client->ps.eFlags &= ~EF_TALK;
	client->ps.eFlags &= ~EF_FIRING;

	// the level will exit when everyone wants to or after timeouts
	if(g_entities[client->ps.clientNum].r.svFlags & SVF_BOT) return;

	// swap and latch button actions
	client->oldbuttons = client->buttons;
	client->buttons = client->pers.cmd.buttons;
	if(client->buttons & (BUTTON_ATTACK | BUTTON_USE_HOLDABLE) & (client->oldbuttons ^ client->buttons)) {
		client->readyToExit = 1;
	}
}

/*
================
ClientEvents
Events will be passed on to the clients for presentation,
but any server game effects are handled here
================
*/
static void ClientEvents(gentity_t *ent, int oldEventSequence) {
	int i, event, damage;
	gclient_t *client;
	vec3_t dir, origin, angles;

	client = ent->client;

	if(oldEventSequence < client->ps.eventSequence - MAX_PS_EVENTS) {
		oldEventSequence = client->ps.eventSequence - MAX_PS_EVENTS;
	}
	for(i = oldEventSequence; i < client->ps.eventSequence; i++) {
		event = client->ps.events[i & (MAX_PS_EVENTS - 1)];

		switch(event) {
			case EV_FALL_MEDIUM:
			case EV_FALL_FAR:
				if(ent->s.eType != ET_PLAYER) {
					break;  // not in the player model
				}
				if(event == EV_FALL_FAR) {
					damage = 10;
				} else {
					damage = 5;
				}
				VectorSet(dir, 0, 0, 1);
				ent->pain_debounce_time = level.time + 200;  // no normal pain sound
				G_Damage(ent, NULL, NULL, NULL, NULL, damage, 0, MOD_FALLING);
				break;

			case EV_FIRE_WEAPON: FireWeapon(ent); break;

			case EV_USE_ITEM1:  // teleporter
				SelectSpawnPoint(ent->client->ps.origin, origin, angles);
				TeleportPlayer(ent, origin, angles, qfalse);
				break;

			case EV_USE_ITEM2:  // medkit
				ent->health = ent->client->ps.stats[STAT_MAX_HEALTH] + 25;
				break;

			case EV_USE_ITEM3:  // kamikaze
				ent->client->invulnerabilityTime = 0;
				G_StartKamikaze(ent);
				break;

			case EV_USE_ITEM4:  // portal
				if(ent->client->portalID) {
					DropPortalSource(ent);
				} else {
					DropPortalDestination(ent);
				}
				break;
			case EV_USE_ITEM5:  // invulnerability
				ent->client->invulnerabilityTime = level.time + 10000;
				break;

			default: break;
		}
	}
}

static int StuckInOtherClient(gentity_t *ent) {
	int i;
	gentity_t *ent2;

	ent2 = &g_entities[0];
	for(i = 0; i < MAX_CLIENTS; i++, ent2++) {
		if(ent2 == ent) {
			continue;
		}
		if(!ent2->inuse) {
			continue;
		}
		if(!ent2->client) {
			continue;
		}
		if(ent2->health <= 0) {
			continue;
		}
		//
		if(ent2->r.absmin[0] > ent->r.absmax[0]) continue;
		if(ent2->r.absmin[1] > ent->r.absmax[1]) continue;
		if(ent2->r.absmin[2] > ent->r.absmax[2]) continue;
		if(ent2->r.absmax[0] < ent->r.absmin[0]) continue;
		if(ent2->r.absmax[1] < ent->r.absmin[1]) continue;
		if(ent2->r.absmax[2] < ent->r.absmin[2]) continue;
		return qtrue;
	}
	return qfalse;
}

static void SendPendingPredictableEvents(playerState_t *ps) {
	gentity_t *t;
	int event, seq;
	int extEvent, number;

	// if there are still events pending
	if(ps->entityEventSequence < ps->eventSequence) {
		// create a temporary entity for this event which is sent to everyone
		// except the client who generated the event
		seq = ps->entityEventSequence & (MAX_PS_EVENTS - 1);
		event = ps->events[seq] | ((ps->entityEventSequence & 3) << 8);
		// set external event to zero before calling BG_PlayerStateToEntityState
		extEvent = ps->externalEvent;
		ps->externalEvent = 0;
		// create temporary entity for event
		t = G_TempEntity(ps->origin, event);
		number = t->s.number;
		BG_PlayerStateToEntityState(ps, &t->s, qtrue);
		t->s.number = number;
		t->s.eType = ET_EVENTS + event;
		t->s.eFlags |= EF_PLAYER_EVENT;
		t->s.otherEntityNum = ps->clientNum;
		// send to everyone except the client who generated the event
		t->r.singleClient = ps->clientNum+1;
		// set back external event
		ps->externalEvent = extEvent;
	}
}

static void PhysgunHold(gentity_t *player) {
	gentity_t *ent = player->grabbedEntity;
	gentity_t *findent;
	vec3_t velocity;

	if(player->client->ps.weapon != WP_PHYSGUN) return;

	if(player->client->buttons & BUTTON_ATTACK && player->client->ps.stats[STAT_HEALTH] && player->client->ps.pm_type != PM_DEAD) {
		if(!player->grabbedEntity) {
			findent = FindEntityForPhysgun(player, gameInfoWeapons[WP_PHYSGUN].range);
			if(findent && findent->isGrabbed == qfalse) {
				if(!G_PlayerIsOwner(player, findent)) return;
				if(findent) player->grabbedEntity = findent;
			}
			if(player->grabbedEntity) Phys_HoldSetup(player, qtrue);
		} else {
			Phys_HoldFrame(player, velocity, qtrue);
		}
	} else if(player->grabbedEntity) {
		ent->isGrabbed = qfalse;
		if(ent->grabNewPhys == PHYS_STATIC) Phys_HoldDropStatic(player, velocity);
		if(ent->grabNewPhys == PHYS_DYNAMIC) Phys_HoldDropDynamic(player, velocity, qtrue);
		VectorClear(player->grabOffset);
		player->grabbedEntity = 0;
	}
}

static void GravitygunHold(gentity_t *player) {
	gentity_t *ent = player->grabbedEntity;
	gentity_t *findent;
	vec3_t velocity;

	if(player->client->ps.weapon != WP_GRAVITYGUN) return;

	if(player->client->buttons & BUTTON_ATTACK && player->client->ps.stats[STAT_HEALTH] && player->client->ps.pm_type != PM_DEAD) {
		if(!player->grabbedEntity) {
			findent = FindEntityForGravitygun(player, gameInfoWeapons[WP_GRAVITYGUN].range);
			if(findent && findent->isGrabbed == qfalse) {
				if(!G_PlayerIsOwner(player, findent)) return;
				if(findent) player->grabbedEntity = findent;
			}
			if(player->grabbedEntity) Phys_HoldSetup(player, qfalse);
		} else {
			Phys_HoldFrame(player, velocity, qfalse);
		}
	} else if(player->grabbedEntity) {
		ent->isGrabbed = qfalse;
		Phys_HoldDropDynamic(player, velocity, qfalse);
		player->grabbedEntity = 0;
		G_AddEvent(player, EV_GRAVITYSOUND, 0);
	}
}

/*
==============
ClientThink
This will be called once for each client frame, which will
usually be a couple times for each server frame on fast clients.
==============
*/
static void ClientThink_real(gentity_t *ent) {
	gclient_t *client;
	gentity_t *vehicle;
	pmove_t pm;
	int oldEventSequence;
	int msec;
	usercmd_t *ucmd;

	client = ent->client;

	// don't think if the client is not yet connected (and thus not yet spawned in)
	if(client->pers.connected != CON_CONNECTED) {
		return;
	}
	// mark the time, so the connection sprite can be removed
	ucmd = &ent->client->pers.cmd;

	// sanity check the command time (anti-cheat)
	if(ucmd->serverTime > level.time + 200) ucmd->serverTime = level.time + 200;
	if(ucmd->serverTime < level.time - 1000) ucmd->serverTime = level.time - 1000;

	client->attackTime = ucmd->serverTime;

	client->lastUpdateFrame = level.framenum;

	msec = ucmd->serverTime - client->ps.commandTime;
	if(msec < 1 && client->sess.spectatorState != SPECTATOR_FOLLOW) return;
	if(msec > 200) msec = 200;

	//
	// check for exiting intermission
	//
	if(level.intermissiontime) {
		ClientIntermissionThink(client);
		return;
	}

	// spectators don't do much
	if(client->sess.sessionTeam == TEAM_SPECTATOR) {
		if(client->sess.spectatorState == SPECTATOR_SCOREBOARD) {
			return;
		}
		SpectatorThink(ent, ucmd);
		return;
	}

	if(client->noclip) {
		client->ps.pm_type = PM_NOCLIP;
	} else if(client->ps.stats[STAT_HEALTH] <= 0) {
		client->ps.pm_type = PM_DEAD;
	} else if(client->ps.pm_type != PM_FREEZE) {
		client->ps.pm_type = PM_NORMAL;
	}

	client->ps.gravity = cvarFloat("g_gravity") * gameInfoNPCTypes[ent->npcType].gravity;

	if(client->vehicleNum) {  // VEHICLE-SYSTEM: setup physics for all
		if(G_FindEntityForEntityNum(client->vehicleNum)) {
			vehicle = G_FindEntityForEntityNum(client->vehicleNum);
			client->ps.stats[STAT_VEHICLE] = vehicle->objectType;
			if(BG_InVehicle(vehicle->objectType)) {
				client->ps.speed = 900;
				client->ps.gravity *= 0.4;
			}
		}
	} else {
		client->ps.speed = cvarFloat("g_speed") * gameInfoNPCTypes[ent->npcType].speed;
		client->ps.stats[STAT_VEHICLE] = 0;
	}

	if(ent->client->noclip) client->ps.speed *= 2.50;

	if(gameInfoItems[client->ps.stats[STAT_PERSISTANT_POWERUP]].giTag == PW_SCOUT) client->ps.speed *= 1.5;
	if(client->ps.powerups[PW_HASTE]) client->ps.speed *= 1.3;

	// Let go of the hook if we aren't firing
	if(client->ps.weapon == WP_GRAPPLING_HOOK && client->hook && !(ucmd->buttons & BUTTON_ATTACK))
		Weapon_HookFree(client->hook);

	// set up for pmove
	oldEventSequence = client->ps.eventSequence;

	memset(&pm, 0, sizeof(pm));

	// check for the hit-scan gauntlet, don't let the action
	// go through as an attack unless it actually hits something
	if(gameInfoWeapons[client->ps.weapon].wType == WT_MELEE && !(ucmd->buttons & BUTTON_TALK) && (ucmd->buttons & BUTTON_ATTACK) && client->ps.weaponTime <= 0 && client->ps.pm_type != PM_DEAD)
		pm.gauntletHit = Melee_Fire(ent, client->ps.weapon);

	// check for invulnerability expansion before doing the Pmove
	if(client->ps.powerups[PW_INVULNERABILITY]) {
		if(!(client->ps.pm_flags & PMF_INVULEXPAND)) {
			vec3_t mins = {-42, -42, -42};
			vec3_t maxs = {42, 42, 42};
			vec3_t oldmins, oldmaxs;

			VectorCopy(ent->r.mins, oldmins);
			VectorCopy(ent->r.maxs, oldmaxs);
			// expand
			VectorCopy(mins, ent->r.mins);
			VectorCopy(maxs, ent->r.maxs);
			trap_LinkEntity(ent);
			// check if this would get anyone stuck in this player
			if(!StuckInOtherClient(ent)) {
				// set flag so the expanded size will be set in PM_CheckDuck
				client->ps.pm_flags |= PMF_INVULEXPAND;
			}
			// set back
			VectorCopy(oldmins, ent->r.mins);
			VectorCopy(oldmaxs, ent->r.maxs);
			trap_LinkEntity(ent);
		}
	}

	pm.ps = &client->ps;
	pm.cmd = *ucmd;
	if(pm.ps->pm_type == PM_DEAD) {
		pm.tracemask = MASK_PLAYERSOLID & ~CONTENTS_BODY;
	} else if(ent->r.svFlags & SVF_BOT) {
		pm.tracemask = MASK_PLAYERSOLID | CONTENTS_BOTCLIP;
	} else {
		pm.tracemask = MASK_PLAYERSOLID;
	}
	pm.trace = trap_Trace;
	pm.pointcontents = trap_PointContents;

	VectorCopy(client->ps.origin, client->oldOrigin);
	Pmove(&pm);

	// save results of pmove
	if(ent->client->ps.eventSequence != oldEventSequence) {
		ent->eventTime = level.time;
	}
	BG_PlayerStateToEntityState(&ent->client->ps, &ent->s, qtrue);
	SendPendingPredictableEvents(&ent->client->ps);

	if(!(ent->client->ps.eFlags & EF_FIRING)) {
		client->fireHeld = qfalse;  // for grapple
	}

	// use the snapped origin for linking so it matches client predicted versions
	VectorCopy(ent->s.pos.trBase, ent->r.currentOrigin);

	VectorCopy(pm.mins, ent->r.mins);
	VectorCopy(pm.maxs, ent->r.maxs);

	ent->waterlevel = pm.waterlevel;
	ent->watertype = pm.watertype;
	ent->client->ps.weapon = ent->swep_id;
	ent->client->ps.stats[STAT_AMMO] = ent->swep_ammo[ent->swep_id];
	ent->s.weapon = ent->swep_ammo[ent->swep_id];

	// execute client events
	ClientEvents(ent, oldEventSequence);

	// link entity now, after any personal teleporters have been used
	trap_LinkEntity(ent);
	if(!ent->client->noclip) {
		G_TouchTriggers(ent);
		G_KillVoid(ent);
	}

	// NOTE: now copy the exact origin over otherwise clients can be snapped into solid
	VectorCopy(ent->client->ps.origin, ent->r.currentOrigin);

	// test for solid areas in the AAS file
	BotTestAAS(ent->r.currentOrigin);

	// touch other objects
	ClientImpacts(ent, &pm);

	// save results of triggers and client events
	if(ent->client->ps.eventSequence != oldEventSequence)
		ent->eventTime = level.time;

	// swap and latch button actions
	client->oldbuttons = client->buttons;
	client->buttons = ucmd->buttons;
	client->latched_buttons |= client->buttons & ~client->oldbuttons;

	PhysgunHold(ent);
	GravitygunHold(ent);

	Phys_CheckCarCollisions(ent);

	// check for respawning
	if(client->ps.stats[STAT_HEALTH] <= 0) {
		if(level.time > client->respawnTime && ucmd->buttons & (BUTTON_ATTACK | BUTTON_USE_HOLDABLE))
			ClientRespawn(ent);
		return;
	}

	// perform once-a-second actions
	ClientTimerActions(ent, msec);

	SendEntityInfoToClient(ent, msec);
}

/*
==================
ClientThink
A new command has arrived from the client
==================
*/
void ClientThink(int clientNum) {
	gentity_t *ent;

	ent = g_entities + clientNum;
	trap_GetUsercmd(clientNum, &ent->client->pers.cmd);

	if(!(ent->r.svFlags & SVF_BOT)) {
		ClientThink_real(ent);
	}
}

void G_RunClient(gentity_t *ent) {
	if(!(ent->r.svFlags & SVF_BOT)) {
		return;
	}
	ent->client->pers.cmd.serverTime = level.time;
	ClientThink_real(ent);
}

qboolean G_CheckWeapon(int clientNum, int wp, int finish) {
	gentity_t *ent;

	ent = g_entities + clientNum;
	if(ent->swep_list[wp] >= WS_HAVE) {
		if(finish) ent->swep_id = wp;
		ClientUserinfoChanged(clientNum);
		return qtrue;
	} else {
		return qfalse;
	}
}

int G_CheckWeaponAmmo(int clientNum, int wp) {
	gentity_t *ent;

	ent = g_entities + clientNum;
	return ent->swep_ammo[wp];
}

void PM_Add_SwepAmmo(int clientNum, int wp, int count) {
	gentity_t *ent;

	ent = g_entities + clientNum;
	if(!(ent->swep_ammo[wp] == -1)) {
		if(!(ent->swep_ammo[wp] >= 9999)) {
			ent->swep_ammo[wp] += count;
		}
	}
}

static void SpectatorClientEndFrame(gentity_t *ent) {
	gclient_t *cl;

	// if we are doing a chase cam or a remote view, grab the latest info
	if(ent->client->sess.spectatorState == SPECTATOR_FOLLOW) {
		int clientNum, flags;

		clientNum = ent->client->sess.spectatorClient;

		// team follow1 and team follow2 go to whatever clients are playing
		if(clientNum == -1) {
			clientNum = level.follow1;
		} else if(clientNum == -2) {
			clientNum = level.follow2;
		}
		if(clientNum >= 0) {
			cl = &level.clients[clientNum];
			if(cl->pers.connected == CON_CONNECTED && cl->sess.sessionTeam != TEAM_SPECTATOR) {
				ent->client->ps = cl->ps;
				ent->client->ps.pm_flags |= PMF_FOLLOW;
				ent->client->ps.eFlags = flags;
				return;
			} else {
				// drop them to free spectators unless they are dedicated camera followers
				if(ent->client->sess.spectatorClient >= 0) {
					ent->client->sess.spectatorState = SPECTATOR_FREE;
					ClientBegin(ent->client - level.clients);
				}
			}
		}
	}

	if(ent->client->sess.spectatorState == SPECTATOR_SCOREBOARD) {
		ent->client->ps.pm_flags |= PMF_SCOREBOARD;
	} else {
		ent->client->ps.pm_flags &= ~PMF_SCOREBOARD;
	}
}

/*
==============
ClientEndFrame
Called at the end of each server frame for each connected client
A fast client will have multiple ClientThink for each ClientEdFrame,
while a slow client may have multiple ClientEndFrame between ClientThink.
==============
*/
void ClientEndFrame(gentity_t *ent) {
	int i;
	clientPersistant_t *pers;

	if(ent->client->sess.sessionTeam == TEAM_SPECTATOR) {
		SpectatorClientEndFrame(ent);
		return;
	}

	pers = &ent->client->pers;

	// turn off any expired powerups
	for(i = 0; i < MAX_POWERUPS; i++) {
		if(ent->client->ps.powerups[i] < level.time) {
			ent->client->ps.powerups[i] = 0;
		}
	}

	// set powerup for player animation
	if(gameInfoItems[ent->client->ps.stats[STAT_PERSISTANT_POWERUP]].giTag == PW_GUARD) {
		ent->client->ps.powerups[PW_GUARD] = level.time;
	}
	if(gameInfoItems[ent->client->ps.stats[STAT_PERSISTANT_POWERUP]].giTag == PW_SCOUT) {
		ent->client->ps.powerups[PW_SCOUT] = level.time;
	}
	if(gameInfoItems[ent->client->ps.stats[STAT_PERSISTANT_POWERUP]].giTag == PW_DOUBLER) {
		ent->client->ps.powerups[PW_DOUBLER] = level.time;
	}
	if(gameInfoItems[ent->client->ps.stats[STAT_PERSISTANT_POWERUP]].giTag == PW_AMMOREGEN) {
		ent->client->ps.powerups[PW_AMMOREGEN] = level.time;
	}
	if(ent->client->invulnerabilityTime > level.time) {
		ent->client->ps.powerups[PW_INVULNERABILITY] = level.time;
	}

	//
	// If the end of unit layout is displayed, don't give
	// the player any normal movement attributes
	//
	if(level.intermissiontime) {
		return;
	}

	// burn from lava, etc
	P_WorldEffects(ent);

	// apply all the damage taken this frame
	P_DamageFeedback(ent);

	ent->client->ps.stats[STAT_HEALTH] = ent->health;  // FIXME: get rid of ent->health...

	G_SetClientSound(ent);

	BG_PlayerStateToEntityState(&ent->client->ps, &ent->s, qtrue);
	SendPendingPredictableEvents(&ent->client->ps);
}
