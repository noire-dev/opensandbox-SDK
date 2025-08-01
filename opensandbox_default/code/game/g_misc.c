// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

#include "g_local.h"

void TeleportPlayer(gentity_t *player, vec3_t origin, vec3_t angles, qboolean noKnockback) {
    gentity_t *tent;

    // use temp events at source and destination to prevent the effect
    // from getting dropped by a second player event
    if(player->client->sess.sessionTeam != TEAM_SPECTATOR) {
        tent = G_TempEntity(player->client->ps.origin, EV_PLAYER_TELEPORT_OUT);
        tent->s.clientNum = player->s.clientNum;

        tent = G_TempEntity(origin, EV_PLAYER_TELEPORT_IN);
        tent->s.clientNum = player->s.clientNum;
    }

    // unlink to make sure it can't possibly interfere with G_KillBox
    trap_UnlinkEntity(player);

    VectorCopy(origin, player->client->ps.origin);
    player->client->ps.origin[2] += 1;

    // spit the player out
    if(!noKnockback) {
        AngleVectors(angles, player->client->ps.velocity, NULL, NULL);
        VectorScale(player->client->ps.velocity, 400, player->client->ps.velocity);
        player->client->ps.pm_time = 160;  // hold time
        player->client->ps.pm_flags |= PMF_TIME_KNOCKBACK;
    }

    // toggle the teleport bit so the client knows to not lerp
    player->client->ps.eFlags ^= EF_TELEPORT_BIT;

    // reset history for teleport
    G_ResetHistory(player);

    // set angles
    SetClientViewAngle(player, angles);

    // kill anything at the destination
    if(player->client->sess.sessionTeam != TEAM_SPECTATOR) {
        G_KillBox(player);
    }

    // save results of pmove
    BG_PlayerStateToEntityState(&player->client->ps, &player->s, qtrue);

    // use the precise origin for linking
    VectorCopy(player->client->ps.origin, player->r.currentOrigin);

    if(player->client->sess.sessionTeam != TEAM_SPECTATOR) {
        trap_LinkEntity(player);
    }
}

static void locateCamera(gentity_t *ent) {
    vec3_t dir;
    gentity_t *target;
    gentity_t *owner;

    owner = G_PickTarget(ent->target);
    if(!owner) {
        G_Printf("Couldn't find target for misc_partal_surface\n");
        G_FreeEntity(ent);
        return;
    }
    ent->r.ownerNum = owner->s.number;

    // frame holds the rotate speed
    if(owner->spawnflags & 1) {
        ent->s.frame = 25;
    } else if(owner->spawnflags & 2) {
        ent->s.frame = 75;
    }

    // swing camera ?
    if(owner->spawnflags & 4) {
        ent->s.powerups = 0;
    } else {
        ent->s.powerups = 1;
    }

    // clientNum holds the rotate offset
    ent->s.clientNum = owner->s.clientNum;

    VectorCopy(owner->s.origin, ent->s.origin2);

    // see if the portal_camera has a target
    target = G_PickTarget(owner->target);
    if(target) {
        VectorSubtract(target->s.origin, owner->s.origin, dir);
        VectorNormalize(dir);
    } else {
        G_SetMovedir(owner->s.angles, dir);
    }

    ent->s.eventParm = DirToByte(dir);
}

void SP_misc_portal_surface(gentity_t *ent) {
    VectorClear(ent->r.mins);
    VectorClear(ent->r.maxs);
    trap_LinkEntity(ent);

    ent->r.svFlags = SVF_PORTAL;
    ent->s.eType = ET_PORTAL;

    if(!ent->target) {
        VectorCopy(ent->s.origin, ent->s.origin2);
    } else {
        ent->think = locateCamera;
        ent->nextthink = level.time + 100;
    }
}

void SP_misc_portal_camera(gentity_t *ent) {
    float roll;

    VectorClear(ent->r.mins);
    VectorClear(ent->r.maxs);
    trap_LinkEntity(ent);

    G_SpawnFloat("roll", "0", &roll);

    ent->s.clientNum = roll / 360.0 * 256;
}

static void Use_Shooter(gentity_t *ent, gentity_t *other, gentity_t *activator) {
    vec3_t dir;
    float deg;
    vec3_t up, right;

    // see if we have a target
    if(ent->enemy) {
        VectorSubtract(ent->enemy->r.currentOrigin, ent->s.origin, dir);
        VectorNormalize(dir);
    } else {
        VectorCopy(ent->movedir, dir);
    }

    // randomize a bit
    PerpendicularVector(up, dir);
    CrossProduct(up, dir, right);

    deg = crandom() * ent->random;
    VectorMA(dir, deg, up, dir);

    deg = crandom() * ent->random;
    VectorMA(dir, deg, right, dir);

    VectorNormalize(dir);

    fire_missile(ent, ent->s.origin, dir, right, up, ent->s.weapon);

    G_AddEvent(ent, EV_FIRE_WEAPON, 0);
}

static void InitShooter_Finish(gentity_t *ent) {
    ent->enemy = G_PickTarget(ent->target);
    ent->think = 0;
    ent->nextthink = 0;
}

static void InitShooter(gentity_t *ent, int weapon) {
    ent->use = Use_Shooter;
    ent->s.weapon = weapon;

    G_SetMovedir(ent->s.angles, ent->movedir);

    if(!ent->random) {
        ent->random = 1.0;
    }
    ent->random = sin(M_PI * ent->random / 180);
    // target might be a moving object, so we can't set movedir for it
    if(ent->target) {
        ent->think = InitShooter_Finish;
        ent->nextthink = level.time + 500;
    }
    trap_LinkEntity(ent);
}

void SP_shooter_rocket(gentity_t *ent) { InitShooter(ent, WP_ROCKET_LAUNCHER); }

void SP_shooter_plasma(gentity_t *ent) { InitShooter(ent, WP_PLASMAGUN); }

void SP_shooter_grenade(gentity_t *ent) { InitShooter(ent, WP_GRENADE_LAUNCHER); }

static void PortalDie(gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod) { G_FreeEntity(self); }

void DropPortalDestination(gentity_t *player) {
    gentity_t *ent;
    vec3_t snapped;

    // create the portal destination
    ent = G_Spawn();
    ent->s.modelindex = G_ModelIndex("models/powerups/teleporter/tele_exit.md3");

    VectorCopy(player->s.pos.trBase, snapped);
    SnapVector(snapped);
    G_SetOrigin(ent, snapped);
    VectorCopy(player->r.mins, ent->r.mins);
    VectorCopy(player->r.maxs, ent->r.maxs);

    ent->classname = "hi_portal destination";
    ent->s.pos.trType = TR_STATIONARY;

    ent->r.contents = CONTENTS_CORPSE;
    ent->takedamage = qtrue;
    ent->health = 200;
    ent->die = PortalDie;

    VectorCopy(player->s.apos.trBase, ent->s.angles);

    ent->think = G_FreeEntity;
    ent->nextthink = level.time + 2 * 60 * 1000;

    trap_LinkEntity(ent);

    player->client->portalID = ++level.portalSequence;
    ent->count = player->client->portalID;

    // give the item back so they can drop the source now
    player->client->ps.stats[STAT_HOLDABLE_ITEM] = BG_FindItem("Portal") - gameInfoItems;
}

static void PortalTouch(gentity_t *self, gentity_t *other, trace_t *trace) {
    gentity_t *destination;

    // see if we will even let other try to use it
    if(other->health <= 0 || !other->client) {
        return;
    }

    // find the destination
    destination = NULL;
    while((destination = G_Find(destination, FOFS(classname), "hi_portal destination")) != NULL) {
        if(destination->count == self->count) {
            break;
        }
    }

    // if there is not one, die!
    if(!destination) {
        if(self->pos1[0] || self->pos1[1] || self->pos1[2]) {
            TeleportPlayer(other, self->pos1, self->s.angles, qfalse);
        }
        G_Damage(other, other, other, NULL, NULL, 100000, DAMAGE_NO_PROTECTION, MOD_TELEFRAG);
        return;
    }

    TeleportPlayer(other, destination->s.pos.trBase, destination->s.angles, qfalse);
}

static void PortalEnable(gentity_t *self) {
    self->touch = PortalTouch;
    self->think = G_FreeEntity;
    self->nextthink = level.time + 2 * 60 * 1000;
}

void DropPortalSource(gentity_t *player) {
    gentity_t *ent;
    gentity_t *destination;
    vec3_t snapped;

    // create the portal source
    ent = G_Spawn();
    ent->s.modelindex = G_ModelIndex("models/powerups/teleporter/tele_enter.md3");

    VectorCopy(player->s.pos.trBase, snapped);
    SnapVector(snapped);
    G_SetOrigin(ent, snapped);
    VectorCopy(player->r.mins, ent->r.mins);
    VectorCopy(player->r.maxs, ent->r.maxs);

    ent->classname = "hi_portal source";
    ent->s.pos.trType = TR_STATIONARY;

    ent->r.contents = CONTENTS_CORPSE | CONTENTS_TRIGGER;
    ent->takedamage = qtrue;
    ent->health = 200;
    ent->die = PortalDie;

    trap_LinkEntity(ent);

    ent->count = player->client->portalID;
    player->client->portalID = 0;

    ent->nextthink = level.time + 1000;
    ent->think = PortalEnable;

    // find the destination
    destination = NULL;
    while((destination = G_Find(destination, FOFS(classname), "hi_portal destination")) != NULL) {
        if(destination->count == ent->count) {
            VectorCopy(destination->s.pos.trBase, ent->pos1);
            break;
        }
    }
}
