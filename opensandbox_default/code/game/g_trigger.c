// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

#include "g_local.h"

static void InitTrigger(gentity_t *self) {
	if(!VectorCompare(self->s.angles, vec3_origin)) G_SetMovedir(self->s.angles, self->movedir);

	trap_SetBrushModel(self, self->model);
	self->r.contents = CONTENTS_TRIGGER;  // replaces the -1 from trap_SetBrushModel
	self->r.svFlags = SVF_NOCLIENT;
}

// the wait time has passed, so set back up for another activation
static void multi_wait(gentity_t *ent) { ent->nextthink = 0; }

// the trigger was just activated
// ent->activator should be set to the activator so it can be held through a delay
// so wait for the delay time before firing
static void multi_trigger(gentity_t *ent, gentity_t *activator) {
	ent->activator = activator;
	if(ent->nextthink) {
		return;  // can't retrigger until the wait is over
	}

	if(activator->client) {
		if((ent->spawnflags & 1) && activator->client->sess.sessionTeam != TEAM_RED) {
			return;
		}
		if((ent->spawnflags & 2) && activator->client->sess.sessionTeam != TEAM_BLUE) {
			return;
		}
	}

	G_UseTargets(ent, ent->activator);

	if(ent->wait > 0) {
		ent->think = multi_wait;
		ent->nextthink = level.time + (ent->wait + ent->random * crandom()) * 1000;
	} else {
		// we can't just remove (self) here, because this is a touch function
		// called while looping through area links...
		ent->touch = 0;
		ent->nextthink = level.time + FRAMETIME;
		ent->think = G_FreeEntity;
	}
}

static void Use_Multi(gentity_t *ent, gentity_t *other, gentity_t *activator) { multi_trigger(ent, activator); }

static void Touch_Multi(gentity_t *self, gentity_t *other, trace_t *trace) {
	if(!other->client) {
		return;
	}
	multi_trigger(self, other);
}

void SP_trigger_multiple(gentity_t *ent) {
	G_SpawnFloat("wait", "0.5", &ent->wait);
	G_SpawnFloat("random", "0", &ent->random);

	if(ent->random >= ent->wait && ent->wait >= 0) {
		ent->random = ent->wait - FRAMETIME;
		G_Printf("trigger_multiple has random >= wait\n");
	}

	ent->touch = Touch_Multi;
	ent->use = Use_Multi;

	InitTrigger(ent);
	trap_LinkEntity(ent);
}

/*
================
trigger_always
================
*/
static void trigger_always_think(gentity_t *ent) {
	G_UseTargets(ent, ent);
	G_FreeEntity(ent);
}

void SP_trigger_always(gentity_t *ent) {
	// we must have some delay to make sure our use targets are present
	ent->nextthink = level.time + 300;
	ent->think = trigger_always_think;
}

/*
================
trigger_push
================
*/
static void trigger_push_touch(gentity_t *self, gentity_t *other, trace_t *trace) {
	if(!other->client) {
		return;
	}

	BG_TouchJumpPad(&other->client->ps, &self->s);
}

static void AimAtTarget(gentity_t *self) {
	gentity_t *ent;
	vec3_t origin;
	float height, gravity, time, forward;
	float dist;

	VectorAdd(self->r.absmin, self->r.absmax, origin);
	VectorScale(origin, 0.5, origin);

	ent = G_PickTarget(self->target);
	if(!ent) {
		G_FreeEntity(self);
		return;
	}

	height = ent->s.origin[2] - origin[2];
	gravity = g_gravity.value;
	time = sqrt(height / (.5 * gravity));
	if(!time) {
		G_FreeEntity(self);
		return;
	}

	// set s.origin2 to the push velocity
	VectorSubtract(ent->s.origin, origin, self->s.origin2);
	self->s.origin2[2] = 0;
	dist = VectorNormalize(self->s.origin2);

	forward = dist / time;
	VectorScale(self->s.origin2, forward, self->s.origin2);

	self->s.origin2[2] = time * gravity;
}

void SP_trigger_push(gentity_t *self) {
	InitTrigger(self);

	// unlike other triggers, we need to send this one to the client
	self->r.svFlags &= ~SVF_NOCLIENT;

	// make sure the client precaches this sound
	G_SoundIndex("sound/world/jumppad.wav");

	self->s.eType = ET_PUSH_TRIGGER;
	self->touch = trigger_push_touch;
	self->think = AimAtTarget;
	self->nextthink = level.time + FRAMETIME;
	trap_LinkEntity(self);
}

static void Use_target_push(gentity_t *self, gentity_t *other, gentity_t *activator) {
	if(!activator->client) {
		return;
	}

	if(activator->client->ps.pm_type != PM_NORMAL) {
		return;
	}
	if(activator->client->ps.powerups[PW_FLIGHT]) {
		return;
	}

	VectorCopy(self->s.origin2, activator->client->ps.velocity);

	// play fly sound every 1.5 seconds
	if(activator->fly_sound_debounce_time < level.time) {
		activator->fly_sound_debounce_time = level.time + 1500;
		G_Sound(activator, CHAN_AUTO, self->noise_index);
	}
}

void SP_target_push(gentity_t *self) {
	if(!self->speed) {
		self->speed = 1000;
	}
	G_SetMovedir(self->s.angles, self->s.origin2);
	VectorScale(self->s.origin2, self->speed, self->s.origin2);

	if(self->spawnflags & 1) {
		self->noise_index = G_SoundIndex("sound/world/jumppad.wav");
	} else {
		self->noise_index = G_SoundIndex("sound/misc/windfly.wav");
	}
	if(self->target) {
		VectorCopy(self->s.origin, self->r.absmin);
		VectorCopy(self->s.origin, self->r.absmax);
		self->think = AimAtTarget;
		self->nextthink = level.time + FRAMETIME;
	}
	self->use = Use_target_push;
}

/*
================
trigger_teleport
================
*/
static void trigger_teleporter_touch(gentity_t *self, gentity_t *other, trace_t *trace) {
	gentity_t *dest;

	if(!other->client) {
		return;
	}
	if(other->client->ps.pm_type == PM_DEAD) {
		return;
	}
	// Spectators only?
	if((self->spawnflags & 1) && other->client->sess.sessionTeam != TEAM_SPECTATOR) {
		return;
	}

	dest = G_PickTarget(self->target);
	if(!dest) {
		G_Printf("Couldn't find teleporter destination\n");
		return;
	}

	TeleportPlayer(other, dest->s.origin, dest->s.angles, qfalse);
}

void SP_trigger_teleport(gentity_t *self) {
	InitTrigger(self);

	// unlike other triggers, we need to send this one to the client
	// unless is a spectator trigger
	if(self->spawnflags & 1) {
		self->r.svFlags |= SVF_NOCLIENT;
	} else {
		self->r.svFlags &= ~SVF_NOCLIENT;
	}

	// make sure the client precaches this sound
	G_SoundIndex("sound/world/jumppad.wav");

	self->s.eType = ET_TELEPORT_TRIGGER;
	self->touch = trigger_teleporter_touch;

	trap_LinkEntity(self);
}

/*
================
trigger_hurt
================
*/
static void hurt_use(gentity_t *self, gentity_t *other, gentity_t *activator) {
	if(self->r.linked) {
		trap_UnlinkEntity(self);
	} else {
		trap_LinkEntity(self);
	}
}

static void hurt_touch(gentity_t *self, gentity_t *other, trace_t *trace) {
	int dflags;

	if(!other->takedamage) {
		return;
	}

	if(self->timestamp > level.time) {
		return;
	}

	if(self->spawnflags & 16) {
		self->timestamp = level.time + 1000;
	} else {
		self->timestamp = level.time + FRAMETIME;
	}

	// play sound
	if(!(self->spawnflags & 4)) {
		G_Sound(other, CHAN_AUTO, self->noise_index);
	}

	if(self->spawnflags & 8)
		dflags = DAMAGE_NO_PROTECTION;
	else
		dflags = 0;
	G_Damage(other, self, self, NULL, NULL, self->damage, dflags, MOD_TRIGGER_HURT);
}

void SP_trigger_hurt(gentity_t *self) {
	InitTrigger(self);

	self->noise_index = G_SoundIndex("sound/world/electro.wav");
	self->touch = hurt_touch;

	if(!self->damage) {
		self->damage = 5;
	}

	self->r.contents = CONTENTS_TRIGGER;

	if(self->spawnflags & 2) {
		self->use = hurt_use;
	}

	// link in to the world if starting active
	if(!(self->spawnflags & 1)) {
		trap_LinkEntity(self);
	}
}
