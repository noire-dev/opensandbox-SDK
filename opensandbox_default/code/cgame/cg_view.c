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
// cg_view.c -- setup all the parameters (position, angle, etc)
// for a 3D rendering
#include "cg_local.h"

/*
=================
CG_CalcVrect

Sets the coordinates of the rendered window
=================
*/
static void CG_CalcVrect (void) {
	cg.refdef.width = cgs.glconfig.vidWidth;
	cg.refdef.width &= ~1;

	cg.refdef.height = cgs.glconfig.vidHeight;
	cg.refdef.height &= ~1;

	cg.refdef.x = (cgs.glconfig.vidWidth - cg.refdef.width)/2;
	cg.refdef.y = (cgs.glconfig.vidHeight - cg.refdef.height)/2;
}

// leilei - eyes hack

extern vec3_t headpos;
extern vec3_t headang;

/*
===============
CG_ViewFog

===============
*/
static void CG_ViewFog( void ) {
	int i;
	float scale;
	
	if(!mod_fogColorA){
		return;
	}

	cg.viewfog[0].hModel = trap_R_RegisterModel( va("models/fog%i", mod_fogModel) );
	cg.viewfog[0].customShader = trap_R_RegisterShader(va("models/fogtex%i", mod_fogShader));

	for(i = 0; i < 16; i++){
		cg.viewfog[i].hModel = cg.viewfog[0].hModel;
		cg.viewfog[i].customShader = cg.viewfog[0].customShader;

		VectorCopy(cg.refdef.vieworg, cg.viewfog[i].origin);
		VectorCopy(cg.refdef.viewaxis[0], cg.viewfog[i].axis[0]);
		VectorCopy(cg.refdef.viewaxis[1], cg.viewfog[i].axis[1]);
		VectorCopy(cg.refdef.viewaxis[2], cg.viewfog[i].axis[2]);

		cg.viewfog[i].shaderRGBA[0] = mod_fogColorR;
		cg.viewfog[i].shaderRGBA[1] = mod_fogColorG;
		cg.viewfog[i].shaderRGBA[2] = mod_fogColorB;
		cg.viewfog[i].shaderRGBA[3] = (mod_fogColorA / 16) + (i * (mod_fogColorA - (mod_fogColorA / 16)) / 15);

		scale = ((mod_fogDistance*512) * 0.50) + i * (mod_fogInterval * 0.50);
		VectorScale(cg.viewfog[i].axis[0], scale, cg.viewfog[i].axis[0]);
		VectorScale(cg.viewfog[i].axis[1], scale, cg.viewfog[i].axis[1]);
		VectorScale(cg.viewfog[i].axis[2], scale, cg.viewfog[i].axis[2]);

		trap_R_AddRefEntityToScene(&cg.viewfog[i]);
	}
}

static void CG_ViewSky( void ) {
	int i;
	float scale;
	
	if(!mod_skyColorA){
		return;
	}

	cg.viewsky.hModel = trap_R_RegisterModel( "models/fog1" );
	cg.viewsky.customShader = trap_R_RegisterShader(va("models/skytex%i", mod_skyShader));

	VectorCopy(cg.refdef.vieworg, cg.viewsky.origin);
	VectorCopy(cg.refdef.viewaxis[0], cg.viewsky.axis[0]);
	VectorCopy(cg.refdef.viewaxis[1], cg.viewsky.axis[1]);
	VectorCopy(cg.refdef.viewaxis[2], cg.viewsky.axis[2]);

	cg.viewsky.shaderRGBA[0] = mod_skyColorR;
	cg.viewsky.shaderRGBA[1] = mod_skyColorG;
	cg.viewsky.shaderRGBA[2] = mod_skyColorB;
	cg.viewsky.shaderRGBA[3] = mod_skyColorA;

	scale = 4;
	VectorScale(cg.viewsky.axis[0], scale, cg.viewsky.axis[0]);
	VectorScale(cg.viewsky.axis[1], scale, cg.viewsky.axis[1]);
	VectorScale(cg.viewsky.axis[2], scale, cg.viewsky.axis[2]);
	
	cg.viewsky.renderfx = RF_FIRST_PERSON;

	trap_R_AddRefEntityToScene(&cg.viewsky);
}


/*
===============
CG_OffsetThirdPersonView

===============
*/
#define	FOCUS_DISTANCE	512
static void CG_OffsetThirdPersonView( void ) {
	vec3_t		forward, right, up;
	vec3_t		view;
	vec3_t		focusAngles;
	trace_t		trace;
	static vec3_t	mins = { -4, -4, -4 };
	static vec3_t	maxs = { 4, 4, 4 };
	vec3_t		focusPoint;
	float		focusDist;
	float		forwardScale, sideScale;

	cg.refdef.vieworg[2] += cg.predictedPlayerState.viewheight;

	VectorCopy( cg.refdefViewAngles, focusAngles );

	// if dead, look at killer
	if ( (cg.predictedPlayerState.stats[STAT_HEALTH] <= 0) && (cg.snap->ps.pm_type != PM_SPECTATOR) && (cgs.gametype !=GT_ELIMINATION && cgs.gametype !=GT_CTF_ELIMINATION && cgs.gametype !=GT_LMS) ) {
		focusAngles[YAW] = cg.predictedPlayerState.stats[STAT_DEAD_YAW];
		cg.refdefViewAngles[YAW] = cg.predictedPlayerState.stats[STAT_DEAD_YAW];
	}

	if ( focusAngles[PITCH] > 45 ) {
		focusAngles[PITCH] = 45;		// don't go too far overhead
	}
	AngleVectors( focusAngles, forward, right, NULL );

	VectorMA( cg.refdef.vieworg, FOCUS_DISTANCE, forward, focusPoint );

	VectorCopy( cg.refdef.vieworg, view );

	view[2] += 8;
	if(!BG_VehicleCheckClass(cg.snap->ps.stats[STAT_VEHICLE])){
	VectorMA( view, cg_thirdPersonOffset.value, right, view );
	} else {
	VectorMA( view, 0, right, view );		
	}

	cg.refdefViewAngles[PITCH] *= 0.5;

	AngleVectors( cg.refdefViewAngles, forward, right, up );

	forwardScale = cos( 0 / 180 * M_PI );
	sideScale = sin( 0 / 180 * M_PI );
	if(!BG_VehicleCheckClass(cg.snap->ps.stats[STAT_VEHICLE])){
		VectorMA( view, -cg_thirdPersonRange.value * forwardScale, forward, view );
		VectorMA( view, -cg_thirdPersonRange.value * sideScale, right, view );
	} else {
		VectorMA( view, -180 * forwardScale, forward, view );
		VectorMA( view, -180 * sideScale, right, view );		
	}

	// trace a ray from the origin to the viewpoint to make sure the view isn't
	// in a solid block.  Use an 8 by 8 block to prevent the view from near clipping anything
	CG_Trace( &trace, cg.refdef.vieworg, mins, maxs, view, cg.predictedPlayerState.clientNum, MASK_SOLID );
	if ( trace.fraction != 1.0 ) {
		VectorCopy( trace.endpos, view );
		view[2] += (1.0 - trace.fraction) * 32;
		// try another trace to this position, because a tunnel may have the ceiling
		// close enogh that this is poking out
		CG_Trace( &trace, cg.refdef.vieworg, mins, maxs, view, cg.predictedPlayerState.clientNum, MASK_SOLID );
		VectorCopy( trace.endpos, view );
	}

	VectorCopy( view, cg.refdef.vieworg );

	// select pitch to look at focus point from vieword
	VectorSubtract( focusPoint, cg.refdef.vieworg, focusPoint );
	focusDist = sqrt( focusPoint[0] * focusPoint[0] + focusPoint[1] * focusPoint[1] );
	if ( focusDist < 1 ) {
		focusDist = 1;	// should never happen
	}
	cg.refdefViewAngles[PITCH] = -180 / M_PI * atan2( focusPoint[2], focusDist );
	cg.refdefViewAngles[YAW] -= 0;
}


// this causes a compiler bug on mac MrC compiler
static void CG_StepOffset( void ) {
	int		timeDelta;
	
	// smooth out stair climbing
	timeDelta = cg.time - cg.stepTime;
	if ( timeDelta < STEP_TIME ) {
		cg.refdef.vieworg[2] -= cg.stepChange 
			* (STEP_TIME - timeDelta) / STEP_TIME;
	}
}

/*
===============
CG_OffsetFirstPersonView

===============
*/
static void CG_OffsetFirstPersonView( void ) {
	float			*origin;
	float			*angles;
	float			bob;
	float			ratio;
	float			delta;
	float			speed;
	float			f;
	vec3_t			predictedVelocity;
	int				timeDelta;
	
	if ( cg.snap->ps.pm_type == PM_INTERMISSION || cg.snap->ps.pm_type == PM_CUTSCENE ) {
		return;
	}

	origin = cg.refdef.vieworg;
	angles = cg.refdefViewAngles;

	// if dead, fix the angle and don't add any kick
	if ( cg.snap->ps.stats[STAT_HEALTH] <= 0 ) {
		angles[ROLL] = 40;
		angles[PITCH] = -15;
		angles[YAW] = cg.snap->ps.stats[STAT_DEAD_YAW];
		origin[2] += cg.predictedPlayerState.viewheight;
		return;
	}

	// add angles based on weapon kick
	VectorAdd (angles, cg.kick_angles, angles);

	// add angles based on velocity
	VectorCopy( cg.predictedPlayerState.velocity, predictedVelocity );

	if(cg_disableBobbing.integer){
		angles[PITCH] += DotProduct ( predictedVelocity, cg.refdef.viewaxis[0]);
		angles[ROLL] -= DotProduct ( predictedVelocity, cg.refdef.viewaxis[1]);
	} else {
		delta = DotProduct ( predictedVelocity, cg.refdef.viewaxis[0]);
		angles[PITCH] += delta * 0.002;
		delta = DotProduct ( predictedVelocity, cg.refdef.viewaxis[1]);
		angles[ROLL] -= delta * 0.005;

		// make sure the bob is visible even at low speeds
		speed = cg.xyspeed > 200 ? cg.xyspeed : 200;
		delta = cg.bobfracsin * 0.002 * speed;
		if (cg.predictedPlayerState.pm_flags & PMF_DUCKED)
			delta *= 3;		// crouching
		angles[PITCH] += delta;
		delta = cg.bobfracsin * 0.002 * speed;
		if (cg.predictedPlayerState.pm_flags & PMF_DUCKED)
			delta *= 3;		// crouching accentuates roll
		if (cg.bobcycle & 1)
			delta = -delta;
		angles[ROLL] += delta;
	}

	// add view height
	origin[2] += cg.predictedPlayerState.viewheight;

	// smooth out duck height changes
	timeDelta = cg.time - cg.duckTime;
	if ( timeDelta < DUCK_TIME) {
		cg.refdef.vieworg[2] -= cg.duckChange 
			* (DUCK_TIME - timeDelta) / DUCK_TIME;
	}

	// add bob height
	if(cg_disableBobbing.integer){
		bob = 0.0f;
	} else {
		bob = cg.bobfracsin * cg.xyspeed * 0.002;
		if (bob > 6) {
			bob = 6;
		}
	}

	origin[2] += bob;


	// add fall height
	delta = cg.time - cg.landTime;
	if ( delta < LAND_DEFLECT_TIME ) {
		f = delta / LAND_DEFLECT_TIME;
		cg.refdef.vieworg[2] += cg.landChange * f;
	} else if ( delta < LAND_DEFLECT_TIME + LAND_RETURN_TIME ) {
		delta -= LAND_DEFLECT_TIME;
		f = 1.0 - ( delta / LAND_RETURN_TIME );
		cg.refdef.vieworg[2] += cg.landChange * f;
	}

	// add step offset
	CG_StepOffset();

	// add kick offset
	VectorAdd (origin, cg.kick_origin, origin);
}

void CG_ZoomDown_f( void ) {
	if (cg.scoreBoardShowing){
		cg.teamoverlay = qtrue;
	} else {
		if ( cg.zoomed ) {
			return;
		}
		if ( cg.snap->ps.generic2 != WP_PHYSGUN ){
		cg.zoomed = qtrue;
		cg.zoomTime = cg.time;
		} else {
		trap_SendConsoleCommand("altfire_physgun\n");
		}
	}
}

void CG_ZoomUp_f( void ) { 
	if (cg.scoreBoardShowing){
		cg.teamoverlay = qfalse;
	} else {
		if ( !cg.zoomed ) {
			return;
		}
		cg.zoomed = qfalse;
		cg.zoomTime = cg.time;
	}
}


/*
====================
CG_CalcFov

Fixed fov at intermissions, otherwise account for fov variable and zooms.
====================
*/
#define	WAVE_AMPLITUDE	1
#define	WAVE_FREQUENCY	0.4

static void CG_CalcFov( void ) {
	float	x;
	float	phase;
	float	v;
	int		contents;
	float	fov_x, fov_y;
	float	zoomFov;
	float	f;

	if ( cg.predictedPlayerState.pm_type == PM_INTERMISSION ) {
		// if in intermission, use a fixed value
		fov_x = 90;
	} else {
		fov_x = cg_fov.value;
		if ( fov_x < 1 ) {
			fov_x = 1;
		} else if ( fov_x > 160 ) {
			fov_x = 160;
		}

        // account for zooms
        zoomFov = cg_zoomFov.value;
        if ( zoomFov < 1 ) {
                zoomFov = 1;
        } else if ( zoomFov > 160 ) {
                zoomFov = 160;
        }

		if ( cg.zoomed ) {
			f = ( cg.time - cg.zoomTime ) / (float)ZOOM_TIME;
			if ( f > 1.0 ) {
				fov_x = zoomFov;
			} else {
				fov_x = fov_x + f * ( zoomFov - fov_x );
			}
		} else {
			f = ( cg.time - cg.zoomTime ) / (float)ZOOM_TIME;
			if ( f > 1.0 ) {
				fov_x = fov_x;
			} else {
				fov_x = zoomFov + f * ( fov_x - zoomFov );
			}
		}
	}

	x = cg.refdef.width / tan( fov_x / 360 * M_PI );
	fov_y = atan2( cg.refdef.height, x );
	fov_y = fov_y * 360 / M_PI;

	// warp if underwater
	contents = CG_PointContents( cg.refdef.vieworg, -1 );
	if ( contents & ( CONTENTS_WATER | CONTENTS_SLIME | CONTENTS_LAVA ) ){
		phase = cg.time / 1000.0 * WAVE_FREQUENCY * M_PI * 2;
		v = WAVE_AMPLITUDE * sin( phase );
		fov_x += v;
		fov_y -= v;
	}

	// set it
	cg.refdef.fov_x = fov_x;
	cg.refdef.fov_y = fov_y;

	if ( !cg.zoomed ) {
		cg.zoomSensitivity = 1;
	} else {
		cg.zoomSensitivity = cg.refdef.fov_y / 75.0;
	}

	return;
}



/*
===============
CG_DamageBlendBlob

===============
*/
static void CG_DamageBlendBlob( void ) {
	int			t;
	int			maxTime;
	refEntity_t		ent;

	if ( !cg.damageValue ) {
		return;
	}

	maxTime = DAMAGE_TIME;
	t = cg.time - cg.damageTime;
	if ( t <= 0 || t >= maxTime ) {
		return;
	}


	memset( &ent, 0, sizeof( ent ) );
	ent.reType = RT_SPRITE;
	ent.renderfx = RF_FIRST_PERSON;

	VectorMA( cg.refdef.vieworg, 8, cg.refdef.viewaxis[0], ent.origin );
	VectorMA( ent.origin, cg.damageX * -8, cg.refdef.viewaxis[1], ent.origin );
	VectorMA( ent.origin, cg.damageY * 8, cg.refdef.viewaxis[2], ent.origin );

	ent.radius = cg.damageValue * 3;
	ent.customShader = cgs.media.viewBloodShader;
	ent.shaderRGBA[0] = 255;
	ent.shaderRGBA[1] = 255;
	ent.shaderRGBA[2] = 255;
	ent.shaderRGBA[3] = 200 * ( 1.0 - ((float)t / maxTime) );
	trap_R_AddRefEntityToScene( &ent );
}

static void CG_CalcCutsceneFov(int startFov, int endFov, float progress) {
	int diff;
	float fov_x, fov_y;
	int x, contents;
	float phase, v;

	//calculate new FOV
	diff = endFov - startFov;
	fov_x = startFov + (diff * progress);
	
	x = cg.refdef.width / tan( fov_x / 360 * M_PI );
	fov_y = atan2( cg.refdef.height, x );
	fov_y = fov_y * 360 / M_PI;

	// warp if underwater
	contents = CG_PointContents( cg.refdef.vieworg, -1 );
	if ( contents & ( CONTENTS_WATER | CONTENTS_SLIME | CONTENTS_LAVA ) ){
		phase = cg.time / 1000.0 * WAVE_FREQUENCY * M_PI * 2;
		v = WAVE_AMPLITUDE * sin( phase );
		fov_x += v;
		fov_y -= v;
	}

	// set it
	cg.refdef.fov_x = fov_x;
	cg.refdef.fov_y = fov_y;

	return;
}

static void CG_CalcCutsceneViewValues( ) {
	const char *cutsceneData;
	char buf[MAX_INFO_STRING];
	float wait;
	int start_time;
	vec3_t destOrigin, destAngles;
	vec3_t newOrigin, newAngles;
	int timePassed;
	float progress;
	float diff;
	int motion;
	int newFov, destFov;

	cutsceneData = CG_ConfigString( CS_CUTSCENE );

	motion = atoi(Info_ValueForKey(cutsceneData, "m"));
	start_time = atoi(Info_ValueForKey(cutsceneData, "t"));
	wait = atof(Info_ValueForKey(cutsceneData, "w"));
	newOrigin[0] = atof(Info_ValueForKey(cutsceneData, "o10"));
	newOrigin[1] = atof(Info_ValueForKey(cutsceneData, "o11"));
	newOrigin[2] = atof(Info_ValueForKey(cutsceneData, "o12"));
	newAngles[0] = atof(Info_ValueForKey(cutsceneData, "a10"));
	newAngles[1] = atof(Info_ValueForKey(cutsceneData, "a11"));
	newAngles[2] = atof(Info_ValueForKey(cutsceneData, "a12"));
	newFov = atoi(Info_ValueForKey(cutsceneData, "f1"));

	if ( motion & 1 ) {
		destOrigin[0] = atof(Info_ValueForKey(cutsceneData, "o20"));
		destOrigin[1] = atof(Info_ValueForKey(cutsceneData, "o21"));
		destOrigin[2] = atof(Info_ValueForKey(cutsceneData, "o22"));
		destAngles[0] = atof(Info_ValueForKey(cutsceneData, "a20"));
		destAngles[1] = atof(Info_ValueForKey(cutsceneData, "a21"));
		destAngles[2] = atof(Info_ValueForKey(cutsceneData, "a22"));
		destFov = atoi(Info_ValueForKey(cutsceneData, "f2"));

		//determine how long the current camera pan has taken
		timePassed = cg.time - start_time;
		progress = timePassed / (wait * 1000);

		//calculate new origin
		diff = destOrigin[0] - newOrigin[0];
		newOrigin[0] += diff * progress;

		diff = destOrigin[1] - newOrigin[1];
		newOrigin[1] += diff * progress;
		
		diff = destOrigin[2] - newOrigin[2];
		newOrigin[2] += diff * progress;

		//calculate new angles
		diff = destAngles[0] - newAngles[0];
		if ( diff > 180 ) {
			diff = 0 - (360 - diff);
		} else if ( diff < -180 ) {
			diff = 0 - (-360 - diff);
		}
		newAngles[0] += diff * progress;
		
		diff = destAngles[1] - newAngles[1];
		if ( diff > 180 ) {
			diff = 0 - (360 - diff);
		} else if  ( diff < -180 ) {
			diff = 0 - (-360 - diff);
		}
		newAngles[1] += diff * progress;
		
		diff = destAngles[2] - newAngles[2];
		if ( diff > 180 ) {
			diff = 0 - (360 - diff);
		} else if  ( diff < -180 ) {
			diff = 0 - (-360 - diff);
		}
		newAngles[2] += diff * progress;

		VectorCopy( newOrigin, cg.refdef.vieworg );
		VectorCopy( newAngles, cg.refdefViewAngles );

		CG_CalcCutsceneFov(newFov, destFov, progress);
	} else {
		VectorCopy( newOrigin, cg.refdef.vieworg );
		VectorCopy( newAngles, cg.refdefViewAngles );
		CG_CalcCutsceneFov(newFov, newFov, progress);
	}

	AnglesToAxis( cg.refdefViewAngles, cg.refdef.viewaxis );

	return;
}

/*
===============
CG_CalcViewValues

Sets cg.refdef view values
===============
*/
static void CG_CalcViewValues( void ) {
	playerState_t	*ps;

	memset( &cg.refdef, 0, sizeof( cg.refdef ) );

	// calculate size of 3D view
	CG_CalcVrect();

	ps = &cg.predictedPlayerState;

	//cutscene view
	if ( ps->pm_type == PM_CUTSCENE ) {
		CG_CalcCutsceneViewValues();	//this also calculates fov
		return;
	}

	// intermission view
	if ( ps->pm_type == PM_INTERMISSION ) {
		VectorCopy( ps->origin, cg.refdef.vieworg );
		VectorCopy( ps->viewangles, cg.refdefViewAngles );
		AnglesToAxis( cg.refdefViewAngles, cg.refdef.viewaxis );
		CG_CalcFov();
		return;
	}

	cg.bobcycle = ( ps->bobCycle & 128 ) >> 7;
	cg.bobfracsin = fabs( sin( ( ps->bobCycle & 127 ) / 127.0 * M_PI ) );

	cg.xyspeed = sqrt( ps->velocity[0] * ps->velocity[0] +
		ps->velocity[1] * ps->velocity[1] );


	VectorCopy( ps->origin, cg.refdef.vieworg );
	VectorCopy( ps->viewangles, cg.refdefViewAngles );

	if ( cg.renderingThirdPerson ) {
		// back away from character
		CG_OffsetThirdPersonView();
	} else {
		// offset for local bobbing and kicks
		CG_OffsetFirstPersonView();
	}

	// leilei - View-from-the-model-eyes feature, aka "fullbody awareness" lol
	if (cg.renderingEyesPerson && !cg.renderingThirdPerson){
		vec3_t		forward, up;	
		cg.refdefViewAngles[ROLL] = headang[ROLL];
		cg.refdefViewAngles[PITCH] = headang[PITCH];
		cg.refdefViewAngles[YAW] = headang[YAW];

		AngleVectors( headang, forward, NULL, up );
		if (cg.renderingEyesPerson){
			VectorMA( headpos, cg_cameraEyes_Fwd.value, forward, headpos );
			VectorMA( headpos, cg_cameraEyes_Up.value, up, headpos );
		}

		cg.refdef.vieworg[0] = ps->origin[0] + headpos[0];
		cg.refdef.vieworg[1] = ps->origin[1] + headpos[1];
		cg.refdef.vieworg[2] = ps->origin[2] + headpos[2] ;
		
	}

	// position eye reletive to origin
	AnglesToAxis( cg.refdefViewAngles, cg.refdef.viewaxis );

	if ( cg.hyperspace ) {
		cg.refdef.rdflags |= RDF_NOWORLDMODEL | RDF_HYPERSPACE;
	}

	// field of view
	CG_CalcFov();
	return;
}


/*
=====================
CG_PowerupTimerSounds
=====================
*/
static void CG_PowerupTimerSounds( void ) {
	int		i;
	int		t;

	// powerup timers going away
	for ( i = 0 ; i < MAX_POWERUPS ; i++ ) {
		t = cg.snap->ps.powerups[i];
		if ( t <= cg.time ) {
			continue;
		}
		if ( t - cg.time >= POWERUP_BLINKS * POWERUP_BLINK_TIME ) {
			continue;
		}
		if ( ( t - cg.time ) / POWERUP_BLINK_TIME != ( t - cg.oldTime ) / POWERUP_BLINK_TIME ) {
			trap_S_StartSound( NULL, cg.snap->ps.clientNum, CHAN_ITEM, cgs.media.wearOffSound );
		}
	}
}

/*
=====================
CG_AddBufferedSound
=====================
*/
void CG_AddBufferedSound( sfxHandle_t sfx ) {
	if ( !sfx )
		return;
	cg.soundBuffer[cg.soundBufferIn] = sfx;
	cg.soundBufferIn = (cg.soundBufferIn + 1) % MAX_SOUNDBUFFER;
	if (cg.soundBufferIn == cg.soundBufferOut) {
		cg.soundBufferOut++;
	}
}

/*
=====================
CG_PlayBufferedSounds
=====================
*/
static void CG_PlayBufferedSounds( void ) {
	if ( cg.soundTime < cg.time ) {
		if (cg.soundBufferOut != cg.soundBufferIn && cg.soundBuffer[cg.soundBufferOut]) {
			trap_S_StartLocalSound(cg.soundBuffer[cg.soundBufferOut], CHAN_ANNOUNCER);
			cg.soundBuffer[cg.soundBufferOut] = 0;
			cg.soundBufferOut = (cg.soundBufferOut + 1) % MAX_SOUNDBUFFER;
			cg.soundTime = cg.time + 750;
		}
	}
}

/*
=================
CG_DrawActiveFrame

Generates and draws a game scene and status information at the given time.
=================
*/
void CG_DrawActiveFrame( int serverTime, qboolean demoPlayback ) {
	cg.time = serverTime;
	cg.demoPlayback = demoPlayback;

	// update cvars
	CG_UpdateCvars();

	// if we are only updating the screen as a loading
	// pacifier, don't even try to read snapshots
	if ( cg.infoScreenText[0] != 0 ) {
		CG_DrawInformation();
		return;
	}

	// any looped sounds will be respecified as entities
	// are added to the render list
	trap_S_ClearLoopingSounds(qfalse);

	// clear all the render lists
	trap_R_ClearScene();

	// set up cg.snap and possibly cg.nextSnap
	CG_ProcessSnapshots();

	// if we haven't received any snapshots yet, all
	// we can draw is the information screen
	if ( !cg.snap || ( cg.snap->snapFlags & SNAPFLAG_NOT_ACTIVE ) ) {
		CG_DrawInformation();
		return;
	}

	// let the client system know what our weapon and zoom settings are
	trap_SetUserCmdValue( cg.weaponSelect, cg.zoomSensitivity );

	// this counter will be bumped for every valid scene we generate
	cg.clientFrame++;

	// update cg.predictedPlayerState
	CG_PredictPlayerState();

	// decide on third person view
	cg.renderingThirdPerson = cg_thirdPerson.integer && cg.snap->ps.pm_type != PM_CUTSCENE && cg.snap->ps.pm_type != PM_SPECTATOR || (cg.snap->ps.stats[STAT_HEALTH] <= 0) || cg.snap->ps.stats[STAT_VEHICLE];
	cg.renderingEyesPerson = !cg_thirdPerson.integer && cg_cameraEyes.integer && cg.snap->ps.pm_type != PM_CUTSCENE && cg.snap->ps.pm_type != PM_SPECTATOR || cg.snap->ps.stats[STAT_VEHICLE];

	// build cg.refdef
	CG_CalcViewValues();

	// first person blend blobs, done after AnglesToAxis
	if ( !cg.renderingThirdPerson ) {
		CG_DamageBlendBlob();
	}

	// build the render lists
	if ( !cg.hyperspace ) {
		CG_AddPacketEntities();			// adter calcViewValues, so predicted player state is correct
		CG_AddMarks();
		CG_AddLocalEntities();
		CG_AddAtmosphericEffects();
		CG_ViewFog();
		CG_ViewSky();
	}
	CG_AddViewWeapon( &cg.predictedPlayerState );

	// add buffered sounds
	CG_PlayBufferedSounds();

	cg.refdef.time = cg.time;
	memcpy( cg.refdef.areamask, cg.snap->areamask, sizeof( cg.refdef.areamask ) );

	// warning sounds when powerup is wearing off
	CG_PowerupTimerSounds();

	// update audio positions
	trap_S_Respatialize( cg.snap->ps.clientNum, cg.refdef.vieworg, cg.refdef.viewaxis );

	// actually issue the rendering calls
	CG_DrawActive();

	// add frames to lagometer
	cg.frametime = cg.time - cg.oldTime;
	if ( cg.frametime < 0 ) {
		cg.frametime = 0;
	}
	cg.oldTime = cg.time;
	CG_AddLagometerFrameInfo();

	if ( cg_stats.integer ) {
		CG_Printf( "cg.clientFrame:%i\n", cg.clientFrame );
	}

}

