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
// g_misc.c

char* 		sandbox_class_allowed[] = {		//classes allowed in Sandbox
	"",
	"none",
	"weapon_machinegun",
	"weapon_shotgun",
	"weapon_grenadelauncher",
	"weapon_rocketlauncher",
	"weapon_lightning",
	"weapon_railgun",
	"weapon_plasmagun",
	"weapon_bfg",
	"weapon_grapplinghook",
	"weapon_nailgun",
	"weapon_prox_launcher",
	"weapon_chaingun",
	"weapon_flamethrower",
	"weapon_antimatter",
	"weapon_physgun",
	"weapon_gravitygun",
	"weapon_toolgun",
	"weapon_thrower",
	"weapon_bouncer",
	"weapon_thunder",
	"weapon_exploder",
	"weapon_knocker",
	"weapon_propgun",
	"weapon_regenerator",
	"weapon_nuke",
	"ammo_bullets",
	"ammo_shells",
	"ammo_grenades",
	"ammo_cells",
	"ammo_lightning",
	"ammo_rockets",
	"ammo_slugs",
	"ammo_bfg",
	"ammo_nails",
	"ammo_mines",
	"ammo_belt",
	"ammo_flame",
	"item_armor_shard",
	"item_armor_vest",
	"item_armor_combat",
	"item_armor_body",
	"item_armor_full",
	"item_health_small",
	"item_health",
	"item_health_large",
	"item_health_mega",
	"item_quad",
	"item_enviro",
	"item_haste",
	"item_invis",
	"item_regen",
	"item_flight",
	"item_scout",
	"item_doubler",
	"item_ammoregen",
	"item_guard",
	"holdable_teleporter",
	"holdable_medkit",
	"holdable_kamikaze",
	"holdable_invulnerability",
	"holdable_portal",
	"holdable_key_blue",
	"holdable_key_gold",
	"holdable_key_green",
	"holdable_key_iron",
	"holdable_key_master",
	"holdable_key_red",
	"holdable_key_silver",
	"holdable_key_yellow",
	"team_CTF_redflag",
	"team_CTF_blueflag",
	"team_CTF_neutralflag",
	0
};

char* 		editor_class_allowed[] = {		//classes allowed in Map Editor
	"info_player_deathmatch",
	"info_player_dd",
	"info_player_dd_red",
	"info_player_dd_blue",
	"team_CTF_redplayer",
	"team_CTF_blueplayer",
	"team_CTF_redspawn",
	"team_CTF_bluespawn",
	"misc_teleporter_dest",
	0
};

char* 		standard_class_spawn[] = {		//classes spawned without sandbox settings
	"info_player_deathmatch",
	"info_player_dd",
	"info_player_dd_red",
	"info_player_dd_blue",
	"team_CTF_redplayer",
	"team_CTF_blueplayer",
	"team_CTF_redspawn",
	"team_CTF_bluespawn",
	"misc_teleporter_dest",
	0
};

#include "g_local.h"

/*QUAKED info_camp (0 0.5 0) (-4 -4 -4) (4 4 4)
Used as a positional target for calculations in the utilities (spotlights, etc), but removed during gameplay.
*/
void SP_info_camp( gentity_t *self ) {
	G_SetOrigin( self, self->s.origin );
}

/*QUAKED info_waypoint (0 0.5 0) (-4 -4 -4) (4 4 4)
Used as a waypoint target for bot patrolling
*/
void SP_info_waypoint( gentity_t *self ) {
}

/*QUAKED info_backpack (0 0.5 0) ?
Used as a teleportation target for backpacks that fall into a nodrop brush
*/
void SP_info_backpack( gentity_t *self ) {
}

/*QUAKED info_null (0 0.5 0) (-4 -4 -4) (4 4 4)
Used as a positional target for calculations in the utilities (spotlights, etc), but removed during gameplay.
*/
void SP_info_null( gentity_t *self ) {
	G_FreeEntity( self );
}


/*QUAKED info_notnull (0 0.5 0) (-4 -4 -4) (4 4 4)
Used as a positional target for in-game calculation, like jumppad targets.
target_position does the same thing
*/
void SP_info_notnull( gentity_t *self ){
	G_SetOrigin( self, self->s.origin );
}


/*QUAKED light (0 1 0) (-8 -8 -8) (8 8 8) linear
Non-displayed light.
"light" overrides the default 300 intensity.
Linear checbox gives linear falloff instead of inverse square
Lights pointed at a target will be spotlights.
"radius" overrides the default 64 unit radius of a spotlight at the target point.
*/
void SP_light( gentity_t *self ) {
	G_FreeEntity( self );
}

/*QUAKED info_camera (0 0.5 0) (-4 -4 -4) (4 4 4)
Used as a positional and viewangles target for in-game cutscenes.
*/
void originToVariableInfo( char variableInfo[MAX_INFO_STRING], vec3_t origin ) {
	Info_SetValueForKey(variableInfo, "o10", va("%f", origin[0]));
	Info_SetValueForKey(variableInfo, "o11", va("%f", origin[1]));
	Info_SetValueForKey(variableInfo, "o12", va("%f", origin[2]));
}

void anglesToVariableInfo(char variableInfo[MAX_INFO_STRING], vec3_t angles) {
	Info_SetValueForKey(variableInfo, "a10", va("%f", angles[0]));
	Info_SetValueForKey(variableInfo, "a11", va("%f", angles[1]));
	Info_SetValueForKey(variableInfo, "a12", va("%f", angles[2]));
}

void Use_Camera (gentity_t *self, gentity_t *other, gentity_t *activator) {
	char variableInfo[MAX_INFO_STRING];

	//make camera movement info available for client 
	variableInfo[0] = '\0';
	Info_SetValueForKey(variableInfo, "w", va("%f", self->wait));
	Info_SetValueForKey(variableInfo, "t", va("%i", level.time));

	//add origin, viewangles and fov of source camera
	if (self->armor > 0) {	//armor is abused here to indicate whether or not camera should use player's origin/viewangles or its own
		//use origin and viewangles of player
		originToVariableInfo(variableInfo, level.clients[0].ps.origin);
		anglesToVariableInfo(variableInfo, level.clients[0].ps.viewangles);
	}
	else {
		//use origin and angles of this camera
		originToVariableInfo(variableInfo, self->s.origin);
		anglesToVariableInfo(variableInfo, self->s.angles);
	}
	Info_SetValueForKey(variableInfo, "f1", va("%i", self->count));
	
	if ( self->nextTrain && (self->spawnflags & 1) ) {
		//add origin and viewangles of target camera
		Info_SetValueForKey(variableInfo, "m", va("%i", self->spawnflags));
		Info_SetValueForKey(variableInfo, "o20", va("%f", self->nextTrain->s.origin[0]));
		Info_SetValueForKey(variableInfo, "o21", va("%f", self->nextTrain->s.origin[1]));
		Info_SetValueForKey(variableInfo, "o22", va("%f", self->nextTrain->s.origin[2]));
		Info_SetValueForKey(variableInfo, "a20", va("%f", self->nextTrain->s.angles[0]));
		Info_SetValueForKey(variableInfo, "a21", va("%f", self->nextTrain->s.angles[1]));
		Info_SetValueForKey(variableInfo, "a22", va("%f", self->nextTrain->s.angles[2]));
		Info_SetValueForKey(variableInfo, "f2", va("%i", self->nextTrain->count));
	} else {
		Info_SetValueForKey(variableInfo, "m", "0");	//0 means no camera motion
	}
	//G_Printf("%s\n", variableInfo);
	trap_SetConfigstring( CS_CUTSCENE, variableInfo );

	/*if(activator->client->ps.pm_type != PM_CUTSCENE){
	VectorCopy(activator->client->ps.origin, activator->orgOrigin);
	}*/
	activator->client->ps.pm_type = PM_CUTSCENE;
	self->activator = activator;
	self->nextthink = level.time + (self->wait * 1000);
}

void Think_Camera (gentity_t *self) {
	int i;

	if ( self->nextTrain ) {
		//jump to next camera
		self->nextTrain->use( self->nextTrain, self->activator, self->activator );
	} else {
		//cutscene should end so give player normal control
		self->activator->client->ps.pm_type = PM_NORMAL;

		//move player back to original location
		VectorCopy(self->activator->orgOrigin, self->activator->client->ps.origin);
	
		//give movement control back to bots
		if ( self->parent->spawnflags & 1 ) {
			for ( i = 0 ; i < level.maxclients ; i++ ) {
				if ( level.clients[i].pers.connected != CON_DISCONNECTED && level.clients[i].ps.pm_type != PM_DEAD )
					level.clients[i].ps.pm_type = PM_NORMAL;
			}
		}

		//link the player back into the world
		trap_LinkEntity( self->activator );
		self->activator->r.contents += CONTENTS_BODY;

		//enable synchronousClients if it was enabled before the cutscene kicked in. The skill field is abused to
		//temporarily store the pre-cutscene value of g_synchronousclients
		if (g_allowSyncCutscene.integer == 0)
			trap_Cvar_Set("g_synchronousClients", va("%1.0f", self->activator->skill ));
	}
}

void SP_info_camera( gentity_t *self ) {	
	G_SpawnFloat( "wait", "1", &self->wait );
	G_SpawnInt( "fov", "90", &self->count );	//abusing self->count here to store the FOV for the camera

	self->use = Use_Camera;
	self->think = Think_Camera;
	G_SetOrigin( self, self->s.origin );
}



/*
=================================================================================

TELEPORTERS

=================================================================================
*/

/*=========
Sets player's location without spitting out the player
===========*/
void TeleportPlayerNoKnockback( gentity_t *player, vec3_t origin, vec3_t angles, int angle ) {
	gentity_t	*tent;
	vec3_t orgAngles;
	vec3_t changedvel;

	// unlink to make sure it can't possibly interfere with G_KillBox
	//trap_UnlinkEntity (player);

	VectorCopy ( origin, player->client->ps.origin );
	//player->client->ps.origin[2] += 1;

	// spit the player out
	//AngleVectors( angles, player->client->ps.velocity, NULL, NULL );
	//VectorScale( player->client->ps.velocity, 400, player->client->ps.velocity );
	//player->client->ps.pm_time = 160;		// hold time
	//player->client->ps.pm_flags |= PMF_TIME_KNOCKBACK;

	// toggle the teleport bit so the client knows to not lerp
	player->client->ps.eFlags ^= EF_TELEPORT_BIT;
	
	VectorCopy( player->client->ps.velocity, changedvel );
	
	if(angle == 90){
	changedvel[0] = player->client->ps.velocity[1];
	changedvel[1] = -player->client->ps.velocity[0];
	}
	if(angle == 180){
	changedvel[0] = -player->client->ps.velocity[0];
	changedvel[1] = -player->client->ps.velocity[1];
	}
	if(angle == 270){
	changedvel[0] = -player->client->ps.velocity[1];
	changedvel[1] = -player->client->ps.velocity[0];
	}
	
	VectorCopy( changedvel, player->client->ps.velocity );
	
	SetClientViewAngle( player, angles );

	player->s.angles[0] = atof(va("%.4f", player->s.angles[0]));
	player->s.angles[1] = atof(va("%.4f", player->s.angles[1]));
	player->s.angles[2] = atof(va("%.4f", player->s.angles[2]));

	// kill anything at the destination
	/*if ( player->client->sess.sessionTeam != TEAM_SPECTATOR ) {
		G_KillBox (player);
	}*/

	// save results of pmove
	BG_PlayerStateToEntityState( &player->client->ps, &player->s, qtrue );

	// use the precise origin for linking
	//VectorCopy( player->client->ps.origin, player->r.currentOrigin );

	/*if ( player->client->sess.sessionTeam != TEAM_SPECTATOR ) {
		trap_LinkEntity (player);
	}*/
}

void TeleportPlayerForLayer( gentity_t *player, float level, float curlevel) {
	float changed2;

	changed2 = curlevel - level;

	player->client->ps.origin[2] += changed2;
	player->client->ps.eFlags ^= EF_TELEPORT_BIT;

	// save results of pmove
	BG_PlayerStateToEntityState( &player->client->ps, &player->s, qtrue );
}

void TeleportPlayer( gentity_t *player, vec3_t origin, vec3_t angles ) {
	gentity_t	*tent;
	qboolean noAngles;

	noAngles = (angles[0] > 999999.0);
	// use temp events at source and destination to prevent the effect
	// from getting dropped by a second player event
	if ( player->client->sess.sessionTeam != TEAM_SPECTATOR && player->client->ps.pm_type != PM_SPECTATOR) {
		tent = G_TempEntity( player->client->ps.origin, EV_PLAYER_TELEPORT_OUT );
		tent->s.clientNum = player->s.clientNum;

		tent = G_TempEntity( origin, EV_PLAYER_TELEPORT_IN );
		tent->s.clientNum = player->s.clientNum;
	}

	// unlink to make sure it can't possibly interfere with G_KillBox
	trap_UnlinkEntity (player);

	VectorCopy ( origin, player->client->ps.origin );
	player->client->ps.origin[2] += 1;

	if (!noAngles) {
		// spit the player out
		AngleVectors( angles, player->client->ps.velocity, NULL, NULL );
		VectorScale( player->client->ps.velocity, 400, player->client->ps.velocity );
		player->client->ps.pm_time = 160;		// hold time
		player->client->ps.pm_flags |= PMF_TIME_KNOCKBACK;

		// set angles
		SetClientViewAngle(player, angles);
	}

	// toggle the teleport bit so the client knows to not lerp
	player->client->ps.eFlags ^= EF_TELEPORT_BIT;

//unlagged - backward reconciliation #3
	// we don't want players being backward-reconciled back through teleporters
	G_ResetHistory( player );
//unlagged - backward reconciliation #3

	// kill anything at the destination
	if ( player->client->sess.sessionTeam != TEAM_SPECTATOR && player->client->ps.pm_type != PM_SPECTATOR ) {
		G_KillBox (player);
	}

	// save results of pmove
	BG_PlayerStateToEntityState( &player->client->ps, &player->s, qtrue );

	// use the precise origin for linking
	VectorCopy( player->client->ps.origin, player->r.currentOrigin );

	if ( player->client->sess.sessionTeam != TEAM_SPECTATOR && player->client->ps.pm_type != PM_SPECTATOR ) {
		trap_LinkEntity (player);
	}
}


/*QUAKED misc_teleporter_dest (1 0 0) (-32 -32 -24) (32 32 -16)
Point teleporters at these.
Now that we don't have teleport destination pads, this is just
an info_notnull
*/
void SP_misc_teleporter_dest( gentity_t *ent ) {
	VectorCopy( ent->s.origin, ent->s.pos.trBase );
	VectorCopy( ent->s.origin, ent->r.currentOrigin );
	ent->s.eType = ET_GENERAL;
	ent->s.pos.trType = TR_STATIONARY;
	ent->r.contents = CONTENTS_TRIGGER;
	if(g_gametype.integer == GT_MAPEDITOR){
		VectorSet( ent->r.mins, -16, -16, -25);
		VectorSet( ent->r.maxs, 16, 16, 25 );
		ent->s.scales[0] = 0.64;
		ent->s.scales[1] = 0.64;
		ent->s.scales[2] = 1.00;
		ent->s.modelindex = G_ModelIndex( "props/cube" );
		ent->s.generic2 = 255;
		ent->s.generic3 = 900;	//mass for correct physics
		ent->sandboxObject = OBJ_EDITOR;
	}
	
	trap_LinkEntity( ent );
}

/*QUAKED misc_model (1 0 0) (-16 -16 -16) (16 16 16)
"model"		arbitrary .md3 file to display
*/
void SP_misc_model( gentity_t *ent ) {
	G_FreeEntity( ent );
}

void locateCamera( gentity_t *ent ) {
	vec3_t		dir;
	gentity_t	*target;
	gentity_t	*owner;

	owner = G_PickTarget( ent->target );
	if ( !owner ) {
                G_Printf( "Couldn't find target for misc_portal_surface\n" );
		G_FreeEntity( ent );
		return;
	}
	ent->r.ownerNum = owner->s.number;

	// frame holds the rotate speed
	if ( owner->spawnflags & 1 ) {
		ent->s.frame = 25;
	} else if ( owner->spawnflags & 2 ) {
		ent->s.frame = 75;
	}

	// swing camera ?
	if ( owner->spawnflags & 4 ) {
		// set to 0 for no rotation at all
		ent->s.powerups = 0;
	}
	else {
		ent->s.powerups = 1;
	}

	// clientNum holds the rotate offset
	ent->s.clientNum = owner->s.clientNum;

	VectorCopy( owner->s.origin, ent->s.origin2 );

	// see if the portal_camera has a target
	target = G_PickTarget( owner->target );
	if ( target ) {
		VectorSubtract( target->s.origin, owner->s.origin, dir );
		VectorNormalize( dir );
	} else {
		G_SetMovedir( owner->s.angles, dir );
	}

	ent->s.eventParm = DirToByte( dir );
}

/*QUAKED misc_portal_surface (0 0 1) (-8 -8 -8) (8 8 8)
The portal surface nearest this entity will show a view from the targeted misc_portal_camera, or a mirror view if untargeted.
This must be within 64 world units of the surface!
*/
void SP_misc_portal_surface(gentity_t *ent) {
	VectorClear( ent->r.mins );
	VectorClear( ent->r.maxs );
	trap_LinkEntity (ent);

	ent->r.svFlags = SVF_PORTAL;
	ent->s.eType = ET_PORTAL;

	if ( !ent->target ) {
		VectorCopy( ent->s.origin, ent->s.origin2 );
	} else {
		ent->think = locateCamera;
		ent->nextthink = level.time + 100;
	}
}

/*QUAKED misc_portal_camera (0 0 1) (-8 -8 -8) (8 8 8) slowrotate fastrotate noswing
The target for a misc_portal_director.  You can set either angles or target another entity to determine the direction of view.
"roll" an angle modifier to orient the camera around the target vector;
*/
void SP_misc_portal_camera(gentity_t *ent) {
	float	roll;

	VectorClear( ent->r.mins );
	VectorClear( ent->r.maxs );
	trap_LinkEntity (ent);

	G_SpawnFloat( "roll", "0", &roll );

	ent->s.clientNum = roll/360.0 * 256;
}

/*
======================================================================

  SHOOTERS

======================================================================
*/

void Use_Shooter( gentity_t *ent, gentity_t *other, gentity_t *activator ) {
	vec3_t		dir;
	float		deg;
	vec3_t		up, right;
	gentity_t	*bolt;

	if ( !ent->r.linked )
		return;

	// see if we have a target
	if ( ent->enemy ) {
		VectorSubtract( ent->enemy->r.currentOrigin, ent->s.origin, dir );
		VectorNormalize( dir );
	} else if ( ent->spawnflags & 1 ) {
		
		if ( !level.player || level.player->client->ps.pm_type == PM_CUTSCENE)
			return;

		VectorSubtract( level.player->r.currentOrigin, ent->s.origin, dir );
		VectorNormalize( dir );
	} else {
		VectorCopy( ent->movedir, dir );
	}

	// randomize a bit
	PerpendicularVector( up, dir );
	CrossProduct( up, dir, right );

	deg = crandom() * ent->random;
	VectorMA( dir, deg, up, dir );

	deg = crandom() * ent->random;
	VectorMA( dir, deg, right, dir );

	VectorNormalize( dir );

	switch ( ent->s.weapon ) {
	case WP_GRENADE_LAUNCHER:
		fire_grenade( ent, ent->s.origin, dir );
		break;
	case WP_ROCKET_LAUNCHER:
		fire_rocket( ent, ent->s.origin, dir );
		break;
	case WP_PLASMAGUN:
		fire_plasma( ent, ent->s.origin, dir );
		break;
	case WP_BFG:
		fire_bfg( ent, ent->s.origin, dir );
		break;
	case WP_NAILGUN:
		fire_custom( ent, ent->s.origin, dir );
		break;
	case WP_PROX_LAUNCHER:
		fire_prox( ent, ent->s.origin, dir );
		break;
	case WP_FLAMETHROWER:
		fire_flame( ent, ent->s.origin, dir );
		break;
	case WP_ANTIMATTER:
		fire_antimatter( ent, ent->s.origin, dir );
		break;
	}

	G_AddEvent( ent, EV_FIRE_WEAPON, 0 );
}


static void InitShooter_Finish( gentity_t *ent ) {
	ent->enemy = G_PickTarget( ent->target );
	ent->think = 0;
	ent->nextthink = 0;
}

void InitShooter( gentity_t *ent, int weapon ) {
	ent->use = Use_Shooter;
	ent->s.weapon = weapon;

	RegisterItem( BG_FindItemForWeapon( weapon ) );

	G_SetMovedir( ent->s.angles, ent->movedir );

	if ( !ent->random ) {
		ent->random = 1.0;
	}
	ent->random = sin( M_PI * ent->random / 180 );
	// target might be a moving object, so we can't set movedir for it
	if ( ent->target ) {
		ent->think = InitShooter_Finish;
		ent->nextthink = level.time + 500;
	}
	trap_LinkEntity( ent );
}

/*QUAKED shooter_rocket (1 0 0) (-16 -16 -16) (16 16 16)
Fires at either the target or the current direction.
"random" the number of degrees of deviance from the taget. (1.0 default)
*/
void SP_shooter_rocket( gentity_t *ent ) {
	InitShooter( ent, WP_ROCKET_LAUNCHER );
}

/*QUAKED shooter_plasma (1 0 0) (-16 -16 -16) (16 16 16)
Fires at either the target or the current direction.
"random" is the number of degrees of deviance from the taget. (1.0 default)
*/
void SP_shooter_plasma( gentity_t *ent ) {
	InitShooter( ent, WP_PLASMAGUN);
}

/*QUAKED shooter_grenade (1 0 0) (-16 -16 -16) (16 16 16)
Fires at either the target or the current direction.
"random" is the number of degrees of deviance from the taget. (1.0 default)
*/
void SP_shooter_grenade( gentity_t *ent ) {
	InitShooter( ent, WP_GRENADE_LAUNCHER);
}

/*QUAKED SP_shooter_bfg (1 0 0) (-16 -16 -16) (16 16 16)
Fires at either the target or the current direction.
"random" is the number of degrees of deviance from the taget. (1.0 default)
*/
void SP_shooter_bfg( gentity_t *ent ) {
	InitShooter( ent, WP_BFG);
}

/*QUAKED SP_shooter_prox (1 0 0) (-16 -16 -16) (16 16 16)
Fires at either the target or the current direction.
"random" is the number of degrees of deviance from the taget. (1.0 default)
*/
void SP_shooter_prox( gentity_t *ent ) {
	InitShooter( ent, WP_PROX_LAUNCHER);
}

/*QUAKED SP_shooter_flame (1 0 0) (-16 -16 -16) (16 16 16)
Fires at either the target or the current direction.
"random" is the number of degrees of deviance from the taget. (1.0 default)
*/
void SP_shooter_flame( gentity_t *ent ) {
	InitShooter( ent, WP_FLAMETHROWER);
}

/*QUAKED SP_shooter_antimatter (1 0 0) (-16 -16 -16) (16 16 16)
Fires at either the target or the current direction.
"random" is the number of degrees of deviance from the taget. (1.0 default)
*/
void SP_shooter_antimatter( gentity_t *ent ) {
	InitShooter( ent, WP_ANTIMATTER);
}

/*QUAKED SP_shooter_custom (1 0 0) (-16 -16 -16) (16 16 16)
Fires at either the target or the current direction.
"random" is the number of degrees of deviance from the taget. (1.0 default)
*/
void SP_shooter_custom( gentity_t *ent ) {
	InitShooter( ent, WP_NAILGUN);
}

static void PortalDie (gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod) {
	G_FreeEntity( self );
	//FIXME do something more interesting
}

void BlockDie (gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod) {
	if(self->vehicle || self->objectType == OT_TNT){ //VEHICLE-SYSTEM: vehicle's explode for all
	G_StartCarExplode(self);
	}
	if(self->objectType == OT_NUKE){
	G_StartNukeExplode(self);
	}
	G_FreeEntity( self );
}


void DropPortalDestination( gentity_t *player ) {
	gentity_t	*ent;
	vec3_t		snapped;

	// create the portal destination
	ent = G_Spawn();
	ent->s.modelindex = G_ModelIndex( "models/powerups/teleporter/tele_exit.md3" );

	VectorCopy( player->s.pos.trBase, snapped );
	SnapVector( snapped );
	G_SetOrigin( ent, snapped );
	VectorCopy( player->r.mins, ent->r.mins );
	VectorCopy( player->r.maxs, ent->r.maxs );

	ent->classname = "hi_portal destination";
	ent->s.pos.trType = TR_STATIONARY;

	ent->r.contents = CONTENTS_CORPSE;
	ent->takedamage = qtrue;
	ent->health = g_portalhealth.integer;
	ent->die = PortalDie;

	VectorCopy( player->s.apos.trBase, ent->s.angles );

	ent->think = G_FreeEntity;
	ent->nextthink = level.time + g_portaltimeout.integer * 1000;

	trap_LinkEntity( ent );

	player->client->portalID = ++level.portalSequence;
	ent->count = player->client->portalID;

	// give the item back so they can drop the source now
	player->client->ps.stats[STAT_HOLDABLE_ITEM] |= (1 << HI_PORTAL);
}


static void PortalTouch( gentity_t *self, gentity_t *other, trace_t *trace) {
	gentity_t	*destination;

	// see if we will even let other try to use it
	if( other->health <= 0 ) {
		return;
	}
	if( !other->client ) {
		return;
	}
	if ( other->client->ps.powerups[PW_NEUTRALFLAG] ) {		// only happens in One Flag CTF
		Drop_Item( other, BG_FindItemForPowerup( PW_NEUTRALFLAG ), 0 );
		other->client->ps.powerups[PW_NEUTRALFLAG] = 0;
	}
	else if ( other->client->ps.powerups[PW_REDFLAG] ) {		// only happens in standard CTF
		Drop_Item( other, BG_FindItemForPowerup( PW_REDFLAG ), 0 );
		other->client->ps.powerups[PW_REDFLAG] = 0;
	}
	else if ( other->client->ps.powerups[PW_BLUEFLAG] ) {	// only happens in standard CTF
		Drop_Item( other, BG_FindItemForPowerup( PW_BLUEFLAG ), 0 );
		other->client->ps.powerups[PW_BLUEFLAG] = 0;
	}

	// find the destination
	destination = NULL;
	while( (destination = G_Find(destination, FOFS(classname), "hi_portal destination")) != NULL ) {
		if( destination->count == self->count ) {
			break;
		}
	}

	// if there is not one, die!
	if( !destination ) {
		if( self->pos1[0] || self->pos1[1] || self->pos1[2] ) {
			TeleportPlayer( other, self->pos1, self->s.angles );
		}
		G_Damage( other, other, other, NULL, NULL, 100000, DAMAGE_NO_PROTECTION, MOD_TELEFRAG );
		return;
	}

	TeleportPlayer( other, destination->s.pos.trBase, destination->s.angles );
}

static void PortalEnable( gentity_t *self ) {
	self->touch = PortalTouch;
	self->think = G_FreeEntity;
	self->nextthink = level.time + g_portaltimeout.integer * 1000;
}

static void ShowSandObject( gentity_t *self ) {
	trap_LinkEntity( self );
}

void DropPortalSource( gentity_t *player ) {
	gentity_t	*ent;
	gentity_t	*destination;
	vec3_t		snapped;

	// create the portal source
	ent = G_Spawn();
	ent->s.modelindex = G_ModelIndex( "models/powerups/teleporter/tele_enter.md3" );

	VectorCopy( player->s.pos.trBase, snapped );
	SnapVector( snapped );
	G_SetOrigin( ent, snapped );
	VectorCopy( player->r.mins, ent->r.mins );
	VectorCopy( player->r.maxs, ent->r.maxs );

	ent->classname = "hi_portal source";
	ent->s.pos.trType = TR_STATIONARY;

	ent->r.contents = CONTENTS_CORPSE | CONTENTS_TRIGGER;
	ent->takedamage = qtrue;
	ent->health = g_portalhealth.integer;
	ent->die = PortalDie;

	trap_LinkEntity( ent );

	ent->count = player->client->portalID;
	player->client->portalID = 0;

//	ent->spawnflags = player->client->ps.persistant[PERS_TEAM];

	ent->nextthink = level.time + 1000;
	ent->think = PortalEnable;

	// find the destination
	destination = NULL;
	while( (destination = G_Find(destination, FOFS(classname), "hi_portal destination")) != NULL ) {
		if( destination->count == ent->count ) {
			VectorCopy( destination->s.pos.trBase, ent->pos1 );
			break;
		}
	}

}


char *G_ClearStringMisc( char *input ){
	if( input[0] == '"' ){
		input[0] = '\0';
		input++;
	}
	if( input[strlen(input)-1] == '"' ){
		input[strlen(input)-1] = '\0';
	}
	return input;
}

/*
================
G_TouchProp

================
*/
void G_TouchProp( gentity_t *self, gentity_t *other, trace_t *trace ) {
	if( other->health <= 0 ) {
		return;
	}
	if( !other->client ) {
		return;
	}
	if( !other->singlebot ) {
		return;
	}
	if( other->client->vehiclenum ) {
		return;
	}
	if(	self->vehicle <= 0) {
		return;
	}
	if( self->parent && self->parent->client->vehiclenum ) {
		return;
	}
	other->client->vehiclenum = self->s.number;
	self->parent = other;
	ClientUserinfoChanged( other->s.clientNum );
	VectorCopy(self->s.origin, other->s.origin);
	VectorCopy(self->s.pos.trBase, other->s.pos.trBase);
	other->s.apos.trBase[1] = self->s.apos.trBase[1];
	VectorCopy(self->r.currentOrigin, other->r.currentOrigin);
	VectorSet( other->r.mins, -25, -25, -15 );
	VectorSet( other->r.maxs, 25, 25, 15 );
	self->think = VehiclePhys;
	self->nextthink = level.time + 1;
}

void setModel(gentity_t *ent, char *modelName) {
	int len;
    if (!ent || !modelName) {
        Com_Printf("Invalid entity or model name\n");
        return;
    }

    len = strlen(modelName);
    if (len == 0) {
        Com_Printf("Model name is empty\n");
        return;
    }

    ent->s.modelindex = G_ModelIndex(modelName);
    CopyAlloc(ent->model, modelName);

    if (len >= 4 && !Q_stricmp(ent->model + len - 4, ".md3")) {
        Com_Printf("MD3 Model load: '%s'\n", ent->model);
        ent->model[len - 4] = '\0'; // Убираем расширение
        memset(ent->model + len - 3, 0, 4); // Удаляем остатки
    }

    if (len >= 4 && !Q_stricmp(ent->model + len - 4, ".mdr")) {
        Com_Printf("MDR Model load: '%s'\n", ent->model);
        ent->model[len - 4] = '\0'; // Убираем расширение
        memset(ent->model + len - 3, 0, 4); // Удаляем остатки
    }

    if (len >= 4 && !Q_stricmp(ent->model + len - 4, ".iqm")) {
        Com_Printf("IQM Model load: '%s'\n", ent->model);
        ent->model[len - 4] = '\0'; // Убираем расширение
        memset(ent->model + len - 3, 0, 4); // Удаляем остатки
    }

	if(FS_FileExists(va("%s.bsp", ent->model))){		//Проверка модели коллизии
		trap_SetBrushModel(ent, va("%s.bsp", ent->model));	//Загружаем если есть
		ent->s.modelindex2 = G_ModelIndex(modelName);	//Коллизия созраняется в первый слот а модель теперь в втором слоте
	}
}

/*QUAKED func_prop (0 .5 .8) ?
A bmodel that just sits there, doing nothing.  Can be used for conditional walls and models.
"model2"	.md3 model to also draw
"color"		constantLight color
"light"		constantLight radius
*/

void SP_func_prop( gentity_t *ent ) {
	spawn_t	*s;
	gitem_t	*item;
	qboolean spawn_entity = qfalse;
	
	// Create entity
	CopyAlloc(ent->classname, ent->sb_class);

	//Origin
	VectorCopy( ent->s.origin, ent->s.pos.trBase );
	VectorCopy( ent->s.origin, ent->r.currentOrigin );

	//Type
	ent->sandboxObject = OBJ_SANDBOX;

	// Entity spawn
	for ( s=spawns_table ; s->name ; s++ ) {
		if ( !strcmp(s->name, ent->classname) ) {
			CopyAlloc(ent->sb_class, ent->classname);
			s->spawn(ent);

			spawn_entity = qtrue;
		}
	}
	//Light
	ent->s.constantLight = ent->sb_red | ( ent->sb_green << 8 ) | ( ent->sb_blue << 16 ) | ( ent->sb_radius << 24 );

	//Sound
	ent->s.loopSound = G_SoundIndex(ent->sb_sound);

	//Setting collision
	if(ent->vehicle <= 0 || spawn_entity){
		VectorSet( ent->r.mins, -ent->sb_coltype*ent->s.scales[0], -ent->sb_coltype*ent->s.scales[1], -ent->sb_coltype*ent->s.scales[2]);
		VectorSet( ent->r.maxs, ent->sb_coltype*ent->s.scales[0], ent->sb_coltype*ent->s.scales[1], ent->sb_coltype*ent->s.scales[2] );
		} else {
		VectorSet( ent->r.mins, -25, -25, -15);
		VectorSet( ent->r.maxs, 25, 25, 15 );
	}

	//Phys frame
	ent->s.pos.trTime = level.time;

	//Damage
	ent->takedamage = qtrue;
	if(ent->sb_takedamage == 0){
		ent->takedamage = qfalse;
	}
	if(ent->sb_takedamage == 1){
		ent->takedamage = qtrue;
	}
	if(ent->sb_takedamage2 == 0){
		ent->takedamage2 = qfalse;
	}
	if(ent->sb_takedamage2 == 1){
		ent->takedamage2 = qtrue;
	}

	//Physics
	if(ent->sb_phys == 1){ ent->s.pos.trType = TR_STATIONARY; ent->physicsObject = qfalse; }
	if(ent->sb_phys == 2){ ent->s.pos.trType = TR_GRAVITY; ent->s.pos.trTime = level.time; ent->physicsObject = qtrue; }

	//Collision
	if(ent->sb_coll == 0){
		ent->r.contents = CONTENTS_SOLID;	
	}
	if(ent->sb_coll == 1){
		ent->r.contents = CONTENTS_TRIGGER;	
	}

	//Material
	ent->s.generic2 = ent->sb_material;

	//Mass
	ent->s.generic3 = ent->sb_gravity;

	//Type
	ent->s.torsoAnim = ent->objectType;

	//Prop class for saving
	ent->classname = "func_prop";

	if(!spawn_entity){
		//Type
		ent->s.eType = ET_GENERAL;

		//Die function
		ent->die = BlockDie;

		//Touch function
		ent->touch = G_TouchProp;
	}

	//Angles
	VectorCopy(ent->s.apos.trBase, ent->s.angles);
	VectorCopy(ent->s.apos.trBase, ent->r.currentAngles);
	VectorCopy( ent->s.angles, ent->s.apos.trBase );

	//Load model
	setModel(ent, ent->model);

	//Link
	trap_LinkEntity( ent );
}

void G_BuildPropSL( char *arg02, char *arg03, vec3_t xyz, gentity_t *player, char *arg04, char *arg05, char *arg06, char *arg07, char *arg08, char *arg09, char *arg10, char *arg11, char *arg12, char *arg13, char *arg14, char *arg15, char *arg16, char *arg17, char *arg18, char *arg19, char *arg20, char *arg21, char *arg22, char *arg23) {
	gentity_t	*ent;
	vec3_t		position;
	spawn_t		*s;
	gitem_t		*item;
	qboolean spawn_entity = qfalse;
	int			i;
	qboolean extended_spawn = qtrue;
	qboolean allow_spawn = qfalse;
	
	position[0] = ((int)((xyz[0] + (xyz[0] < 0 ? -atoi(arg06) : atoi(arg06))) / (atoi(arg06) * 2)) * (atoi(arg06) * 2));
	position[1] = ((int)((xyz[1] + (xyz[1] < 0 ? -atoi(arg06) : atoi(arg06))) / (atoi(arg06) * 2)) * (atoi(arg06) * 2));
	if(atoi(arg09) <= 0){
		position[2] = ((int)((xyz[2] + (xyz[2] < 0 ? -atoi(arg06) : atoi(arg06))) / (atoi(arg06) * 2)) * (atoi(arg06) * 2));
	} else {
		position[2] = xyz[2] + atoi(arg05);
	}
	
	// Create entity
	ent = G_Spawn();
	CopyAlloc(ent->classname, arg03);
	for ( i = 0; standard_class_spawn[i] != 0; i++ ) {		//Classlist for standard spawn
		if ( !strcmp(ent->classname, standard_class_spawn[i]) ) {
			extended_spawn = qfalse;
		}
	}
	for ( i = 0; sandbox_class_allowed[i] != 0; i++ ) {		//Check allowed sandbox list
		if ( !strcmp(ent->classname, sandbox_class_allowed[i]) ) {
			allow_spawn = qtrue;
		}
	}
	if(g_gametype.integer == GT_MAPEDITOR){
		for ( i = 0; editor_class_allowed[i] != 0; i++ ) {		//Check allowed editor list
			if ( !strcmp(ent->classname, editor_class_allowed[i]) ) {
				allow_spawn = qtrue;
			}
		}
	}

	if(!allow_spawn){
		G_FreeEntity(ent);
		trap_SendServerCommand( player->s.clientNum, "cp \"Spawning of this class is not allowed\n\"" );
		return;
	}

	//Origin
	VectorCopy( position, ent->s.origin );
	VectorCopy( position, ent->s.pos.trBase );
	VectorCopy( position, ent->r.currentOrigin );

	//Basic
	ent->sandboxObject = OBJ_SANDBOX;
	ent->spawnflags = atoi(arg07);

	if(extended_spawn){
		//Type
		ent->objectType = OT_BASIC;
		ent->s.torsoAnim = OT_BASIC;

		//Damage
		ent->sb_takedamage = 1;
		ent->sb_takedamage2 = 1;
		if(atoi(arg12) == -1){
			ent->sb_takedamage2 = 0;
		}
		ent->takedamage = ent->sb_takedamage;
		ent->takedamage2 = ent->sb_takedamage2;

		//Owner
		if(atoi(arg04) == 1){
		ent->owner = player->s.clientNum + 1;
		ent->ownername = player->client->pers.netname;
		}

		//Material
		ent->s.generic2 = atoi(arg08);
		ent->sb_material = atoi(arg08);

		//Physics
		if(atoi(arg09) == 0){
		ent->s.pos.trType = TR_STATIONARY; ent->physicsObject = qfalse; ent->sb_phys = 1;
		}
		if(atoi(arg09) == 1){
		ent->s.pos.trType = TR_GRAVITY; ent->s.pos.trTime = level.time; ent->physicsObject = qtrue; ent->physicsBounce = atof(arg22); ent->sb_phys = 2;
		}

		//Collision
		if(atoi(arg10) == 0){
		ent->r.contents = CONTENTS_SOLID;
		ent->sb_coll = 0;
		}
		if(atoi(arg10) == 1){
		ent->r.contents = CONTENTS_TRIGGER;
		ent->sb_coll = 1;
		}

		//Sound
		ent->s.loopSound = G_SoundIndex(arg11);
		CopyAlloc(ent->sb_sound, arg11);

		//HP
		ent->health = atoi(arg12);

		//Light
		ent->s.constantLight = atoi(arg13) | ( atoi(arg14) << 8 ) | ( atoi(arg15) << 16 ) | ( atoi(arg16) << 24 );
		ent->sb_red = atoi(arg13);
		ent->sb_green = atoi(arg14);
		ent->sb_blue = atoi(arg15);
		ent->sb_radius = atoi(arg16);

		//Scale
		ent->s.scales[0] = atof(arg17);
		ent->s.scales[1] = atof(arg18);
		ent->s.scales[2] = atof(arg19);

		//Type
		ent->objectType = atoi(arg20);
		ent->s.torsoAnim = atoi(arg20);
		ent->vehicle = atoi(arg21);

		//Mass
		ent->sb_gravity = atoi(arg23);
		ent->s.generic3 = atoi(arg23);
	}

	// Item spawn
	for ( item=bg_itemlist+1 ; item->classname ; item++ ) {
		if ( !strcmp(item->classname, ent->classname) ) {
			position[2] += 48;
			VectorCopy( position, ent->s.origin );
			VectorCopy( position, ent->s.pos.trBase );
			VectorCopy( position, ent->r.currentOrigin );
			G_SpawnItem( ent, item );
			return;
		}
	}

	// Entity spawn
	for ( s=spawns_table ; s->name ; s++ ) {
		if ( !strcmp(s->name, ent->classname) ) {
			CopyAlloc(ent->sb_class, ent->classname);
			s->spawn(ent);

			spawn_entity = qtrue;

			if(!extended_spawn){		//Standard spawn
				return;
			}
		}
	}

	//Prop class for saving
	ent->classname = "func_prop";

	if(!spawn_entity){
		//Type
		ent->s.eType = ET_GENERAL;

		//Die function
		ent->die = BlockDie;	

		//Touch function
		ent->touch = G_TouchProp;
	}

	//Setting collsion
	if(atoi(arg21) <= 0 || spawn_entity){
	ent->sb_coltype = atoi(arg05);
	VectorSet( ent->r.mins, -ent->sb_coltype*ent->s.scales[0], -ent->sb_coltype*ent->s.scales[1], -ent->sb_coltype*ent->s.scales[2]);
	VectorSet( ent->r.maxs, ent->sb_coltype*ent->s.scales[0], ent->sb_coltype*ent->s.scales[1], ent->sb_coltype*ent->s.scales[2] );
	} else {
	ent->sb_coltype = atoi(arg05);
	VectorSet( ent->r.mins, -25, -25, -15);
	VectorSet( ent->r.maxs, 25, 25, 15 );
	}

	//Load model
	setModel(ent, arg02);

	//Link
	trap_LinkEntity( ent );
}

void G_ModProp( gentity_t *targ, gentity_t *attacker, char *arg01, char *arg02, char *arg03, char *arg04, char *arg05, char *arg06, char *arg07, char *arg08, char *arg09, char *arg10, char *arg11, char *arg12, char *arg13, char *arg14, char *arg15, char *arg16, char *arg17, char *arg18, char *arg19 ) { //tool_id

	if(g_gametype.integer != GT_SANDBOX && g_gametype.integer != GT_MAPEDITOR){
		return; 
	}
	if(!g_allowtoolgun.integer){
		return; 
	}
	if(targ->client && !targ->singlebot){
		return;
	}
	if(!attacker->client){
		return;
	}
	if(targ->owner != attacker->s.clientNum + 1){
		if(targ->owner != 0){
			trap_SendServerCommand( attacker->s.clientNum, va( "cp \"Owned by %s\n\"", targ->ownername ));
			return;
		}	
	}

	if(attacker->tool_id == TL_CREATE){
		// client-side command for spawn prop
	}

	if(attacker->tool_id == TL_MATERIAL){
		targ->s.generic2 = atoi(arg01);
		targ->sb_material = atoi(arg01);
	}

	if(attacker->tool_id == TL_DELETE){
		if(!targ->singlebot){
			G_FreeEntity(targ);
		} else {
			DropClientSilently( targ->client->ps.clientNum );	
		}
	}

	if(attacker->tool_id == TL_MODEL){
		setModel(targ, arg01);
	}

	if(attacker->tool_id == TL_PHYSICS){
		if(atoi(arg19) == 0){
		targ->s.pos.trType = TR_STATIONARY; targ->physicsObject = qfalse; targ->sb_phys = 1;
		}
		if(atoi(arg19) == 1){
		targ->s.pos.trType = TR_GRAVITY; targ->s.pos.trTime = level.time; targ->physicsObject = qtrue; targ->sb_phys = 2;
		}
	}

	if(attacker->tool_id == TL_PRIVATE){
		if(atoi(arg19) == 0){
		targ->owner = 0;
		targ->ownername = 0;
		}
		if(atoi(arg19) == 1){
		targ->owner = attacker->s.clientNum + 1;
		targ->ownername = attacker->client->pers.netname;
		}
		if(atoi(arg19) == 2){
		if(targ->ownername){
		trap_SendServerCommand( attacker->s.clientNum, va( "cp \"Owned by %s\n\"", targ->ownername ));
		} 
		if(!targ->ownername){
		trap_SendServerCommand( attacker->s.clientNum, "cp \"Not owned\n\"" );
		} 
		}
	}

	if(attacker->tool_id == TL_COLLISION){
		if(atoi(arg19) == 0){
		targ->r.contents = CONTENTS_SOLID;
		targ->sb_coll = 0;
		}
		if(atoi(arg19) == 1){
		targ->r.contents = CONTENTS_TRIGGER;
		targ->sb_coll = 1;
		}
	}

	if(attacker->tool_id == TL_HEALTH){
		targ->health = atoi(arg01);
	}

	if(attacker->tool_id == TL_COLOR){
		targ->s.constantLight = atoi(arg01) | ( atoi(arg02) << 8 ) | ( atoi(arg03) << 16 ) | ( atoi(arg04) << 24 );
		targ->sb_red = atoi(arg01);
		targ->sb_green = atoi(arg02);
		targ->sb_blue = atoi(arg03);
		targ->sb_radius = atoi(arg04);
		trap_UnlinkEntity( targ );
		trap_LinkEntity( targ );
	}

	if(attacker->tool_id == TL_ANGLE){
	if(atoi(arg19) == 0){
		targ->s.apos.trBase[0] += atof(arg01);
	}
	if(atoi(arg19) == 1){
		targ->s.apos.trBase[1] += atof(arg01);
	}
	if(atoi(arg19) == 2){
		targ->s.apos.trBase[2] += atof(arg01);
	}
	}

	if(attacker->tool_id == TL_SCALE){
	if(atoi(arg19) == 0){
		targ->s.scales[0] = atof(arg01);
	}
	if(atoi(arg19) == 1){
		targ->s.scales[1] = atof(arg01);
	}
	if(atoi(arg19) == 2){
		targ->s.scales[2] = atof(arg01);
	}
		VectorSet( targ->r.mins, -targ->sb_coltype*targ->s.scales[0], -targ->sb_coltype*targ->s.scales[1], -targ->sb_coltype*targ->s.scales[2]);
		VectorSet( targ->r.maxs, targ->sb_coltype*targ->s.scales[0], targ->sb_coltype*targ->s.scales[1], targ->sb_coltype*targ->s.scales[2] );
	}

	if(attacker->tool_id == TL_REPLACEITEM){
		gitem_t	*item;
		int i = 1;
		if(targ->s.eType != ET_ITEM){
			trap_SendServerCommand( attacker->s.clientNum, "cp \"This must be the item\n\"" );
			return;
		}
		for ( item=bg_itemlist+1, i = 1; item->classname; item++, i++ ) {
			if ( !strcmp(item->classname, arg01) ) {
				targ->item = &bg_itemlist[i];
				targ->item->classname = bg_itemlist[i].classname;
				targ->classname = bg_itemlist[i].classname;
				targ->s.modelindex = i;
			}
		}
	}

	if(attacker->tool_id == TL_COUNT){
		targ->count = atoi(arg01);
	}

}

void G_PropSmoke( gentity_t *ent, float impact ){
	gentity_t   *temp;
	
	temp = G_TempEntity(ent->r.currentOrigin, EV_SMOKEPUFF);
	temp->s.constantLight = (((70 | 64 << 8 ) | 64 << 16 ) | ( 255 << 24 ));
	temp->s.eventParm = 2;	//eventParm is used to determine the amount of time the smoke puff exists
	temp->s.generic1 = 100;	//generic1 is used to determine the movement speed of the smoke puff
	temp->s.otherEntityNum = (int)impact; //otherEntityNum is used to determine the size of the smokepuff. The default is 32.
	VectorCopy(ent->s.angles, temp->s.angles);
}

void G_PropWaterSplash( gentity_t *ent, float impact ){
	gentity_t   *temp;
	
	temp = G_TempEntity(ent->r.currentOrigin, EV_WATERPUFF);
	temp->s.eventParm = 1;	//eventParm is used to determine the amount of time the smoke puff exists
	temp->s.generic1 = 100;	//generic1 is used to determine the movement speed of the smoke puff
	temp->s.otherEntityNum = (int)impact; //otherEntityNum is used to determine the size of the smokepuff. The default is 32.
}

/*
================
G_BounceProp

================
*/

void G_BounceProp( gentity_t *ent, trace_t *trace ) {
	vec3_t	velocity;
	float	dot;
	int		hitTime;
    float 	randomOffset[3];
	int		i;

	// reflect the velocity on the trace plane
	hitTime = level.previousTime + ( level.time - level.previousTime ) * trace->fraction;
	ST_EvaluateTrajectoryDelta( &ent->s.pos, hitTime, velocity, ent->s.generic3 );
	dot = DotProduct( velocity, trace->plane.normal );
	VectorMA( velocity, -2*dot, trace->plane.normal, ent->s.pos.trDelta );

	VectorScale( ent->s.pos.trDelta, ent->physicsBounce, ent->s.pos.trDelta );

    for (i = 0; i < 3; i++) {
        randomOffset[i] = ((float)rand() / 32767 - 0.5f) * 20.0f;
    }
    VectorAdd(ent->s.pos.trDelta, randomOffset, ent->s.pos.trDelta);
	// check for stop
	if ( trace->plane.normal[2] > 0.2 && VectorLength( ent->s.pos.trDelta ) < 40 && !ent->isGrabbed) {
        ent->s.apos.trBase[0] = 0;
        ent->s.apos.trBase[2] = 0;
        ent->r.currentAngles[0] = 0;
        ent->r.currentAngles[2] = 0;
        G_DisablePropPhysics(ent, trace->endpos);
		return;
	}

	VectorAdd( ent->r.currentOrigin, trace->plane.normal, ent->r.currentOrigin);
	VectorCopy( ent->r.currentOrigin, ent->s.pos.trBase );
	ent->s.pos.trTime = level.time;
}

/*
================
G_RunProp

================
*/
void G_RunProp(gentity_t *ent) {
    vec3_t origin;
    trace_t tr;
	gentity_t *hit;
	float impactForce;
	float impactForceAll;
	float impactForceFixed;
	vec3_t impactVector;
	
	if(ent->r.currentOrigin[2] <= -520000){
		G_FreeEntity(ent);
		return;
	}

    // Unlink the entity so that it won't interact with other entities during the calculation
    trap_UnlinkEntity(ent);
	
    // If ground entity has been set to -1, apply gravity if necessary
    if (ent->s.groundEntityNum == -1 && ent->s.pos.trType != TR_GRAVITY) {
        ent->s.pos.trType = TR_GRAVITY;
        ent->s.pos.trTime = level.time;
    }

    // If the entity is stationary, re-link it and run the think function
    if (ent->s.pos.trType == TR_STATIONARY) {
        trap_LinkEntity(ent);
        G_RunThink(ent);
        return;
    }
	
	// Get current position based on the entity's trajectory
	if(ent->s.eType == ET_GENERAL){ 		//is prop
    ST_EvaluateTrajectory(&ent->s.pos, level.time, origin, ent->s.generic3);
	} else {
    BG_EvaluateTrajectory(&ent->s.pos, level.time, origin);		
	}

    // Trace a line from the current origin to the new position
    trap_Trace(&tr, ent->r.currentOrigin, ent->r.mins, ent->r.maxs, origin, ent->s.number, MASK_PLAYERSOLID);

    // Save origin
    VectorCopy(tr.endpos, ent->s.origin);
	VectorCopy(tr.endpos, ent->r.currentOrigin);
	
    // Link the entity back into the world
    trap_LinkEntity(ent);
	
	// Calculate the impact force
	impactForce = sqrt(ent->s.pos.trDelta[0] * ent->s.pos.trDelta[0] + ent->s.pos.trDelta[1] * ent->s.pos.trDelta[1]);
	impactForceAll = sqrt(ent->s.pos.trDelta[0] * ent->s.pos.trDelta[0] + ent->s.pos.trDelta[1] * ent->s.pos.trDelta[1] + ent->s.pos.trDelta[2] * ent->s.pos.trDelta[2]);
	impactForceFixed = sqrt(ent->s.pos.trDelta[0] * ent->s.pos.trDelta[0] + ent->s.pos.trDelta[1] * ent->s.pos.trDelta[1] + g_gravity.integer*g_gravityModifier.value * g_gravity.integer*g_gravityModifier.value);
	
	impactForce *= (ent->s.generic3/800);
	impactForceAll *= (ent->s.generic3/800);
	impactForceFixed *= (ent->s.generic3/800);

	if(ent->s.pos.trType == TR_GRAVITY || ent->s.pos.trType == TR_GRAVITY_WATER){
		if(trap_PointContents(tr.endpos, ent->s.number) & MASK_WATER){
			if(ent->s.pos.trType != TR_GRAVITY_WATER){
				ent->s.pos.trType = TR_GRAVITY_WATER;
				G_PropWaterSplash( ent, impactForceFixed);
				G_EnablePropPhysics(ent);
				ent->s.pos.trDelta[0] *= 0.50;
				ent->s.pos.trDelta[1] *= 0.50;
				if(ent->s.pos.trDelta[2]){
				ent->s.pos.trDelta[2] = 0;
				}
			}
		} else {
			if(ent->s.pos.trType != TR_GRAVITY){
				ent->s.pos.trType = TR_GRAVITY;
				G_EnablePropPhysics(ent);
			}
		}
	}

    // If there's a collision, handle it
    if (tr.fraction < 1.0f && tr.entityNum != ENTITYNUM_NONE) {
        hit = &g_entities[tr.entityNum];

        if (hit->s.number != ent->s.number) {  // Ignore self
            // Optionally apply a force or velocity to the hit entity to simulate the push
			if (impactForce > PHYS_SENS) {
				if (!hit->client){
					G_EnablePropPhysics(hit);
				}
				VectorCopy(ent->s.pos.trDelta, impactVector);
				VectorScale(impactVector, PHYS_PROP_IMPACT, impactVector);	
				if (!hit->client){
            		VectorAdd(hit->s.pos.trDelta, impactVector, hit->s.pos.trDelta);  // Transfer velocity from the prop to the hit entity
				} else {
					if(hit->grabbedEntity != ent && hit->botskill != 9){
						VectorAdd(hit->client->ps.velocity, impactVector, hit->client->ps.velocity);  // Transfer velocity from the prop to the hit player
					}
				}
			}
			if(impactForceAll > PHYS_DAMAGESENS && !tr.startsolid){
				if(hit->grabbedEntity != ent){
				G_PropDamage(hit, ent->lastPlayer, (int)(impactForceAll * PHYS_DAMAGE));
				}
				G_PropDamage(ent, NULL, (int)(impactForceAll * PHYS_DAMAGE));

				if(hit->grabbedEntity != ent){
					if(ent->s.pos.trType != TR_GRAVITY_WATER){
						if(ent->objectType == OT_BASIC){
							G_AddEvent( ent, EV_OT1_IMPACT, 0 );
							G_PropSmoke( ent, impactForceAll*0.20);
						}
						if(ent->objectType == OT_VEHICLE){
							G_AddEvent( ent, EV_CRASH25, 0 );
							G_PropSmoke( ent, impactForceAll*0.20);
						}
						if(ent->objectType == OT_TNT){
							G_AddEvent( ent, EV_OT1_IMPACT, 0 );
							G_PropSmoke( ent, impactForceAll*0.20);
						}
					}
				}
			}
        }
    }

    // Rotate entity during movement (optional physics feature)
	if (!ent->isGrabbed && !tr.startsolid){
		if(ent->s.pos.trType != TR_GRAVITY_WATER){
    		if (ent->s.pos.trDelta[2] != 0) {
    		    ent->s.apos.trBase[0] -= ent->s.pos.trDelta[2] * PHYS_ROTATING * 0.20;
				ent->s.apos.trBase[1] -= ent->s.pos.trDelta[2] * PHYS_ROTATING * 0.20;
    		}
    		if (ent->s.pos.trDelta[1] != 0) {
    		    ent->s.apos.trBase[1] -= ent->s.pos.trDelta[1] * PHYS_ROTATING;
    		}
    		if (ent->s.pos.trDelta[0] != 0) {
    		    ent->s.apos.trBase[0] += ent->s.pos.trDelta[0] * PHYS_ROTATING;
    		}
		} else {
    		if (ent->s.pos.trDelta[2] != 0) {
    		    ent->s.apos.trBase[0] -= ent->s.pos.trDelta[2] * PHYS_ROTATING * 0.10;
				ent->s.apos.trBase[1] -= ent->s.pos.trDelta[2] * PHYS_ROTATING * 0.10;
    		}
    		if (ent->s.pos.trDelta[1] != 0) {
    		    ent->s.apos.trBase[1] -= ent->s.pos.trDelta[1] * PHYS_ROTATING * 0.50;
    		}
    		if (ent->s.pos.trDelta[0] != 0) {
    		    ent->s.apos.trBase[0] += ent->s.pos.trDelta[0] * PHYS_ROTATING * 0.50;
    		}
		}
	}

    // Save rotate
    VectorCopy(ent->s.apos.trBase, ent->s.angles);
	VectorCopy(ent->s.apos.trBase, ent->r.currentAngles);
	if(ent->s.modelindex2){
		ent->r.currentAngles[1] += 90;	//if brush model, rotate bsp model
	}

    // Link the entity back into the world
    trap_LinkEntity(ent);

    // Check for solid start (possible embedded in another object)
    if (tr.startsolid) {
        tr.fraction = 0;
    }

    // Run think function after updating entity
    G_RunThink(ent);
	
	if ( tr.fraction == 1 ) {
		return;
	}

	G_BounceProp(ent, &tr);
}

/*
================
G_FindEntityForEntityNum

================
*/
gentity_t *G_FindEntityForEntityNum(int entityn) {
    int i;
    gentity_t *ent;

    // go through all allocated objects
    for (i = 0, ent = g_entities; i < level.num_entities; i++, ent++) {
        if (ent->s.number == entityn) {
            return ent;
        }
    }
    
    return NULL;
}

gentity_t *G_FindEntityForClientNum(int entityn) {
    int i;
    gentity_t *ent;

    for (i = 0, ent = g_entities; i < level.num_entities; i++, ent++) {
        if (ent->client || ent->client->ps.clientNum == entityn) {
            return ent;
        }
    }
    
    return NULL;
}

