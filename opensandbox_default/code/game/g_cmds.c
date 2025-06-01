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
==================
DeathmatchScoreboardMessage

==================
*/
void DeathmatchScoreboardMessage( gentity_t *ent ) {
	char		entry[64];
	char		string[1024];
	int			stringlength;
	int			i, j;
	gclient_t	*cl;
	gentity_t   *client;

	// send the latest information on all clients
	string[0] = 0;
	stringlength = 0;

	for (i=0 ; i < level.numConnectedClients ; i++) {
		int		ping;

		cl = &level.clients[level.sortedClients[i]];
		client = g_entities + cl->ps.clientNum;

		if (client->npcType){
			continue;
		}

		if ( cl->pers.connected == CON_CONNECTING ) {
			ping = -1;
		} else {
			ping = cl->ps.ping < 999 ? cl->ps.ping : 999;
			ping += 1;
		}

		Com_sprintf(entry, sizeof(entry), " %i %i %i %i", level.sortedClients[i], cl->ps.persistant[PERS_SCORE], ping, (level.time - cl->pers.enterTime)/60000);
		j = strlen(entry);
		if (stringlength + j >= sizeof(string))
			break;
		strcpy (string + stringlength, entry);
		stringlength += j;
	}

	trap_SendServerCommand(ent-g_entities, va("scores %i %i %i%s", i, level.teamScores[TEAM_RED], level.teamScores[TEAM_BLUE], string));
}

void G_SendGameCvars(gentity_t *ent) {
	char string[4096];
	Com_sprintf(string, sizeof(string), "%i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %i %f %f %f %f %f %i %i %i %i %f %f %i %i %i %i %i %i %i %i %i %i %i %i %i",
	            mod_sgspread, mod_sgcount, mod_lgrange, mod_mgspread, mod_cgspread, mod_jumpheight, mod_gdelay, mod_mgdelay ,mod_sgdelay, mod_gldelay, mod_rldelay, mod_lgdelay, mod_pgdelay, mod_rgdelay, mod_bfgdelay, mod_ngdelay, mod_pldelay, mod_cgdelay, mod_ftdelay, mod_scoutfirespeed, mod_ammoregenfirespeed, mod_doublerfirespeed, mod_guardfirespeed, mod_hastefirespeed, mod_noplayerclip, mod_ammolimit, mod_invulmove, mod_amdelay, mod_teamred_firespeed, mod_teamblue_firespeed, mod_medkitlimit, mod_medkitinf, mod_teleporterinf, mod_portalinf, mod_kamikazeinf, mod_invulinf, mod_accelerate, mod_movetype, mod_gravity, g_skyColorR.integer, g_skyColorG.integer, g_skyColorB.integer, g_skyColorA.integer);
	trap_SendServerCommand(ent-g_entities, va( "gCvars %s", string));
}

void G_SendSwepWeapons(gentity_t *ent) {
    char string[4096] = "";
    int i;
    int len;

    for (i = 1; i < WEAPONS_NUM; i++) {
		if(ent->swep_list[i] > 0){
			if(ent->swep_ammo[i] > 0 || ent->swep_ammo[i] == -1){
				ent->swep_list[i] = 1;	//we have weapon and ammo
			} else {
				ent->swep_list[i] = 2;	//we have weapon only
			}
		}
        if (ent->swep_list[i] == 1) {
            Q_strcat(string, sizeof(string), va("%i ", i));
        }
	    if (ent->swep_list[i] == 2) {
            Q_strcat(string, sizeof(string), va("%i ", i * -1));	//use -id for send 2
        }
    }
    len = strlen(string);
    if (len > 0 && string[len - 1] == ' ') {
        string[len - 1] = '\0';
    }

    trap_SendServerCommand(ent - g_entities, va("sweps %s", string));
}

void G_SendSpawnSwepWeapons(gentity_t *ent) {
    char string[4096] = "";
    int i;
    int len;

    for (i = 1; i < WEAPONS_NUM; i++) {
		if(ent->swep_list[i] > 0){
			if(ent->swep_ammo[i] > 0 || ent->swep_ammo[i] == -1){
				ent->swep_list[i] = 1;	//we have weapon and ammo
			} else {
				ent->swep_list[i] = 2;	//we have weapon only
			}
		}
        if (ent->swep_list[i] == 1) {
            Q_strcat(string, sizeof(string), va("%i ", i));
        }
	    if (ent->swep_list[i] == 2) {
            Q_strcat(string, sizeof(string), va("%i ", i * -1));	//use -id for send 2
        }
    }
    len = strlen(string);
    if (len > 0 && string[len - 1] == ' ') {
        string[len - 1] = '\0';
    }

    trap_SendServerCommand(ent - g_entities, va("wpspawn %s", string));
	ClientUserinfoChanged( ent->s.clientNum );
}

void RespawnTimeMessage(gentity_t *ent, int time) {
    trap_SendServerCommand( ent-g_entities, va("respawn %i", time) );
}

void ObeliskHealthMessage() {
    if(level.MustSendObeliskHealth) {
        trap_SendServerCommand( -1, va("oh %i %i",level.healthRedObelisk,level.healthBlueObelisk) );
        level.MustSendObeliskHealth = qfalse;
    }
}

/*
==================
Cmd_Score_f

Request current scoreboard information
==================
*/
void Cmd_Score_f( gentity_t *ent ) {
	DeathmatchScoreboardMessage( ent );
}

/*
==================
CheatsOk
==================
*/
qboolean CheatsOk( gentity_t *ent ) {
	if ( !g_cheats.integer ) {
		trap_SendServerCommand( ent-g_entities, va("print \"Cheats are not enabled on this server.\n\""));
		return qfalse;
	}
	return qtrue;
}


/*
==================
ConcatArgs
==================
*/
char *ConcatArgs( int start ) {
	int		i, c, tlen;
	static char	line[MAX_STRING_CHARS];
	int		len;
	char	arg[MAX_STRING_CHARS];

	len = 0;
	c = trap_Argc();
	for ( i = start ; i < c ; i++ ) {
		trap_Argv( i, arg, sizeof( arg ) );
		tlen = strlen( arg );
		if ( len + tlen >= MAX_STRING_CHARS - 1 ) {
			break;
		}
		memcpy( line + len, arg, tlen );
		len += tlen;
		if ( i != c - 1 ) {
			line[len] = ' ';
			len++;
		}
	}

	line[len] = 0;

	return line;
}

/*
==================
ClientNumberFromString

Returns a player number for either a number or name string
Returns -1 if invalid
==================
*/
int ClientNumberFromString( gentity_t *to, char *s ) {
	gclient_t	*cl;
	int			idnum;
    char        cleanName[MAX_STRING_CHARS];

	// numeric values are just slot numbers
	if (s[0] >= '0' && s[0] <= '9') {
		idnum = atoi( s );
		if ( idnum < 0 || idnum >= level.maxclients ) {
			trap_SendServerCommand( to-g_entities, va("print \"Bad client slot: %i\n\"", idnum));
			return -1;
		}

		cl = &level.clients[idnum];
		if ( cl->pers.connected != CON_CONNECTED ) {
			trap_SendServerCommand( to-g_entities, va("print \"Client %i is not active\n\"", idnum));
			return -1;
		}
		return idnum;
	}

	// check for a name match
	for ( idnum=0,cl=level.clients ; idnum < level.maxclients ; idnum++,cl++ ) {
		if ( cl->pers.connected != CON_CONNECTED ) {
			continue;
		}
        Q_strncpyz(cleanName, cl->pers.netname, sizeof(cleanName));
        Q_CleanStr(cleanName);
        if ( Q_strequal( cleanName, s ) ) {
			return idnum;
		}
	}

	trap_SendServerCommand( to-g_entities, va("print \"User %s is not on the server\n\"", s));
	return -1;
}

/*
==================
Cmd_Give_f

Give items to a client
==================
*/
void Cmd_Give_f (gentity_t *ent) {
	char		*name;
	gitem_t		*it;
	int			i;
	qboolean	give_all;
	gentity_t		*it_ent;
	trace_t		trace;

	if(g_gametype.integer != GT_SANDBOX && g_gametype.integer != GT_MAPEDITOR && !CheatsOk(ent)){ return; }
	if(!g_allowitems.integer){ return; }

	name = ConcatArgs( 1 );

	if Q_strequal(name, "all")
		give_all = qtrue;
	else
		give_all = qfalse;

	if (give_all || Q_strequal( name, "health"))
	{
		ent->health = ent->client->ps.stats[STAT_MAX_HEALTH];
		if (!give_all)
			return;
	}

	if (give_all || Q_strequal(name, "weapons"))
	{
		for(i = 1; i < WEAPONS_NUM; i++){
			ent->swep_list[i] = 1; 
			ent->swep_ammo[i] = 9999; 
		}
		if (!give_all)
			return;
	}

	if (give_all || Q_strequal(name, "ammo"))
	{
		for ( i = 1; i < WEAPONS_NUM; i++ ) {
			ent->swep_ammo[i] = 9999;
		}
		SetUnlimitedWeapons(ent);
		if (!give_all)
			return;
	}

	if (give_all || Q_strequal(name, "armor"))
	{
		ent->client->ps.stats[STAT_ARMOR] = 200;

		if (!give_all)
			return;
	}

	// spawn a specific item right on the player
	if ( !give_all ) {
		it = BG_FindItem (name);
		if (!it) {
			return;
		}

		it_ent = G_Spawn();
		VectorCopy( ent->r.currentOrigin, it_ent->s.origin );
		it_ent->classname = it->classname;
		G_SpawnItem (it_ent, it);
		FinishSpawningItem(it_ent );
		memset( &trace, 0, sizeof( trace ) );
		Touch_Item (it_ent, ent, &trace);
		if (it_ent->inuse) {
			G_FreeEntity( it_ent );
		}
	}
}

/*
==================
Cmd_VehicleExit_f

Exit from vehicle for player
==================
*/
void Cmd_VehicleExit_f (gentity_t *ent)
{
	if(ent->client->vehiclenum){
	ent->client->vehiclenum = 0;
	ClientUserinfoChanged( ent->s.clientNum );
	}
}

/*
==================
Cmd_God_f

Sets client to godmode

argv(0) god
==================
*/
void Cmd_God_f (gentity_t *ent)
{
	char	*msg;

	if ( !CheatsOk( ent ) ) {
		return;
	}

	ent->flags ^= FL_GODMODE;
	if (!(ent->flags & FL_GODMODE) )
		msg = "godmode OFF\n";
	else
		msg = "godmode ON\n";

	trap_SendServerCommand( ent-g_entities, va("print \"%s\"", msg));
}

/*
==================
Cmd_Noclip_f

argv(0) noclip
==================
*/
void Cmd_Noclip_f( gentity_t *ent ) {
	if(g_gametype.integer != GT_SANDBOX && g_gametype.integer != GT_MAPEDITOR){ return; }
	if(!g_allownoclip.integer){ return; }

	ent->client->noclip = !ent->client->noclip;
}

/*
==================
Cmd_TeamTask_f
==================
*/
void Cmd_TeamTask_f( gentity_t *ent ) {
	char userinfo[MAX_INFO_STRING];
	char		arg[MAX_TOKEN_CHARS];
	int task;
	int client = ent->client - level.clients;

	if ( trap_Argc() != 2 ) {
		return;
	}
	trap_Argv( 1, arg, sizeof( arg ) );
	task = atoi( arg );

	trap_GetUserinfo(client, userinfo, sizeof(userinfo));
	Info_SetValueForKey(userinfo, "teamtask", va("%d", task));
	trap_SetUserinfo(client, userinfo);
	ClientUserinfoChanged(client);
}

/*
=================
Cmd_Kill_f
=================
*/
void Cmd_Kill_f( gentity_t *ent ) {
	if ( ent->client->sess.sessionTeam == TEAM_SPECTATOR ) {
		return;
	}
	if (ent->health <= 0) {
		return;
	}
	ent->flags &= ~FL_GODMODE;
	ent->client->ps.stats[STAT_HEALTH] = ent->health = -999;
        if(ent->client->lastSentFlying>-1)
            //If player is in the air because of knockback we give credit to the person who sent it flying
            player_die (ent, ent, &g_entities[ent->client->lastSentFlying], 100000, MOD_FALLING);
        else
            player_die (ent, ent, ent, 100000, MOD_SUICIDE);
}

/*
=================
BroadCastTeamChange

Let everyone know about a team change
=================
*/
void BroadcastTeamChange( gclient_t *client, int oldTeam )
{
	if ( client->sess.sessionTeam == TEAM_RED ) {
		trap_SendServerCommand( -1, va("cp \"%s" S_COLOR_WHITE " joined the red team.\n\"",
			client->pers.netname) );
	} else if ( client->sess.sessionTeam == TEAM_BLUE ) {
		trap_SendServerCommand( -1, va("cp \"%s" S_COLOR_WHITE " joined the blue team.\n\"",
		client->pers.netname));
	} else if ( client->sess.sessionTeam == TEAM_SPECTATOR && oldTeam != TEAM_SPECTATOR ) {
		trap_SendServerCommand( -1, va("cp \"%s" S_COLOR_WHITE " joined the spectators.\n\"",
		client->pers.netname));
	} else if ( client->sess.sessionTeam == TEAM_FREE ) {
		trap_SendServerCommand( -1, va("cp \"%s" S_COLOR_WHITE " joined the battle.\n\"",
		client->pers.netname));
	}
}

/*
=================
SetTeam
KK-OAX Modded this to accept a forced admin change.
=================
*/
void SetTeam( gentity_t *ent, char *s ) {
	int					team, oldTeam;
	gclient_t			*client;
	int					clientNum;
	spectatorState_t	specState;
	int					specClient;
	int					teamLeader;
    char	            userinfo[MAX_INFO_STRING];

	//
	// see what change is requested
	//
	client = ent->client;

	clientNum = client - level.clients;
    trap_GetUserinfo( clientNum, userinfo, sizeof( userinfo ) );
	specClient = 0;
	specState = SPECTATOR_NOT;
	if ( Q_strequal( s, "scoreboard" ) || Q_strequal( s, "score" )  ) {
		team = TEAM_SPECTATOR;
		specState = SPECTATOR_SCOREBOARD;
	} else if ( Q_strequal( s, "follow1" ) ) {
		team = TEAM_SPECTATOR;
		specState = SPECTATOR_FOLLOW;
		specClient = -1;
	} else if ( Q_strequal( s, "follow2" ) ) {
		team = TEAM_SPECTATOR;
		specState = SPECTATOR_FOLLOW;
		specClient = -2;
	} else if ( Q_strequal( s, "spectator" ) || Q_strequal( s, "s" ) ) {
		team = TEAM_SPECTATOR;
		specState = SPECTATOR_FREE;
	} else if ( g_gametype.integer >= GT_TEAM ) {
		// if running a team game, assign player to one of the teams
		specState = SPECTATOR_NOT;
		if ( Q_strequal( s, "red" ) || Q_strequal( s, "r" ) ) {
			team = TEAM_RED;
		} else if ( Q_strequal( s, "blue" ) || Q_strequal( s, "b" ) ) {
			team = TEAM_BLUE;
		} else if ( Q_strequal( s, "free" ) && ent->npcType ) { //FREE_TEAM
			team = TEAM_FREE;
		} else {
			// pick the team with the least number of players
			team = PickTeam( clientNum );
		}
	} else {
		// force them to spectators if there aren't any spots free
		team = TEAM_FREE;
	}

	//
	// decide if we will allow the change
	//
	oldTeam = client->sess.sessionTeam;
	if ( team == oldTeam && team != TEAM_SPECTATOR ) {
		return;
	}

	//
	// execute the team change
	//
	
	// if the player was dead leave the body
	if ( client->ps.stats[STAT_HEALTH] <= 0 ) {
		CopyToBodyQue(ent);
	}

	// he starts at 'base'
	client->pers.teamState.state = TEAM_BEGIN;
	if ( oldTeam != TEAM_SPECTATOR ) {
        int teamscore = -99;

        //Prevent a team from loosing point because of player leaving team
        if(g_gametype.integer == GT_TEAM && ent->client->ps.stats[STAT_HEALTH])
            teamscore = level.teamScores[ ent->client->sess.sessionTeam ];

		// Kill him (makes sure he loses flags, etc)
		ent->flags &= ~FL_GODMODE;
		ent->client->ps.stats[STAT_HEALTH] = ent->health = 0;
		player_die (ent, ent, ent, 100000, MOD_SUICIDE);
        if(teamscore != -99)
            level.teamScores[ ent->client->sess.sessionTeam ] = teamscore;
	}

    if(oldTeam!=TEAM_SPECTATOR)
        PlayerStore_store(Info_ValueForKey(userinfo,"cl_guid"), client->ps);

	client->sess.sessionTeam = team;
	client->sess.spectatorState = specState;
	client->sess.spectatorClient = specClient;

	client->sess.teamLeader = qfalse;
	if ( team == TEAM_RED || team == TEAM_BLUE ) {
		teamLeader = TeamLeader( team );
		// if there is no team leader or the team leader is a bot and this client is not a bot
		if ( teamLeader == -1 || ( !(g_entities[clientNum].r.svFlags & SVF_BOT) && (g_entities[teamLeader].r.svFlags & SVF_BOT) ) ) {
			SetLeader( team, clientNum );
		}
	}
	// make sure there is a team leader on the team the player came from
	if ( oldTeam == TEAM_RED || oldTeam == TEAM_BLUE ) {
		CheckTeamLeader( oldTeam );
	}

	BroadcastTeamChange( client, oldTeam );

	// get and distribute relevent paramters
	ClientUserinfoChanged( clientNum );

	ClientBegin( clientNum );
}

/*
=================
StopFollowing

If the client being followed leaves the game, or you just want to drop
to free floating spectator mode
=================
*/
void StopFollowing( gentity_t *ent ) {
	ent->client->ps.persistant[ PERS_TEAM ] = TEAM_SPECTATOR;	
	ent->client->sess.sessionTeam = TEAM_SPECTATOR;	
	ent->client->sess.spectatorState = SPECTATOR_FREE;
	ent->client->ps.pm_flags &= ~PMF_FOLLOW;
	ent->r.svFlags &= ~SVF_BOT;
	ent->client->ps.clientNum = ent - g_entities;
}

/*
=================
Cmd_Team_f
=================
*/
void Cmd_Team_f( gentity_t *ent ) {
	int			oldTeam;
	char		s[MAX_TOKEN_CHARS];

	if ( trap_Argc() != 2 ) {
		oldTeam = ent->client->sess.sessionTeam;
		switch ( oldTeam ) {
		case TEAM_BLUE:
			trap_SendServerCommand( ent-g_entities, "print \"Blue team\n\"" );
			break;
		case TEAM_RED:
			trap_SendServerCommand( ent-g_entities, "print \"Red team\n\"" );
			break;
		case TEAM_FREE:
			trap_SendServerCommand( ent-g_entities, "print \"Deathmatch-Playing\n\"" );
			break;
		case TEAM_SPECTATOR:
			trap_SendServerCommand( ent-g_entities, "print \"Spectator team\n\"" );
			break;
		}
		return;
	}

	trap_Argv( 1, s, sizeof( s ) );

	SetTeam( ent, s );

	ent->client->switchTeamTime = level.time + 5000;
}


/*
=================
Cmd_Follow_f
=================
*/
void Cmd_Follow_f( gentity_t *ent ) {
	int		i;
	char	arg[MAX_TOKEN_CHARS];

	if ( trap_Argc() != 2 ) {
		if ( ent->client->sess.spectatorState == SPECTATOR_FOLLOW ) {
			StopFollowing( ent );
		}
		return;
	}

	trap_Argv( 1, arg, sizeof( arg ) );
	i = ClientNumberFromString( ent, arg );
	if ( i == -1 ) {
		return;
	}

	// can't follow self
	if ( &level.clients[ i ] == ent->client ) {
		return;
	}

	// can't follow another spectator (or an eliminated player)
	if ( level.clients[ i ].sess.sessionTeam == TEAM_SPECTATOR ) {
		return;
	}

	// first set them to spectator
	//if ( ent->client->sess.sessionTeam != TEAM_SPECTATOR ) {
	if ( ent->client->sess.spectatorState == SPECTATOR_NOT ) {
		SetTeam( ent, "spectator" );
	}

	ent->client->sess.spectatorState = SPECTATOR_FOLLOW;
	ent->client->sess.spectatorClient = i;
}

/*
=================
Cmd_FollowCycle_f
KK-OAX Modified to trap arguments.
=================
*/
void Cmd_FollowCycle_f( gentity_t *ent ) {
	int		clientnum;
	int		original;
    int     count;
    char    args[11];
    int     dir;
	gentity_t		*checkNPC;

    if( ent->client->sess.sessionTeam == TEAM_NONE ) {
        dir = 1;
    }

    trap_Argv( 0, args, sizeof( args ) );
    if( Q_strequal( args, "followprev" )) {
        dir = -1;
    } else if( Q_strequal( args, "follownext" )) {
        dir = 1;
    } else {
        dir = 1;
    }

	// first set them to spectator
	if ( ent->client->sess.spectatorState == SPECTATOR_NOT ) {
		SetTeam( ent, "spectator" );
	}

	if ( dir != 1 && dir != -1 ) {
		G_Error( "Cmd_FollowCycle_f: bad dir %i", dir );
	}

	clientnum = ent->client->sess.spectatorClient;
	original = clientnum;
        count = 0;
	do {
		clientnum += dir;
                count++;
		if ( clientnum >= level.maxclients ) {
			clientnum = 0;
		}
		if ( clientnum < 0 ) {
			clientnum = level.maxclients - 1;
		}

        if(count>level.maxclients) //We have looked at all clients at least once and found nothing
            return; //We might end up in an infinite loop here. Stop it!

		// can only follow connected clients
		if ( level.clients[ clientnum ].pers.connected != CON_CONNECTED ) {
			continue;
		}

		// can't follow another spectator
		if ( level.clients[ clientnum ].sess.sessionTeam == TEAM_SPECTATOR ) {
			continue;
		}

		// can't follow npc
		checkNPC = g_entities + clientnum;
		if ( checkNPC->npcType ) {
			continue;
		}

		// this is good, we can use it
		ent->client->sess.spectatorClient = clientnum;
		ent->client->sess.spectatorState = SPECTATOR_FOLLOW;
		return;
	} while ( clientnum != original );

	// leave it where it was
}


/*
==================
G_Say
==================
*/

static void G_SayTo( gentity_t *ent, gentity_t *other, int mode, int color, const char *name, const char *message ) {
	if (!other) {
		return;
	}
	if (!other->inuse) {
		return;
	}
	if (!other->client) {
		return;
	}
	if ( other->client->pers.connected != CON_CONNECTED ) {
		return;
	}

    if ((ent->r.svFlags & SVF_BOT) && cvar_VariableValue( "bot_nochat" )>1) return;

	trap_SendServerCommand( other-g_entities, va("%s \"%s%c%c%s\"",
		mode == SAY_TEAM ? "tchat" : "chat",
		name, Q_COLOR_ESCAPE, color, message));
}

#define EC		"\x19"

void G_Say( gentity_t *ent, gentity_t *target, int mode, const char *chatText ) {
	int			j;
	gentity_t	*other;
	int			color;
	char		name[64];
	// don't let text be too long for malicious reasons
	char		text[MAX_SAY_TEXT];
	char		location[64];

    if ((ent->r.svFlags & SVF_BOT) && cvar_VariableValue( "bot_nochat" )>1) return;

	switch ( mode ) {
	default:
	case SAY_ALL:
		Com_sprintf (name, sizeof(name), "%s%c%c"EC": ", ent->client->pers.netname, Q_COLOR_ESCAPE, COLOR_WHITE );
		color = COLOR_GREEN;
		break;
	case SAY_TEAM:
		if (Team_GetLocationMsg(ent, location, sizeof(location)))
			Com_sprintf (name, sizeof(name), EC"(%s%c%c"EC") (%s)"EC": ",
				ent->client->pers.netname, Q_COLOR_ESCAPE, COLOR_WHITE, location);
		else
			Com_sprintf (name, sizeof(name), EC"(%s%c%c"EC")"EC": ",
				ent->client->pers.netname, Q_COLOR_ESCAPE, COLOR_WHITE );
		color = COLOR_CYAN;
		break;
	case SAY_TELL:
		if (target && g_gametype.integer >= GT_TEAM &&
			target->client->sess.sessionTeam == ent->client->sess.sessionTeam &&
			Team_GetLocationMsg(ent, location, sizeof(location)))
			Com_sprintf (name, sizeof(name), EC"[%s%c%c"EC"] (%s)"EC": ", ent->client->pers.netname, Q_COLOR_ESCAPE, COLOR_WHITE, location );
		else
			Com_sprintf (name, sizeof(name), EC"[%s%c%c"EC"]"EC": ", ent->client->pers.netname, Q_COLOR_ESCAPE, COLOR_WHITE );
		color = COLOR_MAGENTA;
		break;
	}

	Q_strncpyz( text, chatText, sizeof(text) );

	if ( target ) {
		G_SayTo( ent, target, mode, color, name, text );
		return;
	}

	// echo the text to the console
	if ( g_dedicated.integer ) {
		G_Printf( "%s%s\n", name, text);
	}

	// send it to all the apropriate clients
	for (j = 0; j < level.maxclients; j++) {
		other = &g_entities[j];
		G_SayTo( ent, other, mode, color, name, text );
	}
}


/*
==================
Cmd_Say_f
KK-OAX Modified this to trap the additional arguments from console.
==================
*/
static void Cmd_Say_f( gentity_t *ent ){
	char		*p;
	char        arg[MAX_TOKEN_CHARS];
	int         mode = SAY_ALL;

    trap_Argv( 0, arg, sizeof( arg ) );
    if( Q_strequal( arg, "say_team" ) )
        mode = SAY_TEAM ;

    if( trap_Argc( ) < 2 )
        return;

    p = ConcatArgs( 1 );

    G_Say( ent, NULL, mode, p );
}

/*
==================
Cmd_SpawnList_Item_f
Added for OpenSandbox.
==================
*/
static void Cmd_SpawnList_Item_f( gentity_t *ent ){
	vec3_t		end, start, forward, up, right;
	trace_t		tr;
	gentity_t 	*tent;
	char		arg01[64];
	char		arg02[64];
	char		arg03[64];
	char		arg04[64];
	char		arg05[64];
	char		arg06[64];
	char		arg07[64];
	char		arg08[64];
	char		arg09[64];
	char		arg10[64];
	char		arg11[64];
	char		arg12[64];
	char		arg13[64];
	char		arg14[64];
	char		arg15[64];
	char		arg16[64];
	char		arg17[64];
	char		arg18[64];
	char		arg19[64];
	char		arg20[64];
	char		arg21[64];
	char		arg22[64];
	
	if(g_gametype.integer != GT_SANDBOX && g_gametype.integer != GT_MAPEDITOR){ return; }

	if ( ent->client->sess.sessionTeam == TEAM_SPECTATOR ) {
		return;
	}
		
	//tr.endpos
	trap_Argv( 1, arg01, sizeof( arg01 ) );
	trap_Argv( 2, arg02, sizeof( arg02 ) );
	trap_Argv( 3, arg03, sizeof( arg03 ) );
	trap_Argv( 4, arg04, sizeof( arg04 ) );
	trap_Argv( 5, arg05, sizeof( arg05 ) );
	trap_Argv( 6, arg06, sizeof( arg06 ) );
	trap_Argv( 7, arg07, sizeof( arg07 ) );
	trap_Argv( 8, arg08, sizeof( arg08 ) );
	trap_Argv( 9, arg09, sizeof( arg09 ) );
	trap_Argv( 10, arg10, sizeof( arg10 ) );
	trap_Argv( 11, arg11, sizeof( arg11 ) );
	trap_Argv( 12, arg12, sizeof( arg12 ) );
	trap_Argv( 13, arg13, sizeof( arg13 ) );
	trap_Argv( 14, arg14, sizeof( arg14 ) );
	trap_Argv( 15, arg15, sizeof( arg15 ) );
	trap_Argv( 16, arg16, sizeof( arg16 ) );
	trap_Argv( 17, arg17, sizeof( arg17 ) );
	trap_Argv( 18, arg18, sizeof( arg18 ) );
	trap_Argv( 19, arg19, sizeof( arg19 ) );
	trap_Argv( 20, arg20, sizeof( arg20 ) );
	trap_Argv( 21, arg21, sizeof( arg21 ) );
	trap_Argv( 22, arg22, sizeof( arg22 ) );
	
	//Set Aiming Directions
	AngleVectors(ent->client->ps.viewangles, forward, right, up);
	CalcMuzzlePoint(ent, forward, right, up, start);
	VectorMA (start, TOOLGUN_RANGE, forward, end);

	//Trace Position
	trap_Trace (&tr, start, NULL, NULL, end, ent->s.number, MASK_SELECT );
	
	if(!Q_stricmp (arg01, "prop")){
	if(!g_allowprops.integer){ return; }
	
	tent = G_TempEntity( tr.endpos, EV_PARTICLES_GRAVITY );
	tent->s.constantLight = (((rand() % 256 | rand() % 256 << 8 ) | rand() % 256 << 16 ) | ( 255 << 24 ));
	tent->s.eventParm = 24; //eventParm is used to determine the number of particles
	tent->s.generic1 = 500; //generic1 is used to determine the speed of the particles
	tent->s.generic2 = 16; //generic2 is used to determine the size of the particles
	G_BuildPropSL( arg02, arg03, tr.endpos, ent, arg04, arg05, arg06, arg07, arg08, arg09, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19, arg20, arg21, arg22);
	
	return;
	}
	if(!Q_stricmp (arg01, "npc")){
	if(!g_allownpc.integer){ return; }
	
	tent = G_Spawn();
	tent->sb_isnpc = 1;
	VectorCopy( tr.endpos, tent->s.origin);
	tent->s.origin[2] += 25;
	tent->classname = "sandbox_npc";
	CopyAlloc(tent->clientname, arg02);
	tent->type = NPC_ENEMY;
	if(!Q_stricmp (arg03, "NPC_Enemy")){
	tent->type = NPC_ENEMY;
	}
	if(!Q_stricmp (arg03, "NPC_Citizen")){
	tent->type = NPC_CITIZEN;
	}
	if(!Q_stricmp (arg03, "NPC_Guard")){
	tent->type = NPC_GUARD;
	}
	if(!Q_stricmp (arg03, "NPC_Partner")){
	tent->type = NPC_PARTNER;
	}
	if(!Q_stricmp (arg03, "NPC_PartnerEnemy")){
	tent->type = NPC_PARTNERENEMY;
	}
	tent->skill = atof(arg04);
	tent->health = atoi(arg05);
	CopyAlloc(tent->message, arg06);	
	tent->mtype = atoi(arg08);
	if(!Q_stricmp (arg07, "0") ){
	CopyAlloc(tent->target, arg02);	
	} else {
	CopyAlloc(tent->target, arg07);	
	}
	if(tent->health <= 0){
	tent->health = 100;
	}
	if(tent->skill <= 0){
	tent->skill = 1;
	}
	if(!Q_stricmp (tent->message, "0") || !tent->message ){
	CopyAlloc(tent->message, tent->clientname);
	}
	G_AddBot(tent->clientname, tent->skill, "Blue", 0, tent->message, tent->s.number, tent->target, tent->type, tent );
	
	if(tent->parent){
		Undo_AddElement(ent, tent->parent->s.clientNum, UNDO_NPCSPAWN);
	}

	trap_Cvar_Set("g_spSkill", arg04);
	return;
	}
}

/*
==================
Cmd_Modify_Prop_f
Added for OpenSandbox.
==================
*/
static void Cmd_Modify_Prop_f( gentity_t *ent ){
	vec3_t		end, start, forward, up, right;
	trace_t		tr;
	gentity_t 	*tent;
	gentity_t	*traceEnt;
	char		arg01[64];
	char		arg02[64];
	char		arg03[64];
	char		arg04[64];
	char		arg05[64];
	char		arg06[64];
	char		arg07[64];
	char		arg08[64];
	char		arg09[64];
	char		arg10[64];
	char		arg11[64];
	char		arg12[64];
	char		arg13[64];
	char		arg14[64];
	char		arg15[64];
	char		arg16[64];
	char		arg17[64];
	char		arg18[64];
	char		arg19[64];
	
	if(g_gametype.integer != GT_SANDBOX && g_gametype.integer != GT_MAPEDITOR){ return; }

	if ( ent->client->sess.sessionTeam == TEAM_SPECTATOR ) {
		return;
	}
		
	//tr.endpos
	trap_Argv( 1, arg01, sizeof( arg01 ) );
	trap_Argv( 2, arg02, sizeof( arg02 ) );
	trap_Argv( 3, arg03, sizeof( arg03 ) );
	trap_Argv( 4, arg04, sizeof( arg04 ) );
	trap_Argv( 5, arg05, sizeof( arg05 ) );
	trap_Argv( 6, arg06, sizeof( arg06 ) );
	trap_Argv( 7, arg07, sizeof( arg07 ) );
	trap_Argv( 8, arg08, sizeof( arg08 ) );
	trap_Argv( 9, arg09, sizeof( arg09 ) );
	trap_Argv( 10, arg10, sizeof( arg10 ) );
	trap_Argv( 11, arg11, sizeof( arg11 ) );
	trap_Argv( 12, arg12, sizeof( arg12 ) );
	trap_Argv( 13, arg13, sizeof( arg13 ) );
	trap_Argv( 14, arg14, sizeof( arg14 ) );
	trap_Argv( 15, arg15, sizeof( arg15 ) );
	trap_Argv( 16, arg16, sizeof( arg16 ) );
	trap_Argv( 17, arg17, sizeof( arg17 ) );
	trap_Argv( 18, arg18, sizeof( arg18 ) );
	trap_Argv( 19, arg19, sizeof( arg19 ) );
	
	//Set Aiming Directions
	AngleVectors(ent->client->ps.viewangles, forward, right, up);
	CalcMuzzlePoint(ent, forward, right, up, start);
	VectorMA (start, TOOLGUN_RANGE, forward, end);

	//Trace Position
	trap_Trace (&tr, start, NULL, NULL, end, ent->s.number, MASK_SELECT );
	
	traceEnt = &g_entities[ tr.entityNum ];		//entity for modding

	if(!traceEnt->sandboxObject && !traceEnt->npcType && ent->s.eType != ET_ITEM){
		return;
	}
	
	tent = G_TempEntity( tr.endpos, EV_PARTICLES_GRAVITY );
	tent->s.constantLight = (((rand() % 256 | rand() % 256 << 8 ) | rand() % 256 << 16 ) | ( 255 << 24 ));
	tent->s.eventParm = 24; //eventParm is used to determine the number of particles
	tent->s.generic1 = 125; //generic1 is used to determine the speed of the particles
	tent->s.generic2 = 3; //generic2 is used to determine the size of the particles
	G_ModProp( traceEnt, ent, arg01, arg02, arg03, arg04, arg05, arg06, arg07, arg08, arg09, arg10, arg11, arg12, arg13, arg14, arg15, arg16, arg17, arg18, arg19);
	return;
}

/*
==================
Cmd_Altfire_Physgun_f
Added for OpenSandbox.
==================
*/
static void Cmd_Altfire_Physgun_f( gentity_t *ent ){
	if ( ent->client->ps.generic2 == WP_PHYSGUN ){
	    if (ent->client->buttons & BUTTON_ATTACK) {
			if (ent->grabbedEntity) {
				ent->grabbedEntity->grabNewPhys = PHYS_STATIC;	//say physgun about freeze option
			}
		}
	}
}

/*
==================
Cmd_PhysgunDist_f
Added for OpenSandbox.
==================
*/
static void Cmd_PhysgunDist_f( gentity_t *ent ){
	char		mode[MAX_TOKEN_CHARS];
	
	trap_Argv( 1, mode, sizeof( mode ) );
	
	if ( ent->client->ps.generic2 == WP_PHYSGUN ){
	    if (ent->client->buttons & BUTTON_ATTACK) {
			if (ent->grabbedEntity) {
				if(atoi(mode) == 0){
					ent->grabDist -= 20;
					if(ent->grabbedEntity->sb_coltype){
						if(ent->grabDist < ent->grabbedEntity->sb_coltype+1){
							ent->grabDist = ent->grabbedEntity->sb_coltype+1;
						}
					} else {
						if(ent->grabDist < 100){
							ent->grabDist = 100;
						}	
					}
				}
				if(atoi(mode) == 1){
					ent->grabDist += 20;
				}
			}
		}
	}
}

/*
==================
Cmd_Flashlight_f
Added for OpenSandbox.
==================
*/
static void Cmd_Flashlight_f( gentity_t *ent ){

	if ( ent->client->sess.sessionTeam == TEAM_SPECTATOR ) {
		return;
	}

	if(!ent->flashlight){
		ent->flashlight = qtrue;
		ClientUserinfoChanged( ent->s.clientNum );
		return;
	}
	if(ent->flashlight){
		ent->flashlight = qfalse;
		ClientUserinfoChanged( ent->s.clientNum );
		return;
	}
	
}

/*
==================
Cmd_Undo_f
Added for OpenSandbox.
==================
*/
static void Cmd_Undo_f(gentity_t *ent) {
    int id, type;
	qboolean isRemoved;

    if (g_gametype.integer != GT_SANDBOX && g_gametype.integer != GT_MAPEDITOR) {
        return;
    }

    while (Undo_LastElement(ent, &id, &type, &isRemoved)) {
        if (isRemoved || id == 0) {
            Undo_RemoveElement(ent);
            continue;
        }

        switch (type) {
            case UNDO_PROPSPAWN: {
                gentity_t *prop = G_FindEntityForEntityNum(id);
                if (prop) {
                    G_FreeEntity(prop);
                    trap_SendServerCommand(ent - g_entities, "undoProp \n");
                }
                return;
            }

            case UNDO_NPCSPAWN:
                DropClientSilently(id);
                trap_SendServerCommand(ent - g_entities, "undoNPC \n");
                return;

            default:
                return;
        }
    }
}

/*
==================
Cmd_Where_f
==================
*/
void Cmd_Where_f( gentity_t *ent ) {
	trap_SendServerCommand( ent-g_entities, va("print \"%s\n\"", vtos(ent->r.currentOrigin) ) );
}

static const char *gameNames[] = {
	"Sandbox",
	"Map Editor",
	"Free For All",
	"Team Deathmatch",
	"Capture the Flag",
	"One Flag Capture",
	"Overload",
	"Harvester",
	0
};

/*
=================
Cmd_SetViewpos_f
=================
*/
void Cmd_SetViewpos_f( gentity_t *ent ) {
	vec3_t		origin;
	char		buffer[MAX_TOKEN_CHARS];
	int			i;

	if ( !g_cheats.integer ) {
		trap_SendServerCommand( ent-g_entities, va("print \"Cheats are not enabled on this server.\n\""));
		return;
	}
	if ( trap_Argc() != 4 ) {
		trap_SendServerCommand( ent-g_entities, va("print \"usage: setviewpos x y z\n\""));
		return;
	}

	for ( i = 0 ; i < 3 ; i++ ) {
		trap_Argv( i + 1, buffer, sizeof( buffer ) );
		origin[i] = atof( buffer );
	}

	TeleportPlayer(ent, origin, ent->client->ps.viewangles);
}

/*
=================
Cmd_UseTarget_f
=================
*/
void Cmd_UseTarget_f( gentity_t *ent ) {
	char		*p;
	char        arg[MAX_TOKEN_CHARS];
	gentity_t 	*act;

    trap_Argv( 0, arg, sizeof( arg ) );

    if( trap_Argc( ) < 2 )
        return;

    p = ConcatArgs( 1 );
	
	ent->target = p;

	act = G_PickTarget( ent->target );
	if ( act && act->use && act->allowuse ) {
		act->use( act, ent, ent );
	}
}

/*
=================
Cmd_ActivateTarget_f
=================
*/
void Cmd_ActivateTarget_f(gentity_t *ent) {
	char	*p;
	char	arg[MAX_TOKEN_CHARS];

    trap_Argv( 0, arg, sizeof( arg ) );

    if( trap_Argc() < 2 )
        return;

    p = ConcatArgs(1);
	
	ent->target = va("activate_%i_%s", ent->s.clientNum, p);

	G_PickAllTargets(ent);
}

//KK-OAX This is the table that ClientCommands runs the console entry against.
commands_t cmds[ ] =
{
  // can be used even during intermission
  { "say", CMD_MESSAGE|CMD_INTERMISSION, Cmd_Say_f },
  { "say_team", CMD_MESSAGE|CMD_INTERMISSION, Cmd_Say_f },
  { "score", CMD_INTERMISSION, Cmd_Score_f },

  // cheats
  { "give", CMD_LIVING, Cmd_Give_f },
  { "god", CMD_CHEAT|CMD_LIVING, Cmd_God_f },
  { "setviewpos", CMD_CHEAT, Cmd_SetViewpos_f },
  { "noclip", CMD_LIVING, Cmd_Noclip_f },

  // interaction
  { "exitvehicle", CMD_LIVING, Cmd_VehicleExit_f },
  { "kill", CMD_TEAM|CMD_LIVING, Cmd_Kill_f },
  { "sl", CMD_LIVING, Cmd_SpawnList_Item_f },	//hidden
  { "tm", CMD_LIVING, Cmd_Modify_Prop_f },		//hidden
  { "altfire_physgun", CMD_LIVING, Cmd_Altfire_Physgun_f },		//hidden
  { "physgun_dist", CMD_LIVING, Cmd_PhysgunDist_f },	//hidden
  { "flashlight", CMD_LIVING, Cmd_Flashlight_f },
  { "undo", CMD_LIVING, Cmd_Undo_f },
  { "usetarget", CMD_LIVING, Cmd_UseTarget_f },
  { "activate", CMD_LIVING, Cmd_ActivateTarget_f }, 
  { "where", 0, Cmd_Where_f },

  // game commands
  { "follow", CMD_NOTEAM, Cmd_Follow_f },
  { "follownext", CMD_NOTEAM, Cmd_FollowCycle_f },
  { "followprev", CMD_NOTEAM, Cmd_FollowCycle_f },
  { "teamtask", CMD_TEAM, Cmd_TeamTask_f },
  { "freespectator", CMD_NOTEAM, StopFollowing }

};

static int numCmds = sizeof( cmds ) / sizeof( cmds[ 0 ] );

/*
=================
ClientCommand
KK-OAX, Takes the client command and runs it through a loop which matches
it against the table.
=================
*/
void ClientCommand( int clientNum )
{
    gentity_t *ent;
    char      cmd[ MAX_TOKEN_CHARS ];
    int       i;

    ent = g_entities + clientNum;
    if( !ent->client )
        return;   // not fully in game yet

    trap_Argv( 0, cmd, sizeof( cmd ) );

    for( i = 0; i < numCmds; i++ )
    {
        if( Q_stricmp( cmd, cmds[ i ].cmdName ) == 0 )
            break;
    }

	if (i == numCmds) 
	{
	    trap_SendServerCommand(clientNum, va("print \"Unknown command: %s\n\"", cmd));
	    return;
	}

	// do tests here to reduce the amount of repeated code
    if( !( cmds[ i ].cmdFlags & CMD_INTERMISSION ) && level.intermissiontime )
        return;

    if( cmds[ i ].cmdFlags & CMD_CHEAT && !g_cheats.integer )
    {
        trap_SendServerCommand( clientNum, "print \"Cheats are not enabled on this server\n\"" );
        return;
    }

    //KK-OAX Do I need to change this for FFA gametype?
    if( cmds[ i ].cmdFlags & CMD_TEAM &&
        ent->client->sess.sessionTeam == TEAM_SPECTATOR )
    {
        trap_SendServerCommand( clientNum, "print \"Join a team first\n\"" );
        return;
    }

    if( ( cmds[ i ].cmdFlags & CMD_NOTEAM ||
        ( cmds[ i ].cmdFlags & CMD_CHEAT_TEAM && !g_cheats.integer ) ) &&
        ent->client->sess.sessionTeam != TEAM_NONE )
    {
        trap_SendServerCommand( clientNum, "print \"Cannot use this command when on a team\n\"" );
        return;
    }

    if( ( ent->client->ps.pm_type == PM_DEAD ) && ( cmds[ i ].cmdFlags & CMD_LIVING ) )
    {
        trap_SendServerCommand( clientNum, "print \"Must be alive to use this command\n\"" );
        return;
    }

    cmds[ i ].cmdHandler( ent );
}
