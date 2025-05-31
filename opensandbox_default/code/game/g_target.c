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

void Use_Target_Give( gentity_t *ent, gentity_t *other, gentity_t *activator ) {
	gentity_t	*t;
	trace_t		trace;

	if ( ent->spawnflags & 1 )
		activator = level.player;
	
	if ( !activator->client ) {
		return;
	}

	if ( !ent->target ) {
		return;
	}

	memset( &trace, 0, sizeof( trace ) );
	t = NULL;
	while ( (t = G_Find (t, FOFS(targetname), ent->target)) != NULL ) {
		if ( !t->item ) {
			continue;
		}
		Touch_Item2( t, activator, &trace, qtrue );

		// make sure it isn't going to respawn or show any events
		t->nextthink = 0;
		trap_UnlinkEntity( t );
	}
}

void SP_target_give( gentity_t *ent ) {
	ent->use = Use_Target_Give;
}

void Think_Target_Delay( gentity_t *ent ) {
	if (!ent->r.linked)
		return;

	ent->nextthink = 0;
	G_UseTargets( ent, ent->activator );
}

void Use_Target_Delay( gentity_t *ent, gentity_t *other, gentity_t *activator ) {
	if (!ent->r.linked)
		return;

	if ( ent->nextthink && (ent->spawnflags & 1) ) {
		ent->nextthink = 0;
	} else {
	ent->nextthink = level.time + ( ent->wait + ent->random * crandom() ) * 1000;
	ent->think = Think_Target_Delay;
	ent->activator = activator;
	}
}

void SP_target_delay( gentity_t *ent ) {
	// check delay for backwards compatability
	if ( !G_SpawnFloat( "delay", "0", &ent->wait ) ) {
		G_SpawnFloat( "wait", "1", &ent->wait );
	}

	if ( !ent->wait ) {
		ent->wait = 1;
	}
	ent->use = Use_Target_Delay;
	ent->r.linked = qtrue;
}

void Use_Target_Print (gentity_t *ent, gentity_t *other, gentity_t *activator) {
if ( ent->spawnflags & 8 ) {
	if ( activator->client && ( ent->spawnflags & 4 ) ) {
		trap_SendServerCommand( activator-g_entities, va("clp \"%s\"", ent->message ));
		return;
	}

	if ( ent->spawnflags & 3 ) {
		if ( ent->spawnflags & 1 ) {
			G_TeamCommand( TEAM_RED, va("clp \"%s\"", ent->message) );
		}
		if ( ent->spawnflags & 2 ) {
			G_TeamCommand( TEAM_BLUE, va("clp \"%s\"", ent->message) );
		}
		return;
	}
	trap_SendServerCommand( -1, va("clp \"%s\"", ent->message ));
} else {
	if ( activator->client && ( ent->spawnflags & 4 ) ) {
		trap_SendServerCommand( activator-g_entities, va("cp \"%s\"", ent->message ));
		return;
	}

	if ( ent->spawnflags & 3 ) {
		if ( ent->spawnflags & 1 ) {
			G_TeamCommand( TEAM_RED, va("cp \"%s\"", ent->message) );
		}
		if ( ent->spawnflags & 2 ) {
			G_TeamCommand( TEAM_BLUE, va("cp \"%s\"", ent->message) );
		}
		return;
	}
	trap_SendServerCommand( -1, va("cp \"%s\"", ent->message ));
}
}

void SP_target_print( gentity_t *ent ) {
	G_SpawnFloat( "wait", "-1", &ent->wait );

	ent->use = Use_Target_Print;
}

void Use_Target_Speaker (gentity_t *ent, gentity_t *other, gentity_t *activator) {
	if (ent->spawnflags & 3) {	// looping sound toggles
		if (ent->s.loopSound)
			ent->s.loopSound = 0;	// turn it off
		else
			ent->s.loopSound = ent->noise_index;	// start it
	}else {	// normal sound
		if ( ent->spawnflags & 8 ) {
			G_AddEvent( activator, EV_GENERAL_SOUND, ent->noise_index );
		} else if (ent->spawnflags & 4) {
			G_AddEvent( ent, EV_GLOBAL_SOUND, ent->noise_index );
		} else {
			G_AddEvent( ent, EV_GENERAL_SOUND, ent->noise_index );
		}
	}
}

void SP_target_speaker( gentity_t *ent ) {
	char	buffer[MAX_QPATH];
	char	*s;

	G_SpawnFloat( "wait", "0", &ent->wait );
	G_SpawnFloat( "random", "0", &ent->random );

	if ( !G_SpawnString( "noise", "NOSOUND", &s ) ) {
		//G_Error( "target_speaker without a noise key at %s", vtos( ent->s.origin ) );
	}

	// force all client reletive sounds to be "activator" speakers that
	// play on the entity that activates it
	if ( s[0] == '*' ) {
		ent->spawnflags |= 8;
	}

	if (!strstr( s, ".wav" )) {
		Com_sprintf (buffer, sizeof(buffer), "%s.wav", s );
	} else {
		Q_strncpyz( buffer, s, sizeof(buffer) );
	}
	ent->noise_index = G_SoundIndex(buffer);

	// a repeating speaker can be done completely client side
	ent->s.eType = ET_SPEAKER;
	ent->s.eventParm = ent->noise_index;
	ent->s.frame = ent->wait * 10;
	ent->s.clientNum = ent->random * 10;


	// check for prestarted looping sound
	if ( ent->spawnflags & 1 ) {
		ent->s.loopSound = ent->noise_index;
	}

	ent->use = Use_Target_Speaker;

	if (ent->spawnflags & 4) {
		ent->r.svFlags |= SVF_BROADCAST;
	}

	VectorCopy( ent->s.origin, ent->s.pos.trBase );

	// must link the entity so we get areas and clusters so
	// the server can determine who to send updates to
	trap_LinkEntity( ent );
}

void target_laser_think(gentity_t *self) {
	vec3_t	end;
	trace_t	tr;
	vec3_t	point;

	// if pointed at another entity, set movedir to point at it
	if (self->enemy) {
		//VectorMA(self->enemy->s.origin, 0.5, self->enemy->r.mins, point);
		VectorMA(self->enemy->r.currentOrigin, 0.5, self->enemy->r.mins, point);
		
		VectorMA(point, 0.5, self->enemy->r.maxs, point);
		VectorSubtract(point, self->s.origin, self->movedir);
		VectorNormalize(self->movedir);
	}

	// fire forward and see what we hit
	VectorMA(self->s.origin, 4096, self->movedir, end);

	trap_Trace(&tr, self->s.origin, NULL, NULL, end, self->s.number, CONTENTS_SOLID | CONTENTS_BODY );

	if (tr.entityNum != ENTITYNUM_NONE && self->damage) {
		// hurt it if we can
		G_Damage(&g_entities[tr.entityNum], self, self, self->movedir,
			tr.endpos, self->damage, 0, MOD_TARGET_LASER);
	}

	VectorCopy(tr.endpos, self->s.origin2);

	trap_LinkEntity(self);
	self->nextthink = level.time + FRAMETIME;
}


void target_laser_on (gentity_t *self)
{
	if (!self->activator)
		self->activator = self;
	target_laser_think (self);
}

void target_laser_off (gentity_t *self)
{
	trap_UnlinkEntity( self );
	self->nextthink = 0;
}

void target_laser_use (gentity_t *self, gentity_t *other, gentity_t *activator)
{
	self->activator = activator;
	if ( self->nextthink > 0 )
		target_laser_off (self);
	else
		target_laser_on (self);
}

void target_laser_start (gentity_t *self)
{
	gentity_t *ent;

	self->s.eType = ET_BEAM;

	if (self->target) {
		ent = G_Find (NULL, FOFS(targetname), self->target);
		if (!ent) {
			G_Printf ("%s at %s: %s is a bad target\n", self->classname, vtos(self->s.origin), self->target);
		}
		self->enemy = ent;
	} else {
		G_SetMovedir (self->s.angles, self->movedir);
	}

	self->use = target_laser_use;
	self->think = target_laser_think;

	if ( !self->damage ) {
		self->damage = 1;
	}

	if (self->spawnflags & 1)
		target_laser_on (self);
	else
		target_laser_off (self);


	if (self->spawnflags & 2)
		self->s.generic1 = 0;
	else
		self->s.generic1 = 1;
}

void SP_target_laser (gentity_t *self)
{
	char        *sound;
	vec3_t		color;
	int			r, g, b;

	// if the "noise" key is set, use a constant looping sound when moving
	if (G_SpawnString("noise", "100", &sound)) {
		self->s.loopSound = G_SoundIndex(sound);
	}
	// let everything else get spawned before we start firing
	self->think = target_laser_start;
	self->nextthink = level.time + FRAMETIME;

	//beam color
	G_SpawnVector("color", "1 0 0", color);

	r = color[0] * 255;
	if (r > 255) {
		r = 255;
	}
	g = color[1] * 255;
	if (g > 255) {
		g = 255;
	}
	b = color[2] * 255;
	if (b > 255) {
		b = 255;
	}
	self->s.constantLight = r | (g << 8) | (b << 16);
}

void target_teleporter_use( gentity_t *self, gentity_t *other, gentity_t *activator ) {
	gentity_t	*dest;

	if (!activator->client)
		return;
	dest = 	G_PickTarget( self->target );
	if (!dest) {
                G_Printf ("Couldn't find teleporter destination\n");
		return;
	}

	TeleportPlayer( activator, dest->s.origin, dest->s.angles );
}

void SP_target_teleporter( gentity_t *self ) {
	if (!self->targetname)
            G_Printf("untargeted %s at %s\n", self->classname, vtos(self->s.origin));

	self->use = target_teleporter_use;
}

void target_relay_use (gentity_t *self, gentity_t *other, gentity_t *activator) {
	if (!self->r.linked)
		return;

	if ( ( self->spawnflags & 1 ) && activator->client 
		&& activator->client->sess.sessionTeam != TEAM_RED ) {
		return;
	}
	if ( ( self->spawnflags & 2 ) && activator->client 
		&& activator->client->sess.sessionTeam != TEAM_BLUE ) {
		return;
	}
	if ( self->spawnflags & 4 ) {
		gentity_t	*ent;

		ent = G_PickTarget( self->target );
		if ( ent && ent->use ) {
			ent->use( ent, self, activator );
		}

		if ( self->target2 ) {
			ent = G_PickTarget( self->target2 );
			if ( ent && ent->use ) {
				ent->use( ent, self, activator );
			}
		}
		return;
	}

	if (!self->count || self->count < 0) {
		self->count = 1;
	}

	if (!self->damage)		//damage is used to keep track of the number of times the target_relay was triggered
		self->damage = 1;
	else
		self->damage++;

	if (self->damage == self->count)
	{
	G_UseTargets (self, activator);
		
		if ( self->spawnflags & 8 )
			G_FreeEntity( self );
		else
			self->damage = 0;
	}
}

void SP_target_relay (gentity_t *self) {
	self->use = target_relay_use;
	self->r.linked = qtrue;
}

void target_mapchange_use (gentity_t *self, gentity_t *other, gentity_t *activator) {
	self->nextthink = level.time + FADEOUT_TIME;
	
	//store session data to persist health/armor/weapons/ammo and variables to next level (only in SP mode)
	G_Sav_SaveData( activator, 0 );
	
	G_FadeOut( FADEOUT_TIME / 1000, -1 );
}

void target_mapchange_think (gentity_t *self) {
	char	*cmd;	

	cmd = "map";

	//perform map switch
	if ( self->mapname )
		trap_SendConsoleCommand( EXEC_INSERT, va( "%s %s\n", cmd, self->mapname ) ); 
	else
		trap_SendConsoleCommand( EXEC_INSERT, "map_restart 0\n" ); //shouldn't happen
}

void SP_target_mapchange (gentity_t *self) {
	char info[1024];

	if ( !self->mapname )
	{
		trap_GetServerinfo(info, sizeof(info));
		self->mapname = Info_ValueForKey( info, "mapname" );
	}
	self->use = target_mapchange_use;
	self->think = target_mapchange_think;
}

void botsandbox_check (gentity_t *self){
	if(self->parent && self->parent->health <= 0){
		self->think = 0;
		self->nextthink = level.time + 1;
		G_FreeEntity(self);
	}
	VectorCopy( self->parent->s.pos.trBase, self->s.origin );
	self->think = botsandbox_check;
	self->nextthink = level.time + 1;
}

void target_unlink_use (gentity_t *self, gentity_t *other, gentity_t *activator) {
	G_ToggleTargetsLinked( self );
}

//used for immediately spawnflag
void target_unlink_think (gentity_t *self) {
	self->nextthink = 0;
	G_ToggleTargetsLinked( self );
}

void SP_target_unlink (gentity_t *self) {
	self->use = target_unlink_use;
	
	if ( ( self->spawnflags & 16 ) ) {
		self->nextthink = level.time + FRAMETIME * 3;	//unlink entities next frame so they can spawn first
		self->think = target_unlink_think;
	}
}

void target_kill_use( gentity_t *self, gentity_t *other, gentity_t *activator ) {
	G_Damage ( activator, NULL, NULL, NULL, NULL, 100000, DAMAGE_NO_PROTECTION, MOD_TELEFRAG);
}

void SP_target_kill( gentity_t *self ) {
	self->use = target_kill_use;
}

void SP_target_position( gentity_t *self ){
	G_SetOrigin( self, self->s.origin );
}

static void target_location_linkup(gentity_t *ent)
{
	int i;
	int n;

	if (level.locationLinked) 
		return;

	level.locationLinked = qtrue;

	level.locationHead = NULL;

	trap_SetConfigstring( CS_LOCATIONS, "unknown" );

	for (i = 0, ent = g_entities, n = 1;
			i < level.num_entities;
			i++, ent++) {
		if (ent->classname && !Q_stricmp(ent->classname, "target_location") ) {
			// lets overload some variables!
			ent->health = n; // use for location marking
			trap_SetConfigstring( CS_LOCATIONS + n, ent->message );
			n++;
			ent->nextTrain = level.locationHead;
			level.locationHead = ent;
		}
	}

	// All linked together now
}

void SP_target_location( gentity_t *self ){
	self->think = target_location_linkup;
	self->nextthink = level.time + 200;  // Let them all spawn first

	G_SetOrigin( self, self->s.origin );
}

void script_variable_use (gentity_t *self, gentity_t *other, gentity_t *activator) {
	char value[128];

	if ( self->spawnflags & 1 || self->spawnflags & 2)
	{
		trap_Cvar_VariableStringBuffer(self->key, value, sizeof( value ));
		
		if ( (self->spawnflags & 1) && !strcmp(value, self->value) ) {
			G_UseTargets (self, activator);
		}
		
		if ( (self->spawnflags & 2) && strcmp(value, self->value) ) {
			G_UseTargets (self, activator);
		}
		
		if ( (self->spawnflags & 4) && (atoi(value) <= atoi(self->value)) ) {
			G_UseTargets (self, activator);
		}
		
		if ( (self->spawnflags & 8) && (atoi(value) >= atoi(self->value)) ) {
			G_UseTargets (self, activator);
		}
		
		return;
	}
	if ( self->spawnflags & 8192 ){
		trap_SendConsoleCommand( EXEC_APPEND, va("seta %s %s\n", self->key, self->value) );
	} else {
		trap_Cvar_Set( self->key, self->value );
	}
}

//used for immediately spawnflag
void script_variable_think (gentity_t *self) {
	self->nextthink = 0;
	script_variable_use( self, NULL, self );
}

void SP_script_variable (gentity_t *self) {
	if ( !self->key ) {
		G_Printf("WARNING: script_variable without key at %s\n", vtos(self->s.origin));
		G_FreeEntity( self );
		return;
	}

	if ( !self->key ) {
		G_Printf("WARNING: script_variable without value at %s\n", vtos(self->s.origin));
		G_FreeEntity( self );
		return;
	}
	
	self->use = script_variable_use;

	if ( ( self->spawnflags & 4096 ) ) {
		self->nextthink = level.time + FRAMETIME * 3;	//trigger entities next frame so they can spawn first
		self->think = script_variable_think;
	}
}

void use_script_cmd (gentity_t *ent, gentity_t *other, gentity_t *activator) {
	if (ent->spawnflags & 1) {
	trap_SendConsoleCommand( EXEC_APPEND, va("%s\n", ent->target) );
	}
	if (ent->spawnflags & 2) {
	trap_SendServerCommand( activator-g_entities, va("clcmd \"%s\"", ent->target ));	
	}
	if (ent->spawnflags & 4) {
	trap_SendServerCommand( -1, va("clcmd \"%s\"", ent->target ));	
	}
}

//used for immediately spawnflag
void script_cmd_think (gentity_t *ent) {
	ent->nextthink = 0;
	use_script_cmd( ent, NULL, ent );
}

void SP_script_cmd( gentity_t *ent ) {
	if ( !ent->target ) {
		G_Printf ("No target in script_cmd\n");
	}
	if ( ent->spawnflags & 4096 ) {
		ent->nextthink = level.time + FRAMETIME * 10;	//trigger entities next frame so they can spawn first
		ent->think = script_cmd_think;
	}
	ent->use = use_script_cmd;
}
