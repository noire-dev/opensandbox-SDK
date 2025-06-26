// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

#include "cg_local.h"

char	*cg_customSoundNames[MAX_CUSTOM_SOUNDS] = {
	"*death1.wav",
	"*death2.wav",
	"*death3.wav",
	"*jump1.wav",
	"*pain25_1.wav",
	"*pain50_1.wav",
	"*pain75_1.wav",
	"*pain100_1.wav",
	"*falling1.wav",
	"*gasp.wav",
	"*drown.wav",
	"*fall1.wav",
	"*taunt.wav"
};

// leilei - eyes hack
vec3_t headpos;
vec3_t headang;

/*
================
CG_CustomSound

================
*/
sfxHandle_t	CG_CustomSound( int clientNum, const char *soundName ) {
	clientInfo_t *ci;
	int			i;

	if ( soundName[0] != '*' ) {
		return trap_S_RegisterSound( soundName, qfalse );
	}

	if ( clientNum < 0 || clientNum >= MAX_CLIENTS ) {
		clientNum = 0;
	}
	ci = &cgs.clientinfo[ clientNum ];

	for ( i = 0 ; i < MAX_CUSTOM_SOUNDS && cg_customSoundNames[i] ; i++ ) {
		if ( !strcmp( soundName, cg_customSoundNames[i] ) ) {
			return ci->sounds[i];
		}
	}

	CG_Error( "Unknown custom sound: %s", soundName );
	return 0;
}

/*
=============================================================================

CLIENT INFO

=============================================================================
*/

/*
======================
CG_ParseAnimationFile

Read a configuration file containing animation coutns and rates
models/players/visor/animation.cfg, etc
======================
*/
static qboolean	CG_ParseAnimationFile( const char *filename, clientInfo_t *ci ) {
	char		*text_p, *prev;
	int			len;
	int			i;
	char		*token;
	float		fps;
	int			skip;
	char		text[20000];
	fileHandle_t	f;
	animation_t *animations;

	animations = ci->animations;

	// load the file
	len = trap_FS_FOpenFile( filename, &f, FS_READ );
	if ( len <= 0 ) {
		return qfalse;
	}
	if ( len >= sizeof( text ) - 1 ) {
		CG_Printf( "File %s too long\n", filename );
		trap_FS_FCloseFile( f );
		return qfalse;
	}
	trap_FS_Read( text, len, f );
	text[len] = 0;
	trap_FS_FCloseFile( f );

	// parse the text
	text_p = text;
	skip = 0;	// quite the compiler warning

	ci->footsteps = FOOTSTEP_NORMAL;
	VectorClear( ci->headOffset );
	ci->gender = GENDER_MALE;
	ci->fixedlegs = qfalse;
	ci->fixedtorso = qfalse;

	// read optional parameters
	while ( 1 ) {
		prev = text_p;	// so we can unget
		token = COM_Parse( &text_p );
		if ( !token ) {
			break;
		}
		if ( !Q_stricmp( token, "footsteps" ) ) {
			token = COM_Parse( &text_p );
			if ( !token ) {
				break;
			}
			if ( !Q_stricmp( token, "default" ) || !Q_stricmp( token, "normal" ) ) {
				ci->footsteps = FOOTSTEP_NORMAL;
			} else if ( !Q_stricmp( token, "boot" ) ) {
				ci->footsteps = FOOTSTEP_BOOT;
			} else if ( !Q_stricmp( token, "flesh" ) ) {
				ci->footsteps = FOOTSTEP_FLESH;
			} else if ( !Q_stricmp( token, "mech" ) ) {
				ci->footsteps = FOOTSTEP_MECH;
			} else if ( !Q_stricmp( token, "energy" ) ) {
				ci->footsteps = FOOTSTEP_ENERGY;
			} else {
				CG_Printf( "Bad footsteps parm in %s: %s\n", filename, token );
			}
			continue;
		} else if ( !Q_stricmp( token, "headoffset" ) ) {
			for ( i = 0 ; i < 3 ; i++ ) {
				token = COM_Parse( &text_p );
				if ( !token ) {
					break;
				}
				ci->headOffset[i] = atof( token );
			}
			continue;
		} else if ( !Q_stricmp( token, "sex" ) ) {
			token = COM_Parse( &text_p );
			if ( !token ) {
				break;
			}
			if ( token[0] == 'f' || token[0] == 'F' ) {
				ci->gender = GENDER_FEMALE;
			} else if ( token[0] == 'n' || token[0] == 'N' ) {
				ci->gender = GENDER_NEUTER;
			} else {
				ci->gender = GENDER_MALE;
			}
			continue;
		} else if ( !Q_stricmp( token, "fixedlegs" ) ) {
			ci->fixedlegs = qtrue;
			continue;
		} else if ( !Q_stricmp( token, "fixedtorso" ) ) {
			ci->fixedtorso = qtrue;
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
		if ( !*token ) {
			if( i >= TORSO_GETFLAG && i <= TORSO_NEGATIVE ) {
				animations[i].firstFrame = animations[TORSO_GESTURE].firstFrame;
				animations[i].frameLerp = animations[TORSO_GESTURE].frameLerp;
				animations[i].initialLerp = animations[TORSO_GESTURE].initialLerp;
				animations[i].loopFrames = animations[TORSO_GESTURE].loopFrames;
				animations[i].numFrames = animations[TORSO_GESTURE].numFrames;
				animations[i].reversed = qfalse;
				animations[i].flipflop = qfalse;
				continue;
			}
			break;
		}
		animations[i].firstFrame = atoi( token );
		// leg only frames are adjusted to not count the upper body only frames
		if ( i == LEGS_WALKCR ) {
			skip = animations[LEGS_WALKCR].firstFrame - animations[TORSO_GESTURE].firstFrame;
		}
		if ( i >= LEGS_WALKCR && i<TORSO_GETFLAG) {
			animations[i].firstFrame -= skip;
		}

		token = COM_Parse( &text_p );
		if ( !*token ) {
			break;
		}
		animations[i].numFrames = atoi( token );

		animations[i].reversed = qfalse;
		animations[i].flipflop = qfalse;
		// if numFrames is negative the animation is reversed
		if (animations[i].numFrames < 0) {
			animations[i].numFrames = -animations[i].numFrames;
			animations[i].reversed = qtrue;
		}

		token = COM_Parse( &text_p );
		if ( !*token ) {
			break;
		}
		animations[i].loopFrames = atoi( token );

		token = COM_Parse( &text_p );
		if ( !*token ) {
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
		CG_Printf( "Error parsing animation file: %s\n", filename );
		return qfalse;
	}

	// crouch backward animation
	memcpy(&animations[LEGS_BACKCR], &animations[LEGS_WALKCR], sizeof(animation_t));
	animations[LEGS_BACKCR].reversed = qtrue;
	// walk backward animation
	memcpy(&animations[LEGS_BACKWALK], &animations[LEGS_WALK], sizeof(animation_t));
	animations[LEGS_BACKWALK].reversed = qtrue;
	// flag moving fast
	animations[FLAG_RUN].firstFrame = 0;
	animations[FLAG_RUN].numFrames = 16;
	animations[FLAG_RUN].loopFrames = 16;
	animations[FLAG_RUN].frameLerp = 1000 / 15;
	animations[FLAG_RUN].initialLerp = 1000 / 15;
	animations[FLAG_RUN].reversed = qfalse;
	// flag not moving or moving slowly
	animations[FLAG_STAND].firstFrame = 16;
	animations[FLAG_STAND].numFrames = 5;
	animations[FLAG_STAND].loopFrames = 0;
	animations[FLAG_STAND].frameLerp = 1000 / 20;
	animations[FLAG_STAND].initialLerp = 1000 / 20;
	animations[FLAG_STAND].reversed = qfalse;
	// flag speeding up
	animations[FLAG_STAND2RUN].firstFrame = 16;
	animations[FLAG_STAND2RUN].numFrames = 5;
	animations[FLAG_STAND2RUN].loopFrames = 1;
	animations[FLAG_STAND2RUN].frameLerp = 1000 / 15;
	animations[FLAG_STAND2RUN].initialLerp = 1000 / 15;
	animations[FLAG_STAND2RUN].reversed = qtrue;
	//
	// new anims changes
	//
	animations[TORSO_GETFLAG].flipflop = qtrue;
	animations[TORSO_GUARDBASE].flipflop = qtrue;
	animations[TORSO_PATROL].flipflop = qtrue;
	animations[TORSO_AFFIRMATIVE].flipflop = qtrue;
	animations[TORSO_NEGATIVE].flipflop = qtrue;
	//
	return qtrue;
}

/*
==========================
CG_FileExists
==========================
*/
static qboolean	CG_FileExists(const char *filename) {
	int len;

	len = trap_FS_FOpenFile( filename, NULL, FS_READ );
	if (len>0) {
		return qtrue;
	}
	return qfalse;
}

/*
==========================
CG_RegisterClientSkin
==========================
*/
static qboolean	CG_RegisterClientSkin( clientInfo_t *ci, const char *modelName, const char *skinName, const char *headModelName, const char *headSkinName, const char *legsModelName, const char *legsSkinName ) {
	char filename[MAX_QPATH];

	Com_sprintf( filename, sizeof( filename ), "models/players/%s/lower_%s.skin", modelName, legsSkinName );
	ci->legsSkin = trap_R_RegisterSkin( filename );
	if (!ci->legsSkin) {
		Com_sprintf( filename, sizeof( filename ), "models/players/%s/lower_%s", modelName, legsSkinName );
		ci->legsShader = trap_R_RegisterShader( filename );
	}

	Com_sprintf( filename, sizeof( filename ), "models/players/%s/upper_%s.skin", modelName, skinName );
	ci->torsoSkin = trap_R_RegisterSkin( filename );
	if (!ci->torsoSkin) {
		Com_sprintf( filename, sizeof( filename ), "models/players/%s/upper_%s", modelName, skinName );
		ci->torsoShader = trap_R_RegisterShader( filename );
	}
	
	Com_sprintf( filename, sizeof( filename ), "models/players/%s/head_%s.skin", headModelName, headSkinName );
	ci->headSkin = trap_R_RegisterSkin( filename );
	if (!ci->headSkin) {
		Com_sprintf( filename, sizeof( filename ), "models/players/%s/head_%s", headModelName, headSkinName );
		ci->headShader = trap_R_RegisterShader( filename );
	}

	// if one skin loaded
	if ( !ci->legsSkin && !ci->torsoSkin && !ci->headSkin && !ci->legsShader && !ci->torsoShader && !ci->headShader ) {
		return qfalse;
	}
return qtrue;
}

/*
==========================
CG_RegisterClientModelname
==========================
*/
static qboolean CG_RegisterClientModelname( clientInfo_t *ci, const char *modelName, const char *skinName, const char *headModelName, const char *headSkinName, const char *teamName, const char *legsModelName, const char *legsSkinName ) {
	char	filename[MAX_QPATH*2];
	const char		*headName;

	if ( headModelName[0] == '\0' ) {
		headName = modelName;
	}
	else {
		headName = headModelName;
	}
	Com_sprintf( filename, sizeof( filename ), "models/players/%s/lower.md3", modelName );
	ci->legsModel = trap_R_RegisterModel( filename );
	if ( !ci->legsModel ) {
		Com_Printf( "Failed to load model file %s\n", filename );
		return qfalse;
	}

	Com_sprintf( filename, sizeof( filename ), "models/players/%s/upper.md3", modelName );
	ci->torsoModel = trap_R_RegisterModel( filename );
	if ( !ci->torsoModel ) {
		Com_Printf( "Failed to load model file %s\n", filename );
		return qfalse;
	}

	Com_sprintf( filename, sizeof( filename ), "models/players/%s/head.md3", headName );
	ci->headModel = trap_R_RegisterModel( filename );
	if ( !ci->headModel ) {
		Com_Printf( "Failed to load model file %s\n", filename );
		return qfalse;
	}

	// if any skins failed to load, return failure
	if ( !CG_RegisterClientSkin( ci, modelName, skinName, headName, headSkinName, legsModelName, legsSkinName ) ) {
		//Com_Printf( "Failed to load skin file: %s : %s : %s, %s : %s\n", newTeamName, modelName, skinName, headName, headSkinName );
		return qfalse;
	}
	

	// load the animations
	Com_sprintf( filename, sizeof( filename ), "models/players/%s/animation.cfg", modelName );
	if ( !CG_ParseAnimationFile( filename, ci ) ) {
		Com_sprintf( filename, sizeof( filename ), "models/players/characters/%s/animation.cfg", modelName );
		if ( !CG_ParseAnimationFile( filename, ci ) ) {
			Com_Printf( "Failed to load animation file %s\n", filename );
			return qfalse;
		}
	}
	
	if(!ci->modelIcon){
	Com_sprintf( filename, sizeof( filename ), "models/players/%s/icon_%s.tga", modelName, skinName );
	if(CG_FileExists(filename)){
		ci->modelIcon = trap_R_RegisterShaderNoMip( filename );
	}
	}
	if(!ci->modelIcon){
	Com_sprintf( filename, sizeof( filename ), "models/players/%s/icon_%s.png", modelName, skinName );
	if(CG_FileExists(filename)){
		ci->modelIcon = trap_R_RegisterShaderNoMip( filename );
	}
	}
	if(!ci->modelIcon){
	Com_sprintf( filename, sizeof( filename ), "models/players/%s/icon_%s.jpg", modelName, skinName );
	if(CG_FileExists(filename)){
		ci->modelIcon = trap_R_RegisterShaderNoMip( filename );
	}
	}
	if(!ci->modelIcon){
	Com_sprintf( filename, sizeof( filename ), "models/players/%s/icon_%s.bmp", modelName, skinName );
	if(CG_FileExists(filename)){
		ci->modelIcon = trap_R_RegisterShaderNoMip( filename );
	}
	}

	if ( !ci->modelIcon ) {
		return qfalse;
	}

	return qtrue;
}

/*
==========================
CG_ReloadClientModelname
==========================
*/
static qboolean CG_ReloadClientModelname( clientInfo_t *ci, const char *modelName, const char *skinName, const char *headModelName, const char *headSkinName, const char *teamName, const char *legsModelName, const char *legsSkinName ) {
	const char		*headName;

	if ( headModelName[0] == '\0' ) {
		headName = modelName;
	} else {
		headName = headModelName;
	}

	// if any skins failed to load, return failure
	if ( !CG_RegisterClientSkin( ci, modelName, skinName, headName, headSkinName, legsModelName, legsSkinName ) ) {
		return qfalse;
	}

	if ( !ci->modelIcon ) {
		return qfalse;
	}

	return qtrue;
}

/*
===================
CG_LoadClientInfo

Load it now, taking the disk hits.
This will usually be deferred to a safe time
===================
*/
static void CG_LoadClientInfo( int clientNum, clientInfo_t *ci ) {
	const char	*dir, *fallback;
	int			i, modelloaded;
	const char	*s;
	char		teamname[MAX_QPATH];

	teamname[0] = 0;

	modelloaded = qtrue;
	if ( !CG_RegisterClientModelname( ci, ci->modelName, ci->skinName, ci->headModelName, ci->headSkinName, teamname, ci->legsModelName, ci->legsSkinName ) ) {

		// fall back to default team name
		if( cgs.gametype >= GT_TEAM ) {
			// keep skin name
			if( ci->team == TEAM_BLUE ) {
				Q_strncpyz(teamname, DEFAULT_BLUETEAM_NAME, sizeof(teamname) );
			} else {
				Q_strncpyz(teamname, DEFAULT_REDTEAM_NAME, sizeof(teamname) );
			}
			if ( !CG_RegisterClientModelname( ci, DEFAULT_TEAM_MODEL, ci->skinName, DEFAULT_TEAM_HEAD, ci->skinName, teamname, "default", "default" ) ) {
			}
		} else {
			if ( !CG_RegisterClientModelname( ci, DEFAULT_MODEL, "default", DEFAULT_MODEL, "default", teamname, "default", "default" ) ) {
			}
		}
		modelloaded = qfalse;
	}

	// sounds
	dir = ci->modelName;
	fallback = cgs.gametype >= GT_TEAM ? DEFAULT_TEAM_MODEL : DEFAULT_MODEL;

	for ( i = 0 ; i < MAX_CUSTOM_SOUNDS ; i++ ) {
		s = cg_customSoundNames[i];
		if ( !s ) {
			break;
		}
		ci->sounds[i] = 0;
		// if the model didn't load use the sounds of the default model
		if (modelloaded) {
			ci->sounds[i] = trap_S_RegisterSound( va("sound/player/%s/%s", dir, s + 1), qfalse );
		}
		if ( !ci->sounds[i] ) {
			ci->sounds[i] = trap_S_RegisterSound( va("sound/player/%s/%s", fallback, s + 1), qfalse );
		}
	}

	ci->deferred = qfalse;

	// reset any existing players and bodies, because they might be in bad
	// frames for this new model
	for ( i = 0 ; i < MAX_GENTITIES ; i++ ) {
		if ( cg_entities[i].currentState.clientNum == clientNum
			&& cg_entities[i].currentState.eType == ET_PLAYER ) {
			CG_ResetPlayerEntity( &cg_entities[i] );
		}
	}
}

/*
===================
CG_ReloadClientInfo

Load it now, taking the disk hits.
This will usually be deferred to a safe time
===================
*/
static void CG_ReloadClientInfo( int clientNum, clientInfo_t *ci ) {
	int			modelloaded;
	char		teamname[MAX_QPATH];

	teamname[0] = 0;

	modelloaded = qtrue;
	if ( !CG_ReloadClientModelname( ci, ci->modelName, ci->skinName, ci->headModelName, ci->headSkinName, teamname, ci->legsModelName, ci->legsSkinName ) ) {

		// fall back to default team name
		if( cgs.gametype >= GT_TEAM ) {
			// keep skin name
			if( ci->team == TEAM_BLUE ) {
				Q_strncpyz(teamname, DEFAULT_BLUETEAM_NAME, sizeof(teamname) );
			} else {
				Q_strncpyz(teamname, DEFAULT_REDTEAM_NAME, sizeof(teamname) );
			}
			if ( !CG_ReloadClientModelname( ci, DEFAULT_TEAM_MODEL, ci->skinName, DEFAULT_TEAM_HEAD, ci->skinName, teamname, "default", "default" ) ) {
			}
		} else {
			if ( !CG_ReloadClientModelname( ci, DEFAULT_MODEL, "default", DEFAULT_MODEL, "default", teamname, "default", "default" ) ) {
			}
		}
		modelloaded = qfalse;
	}
}

/*
======================
CG_CopyClientInfoModel
======================
*/
static void CG_CopyClientInfoModel( clientInfo_t *from, clientInfo_t *to ) {
	VectorCopy( from->headOffset, to->headOffset );
	to->footsteps = from->footsteps;
	to->gender = from->gender;

	to->legsModel = from->legsModel;
	to->legsSkin = from->legsSkin;
	to->legsShader = from->legsShader;
	to->torsoModel = from->torsoModel;
	to->torsoSkin = from->torsoSkin;
	to->torsoShader = from->torsoShader;
	to->headModel = from->headModel;
	to->headSkin = from->headSkin;
	to->headShader = from->headShader;
	to->modelIcon = from->modelIcon;

	memcpy( to->animations, from->animations, sizeof( to->animations ) );
	memcpy( to->sounds, from->sounds, sizeof( to->sounds ) );
}

/*
======================
CG_ScanForExistingClientInfo
======================
*/
static qboolean CG_ScanForExistingClientInfo( clientInfo_t *ci ) {
	int		i;
	clientInfo_t	*match;

	for ( i = 0 ; i < cgs.maxclients ; i++ ) {
		match = &cgs.clientinfo[ i ];
		if ( !match->infoValid ) {
			continue;
		}
		if ( match->deferred ) {
			continue;
		}
		if ( !Q_stricmp( ci->modelName, match->modelName )
			&& !Q_stricmp( ci->skinName, match->skinName )
			&& !Q_stricmp( ci->headModelName, match->headModelName )
			&& !Q_stricmp( ci->headSkinName, match->headSkinName )
			&& (cgs.gametype < GT_TEAM || ci->team == match->team) ) {
			// this clientinfo is identical, so use it's handles

			ci->deferred = qfalse;

			CG_CopyClientInfoModel( match, ci );

			return qtrue;
		}
	}

	// nothing matches, so defer the load
	return qfalse;
}

/*
======================
CG_NewClientInfo
======================
*/
void CG_NewClientInfo( int clientNum ) {
	clientInfo_t *ci;
	clientInfo_t newInfo;
	const char	*configstring;
	const char	*v;
	char		*slash;

	ci = &cgs.clientinfo[clientNum];

	configstring = CG_ConfigString( clientNum + CS_PLAYERS );
	if ( !configstring[0] ) {
		memset( ci, 0, sizeof( *ci ) );
		return;		// player just left
	}

	// build into a temp buffer so the defer checks can use
	// the old value
	memset( &newInfo, 0, sizeof( newInfo ) );

	// isolate the player's name
	v = Info_ValueForKey(configstring, "n");
	Q_strncpyz( newInfo.name, v, sizeof( newInfo.name ) );
	
	v = Info_ValueForKey( configstring, "vn" );
	newInfo.vehicleNum = atoi( v );

	// npc?
	v = Info_ValueForKey( configstring, "i" );
	newInfo.isNPC = atoi( v );

	// bot skill
	v = Info_ValueForKey( configstring, "s" );
	newInfo.botSkill = atoi( v );

	// team
	v = Info_ValueForKey( configstring, "t" );
	newInfo.team = atoi( v );

	// team task
	v = Info_ValueForKey( configstring, "tt" );
	newInfo.teamTask = atoi(v);

	// team leader
	v = Info_ValueForKey( configstring, "tl" );
	newInfo.teamLeader = atoi(v);

	// flashlight
	v = Info_ValueForKey( configstring, "f" );
	newInfo.flashlight = atoi(v);

	// model
	v = Info_ValueForKey( configstring, "m" );

	Q_strncpyz( newInfo.modelName, v, sizeof( newInfo.modelName ) );

	slash = strchr( newInfo.modelName, '/' );
	if ( !slash ) {
		// modelName didn not include a skin name
		Q_strncpyz( newInfo.skinName, "default", sizeof( newInfo.skinName ) );
	} else {
		Q_strncpyz( newInfo.skinName, slash + 1, sizeof( newInfo.skinName ) );
		// truncate modelName
		*slash = 0;
	}

	// head model
	v = Info_ValueForKey( configstring, "hm" );
	
	Q_strncpyz( newInfo.headModelName, v, sizeof( newInfo.headModelName ) );

	slash = strchr( newInfo.headModelName, '/' );
	if ( !slash ) {
		// modelName didn not include a skin name
		Q_strncpyz( newInfo.headSkinName, "default", sizeof( newInfo.headSkinName ) );
	} else {
		Q_strncpyz( newInfo.headSkinName, slash + 1, sizeof( newInfo.headSkinName ) );
		// truncate modelName
		*slash = 0;
	}

	// legs model
	v = Info_ValueForKey( configstring, "lm" );

	Q_strncpyz( newInfo.legsModelName, v, sizeof( newInfo.legsModelName ) );

	slash = strchr( newInfo.legsModelName, '/' );
	if ( !slash ) {
		// modelName didn not include a skin name
		Q_strncpyz( newInfo.legsSkinName, "default", sizeof( newInfo.legsSkinName ) );
	} else {
		Q_strncpyz( newInfo.legsSkinName, slash + 1, sizeof( newInfo.legsSkinName ) );
		// truncate modelName
		*slash = 0;
	}

	//Colors
	v = Info_ValueForKey( configstring, "hr" );
	newInfo.headR = atoi( v );

	v = Info_ValueForKey( configstring, "hg" );
	newInfo.headG = atoi( v );

	v = Info_ValueForKey( configstring, "hb" );
	newInfo.headB = atoi( v );

	v = Info_ValueForKey( configstring, "mr" );
	newInfo.modelR = atoi( v );

	v = Info_ValueForKey( configstring, "mg" );
	newInfo.modelG = atoi( v );

	v = Info_ValueForKey( configstring, "mb" );
	newInfo.modelB = atoi( v );

	v = Info_ValueForKey( configstring, "lr" );
	newInfo.legsR = atoi( v );

	v = Info_ValueForKey( configstring, "lg" );
	newInfo.legsG = atoi( v );

	v = Info_ValueForKey( configstring, "lb" );
	newInfo.legsB = atoi( v );
	
	v = Info_ValueForKey( configstring, "pr" );
	newInfo.physR = atoi( v );

	v = Info_ValueForKey( configstring, "pg" );
	newInfo.physG = atoi( v );

	v = Info_ValueForKey( configstring, "pb" );
	newInfo.physB = atoi( v );

	// scan for an existing clientinfo that matches this modelname
	// so we can avoid loading checks if possible
	if ( !CG_ScanForExistingClientInfo( &newInfo ) ) {
		CG_LoadClientInfo( clientNum, &newInfo );
	}

	// replace whatever was there with the new one
	newInfo.infoValid = qtrue;
	*ci = newInfo;
}

void CG_ReloadPlayers( void ) {
	int		i;
	clientInfo_t	*ci;

	for ( i = 0, ci = cgs.clientinfo ; i < cgs.maxclients ; i++, ci++ ) {
		if ( ci->infoValid ) {
			CG_ReloadClientInfo( i, ci );
		}
	}
}

/*
=============================================================================

PLAYER ANIMATION

=============================================================================
*/

/*
===============
CG_SetLerpFrameAnimation

may include ANIM_TOGGLEBIT
===============
*/
static void CG_SetLerpFrameAnimation( clientInfo_t *ci, lerpFrame_t *lf, int newAnimation ) {
	animation_t	*anim;

	lf->animationNumber = newAnimation;
	newAnimation &= ~ANIM_TOGGLEBIT;

	if ( newAnimation < 0 || newAnimation >= MAX_TOTALANIMATIONS ) {
		CG_Error( "Bad animation number: %i", newAnimation );
	}

	anim = &ci->animations[ newAnimation ];

	lf->animation = anim;
	lf->animationTime = lf->frameTime + anim->initialLerp;
}

/*
===============
CG_RunLerpFrame

Sets cg.snap, cg.oldFrame, and cg.backlerp
cg.time should be between oldFrameTime and frameTime after exit
===============
*/
static void CG_RunLerpFrame( clientInfo_t *ci, lerpFrame_t *lf, int newAnimation, float speedScale ) {
	int			f, numFrames;
	animation_t	*anim;

	// see if the animation sequence is switching
	if ( newAnimation != lf->animationNumber || !lf->animation ) {
		CG_SetLerpFrameAnimation( ci, lf, newAnimation );
	}

	// if we have passed the current frame, move it to
	// oldFrame and calculate a new frame
	if ( cg.time >= lf->frameTime ) {
		lf->oldFrame = lf->frame;
		lf->oldFrameTime = lf->frameTime;

		// get the next frame based on the animation
		anim = lf->animation;
		if ( !anim->frameLerp ) {
			return;		// shouldn't happen
		}
		if ( cg.time < lf->animationTime ) {
			lf->frameTime = lf->animationTime;		// initial lerp
		} else {
			lf->frameTime = lf->oldFrameTime + anim->frameLerp;
		}
		f = ( lf->frameTime - lf->animationTime ) / anim->frameLerp;
		f *= speedScale;		// adjust for haste, etc

		numFrames = anim->numFrames;
		if (anim->flipflop) {
			numFrames *= 2;
		}
		if ( f >= numFrames ) {
			f -= numFrames;
			if ( anim->loopFrames ) {
				f %= anim->loopFrames;
				f += anim->numFrames - anim->loopFrames;
			} else {
				f = numFrames - 1;
				// the animation is stuck at the end, so it
				// can immediately transition to another sequence
				lf->frameTime = cg.time;
			}
		}
		if ( anim->reversed ) {
			lf->frame = anim->firstFrame + anim->numFrames - 1 - f;
		}
		else if (anim->flipflop && f>=anim->numFrames) {
			lf->frame = anim->firstFrame + anim->numFrames - 1 - (f%anim->numFrames);
		}
		else {
			lf->frame = anim->firstFrame + f;
		}
		if ( cg.time > lf->frameTime ) {
			lf->frameTime = cg.time;
		}
	}

	if ( lf->frameTime > cg.time + 200 ) {
		lf->frameTime = cg.time;
	}

	if ( lf->oldFrameTime > cg.time ) {
		lf->oldFrameTime = cg.time;
	}
	// calculate current lerp value
	if ( lf->frameTime == lf->oldFrameTime ) {
		lf->backlerp = 0;
	} else {
		lf->backlerp = 1.0 - (float)( cg.time - lf->oldFrameTime ) / ( lf->frameTime - lf->oldFrameTime );
	}
}


/*
===============
CG_ClearLerpFrame
===============
*/
static void CG_ClearLerpFrame( clientInfo_t *ci, lerpFrame_t *lf, int animationNumber ) {
	lf->frameTime = lf->oldFrameTime = cg.time;
	CG_SetLerpFrameAnimation( ci, lf, animationNumber );
	lf->oldFrame = lf->frame = lf->animation->firstFrame;
}


/*
===============
CG_PlayerAnimation
===============
*/
static void CG_PlayerAnimation( centity_t *cent, int *legsOld, int *legs, float *legsBackLerp,
						int *torsoOld, int *torso, float *torsoBackLerp ) {
	clientInfo_t	*ci;
	int				clientNum;
	float			speedScale;

	clientNum = cent->currentState.clientNum;

	if ( cent->currentState.powerups & ( 1 << PW_HASTE ) ) {
		speedScale = 1.5;
	} else {
		speedScale = 1;
	}

	ci = &cgs.clientinfo[ clientNum ];

	// do the shuffle turn frames locally
	if ( cent->pe.legs.yawing && ( cent->currentState.legsAnim & ~ANIM_TOGGLEBIT ) == LEGS_IDLE ) {
		CG_RunLerpFrame( ci, &cent->pe.legs, LEGS_TURN, speedScale );
	} else {
		CG_RunLerpFrame( ci, &cent->pe.legs, cent->currentState.legsAnim, speedScale );
	}

	*legsOld = cent->pe.legs.oldFrame;
	*legs = cent->pe.legs.frame;
	*legsBackLerp = cent->pe.legs.backlerp;

	CG_RunLerpFrame( ci, &cent->pe.torso, cent->currentState.torsoAnim, speedScale );

	*torsoOld = cent->pe.torso.oldFrame;
	*torso = cent->pe.torso.frame;
	*torsoBackLerp = cent->pe.torso.backlerp;
}

/*
=============================================================================

PLAYER ANGLES

=============================================================================
*/

/*
==================
CG_SwingAngles
==================
*/
static void CG_SwingAngles( float destination, float swingTolerance, float clampTolerance,
					float speed, float *angle, qboolean *swinging ) {
	float	swing;
	float	move;
	float	scale;

	if ( !*swinging ) {
		// see if a swing should be started
		swing = AngleSubtract( *angle, destination );
		if ( swing > swingTolerance || swing < -swingTolerance ) {
			*swinging = qtrue;
		}
	}

	if ( !*swinging ) {
		return;
	}

	// modify the speed depending on the delta
	// so it doesn't seem so linear
	swing = AngleSubtract( destination, *angle );
	scale = fabs( swing );
	if ( scale < swingTolerance * 0.5 ) {
		scale = 0.5;
	} else if ( scale < swingTolerance ) {
		scale = 1.0;
	} else {
		scale = 2.0;
	}

	// swing towards the destination angle
	if ( swing >= 0 ) {
		move = cg.frametime * scale * speed;
		if ( move >= swing ) {
			move = swing;
			*swinging = qfalse;
		}
		*angle = AngleMod( *angle + move );
	} else if ( swing < 0 ) {
		move = cg.frametime * scale * -speed;
		if ( move <= swing ) {
			move = swing;
			*swinging = qfalse;
		}
		*angle = AngleMod( *angle + move );
	}

	// clamp to no more than tolerance
	swing = AngleSubtract( destination, *angle );
	if ( swing > clampTolerance ) {
		*angle = AngleMod( destination - (clampTolerance - 1) );
	} else if ( swing < -clampTolerance ) {
		*angle = AngleMod( destination + (clampTolerance - 1) );
	}
}

/*
=================
CG_AddPainTwitch
=================
*/
static void CG_AddPainTwitch( centity_t *cent, vec3_t torsoAngles ) {
	int		t;
	float	f;

	t = cg.time - cent->pe.painTime;
	if ( t >= PAIN_TWITCH_TIME ) {
		return;
	}

	f = 1.0 - (float)t / PAIN_TWITCH_TIME;

	if ( cent->pe.painDirection ) {
		torsoAngles[ROLL] += 20 * f;
	} else {
		torsoAngles[ROLL] -= 20 * f;
	}
}


/*
===============
CG_PlayerAngles

Handles seperate torso motion

  legs pivot based on direction of movement

  head always looks exactly at cent->lerpAngles

  if motion < 25 degrees, show in head only
  if < 40 degrees, also show in torso
===============
*/
#define L_ANIM ( cent->currentState.legsAnim & ~ANIM_TOGGLEBIT )
#define T_ANIM ( cent->currentState.torsoAnim & ~ANIM_TOGGLEBIT )

vec3_t		eyeat;

static void CG_PlayerAngles( centity_t *cent, vec3_t legs[3], vec3_t torso[3], vec3_t head[3] ) {
	vec3_t		legsAngles, torsoAngles, headAngles;
	float		dest;
	static	int	movementOffsets[8] = { 0, 22, 45, -22, 0, 22, -45, -22 };
	vec3_t		velocity;
	float		speed;
	int			dir, clientNum;
	clientInfo_t	*ci;
	int		camereyes;


	VectorCopy( cent->lerpAngles, headAngles );
	//headAngles[YAW] = AngleMod( headAngles[YAW] );

	VectorClear( legsAngles );
	VectorClear( torsoAngles );

	camereyes = 0;
	if ( cent->currentState.number == cg.snap->ps.clientNum) {
		camereyes = 1; // it's me!
	}

	// --------- yaw -------------

	// allow yaw to drift a bit
	if( 
	   L_ANIM != LEGS_IDLE
	&& L_ANIM != LEGS_IDLECR

	|| T_ANIM == TORSO_ATTACK
	|| T_ANIM == TORSO_ATTACK2
	){
		cent->pe.torso.yawing = qtrue;	// center
		cent->pe.torso.pitching = qtrue;	// center
		cent->pe.legs.yawing = qtrue;	// center
	}

	// adjust legs for movement dir
	if ( cent->currentState.eFlags & EF_DEAD ) {
		// don't let dead bodies twitch
		dir = 0;
	} else {
		dir = cent->currentState.angles2[YAW];
		if ( dir < 0 || dir > 7 ) {
			CG_Error( "Bad player movement angle" );
		}
	}
	legsAngles[YAW] = headAngles[YAW] + movementOffsets[ dir ];
	torsoAngles[YAW] = headAngles[YAW] + 0.25 * movementOffsets[ dir ];

	// torso
	CG_SwingAngles( torsoAngles[YAW], 60000, 90, 0.3, &cent->pe.torso.yawAngle, &cent->pe.torso.yawing );
	CG_SwingAngles( legsAngles[YAW], 60000, 90, 0.3, &cent->pe.legs.yawAngle, &cent->pe.legs.yawing );

	torsoAngles[YAW] = cent->pe.torso.yawAngle;
	legsAngles[YAW] = cent->pe.legs.yawAngle;


	// --------- pitch -------------

	// only show a fraction of the pitch angle in the torso
	if ( headAngles[PITCH] > 180 ) {
		dest = (-360 + headAngles[PITCH]) * 0.75f;
	} else {
		dest = headAngles[PITCH] * 0.75f;
	}
	CG_SwingAngles( dest, 15, 30, 0.1f, &cent->pe.torso.pitchAngle, &cent->pe.torso.pitching );
	torsoAngles[PITCH] = cent->pe.torso.pitchAngle;

	//
	clientNum = cent->currentState.clientNum;
	if ( clientNum >= 0 && clientNum < MAX_CLIENTS ) {
		ci = &cgs.clientinfo[ clientNum ];
		if ( ci->fixedtorso ) {
			torsoAngles[PITCH] = 0.0f;
		}

	if(cg.renderingThirdPerson){
		torsoAngles[PITCH] = 0.0f;
	}

	if(cg.renderingEyesPerson){
		torsoAngles[PITCH] = 0.0f;
	}
	}

	// --------- roll -------------

	// lean towards the direction of travel
	VectorCopy( cent->currentState.pos.trDelta, velocity );
	speed = VectorNormalize( velocity );
	if ( speed ) {
		vec3_t	axis[3];
		float	side;

		speed *= 0.05f;

		AnglesToAxis( legsAngles, axis );
		side = speed * DotProduct( velocity, axis[1] );
		legsAngles[ROLL] -= side;

		side = speed * DotProduct( velocity, axis[0] );
		legsAngles[PITCH] += side;
	}

	clientNum = cent->currentState.clientNum;
	if ( clientNum >= 0 && clientNum < MAX_CLIENTS ) {
		ci = &cgs.clientinfo[ clientNum ];
		if ( ci->fixedlegs ) {
			legsAngles[YAW] = torsoAngles[YAW];
			legsAngles[PITCH] = 0.0f;
			legsAngles[ROLL] = 0.0f;
		}
	}

	// pain twitch
	CG_AddPainTwitch( cent, torsoAngles );
	// TODO: make eyes erode on pain twitch

	// leilei - eyes hack
	if (camereyes){
		cent->eyesAngles[YAW] = headAngles[YAW];
		cent->eyesAngles[PITCH] = headAngles[PITCH];
		cent->eyesAngles[ROLL] = headAngles[ROLL];
	}

	// pull the angles back out of the hierarchial chain
	AnglesSubtract( headAngles, torsoAngles, headAngles );
	AnglesSubtract( torsoAngles, legsAngles, torsoAngles );
	AnglesToAxis( legsAngles, legs );
	AnglesToAxis( torsoAngles, torso );
	AnglesToAxis( torsoAngles, head );
}

/*
===============
CG_HasteTrail
===============
*/
static void CG_HasteTrail( centity_t *cent ) {
	localEntity_t	*smoke;
	vec3_t			origin;
	int				anim;

	if ( cent->trailTime > cg.time ) {
		return;
	}
	anim = cent->pe.legs.animationNumber & ~ANIM_TOGGLEBIT;
	if ( anim != LEGS_RUN && anim != LEGS_BACK ) {
		return;
	}

	cent->trailTime += 100;
	if ( cent->trailTime < cg.time ) {
		cent->trailTime = cg.time;
	}

	VectorCopy( cent->lerpOrigin, origin );
	origin[2] -= 16;

	smoke = CG_SmokePuff( origin, vec3_origin,
				  8,
				  1, 1, 1, 1,
				  500,
				  cg.time,
				  0,
				  0,
				  cgs.media.hastePuffShader );

	// use the optimized local entity add
	smoke->leType = LE_SCALE_FADE;
}

/*
===============
CG_BreathPuffs
===============
*/
static void CG_BreathPuffs( centity_t *cent, refEntity_t *head) {
	clientInfo_t *ci;
	vec3_t up, origin;
	int contents;

	ci = &cgs.clientinfo[ cent->currentState.number ];

	if (!cg_enableBreath.integer) {
		return;
	}
	if ( cent->currentState.number == cg.snap->ps.clientNum && !cg.renderingThirdPerson) {
		return;
	}
	if ( cent->currentState.eFlags & EF_DEAD ) {
		return;
	}
	contents = CG_PointContents( head->origin, 0 );
	if ( contents & ( CONTENTS_WATER | CONTENTS_SLIME | CONTENTS_LAVA ) ) {
		return;
	}
	if ( ci->breathPuffTime > cg.time ) {
		return;
	}

	VectorSet( up, 0, 0, 8 );
	VectorMA(head->origin, 8, head->axis[0], origin);
	VectorMA(origin, -4, head->axis[2], origin);
	CG_SmokePuff( origin, up, 16, 1, 1, 1, 0.66f, 1500, cg.time, cg.time + 400, LEF_PUFF_DONT_SCALE, cgs.media.shotgunSmokePuffShader );
	ci->breathPuffTime = cg.time + 2000;
}

/*
===============
CG_DustTrail
===============
*/
static void CG_DustTrail( centity_t *cent ) {
	int				anim;
	localEntity_t	*dust;
	vec3_t end, vel;
	trace_t tr;

	if (!cg_enableDust.integer)
		return;

	if ( cent->dustTrailTime > cg.time ) {
		return;
	}

	anim = cent->pe.legs.animationNumber & ~ANIM_TOGGLEBIT;
	if ( anim != LEGS_LANDB && anim != LEGS_LAND ) {
		return;
	}

	cent->dustTrailTime += 40;
	if ( cent->dustTrailTime < cg.time ) {
		cent->dustTrailTime = cg.time;
	}

	VectorCopy(cent->currentState.pos.trBase, end);
	end[2] -= 64;
	CG_Trace( &tr, cent->currentState.pos.trBase, NULL, NULL, end, cent->currentState.number, MASK_PLAYERSOLID );

	if ( !(tr.surfaceFlags & SURF_DUST) )
		return;

	VectorCopy( cent->currentState.pos.trBase, end );
	end[2] -= 16;

	VectorSet(vel, 0, 0, -30);
	dust = CG_SmokePuff( end, vel,
				  24,
				  .8f, .8f, 0.7f, 0.33f,
				  500,
				  cg.time,
				  0,
				  0,
				  cgs.media.dustPuffShader );
}

/*
===============
CG_TrailItem
===============
*/
static void CG_TrailItem( centity_t *cent, qhandle_t hModel ) {
	refEntity_t		ent;
	vec3_t			angles;
	vec3_t			axis[3];

	VectorCopy( cent->lerpAngles, angles );
	angles[PITCH] = 0;
	angles[ROLL] = 0;
	AnglesToAxis( angles, axis );

	memset( &ent, 0, sizeof( ent ) );
	VectorMA( cent->lerpOrigin, -16, axis[0], ent.origin );
	ent.origin[2] += 16;
	angles[YAW] += 90;
	AnglesToAxis( angles, ent.axis );

	ent.hModel = hModel;
	trap_R_AddRefEntityToScene( &ent );
}

/*
===============
CG_PlayerTokens
===============
*/
static void CG_PlayerTokens( centity_t *cent, int renderfx ) {
	int			tokens, i, j;
	float		angle;
	refEntity_t	ent;
	vec3_t		dir, origin;
	skulltrail_t *trail;
	trail = &cg.skulltrails[cent->currentState.number];
	tokens = cent->currentState.generic1;
	if ( !tokens ) {
		trail->numpositions = 0;
		return;
	}

	if ( tokens > MAX_SKULLTRAIL ) {
		tokens = MAX_SKULLTRAIL;
	}

	// add skulls if there are more than last time
	for (i = 0; i < tokens - trail->numpositions; i++) {
		for (j = trail->numpositions; j > 0; j--) {
			VectorCopy(trail->positions[j-1], trail->positions[j]);
		}
		VectorCopy(cent->lerpOrigin, trail->positions[0]);
	}
	trail->numpositions = tokens;

	// move all the skulls along the trail
	VectorCopy(cent->lerpOrigin, origin);
	for (i = 0; i < trail->numpositions; i++) {
		VectorSubtract(trail->positions[i], origin, dir);
		if (VectorNormalize(dir) > 30) {
			VectorMA(origin, 30, dir, trail->positions[i]);
		}
		VectorCopy(trail->positions[i], origin);
	}

	memset( &ent, 0, sizeof( ent ) );
	if( cgs.clientinfo[ cent->currentState.clientNum ].team == TEAM_BLUE ) {
		ent.hModel = cgs.media.redCubeModel;
	} else {
		ent.hModel = cgs.media.blueCubeModel;
	}
	ent.renderfx = renderfx;

	VectorCopy(cent->lerpOrigin, origin);
	for (i = 0; i < trail->numpositions; i++) {
		VectorSubtract(origin, trail->positions[i], ent.axis[0]);
		ent.axis[0][2] = 0;
		VectorNormalize(ent.axis[0]);
		VectorSet(ent.axis[2], 0, 0, 1);
		CrossProduct(ent.axis[0], ent.axis[2], ent.axis[1]);

		VectorCopy(trail->positions[i], ent.origin);
		angle = (((cg.time + 500 * MAX_SKULLTRAIL - 500 * i) / 16) & 255) * (M_PI * 2) / 255;
		ent.origin[2] += sin(angle) * 10;
		trap_R_AddRefEntityToScene( &ent );
		VectorCopy(trail->positions[i], origin);
	}
}


/*
===============
CG_PlayerPowerups
===============
*/
static void CG_PlayerPowerups( centity_t *cent, refEntity_t *torso ) {
	int		powerups;
	clientInfo_t	*ci;

	powerups = cent->currentState.powerups;
	if ( !powerups ) {
		return;
	}


	// quad gives a dlight
	if ( powerups & ( 1 << PW_QUAD ) ) {
		trap_R_AddLightToScene( cent->lerpOrigin, 200 + (rand()&31), 0.2f, 0.2f, 1 );
	}

	ci = &cgs.clientinfo[ cent->currentState.clientNum ];
	// redflag
	if ( powerups & ( 1 << PW_REDFLAG ) ) {
		CG_TrailItem( cent, cgs.media.redFlagModel );
		trap_R_AddLightToScene( cent->lerpOrigin, 200 + (rand()&31), 1.0, 0.2f, 0.2f );
	}

	// blueflag
	if ( powerups & ( 1 << PW_BLUEFLAG ) ) {
		CG_TrailItem( cent, cgs.media.blueFlagModel );
		trap_R_AddLightToScene( cent->lerpOrigin, 200 + (rand()&31), 0.2f, 0.2f, 1.0 );
	}

	// neutralflag
	if ( powerups & ( 1 << PW_NEUTRALFLAG ) ) {
		CG_TrailItem( cent, cgs.media.neutralFlagModel );
		trap_R_AddLightToScene( cent->lerpOrigin, 200 + (rand()&31), 1.0, 1.0, 1.0 );
	}

	// haste leaves smoke trails
	if ( powerups & ( 1 << PW_HASTE ) ) {
		CG_HasteTrail( cent );
	}
}

/*
===============
CG_PlayerShadow

Returns the Z component of the surface being shadowed

  should it return a full plane instead of a Z?
===============
*/
#define	SHADOW_DISTANCE		128
static qboolean CG_PlayerShadow( centity_t *cent ) {
	vec3_t		end, mins = {-15, -15, 0}, maxs = {15, 15, 2};
	trace_t		trace;
	float		alpha;

	if ( cg_shadows.integer == 0 ) {
		return qfalse;
	}

	// no shadows when invisible
	if ( cent->currentState.powerups & ( 1 << PW_INVIS ) ) {
		return qfalse;
	}

	// send a trace down from the player to the ground
	VectorCopy( cent->lerpOrigin, end );
	end[2] -= SHADOW_DISTANCE;

	trap_CM_BoxTrace( &trace, cent->lerpOrigin, end, mins, maxs, 0, MASK_PLAYERSOLID );

	// no shadow if too high
	if ( trace.fraction == 1.0 || trace.startsolid || trace.allsolid ) {
		return qfalse;
	}

	// fade the shadow out with height
	alpha = 1.0 - trace.fraction;

	// add the mark as a temporary, so it goes directly to the renderer
	// without taking a spot in the cg_marks array
	CG_ImpactMark( cgs.media.shadowMarkShader, trace.endpos, trace.plane.normal, cent->pe.legs.yawAngle, alpha,alpha,alpha,1, qfalse, 24, qtrue );

	return qtrue;
}

/*
===============
CG_AddRefEntityWithPowerups

Adds a piece with modifications or duplications for powerups
Also called by CG_Missile for quad rockets, but nobody can tell...
===============
*/
void CG_AddRefEntityWithPowerups( refEntity_t *ent, entityState_t *state, int team, qboolean isMissile ) {

	if ( state->powerups & ( 1 << PW_INVIS ) ) {
		ent->customShader = cgs.media.invisShader;
		trap_R_AddRefEntityToScene( ent );
	} else {
		trap_R_AddRefEntityToScene( ent );
		if ( state->powerups & ( 1 << PW_QUAD ) ) {
			if (team == TEAM_RED)
				ent->customShader = cgs.media.redQuadShader;
			else
				ent->customShader = cgs.media.quadShader;
			trap_R_AddRefEntityToScene( ent );
		}
		if ( state->powerups & ( 1 << PW_REGEN ) ) {
			if ( ( ( cg.time / 100 ) % 10 ) == 1 ) {
				ent->customShader = cgs.media.regenShader;
				trap_R_AddRefEntityToScene( ent );
			}
		}
		if ( state->powerups & ( 1 << PW_BATTLESUIT ) ) {
			ent->customShader = cgs.media.battleSuitShader;
			trap_R_AddRefEntityToScene( ent );
		}
	}
}

/*
===============
CG_Player
===============
*/

void CG_Player( centity_t *cent ) {
	clientInfo_t	*ci;
	refEntity_t		legs;
	refEntity_t		torso;
	refEntity_t		head;
	int				clientNum;
	int				renderfx;
	qboolean		shadow;
	refEntity_t		skull;
	refEntity_t		powerup;
	int				t;
	float			c;
	float			angle;
	vec3_t			dir, angles;
	int 			camereyes = 0;

	// the client number is stored in clientNum.  It can't be derived
	// from the entity number, because a single client may have
	// multiple corpses on the level using the same clientinfo
	clientNum = cent->currentState.clientNum;
	if ( clientNum < 0 || clientNum >= MAX_CLIENTS ) {
		CG_Error( "Bad clientNum on player entity");
	}
	ci = &cgs.clientinfo[ clientNum ];

	// not have valid clientinfo
	if ( !ci->infoValid ) {
		return;
	}

	// get the player model information
	renderfx = 0;
	if ( cent->currentState.number == cg.snap->ps.clientNum) {
		camereyes = 1;	// it's me!
		if (!cg.renderingThirdPerson) {
			renderfx = RF_THIRD_PERSON;			// only draw in mirrors
		}
	}

	memset( &legs, 0, sizeof(legs) );
	memset( &torso, 0, sizeof(torso) );
	memset( &head, 0, sizeof(head) );

	// get the rotation information
	CG_PlayerAngles( cent, legs.axis, torso.axis, head.axis );

	// get the animation state (after rotation, to allow feet shuffle)
	CG_PlayerAnimation( cent, &legs.oldframe, &legs.frame, &legs.backlerp, &torso.oldframe, &torso.frame, &torso.backlerp );

	// add the shadow
	shadow = CG_PlayerShadow( cent );

	renderfx |= RF_LIGHTING_ORIGIN;			// use the same origin for all
	if( cgs.gametype == GT_HARVESTER ) {
		CG_PlayerTokens( cent, renderfx );
	}

	//
	// add the legs
	//
	legs.hModel = ci->legsModel;
	legs.customSkin = ci->legsSkin;
	if(!legs.customSkin){
		legs.customShader = ci->legsShader;	
	}
	legs.shaderRGBA[0] = ci->legsR;
	legs.shaderRGBA[1] = ci->legsG;
	legs.shaderRGBA[2] = ci->legsB;
	legs.shaderRGBA[3] = 255;
	VectorCopy( cent->lerpOrigin, legs.origin );

	VectorCopy( cent->lerpOrigin, legs.lightingOrigin );
	
	if(ci->vehicleNum > 0){
		VectorScale(legs.axis[0], 0, legs.axis[0]);
		VectorScale(legs.axis[1], 0, legs.axis[1]);
		VectorScale(legs.axis[2], 0, legs.axis[2]);
	}

	legs.renderfx = renderfx;
	VectorCopy (legs.origin, legs.oldorigin);	// don't positionally lerp at all

	if (cg.renderingEyesPerson){
		legs.renderfx &= RF_FIRST_PERSON;
	}

	CG_AddRefEntityWithPowerups( &legs, &cent->currentState, ci->team, qfalse );

	// if the model failed, allow the default nullmodel to be displayed
	if (!legs.hModel) {
		return;
	}

	//
	// add the torso
	//
	torso.hModel = ci->torsoModel;
	if (!torso.hModel) {
		return;
	}

	torso.customSkin = ci->torsoSkin;
	if(!torso.customSkin){
		torso.customShader = ci->torsoShader;	
	}
	torso.shaderRGBA[0] = ci->modelR;
	torso.shaderRGBA[1] = ci->modelG;
	torso.shaderRGBA[2] = ci->modelB;
	torso.shaderRGBA[3] = 255;

	VectorCopy( cent->lerpOrigin, torso.lightingOrigin );

	CG_PositionRotatedEntityOnTag( &torso, &legs, ci->legsModel, "tag_torso");

	torso.renderfx = renderfx;
	if (cg.renderingEyesPerson){
		torso.renderfx &= RF_FIRST_PERSON;
	}
	
	CG_AddRefEntityWithPowerups( &torso, &cent->currentState, ci->team, qfalse );

	if ( cent->currentState.number != cg.snap->ps.clientNum) {
		if ( cent->currentState.eFlags & EF_KAMIKAZE ) {

			memset( &skull, 0, sizeof(skull) );

			VectorCopy( cent->lerpOrigin, skull.lightingOrigin );
			skull.renderfx = renderfx;

			if ( cent->currentState.eFlags & EF_DEAD ) {
				// one skull bobbing above the dead body
				angle = ((cg.time / 7) & 255) * (M_PI * 2) / 255;
				if (angle > M_PI * 2)
					angle -= (float)M_PI * 2;
				dir[0] = sin(angle) * 20;
				dir[1] = cos(angle) * 20;
				angle = ((cg.time / 4) & 255) * (M_PI * 2) / 255;
				dir[2] = 15 + sin(angle) * 8;
				VectorAdd(torso.origin, dir, skull.origin);

				dir[2] = 0;
				VectorCopy(dir, skull.axis[1]);
				VectorNormalize(skull.axis[1]);
				VectorSet(skull.axis[2], 0, 0, 1);
				CrossProduct(skull.axis[1], skull.axis[2], skull.axis[0]);

				skull.hModel = cgs.media.kamikazeHeadModel;
				trap_R_AddRefEntityToScene( &skull );
				skull.hModel = cgs.media.kamikazeHeadTrail;
				trap_R_AddRefEntityToScene( &skull );
			} else {
				// three skulls spinning around the player
				angle = ((cg.time / 4) & 255) * (M_PI * 2) / 255;
				dir[0] = cos(angle) * 20;
				dir[1] = sin(angle) * 20;
				dir[2] = cos(angle) * 20;
				VectorAdd(torso.origin, dir, skull.origin);

				angles[0] = sin(angle) * 30;
				angles[1] = (angle * 180 / M_PI) + 90;
				if (angles[1] > 360)
					angles[1] -= 360;
				angles[2] = 0;
				AnglesToAxis( angles, skull.axis );

				skull.hModel = cgs.media.kamikazeHeadModel;
				trap_R_AddRefEntityToScene( &skull );
				// flip the trail because this skull is spinning in the other direction
				VectorInverse(skull.axis[1]);
				skull.hModel = cgs.media.kamikazeHeadTrail;
				trap_R_AddRefEntityToScene( &skull );

				angle = ((cg.time / 4) & 255) * (M_PI * 2) / 255 + M_PI;
				if (angle > M_PI * 2)
					angle -= (float)M_PI * 2;
				dir[0] = sin(angle) * 20;
				dir[1] = cos(angle) * 20;
				dir[2] = cos(angle) * 20;
				VectorAdd(torso.origin, dir, skull.origin);

				angles[0] = cos(angle - 0.5 * M_PI) * 30;
				angles[1] = 360 - (angle * 180 / M_PI);
				if (angles[1] > 360)
					angles[1] -= 360;
				angles[2] = 0;
				AnglesToAxis( angles, skull.axis );

				skull.hModel = cgs.media.kamikazeHeadModel;
				trap_R_AddRefEntityToScene( &skull );
				skull.hModel = cgs.media.kamikazeHeadTrail;
				trap_R_AddRefEntityToScene( &skull );

				angle = ((cg.time / 3) & 255) * (M_PI * 2) / 255 + 0.5 * M_PI;
				if (angle > M_PI * 2)
					angle -= (float)M_PI * 2;
				dir[0] = sin(angle) * 20;
				dir[1] = cos(angle) * 20;
				dir[2] = 0;
				VectorAdd(torso.origin, dir, skull.origin);

				VectorCopy(dir, skull.axis[1]);
				VectorNormalize(skull.axis[1]);
				VectorSet(skull.axis[2], 0, 0, 1);
				CrossProduct(skull.axis[1], skull.axis[2], skull.axis[0]);

				skull.hModel = cgs.media.kamikazeHeadModel;
				trap_R_AddRefEntityToScene( &skull );
				skull.hModel = cgs.media.kamikazeHeadTrail;
				trap_R_AddRefEntityToScene( &skull );
			}
		}
	}

	if ( cent->currentState.number != cg.snap->ps.clientNum && !(cent->currentState.powerups & (1 << PW_INVIS))) {
		if ( cent->currentState.powerups & ( 1 << PW_GUARD ) ) {
			memcpy(&powerup, &torso, sizeof(torso));
			powerup.hModel = cgs.media.guardPowerupModel;
			powerup.frame = 0;
			powerup.oldframe = 0;
			powerup.customSkin = 0;
			trap_R_AddRefEntityToScene( &powerup );
		}
		if ( cent->currentState.powerups & ( 1 << PW_SCOUT ) ) {
			memcpy(&powerup, &torso, sizeof(torso));
			powerup.hModel = cgs.media.scoutPowerupModel;
			powerup.frame = 0;
			powerup.oldframe = 0;
			powerup.customSkin = 0;
			trap_R_AddRefEntityToScene( &powerup );
		}
		if ( cent->currentState.powerups & ( 1 << PW_DOUBLER ) ) {
			memcpy(&powerup, &torso, sizeof(torso));
			powerup.hModel = cgs.media.doublerPowerupModel;
			powerup.frame = 0;
			powerup.oldframe = 0;
			powerup.customSkin = 0;
			trap_R_AddRefEntityToScene( &powerup );
		}
		if ( cent->currentState.powerups & ( 1 << PW_AMMOREGEN ) ) {
			memcpy(&powerup, &torso, sizeof(torso));
			powerup.hModel = cgs.media.ammoRegenPowerupModel;
			powerup.frame = 0;
			powerup.oldframe = 0;
			powerup.customSkin = 0;
			trap_R_AddRefEntityToScene( &powerup );
		}
	}

	if ( cent->currentState.powerups & ( 1 << PW_INVULNERABILITY ) ) {
		if ( !ci->invulnerabilityStartTime ) {
			ci->invulnerabilityStartTime = cg.time;
		}
		ci->invulnerabilityStopTime = cg.time;
	} else {
		ci->invulnerabilityStartTime = 0;
	}
	if ( (cent->currentState.powerups & ( 1 << PW_INVULNERABILITY ) ) ||
		cg.time - ci->invulnerabilityStopTime < 250 ) {

		memcpy(&powerup, &torso, sizeof(torso));
		powerup.hModel = cgs.media.invulnerabilityPowerupModel;
		powerup.customSkin = 0;
		// always draw
		powerup.renderfx &= ~RF_THIRD_PERSON;
		VectorCopy(cent->lerpOrigin, powerup.origin);

		if ( cg.time - ci->invulnerabilityStartTime < 250 ) {
			c = (float) (cg.time - ci->invulnerabilityStartTime) / 250;
		} else if (cg.time - ci->invulnerabilityStopTime < 250 ) {
			c = (float) (250 - (cg.time - ci->invulnerabilityStopTime)) / 250;
		} else {
			c = 1;
		}
		VectorSet( powerup.axis[0], c, 0, 0 );
		VectorSet( powerup.axis[1], 0, c, 0 );
		VectorSet( powerup.axis[2], 0, 0, c );
		trap_R_AddRefEntityToScene( &powerup );
	}

	t = cg.time - ci->medkitUsageTime;
	if ( ci->medkitUsageTime && t < 500 ) {
		memcpy(&powerup, &torso, sizeof(torso));
		powerup.hModel = cgs.media.medkitUsageModel;
		powerup.customSkin = 0;
		// always draw
		powerup.renderfx &= ~RF_THIRD_PERSON;
		VectorClear(angles);
		AnglesToAxis(angles, powerup.axis);
		VectorCopy(cent->lerpOrigin, powerup.origin);
		powerup.origin[2] += -24 + (float) t * 80 / 500;
		if ( t > 400 ) {
			c = (float) (t - 1000) * 0xff / 100;
			powerup.shaderRGBA[0] = 0xff - c;
			powerup.shaderRGBA[1] = 0xff - c;
			powerup.shaderRGBA[2] = 0xff - c;
			powerup.shaderRGBA[3] = 0xff - c;
		} else {
			powerup.shaderRGBA[0] = 0xff;
			powerup.shaderRGBA[1] = 0xff;
			powerup.shaderRGBA[2] = 0xff;
			powerup.shaderRGBA[3] = 0xff;
		}
		trap_R_AddRefEntityToScene( &powerup );
	}

	//
	// add the head
	//
	head.hModel = ci->headModel;
	if (!head.hModel) {
		return;
	}
	head.customSkin = ci->headSkin;
	if(!head.customSkin){
		head.customShader = ci->headShader;	
	}
	head.shaderRGBA[0] = ci->headR;
	head.shaderRGBA[1] = ci->headG;
	head.shaderRGBA[2] = ci->headB;
	head.shaderRGBA[3] = 255;

	VectorCopy( cent->lerpOrigin, head.lightingOrigin );

	CG_PositionRotatedEntityOnTag( &head, &torso, ci->torsoModel, "tag_head");

	//
	// add the eyes
	//
	if (camereyes){
		cent->eyesOrigin[0] = head.origin[0];
		cent->eyesOrigin[1] = head.origin[1];
		cent->eyesOrigin[2] = head.origin[2];
		if (cg.renderingEyesPerson){
			VectorCopy(cent->lerpAngles, headang);
		}

		if (cg.renderingEyesPerson){
			VectorCopy(head.origin, cent->eyesOrigin);
			VectorSubtract(cent->eyesOrigin, cent->lerpOrigin, cent->eyesOrigin);
			VectorCopy(cent->eyesOrigin, headpos);
		}
	}

	VectorCopy(cent->pe.eyepos, head.eyepos[0]);				// Copy it to our refdef for the renderer

	head.renderfx = renderfx;

	CG_AddRefEntityWithPowerups( &head, &cent->currentState, ci->team, qfalse );

	CG_BreathPuffs(cent, &head);

	CG_DustTrail(cent);

	//
	// add the gun / barrel / flash
	//
	if(cent->currentState.eFlags & EF_DEAD)
		return;

	if(cent->currentState.number != cg.snap->ps.clientNum){
		if(ci->team == cg.snap->ps.persistant[PERS_TEAM] && ci->team != TEAM_FREE){
			CG_Add3DString(cent->lerpOrigin[0], cent->lerpOrigin[1], cent->lerpOrigin[2]+48, ci->name, UI_DROPSHADOW, color_white, 1.00, 2048, 3072, qfalse);
		} else if (ci->team == TEAM_FREE) {
			CG_Add3DString(cent->lerpOrigin[0], cent->lerpOrigin[1], cent->lerpOrigin[2]+48, ci->name, UI_DROPSHADOW, color_white, 1.00, 512, 768, qtrue);
		}
	}

	CG_AddPlayerWeapon( &torso, NULL, cent, ci->team, ci );
	CG_PlayerPowerups( cent, &torso );
}

/*
===============
CG_ResetPlayerEntity

A player just came into view or teleported, so reset all animation info
===============
*/
void CG_ResetPlayerEntity( centity_t *cent ) {
	cent->errorTime = -99999;		// guarantee no error decay added
	cent->extrapolated = qfalse;

	CG_ClearLerpFrame( &cgs.clientinfo[ cent->currentState.clientNum ], &cent->pe.legs, cent->currentState.legsAnim );
	CG_ClearLerpFrame( &cgs.clientinfo[ cent->currentState.clientNum ], &cent->pe.torso, cent->currentState.torsoAnim );

	BG_EvaluateTrajectory( &cent->currentState.pos, cg.time, cent->lerpOrigin );
	BG_EvaluateTrajectory( &cent->currentState.apos, cg.time, cent->lerpAngles );

	VectorCopy( cent->lerpOrigin, cent->rawOrigin );
	VectorCopy( cent->lerpAngles, cent->rawAngles );

	memset( &cent->pe.legs, 0, sizeof( cent->pe.legs ) );
	cent->pe.legs.yawAngle = cent->rawAngles[YAW];
	cent->pe.legs.yawing = qfalse;
	cent->pe.legs.pitchAngle = 0;
	cent->pe.legs.pitching = qfalse;

	memset( &cent->pe.torso, 0, sizeof( cent->pe.legs ) );
	cent->pe.torso.yawAngle = cent->rawAngles[YAW];
	cent->pe.torso.yawing = qfalse;
	cent->pe.torso.pitchAngle = cent->rawAngles[PITCH];
	cent->pe.torso.pitching = qfalse;

	memset( &cent->pe.head, 0, sizeof( cent->pe.head ) );
	cent->pe.head.yawAngle = cent->rawAngles[YAW];
	cent->pe.head.yawing = qfalse;
	cent->pe.head.pitchAngle = cent->rawAngles[PITCH];
	cent->pe.head.pitching = qfalse;
}
