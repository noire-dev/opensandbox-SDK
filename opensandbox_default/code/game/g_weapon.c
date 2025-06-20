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
//
// g_weapon.c
// perform the server side effects of a weapon firing

#include "g_local.h"

static	float	s_quadFactor;
static	vec3_t	forward, right, up;
static	vec3_t	muzzle;

#define NUM_NAILSHOTS 16

/*
================
G_BounceProjectile
================
*/
void G_BounceProjectile( vec3_t start, vec3_t impact, vec3_t dir, vec3_t endout ) {
	vec3_t v, newv;
	float dot;

	VectorSubtract( impact, start, v );
	dot = DotProduct( v, dir );
	VectorMA( v, -2*dot, dir, newv );

	VectorNormalize(newv);
	VectorMA(impact, 8192, newv, endout);
}


/*
============
Laser Sight Stuff

Flash Light Functions
============
*/
void Laser_Gen( gentity_t *ent )	{

	gentity_t	*las;

	las = G_Spawn();

	las->nextthink = level.time + 10;
	las->think = Laser_Think;
	las->r.ownerNum = ent->s.number;
	las->parent = ent;
	las->s.eType = ET_LASER;

	las->s.eventParm = 1; //for distance
	las->classname = "flashlight";

	ent->client->lasersight = las;
}

void Laser_Think(gentity_t *self) {
    vec3_t end, start, forward, right, up, add;
    trace_t tr;
    float distance, scaled_distance;

    // If Player Dies, You Die -> now thanks to Camouflage!
    if (self->parent->client->ps.pm_type == PM_DEAD) {
        G_FreeEntity(self);
        return;
    }

    // Set Aiming Directions
    if (!self->parent->client->vehiclenum) {
        AngleVectors(self->parent->client->ps.viewangles, forward, right, up);
        CalcMuzzlePoint(self->parent, forward, right, up, start);
        VectorMA(start, 1024, forward, end);
    } else {
        add[0] = 0;
        add[1] = self->parent->s.apos.trBase[1];
        add[2] = 0;
        AngleVectors(add, forward, right, up);
        CalcMuzzlePoint(self->parent, forward, right, up, start);
        VectorMA(start, 320, forward, end);	
    }

    // Trace Position
    trap_Trace(&tr, start, NULL, NULL, end, self->parent->s.number, MASK_SHOT);

    // Calculate distance to the first collision point
    distance = VectorDistance(start, tr.endpos);

    // Scale distance to the range of 0 to 255
    scaled_distance = 255.0f * (1.0f - (distance / 1024.0f));
    if (scaled_distance > 255.0f) {
        scaled_distance = 255.0f;  // Clamp to 255 if exceeding
    }

    // Did you not hit anything?
    if (tr.surfaceFlags & SURF_NOIMPACT || tr.surfaceFlags & SURF_SKY) {
        self->nextthink = level.time + 8;
        trap_UnlinkEntity(self);
        return;
    }

    // Move you forward to keep you visible
    VectorMA(tr.endpos, -16, forward, tr.endpos);

    // Set scaled distance in eventParm
    self->s.eventParm = (int)scaled_distance;

    // Set your position
    VectorCopy(tr.endpos, self->r.currentOrigin);
    VectorCopy(tr.endpos, self->s.pos.trBase);

    vectoangles(tr.plane.normal, self->s.angles);

    trap_LinkEntity(self);

    // Prep next move
    self->nextthink = level.time + 8;
}

/*
======================================================================

GAUNTLET

======================================================================
*/

void Weapon_Gauntlet( gentity_t *ent ) {

}

/*
===============
CheckGauntletAttack
===============
*/
qboolean CheckGauntletAttack( gentity_t *ent ) {
	trace_t		tr;
	vec3_t		end;
	gentity_t	*tent;
	gentity_t	*traceEnt;
	int			damage;

	// set aiming directions
	AngleVectors (ent->client->ps.viewangles, forward, right, up);

	CalcMuzzlePoint ( ent, forward, right, up, muzzle );
	VectorMA (muzzle, 64, forward, end);
	trap_Trace (&tr, muzzle, NULL, NULL, end, ent->s.number, MASK_SHOT);

	if ( tr.surfaceFlags & SURF_NOIMPACT ) {
		return qfalse;
	}

	traceEnt = &g_entities[ tr.entityNum ];

	// send blood impact
	if ( traceEnt->takedamage && traceEnt->client ) {
		tent = G_TempEntity( tr.endpos, EV_MISSILE_MISS );
		tent->s.otherEntityNum = traceEnt->s.number;
		tent->s.eventParm = DirToByte( tr.plane.normal );
		tent->s.weapon = ent->s.weapon;
	}
	
	if(!ent->client->vehiclenum){
		if ( !traceEnt->takedamage) {
			return qfalse;
		}
	}

	if (ent->client->ps.powerups[PW_QUAD] ) {
		s_quadFactor = 3;
	} else {
		s_quadFactor = 1;
	}
	if( ent->client->persistantPowerup && ent->client->persistantPowerup->item && ent->client->persistantPowerup->item->giTag == PW_DOUBLER ) {
		s_quadFactor *= 2.0;
	}
	if(ent->skill == 9){
		s_quadFactor *= 5;
	}

	damage = 50 * s_quadFactor;
	G_Damage( traceEnt, ent, ent, forward, tr.endpos,
		damage, 0, MOD_GAUNTLET );

	return qtrue;
}


/*
======================================================================

MACHINEGUN

======================================================================
*/

//unlagged - attack prediction #3
// moved from g_weapon.c
/*
======================
SnapVectorTowards

Round a vector to integers for more efficient network
transmission, but make sure that it rounds towards a given point
rather than blindly truncating.  This prevents it from truncating
into a wall.
======================
*/
void SnapVectorTowards( vec3_t v, vec3_t to ) {
	int		i;

	for ( i = 0 ; i < 3 ; i++ ) {
		if ( to[i] <= v[i] ) {
			v[i] = (int)v[i];
		} else {
			v[i] = (int)v[i] + 1;
		}
	}
}
//unlagged - attack prediction #3

#define CHAINGUN_SPREAD		600
#define MACHINEGUN_SPREAD	200
#define	MACHINEGUN_DAMAGE	7

void Bullet_Fire (gentity_t *ent, float spread, int damage ) {
	trace_t		tr;
	vec3_t		end;
	vec3_t		impactpoint, bouncedir;
	float		r;
	float		u;
	gentity_t	*tent;
	gentity_t	*traceEnt;
	int			i, passent;
	int			seed = ent->client->attackTime % 256;

	damage *= s_quadFactor;

	r = Q_random(&seed) * M_PI * 2.0f;
	u = sin(r) * Q_crandom(&seed) * spread * 16;
	r = cos(r) * Q_crandom(&seed) * spread * 16;

	VectorMA (muzzle, 8192*16, forward, end);
	VectorMA (end, r, right, end);
	VectorMA (end, u, up, end);

	passent = ent->s.number;
	for (i = 0; i < 10; i++) {
		G_DoTimeShiftFor( ent );
		trap_Trace (&tr, muzzle, NULL, NULL, end, passent, MASK_SHOT);
		G_UndoTimeShiftFor( ent );

		if ( tr.surfaceFlags & SURF_NOIMPACT ) {
			return;
		}

		traceEnt = &g_entities[ tr.entityNum ];

		// snap the endpos to integers, but nudged towards the line
		SnapVectorTowards( tr.endpos, muzzle );

		// send bullet impact
		if ( traceEnt->takedamage && traceEnt->client ) {
			tent = G_TempEntity( tr.endpos, EV_BULLET_HIT_FLESH );
			tent->s.eventParm = traceEnt->s.number;
			tent->s.clientNum = ent->s.clientNum;
		} else {
			tent = G_TempEntity( tr.endpos, EV_BULLET_HIT_WALL );
			tent->s.eventParm = DirToByte( tr.plane.normal );
			tent->s.clientNum = ent->s.clientNum;
		}
		tent->s.otherEntityNum = ent->s.number;

		if ( traceEnt->takedamage) {
			if ( traceEnt->client && traceEnt->client->invulnerabilityTime > level.time ) {
				if (G_InvulnerabilityEffect( traceEnt, forward, tr.endpos, impactpoint, bouncedir )) {
					G_BounceProjectile( muzzle, impactpoint, bouncedir, end );
					VectorCopy( impactpoint, muzzle );
					// the player can hit him/herself with the bounced rail
					passent = ENTITYNUM_NONE;
				} else {
					VectorCopy( tr.endpos, muzzle );
					passent = traceEnt->s.number;
				}
				continue;
			} else {
    	        if(spread == CHAINGUN_SPREAD)  {
    	            G_Damage( traceEnt, ent, ent, forward, tr.endpos, damage, 0, MOD_CHAINGUN);
    	        } else {
					G_Damage( traceEnt, ent, ent, forward, tr.endpos, damage, 0, MOD_MACHINEGUN);
    	        }
			}
		}
		break;
	}
}


/*
======================================================================

BFG

======================================================================
*/

void BFG_Fire ( gentity_t *ent ) {
	gentity_t	*m;

	m = fire_bfg (ent, muzzle, forward);
	m->damage *= s_quadFactor;
	m->splashDamage *= s_quadFactor;

//	VectorAdd( m->s.pos.trDelta, ent->client->ps.velocity, m->s.pos.trDelta );	// "real" physics
}

/*
======================================================================

SHOTGUN

======================================================================
*/

// DEFAULT_SHOTGUN_SPREAD and DEFAULT_SHOTGUN_COUNT	are in bg_public.h, because
// client predicts same spreads
#define	DEFAULT_SHOTGUN_DAMAGE	10

void ShotgunPellet( vec3_t start, vec3_t end, gentity_t *ent ) {
	trace_t		tr;
	int			damage, i, passent;
	gentity_t	*traceEnt;
	gentity_t		*tent;
	vec3_t		impactpoint, bouncedir;
	vec3_t		tr_start, tr_end;

	passent = ent->s.number;
	VectorCopy( start, tr_start );
	VectorCopy( end, tr_end );
	for (i = 0; i < 10; i++) {
		trap_Trace (&tr, tr_start, NULL, NULL, tr_end, passent, MASK_SHOT);
		traceEnt = &g_entities[ tr.entityNum ];

		// send bullet impact
		if (  tr.surfaceFlags & SURF_NOIMPACT ) {
			return;
		}

		if ( traceEnt->takedamage) {
			damage = DEFAULT_SHOTGUN_DAMAGE * s_quadFactor;
			if ( traceEnt->client && traceEnt->client->invulnerabilityTime > level.time ) {
				if (G_InvulnerabilityEffect( traceEnt, forward, tr.endpos, impactpoint, bouncedir )) {
					G_BounceProjectile( tr_start, impactpoint, bouncedir, tr_end );
					VectorCopy( impactpoint, tr_start );
					// the player can hit him/herself with the bounced rail
					passent = ENTITYNUM_NONE;
				}
				else {
					VectorCopy( tr.endpos, tr_start );
					passent = traceEnt->s.number;
				}
				continue;
			}
			else {
				G_Damage( traceEnt, ent, ent, forward, tr.endpos, damage, 0, MOD_SHOTGUN);
			}
		}
	}
}

// this should match CG_ShotgunPattern
void ShotgunPattern( vec3_t origin, vec3_t origin2, int seed, gentity_t *ent ) {
	int			i;
	float		r, u;
	vec3_t		end;
	vec3_t		forward, right, up;
	int			oldScore;

	// derive the right and up vectors from the forward vector, because
	// the client won't have any other information
	VectorNormalize2( origin2, forward );
	PerpendicularVector( right, forward );
	CrossProduct( forward, right, up );

	oldScore = ent->client->ps.persistant[PERS_SCORE];

	// backward-reconcile the other clients
	G_DoTimeShiftFor( ent );

	// generate the "random" spread pattern
	for ( i = 0 ; i < DEFAULT_SHOTGUN_COUNT ; i++ ) {
		r = Q_crandom( &seed ) * DEFAULT_SHOTGUN_SPREAD * 16;
		u = Q_crandom( &seed ) * DEFAULT_SHOTGUN_SPREAD * 16;
		VectorMA( origin, 8192 * 16, forward, end);
		VectorMA (end, r, right, end);
		VectorMA (end, u, up, end);

		ShotgunPellet( origin, end, ent );
	}

	// put them back
	G_UndoTimeShiftFor( ent );
}


void weapon_supershotgun_fire (gentity_t *ent) {
	gentity_t		*tent;

	// send shotgun blast
	tent = G_TempEntity( muzzle, EV_SHOTGUN );
	VectorScale( forward, 4096, tent->s.origin2 );
	SnapVector( tent->s.origin2 );
//Sago: This sound like a bad idea...
//unlagged - attack prediction #2
	// this has to be something the client can predict now
	//tent->s.eventParm = rand() & 255;		// seed for spread pattern
	tent->s.eventParm = ent->client->attackTime % 256; // seed for spread pattern
//unlagged - attack prediction #2
	tent->s.otherEntityNum = ent->s.number;

	ShotgunPattern( tent->s.pos.trBase, tent->s.origin2, tent->s.eventParm, ent );
}


/*
======================================================================

GRENADE LAUNCHER

======================================================================
*/

void weapon_grenadelauncher_fire (gentity_t *ent) {
	gentity_t	*m;

	// extra vertical velocity
	forward[2] += 0.2f;
	VectorNormalize( forward );

	m = fire_grenade (ent, muzzle, forward);
	m->damage *= s_quadFactor;
	m->splashDamage *= s_quadFactor;

//	VectorAdd( m->s.pos.trDelta, ent->client->ps.velocity, m->s.pos.trDelta );	// "real" physics
}

/*
======================================================================

ROCKET

======================================================================
*/

void Weapon_RocketLauncher_Fire (gentity_t *ent) {
	gentity_t	*m;

	m = fire_rocket (ent, muzzle, forward);
	m->damage *= s_quadFactor;
	m->splashDamage *= s_quadFactor;

//	VectorAdd( m->s.pos.trDelta, ent->client->ps.velocity, m->s.pos.trDelta );	// "real" physics
}


/*
======================================================================

PLASMA GUN

======================================================================
*/

void Weapon_Plasmagun_Fire (gentity_t *ent) {
	gentity_t	*m;

	m = fire_plasma (ent, muzzle, forward);
	m->damage *= s_quadFactor;
	m->splashDamage *= s_quadFactor;

//	VectorAdd( m->s.pos.trDelta, ent->client->ps.velocity, m->s.pos.trDelta );	// "real" physics
}

/*
======================================================================

RAILGUN

======================================================================
*/


/*
=================
weapon_railgun_fire
=================
*/
#define	MAX_RAIL_HITS	4
void weapon_railgun_fire (gentity_t *ent) {
	vec3_t		end;
	vec3_t impactpoint, bouncedir;
	trace_t		trace;
	gentity_t	*tent;
	gentity_t	*traceEnt;
	int			damage;
	int			i;
	int			hits;
	int			unlinked;
	int			passent;
	gentity_t	*unlinkedEntities[MAX_RAIL_HITS];

	damage = 100 * s_quadFactor;

	VectorMA (muzzle, 8192, forward, end);

	// backward-reconcile the other clients
	G_DoTimeShiftFor( ent );

	// trace only against the solids, so the railgun will go through people
	unlinked = 0;
	hits = 0;
	passent = ent->s.number;
	do {
		trap_Trace (&trace, muzzle, NULL, NULL, end, passent, MASK_SHOT );
		if ( trace.entityNum >= ENTITYNUM_MAX_NORMAL ) {
			break;
		}
		traceEnt = &g_entities[ trace.entityNum ];
		if ( traceEnt->takedamage ) {
			if ( traceEnt->client && traceEnt->client->invulnerabilityTime > level.time ) {
				if ( G_InvulnerabilityEffect( traceEnt, forward, trace.endpos, impactpoint, bouncedir ) ) {
					G_BounceProjectile( muzzle, impactpoint, bouncedir, end );
					// snap the endpos to integers to save net bandwidth, but nudged towards the line
					SnapVectorTowards( trace.endpos, muzzle );
					// send railgun beam effect
					tent = G_TempEntity( trace.endpos, EV_RAILTRAIL );
					// set player number for custom colors on the railtrail
					tent->s.clientNum = ent->s.clientNum;
					VectorCopy( muzzle, tent->s.origin2 );
					// move origin a bit to come closer to the drawn gun muzzle
					VectorMA( tent->s.origin2, 4, right, tent->s.origin2 );
					VectorMA( tent->s.origin2, -1, up, tent->s.origin2 );
					tent->s.eventParm = 255;	// don't make the explosion at the end
					//
					VectorCopy( impactpoint, muzzle );
					// the player can hit him/herself with the bounced rail
					passent = ENTITYNUM_NONE;
				}
			} else {
				G_Damage (traceEnt, ent, ent, forward, trace.endpos, damage, 0, MOD_RAILGUN);
			}
		}
		if ( trace.contents & CONTENTS_SOLID ) {
			break;		// we hit something solid enough to stop the beam
		}
		// unlink this entity, so the next trace will go past it
		trap_UnlinkEntity( traceEnt );
		unlinkedEntities[unlinked] = traceEnt;
		unlinked++;
	} while ( unlinked < MAX_RAIL_HITS );

	// put them back
	G_UndoTimeShiftFor( ent );

	// link back in any entities we unlinked
	for ( i = 0 ; i < unlinked ; i++ ) {
		trap_LinkEntity( unlinkedEntities[i] );
	}

	// the final trace endpos will be the terminal point of the rail trail

	// snap the endpos to integers to save net bandwidth, but nudged towards the line
	SnapVectorTowards( trace.endpos, muzzle );

	// send railgun beam effect
	tent = G_TempEntity( trace.endpos, EV_RAILTRAIL );

	// set player number for custom colors on the railtrail
	tent->s.clientNum = ent->s.clientNum;

	VectorCopy( muzzle, tent->s.origin2 );
	// move origin a bit to come closer to the drawn gun muzzle
	VectorMA( tent->s.origin2, 4, right, tent->s.origin2 );
	VectorMA( tent->s.origin2, -1, up, tent->s.origin2 );

	// no explosion at end if SURF_NOIMPACT, but still make the trail
	if ( trace.surfaceFlags & SURF_NOIMPACT ) {
		tent->s.eventParm = 255;	// don't make the explosion at the end
	} else {
		tent->s.eventParm = DirToByte( trace.plane.normal );
	}
	tent->s.clientNum = ent->s.clientNum;
}

/*
======================================================================

GRAPPLING HOOK

======================================================================
*/

void Weapon_GrapplingHook_Fire (gentity_t *ent)
{
	if (!ent->client->fireHeld && !ent->client->hook)
		fire_grapple (ent, muzzle, forward);

	ent->client->fireHeld = qtrue;
}

void Weapon_Grapple_Fire (gentity_t *ent)
{
	if (!ent->client->fireHeld && !ent->client->hook)
		fire_grapple (ent, muzzle, forward);

	ent->client->fireHeld = qtrue;
}

void Weapon_HookFree (gentity_t *ent)
{
	ent->parent->client->hook = NULL;
	ent->parent->client->ps.pm_flags &= ~PMF_GRAPPLE_PULL;
	G_FreeEntity( ent );
}

void Weapon_HookThink (gentity_t *ent)
{
	if (ent->enemy) {
		vec3_t v, oldorigin;

		VectorCopy(ent->r.currentOrigin, oldorigin);
		v[0] = ent->enemy->r.currentOrigin[0] + (ent->enemy->r.mins[0] + ent->enemy->r.maxs[0]) * 0.5;
		v[1] = ent->enemy->r.currentOrigin[1] + (ent->enemy->r.mins[1] + ent->enemy->r.maxs[1]) * 0.5;
		v[2] = ent->enemy->r.currentOrigin[2] + (ent->enemy->r.mins[2] + ent->enemy->r.maxs[2]) * 0.5;
		SnapVectorTowards( v, oldorigin );	// save net bandwidth

		G_SetOrigin( ent, v );
	}

	VectorCopy( ent->r.currentOrigin, ent->parent->client->ps.grapplePoint);
}

/*
======================================================================

LIGHTNING GUN

======================================================================
*/

void Weapon_LightningFire( gentity_t *ent ) {
	trace_t		tr;
	vec3_t		end;
	vec3_t impactpoint, bouncedir;
	gentity_t	*traceEnt, *tent;
	int			damage, i, passent;

	damage = 8 * s_quadFactor;

	passent = ent->s.number;
	for (i = 0; i < 10; i++) {
		VectorMA( muzzle, LIGHTNING_RANGE, forward, end );

	G_DoTimeShiftFor( ent );

	trap_Trace( &tr, muzzle, NULL, NULL, end, passent, MASK_SHOT );

	G_UndoTimeShiftFor( ent );

	// if not the first trace (the lightning bounced of an invulnerability sphere)
	if (i) {
		tent = G_TempEntity( muzzle, EV_LIGHTNINGBOLT );
		VectorCopy( tr.endpos, end );
		SnapVector( end );
		VectorCopy( end, tent->s.origin2 );
	}
	if ( tr.entityNum == ENTITYNUM_NONE ) {
		return;
	}

	traceEnt = &g_entities[ tr.entityNum ];

	if ( traceEnt->takedamage) {
		if ( traceEnt->client && traceEnt->client->invulnerabilityTime > level.time ) {
			if (G_InvulnerabilityEffect( traceEnt, forward, tr.endpos, impactpoint, bouncedir )) {
				G_BounceProjectile( muzzle, impactpoint, bouncedir, end );
				VectorCopy( impactpoint, muzzle );
				VectorSubtract( end, impactpoint, forward );
				VectorNormalize(forward);
				// the player can hit him/herself with the bounced lightning
				passent = ENTITYNUM_NONE;
			}
			else {
				VectorCopy( tr.endpos, muzzle );
				passent = traceEnt->s.number;
			}
			continue;
		}
		else {
			G_Damage( traceEnt, ent, ent, forward, tr.endpos,
				damage, 0, MOD_LIGHTNING);
		}
	}

	if ( traceEnt->takedamage && traceEnt->client ) {
		tent = G_TempEntity( tr.endpos, EV_MISSILE_MISS );
		tent->s.otherEntityNum = traceEnt->s.number;
		tent->s.eventParm = DirToByte( tr.plane.normal );
		tent->s.weapon = ent->s.weapon;
	} else if ( !( tr.surfaceFlags & SURF_NOIMPACT ) ) {
		tent = G_TempEntity( tr.endpos, EV_MISSILE_MISS );
		tent->s.eventParm = DirToByte( tr.plane.normal );
	}

	break;
	}
}

/*
======================================================================

NAILGUN

======================================================================
*/

void Weapon_Nailgun_Fire (gentity_t *ent) {
	gentity_t	*m;
	int			count;

	for( count = 0; count < NUM_NAILSHOTS; count++ ) {
		m = fire_nail (ent, muzzle, forward, right, up );
		m->damage *= s_quadFactor;
		m->splashDamage *= s_quadFactor;
	}

//	VectorAdd( m->s.pos.trDelta, ent->client->ps.velocity, m->s.pos.trDelta );	// "real" physics
}

/*
======================================================================

PROXIMITY MINE LAUNCHER

======================================================================
*/

void weapon_proxlauncher_fire (gentity_t *ent) {
	gentity_t	*m;

	// extra vertical velocity
	forward[2] += 0.2f;
	VectorNormalize( forward );

	m = fire_prox (ent, muzzle, forward);
	m->damage *= s_quadFactor;
	m->splashDamage *= s_quadFactor;

//	VectorAdd( m->s.pos.trDelta, ent->client->ps.velocity, m->s.pos.trDelta );	// "real" physics
}

void Weapon_Flamethrower_Fire (gentity_t *ent) {
	gentity_t	*m;

	m = fire_flame (ent, muzzle, forward);
	m->damage *= s_quadFactor;
	m->splashDamage *= s_quadFactor;

//	VectorAdd( m->s.pos.trDelta, ent->client->ps.velocity, m->s.pos.trDelta );	// "real" physics
}

void Weapon_Antimatter_Fire (gentity_t *ent) {
	gentity_t	*m;

	m = fire_antimatter (ent, muzzle, forward);
	m->damage *= s_quadFactor;
	m->splashDamage *= s_quadFactor;

//	VectorAdd( m->s.pos.trDelta, ent->client->ps.velocity, m->s.pos.trDelta );	// "real" physics
}

/*
======================================================================

TOOLGUN

======================================================================
*/

void Weapon_Toolgun( gentity_t *ent ) {
	trace_t		tr;
	vec3_t		end;
	gentity_t	*traceEnt;

	// set aiming directions
	AngleVectors (ent->client->ps.viewangles, forward, right, up);

	CalcMuzzlePoint ( ent, forward, right, up, muzzle );
	VectorMA (muzzle, TOOLGUN_RANGE, forward, end);
	trap_Trace (&tr, muzzle, NULL, NULL, end, ent->s.number, MASK_SELECT);

	traceEnt = &g_entities[ tr.entityNum ];

	if(g_extendedsandbox.integer){
		if(!traceEnt->sandboxObject && traceEnt->s.eType != ET_PLAYER){
			return;
		}
	} else {
		if(!traceEnt->sandboxObject && !traceEnt->npcType){
			return;
		}
	}

	G_Damage( traceEnt, ent, ent, forward, tr.endpos,
		1, 0, MOD_TOOLGUN );

	return;
}

void Weapon_Toolgun_Info( gentity_t *ent ) {
	trace_t		tr;
	vec3_t		end;
	gentity_t	*traceEnt;
	int			i;

	char info_entity[MAX_ENTITYINFO][64];

	// set aiming directions
	AngleVectors (ent->client->ps.viewangles, forward, right, up);

	CalcMuzzlePoint ( ent, forward, right, up, muzzle );
	VectorMA (muzzle, TOOLGUN_RANGE, forward, end);
	trap_Trace (&tr, muzzle, NULL, NULL, end, ent->s.number, MASK_SELECT);

	traceEnt = &g_entities[ tr.entityNum ];

	if(g_extendedsandbox.integer){
		if(!traceEnt->sandboxObject && traceEnt->s.eType != ET_PLAYER){
			trap_SendServerCommand( ent->s.clientNum, va("t_info \"%s\"", "") );
			return;
		}
	} else {
		if(!traceEnt->sandboxObject && !traceEnt->npcType){
			trap_SendServerCommand( ent->s.clientNum, va("t_info \"%s\"", "") );
			return;
		}
	}

	if(ent->swep_id == WP_TOOLGUN){
		//Classname
		if (!strcmp(traceEnt->sb_class, "none") || !strcmp(traceEnt->sb_class, "")) {
			strcpy(info_entity[0], traceEnt->classname);
		} else {
			strcpy(info_entity[0], traceEnt->sb_class);
		}

		//Model
		if(traceEnt->s.eType == ET_PLAYER){	
			strcpy(info_entity[1], va("%i", traceEnt->s.clientNum));
		} else if(traceEnt->s.eType == ET_ITEM){	
			strcpy(info_entity[1], "<NULL>");
		} else {
			strcpy(info_entity[1], traceEnt->model);
		}

		//Material
		if(traceEnt->sb_material){
			strcpy(info_entity[2], va("%i", traceEnt->sb_material));
		} else {
			strcpy(info_entity[2], "0");
		}
		
		//Count
		if(traceEnt->s.eType == ET_ITEM){
			strcpy(info_entity[3], va("%i", traceEnt->count));
		} else {
			strcpy(info_entity[3], "<NULL>");
		}

		for(i = 0; i < MAX_ENTITYINFO; i++){
			if (!strcmp(info_entity[i], "")) {
				strcpy(info_entity[i], "<NULL>");
			}
		}

		//Send entityInfo
		trap_SendServerCommand( ent->s.clientNum, va("t_info \"%s %s %s %s\"", info_entity[0], info_entity[1], info_entity[2], info_entity[3]) );
	}

	return;
}

/*
======================================================================

NEW WEAPONS

======================================================================
*/

void Weapon_Thrower_Fire (gentity_t *ent) {
	gentity_t	*m;
	int			count;

	for( count = 0; count < 1; count++ ) {
		m = fire_thrower (ent, muzzle, forward, right, up );
		m->damage *= s_quadFactor;
		m->splashDamage *= s_quadFactor;
	}
}

void Weapon_Bouncer_Fire (gentity_t *ent) {
	gentity_t	*m;
	int			count;

	for( count = 0; count < 11; count++ ) {
		m = fire_bouncer (ent, muzzle, forward, right, up );
		m->damage *= s_quadFactor;
		m->splashDamage *= s_quadFactor;
	}
}

void Weapon_Thunder_Fire(gentity_t *ent) {
    gentity_t *m;
    int count;
    vec3_t forward, right, up, spread, newDir;

    // Получаем вектора для направления выстрела
    AngleVectors(ent->client->ps.viewangles, forward, right, up);

    for (count = 0; count < 6; count++) {
        spread[0] = crandom() * 0.45;
        spread[1] = crandom() * 0.45;
        spread[2] = crandom() * 0.10;

        VectorAdd(forward, spread, newDir);

        VectorNormalize(newDir);

        m = fire_grenade(ent, muzzle, newDir);

        m->damage *= s_quadFactor;
        m->splashDamage *= s_quadFactor;
    }
}

void Weapon_Exploder_Fire (gentity_t *ent) {
	gentity_t	*m;

	m = fire_exploder (ent, muzzle, forward);
	m->damage *= s_quadFactor;
	m->splashDamage *= s_quadFactor;
}

void Weapon_Knocker_Fire (gentity_t *ent) {
	gentity_t	*m;
	int			count;

	for( count = 0; count < 1; count++ ) {
		m = fire_knocker (ent, muzzle, forward, right, up );
		m->damage *= s_quadFactor;
		m->splashDamage *= s_quadFactor;
	}
}

void Weapon_Propgun_Fire (gentity_t *ent) {
	gentity_t	*m;
	int			count;

	for( count = 0; count < 1; count++ ) {
		m = fire_propgun (ent, muzzle, forward, right, up );
		m->damage *= s_quadFactor;
		m->splashDamage *= s_quadFactor;
	}

//	VectorAdd( m->s.pos.trDelta, ent->client->ps.velocity, m->s.pos.trDelta );	// "real" physics
}

void Weapon_Regenerator_Fire (gentity_t *ent) {
	gentity_t	*m;
	int			count;

	for( count = 0; count < 1; count++ ) {
		m = fire_regenerator (ent, muzzle, forward);
		m->damage *= s_quadFactor;
		m->splashDamage *= s_quadFactor;
	}
}

void Weapon_Nuke_Fire (gentity_t *ent) {
	gentity_t	*m;
	int			count;

	for( count = 0; count < 8; count++ ) {
		m = fire_nuke (ent, muzzle, forward, right, up );
		m->damage *= s_quadFactor;
		m->splashDamage *= s_quadFactor;
	}
}

/*
===============
CalcMuzzlePoint

set muzzle location relative to pivoting eye
===============
*/
void CalcMuzzlePoint ( gentity_t *ent, vec3_t forward, vec3_t right, vec3_t up, vec3_t muzzlePoint ) {
	VectorCopy( ent->s.pos.trBase, muzzlePoint );
	muzzlePoint[2] += ent->client->ps.viewheight;
	VectorMA( muzzlePoint, 14, forward, muzzlePoint );
	// snap to integer coordinates for more efficient network bandwidth usage
	SnapVector( muzzlePoint );
}

/*
===============
CalcMuzzlePointOrigin

set muzzle location relative to pivoting eye
===============
*/
void CalcMuzzlePointOrigin ( gentity_t *ent, vec3_t origin, vec3_t forward, vec3_t right, vec3_t up, vec3_t muzzlePoint ) {
	VectorCopy( ent->s.pos.trBase, muzzlePoint );
	muzzlePoint[2] += ent->client->ps.viewheight;
	VectorMA( muzzlePoint, 14, forward, muzzlePoint );
	// snap to integer coordinates for more efficient network bandwidth usage
	SnapVector( muzzlePoint );
}

/*
===============
FireWeapon
===============
*/
void FireWeapon( gentity_t *ent ) {
	//Make people drop out of follow mode (this should be moved, so people can change betwean players.)
	if (ent->client->sess.spectatorState == SPECTATOR_FOLLOW) {
		StopFollowing( ent );
		return;
	}

	if (ent->client->ps.powerups[PW_QUAD] ) {
		s_quadFactor = 3;
	} else {
		s_quadFactor = 1;
	}
	if( ent->client->persistantPowerup && ent->client->persistantPowerup->item && ent->client->persistantPowerup->item->giTag == PW_DOUBLER ) {
		s_quadFactor *= 2.0;
	}
	if(ent->skill == 9){
		s_quadFactor *= 5;
	}

    if (ent->client->spawnprotected)
        ent->client->spawnprotected = qfalse;

	// set aiming directions
	AngleVectors (ent->client->ps.viewangles, forward, right, up);

	CalcMuzzlePointOrigin ( ent, ent->client->oldOrigin, forward, right, up, muzzle );
	
	// fire the specific weapon
	switch( ent->swep_id ) {
	case WP_GAUNTLET:
		Weapon_Gauntlet( ent );
		break;
	case WP_MACHINEGUN:
		Bullet_Fire( ent, MACHINEGUN_SPREAD, MACHINEGUN_DAMAGE );
		break;
	case WP_SHOTGUN:
		weapon_supershotgun_fire( ent );
		break;
	case WP_GRENADE_LAUNCHER:
		weapon_grenadelauncher_fire (ent);
		break;
	case WP_ROCKET_LAUNCHER:
		Weapon_RocketLauncher_Fire( ent );
		break;
	case WP_LIGHTNING:
		Weapon_LightningFire( ent );
		break;
	case WP_RAILGUN:
		weapon_railgun_fire( ent );
		break;
	case WP_PLASMAGUN:
		Weapon_Plasmagun_Fire( ent );
		break;
	case WP_BFG:
		BFG_Fire( ent );
		break;
	case WP_GRAPPLING_HOOK:
		Weapon_GrapplingHook_Fire( ent );
		break;
	case WP_NAILGUN:
		Weapon_Nailgun_Fire( ent );
		break;
	case WP_PROX_LAUNCHER:
		weapon_proxlauncher_fire( ent );
		break;
	case WP_CHAINGUN:
		Bullet_Fire( ent, CHAINGUN_SPREAD, MACHINEGUN_DAMAGE );
		break;
	case WP_FLAMETHROWER:
		Weapon_Flamethrower_Fire( ent );
		break;
	case WP_ANTIMATTER:
		Weapon_Antimatter_Fire( ent );
		break;
	case WP_TOOLGUN:
		Weapon_Toolgun( ent );
		break;
	case WP_PHYSGUN:
		//PhysgunHold()
		break;
	case WP_GRAVITYGUN:
		//GravitygunHold()
		break;
	case WP_THROWER:
		Weapon_Thrower_Fire( ent );
		break;
	case WP_BOUNCER:
		Weapon_Bouncer_Fire( ent );
		break;
	case WP_THUNDER:
		Weapon_Thunder_Fire( ent );
		break;
	case WP_EXPLODER:
		Weapon_Exploder_Fire( ent );
		break;
	case WP_KNOCKER:
		Weapon_Knocker_Fire( ent );
		break;
	case WP_PROPGUN:
		Weapon_Propgun_Fire( ent );
		break;
	case WP_REGENERATOR:
		Weapon_Regenerator_Fire( ent );
		break;
	case WP_NUKE:
		Weapon_Nuke_Fire( ent );
		break;

	default:
		break;
	}
	
}


/*
===============
KamikazeRadiusDamage
===============
*/
void KamikazeRadiusDamage( vec3_t origin, gentity_t *attacker, float damage, float radius, int mod ) {
	float		dist;
	gentity_t	*ent;
	int			numListedEntities;
	vec3_t		mins, maxs;
	vec3_t		v;
	vec3_t		dir;
	int			i, e;

	if ( radius < 1 ) {
		radius = 1;
	}

	for ( i = 0 ; i < 3 ; i++ ) {
		mins[i] = origin[i] - radius;
		maxs[i] = origin[i] + radius;
	}

	numListedEntities = trap_EntitiesInBox( mins, maxs, SourceTechEntityList, MAX_GENTITIES );

	for ( e = 0 ; e < numListedEntities ; e++ ) {
		ent = &g_entities[SourceTechEntityList[ e ]];

		if (!ent->takedamage) {
			continue;
		}

		// dont hit things we have already hit
		if( ent->kamikazeTime > level.time ) {
			continue;
		}

		// find the distance from the edge of the bounding box
		for ( i = 0 ; i < 3 ; i++ ) {
			if ( origin[i] < ent->r.absmin[i] ) {
				v[i] = ent->r.absmin[i] - origin[i];
			} else if ( origin[i] > ent->r.absmax[i] ) {
				v[i] = origin[i] - ent->r.absmax[i];
			} else {
				v[i] = 0;
			}
		}

		dist = VectorLength( v );
		if ( dist >= radius ) {
			continue;
		}

//		if( CanDamage (ent, origin) ) {
			VectorSubtract (ent->r.currentOrigin, origin, dir);
			// push the center of mass higher than the origin so players
			// get knocked into the air more
			dir[2] += 24;
			G_Damage( ent, NULL, attacker, dir, origin, damage, DAMAGE_RADIUS|DAMAGE_NO_TEAM_PROTECTION, mod );
			ent->kamikazeTime = level.time + 3000;
//		}
	}
}

/*
===============
KamikazeShockWave
===============
*/
void KamikazeShockWave( vec3_t origin, gentity_t *attacker, float damage, float push, float radius, int mod ) {
	float		dist;
	gentity_t	*ent;
	int			numListedEntities;
	vec3_t		mins, maxs;
	vec3_t		v;
	vec3_t		dir;
	int			i, e;

	if ( radius < 1 )
		radius = 1;

	for ( i = 0 ; i < 3 ; i++ ) {
		mins[i] = origin[i] - radius;
		maxs[i] = origin[i] + radius;
	}

	numListedEntities = trap_EntitiesInBox( mins, maxs, SourceTechEntityList, MAX_GENTITIES );

	for ( e = 0 ; e < numListedEntities ; e++ ) {
		ent = &g_entities[SourceTechEntityList[ e ]];

		// dont hit things we have already hit
		if( ent->kamikazeShockTime > level.time ) {
			continue;
		}

		// find the distance from the edge of the bounding box
		for ( i = 0 ; i < 3 ; i++ ) {
			if ( origin[i] < ent->r.absmin[i] ) {
				v[i] = ent->r.absmin[i] - origin[i];
			} else if ( origin[i] > ent->r.absmax[i] ) {
				v[i] = origin[i] - ent->r.absmax[i];
			} else {
				v[i] = 0;
			}
		}

		dist = VectorLength( v );
		if ( dist >= radius ) {
			continue;
		}

//		if( CanDamage (ent, origin) ) {
			VectorSubtract (ent->r.currentOrigin, origin, dir);
			dir[2] += 24;
			G_Damage( ent, NULL, attacker, dir, origin, damage, DAMAGE_RADIUS|DAMAGE_NO_TEAM_PROTECTION, mod );
			//
			dir[2] = 0;
			VectorNormalize(dir);
			if ( ent->client ) {
				ent->client->ps.velocity[0] = dir[0] * push;
				ent->client->ps.velocity[1] = dir[1] * push;
				ent->client->ps.velocity[2] = 100;
			}
			ent->kamikazeShockTime = level.time + 3000;
//		}
	}
}

/*
===============
KamikazeDamage
===============
*/
void KamikazeDamage( gentity_t *self ) {
	int i;
	float t;
	gentity_t *ent;
	vec3_t newangles;

	self->count += 100;

	if (self->count >= KAMI_SHOCKWAVE_STARTTIME) {
		// shockwave push back
		t = self->count - KAMI_SHOCKWAVE_STARTTIME;
		KamikazeShockWave(self->s.pos.trBase, self->activator, 25, 400,	(int) (float) t * KAMI_SHOCKWAVE_MAXRADIUS / (KAMI_SHOCKWAVE_ENDTIME - KAMI_SHOCKWAVE_STARTTIME), MOD_KAMIKAZE );
	}
	//
	if (self->count >= KAMI_EXPLODE_STARTTIME) {
		// do our damage
		t = self->count - KAMI_EXPLODE_STARTTIME;
		KamikazeRadiusDamage( self->s.pos.trBase, self->activator, 400,	(int) (float) t * KAMI_BOOMSPHERE_MAXRADIUS / (KAMI_IMPLODE_STARTTIME - KAMI_EXPLODE_STARTTIME), MOD_KAMIKAZE );
	}

	// either cycle or kill self
	if( self->count >= KAMI_SHOCKWAVE_ENDTIME ) {
		G_FreeEntity( self );
		return;
	}
	self->nextthink = level.time + 100;

	// add earth quake effect
	newangles[0] = crandom() * 2;
	newangles[1] = crandom() * 2;
	newangles[2] = 0;
	for (i = 0; i < MAX_CLIENTS; i++)
	{
		ent = &g_entities[i];
		if (!ent->inuse)
			continue;
		if (!ent->client)
			continue;

		if (ent->client->ps.groundEntityNum != ENTITYNUM_NONE) {
			ent->client->ps.velocity[0] += crandom() * 120;
			ent->client->ps.velocity[1] += crandom() * 120;
			ent->client->ps.velocity[2] = 30 + random() * 25;
		}

		ent->client->ps.delta_angles[0] += ANGLE2SHORT(newangles[0] - self->movedir[0]);
		ent->client->ps.delta_angles[1] += ANGLE2SHORT(newangles[1] - self->movedir[1]);
		ent->client->ps.delta_angles[2] += ANGLE2SHORT(newangles[2] - self->movedir[2]);
	}
	VectorCopy(newangles, self->movedir);
}

/*
===============
G_StartKamikaze
===============
*/
void G_StartKamikaze( gentity_t *ent ) {
	gentity_t	*explosion;
	gentity_t	*te;
	vec3_t		snapped;

	// start up the explosion logic
	explosion = G_Spawn();

	explosion->s.eType = ET_EVENTS + EV_KAMIKAZE;
	explosion->eventTime = level.time;

	if ( ent->client ) {
		VectorCopy( ent->s.pos.trBase, snapped );
	}
	else {
		VectorCopy( ent->activator->s.pos.trBase, snapped );
	}
	SnapVector( snapped );		// save network bandwidth
	G_SetOrigin( explosion, snapped );

	explosion->classname = "kamikaze";
	explosion->s.pos.trType = TR_STATIONARY;

	explosion->kamikazeTime = level.time;

	explosion->think = KamikazeDamage;
	explosion->nextthink = level.time + 100;
	explosion->count = 0;
	VectorClear(explosion->movedir);

	trap_LinkEntity( explosion );

	if (ent->client) {
		//
		explosion->activator = ent;
		//
		ent->s.eFlags &= ~EF_KAMIKAZE;
		// nuke the guy that used it
		//G_Damage( ent, ent, ent, NULL, NULL, 100000, DAMAGE_NO_PROTECTION, MOD_KAMIKAZE );
	}
	else {
		if ( !strcmp(ent->activator->classname, "bodyque") ) {
			explosion->activator = &g_entities[ent->activator->r.ownerNum];
		}
		else {
			explosion->activator = ent->activator;
		}
	}

	// play global sound at all clients
	te = G_TempEntity(snapped, EV_GLOBAL_TEAM_SOUND );
	te->r.svFlags |= SVF_BROADCAST;
	te->s.eventParm = GTS_KAMIKAZE;
}

void CarExplodeDamage( gentity_t *self ) {
	float t;

	self->count += 100;

	if (self->count >= KAMI_SHOCKWAVE_STARTTIME) {
		// shockwave push back
		t = self->count - KAMI_SHOCKWAVE_STARTTIME;
		KamikazeShockWave(self->s.pos.trBase, self->activator, 25, 400,	(int) (float) t * 300 / (KAMI_SHOCKWAVE_ENDTIME - KAMI_SHOCKWAVE_STARTTIME), MOD_CAREXPLODE );
	}
	//
	if (self->count >= KAMI_EXPLODE_STARTTIME) {
		// do our damage
		t = self->count - KAMI_EXPLODE_STARTTIME;
		KamikazeRadiusDamage( self->s.pos.trBase, self->activator, 400,	(int) (float) t * 150 / (KAMI_IMPLODE_STARTTIME - KAMI_EXPLODE_STARTTIME), MOD_CAREXPLODE );
	}

	// either cycle or kill self
	if( self->count >= KAMI_SHOCKWAVE_ENDTIME ) {
		G_FreeEntity( self );
		return;
	}
	self->nextthink = level.time + 100;
}

void NukeExplodeDamage( gentity_t *self ) {
	float t;

	self->count += 100;

	if (self->count >= KAMI_SHOCKWAVE_STARTTIME) {
		// shockwave push back
		t = self->count - KAMI_SHOCKWAVE_STARTTIME;
		KamikazeShockWave(self->s.pos.trBase, self->lastPlayer, 50, 400,	800, MOD_NUKE );
	}
	//
	if (self->count >= KAMI_EXPLODE_STARTTIME) {
		// do our damage
		t = self->count - KAMI_EXPLODE_STARTTIME;
		KamikazeRadiusDamage( self->s.pos.trBase, self->lastPlayer, 100, 800, MOD_NUKE );
	}

	// either cycle or kill self
	if( self->count >= KAMI_SHOCKWAVE_ENDTIME ) {
		G_FreeEntity( self );
		return;
	}
	self->nextthink = level.time + 100;
}

void G_StartCarExplode( gentity_t *ent ) {
	gentity_t	*explosion;
	gentity_t	*te;
	vec3_t		snapped;

	// start up the explosion logic
	explosion = G_Spawn();

	explosion->s.eType = ET_EVENTS + EV_KAMIKAZE;
	explosion->eventTime = level.time;

	VectorCopy( ent->r.currentOrigin, snapped );
	SnapVector( snapped );		// save network bandwidth
	G_SetOrigin( explosion, snapped );

	explosion->classname = "kamikaze";
	explosion->s.pos.trType = TR_STATIONARY;

	explosion->kamikazeTime = level.time;

	explosion->think = CarExplodeDamage;
	explosion->nextthink = level.time + 100;
	explosion->count = 0;
	VectorClear(explosion->movedir);

	trap_LinkEntity( explosion );

	if ( !strcmp(ent->activator->classname, "bodyque") ) {
		explosion->activator = &g_entities[ent->activator->r.ownerNum];
	}
	else {
		explosion->activator = ent->activator;
	}

	// play global sound at all clients
	te = G_TempEntity(snapped, EV_GLOBAL_TEAM_SOUND );
	te->r.svFlags |= SVF_BROADCAST;
	te->s.eventParm = GTS_KAMIKAZE;
}

void G_StartNukeExplode( gentity_t *ent ) {
	gentity_t	*explosion;
	gentity_t	*te;
	vec3_t		snapped;

	// start up the explosion logic
	explosion = G_Spawn();

	explosion->s.eType = ET_EVENTS + EV_KAMIKAZE;
	explosion->eventTime = level.time;

	VectorCopy( ent->r.currentOrigin, snapped );
	SnapVector( snapped );		// save network bandwidth
	G_SetOrigin( explosion, snapped );

	explosion->classname = "kamikaze";
	explosion->s.pos.trType = TR_STATIONARY;

	explosion->kamikazeTime = level.time;

	explosion->think = NukeExplodeDamage;
	explosion->nextthink = level.time + 100;
	explosion->count = 0;
	VectorClear(explosion->movedir);

	trap_LinkEntity( explosion );

	if ( !strcmp(ent->activator->classname, "bodyque") ) {
		explosion->activator = &g_entities[ent->activator->r.ownerNum];
	}
	else {
		explosion->activator = ent->activator;
	}

	// play global sound at all clients
	te = G_TempEntity(snapped, EV_GLOBAL_TEAM_SOUND );
	te->r.svFlags |= SVF_BROADCAST;
	te->s.eventParm = GTS_KAMIKAZE;
}
