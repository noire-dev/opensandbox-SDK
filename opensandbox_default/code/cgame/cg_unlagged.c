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

// we'll need these prototypes
void CG_ShotgunPattern( vec3_t origin, vec3_t origin2, int seed, int otherEntNum );
void CG_Bullet( vec3_t end, int sourceEntityNum, vec3_t normal, qboolean flesh, int fleshEntityNum );

// and this as well
//Must be in sync with g_weapon.c
#define MACHINEGUN_SPREAD	200
#define CHAINGUN_SPREAD		600

/*
=======================
CG_PredictWeaponEffects

Draws predicted effects for the railgun, shotgun, and machinegun.  The
lightning gun is done in CG_LightningBolt, since it was just a matter
of setting the right origin and angles.
=======================
*/
void CG_PredictWeaponEffects( centity_t *cent ) {
	vec3_t		muzzlePoint, forward, right, up;
	entityState_t *ent = &cent->currentState;
	clientInfo_t	*ci;
	int				weaphack;

	// if the client isn't us, forget it
	if ( cent->currentState.number != cg.predictedPlayerState.clientNum ) {
		return;
	}
	
	ci = &cgs.clientinfo[ cent->currentState.clientNum ];
	
	if(ci->swepid >= 1){
	weaphack = ci->swepid;
	} else {
	weaphack = ent->weapon;
	}

	// get the muzzle point
	VectorCopy( cg.predictedPlayerState.origin, muzzlePoint );
	muzzlePoint[2] += cg.predictedPlayerState.viewheight;

	// get forward, right, and up
	AngleVectors( cg.predictedPlayerState.viewangles, forward, right, up );
	VectorMA( muzzlePoint, 14, forward, muzzlePoint );

	// was it a rail attack?
	if ( weaphack == WP_RAILGUN ) {
		// do we have it on for the rail gun?
		trace_t trace;
		vec3_t endPoint;

		// trace forward
		VectorMA( muzzlePoint, 8192, forward, endPoint );

		// find the rail's end point
		CG_Trace( &trace, muzzlePoint, vec3_origin, vec3_origin, endPoint, cg.predictedPlayerState.clientNum, CONTENTS_SOLID );

		// do the magic-number adjustment
		VectorMA( muzzlePoint, 4, right, muzzlePoint );
		VectorMA( muzzlePoint, -1, up, muzzlePoint );

        if(!cg.renderingThirdPerson) {
        	if(cg_drawGun.integer == 2)
				VectorMA(muzzlePoint, 8, cg.refdef.viewaxis[1], muzzlePoint);
        	else if(cg_drawGun.integer == 3)
				VectorMA(muzzlePoint, 4, cg.refdef.viewaxis[1], muzzlePoint);
        }

		// draw a rail trail
		CG_RailTrail( &cgs.clientinfo[cent->currentState.number], muzzlePoint, trace.endpos );

		// explosion at end if not SURF_NOIMPACT
		if ( !(trace.surfaceFlags & SURF_NOIMPACT) ) {
			// predict an explosion
			CG_MissileHitWall( weaphack, cg.predictedPlayerState.clientNum, trace.endpos, trace.plane.normal, IMPACTSOUND_DEFAULT );
		}
	}
	// was it a shotgun attack?
	else if ( weaphack == WP_SHOTGUN ) {
		int contents;
		vec3_t endPoint, v;
		vec3_t			up;

		// do everything like the server does
		SnapVector( muzzlePoint );

		VectorScale( forward, 4096, endPoint );
		SnapVector( endPoint );

		VectorSubtract( endPoint, muzzlePoint, v );
		VectorNormalize( v );
		VectorScale( v, 32, v );
		VectorAdd( muzzlePoint, v, v );

		contents = trap_CM_PointContents( muzzlePoint, 0 );
		if ( !( contents & CONTENTS_WATER ) ) {
			VectorSet( up, 0, 0, 8 );
			CG_SmokePuff( v, up, 32, 1, 1, 1, 0.33f, 900, cg.time, 0, LEF_PUFF_DONT_SCALE, cgs.media.shotgunSmokePuffShader );
		}

		// do the shotgun pellets
		CG_ShotgunPattern( muzzlePoint, endPoint, cg.oldTime % 256, cg.predictedPlayerState.clientNum );
	}
	// was it a machinegun attack?
	else if ( weaphack == WP_MACHINEGUN ) {
		int seed = cg.oldTime % 256;
		float r, u;
		trace_t tr;
		qboolean flesh;
		int fleshEntityNum = 0;
		vec3_t endPoint;

		// do everything exactly like the server does
		r = Q_random(&seed) * M_PI * 2.0f;
		u = sin(r) * Q_crandom(&seed) * mod_mgspread * 16;
		r = cos(r) * Q_crandom(&seed) * mod_mgspread * 16;

		VectorMA( muzzlePoint, 8192*16, forward, endPoint );
		VectorMA( endPoint, r, right, endPoint );
		VectorMA( endPoint, u, up, endPoint );

		CG_Trace(&tr, muzzlePoint, NULL, NULL, endPoint, cg.predictedPlayerState.clientNum, MASK_SHOT );

		if ( tr.surfaceFlags & SURF_NOIMPACT ) {
			return;
		}

		// snap the endpos to integers, but nudged towards the line
		SnapVectorTowards( tr.endpos, muzzlePoint );

		// do bullet impact
		if ( tr.entityNum < MAX_CLIENTS ) {
			flesh = qtrue;
			fleshEntityNum = tr.entityNum;
		} else {
			flesh = qfalse;
		}

		// do the bullet impact
		CG_Bullet( tr.endpos, cg.predictedPlayerState.clientNum, tr.plane.normal, flesh, fleshEntityNum );
	}
        // was it a chaingun attack?
	else if ( weaphack == WP_CHAINGUN ) {
		int seed = cg.oldTime % 256;
		float r, u;
		trace_t tr;
		qboolean flesh;
		int fleshEntityNum = 0;
		vec3_t endPoint;

		// do everything exactly like the server does
		r = Q_random(&seed) * M_PI * 2.0f;
		u = sin(r) * Q_crandom(&seed) * mod_cgspread * 16;
		r = cos(r) * Q_crandom(&seed) * mod_cgspread * 16;

		VectorMA( muzzlePoint, 8192*16, forward, endPoint );
		VectorMA( endPoint, r, right, endPoint );
		VectorMA( endPoint, u, up, endPoint );

		CG_Trace(&tr, muzzlePoint, NULL, NULL, endPoint, cg.predictedPlayerState.clientNum, MASK_SHOT );

		if ( tr.surfaceFlags & SURF_NOIMPACT ) {
			return;
		}

		// snap the endpos to integers, but nudged towards the line
		SnapVectorTowards( tr.endpos, muzzlePoint );

		// do bullet impact
		if ( tr.entityNum < MAX_CLIENTS ) {
			flesh = qtrue;
			fleshEntityNum = tr.entityNum;
		} else {
			flesh = qfalse;
		}

		// do the bullet impact
		CG_Bullet( tr.endpos, cg.predictedPlayerState.clientNum, tr.plane.normal, flesh, fleshEntityNum );
	}
}
