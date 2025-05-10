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

void Use_target_remove_powerups( gentity_t *ent, gentity_t *other, gentity_t *activator ) {
	int i;

	if( !activator->client ) {
		return;
	}

	if ( !ent->spawnflags )
		ent->spawnflags = 3;	//POWERUPS + FLAGS

	//remove powerups
	if ( ent->spawnflags & 1 ) {
		activator->client->ps.powerups[PW_QUAD] = 0;
		activator->client->ps.powerups[PW_BATTLESUIT] = 0;
		activator->client->ps.powerups[PW_HASTE] = 0;
		activator->client->ps.powerups[PW_INVIS] = 0;
		activator->client->ps.powerups[PW_REGEN] = 0;
		activator->client->ps.powerups[PW_FLIGHT] = 0;
	}

	//remove weapons and ammo
	if ( ent->spawnflags & 4 ) {
		activator->client->ps.generic2 = WP_GAUNTLET;

		for ( i = 2; i < WEAPONS_NUM; i++ ) {
			activator->swep_list[i] = 0;
			activator->swep_ammo[i] = 0;
		}
	}

	//remove holdables and keys
	if ( ent->spawnflags & 8 ) {
		activator->client->ps.stats[STAT_HOLDABLE_ITEM] = 0;
	}
}

void SP_target_remove_powerups( gentity_t *ent ) {
	ent->use = Use_target_remove_powerups;
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

void Use_Target_Score (gentity_t *ent, gentity_t *other, gentity_t *activator) {
	AddScore( activator, ent->r.currentOrigin, ent->count );
}

void SP_target_score( gentity_t *ent ) {
	if ( !ent->count ) {
		ent->count = 1;
	}
	ent->use = Use_Target_Score;
}

void Use_Target_Clienttarg (gentity_t *ent, gentity_t *other, gentity_t *activator) {
		activator->targetname = ent->value;
}

void SP_target_clienttarg( gentity_t *ent ) {
	ent->use = Use_Target_Clienttarg;
}

void Use_Target_Stats (gentity_t *ent, gentity_t *other, gentity_t *activator) {
if(ent->spawnflags & 1){
if(ent->type == 1){
activator->client->ps.stats[STAT_HEALTH] += ent->count;
activator->health += ent->count;
}
if(ent->type == 2){
activator->client->ps.stats[STAT_HOLDABLE_ITEM] += ent->count;
}
if(ent->type == 3){
activator->client->ps.stats[STAT_PERSISTANT_POWERUP] += ent->count;
}
if(ent->type == 4){

}
if(ent->type == 5){
activator->client->ps.stats[STAT_ARMOR] += ent->count;
}
if(ent->type == 6){
activator->client->ps.stats[STAT_MAX_HEALTH] += ent->count;
}
if(ent->type == 7){
activator->client->ps.stats[STAT_NO_PICKUP] += ent->count;
}
if(ent->type == 8){

}
if(ent->type == 9){
activator->client->ps.stats[STAT_MONEY] += ent->count;
activator->client->pers.oldmoney += ent->count;
}
return;
}
if(ent->type == 1){
activator->client->ps.stats[STAT_HEALTH] = ent->count;
activator->health = ent->count;
}
if(ent->type == 2){
activator->client->ps.stats[STAT_HOLDABLE_ITEM] = ent->count;
}
if(ent->type == 3){
activator->client->ps.stats[STAT_PERSISTANT_POWERUP] = ent->count;
}
if(ent->type == 4){

}
if(ent->type == 5){
activator->client->ps.stats[STAT_ARMOR] = ent->count;
}
if(ent->type == 6){
activator->client->ps.stats[STAT_MAX_HEALTH] = ent->count;
}
if(ent->type == 7){
activator->client->ps.stats[STAT_NO_PICKUP] = ent->count;
}
if(ent->type == 8){

}
if(ent->type == 9){
activator->client->ps.stats[STAT_MONEY] = ent->count;
activator->client->pers.oldmoney = ent->count;
}
}

void SP_target_stats( gentity_t *ent ) {
	if ( !ent->type ) {
		G_Printf ("No type in target_stats\n");
	}
	ent->use = Use_Target_Stats;
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
	int			r, g, b, i;

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
	/*
	i = light / 4;
	if (i > 255) {
		i = 255;
	}
	self->s.constantLight = r | (g << 8) | (b << 16) | (i << 24);
	*/
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

void target_logic_reset (gentity_t *self) {
	int i;
	for (i = 0; i < MAX_LOGIC_ENTITIES; i++)
		self->logicEntities[i] = 0;
}

void target_logic_use (gentity_t *self, gentity_t *other, gentity_t *activator) {
	int			i;
	int			triggerCount;		//number of entities targetting this target_logic
	int			triggeredCount;		//number of entities targetting this target_logic that have already been triggered
	qboolean	found;
	gentity_t	*t;

	//determine the number of entities (triggers) targeting this target_logic
	triggerCount = triggeredCount = 0;
	t = NULL;
	while ( (t = G_Find (t, FOFS(target), self->targetname)) != NULL ) {
		found = qfalse;
		if ( t == self ) {
			G_Printf ("WARNING: Entity %s at %s used itself.\n", self->classname, vtos(self->s.origin));
		} else {
			triggerCount++;
		}
	}

	//count the number of entities that have already triggered the target_logic
	for ( i = 0; i < MAX_LOGIC_ENTITIES; i++ ) {
		if ( self->logicEntities[i] ) {
			triggeredCount++;

			//if the entity that is currently being triggered is in the list, see if we should remove it again
			if ( self->logicEntities[i] == other->s.number ) {
				found = qtrue;
				
				//STAY_ON is not selected, remove the trigger for the list of triggered entities
				if ( !(self->spawnflags & 8) ) {
					self->logicEntities[i] = 0;
					triggeredCount--;		//the trigger was counted as being triggerd, but is not so anymore
				}
			}
		}
	}

	//the entity was not yet in the list of triggered entities, so add it
	if ( !found ) {
		for ( i = 0; i < MAX_LOGIC_ENTITIES; i++ ) {
			if ( !self->logicEntities[i] )
			{
				self->logicEntities[i] = other->s.number;
				triggeredCount++;
				break;
			}
		}
	}
       
	if ( triggerCount == triggeredCount ) {
		target_logic_reset( self );

		//If RANDOM is selected so use a random target
		if ( self->spawnflags & 4 ) {
			gentity_t	*ent;

			ent = G_PickTarget( self->target );
			if ( ent && ent->use ) {
				ent->use( ent, self, activator );
			}
			return;
		}
		
		//The RANDOM spawnflag wasn't selected, so use all targets
		G_UseTargets (self, activator);
	}
}



void SP_target_logic (gentity_t *self) {
	self->use = target_logic_use;
	target_logic_reset( self );
}

void target_mapchange_use (gentity_t *self, gentity_t *other, gentity_t *activator) {
	self->nextthink = level.time + FADEOUT_TIME;
	
	//store session data to persist health/armor/weapons/ammo and variables to next level (only in SP mode)
	G_Sav_SaveData( activator, 0 );
	
	G_FadeOut( FADEOUT_TIME / 1000, -1 );
}

void target_mapchange_think (gentity_t *self) {
	char	*cmd;	

	//determine map switch command to use
	if ( g_gametype.integer == GT_SINGLE )
		cmd = "spmap";		//keep single player enabled
	else
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

void target_gravity_use (gentity_t *self, gentity_t *other, gentity_t *activator) {
	int i;

	if ( !self->count )
		self->count = g_gravity.integer;
	
	if ( (self->spawnflags & 1) )
	{
		for (i = 0; i < level.maxclients; i++)
		{
			level.clients[i].ps.gravity = self->count;
		}
	}
	else
		activator->client->ps.gravity = self->count;
}

void SP_target_gravity (gentity_t *self) {
	self->use = target_gravity_use;
}

void target_botspawn_use (gentity_t *self, gentity_t *other, gentity_t *activator) {
	if ( g_debugBotspawns.integer ) 
		G_Printf("\ntime %i\n spawn bot \"%s\"\n botspawn \"%s\" / \"%s\" (%i)\n waypoint \"%s\"\n health %i\n", level.time, self->clientname, self->targetname, self->targetname2, self->s.number, self->target, self->health);
	G_AddCustomBot( self->clientname, self->s.number, self->target, self->skill, self->type, self->message );
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

void SP_target_botspawn (gentity_t *self) {
	float healthMultiplier = 1;
	float skill;
	if(self->sb_isnpc <= 0){
	if ( !self->clientname || !strcmp(self->clientname, "") )
		self->clientname = "sarge";

	G_SpawnFloat( "skill", "0", &self->skill );

	//determine bot's health based on current and relative skill level
	if ( !self->health )
		self->health = 100;

	skill = self->skill;
	if ( skill < 1 )
		skill = 1;

	//bot's health will be 20% less for each skill level under level 3 and 20% more for each skill level above level 3
	healthMultiplier = 1 + ((skill - 3) * 0.2);
	self->health *= healthMultiplier;

	self->use = target_botspawn_use;
	} else {
		G_AddBot(self->clientname, self->skill, "Blue", 0, self->message, self->s.number, self->target, self->type, self );
	}
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

void target_playerspeed_use (gentity_t *self, gentity_t *other, gentity_t *activator) {
	if ( !self->speed ) {
		self->speed = g_speed.value;
	}

	if ( self->speed == -1 )
		activator->client->ps.pm_type = PM_FREEZE;
	else {
		activator->client->ps.pm_type = PM_NORMAL;
		activator->speed = self->speed;	//this doesn't actually change the player's speed. This value is read in ClientThink_real (g_active.c) again.
	}
}

void SP_target_playerspeed (gentity_t *self) {
	self->use = target_playerspeed_use;
}

void target_debrisemitter_use (gentity_t *self, gentity_t *other, gentity_t *activator) {
	gentity_t *ent;
	
	if ( !self->count )
		self->count = 10;

	ent = G_TempEntity( self->s.origin, PickDebrisType( self->spawnflags ) );
	ent->s.eventParm = self->count;
}

void SP_target_debrisemitter (gentity_t *self) {
	self->use = target_debrisemitter_use;
}

void target_objective_use (gentity_t *self, gentity_t *other, gentity_t *activator) {
	
	if ( self->spawnflags & 1 )
		trap_SetConfigstring( CS_SECONDARYOBJECTIVE, self->message );
	else
		trap_SetConfigstring( CS_PRIMARYOBJECTIVE, self->message );

	if ( !(self->spawnflags & 2) )
		trap_SendServerCommand( -1, "ou" );
}

void SP_target_objective (gentity_t *self) {
	G_SpawnInt( "id", "-1", &self->s.generic1 );
	self->use = target_objective_use;
}

void target_skill_use (gentity_t *self, gentity_t *other, gentity_t *activator) {
	trap_SendConsoleCommand( EXEC_INSERT, va( "seta g_spskill %i\n", self->skill) );
}

void SP_target_skill (gentity_t *self) {
	G_SpawnFloat( "skill", "2", &self->skill );

	if ( self->skill > 14 ) 
		self->skill = 14;
	else if ( self->skill < 1 )
		self->skill = 1;
	
	self->use = target_skill_use;
}

void target_earthquake_use (gentity_t *self, gentity_t *other, gentity_t *activator) {
	G_AddEvent(activator, EV_EARTHQUAKE, self->s.generic1);
}

void SP_target_earthquake (gentity_t *self) {
	int param;
	float length;		// length in seconds (3 to 32)
	float intensity;	// intensity (1 to 16)
	int length_;
	int intensity_;
	
	// read parameters
	G_SpawnFloat( "length", "3.0", &length );
	G_SpawnFloat( "intensity", "1.0", &intensity );
	if ( length < 2 ) length = 2;
	if ( length > 32 ) length = 32;
	if ( intensity < 1 ) intensity = 1;
	if ( intensity > 16 ) intensity = 16;
	
	// adjust parameters
	length_ = ((int)(length) - 2) / 2;
	intensity_ = (int)intensity - 1;
	param = ( intensity_ | (length_ << 4) );
	self->s.generic1 = param;
	self->use = target_earthquake_use;
	self->s.eType = ET_EVENTS;
	trap_LinkEntity (self);
}

void target_effect_use (gentity_t *self, gentity_t *other, gentity_t *activator) {
	gentity_t	*ent2;
	gentity_t	*ent3;
	gentity_t	*ent4;
	gentity_t	*ent5;
	gentity_t   *ent6;

	if ( !self->r.linked )
		return;

	//explosion
	if ( self->spawnflags & SF_EFFECT_EXPLOSION ) {
		G_TempEntity( self->s.origin, EV_EXPLOSION );
	}

	//particles_gravity
	if ( self->spawnflags & SF_EFFECT_PARTICLES_GRAVITY ) {
		ent2 = G_TempEntity( self->s.origin, EV_PARTICLES_GRAVITY );
		ent2->s.constantLight = self->s.constantLight;	//constantLight is used to determine particle color
		ent2->s.eventParm = self->count; //eventParm is used to determine the number of particles
		ent2->s.generic1 = self->speed; //generic1 is used to determine the speed of the particles
		ent2->s.generic2 = atoi(self->value); //generic2 is used to determine the size of the particles
	}

	//particles_linear
	if ( self->spawnflags & SF_EFFECT_PARTICLES_LINEAR ) {
		ent3 = G_TempEntity( self->s.origin, EV_PARTICLES_LINEAR );
		ent3->s.constantLight = self->s.constantLight;	//constantLight is used to determine particle color
		ent3->s.eventParm = self->count; //eventParm is used to determine the number of particles
		ent3->s.generic1 = self->speed; //generic1 is used to determine the speed of the particles
		ent2->s.generic2 = atoi(self->value); //generic2 is used to determine the size of the particles
	}

	//particles_linear_up
	if ( self->spawnflags & SF_EFFECT_PARTICLES_LINEAR_UP ) {
		ent4 = G_TempEntity( self->s.origin, EV_PARTICLES_LINEAR_UP );
		ent4->s.constantLight = self->s.constantLight;	//constantLight is used to determine particle color
		ent4->s.eventParm = self->count; //eventParm is used to determine the number of particles
		ent4->s.generic1 = self->speed; //generic1 is used to determine the speed of the particles
		ent2->s.generic2 = atoi(self->value); //generic2 is used to determine the size of the particles
	}

	//particles_linear_down
	if ( self->spawnflags & SF_EFFECT_PARTICLES_LINEAR_DOWN ) {
		ent5 = G_TempEntity( self->s.origin, EV_PARTICLES_LINEAR_DOWN );
		ent5->s.constantLight = self->s.constantLight;	//constantLight is used to determine particle color
		ent5->s.eventParm = self->count; //eventParm is used to determine the number of particles
		ent5->s.generic1 = self->speed; //generic1 is used to determine the speed of the particles
		ent2->s.generic2 = atoi(self->value); //generic2 is used to determine the size of the particles
	}

	//overlay
	if ( self->spawnflags & SF_EFFECT_OVERLAY ) {
		if ( self->overlay ) {
			trap_SetConfigstring( CS_OVERLAY, self->overlay );
		} else {
			trap_SetConfigstring( CS_OVERLAY, "" );
		}
		
		//send event to player which will register the asset so it may subsequently be drawn each frame.
		G_TempEntity( self->s.origin, EV_OVERLAY );
	}

	//fade
	if ( self->spawnflags & SF_EFFECT_FADE ) {
		if ( self->spawnflags & SF_EFFECT_ACTIVATOR ) {
		G_Fade( self->wait, self->rgba1, self->rgba2, activator-g_entities ); 
		} else {
		G_Fade( self->wait, self->rgba1, self->rgba2, -1 ); 
		}
	}

	//smoke
	if (self->spawnflags & SF_EFFECT_SMOKEPUFF) {
		ent6 = G_TempEntity(self->s.origin, EV_SMOKEPUFF);
		ent6->s.constantLight = self->s.constantLight;
		ent6->s.eventParm = self->wait;	//eventParm is used to determine the amount of time the smoke puff exists
		ent6->s.generic1 = self->speed;	//generic1 is used to determine the movement speed of the smoke puff
		ent6->s.otherEntityNum = self->distance * 32; //otherEntityNum is used to determine the size of the smokepuff. The default is 32.
		VectorCopy(self->s.angles, ent6->s.angles);
	}
}

void target_effect_think(gentity_t* self) {
	gentity_t* tmp;
	vec3_t		wut;
	vec3_t		dir;

	if (self->target) {
		tmp = G_PickTarget(self->target);
		VectorSubtract(tmp->s.origin, self->s.origin, dir);
		VectorCopy(dir, self->s.angles);
	}
	else if (self->target2) {
		tmp = G_PickTarget(self->target2);
		VectorSubtract(tmp->s.origin, self->s.origin, dir);
		VectorCopy(dir, self->s.angles);
	}
	else {
		self->s.angles[0] = 0;
		self->s.angles[1] = 0;
		self->s.angles[2] = 1;
	}
	VectorNormalize(self->s.angles);

	self->nextthink = 0;
}

void SP_target_effect (gentity_t *self) {
	vec3_t		color;
	float		light;
	int			r, g, b, i;
	float		scale;
	
	//check if effects are selected
	if ( !self->spawnflags ) {
		G_Printf( va( S_COLOR_YELLOW "WARNING: %s without selected effects at %s\n", self->classname, vtos(self->s.origin) ) );
		G_FreeEntity( self );
	}
	
	// particle/smoke color
	if ( 
		self->spawnflags & SF_EFFECT_PARTICLES_GRAVITY || 
		self->spawnflags & SF_EFFECT_PARTICLES_LINEAR || 
		self->spawnflags & SF_EFFECT_PARTICLES_LINEAR_UP ||
		self->spawnflags & SF_EFFECT_PARTICLES_LINEAR_DOWN ||
		self->spawnflags & SF_EFFECT_SMOKEPUFF
	) {
		G_SpawnVector( "color", "1 1 1", color );
		G_SpawnInt( "value", "3", &self->s.generic2 );		//particle size

		r = color[0] * 255;
		if ( r > 255 ) {
			r = 255;
		}
		g = color[1] * 255;
		if ( g > 255 ) {
			g = 255;
		}
		b = color[2] * 255;
		if ( b > 255 ) {
			b = 255;
		}
		i = light / 4;
		if ( i > 255 ) {
			i = 255;
		}
		self->s.constantLight = r | ( g << 8 ) | ( b << 16 ) | ( i << 24 );
	}

	//particle count
	if ( 
		self->spawnflags & SF_EFFECT_PARTICLES_GRAVITY || 
		self->spawnflags & SF_EFFECT_PARTICLES_LINEAR || 
		self->spawnflags & SF_EFFECT_PARTICLES_LINEAR_UP ||
		self->spawnflags & SF_EFFECT_PARTICLES_LINEAR_DOWN
	) {
		if ( !self->count )
			self->count = 100;
		else if (self->count > 255)
			self->count = 255;

		if ( !self->speed )
			self->speed = 100;
	}

	//smoke speed, duration and angle
	if (self->spawnflags & SF_EFFECT_SMOKEPUFF) {
		if ( !self->speed ) {
			self->speed = 16;
		}
		if (!self->wait) {
			self->wait = 2;
		}
		if (!G_SpawnFloat("scale", "1", &scale)) {
			scale = 1;
		}

		self->distance = scale;		//abuse the distance field for scale
		
		self->nextthink = level.time + FRAMETIME * 3;
		self->think = target_effect_think;
	}

	//fade info
	if ( self->spawnflags & SF_EFFECT_FADE ) {
		G_SpawnVector4( "startcolor", "0 0 0 0", self->rgba1 );
		G_SpawnVector4( "endcolor", "0 0 0 1", self->rgba2 );
		if ( !self->wait ) {
			self->wait = 2;
		}
	}

	self->use = target_effect_use;

	trap_LinkEntity( self );
}

void target_finish_think(gentity_t* self) {
	target_finish_use(self, self, level.player);
}

void target_finish_use (gentity_t *self, gentity_t *other, gentity_t *activator) {
	int secretFound, secretCount;
	playerscore_t highScores;
	playerscore_t scores;

	// bots should not be able to activate this
	if ( IsBot( activator ) )
		return;

	if (level.player->client->ps.pm_type == PM_CUTSCENE) {
		// we're in a cutscene. Things break if we go to intermission while in a cutscene. Make target_finish wait for cutscene to end.
		self->think = target_finish_think;
		self->nextthink = level.time + FRAMETIME;
	} else {
		//set number of secrets to persistant so it can be displayed in the client side scoreboard. If user persistant secretcount already
		//contains a secretcount from a previous level, add that to the secretcount of this level.

		//TODO: This code below is "run once" code (I think). Rewrite it so it can be run over and over again so we can properly determine the player's score at any point in time
		secretFound = (level.player->client->ps.persistant[PERS_SECRETS] & 0x7F);
		secretCount = ((level.player->client->ps.persistant[PERS_SECRETS] >> 7) & 0x7F) + level.secretCount;
		level.player->client->ps.persistant[PERS_SECRETS] = secretFound + (secretCount << 7);

		// calculate player's score
		scores = G_CalculatePlayerScore(level.player);

		BeginIntermission();
	}
}

void SP_target_finish (gentity_t *self) {
	self->use = target_finish_use;
}

void modify_entity ( gentity_t *self, gentity_t *ent ) {
	if ( !strcmp( self->key, "spawnflags" ) ) {
		ent->spawnflags = atoi(self->value);
		return;
	}

	if (!strcmp(self->key, "wait")) {
		ent->wait = atof(self->value);
		return;
	}
	
	if (!strcmp(self->key, "random")) {
		ent->random = atof(self->value);
		return;
	}
	
	if (!strcmp(self->key, "team")) {
		ent->team = self->value;
		return;
	}

	if ( !strcmp( self->key, "message" ) ) {
		ent->message = self->value;
		return;
	}
	
	if ( !strcmp( self->key, "botname" ) ) {
		ent->botname = self->value;
		return;
	}
	
	if ( !strcmp( self->key, "playerangle" ) ) {
		ent->playerangle = atoi(self->value);
		return;
	}

	if ( !strcmp( self->key, "price" ) ) {
		ent->price = atoi(self->value);
		return;
	}

	if ( !strcmp( self->key, "target" ) ) {
		ent->target = self->value;
		if ( !strcmp( ent->classname, "path_corner" ) )
			Think_SetupTrainTargets( ent );
		
		if ( strstr( ent->classname, "shooter_" ) ) {
			InitShooter( ent, ent->s.weapon );
		}

		return;
	}

	if ( !strcmp( self->key, "targetname" ) ) {
		ent->targetname = self->value;
		if ( !strcmp( ent->classname, "path_corner" ) )
			Think_SetupTrainTargets( ent );
		return;
	}

	if ( !strcmp( self->key, "targetshadername" ) ) {
		ent->targetShaderName = self->value;
		return;
	}

	if ( !strcmp( self->key, "targetnewshadername" ) ) {
		ent->targetShaderNewName = self->value;
		return;
	}

	if ( !strcmp( self->key, "angle" ) ) {
		ent->angle = atof(self->value);
		return;
	}

	if ( !strcmp( self->key, "speed" ) ) {
		ent->speed = atof(self->value);
		return;
	}

	if ( !strcmp( self->key, "health" ) ) {
		ent->health = atoi(self->value);
		return;
	}

	if ( !strcmp( self->key, "count" ) ) {
		ent->count = atoi(self->value);
		return;
	}

	if ( !strcmp( self->key, "clientname" ) ) {
		ent->clientname = self->value;
		return;
	}

	if ( !strcmp( self->key, "mapname" ) ) {
		ent->mapname = self->value;
		return;
	}
	
	if ( !strcmp( self->key, "music" ) ) {
		ent->music = self->value;
		return;
	}
	
	if ( !strcmp( self->key, "teleportertarget" ) ) {
		ent->teleporterTarget = self->value;
		return;
	}

	if ( !strcmp( self->key, "target2" ) ) {
		ent->target2 = self->value;
		return;
	}

	if ( !strcmp( self->key, "targetname2" ) ) {
		ent->targetname2 = self->value;
		return;
	}

	if ( !strcmp( self->key, "deathtarget" ) ) {
		ent->deathTarget = self->value;
		return;
	}

	if ( !strcmp( self->key, "loottarget" ) ) {
		ent->lootTarget = self->value;
		return;
	}

	if ( !strcmp( self->key, "skill" ) ) {
		ent->skill = atof(self->value);
		return;
	}
	
	if ( !strcmp( self->key, "overlay" ) ) {
		ent->overlay = self->value;
		return;
	}

	if ( !strcmp( self->key, "armor" ) ) {
		ent->armor = atoi(self->value);
		return;
	}

	if ( !strcmp( self->key, "key" ) ) {
		ent->key = self->value;
		return;
	}

	if ( !strcmp( self->key, "value" ) ) {
		ent->value = self->value;
		return;
	}

	if ( !strcmp( self->key, "light" ) ) {
		int		cl;
		int r, g, b, i;

		cl = ent->s.constantLight;

		r = (cl & 0xFF);
		g = ((cl >> 8) & 0xFF);
		b = ((cl >> 16) & 0xFF);

		ent->s.constantLight = r | ( g << 8 ) | ( b << 16 ) | ( atoi(self->value) << 24 );		
		return;
	}

	if ( !strcmp( self->key, "color" ) ) {
		int		cl, gn;
		int r, g, b, i;

		cl = ent->s.constantLight;
		gn = self->s.generic1;		

		r = (gn & 0xFF);
		g = ((gn >> 8) & 0xFF);
		b = ((gn >> 16) & 0xFF);
		i = ((cl >> 24) & 0xFF) * 4.0;

		ent->s.constantLight = r | ( g << 8 ) | ( b << 16 ) | ( i << 24 );		
		return;
	}

	if ( !strcmp( self->key, "damage" ) || !strcmp( self->key, "dmg" ) ) {
		ent->damage = atoi(self->value);
		return;
	}

	G_Printf("WARNING: Incorrect key \"%s\" for %s at %s\n", self->key, self->classname, vtos(self->s.origin));
}

void target_modify_use (gentity_t *self, gentity_t *other, gentity_t *activator) {
	gentity_t *t;

	t = NULL;
	while ( (t = G_Find (t, FOFS(targetname), self->target)) != NULL ) {
		modify_entity(self, t);
		if(self->spawnflags & 1){
		G_CallSpawn(t);
		}
		if ( !self->inuse ) {
			G_Printf("entity was removed while using targets\n");
			return;
		}
	}

	t = NULL;
	while ( (t = G_Find (t, FOFS(targetname2), self->target)) != NULL ) {
		modify_entity(self, t);
		if(self->spawnflags & 1){
		G_CallSpawn(t);
		}
		if ( !self->inuse ) {
			G_Printf("entity was removed while using targets\n");
			return;
		}
	}

	t = NULL;
	while ( (t = G_Find (t, FOFS(targetname), self->target2)) != NULL ) {
		modify_entity(self, t);
		if(self->spawnflags & 1){
		G_CallSpawn(t);
		}
		if ( !self->inuse ) {
			G_Printf("entity was removed while using targets\n");
			return;
		}
	}

	t = NULL;
	while ( (t = G_Find (t, FOFS(targetname2), self->target2)) != NULL ) {
		modify_entity(self, t);
		if(self->spawnflags & 1){
		G_CallSpawn(t);
		}
		if ( !self->inuse ) {
			G_Printf("entity was removed while using targets\n");
			return;
		}
	}
}

void SP_target_modify (gentity_t *self) {
	vec3_t	color;
	int		r, g, b, i;

	self->use = target_modify_use;

	//store color value in generic1
	if ( !strcmp(self->key, "color" ) ) {
		G_SpawnVector( "value", "1 1 1", color );

		r = color[0] * 255;
		if ( r > 255 ) {
			r = 255;
		}
		g = color[1] * 255;
		if ( g > 255 ) {
			g = 255;
		}
		b = color[2] * 255;
		if ( b > 255 ) {
			b = 255;
		}
		i = 0;
		self->s.generic1 = r | ( g << 8 ) | ( b << 16 ) | ( i << 24 );
	}
}

void target_secret_use (gentity_t *self, gentity_t *other, gentity_t *activator) {
	activator->client->ps.persistant[PERS_SECRETS]++;

	if ( !(self->spawnflags & 1) ) {
		if ( self->message )
			trap_SendServerCommand( -1, va("cp \"%s\"", self->message ));
		else
			trap_SendServerCommand( -1, va("cp \"%s\"", "You found a secret!" ));
	}

	//remove the entity so it cannot be triggered again
	G_FreeEntity( self );
}

void SP_target_secret (gentity_t *self) {
	
	//register the secret
	level.secretCount++;

	self->use = target_secret_use;
}

void target_playerstats_use (gentity_t *self, gentity_t *other, gentity_t *activator) {
	if ( IsBot(activator) )		//do not allow bots to trigger this entity
		return;

	if ((activator->client->ps.stats[STAT_HEALTH] < self->health || !(self->spawnflags & 1)) && !(self->spawnflags & 2))
		activator->health = activator->client->ps.stats[STAT_HEALTH] = self->health;

	if ((activator->client->ps.stats[STAT_ARMOR] < self->armor || !(self->spawnflags & 1)) && !(self->spawnflags & 4))
		activator->client->ps.stats[STAT_ARMOR] = self->armor;
}

void SP_target_playerstats (gentity_t *self) {
	self->use = target_playerstats_use;
}

void target_cutscene_use (gentity_t *self, gentity_t *other, gentity_t *activator) {
	int i;

	//if cutscenes are disabled, do nothing
	if ( g_disableCutscenes.integer ) {
		return;
	}

	//bots shouldn't be able to activate this entity
	if ( IsBot( activator ) )
		return;

	//if the entity doesnt target camera, return
	if ( !self->nextTrain ) {
		G_Printf("WARNING: %s at %s does not target an info_camera", self->classname, vtos(self->s.origin));
		G_FreeEntity( self );
		return;
	}

	//unlink the player from the world so he becomes non-solid, as the player will physically be in the position of the camera
	trap_UnlinkEntity( level.player );
	level.player->r.contents -= CONTENTS_BODY;

	//prevent bots from moving/shooting while playing the cutscene
	if ( self->spawnflags & 1 ) {
		for ( i = 0 ; i < level.maxclients ; i++ ) {
			if ( level.clients[i].pers.connected != CON_DISCONNECTED && level.clients[i].ps.pm_type != PM_DEAD )
				level.clients[i].ps.pm_type = PM_CUTSCENE;
		}
	}

	//set player's velocity to 0 so he stops right in his tracks (without this the player slides slightly forward when the cutscene ends)
	level.player->client->ps.velocity[0] = 0;
	level.player->client->ps.velocity[1] = 0;
	level.player->client->ps.velocity[2] = 0;

	//set player's orgOrigin so we can move the player back to its original location when the cutscene ends
	//if (level.player->client->ps.pm_type != PM_CUTSCENE) {
	VectorCopy(level.player->client->ps.origin, level.player->orgOrigin);
	//}

	//disable synchronousClients to prevent "CVAR_Update: handle out of range" error. See issue 162.
	if (g_allowSyncCutscene.integer == 0) {
		level.player->skill = g_synchronousClients.integer;	//abusing the skill field to temporarily store the sync-clients value
		trap_Cvar_Set("g_synchronousClients", "0");
	}
	
	//activate the first camera
	self->nextTrain->use( self->nextTrain, other, level.player);
}

void target_cutscene_think (gentity_t *self) {
	G_LinkCameras( self );
}

void SP_target_cutscene (gentity_t *self) {
	/*if ( !self->target && !self->target2 ) {
		G_Printf("WARNING: %s without a target or target2 at %s\n", self->classname, vtos(self->s.origin));
		G_FreeEntity( self );
		return;
	}*/

	self->nextthink = level.time + FRAMETIME * 3;
	self->think = target_cutscene_think;
	self->use = target_cutscene_use;
}

void target_botremove_use (gentity_t *self, gentity_t *other, gentity_t *activator) {
	G_RemoveBotsForTarget( self, (self->spawnflags & 1), (self->spawnflags & 2) );
}

void SP_target_botremove (gentity_t *self) {
	if ( !self->target && !self->target2 ) {
		G_Printf("WARNING: %s without a target or target2 at %s\n", self->classname, vtos(self->s.origin));
		G_FreeEntity( self );
		return;
	}
	
	self->use = target_botremove_use;
}

void target_music_use (gentity_t *self, gentity_t *other, gentity_t *activator) {
	trap_SetConfigstring( CS_MUSIC, self->music );
}

void SP_target_music (gentity_t *self) {
	char	*s;
	char	buffer[MAX_INFO_STRING];
	G_SpawnString( "music", "", &s );
	Q_strncpyz( self->music, s, sizeof(self->music) );

	self->use = target_music_use;
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

// FIXME: improve these so they only need to be send to the client once?
void SP_rally_weather_rain( gentity_t *ent ){
	trap_SetBrushModel( ent, ent->model );
	ent->s.eType = ET_WEATHER;

	ent->s.powerups = ent->type;
	ent->s.weapon = 0;
	ent->s.legsAnim = ent->spawnflags;

	trap_LinkEntity (ent);
}


void SP_rally_weather_snow( gentity_t *ent ){
	trap_SetBrushModel( ent, ent->model );
	ent->s.eType = ET_WEATHER;

	ent->s.powerups = ent->type;
	ent->s.weapon = 1;
	ent->s.legsAnim = 0;

	trap_LinkEntity (ent);
}

void script_variable_use (gentity_t *self, gentity_t *other, gentity_t *activator) {
	char buf[MAX_INFO_STRING];
	char variableInfo[MAX_INFO_STRING];
	char value[128];

	if ( self->spawnflags & 1 || self->spawnflags & 2)
	{
		trap_Cvar_VariableStringBuffer(self->key, value, sizeof( value ));
		if ( g_debugVariables.integer ) {
			G_Printf("\nDebugvariables: comparing variable \"%s\" to \"%s\"\n", self->key, self->value);
			G_Printf("In-memory value for variable = \"%s\"\n", value);
		}
		
		if ( (self->spawnflags & 1) && !strcmp(value, self->value) ) {
			if ( g_debugVariables.integer ) G_Printf("Variable =, targets will be activated\n");
			G_UseTargets (self, activator);
		}
		
		if ( (self->spawnflags & 2) && strcmp(value, self->value) ) {
			if ( g_debugVariables.integer ) G_Printf("Variable !=, targets will be activated\n");
			G_UseTargets (self, activator);
		}
		
		if ( (self->spawnflags & 4) && (atoi(value) <= atoi(self->value)) ) {
			if ( g_debugVariables.integer ) G_Printf("Variable <=, targets will be activated\n");
			G_UseTargets (self, activator);
		}
		
		if ( (self->spawnflags & 8) && (atoi(value) >= atoi(self->value)) ) {
			if ( g_debugVariables.integer ) G_Printf("Variable >=, targets will be activated\n");
			G_UseTargets (self, activator);
		}
		
		return;
	}
	if ( self->spawnflags & 8192 ){
	trap_SendConsoleCommand( EXEC_APPEND, va("seta %s %s\n", self->key, self->value) );
	} else {
	trap_Cvar_Set( self->key, self->value );
	}
	if ( g_debugVariables.integer ) {
		G_Printf("\nDebugvariables: setting variable \"%s\" to \"%s\"\n", self->key, self->value);
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

void script_layer_use( gentity_t *self, gentity_t *other, gentity_t *activator ) {
	gentity_t	*dest;

	if (!activator->client)
		return;

	TeleportPlayerForLayer( activator, self->distance, self->s.origin[2] );
}

void SP_script_layer( gentity_t *self ) {
	self->use = script_layer_use;
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

void use_script_menu (gentity_t *ent, gentity_t *other, gentity_t *activator) {
	if (ent->spawnflags & 1) {
	trap_SendConsoleCommand( EXEC_APPEND, va("nsgui %s\n", ent->target) );
	}
	if (ent->spawnflags & 2) {
	trap_SendServerCommand( activator-g_entities, va("clcmd \"nsgui %s\"", ent->target ));	
	}
	if (ent->spawnflags & 4) {
	trap_SendServerCommand( -1, va("clcmd \"nsgui %s\"", ent->target ));	
	}
}

//used for immediately spawnflag
void script_menu_think (gentity_t *ent) {
	ent->nextthink = 0;
	use_script_menu( ent, NULL, ent );
}

void SP_script_menu( gentity_t *ent ) {
	if ( !ent->target ) {
	G_Printf ("No target in script_menu\n");
	}
	if ( ent->spawnflags & 4096 ) {
		ent->nextthink = level.time + FRAMETIME * 10;	//trigger entities next frame so they can spawn first
		ent->think = script_menu_think;
	}
	ent->use = use_script_menu;
}

void use_script_aicontrol (gentity_t *ent, gentity_t *other, gentity_t *activator) {
	trap_SendConsoleCommand( EXEC_APPEND, va("say_team %s %s\n", ent->clientname, ent->target) );
}

void SP_script_aicontrol( gentity_t *ent ) {
	if ( !ent->target ) {
	G_Printf ("No target in script_aicontrol\n");
	}
	ent->use = use_script_aicontrol;
}

