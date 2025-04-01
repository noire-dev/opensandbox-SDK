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
// g_physics.c

#include "g_local.h"

/*
================
Phys_Disable

Disables physics
================
*/
void Phys_Disable( gentity_t *ent, vec3_t origin ) {
	VectorCopy( origin, ent->r.currentOrigin );	//physics origin
	ent->s.pos.trType = TR_STATIONARY;
	ent->s.pos.trTime = 0;
	ent->s.pos.trDuration = 0;
	VectorClear( ent->s.pos.trDelta );
}

/*
================
Phys_Enable

Enables physics
================
*/
void Phys_Enable( gentity_t *ent ) {
	if(ent->sb_phys != 2){	//if it's static object, not turn phys
		return;	
	}
	VectorCopy( ent->r.currentOrigin, ent->s.pos.trBase );		//restore client origin from physics origin
	if(ent->s.pos.trType != TR_GRAVITY_WATER){
	ent->s.pos.trType = TR_GRAVITY;
	}
	ent->s.pos.trTime = level.time;
	ent->s.pos.trDuration = level.time;
}

/*
================
Phys_Damage

Damage from physic object
================
*/
void Phys_Damage (gentity_t *targ, gentity_t *attacker, int damage){
	G_Damage( targ, attacker, attacker, NULL, NULL, damage, 0, MOD_PROP );
}

/*
================
Phys_CarDamage

Damage from physic car
================
*/
void Phys_CarDamage (gentity_t *targ, gentity_t *attacker, int damage){
	G_Damage( targ, attacker, attacker, NULL, NULL, damage, 0, MOD_CAR );
}

/*
================
Phys_Smoke

Smoke from physic object
================
*/
void Phys_Smoke( gentity_t *ent, float impact ) {
	gentity_t   *temp;
	
	temp = G_TempEntity(ent->r.currentOrigin, EV_SMOKEPUFF);
	temp->s.constantLight = (((70 | 64 << 8 ) | 64 << 16 ) | ( 255 << 24 ));
	temp->s.eventParm = 2;	//eventParm is used to determine the amount of time the smoke puff exists
	temp->s.generic1 = 100;	//generic1 is used to determine the movement speed of the smoke puff
	temp->s.otherEntityNum = (int)impact; //otherEntityNum is used to determine the size of the smokepuff. The default is 32.
	VectorCopy(ent->s.angles, temp->s.angles);
}

/*
================
Phys_WaterSplash

Water splash from physic object
================
*/
void Phys_WaterSplash( gentity_t *ent, float impact ){
	gentity_t   *temp;
	
	temp = G_TempEntity(ent->r.currentOrigin, EV_WATERPUFF);
	temp->s.eventParm = 1;	//eventParm is used to determine the amount of time the smoke puff exists
	temp->s.generic1 = 100;	//generic1 is used to determine the movement speed of the smoke puff
	temp->s.otherEntityNum = (int)impact; //otherEntityNum is used to determine the size of the smokepuff. The default is 32.
}

/*
================
Phys_Bounce

Bounce for physic object
================
*/
void Phys_Bounce( gentity_t *ent, trace_t *trace ) {
	vec3_t	velocity;
	float	dot;
	int		hitTime;
    float 	randomOffset[3];
	int		i;

	// reflect the velocity on the trace plane
	hitTime = level.previousTime + ( level.time - level.previousTime ) * trace->fraction;
	ST_EvaluateTrajectoryDelta( &ent->s.pos, hitTime, velocity, ent->s.generic3 );
	dot = DotProduct( velocity, trace->plane.normal );
	VectorMA( velocity, -2*dot, trace->plane.normal, ent->s.pos.trDelta );

	VectorScale( ent->s.pos.trDelta, ent->physicsBounce, ent->s.pos.trDelta );

    for (i = 0; i < 3; i++) {
        randomOffset[i] = ((float)rand() / 32767 - 0.5f) * 20.0f;
    }
    VectorAdd(ent->s.pos.trDelta, randomOffset, ent->s.pos.trDelta);

	// check for stop
	if ( trace->plane.normal[2] > 0.2 && VectorLength( ent->s.pos.trDelta ) < 40 && !ent->isGrabbed) {        
        Phys_Disable(ent, trace->endpos);
		return;
	}

	VectorAdd( ent->r.currentOrigin, trace->plane.normal, ent->r.currentOrigin);
	VectorCopy( ent->r.currentOrigin, ent->s.pos.trBase );	//update client origin from physics origin
	ent->s.pos.trTime = level.time;
}

/*
================
Phys_CalculateBounds

Calculate bounds for physic object
================
*/
void Phys_CalculateBounds(const vec3_t angles, const vec3_t mins, const vec3_t maxs, vec3_t outMins, vec3_t outMaxs) {
    vec3_t forward, right, up;
    vec3_t corners[8];
	vec3_t rotatedCorners[8];
	int    i, j;

    AngleVectors(angles, forward, right, up);

    for (i = 0; i < 8; i++) {
        corners[i][0] = (i & 1) ? maxs[0] : mins[0];
        corners[i][1] = (i & 2) ? maxs[1] : mins[1];
        corners[i][2] = (i & 4) ? maxs[2] : mins[2];
    }

    for (i = 0; i < 8; i++) {
        rotatedCorners[i][0] = corners[i][0] * forward[0] + corners[i][1] * right[0] + corners[i][2] * up[0];
        rotatedCorners[i][1] = corners[i][0] * forward[1] + corners[i][1] * right[1] + corners[i][2] * up[1];
        rotatedCorners[i][2] = corners[i][0] * forward[2] + corners[i][1] * right[2] + corners[i][2] * up[2];
    }

    VectorCopy(rotatedCorners[0], outMins);
    VectorCopy(rotatedCorners[0], outMaxs);
    for (i = 1; i < 8; i++) {
        for (j = 0; j < 3; j++) {
            if (rotatedCorners[i][j] < outMins[j]) outMins[j] = rotatedCorners[i][j];
            if (rotatedCorners[i][j] > outMaxs[j]) outMaxs[j] = rotatedCorners[i][j];
        }
    }
}

/*
================
Phys_CheckGround

Check ground for physic object
================
*/
qboolean Phys_CheckGround(gentity_t *ent, float distance) {
    vec3_t start, end;
    trace_t tr;

    VectorCopy(ent->r.currentOrigin, start);
    VectorCopy(start, end);
	end[2] -= distance;

    trap_Trace(&tr, start, ent->r.mins, ent->r.maxs, end, ent->s.number, MASK_PLAYERSOLID);

    return (tr.fraction < 1.0f);
}

/*
================
Phys_Rotate

Rotate for physic object
================
*/
void Phys_Rotate(gentity_t *ent, trace_t *tr) {

	if (!ent->isGrabbed && !tr->startsolid){
		if(ent->s.pos.trType != TR_GRAVITY_WATER){
			if (ent->s.pos.trDelta[2] != 0) {
				ent->r.currentAngles[0] -= ent->s.pos.trDelta[2] * PHYS_ROTATING * 0.20;
				ent->r.currentAngles[1] -= ent->s.pos.trDelta[2] * PHYS_ROTATING * 0.20;
			}
			if (ent->s.pos.trDelta[1] != 0) {
				ent->r.currentAngles[1] -= ent->s.pos.trDelta[1] * PHYS_ROTATING;
			}
			if (ent->s.pos.trDelta[0] != 0) {
				ent->r.currentAngles[0] += ent->s.pos.trDelta[0] * PHYS_ROTATING;
			}
		} else {
			if (ent->s.pos.trDelta[2] != 0) {
				ent->r.currentAngles[0] -= ent->s.pos.trDelta[2] * PHYS_ROTATING * 0.10;
				ent->r.currentAngles[1] -= ent->s.pos.trDelta[2] * PHYS_ROTATING * 0.10;
			}
			if (ent->s.pos.trDelta[1] != 0) {
				ent->r.currentAngles[1] -= ent->s.pos.trDelta[1] * PHYS_ROTATING * 0.50;
			}
			if (ent->s.pos.trDelta[0] != 0) {
				ent->r.currentAngles[0] += ent->s.pos.trDelta[0] * PHYS_ROTATING * 0.50;
			}
		}
	}

}

/*
================
Phys_Impact

Impact for physic object
================
*/
void Phys_Impact(gentity_t *ent, trace_t *tr) {
	gentity_t *hit;
	vec3_t impactVector;
	float impactForce;
	float impactForceAll;
	float impactForceFixed;

	// Calculate the impact force
	impactForce = sqrt(ent->s.pos.trDelta[0] * ent->s.pos.trDelta[0] + ent->s.pos.trDelta[1] * ent->s.pos.trDelta[1]);
	impactForceAll = sqrt(ent->s.pos.trDelta[0] * ent->s.pos.trDelta[0] + ent->s.pos.trDelta[1] * ent->s.pos.trDelta[1] + ent->s.pos.trDelta[2] * ent->s.pos.trDelta[2]);
	impactForceFixed = sqrt(ent->s.pos.trDelta[0] * ent->s.pos.trDelta[0] + ent->s.pos.trDelta[1] * ent->s.pos.trDelta[1] + g_gravity.integer*g_gravityModifier.value * g_gravity.integer*g_gravityModifier.value);
	
	impactForce *= (ent->s.generic3/800);
	impactForceAll *= (ent->s.generic3/800);
	impactForceFixed *= (ent->s.generic3/800);

	if(ent->s.pos.trType == TR_GRAVITY || ent->s.pos.trType == TR_GRAVITY_WATER){
		if(trap_PointContents(tr->endpos, ent->s.number) & MASK_WATER){
			if(ent->s.pos.trType != TR_GRAVITY_WATER){
				ent->s.pos.trType = TR_GRAVITY_WATER;
				Phys_WaterSplash( ent, impactForceFixed);
				Phys_Enable(ent);
				ent->s.pos.trDelta[0] *= 0.50;
				ent->s.pos.trDelta[1] *= 0.50;
				if(ent->s.pos.trDelta[2]){
				ent->s.pos.trDelta[2] = 0;
				}
			}
		} else {
			if(ent->s.pos.trType != TR_GRAVITY){
				ent->s.pos.trType = TR_GRAVITY;
				Phys_Enable(ent);
			}
		}
	}

    // If there's a collision, handle it
    if (tr->fraction < 1.0f && tr->entityNum != ENTITYNUM_NONE) {
        hit = &g_entities[tr->entityNum];

        if (hit->s.number != ent->s.number) {  // Ignore self
            // Optionally apply a force or velocity to the hit entity to simulate the push
			if (impactForce > PHYS_SENS) {
				if (!hit->client){
					Phys_Enable(hit);
				}
				VectorCopy(ent->s.pos.trDelta, impactVector);
				VectorScale(impactVector, PHYS_PROP_IMPACT, impactVector);	
				if (!hit->client){
            		VectorAdd(hit->s.pos.trDelta, impactVector, hit->s.pos.trDelta);  // Transfer velocity from the prop to the hit entity
				} else {
					if(hit->grabbedEntity != ent && hit->botskill != 9){
						VectorAdd(hit->client->ps.velocity, impactVector, hit->client->ps.velocity);  // Transfer velocity from the prop to the hit player
					}
				}
			}
			if(impactForceAll > PHYS_DAMAGESENS && !tr->startsolid){
				if(hit->grabbedEntity != ent){
					Phys_Damage(hit, ent->lastPlayer, (int)(impactForceAll * PHYS_DAMAGE));
				}
				Phys_Damage(ent, NULL, (int)(impactForceAll * PHYS_DAMAGE));

				if(hit->grabbedEntity != ent){
					if(ent->s.pos.trType != TR_GRAVITY_WATER){
						if(ent->objectType == OT_BASIC){
							G_AddEvent( ent, EV_OT1_IMPACT, 0 );
							Phys_Smoke( ent, impactForceAll*0.20);
						}
						if(ent->objectType == OT_VEHICLE){
							G_AddEvent( ent, EV_CRASH25, 0 );
							Phys_Smoke( ent, impactForceAll*0.20);
						}
						if(ent->objectType == OT_TNT){
							G_AddEvent( ent, EV_OT1_IMPACT, 0 );
							Phys_Smoke( ent, impactForceAll*0.20);
						}
					}
				}
			}
        }
    }

}

/*
================
Phys_Frame

Physics frame for object
================
*/
void Phys_Frame(gentity_t *ent) {
    vec3_t origin;
    vec3_t adjustedMins, adjustedMaxs;
    trace_t tr;
	
	if(ent->r.currentOrigin[2] <= -520000){
		G_FreeEntity(ent);
		return;
	}

    // Unlink the entity so that it won't interact with other entities during the calculation
    trap_UnlinkEntity(ent);
	
    // If ground entity has been set to -1, apply gravity if necessary
    if (ent->s.groundEntityNum == -1 && ent->s.pos.trType != TR_GRAVITY) {
        ent->s.pos.trType = TR_GRAVITY;
        ent->s.pos.trTime = level.time;
    }

	// Update rotate
	VectorCopy(ent->r.currentAngles, ent->s.angles);		//update server angles from physics angles
	VectorCopy(ent->r.currentAngles, ent->s.apos.trBase);	//update client angles from physics angles

    // If the entity is stationary, re-link it and run the think function
    if (ent->s.pos.trType == TR_STATIONARY && Phys_CheckGround(ent, 8.0f)) {
        trap_LinkEntity(ent);
        G_RunThink(ent);
        return;
    } else {
		if(ent->s.pos.trType == TR_STATIONARY){
			Phys_Enable(ent);
		}
	}
	
	// Get current position based on the entity's trajectory
	if(ent->s.eType == ET_GENERAL){ 		//is prop
    	ST_EvaluateTrajectory(&ent->s.pos, level.time, origin, ent->s.generic3);
	} else {
    	BG_EvaluateTrajectory(&ent->s.pos, level.time, origin);		
	}

    // Trace a line from the current origin to the new position
	Phys_CalculateBounds(ent->r.currentAngles, ent->r.mins, ent->r.maxs, adjustedMins, adjustedMaxs);
    trap_Trace(&tr, ent->r.currentOrigin, adjustedMins, adjustedMaxs, origin, ent->s.number, MASK_PLAYERSOLID);

    // Save origin
    VectorCopy(tr.endpos, ent->s.origin);			//update server origin from trace endpos
	VectorCopy(tr.endpos, ent->r.currentOrigin);	//update physics origin from trace endpos
	
    // Link the entity back into the world
    trap_LinkEntity(ent);

	//Check impacts
	Phys_Impact(ent, &tr);

	//Change angles
	Phys_Rotate(ent, &tr);

    // Check for solid start (possible embedded in another object)
    if (tr.startsolid) {
        tr.fraction = 0;
    }

    // Run think function after updating entity
    G_RunThink(ent);
	
	if ( tr.fraction == 1 ) {
		return;
	}

	//Process bounce
	Phys_Bounce(ent, &tr);
}
