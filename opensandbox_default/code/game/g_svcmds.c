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

#include "g_local.h"

/*
===================
Svcmd_EntityList_f
===================
*/
void Svcmd_EntityList_f (void) {
	int			e;
	gentity_t	*check;

	check = g_entities+1;
	for (e = 1; e < level.num_entities ; e++, check++) {
		if (!check->inuse)
			continue;
		G_Printf("%3i:    ", e);

		if (check->classname)
			G_Printf("%s", check->classname);
		G_Printf("\n");
	}
}

gclient_t	*ClientForString( const char *s ) {
	gclient_t	*cl;
	int			i;
	int			idnum;

	// numeric values are just slot numbers
	if ( s[0] >= '0' && s[0] <= '9' ) {
		idnum = atoi( s );
		if ( idnum < 0 || idnum >= level.maxclients ) {
			Com_Printf( "Bad client slot: %i\n", idnum );
			return NULL;
		}

		cl = &level.clients[idnum];
		if ( cl->pers.connected == CON_DISCONNECTED ) {
			G_Printf( "Client %i is not connected\n", idnum );
			return NULL;
		}
		return cl;
	}

	// check for a name match
	for (i=0 ; i < level.maxclients ; i++) {
		cl = &level.clients[i];
		if (cl->pers.connected == CON_DISCONNECTED)
			continue;
		if (!Q_stricmp(cl->pers.netname, s))
			return cl;
	}

	G_Printf( "User %s is not on the server\n", s );
	return NULL;
}

/*
===================
Svcmd_ForceTeam_f
forceteam <player> <team>
===================
*/
void	Svcmd_ForceTeam_f( void ) {
	gclient_t	*cl;
	char		str[MAX_TOKEN_CHARS];

	// find the player
	trap_Argv( 1, str, sizeof( str ) );
	cl = ClientForString( str );
	if ( !cl ) {
		return;
	}

	// set the team
	trap_Argv( 2, str, sizeof( str ) );
	SetTeam( &g_entities[cl - level.clients], str );
}

/*
===============
Server Command Table
================
*/
struct {
  char      *cmd;
  void      (*function)(void);
} svcmds[] = {
  { "entityList", Svcmd_EntityList_f },
  { "forceTeam", Svcmd_ForceTeam_f },
  { "addbot", Svcmd_AddBot_f },

  { "savemap", G_WriteMapfile_f },
  { "deletemap", G_DeleteMapfile_f },
  { "clearmap", G_ClearMap_f },
  { "clearmap_sandbox", G_ClearSandboxMap_f },
  { "loadmap", G_LoadMapfile_f },

  //Noire.Script
  { "ns_openscript", Svcmd_NS_OpenScript_f },
  { "ns_interpret", Svcmd_NS_Interpret_f },
  { "ns_variablelist", Svcmd_NS_VariableList_f },
  { "ns_threadlist", Svcmd_NS_ThreadList_f },
  { "ns_sendvariable", Svcmd_NS_SendVariable_f },
};

/*
=================
ConsoleCommand
=================
*/
qboolean  ConsoleCommand( void ) {
	char cmd[ MAX_TOKEN_CHARS ];
	int  i;

	trap_Argv( 0, cmd, sizeof( cmd ) );

	for(i = 0; i < sizeof(svcmds) / sizeof(svcmds[0]); i++){
		if(!Q_stricmp(cmd, svcmds[i].cmd)) {
			svcmds[i].function();
			return qtrue;
		}
	}

	if(g_dedicated.integer)
		G_Printf( "unknown command: %s\n", cmd );

  return qfalse;
}
