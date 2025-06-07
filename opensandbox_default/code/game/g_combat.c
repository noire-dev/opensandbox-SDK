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

#include "g_local.h"

/*
============
AddScore

Adds score to both the client and his team
============
*/
void AddScore( gentity_t *ent, int score ) {
        int i;

	if ( !ent->client || ent->npcType  ) {
		return;
	}

        //No scoring during intermission
        if ( level.intermissiontime ) {
            return;
        }
	// show score plum
    if( level.numNonSpectatorClients<3 && score < 0 && g_gametype.integer<GT_TEAM) {
        for ( i = 0 ; i < level.maxclients ; i++ ) {
            if ( level.clients[ i ].pers.connected != CON_CONNECTED )
                continue; //Client was not connected
            if (level.clients[i].sess.sessionTeam == TEAM_SPECTATOR)
                continue; //Don't give anything to spectators
            if (g_entities+i == ent)
                continue; //Don't award dead one
            level.clients[i].ps.persistant[PERS_SCORE] -= score;
        }
    } else {
		if(score == 1){
        ent->client->ps.persistant[PERS_SCORE] += score;
        if ( g_gametype.integer == GT_TEAM ) {
            int team = ent->client->ps.persistant[PERS_TEAM];
                level.teamScores[ team ] += score;
        }
		}
    }
	CalculateRanks();
}

/*
=================
TossClientItems

Toss the weapon and powerups for the killed player
=================
*/
void TossClientItems( gentity_t *self ) {
	item_t		*item;
	int			weapon;
	int			i;
	gentity_t	*drop;

	// drop the weapon if not a gauntlet or machinegun
	weapon = self->s.weapon;

	if(self->npcType >= 1){
        return;
	}

	// drop all weapons
	for ( i = 1 ; i < WEAPONS_NUM ; i++ ) {
		if ( self->swep_list[ i ] >= 1 ) {
			item = BG_FindItemForWeapon( i );
			if ( !item || i == WP_GAUNTLET )
				continue;
			drop = Drop_Item( self, item );
			drop->count = ( self->swep_ammo[ i ] );
			if ( drop->count < 1 ) {
				drop->count = 1;
			}
		}
	}

	// drop all powerups
	for ( i = 1 ; i < PW_NUM_POWERUPS ; i++ ) {
		if ( self->client->ps.powerups[ i ] > level.time ) {
			item = BG_FindItemForPowerup( i );
			if ( !item )
				continue;
			drop = Drop_Item( self, item );
			drop->count = ( self->client->ps.powerups[ i ] - level.time ) / 1000;
			if ( drop->count < 1 ) {
				drop->count = 1;
			}
		}
	}

	// drop holdable
	for ( i = 1 ; i < PW_NUM_POWERUPS ; i++ ) {
		if ( self->client->ps.powerups[ i ] > level.time ) {
			item = BG_FindItemForPowerup( i );
			if ( !item )
				continue;
			drop = Drop_Item( self, item );
			drop->count = ( self->client->ps.powerups[ i ] - level.time ) / 1000;
			if ( drop->count < 1 ) {
				drop->count = 1;
			}
		}
	}
}

/*
=================
TossClientCubes
=================
*/
extern gentity_t	*neutralObelisk;

void TossClientCubes( gentity_t *self ) {
	item_t		*item;
	gentity_t	*drop;
	vec3_t		velocity;
	vec3_t		angles;
	vec3_t		origin;

	self->client->ps.generic1 = 0;

	if( self->client->sess.sessionTeam == TEAM_RED ) {
		item = BG_FindItem( "Red Cube" );
	}
	else {
		item = BG_FindItem( "Blue Cube" );
	}

	angles[YAW] = (float)(level.time % 360);
	angles[PITCH] = 0;	// always forward
	angles[ROLL] = 0;

	AngleVectors( angles, velocity, NULL, NULL );
	VectorScale( velocity, 150, velocity );
	velocity[2] += 200 + crandom() * 50;

	if( neutralObelisk ) {
		VectorCopy( neutralObelisk->s.pos.trBase, origin );
		origin[2] += 44;
	} else {
		VectorClear( origin ) ;
	}

	drop = LaunchItem( item, origin, velocity );

	drop->nextthink = level.time + 300 * 1000;
	drop->think = G_FreeEntity;
	drop->spawnflags = self->client->sess.sessionTeam;
}

/*
=================
TossClientPersistantPowerups
=================
*/
void TossClientPersistantPowerups( gentity_t *ent ) {
	gentity_t	*powerup;

	if( !ent->client || !ent->client->persistantPowerup ) {
		return;
	}

	powerup = ent->client->persistantPowerup;

	powerup->r.svFlags &= ~SVF_NOCLIENT;
	powerup->s.eFlags &= ~EF_NODRAW;
	powerup->r.contents = CONTENTS_TRIGGER;
	trap_LinkEntity( powerup );

	ent->client->ps.stats[STAT_PERSISTANT_POWERUP] = 0;
	ent->client->persistantPowerup = NULL;
}

/*
==================
GibEntity
==================
*/
void GibEntity( gentity_t *self, int killer ) {
	gentity_t *ent;
	int i;

	//if this entity still has kamikaze
	if (self->s.eFlags & EF_KAMIKAZE) {
		// check if there is a kamikaze timer around for this owner
		for (i = 0; i < MAX_GENTITIES; i++) {
			ent = &g_entities[i];
			if (!ent->inuse)
				continue;
			if (ent->activator != self)
				continue;
			if (strcmp(ent->classname, "kamikaze timer"))
				continue;
			G_FreeEntity(ent);
			break;
		}
	}
	G_AddEvent( self, EV_GIB_PLAYER, killer );
	self->takedamage = qfalse;
	self->s.eType = ET_INVISIBLE;
	self->r.contents = 0;
}

/*
==================
body_die
==================
*/
void body_die( gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int meansOfDeath ) {
	if ( self->health > GIB_HEALTH ) {
		return;
	}
	if ( !g_blood.integer ) {
		self->health = GIB_HEALTH+1;
		return;
	}

	GibEntity( self, 0 );
}


// these are just for logging, the client prints its own messages
char	*modNames[] = {
	"MOD_UNKNOWN",
	"MOD_SHOTGUN",
	"MOD_GAUNTLET",
	"MOD_MACHINEGUN",
	"MOD_GRENADE",
	"MOD_GRENADE_SPLASH",
	"MOD_ROCKET",
	"MOD_ROCKET_SPLASH",
	"MOD_PLASMA",
	"MOD_PLASMA_SPLASH",
	"MOD_RAILGUN",
	"MOD_LIGHTNING",
	"MOD_BFG",
	"MOD_BFG_SPLASH",
	"MOD_FLAME",
	"MOD_FLAME_SPLASH",
	"MOD_ANTIMATTER",
	"MOD_ANTIMATTER_SPLASH",
	"MOD_TOOLGUN",
	"MOD_WATER",
	"MOD_SLIME",
	"MOD_LAVA",
	"MOD_CRUSH",
	"MOD_TELEFRAG",
	"MOD_FALLING",
	"MOD_SUICIDE",
	"MOD_TARGET_LASER",
	"MOD_TRIGGER_HURT",
	"MOD_NAIL",
	"MOD_CHAINGUN",
	"MOD_PROXIMITY_MINE",
	"MOD_KAMIKAZE",
	"MOD_JUICED",
	"MOD_GRAPPLE",
	"MOD_CAR",
	"MOD_CAREXPLODE",
	"MOD_PROP",
	"MOD_BREAKABLE_SPLASH"
};

/*
==================
Kamikaze_DeathActivate
==================
*/
void Kamikaze_DeathActivate( gentity_t *ent ) {
	G_StartKamikaze(ent);
	G_FreeEntity(ent);
}

/*
==================
Kamikaze_DeathTimer
==================
*/
void Kamikaze_DeathTimer( gentity_t *self ) {
	gentity_t *ent;

	ent = G_Spawn();
	ent->classname = "kamikaze timer";
	VectorCopy(self->s.pos.trBase, ent->s.pos.trBase);
	ent->r.svFlags |= SVF_NOCLIENT;
	ent->think = Kamikaze_DeathActivate;
	ent->nextthink = level.time + 50;

	ent->activator = self;
}

/*
==================
player_die
==================
*/
void player_die( gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int meansOfDeath ) {
	gentity_t	*ent;
	int			anim;
	int			contents;
	int			killer;
	int			i;
	char		*killerName, *obit;

	if ( self->client->ps.pm_type == PM_DEAD || level.intermissiontime ) {
		return;
	}

	if (self->client && self->client->hook) {
		Weapon_HookFree(self->client->hook);
	}
	if ((self->client->ps.eFlags & EF_TICKING) && self->activator) {
		self->client->ps.eFlags &= ~EF_TICKING;
		self->activator->think = G_FreeEntity;
		self->activator->nextthink = level.time;
	}
	self->client->ps.pm_type = PM_DEAD;

	if ( attacker ) {
		killer = attacker->s.number;
		if ( attacker->client ) {
			killerName = attacker->client->pers.netname;
		} else {
			killerName = "<non-client>";
		}
	} else {
		killer = ENTITYNUM_WORLD;
		killerName = "<world>";
	}

	if ( killer < 0 || killer >= MAX_CLIENTS ) {
		killer = ENTITYNUM_WORLD;
		killerName = "<world>";
	}

	if ( meansOfDeath < 0 || meansOfDeath >= sizeof( modNames ) / sizeof( modNames[0] ) ) {
		obit = "<bad obituary>";
	} else {
		obit = modNames[ meansOfDeath ];
	}

	// broadcast the death event to everyone
	ent = G_TempEntity( self->r.currentOrigin, EV_OBITUARY );
	ent->s.eventParm = meansOfDeath;
	ent->s.otherEntityNum = self->s.number;
	ent->s.otherEntityNum2 = killer;
	ent->r.svFlags = SVF_BROADCAST;	// send to everyone

	self->enemy = attacker;

	self->client->ps.persistant[PERS_KILLED]++;

	if (attacker && attacker->client) {
		if ( attacker == self || OnSameTeam (self, attacker ) ) {
			AddScore( attacker, -1 );
		} else {
			AddScore( attacker, 1 );
		}
	} else {
		AddScore( self, -1 );
	}

	// if client is in a nodrop area, don't drop anything (but return CTF flags!)
	contents = trap_PointContents( self->r.currentOrigin, -1 );
	if ( !( contents & CONTENTS_NODROP )) {
		TossClientItems( self );
	} else {
		if ( self->client->ps.powerups[PW_NEUTRALFLAG] ) {		// only happens in One Flag CTF
			Team_ReturnFlag( TEAM_FREE );
		}
		else if ( self->client->ps.powerups[PW_REDFLAG] ) {		// only happens in standard CTF
			Team_ReturnFlag( TEAM_RED );
		}
		else if ( self->client->ps.powerups[PW_BLUEFLAG] ) {	// only happens in standard CTF
			Team_ReturnFlag( TEAM_BLUE );
		}
	}

	TossClientPersistantPowerups( self );
	if( g_gametype.integer == GT_HARVESTER ) {
		TossClientCubes( self );
	}

	Cmd_Score_f( self );		// show scores
	// send updated scores to any clients that are following this one,
	// or they would get stale scoreboards
	for ( i = 0 ; i < level.maxclients ; i++ ) {
		gclient_t	*client;

		client = &level.clients[i];
		if ( client->pers.connected != CON_CONNECTED ) {
			continue;
		}
		if ( client->sess.sessionTeam != TEAM_SPECTATOR ) {
			continue;
		}
		if ( client->sess.spectatorClient == self->s.number ) {
			Cmd_Score_f( g_entities + i );
		}
	}

	self->takedamage = qtrue;	// can still be gibbed

	self->s.weapon = WP_NONE;
	self->s.powerups = 0;
	self->r.contents = CONTENTS_CORPSE;

	self->s.loopSound = 0;

	self->r.maxs[2] = -8;

	// don't allow respawn until the death anim is done
	// g_forcerespawn may force spawning at some later time
	self->client->respawnTime = level.time + 1700;

	// remove powerups
	memset( self->client->ps.powerups, 0, sizeof(self->client->ps.powerups) );

	// never gib in a nodrop
	if ( (self->health <= GIB_HEALTH && !(contents & CONTENTS_NODROP) && g_blood.integer) || meansOfDeath == MOD_SUICIDE) {
		// gib death
		GibEntity( self, killer );
	} else {
		// normal death
		static int i;

		switch ( i ) {
		case 0:
			anim = BOTH_DEATH1;
			break;
		case 1:
			anim = BOTH_DEATH2;
			break;
		case 2:
		default:
			anim = BOTH_DEATH3;
			break;
		}

		// for the no-blood option, we need to prevent the health
		// from going to gib level
		if ( self->health <= GIB_HEALTH ) {
			self->health = GIB_HEALTH+1;
		}

		self->client->ps.legsAnim = 
			( ( self->client->ps.legsAnim & ANIM_TOGGLEBIT ) ^ ANIM_TOGGLEBIT ) | anim;
		self->client->ps.torsoAnim = 
			( ( self->client->ps.torsoAnim & ANIM_TOGGLEBIT ) ^ ANIM_TOGGLEBIT ) | anim;

		G_AddEvent( self, EV_DEATH1 + i, killer );

		// the body can still be gibbed
		self->die = body_die;

		// globally cycle through the different death animations
		i = ( i + 1 ) % 3;

		if (self->s.eFlags & EF_KAMIKAZE) {
			Kamikaze_DeathTimer( self );
		}
	}

	trap_LinkEntity (self);
}


/*
================
CheckArmor
================
*/
int CheckArmor (gentity_t *ent, int damage, int dflags) {
	gclient_t	*client;
	int			save;
	int			count;

	if (!damage)
		return 0;

	client = ent->client;

	if (!client)
		return 0;

	if (dflags & DAMAGE_NO_ARMOR)
		return 0;

	// armor
	count = client->ps.stats[STAT_ARMOR];
	save = damage * 0.80;
	if (save >= count)
		save = count;

	if (!save)
		return 0;

	client->ps.stats[STAT_ARMOR] -= save;

	return save;
}

/*
================
RaySphereIntersections
================
*/
int RaySphereIntersections( vec3_t origin, float radius, vec3_t point, vec3_t dir, vec3_t intersections[2] ) {
	float b, c, d, t;

	//	| origin - (point + t * dir) | = radius
	//	a = dir[0]^2 + dir[1]^2 + dir[2]^2;
	//	b = 2 * (dir[0] * (point[0] - origin[0]) + dir[1] * (point[1] - origin[1]) + dir[2] * (point[2] - origin[2]));
	//	c = (point[0] - origin[0])^2 + (point[1] - origin[1])^2 + (point[2] - origin[2])^2 - radius^2;

	// normalize dir so a = 1
	VectorNormalize(dir);
	b = 2 * (dir[0] * (point[0] - origin[0]) + dir[1] * (point[1] - origin[1]) + dir[2] * (point[2] - origin[2]));
	c = (point[0] - origin[0]) * (point[0] - origin[0]) +
		(point[1] - origin[1]) * (point[1] - origin[1]) +
		(point[2] - origin[2]) * (point[2] - origin[2]) -
		radius * radius;

	d = b * b - 4 * c;
	if (d > 0) {
		t = (- b + sqrt(d)) / 2;
		VectorMA(point, t, dir, intersections[0]);
		t = (- b - sqrt(d)) / 2;
		VectorMA(point, t, dir, intersections[1]);
		return 2;
	}
	else if (d == 0) {
		t = (- b ) / 2;
		VectorMA(point, t, dir, intersections[0]);
		return 1;
	}
	return 0;
}

/*
================
G_InvulnerabilityEffect
================
*/
int G_InvulnerabilityEffect( gentity_t *targ, vec3_t dir, vec3_t point, vec3_t impactpoint, vec3_t bouncedir ) {
	gentity_t	*impact;
	vec3_t		intersections[2], vec;
	int			n;

	if ( !targ->client ) {
		return qfalse;
	}
	VectorCopy(dir, vec);
	VectorInverse(vec);
	// sphere model radius = 42 units
	n = RaySphereIntersections( targ->client->ps.origin, 42, point, vec, intersections);
	if (n > 0) {
		impact = G_TempEntity( targ->client->ps.origin, EV_INVUL_IMPACT );
		VectorSubtract(intersections[0], targ->client->ps.origin, vec);
		vectoangles(vec, impact->s.angles);
		impact->s.angles[0] += 90;
		if (impact->s.angles[0] > 360)
			impact->s.angles[0] -= 360;
		if ( impactpoint ) {
			VectorCopy( intersections[0], impactpoint );
		}
		if ( bouncedir ) {
			VectorCopy( vec, bouncedir );
			VectorNormalize( bouncedir );
		}
		return qtrue;
	}
	else {
		return qfalse;
	}
}

int engine10hook(int value, int src_min, int src_max) {
    return 10 * (value - src_min) / (src_max - src_min);
}

void VehiclePhys( gentity_t *self ) {
	if(!self->parent || !self || !self->parent->client->vehiclenum || self->parent->health <= 0 || self->health <= 0){
	self->think = 0;
	self->nextthink = 0;
	self->r.contents = CONTENTS_SOLID;
	self->sb_coll = CONTENTS_SOLID;
	self->s.pos.trType = TR_GRAVITY;
	self->s.pos.trTime = level.time;
	ClientUserinfoChanged( self->parent->s.clientNum );
	VectorSet( self->parent->r.mins, -15, -15, -24 );
	VectorSet( self->parent->r.maxs, 15, 15, 32 );
	VectorSet( self->parent->client->ps.origin, self->r.currentOrigin[0], self->r.currentOrigin[1], self->r.currentOrigin[2] + 40);
	self->parent->client->vehiclenum = 0;
	self->s.legsAnim = 0;
	self->s.generic1 = 0; 		//smooth vehicles
	self->parent->client->ps.gravity = g_gravity.value;
	return;
	}
	
	self->s.pos.trType = TR_STATIONARY;
	self->sb_phys = PHYS_STATIC;
	
	self->r.contents = CONTENTS_TRIGGER;
	self->sb_coll = CONTENTS_TRIGGER;

	trap_UnlinkEntity( self );
	
	VectorCopy(self->parent->s.origin, self->s.origin);
	VectorCopy(self->parent->s.pos.trBase, self->s.pos.trBase);
	if (VectorLength(self->parent->client->ps.velocity) > 5) {
	self->s.apos.trBase[1] = self->parent->s.apos.trBase[1];
	}
	if(engine10hook(sqrt(self->parent->client->ps.velocity[0] * self->parent->client->ps.velocity[0] + self->parent->client->ps.velocity[1] * self->parent->client->ps.velocity[1]), 0, 900) <= 10){ //900 is car speed
	self->s.legsAnim = engine10hook(sqrt(self->parent->client->ps.velocity[0] * self->parent->client->ps.velocity[0] + self->parent->client->ps.velocity[1] * self->parent->client->ps.velocity[1]), 0, 900); //900 is car speed
	}
	VectorCopy(self->parent->r.currentOrigin, self->r.currentOrigin);
	self->parent->client->ps.stats[STAT_VEHICLEHP] = self->health; //VEHICLE-SYSTEM: vehicle's hp instead player
	self->s.generic1 = self->parent->s.clientNum+1; 		//smooth vehicles
	
	trap_LinkEntity( self );
	
	self->think = VehiclePhys;
	self->nextthink = level.time + 1;
}

qboolean G_EnterInCar(gentity_t *player, gentity_t *vehicle) {
    // Validate conditions
    if (!player->client || !vehicle->vehicle) {
        return qtrue;
    }

    // Player is already in another vehicle
    if (player->client->vehiclenum) {
        return qtrue;
    }

    // Assign vehicle to player
    player->client->vehiclenum = vehicle->s.number;
    vehicle->parent = player;

    // Update player properties
    ClientUserinfoChanged(player->s.clientNum);
    
    // Position synchronization
    VectorCopy(vehicle->s.origin, player->s.origin);
    VectorCopy(vehicle->s.pos.trBase, player->s.pos.trBase);
    player->s.apos.trBase[1] = vehicle->s.apos.trBase[1]; // Only copy yaw
    VectorCopy(vehicle->r.currentOrigin, player->r.currentOrigin);
    
    // Adjust player collision bounds
    VectorSet(player->r.mins, -25, -25, -15);
    VectorSet(player->r.maxs, 25, 25, 15);

    // Activate vehicle physics
    vehicle->think = VehiclePhys;
    vehicle->nextthink = level.time + 1;

    return qtrue;
}

void G_ExitVehicle (int num){
	Cmd_VehicleExit_f( G_FindEntityForClientNum(num));
}

void G_Damage( gentity_t *targ, gentity_t *inflictor, gentity_t *attacker,
			   vec3_t dir, vec3_t point, int damage, int dflags, int mod ) {
	gclient_t	*client;
	int			take;
	int			save;
	int			asave;
	int			knockback;
	int			max;
	vec3_t		bouncedir, impactpoint;
	gentity_t 	*act;

	if (!G_NpcFactionProp(NP_HARM, attacker) && attacker->npcType >= 1 && targ->npcType == attacker->npcType)
		return;

	if(mod == MOD_TOOLGUN){
		return;
	}

	if (!targ->takedamage && !targ->sandboxObject) {
		return;
	}

	if(mod == MOD_GAUNTLET && attacker->health <= 0){
		return;
	}

	if (mod == MOD_GAUNTLET) {
		if (targ->vehicle) {
			if(G_EnterInCar(attacker, targ)){
				return;
			}
		}
	}

	//damage car
	if (targ->client && targ->client->vehiclenum){ //VEHICLE-SYSTEM: damage vehicle instead player
 		targ = G_FindEntityForEntityNum(targ->client->vehiclenum);
	}

	if(mod == MOD_REGENERATOR && targ->client){
		if (!targ->client->ps.powerups[PW_REGEN]) {
			targ->client->ps.powerups[PW_REGEN] = level.time - ( level.time % 1000 );
		}
		targ->client->ps.powerups[PW_REGEN] += 5 * 1000;
	}

	// the intermission has allready been qualified for, so don't
	// allow any extra scoring
	if ( level.intermissionQueued ) {
		return;
	}
	if ( targ->client && mod != MOD_JUICED) {
		if ( targ->client->invulnerabilityTime > level.time) {
			if ( dir && point ) {
				G_InvulnerabilityEffect( targ, dir, point, impactpoint, bouncedir );
			}
			return;
		}
	}
	if ( !inflictor ) {
		inflictor = &g_entities[ENTITYNUM_WORLD];
	}
	if ( !attacker ) {
		attacker = &g_entities[ENTITYNUM_WORLD];
	}

	// shootable doors / buttons don't actually have any health
	if ( targ->s.eType == ET_MOVER ) {
		if ( targ->use && targ->moverState == MOVER_POS1 ) {
			targ->use( targ, inflictor, attacker );
		}
		return;
	}
	if( g_gametype.integer == GT_OBELISK && CheckObeliskAttack( targ, attacker ) ) {
		return;
	}
	// reduce damage by the attacker's handicap value
	// unless they are rocket jumping
	if ( attacker->client && attacker != targ ) {
		max = attacker->client->ps.stats[STAT_MAX_HEALTH];
		if( gameInfoItems[attacker->client->ps.stats[STAT_PERSISTANT_POWERUP]].giTag == PW_GUARD ) {
			max /= 2;
		}
		damage = damage * max / 100;
	}

	client = targ->client;

	if ( client ) {
		if ( client->noclip ) {
			return;
		}
	}

	if ( !dir ) {
		dflags |= DAMAGE_NO_KNOCKBACK;
	} else {
		VectorNormalize(dir);
	}

	knockback = damage;

	if ( mod == MOD_NUKE )
	knockback *= 3.00;
	if ( mod == MOD_KNOCKER )
	knockback *= 200;

	if ( targ->flags & FL_NO_KNOCKBACK ) {
		knockback = 0;
	}
	if ( dflags & DAMAGE_NO_KNOCKBACK ) {
		knockback = 0;
	}

	// figure momentum add, even if the damage won't be taken
	if ( knockback && targ->client || knockback && targ->sandboxObject ) {
		vec3_t	kvel;
		float	mass;

		mass = 200;

		if(targ->sandboxObject){
			VectorScale (dir, g_knockback.value*3 * (float)knockback / mass, kvel);
		} else {
			VectorScale (dir, g_knockback.value * (float)knockback / mass, kvel);
		}
		if(targ->client){
			VectorAdd (targ->client->ps.velocity, kvel, targ->client->ps.velocity);
		}
		if(targ->sandboxObject){ //WELD-TOOL
			if(!targ->phys_parent){
				Phys_Enable( targ );
				targ->lastPlayer = attacker;
				VectorAdd (targ->s.pos.trDelta, kvel, targ->s.pos.trDelta);
			} else {
				Phys_Enable( targ->phys_parent );
				targ->phys_parent->lastPlayer = attacker;
				if (targ->phys_parent->phys_weldedObjectsNum > 0) {		//mass
					VectorScale(kvel, 1.0f / targ->phys_parent->phys_weldedObjectsNum, kvel);
				}
				VectorAdd (targ->phys_parent->s.pos.trDelta, kvel, targ->phys_parent->s.pos.trDelta);
			}
		}

		// set the timer so that the other client can't cancel
		// out the movement immediately
		if(!targ->sandboxObject){
			if ( !targ->client->ps.pm_time ) {
				int		t;
			
				t = knockback * 2;
				if ( t < 50 ) {
					t = 50;
				}
				if ( t > 200 ) {
					t = 200;
				}
				targ->client->ps.pm_time = t;
				targ->client->ps.pm_flags |= PMF_TIME_KNOCKBACK;
			}
		}
	}

	if (targ->sandboxObject && targ->health == -1) {
		return;
	}

	if(targ->botskill == 7){
		return;
	}

	// check for completely getting out of the damage
	if ( !(dflags & DAMAGE_NO_PROTECTION) ) {
		// if TF_NO_FRIENDLY_FIRE is set, don't do damage to the target
		// if the attacker was on the same team
		if ( mod != MOD_JUICED && targ != attacker && !(dflags & DAMAGE_NO_TEAM_PROTECTION) && OnSameTeam (targ, attacker)  ) {
			if ( !g_friendlyFire.integer ) {
				return;
			}
		}
		if (mod == MOD_PROXIMITY_MINE) {
			if (inflictor && inflictor->parent && OnSameTeam(targ, inflictor->parent)) {
				return;
			}
			if (targ == attacker) {
				return;
			}
		}

		// check for godmode
		if ( targ->flags & FL_GODMODE ) {
			return;
		}
	}

	// battlesuit protects from all radius damage (but takes knockback)
	// and protects 50% against all damage
	if ( client && client->ps.powerups[PW_BATTLESUIT] ) {
		if ( ( dflags & DAMAGE_RADIUS ) || ( mod == MOD_FALLING ) ) {
			return;
		}
		damage *= 0.5;
	}

	// add to the attacker's hit counter (if the target isn't a general entity like a prox mine)
	if ( attacker->client && targ != attacker && targ->health > 0
			&& targ->s.eType != ET_MISSILE
			&& targ->s.eType != ET_GENERAL) {
		if ( OnSameTeam( targ, attacker ) ) {
			attacker->client->ps.persistant[PERS_HITS]--;
		} else {
			attacker->client->ps.persistant[PERS_HITS]++;
		}
		attacker->client->ps.persistant[PERS_ATTACKEE_ARMOR] = (targ->health<<8)|(client->ps.stats[STAT_ARMOR]);
	}

	// always give half damage if hurting self
	// calculated after knockback, so rocket jumping works
	if ( targ == attacker) {
		damage *= 0.20;
	}

	if ( damage < 1 ) {
		damage = 1;
	}
	take = damage;
	save = 0;

	// save some from armor
	asave = CheckArmor (targ, take, dflags);
	take -= asave;

	// add to the damage inflicted on a player this frame
	// the total will be turned into screen blends and view angle kicks
	// at the end of the frame
	if ( client ) {
		client->damage_armor += asave;
		client->damage_blood += take;
		client->damage_knockback += knockback;
		if ( dir ) {
			VectorCopy ( dir, client->damage_from );
			client->damage_fromWorld = qfalse;
		} else {
			VectorCopy ( targ->r.currentOrigin, client->damage_from );
			client->damage_fromWorld = qtrue;
		}
	}

	if (targ->client) {
		// set the last client who damaged the target
		targ->client->lasthurt_client = attacker->s.number;
		targ->client->lasthurt_mod = mod;
	}

	// do the damage
	if (take) {
		targ->health = targ->health - take;
		if ( targ->client ) {
			targ->client->ps.stats[STAT_HEALTH] = targ->health;
		}
			
		if ( targ->health <= 0 ) {
			if ( client )
				targ->flags |= FL_NO_KNOCKBACK;

			if (targ->health < -999)
				targ->health = -999;

			targ->enemy = attacker;
			targ->die (targ, inflictor, attacker, take, mod);
			return;
		} else if ( targ->pain ) {
			targ->pain (targ, attacker, take);
		}
	}
}

/*
============
CanDamage

Returns qtrue if the inflictor can directly damage the target.  Used for
explosions and melee attacks.
============
*/
qboolean CanDamage (gentity_t *targ, vec3_t origin) {
	vec3_t	dest;
	trace_t	tr;
	vec3_t	midpoint;

	// use the midpoint of the bounds instead of the origin, because
	// bmodels may have their origin is 0,0,0
	VectorAdd (targ->r.absmin, targ->r.absmax, midpoint);
	VectorScale (midpoint, 0.5, midpoint);

	VectorCopy (midpoint, dest);
	trap_Trace ( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
	if (tr.fraction == 1.0 || tr.entityNum == targ->s.number)
		return qtrue;

	// this should probably check in the plane of projection, 
	// rather than in world coordinate, and also include Z
	VectorCopy (midpoint, dest);
	dest[0] += 15.0;
	dest[1] += 15.0;
	trap_Trace ( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
	if (tr.fraction == 1.0)
		return qtrue;

	VectorCopy (midpoint, dest);
	dest[0] += 15.0;
	dest[1] -= 15.0;
	trap_Trace ( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
	if (tr.fraction == 1.0)
		return qtrue;

	VectorCopy (midpoint, dest);
	dest[0] -= 15.0;
	dest[1] += 15.0;
	trap_Trace ( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
	if (tr.fraction == 1.0)
		return qtrue;

	VectorCopy (midpoint, dest);
	dest[0] -= 15.0;
	dest[1] -= 15.0;
	trap_Trace ( &tr, origin, vec3_origin, vec3_origin, dest, ENTITYNUM_NONE, MASK_SOLID);
	if (tr.fraction == 1.0)
		return qtrue;


	return qfalse;
}

/*
============
G_RadiusDamage
============
*/
void G_RadiusDamage ( vec3_t origin, gentity_t *attacker, float damage, float radius,
					 gentity_t *ignore, int mod) {
	float		points, dist;
	gentity_t	*ent;
	int			numListedEntities;
	vec3_t		mins, maxs;
	vec3_t		v;
	vec3_t		dir;
	int			i, e;

	if ( radius < 1 ) {
		radius = 1;
	}

	for ( i = 0 ; i < 3 ; i++ ) {
		mins[i] = origin[i] - radius;
		maxs[i] = origin[i] + radius;
	}

	numListedEntities = trap_EntitiesInBox( mins, maxs, SourceTechEntityList, MAX_GENTITIES );

	for ( e = 0 ; e < numListedEntities ; e++ ) {
		ent = &g_entities[SourceTechEntityList[ e ]];

		if (ent == ignore)
			continue;
		if (!ent->takedamage)
			continue;

		// find the distance from the edge of the bounding box
		for ( i = 0 ; i < 3 ; i++ ) {
			if ( origin[i] < ent->r.absmin[i] ) {
				v[i] = ent->r.absmin[i] - origin[i];
			} else if ( origin[i] > ent->r.absmax[i] ) {
				v[i] = origin[i] - ent->r.absmax[i];
			} else {
				v[i] = 0;
			}
		}

		dist = VectorLength( v );
		if ( dist >= radius ) {
			continue;
		}

		points = damage * ( 1.0 - dist / radius );

		if( CanDamage (ent, origin) ) {
			VectorSubtract (ent->r.currentOrigin, origin, dir);
			// push the center of mass higher than the origin so players
			// get knocked into the air more
			dir[2] += 24;
			G_Damage (ent, NULL, attacker, dir, origin, (int)points, DAMAGE_RADIUS, mod);
		}
	}
}
