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
// ui_players.c

#include "ui_local.h"

#define DEFAULT_MODEL			"beret"

static qboolean drawTeamModel = qfalse;

/*
======================
UI_PositionRotatedEntityOnTag
======================
*/
static void UI_PositionRotatedEntityOnTag( refEntity_t *entity, const refEntity_t *parent, 
							clipHandle_t parentModel, char *tagName ) {
	int				i;
	orientation_t	lerped;
	vec3_t			tempAxis[3];

	// lerp the tag
	trap_CM_LerpTag( &lerped, parentModel, parent->oldframe, parent->frame,
		1.0 - parent->backlerp, tagName );

	// FIXME: allow origin offsets along tag?
	VectorCopy( parent->origin, entity->origin );
	for ( i = 0 ; i < 3 ; i++ ) {
		VectorMA( entity->origin, lerped.origin[i], parent->axis[i], entity->origin );
	}

	// cast away const because of compiler problems
	MatrixMultiply( entity->axis, ((refEntity_t *)parent)->axis, tempAxis );
	MatrixMultiply( lerped.axis, tempAxis, entity->axis );
}

/*
===============
UI_DrawPlayer
===============
*/
static void UI_DrawPlayer( float x, float y, float w, float h, modelAnim_t *m ) {
	refdef_t		refdef;
	refEntity_t		legs;
	refEntity_t		torso;
	refEntity_t		head;
	refEntity_t		gun;
	refEntity_t		barrel;
	refEntity_t		flash;
	vec3_t			origin;
	int				renderfx;
	vec3_t			mins = {-16, -16, -24};
	vec3_t			maxs = {16, 16, 32};
	float			len;
	float			xx;
	playerInfo_t	*pi;

	pi = &m->player;	
	if ( !pi->legsModel || !pi->torsoModel || !pi->headModel || !pi->animations[0].numFrames ) {
		return;
	}

	UI_AdjustFrom640( &x, &y, &w, &h );

	memset( &refdef, 0, sizeof( refdef ) );
	memset( &legs, 0, sizeof(legs) );
	memset( &torso, 0, sizeof(torso) );
	memset( &head, 0, sizeof(head) );

	refdef.rdflags = RDF_NOWORLDMODEL;

	AxisClear( refdef.viewaxis );

	refdef.x = x;
	refdef.y = y;
	refdef.width = w;
	refdef.height = h;

	refdef.fov_x = (int)((float)refdef.width / 640.0f * 90.0f);
	xx = refdef.width / tan( refdef.fov_x / 360 * M_PI );
	refdef.fov_y = atan2( refdef.height, xx );
	refdef.fov_y *= ( 360 / M_PI );

	// calculate distance so the player nearly fills the box
	len = 0.7 * ( maxs[2] - mins[2] );		
	origin[0] = len / tan( DEG2RAD(refdef.fov_x) * 0.44 );
	origin[1] = 0.44 * ( mins[1] + maxs[1] );
	origin[2] = -0.44 * ( mins[2] + maxs[2] );

	refdef.time = 0;

	trap_R_ClearScene();

	renderfx = RF_LIGHTING_ORIGIN | RF_NOSHADOW;

	//
	// add the legs
	//
	legs.hModel = pi->legsModel;
	legs.customSkin = pi->legsSkin;
	legs.customShader = pi->legsShader;
	legs.shaderRGBA[0] = trap_Cvar_VariableValue( "cg_plightred");
	legs.shaderRGBA[1] = trap_Cvar_VariableValue( "cg_plightgreen");
	legs.shaderRGBA[2] = trap_Cvar_VariableValue( "cg_plightblue");
	legs.shaderRGBA[3] = 255;

	VectorCopy( origin, legs.origin );

	VectorCopy( origin, legs.lightingOrigin );
	legs.renderfx = renderfx;
	VectorCopy (legs.origin, legs.oldorigin);

	trap_R_AddRefEntityToScene( &legs );

	//
	// add the torso
	//
	torso.hModel = pi->torsoModel;
	torso.customSkin = pi->torsoSkin;
	if(!torso.customSkin){
	torso.customShader = pi->torsoShader;
	}
	torso.shaderRGBA[0] = trap_Cvar_VariableValue( "cg_tolightred");
	torso.shaderRGBA[1] = trap_Cvar_VariableValue( "cg_tolightgreen");
	torso.shaderRGBA[2] = trap_Cvar_VariableValue( "cg_tolightblue");
	torso.shaderRGBA[3] = 255;

	VectorCopy( origin, torso.lightingOrigin );

	UI_PositionRotatedEntityOnTag( &torso, &legs, pi->legsModel, "tag_torso");

	torso.renderfx = renderfx;

	trap_R_AddRefEntityToScene( &torso );

	//
	// add the head
	//
	head.hModel = pi->headModel;
	head.customSkin = pi->headSkin;
	if(!head.customSkin){
	head.customShader = pi->headShader;
	}
	head.shaderRGBA[0] = trap_Cvar_VariableValue( "cg_helightred");
	head.shaderRGBA[1] = trap_Cvar_VariableValue( "cg_helightgreen");
	head.shaderRGBA[2] = trap_Cvar_VariableValue( "cg_helightblue");
	head.shaderRGBA[3] = 255;

	VectorCopy( origin, head.lightingOrigin );

	UI_PositionRotatedEntityOnTag( &head, &torso, pi->torsoModel, "tag_head");

	head.renderfx = renderfx;

	trap_R_AddRefEntityToScene( &head );

	//
	// add an accent light
	//
	origin[0] -= 100;	// + = behind, - = in front
	origin[1] += 100;	// + = left, - = right
	origin[2] += 100;	// + = above, - = below
	trap_R_AddLightToScene( origin, 500, 1.0, 1.0, 1.0 );

	origin[0] -= 100;
	origin[1] -= 100;
	origin[2] -= 100;
	trap_R_AddLightToScene( origin, 500, 1.0, 0.0, 0.0 );

	trap_R_RenderScene( &refdef );
}


/*
==========================
UI_RegisterClientSkin
==========================
*/
static qboolean UI_RegisterClientSkin( playerInfo_t *pi, const char *modelName, const char *skinName, qboolean head, qboolean legs, qboolean model)
{
	char		filename[MAX_QPATH];
	qboolean 	skinLoaded = qfalse;

	if (head)
	{
		Com_sprintf( filename, sizeof( filename ), "models/players/%s/head_%s.skin", modelName, skinName );
		pi->headSkin = trap_R_RegisterSkin( filename );
		if(!pi->headSkin){
		Com_sprintf( filename, sizeof( filename ), "models/players/%s/head_%s", modelName, skinName );
		pi->headShader = trap_R_RegisterShaderNoMip( filename );
		}

		if (pi->headSkin || pi->headShader )
			skinLoaded = qtrue;
		}
	if (legs)
	{
		Com_sprintf( filename, sizeof( filename ), "models/players/%s/lower_%s.skin", modelName, skinName );
		pi->legsSkin = trap_R_RegisterSkin( filename );
		if(!pi->legsSkin){
		Com_sprintf( filename, sizeof( filename ), "models/players/%s/lower_%s", modelName, skinName );
		pi->legsShader = trap_R_RegisterShaderNoMip( filename );
		}
		
		if ( pi->legsSkin || pi->legsShader ) {
			skinLoaded = qtrue;
		}
	}		
	if (model)
	{
		Com_sprintf( filename, sizeof( filename ), "models/players/%s/upper_%s.skin", modelName, skinName );
		pi->torsoSkin = trap_R_RegisterSkin( filename );
		if(!pi->torsoSkin){
		Com_sprintf( filename, sizeof( filename ), "models/players/%s/upper_%s", modelName, skinName );
		pi->torsoShader = trap_R_RegisterShaderNoMip( filename );
		}

		if (pi->torsoSkin || pi->torsoShader ) {
			skinLoaded = qtrue;
		}
	}

	if (skinLoaded) {
		return qtrue;
	} else {
		return qfalse;
	}
}


/*
======================
UI_ParseAnimationFile
======================
*/
static qboolean UI_ParseAnimationFile( const char *filename, playerInfo_t* pi) {
	char		*text_p, *prev;
	int			len;
	int			i;
	char		*token;
	float		fps;
	int			skip;
	char		text[20000];
	fileHandle_t	f;
	animation_t* animations;

	animations = pi->animations;

	memset( animations, 0, sizeof( animation_t ) * MAX_ANIMATIONS );

	// load the file
	len = trap_FS_FOpenFile( filename, &f, FS_READ );
	if ( len <= 0 ) {
		return qfalse;
	}
	if ( len >= ( sizeof( text ) - 1 ) ) {
		Com_Printf( "File %s too long\n", filename );
		return qfalse;
	}
	trap_FS_Read( text, len, f );
	text[len] = 0;
	trap_FS_FCloseFile( f );

	// parse the text
	text_p = text;
	skip = 0;	// quite the compiler warning
	pi->fixedtorso = qfalse;
	pi->fixedlegs = qfalse;

	// read optional parameters
	while ( 1 ) {
		prev = text_p;	// so we can unget
		token = COM_Parse( &text_p );
		if ( !token ) {
			break;
		}
		if ( !Q_stricmp( token, "footsteps" ) ) { //just skip
			token = COM_Parse( &text_p );
			continue;
		} else if ( !Q_stricmp( token, "headoffset" ) ) {
			for ( i = 0 ; i < 3 ; i++ ) {
				token = COM_Parse( &text_p );
				if ( !token ) {
					break;
				}
			}
			continue;
		} else if ( !Q_stricmp( token, "sex" ) ) {
			token = COM_Parse( &text_p );
			if ( !token ) {
				break;
			}
			continue;
		} else if ( !Q_stricmp( token, "fixedlegs" ) ) {
			pi->fixedlegs = qtrue;
			continue;
		} else if ( !Q_stricmp( token, "fixedtorso" ) ) {
			pi->fixedtorso = qtrue;
			continue;
		}

		// if it is a number, start parsing animations
		if ( token[0] >= '0' && token[0] <= '9' ) {
			text_p = prev;	// unget the token
			break;
		}

		Com_Printf( "unknown token '%s' is %s\n", token, filename );
	}

	// read information for each frame
	for ( i = 0 ; i < MAX_ANIMATIONS ; i++ ) {

		token = COM_Parse( &text_p );
		if ( !token ) {
			break;
		}
		animations[i].firstFrame = atoi( token );
		// leg only frames are adjusted to not count the upper body only frames
		if ( i == LEGS_WALKCR ) {
			skip = animations[LEGS_WALKCR].firstFrame - animations[TORSO_GESTURE].firstFrame;
		}
		if ( i >= LEGS_WALKCR ) {
			animations[i].firstFrame -= skip;
		}

		token = COM_Parse( &text_p );
		if ( !token ) {
			break;
		}
		animations[i].numFrames = atoi( token );

		token = COM_Parse( &text_p );
		if ( !token ) {
			break;
		}
		animations[i].loopFrames = atoi( token );

		token = COM_Parse( &text_p );
		if ( !token ) {
			break;
		}
		fps = atof( token );
		if ( fps == 0 ) {
			fps = 1;
		}
		animations[i].frameLerp = 1000 / fps;
		animations[i].initialLerp = 1000 / fps;
	}

	if ( i != MAX_ANIMATIONS ) {
		Com_Printf( "Error parsing animation file: %s", filename );
		return qfalse;
	}

	return qtrue;
}

/*
==========================
UI_RegisterClientModelname
==========================
*/
qboolean UI_RegisterClientModelname( modelAnim_t* m)
{
	playerInfo_t *pi;

	// separated skin and model names
	char		bodyName[MAX_QPATH];
	char		headName[MAX_QPATH];
	char		legsName[MAX_QPATH];
	char		skinBodyName[MAX_QPATH];
	char		skinHeadName[MAX_QPATH];
	char		skinLegsName[MAX_QPATH];

	char		filename[MAX_QPATH];
	char		*slash;

	// combined model and skin name, before splitting
	char		*modelSkinName, *modelHeadName, *modelLegsName;

	pi = &m->player;

	pi->torsoModel = 0;
	pi->headModel = 0;

	// define which model we're working with
	if (GUI_PlayerInfo_IsTeamModel())
	{
		modelSkinName = m->team_modelskin;
		modelHeadName = m->team_headskin;
		modelLegsName = m->team_legsskin;
	}
	else
	{
		modelSkinName = m->modelskin;
		modelHeadName = m->headskin;
		modelLegsName = m->legsskin;
	}

	// validate
	m->bUnknownModel = qfalse;
	m->bUnknownHeadModel = qfalse;
	m->bUnknownLegsModel = qfalse;

	// do the body model
	Q_strncpyz( bodyName, modelSkinName, sizeof( bodyName ) );

	slash = strchr( bodyName, '/' );
	if ( !slash ) {
		// modelName did not include a skin name
		Q_strncpyz( skinBodyName, "default", sizeof( skinBodyName ) );
	} else {
		Q_strncpyz( skinBodyName, slash + 1, sizeof( skinBodyName ) );
		// truncate bodyName
		*slash = 0;
	}
	
	// do the legs model
	Q_strncpyz( legsName, modelLegsName, sizeof( legsName ) );

	slash = strchr( legsName, '/' );
	if ( !slash ) {
		// modelName did not include a skin name
		Q_strncpyz( skinLegsName, "default", sizeof( skinLegsName ) );
	} else {
		Q_strncpyz( skinLegsName, slash + 1, sizeof( skinLegsName ) );
		// truncate bodyName
		*slash = 0;
	}

	// do the head model separately
	Q_strncpyz( headName, modelHeadName, sizeof( headName ) );

	slash = strchr( headName, '/' );
	if ( !slash ) {
		// modelName did not include a skin name
		Q_strncpyz( skinHeadName, "default", sizeof( skinHeadName ) );
	} else {
		Q_strncpyz( skinHeadName, slash + 1, sizeof( skinHeadName ) );
		// truncate modelName
		*slash = 0;
	}

	// load cmodels before models so filecache works
	Com_sprintf( filename, sizeof( filename ), "models/players/%s/lower.md3", bodyName );
	pi->legsModel = trap_R_RegisterModel( filename );
	if ( !pi->legsModel ) {
		Com_Printf( "Failed to load model file %s\n", filename );
		m->bUnknownModel = qtrue;
		return qfalse;
	}

	Com_sprintf( filename, sizeof( filename ), "models/players/%s/upper.md3", bodyName );
	pi->torsoModel = trap_R_RegisterModel( filename );
	if ( !pi->torsoModel ) {
		Com_Printf( "Failed to load model file %s\n", filename );
		m->bUnknownModel = qtrue;
		return qfalse;
	}

	Com_sprintf( filename, sizeof( filename ), "models/players/%s/head.md3", headName );
	pi->headModel = trap_R_RegisterModel( filename );
	if ( !pi->headModel ) {
		Com_Printf( "Failed to load model file %s\n", filename );
		m->bUnknownHeadModel = qtrue;
		return qfalse;
	}

	// this is the body part of the model
	UI_RegisterClientSkin( pi, bodyName, skinBodyName, qfalse, qfalse, qtrue );

	// and this is the head part of the model
	UI_RegisterClientSkin( pi, headName, skinHeadName, qtrue, qfalse, qfalse );
	
	// and this is the legs part of the model
	UI_RegisterClientSkin( pi, legsName, skinLegsName, qfalse, qtrue, qfalse );

	// load the animations
	// always for the body, never the head!
	Com_sprintf( filename, sizeof( filename ), "models/players/%s/animation.cfg", bodyName );
	if ( !UI_ParseAnimationFile( filename, pi ) ) {
		Com_Printf( "Failed to load animation file %s\n", filename );
		return qfalse;
	}

	return qtrue;
}

/*
===============
UI_PlayerInfo_SetModel
===============
*/
static qboolean UI_PlayerInfo_SetModel( modelAnim_t* m )
{
	playerInfo_t *pi;

	pi = &m->player;
	if (!UI_RegisterClientModelname( m ))
		return qfalse;

	pi->newModel = qtrue;

	return qtrue;
}

/*
=================
PlayerInfo_SetupNewModel
=================
*/
static void PlayerInfo_SetupNewModel( modelAnim_t* m)
{
	char dir[MODELNAME_BUFFER];
	int i;
	char* fallback;
	char* slash;
	char* str;
	vec3_t tmp_view;
	vec3_t tmp_move;

	VectorClear(tmp_view);
	VectorClear(tmp_move);
	tmp_view[YAW] = 180 - 30;

	memset( &m->player, 0, sizeof(playerInfo_t));

	VectorCopy(tmp_view, m->viewangles);
	VectorCopy(tmp_move, m->moveangles);
	VectorCopy(tmp_view, m->player.viewAngles);
	VectorCopy(tmp_move, m->player.moveAngles);

	m->bUnknownModel = qfalse;
	if (!UI_PlayerInfo_SetModel( m )) {
		m->bForceUpdate = qfalse;
		return;
	}

	// get model name only, handle case where "/default" assumed
	slash = 0;
	fallback = DEFAULT_MODEL;
	if (GUI_PlayerInfo_IsTeamModel())
		str = m->team_modelskin;
	else
		str = m->modelskin;

	slash = strchr(str, '/');
	if (slash)
	{
		Q_strncpyz(dir, str, slash - str + 1);
	} else {
		strcpy(dir, str);
	}

	// model change done
	m->cursorx = m->cursory = -1;
	m->NextIdleAnimTime = 0;
}

/*
=================
GUI_PlayerInfo_InitModel
=================
*/
void GUI_PlayerInfo_InitModel(modelAnim_t* m)
{
	char 	buffer[MODELNAME_BUFFER];
	int 	i;

	trap_Cvar_VariableStringBuffer( "model", buffer, MODELNAME_BUFFER );
	strcpy(m->modelskin, buffer);

	trap_Cvar_VariableStringBuffer( "headmodel", buffer, MODELNAME_BUFFER );
	strcpy(m->headskin, buffer);
	
	trap_Cvar_VariableStringBuffer( "legsskin", buffer, MODELNAME_BUFFER );
	strcpy(m->legsskin, buffer);

	trap_Cvar_VariableStringBuffer( "team_model", buffer, MODELNAME_BUFFER );
	strcpy(m->team_modelskin, buffer);

	trap_Cvar_VariableStringBuffer( "team_headmodel", buffer, MODELNAME_BUFFER );
	strcpy(m->team_headskin, buffer);
	
	trap_Cvar_VariableStringBuffer( "team_legsskin", buffer, MODELNAME_BUFFER );
	strcpy(m->team_legsskin, buffer);

	// init the recent animations history
	for (i = 0; i < MAX_RECENT_ANIMS; i++) {
		m->recent_anims[i] = ANIM_IDLE;
	}

	PlayerInfo_SetupNewModel(m);
}

/*
=================
GUI_PlayerInfo_SetTeamModel
=================
*/
void GUI_PlayerInfo_DrawTeamModel( modelAnim_t* m, qboolean teamModel )
{
	if (teamModel != drawTeamModel)
	{
		drawTeamModel = teamModel;
		m->bForceUpdate = qtrue;
	}
}

/*
=================
GUI_PlayerInfo_IsTeamModel
=================
*/
qboolean GUI_PlayerInfo_IsTeamModel( void )
{
	return drawTeamModel;
}

/*
=================
GUI_ModelSkin
=================
*/
const char* GUI_ModelSkin( const char* modelname )
{
	static char skin[MODELNAME_BUFFER];
	char* ptr;

	skin[0] = 0;
	if (!modelname || !modelname[0])
	{
		ptr = "default";
	}
	else
	{
		ptr = strchr(modelname, '/');
		if (!ptr) {
			ptr = "default";
		}
		else {
			ptr++;
		}
	}

	strcpy(skin, ptr);
	return skin;
}

/*
=================
GUI_ModelName
=================
*/
const char* GUI_ModelName( const char* modelname )
{
	static char model[MODELNAME_BUFFER];
	char* ptr;

	model[0] = 0;
	if (!modelname || !modelname[0])
	{
		strcpy(model, DEFAULT_MODEL);
		return model;
	}


	ptr = strchr(modelname, '/');

	if (ptr)
		Q_strncpyz(model, modelname, ptr - modelname + 1);
	else {
		strcpy(model, modelname);
	}

	return model;
}

/*
=================
GUI_PlayerInfo_AnimateModel
=================
*/
void GUI_PlayerInfo_AnimateModel( modelAnim_t* m)
{
	menuelement_s* b;
	char buffer[MODELNAME_BUFFER];
	int modelchange, team_modelchange, draw_team;

	b = &m->bitmap;

	if( trap_MemoryRemaining() <= LOW_MEMORY ) {
		UI_DrawString( b->generic.x, b->generic.y + b->height / 2, "LOW MEMORY", UI_LEFT, color_white );
		return;
	}

	// check if model has changed
	// only update models when we're actually viewing them
	modelchange = 0;
	team_modelchange = 0;
	draw_team = GUI_PlayerInfo_IsTeamModel();
		trap_Cvar_VariableStringBuffer( "model", buffer, MODELNAME_BUFFER );
		if (Q_stricmp(buffer, m->modelskin) ) {
			strcpy(m->modelskin, buffer);
			modelchange = 1;
		}

		trap_Cvar_VariableStringBuffer( "headmodel", buffer, MODELNAME_BUFFER );
		if (Q_stricmp(buffer, m->headskin) ) {
			strcpy(m->headskin, buffer);
			modelchange = 1;
		}
		
		trap_Cvar_VariableStringBuffer( "legsskin", buffer, MODELNAME_BUFFER );
		if (Q_stricmp(buffer, m->legsskin) ) {
			strcpy(m->legsskin, buffer);
			modelchange = 1;
		}

		trap_Cvar_VariableStringBuffer( "team_model", buffer, MODELNAME_BUFFER );
		if (Q_stricmp(buffer, m->team_modelskin) ) {
			strcpy(m->team_modelskin, buffer);
			team_modelchange = 1;
		}

		trap_Cvar_VariableStringBuffer( "team_headmodel", buffer, MODELNAME_BUFFER );
		if (Q_stricmp(buffer, m->team_headskin) ) {
			strcpy(m->team_headskin, buffer);
			team_modelchange = 1;
		}
		
		trap_Cvar_VariableStringBuffer( "team_legsskin", buffer, MODELNAME_BUFFER );
		if (Q_stricmp(buffer, m->team_legsskin) ) {
			strcpy(m->team_legsskin, buffer);
			team_modelchange = 1;
		}

		if ((modelchange && !draw_team) || (team_modelchange && draw_team))
			m->bForceUpdate = qtrue;

		PlayerInfo_SetupNewModel(m);

	if( m->bUnknownModel ) {
		UI_DrawString( b->generic.x, b->generic.y + b->height / 2, "UNKNOWN MODEL", UI_LEFT, color_white );
		return;
	}

	if( m->bUnknownHeadModel ) {
		UI_DrawString( b->generic.x, b->generic.y + b->height / 2, "UNKNOWN HEAD", UI_LEFT, color_white );
		return;
	}
	
	if( m->bUnknownLegsModel ) {
		UI_DrawString( b->generic.x, b->generic.y + b->height / 2, "UNKNOWN LEGS", UI_LEFT, color_white );
		return;
	}

	m->bForceUpdate = qfalse;

	UI_DrawPlayer( b->generic.x, b->generic.y, b->width, b->height, m );
}
