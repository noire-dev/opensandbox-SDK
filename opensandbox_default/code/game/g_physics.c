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

#define PHYS_COL		 	0.75
#define PHYS_COL_CHECK	 	0.75

/*
================
Phys_CheckCarCollisions

Checks car collisions
================
*/
void Phys_CheckCarCollisions(gentity_t *ent) {
	vec3_t newMins, newMaxs;
    trace_t tr;
	gentity_t *hit;
	float impactForce;
	vec3_t impactVector;
	vec3_t end, start, forward, up, right;
	
	if(BG_VehicleCheckClass(ent->client->ps.stats[STAT_VEHICLE]) != VCLASS_CAR && ent->botskill != 9){
	return;
	}
	
	//Set Aiming Directions
	AngleVectors(ent->client->ps.viewangles, forward, right, up);
	CalcMuzzlePoint(ent, forward, right, up, start);
	VectorMA (start, 1, forward, end);
	
	VectorCopy(ent->r.mins, newMins);
	VectorCopy(ent->r.maxs, newMaxs);
	VectorScale(newMins, 1.15, newMins);
	VectorScale(newMaxs, 1.15, newMaxs);
	newMins[2] *= 0.20;
	newMaxs[2] *= 0.20;
	trap_Trace(&tr, ent->r.currentOrigin, newMins, newMaxs, end, ent->s.number, MASK_PLAYERSOLID);
	
	if (tr.fraction < 1.0f && tr.entityNum != ENTITYNUM_NONE) {
        hit = &g_entities[tr.entityNum];

        if (hit->s.number != ent->s.number) {  // Ignore self
            // Calculate the impact force
            impactForce = sqrt(ent->client->ps.velocity[0] * ent->client->ps.velocity[0] + ent->client->ps.velocity[1] * ent->client->ps.velocity[1]);

            // Optionally apply a force or velocity to the hit entity to simulate the push
			if (impactForce > VEHICLE_SENS) {
			if (!hit->client){
			Phys_Enable(hit);
			}
			VectorCopy(ent->client->ps.velocity, impactVector);
			VectorScale(impactVector, VEHICLE_PROP_IMPACT, impactVector);
			impactVector[2] = impactForce*0.15;
			if (!hit->client){
			hit->lastPlayer = ent;		//for save attacker
            VectorAdd(hit->s.pos.trDelta, impactVector, hit->s.pos.trDelta);  // Transfer velocity from the prop to the hit entity
			} else {
			VectorAdd(hit->client->ps.velocity, impactVector, hit->client->ps.velocity);  // Transfer velocity from the prop to the hit player
			}
			}
			if(impactForce > VEHICLE_DAMAGESENS){
			if(hit->grabbedEntity != ent){
			if(BG_VehicleCheckClass(ent->client->ps.stats[STAT_VEHICLE]) == VCLASS_CAR || (ent->botskill == 9 && hit->botskill != 9)){
				Phys_CarDamage(hit, ent, (int)(impactForce * VEHICLE_DAMAGE));
			}
			}
			}
			if(impactForce > VEHICLE_DAMAGESENS*6){
				if(BG_VehicleCheckClass(ent->client->ps.stats[STAT_VEHICLE]) == VCLASS_CAR){
					Phys_Smoke( ent, impactForce*0.20);
				}
			}
        }
    }
}

/*
================
Phys_HoldDropStatic

Throw entity with physgun or gravitygun with PHYS_STATIC
================
*/
void Phys_HoldDropStatic(gentity_t *player, vec3_t velocity){
	gentity_t 	*ent = player->grabbedEntity;

	if(!ent->client){
		ent->s.pos.trType = TR_STATIONARY;
		ent->sb_phys = PHYS_STATIC;
		if(ent->s.eType == ET_ITEM){
			ent->spawnflags = 1;			//for items phys
		}
		VectorClear( ent->s.pos.trDelta );
	} else {
		VectorClear( ent->client->ps.velocity );
	}
}

/*
================
Phys_HoldDropDynamic

Throw entity with physgun or gravitygun with PHYS_DYNAMIC
================
*/
void Phys_HoldDropDynamic(gentity_t *player, vec3_t velocity, qboolean isPhysgun){
	gentity_t 	*ent = player->grabbedEntity;

	if(!ent->client){
		ent->s.pos.trType = TR_GRAVITY;
		ent->s.pos.trTime = level.time;
		ent->sb_phys = PHYS_DYNAMIC;
		if(ent->s.eType == ET_ITEM){
			ent->spawnflags = 0;			//for items phys
		}
		if(isPhysgun){
			VectorSubtract(ent->r.currentOrigin, ent->grabOldOrigin, velocity);
			VectorScale(velocity, 15, velocity);
		} else {
			VectorSubtract(ent->r.currentOrigin, player->r.currentOrigin, velocity);
			VectorScale(velocity, 10, velocity);
		}
		VectorCopy(velocity, ent->s.pos.trDelta);
	} else {
		if(!isPhysgun){
			VectorSubtract(ent->r.currentOrigin, player->r.currentOrigin, velocity);
			VectorScale(velocity, 10, velocity);
			VectorCopy(velocity, ent->client->ps.velocity);
		}
		if(g_awardpushing.integer){
			ent->client->lastSentFlying = player->s.number;
			ent->client->lastSentFlyingTime = level.time;
		}
	}
}

/*
================
Phys_HoldSetup

Setup hold physics for entity with physgun or gravitygun
================
*/
void Phys_HoldSetup(gentity_t *player, qboolean isPhysgun){
	gentity_t 	*ent = player->grabbedEntity;

	ent->isGrabbed = qtrue;
	if(!ent->client){
	if(isPhysgun){
		ent->grabNewPhys = PHYS_DYNAMIC;
	}
	ent->s.pos.trType = TR_GRAVITY;
	ent->sb_phys = PHYS_DYNAMIC;
	Phys_Enable(ent);
	}
}

/*
================
Phys_HoldFrame

Physics frame for hold entity with physgun or gravitygun
================
*/
void Phys_HoldFrame(gentity_t *player, vec3_t velocity, qboolean isPhysgun){
	gentity_t 	*ent = player->grabbedEntity;
	vec3_t		end;

	trap_UnlinkEntity( ent );
	Phys_CheckWeldedEntities(ent);
	if(isPhysgun){
		CrosshairPointPhys(player, player->grabDist, end);
	} else {
		CrosshairPointGravity(player, GRAVITYGUN_DIST, end);
	}
	trap_LinkEntity( ent );
	Phys_RestoreWeldedEntities(ent);
	if(isPhysgun){
		VectorAdd(end, player->grabOffset, end);				//physgun offset
		VectorCopy(ent->r.currentOrigin, ent->grabOldOrigin);	//save old frame for speed apply
	}
	ent->lastPlayer = player; //save attacker
	if(!ent->client){
	VectorClear( ent->s.pos.trDelta );
	VectorCopy(end, ent->s.origin);
	VectorCopy(end, ent->s.pos.trBase);
	VectorCopy(end, ent->r.currentOrigin);
	Phys_Enable(ent);
	if(player->client->pers.cmd.buttons & BUTTON_GESTURE && isPhysgun){ //rotate
		if(!ent->r.bmodel){
			ent->s.apos.trBase[0] = player->client->pers.cmd.angles[0];
		}
		ent->s.apos.trBase[1] = player->client->pers.cmd.angles[1];
	}
	} else {
	VectorClear( ent->client->ps.velocity );
	VectorCopy( end, ent->client->ps.origin );
	if(isPhysgun){
		end[2] += 1;	//player not stuck
	}
	VectorCopy( end, ent->r.currentOrigin );
	if(isPhysgun){
		VectorSubtract(ent->r.currentOrigin, ent->grabOldOrigin, velocity);
		VectorScale(velocity, 15, velocity);
		VectorCopy(velocity, ent->client->ps.velocity);
	}
	}
}

/*
=============
Phys_RunPhysThink

Runs thinking code for phys frame
=============
*/
void Phys_RunPhysThink (gentity_t *ent) {
	float	thinktime;

	thinktime = ent->phys_nextthink;
	if (thinktime <= 0) {
		return;
	}
	if (thinktime > level.time) {
		return;
	}

	ent->phys_nextthink = 0;
	if (!ent->phys_think) {
		G_Error ( "NULL ent->phys_think");
	}
	ent->phys_think (ent);
}

/*
================
Phys_Disable

Disables physics
================
*/
void Phys_Disable( gentity_t *ent, vec3_t origin ) {
	VectorCopy( origin, ent->r.currentOrigin );	//physics origin
	VectorCopy( origin, ent->s.pos.trBase );	//client origin
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
	if(ent->sb_phys == PHYS_STATIC){	//if it's static object, not turn phys
		return;	
	}
	VectorCopy( ent->r.currentOrigin, ent->s.pos.trBase );		//restore client origin from physics origin
	if(ent->phys_inWater){
		ent->s.pos.trType = TR_GRAVITY_WATER;
	} else {
		ent->s.pos.trType = TR_GRAVITY;
	}
	ent->s.pos.trTime = level.time;
	ent->s.pos.trDuration = level.time;
	ent->phys_nextthink = 0;
	ent->phys_think = NULL;
}

/*
================
Phys_Unweld

Remove weld from physic object
================
*/
void Phys_Unweld( gentity_t *ent ) {
	gentity_t *object;
	int i;
	if(ent->phys_weldedObjectsNum){
		for (i = 0; i < MAX_GENTITIES; i++) {
			object = &g_entities[i];
			if (ent->s.number == object->phys_parent->s.number) {
				ent->s.pos.trType = TR_GRAVITY; ent->s.pos.trTime = level.time; ent->sb_phys = PHYS_DYNAMIC;
				Phys_Enable(ent);
				object->s.pos.trType = TR_GRAVITY; object->s.pos.trTime = level.time; object->sb_phys = PHYS_DYNAMIC;
				Phys_Enable(object);
				object->phys_parent = NULL;
				VectorClear(object->phys_relativeOrigin);
				VectorClear(object->phys_rv_0);
				VectorClear(object->phys_rv_1);
				VectorClear(object->phys_rv_2);
			}
		}
		return;
	}

	if(ent->phys_parent){
		ent->s.pos.trType = TR_GRAVITY; ent->s.pos.trTime = level.time; ent->sb_phys = PHYS_DYNAMIC;
		Phys_Enable(ent);
		ent->phys_parent = NULL;
		VectorClear(ent->phys_relativeOrigin);
		VectorClear(ent->phys_rv_0);
		VectorClear(ent->phys_rv_1);
		VectorClear(ent->phys_rv_2);
		return;
	}
}


/*
================
Phys_SnapToNearestAngle

Snaps physic object to nearest rotate angle
================
*/
float Phys_SnapToNearestAngle(float angle) {
	float angles[4] = {0.0f, 90.0f, 180.0f, 270.0f};
	float closest = angles[0];
	float minDist = fabs(angle - closest);
	int   i;

	for (i = 1; i < 4; i++) {
		float dist = fabs(angle - angles[i]);
		if (dist < minDist) {
			minDist = dist;
			closest = angles[i];
		}
	}
	return closest;
}

/*
================
Phys_SmoothReturnToNearestAngle

Return physic object to correct rotates
================
*/
void Phys_SmoothReturnToNearestAngle( gentity_t *ent ) {
    float speed = 0.50f;

    float velocity = VectorLength(ent->s.pos.trDelta);
    float mass = ent->s.origin2[O2_MASS];

    speed += (velocity * 1.00f) / mass;

    if (speed > 25.00) {
        speed = 25.00;
    }

    // Плавное движение для X и Z
    ent->s.apos.trBase[0] += (Phys_SnapToNearestAngle(ent->s.apos.trBase[0]) - ent->s.apos.trBase[0]) * speed;
    ent->s.apos.trBase[2] += (Phys_SnapToNearestAngle(ent->s.apos.trBase[2]) - ent->s.apos.trBase[2]) * speed;

    // Проверка, если значения достаточно близки к целям, чтобы остановиться
    if (fabs(ent->s.apos.trBase[0] - Phys_SnapToNearestAngle(ent->s.apos.trBase[0])) < 0.01f) ent->s.apos.trBase[0] = Phys_SnapToNearestAngle(ent->s.apos.trBase[0]);
    if (fabs(ent->s.apos.trBase[2] - Phys_SnapToNearestAngle(ent->s.apos.trBase[2])) < 0.01f) ent->s.apos.trBase[2] = Phys_SnapToNearestAngle(ent->s.apos.trBase[2]);

    // Если объект еще не достиг угла, продолжаем его двигать
    if (ent->s.apos.trBase[0] != Phys_SnapToNearestAngle(ent->s.apos.trBase[0]) || ent->s.apos.trBase[2] != Phys_SnapToNearestAngle(ent->s.apos.trBase[2])) {
        ent->phys_nextthink = level.time + 1;
        ent->phys_think = Phys_SmoothReturnToNearestAngle;
    }
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
void Phys_Bounce( gentity_t *ent, trace_t *tr ) {
	vec3_t	velocity;
	float	dot;
	int		hitTime;
    float 	randomOffset[3];
	int		i;
	gentity_t *hit;

	hit = &g_entities[tr->entityNum];

	// reflect the velocity on the trace plane
	hitTime = level.previousTime + ( level.time - level.previousTime ) * tr->fraction;
	ST_EvaluateTrajectoryDelta( &ent->s.pos, hitTime, velocity, ent->s.origin2[O2_MASS] );
	dot = DotProduct( velocity, tr->plane.normal );
	VectorMA( velocity, -2*dot, tr->plane.normal, ent->s.pos.trDelta );

	VectorScale( ent->s.pos.trDelta, ent->physicsBounce, ent->s.pos.trDelta );

	// check for stop
    if (tr->plane.normal[2] > 0.2 && VectorLength(ent->s.pos.trDelta) < 40 && !ent->isGrabbed) {  
		ent->phys_think = Phys_SmoothReturnToNearestAngle;
		ent->phys_nextthink = level.time + 2;
		Phys_Disable(ent, tr->endpos);
		return;
	}

	for (i = 0; i < 3; i++) {
		randomOffset[i] = ((float)rand() / 32767 - 0.5f) * VectorLength(ent->s.pos.trDelta)*0.60;
	}
	VectorAdd(ent->s.pos.trDelta, randomOffset, ent->s.pos.trDelta);

	if (hit->s.number != ent->s.number && hit->sandboxObject) {
		for (i = 0; i < 3; i++) {
    	    randomOffset[i] = ((float)rand() / 32767 - 0.5f) * VectorLength(hit->s.pos.trDelta)*1.20;
    	}
    	VectorAdd(ent->s.pos.trDelta, randomOffset, ent->s.pos.trDelta);
	}

	VectorAdd( ent->r.currentOrigin, tr->plane.normal, ent->r.currentOrigin);
	VectorCopy( ent->r.currentOrigin, ent->s.pos.trBase );	//update client origin from physics origin
	ent->s.pos.trTime = level.time;
}

/*
================
Phys_CorrectBounds

Correct bounds for physic object
================
*/
void Phys_CorrectBounds( gentity_t *ent, vec3_t outMins, vec3_t outMaxs ) {
	float	collmodifier = PHYS_COL;

	VectorCopy(ent->r.mins, outMins);
    VectorCopy(ent->r.maxs, outMaxs);
	if (ent->phys_inAir && ent->phys_inSolid){
		outMins[0] *= collmodifier;
		outMins[1] *= collmodifier;
		outMins[2] *= 1.00;
		outMaxs[0] *= collmodifier;
		outMaxs[1] *= collmodifier;
		outMaxs[2] *= collmodifier;
	}
}

/*
================
Phys_SelectPhysModel

Phys model for physic object
================
*/
void Phys_SelectPhysModel(gentity_t *ent) {

	float impactForceFixed;

	impactForceFixed = sqrt(ent->s.pos.trDelta[0] * ent->s.pos.trDelta[0] + ent->s.pos.trDelta[1] * ent->s.pos.trDelta[1] + g_gravity.integer*g_gravityModifier.value * g_gravity.integer*g_gravityModifier.value);

	impactForceFixed *= ent->s.origin2[O2_MASS];

	if(trap_PointContents(ent->r.currentOrigin, ent->s.number) & MASK_WATER){
		if(!ent->phys_inWater){
			ent->s.pos.trType = TR_GRAVITY_WATER;
			Phys_WaterSplash( ent, impactForceFixed);
			Phys_Enable(ent);
			ent->s.pos.trDelta[0] *= 0.50;
			ent->s.pos.trDelta[1] *= 0.50;
			if(ent->s.pos.trDelta[2]){
				ent->s.pos.trDelta[2] = 0;
			}
		}
		return;
	}
	
	if(ent->s.pos.trType != TR_STATIONARY){
		ent->s.pos.trType = TR_GRAVITY;		//default phys
	}
}

/*
================
Phys_UpdateState

Updates state for physic object and turn off physics
================
*/
qboolean Phys_UpdateState( gentity_t *ent ) {

	ent->s.apos.trBase[0] = AngleNormalize360(ent->s.apos.trBase[0]);
	ent->s.apos.trBase[1] = AngleNormalize360(ent->s.apos.trBase[1]);
	ent->s.apos.trBase[2] = AngleNormalize360(ent->s.apos.trBase[2]);

	if(ent->sb_phys == PHYS_STATIC){	//if it's static object, reset delta and other properties
		VectorClear(ent->s.pos.trDelta);
		ent->phys_inAir = qfalse;
		ent->phys_inSolid = qfalse;
	} else {
		ent->phys_inAir = Phys_Check(ent, 1.0f, PHYSCHECK_INAIR);
		ent->phys_inSolid = Phys_Check(ent, 0.0f, PHYSCHECK_SOLID);
	}

    // If the entity is stationary and not on air - disable physics
    if (ent->s.pos.trType == TR_STATIONARY) {
        if(!ent->phys_inAir){
			trap_LinkEntity(ent);
			Phys_RestoreWeldedEntities(ent);
        	G_RunThink(ent);
			Phys_RunPhysThink(ent);
        	return qfalse;
		} else {
			Phys_Enable(ent);
		}
	}
	return qtrue;
}

/*
================
Phys_Check

Check ground or solid for physic object
================
*/
qboolean Phys_Check(gentity_t *ent, float distance, int checkNum) {
    vec3_t start, end;
    vec3_t checkMins, checkMaxs;
    float collmodifier = 1.00;
    trace_t tr;

    VectorCopy(ent->r.currentOrigin, start);
    VectorCopy(start, end);

    if (checkNum == PHYSCHECK_INAIR) {
        end[2] -= distance;
		collmodifier = PHYS_COL_CHECK;
    }

    VectorCopy(ent->r.mins, checkMins);
    VectorCopy(ent->r.maxs, checkMaxs);
    checkMins[0] *= collmodifier;
    checkMins[1] *= collmodifier;
    checkMins[2] *= 1.00;
    checkMaxs[0] *= collmodifier;
    checkMaxs[1] *= collmodifier;
	if (checkNum == PHYSCHECK_INAIR) {
		checkMaxs[2] *= -1.00;
		checkMaxs[2] += 1.00;
	} else {
		checkMaxs[2] *= collmodifier;
	}

    trap_Trace(&tr, start, checkMins, checkMaxs, end, ent->s.number, MASK_PLAYERSOLID);

    if (checkNum != PHYSCHECK_SOLID) {
        return !(tr.fraction < 1.0f); // Для проверки земли возвращаем результат на основе fraction
    } else {
        return tr.startsolid; // Для проверки столкновения возвращаем startsolid
    }
}

/*
================
Phys_PointCheck

Check point for physic object
================
*/
void Phys_PointCheck(gentity_t *ent) {

	if(trap_PointContents(ent->r.currentOrigin, ent->s.number) & MASK_WATER){
		ent->phys_inWater = qtrue;
	} else {
		ent->phys_inWater = qfalse;
	}

}

/*
================
Phys_Rotate

Rotate for physic object
================
*/
void Phys_Rotate(gentity_t *ent, trace_t *tr) {

    if (ent->phys_think) {
        return;
    }

    if (!ent->isGrabbed && !tr->startsolid) {

        // Определяем множитель вращения в зависимости от типа движения
        float rotationMultiplier = (ent->s.pos.trType != TR_GRAVITY_WATER) ? PHYS_ROTATING : PHYS_ROTATING * 0.5;

        // Обработка изменения координат по осям
        if (ent->s.pos.trDelta[2] != 0) {
            ent->s.apos.trBase[0] -= ent->s.pos.trDelta[2] * rotationMultiplier * 0.20;
            ent->s.apos.trBase[1] -= ent->s.pos.trDelta[2] * rotationMultiplier * 0.20;
        }
        if (ent->s.pos.trDelta[1] != 0) {
            ent->s.apos.trBase[1] -= ent->s.pos.trDelta[1] * rotationMultiplier;
        }
        if (ent->s.pos.trDelta[0] != 0) {
            ent->s.apos.trBase[0] += ent->s.pos.trDelta[0] * rotationMultiplier;
        }
    }

    // Нормализуем углы, чтобы они всегда оставались в правильном диапазоне
    ent->s.apos.trBase[0] = AngleNormalize360(ent->s.apos.trBase[0]);
    ent->s.apos.trBase[1] = AngleNormalize360(ent->s.apos.trBase[1]);
    ent->s.apos.trBase[2] = AngleNormalize360(ent->s.apos.trBase[2]);
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

	if ( ent->phys_inSolid ) {
		return;
	}

	// Calculate the impact force
	impactForce = sqrt(ent->s.pos.trDelta[0] * ent->s.pos.trDelta[0] + ent->s.pos.trDelta[1] * ent->s.pos.trDelta[1]);
	impactForceAll = sqrt(ent->s.pos.trDelta[0] * ent->s.pos.trDelta[0] + ent->s.pos.trDelta[1] * ent->s.pos.trDelta[1] + ent->s.pos.trDelta[2] * ent->s.pos.trDelta[2]);
	
	impactForce *= ent->s.origin2[O2_MASS];
	impactForceAll *= ent->s.origin2[O2_MASS];

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
Phys_CheckWeldedEntities

Check and calculate welded objects to this object
================
*/
void Phys_CheckWeldedEntities(gentity_t *ent) {
    gentity_t *object;
    int i = 0;
    int j = 0;
    
    if (!ent->phys_weldedObjectsNum) {
        return;
    }

    for (i = 0; i < MAX_GENTITIES; i++) {
        object = &g_entities[i];
        if (ent == object->phys_parent) {
            trap_UnlinkEntity(object);
            j++;
        }
    }

    ent->phys_weldedObjectsNum = j;
}

/*
================
Phys_RestoreWeldedEntities

Restores welded objects to this object
================
*/
void Phys_RestoreWeldedEntities(gentity_t *ent) {
	gentity_t *object;
	int i = 0;

	if(!ent->phys_weldedObjectsNum){
		return;
	}

    for (i = 0; i < MAX_GENTITIES; i++) {
        object = &g_entities[i];
		if (ent == object->phys_parent) {
            vec3_t forward, right, up;
            vec3_t rotatedOffset, finalPos;
			vec3_t entForward, entRight, entUp;
			vec3_t relForward, relRight, relUp;
			vec3_t newForward, newRight, newUp;
			float matrix[3][3];
			vec3_t newAngles;

			//
			// Origin
			//

            AngleVectors(ent->s.apos.trBase, forward, right, up);
            rotatedOffset[0] = forward[0] * object->phys_relativeOrigin[0] + right[0] * object->phys_relativeOrigin[1] + up[0] * object->phys_relativeOrigin[2];
            rotatedOffset[1] = forward[1] * object->phys_relativeOrigin[0] + right[1] * object->phys_relativeOrigin[1] + up[1] * object->phys_relativeOrigin[2];
            rotatedOffset[2] = forward[2] * object->phys_relativeOrigin[0] + right[2] * object->phys_relativeOrigin[1] + up[2] * object->phys_relativeOrigin[2];

            VectorAdd(ent->r.currentOrigin, rotatedOffset, finalPos);
			
            VectorCopy(finalPos, object->s.origin);
            VectorCopy(finalPos, object->r.currentOrigin);
            VectorCopy(finalPos, object->s.pos.trBase);

			//
			// Angles
			//

			AngleVectors(ent->s.apos.trBase, entForward, entRight, entUp);

			VectorCopy(object->phys_rv_0, relForward);
			VectorCopy(object->phys_rv_1, relRight);
			VectorCopy(object->phys_rv_2, relUp);

			newForward[0] = entForward[0]*relForward[0] + entRight[0]*relForward[1] + entUp[0]*relForward[2];
			newForward[1] = entForward[1]*relForward[0] + entRight[1]*relForward[1] + entUp[1]*relForward[2];
			newForward[2] = entForward[2]*relForward[0] + entRight[2]*relForward[1] + entUp[2]*relForward[2];
			
			newRight[0] = entForward[0]*relRight[0] + entRight[0]*relRight[1] + entUp[0]*relRight[2];
			newRight[1] = entForward[1]*relRight[0] + entRight[1]*relRight[1] + entUp[1]*relRight[2];
			newRight[2] = entForward[2]*relRight[0] + entRight[2]*relRight[1] + entUp[2]*relRight[2];
			
			newUp[0] = entForward[0]*relUp[0] + entRight[0]*relUp[1] + entUp[0]*relUp[2];
			newUp[1] = entForward[1]*relUp[0] + entRight[1]*relUp[1] + entUp[1]*relUp[2];
			newUp[2] = entForward[2]*relUp[0] + entRight[2]*relUp[1] + entUp[2]*relUp[2];

			OrthogonalizeMatrix(newForward, newRight, newUp);

			matrix[0][0] = newForward[0]; // X forward
			matrix[0][1] = newRight[0];   // X right
			matrix[0][2] = newUp[0];      // X up

			matrix[1][0] = newForward[1]; // Y forward
			matrix[1][1] = newRight[1];   // Y right
			matrix[1][2] = newUp[1];      // Y up

			matrix[2][0] = newForward[2]; // Z forward
			matrix[2][1] = newRight[2];   // Z right
			matrix[2][2] = newUp[2];      // Z up

			AxisToAngles(matrix, newAngles);

			newAngles[2] -= 180;	//it's work well

			VectorCopy(newAngles, object->s.angles);
    		VectorCopy(newAngles, object->s.apos.trBase);

			//
			// Disable phys
			//

			object->s.pos.trType = TR_STATIONARY; object->sb_phys = PHYS_STATIC;
			Phys_Disable(object, object->r.currentOrigin);

			trap_LinkEntity(object);
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

	if(ent->phys_parent){
		return;
	}

    // Unlink the entity so that it won't interact with other entities during the calculation
    trap_UnlinkEntity(ent);
	Phys_CheckWeldedEntities(ent);

	// Update rotate
	VectorCopy(ent->s.apos.trBase, ent->s.angles);			//update server angles from client angles
	if (ent->s.pos.trType == TR_STATIONARY) {
		VectorCopy(ent->s.apos.trBase, ent->r.currentAngles);	//update physics angles from client angles
	}

	if(!Phys_UpdateState(ent)){		//disable physics and update state
		return;
	}

	//Check phys models
	Phys_SelectPhysModel(ent);

	//Check point state
	Phys_PointCheck(ent);
	
	// Get current position based on the entity's trajectory
	if(ent->s.eType == ET_GENERAL){ 		//is prop
    	ST_EvaluateTrajectory(&ent->s.pos, level.time, origin, ent->s.origin2[O2_MASS]);
	} else {
    	BG_EvaluateTrajectory(&ent->s.pos, level.time, origin);		
	}

    // Trace a line from the current origin to the new position
	Phys_CorrectBounds(ent, adjustedMins, adjustedMaxs);
    trap_Trace(&tr, ent->r.currentOrigin, adjustedMins, adjustedMaxs, origin, ent->s.number, MASK_PLAYERSOLID);

    // Save origin
    VectorCopy(tr.endpos, ent->s.origin);			//update server origin from trace endpos
	VectorCopy(tr.endpos, ent->r.currentOrigin);	//update physics origin from trace endpos
	
    // Link the entity back into the world
    trap_LinkEntity(ent);
	Phys_RestoreWeldedEntities(ent);

	//Check impacts
	Phys_Impact(ent, &tr);

	G_RunThink(ent);

	//Change angles
	Phys_Rotate(ent, &tr);

    // Check for solid start (possible embedded in another object)
    if (tr.startsolid) {
        tr.fraction = 0;
    }

	Phys_RunPhysThink(ent);
	
	if ( tr.fraction == 1 ) {
		return;
	}

	//Process bounce
	Phys_Bounce(ent, &tr);
}
