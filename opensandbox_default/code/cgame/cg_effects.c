// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

#include "cg_local.h"

void CG_BubbleTrail(vec3_t start, vec3_t end, float spacing) {
	vec3_t move;
	vec3_t vec;
	float len;
	int i;

	if(cg_noProjectileTrail.integer) return;

	VectorCopy(start, move);
	VectorSubtract(end, start, vec);
	len = VectorNormalize(vec);

	// advance a random amount first
	i = rand() % (int)spacing;
	VectorMA(move, i, vec, move);

	VectorScale(vec, spacing, vec);

	for(; i < len; i += spacing) {
		localEntity_t *le;
		refEntity_t *re;

		le = CG_AllocLocalEntity();
		le->leFlags = LEF_PUFF_DONT_SCALE;
		le->leType = LE_MOVE_SCALE_FADE;
		le->startTime = cg.time;
		le->endTime = cg.time + 1000 + random() * 250;
		le->lifeRate = 1.0 / (le->endTime - le->startTime);

		re = &le->refEntity;
		re->shaderTime = cg.time / 1000.0f;

		re->reType = RT_SPRITE;
		re->rotation = 0;
		re->radius = 3;
		re->customShader = cgs.media.waterBubbleShader;
		re->shaderRGBA[0] = 0xff;
		re->shaderRGBA[1] = 0xff;
		re->shaderRGBA[2] = 0xff;
		re->shaderRGBA[3] = 0xff;

		le->color[3] = 1.0;

		le->pos.trType = TR_LINEAR;
		le->pos.trTime = cg.time;
		VectorCopy(move, le->pos.trBase);
		le->pos.trDelta[0] = crandom() * 5;
		le->pos.trDelta[1] = crandom() * 5;
		le->pos.trDelta[2] = crandom() * 5 + 6;

		VectorAdd(move, vec, move);
	}
}

localEntity_t *CG_SmokePuff(const vec3_t p, const vec3_t vel, float radius, float r, float g, float b, float a, float duration, int startTime, int fadeInTime, int leFlags, qhandle_t hShader) {
	static int seed = 0x92;
	localEntity_t *le;
	refEntity_t *re;

	le = CG_AllocLocalEntity();
	le->leFlags = leFlags;
	le->radius = radius;

	re = &le->refEntity;
	re->rotation = Q_random(&seed) * 360;
	re->radius = radius;
	re->shaderTime = startTime / 1000.0f;

	le->leType = LE_MOVE_SCALE_FADE;
	le->startTime = startTime;
	le->fadeInTime = fadeInTime;
	le->endTime = startTime + duration;
	if(fadeInTime > startTime) {
		le->lifeRate = 1.0 / (le->endTime - le->fadeInTime);
	} else {
		le->lifeRate = 1.0 / (le->endTime - le->startTime);
	}
	le->color[0] = r;
	le->color[1] = g;
	le->color[2] = b;
	le->color[3] = a;

	le->pos.trType = TR_LINEAR_STOP;
	le->pos.trTime = startTime;
	VectorCopy(vel, le->pos.trDelta);
	VectorCopy(p, le->pos.trBase);

	VectorCopy(p, re->origin);
	re->customShader = hShader;

	re->shaderRGBA[0] = le->color[0] * 0xff;
	re->shaderRGBA[1] = le->color[1] * 0xff;
	re->shaderRGBA[2] = le->color[2] * 0xff;
	re->shaderRGBA[3] = 0xff;

	re->reType = RT_SPRITE;
	re->radius = le->radius;

	return le;
}

void CG_SpawnEffect(vec3_t org) {
	localEntity_t *le;
	refEntity_t *re;

	le = CG_AllocLocalEntity();
	le->leFlags = 0;
	le->leType = LE_FADE_RGB;
	le->startTime = cg.time;
	le->endTime = cg.time + 500;
	le->lifeRate = 1.0 / (le->endTime - le->startTime);

	le->color[0] = le->color[1] = le->color[2] = le->color[3] = 1.0;

	re = &le->refEntity;

	re->reType = RT_MODEL;
	re->shaderTime = cg.time / 1000.0f;
	re->hModel = cgs.media.teleportEffectModel;
	AxisClear(re->axis);

	VectorCopy(org, re->origin);
	re->origin[2] += 16;
}

void CG_LightningBoltBeam(vec3_t start, vec3_t end) {
	localEntity_t *le;
	refEntity_t *beam;

	le = CG_AllocLocalEntity();
	le->leFlags = 0;
	le->leType = LE_SHOWREFENTITY;
	le->startTime = cg.time;
	le->endTime = cg.time + 50;

	beam = &le->refEntity;

	VectorCopy(start, beam->origin);
	// this is the end point
	VectorCopy(end, beam->oldorigin);

	beam->reType = RT_LIGHTNING;
	beam->customShader = cgs.media.lightningShader;
}

void CG_KamikazeEffect(vec3_t org) {
	localEntity_t *le;
	refEntity_t *re;

	le = CG_AllocLocalEntity();
	le->leFlags = 0;
	le->leType = LE_KAMIKAZE;
	le->startTime = cg.time;
	le->endTime = cg.time + 3000;  // 2250;
	le->lifeRate = 1.0 / (le->endTime - le->startTime);
	le->color[0] = le->color[1] = le->color[2] = le->color[3] = 1.0;

	VectorClear(le->angles.trBase);

	re = &le->refEntity;
	re->reType = RT_MODEL;
	re->shaderTime = cg.time / 1000.0f;
	re->hModel = cgs.media.kamikazeEffectModel;

	VectorCopy(org, re->origin);
}

void CG_ObeliskExplode(vec3_t org, int entityNum) {
	localEntity_t *le;
	vec3_t origin;

	// create an explosion
	VectorCopy(org, origin);
	origin[2] += 64;
	le = CG_MakeExplosion(origin, vec3_origin, cgs.media.dishFlashModel, cgs.media.rocketExplosionShader, 600, qtrue);
	le->light = 300;
	le->lightColor[0] = 1;
	le->lightColor[1] = 0.75;
	le->lightColor[2] = 0.0;
}

void CG_ObeliskPain(vec3_t org) {
	float r;
	sfxHandle_t sfx;

	// hit sound
	r = rand() & 3;
	if(r < 2) {
		sfx = cgs.media.obeliskHitSound1;
	} else if(r == 2) {
		sfx = cgs.media.obeliskHitSound2;
	} else {
		sfx = cgs.media.obeliskHitSound3;
	}
	trap_S_StartSound(org, ENTITYNUM_NONE, CHAN_BODY, sfx);
}

void CG_InvulnerabilityImpact(vec3_t org, vec3_t angles) {
	localEntity_t *le;
	refEntity_t *re;
	int r;
	sfxHandle_t sfx;

	le = CG_AllocLocalEntity();
	le->leFlags = 0;
	le->leType = LE_INVULIMPACT;
	le->startTime = cg.time;
	le->endTime = cg.time + 1000;
	le->lifeRate = 1.0 / (le->endTime - le->startTime);

	le->color[0] = le->color[1] = le->color[2] = le->color[3] = 1.0;

	re = &le->refEntity;

	re->reType = RT_MODEL;
	re->shaderTime = cg.time / 1000.0f;

	re->hModel = cgs.media.invulnerabilityImpactModel;

	VectorCopy(org, re->origin);
	AnglesToAxis(angles, re->axis);

	r = rand() & 3;
	if(r < 2) {
		sfx = cgs.media.invulnerabilityImpactSound1;
	} else if(r == 2) {
		sfx = cgs.media.invulnerabilityImpactSound2;
	} else {
		sfx = cgs.media.invulnerabilityImpactSound3;
	}
	trap_S_StartSound(org, ENTITYNUM_NONE, CHAN_BODY, sfx);
}

void CG_InvulnerabilityJuiced(vec3_t org) {
	localEntity_t *le;
	refEntity_t *re;
	vec3_t angles;

	le = CG_AllocLocalEntity();
	le->leFlags = 0;
	le->leType = LE_INVULJUICED;
	le->startTime = cg.time;
	le->endTime = cg.time + 10000;
	le->lifeRate = 1.0 / (le->endTime - le->startTime);

	le->color[0] = le->color[1] = le->color[2] = le->color[3] = 1.0;

	re = &le->refEntity;

	re->reType = RT_MODEL;
	re->shaderTime = cg.time / 1000.0f;

	re->hModel = cgs.media.invulnerabilityJuicedModel;

	VectorCopy(org, re->origin);
	VectorClear(angles);
	AnglesToAxis(angles, re->axis);

	trap_S_StartSound(org, ENTITYNUM_NONE, CHAN_BODY, cgs.media.invulnerabilityJuicedSound);
}

localEntity_t *CG_MakeExplosion(vec3_t origin, vec3_t dir, qhandle_t hModel, qhandle_t shader, int msec, qboolean isSprite) {
	float ang;
	localEntity_t *ex;
	int offset;
	vec3_t tmpVec, newOrigin;

	if(msec <= 0) CG_Error("CG_MakeExplosion: msec = %i", msec);

	// skew the time a bit so they aren't all in sync
	offset = rand() & 63;

	ex = CG_AllocLocalEntity();
	if(isSprite) {
		ex->leType = LE_SPRITE_EXPLOSION;

		// randomly rotate sprite orientation
		ex->refEntity.rotation = rand() % 360;
		VectorScale(dir, 16, tmpVec);
		VectorAdd(tmpVec, origin, newOrigin);
	} else {
		ex->leType = LE_EXPLOSION;
		VectorCopy(origin, newOrigin);

		// set axis with random rotate
		if(!dir) {
			AxisClear(ex->refEntity.axis);
		} else {
			ang = rand() % 360;
			VectorCopy(dir, ex->refEntity.axis[0]);
			RotateAroundDirection(ex->refEntity.axis, ang);
		}
	}

	ex->startTime = cg.time - offset;
	ex->endTime = ex->startTime + msec;

	// bias the time so all shader effects start correctly
	ex->refEntity.shaderTime = ex->startTime / 1000.0f;
	ex->refEntity.hModel = hModel;
	ex->refEntity.customShader = shader;

	// set origin
	VectorCopy(newOrigin, ex->refEntity.origin);
	VectorCopy(newOrigin, ex->refEntity.oldorigin);

	ex->color[0] = ex->color[1] = ex->color[2] = 1.0;

	return ex;
}

void CG_Bleed(vec3_t origin, int entityNum) {
	localEntity_t *ex;

	if(!cg_blood.integer) return;

	ex = CG_AllocLocalEntity();
	ex->leType = LE_EXPLOSION;

	ex->startTime = cg.time;
	ex->endTime = ex->startTime + 500;

	VectorCopy(origin, ex->refEntity.origin);
	ex->refEntity.reType = RT_SPRITE;
	ex->refEntity.rotation = rand() % 360;
	ex->refEntity.radius = 24;

	ex->refEntity.customShader = cgs.media.bloodExplosionShader;

	// don't show player's own blood in view
	if(entityNum == cg.snap->ps.clientNum) {
		ex->refEntity.renderfx |= RF_THIRD_PERSON;
	}
}

static void CG_LaunchGib(vec3_t origin, vec3_t velocity, qhandle_t hModel) {
	localEntity_t *le;
	refEntity_t *re;

	le = CG_AllocLocalEntity();
	re = &le->refEntity;

	le->leType = LE_FRAGMENT2;
	le->startTime = cg.time;
	le->endTime = le->startTime + cg_effectsTime.integer * 1000;
	VectorCopy(origin, re->origin);
	AxisCopy(axisDefault, re->axis);
	re->hModel = hModel;

	le->pos.trType = TR_GRAVITY;
	VectorCopy(origin, le->pos.trBase);
	VectorCopy(velocity, le->pos.trDelta);
	le->pos.trTime = cg.time;

	le->bounceFactor = 0.6f;

	le->leBounceSoundType = LEBS_BLOOD;
	le->leMarkType = LEMT_BLOOD;
}

#define GIB_VELOCITY 500
#define GIB_JUMP 350
void CG_GibPlayer(vec3_t playerOrigin) {
	vec3_t origin, velocity;
	int j;

	if(!cg_blood.integer) return;

	VectorCopy(playerOrigin, origin);
	velocity[0] = crandom() * GIB_VELOCITY;
	velocity[1] = crandom() * GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom() * GIB_VELOCITY;
	if(rand() & 1) {
		CG_LaunchGib(origin, velocity, cgs.media.gibSkull);
	} else {
		CG_LaunchGib(origin, velocity, cgs.media.gibBrain);
	}

	VectorCopy(playerOrigin, origin);
	velocity[0] = crandom() * GIB_VELOCITY;
	velocity[1] = crandom() * GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom() * GIB_VELOCITY;
	CG_LaunchGib(origin, velocity, cgs.media.gibAbdomen);

	VectorCopy(playerOrigin, origin);
	velocity[0] = crandom() * GIB_VELOCITY;
	velocity[1] = crandom() * GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom() * GIB_VELOCITY;
	CG_LaunchGib(origin, velocity, cgs.media.gibArm);

	VectorCopy(playerOrigin, origin);
	velocity[0] = crandom() * GIB_VELOCITY;
	velocity[1] = crandom() * GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom() * GIB_VELOCITY;
	CG_LaunchGib(origin, velocity, cgs.media.gibChest);

	VectorCopy(playerOrigin, origin);
	velocity[0] = crandom() * GIB_VELOCITY;
	velocity[1] = crandom() * GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom() * GIB_VELOCITY;
	CG_LaunchGib(origin, velocity, cgs.media.gibFist);

	VectorCopy(playerOrigin, origin);
	velocity[0] = crandom() * GIB_VELOCITY;
	velocity[1] = crandom() * GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom() * GIB_VELOCITY;
	CG_LaunchGib(origin, velocity, cgs.media.gibFoot);

	VectorCopy(playerOrigin, origin);
	velocity[0] = crandom() * GIB_VELOCITY;
	velocity[1] = crandom() * GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom() * GIB_VELOCITY;
	CG_LaunchGib(origin, velocity, cgs.media.gibForearm);

	VectorCopy(playerOrigin, origin);
	velocity[0] = crandom() * GIB_VELOCITY;
	velocity[1] = crandom() * GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom() * GIB_VELOCITY;
	CG_LaunchGib(origin, velocity, cgs.media.gibIntestine);

	VectorCopy(playerOrigin, origin);
	velocity[0] = crandom() * GIB_VELOCITY;
	velocity[1] = crandom() * GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom() * GIB_VELOCITY;
	CG_LaunchGib(origin, velocity, cgs.media.gibLeg);

	VectorCopy(playerOrigin, origin);
	velocity[0] = crandom() * GIB_VELOCITY;
	velocity[1] = crandom() * GIB_VELOCITY;
	velocity[2] = GIB_JUMP + crandom() * GIB_VELOCITY;
	CG_LaunchGib(origin, velocity, cgs.media.gibLeg);

	for(j = 1; j <= cg_effectsGibs.integer; j++) {
		VectorCopy(playerOrigin, origin);
		velocity[0] = crandom() * GIB_VELOCITY;
		velocity[1] = crandom() * GIB_VELOCITY;
		velocity[2] = GIB_JUMP + crandom() * GIB_VELOCITY;
		CG_LaunchGib(origin, velocity, cgs.media.gibChest);
	}
}

static void CG_Particles(vec3_t origin, int count, int speed, int lifetime, int radius, int type, byte r, byte g, byte b) {
	int jump;          // amount to nudge the particles trajectory vector up by
	qhandle_t shader;  // shader to use for the particles
	int index;
	vec3_t randVec, tempVec;
	qboolean moveUp;

	jump = speed;
	shader = cgs.media.sparkShader;

	for(index = 0; index < count; index++) {
		localEntity_t *le;
		refEntity_t *re;

		le = CG_AllocLocalEntity();  // allocate a local entity
		re = &le->refEntity;
		le->leFlags = LEF_PUFF_DONT_SCALE;                               // don't change the particle size
		le->leType = LE_MOVE_SCALE_FADE;                                 // particle should fade over time
		le->startTime = cg.time;                                         // set the start time of the particle to the current time
		le->endTime = cg.time + lifetime + (random() * (lifetime / 2));  // life time will be anywhere between [lifetime] and [lifetime * 1.5]
		le->lifeRate = 1.0 / (le->endTime - le->startTime);
		re = &le->refEntity;
		re->shaderTime = cg.time / 1000.0f;
		re->reType = RT_SPRITE;
		re->rotation = 0;
		re->radius = radius;
		re->customShader = shader;
		re->shaderRGBA[0] = r;
		re->shaderRGBA[1] = g;
		re->shaderRGBA[2] = b;
		re->shaderRGBA[3] = 0xFF;
		le->color[3] = 1.0;
		if(type == PT_GRAVITY)
			le->pos.trType = TR_GRAVITY;  // moves in a gravity affected arc
		else
			le->pos.trType = TR_LINEAR;  // moves in straight line, outward from the origin
		le->pos.trTime = cg.time;
		VectorCopy(origin, le->pos.trBase);
		VectorCopy(origin, re->origin);

		tempVec[0] = crandom();  // between 1 and -1
		tempVec[1] = crandom();
		tempVec[2] = crandom();
		VectorNormalize(tempVec);
		VectorScale(tempVec, speed, randVec);

		if(type == PT_GRAVITY || type == PT_LINEAR_UP)
			moveUp = qtrue;
		else if(type == PT_LINEAR_DOWN)
			moveUp = qfalse;
		else if(crandom() < 0)
			moveUp = qtrue;
		else
			moveUp = qfalse;

		if(moveUp)
			randVec[2] += jump;  // nudge the particles up a bit
		else
			randVec[2] -= jump;  // nudge the particles down a bit

		VectorCopy(randVec, le->pos.trDelta);
	}
}

void CG_ParticlesFromEntityState(vec3_t origin, int type, entityState_t *es) {
	byte r, g, b;
	int lifetime = 3000;
	int radius = 3;
	int speed = es->generic1;

	r = es->constantLight & 255;
	g = (es->constantLight >> 8) & 255;
	b = (es->constantLight >> 16) & 255;
	radius = es->generic2;

	CG_Particles(origin, es->eventParm, speed, lifetime, radius, type, r, g, b);
}
