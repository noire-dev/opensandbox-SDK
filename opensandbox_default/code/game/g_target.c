// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

#include "g_local.h"

static void Use_Target_Give(gentity_t *ent, gentity_t *other, gentity_t *activator) {
	gentity_t *t;
	trace_t trace;

	if(!activator->client || !ent->target) return;

	memset(&trace, 0, sizeof(trace));
	t = NULL;
	while((t = G_Find(t, FOFS(targetname), ent->target)) != NULL) {
		if(!t->item) continue;

		Touch_Item(t, activator, &trace);

		// make sure it isn't going to respawn or show any events
		t->nextthink = 0;
		trap_UnlinkEntity(t);
	}
}

void SP_target_give(gentity_t *ent) { ent->use = Use_Target_Give; }

static void Think_Target_Delay(gentity_t *ent) { G_UseTargets(ent, ent->activator); }

static void Use_Target_Delay(gentity_t *ent, gentity_t *other, gentity_t *activator) {
	ent->nextthink = level.time + (ent->wait + ent->random * crandom()) * 1000;
	ent->think = Think_Target_Delay;
	ent->activator = activator;
}

void SP_target_delay(gentity_t *ent) {
	// check delay for backwards compatability
	if(!G_SpawnFloat("delay", "0", &ent->wait)) {
		G_SpawnFloat("wait", "1", &ent->wait);
	}

	if(!ent->wait) ent->wait = 1;
	ent->use = Use_Target_Delay;
}

static void Use_Target_Print(gentity_t *ent, gentity_t *other, gentity_t *activator) {
	if(activator->client && (ent->spawnflags & 4)) {
		trap_SendServerCommand(activator - g_entities, va("cp \"%s\"", ent->message));
		return;
	}

	if(ent->spawnflags & 3) {
		if(ent->spawnflags & 1) {
			G_TeamCommand(TEAM_RED, va("cp \"%s\"", ent->message));
		}
		if(ent->spawnflags & 2) {
			G_TeamCommand(TEAM_BLUE, va("cp \"%s\"", ent->message));
		}
		return;
	}

	trap_SendServerCommand(-1, va("cp \"%s\"", ent->message));
}

void SP_target_print(gentity_t *ent) { ent->use = Use_Target_Print; }

static void Use_Target_Speaker(gentity_t *ent, gentity_t *other, gentity_t *activator) {
	if(ent->spawnflags & 3) {  // looping sound toggles
		if(ent->s.loopSound)
			ent->s.loopSound = 0;  // turn it off
		else
			ent->s.loopSound = ent->noise_index;  // start it
	} else {                                      // normal sound
		if(ent->spawnflags & 8) {
			G_AddEvent(activator, EV_GENERAL_SOUND, ent->noise_index);
		} else if(ent->spawnflags & 4) {
			G_AddEvent(ent, EV_GLOBAL_SOUND, ent->noise_index);
		} else {
			G_AddEvent(ent, EV_GENERAL_SOUND, ent->noise_index);
		}
	}
}

void SP_target_speaker(gentity_t *ent) {
	char buffer[MAX_QPATH];
	char *s;

	G_SpawnFloat("wait", "0", &ent->wait);
	G_SpawnFloat("random", "0", &ent->random);

	if(!G_SpawnString("noise", "NOSOUND", &s)) {
		G_Error("target_speaker without a noise key at %s", vtos(ent->s.origin));
	}

	// force all client reletive sounds to be "activator" speakers that
	// play on the entity that activates it
	if(s[0] == '*') ent->spawnflags |= 8;

	if(!strstr(s, ".wav")) {
		Com_sprintf(buffer, sizeof(buffer), "%s.wav", s);
	} else {
		Q_strncpyz(buffer, s, sizeof(buffer));
	}
	ent->noise_index = G_SoundIndex(buffer);

	// a repeating speaker can be done completely client side
	ent->s.eType = ET_SPEAKER;
	ent->s.eventParm = ent->noise_index;
	ent->s.frame = ent->wait * 10;
	ent->s.clientNum = ent->random * 10;

	// check for prestarted looping sound
	if(ent->spawnflags & 1) ent->s.loopSound = ent->noise_index;

	ent->use = Use_Target_Speaker;

	if(ent->spawnflags & 4) ent->r.svFlags |= SVF_BROADCAST;

	VectorCopy(ent->s.origin, ent->s.pos.trBase);

	// must link the entity so we get areas and clusters so
	// the server can determine who to send updates to
	trap_LinkEntity(ent);
}

static void target_teleporter_use(gentity_t *self, gentity_t *other, gentity_t *activator) {
	gentity_t *dest;

	if(!activator->client) return;
	dest = G_PickTarget(self->target);
	if(!dest) {
		G_Printf("Couldn't find teleporter destination\n");
		return;
	}

	TeleportPlayer(activator, dest->s.origin, dest->s.angles, qfalse);
}

void SP_target_teleporter(gentity_t *self) {
	if(!self->targetname) G_Printf("untargeted %s at %s\n", self->classname, vtos(self->s.origin));

	self->use = target_teleporter_use;
}

static void target_relay_use(gentity_t *self, gentity_t *other, gentity_t *activator) {
	if((self->spawnflags & 1) && activator->client && activator->client->sess.sessionTeam != TEAM_RED) {
		return;
	}
	if((self->spawnflags & 2) && activator->client && activator->client->sess.sessionTeam != TEAM_BLUE) {
		return;
	}
	if(self->spawnflags & 4) {
		gentity_t *ent;

		ent = G_PickTarget(self->target);
		if(ent && ent->use) {
			ent->use(ent, self, activator);
		}
		return;
	}
	G_UseTargets(self, activator);
}

void SP_target_relay(gentity_t *self) { self->use = target_relay_use; }

static void target_kill_use(gentity_t *self, gentity_t *other, gentity_t *activator) { G_Damage(activator, NULL, NULL, NULL, NULL, 100000, DAMAGE_NO_PROTECTION, MOD_TELEFRAG); }

void SP_target_kill(gentity_t *self) { self->use = target_kill_use; }

void SP_target_position(gentity_t *self) { G_SetOrigin(self, self->s.origin); }

static void target_location_linkup(gentity_t *ent) {
	int i;
	int n;

	if(level.locationLinked) return;

	level.locationLinked = qtrue;
	level.locationHead = NULL;
	trap_SetConfigstring(CS_LOCATIONS, "unknown");

	for(i = 0, ent = g_entities, n = 1; i < level.num_entities; i++, ent++) {
		if(ent->classname && !Q_stricmp(ent->classname, "target_location")) {
			// lets overload some variables!
			if(CS_LOCATIONS + n >= MAX_CONFIGSTRINGS) return;
			ent->health = n;  // use for location marking
			trap_SetConfigstring(CS_LOCATIONS + n, ent->message);
			n++;
			ent->nextTrain = level.locationHead;
			level.locationHead = ent;
		}
	}
}

void SP_target_location(gentity_t *self) {
	self->think = target_location_linkup;
	self->nextthink = level.time + 200;  // Let them all spawn first

	G_SetOrigin(self, self->s.origin);
}

static void script_variable_use(gentity_t *self, gentity_t *other, gentity_t *activator) {
	char *value;

	if(self->spawnflags & 1 || self->spawnflags & 2) {
		value = cvarString(self->key);

		if((self->spawnflags & 1) && !strcmp(value, self->value)) {
			G_UseTargets(self, activator);
		}

		if((self->spawnflags & 2) && strcmp(value, self->value)) {
			G_UseTargets(self, activator);
		}

		if((self->spawnflags & 4) && (atoi(value) <= atoi(self->value))) {
			G_UseTargets(self, activator);
		}

		if((self->spawnflags & 8) && (atoi(value) >= atoi(self->value))) {
			G_UseTargets(self, activator);
		}

		return;
	}
	if(self->spawnflags & 8192) {
		trap_Cmd(EXEC_APPEND, va("%s - %s\n", self->key, self->value));
	} else {
		cvarSet(self->key, self->value);
	}
}

static void script_variable_think(gentity_t *self) {
	self->nextthink = 0;
	script_variable_use(self, NULL, self);
}

void SP_script_variable(gentity_t *self) {
	if(!self->key) {
		G_Printf("WARNING: script_variable without key at %s\n", vtos(self->s.origin));
		G_FreeEntity(self);
		return;
	}

	if(!self->key) {
		G_Printf("WARNING: script_variable without value at %s\n", vtos(self->s.origin));
		G_FreeEntity(self);
		return;
	}

	self->use = script_variable_use;

	if((self->spawnflags & 4096)) {
		self->nextthink = level.time + FRAMETIME * 3;  // trigger entities next frame so they can spawn first
		self->think = script_variable_think;
	}
}

static void use_script_cmd(gentity_t *ent, gentity_t *other, gentity_t *activator) {
	if(ent->spawnflags & 1) {
		trap_Cmd(EXEC_APPEND, va("%s\n", ent->target));
	}
	if(ent->spawnflags & 2) {
		trap_SendServerCommand(activator - g_entities, va("clcmd \"%s\"", ent->target));
	}
	if(ent->spawnflags & 4) {
		trap_SendServerCommand(-1, va("clcmd \"%s\"", ent->target));
	}
}

static void script_cmd_think(gentity_t *ent) {
	ent->nextthink = 0;
	use_script_cmd(ent, NULL, ent);
}

void SP_script_cmd(gentity_t *ent) {
	if(!ent->target) {
		G_Printf("No target in script_cmd\n");
	}
	if(ent->spawnflags & 4096) {
		ent->nextthink = level.time + FRAMETIME * 10;  // trigger entities next frame so they can spawn first
		ent->think = script_cmd_think;
	}
	ent->use = use_script_cmd;
}
