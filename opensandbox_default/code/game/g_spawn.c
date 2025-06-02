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

qboolean	G_SpawnString( const char *key, const char *defaultString, char **out ) {
	int		i;

	if ( !level.spawning )
		*out = (char *)defaultString;

	for ( i = 0 ; i < level.numSpawnVars ; i++ ) {
		if ( !Q_stricmp( key, level.spawnVars[i][0] ) ) {
			*out = level.spawnVars[i][1];
			return qtrue;
		}
	}

	*out = (char *)defaultString;
	return qfalse;
}

qboolean	G_SpawnFloat( const char *key, const char *defaultString, float *out ) {
	char		*s;
	qboolean	present;

	present = G_SpawnString( key, defaultString, &s );
	*out = atof( s );
	return present;
}

qboolean	G_SpawnInt( const char *key, const char *defaultString, int *out ) {
	char		*s;
	qboolean	present;

	present = G_SpawnString( key, defaultString, &s );
	*out = atoi( s );
	return present;
}

qboolean	G_SpawnVector( const char *key, const char *defaultString, float *out ) {
	char		*s;
	qboolean	present;

	present = G_SpawnString( key, defaultString, &s );
	sscanf( s, "%f %f %f", &out[0], &out[1], &out[2] );
	return present;
}

void SP_EmptySpawn(gentity_t *ent) { G_SetOrigin( ent, ent->s.origin ); }
void SP_DeleteSpawn(gentity_t *ent) { G_FreeEntity(ent); }

//
// fields are needed for spawning from the entity string
//
typedef enum {
	F_INT, 
	F_FLOAT,
	F_STRING,
	F_VECTOR,
	F_ANGLEHACK,
	F_IGNORE
} fieldtype_t;

typedef struct {
	char	*name;
	int		ofs;
	fieldtype_t	type;
	int		flags;
} field_t;

field_t fields[] = {
	{"classname", FOFS(classname), F_STRING},
	{"model", FOFS(model), F_STRING},
	{"model2", FOFS(model2), F_STRING},
	{"origin", FOFS(s.origin), F_VECTOR},
	{"angles", FOFS(s.angles), F_VECTOR},
	{"modelscale_vec", FOFS(s.scales), F_VECTOR},
	{"spawnflags", FOFS(spawnflags), F_INT},
	{"speed", FOFS(speed), F_FLOAT},
	{"target", FOFS(target), F_STRING},
	{"targetname", FOFS(targetname), F_STRING},
	{"message", FOFS(message), F_STRING},
	{"team", FOFS(team), F_STRING},
	{"wait", FOFS(wait), F_FLOAT},
	{"random", FOFS(random), F_FLOAT},
	{"count", FOFS(count), F_INT},
	{"health", FOFS(health), F_INT},
	{"light", 0, F_IGNORE},
	{"dmg", FOFS(damage), F_INT},
	{"mtype", FOFS(mtype), F_INT},
	{"mtimeout", FOFS(mtimeout), F_INT},
	{"mhoming", FOFS(mhoming), F_INT},
	{"mspeed", FOFS(mspeed), F_INT},
	{"mbounce", FOFS(mbounce), F_INT},
	{"mdamage", FOFS(mdamage), F_INT},
	{"msdamage", FOFS(msdamage), F_INT},
	{"msradius", FOFS(msradius), F_INT},
	{"mgravity", FOFS(mgravity), F_INT},
	{"mnoclip", FOFS(mnoclip), F_INT},
	{"allowuse", FOFS(allowuse), F_INT},
	{"angle", FOFS(s.angles), F_ANGLEHACK},		//deleted in mapfiles
	{"mapname", FOFS(mapname), F_STRING},
	{"clientname", FOFS(clientname), F_STRING},
	{"teleporterTarget", FOFS(teleporterTarget), F_STRING},
	{"deathTarget", FOFS(deathTarget), F_STRING},
	{"lootTarget", FOFS(lootTarget), F_STRING},
	{"skill", FOFS(skill), F_FLOAT},
	{"overlay", FOFS(overlay), F_STRING},
	{"target2", FOFS(target2), F_STRING},
	{"damagetarget", FOFS(damagetarget), F_STRING},
	{"targetname2", FOFS(targetname2), F_STRING},
	{"key", FOFS(key), F_STRING},
	{"value", FOFS(value), F_STRING},
	{"armor", FOFS(armor), F_INT},
	{"music", FOFS(music), F_STRING},
	{"sb_class", FOFS(sb_class), F_STRING},
	{"sb_sound", FOFS(sb_sound), F_STRING},
	{"sb_coltype", FOFS(sb_coltype), F_INT},
	{"physicsBounce", FOFS(physicsBounce), F_FLOAT},
	{"vehicle", FOFS(vehicle), F_INT},
	{"sb_material", FOFS(sb_material), F_INT},
	{"sb_gravity", FOFS(sb_gravity), F_FLOAT},
	{"sb_phys", FOFS(sb_phys), F_INT},
	{"sb_coll", FOFS(sb_coll), F_INT},
	{"sb_red", FOFS(sb_red), F_INT},
	{"sb_green", FOFS(sb_green), F_INT},
	{"sb_blue", FOFS(sb_blue), F_INT},
	{"sb_radius", FOFS(sb_radius), F_INT},
	{"sb_isnpc", FOFS(sb_isnpc), F_INT},
	{"objectType", FOFS(objectType), F_INT},

	{"phys_relativeOrigin", FOFS(phys_relativeOrigin), F_VECTOR},
	{"phys_rv_0", FOFS(phys_rv_0), F_VECTOR},
	{"phys_rv_1", FOFS(phys_rv_1), F_VECTOR},
	{"phys_rv_2", FOFS(phys_rv_2), F_VECTOR},

	{"sb_phys_welded", FOFS(sb_phys_welded), F_INT},
	{"sb_phys_parent", FOFS(sb_phys_parent), F_INT},
	
	{"distance", FOFS(distance), F_FLOAT},
	{"type", FOFS(type), F_INT},
	
	{NULL}
};

spawn_t	spawns_table[] = {
	{"info_player_start", SP_info_player_start},
	{"info_player_deathmatch", SP_info_player_deathmatch},
	{"info_player_intermission", SP_EmptySpawn},
	{"info_null", SP_DeleteSpawn},
	{"info_notnull", SP_EmptySpawn},
	{"info_camp", SP_EmptySpawn},

	{"func_plat", SP_func_plat},
	{"func_button", SP_func_button},
	{"func_door", SP_func_door},
	{"func_static", SP_func_static},
	{"func_rotating", SP_func_rotating},
	{"func_bobbing", SP_func_bobbing},
	{"func_pendulum", SP_func_pendulum},
	{"func_train", SP_func_train},
	{"func_group", SP_DeleteSpawn},

	{"trigger_always", SP_trigger_always},
	{"trigger_multiple", SP_trigger_multiple},
	{"trigger_push", SP_trigger_push},
	{"trigger_teleport", SP_trigger_teleport},
	{"trigger_hurt", SP_trigger_hurt},

	{"target_give", SP_target_give},
	{"target_delay", SP_target_delay},
	{"target_speaker", SP_target_speaker},
	{"target_print", SP_target_print},
	{"target_teleporter", SP_target_teleporter},
	{"target_relay", SP_target_relay},
	{"target_kill", SP_target_kill},
	{"target_position", SP_target_position},
	{"target_location", SP_target_location},
	{"target_push", SP_target_push},

	{"light", SP_DeleteSpawn},
	{"path_corner", SP_path_corner},

	{"misc_teleporter_dest", SP_EmptySpawn},
	{"misc_model", SP_DeleteSpawn},
	{"misc_portal_surface", SP_misc_portal_surface},
	{"misc_portal_camera", SP_misc_portal_camera},

	{"shooter_rocket", SP_shooter_rocket},
	{"shooter_grenade", SP_shooter_grenade},
	{"shooter_plasma", SP_shooter_plasma},

	{"team_CTF_redplayer", SP_team_CTF_redplayer},
	{"team_CTF_blueplayer", SP_team_CTF_blueplayer},
	{"team_CTF_redspawn", SP_team_CTF_redspawn},
	{"team_CTF_bluespawn", SP_team_CTF_bluespawn},
	{"team_redobelisk", SP_team_redobelisk},
	{"team_blueobelisk", SP_team_blueobelisk},
	{"team_neutralobelisk", SP_team_neutralobelisk},

	{"script_variable", SP_script_variable},
	{"script_cmd", SP_script_cmd},

	{"sandbox_prop", SP_sandbox_prop},
	{"sandbox_npc", SP_sandbox_npc},

	{"item_botroam", SP_EmptySpawn},

	{NULL, 0}
};

/*
===============
G_CallSpawn

Finds the spawn function for the entity and calls it,
returning qfalse if not found
===============
*/
qboolean G_CallSpawn( gentity_t *ent ) {
	spawn_t	*s;
	gitem_t	*item;
    char 	itemname[128];
	
	if( strcmp(ent->classname, "none") == 0 )
		return qfalse;

    Com_sprintf(itemname, sizeof(itemname), "%s", ent->classname);

	if( g_gametype.integer == GT_OBELISK ) {
		if( strcmp(itemname, "team_CTF_redflag") == 0 ) {
			Com_sprintf(itemname, sizeof(itemname), "%s", "team_redobelisk");
		}
		if( strcmp(itemname, "team_CTF_blueflag") == 0 ) {
			Com_sprintf(itemname, sizeof(itemname), "%s", "team_blueobelisk");
		}
	}
	if( g_gametype.integer == GT_HARVESTER ) {
		if( strcmp(itemname, "team_CTF_redflag") == 0 ) {
			Com_sprintf(itemname, sizeof(itemname), "%s", "team_redobelisk");
		}
		if( strcmp(itemname, "team_CTF_blueflag") == 0 ) {
			Com_sprintf(itemname, sizeof(itemname), "%s", "team_blueobelisk");
		}
		if( strcmp(itemname, "team_CTF_neutralflag") == 0 ) {
			Com_sprintf(itemname, sizeof(itemname), "%s", "team_neutralobelisk");
		}
	}
	if( g_gametype.integer == GT_FFA || g_gametype.integer == GT_TEAM ) {
		if( strcmp(itemname, "team_CTF_redplayer") == 0 ) {
			Com_sprintf(itemname, sizeof(itemname), "%s", "info_player_deathmatch");
		}
		if( strcmp(itemname, "team_CTF_blueplayer") == 0 ) {
			Com_sprintf(itemname, sizeof(itemname), "%s", "info_player_deathmatch");
		}
		if( strcmp(itemname, "team_CTF_redspawn") == 0 ) {
			Com_sprintf(itemname, sizeof(itemname), "%s", "info_player_deathmatch");
		}
		if( strcmp(itemname, "team_CTF_bluespawn") == 0 ) {
			Com_sprintf(itemname, sizeof(itemname), "%s", "info_player_deathmatch");
		}
	}

	if ( itemname[0]==0) {
        G_Printf ("G_CallSpawn: NULL classname\n");
		return qfalse;
	}

	// check item spawn functions
	for ( item=bg_itemlist+1 ; item->classname ; item++ ) {
		if ( !strcmp(item->classname, itemname) ) {
			G_SpawnItem( ent, item );
			return qtrue;
		}
	}

	// check normal spawn functions
	for ( s=spawns_table ; s->name ; s++ ) {
		if ( !strcmp(s->name, itemname) ) {
			// found it
			s->spawn(ent);
			return qtrue;
		}
	}

    G_Printf ("%s doesn't have a spawn function\n", itemname);
	return qfalse;
}

/*
=============
G_NewString

Builds a copy of the string, translating \n to real linefeeds
so message texts can be multi-line
=============
*/
char *G_NewString( const char *string ) {
	char	*newb, *new_p;
	int		i,l;
	
	l = strlen(string) + 1;
	newb = BG_Alloc( l );
	new_p = newb;

	// turn \n into a real linefeed
	for ( i=0 ; i< l ; i++ ) {
		if (string[i] == '\\' && i < l-1) {
			i++;
			if (string[i] == 'n') {
				*new_p++ = '\n';
			} else {
				*new_p++ = '\\';
			}
		} else {
			*new_p++ = string[i];
		}
	}
	
	return newb;
}

/*
===============
G_ParseField

Takes a key/value pair and sets the binary values
in a gentity
===============
*/
void G_ParseField( const char *key, const char *value, gentity_t *ent ) {
	field_t	*f;
	byte	*b;
	float	v;
	vec3_t	vec;

	for ( f=fields ; f->name ; f++ ) {
		if ( !Q_stricmp(f->name, key) ) {
			// found it
			b = (byte *)ent;

			switch( f->type ) {
			case F_STRING:
				*(char **)(b+f->ofs) = G_NewString (value);
				break;
			case F_VECTOR:
				sscanf (value, "%f %f %f", &vec[0], &vec[1], &vec[2]);
				((float *)(b+f->ofs))[0] = vec[0];
				((float *)(b+f->ofs))[1] = vec[1];
				((float *)(b+f->ofs))[2] = vec[2];
				break;
			case F_INT:
				*(int *)(b+f->ofs) = atoi(value);
				break;
			case F_FLOAT:
				*(float *)(b+f->ofs) = atof(value);
				break;
			case F_ANGLEHACK:
				v = atof(value);
				((float *)(b+f->ofs))[0] = 0;
				((float *)(b+f->ofs))[1] = v;
				((float *)(b+f->ofs))[2] = 0;
				break;
			default:
			case F_IGNORE:
				break;
			}
			return;
		}
	}
}

/*
===================
G_SpawnGEntityFromSpawnVars

Spawn an entity and fill in all of the level fields from
level.spawnVars[], then call the class specfic spawn function
===================
*/
void G_SpawnGEntityFromSpawnVars( void ) {
	int			i;
	gentity_t	*ent;
	char		*s, *value, *gametypeName;
	static char *gametypeNames[] = {"sandbox", "mapeditor", "ffa", "team", "ctf", "oneflag", "obelisk", "harvester"};

	// get the next free entity
	ent = G_Spawn();

	for ( i = 0 ; i < level.numSpawnVars ; i++ ) {
		G_ParseField( level.spawnVars[i][0], level.spawnVars[i][1], ent );
	}

	if ( g_gametype.integer >= GT_TEAM ) {
		G_SpawnInt( "notteam", "0", &i );
		if ( i && g_gametype.integer != GT_MAPEDITOR ) {
			G_FreeEntity( ent );
			return;
		}
	} else {
		G_SpawnInt( "notfree", "0", &i );
		if ( i && g_gametype.integer != GT_MAPEDITOR ) {
			G_FreeEntity( ent );
			return;
		}
	}

	G_SpawnInt( "notta", "0", &i );
	if ( i && g_gametype.integer != GT_MAPEDITOR ) {
		G_FreeEntity( ent );
		return;
	}

    if( G_SpawnString( "!gametype", NULL, &value ) ) {
		if( g_gametype.integer >= GT_SANDBOX && g_gametype.integer < GT_MAX_GAME_TYPE ) {
			gametypeName = gametypeNames[g_gametype.integer];

			s = strstr( value, gametypeName );
			if( s && g_gametype.integer != GT_MAPEDITOR ) {
				G_FreeEntity( ent );
				return;
			}
		}
	}

	if( G_SpawnString( "gametype", NULL, &value ) ) {
		if( g_gametype.integer >= GT_SANDBOX && g_gametype.integer < GT_MAX_GAME_TYPE ) {
			gametypeName = gametypeNames[g_gametype.integer];

			s = strstr( value, gametypeName );
			if( !s && g_gametype.integer != GT_MAPEDITOR ) {
				G_FreeEntity( ent );
				return;
			}
		}
	}

	// move editor origin to pos
	VectorCopy( ent->s.origin, ent->s.pos.trBase );
	VectorCopy( ent->s.origin, ent->r.currentOrigin );

	// if we didn't get a classname, don't bother spawning anything
	if ( !G_CallSpawn( ent ) )
		G_FreeEntity( ent );
}

/*
====================
G_AddSpawnVarToken
====================
*/
char *G_AddSpawnVarToken( const char *string ) {
	int		l;
	char	*dest;

	l = strlen( string );
	if ( level.numSpawnVarChars + l + 1 > MAX_SPAWN_VARS_CHARS ) {
		G_Error( "G_AddSpawnVarToken: MAX_SPAWN_VARS" );
	}

	dest = level.spawnVarChars + level.numSpawnVarChars;
	memcpy( dest, string, l+1 );

	level.numSpawnVarChars += l + 1;

	return dest;
}

/*
====================
G_ParseSpawnVars

Parses a brace bounded set of key / value pairs out of the
level's entity strings into level.spawnVars[]

This does not actually spawn an entity.
====================
*/
qboolean G_ParseSpawnVars( void ) {
	char		keyname[MAX_TOKEN_CHARS];
	char		com_token[MAX_TOKEN_CHARS];

	level.numSpawnVars = 0;
	level.numSpawnVarChars = 0;

	// parse the opening brace
	if ( !trap_GetEntityToken( com_token, sizeof( com_token ) ) ) {
		// end of spawn string
		return qfalse;
	}
	if ( com_token[0] != '{' ) {
		G_Error( "G_ParseSpawnVars: found %s when expecting {",com_token );
	}

	// go through all the key / value pairs
	while ( 1 ) {	
		// parse key
		if ( !trap_GetEntityToken( keyname, sizeof( keyname ) ) ) {
			G_Error( "G_ParseSpawnVars: EOF without closing brace" );
		}

		if ( keyname[0] == '}' ) {
			break;
		}
		
		// parse value	
		if ( !trap_GetEntityToken( com_token, sizeof( com_token ) ) ) {
			G_Error( "G_ParseSpawnVars: EOF without closing brace" );
		}

		if ( com_token[0] == '}' ) {
			G_Error( "G_ParseSpawnVars: closing brace without data" );
		}
		if ( level.numSpawnVars == MAX_SPAWN_VARS ) {
			G_Error( "G_ParseSpawnVars: MAX_SPAWN_VARS" );
		}
		level.spawnVars[ level.numSpawnVars ][0] = G_AddSpawnVarToken( keyname );
		level.spawnVars[ level.numSpawnVars ][1] = G_AddSpawnVarToken( com_token );
		level.numSpawnVars++;
	}

	return qtrue;
}

void SP_worldspawn( void ) {
	char	*s;

	G_SpawnString( "classname", "", &s );
	if ( Q_stricmp( s, "worldspawn" ) ) {
		G_Error( "SP_worldspawn: The first entity isn't 'worldspawn'" );
	}

	// make some data visible to connecting client
	trap_SetConfigstring( CS_GAME_VERSION, GAME_VERSION );
	trap_SetConfigstring( CS_LEVEL_START_TIME, va("%i", level.startTime ) );
	
	G_SpawnString( "music", "", &s );
	trap_SetConfigstring( CS_MUSIC, s );

	G_SpawnString( "gravity", "800", &s );
	trap_Cvar_Set( "g_gravity", s );

	G_SpawnString( "enableDust", "0", &s );
	trap_Cvar_Set( "g_enableDust", s );
    
    G_SpawnString( "enableSnow", "0", &s );
    trap_Cvar_Set( "g_enableSnow", s );
    
	G_SpawnString( "enableBreath", "0", &s );
	trap_Cvar_Set( "g_enableBreath", s );

	g_entities[ENTITYNUM_WORLD].s.number = ENTITYNUM_WORLD;
    g_entities[ENTITYNUM_WORLD].r.ownerNum = ENTITYNUM_NONE;
	g_entities[ENTITYNUM_WORLD].classname = "worldspawn";

    g_entities[ENTITYNUM_NONE].s.number = ENTITYNUM_NONE;
    g_entities[ENTITYNUM_NONE].r.ownerNum = ENTITYNUM_NONE;
    g_entities[ENTITYNUM_NONE].classname = "nothing";
}

/*
==============
G_SpawnEntitiesFromString

Parses textual entity definitions out of an entstring and spawns gentities.
==============
*/
void G_SpawnEntitiesFromString( void ) {
	// allow calls to G_Spawn*()
	level.spawning = qtrue;
	level.numSpawnVars = 0;

	// the worldspawn is not an actual entity, but it still
	// has a "spawn" function to perform any global setup
	// needed by a level (setting configstrings or cvars, etc)
	if ( !G_ParseSpawnVars() ) {
		G_Error( "SpawnEntities: no entities" );
	}
	SP_worldspawn();

	// parse ents
	while( G_ParseSpawnVars() ) {
		G_SpawnGEntityFromSpawnVars();
	}

	level.spawning = qfalse;			// any future calls to G_Spawn*() will be errors
}

