// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

#include "g_local.h"

static char* sandbox_class_allowed[] = { //classes allowed in Sandbox
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
	"item_armor_combat",
	"item_armor_body",
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

	"sb.shooter",
	0
};

static char* editor_class_allowed[] = { //classes allowed in Map Editor
	0
};

static char* standard_class_spawn[] = { //classes spawned without sandbox settings
	0
};

static void SB_Shooter_Use(gentity_t *ent, gentity_t *other, gentity_t *activator) {
	vec3_t dir;
	vec3_t up, right;

	if(!ent->r.linked) return;

	G_SetMovedir(ent->s.angles, ent->movedir);
	VectorCopy(ent->movedir, dir);

	fire_missile(ent, ent->s.origin, dir, right, up, ent->s.weapon);

	G_AddEvent(ent, EV_FIRE_WEAPON, 0);
}

static void SB_Shooter(gentity_t *ent) {
	ent->use = SB_Shooter_Use;
	ent->s.weapon = ent->spawnflags;

	if(!ent->spawnflags) {
		G_FreeEntity(ent);
		return;
	}

	trap_LinkEntity(ent);
}

static spawn_t gameInfoSandboxEntities[] = {
	{"sb.shooter", 				SB_Shooter},

	{NULL, 0}
};

void G_DieProp(gentity_t *self, gentity_t *inflictor, gentity_t *attacker, int damage, int mod) {
	if(self->vehicle || self->objectType == OT_TNT) {  // VEHICLE-SYSTEM: vehicle's explode for all
		G_StartCarExplode(self);
	}
	if(self->objectType == OT_NUKE) {
		G_StartNukeExplode(self);
	}
	G_FreeEntity(self);
}

static void G_TouchProp(gentity_t *self, gentity_t *other, trace_t *trace) {
	if(self->objectType == OT_VEHICLE) {
		if(!other->health || !other->client || !other->npcType || other->client->vehicleNum) {
			return;
		}
		if(self->parent && self->parent->client->vehicleNum) {
			return;
		}
		other->client->vehicleNum = self->s.number;
		self->parent = other;
		ClientUserinfoChanged(other->s.clientNum);
		VectorCopy(self->s.origin, other->s.origin);
		VectorCopy(self->s.pos.trBase, other->s.pos.trBase);
		other->s.apos.trBase[1] = self->s.apos.trBase[1];
		VectorCopy(self->r.currentOrigin, other->r.currentOrigin);
		VectorSet(other->r.mins, -25, -25, -15);
		VectorSet(other->r.maxs, 25, 25, 15);
		self->think = Phys_VehiclePlayer;
		self->nextthink = level.time + 1;
	}
}

static void setModel(gentity_t *ent, char *modelName) {
	int len;
	if(!ent || !modelName) {
		Com_Printf("Invalid entity or model name\n");
		return;
	}

	len = strlen(modelName);
	if(len == 0) {
		Com_Printf("Model name is empty\n");
		return;
	}

	ent->s.modelindex = G_ModelIndex(modelName);
	CopyAlloc(ent->model, modelName);

	if(len >= 4 && !Q_stricmp(ent->model + len - 4, ".md3")) {
		Com_Printf("Mesh load: '%s'\n", ent->model);
		ent->model[len - 4] = '\0';          // Убираем расширение
		memset(ent->model + len - 3, 0, 4);  // Удаляем остатки
	}

	if(FS_FileExists(va("%s.bsp", ent->model))) {           // Проверка модели коллизии
		trap_SetBrushModel(ent, va("%s.bsp", ent->model));  // Загружаем если есть
		ent->s.modelindex2 = G_ModelIndex(modelName);       // Коллизия созраняется в первый слот а модель теперь в втором слоте
	}
}

void SP_sandbox_npc(gentity_t *ent) { G_AddBot(ent->clientname, ent->skill, "Blue", ent->message, ent); }

void SP_sandbox_prop(gentity_t *ent) {
	spawn_t *s;
	qboolean spawn_entity = qfalse;

	// Create entity
	CopyAlloc(ent->classname, ent->sb_class);

	// Origin
	VectorCopy(ent->s.origin, ent->s.pos.trBase);     // Client
	VectorCopy(ent->s.origin, ent->r.currentOrigin);  // Physics

	// Type
	ent->sandboxObject = OBJ_SANDBOX;

	// Classic entity spawn
	for(s = gameInfoEntities; s->name; s++) {
		if(!strcmp(s->name, ent->classname)) {
			s->spawn(ent);

			spawn_entity = qtrue;
		}
	}

	// Sandbox entity spawn
	for(s = gameInfoSandboxEntities; s->name; s++) {
		if(!strcmp(s->name, ent->classname)) {
			s->spawn(ent);

			spawn_entity = qtrue;
		}
	}

	// Light
	ent->s.constantLight = ent->sb_red | (ent->sb_green << 8) | (ent->sb_blue << 16) | (ent->sb_radius << 24);

	// Sound
	ent->s.loopSound = G_SoundIndex(ent->sb_sound);

	// Setting collision
	if(ent->vehicle <= 0 || spawn_entity) {
		VectorSet(ent->r.mins, -ent->sb_coltype * ent->s.scales[0], -ent->sb_coltype * ent->s.scales[1], -ent->sb_coltype * ent->s.scales[2]);
		VectorSet(ent->r.maxs, ent->sb_coltype * ent->s.scales[0], ent->sb_coltype * ent->s.scales[1], ent->sb_coltype * ent->s.scales[2]);
	} else {
		VectorSet(ent->r.mins, -25, -25, -15);
		VectorSet(ent->r.maxs, 25, 25, 15);
	}

	// Phys frame
	ent->s.pos.trTime = level.time;

	// Damage
	ent->takedamage = qtrue;

	// Physics
	if(ent->sb_phys == PHYS_STATIC) {
		ent->s.pos.trType = TR_STATIONARY;
	}
	if(ent->sb_phys == PHYS_DYNAMIC) {
		ent->s.pos.trType = TR_GRAVITY;
		ent->s.pos.trTime = level.time;
	}

	// Collision
	ent->r.contents = ent->sb_coll;

	// Material
	ent->s.generic2 = ent->sb_material;

	// Mass
	ent->s.angles2[A2_MASS] = ent->sb_gravity;

	// Type
	ent->s.torsoAnim = ent->objectType;

	// Prop class for saving
	ent->classname = "sandbox_prop";

	if(!spawn_entity) {
		// Type
		ent->s.eType = ET_GENERAL;

		// Die function
		ent->die = G_DieProp;

		// Touch function
		ent->touch = G_TouchProp;
	}

	// Angles
	VectorCopy(ent->s.angles, ent->s.apos.trBase);

	// Load model
	setModel(ent, ent->model);

	// Link
	trap_LinkEntity(ent);
}

void G_BuildProp(char *arg02, char *arg03, vec3_t xyz, gentity_t *player, char *arg04, char *arg05, char *arg06, char *arg07, char *arg08, char *arg09, char *arg10, char *arg11, char *arg12, char *arg13, char *arg14, char *arg15, char *arg16, char *arg17, char *arg18, char *arg19, char *arg20, char *arg21, char *arg22) {
	gentity_t *ent;
	vec3_t position;
	spawn_t *s;
	item_t *item;
	qboolean spawn_entity = qfalse;
	int i;
	qboolean extended_spawn = qtrue;
	qboolean allow_spawn = qfalse;

	position[0] = ((int)((xyz[0] + (xyz[0] < 0 ? -atoi(arg06) : atoi(arg06))) / (atoi(arg06) * 2)) * (atoi(arg06) * 2));
	position[1] = ((int)((xyz[1] + (xyz[1] < 0 ? -atoi(arg06) : atoi(arg06))) / (atoi(arg06) * 2)) * (atoi(arg06) * 2));
	if(atoi(arg09) <= 0) {
		position[2] = ((int)((xyz[2] + (xyz[2] < 0 ? -atoi(arg06) : atoi(arg06))) / (atoi(arg06) * 2)) * (atoi(arg06) * 2));
	} else {
		position[2] = xyz[2] + atof(arg05);
	}

	// Create entity
	ent = G_Spawn();
	CopyAlloc(ent->classname, arg03);
	CopyAlloc(ent->sb_class, arg03);
	for(i = 0; standard_class_spawn[i] != 0; i++) {  // Classlist for standard spawn
		if(!strcmp(ent->classname, standard_class_spawn[i])) {
			extended_spawn = qfalse;
		}
	}
	for(i = 0; sandbox_class_allowed[i] != 0; i++) {  // Check allowed sandbox list
		if(!strcmp(ent->classname, sandbox_class_allowed[i])) {
			allow_spawn = qtrue;
		}
	}
	if(g_gametype.integer == GT_MAPEDITOR) {
		for(i = 0; editor_class_allowed[i] != 0; i++) {  // Check allowed editor list
			if(!strcmp(ent->classname, editor_class_allowed[i])) {
				allow_spawn = qtrue;
			}
		}
	}

	if(!allow_spawn) {
		G_FreeEntity(ent);
		trap_SendServerCommand(player->s.clientNum, "cllp \"Spawning of this class is not allowed\n\"");
		return;
	}

	Undo_AddElement(player, ent->s.number, UNDO_PROPSPAWN);

	// Origin
	VectorCopy(position, ent->s.origin);
	VectorCopy(position, ent->s.pos.trBase);
	VectorCopy(position, ent->r.currentOrigin);

	// Basic
	ent->sandboxObject = OBJ_SANDBOX;
	ent->spawnflags = atoi(arg07);

	if(extended_spawn) {
		// Damage
		ent->takedamage = qtrue;

		// Owner
		if(atoi(arg04) == 1) {
			ent->owner = player;
		}

		// Material
		ent->s.generic2 = atoi(arg08);
		ent->sb_material = atoi(arg08);

		// Physics
		if(atoi(arg09) == 0) {
			ent->s.pos.trType = TR_STATIONARY;
			ent->s.pos.trTime = level.time;
			ent->physicsBounce = atof(arg21);
			ent->sb_phys = PHYS_STATIC;
		}
		if(atoi(arg09) == 1) {
			ent->s.pos.trType = TR_GRAVITY;
			ent->s.pos.trTime = level.time;
			ent->physicsBounce = atof(arg21);
			ent->sb_phys = PHYS_DYNAMIC;
		}

		// Collision
		ent->r.contents = atoi(arg10);
		ent->sb_coll = atoi(arg10);

		// Sound
		ent->s.loopSound = G_SoundIndex(arg11);
		CopyAlloc(ent->sb_sound, arg11);

		// HP
		ent->health = atoi(arg12);

		// Light
		ent->s.constantLight = atoi(arg13) | (atoi(arg14) << 8) | (atoi(arg15) << 16) | (atoi(arg16) << 24);
		ent->sb_red = atoi(arg13);
		ent->sb_green = atoi(arg14);
		ent->sb_blue = atoi(arg15);
		ent->sb_radius = atoi(arg16);

		// Scale
		ent->s.scales[0] = atof(arg17);
		ent->s.scales[1] = atof(arg18);
		ent->s.scales[2] = atof(arg19);

		// Type
		ent->objectType = atoi(arg20);
		ent->s.torsoAnim = atoi(arg20);
		if(atoi(arg20) == OT_VEHICLE) {
			ent->vehicle = 1;
		} else {
			ent->vehicle = 0;
		}

		// Mass
		ent->sb_gravity = atof(arg22);
		ent->s.angles2[A2_MASS] = atof(arg22);
	}

	// Item spawn
	for(item = gameInfoItems + 1; item->classname; item++) {
		if(!strcmp(item->classname, ent->classname)) {
			position[2] += 48;
			VectorCopy(position, ent->s.origin);
			VectorCopy(position, ent->s.pos.trBase);
			VectorCopy(position, ent->r.currentOrigin);
			G_SpawnItem(ent, item);
			return;
		}
	}

	// Classic entity spawn
	for(s = gameInfoEntities; s->name; s++) {
		if(!strcmp(s->name, ent->classname)) {
			s->spawn(ent);

			spawn_entity = qtrue;

			if(!extended_spawn) {  // Standard spawn
				return;
			}
		}
	}

	// Sandbox entity spawn
	for(s = gameInfoSandboxEntities; s->name; s++) {
		if(!strcmp(s->name, ent->classname)) {
			s->spawn(ent);

			spawn_entity = qtrue;

			if(!extended_spawn) {  // Standard spawn
				return;
			}
		}
	}

	// Prop class for saving
	ent->classname = "sandbox_prop";

	if(!spawn_entity) {
		// Type
		ent->s.eType = ET_GENERAL;

		// Die function
		ent->die = G_DieProp;

		// Touch function
		ent->touch = G_TouchProp;
	}

	// Setting collsion
	if(atoi(arg21) <= 0 || spawn_entity) {
		ent->sb_coltype = atof(arg05);
		VectorSet(ent->r.mins, -ent->sb_coltype * ent->s.scales[0], -ent->sb_coltype * ent->s.scales[1], -ent->sb_coltype * ent->s.scales[2]);
		VectorSet(ent->r.maxs, ent->sb_coltype * ent->s.scales[0], ent->sb_coltype * ent->s.scales[1], ent->sb_coltype * ent->s.scales[2]);
	} else {
		ent->sb_coltype = atof(arg05);
		VectorSet(ent->r.mins, -25, -25, -15);
		VectorSet(ent->r.maxs, 25, 25, 15);
	}

	// Load model
	setModel(ent, arg02);

	// Link
	trap_LinkEntity(ent);
}

void G_ModProp(gentity_t *targ, gentity_t *attacker, char *arg01, char *arg02, char *arg03, char *arg04, char *arg05, char *arg06, char *arg07, char *arg08, char *arg09, char *arg10, char *arg11, char *arg12, char *arg13, char *arg14, char *arg15, char *arg16, char *arg17, char *arg18, char *arg19) {  // tool_id
	gentity_t *entity;

	entity = targ;
	if(g_gametype.integer != GT_SANDBOX && g_gametype.integer != GT_MAPEDITOR) {
		return;
	}

	if(entity->client && !entity->npcType) {
		return;
	}

	if(!G_PlayerIsOwner(attacker, entity)) return;

	if(attacker->tool_id == TL_CREATE) {
		// client-side command for spawn prop
	}

	if(attacker->tool_id == TL_MATERIAL) {
		entity->s.generic2 = atoi(arg01);
		entity->sb_material = atoi(arg01);
	}

	if(attacker->tool_id == TL_DELETE) {
		if(!entity->npcType) {
			G_FreeEntity(entity);
		} else {
			DropClientSilently(entity->client->ps.clientNum);
		}
	}

	if(attacker->tool_id == TL_MODEL) {
		setModel(entity, arg01);
	}

	if(attacker->tool_id == TL_PHYSICS) {
		if(atoi(arg19) == 0) {
			entity->s.pos.trType = TR_STATIONARY;
			entity->sb_phys = PHYS_STATIC;
			Phys_Disable(entity, entity->s.pos.trBase);
		}
		if(atoi(arg19) == 1) {
			entity->s.pos.trType = TR_GRAVITY;
			entity->s.pos.trTime = level.time;
			entity->sb_phys = PHYS_DYNAMIC;
			Phys_Enable(entity);
		}
	}

	if(attacker->tool_id == TL_PRIVATE) {
		if(atoi(arg19) == 0) {
			entity->owner = 0;
		}
		if(atoi(arg19) == 1) {
			entity->owner = attacker;
		}
		if(atoi(arg19) == 2) {
			if(entity->owner) {
				trap_SendServerCommand(attacker->s.clientNum, va("cllp \"Owned by %s\n\"", entity->owner->client->pers.netname));
			}
			if(!entity->owner) {
				trap_SendServerCommand(attacker->s.clientNum, "cllp \"Not owned\n\"");
			}
		}
	}

	if(attacker->tool_id == TL_COLLISION) {
		if(atoi(arg19) == 0) {
			entity->r.contents = CONTENTS_SOLID;
			entity->sb_coll = CONTENTS_SOLID;
		}
		if(atoi(arg19) == 1) {
			entity->r.contents = CONTENTS_TRIGGER;
			entity->sb_coll = CONTENTS_TRIGGER;
		}
	}

	if(attacker->tool_id == TL_HEALTH) {
		entity->health = atoi(arg01);
	}

	if(attacker->tool_id == TL_COLOR) {
		entity->s.constantLight = atoi(arg01) | (atoi(arg02) << 8) | (atoi(arg03) << 16) | (atoi(arg04) << 24);
		entity->sb_red = atoi(arg01);
		entity->sb_green = atoi(arg02);
		entity->sb_blue = atoi(arg03);
		entity->sb_radius = atoi(arg04);
	}

	if(attacker->tool_id == TL_ANGLE) {
		entity = G_FindWeldEntity(entity);  // find weld root or return ent
		if(atoi(arg19) == 0) {
			entity->s.apos.trBase[0] += atof(arg01);
		}
		if(atoi(arg19) == 1) {
			entity->s.apos.trBase[1] += atof(arg01);
		}
		if(atoi(arg19) == 2) {
			entity->s.apos.trBase[2] += atof(arg01);
		}
		if(atoi(arg19) == 3) {
			entity->s.apos.trBase[0] = 0.0;
			entity->s.apos.trBase[1] = 0.0;
			entity->s.apos.trBase[2] = 0.0;
		}
		VectorCopy(entity->s.apos.trBase, targ->s.angles);
		VectorCopy(entity->s.apos.trBase, targ->r.currentAngles);
	}

	if(attacker->tool_id == TL_SCALE) {
		if(atoi(arg19) == 0) {
			entity->s.scales[0] = atof(arg01);
		}
		if(atoi(arg19) == 1) {
			entity->s.scales[1] = atof(arg01);
		}
		if(atoi(arg19) == 2) {
			entity->s.scales[2] = atof(arg01);
		}
		if(atoi(arg19) == 3) {
			entity->s.scales[0] = 1.0;
			entity->s.scales[1] = 1.0;
			entity->s.scales[2] = 1.0;
		}
		VectorSet(entity->r.mins, -entity->sb_coltype * entity->s.scales[0], -entity->sb_coltype * entity->s.scales[1], -entity->sb_coltype * entity->s.scales[2]);
		VectorSet(entity->r.maxs, entity->sb_coltype * entity->s.scales[0], entity->sb_coltype * entity->s.scales[1], entity->sb_coltype * entity->s.scales[2]);
	}

	if(attacker->tool_id == TL_REPLACEITEM) {
		item_t *item;
		int i;
		if(entity->s.eType != ET_ITEM) {
			trap_SendServerCommand(attacker->s.clientNum, "cllp \"This must be the item\n\"");
			return;
		}
		for(item = gameInfoItems + 1, i = 1; item->classname; item++, i++) {
			if(!strcmp(item->classname, arg01)) {
				entity->item = &gameInfoItems[i];
				entity->item->classname = gameInfoItems[i].classname;
				entity->classname = gameInfoItems[i].classname;
				entity->s.modelindex = i;
			}
		}
	}

	if(attacker->tool_id == TL_COUNT) {
		entity->count = atoi(arg01);
	}

	if(attacker->tool_id == TL_WELD) {
		vec3_t parentForward, parentRight, parentUp;
		vec3_t weldForward, weldRight, weldUp;
		vec3_t relForward, relRight, relUp;

		if(entity->client) {
			trap_SendServerCommand(attacker->s.clientNum, "cllp \"This is not prop\n\"");
			return;
		}

		if(atoi(arg19) == 0) {  // Weld
			if(!attacker->tool_entity) {
				if(!entity->phys_weldedObjectsNum) {
					attacker->tool_entity = entity;
					trap_SendServerCommand(attacker->s.clientNum, "cllp \"Сlick on the second object\n\"");
				} else {
					trap_SendServerCommand(attacker->s.clientNum, "cllp \"This is root object\n\"");
					return;
				}
			} else {
				entity = G_FindWeldEntity(entity);  // find weld root or return ent
				attacker->tool_entity->phys_parent = entity;

				// Save origin
				VectorSubtract(attacker->tool_entity->r.currentOrigin, entity->r.currentOrigin, attacker->tool_entity->phys_relativeOrigin);
				attacker->tool_entity->phys_relativeOrigin[1] = -attacker->tool_entity->phys_relativeOrigin[1];

				// Vector from parent
				AngleVectors(entity->s.apos.trBase, parentForward, parentRight, parentUp);
				AngleVectors(attacker->tool_entity->s.apos.trBase, weldForward, weldRight, weldUp);

				// Diff vectors
				relForward[0] = DotProduct(weldForward, parentForward);
				relForward[1] = DotProduct(weldForward, parentRight);
				relForward[2] = DotProduct(weldForward, parentUp);

				relRight[0] = DotProduct(weldRight, parentForward);
				relRight[1] = DotProduct(weldRight, parentRight);
				relRight[2] = DotProduct(weldRight, parentUp);

				relUp[0] = DotProduct(weldUp, parentForward);
				relUp[1] = DotProduct(weldUp, parentRight);
				relUp[2] = DotProduct(weldUp, parentUp);

				OrthogonalizeMatrix(relForward, relRight, relUp);

				// Save result
				VectorCopy(relForward, attacker->tool_entity->phys_rv_0);
				VectorCopy(relRight, attacker->tool_entity->phys_rv_1);
				VectorCopy(relUp, attacker->tool_entity->phys_rv_2);
				entity->phys_weldedObjectsNum += 1;
				attacker->tool_entity = NULL;
			}
		}
		if(atoi(arg19) == 1) {  // Unweld
			attacker->tool_entity = NULL;

			if(!entity->phys_weldedObjectsNum && !entity->phys_parent) {
				trap_SendServerCommand(attacker->s.clientNum, "cllp \"This prop not welded\n\"");
				return;
			}

			Phys_Unweld(entity);
		}
	}

	if(attacker->tool_id == TL_BIND) {
		if(atoi(arg19) == 0) {
			CopyAlloc(entity->targetname, va("activate_%i_%s", attacker->s.clientNum, arg01));
		}
		if(atoi(arg19) == 1) {
			entity->targetname = NULL;
		}
	}
}

static void Undo_ShiftStack(gentity_t *ent) {
	int i;

	for(i = MAX_UNDO_STACK - 1; i > 0; i--) {
		ent->client->undoStack[i] = ent->client->undoStack[i - 1];
	}
}

void Undo_AddElement(gentity_t *ent, int id, int type) {
	Undo_ShiftStack(ent);

	ent->client->undoStack[0].id = id;
	ent->client->undoStack[0].type = type;
	ent->client->undoStack[0].isRemoved = qfalse;
}

qboolean Undo_LastElement(gentity_t *ent, int *id, int *type, qboolean *isRemoved) {
	int i;
	gentity_t *pent;

	for(i = 0; i < MAX_UNDO_STACK - 1; i++) {
		if(ent->client->undoStack[i].type == UNDO_PROPSPAWN) {
			pent = G_FindEntityForEntityNum(ent->client->undoStack[i].id);
		}
		if(ent->client->undoStack[i].type == UNDO_NPCSPAWN) {
			pent = G_FindEntityForClientNum(ent->client->undoStack[i].id);
		}
		if(pent) {
			ent->client->undoStack[i].isRemoved = qfalse;
		} else {
			ent->client->undoStack[i].isRemoved = qtrue;
		}
	}

	if(ent->client->undoStack[0].id == 0 && ent->client->undoStack[0].type == 0 && ent->client->undoStack[0].isRemoved == qfalse) {
		return qfalse;
	}

	*id = ent->client->undoStack[0].id;
	*type = ent->client->undoStack[0].type;
	*isRemoved = ent->client->undoStack[0].isRemoved;

	return qtrue;
}

void Undo_RemoveElement(gentity_t *ent) {
	int i;

	for(i = 0; i < MAX_UNDO_STACK - 1; i++) {
		ent->client->undoStack[i] = ent->client->undoStack[i + 1];
	}

	ent->client->undoStack[MAX_UNDO_STACK - 1].id = 0;
	ent->client->undoStack[MAX_UNDO_STACK - 1].type = 0;
	ent->client->undoStack[MAX_UNDO_STACK - 1].isRemoved = qfalse;
}
