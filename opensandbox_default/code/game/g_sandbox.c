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
// g_sandbox.c

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

void G_DieProp (gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod) {
	if(self->vehicle || self->objectType == OT_TNT){ //VEHICLE-SYSTEM: vehicle's explode for all
	G_StartCarExplode(self);
	}
	if(self->objectType == OT_NUKE){
	G_StartNukeExplode(self);
	}
	G_FreeEntity( self );
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
	VectorCopy( ent->s.origin, ent->s.pos.trBase );		//Client
	VectorCopy( ent->s.origin, ent->r.currentOrigin );	//Physics

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
	if(ent->sb_phys == PHYS_STATIC){ ent->s.pos.trType = TR_STATIONARY; }
	if(ent->sb_phys == PHYS_DYNAMIC){ ent->s.pos.trType = TR_GRAVITY; ent->s.pos.trTime = level.time; }

	//Collision
	ent->r.contents = ent->sb_coll;	

	//Material
	ent->s.generic2 = ent->sb_material;

	//Mass
	ent->s.origin2[O2_MASS] = ent->sb_gravity;

	//Type
	ent->s.torsoAnim = ent->objectType;

	//Prop class for saving
	ent->classname = "func_prop";

	if(!spawn_entity){
		//Type
		ent->s.eType = ET_GENERAL;

		//Die function
		ent->die = G_DieProp;

		//Touch function
		ent->touch = G_TouchProp;
	}

	//Angles
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
		trap_SendServerCommand( player->s.clientNum, "cllp \"Spawning of this class is not allowed\n\"" );
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
		ent->s.pos.trType = TR_STATIONARY; ent->s.pos.trTime = level.time; ent->physicsBounce = atof(arg22); ent->sb_phys = PHYS_STATIC;
		}
		if(atoi(arg09) == 1){
		ent->s.pos.trType = TR_GRAVITY; ent->s.pos.trTime = level.time; ent->physicsBounce = atof(arg22); ent->sb_phys = PHYS_DYNAMIC;
		}

		//Collision
		ent->r.contents = atoi(arg10);
		ent->sb_coll = atoi(arg10);

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
		ent->sb_gravity = atof(arg23);
		ent->s.origin2[O2_MASS] = atof(arg23);
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
		ent->die = G_DieProp;	

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
	if(!G_PlayerIsOwner(attacker, targ)) return;

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
		targ->s.pos.trType = TR_STATIONARY; targ->sb_phys = PHYS_STATIC;
		}
		if(atoi(arg19) == 1){
		targ->s.pos.trType = TR_GRAVITY; targ->s.pos.trTime = level.time; targ->sb_phys = PHYS_DYNAMIC;
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
		trap_SendServerCommand( attacker->s.clientNum, va( "cllp \"Owned by %s\n\"", targ->ownername ));
		} 
		if(!targ->ownername){
		trap_SendServerCommand( attacker->s.clientNum, "cllp \"Not owned\n\"" );
		} 
		}
	}

	if(attacker->tool_id == TL_COLLISION){
		if(atoi(arg19) == 0){
		targ->r.contents = CONTENTS_SOLID;
		targ->sb_coll = CONTENTS_SOLID;
		}
		if(atoi(arg19) == 1){
		targ->r.contents = CONTENTS_TRIGGER;
		targ->sb_coll = CONTENTS_TRIGGER;
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
	VectorCopy(targ->s.apos.trBase, targ->s.angles);
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
			trap_SendServerCommand( attacker->s.clientNum, "cllp \"This must be the item\n\"" );
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

	if(attacker->tool_id == TL_WELD){
		if(!attacker->tool_entity){
			if(!targ->phys_hasWeldedObjects){
				attacker->tool_entity = targ;
				trap_SendServerCommand( attacker->s.clientNum, "cllp \"Сlick on the second object\n\"" );
			} else {
				trap_SendServerCommand( attacker->s.clientNum, "cllp \"This is root object\n\"" );
				return;
			}
		} else {
			if(!targ->phys_parent){
				attacker->tool_entity->phys_parent = targ;
				VectorSubtract(attacker->tool_entity->r.currentOrigin, targ->r.currentOrigin, attacker->tool_entity->phys_relativeOrigin);
				VectorSubtract(attacker->tool_entity->s.apos.trBase, targ->s.apos.trBase, attacker->tool_entity->phys_relativeAngles);
			} else {
				attacker->tool_entity->phys_parent = targ->phys_parent;
				VectorSubtract(attacker->tool_entity->r.currentOrigin, targ->phys_parent->r.currentOrigin, attacker->tool_entity->phys_relativeOrigin);
				VectorSubtract(attacker->tool_entity->s.apos.trBase, targ->phys_parent->s.apos.trBase, attacker->tool_entity->phys_relativeAngles);
			}
			targ->phys_hasWeldedObjects = qtrue;
			attacker->tool_entity = NULL;
		}
	}

}
