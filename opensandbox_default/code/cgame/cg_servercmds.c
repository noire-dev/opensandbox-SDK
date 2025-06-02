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
// cg_servercmds.c -- reliably sequenced text commands sent by the server
// these are processed at snapshot transition time, so there will definately
// be a valid snapshot this frame

#include "cg_local.h"

#define NUM_DATA 4
#define FIRST_DATA 3

static void CG_ParseScores( void ) {
	int		i, powerups;

	cg.numScores = atoi( CG_Argv( 1 ) );
	if ( cg.numScores > MAX_CLIENTS ) {
		cg.numScores = MAX_CLIENTS;
	}

	cg.teamScores[0] = atoi( CG_Argv( 2 ) );
	cg.teamScores[1] = atoi( CG_Argv( 3 ) );

	memset( cg.scores, 0, sizeof( cg.scores ) );

	for ( i = 0 ; i < cg.numScores ; i++ ) {
		cg.scores[i].client = atoi( CG_Argv( i * NUM_DATA + FIRST_DATA + 1 ) );
		cg.scores[i].score = atoi( CG_Argv( i * NUM_DATA + FIRST_DATA + 2 ) );
		cg.scores[i].ping = atoi( CG_Argv( i * NUM_DATA + FIRST_DATA + 3 ) );
		cg.scores[i].time = atoi( CG_Argv( i * NUM_DATA + FIRST_DATA + 4 ) );

		if ( cg.scores[i].client < 0 || cg.scores[i].client >= MAX_CLIENTS ) {
			cg.scores[i].client = 0;
		}
	}
}

static void CG_ParseObeliskHealth( void ) {
    cg.redObeliskHealth = atoi( CG_Argv(1) );
    cg.blueObeliskHealth = atoi( CG_Argv(2) );
}

static void CG_ParseRespawnTime( void ) {
    cg.respawnTime = atoi( CG_Argv(1) );
}

static void CG_ParseGameCvars(void) {
	mod_jumpheight     		= atoi(CG_Argv(1));
	mod_gravity       		= atoi(CG_Argv(2));
}

static void CG_ParseSweps(void) {
    int i;
    int weaponIndex;
    int numArgs = trap_Argc();

    for (i = 0; i < WEAPONS_NUM; i++) {
        cg.swep_listcl[i] = 0;
    }

    for (i = 1; i < numArgs; i++) {
        weaponIndex = atoi(CG_Argv(i));

        if (weaponIndex >= -WEAPONS_NUM && weaponIndex < WEAPONS_NUM) {
            if (weaponIndex > 0) {
                cg.swep_listcl[weaponIndex] = 1;
            } else if (weaponIndex < 0) {
                cg.swep_listcl[-weaponIndex] = 2;
            }
        }
    }
}

static void CG_ParseSpawnSweps(void) {
    int i;
    int weaponIndex;
    int numArgs = trap_Argc();

    for (i = 0; i < WEAPONS_NUM; i++) {
        cg.swep_spawncl[i] = 0;
    }

    for (i = 1; i < numArgs; i++) {
        weaponIndex = atoi(CG_Argv(i));

        if (weaponIndex >= -WEAPONS_NUM && weaponIndex < WEAPONS_NUM) {
            if (weaponIndex > 0) {
                cg.swep_spawncl[weaponIndex] = 1;
            } else if (weaponIndex < 0) {
                cg.swep_spawncl[-weaponIndex] = 2;
            }
        }
    }
}

/*
================
CG_ParseServerinfo

This is called explicitly when the gamestate is first received,
and whenever the server updates any serverinfo flagged cvars
================
*/
void CG_ParseServerinfo( void ) {
	const char	*info;
	char	*mapname;

	info = CG_ConfigString( CS_SERVERINFO );
	cgs.gametype = atoi( Info_ValueForKey( info, "g_gametype" ) );
	trap_Cvar_Set("g_gametype", va("%i", cgs.gametype));
	cgs.teamflags = atoi( Info_ValueForKey( info, "teamflags" ) );
	cgs.fraglimit = atoi( Info_ValueForKey( info, "fraglimit" ) );
	cgs.capturelimit = atoi( Info_ValueForKey( info, "capturelimit" ) );
	cgs.timelimit = atoi( Info_ValueForKey( info, "timelimit" ) );
	cgs.maxclients = atoi( Info_ValueForKey( info, "g_maxClients" ) );
	mapname = Info_ValueForKey( info, "mapname" );
	Com_sprintf( cgs.mapname, sizeof( cgs.mapname ), "maps/%s.bsp", mapname );
}

/*
================
CG_SetConfigValues

Called on load to set the initial values from configure strings
================
*/
void CG_SetConfigValues( void ) {
	const char *s;

	cgs.scores1 = atoi( CG_ConfigString( CS_SCORES1 ) );
	cgs.scores2 = atoi( CG_ConfigString( CS_SCORES2 ) );
	cgs.levelStartTime = atoi( CG_ConfigString( CS_LEVEL_START_TIME ) );
	if( cgs.gametype == GT_CTF ) {
		s = CG_ConfigString( CS_FLAGSTATUS );
		cgs.redflag = s[0] - '0';
		cgs.blueflag = s[1] - '0';
	}

	else if( cgs.gametype == GT_1FCTF ) {
		s = CG_ConfigString( CS_FLAGSTATUS );
		cgs.flagStatus = s[0] - '0';
	}
}

/*
================
CG_ConfigStringModified

================
*/
static void CG_ConfigStringModified( void ) {
	const char	*str;
	int		num;

	num = atoi( CG_Argv( 1 ) );

	// get the gamestate from the client system, which will have the
	// new configstring already integrated
	trap_GetGameState( &cgs.gameState );

	// look up the individual string that was modified
	str = CG_ConfigString( num );

	// do something with it if necessary
	if ( num == CS_MUSIC ) {
		CG_StartMusic();
	} else if ( num == CS_SERVERINFO ) {
		CG_ParseServerinfo();
	} else if ( num == CS_SCORES1 ) {
		cgs.scores1 = atoi( str );
	} else if ( num == CS_SCORES2 ) {
		cgs.scores2 = atoi( str );
	} else if ( num == CS_LEVEL_START_TIME ) {
		cgs.levelStartTime = atoi( str );
	} else if ( num == CS_INTERMISSION ) {
		cg.intermissionStarted = atoi( str );
	} else if ( num >= CS_MODELS && num < CS_MODELS+MAX_MODELS ) {
		cgs.gameModels[ num-CS_MODELS ] = trap_R_RegisterModel( str );
	} else if ( num >= CS_SOUNDS && num < CS_SOUNDS+MAX_SOUNDS ) {
		if ( str[0] != '*' ) {	// player specific sounds don't register here
			cgs.gameSounds[ num-CS_SOUNDS] = trap_S_RegisterSound( str, qfalse );
		}
	} else if ( num >= CS_PLAYERS && num < CS_PLAYERS+MAX_CLIENTS ) {
		CG_NewClientInfo( num - CS_PLAYERS );
		CG_BuildSpectatorString();
	} else if ( num == CS_FLAGSTATUS ) {
		if( cgs.gametype == GT_CTF ) {
			// format is rb where its red/blue, 0 is at base, 1 is taken, 2 is dropped
			cgs.redflag = str[0] - '0';
			cgs.blueflag = str[1] - '0';
    } else if( cgs.gametype == GT_1FCTF ) {
			cgs.flagStatus = str[0] - '0';
		}

	}

}

/*
===============
CG_MapRestart

The server has issued a map_restart, so the next snapshot
is completely new and should not be interpolated to.

A tournement restart will clear everything, but doesn't
require a reload of all the media
===============
*/
static void CG_MapRestart( void ) {
	CG_InitLocalEntities();
	CG_InitMarkPolys();

	// make sure the "3 frags left" warnings play again
	cg.fraglimitWarnings = 0;

	cg.timelimitWarnings = 0;

	cg.intermissionStarted = qfalse;

	cg.mapRestart = qtrue;

	CG_StartMusic();

	trap_S_ClearLoopingSounds(qtrue);
}

/*
=================
CG_RemoveChatEscapeChar
=================
*/
static void CG_RemoveChatEscapeChar( char *text ) {
	int i, l;

	l = 0;
	for ( i = 0; text[i]; i++ ) {
		if (text[i] == '\x19')
			continue;
		text[l++] = text[i];
	}
	text[l] = '\0';
}

/*
=================
CG_ServerCommand

The string has been tokenized and can be retrieved with
Cmd_Argc() / Cmd_Argv()
=================
*/
static void CG_ServerCommand( void ) {
	const char	*cmd;
	char		text[MAX_SAY_TEXT];
	int			offset;
	const char  *arg;

	cmd = CG_Argv(0);

	if ( !cmd[0] ) {
		// server claimed the command
		return;
	}

	if ( !strcmp( cmd, "cp" ) ) {
		CG_CenterPrint( CG_Argv(1), SCREEN_HEIGHT * 0.30, BIGCHAR_WIDTH );
		return;
	}
	
	if ( !strcmp( cmd, "clp" ) ) {
		CG_CenterPrint( CG_Argv(1), SCREEN_HEIGHT * 0.75, BASEFONT_INDENT );
		return;
	}

	if ( !strcmp( cmd, "cllp" ) ) {
		CG_CenterPrint( CG_Argv(1), SCREEN_HEIGHT * 0.90, BASEFONT_INDENT );
		return;
	}
	
	if ( !strcmp( cmd, "clcmd" ) ) {
		trap_SendConsoleCommand( va("%s\n", CG_Argv(1)) );
		return;
	}
	
	if ( !strcmp( cmd, "lp" ) ) {
		CG_CenterPrint( CG_Argv(1), SCREEN_HEIGHT * 0.75, BASEFONT_INDENT*1.5 );
		return;
	}

	if ( !strcmp( cmd, "notify" ) ) {
		CG_AddNotify(CG_Argv(1), NOTIFY_INFO);
		return;
	}

	if ( !strcmp( cmd, "undoProp" ) ) {
		CG_AddNotify ("Undone \"Prop\"", NOTIFY_UNDO);
		return;
	}

	if ( !strcmp( cmd, "undoNPC" ) ) {
		CG_AddNotify ("Undone \"NPC\"", NOTIFY_UNDO);
		return;
	}

	if ( !strcmp( cmd, "cs" ) ) {
		CG_ConfigStringModified();
		return;
	}

	if ( !strcmp( cmd, "print" ) ) {
		//if the message to print is about a client being dropped after a silent drop, suppress the drop message
		arg = CG_Argv(1);
		offset = strlen(arg) - strlen("DR_SILENT_DROP") - 1 ;
		if ( !strcmp(&arg[offset], "DR_SILENT_DROP\n") )
			return;
		
		CG_Printf( "%s", CG_Argv(1) );
		return;
	}

	if ( !strcmp( cmd, "chat" ) ) {
        trap_S_StartLocalSound( cgs.media.talkSound, CHAN_LOCAL_SOUND );
		Q_strncpyz( text, CG_Argv(1), MAX_SAY_TEXT );
		CG_RemoveChatEscapeChar( text );
		CG_PrintfChat( qfalse, "%s\n", text );
		return;
	}

	if ( !strcmp( cmd, "tchat" ) ) {
    	trap_S_StartLocalSound( cgs.media.talkSound, CHAN_LOCAL_SOUND );
		Q_strncpyz( text, CG_Argv(1), MAX_SAY_TEXT );
		CG_RemoveChatEscapeChar( text );
		CG_PrintfChat( qtrue, "%s\n", text );
		return;
	}

	if ( !strcmp( cmd, "scores" ) ) {
		CG_ParseScores();
		return;
	}

	if ( !strcmp( cmd, "map_restart" ) ) {
		CG_MapRestart();
		return;
	}

    if ( !strcmp (cmd, "oh") ) {
        CG_ParseObeliskHealth();
        return;
    }

    if ( !strcmp( cmd, "respawn" ) ) {
		CG_ParseRespawnTime();
		return;
	}

	if ( !strcmp( cmd, "gCvars" ) ) {
        CG_ParseGameCvars();
        return;
    }
	
	if ( !strcmp( cmd, "sweps" ) ) {
        CG_ParseSweps();
        return;
    }

	if ( !strcmp( cmd, "wpspawn" ) ) {
        CG_ParseSpawnSweps();
        return;
    }

	if ( !strcmp( cmd, "t_info" ) ) {
		Q_strncpyz(cg.entityInfo, CG_Argv(1), sizeof(cg.entityInfo));
        return;
    }

	CG_Printf( "Unknown client game command: %s\n", cmd );
}


/*
====================
CG_ExecuteNewServerCommands

Execute all of the server commands that were received along
with this this snapshot.
====================
*/
void CG_ExecuteNewServerCommands( int latestSequence ) {
	while ( cgs.serverCommandSequence < latestSequence ) {
		if ( trap_GetServerCommand( ++cgs.serverCommandSequence ) ) {
			CG_ServerCommand();
		}
	}
}
