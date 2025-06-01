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
#include "../qcommon/ns_local.h"

/*
================
G_WriteClientSessionData

Called on game shutdown
================
*/
void G_WriteClientSessionData( gclient_t *client ) {
	const char	*s;
	const char	*var;

	s = va("%i %i %i %i %i", 
		client->sess.sessionTeam,
		client->sess.spectatorNum,
		client->sess.spectatorState,
		client->sess.spectatorClient,
		client->sess.teamLeader
		);

	var = va( "session%i", (int)(client - level.clients) );
	trap_Cvar_Set( var, s );
}

/*
================
G_ReadSessionData

Called on a reconnect
================
*/
void G_ReadSessionData( gclient_t *client ) {
	char	s[MAX_STRING_CHARS];
	const char	*var;

	// bk001205 - format
	int teamLeader;
	int spectatorState;
	int sessionTeam;

	var = va( "session%i", (int)(client - level.clients) );
	trap_Cvar_VariableStringBuffer( var, s, sizeof(s) );

	sscanf( s, "%i %i %i %i %i",
		&sessionTeam,                 // bk010221 - format
		&client->sess.spectatorNum,
		&spectatorState,              // bk010221 - format
		&client->sess.spectatorClient,
		&teamLeader                   // bk010221 - format
		);

	// bk001205 - format issues
	client->sess.sessionTeam = (team_t)sessionTeam;
	client->sess.spectatorState = (spectatorState_t)spectatorState;
	client->sess.teamLeader = (qboolean)teamLeader;
}

/*
================
G_InitSessionData

Called on a first-time connect
================
*/
void G_InitSessionData( gclient_t *client, char *userinfo ) {
	clientSession_t	*sess;
	const char		*value;

	sess = &client->sess;

	// initial team determination
	if ( g_gametype.integer >= GT_TEAM ) {
		if ( g_teamAutoJoin.integer ) {
			sess->sessionTeam = PickTeam( -1 );
			BroadcastTeamChange( client, -1 );
		} else {
			// always spawn as spectator in team games
			sess->sessionTeam = TEAM_SPECTATOR;	
		}
	} else {
		value = Info_ValueForKey( userinfo, "team" );
		if ( value[0] == 's' ) {
			// a willing spectator, not a waiting-in-line
			sess->sessionTeam = TEAM_SPECTATOR;
		} else {
			switch ( g_gametype.integer ) {
			default:
			case GT_SANDBOX:
			case GT_MAPEDITOR:
			case GT_FFA:
				sess->sessionTeam = TEAM_FREE;
				break;
			}
		}
	}

	sess->spectatorState = SPECTATOR_FREE;

	G_WriteClientSessionData( client );
}

/*
==================
G_InitWorldSession

==================
*/
void G_InitWorldSession( void ) {
	char	s[MAX_STRING_CHARS];
	int		gt;

	//restore session from vQ3 session data
	trap_Cvar_VariableStringBuffer( "session", s, sizeof(s) );
	gt = atoi( s );

	// if the gametype changed since the last session, don't use any client sessions
	if ( g_gametype.integer != gt ) {
		level.newSession = qtrue;
        G_Printf( "Gametype changed, clearing session data.\n" );
	}
}

/*
==================
G_WriteSessionData

==================
*/
void G_WriteSessionData( void ) {
	int		i;

	trap_Cvar_Set( "session", va("%i", g_gametype.integer) );

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if ( level.clients[i].pers.connected == CON_CONNECTED ) {
			G_WriteClientSessionData( &level.clients[i] );
		}
	}
}
