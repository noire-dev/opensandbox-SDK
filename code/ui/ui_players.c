// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// OpenSandbox — GPLv2; see LICENSE for details.
// ui_players.c

#include "../qcommon/js_local.h"

#define SWINGSPEED 0.3f

#define DEFAULT_MODEL "beret"
#define IDLE_ANIM_STARTTIME 8000

#define ANIM_SLOWDOWN 1.7

static int dp_realtime;

qboolean drawTeamModel = qfalse;

static void UI_PositionRotatedEntityOnTag(refEntity_t *entity, const refEntity_t *parent, clipHandle_t parentModel, char *tagName) {
	int i;
	orientation_t lerped;
	vec3_t tempAxis[3];

	// lerp the tag
	trap_R_LerpTag(&lerped, parentModel, parent->oldframe, parent->frame, 1.0 - parent->backlerp, tagName);

	// FIXME: allow origin offsets along tag?
	VectorCopy(parent->origin, entity->origin);
	for(i = 0; i < 3; i++) {
		VectorMA(entity->origin, lerped.origin[i], parent->axis[i], entity->origin);
	}

	// cast away const because of compiler problems
	MatrixMultiply(entity->axis, ((refEntity_t *)parent)->axis, tempAxis);
	MatrixMultiply(lerped.axis, tempAxis, entity->axis);
}

static void UI_SetLerpFrameAnimation(playerInfo_t *ci, lerpFrame_t *lf, int newAnimation) {
	animation_t *anim;

	lf->animationNumber = newAnimation;
	newAnimation &= ~ANIM_TOGGLEBIT;

	if(newAnimation < 0 || newAnimation >= MAX_ANIMATIONS) {
		trap_Error(va("Bad animation number: %i", newAnimation));
	}

	anim = &ci->animations[newAnimation];

	lf->animation = anim;
	lf->animationTime = lf->frameTime + anim->initialLerp;
}

static void UI_RunLerpFrame(playerInfo_t *ci, lerpFrame_t *lf, int newAnimation) {
	int f;
	animation_t *anim;

	// see if the animation sequence is switching
	if(newAnimation != lf->animationNumber || !lf->animation) {
		UI_SetLerpFrameAnimation(ci, lf, newAnimation);
	}

	// if we have passed the current frame, move it to
	// oldFrame and calculate a new frame
	if(dp_realtime >= lf->frameTime) {
		lf->oldFrame = lf->frame;
		lf->oldFrameTime = lf->frameTime;

		// get the next frame based on the animation
		anim = lf->animation;
		if(dp_realtime < lf->animationTime) {
			lf->frameTime = lf->animationTime; // initial lerp
		} else {
			lf->frameTime = lf->oldFrameTime + anim->frameLerp;
		}
		f = (lf->frameTime - lf->animationTime) / anim->frameLerp;
		if(f >= anim->numFrames) {
			f -= anim->numFrames;
			if(anim->loopFrames) {
				f %= anim->loopFrames;
				f += anim->numFrames - anim->loopFrames;
			} else {
				f = anim->numFrames - 1;
				// the animation is stuck at the end, so it
				// can immediately transition to another sequence
				lf->frameTime = dp_realtime;
			}
		}
		lf->frame = anim->firstFrame + f;
		if(dp_realtime > lf->frameTime) {
			lf->frameTime = dp_realtime;
		}
	}

	if(lf->frameTime > dp_realtime + 200) {
		lf->frameTime = dp_realtime;
	}

	if(lf->oldFrameTime > dp_realtime) {
		lf->oldFrameTime = dp_realtime;
	}
	// calculate current lerp value
	if(lf->frameTime == lf->oldFrameTime) {
		lf->backlerp = 0;
	} else {
		lf->backlerp = 1.0 - (float)(dp_realtime - lf->oldFrameTime) / (lf->frameTime - lf->oldFrameTime);
	}
}

static void UI_PlayerAnimation(playerInfo_t *pi, int *legsOld, int *legs, float *legsBackLerp, int *torsoOld, int *torso, float *torsoBackLerp) {

	if(pi->legs.yawing && (pi->legsAnim & ~ANIM_TOGGLEBIT) == LEGS_IDLE) {
		UI_RunLerpFrame(pi, &pi->legs, LEGS_TURN);
	} else {
		UI_RunLerpFrame(pi, &pi->legs, pi->legsAnim);
	}
	*legsOld = pi->legs.oldFrame;
	*legs = pi->legs.frame;
	*legsBackLerp = pi->legs.backlerp;

	UI_RunLerpFrame(pi, &pi->torso, pi->torsoAnim);
	*torsoOld = pi->torso.oldFrame;
	*torso = pi->torso.frame;
	*torsoBackLerp = pi->torso.backlerp;
}

static void UI_SwingAngles(float destination, float swingTolerance, float clampTolerance, float speed, float *angle, qboolean *swinging) {
	float swing;
	float move;
	float scale;

	if(!*swinging) {
		// see if a swing should be started
		swing = AngleSubtract(*angle, destination);
		if(swing > swingTolerance || swing < -swingTolerance) {
			*swinging = qtrue;
		}
	}

	if(!*swinging) return;

	// modify the speed depending on the delta
	// so it doesn't seem so linear
	swing = AngleSubtract(destination, *angle);
	scale = fabs(swing);
	if(scale < swingTolerance * 0.5) {
		scale = 0.5;
	} else if(scale < swingTolerance) {
		scale = 1.0;
	} else {
		scale = 2.0;
	}

	// swing towards the destination angle
	if(swing >= 0) {
		move = uis.frametime * scale * speed;
		if(move >= swing) {
			move = swing;
			*swinging = qfalse;
		}
		*angle = AngleMod(*angle + move);
	} else if(swing < 0) {
		move = uis.frametime * scale * -speed;
		if(move <= swing) {
			move = swing;
			*swinging = qfalse;
		}
		*angle = AngleMod(*angle + move);
	}

	// clamp to no more than tolerance
	swing = AngleSubtract(destination, *angle);
	if(swing > clampTolerance) {
		*angle = AngleMod(destination - (clampTolerance - 1));
	} else if(swing < -clampTolerance) {
		*angle = AngleMod(destination + (clampTolerance - 1));
	}
}

static float UI_MovedirAdjustment(playerInfo_t *pi) {
	vec3_t relativeAngles;
	vec3_t moveVector;

	VectorSubtract(pi->viewAngles, pi->moveAngles, relativeAngles);
	AngleVectors(relativeAngles, moveVector, NULL, NULL);
	if(Q_fabs(moveVector[0]) < 0.01) {
		moveVector[0] = 0.0;
	}
	if(Q_fabs(moveVector[1]) < 0.01) {
		moveVector[1] = 0.0;
	}

	if(moveVector[1] == 0 && moveVector[0] > 0) {
		return 0;
	}
	if(moveVector[1] < 0 && moveVector[0] > 0) {
		return 22;
	}
	if(moveVector[1] < 0 && moveVector[0] == 0) {
		return 45;
	}
	if(moveVector[1] < 0 && moveVector[0] < 0) {
		return -22;
	}
	if(moveVector[1] == 0 && moveVector[0] < 0) {
		return 0;
	}
	if(moveVector[1] > 0 && moveVector[0] < 0) {
		return 22;
	}
	if(moveVector[1] > 0 && moveVector[0] == 0) {
		return -45;
	}

	return -22;
}

static void UI_PlayerAngles(modelAnim_t *m, vec3_t legs[3], vec3_t torso[3], vec3_t head[3]) {
	vec3_t legsAngles, torsoAngles, headAngles;
	float dest;
	float adjust;
	playerInfo_t *pi;

	pi = &m->player;
	VectorCopy(pi->viewAngles, headAngles);
	headAngles[YAW] = AngleMod(headAngles[YAW]);
	VectorClear(legsAngles);
	VectorClear(torsoAngles);

	// --------- yaw -------------

	// allow yaw to drift a bit
	if((pi->legsAnim & ~ANIM_TOGGLEBIT) != LEGS_IDLE || (pi->torsoAnim & ~ANIM_TOGGLEBIT) != TORSO_STAND) {
		// if not standing still, always point all in the same direction
		pi->torso.yawing = qtrue;   // always center
		pi->torso.pitching = qtrue; // always center
		pi->legs.yawing = qtrue;    // always center
	}

	// adjust legs for movement dir
	adjust = UI_MovedirAdjustment(pi);
	legsAngles[YAW] = headAngles[YAW] + adjust;
	torsoAngles[YAW] = headAngles[YAW] + 0.25 * adjust;

	// torso
	UI_SwingAngles(torsoAngles[YAW], 25.0f, 90.0f, SWINGSPEED, &pi->torso.yawAngle, &pi->torso.yawing);
	UI_SwingAngles(legsAngles[YAW], 40.0f, 90.0f, SWINGSPEED, &pi->legs.yawAngle, &pi->legs.yawing);

	torsoAngles[YAW] = pi->torso.yawAngle;
	legsAngles[YAW] = pi->legs.yawAngle;

	// --------- pitch -------------

	// only show a fraction of the pitch angle in the torso
	if(headAngles[PITCH] > 180) {
		dest = (-360 + headAngles[PITCH]) * 0.75;
	} else {
		dest = headAngles[PITCH] * 0.75;
	}
	UI_SwingAngles(dest, 15, 30, 0.1f, &pi->torso.pitchAngle, &pi->torso.pitching);
	torsoAngles[PITCH] = pi->torso.pitchAngle;

	if(pi->fixedtorso) {
		torsoAngles[PITCH] = 0.0f;
	}

	if(pi->fixedlegs) {
		legsAngles[YAW] = torsoAngles[YAW];
		legsAngles[PITCH] = 0.0f;
		legsAngles[ROLL] = 0.0f;
	}

	// pull the angles back out of the hierarchial chain
	AnglesSubtract(headAngles, torsoAngles, headAngles);
	AnglesSubtract(torsoAngles, legsAngles, torsoAngles);

	AnglesToAxis(legsAngles, legs);
	AnglesToAxis(torsoAngles, torso);
	AnglesToAxis(headAngles, head);
}

static void UI_DrawPlayer(float x, float y, float w, float h, modelAnim_t *m, int time) {
	refdef_t refdef;
	refEntity_t legs;
	refEntity_t torso;
	refEntity_t head;
	vec3_t origin;
	int renderfx;
	vec3_t mins = {-16, -16, -24};
	vec3_t maxs = {16, 16, 32};
	float len;
	float xx;
	playerInfo_t *pi;

	pi = &m->player;
	if(!pi->legsModel || !pi->torsoModel || !pi->headModel || !pi->animations[0].numFrames) {
		return;
	}

	dp_realtime = time;

	UI_AdjustFrom640(&x, &y, &w, &h);

	memset(&refdef, 0, sizeof(refdef));
	memset(&legs, 0, sizeof(legs));
	memset(&torso, 0, sizeof(torso));
	memset(&head, 0, sizeof(head));

	refdef.rdflags = RDF_NOWORLDMODEL;

	AxisClear(refdef.viewaxis);

	refdef.x = x;
	refdef.y = y;
	refdef.width = w;
	refdef.height = h;

	refdef.fov_x = (int)((float)refdef.width / 640.0f * 90.0f);
	xx = refdef.width / tan(refdef.fov_x / 360 * M_PI);
	refdef.fov_y = atan2(refdef.height, xx);
	refdef.fov_y *= (360 / M_PI);

	// calculate distance so the player nearly fills the box
	len = 0.7 * (maxs[2] - mins[2]);
	origin[0] = len / tan(DEG2RAD(refdef.fov_x) * 0.44);
	origin[1] = 0.44 * (mins[1] + maxs[1]);
	origin[2] = -0.44 * (mins[2] + maxs[2]);

	refdef.time = dp_realtime;

	trap_R_ClearScene();

	// get the rotation information
	UI_PlayerAngles(m, legs.axis, torso.axis, head.axis);

	// get the animation state (after rotation, to allow feet shuffle)
	UI_PlayerAnimation(pi, &legs.oldframe, &legs.frame, &legs.backlerp, &torso.oldframe, &torso.frame, &torso.backlerp);

	renderfx = RF_LIGHTING_ORIGIN;

	//
	// add the legs
	//
	legs.hModel = pi->legsModel;
	legs.customSkin = pi->legsSkin;
	legs.customShader = pi->legsShader;
	legs.shaderRGBA[0] = cvarInt("legsR");
	legs.shaderRGBA[1] = cvarInt("legsG");
	legs.shaderRGBA[2] = cvarInt("legsB");
	legs.shaderRGBA[3] = 255;

	VectorCopy(origin, legs.origin);

	VectorCopy(origin, legs.lightingOrigin);
	legs.renderfx = renderfx;
	VectorCopy(legs.origin, legs.oldorigin);

	trap_R_AddRefEntityToScene(&legs);

	if(!legs.hModel) {
		return;
	}

	//
	// add the torso
	//
	torso.hModel = pi->torsoModel;
	if(!torso.hModel) {
		return;
	}

	torso.customSkin = pi->torsoSkin;
	if(!torso.customSkin) {
		torso.customShader = pi->torsoShader;
	}
	torso.shaderRGBA[0] = cvarInt("modelR");
	torso.shaderRGBA[1] = cvarInt("modelG");
	torso.shaderRGBA[2] = cvarInt("modelB");
	torso.shaderRGBA[3] = 255;

	VectorCopy(origin, torso.lightingOrigin);

	UI_PositionRotatedEntityOnTag(&torso, &legs, pi->legsModel, "tag_torso");

	torso.renderfx = renderfx;

	trap_R_AddRefEntityToScene(&torso);

	//
	// add the head
	//
	head.hModel = pi->headModel;
	if(!head.hModel) {
		return;
	}
	head.customSkin = pi->headSkin;
	if(!head.customSkin) {
		head.customShader = pi->headShader;
	}
	head.shaderRGBA[0] = cvarInt("headR");
	head.shaderRGBA[1] = cvarInt("headG");
	head.shaderRGBA[2] = cvarInt("headB");
	head.shaderRGBA[3] = 255;

	VectorCopy(origin, head.lightingOrigin);

	UI_PositionRotatedEntityOnTag(&head, &torso, pi->torsoModel, "tag_head");

	head.renderfx = renderfx;

	trap_R_AddRefEntityToScene(&head);

	//
	// add an accent light
	//
	origin[0] -= 100; // + = behind, - = in front
	origin[1] += 100; // + = left, - = right
	origin[2] += 100; // + = above, - = below
	trap_R_AddLightToScene(origin, 500, 1.0, 1.0, 1.0);

	origin[0] -= 100;
	origin[1] -= 100;
	origin[2] -= 100;
	trap_R_AddLightToScene(origin, 500, 1.0, 0.0, 0.0);

	trap_R_RenderScene(&refdef);
}

static qboolean UI_RegisterClientSkin(playerInfo_t *pi, const char *modelName, const char *skinName, qboolean head, qboolean legs, qboolean model) {
	char filename[MAX_QPATH];
	qboolean skinLoaded = qfalse;

	if(head) {
		Com_sprintf(filename, sizeof(filename), "models/players/%s/head_%s.skin", modelName, skinName);
		pi->headSkin = trap_R_RegisterSkin(filename);
		if(!pi->headSkin) {
			Com_sprintf(filename, sizeof(filename), "models/players/%s/head_%s", modelName, skinName);
			pi->headShader = trap_R_RegisterShaderNoMip(filename);
		}

		if(pi->headSkin || pi->headShader) skinLoaded = qtrue;
	}
	if(legs) {
		Com_sprintf(filename, sizeof(filename), "models/players/%s/lower_%s.skin", modelName, skinName);
		pi->legsSkin = trap_R_RegisterSkin(filename);
		if(!pi->legsSkin) {
			Com_sprintf(filename, sizeof(filename), "models/players/%s/lower_%s", modelName, skinName);
			pi->legsShader = trap_R_RegisterShaderNoMip(filename);
		}

		if(pi->legsSkin || pi->legsShader) {
			skinLoaded = qtrue;
		}
	}
	if(model) {
		Com_sprintf(filename, sizeof(filename), "models/players/%s/upper_%s.skin", modelName, skinName);
		pi->torsoSkin = trap_R_RegisterSkin(filename);
		if(!pi->torsoSkin) {
			Com_sprintf(filename, sizeof(filename), "models/players/%s/upper_%s", modelName, skinName);
			pi->torsoShader = trap_R_RegisterShaderNoMip(filename);
		}

		if(pi->torsoSkin || pi->torsoShader) {
			skinLoaded = qtrue;
		}
	}

	if(skinLoaded) {
		return qtrue;
	} else {
		return qfalse;
	}
}

static qboolean UI_ParseAnimationFile(const char *filename, playerInfo_t *pi) {
	char *text_p, *prev;
	int len;
	int i;
	char *token;
	float fps;
	int skip;
	char text[20000];
	fileHandle_t f;
	animation_t *animations;

	animations = pi->animations;

	memset(animations, 0, sizeof(animation_t) * MAX_ANIMATIONS);

	// load the file
	len = FS_Open(filename, &f, FS_READ);
	if(len <= 0) {
		return qfalse;
	}
	if(len >= (sizeof(text) - 1)) {
		Com_Printf("File %s too long\n", filename);
		return qfalse;
	}
	FS_Read(text, len, f);
	text[len] = 0;
	FS_Close(f);

	// parse the text
	text_p = text;
	skip = 0; // quite the compiler warning
	pi->fixedtorso = qfalse;
	pi->fixedlegs = qfalse;

	// read optional parameters
	while(1) {
		prev = text_p; // so we can unget
		token = COM_Parse(&text_p);
		if(!token) {
			break;
		}
		if(!Q_stricmp(token, "footsteps")) {
			token = COM_Parse(&text_p);
			if(!token) {
				break;
			}
			continue;
		} else if(!Q_stricmp(token, "headoffset")) {
			for(i = 0; i < 3; i++) {
				token = COM_Parse(&text_p);
				if(!token) {
					break;
				}
			}
			continue;
		} else if(!Q_stricmp(token, "sex")) {
			token = COM_Parse(&text_p);
			if(!token) {
				break;
			}
			continue;
		} else if(!Q_stricmp(token, "fixedlegs")) {
			pi->fixedlegs = qtrue;
			continue;
		} else if(!Q_stricmp(token, "fixedtorso")) {
			pi->fixedtorso = qtrue;
			continue;
		}

		// if it is a number, start parsing animations
		if(token[0] >= '0' && token[0] <= '9') {
			text_p = prev; // unget the token
			break;
		}

		Com_Printf("unknown token '%s' is %s\n", token, filename);
	}

	// read information for each frame
	for(i = 0; i < MAX_ANIMATIONS; i++) {
		token = COM_Parse(&text_p);
		if(!token) {
			break;
		}
		animations[i].firstFrame = atoi(token);
		// leg only frames are adjusted to not count the upper body only frames
		if(i == LEGS_WALKCR) {
			skip = animations[LEGS_WALKCR].firstFrame - animations[TORSO_GESTURE].firstFrame;
		}
		if(i >= LEGS_WALKCR) {
			animations[i].firstFrame -= skip;
		}

		token = COM_Parse(&text_p);
		if(!token) {
			break;
		}
		animations[i].numFrames = atoi(token);

		token = COM_Parse(&text_p);
		if(!token) {
			break;
		}
		animations[i].loopFrames = atoi(token);

		token = COM_Parse(&text_p);
		if(!token) {
			break;
		}
		fps = atof(token);
		if(fps == 0) {
			fps = 1;
		}
		animations[i].frameLerp = 1000 / fps;
		animations[i].initialLerp = 1000 / fps;
	}

	if(i != MAX_ANIMATIONS) {
		Com_Printf("Error parsing animation file: %s", filename);
		return qfalse;
	}

	return qtrue;
}

static qboolean UI_RegisterClientModelname(modelAnim_t *m) {
	playerInfo_t *pi;

	// separated skin and model names
	char bodyName[MAX_QPATH];
	char headName[MAX_QPATH];
	char legsName[MAX_QPATH];
	char skinBodyName[MAX_QPATH];
	char skinHeadName[MAX_QPATH];
	char skinLegsName[MAX_QPATH];

	char filename[MAX_QPATH];
	char *slash;

	// combined model and skin name, before splitting
	char *modelSkinName, *modelHeadName, *modelLegsName;

	pi = &m->player;

	pi->torsoModel = 0;
	pi->headModel = 0;

	// define which model we're working with
	if(drawTeamModel) {
		modelSkinName = m->team_modelskin;
		modelHeadName = m->team_headskin;
		modelLegsName = m->team_legsskin;
	} else {
		modelSkinName = m->modelskin;
		modelHeadName = m->headskin;
		modelLegsName = m->legsskin;
	}

	// validate
	m->bUnknownModel = qfalse;
	m->bUnknownHeadModel = qfalse;
	m->bUnknownLegsModel = qfalse;

	// do the body model
	Q_strncpyz(bodyName, modelSkinName, sizeof(bodyName));

	slash = strchr(bodyName, '/');
	if(!slash) {
		// modelName did not include a skin name
		Q_strncpyz(skinBodyName, "default", sizeof(skinBodyName));
	} else {
		Q_strncpyz(skinBodyName, slash + 1, sizeof(skinBodyName));
		// truncate bodyName
		*slash = 0;
	}

	// do the legs model
	Q_strncpyz(legsName, modelLegsName, sizeof(legsName));

	slash = strchr(legsName, '/');
	if(!slash) {
		// modelName did not include a skin name
		Q_strncpyz(skinLegsName, "default", sizeof(skinLegsName));
	} else {
		Q_strncpyz(skinLegsName, slash + 1, sizeof(skinLegsName));
		// truncate bodyName
		*slash = 0;
	}

	// do the head model separately
	Q_strncpyz(headName, modelHeadName, sizeof(headName));

	slash = strchr(headName, '/');
	if(!slash) {
		// modelName did not include a skin name
		Q_strncpyz(skinHeadName, "default", sizeof(skinHeadName));
	} else {
		Q_strncpyz(skinHeadName, slash + 1, sizeof(skinHeadName));
		// truncate modelName
		*slash = 0;
	}

	// load cmodels before models so filecache works
	Com_sprintf(filename, sizeof(filename), "models/players/%s/lower.md3", bodyName);
	pi->legsModel = trap_R_RegisterModel(filename);
	if(!pi->legsModel) {
		Com_Printf("Failed to load model file %s\n", filename);
		m->bUnknownModel = qtrue;
		return qfalse;
	}

	Com_sprintf(filename, sizeof(filename), "models/players/%s/upper.md3", bodyName);
	pi->torsoModel = trap_R_RegisterModel(filename);
	if(!pi->torsoModel) {
		Com_Printf("Failed to load model file %s\n", filename);
		m->bUnknownModel = qtrue;
		return qfalse;
	}

	Com_sprintf(filename, sizeof(filename), "models/players/%s/head.md3", headName);
	pi->headModel = trap_R_RegisterModel(filename);
	if(!pi->headModel) {
		Com_Printf("Failed to load model file %s\n", filename);
		m->bUnknownHeadModel = qtrue;
		return qfalse;
	}

	// this is the body part of the model
	UI_RegisterClientSkin(pi, bodyName, skinBodyName, qfalse, qfalse, qtrue);

	// and this is the head part of the model
	UI_RegisterClientSkin(pi, headName, skinHeadName, qtrue, qfalse, qfalse);

	// and this is the legs part of the model
	UI_RegisterClientSkin(pi, legsName, skinLegsName, qfalse, qtrue, qfalse);

	// load the animations
	// always for the body, never the head!
	Com_sprintf(filename, sizeof(filename), "models/players/%s/animation.cfg", bodyName);
	if(!UI_ParseAnimationFile(filename, pi)) {
		Com_Printf("Failed to load animation file %s\n", filename);
		return qfalse;
	}

	return qtrue;
}

static qboolean UI_PlayerInfo_SetModel(modelAnim_t *m) {
	playerInfo_t *pi;

	pi = &m->player;
	if(!UI_RegisterClientModelname(m)) return qfalse;

	pi->newModel = qtrue;

	return qtrue;
}

static void UI_PlayerInfo_SetInfo(playerInfo_t *pi, int legsAnim, int torsoAnim, vec3_t viewAngles, vec3_t moveAngles) {
	int currentAnim;

	// view angles
	VectorCopy(viewAngles, pi->viewAngles);

	// move angles
	VectorCopy(moveAngles, pi->moveAngles);

	if(pi->newModel) {
		pi->newModel = qfalse;

		pi->legsAnim = ((pi->legsAnim & ANIM_TOGGLEBIT) ^ ANIM_TOGGLEBIT) | legsAnim;
		pi->legs.yawAngle = viewAngles[YAW];
		pi->legs.yawing = qfalse;

		pi->torsoAnim = ((pi->torsoAnim & ANIM_TOGGLEBIT) ^ ANIM_TOGGLEBIT) | torsoAnim;
		pi->torso.yawAngle = viewAngles[YAW];
		pi->torso.yawing = qfalse;
		pi->torso.pitchAngle = viewAngles[PITCH];
		pi->torso.pitching = qfalse;
		return;
	}

	// leg animation
	currentAnim = pi->legsAnim & ~ANIM_TOGGLEBIT;

	// torso animation
	if(torsoAnim == TORSO_STAND || torsoAnim == TORSO_STAND2) {
		torsoAnim = TORSO_STAND2;
	}

	currentAnim = pi->torsoAnim & ~ANIM_TOGGLEBIT;
}

static void PlayerInfo_ModelTrackCursor(modelAnim_t *m) {
	vec3_t cursor3d;
	menuelement_s *b;

	b = &m->bitmap;
	cursor3d[0] = 175.0;
	cursor3d[1] = (m->cursorx - (b->generic.x + b->width / 4));
	cursor3d[2] = (m->cursory - (b->generic.y + b->height / 4));

	VectorClear(m->viewangles);
	VectorClear(m->moveangles);

	vectoangles(cursor3d, m->viewangles);

	VectorCopy(m->viewangles, m->moveangles);

	// prevent stoop or back-breaking during attract mode
	if(m->bDoingIdleAnim) {
		if(m->viewangles[PITCH] > 20)
			m->viewangles[PITCH] = 20;
		else if(m->viewangles[PITCH] < -20)
			m->viewangles[PITCH] = -20;
	}

	// orient for co-ordinate system
	m->viewangles[PITCH] = -m->viewangles[PITCH];
	m->viewangles[YAW] += 180;

	VectorCopy(m->viewangles, m->player.viewAngles);
	VectorCopy(m->moveangles, m->player.moveAngles);
}

static void GUI_PlayerInfo_UpdateAnimation(modelAnim_t *m) {
	m->playerLegs = LEGS_IDLE;
	m->playerTorso = TORSO_STAND2;
	PlayerInfo_ModelTrackCursor(m);

	UI_PlayerInfo_SetInfo(&m->player, m->playerLegs, m->playerTorso, m->viewangles, m->moveangles);
}

static qboolean PlayerInfo_CursorChanged(modelAnim_t *m) {
	// check for cursor position change
	if(uis.cursorx != m->cursorx || uis.cursory != m->cursory) {
		if(m->bDoingIdleAnim) {
			GUI_PlayerInfo_UpdateAnimation(m);
			m->bDoingIdleAnim = qfalse;
		}
		m->cursorx = uis.cursorx;
		m->cursory = uis.cursory;

		return qtrue;
	}

	return qfalse;
}

static void PlayerInfo_SetupNewModel(modelAnim_t *m) {
	vec3_t tmp_view;
	vec3_t tmp_move;

	// store current orientation to avoid too much
	// snap movement when changing models
	if(m->bForceUpdate) {
		VectorCopy(m->player.viewAngles, tmp_view);
		VectorCopy(m->player.moveAngles, tmp_move);
	} else {
		VectorClear(tmp_view);
		VectorClear(tmp_move);

		tmp_view[YAW] = 180 - 30;
	}

	memset(&m->player, 0, sizeof(playerInfo_t));

	VectorCopy(tmp_view, m->viewangles);
	VectorCopy(tmp_move, m->moveangles);
	VectorCopy(tmp_view, m->player.viewAngles);
	VectorCopy(tmp_move, m->player.moveAngles);

	m->bUnknownModel = qfalse;
	if(!UI_PlayerInfo_SetModel(m)) {
		m->bForceUpdate = qfalse;
		return;
	}

	// update animation
	GUI_PlayerInfo_UpdateAnimation(m);

	// model change done
	m->cursorx = m->cursory = -1;
}

void GUI_PlayerInfo_InitModel(modelAnim_t *m) {
	char *buffer;

	buffer = cvarString("model");
	strcpy(m->modelskin, buffer);

	buffer = cvarString("headmodel");
	strcpy(m->headskin, buffer);

	buffer = cvarString("legsmodel");
	strcpy(m->legsskin, buffer);

	buffer = cvarString("team_model");
	strcpy(m->team_modelskin, buffer);

	buffer = cvarString("team_headmodel");
	strcpy(m->team_headskin, buffer);

	buffer = cvarString("team_legsmodel");
	strcpy(m->team_legsskin, buffer);

	PlayerInfo_SetupNewModel(m);
}

void GUI_PlayerInfo_DrawTeamModel(modelAnim_t *m, qboolean teamModel) {
	if(teamModel != drawTeamModel) {
		drawTeamModel = teamModel;
		m->bForceUpdate = qtrue;
	}
}

const char *GUI_ModelName(const char *modelname) {
	static char model[MODELNAME_BUFFER];
	char *ptr;

	model[0] = 0;
	if(!modelname || !modelname[0]) {
		strcpy(model, DEFAULT_MODEL);
		return model;
	}

	ptr = strchr(modelname, '/');

	if(ptr)
		Q_strncpyz(model, modelname, ptr - modelname + 1);
	else {
		strcpy(model, modelname);
	}

	return model;
}

void GUI_PlayerInfo_AnimateModel(modelAnim_t *m) {
	menuelement_s *b;
	int modelchange, team_modelchange, draw_team;

	b = &m->bitmap;

	if(trap_MemoryRemaining() <= LOW_MEMORY) {
		ST_DrawString(b->generic.x, b->generic.y + b->height / 2, "LOW MEMORY", UI_LEFT, color_white, 1.00);
		return;
	}

	// check if model has changed
	// only update models when we're actually viewing them
	modelchange = 0;
	team_modelchange = 0;
	draw_team = drawTeamModel;

	// update might be forced from elsewhere
	if(m->bForceUpdate || uis.firstdraw) {
		PlayerInfo_SetupNewModel(m);
	}

	if(m->bUnknownModel) {
		ST_DrawString(b->generic.x, b->generic.y + b->height / 2, "UNKNOWN MODEL", UI_LEFT, color_white, 1.00);
		return;
	}

	if(m->bUnknownHeadModel) {
		ST_DrawString(b->generic.x, b->generic.y + b->height / 2, "UNKNOWN HEAD", UI_LEFT, color_white, 1.00);
		return;
	}

	if(m->bUnknownLegsModel) {
		ST_DrawString(b->generic.x, b->generic.y + b->height / 2, "UNKNOWN LEGS", UI_LEFT, color_white, 1.00);
		return;
	}

	if(PlayerInfo_CursorChanged(m) || m->bForceUpdate) {
		// follow cursor, and delay idle animation
		PlayerInfo_ModelTrackCursor(m);
	}

	m->bForceUpdate = qfalse;

	UI_DrawPlayer(b->generic.x, b->generic.y, b->width, b->height, m, uis.realtime / ANIM_SLOWDOWN);
}
