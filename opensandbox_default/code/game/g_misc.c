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

#include "g_local.h"

void SP_info_camp( gentity_t *self ) {
	G_SetOrigin( self, self->s.origin );
}

void SP_info_waypoint( gentity_t *self ) {
}

void SP_info_backpack( gentity_t *self ) {
}

void SP_info_null( gentity_t *self ) {
	G_FreeEntity( self );
}

void SP_info_notnull( gentity_t *self ){
	G_SetOrigin( self, self->s.origin );
}

void SP_light( gentity_t *self ) {
	G_FreeEntity( self );
}

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
	trap_SetConfigstring( CS_CUTSCENE, variableInfo );

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

	VectorCopy ( origin, player->client->ps.origin );

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

	// save results of pmove
	BG_PlayerStateToEntityState( &player->client->ps, &player->s, qtrue );
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
		ent->s.angles2[A2_MASS] = 1.00;	//mass for correct physics
		ent->sandboxObject = OBJ_EDITOR;
	}
	
	trap_LinkEntity( ent );
}

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
		fire_nail( ent, ent->s.origin, dir, right, up );
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

void SP_shooter_rocket( gentity_t *ent ) {
	InitShooter( ent, WP_ROCKET_LAUNCHER );
}

void SP_shooter_plasma( gentity_t *ent ) {
	InitShooter( ent, WP_PLASMAGUN);
}

void SP_shooter_grenade( gentity_t *ent ) {
	InitShooter( ent, WP_GRENADE_LAUNCHER);
}

void SP_shooter_bfg( gentity_t *ent ) {
	InitShooter( ent, WP_BFG);
}

void SP_shooter_prox( gentity_t *ent ) {
	InitShooter( ent, WP_PROX_LAUNCHER);
}

void SP_shooter_flame( gentity_t *ent ) {
	InitShooter( ent, WP_FLAMETHROWER);
}

void SP_shooter_antimatter( gentity_t *ent ) {
	InitShooter( ent, WP_ANTIMATTER);
}

void SP_shooter_custom( gentity_t *ent ) {
	InitShooter( ent, WP_NAILGUN);
}

static void PortalDie (gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod) {
	G_FreeEntity( self );
	//FIXME do something more interesting
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
