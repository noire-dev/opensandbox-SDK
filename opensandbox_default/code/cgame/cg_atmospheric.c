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

#include "cg_local.h"

#define MAX_ATMOSPHERIC_PARTICLES		2048  	// maximum # of particles
#define MAX_ATMOSPHERIC_DISTANCE		3000  	// maximum distance from refdef origin that particles are visible

#define MAX_ATMOSPHERIC_HEIGHT			524288  	// maximum world height (FIXME: since 1.27 this should be 65536)
#define MIN_ATMOSPHERIC_HEIGHT			-524288  	// minimum world height (FIXME: since 1.27 this should be -65536)

#define START_SNOW_HEIGHT				256

#define MAX_ATMOSPHERIC_EFFECTSHADERS	6  	  	// maximum different effectshaders for an atmospheric effect

#define ATMOSPHERIC_DROPDELAY			1000
#define ATMOSPHERIC_CUTHEIGHT			800


#define ATMOSPHERIC_RAIN_SPEED			1.1f * mod_gravity
#define ATMOSPHERIC_RAIN_HEIGHT			150

#define ATMOSPHERIC_SNOW_SPEED			0.1f * mod_gravity
#define ATMOSPHERIC_SNOW_HEIGHT			10

#define NUM_ATMOSPHERIC_TYPES			2

typedef struct cg_atmosphericParticle_s {
	vec3_t pos, delta, deltaNormalized, colour, surfacenormal;
	float height, minz, weight;
	qboolean active;
	int contents, surface, nextDropTime;
	qhandle_t *effectshader;
} cg_atmosphericParticle_t;

typedef struct cg_atmosphericEffect_s {
	cg_atmosphericParticle_t particles[MAX_ATMOSPHERIC_PARTICLES];
	qhandle_t effectshaders[MAX_ATMOSPHERIC_EFFECTSHADERS];
	qhandle_t effectwatershader, effectlandshader;
	int lastRainTime, numDrops;
	int gustStartTime, gustEndTime;
	int baseStartTime, baseEndTime;
	int gustMinTime, gustMaxTime;
	int changeMinTime, changeMaxTime;
	int baseMinTime, baseMaxTime;
	float baseWeight, gustWeight;
	int baseDrops, gustDrops;
	int numEffectShaders;
	qboolean waterSplash, landSplash;
	vec3_t baseVec, gustVec;

	qboolean (*ParticleCheckVisible)( int type, cg_atmosphericParticle_t *particle );
	qboolean (*ParticleGenerate)( int type, cg_atmosphericParticle_t *particle, vec3_t currvec, float currweight );
	void (*ParticleRender)( int type, cg_atmosphericParticle_t *particle );
} cg_atmosphericEffect_t;

static cg_atmosphericEffect_t cg_atmFxList[NUM_ATMOSPHERIC_TYPES];

/*
**  Render utility functions
*/

void CG_EffectMark( qhandle_t markShader, const vec3_t origin, const vec3_t dir, float alpha, float radius ) {
	vec3_t			axis[3];
	float			texCoordScale;
	vec3_t			originalPoints[4];
	byte			colors[4];
	int				i;
	polyVert_t		*v;
	polyVert_t		verts[4];

	if ( !cg_addMarks.integer ) {
		return;
	}

	if ( radius <= 0 ) {
		CG_Error( "CG_EffectMark called with <= 0 radius" );
	}

	// create the texture axis
	VectorNormalize2( dir, axis[0] );
	PerpendicularVector( axis[1], axis[0] );
	VectorSet( axis[2], 1, 0, 0 );  	  	  	// This is _wrong_, but the function is for water anyway (i.e. usually flat)
	CrossProduct( axis[0], axis[2], axis[1] );

	texCoordScale = 0.5 * 1.0 / radius;

	// create the full polygon
	for ( i = 0 ; i < 3 ; i++ ) {
		originalPoints[0][i] = origin[i] - radius * axis[1][i] - radius * axis[2][i];
		originalPoints[1][i] = origin[i] + radius * axis[1][i] - radius * axis[2][i];
		originalPoints[2][i] = origin[i] + radius * axis[1][i] + radius * axis[2][i];
		originalPoints[3][i] = origin[i] - radius * axis[1][i] + radius * axis[2][i];
	}

	colors[0] = 127;
	colors[1] = 127;
	colors[2] = 127;
	colors[3] = alpha * 255;

	for ( i = 0, v = verts ; i < 4 ; i++, v++ ) {
		vec3_t  	  	delta;

		VectorCopy( originalPoints[i], v->xyz );

		VectorSubtract( v->xyz, origin, delta );
		v->st[0] = 0.5 + DotProduct( delta, axis[1] ) * texCoordScale;
		v->st[1] = 0.5 + DotProduct( delta, axis[2] ) * texCoordScale;
		*(int *)v->modulate = *(int *)colors;
	}

	trap_R_AddPolyToScene( markShader, 4, verts );
}

/*
**  	Raindrop management functions
*/

static qboolean CG_RainParticleCheckVisible( int type, cg_atmosphericParticle_t *particle ) {
	float		moved;
	vec3_t		distance;
	float			angle, dist, viewyaw, yaw, r;
	entityState_t	*s1;
	vec3_t			mins, maxs;
	int				i;
	cg_atmosphericEffect_t	*cg_atmFx;

	cg_atmFx = &cg_atmFxList[type];

	if( !particle || !particle->active )
		return( qfalse );

	moved = (cg.time - cg_atmFx->lastRainTime) * 0.001;  	// Units moved since last frame
	VectorMA( particle->pos, moved, particle->delta, particle->pos );
	if( particle->pos[2] + ATMOSPHERIC_CUTHEIGHT < particle->minz )
		return( particle->active = qfalse );

	VectorSubtract( particle->pos, cg.refdef.vieworg, distance );

	viewyaw = cg.refdefViewAngles[YAW];
	yaw = VectorToYaw(distance);

	if( sqrt( distance[0] * distance[0] + distance[1] * distance[1] ) > MAX_ATMOSPHERIC_DISTANCE
		|| fabs(AngleDifference(viewyaw, yaw)) > (cg.refdef.fov_x / 2.0f + 5)
	){

		angle = (viewyaw + crandom() * cg.refdef.fov_x / 2.0f) * M_PI / 180.0f;
		r = random();
		dist = 20 + (MAX_ATMOSPHERIC_DISTANCE-20) * (r * r); // ^2: more dense closer

		particle->pos[0] = cg.refdef.vieworg[0] + cos(angle) * dist;
		particle->pos[1] = cg.refdef.vieworg[1] + sin(angle) * dist;
	}

	if ( CG_PointContents( particle->pos, ENTITYNUM_NONE ) == CONTENTS_SOLID ){
		return( qfalse );
	}

	for (i = 0; i < MAX_GENTITIES; i++){
		s1 = &cg_entities[i].currentState;

		if ( s1->eType != ET_WEATHER ) continue; // entity is not a weather entity
		if ( s1->weapon != type ) continue; // entity is not the right type of weather entity

		if ( s1->solid == SOLID_BMODEL )
			trap_R_ModelBounds(cgs.inlineDrawModel[s1->modelindex], mins, maxs);
		else
			trap_R_ModelBounds(cgs.gameModels[s1->modelindex], mins, maxs);

		if ( CG_InsideBox( mins, maxs, particle->pos ) )
			return qtrue;
	}

	return( qfalse );
}

static qboolean CG_RainParticleGenerate( int type, cg_atmosphericParticle_t *particle, vec3_t currvec, float currweight ) {
	float		angle, distance, origz;
	vec3_t		testpoint, testend;
	trace_t		tr;

	float			r;
	entityState_t	*s1;
	vec3_t			mins, maxs;
	int				i;
	cg_atmosphericEffect_t	*cg_atmFx;
	qboolean		visible;

	cg_atmFx = &cg_atmFxList[type];

	angle = (cg.refdefViewAngles[YAW] + crandom() * cg.refdef.fov_x / 2.0f) * M_PI / 180.0f;
	r = random();
	distance = 20 + MAX_ATMOSPHERIC_DISTANCE * (r * r); // ^2: more dense closer

	testpoint[0] = testend[0] = cg.refdef.vieworg[0] + cos(angle) * distance;
	testpoint[1] = testend[1] = cg.refdef.vieworg[1] + sin(angle) * distance;

  	testpoint[2] = origz = cg.refdef.vieworg[2];
	testend[2] = testpoint[2] + MAX_ATMOSPHERIC_HEIGHT;

	visible = qfalse;
	for (i = 0; i < MAX_GENTITIES; i++){
		s1 = &cg_entities[i].currentState;

		if ( s1->eType != ET_WEATHER ) continue; // entity is not a weather entity
		if ( s1->weapon != type ) continue; // entity is not the right type of weather entity

		if ( s1->solid == SOLID_BMODEL )
			trap_R_ModelBounds(cgs.inlineDrawModel[s1->modelindex], mins, maxs);
		else
			trap_R_ModelBounds(cgs.gameModels[s1->modelindex], mins, maxs);

		if ( CG_InsideBox( mins, maxs, testpoint ) )
			visible = qtrue;
	}

	if (!visible)
		return qfalse;

	while( 1 )
	{
		if( testpoint[2] >= MAX_ATMOSPHERIC_HEIGHT )
			return( qfalse );
		if( testend[2] >= MAX_ATMOSPHERIC_HEIGHT )
			testend[2] = MAX_ATMOSPHERIC_HEIGHT - 1;
		CG_Trace( &tr, testpoint, NULL, NULL, testend, ENTITYNUM_NONE, MASK_SOLID|MASK_WATER );
		if( tr.startsolid )  	  	  	// Stuck in something, skip over it.
		{
			testpoint[2] += 64;
			testend[2] = testpoint[2] + MAX_ATMOSPHERIC_HEIGHT;
		}
		else if( tr.fraction == 1 )  	  	// Didn't hit anything, we're (probably) outside the world
			return( qfalse );
		else if( tr.surfaceFlags & SURF_SKY )  	// Hit sky, this is where we start.
			break;
		else return( qfalse );
	}

//	if drawing snow start drawing it a little lower because it falls too slowly
	VectorCopy(tr.endpos, testpoint);
	testpoint[2] -= 10;
	testend[2] = origz + (tr.fraction * MAX_ATMOSPHERIC_HEIGHT * (random() * 0.8f + 0.2f)) - 10;

	CG_Trace( &tr, testpoint, NULL, NULL, testend, ENTITYNUM_NONE, MASK_SOLID|MASK_WATER );
	if ( tr.fraction != 1 ){
		return qfalse;
	}

	particle->active = qtrue;
	particle->colour[0] = 0.6 + 0.2 * random();
	particle->colour[1] = 0.6 + 0.2 * random();
  	particle->colour[2] = 0.6 + 0.2 * random();
	VectorCopy( tr.endpos, particle->pos );
	VectorCopy( currvec, particle->delta );
	particle->delta[2] += crandom() * 100;
	VectorNormalize2( particle->delta, particle->deltaNormalized );
	particle->height = ATMOSPHERIC_RAIN_HEIGHT + crandom() * 100;
	particle->weight = currweight;
	particle->effectshader = &cg_atmFx->effectshaders[0];

	distance =  	((float)(tr.endpos[2] - MIN_ATMOSPHERIC_HEIGHT)) / -particle->delta[2];
	VectorMA( tr.endpos, distance, particle->delta, testend );

	CG_Trace( &tr, particle->pos, NULL, NULL, testend, ENTITYNUM_NONE, MASK_SOLID|MASK_WATER );
	particle->minz = tr.endpos[2];
	tr.endpos[2]--;
	VectorCopy( tr.plane.normal, particle->surfacenormal );
	particle->surface = tr.surfaceFlags;
	particle->contents = CG_PointContents( tr.endpos, ENTITYNUM_NONE );

	return( qtrue );
}

static void CG_RainParticleRender( int type, cg_atmosphericParticle_t *particle ) {
	vec3_t			forward, right;
	polyVert_t		verts[4];
	vec2_t			line;
	float			len, frac;
	vec3_t			start, finish;
	cg_atmosphericEffect_t	*cg_atmFx;

	cg_atmFx = &cg_atmFxList[type];

	if( !particle->active )
		return;

	VectorCopy( particle->pos, start );
	len = particle->height;
	if( start[2] <= particle->minz )
	{
		// Stop rain going through surfaces.
		len = particle->height - particle->minz + start[2];
		VectorMA( start, len - particle->height, particle->deltaNormalized, start );

		if( cg_atmosphericLevel.integer == 2 ){
			frac = (ATMOSPHERIC_CUTHEIGHT - particle->minz + start[2]) / (float) ATMOSPHERIC_CUTHEIGHT;
			// Splash effects on different surfaces
			if( particle->contents & (CONTENTS_WATER|CONTENTS_SLIME) )
			{
				// Water splash
				if( cg_atmFx->effectwatershader && frac > 0 && frac < 1 )
					CG_EffectMark( cg_atmFx->effectwatershader, start, particle->surfacenormal, frac * 0.5, 8 - frac * 8 );
			}
			else if( !(particle->contents & CONTENTS_LAVA) && !(particle->surface & (SURF_NODAMAGE|SURF_NOIMPACT|SURF_NOMARKS|SURF_SKY)) )
			{
				// Solid splash
				if( cg_atmFx->effectlandshader && frac > 0 && frac < 1  )
					CG_ImpactMark( cg_atmFx->effectlandshader, start, particle->surfacenormal, 0, 1, 1, 1, frac * 0.5, qfalse, 3 - frac * 2, qtrue );
			}
		}
	}
	if( len <= 0 )
		return;

	VectorCopy( particle->deltaNormalized, forward );
	VectorMA( start, -len, forward, finish );

	line[0] = DotProduct( forward, cg.refdef.viewaxis[1] );
	line[1] = DotProduct( forward, cg.refdef.viewaxis[2] );

	VectorScale( cg.refdef.viewaxis[1], line[1], right );
	VectorMA( right, -line[0], cg.refdef.viewaxis[2], right );
	VectorNormalize( right );

	VectorMA( finish, particle->weight, right, verts[0].xyz );
	verts[0].st[0] = 1;
	verts[0].st[1] = 0;
	verts[0].modulate[0] = 255;
	verts[0].modulate[1] = 255;
	verts[0].modulate[2] = 255;
	verts[0].modulate[3] = 0;

	VectorMA( finish, -particle->weight, right, verts[1].xyz );
	verts[1].st[0] = 0;
	verts[1].st[1] = 0;
	verts[1].modulate[0] = 255;
	verts[1].modulate[1] = 255;
	verts[1].modulate[2] = 255;
	verts[1].modulate[3] = 0;

	VectorMA( start, -particle->weight, right, verts[2].xyz );
	verts[2].st[0] = 0;
	verts[2].st[1] = 1;
	verts[2].modulate[0] = 255;
	verts[2].modulate[1] = 255;
	verts[2].modulate[2] = 255;
	verts[2].modulate[3] = 127;

	VectorMA( start, particle->weight, right, verts[3].xyz );
	verts[3].st[0] = 1;
	verts[3].st[1] = 1;
	verts[3].modulate[0] = 255;
	verts[3].modulate[1] = 255;
	verts[3].modulate[2] = 255;
	verts[3].modulate[3] = 127;

	trap_R_AddPolyToScene( *particle->effectshader, 4, verts );
}

/*
**  	Snow management functions
*/

static qboolean CG_SnowParticleGenerate( int type, cg_atmosphericParticle_t *particle, vec3_t currvec, float currweight ) {
	float		angle, distance, origz;
	vec3_t		testpoint, testend;
	trace_t		tr;
	float			r;
	entityState_t	*s1;
	vec3_t			mins, maxs;
	int				i;
	cg_atmosphericEffect_t	*cg_atmFx;
	qboolean		visible;

	cg_atmFx = &cg_atmFxList[type];

	angle = (cg.refdefViewAngles[YAW] + crandom() * cg.refdef.fov_x / 2.0f) * M_PI / 180.0f;
	r = random();
	distance = 20 + MAX_ATMOSPHERIC_DISTANCE * (r * r); // ^2: more dense closer
	

	testpoint[0] = testend[0] = cg.refdef.vieworg[0] + cos(angle) * distance;
	testpoint[1] = testend[1] = cg.refdef.vieworg[1] + sin(angle) * distance;
	testpoint[2] = origz = cg.refdef.vieworg[2];
	testend[2] = testpoint[2] + MAX_ATMOSPHERIC_HEIGHT;

	visible = qfalse;
	for (i = 0; i < MAX_GENTITIES; i++){
		s1 = &cg_entities[i].currentState;

		if ( s1->eType != ET_WEATHER ) continue; // entity is not a weather entity
		if ( s1->weapon != type ) continue; // entity is not the right type of weather entity

		if ( s1->solid == SOLID_BMODEL )
			trap_R_ModelBounds(cgs.inlineDrawModel[s1->modelindex], mins, maxs);
		else
			trap_R_ModelBounds(cgs.gameModels[s1->modelindex], mins, maxs);

		if ( CG_InsideBox( mins, maxs, testpoint ) )
			visible = qtrue;
	}

	if (!visible)
		return qfalse;

	while( 1 )
	{
		if( testpoint[2] >= MAX_ATMOSPHERIC_HEIGHT )
			return( qfalse );
		if( testend[2] >= MAX_ATMOSPHERIC_HEIGHT )
			testend[2] = MAX_ATMOSPHERIC_HEIGHT - 1;

		CG_Trace( &tr, testpoint, NULL, NULL, testend, ENTITYNUM_NONE, MASK_SOLID|MASK_WATER );
		if( tr.startsolid )  	  	  	// Stuck in something, skip over it.
		{
			testpoint[2] += 64;
			testend[2] = testpoint[2] + MAX_ATMOSPHERIC_HEIGHT;
		}
		else if( tr.fraction == 1 )  	  	// Didn't hit anything, we're (probably) outside the world
			return( qfalse );
		else if( tr.surfaceFlags & SURF_SKY )  	// Hit sky, this is where we start.
			break;
		else return( qfalse );
	}

	if (tr.endpos[2] - origz > START_SNOW_HEIGHT){
		VectorCopy(tr.endpos, testpoint);
		testpoint[2] -= 10;
		testend[2] = origz + (START_SNOW_HEIGHT * (random() * 0.8f + 0.2f));

		CG_Trace( &tr, testpoint, NULL, NULL, testend, ENTITYNUM_NONE, MASK_SOLID|MASK_WATER );
		if ( tr.fraction != 1 ){
			return qfalse;
		}
	}

	particle->active = qtrue;
	particle->colour[0] = 0.6 + 0.2 * random();
	particle->colour[1] = 0.6 + 0.2 * random();
	particle->colour[2] = 0.6 + 0.2 * random();
	VectorCopy( tr.endpos, particle->pos );
	VectorCopy( currvec, particle->delta );
	particle->delta[2] += crandom() * 25;
	VectorNormalize2( particle->delta, particle->deltaNormalized );
	particle->height = ATMOSPHERIC_SNOW_HEIGHT + crandom() * 8;
	particle->weight = particle->height * 0.5f;
	particle->effectshader = &cg_atmFx->effectshaders[ (int) (random() * ( cg_atmFx->numEffectShaders - 1 )) ];

	distance =  	((float)(tr.endpos[2] - MIN_ATMOSPHERIC_HEIGHT)) / -particle->delta[2];
	VectorMA( tr.endpos, distance, particle->delta, testend );
	CG_Trace( &tr, particle->pos, NULL, NULL, testend, ENTITYNUM_NONE, MASK_SOLID|MASK_WATER );
	particle->minz = tr.endpos[2];
	tr.endpos[2]--;
	VectorCopy( tr.plane.normal, particle->surfacenormal );
	particle->surface = tr.surfaceFlags;
	particle->contents = CG_PointContents( tr.endpos, ENTITYNUM_NONE );

	return( qtrue );
}

static void CG_SnowParticleRender( int type, cg_atmosphericParticle_t *particle ) {
	vec3_t			forward, right;
	polyVert_t		verts[4];
	vec2_t			line;
	float			len, sinTumbling, cosTumbling, particleWidth;
	vec3_t			start, finish;

	if( !particle->active )
		return;

	VectorCopy( particle->pos, start );

	sinTumbling = sin( particle->pos[2] * 0.03125f );
	cosTumbling = cos( ( particle->pos[2] + particle->pos[1] )  * 0.03125f );

	start[0] += 24 * ( 1 - particle->deltaNormalized[2] ) * sinTumbling;
	start[1] += 24 * ( 1 - particle->deltaNormalized[2] ) * cosTumbling;

	len = particle->height;
	if( start[2] <= particle->minz )
	{
		// Stop snow going through surfaces.
		len = particle->height - particle->minz + start[2];
		VectorMA( start, len - particle->height, particle->deltaNormalized, start );
	}
	if( len <= 0 )
		return;

	VectorCopy( particle->deltaNormalized, forward );
	VectorMA( start, -( len * sinTumbling ), forward, finish );

	line[0] = DotProduct( forward, cg.refdef.viewaxis[1] );
	line[1] = DotProduct( forward, cg.refdef.viewaxis[2] );

	VectorScale( cg.refdef.viewaxis[1], line[1], right );
	VectorMA( right, -line[0], cg.refdef.viewaxis[2], right );
	VectorNormalize( right );

	particleWidth = cosTumbling * particle->weight;

	VectorMA( finish, particleWidth, right, verts[0].xyz );
	verts[0].st[0] = 1;
	verts[0].st[1] = 0;
	verts[0].modulate[0] = 255;
	verts[0].modulate[1] = 255;
	verts[0].modulate[2] = 255;
	verts[0].modulate[3] = 255;

	VectorMA( finish, -particleWidth, right, verts[1].xyz );
	verts[1].st[0] = 0;
	verts[1].st[1] = 0;
	verts[1].modulate[0] = 255;
	verts[1].modulate[1] = 255;
	verts[1].modulate[2] = 255;
	verts[1].modulate[3] = 255;

	VectorMA( start, -particleWidth, right, verts[2].xyz );
	verts[2].st[0] = 0;
	verts[2].st[1] = 1;
	verts[2].modulate[0] = 255;
	verts[2].modulate[1] = 255;
	verts[2].modulate[2] = 255;
	verts[2].modulate[3] = 255;

	VectorMA( start, particleWidth, right, verts[3].xyz );
	verts[3].st[0] = 1;
	verts[3].st[1] = 1;
	verts[3].modulate[0] = 255;
	verts[3].modulate[1] = 255;
	verts[3].modulate[2] = 255;
	verts[3].modulate[3] = 255;

	trap_R_AddPolyToScene( *particle->effectshader, 4, verts );
}

/*
**  	Set up gust parameters.
*/

static void CG_EffectGust( cg_atmosphericEffect_t *cg_atmFx ) {
	int diff;

	cg_atmFx->baseEndTime		= cg.time					+ cg_atmFx->baseMinTime		+ (rand() % (cg_atmFx->baseMaxTime - cg_atmFx->baseMinTime));
	diff						= cg_atmFx->changeMaxTime	- cg_atmFx->changeMinTime;
	cg_atmFx->gustStartTime		= cg_atmFx->baseEndTime		+ cg_atmFx->changeMinTime	+ (diff ? (rand() % diff) : 0);
	diff						= cg_atmFx->gustMaxTime		- cg_atmFx->gustMinTime;
	cg_atmFx->gustEndTime		= cg_atmFx->gustStartTime	+ cg_atmFx->gustMinTime		+ (diff ? (rand() % diff) : 0);
	diff						= cg_atmFx->changeMaxTime	- cg_atmFx->changeMinTime;
	cg_atmFx->baseStartTime		= cg_atmFx->gustEndTime		+ cg_atmFx->changeMinTime	+ (diff ? (rand() % diff) : 0);
}

static qboolean CG_EffectGustCurrent( cg_atmosphericEffect_t *cg_atmFx, vec3_t curr, float *weight, int *num ) {
	vec3_t		temp;
	float		frac;

	if( cg.time < cg_atmFx->baseEndTime )
	{
		VectorCopy( cg_atmFx->baseVec, curr );
		*weight = cg_atmFx->baseWeight;
		*num = cg_atmFx->baseDrops;
	}
	else {
		VectorSubtract( cg_atmFx->gustVec, cg_atmFx->baseVec, temp );
		if( cg.time < cg_atmFx->gustStartTime )
		{
			frac = ((float)(cg.time - cg_atmFx->baseEndTime))/((float)(cg_atmFx->gustStartTime - cg_atmFx->baseEndTime));
			VectorMA( cg_atmFx->baseVec, frac, temp, curr );
			*weight = cg_atmFx->baseWeight + (cg_atmFx->gustWeight - cg_atmFx->baseWeight) * frac;
			*num = cg_atmFx->baseDrops + ((float)(cg_atmFx->gustDrops - cg_atmFx->baseDrops)) * frac;
		}
		else if( cg.time < cg_atmFx->gustEndTime )
		{
			VectorCopy( cg_atmFx->gustVec, curr );
			*weight = cg_atmFx->gustWeight;
			*num = cg_atmFx->gustDrops;
		}
		else
		{
			frac = 1.0 - ((float)(cg.time - cg_atmFx->gustEndTime))/((float)(cg_atmFx->baseStartTime - cg_atmFx->gustEndTime));
			VectorMA( cg_atmFx->baseVec, frac, temp, curr );
			*weight = cg_atmFx->baseWeight + (cg_atmFx->gustWeight - cg_atmFx->baseWeight) * frac;
			*num = cg_atmFx->baseDrops + ((float)(cg_atmFx->gustDrops - cg_atmFx->baseDrops)) * frac;
			if( cg.time >= cg_atmFx->baseStartTime )
				return( qtrue );
		}
	}
	return( qfalse );
}

void CG_Atmospheric_SetParticles( int type, int numParticles, qboolean diableSplashes ){
	int		count;
	cg_atmosphericEffect_t	*cg_atmFx;

	cg_atmFx = &cg_atmFxList[type];

	if ( cg_atmFx->numDrops ) return;

	// Set up some default values
	cg_atmFx->baseVec[0] = cg_atmFx->baseVec[1] = 0;
	cg_atmFx->gustVec[0] = 0;
	cg_atmFx->gustVec[1] = 100;
	cg_atmFx->baseMinTime = 5000;
	cg_atmFx->baseMaxTime = 10000;
	cg_atmFx->changeMinTime = 500;
	cg_atmFx->changeMaxTime = 500;
	cg_atmFx->gustMinTime = 500;
	cg_atmFx->gustMaxTime = 2000;
	cg_atmFx->baseDrops = numParticles;
	cg_atmFx->gustDrops = numParticles;
	cg_atmFx->baseWeight = 1.0f;
	cg_atmFx->gustWeight = 2.0f;
	cg_atmFx->waterSplash = !diableSplashes;
	cg_atmFx->landSplash = !diableSplashes;

	cg_atmFx->numDrops = (cg_atmFx->baseDrops > cg_atmFx->gustDrops) ? cg_atmFx->baseDrops : cg_atmFx->gustDrops;
	if( cg_atmFx->numDrops > MAX_ATMOSPHERIC_PARTICLES )
		cg_atmFx->numDrops = MAX_ATMOSPHERIC_PARTICLES;

	// Rain
	if( type == 0 ) {
		cg_atmFx->ParticleCheckVisible = &CG_RainParticleCheckVisible;
		cg_atmFx->ParticleGenerate = &CG_RainParticleGenerate;
		cg_atmFx->ParticleRender = &CG_RainParticleRender;
		cg_atmFx->baseVec[2] = cg_atmFx->gustVec[2] = - ATMOSPHERIC_RAIN_SPEED;

		cg_atmFx->numEffectShaders = 1;
		if( !(cg_atmFx->effectshaders[0] = trap_R_RegisterShader( "gfx/atmosphere/raindrop" )) )
			cg_atmFx->effectshaders[0] = -1;
		if( cg_atmFx->waterSplash )
			cg_atmFx->effectwatershader = trap_R_RegisterShader( "gfx/atmosphere/raindropwater" );
		if( cg_atmFx->landSplash )
			cg_atmFx->effectlandshader = trap_R_RegisterShader( "gfx/atmosphere/raindropsolid" );
	}
	// Snow
	else {
		cg_atmFx->ParticleCheckVisible = &CG_RainParticleCheckVisible;
		cg_atmFx->ParticleGenerate = &CG_SnowParticleGenerate;
		cg_atmFx->ParticleRender = &CG_SnowParticleRender;
		cg_atmFx->baseVec[2] = cg_atmFx->gustVec[2] = - ATMOSPHERIC_SNOW_SPEED;

		for( cg_atmFx->numEffectShaders = 0; cg_atmFx->numEffectShaders < 6; cg_atmFx->numEffectShaders++ ) {
			if( !( cg_atmFx->effectshaders[cg_atmFx->numEffectShaders] = trap_R_RegisterShader( va("gfx/atmosphere/snowflake0%i", cg_atmFx->numEffectShaders ) ) ) )
				cg_atmFx->effectshaders[cg_atmFx->numEffectShaders] = -1;  	// we had some kind of a problem
		}
		cg_atmFx->waterSplash = 0;
		cg_atmFx->landSplash = 0;
	}

	for( count = 0; count < cg_atmFx->numDrops; count++ ){
		if (cg_atmFx->particles[count].active) continue;
		cg_atmFx->particles[count].nextDropTime = ATMOSPHERIC_DROPDELAY + (rand() % ATMOSPHERIC_DROPDELAY);
	}

	CG_EffectGust( cg_atmFx );
}

/*
** Main render loop
*/

void CG_AddAtmosphericEffects() {
	int curr, max, currnum;
	cg_atmosphericParticle_t *particle;
	vec3_t currvec;
	float currweight;
	cg_atmosphericEffect_t	*cg_atmFx;
	int		i;

	if ( !cg_atmosphericLevel.integer )
		return;

	for (i = 0; i < NUM_ATMOSPHERIC_TYPES; i++){
		cg_atmFx = &cg_atmFxList[i];
		if( cg_atmFx->numDrops <= 0 || cg_atmFx->numEffectShaders == 0 ) continue;

		max = (cg_atmosphericLevel.integer == 1) ? (cg_atmFx->numDrops >> 1) : cg_atmFx->numDrops;
		if( CG_EffectGustCurrent( cg_atmFx, currvec, &currweight, &currnum ) )
			CG_EffectGust( cg_atmFx );  	  	  	// Recalculate gust parameters
		for( curr = 0; curr < max; curr++ )
		{
			particle = &cg_atmFx->particles[curr];
			if( !cg_atmFx->ParticleCheckVisible( i, particle ) )
			{
				// Effect has terminated / fallen from screen view

				if( !particle->nextDropTime )
				{
					// Stop rain being synchronized 
					particle->nextDropTime = rand() % ATMOSPHERIC_DROPDELAY;
				}
				else if( currnum < curr || particle->nextDropTime > cg.time )
					continue;
				if( !cg_atmFx->ParticleGenerate( i, particle, currvec, currweight ) )
				{
					// Ensure it doesn't attempt to generate every frame, to prevent
					// 'clumping' when there's only a small sky area available.
					particle->nextDropTime = cg.time + ATMOSPHERIC_DROPDELAY;
					continue;
				}
			}

			cg_atmFx->ParticleRender( i, particle );
		}

		cg_atmFx->lastRainTime = cg.time;
	}
}
