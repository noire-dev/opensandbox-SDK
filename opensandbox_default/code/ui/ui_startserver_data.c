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

#include "ui_local.h"

#include "ui_startserver.h"


//
// global data
//

scriptdata_t s_scriptdata;

char* gametype_cvar_base[NUM_GAMETYPES] = {
	"gui_sandbox_%s",
	"gui_ffa_%s",
	"gui_single_%s",
	"gui_tourney_%s",
	"gui_team_%s",
	"gui_ctf_%s",
	"gui_oneflag_%s",
	"gui_overload_%s",
	"gui_harvester_%s",
	"gui_elim_%s",
	"gui_ctfelim_%s",
	"gui_lms_%s",
	"gui_doubledom_%s",
	"gui_domination_%s"
};

const char* idmap_list[] = {
	"oacmpdm1",
	"oacmpdm2",
	"oacmpdm3",
	"oacmpdm4",
	"oacmpdm5",
	"oacmpdm6",
	"oacmpdm7",
	"oacmpdm8",
	"oacmpdm9",
	"oacmpdm10",
	"oacmpctf1",
	"oacmpctf2",
	"oacmpctf3",
	0
};

const char *gametype_items[NUM_GAMETYPES + 1] = {
	"Sandbox",
	"Free For All",
	"Single Player",
	"Tournament",
	"Team Deathmatch",
	"Capture the Flag",
	"One Flag Capture",
	"Overload",
	"Harvester",
	"Elimination",
	"CTF Elimination",
	"Last Man Standing",
	"Double Domination",
	"Domination",
	0
};

const char *gametype_itemsru[NUM_GAMETYPES + 1] = {
	"Песочница",
	"Все Против Всех",
	"Одиночная Игра",
	"Турнир",
	"Командный Бой",
	"Захват флага",
	"Один Флаг",
	"Атака Базы",
	"Жнец",
	"Устранение",
	"Устранение: Захват флага",
	"Последний оставшийся",
	"Двойное доминирование",
	"Доминирование",
	0
};

//
// individual items
// Must be in ITEM_* numerical order
//
itemnode_t server_itemlist[ITEM_COUNT] = {
	// weapon
	{ ITEMGROUP_WEAPON, ITEM_GRAPPLING_HOOK, 		"weapon_grapplinghook", 	"gh"},
	{ ITEMGROUP_WEAPON, ITEM_MGUN, 		"weapon_machinegun", 	"mg"},
	{ ITEMGROUP_WEAPON, ITEM_SHOTGUN, 	"weapon_shotgun", 		"sg"},
	{ ITEMGROUP_WEAPON, ITEM_GRENADE, 	"weapon_grenadelauncher", "gl"},
	{ ITEMGROUP_WEAPON, ITEM_ROCKET, 	"weapon_rocketlauncher", "rl"},
	{ ITEMGROUP_WEAPON, ITEM_PLASMA, 	"weapon_plasmagun", 	"pg"},
	{ ITEMGROUP_WEAPON, ITEM_LIGHTNING, "weapon_lightning", 	"lg"},
	{ ITEMGROUP_WEAPON, ITEM_RAIL, 		"weapon_railgun", 		"rg"},
	{ ITEMGROUP_WEAPON, ITEM_BFG, 		"weapon_bfg", 			"bfg"},
	{ ITEMGROUP_WEAPON, ITEM_NAILGUN, 		"weapon_nailgun", 			"ng"},
	{ ITEMGROUP_WEAPON, ITEM_PROX_LAUNCHER, 		"weapon_prox_launcher", 			"pl"},
	{ ITEMGROUP_WEAPON, ITEM_CHAINGUN, 		"weapon_chaingun", 			"cg"},
	{ ITEMGROUP_WEAPON, ITEM_FLAMETHROWER, 		"weapon_flamethrower", 			"ft"},
	{ ITEMGROUP_WEAPON, ITEM_ANTIMATTER, 		"weapon_antimatter", 			"am"},

	// ammo
	{ ITEMGROUP_AMMO, ITEM_MGUN_AMMO, 	"ammo_bullets", 		"amg"},
	{ ITEMGROUP_AMMO, ITEM_SHOTGUN_AMMO, "ammo_shells", 		"asg"},
	{ ITEMGROUP_AMMO, ITEM_GRENADE_AMMO, "ammo_grenades", 		"agl"},
	{ ITEMGROUP_AMMO, ITEM_ROCKET_AMMO, "ammo_rockets", 		"arl"},
	{ ITEMGROUP_AMMO, ITEM_PLASMA_AMMO, "ammo_cells", 			"apg"},
	{ ITEMGROUP_AMMO, ITEM_LIGHTNING_AMMO, "ammo_lightning", 	"alg"},
	{ ITEMGROUP_AMMO, ITEM_RAIL_AMMO, 	"ammo_slugs", 			"arg"},
	{ ITEMGROUP_AMMO, ITEM_BFG_AMMO, 	"ammo_bfg", 			"abfg"},
	{ ITEMGROUP_AMMO, ITEM_NAILGUN_AMMO, 	"ammo_nails", 			"abfg"},
	{ ITEMGROUP_AMMO, ITEM_PROX_LAUNCHER_AMMO, 	"ammo_mines", 			"abfg"},
	{ ITEMGROUP_AMMO, ITEM_CHAINGUN_AMMO, 	"ammo_belt", 			"abfg"},
	{ ITEMGROUP_AMMO, ITEM_FLAMETHROWER_AMMO, 	"ammo_flame", 			"aft"},

	// health
	{ ITEMGROUP_HEALTH, ITEM_MEGA,		"item_health_mega",		"mega"},
	{ ITEMGROUP_HEALTH, ITEM_HEALTH_SMALL, "item_health_small",	"hs"},

	{ ITEMGROUP_HEALTH, ITEM_HEALTH_MEDIUM, "item_health", 		"hm"},
	{ ITEMGROUP_HEALTH, ITEM_HEALTH_LARGE, "item_health_large",	"hl"},

	// armour
	{ ITEMGROUP_ARMOUR, ITEM_ARMOUR_PURPLE, "item_armor_full",	"ap"},
	{ ITEMGROUP_ARMOUR, ITEM_ARMOUR_RED,	"item_armor_body",		"ar"},
	{ ITEMGROUP_ARMOUR, ITEM_ARMOUR_YELLOW, "item_armor_combat",	"ay"},
	{ ITEMGROUP_ARMOUR, ITEM_ARMOUR_GREEN, "item_armor_vest",	"ag"},
	{ ITEMGROUP_ARMOUR, ITEM_ARMOUR_SHARD, "item_armor_shard",	"as"},

	// powerups
	{ ITEMGROUP_POWERUPS, ITEM_QUAD,		"item_quad",			"quad"},
	{ ITEMGROUP_POWERUPS, ITEM_REGEN,		"item_regen",			"reg"},
	{ ITEMGROUP_POWERUPS, ITEM_HASTE,		"item_haste",			"hst"},

	{ ITEMGROUP_POWERUPS, ITEM_BATTLESUIT,	"item_enviro",			"bs"},
	{ ITEMGROUP_POWERUPS, ITEM_INVISIBLE, 	"item_invis",			"inv"},
	{ ITEMGROUP_POWERUPS, ITEM_FLIGHT,		"item_flight",			"flt"},

	{ ITEMGROUP_POWERUPS, ITEM_SCOUT,	"item_scout",			"sc"},
	{ ITEMGROUP_POWERUPS, ITEM_GUARD, 	"item_guard",			"gur"},
	{ ITEMGROUP_POWERUPS, ITEM_DOUBLER,		"item_doubler",			"doub"},
	{ ITEMGROUP_POWERUPS, ITEM_AMMOREGEN,	"item_ammoregen",			"amreg"},

	// items
	{ ITEMGROUP_HOLDABLE,	ITEM_MEDKIT, 		"holdable_medkit",		"mk"},
	{ ITEMGROUP_HOLDABLE,	ITEM_TELEPORTER,	"holdable_teleporter",	"tel"},
	{ ITEMGROUP_HOLDABLE,	ITEM_KAMIKAZE, 		"holdable_kamikaze",		"kmz"},
	{ ITEMGROUP_HOLDABLE,	ITEM_INVULNERABILITY,	"holdable_invulnerability",	"invul"},
	{ ITEMGROUP_HOLDABLE,	ITEM_PORTAL, 		"holdable_portal",		"pt"}
};




/*
==========================================================================

DATA ARCHIVE

==========================================================================
*/


// Some of these vars are legacy cvars that were previously stored in q3config.cfg.
// In order to load them into memory for porting over to the new data format
// we need to "register" them and load their current values

// This porting process should be done only once, assuming we installed over
// a previous version of UIE


// Default values for cvars that we haven't yet created

// On startup we load from SourceTechui.dat, check for new vars from this list,
// and add them into data storage.





typedef struct {
	char		*cvarName;
	char		*defaultString;
} gui_cvarTable_t;


gui_cvarTable_t gui_cvarTable[] = {
{ "gui_gametype", "0" },
{ "gui_pure", "0" },
{ "gui_dynamiclight", "0" },
{ "gui_dedicated", "0" },
{ "gui_inactivity", "0" },
{ "gui_config_showid", "0" },
{ "gui_pmove_fixed", "1" },
{ "gui_pmove_msec", "11" },
{ "gui_smoothclients", "1" },
{ "gui_syncclients", "0" },
{ "gui_allowmaxrate", "0" },
{ "gui_maxrate", "0" },
{ "gui_allowdownload", "0" },
{ "gui_allowpass", "0" },
{ "gui_password", "" },
{ "gui_allowvote", "1" },
{ "gui_minPing", "0" },
{ "gui_maxPing", "0" },
{ "gui_allowMinPing", "0" },
{ "gui_allowMaxPing", "0" },
{ "gui_preventConfigBug", "0" },
{ "gui_gravity", "800" },
{ "gui_jumpheight", "270" },
{ "gui_knockback", "1000" },
{ "gui_quadfactor", "3" },
{ "gui_netport", "27960" },
{ "gui_svfps", "60" },
{ "gui_allowprivateclients", "0" },
{ "gui_privateclients", "0" },
{ "gui_privatepassword", "" },
{ "gui_strictAuth", "1" },
{ "gui_lanForceRate", "1" },

{ "gui_maxEntities", "1024" },
{ "gui_singleskill", "1" },
{ "gui_kill", "1" },
{ "gui_damageModifier", "1" },
{ "gui_selectedmod", "default" },
{ "gui_elimination", "0" },
{ "gui_obeliskHealth", "2500" },
{ "gui_obeliskRegenPeriod", "1" },
{ "gui_obeliskRegenAmount", "15" },
{ "gui_obeliskRespawnDelay", "10" },
{ "gui_cubeTimeout", "30" },
{ "gui_flagrespawn", "-1" },
{ "gui_weaponTeamRespawn", "30" },
{ "gui_elimination_ctf_oneway", "0" },
{ "gui_elimination_selfdamage", "0" },
{ "gui_elimination_roundtime", "600" },
{ "gui_elimination_warmup", "0" },
{ "gui_elimination_activewarmup", "0" },
{ "gui_lms_lives", "1" },
{ "gui_lms_mode", "0" },
{ "gui_accelerate", "1" },
{ "gui_spectatorspeed", "700" },
{ "gui_speed", "320" },
{ "gui_gravity", "800" },
{ "gui_gravityModifier", "1" },
{ "gui_knockback", "1000" },
{ "gui_noplayerclip", "0" },
{ "gui_jumpheight", "270" },
{ "gui_regenarmor", "0" },
{ "gui_ammolimit", "9000" },
{ "gui_quadfactor", "3" },
{ "gui_respawntime", "0" },
{ "gui_forcerespawn", "0" },
{ "gui_vampire", "0.0" },
{ "gui_vampire_max_health", "500" },
{ "gui_regen", "0" },
{ "gui_maxweaponpickup", "1" },
{ "gui_droppeditemtime", "30" },
{ "gui_autoflagreturn", "30" },
{ "gui_armorprotect", "0.66" },
{ "gui_respawnwait", "3000" },
{ "gui_speedfactor", "1.3" },
{ "gui_scoutspeedfactor", "1.5" },
{ "gui_scoutfirespeed", "0.60" },
{ "gui_scoutdamagefactor", "1" },
{ "gui_scoutgravitymodifier", "1" },
{ "gui_scout_infammo", "0" },
{ "gui_scouthealthmodifier", "1" },
{ "gui_doublerfirespeed", "1" },
{ "gui_doublerdamagefactor", "2.5" },
{ "gui_doublerspeedfactor", "1" },
{ "gui_doublergravitymodifier", "1" },
{ "gui_doubler_infammo", "0" },
{ "gui_doublerhealthmodifier", "1" },
{ "gui_guardhealthmodifier", "1.5" },
{ "gui_guardfirespeed", "1" },
{ "gui_guarddamagefactor", "1" },
{ "gui_guardspeedfactor", "1" },
{ "gui_guardgravitymodifier", "1" },
{ "gui_guard_infammo", "0" },
{ "gui_ammoregenfirespeed", "0.65" },
{ "gui_ammoregen_infammo", "0" },
{ "gui_ammoregendamagefactor", "1" },
{ "gui_ammoregenspeedfactor", "1" },
{ "gui_ammoregengravitymodifier", "1" },
{ "gui_ammoregenhealthmodifier", "1" },
{ "gui_teamred_speed", "320" },
{ "gui_teamred_gravityModifier", "1" },
{ "gui_teamred_firespeed", "1" },
{ "gui_teamred_damage", "1" },
{ "gui_teamred_infammo", "0" },
{ "gui_teamred_respawnwait", "3000" },
{ "gui_teamred_pickupitems", "1" },
{ "gui_eliminationredrespawn", "0" },
{ "gui_eliminationred_startHealth", "100" },
{ "gui_eliminationred_startArmor", "0" },
{ "gui_teamblue_speed", "320" },
{ "gui_teamblue_gravityModifier", "1" },
{ "gui_teamblue_firespeed", "1" },
{ "gui_teamblue_damage", "1" },
{ "gui_teamblue_infammo", "0" },
{ "gui_teamblue_respawnwait", "3000" },
{ "gui_teamblue_pickupitems", "1" },
{ "gui_eliminationrespawn", "0" },
{ "gui_elimination_startHealth", "100" },
{ "gui_elimination_startArmor", "0" },
{ "gui_eliminationred_grapple", "0" },
{ "gui_eliminationred_gauntlet", "1" },
{ "gui_eliminationred_machinegun", "500" },
{ "gui_eliminationred_shotgun", "0" },
{ "gui_eliminationred_grenade", "0" },
{ "gui_eliminationred_rocket", "0" },
{ "gui_eliminationred_railgun", "0" },
{ "gui_eliminationred_lightning", "0" },
{ "gui_eliminationred_plasmagun", "0" },
{ "gui_eliminationred_bfg", "0" },
{ "gui_eliminationred_chain", "0" },
{ "gui_eliminationred_mine", "0" },
{ "gui_eliminationred_nail", "0" },
{ "gui_eliminationred_flame", "0" },
{ "gui_eliminationred_antimatter", "0" },
{ "gui_elimination_grapple", "0" },
{ "gui_elimination_gauntlet", "1" },
{ "gui_elimination_machinegun", "500" },
{ "gui_elimination_shotgun", "0" },
{ "gui_elimination_grenade", "0" },
{ "gui_elimination_rocket", "0" },
{ "gui_elimination_railgun", "0" },
{ "gui_elimination_lightning", "0" },
{ "gui_elimination_plasmagun", "0" },
{ "gui_elimination_bfg", "0" },
{ "gui_elimination_chain", "0" },
{ "gui_elimination_mine", "0" },
{ "gui_elimination_nail", "0" },
{ "gui_elimination_flame", "0" },
{ "gui_elimination_antimatter", "0" },
{ "gui_eliminationred_quad", "0" },
{ "gui_eliminationred_haste", "0" },
{ "gui_eliminationred_bsuit", "0" },
{ "gui_eliminationred_invis", "0" },
{ "gui_eliminationred_regen", "0" },
{ "gui_eliminationred_flight", "0" },
{ "gui_eliminationred_holdable", "0" },
{ "gui_elimination_quad", "0" },
{ "gui_elimination_haste", "0" },
{ "gui_elimination_bsuit", "0" },
{ "gui_elimination_invis", "0" },
{ "gui_elimination_regen", "0" },
{ "gui_elimination_flight", "0" },
{ "gui_elimination_items", "0" },
{ "gui_elimination_holdable", "0" },
{ "gui_overlay", "0" },
{ "gui_randomItems", "0" },
{ "gui_slickmove", "0" },
{ "gui_spawnprotect", "500" },
{ "gui_elimination_lockspectator", "0" },
{ "gui_awardpushing", "1" },
{ "gui_randomteleport", "0" },
{ "gui_falldamagesmall", "5"  },
{ "gui_falldamagebig", "10" },
{ "gui_waterdamage", "0" },
{ "gui_lavadamage", "30" },
{ "gui_slimedamage", "10" },
{ "gui_drowndamage", "1" },
{ "gui_invulinf", "0" },
{ "gui_invulmove", "0" },
{ "gui_invultime", "10" },
{ "gui_kamikazeinf", "0" },
{ "gui_portalinf", "0" },
{ "gui_portaltimeout", "120" },
{ "gui_portalhealth", "200" },
{ "gui_teleporterinf", "0" },
{ "gui_medkitinf", "0" },
{ "gui_medkitlimit", "200" },
{ "gui_medkitmodifier", "100" },
{ "gui_fasthealthregen", "15" },
{ "gui_slowhealthregen", "5" },
{ "gui_hastefirespeed", "0.65" },
{ "gui_quadtime", "30" },
{ "gui_bsuittime", "30" },
{ "gui_hastetime", "30" },
{ "gui_invistime", "30" },
{ "gui_regentime", "30" },
{ "gui_flighttime", "60" },
{ "gui_armorrespawn", "25" },
{ "gui_healthrespawn", "35" },
{ "gui_ammorespawn", "30" },
{ "gui_holdablerespawn", "60" },
{ "gui_megahealthrespawn", "35" },
{ "gui_poweruprespawn", "120" },
{ "gui_weaponrespawn", "5" },

// Крюк
{ "gui_ghspeed", "800" },
{ "gui_ghtimeout", "10000" },
// Пила
{ "gui_gdelay", "400" },
{ "gui_gdamage", "100" },
{ "gui_grange", "32" },
{ "gui_gknockback", "1" },
// Автомат
{ "gui_mgammocount", "-1" },
{ "gui_mgweaponcount", "-1" },
{ "gui_mgdelay", "100" },
{ "gui_mgdamage", "7" },
{ "gui_mgspread", "200" },
{ "gui_mgexplode", "0" },
{ "gui_mgsdamage", "50" },
{ "gui_mgsradius", "70" },
{ "gui_mgvampire", "0" },
{ "gui_mginf", "0" },
{ "gui_mgknockback", "1" },
// Дробовик
{ "gui_sgammocount", "-1" },
{ "gui_sgweaponcount", "-1" },
{ "gui_sgdelay", "1000" },
{ "gui_sgdamage", "10" },
{ "gui_sgspread", "700" },
{ "gui_sgexplode", "0" },
{ "gui_sgsdamage", "45" },
{ "gui_sgsradius", "35" },
{ "gui_sgcount", "11" },
{ "gui_sgvampire", "0" },
{ "gui_sginf", "0" },
{ "gui_sgknockback", "1" },
// Гранаты
{ "gui_glammocount", "-1" },
{ "gui_glweaponcount", "-1" },
{ "gui_gldelay", "500" },
{ "gui_glspeed", "1000" },
{ "gui_glbounce", "1" },
{ "gui_glgravity", "1" },
{ "gui_gltimeout", "2500" },
{ "gui_glsradius", "120" },
{ "gui_glsdamage", "100" },
{ "gui_gldamage", "100" },
{ "gui_glvampire", "0" },
{ "gui_glinf", "0" },
{ "gui_glbouncemodifier", "0.65" },
{ "gui_glknockback", "1" },
{ "gui_glhoming", "0" },
{ "gui_glguided", "0" },
// Ракеты
{ "gui_rlammocount", "-1" },
{ "gui_rlweaponcount", "-1" },
{ "gui_rldelay", "800" },
{ "gui_rlspeed", "900" },
{ "gui_rlbounce", "0" },
{ "gui_rlgravity", "0" },
{ "gui_rltimeout", "15000" },
{ "gui_rlsradius", "120" },
{ "gui_rlsdamage", "100" },
{ "gui_rldamage", "100" },
{ "gui_rlvampire", "0" },
{ "gui_rlinf", "0" },
{ "gui_rlbouncemodifier", "0.65" },
{ "gui_rlknockback", "1" },
{ "gui_rlhoming", "0" },
{ "gui_rlguided", "0" },
// Молния
{ "gui_lgammocount", "-1" },
{ "gui_lgweaponcount", "-1" },
{ "gui_lgrange", "768" },
{ "gui_lgdelay", "50" },
{ "gui_lgdamage", "8" },
{ "gui_lgvampire", "0" },
{ "gui_lgexplode", "0" },
{ "gui_lgsdamage", "70" },
{ "gui_lgsradius", "75" },
{ "gui_lginf", "0" },
{ "gui_lgknockback", "1" },
// Рэйлган
{ "gui_rgammocount", "-1" },
{ "gui_rgweaponcount", "-1" },
{ "gui_rgdelay", "1500" },
{ "gui_rgdamage", "100" },
{ "gui_rgvampire", "0" },
{ "gui_rginf", "0" },
{ "gui_rgknockback", "1" },
// Плазмаган
{ "gui_pgammocount", "-1" },
{ "gui_pgweaponcount", "-1" },
{ "gui_pgdelay", "100" },
{ "gui_pgspeed", "2000" },
{ "gui_pgbounce", "0" },
{ "gui_pggravity", "0" },
{ "gui_pgtimeout", "10000" },
{ "gui_pgsradius", "20" },
{ "gui_pgsdamage", "15" },
{ "gui_pgdamage", "20" },
{ "gui_pgvampire", "0" },
{ "gui_pginf", "0" },
{ "gui_pgbouncemodifier", "0.65" },
{ "gui_pgknockback", "1" },
{ "gui_pghoming", "0" },
{ "gui_pgguided", "0" },
// Бфг
{ "gui_bfgammocount", "-1" },
{ "gui_bfgweaponcount", "-1" },
{ "gui_bfgdelay", "200" },
{ "gui_bfgspeed", "2000" },
{ "gui_bfgbounce", "0" },
{ "gui_bfggravity", "0" },
{ "gui_bfgtimeout", "10000" },
{ "gui_bfgsradius", "120" },
{ "gui_bfgsdamage", "100" },
{ "gui_bfgdamage", "100" },
{ "gui_bfgvampire", "0" },
{ "gui_bfginf", "0" },
{ "gui_bfgbouncemodifier", "0.65" },
{ "gui_bfgknockback", "1" },
{ "gui_bfghoming", "0" },
{ "gui_bfgguided", "0" },
// Гвоздомёт
{ "gui_ngammocount", "-1" },
{ "gui_ngweaponcount", "-1" },
{ "gui_ngdelay", "1000" },
{ "gui_ngspeed", "555" },
{ "gui_ngbounce", "0" },
{ "gui_nggravity", "0" },
{ "gui_ngtimeout", "10000" },
{ "gui_ngcount", "15" },
{ "gui_ngspread", "500" },
{ "gui_ngdamage", "20" },
{ "gui_ngrandom", "1800" },
{ "gui_ngvampire", "0" },
{ "gui_nginf", "0" },
{ "gui_ngbouncemodifier", "0.65" },
{ "gui_ngknockback", "1" },
{ "gui_nghoming", "0" },
{ "gui_ngguided", "0" },
// Мины
{ "gui_plammocount", "-1" },
{ "gui_plweaponcount", "-1" },
{ "gui_pldelay", "800" },
{ "gui_plspeed", "700" },
{ "gui_plgravity", "1" },
{ "gui_pltimeout", "3000" },
{ "gui_plsradius", "150" },
{ "gui_plsdamage", "100" },
{ "gui_pldamage", "0" },
{ "gui_plvampire", "0" },
{ "gui_plinf", "0" },
{ "gui_plknockback", "1" },
// Пулемёт
{ "gui_cgammocount", "-1" },
{ "gui_cgweaponcount", "-1" },
{ "gui_cgdelay", "30" },
{ "gui_cgspread", "600" },
{ "gui_cgdamage", "7" },
{ "gui_cgvampire", "0" },
{ "gui_cginf", "0" },
{ "gui_cgknockback", "1" },
// Огнемёт
{ "gui_ftammocount", "-1" },
{ "gui_ftweaponcount", "-1" },
{ "gui_ftdelay", "40" },
{ "gui_ftspeed", "600" },
{ "gui_ftbounce", "1" },
{ "gui_ftgravity", "0" },
{ "gui_fttimeout", "1500" },
{ "gui_ftsradius", "20" },
{ "gui_ftsdamage", "15" },
{ "gui_ftdamage", "20" },
{ "gui_ftvampire", "0" },
{ "gui_ftinf", "0" },
{ "gui_ftbouncemodifier", "0.65" },
{ "gui_ftknockback", "1" },
{ "gui_fthoming", "0" },
{ "gui_ftguided", "0" },
// Антиматтер
{ "gui_amweaponcount", "-1" },
{ "gui_amdelay", "40" },
{ "gui_amspeed", "600" },
{ "gui_ambounce", "1" },
{ "gui_amgravity", "1" },
{ "gui_amtimeout", "1000" },
{ "gui_amsradius", "0" },
{ "gui_amsdamage", "0" },
{ "gui_amdamage", "6" },
{ "gui_amvampire", "0" },
{ "gui_aminf", "0" },
{ "gui_ambouncemodifier", "0.65" },
{ "gui_amknockback", "1" },
{ "gui_amhoming", "0" },
{ "gui_amguided", "0" },

	{ "gui_sandbox_fragtype", "0" },
	{ "gui_sandbox_timetype", "0" },
	{ "gui_sandbox_customfraglimits", "" },
	{ "gui_sandbox_customtimelimits", "" },
	{ "gui_sandbox_maplist", "" },
	{ "gui_sandbox_maplistexclude", "" },
	{ "gui_sandbox_MapRepeat", "1" },
	{ "gui_sandbox_MapSource", "0" },
	{ "gui_sandbox_RandomMapCount", "5" },
	{ "gui_sandbox_RandomMapType", "0" },
	{ "gui_sandbox_slottype", "" },
	{ "gui_sandbox_botname", "" },
	{ "gui_sandbox_botexclude", "" },
	{ "gui_sandbox_botskill", "" },
	{ "gui_sandbox_BotSelection", "1" },
	{ "gui_sandbox_BotCount", "0" },
	{ "gui_sandbox_BotChange", "0" },
	{ "gui_sandbox_OpenSlotCount", "99" },
	{ "gui_sandbox_BotSkillType", "0" },
	{ "gui_sandbox_BotSkillValue", "0" },
	{ "gui_sandbox_BotSkillBias", "0" },
	{ "gui_sandbox_PlayerJoinAs", "0" },
	{ "gui_sandbox_hostname", "OpenSandbox Sandbox" },
	{ "gui_sandbox_ForceRespawn", "0" },
	{ "gui_sandbox_itemGroups", "" },
	{ "gui_sandbox_itemsHidden", "" },
	{ "gui_sandbox_WarmUp", "20" },
	{ "gui_sandbox_doWarmUp", "0" },
	{ "gui_sandbox_fraglimit", "0" },
	{ "gui_sandbox_timelimit", "0" },
	{ "gui_sandbox_weaponrespawn", "5" },
	{ "gui_sandbox_viewdistance", "0" },
	
	{ "gui_single_fragtype", "0" },
	{ "gui_single_timetype", "0" },
	{ "gui_single_customfraglimits", "" },
	{ "gui_single_customtimelimits", "" },
	{ "gui_single_maplist", "" },
	{ "gui_single_maplistexclude", "" },
	{ "gui_single_MapRepeat", "1" },
	{ "gui_single_MapSource", "0" },
	{ "gui_single_RandomMapCount", "5" },
	{ "gui_single_RandomMapType", "0" },
	{ "gui_single_slottype", "" },
	{ "gui_single_botname", "" },
	{ "gui_single_botexclude", "" },
	{ "gui_single_botskill", "" },
	{ "gui_single_BotSelection", "1" },
	{ "gui_single_BotCount", "0" },
	{ "gui_single_BotChange", "0" },
	{ "gui_single_OpenSlotCount", "99" },
	{ "gui_single_BotSkillType", "0" },
	{ "gui_single_BotSkillValue", "0" },
	{ "gui_single_BotSkillBias", "0" },
	{ "gui_single_PlayerJoinAs", "0" },
	{ "gui_single_hostname", "OpenSandbox Single" },
	{ "gui_single_ForceRespawn", "0" },
	{ "gui_single_itemGroups", "" },
	{ "gui_single_itemsHidden", "" },
	{ "gui_single_WarmUp", "20" },
	{ "gui_single_doWarmUp", "0" },
	{ "gui_single_fraglimit", "0" },
	{ "gui_single_timelimit", "0" },
	{ "gui_single_weaponrespawn", "5" },
	{ "gui_single_viewdistance", "0" },

	{ "gui_ffa_fragtype", "0" },
	{ "gui_ffa_timetype", "0" },
	{ "gui_ffa_customfraglimits", "" },
	{ "gui_ffa_customtimelimits", "" },
	{ "gui_ffa_maplist", "" },
	{ "gui_ffa_maplistexclude", "" },
	{ "gui_ffa_MapRepeat", "1" },
	{ "gui_ffa_MapSource", "0" },
	{ "gui_ffa_RandomMapCount", "5" },
	{ "gui_ffa_RandomMapType", "0" },
	{ "gui_ffa_slottype", "" },
	{ "gui_ffa_botname", "" },
	{ "gui_ffa_botexclude", "" },
	{ "gui_ffa_botskill", "" },
	{ "gui_ffa_BotSelection", "1" },
	{ "gui_ffa_BotCount", "4" },
	{ "gui_ffa_BotChange", "0" },
	{ "gui_ffa_OpenSlotCount", "99" },
	{ "gui_ffa_BotSkillType", "0" },
	{ "gui_ffa_BotSkillValue", "0" },
	{ "gui_ffa_BotSkillBias", "0" },
	{ "gui_ffa_PlayerJoinAs", "0" },
	{ "gui_ffa_hostname", "OpenSandbox Deathmatch" },
	{ "gui_ffa_ForceRespawn", "0" },
	{ "gui_ffa_itemGroups", "" },
	{ "gui_ffa_itemsHidden", "" },
	{ "gui_ffa_WarmUp", "20" },
	{ "gui_ffa_doWarmUp", "0" },
	{ "gui_ffa_fraglimit", "30" },
	{ "gui_ffa_timelimit", "20" },
	{ "gui_ffa_weaponrespawn", "5" },
	{ "gui_ffa_viewdistance", "0" },

	{ "gui_tourney_fragtype", "0" },
	{ "gui_tourney_timetype", "0" },
	{ "gui_tourney_customfraglimits", "" },
	{ "gui_tourney_customtimelimits", "" },
	{ "gui_tourney_maplist", "" },
	{ "gui_tourney_maplistexclude", "" },
	{ "gui_tourney_MapRepeat", "1" },
	{ "gui_tourney_MapSource", "0" },
	{ "gui_tourney_RandomMapCount", "5" },
	{ "gui_tourney_RandomMapType", "0" },
	{ "gui_tourney_slottype", "" },
	{ "gui_tourney_botname", "" },
	{ "gui_tourney_botexclude", "" },
	{ "gui_tourney_botskill", "" },
	{ "gui_tourney_BotSelection", "1" },
	{ "gui_tourney_BotCount", "4" },
	{ "gui_tourney_BotChange", "0" },
	{ "gui_tourney_OpenSlotCount", "99" },
	{ "gui_tourney_BotSkillType", "0" },
	{ "gui_tourney_BotSkillValue", "0" },
	{ "gui_tourney_BotSkillBias", "0" },
	{ "gui_tourney_PlayerJoinAs", "0" },
	{ "gui_tourney_hostname", "OpenSandbox Tournament" },
	{ "gui_tourney_ForceRespawn", "0" },
	{ "gui_tourney_itemGroups", "" },
	{ "gui_tourney_itemsHidden", "" },
	{ "gui_tourney_WarmUp", "20" },
	{ "gui_tourney_doWarmUp", "1" },
	{ "gui_tourney_fraglimit", "0" },
	{ "gui_tourney_timelimit", "15" },
	{ "gui_tourney_weaponrespawn", "5" },
	{ "gui_tourney_viewdistance", "0" },

	{ "gui_team_fragtype", "0" },
	{ "gui_team_timetype", "0" },
	{ "gui_team_customfraglimits", "" },
	{ "gui_team_customtimelimits", "" },
	{ "gui_team_maplist", "" },
	{ "gui_team_maplistexclude", "" },
	{ "gui_team_MapRepeat", "1" },
	{ "gui_team_MapSource", "0" },
	{ "gui_team_RandomMapCount", "5" },
	{ "gui_team_RandomMapType", "0" },
	{ "gui_team_slottype", "" },
	{ "gui_team_botname", "" },
	{ "gui_team_botexclude", "" },
	{ "gui_team_botexclude", "" },
	{ "gui_team_botskill", "" },
	{ "gui_team_BotSelection", "1" },
	{ "gui_team_BotCount", "4" },
	{ "gui_team_BotChange", "0" },
	{ "gui_team_OpenSlotCount", "99" },
	{ "gui_team_BotSkillType", "0" },
	{ "gui_team_BotSkillValue", "0" },
	{ "gui_team_BotSkillBias", "0" },
	{ "gui_team_PlayerJoinAs", "0" },
	{ "gui_team_TeamSwapped", "0" },
	{ "gui_team_hostname", "OpenSandbox Team Deathmatch" },
	{ "gui_team_ForceRespawn", "0" },
	{ "gui_team_AutoJoin", "0" },
	{ "gui_team_TeamBalance", "0" },
	{ "gui_team_itemGroups", "" },
	{ "gui_team_itemsHidden", "" },
	{ "gui_team_WarmUp", "20" },
	{ "gui_team_doWarmUp", "0" },
	{ "gui_team_friendly", "0" },
	{ "gui_team_fraglimit", "0" },
	{ "gui_team_timelimit", "20" },
	{ "gui_team_weaponrespawn", "30" },
	{ "gui_team_viewdistance", "0" },

	{ "gui_ctf_capturetype", "0" },
	{ "gui_ctf_timetype", "0" },
	{ "gui_ctf_customcapturelimits", "" },
	{ "gui_ctf_customtimelimits", "" },
	{ "gui_ctf_maplist", "" },
	{ "gui_ctf_maplistexclude", "" },
	{ "gui_ctf_MapRepeat", "1" },
	{ "gui_ctf_MapSource", "0" },
	{ "gui_ctf_RandomMapCount", "5" },
	{ "gui_ctf_RandomMapType", "0" },
	{ "gui_ctf_slottype", "" },
	{ "gui_ctf_botname", "" },
	{ "gui_ctf_botskill", "" },
	{ "gui_ctf_BotSelection", "1" },
	{ "gui_ctf_BotCount", "4" },
	{ "gui_ctf_BotChange", "0" },
	{ "gui_ctf_OpenSlotCount", "99" },
	{ "gui_ctf_BotSkillType", "0" },
	{ "gui_ctf_BotSkillValue", "0" },
	{ "gui_ctf_BotSkillBias", "0" },
	{ "gui_ctf_PlayerJoinAs", "0" },
	{ "gui_ctf_TeamSwapped", "0" },
	{ "gui_ctf_hostname", "OpenSandbox CTF" },
	{ "gui_ctf_ForceRespawn", "0" },
	{ "gui_ctf_AutoJoin", "0" },
	{ "gui_ctf_TeamBalance", "0" },
	{ "gui_ctf_itemGroups", "" },
	{ "gui_ctf_itemsHidden", "" },
	{ "gui_ctf_WarmUp", "20" },
	{ "gui_ctf_doWarmUp", "0" },
	{ "gui_ctf_friendly", "0" },
	{ "gui_ctf_capturelimit", "0" },
	{ "gui_ctf_timelimit", "20" },
	{ "gui_ctf_weaponrespawn", "5" },
	{ "gui_ctf_viewdistance", "0" },
	
	{ "gui_oneflag_capturetype", "0" },
	{ "gui_oneflag_timetype", "0" },
	{ "gui_oneflag_customcapturelimits", "" },
	{ "gui_oneflag_customtimelimits", "" },
	{ "gui_oneflag_maplist", "" },
	{ "gui_oneflag_maplistexclude", "" },
	{ "gui_oneflag_MapRepeat", "1" },
	{ "gui_oneflag_MapSource", "0" },
	{ "gui_oneflag_RandomMapCount", "5" },
	{ "gui_oneflag_RandomMapType", "0" },
	{ "gui_oneflag_slottype", "" },
	{ "gui_oneflag_botname", "" },
	{ "gui_oneflag_botskill", "" },
	{ "gui_oneflag_BotSelection", "1" },
	{ "gui_oneflag_BotCount", "4" },
	{ "gui_oneflag_BotChange", "0" },
	{ "gui_oneflag_OpenSlotCount", "99" },
	{ "gui_oneflag_BotSkillType", "0" },
	{ "gui_oneflag_BotSkillValue", "0" },
	{ "gui_oneflag_BotSkillBias", "0" },
	{ "gui_oneflag_PlayerJoinAs", "0" },
	{ "gui_oneflag_TeamSwapped", "0" },
	{ "gui_oneflag_hostname", "OpenSandbox Oneflag CTF" },
	{ "gui_oneflag_ForceRespawn", "0" },
	{ "gui_oneflag_AutoJoin", "0" },
	{ "gui_oneflag_TeamBalance", "0" },
	{ "gui_oneflag_itemGroups", "" },
	{ "gui_oneflag_itemsHidden", "" },
	{ "gui_oneflag_WarmUp", "20" },
	{ "gui_oneflag_doWarmUp", "0" },
	{ "gui_oneflag_friendly", "0" },
	{ "gui_oneflag_capturelimit", "0" },
	{ "gui_oneflag_timelimit", "20" },
	{ "gui_oneflag_weaponrespawn", "5" },
	{ "gui_oneflag_viewdistance", "0" },
	
	{ "gui_overload_capturetype", "0" },
	{ "gui_overload_timetype", "0" },
	{ "gui_overload_customcapturelimits", "" },
	{ "gui_overload_customtimelimits", "" },
	{ "gui_overload_maplist", "" },
	{ "gui_overload_maplistexclude", "" },
	{ "gui_overload_MapRepeat", "1" },
	{ "gui_overload_MapSource", "0" },
	{ "gui_overload_RandomMapCount", "5" },
	{ "gui_overload_RandomMapType", "0" },
	{ "gui_overload_slottype", "" },
	{ "gui_overload_botname", "" },
	{ "gui_overload_botskill", "" },
	{ "gui_overload_BotSelection", "1" },
	{ "gui_overload_BotCount", "4" },
	{ "gui_overload_BotChange", "0" },
	{ "gui_overload_OpenSlotCount", "99" },
	{ "gui_overload_BotSkillType", "0" },
	{ "gui_overload_BotSkillValue", "0" },
	{ "gui_overload_BotSkillBias", "0" },
	{ "gui_overload_PlayerJoinAs", "0" },
	{ "gui_overload_TeamSwapped", "0" },
	{ "gui_overload_hostname", "OpenSandbox Overload" },
	{ "gui_overload_ForceRespawn", "0" },
	{ "gui_overload_AutoJoin", "0" },
	{ "gui_overload_TeamBalance", "0" },
	{ "gui_overload_itemGroups", "" },
	{ "gui_overload_itemsHidden", "" },
	{ "gui_overload_WarmUp", "20" },
	{ "gui_overload_doWarmUp", "0" },
	{ "gui_overload_friendly", "0" },
	{ "gui_overload_capturelimit", "0" },
	{ "gui_overload_timelimit", "20" },
	{ "gui_overload_weaponrespawn", "5" },
	{ "gui_overload_viewdistance", "0" },
	
	{ "gui_harvester_capturetype", "0" },
	{ "gui_harvester_timetype", "0" },
	{ "gui_harvester_customcapturelimits", "" },
	{ "gui_harvester_customtimelimits", "" },
	{ "gui_harvester_maplist", "" },
	{ "gui_harvester_maplistexclude", "" },
	{ "gui_harvester_MapRepeat", "1" },
	{ "gui_harvester_MapSource", "0" },
	{ "gui_harvester_RandomMapCount", "5" },
	{ "gui_harvester_RandomMapType", "0" },
	{ "gui_harvester_slottype", "" },
	{ "gui_harvester_botname", "" },
	{ "gui_harvester_botskill", "" },
	{ "gui_harvester_BotSelection", "1" },
	{ "gui_harvester_BotCount", "4" },
	{ "gui_harvester_BotChange", "0" },
	{ "gui_harvester_OpenSlotCount", "99" },
	{ "gui_harvester_BotSkillType", "0" },
	{ "gui_harvester_BotSkillValue", "0" },
	{ "gui_harvester_BotSkillBias", "0" },
	{ "gui_harvester_PlayerJoinAs", "0" },
	{ "gui_harvester_TeamSwapped", "0" },
	{ "gui_harvester_hostname", "OpenSandbox Harvester" },
	{ "gui_harvester_ForceRespawn", "0" },
	{ "gui_harvester_AutoJoin", "0" },
	{ "gui_harvester_TeamBalance", "0" },
	{ "gui_harvester_itemGroups", "" },
	{ "gui_harvester_itemsHidden", "" },
	{ "gui_harvester_WarmUp", "20" },
	{ "gui_harvester_doWarmUp", "0" },
	{ "gui_harvester_friendly", "0" },
	{ "gui_harvester_capturelimit", "0" },
	{ "gui_harvester_timelimit", "20" },
	{ "gui_harvester_weaponrespawn", "5" },
	{ "gui_harvester_viewdistance", "0" },
	
	{ "gui_elim_capturetype", "0" },
	{ "gui_elim_timetype", "0" },
	{ "gui_elim_customcapturelimits", "" },
	{ "gui_elim_customtimelimits", "" },
	{ "gui_elim_maplist", "" },
	{ "gui_elim_maplistexclude", "" },
	{ "gui_elim_MapRepeat", "1" },
	{ "gui_elim_MapSource", "0" },
	{ "gui_elim_RandomMapCount", "5" },
	{ "gui_elim_RandomMapType", "0" },
	{ "gui_elim_slottype", "" },
	{ "gui_elim_botname", "" },
	{ "gui_elim_botskill", "" },
	{ "gui_elim_BotSelection", "1" },
	{ "gui_elim_BotCount", "4" },
	{ "gui_elim_BotChange", "0" },
	{ "gui_elim_OpenSlotCount", "99" },
	{ "gui_elim_BotSkillType", "0" },
	{ "gui_elim_BotSkillValue", "0" },
	{ "gui_elim_BotSkillBias", "0" },
	{ "gui_elim_PlayerJoinAs", "0" },
	{ "gui_elim_TeamSwapped", "0" },
	{ "gui_elim_hostname", "OpenSandbox Elimination" },
	{ "gui_elim_ForceRespawn", "0" },
	{ "gui_elim_AutoJoin", "0" },
	{ "gui_elim_TeamBalance", "0" },
	{ "gui_elim_itemGroups", "" },
	{ "gui_elim_itemsHidden", "" },
	{ "gui_elim_WarmUp", "20" },
	{ "gui_elim_doWarmUp", "0" },
	{ "gui_elim_friendly", "0" },
	{ "gui_elim_capturelimit", "0" },
	{ "gui_elim_timelimit", "20" },
	{ "gui_elim_weaponrespawn", "5" },
	{ "gui_elim_viewdistance", "0" },
	
	{ "gui_ctfelim_capturetype", "0" },
	{ "gui_ctfelim_timetype", "0" },
	{ "gui_ctfelim_customcapturelimits", "" },
	{ "gui_ctfelim_customtimelimits", "" },
	{ "gui_ctfelim_maplist", "" },
	{ "gui_ctfelim_maplistexclude", "" },
	{ "gui_ctfelim_MapRepeat", "1" },
	{ "gui_ctfelim_MapSource", "0" },
	{ "gui_ctfelim_RandomMapCount", "5" },
	{ "gui_ctfelim_RandomMapType", "0" },
	{ "gui_ctfelim_slottype", "" },
	{ "gui_ctfelim_botname", "" },
	{ "gui_ctfelim_botskill", "" },
	{ "gui_ctfelim_BotSelection", "1" },
	{ "gui_ctfelim_BotCount", "4" },
	{ "gui_ctfelim_BotChange", "0" },
	{ "gui_ctfelim_OpenSlotCount", "99" },
	{ "gui_ctfelim_BotSkillType", "0" },
	{ "gui_ctfelim_BotSkillValue", "0" },
	{ "gui_ctfelim_BotSkillBias", "0" },
	{ "gui_ctfelim_PlayerJoinAs", "0" },
	{ "gui_ctfelim_TeamSwapped", "0" },
	{ "gui_ctfelim_hostname", "OpenSandbox CTF Elimination" },
	{ "gui_ctfelim_ForceRespawn", "0" },
	{ "gui_ctfelim_AutoJoin", "0" },
	{ "gui_ctfelim_TeamBalance", "0" },
	{ "gui_ctfelim_itemGroups", "" },
	{ "gui_ctfelim_itemsHidden", "" },
	{ "gui_ctfelim_WarmUp", "20" },
	{ "gui_ctfelim_doWarmUp", "0" },
	{ "gui_ctfelim_friendly", "0" },
	{ "gui_ctfelim_capturelimit", "0" },
	{ "gui_ctfelim_timelimit", "20" },
	{ "gui_ctfelim_weaponrespawn", "5" },
	{ "gui_ctfelim_viewdistance", "0" },
	
	{ "gui_lms_fragtype", "0" },
	{ "gui_lms_timetype", "0" },
	{ "gui_lms_customcapturelimits", "" },
	{ "gui_lms_customtimelimits", "" },
	{ "gui_lms_maplist", "" },
	{ "gui_lms_maplistexclude", "" },
	{ "gui_lms_MapRepeat", "1" },
	{ "gui_lms_MapSource", "0" },
	{ "gui_lms_RandomMapCount", "5" },
	{ "gui_lms_RandomMapType", "0" },
	{ "gui_lms_slottype", "" },
	{ "gui_lms_botname", "" },
	{ "gui_lms_botskill", "" },
	{ "gui_lms_BotSelection", "1" },
	{ "gui_lms_BotCount", "4" },
	{ "gui_lms_BotChange", "0" },
	{ "gui_lms_OpenSlotCount", "99" },
	{ "gui_lms_BotSkillType", "0" },
	{ "gui_lms_BotSkillValue", "0" },
	{ "gui_lms_BotSkillBias", "0" },
	{ "gui_lms_PlayerJoinAs", "0" },
	{ "gui_lms_TeamSwapped", "0" },
	{ "gui_lms_hostname", "OpenSandbox LMS" },
	{ "gui_lms_ForceRespawn", "0" },
	{ "gui_lms_AutoJoin", "0" },
	{ "gui_lms_TeamBalance", "0" },
	{ "gui_lms_itemGroups", "" },
	{ "gui_lms_itemsHidden", "" },
	{ "gui_lms_WarmUp", "20" },
	{ "gui_lms_doWarmUp", "0" },
	{ "gui_lms_friendly", "0" },
	{ "gui_lms_capturelimit", "0" },
	{ "gui_lms_timelimit", "20" },
	{ "gui_lms_weaponrespawn", "5" },
	{ "gui_lms_viewdistance", "0" },
	
	{ "gui_doubledom_capturetype", "0" },
	{ "gui_doubledom_timetype", "0" },
	{ "gui_doubledom_customcapturelimits", "" },
	{ "gui_doubledom_customtimelimits", "" },
	{ "gui_doubledom_maplist", "" },
	{ "gui_doubledom_maplistexclude", "" },
	{ "gui_doubledom_MapRepeat", "1" },
	{ "gui_doubledom_MapSource", "0" },
	{ "gui_doubledom_RandomMapCount", "5" },
	{ "gui_doubledom_RandomMapType", "0" },
	{ "gui_doubledom_slottype", "" },
	{ "gui_doubledom_botname", "" },
	{ "gui_doubledom_botskill", "" },
	{ "gui_doubledom_BotSelection", "1" },
	{ "gui_doubledom_BotCount", "4" },
	{ "gui_doubledom_BotChange", "0" },
	{ "gui_doubledom_OpenSlotCount", "99" },
	{ "gui_doubledom_BotSkillType", "0" },
	{ "gui_doubledom_BotSkillValue", "0" },
	{ "gui_doubledom_BotSkillBias", "0" },
	{ "gui_doubledom_PlayerJoinAs", "0" },
	{ "gui_doubledom_TeamSwapped", "0" },
	{ "gui_doubledom_hostname", "OpenSandbox Double Domination" },
	{ "gui_doubledom_ForceRespawn", "0" },
	{ "gui_doubledom_AutoJoin", "0" },
	{ "gui_doubledom_TeamBalance", "0" },
	{ "gui_doubledom_itemGroups", "" },
	{ "gui_doubledom_itemsHidden", "" },
	{ "gui_doubledom_WarmUp", "20" },
	{ "gui_doubledom_doWarmUp", "0" },
	{ "gui_doubledom_friendly", "0" },
	{ "gui_doubledom_capturelimit", "0" },
	{ "gui_doubledom_timelimit", "20" },
	{ "gui_doubledom_weaponrespawn", "5" },
	{ "gui_doubledom_viewdistance", "0" },
	
	{ "gui_domination_capturetype", "0" },
	{ "gui_domination_timetype", "0" },
	{ "gui_domination_customcapturelimits", "" },
	{ "gui_domination_customtimelimits", "" },
	{ "gui_domination_maplist", "" },
	{ "gui_domination_maplistexclude", "" },
	{ "gui_domination_MapRepeat", "1" },
	{ "gui_domination_MapSource", "0" },
	{ "gui_domination_RandomMapCount", "5" },
	{ "gui_domination_RandomMapType", "0" },
	{ "gui_domination_slottype", "" },
	{ "gui_domination_botname", "" },
	{ "gui_domination_botskill", "" },
	{ "gui_domination_BotSelection", "1" },
	{ "gui_domination_BotCount", "4" },
	{ "gui_domination_BotChange", "0" },
	{ "gui_domination_OpenSlotCount", "99" },
	{ "gui_domination_BotSkillType", "0" },
	{ "gui_domination_BotSkillValue", "0" },
	{ "gui_domination_BotSkillBias", "0" },
	{ "gui_domination_PlayerJoinAs", "0" },
	{ "gui_domination_TeamSwapped", "0" },
	{ "gui_domination_hostname", "OpenSandbox Domination" },
	{ "gui_domination_ForceRespawn", "0" },
	{ "gui_domination_AutoJoin", "0" },
	{ "gui_domination_TeamBalance", "0" },
	{ "gui_domination_itemGroups", "" },
	{ "gui_domination_itemsHidden", "" },
	{ "gui_domination_WarmUp", "20" },
	{ "gui_domination_doWarmUp", "0" },
	{ "gui_domination_friendly", "0" },
	{ "gui_domination_capturelimit", "0" },
	{ "gui_domination_timelimit", "20" },
	{ "gui_domination_weaponrespawn", "5" },
	{ "gui_domination_viewdistance", "0" },
};


static const int gui_cvarTableSize = sizeof(gui_cvarTable)/sizeof(gui_cvarTable[0]);


typedef struct {
	const char* from;
	const char* to;
	const char* init;
} guiCvarConversion_t;




// list of cvars that we need to convert to our "unified" data type
const guiCvarConversion_t gui_cvarImport[] = {
	{ "ui_ffa_fraglimit", "gui_ffa_fraglimit", "0" },
	{ "ui_ffa_timelimit", "gui_ffa_timelimit", "0" },

	{ "ui_tourney_fraglimit", "gui_tourney_fraglimit", "0" },
	{ "ui_tourney_timelimit", "gui_tourney_timelimit", "0" },

	{ "ui_team_friendly", "gui_team_friendly", "0" },
	{ "ui_team_fraglimit", "gui_team_fraglimit", "0" },
	{ "ui_team_timelimit", "gui_team_timelimit", "0" },

	{ "ui_ctf_friendly", "gui_ctf_friendly", "0" },
	{ "ui_ctf_capturelimit", "gui_ctf_capturelimit", "0" },
	{ "ui_ctf_timelimit", "gui_ctf_timelimit", "0" },

	{ "g_weaponrespawn", "gui_ffa_weaponrespawn", "5" },
	{ "g_weaponrespawn", "gui_tourney_weaponrespawn", "5" },
	{ "g_weaponTeamrespawn", "gui_team_weaponrespawn", "30" },
	{ "g_weaponrespawn", "gui_ctf_weaponrespawn", "5" }
};


const int gui_cvarImportSize = sizeof(gui_cvarImport)/sizeof(gui_cvarImport[0]);



#define MAX_CVAR_DATA (30*1024)
#define GUI_SKIRMISH_DATAFILE "ui.dat"

static qboolean skirmishCvarLoaded = qfalse;
static char skirmishCvarData[MAX_CVAR_DATA];






/*
=================
GUI_ConvertOldCvar
=================
*/
void GUI_ConvertOldCvar( char* cvar, int size)
{
	int i;

	for (i = 0; i < gui_cvarImportSize; i++) {
		if (!Q_stricmp(cvar, gui_cvarImport[i].from)) {
			Q_strncpyz(cvar, gui_cvarImport[i].to, size);

			if (uis.debug)
				Com_Printf("Cvar converted: %s\n", cvar);
			return;
		}
	}
}




/*
=================
GUI_SkirmishCvarExists
=================
*/
qboolean GUI_SkirmishCvarExists( char* base, const char* var_name)
{
	int i;
	char name[64];

	if (!var_name || !*var_name)
		return qfalse;

	if (base) {
		Q_strncpyz(name, va(base, var_name), 64);
	}
	else {
		Q_strncpyz(name, var_name, 64);
	}

	for (i = 0; i < gui_cvarTableSize; i++) {
		if (!Q_stricmp(gui_cvarTable[i].cvarName, name))
			return qtrue;
	}

	return qfalse;
}




/*
=================
GUI_GetSkirmishCvar
=================
*/
qboolean GUI_GetSkirmishCvar( char* base, const char* var_name, char* buffer, int buflen )
{
	char *p, *pnext;
	int len;
	char *str, *str2;
	char name[64];

	if (!var_name || !*var_name || !buffer)
		return qfalse;

	if (!skirmishCvarLoaded)
		GUI_StartServer_LoadSkirmishCvars();

	if (base) {
		Q_strncpyz(name, va(base, var_name), 64);
	}
	else {
		Q_strncpyz(name, var_name, 64);
	}

	len = strlen(name) + 1;
	p = skirmishCvarData;
	while (*p) {
		pnext = p + strlen(p) + 1;
		if (!Q_stricmp(name, COM_Parse(&p))) {
			break;
		}

		p = pnext;
	}

	buffer[0] = '\0';
	if (!*p) {
//		Com_Printf("Cvar not found: %s\n",name);
		return qfalse;
	}

	// return string inside quotes
	Q_strncpyz(buffer, COM_Parse(&p), buflen);

	return qtrue;
}




/*
=================
GUI_GetSkirmishCvarInt
=================
*/
int GUI_GetSkirmishCvarInt( char* base, const char* name )
{
	char buf[32];

	GUI_GetSkirmishCvar(base, name, buf, 32);
	return atoi(buf);
}




/*
=================
GUI_GetSkirmishCvarIntClamp
=================
*/
int GUI_GetSkirmishCvarIntClamp( int min, int max, char* base, const char* name )
{
	int value;
	char buf[32];

	GUI_GetSkirmishCvar(base, name, buf, 32);

	value = atoi(buf);
	if (value < min)
		value = min;
	if (value > max)
		value = max;

	return value;
}





/*
=================
GUI_SetSkirmishCvar
=================
*/
void GUI_SetSkirmishCvar( char* base, const char* var_name, const char* string )
{
	char *p;
	char *old;
	char *newstr;
	int len, next, lenmove, oldlen, addlen;
	char name[64];

	if (!var_name || !*var_name || !string)
		return;

	if (base) {
		Q_strncpyz(name, va(base, var_name), 64);
	}
	else {
		Q_strncpyz(name, var_name, 64);
	}

	len = strlen(name)+1;
	p = skirmishCvarData;
	old = NULL;

	// do search
	while (*p) {
		next = strlen(p)+1;
		// prevent premature termination on longer name string
		if (!old && (next > len) && p[len - 1] == ' ' && Q_stricmpn(p, name, len - 1) == 0) {
			old = p;
		}

		p += next;
	}

	// delete old entry
	if (old) {
		oldlen = strlen(old) + 1;
		lenmove = p -(old + oldlen) + 1;
		memmove(old, old + oldlen, lenmove);

		p -= oldlen;
	}

	// check for overflow - bad news
	newstr = va("%s \"%s\"", name, string);
	addlen = strlen(newstr) + 1;
	if (p - skirmishCvarData + addlen >= MAX_CVAR_DATA) {
		Com_Printf("Skirmish Cvar overflow, cvar %s lost\n", name);
		return;
	}

	// add to end, keeping \0\0 integrity
	Q_strncpyz(p, newstr, addlen);
	p[addlen] = '\0';

//	Com_Printf("Cvar wrote: %s\n", newstr);
}




/*
=================
GUI_SetSkirmishCvarInt
=================
*/
void GUI_SetSkirmishCvarInt( char* base, const char* name, int value)
{
	char buf[32];

	Q_strncpyz(buf, va("%i", value), 32);
	GUI_SetSkirmishCvar(base, name, buf);
}



/*
=================
GUI_StartServer_MergeSkirmishCvars

Convert from text format to internal NUL buffer terminated

May have been edited by a program that changes line terminator formats
so we attempt to recover from this
=================
*/
static void GUI_StartServer_MergeSkirmishCvars( char* cvarList )
{
	char *p;
	char varname[MAX_TOKEN_CHARS];
	char* data;

	p = cvarList;

	while (*p) {
		Q_strncpyz( &varname[0], COM_Parse(&p), MAX_TOKEN_CHARS);
		if (!varname[0])
			break;

		data = COM_Parse(&p);

		GUI_SetSkirmishCvar(NULL, varname, data);

	};
}



/*
=================
GUI_StartServer_SaveSkirmishCvars
=================
*/
void GUI_StartServer_SaveSkirmishCvars( void )
{
	char *p;
	fileHandle_t file;
	int len;

	p = skirmishCvarData;
	while (*p) {
		p+= strlen(p);
		*p++ = '\r';
	}

	len = strlen(skirmishCvarData);
	trap_FS_FOpenFile(GUI_SKIRMISH_DATAFILE, &file, FS_WRITE);
	trap_FS_Write(skirmishCvarData, len, file);
	trap_FS_FCloseFile(file);

	p = skirmishCvarData;
	while (*p) {
		if (*p == '\r')
			*p = '\0';
		p++;
	}

//	Com_Printf("Wrote %s, %i bytes\n", GUI_SKIRMISH_DATAFILE, len);
}



/*
=================
GUI_StartServer_LoadSkirmishCvars
=================
*/
void GUI_StartServer_LoadSkirmishCvars( void )
{
	int i;
	vmCvar_t cvar;
	fileHandle_t file;
	int len;
	char newCvars[MAX_CVAR_DATA];

	memset(skirmishCvarData, 1, MAX_CVAR_DATA);
	skirmishCvarLoaded = qfalse;
	skirmishCvarData[0] = '\0';

	// load from cvars in memory or default values on list.
	// The flags marked here don't override the existing ARCHIVE flag,
	// which is set when the cvar is loaded from q3config.cfg
	for (i = 0; i < gui_cvarTableSize; i++) {
		trap_Cvar_Register(&cvar, gui_cvarTable[i].cvarName, gui_cvarTable[i].defaultString, CVAR_TEMP|CVAR_USER_CREATED);
		GUI_SetSkirmishCvar(NULL, gui_cvarTable[i].cvarName, cvar.string);
	}

	// now override cvars that we need to import
	for (i = 0; i < gui_cvarImportSize; i++) {
		trap_Cvar_Register(&cvar, gui_cvarImport[i].from, gui_cvarImport[i].init, CVAR_TEMP|CVAR_USER_CREATED);
		GUI_SetSkirmishCvar(NULL, gui_cvarImport[i].to, cvar.string);
	}

	skirmishCvarLoaded = qtrue;

	// load cvars from file, and merge them with this prepared list
	len = trap_FS_FOpenFile(GUI_SKIRMISH_DATAFILE, &file, FS_READ);

	if (!(len < MAX_CVAR_DATA - 1)) {
		Com_Printf(GUI_SKIRMISH_DATAFILE" is too large, skirmish reset to default.\n");
		trap_FS_FCloseFile(file);
		return;
	}
	else if (len <= 0) {
		Com_Printf(GUI_SKIRMISH_DATAFILE" doesn't exist, imported default cvars.\n");
		trap_FS_FCloseFile(file);
		return;
	}

	trap_FS_Read(newCvars, len, file);
	trap_FS_FCloseFile(file);

	newCvars[len] = '\0';
	skirmishCvarLoaded = qtrue;

	GUI_StartServer_MergeSkirmishCvars(newCvars);
	GUI_StartServer_SaveSkirmishCvars();
}





/*
==========================================================================

MISC FUNCTIONS

==========================================================================
*/



/*
=================
GUI_StartServer_RegisterDisableCvars
=================
*/
void GUI_StartServer_RegisterDisableCvars(qboolean init)
{
	int i;
	char* disable;

	for (i = 0; i < ITEM_COUNT; i++)
	{
		disable = va("disable_%s", server_itemlist[i].mapitem);
		if (init)
			trap_Cvar_Register(NULL, disable, "0", 0);
		else
			trap_Cvar_Set(disable, "0");
	}
}



/*
=================
StartServer_CanFight
=================
*/
qboolean StartServer_CanFight(void)
{
   int bot_count = 0;

   // number of maps
	if (StartServer_IsRandomGeneratedMapList(s_scriptdata.map.listSource)) {
		if (s_scriptdata.map.SourceCount == 0)
			return qfalse;
	}
	else {
		if (s_scriptdata.map.num_maps == 0)
			return qfalse;
	}

   // number of bots
   if (s_scriptdata.bot.typeSelect == BOTTYPE_SELECTARENASCRIPT) {
      if (StartServer_ValidBotCount() == 0) {
         return qfalse;
      }
   }

	return qtrue;
}



/*
=================
UI_SaveMultiArray
=================
*/
void UI_SaveMultiArray(char* base, const char* key, String_Callback callback, int count, int size, char newnull)
{
	char buf[MAX_STRING_CHARS];
	int i, last;
	char* arraychar;

	if (size * count >= MAX_STRING_CHARS)
	{
		trap_Error( "size*step >= MAX_STRING_CHARS" );
		return;
	}

	last = 0;
	for (i = 0; i < count; i++)
	{
		arraychar = callback(i);
		Q_strncpyz(&buf[last], arraychar, size);
		last += strlen(arraychar);
		buf[last++] = newnull;
	}

	buf[last] = '\0';
	GUI_SetSkirmishCvar( base, key, buf );
}



/*
=================
UI_LoadMultiArray
=================
*/
void UI_LoadMultiArray(char* base, const char* key, String_Callback callback, int count, int size, char newnull)
{
	char buf[MAX_STRING_CHARS];
	char* arraychar;
	int len, i, c;

	GUI_GetSkirmishCvar( base, key, buf, MAX_STRING_CHARS);

	len = strlen(buf);
	for (i = 0; i < len; i++)
		if (buf[i] == newnull)
			buf[i] = '\0';

	i = 0;
	c = 0;
	while (i < len && c < count)
	{
		arraychar = callback(c);
		Q_strncpyz(arraychar, &buf[i], size);
		i += strlen( &buf[i] ) + 1;

		c++;
	}

	// clear remaining elements
	while (c < count)
	{
		arraychar = callback(c);
		memset(arraychar, 0, size);
		c++;
	}
}



/*
==========================================================================

LOADING AND SAVING OF MAP SCRIPT DATA

==========================================================================
*/



// additional map type strings are set in ui_startserver_custommaps.c
const char* randommaptype_items[MAP_RND_MAX + MAX_MAPTYPES + 1] = {
	"(Any)", // MAP_RND_ANY
	"(OpenSandbox only)", // MAP_RND_ID
	"(Non OpenSandbox only)", // MAP_RND_NONID
	"(Bias OpenSandbox)", // MAP_RND_BIASID
	"(Bias Non OpenSandbox)", // MAP_RND_BIASNONID
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0
};
const char* randommaptype_itemsru[MAP_RND_MAX + MAX_MAPTYPES + 1] = {
	"(Любые)", // MAP_RND_ANY
	"(OpenSandbox только)", // MAP_RND_ID
	"(Не OpenSandbox только)", // MAP_RND_NONID
	"(Разброс OpenSandbox)", // MAP_RND_BIASID
	"(Разброс не OpenSandbox)", // MAP_RND_BIASNONID
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
	0
};





/*
=================
StartServer_IsIdMap
=================
*/
qboolean StartServer_IsIdMap(const char* mapname)
{
	const char* const *ptr;

	// check list of idmaps
	ptr = idmap_list;
	while (*ptr) {
		if (!Q_stricmp(*ptr, mapname))
		{
			return qtrue;
		}
		ptr++;
	}

	return qfalse;
}





/*
=================
StartServer_IsRandomGeneratedMapList

Map generation will be from a list of randomly selected map names,
not a user list of map name
=================
*/
qboolean StartServer_IsRandomGeneratedMapList( int type )
{
	if (type == MAP_MS_RANDOM || type == MAP_MS_RANDOMEXCLUDE)
		return qtrue;
	return qfalse;
}





/*
=================
StartServer_MapPageCount
=================
*/
int StartServer_MapPageCount(void)
{
	int count;

	count = 1 + ( s_scriptdata.map.num_maps / NUMMAPS_PERPAGE );
	if (count > MAX_MAPPAGES)
		count = MAX_MAPPAGES;
	return count;
}



/*
=================
StartServer_RangeClipMapIndex
=================
*/
int StartServer_RangeClipMapIndex(int index)
{
	if (index < 0)
		return 0;

	if (index > s_scriptdata.map.num_maps )
		index = s_scriptdata.map.num_maps;

	if (index == MAX_NUMMAPS)
		return MAX_NUMMAPS - 1;

	return index;
}






/*
=================
SSMP_ShortName_Callback
=================
*/
static char* SSMP_ShortName_Callback(int index)
{
	return s_scriptdata.map.data[index].shortName;
}



/*
=================
SSMP_FragLimit_Callback
=================
*/
static char* SSMP_FragLimit_Callback(int index)
{
	return s_scriptdata.map.data[index].fragLimit;
}


/*
=================
SSMP_TimeLimit_Callback
=================
*/
static char* SSMP_TimeLimit_Callback(int index)
{
	return s_scriptdata.map.data[index].timeLimit;
}




/*
=================
StartServer_MapPage_LoadMapList

must be called after s_scriptdata.map.type has been set
=================
*/
void StartServer_LoadMapList(void)
{
	char* s;
	char* ml;

	ml = "maplist";
	if (s_scriptdata.map.listSource == MAP_MS_RANDOMEXCLUDE)
		ml = "maplistexclude";

	s = gametype_cvar_base[s_scriptdata.gametype];
	UI_LoadMultiArray(s, ml, SSMP_ShortName_Callback, MAX_NUMMAPS, SHORTMAP_BUFFER, ';');
}




/*
=================
StartServer_SwapMaps
=================
*/
qboolean StartServer_SwapMaps(int from, int to)
{
	static mapdata_t tmp;

	if (from < 0 || to < 0 || from == to)
		return qfalse;

	if (from >= s_scriptdata.map.num_maps || to >= s_scriptdata.map.num_maps)
		return qfalse;

	memcpy(&tmp, &s_scriptdata.map.data[from], sizeof (mapdata_t));
	memcpy(&s_scriptdata.map.data[from], &s_scriptdata.map.data[to], sizeof (mapdata_t));
	memcpy(&s_scriptdata.map.data[to], &tmp, sizeof (mapdata_t));

	return qtrue;
}





/*
=================
StartServer_StoreMap

Places map data into the array, overwriting a previous entry
=================
*/
void StartServer_StoreMap(int pos, int arena)
{
	const char* info;
	char* shortname;
	char* longname;
	int len;
	int base;

	pos = StartServer_RangeClipMapIndex(pos);
	info = UI_GetArenaInfoByNumber(arena);

	shortname = Info_ValueForKey(info, "map");
	len = strlen(shortname) + 1;
	Q_strncpyz(s_scriptdata.map.data[pos].shortName, shortname, len);

	longname = Info_ValueForKey(info, "longname");
	if (!longname || !*longname)
		longname = shortname;

	len = strlen(longname) + 1;
	Q_strncpyz(s_scriptdata.map.data[pos].longName, longname, len);

	// increase map count if we put map into a previously empty slot
	// set frag/time limits too
	if (pos == s_scriptdata.map.num_maps) {
		s_scriptdata.map.num_maps++;
		Q_strncpyz(s_scriptdata.map.data[pos].fragLimit, va("%i", s_scriptdata.map.fragLimit), MAX_LIMIT_BUF);
		Q_strncpyz(s_scriptdata.map.data[pos].timeLimit, va("%i", s_scriptdata.map.timeLimit), MAX_LIMIT_BUF);
	}
}



/*
=================
StartServer_InsertMap

Creates an empty slot for map data to be added
=================
*/
void StartServer_InsertMap(int pos, int arena)
{
	int last;
	int i;

	// drop any maps that try to overflow
	if (pos > MAX_NUMMAPS - 1)
		return;

	pos = StartServer_RangeClipMapIndex(pos);

	// quietly drop last map on list if we are inserting earlier
	last = s_scriptdata.map.num_maps - 1;
	if (last == MAX_NUMMAPS - 1 )
		last--;
	else
		s_scriptdata.map.num_maps++;

	// move maps up one slot
	for (i = last; i >= pos; i--)
		memcpy(&s_scriptdata.map.data[i+1], &s_scriptdata.map.data[i], sizeof(mapdata_t));

	StartServer_StoreMap(pos, arena);
	Q_strncpyz(s_scriptdata.map.data[pos].fragLimit, va("%i", s_scriptdata.map.fragLimit), MAX_LIMIT_BUF);
	Q_strncpyz(s_scriptdata.map.data[pos].timeLimit, va("%i", s_scriptdata.map.timeLimit), MAX_LIMIT_BUF);
}



/*
=================
StartServer_DeleteMap
=================
*/
void StartServer_DeleteMap(int index)
{
	int lines;
	int i;

	if (index < 0 || index >= MAX_NUMMAPS)
		return;

	lines = (MAX_NUMMAPS - index - 1);
	if (lines) {
		for (i =0 ; i < lines; i++)
			memcpy(&s_scriptdata.map.data[index + i], &s_scriptdata.map.data[index + i + 1], sizeof(mapdata_t));
	}

	// zero final element only
	memset(&s_scriptdata.map.data[MAX_NUMMAPS - 1], 0, sizeof(mapdata_t));

	if (index < s_scriptdata.map.num_maps)
		s_scriptdata.map.num_maps--;
}






/*
=================
StartServer_RefreshMapNames
=================
*/
void StartServer_RefreshMapNames(void)
{
	int i, j, count;
	const char	*info;
	char* arena_mapname;

	i = 0;
	count = UI_GetNumArenas();
	while (i < MAX_NUMMAPS && s_scriptdata.map.data[i].shortName[0]) {
		for( j = 0; j < count; j++ ) {
			info = UI_GetArenaInfoByNumber( j );

			if (!info)
				continue;

			arena_mapname = Info_ValueForKey( info, "map");
			if (!arena_mapname || arena_mapname[0] == '\0') {
				continue;
			}

			if (!Q_stricmp(s_scriptdata.map.data[i].shortName, arena_mapname))
			{
				Q_strncpyz( s_scriptdata.map.data[i].longName, Info_ValueForKey( info, "longname"), LONGMAP_BUFFER);
				break;
			}
		}
		// map not found, quietly delete it from list
		if (j == count) {
			StartServer_DeleteMap(i);
			continue;
		}

		i++;
	}

	s_scriptdata.map.num_maps = i;
}




/*
=================
StartServer_GetArenaFragLimit
=================
*/
static const char* StartServer_GetArenaFragLimit( int map )
{
	static char fraglimit[16];
	const char* info;
	const char* infofrag;

	info = UI_GetArenaInfoByMap(s_scriptdata.map.data[map].shortName);
	infofrag = Info_ValueForKey(info, "fraglimit");

	if (infofrag[0]) {
		Q_strncpyz(fraglimit, infofrag, 16);
	}
	else {
		Q_strncpyz(fraglimit, va("%i", s_scriptdata.map.fragLimit), 16);
	}
		
	return fraglimit;
}




/*
=================
StartServer_MapDoAction
=================
*/
void StartServer_MapDoAction( int src, int dest, int page, int selected )
{
	int i;
	int pageindex;
	const char* fragsrc;
	const char* timesrc;

	pageindex = page * NUMMAPS_PERPAGE;

	//
	// actions using src
	//

	if (src == MAP_CF_CLEARALL)
	{
		while (s_scriptdata.map.num_maps)
		{
			StartServer_DeleteMap(0);
		}
		return;
	}

	if (src == MAP_CF_CLEARPAGE)
	{
		for (i = 0; i < NUMMAPS_PERPAGE; i++)
			StartServer_DeleteMap(pageindex);
		return;
	}

	//
	// actions that combine src and dest
	//

	if (src == MAP_CF_ARENASCRIPT) {
		switch (dest) {
			case MAP_CT_SELECTED:
				if (selected >= 0) {
					fragsrc = StartServer_GetArenaFragLimit(pageindex + selected);
					Q_strncpyz(s_scriptdata.map.data[pageindex + selected].fragLimit, fragsrc, MAX_LIMIT_BUF);
				}
				break;

			case MAP_CT_PAGE:
				for (i = 0; i < NUMMAPS_PERPAGE; i++) {
					fragsrc = StartServer_GetArenaFragLimit(pageindex + i);
					Q_strncpyz(s_scriptdata.map.data[pageindex + i].fragLimit, fragsrc, MAX_LIMIT_BUF);
				}
				break;

			case MAP_CT_ALL:
				for (i = 0; i < s_scriptdata.map.num_maps; i++) {
					fragsrc = StartServer_GetArenaFragLimit(i);
					Q_strncpyz(s_scriptdata.map.data[i].fragLimit, fragsrc, MAX_LIMIT_BUF);
				}

				break;
		}
		return;
	}

	// there's some overlap between the copying of time and frag limits
	// "flow through" from the time to the frag code is expected
	// ANY OTHER CUSTOM VALUES SHOULD APPEAR BEFORE THIS CODE

	// copy time
	timesrc = va("%i", s_scriptdata.map.timeLimit);
	if (src != MAP_CF_FRAG) {
		switch (dest) {
			case MAP_CT_SELECTED:
				if (selected >= 0)
					Q_strncpyz(s_scriptdata.map.data[pageindex + selected].timeLimit, timesrc, MAX_LIMIT_BUF);
				break;
			case MAP_CT_PAGE:
				for (i = 0; i < NUMMAPS_PERPAGE; i++)
					Q_strncpyz(s_scriptdata.map.data[pageindex + i].timeLimit, timesrc, MAX_LIMIT_BUF);
				break;
			case MAP_CT_ALL:
				for (i = 0; i < s_scriptdata.map.num_maps; i++)
					Q_strncpyz(s_scriptdata.map.data[i].timeLimit, timesrc, MAX_LIMIT_BUF);

				break;
		}
	}


	// copy frags/caps
	fragsrc = va("%i", s_scriptdata.map.fragLimit);
	if (src != MAP_CF_TIME) {
		switch (dest) {
			case MAP_CT_SELECTED:
				if (selected >= 0)
					Q_strncpyz(s_scriptdata.map.data[pageindex + selected].fragLimit, fragsrc, MAX_LIMIT_BUF);
				break;
			case MAP_CT_PAGE:
				for (i = 0; i < NUMMAPS_PERPAGE; i++)
					Q_strncpyz(s_scriptdata.map.data[pageindex + i].fragLimit, fragsrc, MAX_LIMIT_BUF);
				break;
			case MAP_CT_ALL:
				for (i = 0; i < s_scriptdata.map.num_maps; i++)
					Q_strncpyz(s_scriptdata.map.data[i].fragLimit, fragsrc, MAX_LIMIT_BUF);
				break;
		}
	}
}





/*
=================
StartServer_BuildMapDistribution
=================
*/
static void StartServer_BuildMapDistribution(void)
{
	int i, j, count, maptype, gametype;
	const char	*info;
	char* arena_mapname;
	int matchbits, gamebits;

	// set zero
	for (i = 0; i < MAX_GAME_TYPE; i++)
		for (j = 0; j < MAX_MAP_GROUP; j++)
			s_scriptdata.map.TypeCount[i][j] = 0;

	count = UI_GetNumArenas();
	for( i = 0; i < count; i++ ) {
		info = UI_GetArenaInfoByNumber( i );

		if (!info)
			continue;

		arena_mapname = Info_ValueForKey( info, "map");
		if (!arena_mapname || arena_mapname[0] == '\0') {
			continue;
		}

		if (StartServer_IsIdMap(arena_mapname))
			maptype = MAP_GROUP_ID;
		else
			maptype = MAP_GROUP_NONID;

		gamebits = GametypeBits( Info_ValueForKey( info, "type") );
		for (j = 0; j < NUM_GAMETYPES; j++)
		{
			gametype = j;
			matchbits = 1 << gametype;

			if (matchbits & gamebits)
				s_scriptdata.map.TypeCount[ gametype ][maptype]++;
		}
	}
}


/*
=================
StartServer_SaveMapList
=================
*/
void StartServer_SaveMapList(void)
{
	char* s;
	char* s1;

	s1 = "maplist";
	if (s_scriptdata.map.listSource == MAP_MS_RANDOMEXCLUDE)
		s1 = "maplistexclude";

	s = gametype_cvar_base[s_scriptdata.gametype];
	UI_SaveMultiArray(s, s1, SSMP_ShortName_Callback, MAX_NUMMAPS, SHORTMAP_BUFFER, ';');
}








/*
=================
StartServer_LoadMapScriptData

Loads map specific gametype data
=================
*/
static void StartServer_LoadMapScriptData(void)
{
	char buf[64];
	int i, index, num, max;
	char* s;
	char *f, *f2;

	index = 0;
	f = "customfraglimits";
	f2 = "fragtype";

	s = gametype_cvar_base[s_scriptdata.gametype];

	if (s_scriptdata.gametype >= GT_CTF && !(s_scriptdata.gametype == GT_LMS)) {
		s_scriptdata.map.fragLimit = GUI_GetSkirmishCvarIntClamp(0, 999, s, "capturelimit");
		f = "customcapturelimits";
		f2 = "capturetype";
	}
	else {
		s_scriptdata.map.fragLimit = GUI_GetSkirmishCvarIntClamp(0, 99, s, "fraglimit");
	}

	s_scriptdata.map.timeLimit = GUI_GetSkirmishCvarIntClamp(0, 999, s, "timelimit");

	// load custom frag/time values
	UI_LoadMultiArray(s, f, SSMP_FragLimit_Callback, MAX_NUMMAPS, MAX_LIMIT_BUF, ';');
	UI_LoadMultiArray(s, "customtimelimits", SSMP_TimeLimit_Callback, MAX_NUMMAPS, MAX_LIMIT_BUF, ';');

	// load type of frag/time value used to start game (none, default, custom)
	s_scriptdata.map.fragLimitType = GUI_GetSkirmishCvarIntClamp(0, 2, s, f2 );
	s_scriptdata.map.timeLimitType = GUI_GetSkirmishCvarIntClamp(0, 2, s, "timetype" );

	// load map source and repeat info
	s_scriptdata.map.Repeat = GUI_GetSkirmishCvarIntClamp(0, 1, s, "MapRepeat");
	s_scriptdata.map.listSource = GUI_GetSkirmishCvarIntClamp(0, MAP_MS_MAX - 1, s, "MapSource");

	// load maps
	// must be after s_scriptdata.map.type is set
	StartServer_LoadMapList();

	s_scriptdata.map.Repeat = GUI_GetSkirmishCvarIntClamp(0, 1, s, "MapRepeat");
	s_scriptdata.map.SourceCount = GUI_GetSkirmishCvarIntClamp(2, 99, s, "RandomMapCount");

	GUI_GetSkirmishCvar( s, "RandomMapType", buf, 64);
	s_scriptdata.map.SourceType = (int)Com_Clamp(0, MAP_RND_MAX - 1, atoi(buf));	// non-numerical values give zero

	max = StartServer_NumCustomMapTypes();
	for (i = 0; i < max; i++)
	{
		if (!Q_stricmp(buf, randommaptype_items[MAP_RND_MAX + i]))
		{
			s_scriptdata.map.SourceType = MAP_RND_MAX + i;
			break;
		}
	}

	// validate each of the map names
	StartServer_RefreshMapNames();
}



/*
=================
StartServer_SaveMapScriptData

Saves map specific gametype data
=================
*/
static void StartServer_SaveMapScriptData(void)
{
	int type;
	char* s;
	char *f, *f2;

	s = gametype_cvar_base[s_scriptdata.gametype];
	f = "customfraglimits";
	f2 = "fragtype";

	GUI_SetSkirmishCvarInt( s, "timelimit", s_scriptdata.map.timeLimit);
	if (s_scriptdata.gametype >= GT_CTF && !(s_scriptdata.gametype == GT_LMS)) {
		GUI_SetSkirmishCvarInt( s, "capturelimit", s_scriptdata.map.fragLimit);
		f = "customcapturelimits";
		f2 = "capturetype";
	}
	else {
		GUI_SetSkirmishCvarInt( s, "fraglimit", s_scriptdata.map.fragLimit);
	}

	StartServer_SaveMapList();

	GUI_SetSkirmishCvarInt( s, "MapSource", s_scriptdata.map.listSource);

	// save custom frag/time values
	UI_SaveMultiArray(s, f, SSMP_FragLimit_Callback, MAX_NUMMAPS, MAX_LIMIT_BUF, ';');
	UI_SaveMultiArray(s, "customtimelimits", SSMP_TimeLimit_Callback, MAX_NUMMAPS, MAX_LIMIT_BUF, ';');

	// save type of frag/time value used to start game (none, default, custom)
	GUI_SetSkirmishCvarInt( s,f2, s_scriptdata.map.fragLimitType);
	GUI_SetSkirmishCvarInt( s,"timetype", s_scriptdata.map.timeLimitType);

	// save map source and repeat info
	GUI_SetSkirmishCvarInt( s, "MapRepeat", s_scriptdata.map.Repeat);
	GUI_SetSkirmishCvarInt( s, "RandomMapCount", s_scriptdata.map.SourceCount);

	type = s_scriptdata.map.SourceType;
	if (type < MAP_RND_MAX)
		GUI_SetSkirmishCvarInt( s, "RandomMapType", type);
	else
		GUI_SetSkirmishCvar( s, "RandomMapType", randommaptype_items[type]);
}















/*
==========================================================================

LOADING AND SAVING OF BOT SCRIPT DATA

==========================================================================
*/

#define BOT_TMPBUFFER 4

static char botskill_tmpbuffer[PLAYER_SLOTS][BOT_TMPBUFFER];	// tmp used to load/save bot skill values




/*
=================
StartServer_IsBotArenaScript
=================
*/
qboolean StartServer_IsBotArenaScript(int type)
{
	if (type == BOTTYPE_ARENASCRIPT || type == BOTTYPE_RANDOMARENASCRIPT ||
         type == BOTTYPE_SELECTARENASCRIPT)
		return qtrue;

	return qfalse;
}


/*
=================
StartServer_IsRandomBotExclude
=================
*/
qboolean StartServer_IsRandomBotExclude(int type)
{
	if (type == BOTTYPE_RANDOMEXCLUDE || type == BOTTYPE_RANDOMARENASCRIPT)
		return qtrue;

	return qfalse;
}




/*
=================
StartServer_IsRandomBotList
=================
*/
qboolean StartServer_IsRandomBotList( int type )
{
	if (type == BOTTYPE_RANDOM || type == BOTTYPE_RANDOMEXCLUDE || type == BOTTYPE_RANDOMARENASCRIPT)
		return qtrue;
	return qfalse;
}







/*
=================
StartServer_SetBotSkillValue
=================
*/
static void StartServer_SetBotSkillValue(botskill_t* b, int value)
{
	value = (int)Com_Clamp(0 , 999, value);
	b->value = (int)Com_Clamp(0, 4, (value / 100) % 10);
	b->low = (int)Com_Clamp(0, 4, (value / 10) % 10);
	b->high = (int)Com_Clamp(0, 4, value % 10);
}



/*
=================
StartServer_SetBotSkillRangeType

Assumes s_scriptdata.bot.typeSelect is already initialized
=================
*/
void StartServer_SetBotSkillRangeType(int skill)
{
	int i;
	qboolean qbool;

	// wrap skill early if selecting from random
	if ((StartServer_IsRandomBotList(s_scriptdata.bot.typeSelect) || StartServer_IsBotArenaScript(s_scriptdata.bot.typeSelect)) 
		&& skill >= BOTSKILL_CUSTOMSINGLE)
	{
		skill = BOTSKILL_SAME;
	}

	s_scriptdata.bot.skillType = skill;
	if (skill == BOTSKILL_SAME || skill == BOTSKILL_CUSTOMSINGLE) {
		qbool = qfalse;
	}
	else {
		qbool = qtrue;
	}

	s_scriptdata.bot.globalSkill.range = qbool;
	for (i = 0; i < PLAYER_SLOTS; i++)
		s_scriptdata.bot.skill[i].range = qbool;
}


/*
=================
SSBP_BotName_Callback
=================
*/
static char* SSBP_BotName_Callback(int index)
{
	return s_scriptdata.bot.name[index];
}



/*
=================
SSBP_BotBuffer_Callback
=================
*/
static char* SSBP_BotBuffer_Callback(int index)
{
	return botskill_tmpbuffer[index];
}




/*
=================
StartServer_ValidBotCount
=================
*/
int StartServer_ValidBotCount( void )
{
   int count = 0;
   int i;

   for (i = 0; i < PLAYER_SLOTS; i++) {
      if (s_scriptdata.bot.slotType[i] != SLOTTYPE_BOT) {
         continue;
      }
      if (s_scriptdata.bot.name[i][0] == '\0') {
         continue;
      }
      count++;
   }

   return count;
}



/*
=================
StartServer_BotOnSelectionList
=================
*/
qboolean StartServer_BotOnSelectionList( const char *checkName )
{
	int i;

	for (i = 0 ; i < PLAYER_SLOTS; i++) {
		if (s_scriptdata.bot.slotType[i] != SLOTTYPE_BOT)
			continue;

		if (Q_stricmp(checkName, s_scriptdata.bot.name[i]) == 0)
			return qtrue;
	}

	return qfalse;
}





/*
=================
StartServer_DeleteBotSlot
=================
*/
qboolean StartServer_DeleteBotSlot(int index)
{
	int i, count, last;

	if (index < 0 || index >= PLAYER_SLOTS)
		return qfalse;

	// number of slots to move
	count = PLAYER_SLOTS - index - 1;
	if (s_scriptdata.gametype >= GT_TEAM && !(s_scriptdata.gametype == GT_LMS)  && !StartServer_IsRandomBotExclude(s_scriptdata.bot.typeSelect) && index < PLAYER_SLOTS_PERCOL) {
		count -= PLAYER_SLOTS_PERCOL;
	}

	if (count) {
		for (i = index; i < index + count; i++) {
			memcpy(s_scriptdata.bot.name[i], s_scriptdata.bot.name[i + 1], MAX_NAME_LENGTH);
			memcpy(&s_scriptdata.bot.skill[i], &s_scriptdata.bot.skill[i + 1], sizeof(botskill_t));
			s_scriptdata.bot.slotType[i] = s_scriptdata.bot.slotType[i + 1];
		}
	}

	// zero last slot
	last = index + count;
	memset(s_scriptdata.bot.name[last], 0, MAX_NAME_LENGTH);
	s_scriptdata.bot.skill[last].value = 0;
	s_scriptdata.bot.skill[last].low = 0;
	s_scriptdata.bot.skill[last].high = 0;

	s_scriptdata.bot.slotType[last] = SLOTTYPE_EMPTY;

	return qtrue;
}



/*
=================
StartServer_InsertBotSlot
=================
*/
qboolean StartServer_InsertBotSlot(int index)
{
	int i, count;

	if (index < 0 || index >= PLAYER_SLOTS)
		return qfalse;

	// number of slots to move
	count = PLAYER_SLOTS - index - 1;
	if (s_scriptdata.gametype >= GT_TEAM && !(s_scriptdata.gametype == GT_LMS)  && index < PLAYER_SLOTS_PERCOL) {
		count -= PLAYER_SLOTS_PERCOL;
	}

	if (count) {
		for (i = index + count - 1 ; i >= index; i--) {
			// memcpy causes problems when copying "up", so move one entry at a time
			// write this before discovering memmove!
			memcpy(s_scriptdata.bot.name[i + 1], s_scriptdata.bot.name[i], MAX_NAME_LENGTH);
			memcpy(&s_scriptdata.bot.skill[i + 1], &s_scriptdata.bot.skill[i], sizeof(botskill_t));
			s_scriptdata.bot.slotType[i + 1] = s_scriptdata.bot.slotType[i];
		}
	}

	memset(s_scriptdata.bot.name[index], 0, MAX_NAME_LENGTH);
	s_scriptdata.bot.skill[index].value = 0;
	s_scriptdata.bot.skill[index].low = 0;
	s_scriptdata.bot.skill[index].high = 0;

	s_scriptdata.bot.slotType[index] = SLOTTYPE_EMPTY;

	return qtrue;
}



/*
=================
StartServer_SetNamedBot
=================
*/
void StartServer_SetNamedBot(int index, char* name)
{
	qboolean init;

	if (index < 0 || index >= PLAYER_SLOTS || !name)
		return;

	if (s_scriptdata.bot.name[index][0] == '\0') {
		s_scriptdata.bot.skill[index] = s_scriptdata.bot.globalSkill;
	}

	Q_strncpyz(s_scriptdata.bot.name[index], name, MAX_NAME_LENGTH);
	s_scriptdata.bot.slotType[index] = SLOTTYPE_BOT;
}



/*
=================
StartServer_InsertNamedBot
=================
*/
void StartServer_InsertNamedBot(int index, char* name)
{
	if (index < 0 || index >= PLAYER_SLOTS || !name)
		return;

	StartServer_InsertBotSlot(index);
	StartServer_SetNamedBot(index, name);
}



/*
=================
StartServer_SlotTeam
=================
*/
int StartServer_SlotTeam(int index)
{
	if (index < 0)
		return SLOTTEAM_INVALID;

	if (s_scriptdata.gametype >= GT_TEAM && !(s_scriptdata.gametype == GT_LMS) ) {
		if (index < PLAYER_SLOTS_PERCOL)
			return SLOTTEAM_ONE;
		if (index < PLAYER_SLOTS)
			return SLOTTEAM_TWO;
	}
	else {
		if (index < PLAYER_SLOTS)
			return SLOTTEAM_NONE;
	}

	return SLOTTEAM_INVALID;
}




/*
=================
StartServer_MoveBotToOtherTeam
=================
*/
void StartServer_MoveBotToOtherTeam(int selected)
{
	int i, firstopen, dest, max, start;

	if (selected < 0 || selected >= PLAYER_SLOTS)
		return;

	if (s_scriptdata.bot.slotType[selected] != SLOTTYPE_BOT)
		return;

	// try to find an empty slot first
	firstopen = -1;
	max = PLAYER_SLOTS_PERCOL;
	start = 0;
	if (selected < PLAYER_SLOTS_PERCOL) {
		max = PLAYER_SLOTS;
		start = PLAYER_SLOTS_PERCOL;
	}

	dest = -1;
	for (i = start; i < max ; i++)
	{
		if (firstopen == -1 && s_scriptdata.bot.slotType[i] == SLOTTYPE_OPEN)
			firstopen = i;

		if (s_scriptdata.bot.slotType[i] == SLOTTYPE_EMPTY) {
			dest = i;
			break;
		}
	}

	// use openslot if we have no free ones
	if (dest == -1) {
		if (firstopen == -1)
			return;
		dest = firstopen;
	}

	
	// copy over details
	Q_strncpyz(s_scriptdata.bot.name[dest], s_scriptdata.bot.name[selected], MAX_NAME_LENGTH);

	s_scriptdata.bot.slotType[dest] = s_scriptdata.bot.slotType[selected];

	s_scriptdata.bot.skill[dest].value = s_scriptdata.bot.skill[selected].value;
	s_scriptdata.bot.skill[dest].high= s_scriptdata.bot.skill[selected].high;
	s_scriptdata.bot.skill[dest].low = s_scriptdata.bot.skill[selected].low;

	StartServer_DeleteBotSlot(selected);
}



/*
=================
StartServer_BotNameDrawn
=================
*/
void StartServer_BotNameDrawn(int index, qboolean drawn)
{
	s_scriptdata.bot.drawName[index] = drawn;
}



/*
=================
StartServer_BotDoAction
=================
*/
void StartServer_DoBotAction(int action, int selected)
{
	int i;
	int count, index, open, slot, bots_done;

	switch (action) {
		case BOT_CT_CLEARALL:
			for (i = 0; i < PLAYER_SLOTS; i++) {
				if (s_scriptdata.bot.slotType[i] == SLOTTYPE_BOT || s_scriptdata.bot.slotType[i] == SLOTTYPE_OPEN)
				{
					s_scriptdata.bot.slotType[i] = SLOTTYPE_EMPTY;
					StartServer_BotNameDrawn(i, qfalse);
					memset(s_scriptdata.bot.name[i], 0, MAX_NAME_LENGTH);
				}
			}
			break;
		case BOT_CT_INDIV_SELECTED:
			if (selected >=0) {
				s_scriptdata.bot.skill[selected].value = s_scriptdata.bot.globalSkill.value;
			}
			break;
		case BOT_CT_RANGE_SELECTED:
			if (selected >=0) {
				s_scriptdata.bot.skill[selected].low = s_scriptdata.bot.globalSkill.low;
				s_scriptdata.bot.skill[selected].high = s_scriptdata.bot.globalSkill.high;
			}
			break;
		case BOT_CT_INDIV_ALL:
			for (i = 0; i < PLAYER_SLOTS; i++) {
				s_scriptdata.bot.skill[i].value = s_scriptdata.bot.globalSkill.value;
			}
			break;
		case BOT_CT_RANGE_ALL:
			for (i = 0; i < PLAYER_SLOTS; i++) {
				s_scriptdata.bot.skill[i].low = s_scriptdata.bot.globalSkill.low;
				s_scriptdata.bot.skill[i].high = s_scriptdata.bot.globalSkill.high;
			}
			break;

		case BOT_CT_NEATEN:

			// perform two passes when we have a team arrangement
			// once on each column, treated separately
			bots_done = 0;
			do {
				open = 0;
				count = PLAYER_SLOTS;
				index = bots_done;
				if (s_scriptdata.gametype >= GT_TEAM && !(s_scriptdata.gametype == GT_LMS)  && !StartServer_IsRandomBotExclude(s_scriptdata.bot.typeSelect)) {
					count = PLAYER_SLOTS_PERCOL;
				}

				// compact all the bots, counting the open slots
				while (count > 0) {
					slot = s_scriptdata.bot.slotType[index];
					if (slot == SLOTTYPE_OPEN)
						open++;

					if (slot == SLOTTYPE_BOT || slot == SLOTTYPE_HUMAN)
						index++;
					else
						StartServer_DeleteBotSlot(index);

					count--;
					bots_done++;
				}

				// place all the open slots
				for (i = 0; i < open; i++) {
					s_scriptdata.bot.slotType[index] = SLOTTYPE_OPEN;
					index++;
				}
			} while (bots_done < PLAYER_SLOTS);

			break;
	}
}



/*
=================
StartServer_ValidateBotSlotCount
=================
*/
void StartServer_ValidateBotSlotCount(int bots, int open)
{
	if (bots < 0)
		bots = 0;

	if (open < 0)
		open = 0;

	if (bots > MAX_CLIENTS)
		bots = MAX_CLIENTS;

	if (open > MAX_CLIENTS)
		open = MAX_CLIENTS;

	// sacrifice open slots for bots
	if (bots + open > MAX_CLIENTS)
		open = MAX_CLIENTS - bots;

	s_scriptdata.bot.numberBots = bots;
	s_scriptdata.bot.numberOpen = open;	
}



/*
=================
StartServer_AdjustBotSelectionFromGametype

wraps the bot.typeSelect safely, based on the current gametype
=================
*/
static void StartServer_AdjustBotSelectionFromGametype(void)
{
	if (s_scriptdata.gametype >= GT_TEAM && !(s_scriptdata.gametype == GT_LMS)  && StartServer_IsBotArenaScript(s_scriptdata.bot.typeSelect))
		s_scriptdata.bot.typeSelect = BOTTYPE_SELECT;
}



/*
=================
StartServer_LoadBotNameList
=================
*/
void StartServer_LoadBotNameList(int type)
{
	char* s;
	char* s1;
	int i;

	s_scriptdata.bot.typeSelect = type;
	s = gametype_cvar_base[ s_scriptdata.gametype ];

	memset(&s_scriptdata.bot.name, 0, PLAYER_SLOTS*MAX_NAME_LENGTH);

	for (i = 0 ; i < PLAYER_SLOTS; i++) {
		s_scriptdata.bot.slotType[i] = SLOTTYPE_EMPTY;
		StartServer_SetBotSkillValue(&s_scriptdata.bot.skill[i], 0 );
	}

    StartServer_AdjustBotSelectionFromGametype();

	// check if we need to load any bot data at all
	if (s_scriptdata.bot.typeSelect == BOTTYPE_ARENASCRIPT)
		return;

	// find the right type of bot data to load
	if (StartServer_IsRandomBotExclude(s_scriptdata.bot.typeSelect)) {
		s1 = "botexclude";
	}
	else {
		s1 = "botname";
	}	

    // do the load
	UI_LoadMultiArray( s, s1, SSBP_BotName_Callback, PLAYER_SLOTS, MAX_NAME_LENGTH, ';' );

	if (StartServer_IsRandomBotExclude(s_scriptdata.bot.typeSelect))
	{
		for ( i = 0 ; i < PLAYER_SLOTS; i++) {
			if (s_scriptdata.bot.name[i][0])
				s_scriptdata.bot.slotType[i] = SLOTTYPE_BOT;
			else
				s_scriptdata.bot.slotType[i] = SLOTTYPE_EMPTY;
			StartServer_SetBotSkillValue(&s_scriptdata.bot.skill[i], 0 );
		}
	}
	else
	{
		UI_LoadMultiArray( s, "slottype", SSBP_BotBuffer_Callback, PLAYER_SLOTS, BOT_TMPBUFFER, ';' );
		for ( i = 0 ; i < PLAYER_SLOTS; i++) {
			s_scriptdata.bot.slotType[i] = (int)Com_Clamp(0, SLOTTYPE_COUNT, atoi(botskill_tmpbuffer[i]));
		}

		UI_LoadMultiArray( s, "botskill", SSBP_BotBuffer_Callback, PLAYER_SLOTS, BOT_TMPBUFFER, ';' );
		for ( i = 0 ; i < PLAYER_SLOTS; i++) {
			StartServer_SetBotSkillValue(&s_scriptdata.bot.skill[i], atoi(botskill_tmpbuffer[i]) );
		}

		if (!s_scriptdata.bot.joinAs)
			s_scriptdata.bot.slotType[0] = SLOTTYPE_HUMAN;
	}

	// set any slot other that first as non-human
	// (only tampering should cause this)
	for ( i = 1; i < PLAYER_SLOTS; i++ ) {
		if (s_scriptdata.bot.slotType[i] == SLOTTYPE_HUMAN) {
			s_scriptdata.bot.slotType[i] = SLOTTYPE_OPEN;
		}
	}
}



/*
=================
StartServer_SaveBotNameList
=================
*/
void StartServer_SaveBotNameList(void)
{
	char* s;
	char* s1;
	int i;
	botskill_t* b;
	qboolean exclude;

	s = gametype_cvar_base[ s_scriptdata.gametype ];

	if (StartServer_IsRandomBotExclude(s_scriptdata.bot.typeSelect)) {
		exclude = qtrue;
		s1 = "botexclude";
	}
	else
	{
		exclude = qfalse;
		s1 = "botname";
	}

	UI_SaveMultiArray( s, s1, SSBP_BotName_Callback, PLAYER_SLOTS, MAX_NAME_LENGTH, ';' );

	if (!exclude)
	{
		for ( i = 0 ; i < PLAYER_SLOTS; i++) {
			Q_strncpyz( botskill_tmpbuffer[i], va("%i", s_scriptdata.bot.slotType[i]), BOT_TMPBUFFER);
		}
		UI_SaveMultiArray( s, "slottype", SSBP_BotBuffer_Callback, PLAYER_SLOTS, BOT_TMPBUFFER, ';' );

		for ( i = 0 ; i < PLAYER_SLOTS; i++) {
			b = &s_scriptdata.bot.skill[i];
			Q_strncpyz( botskill_tmpbuffer[i], va("%i%i%i", b->value, b->low, b->high), BOT_TMPBUFFER);
		}
		UI_SaveMultiArray( s, "botskill", SSBP_BotBuffer_Callback, PLAYER_SLOTS, BOT_TMPBUFFER, ';' );
	}
}





/*
=================
StartServer_LoadBotScriptData

Loads bot specific gametype data
=================
*/
void StartServer_LoadBotScriptData(void)
{
	char* s;
	int i, value, max;
	botskill_t* b;

	s = gametype_cvar_base[ s_scriptdata.gametype ];

	//
	// load state values
	//

	// join game as
	s_scriptdata.bot.joinAs = GUI_GetSkirmishCvarIntClamp(0, 1, s, "PlayerJoinAs");

	// skill selection method for bots
	// assumes BotSelection method is already set in Start_Server_LoadBotNameList()
	StartServer_SetBotSkillRangeType(GUI_GetSkirmishCvarIntClamp(0, BOTSKILL_COUNT, s, "BotSkillType"));

	// number of bots if randomly generated
	s_scriptdata.bot.numberBots = GUI_GetSkirmishCvarIntClamp(0, 256, s, "BotCount");

	// frequency of bot changing on maps
	s_scriptdata.bot.changeBots = GUI_GetSkirmishCvarIntClamp(0, BOTCHANGE_COUNT, s, "BotChange");

	// number of open slots if bots are randomly selected
	s_scriptdata.bot.numberOpen = GUI_GetSkirmishCvarIntClamp(0, 256, s, "OpenSlotCount");

	// skill range values
	StartServer_SetBotSkillValue(&s_scriptdata.bot.globalSkill, GUI_GetSkirmishCvarInt(s, "BotSkillValue"));

	// skill bias
	s_scriptdata.bot.skillBias = GUI_GetSkirmishCvarIntClamp(0, SKILLBIAS_COUNT, s, "BotSkillBias");

	// swap teams
	if (s_scriptdata.gametype >= GT_TEAM && !(s_scriptdata.gametype == GT_LMS) ) {
		s_scriptdata.bot.teamSwapped = GUI_GetSkirmishCvarIntClamp(0, 1, s, "TeamSwapped");
	}

	// load bot stats
	// requires bot.joinAs to be set
	StartServer_LoadBotNameList(GUI_GetSkirmishCvarIntClamp(0, BOTTYPE_MAX, s, "BotSelection"));
}




/*
=================
StartServer_SaveBotScriptData

Saves bot specific gametype data
=================
*/
static void StartServer_SaveBotScriptData(void)
{
	char* s;
	int i, value;
	botskill_t* b;

	//
	// save state values
	//

	s = gametype_cvar_base[ s_scriptdata.gametype ];

	// method of selecting bots
	GUI_SetSkirmishCvarInt(s, "BotSelection", s_scriptdata.bot.typeSelect);

	// number of bots if randomly generated
	GUI_SetSkirmishCvarInt(s, "BotCount", s_scriptdata.bot.numberBots);

	// frequency of bot changing on maps
	GUI_SetSkirmishCvarInt(s, "BotChange", s_scriptdata.bot.changeBots);

	// number of open slots if bots are randomly selected
	GUI_SetSkirmishCvarInt(s, "OpenSlotCount", s_scriptdata.bot.numberOpen);

	// skill selection method for bots
	GUI_SetSkirmishCvarInt(s, "BotSkillType", s_scriptdata.bot.skillType);

	// skill range values
	b = &s_scriptdata.bot.globalSkill;
	value = (b->value * 100) +  (b->low * 10) + b->high;
	GUI_SetSkirmishCvarInt(s, "BotSkillValue", value);

	// skill bias
	GUI_SetSkirmishCvarInt(s, "BotSkillBias", s_scriptdata.bot.skillBias);

	// join game as
	GUI_SetSkirmishCvarInt(s, "PlayerJoinAs", s_scriptdata.bot.joinAs);

	// swap teams
	if (s_scriptdata.gametype >= GT_TEAM)
		GUI_SetSkirmishCvarInt(s, "TeamSwapped", s_scriptdata.bot.teamSwapped);

	//
	// bots
	//

	// save bot stats
	StartServer_SaveBotNameList();
}



/*
==========================================================================

LOADING AND SAVING OF ITEM SCRIPT DATA

==========================================================================
*/



/*
=================
StartServer_LoadItemScriptData

Loads item specific gametype data
=================
*/
void StartServer_LoadDisabledItems(void)
{
	char* s;
	char buffer[256];
	char* ptr, *ptr_last;
	int i;

	s = gametype_cvar_base[ s_scriptdata.gametype ];

	// load the group on/off/custom values
	memset(buffer,0,sizeof(buffer));
	GUI_GetSkirmishCvar(s, "itemGroups", buffer, 256);
	ptr = buffer;
	for (i = 0; i < ITEMGROUP_COUNT; i++)
	{
		if (*ptr) {
			s_scriptdata.item.groupstate[i] = (int)Com_Clamp(0, ALLGROUPS_HIDDEN, *ptr - '0');
			ptr++;
		}
		else {
			s_scriptdata.item.groupstate[i] = ALLGROUPS_ENABLED;
		}
	}

	// load individual item values
	// we only load a list of items that are "off"
	// no assumption about order is made
	for (i = 0; i < ITEM_COUNT; i++)
	{
		s_scriptdata.item.enabled[i] = qtrue;
	}

	memset(buffer, 0, sizeof(buffer));
	GUI_GetSkirmishCvar(s, "itemsHidden", buffer, 256);
	ptr = buffer;
	while (*ptr) {
		ptr_last = strchr(ptr, '\\');

		if (!ptr_last)
			break;

		*ptr_last = '\0';
		for (i = 0; i < ITEM_COUNT; i++) {
			if (!Q_stricmp(ptr, server_itemlist[i].shortitem))
			{
				s_scriptdata.item.enabled[i] = qfalse;
				break;
			}

		}

		// move to next char
		ptr = ptr_last + 1;
	}
}



/*
=================
StartServer_LoadItemScriptData

Loads item specific gametype data
=================
*/
static void StartServer_LoadItemScriptData(void)
{
	StartServer_LoadDisabledItems();
}



/*
=================
StartServer_SaveItemScriptData

Saves item specific gametype data
=================
*/
static void StartServer_SaveItemScriptData(void)
{
	char* s;
	char buffer[256];
	int i;

	s = gametype_cvar_base[ s_scriptdata.gametype ];

	// save the group values
	for (i = 0; i < ITEMGROUP_COUNT; i++)
	{
		buffer[i] = '0' + s_scriptdata.item.groupstate[i];
	}
	buffer[ITEMGROUP_COUNT] = '\0';

	GUI_SetSkirmishCvar(s, "itemGroups", buffer);

	// save individual item values
	// we only save a list of items that are "off"
	// always terminate with a slash
	buffer[0] = '\0';
	for (i = 0; i < ITEM_COUNT; i++)
	{
		if (!s_scriptdata.item.enabled[i]) {
			Q_strcat(buffer, 256, va("%s\\", server_itemlist[i].shortitem));
		}
	}
	GUI_SetSkirmishCvar(s, "itemsHidden", buffer);
}




/*
==========================================================================

LOADING AND SAVING OF SERVER SCRIPT DATA

==========================================================================
*/



/*
=================
StartServer_LoadServerScriptData

Loads server specific gametype data
=================
*/
static void StartServer_LoadServerScriptData(void)
{
	char* s, *t;
	int value, gametype;

	s_scriptdata.server.pure = GUI_GetSkirmishCvarIntClamp(0, 1, NULL, "gui_pure" );
	s_scriptdata.server.dynamiclight = GUI_GetSkirmishCvarIntClamp(0, 1, NULL, "gui_dynamiclight" );
	s_scriptdata.server.pmove_fixed = GUI_GetSkirmishCvarIntClamp(0, 1, NULL, "gui_pmove_fixed" );
	s_scriptdata.server.pmove_msec = GUI_GetSkirmishCvarIntClamp(0, 999, NULL, "gui_pmove_msec" );

	s_scriptdata.server.smoothclients = GUI_GetSkirmishCvarIntClamp(0, 1, NULL, "gui_smoothclients" );
	s_scriptdata.server.syncClients = GUI_GetSkirmishCvarIntClamp(0, 1, NULL, "gui_syncclients" );

	s_scriptdata.server.minPing = GUI_GetSkirmishCvarIntClamp(0, 999, NULL, "gui_minping" );
	s_scriptdata.server.maxPing = GUI_GetSkirmishCvarIntClamp(0, 999, NULL, "gui_maxping" );
	s_scriptdata.server.allowMinPing = GUI_GetSkirmishCvarIntClamp(0, 1, NULL, "gui_allowMinPing" );
	s_scriptdata.server.allowMaxPing = GUI_GetSkirmishCvarIntClamp(0, 1, NULL, "gui_allowMaxPing" );

	s_scriptdata.server.allowmaxrate = GUI_GetSkirmishCvarIntClamp(0, 1, NULL, "gui_allowmaxrate" );
	s_scriptdata.server.maxrate = GUI_GetSkirmishCvarIntClamp(0, 99999, NULL, "gui_maxrate" );

	s_scriptdata.server.allowdownload = GUI_GetSkirmishCvarIntClamp(0, 1, NULL, "gui_allowdownload" );
	s_scriptdata.server.allowvote = GUI_GetSkirmishCvarIntClamp(0, 1, NULL, "gui_allowvote" );
	s_scriptdata.server.allowpass = GUI_GetSkirmishCvarIntClamp(0, 1, NULL, "gui_allowpass" );

	s_scriptdata.server.netport = GUI_GetSkirmishCvarIntClamp(1024, 65535, NULL, "gui_netport" );
	s_scriptdata.server.sv_fps = GUI_GetSkirmishCvarIntClamp(0, 160, NULL, "gui_svfps" );

	GUI_GetSkirmishCvar(NULL, "gui_password",s_scriptdata.server.password, MAX_PASSWORD_LENGTH);

	s_scriptdata.server.allowPrivateClients = GUI_GetSkirmishCvarIntClamp(0, 32, NULL, "gui_allowprivateclients" );
	s_scriptdata.server.privateClients = GUI_GetSkirmishCvarIntClamp(0, 32, NULL, "gui_privateclients" );
	GUI_GetSkirmishCvar(NULL, "gui_privatepassword",s_scriptdata.server.privatePassword, MAX_PASSWORD_LENGTH);

	s_scriptdata.server.preventConfigBug = GUI_GetSkirmishCvarIntClamp(0, 1, NULL, "gui_preventConfigBug");

	if (s_scriptdata.multiplayer) {
		s_scriptdata.server.dedicatedServer = GUI_GetSkirmishCvarIntClamp(0, SRVDED_INTERNET, NULL, "gui_dedicated" );
		s_scriptdata.server.inactivityTime = GUI_GetSkirmishCvarIntClamp(0, 999, NULL, "gui_inactivity" );

		s_scriptdata.server.strictAuth = GUI_GetSkirmishCvarIntClamp(0, 1, NULL, "gui_strictAuth" );
		s_scriptdata.server.lanForceRate = GUI_GetSkirmishCvarIntClamp(0, 1, NULL, "gui_lanForceRate" );
	}
	else {
		s_scriptdata.server.dedicatedServer = SRVDED_OFF;
		s_scriptdata.server.inactivityTime = 0;

		s_scriptdata.server.strictAuth = 0;
		s_scriptdata.server.lanForceRate = 0;
	}
	
//Noire.dev cvars
s_scriptdata.server.maxEntities = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_maxEntities" );
s_scriptdata.server.singleskill = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_singleskill" );
s_scriptdata.server.kill = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_kill" );
GUI_GetSkirmishCvar(NULL, "gui_damageModifier",s_scriptdata.server.damageModifier, 32);
GUI_GetSkirmishCvar(NULL, "gui_selectedmod",s_scriptdata.server.selectedmod, 32);
s_scriptdata.server.elimination = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_elimination" );
s_scriptdata.server.obeliskHealth = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_obeliskHealth" );
s_scriptdata.server.obeliskRegenPeriod = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_obeliskRegenPeriod" );
s_scriptdata.server.obeliskRegenAmount = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_obeliskRegenAmount" );
s_scriptdata.server.obeliskRespawnDelay = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_obeliskRespawnDelay" );
s_scriptdata.server.cubeTimeout = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_cubeTimeout" );
s_scriptdata.server.flagrespawn = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_flagrespawn" );
s_scriptdata.server.weaponTeamRespawn = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_weaponTeamRespawn" );
s_scriptdata.server.elimination_ctf_oneway = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_elimination_ctf_oneway" );
s_scriptdata.server.elimination_selfdamage = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_elimination_selfdamage" );
s_scriptdata.server.elimination_roundtime = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_elimination_roundtime" );
s_scriptdata.server.elimination_warmup = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_elimination_warmup" );
s_scriptdata.server.elimination_activewarmup = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_elimination_activewarmup" );
s_scriptdata.server.lms_lives = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_lms_lives" );
s_scriptdata.server.lms_mode = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_lms_mode" );
s_scriptdata.server.accelerate = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_accelerate" );
s_scriptdata.server.spectatorspeed = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_spectatorspeed" );
s_scriptdata.server.speed = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_speed" );
s_scriptdata.server.gravity = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_gravity" );
GUI_GetSkirmishCvar(NULL, "gui_gravityModifier",s_scriptdata.server.gravityModifier, 32);
s_scriptdata.server.knockback = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_knockback" );
s_scriptdata.server.noplayerclip = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_noplayerclip" );
s_scriptdata.server.jumpheight = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_jumpheight" );
s_scriptdata.server.regenarmor = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_regenarmor" );
s_scriptdata.server.ammolimit = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_ammolimit" );
GUI_GetSkirmishCvar(NULL, "gui_quadfactor",s_scriptdata.server.quadfactor, 32);
s_scriptdata.server.respawntime = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_respawntime" );
s_scriptdata.server.forcerespawn = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_forcerespawn" );
GUI_GetSkirmishCvar(NULL, "gui_vampire",s_scriptdata.server.vampire, 32);
s_scriptdata.server.vampire_max_health = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_vampire_max_health" );
s_scriptdata.server.regen = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_regen" );
s_scriptdata.server.maxweaponpickup = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_maxweaponpickup" );
s_scriptdata.server.droppeditemtime = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_droppeditemtime" );
s_scriptdata.server.autoflagreturn = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_autoflagreturn" );
GUI_GetSkirmishCvar(NULL, "gui_armorprotect",s_scriptdata.server.armorprotect, 32);
s_scriptdata.server.respawnwait = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_respawnwait" );
GUI_GetSkirmishCvar(NULL, "gui_speedfactor",s_scriptdata.server.speedfactor, 32);
GUI_GetSkirmishCvar(NULL, "gui_scoutspeedfactor",s_scriptdata.server.scoutspeedfactor, 32);
GUI_GetSkirmishCvar(NULL, "gui_scoutfirespeed",s_scriptdata.server.scoutfirespeed, 32);
GUI_GetSkirmishCvar(NULL, "gui_scoutdamagefactor",s_scriptdata.server.scoutdamagefactor, 32);
GUI_GetSkirmishCvar(NULL, "gui_scoutgravitymodifier",s_scriptdata.server.scoutgravitymodifier, 32);
s_scriptdata.server.scout_infammo = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_scout_infammo" );
GUI_GetSkirmishCvar(NULL, "gui_scouthealthmodifier",s_scriptdata.server.scouthealthmodifier, 32);
GUI_GetSkirmishCvar(NULL, "gui_doublerfirespeed",s_scriptdata.server.doublerfirespeed, 32);
GUI_GetSkirmishCvar(NULL, "gui_doublerdamagefactor",s_scriptdata.server.doublerdamagefactor, 32);
GUI_GetSkirmishCvar(NULL, "gui_doublerspeedfactor",s_scriptdata.server.doublerspeedfactor, 32);
GUI_GetSkirmishCvar(NULL, "gui_doublergravitymodifier",s_scriptdata.server.doublergravitymodifier, 32);
s_scriptdata.server.doubler_infammo = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_doubler_infammo" );
GUI_GetSkirmishCvar(NULL, "gui_doublerhealthmodifier",s_scriptdata.server.doublerhealthmodifier, 32);
GUI_GetSkirmishCvar(NULL, "gui_guardhealthmodifier",s_scriptdata.server.guardhealthmodifier, 32);
GUI_GetSkirmishCvar(NULL, "gui_guardfirespeed",s_scriptdata.server.guardfirespeed, 32);
GUI_GetSkirmishCvar(NULL, "gui_guarddamagefactor",s_scriptdata.server.guarddamagefactor, 32);
GUI_GetSkirmishCvar(NULL, "gui_guardspeedfactor",s_scriptdata.server.guardspeedfactor, 32);
GUI_GetSkirmishCvar(NULL, "gui_guardgravitymodifier",s_scriptdata.server.guardgravitymodifier, 32);
s_scriptdata.server.guard_infammo = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_guard_infammo" );
GUI_GetSkirmishCvar(NULL, "gui_ammoregenfirespeed",s_scriptdata.server.ammoregenfirespeed, 32);
s_scriptdata.server.ammoregen_infammo = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_ammoregen_infammo" );
GUI_GetSkirmishCvar(NULL, "gui_ammoregendamagefactor",s_scriptdata.server.ammoregendamagefactor, 32);
GUI_GetSkirmishCvar(NULL, "gui_ammoregenspeedfactor",s_scriptdata.server.ammoregenspeedfactor, 32);
GUI_GetSkirmishCvar(NULL, "gui_ammoregengravitymodifier",s_scriptdata.server.ammoregengravitymodifier, 32);
GUI_GetSkirmishCvar(NULL, "gui_ammoregenhealthmodifier",s_scriptdata.server.ammoregenhealthmodifier, 32);
s_scriptdata.server.teamred_speed = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_teamred_speed" );
GUI_GetSkirmishCvar(NULL, "gui_teamred_gravityModifier",s_scriptdata.server.teamred_gravityModifier, 32);
GUI_GetSkirmishCvar(NULL, "gui_teamred_firespeed",s_scriptdata.server.teamred_firespeed, 32);
GUI_GetSkirmishCvar(NULL, "gui_teamred_damage",s_scriptdata.server.teamred_damage, 32);
s_scriptdata.server.teamred_infammo = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_teamred_infammo" );
s_scriptdata.server.teamred_respawnwait = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_teamred_respawnwait" );
s_scriptdata.server.teamred_pickupitems = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_teamred_pickupitems" );
s_scriptdata.server.eliminationredrespawn = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_eliminationredrespawn" );
s_scriptdata.server.eliminationred_startHealth = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_eliminationred_startHealth" );
s_scriptdata.server.eliminationred_startArmor = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_eliminationred_startArmor" );
s_scriptdata.server.teamblue_speed = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_teamblue_speed" );
GUI_GetSkirmishCvar(NULL, "gui_teamblue_gravityModifier",s_scriptdata.server.teamblue_gravityModifier, 32);
GUI_GetSkirmishCvar(NULL, "gui_teamblue_firespeed",s_scriptdata.server.teamblue_firespeed, 32);
GUI_GetSkirmishCvar(NULL, "gui_teamblue_damage",s_scriptdata.server.teamblue_damage, 32);
s_scriptdata.server.teamblue_infammo = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_teamblue_infammo" );
s_scriptdata.server.teamblue_respawnwait = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_teamblue_respawnwait" );
s_scriptdata.server.teamblue_pickupitems = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_teamblue_pickupitems" );
s_scriptdata.server.eliminationrespawn = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_eliminationrespawn" );
s_scriptdata.server.elimination_startHealth = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_elimination_startHealth" );
s_scriptdata.server.elimination_startArmor = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_elimination_startArmor" );
s_scriptdata.server.eliminationred_grapple = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_eliminationred_grapple" );
s_scriptdata.server.eliminationred_gauntlet = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_eliminationred_gauntlet" );
s_scriptdata.server.eliminationred_machinegun = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_eliminationred_machinegun" );
s_scriptdata.server.eliminationred_shotgun = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_eliminationred_shotgun" );
s_scriptdata.server.eliminationred_grenade = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_eliminationred_grenade" );
s_scriptdata.server.eliminationred_rocket = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_eliminationred_rocket" );
s_scriptdata.server.eliminationred_railgun = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_eliminationred_railgun" );
s_scriptdata.server.eliminationred_lightning = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_eliminationred_lightning" );
s_scriptdata.server.eliminationred_plasmagun = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_eliminationred_plasmagun" );
s_scriptdata.server.eliminationred_bfg = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_eliminationred_bfg" );
s_scriptdata.server.eliminationred_chain = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_eliminationred_chain" );
s_scriptdata.server.eliminationred_mine = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_eliminationred_mine" );
s_scriptdata.server.eliminationred_nail = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_eliminationred_nail" );
s_scriptdata.server.eliminationred_flame = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_eliminationred_flame" );
s_scriptdata.server.eliminationred_antimatter = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_eliminationred_antimatter" );
s_scriptdata.server.elimination_grapple = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_elimination_grapple" );
s_scriptdata.server.elimination_gauntlet = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_elimination_gauntlet" );
s_scriptdata.server.elimination_machinegun = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_elimination_machinegun" );
s_scriptdata.server.elimination_shotgun = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_elimination_shotgun" );
s_scriptdata.server.elimination_grenade = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_elimination_grenade" );
s_scriptdata.server.elimination_rocket = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_elimination_rocket" );
s_scriptdata.server.elimination_railgun = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_elimination_railgun" );
s_scriptdata.server.elimination_lightning = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_elimination_lightning" );
s_scriptdata.server.elimination_plasmagun = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_elimination_plasmagun" );
s_scriptdata.server.elimination_bfg = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_elimination_bfg" );
s_scriptdata.server.elimination_chain = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_elimination_chain" );
s_scriptdata.server.elimination_mine = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_elimination_mine" );
s_scriptdata.server.elimination_nail = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_elimination_nail" );
s_scriptdata.server.elimination_flame = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_elimination_flame" );
s_scriptdata.server.elimination_antimatter = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_elimination_antimatter" );
s_scriptdata.server.eliminationred_quad = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_eliminationred_quad" );
s_scriptdata.server.elimination_haste = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_elimination_haste" );
s_scriptdata.server.elimination_bsuit = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_elimination_bsuit" );
s_scriptdata.server.elimination_invis = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_elimination_invis" );
s_scriptdata.server.elimination_regen = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_elimination_regen" );
s_scriptdata.server.elimination_flight = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_elimination_flight" );
s_scriptdata.server.elimination_items = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_elimination_items" );
s_scriptdata.server.elimination_holdable = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_elimination_holdable" );
s_scriptdata.server.overlay = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_overlay" );
s_scriptdata.server.randomItems = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_randomItems" );
s_scriptdata.server.slickmove = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_slickmove" );
s_scriptdata.server.spawnprotect = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_spawnprotect" );
s_scriptdata.server.elimination_lockspectator = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_elimination_lockspectator" );
s_scriptdata.server.awardpushing = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_awardpushing" );
s_scriptdata.server.randomteleport = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_randomteleport" );
s_scriptdata.server.falldamagesmall = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_falldamagesmall" );
s_scriptdata.server.falldamagebig = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_falldamagebig" );
s_scriptdata.server.waterdamage = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_waterdamage" );
s_scriptdata.server.lavadamage = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_lavadamage" );
s_scriptdata.server.slimedamage = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_slimedamage" );
s_scriptdata.server.drowndamage = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_drowndamage" );
s_scriptdata.server.invulinf = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_invulinf" );
s_scriptdata.server.invulmove = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_invulmove" );
s_scriptdata.server.invultime = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_invultime" );
s_scriptdata.server.kamikazeinf = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_kamikazeinf" );
s_scriptdata.server.portalinf = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_portalinf" );
s_scriptdata.server.portaltimeout = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_portaltimeout" );
s_scriptdata.server.portalhealth = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_portalhealth" );
s_scriptdata.server.teleporterinf = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_teleporterinf" );
s_scriptdata.server.medkitinf = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_medkitinf" );
s_scriptdata.server.medkitlimit = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_medkitlimit" );
s_scriptdata.server.medkitmodifier = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_medkitmodifier" );
s_scriptdata.server.fasthealthregen = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_fasthealthregen" );
s_scriptdata.server.slowhealthregen = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_slowhealthregen" );
GUI_GetSkirmishCvar(NULL, "gui_hastefirespeed",s_scriptdata.server.hastefirespeed, 32);
s_scriptdata.server.quadtime = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_quadtime" );
s_scriptdata.server.bsuittime = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_bsuittime" );
s_scriptdata.server.hastetime = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_hastetime" );
s_scriptdata.server.invistime = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_invistime" );
s_scriptdata.server.regentime = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_regentime" );
s_scriptdata.server.flighttime = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_flighttime" );
s_scriptdata.server.armorrespawn = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_armorrespawn" );
s_scriptdata.server.healthrespawn = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_healthrespawn" );
s_scriptdata.server.ammorespawn = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_ammorespawn" );
s_scriptdata.server.holdablerespawn = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_holdablerespawn" );
s_scriptdata.server.megahealthrespawn = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_megahealthrespawn" );
s_scriptdata.server.poweruprespawn = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_poweruprespawn" );
s_scriptdata.server.weaponrespawn = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_weaponrespawn" );
// Крюк
s_scriptdata.server.g_ghspeed = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_ghspeed" );
s_scriptdata.server.g_ghtimeout = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_ghtimeout" );
// Пила
s_scriptdata.server.g_gdelay = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_gdelay" );
s_scriptdata.server.g_gdamage = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_gdamage" );
s_scriptdata.server.g_grange = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_grange" );
// Автомат
s_scriptdata.server.g_mgammocount = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_mgammocount" );
s_scriptdata.server.g_mgweaponcount = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_mgweaponcount" );
s_scriptdata.server.g_mgdelay = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_mgdelay" );
s_scriptdata.server.g_mgdamage = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_mgdamage" );
s_scriptdata.server.g_mgspread = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_mgspread" );
s_scriptdata.server.g_mgexplode = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_mgexplode" );
s_scriptdata.server.g_mgsdamage = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_mgsdamage" );
s_scriptdata.server.g_mgsradius = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_mgsradius" );
s_scriptdata.server.g_mgvampire = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_mgvampire" );
s_scriptdata.server.g_mginf = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_mginf" );
// Дробовик
s_scriptdata.server.g_sgammocount = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_sgammocount" );
s_scriptdata.server.g_sgweaponcount = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_sgweaponcount" );
s_scriptdata.server.g_sgdelay = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_sgdelay" );
s_scriptdata.server.g_sgdamage = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_sgdamage" );
s_scriptdata.server.g_sgspread = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_sgspread" );
s_scriptdata.server.g_sgexplode = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_sgexplode" );
s_scriptdata.server.g_sgsdamage = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_sgsdamage" );
s_scriptdata.server.g_sgsradius = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_sgsradius" );
s_scriptdata.server.g_sgcount = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_sgcount" );
s_scriptdata.server.g_sgvampire = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_sgvampire" );
s_scriptdata.server.g_sginf = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_sginf" );

// Гранаты
s_scriptdata.server.g_glammocount = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_glammocount" );
s_scriptdata.server.g_glweaponcount = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_glweaponcount" );
s_scriptdata.server.g_gldelay = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_gldelay" );
s_scriptdata.server.g_glspeed = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_glspeed" );
s_scriptdata.server.g_glbounce = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_glbounce" );
s_scriptdata.server.g_glgravity = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_glgravity" );
s_scriptdata.server.g_gltimeout = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_gltimeout" );
s_scriptdata.server.g_glsradius = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_glsradius" );
s_scriptdata.server.g_glsdamage = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_glsdamage" );
s_scriptdata.server.g_gldamage = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_gldamage" );
s_scriptdata.server.g_glvampire = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_glvampire" );
s_scriptdata.server.g_glinf = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_glinf" );
s_scriptdata.server.g_glhoming = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_glhoming" );
s_scriptdata.server.g_glguided = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_glguided" );
// Ракеты
s_scriptdata.server.g_rlammocount = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_rlammocount" );
s_scriptdata.server.g_rlweaponcount = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_rlweaponcount" );
s_scriptdata.server.g_rldelay = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_rldelay" );
s_scriptdata.server.g_rlspeed = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_rlspeed" );
s_scriptdata.server.g_rlbounce = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_rlbounce" );
s_scriptdata.server.g_rlgravity = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_rlgravity" );
s_scriptdata.server.g_rltimeout = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_rltimeout" );
s_scriptdata.server.g_rlsradius = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_rlsradius" );
s_scriptdata.server.g_rlsdamage = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_rlsdamage" );
s_scriptdata.server.g_rldamage = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_rldamage" );
s_scriptdata.server.g_rlvampire = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_rlvampire" );
s_scriptdata.server.g_rlinf = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_rlinf" );
s_scriptdata.server.g_rlhoming = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_rlhoming" );
s_scriptdata.server.g_rlguided = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_rlguided" );
// Молния
s_scriptdata.server.g_lgammocount = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_lgammocount" );
s_scriptdata.server.g_lgweaponcount = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_lgweaponcount" );
s_scriptdata.server.g_lgrange = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_lgrange" );
s_scriptdata.server.g_lgdelay = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_lgdelay" );
s_scriptdata.server.g_lgdamage = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_lgdamage" );
s_scriptdata.server.g_lgvampire = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_lgvampire" );
s_scriptdata.server.g_lgexplode = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_lgexplode" );
s_scriptdata.server.g_lgsdamage = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_lgsdamage" );
s_scriptdata.server.g_lgsradius = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_lgsradius" );
s_scriptdata.server.g_lginf = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_lginf" );
// Рэйлган
s_scriptdata.server.g_rgammocount = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_rgammocount" );
s_scriptdata.server.g_rgweaponcount = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_rgweaponcount" );
s_scriptdata.server.g_rgdelay = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_rgdelay" );
s_scriptdata.server.g_rgdamage = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_rgdamage" );
s_scriptdata.server.g_rgvampire = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_rgvampire" );
s_scriptdata.server.g_rginf = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_rginf" );
// Плазмаган
s_scriptdata.server.g_pgammocount = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_pgammocount" );
s_scriptdata.server.g_pgweaponcount = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_pgweaponcount" );
s_scriptdata.server.g_pgdelay = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_pgdelay" );
s_scriptdata.server.g_pgspeed = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_pgspeed" );
s_scriptdata.server.g_pgbounce = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_pgbounce" );
s_scriptdata.server.g_pggravity = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_pggravity" );
s_scriptdata.server.g_pgtimeout = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_pgtimeout" );
s_scriptdata.server.g_pgsradius = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_pgsradius" );
s_scriptdata.server.g_pgsdamage = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_pgsdamage" );
s_scriptdata.server.g_pgdamage = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_pgdamage" );
s_scriptdata.server.g_pgvampire = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_pgvampire" );
s_scriptdata.server.g_pginf = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_pginf" );
s_scriptdata.server.g_pghoming = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_pghoming" );
s_scriptdata.server.g_pgguided = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_pgguided" );
// Бфг
s_scriptdata.server.g_bfgammocount = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_bfgammocount" );
s_scriptdata.server.g_bfgweaponcount = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_bfgweaponcount" );
s_scriptdata.server.g_bfgdelay = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_bfgdelay" );
s_scriptdata.server.g_bfgspeed = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_bfgspeed" );
s_scriptdata.server.g_bfgbounce = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_bfgbounce" );
s_scriptdata.server.g_bfggravity = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_bfggravity" );
s_scriptdata.server.g_bfgtimeout = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_bfgtimeout" );
s_scriptdata.server.g_bfgsradius = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_bfgsradius" );
s_scriptdata.server.g_bfgsdamage = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_bfgsdamage" );
s_scriptdata.server.g_bfgdamage = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_bfgdamage" );
s_scriptdata.server.g_bfgvampire = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_bfgvampire" );
s_scriptdata.server.g_bfginf = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_bfginf" );
s_scriptdata.server.g_bfghoming = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_bfghoming" );
s_scriptdata.server.g_bfgguided = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_bfgguided" );
// Гвоздомёт
s_scriptdata.server.g_ngammocount = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_ngammocount" );
s_scriptdata.server.g_ngweaponcount = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_ngweaponcount" );
s_scriptdata.server.g_ngdelay = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_ngdelay" );
s_scriptdata.server.g_ngspeed = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_ngspeed" );
s_scriptdata.server.g_ngbounce = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_ngbounce" );
s_scriptdata.server.g_nggravity = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_nggravity" );
s_scriptdata.server.g_ngtimeout = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_ngtimeout" );
s_scriptdata.server.g_ngcount = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_ngcount" );
s_scriptdata.server.g_ngspread = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_ngspread" );
s_scriptdata.server.g_ngdamage = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_ngdamage" );
s_scriptdata.server.g_ngrandom = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_ngrandom" );
s_scriptdata.server.g_ngvampire = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_ngvampire" );
s_scriptdata.server.g_nginf = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_nginf" );
s_scriptdata.server.g_nghoming = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_nghoming" );
s_scriptdata.server.g_ngguided = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_ngguided" );
// Мины
s_scriptdata.server.g_plammocount = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_plammocount" );
s_scriptdata.server.g_plweaponcount = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_plweaponcount" );
s_scriptdata.server.g_pldelay = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_pldelay" );
s_scriptdata.server.g_plspeed = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_plspeed" );
s_scriptdata.server.g_plgravity = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_plgravity" );
s_scriptdata.server.g_pltimeout = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_pltimeout" );
s_scriptdata.server.g_plsradius = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_plsradius" );
s_scriptdata.server.g_plsdamage = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_plsdamage" );
s_scriptdata.server.g_pldamage = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_pldamage" );
s_scriptdata.server.g_plvampire = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_plvampire" );
s_scriptdata.server.g_plinf = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_plinf" );
// Пулемёт
s_scriptdata.server.g_cgammocount = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_cgammocount" );
s_scriptdata.server.g_cgweaponcount = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_cgweaponcount" );
s_scriptdata.server.g_cgdelay = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_cgdelay" );
s_scriptdata.server.g_cgspread = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_cgspread" );
s_scriptdata.server.g_cgdamage = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_cgdamage" );
s_scriptdata.server.g_cgvampire = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_cgvampire" );
s_scriptdata.server.g_cginf = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_cginf" );
// Огнемёт
s_scriptdata.server.g_ftammocount = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_ftammocount" );
s_scriptdata.server.g_ftweaponcount = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_ftweaponcount" );
s_scriptdata.server.g_ftdelay = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_ftdelay" );
s_scriptdata.server.g_ftspeed = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_ftspeed" );
s_scriptdata.server.g_ftbounce = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_ftbounce" );
s_scriptdata.server.g_ftgravity = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_ftgravity" );
s_scriptdata.server.g_fttimeout = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_fttimeout" );
s_scriptdata.server.g_ftsradius = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_ftsradius" );
s_scriptdata.server.g_ftsdamage = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_ftsdamage" );
s_scriptdata.server.g_ftdamage = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_ftdamage" );
s_scriptdata.server.g_ftvampire = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_ftvampire" );
s_scriptdata.server.g_ftinf = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_ftinf" );
s_scriptdata.server.g_fthoming = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_fthoming" );
s_scriptdata.server.g_ftguided = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_ftguided" );
// Антиматтер
s_scriptdata.server.g_amweaponcount = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_amweaponcount" );
s_scriptdata.server.g_amdelay = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_amdelay" );
s_scriptdata.server.g_amspeed = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_amspeed" );
s_scriptdata.server.g_ambounce = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_ambounce" );
s_scriptdata.server.g_amgravity = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_amgravity" );
s_scriptdata.server.g_amtimeout = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_amtimeout" );
s_scriptdata.server.g_amsradius = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_amsradius" );
s_scriptdata.server.g_amsdamage = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_amsdamage" );
s_scriptdata.server.g_amdamage = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_amdamage" );
s_scriptdata.server.g_amvampire = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_amvampire" );
s_scriptdata.server.g_aminf = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_aminf" );
s_scriptdata.server.g_amhoming = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_amhoming" );
s_scriptdata.server.g_amguided = GUI_GetSkirmishCvarIntClamp(-9999999, 9999999, NULL, "gui_amguided" );


GUI_GetSkirmishCvar(NULL, "gui_glbouncemodifier",s_scriptdata.server.g_glbouncemodifier, 32);
GUI_GetSkirmishCvar(NULL, "gui_rlbouncemodifier",s_scriptdata.server.g_rlbouncemodifier, 32);
GUI_GetSkirmishCvar(NULL, "gui_pgbouncemodifier",s_scriptdata.server.g_pgbouncemodifier, 32);
GUI_GetSkirmishCvar(NULL, "gui_bfgbouncemodifier",s_scriptdata.server.g_bfgbouncemodifier, 32);
GUI_GetSkirmishCvar(NULL, "gui_ngbouncemodifier",s_scriptdata.server.g_ngbouncemodifier, 32);
GUI_GetSkirmishCvar(NULL, "gui_ftbouncemodifier",s_scriptdata.server.g_ftbouncemodifier, 32);
GUI_GetSkirmishCvar(NULL, "gui_ambouncemodifier",s_scriptdata.server.g_ambouncemodifier, 32);
GUI_GetSkirmishCvar(NULL, "gui_gknockback",s_scriptdata.server.g_gknockback, 32);
GUI_GetSkirmishCvar(NULL, "gui_mgknockback",s_scriptdata.server.g_mgknockback, 32);
GUI_GetSkirmishCvar(NULL, "gui_sgknockback",s_scriptdata.server.g_sgknockback, 32);
GUI_GetSkirmishCvar(NULL, "gui_glknockback",s_scriptdata.server.g_glknockback, 32);
GUI_GetSkirmishCvar(NULL, "gui_rlknockback",s_scriptdata.server.g_rlknockback, 32);
GUI_GetSkirmishCvar(NULL, "gui_lgknockback",s_scriptdata.server.g_lgknockback, 32);
GUI_GetSkirmishCvar(NULL, "gui_rgknockback",s_scriptdata.server.g_rgknockback, 32);
GUI_GetSkirmishCvar(NULL, "gui_pgknockback",s_scriptdata.server.g_pgknockback, 32);
GUI_GetSkirmishCvar(NULL, "gui_bfgknockback",s_scriptdata.server.g_bfgknockback, 32);
GUI_GetSkirmishCvar(NULL, "gui_ngknockback",s_scriptdata.server.g_ngknockback, 32);
GUI_GetSkirmishCvar(NULL, "gui_plknockback",s_scriptdata.server.g_plknockback, 32);
GUI_GetSkirmishCvar(NULL, "gui_cgknockback",s_scriptdata.server.g_cgknockback, 32);
GUI_GetSkirmishCvar(NULL, "gui_ftknockback",s_scriptdata.server.g_ftknockback, 32);
GUI_GetSkirmishCvar(NULL, "gui_amknockback",s_scriptdata.server.g_amknockback, 32);

	//
	// gametype specific values
	//
	
	trap_Cvar_Set( "cl_selectedmod", s_scriptdata.server.selectedmod );

	gametype = s_scriptdata.gametype;
	s = gametype_cvar_base[ gametype ];

	// reset server text control
	GUI_GetSkirmishCvar( s, "hostname", s_scriptdata.server.hostname, MAX_HOSTNAME_LENGTH );

	//s_scriptdata.server.forceRespawn = GUI_GetSkirmishCvarIntClamp(0, 999, s, "ForceRespawn" );
	s_scriptdata.server.allowWarmup = GUI_GetSkirmishCvarIntClamp(0, 1, s, "doWarmUp" );
	s_scriptdata.server.warmupTime = GUI_GetSkirmishCvarIntClamp(0, 999, s, "WarmUp" );
	s_scriptdata.server.viewdistance = GUI_GetSkirmishCvarIntClamp(0, 90, s, "viewdistance" );

	if (gametype >= GT_TEAM && !(gametype == GT_LMS) ) {
		t = 0;
		if (gametype == GT_TEAM)
			t = "ui_team_friendly";
		else if (gametype == GT_CTF)
			t = "ui_ctf_friendly";

		if (t)
			s_scriptdata.server.friendlyFire = (int)Com_Clamp(0, 1, trap_Cvar_VariableValue(t));

		s_scriptdata.server.autoJoin = GUI_GetSkirmishCvarIntClamp(0, 1, s, "AutoJoin" );
		s_scriptdata.server.teamBalance = GUI_GetSkirmishCvarIntClamp(0, 1, s, "TeamBalance" );
	}
	else {
		s_scriptdata.server.autoJoin = 0;
		s_scriptdata.server.teamBalance = 0;
		s_scriptdata.server.friendlyFire = 0;
	}
}



/*
=================
StartServer_SaveServerScriptData

Saves server specific gametype data
=================
*/
static void StartServer_SaveServerScriptData(void)
{
	char* s;
	int friendly, gametype;

	GUI_SetSkirmishCvarInt( NULL, "gui_pure", s_scriptdata.server.pure);
	GUI_SetSkirmishCvarInt( NULL, "gui_dynamiclight", s_scriptdata.server.dynamiclight);

	GUI_SetSkirmishCvarInt( NULL, "gui_pmove_fixed", s_scriptdata.server.pmove_fixed);
	GUI_SetSkirmishCvarInt( NULL, "gui_pmove_msec", s_scriptdata.server.pmove_msec);
	GUI_SetSkirmishCvarInt( NULL, "gui_smoothclients", s_scriptdata.server.smoothclients);

	GUI_SetSkirmishCvarInt( NULL, "gui_syncclients", s_scriptdata.server.syncClients);

	GUI_SetSkirmishCvarInt( NULL, "gui_minping", s_scriptdata.server.minPing);
	GUI_SetSkirmishCvarInt( NULL, "gui_maxping", s_scriptdata.server.maxPing);
	GUI_SetSkirmishCvarInt( NULL, "gui_allowMinPing", s_scriptdata.server.allowMinPing);
	GUI_SetSkirmishCvarInt( NULL, "gui_allowMaxPing", s_scriptdata.server.allowMaxPing);

	GUI_SetSkirmishCvarInt( NULL, "gui_maxrate", s_scriptdata.server.maxrate);
	GUI_SetSkirmishCvarInt( NULL, "gui_allowmaxrate", s_scriptdata.server.allowmaxrate);
	GUI_SetSkirmishCvarInt( NULL, "gui_allowdownload", s_scriptdata.server.allowdownload);
	GUI_SetSkirmishCvarInt( NULL, "gui_allowvote", s_scriptdata.server.allowvote);
	GUI_SetSkirmishCvarInt( NULL, "gui_allowpass", s_scriptdata.server.allowpass);

	GUI_SetSkirmishCvarInt( NULL, "gui_netport", s_scriptdata.server.netport);
	GUI_SetSkirmishCvarInt( NULL, "gui_svfps", s_scriptdata.server.sv_fps);

	GUI_SetSkirmishCvar( NULL, "gui_password", s_scriptdata.server.password);

	GUI_SetSkirmishCvarInt( NULL, "gui_allowprivateclients", s_scriptdata.server.allowPrivateClients);
	GUI_SetSkirmishCvarInt( NULL, "gui_privateclients", s_scriptdata.server.privateClients);
	GUI_SetSkirmishCvar( NULL, "gui_privatepassword", s_scriptdata.server.privatePassword);

	GUI_SetSkirmishCvarInt( NULL, "gui_preventConfigBug", s_scriptdata.server.preventConfigBug);

	if (s_scriptdata.multiplayer) {
		GUI_SetSkirmishCvarInt( NULL, "gui_dedicated", s_scriptdata.server.dedicatedServer);
		GUI_SetSkirmishCvarInt( NULL, "gui_inactivity", s_scriptdata.server.inactivityTime);

		GUI_SetSkirmishCvarInt( NULL, "gui_strictAuth", s_scriptdata.server.strictAuth);
		GUI_SetSkirmishCvarInt( NULL, "gui_lanForceRate", s_scriptdata.server.lanForceRate);
	}
	
//Noire.dev cvars
GUI_SetSkirmishCvarInt( NULL, "gui_maxEntities", s_scriptdata.server.maxEntities);
GUI_SetSkirmishCvarInt( NULL, "gui_singleskill", s_scriptdata.server.singleskill);
GUI_SetSkirmishCvarInt( NULL, "gui_kill", s_scriptdata.server.kill);
GUI_SetSkirmishCvar( NULL, "gui_damageModifier", s_scriptdata.server.damageModifier);
GUI_SetSkirmishCvar( NULL, "gui_selectedmod", s_scriptdata.server.selectedmod);
GUI_SetSkirmishCvarInt( NULL, "gui_elimination", s_scriptdata.server.elimination);
GUI_SetSkirmishCvarInt( NULL, "gui_obeliskHealth", s_scriptdata.server.obeliskHealth);
GUI_SetSkirmishCvarInt( NULL, "gui_obeliskRegenPeriod", s_scriptdata.server.obeliskRegenPeriod);
GUI_SetSkirmishCvarInt( NULL, "gui_obeliskRegenAmount", s_scriptdata.server.obeliskRegenAmount);
GUI_SetSkirmishCvarInt( NULL, "gui_obeliskRespawnDelay", s_scriptdata.server.obeliskRespawnDelay);
GUI_SetSkirmishCvarInt( NULL, "gui_cubeTimeout", s_scriptdata.server.cubeTimeout);
GUI_SetSkirmishCvarInt( NULL, "gui_flagrespawn", s_scriptdata.server.flagrespawn);
GUI_SetSkirmishCvarInt( NULL, "gui_weaponTeamRespawn", s_scriptdata.server.weaponTeamRespawn);
GUI_SetSkirmishCvarInt( NULL, "gui_elimination_ctf_oneway", s_scriptdata.server.elimination_ctf_oneway);
GUI_SetSkirmishCvarInt( NULL, "gui_elimination_selfdamage", s_scriptdata.server.elimination_selfdamage);
GUI_SetSkirmishCvarInt( NULL, "gui_elimination_roundtime", s_scriptdata.server.elimination_roundtime);
GUI_SetSkirmishCvarInt( NULL, "gui_elimination_warmup", s_scriptdata.server.elimination_warmup);
GUI_SetSkirmishCvarInt( NULL, "gui_elimination_activewarmup", s_scriptdata.server.elimination_activewarmup);
GUI_SetSkirmishCvarInt( NULL, "gui_lms_lives", s_scriptdata.server.lms_lives);
GUI_SetSkirmishCvarInt( NULL, "gui_lms_mode", s_scriptdata.server.lms_mode);
GUI_SetSkirmishCvarInt( NULL, "gui_accelerate", s_scriptdata.server.accelerate);
GUI_SetSkirmishCvarInt( NULL, "gui_spectatorspeed", s_scriptdata.server.spectatorspeed);
GUI_SetSkirmishCvarInt( NULL, "gui_speed", s_scriptdata.server.speed);
GUI_SetSkirmishCvarInt( NULL, "gui_gravity", s_scriptdata.server.gravity);
GUI_SetSkirmishCvar( NULL, "gui_gravityModifier", s_scriptdata.server.gravityModifier);
GUI_SetSkirmishCvarInt( NULL, "gui_knockback", s_scriptdata.server.knockback);
GUI_SetSkirmishCvarInt( NULL, "gui_noplayerclip", s_scriptdata.server.noplayerclip);
GUI_SetSkirmishCvarInt( NULL, "gui_jumpheight", s_scriptdata.server.jumpheight);
GUI_SetSkirmishCvarInt( NULL, "gui_regenarmor", s_scriptdata.server.regenarmor);
GUI_SetSkirmishCvarInt( NULL, "gui_ammolimit", s_scriptdata.server.ammolimit);
GUI_SetSkirmishCvar( NULL, "gui_quadfactor", s_scriptdata.server.quadfactor);
GUI_SetSkirmishCvarInt( NULL, "gui_respawntime", s_scriptdata.server.respawntime);
GUI_SetSkirmishCvarInt( NULL, "gui_forcerespawn", s_scriptdata.server.forcerespawn);
GUI_SetSkirmishCvar( NULL, "gui_vampire", s_scriptdata.server.vampire);
GUI_SetSkirmishCvarInt( NULL, "gui_vampire_max_health", s_scriptdata.server.vampire_max_health);
GUI_SetSkirmishCvarInt( NULL, "gui_regen", s_scriptdata.server.regen);
GUI_SetSkirmishCvarInt( NULL, "gui_maxweaponpickup", s_scriptdata.server.maxweaponpickup);
GUI_SetSkirmishCvarInt( NULL, "gui_droppeditemtime", s_scriptdata.server.droppeditemtime);
GUI_SetSkirmishCvarInt( NULL, "gui_autoflagreturn", s_scriptdata.server.autoflagreturn);
GUI_SetSkirmishCvar( NULL, "gui_armorprotect", s_scriptdata.server.armorprotect);
GUI_SetSkirmishCvarInt( NULL, "gui_respawnwait", s_scriptdata.server.respawnwait);
GUI_SetSkirmishCvar( NULL, "gui_speedfactor", s_scriptdata.server.speedfactor);
GUI_SetSkirmishCvar( NULL, "gui_scoutspeedfactor", s_scriptdata.server.scoutspeedfactor);
GUI_SetSkirmishCvar( NULL, "gui_scoutfirespeed", s_scriptdata.server.scoutfirespeed);
GUI_SetSkirmishCvar( NULL, "gui_scoutdamagefactor", s_scriptdata.server.scoutdamagefactor);
GUI_SetSkirmishCvar( NULL, "gui_scoutgravitymodifier", s_scriptdata.server.scoutgravitymodifier);
GUI_SetSkirmishCvarInt( NULL, "gui_scout_infammo", s_scriptdata.server.scout_infammo);
GUI_SetSkirmishCvar( NULL, "gui_scouthealthmodifier", s_scriptdata.server.scouthealthmodifier);
GUI_SetSkirmishCvar( NULL, "gui_doublerfirespeed", s_scriptdata.server.doublerfirespeed);
GUI_SetSkirmishCvar( NULL, "gui_doublerdamagefactor", s_scriptdata.server.doublerdamagefactor);
GUI_SetSkirmishCvar( NULL, "gui_doublerspeedfactor", s_scriptdata.server.doublerspeedfactor);
GUI_SetSkirmishCvar( NULL, "gui_doublergravitymodifier", s_scriptdata.server.doublergravitymodifier);
GUI_SetSkirmishCvarInt( NULL, "gui_doubler_infammo", s_scriptdata.server.doubler_infammo);
GUI_SetSkirmishCvar( NULL, "gui_doublerhealthmodifier", s_scriptdata.server.doublerhealthmodifier);
GUI_SetSkirmishCvar( NULL, "gui_guardhealthmodifier", s_scriptdata.server.guardhealthmodifier);
GUI_SetSkirmishCvar( NULL, "gui_guardfirespeed", s_scriptdata.server.guardfirespeed);
GUI_SetSkirmishCvar( NULL, "gui_guarddamagefactor", s_scriptdata.server.guarddamagefactor);
GUI_SetSkirmishCvar( NULL, "gui_guardspeedfactor", s_scriptdata.server.guardspeedfactor);
GUI_SetSkirmishCvar( NULL, "gui_guardgravitymodifier", s_scriptdata.server.guardgravitymodifier);
GUI_SetSkirmishCvarInt( NULL, "gui_guard_infammo", s_scriptdata.server.guard_infammo);
GUI_SetSkirmishCvar( NULL, "gui_ammoregenfirespeed", s_scriptdata.server.ammoregenfirespeed);
GUI_SetSkirmishCvarInt( NULL, "gui_ammoregen_infammo", s_scriptdata.server.ammoregen_infammo);
GUI_SetSkirmishCvar( NULL, "gui_ammoregendamagefactor", s_scriptdata.server.ammoregendamagefactor);
GUI_SetSkirmishCvar( NULL, "gui_ammoregenspeedfactor", s_scriptdata.server.ammoregenspeedfactor);
GUI_SetSkirmishCvar( NULL, "gui_ammoregengravitymodifier", s_scriptdata.server.ammoregengravitymodifier);
GUI_SetSkirmishCvar( NULL, "gui_ammoregenhealthmodifier", s_scriptdata.server.ammoregenhealthmodifier);
GUI_SetSkirmishCvarInt( NULL, "gui_teamred_speed", s_scriptdata.server.teamred_speed);
GUI_SetSkirmishCvar( NULL, "gui_teamred_gravityModifier", s_scriptdata.server.teamred_gravityModifier);
GUI_SetSkirmishCvar( NULL, "gui_teamred_firespeed", s_scriptdata.server.teamred_firespeed);
GUI_SetSkirmishCvar( NULL, "gui_teamred_damage", s_scriptdata.server.teamred_damage);
GUI_SetSkirmishCvarInt( NULL, "gui_teamred_infammo", s_scriptdata.server.teamred_infammo);
GUI_SetSkirmishCvarInt( NULL, "gui_teamred_respawnwait", s_scriptdata.server.teamred_respawnwait);
GUI_SetSkirmishCvarInt( NULL, "gui_teamred_pickupitems", s_scriptdata.server.teamred_pickupitems);
GUI_SetSkirmishCvarInt( NULL, "gui_eliminationredrespawn", s_scriptdata.server.eliminationredrespawn);
GUI_SetSkirmishCvarInt( NULL, "gui_eliminationred_startHealth", s_scriptdata.server.eliminationred_startHealth);
GUI_SetSkirmishCvarInt( NULL, "gui_eliminationred_startArmor", s_scriptdata.server.eliminationred_startArmor);
GUI_SetSkirmishCvarInt( NULL, "gui_teamblue_speed", s_scriptdata.server.teamblue_speed);
GUI_SetSkirmishCvar( NULL, "gui_teamblue_gravityModifier", s_scriptdata.server.teamblue_gravityModifier);
GUI_SetSkirmishCvar( NULL, "gui_teamblue_firespeed", s_scriptdata.server.teamblue_firespeed);
GUI_SetSkirmishCvar( NULL, "gui_teamblue_damage", s_scriptdata.server.teamblue_damage);
GUI_SetSkirmishCvarInt( NULL, "gui_teamblue_infammo", s_scriptdata.server.teamblue_infammo);
GUI_SetSkirmishCvarInt( NULL, "gui_teamblue_respawnwait", s_scriptdata.server.teamblue_respawnwait);
GUI_SetSkirmishCvarInt( NULL, "gui_teamblue_pickupitems", s_scriptdata.server.teamblue_pickupitems);
GUI_SetSkirmishCvarInt( NULL, "gui_eliminationrespawn", s_scriptdata.server.eliminationrespawn);
GUI_SetSkirmishCvarInt( NULL, "gui_elimination_startHealth", s_scriptdata.server.elimination_startHealth);
GUI_SetSkirmishCvarInt( NULL, "gui_elimination_startArmor", s_scriptdata.server.elimination_startArmor);
GUI_SetSkirmishCvarInt( NULL, "gui_eliminationred_grapple", s_scriptdata.server.eliminationred_grapple);
GUI_SetSkirmishCvarInt( NULL, "gui_eliminationred_gauntlet", s_scriptdata.server.eliminationred_gauntlet);
GUI_SetSkirmishCvarInt( NULL, "gui_eliminationred_machinegun", s_scriptdata.server.eliminationred_machinegun);
GUI_SetSkirmishCvarInt( NULL, "gui_eliminationred_shotgun", s_scriptdata.server.eliminationred_shotgun);
GUI_SetSkirmishCvarInt( NULL, "gui_eliminationred_grenade", s_scriptdata.server.eliminationred_grenade);
GUI_SetSkirmishCvarInt( NULL, "gui_eliminationred_rocket", s_scriptdata.server.eliminationred_rocket);
GUI_SetSkirmishCvarInt( NULL, "gui_eliminationred_railgun", s_scriptdata.server.eliminationred_railgun);
GUI_SetSkirmishCvarInt( NULL, "gui_eliminationred_lightning", s_scriptdata.server.eliminationred_lightning);
GUI_SetSkirmishCvarInt( NULL, "gui_eliminationred_plasmagun", s_scriptdata.server.eliminationred_plasmagun);
GUI_SetSkirmishCvarInt( NULL, "gui_eliminationred_bfg", s_scriptdata.server.eliminationred_bfg);
GUI_SetSkirmishCvarInt( NULL, "gui_eliminationred_chain", s_scriptdata.server.eliminationred_chain);
GUI_SetSkirmishCvarInt( NULL, "gui_eliminationred_mine", s_scriptdata.server.eliminationred_mine);
GUI_SetSkirmishCvarInt( NULL, "gui_eliminationred_nail", s_scriptdata.server.eliminationred_nail);
GUI_SetSkirmishCvarInt( NULL, "gui_eliminationred_flame", s_scriptdata.server.eliminationred_flame);
GUI_SetSkirmishCvarInt( NULL, "gui_eliminationred_antimatter", s_scriptdata.server.eliminationred_antimatter);
GUI_SetSkirmishCvarInt( NULL, "gui_elimination_grapple", s_scriptdata.server.elimination_grapple);
GUI_SetSkirmishCvarInt( NULL, "gui_elimination_gauntlet", s_scriptdata.server.elimination_gauntlet);
GUI_SetSkirmishCvarInt( NULL, "gui_elimination_machinegun", s_scriptdata.server.elimination_machinegun);
GUI_SetSkirmishCvarInt( NULL, "gui_elimination_shotgun", s_scriptdata.server.elimination_shotgun);
GUI_SetSkirmishCvarInt( NULL, "gui_elimination_grenade", s_scriptdata.server.elimination_grenade);
GUI_SetSkirmishCvarInt( NULL, "gui_elimination_rocket", s_scriptdata.server.elimination_rocket);
GUI_SetSkirmishCvarInt( NULL, "gui_elimination_railgun", s_scriptdata.server.elimination_railgun);
GUI_SetSkirmishCvarInt( NULL, "gui_elimination_lightning", s_scriptdata.server.elimination_lightning);
GUI_SetSkirmishCvarInt( NULL, "gui_elimination_plasmagun", s_scriptdata.server.elimination_plasmagun);
GUI_SetSkirmishCvarInt( NULL, "gui_elimination_bfg", s_scriptdata.server.elimination_bfg);
GUI_SetSkirmishCvarInt( NULL, "gui_elimination_chain", s_scriptdata.server.elimination_chain);
GUI_SetSkirmishCvarInt( NULL, "gui_elimination_mine", s_scriptdata.server.elimination_mine);
GUI_SetSkirmishCvarInt( NULL, "gui_elimination_nail", s_scriptdata.server.elimination_nail);
GUI_SetSkirmishCvarInt( NULL, "gui_elimination_flame", s_scriptdata.server.elimination_flame);
GUI_SetSkirmishCvarInt( NULL, "gui_elimination_antimatter", s_scriptdata.server.elimination_antimatter);
GUI_SetSkirmishCvarInt( NULL, "gui_eliminationred_quad", s_scriptdata.server.eliminationred_quad);
GUI_SetSkirmishCvarInt( NULL, "gui_elimination_haste", s_scriptdata.server.elimination_haste);
GUI_SetSkirmishCvarInt( NULL, "gui_elimination_bsuit", s_scriptdata.server.elimination_bsuit);
GUI_SetSkirmishCvarInt( NULL, "gui_elimination_invis", s_scriptdata.server.elimination_invis);
GUI_SetSkirmishCvarInt( NULL, "gui_elimination_regen", s_scriptdata.server.elimination_regen);
GUI_SetSkirmishCvarInt( NULL, "gui_elimination_flight", s_scriptdata.server.elimination_flight);
GUI_SetSkirmishCvarInt( NULL, "gui_elimination_items", s_scriptdata.server.elimination_items);
GUI_SetSkirmishCvarInt( NULL, "gui_elimination_holdable", s_scriptdata.server.elimination_holdable);
GUI_SetSkirmishCvarInt( NULL, "gui_overlay", s_scriptdata.server.overlay);
GUI_SetSkirmishCvarInt( NULL, "gui_randomItems", s_scriptdata.server.randomItems);
GUI_SetSkirmishCvarInt( NULL, "gui_slickmove", s_scriptdata.server.slickmove);
GUI_SetSkirmishCvarInt( NULL, "gui_spawnprotect", s_scriptdata.server.spawnprotect);
GUI_SetSkirmishCvarInt( NULL, "gui_elimination_lockspectator", s_scriptdata.server.elimination_lockspectator);
GUI_SetSkirmishCvarInt( NULL, "gui_awardpushing", s_scriptdata.server.awardpushing);
GUI_SetSkirmishCvarInt( NULL, "gui_randomteleport", s_scriptdata.server.randomteleport);
GUI_SetSkirmishCvarInt( NULL, "gui_falldamagesmall", s_scriptdata.server.falldamagesmall);
GUI_SetSkirmishCvarInt( NULL, "gui_falldamagebig", s_scriptdata.server.falldamagebig);
GUI_SetSkirmishCvarInt( NULL, "gui_waterdamage", s_scriptdata.server.waterdamage);
GUI_SetSkirmishCvarInt( NULL, "gui_lavadamage", s_scriptdata.server.lavadamage);
GUI_SetSkirmishCvarInt( NULL, "gui_slimedamage", s_scriptdata.server.slimedamage);
GUI_SetSkirmishCvarInt( NULL, "gui_drowndamage", s_scriptdata.server.drowndamage);
GUI_SetSkirmishCvarInt( NULL, "gui_invulinf", s_scriptdata.server.invulinf);
GUI_SetSkirmishCvarInt( NULL, "gui_invulmove", s_scriptdata.server.invulmove);
GUI_SetSkirmishCvarInt( NULL, "gui_invultime", s_scriptdata.server.invultime);
GUI_SetSkirmishCvarInt( NULL, "gui_kamikazeinf", s_scriptdata.server.kamikazeinf);
GUI_SetSkirmishCvarInt( NULL, "gui_portalinf", s_scriptdata.server.portalinf);
GUI_SetSkirmishCvarInt( NULL, "gui_portaltimeout", s_scriptdata.server.portaltimeout);
GUI_SetSkirmishCvarInt( NULL, "gui_portalhealth", s_scriptdata.server.portalhealth);
GUI_SetSkirmishCvarInt( NULL, "gui_teleporterinf", s_scriptdata.server.teleporterinf);
GUI_SetSkirmishCvarInt( NULL, "gui_medkitinf", s_scriptdata.server.medkitinf);
GUI_SetSkirmishCvarInt( NULL, "gui_medkitlimit", s_scriptdata.server.medkitlimit);
GUI_SetSkirmishCvarInt( NULL, "gui_medkitmodifier", s_scriptdata.server.medkitmodifier);
GUI_SetSkirmishCvarInt( NULL, "gui_fasthealthregen", s_scriptdata.server.fasthealthregen);
GUI_SetSkirmishCvarInt( NULL, "gui_slowhealthregen", s_scriptdata.server.slowhealthregen);
GUI_SetSkirmishCvar( NULL, "gui_hastefirespeed", s_scriptdata.server.hastefirespeed);
GUI_SetSkirmishCvarInt( NULL, "gui_quadtime", s_scriptdata.server.quadtime);
GUI_SetSkirmishCvarInt( NULL, "gui_bsuittime", s_scriptdata.server.bsuittime);
GUI_SetSkirmishCvarInt( NULL, "gui_hastetime", s_scriptdata.server.hastetime);
GUI_SetSkirmishCvarInt( NULL, "gui_invistime", s_scriptdata.server.invistime);
GUI_SetSkirmishCvarInt( NULL, "gui_regentime", s_scriptdata.server.regentime);
GUI_SetSkirmishCvarInt( NULL, "gui_flighttime", s_scriptdata.server.flighttime);
GUI_SetSkirmishCvarInt( NULL, "gui_armorrespawn", s_scriptdata.server.armorrespawn);
GUI_SetSkirmishCvarInt( NULL, "gui_healthrespawn", s_scriptdata.server.healthrespawn);
GUI_SetSkirmishCvarInt( NULL, "gui_ammorespawn", s_scriptdata.server.ammorespawn);
GUI_SetSkirmishCvarInt( NULL, "gui_holdablerespawn", s_scriptdata.server.holdablerespawn);
GUI_SetSkirmishCvarInt( NULL, "gui_megahealthrespawn", s_scriptdata.server.megahealthrespawn);
GUI_SetSkirmishCvarInt( NULL, "gui_poweruprespawn", s_scriptdata.server.poweruprespawn);
GUI_SetSkirmishCvarInt( NULL, "gui_weaponrespawn", s_scriptdata.server.weaponrespawn);
// Крюк
GUI_SetSkirmishCvarInt( NULL, "gui_ghspeed", s_scriptdata.server.g_ghspeed);
GUI_SetSkirmishCvarInt( NULL, "gui_ghtimeout", s_scriptdata.server.g_ghtimeout);
// Пила
GUI_SetSkirmishCvarInt( NULL, "gui_gdelay", s_scriptdata.server.g_gdelay);
GUI_SetSkirmishCvarInt( NULL, "gui_gdamage", s_scriptdata.server.g_gdamage);
GUI_SetSkirmishCvarInt( NULL, "gui_grange", s_scriptdata.server.g_grange);
GUI_SetSkirmishCvar( NULL, "gui_gknockback", s_scriptdata.server.g_gknockback);
// Автомат
GUI_SetSkirmishCvarInt( NULL, "gui_mgammocount", s_scriptdata.server.g_mgammocount);
GUI_SetSkirmishCvarInt( NULL, "gui_mgweaponcount", s_scriptdata.server.g_mgweaponcount);
GUI_SetSkirmishCvarInt( NULL, "gui_mgdelay", s_scriptdata.server.g_mgdelay);
GUI_SetSkirmishCvarInt( NULL, "gui_mgdamage", s_scriptdata.server.g_mgdamage);
GUI_SetSkirmishCvarInt( NULL, "gui_mgspread", s_scriptdata.server.g_mgspread);
GUI_SetSkirmishCvarInt( NULL, "gui_mgexplode", s_scriptdata.server.g_mgexplode);
GUI_SetSkirmishCvarInt( NULL, "gui_mgsdamage", s_scriptdata.server.g_mgsdamage);
GUI_SetSkirmishCvarInt( NULL, "gui_mgsradius", s_scriptdata.server.g_mgsradius);
GUI_SetSkirmishCvarInt( NULL, "gui_mgvampire", s_scriptdata.server.g_mgvampire);
GUI_SetSkirmishCvarInt( NULL, "gui_mginf", s_scriptdata.server.g_mginf);
GUI_SetSkirmishCvar( NULL, "gui_mgknockback", s_scriptdata.server.g_mgknockback);
// Дробовик
GUI_SetSkirmishCvarInt( NULL, "gui_sgammocount", s_scriptdata.server.g_sgammocount);
GUI_SetSkirmishCvarInt( NULL, "gui_sgweaponcount", s_scriptdata.server.g_sgweaponcount);
GUI_SetSkirmishCvarInt( NULL, "gui_sgdelay", s_scriptdata.server.g_sgdelay);
GUI_SetSkirmishCvarInt( NULL, "gui_sgdamage", s_scriptdata.server.g_sgdamage);
GUI_SetSkirmishCvarInt( NULL, "gui_sgspread", s_scriptdata.server.g_sgspread);
GUI_SetSkirmishCvarInt( NULL, "gui_sgexplode", s_scriptdata.server.g_sgexplode);
GUI_SetSkirmishCvarInt( NULL, "gui_sgsdamage", s_scriptdata.server.g_sgsdamage);
GUI_SetSkirmishCvarInt( NULL, "gui_sgsradius", s_scriptdata.server.g_sgsradius);
GUI_SetSkirmishCvarInt( NULL, "gui_sgcount", s_scriptdata.server.g_sgcount);
GUI_SetSkirmishCvarInt( NULL, "gui_sgvampire", s_scriptdata.server.g_sgvampire);
GUI_SetSkirmishCvarInt( NULL, "gui_sginf", s_scriptdata.server.g_sginf);
GUI_SetSkirmishCvar( NULL, "gui_sgknockback", s_scriptdata.server.g_sgknockback);
// Гранаты
GUI_SetSkirmishCvarInt( NULL, "gui_glammocount", s_scriptdata.server.g_glammocount);
GUI_SetSkirmishCvarInt( NULL, "gui_glweaponcount", s_scriptdata.server.g_glweaponcount);
GUI_SetSkirmishCvarInt( NULL, "gui_gldelay", s_scriptdata.server.g_gldelay);
GUI_SetSkirmishCvarInt( NULL, "gui_glspeed", s_scriptdata.server.g_glspeed);
GUI_SetSkirmishCvarInt( NULL, "gui_glbounce", s_scriptdata.server.g_glbounce);
GUI_SetSkirmishCvarInt( NULL, "gui_glgravity", s_scriptdata.server.g_glgravity);
GUI_SetSkirmishCvarInt( NULL, "gui_gltimeout", s_scriptdata.server.g_gltimeout);
GUI_SetSkirmishCvarInt( NULL, "gui_glsradius", s_scriptdata.server.g_glsradius);
GUI_SetSkirmishCvarInt( NULL, "gui_glsdamage", s_scriptdata.server.g_glsdamage);
GUI_SetSkirmishCvarInt( NULL, "gui_gldamage", s_scriptdata.server.g_gldamage);
GUI_SetSkirmishCvarInt( NULL, "gui_glvampire", s_scriptdata.server.g_glvampire);
GUI_SetSkirmishCvarInt( NULL, "gui_glinf", s_scriptdata.server.g_glinf);
GUI_SetSkirmishCvar( NULL, "gui_glbouncemodifier", s_scriptdata.server.g_glbouncemodifier);
GUI_SetSkirmishCvar( NULL, "gui_glknockback", s_scriptdata.server.g_glknockback);
GUI_SetSkirmishCvarInt( NULL, "gui_glhoming", s_scriptdata.server.g_glhoming);
GUI_SetSkirmishCvarInt( NULL, "gui_glguided", s_scriptdata.server.g_glguided);
// Ракеты
GUI_SetSkirmishCvarInt( NULL, "gui_rlammocount", s_scriptdata.server.g_rlammocount);
GUI_SetSkirmishCvarInt( NULL, "gui_rlweaponcount", s_scriptdata.server.g_rlweaponcount);
GUI_SetSkirmishCvarInt( NULL, "gui_rldelay", s_scriptdata.server.g_rldelay);
GUI_SetSkirmishCvarInt( NULL, "gui_rlspeed", s_scriptdata.server.g_rlspeed);
GUI_SetSkirmishCvarInt( NULL, "gui_rlbounce", s_scriptdata.server.g_rlbounce);
GUI_SetSkirmishCvarInt( NULL, "gui_rlgravity", s_scriptdata.server.g_rlgravity);
GUI_SetSkirmishCvarInt( NULL, "gui_rltimeout", s_scriptdata.server.g_rltimeout);
GUI_SetSkirmishCvarInt( NULL, "gui_rlsradius", s_scriptdata.server.g_rlsradius);
GUI_SetSkirmishCvarInt( NULL, "gui_rlsdamage", s_scriptdata.server.g_rlsdamage);
GUI_SetSkirmishCvarInt( NULL, "gui_rldamage", s_scriptdata.server.g_rldamage);
GUI_SetSkirmishCvarInt( NULL, "gui_rlvampire", s_scriptdata.server.g_rlvampire);
GUI_SetSkirmishCvarInt( NULL, "gui_rlinf", s_scriptdata.server.g_rlinf);
GUI_SetSkirmishCvar( NULL, "gui_rlbouncemodifier", s_scriptdata.server.g_rlbouncemodifier);
GUI_SetSkirmishCvar( NULL, "gui_rlknockback", s_scriptdata.server.g_rlknockback);
GUI_SetSkirmishCvarInt( NULL, "gui_rlhoming", s_scriptdata.server.g_rlhoming);
GUI_SetSkirmishCvarInt( NULL, "gui_rlguided", s_scriptdata.server.g_rlguided);
// Молния
GUI_SetSkirmishCvarInt( NULL, "gui_lgammocount", s_scriptdata.server.g_lgammocount);
GUI_SetSkirmishCvarInt( NULL, "gui_lgweaponcount", s_scriptdata.server.g_lgweaponcount);
GUI_SetSkirmishCvarInt( NULL, "gui_lgrange", s_scriptdata.server.g_lgrange);
GUI_SetSkirmishCvarInt( NULL, "gui_lgdelay", s_scriptdata.server.g_lgdelay);
GUI_SetSkirmishCvarInt( NULL, "gui_lgdamage", s_scriptdata.server.g_lgdamage);
GUI_SetSkirmishCvarInt( NULL, "gui_lgvampire", s_scriptdata.server.g_lgvampire);
GUI_SetSkirmishCvarInt( NULL, "gui_lgexplode", s_scriptdata.server.g_lgexplode);
GUI_SetSkirmishCvarInt( NULL, "gui_lgsdamage", s_scriptdata.server.g_lgsdamage);
GUI_SetSkirmishCvarInt( NULL, "gui_lgsradius", s_scriptdata.server.g_lgsradius);
GUI_SetSkirmishCvarInt( NULL, "gui_lginf", s_scriptdata.server.g_lginf);
GUI_SetSkirmishCvar( NULL, "gui_lgknockback", s_scriptdata.server.g_lgknockback);
// Рэйлган
GUI_SetSkirmishCvarInt( NULL, "gui_rgammocount", s_scriptdata.server.g_rgammocount);
GUI_SetSkirmishCvarInt( NULL, "gui_rgweaponcount", s_scriptdata.server.g_rgweaponcount);
GUI_SetSkirmishCvarInt( NULL, "gui_rgdelay", s_scriptdata.server.g_rgdelay);
GUI_SetSkirmishCvarInt( NULL, "gui_rgdamage", s_scriptdata.server.g_rgdamage);
GUI_SetSkirmishCvarInt( NULL, "gui_rgvampire", s_scriptdata.server.g_rgvampire);
GUI_SetSkirmishCvarInt( NULL, "gui_rginf", s_scriptdata.server.g_rginf);
GUI_SetSkirmishCvar( NULL, "gui_rgknockback", s_scriptdata.server.g_rgknockback);
// Плазмаган
GUI_SetSkirmishCvarInt( NULL, "gui_pgammocount", s_scriptdata.server.g_pgammocount);
GUI_SetSkirmishCvarInt( NULL, "gui_pgweaponcount", s_scriptdata.server.g_pgweaponcount);
GUI_SetSkirmishCvarInt( NULL, "gui_pgdelay", s_scriptdata.server.g_pgdelay);
GUI_SetSkirmishCvarInt( NULL, "gui_pgspeed", s_scriptdata.server.g_pgspeed);
GUI_SetSkirmishCvarInt( NULL, "gui_pgbounce", s_scriptdata.server.g_pgbounce);
GUI_SetSkirmishCvarInt( NULL, "gui_pggravity", s_scriptdata.server.g_pggravity);
GUI_SetSkirmishCvarInt( NULL, "gui_pgtimeout", s_scriptdata.server.g_pgtimeout);
GUI_SetSkirmishCvarInt( NULL, "gui_pgsradius", s_scriptdata.server.g_pgsradius);
GUI_SetSkirmishCvarInt( NULL, "gui_pgsdamage", s_scriptdata.server.g_pgsdamage);
GUI_SetSkirmishCvarInt( NULL, "gui_pgdamage", s_scriptdata.server.g_pgdamage);
GUI_SetSkirmishCvarInt( NULL, "gui_pgvampire", s_scriptdata.server.g_pgvampire);
GUI_SetSkirmishCvarInt( NULL, "gui_pginf", s_scriptdata.server.g_pginf);
GUI_SetSkirmishCvar( NULL, "gui_pgbouncemodifier", s_scriptdata.server.g_pgbouncemodifier);
GUI_SetSkirmishCvar( NULL, "gui_pgknockback", s_scriptdata.server.g_pgknockback);
GUI_SetSkirmishCvarInt( NULL, "gui_pghoming", s_scriptdata.server.g_pghoming);
GUI_SetSkirmishCvarInt( NULL, "gui_pgguided", s_scriptdata.server.g_pgguided);
// Бфг
GUI_SetSkirmishCvarInt( NULL, "gui_bfgammocount", s_scriptdata.server.g_bfgammocount);
GUI_SetSkirmishCvarInt( NULL, "gui_bfgweaponcount", s_scriptdata.server.g_bfgweaponcount);
GUI_SetSkirmishCvarInt( NULL, "gui_bfgdelay", s_scriptdata.server.g_bfgdelay);
GUI_SetSkirmishCvarInt( NULL, "gui_bfgspeed", s_scriptdata.server.g_bfgspeed);
GUI_SetSkirmishCvarInt( NULL, "gui_bfgbounce", s_scriptdata.server.g_bfgbounce);
GUI_SetSkirmishCvarInt( NULL, "gui_bfggravity", s_scriptdata.server.g_bfggravity);
GUI_SetSkirmishCvarInt( NULL, "gui_bfgtimeout", s_scriptdata.server.g_bfgtimeout);
GUI_SetSkirmishCvarInt( NULL, "gui_bfgsradius", s_scriptdata.server.g_bfgsradius);
GUI_SetSkirmishCvarInt( NULL, "gui_bfgsdamage", s_scriptdata.server.g_bfgsdamage);
GUI_SetSkirmishCvarInt( NULL, "gui_bfgdamage", s_scriptdata.server.g_bfgdamage);
GUI_SetSkirmishCvarInt( NULL, "gui_bfgvampire", s_scriptdata.server.g_bfgvampire);
GUI_SetSkirmishCvarInt( NULL, "gui_bfginf", s_scriptdata.server.g_bfginf);
GUI_SetSkirmishCvar( NULL, "gui_bfgbouncemodifier", s_scriptdata.server.g_bfgbouncemodifier);
GUI_SetSkirmishCvar( NULL, "gui_bfgknockback", s_scriptdata.server.g_bfgknockback);
GUI_SetSkirmishCvarInt( NULL, "gui_bfghoming", s_scriptdata.server.g_bfghoming);
GUI_SetSkirmishCvarInt( NULL, "gui_bfgguided", s_scriptdata.server.g_bfgguided);
// Гвоздомёт
GUI_SetSkirmishCvarInt( NULL, "gui_ngammocount", s_scriptdata.server.g_ngammocount);
GUI_SetSkirmishCvarInt( NULL, "gui_ngweaponcount", s_scriptdata.server.g_ngweaponcount);
GUI_SetSkirmishCvarInt( NULL, "gui_ngdelay", s_scriptdata.server.g_ngdelay);
GUI_SetSkirmishCvarInt( NULL, "gui_ngspeed", s_scriptdata.server.g_ngspeed);
GUI_SetSkirmishCvarInt( NULL, "gui_ngbounce", s_scriptdata.server.g_ngbounce);
GUI_SetSkirmishCvarInt( NULL, "gui_nggravity", s_scriptdata.server.g_nggravity);
GUI_SetSkirmishCvarInt( NULL, "gui_ngtimeout", s_scriptdata.server.g_ngtimeout);
GUI_SetSkirmishCvarInt( NULL, "gui_ngcount", s_scriptdata.server.g_ngcount);
GUI_SetSkirmishCvarInt( NULL, "gui_ngspread", s_scriptdata.server.g_ngspread);
GUI_SetSkirmishCvarInt( NULL, "gui_ngdamage", s_scriptdata.server.g_ngdamage);
GUI_SetSkirmishCvarInt( NULL, "gui_ngrandom", s_scriptdata.server.g_ngrandom);
GUI_SetSkirmishCvarInt( NULL, "gui_ngvampire", s_scriptdata.server.g_ngvampire);
GUI_SetSkirmishCvarInt( NULL, "gui_nginf", s_scriptdata.server.g_nginf);
GUI_SetSkirmishCvar( NULL, "gui_ngbouncemodifier", s_scriptdata.server.g_ngbouncemodifier);
GUI_SetSkirmishCvar( NULL, "gui_ngknockback", s_scriptdata.server.g_ngknockback);
GUI_SetSkirmishCvarInt( NULL, "gui_nghoming", s_scriptdata.server.g_nghoming);
GUI_SetSkirmishCvarInt( NULL, "gui_ngguided", s_scriptdata.server.g_ngguided);
// Мины
GUI_SetSkirmishCvarInt( NULL, "gui_plammocount", s_scriptdata.server.g_plammocount);
GUI_SetSkirmishCvarInt( NULL, "gui_plweaponcount", s_scriptdata.server.g_plweaponcount);
GUI_SetSkirmishCvarInt( NULL, "gui_pldelay", s_scriptdata.server.g_pldelay);
GUI_SetSkirmishCvarInt( NULL, "gui_plspeed", s_scriptdata.server.g_plspeed);
GUI_SetSkirmishCvarInt( NULL, "gui_plgravity", s_scriptdata.server.g_plgravity);
GUI_SetSkirmishCvarInt( NULL, "gui_pltimeout", s_scriptdata.server.g_pltimeout);
GUI_SetSkirmishCvarInt( NULL, "gui_plsradius", s_scriptdata.server.g_plsradius);
GUI_SetSkirmishCvarInt( NULL, "gui_plsdamage", s_scriptdata.server.g_plsdamage);
GUI_SetSkirmishCvarInt( NULL, "gui_pldamage", s_scriptdata.server.g_pldamage);
GUI_SetSkirmishCvarInt( NULL, "gui_plvampire", s_scriptdata.server.g_plvampire);
GUI_SetSkirmishCvarInt( NULL, "gui_plinf", s_scriptdata.server.g_plinf);
GUI_SetSkirmishCvar( NULL, "gui_plknockback", s_scriptdata.server.g_plknockback);
// Пулемёт
GUI_SetSkirmishCvarInt( NULL, "gui_cgammocount", s_scriptdata.server.g_cgammocount);
GUI_SetSkirmishCvarInt( NULL, "gui_cgweaponcount", s_scriptdata.server.g_cgweaponcount);
GUI_SetSkirmishCvarInt( NULL, "gui_cgdelay", s_scriptdata.server.g_cgdelay);
GUI_SetSkirmishCvarInt( NULL, "gui_cgspread", s_scriptdata.server.g_cgspread);
GUI_SetSkirmishCvarInt( NULL, "gui_cgdamage", s_scriptdata.server.g_cgdamage);
GUI_SetSkirmishCvarInt( NULL, "gui_cgvampire", s_scriptdata.server.g_cgvampire);
GUI_SetSkirmishCvarInt( NULL, "gui_cginf", s_scriptdata.server.g_cginf);
GUI_SetSkirmishCvar( NULL, "gui_cgknockback", s_scriptdata.server.g_cgknockback);
// Огнемёт
GUI_SetSkirmishCvarInt( NULL, "gui_ftammocount", s_scriptdata.server.g_ftammocount);
GUI_SetSkirmishCvarInt( NULL, "gui_ftweaponcount", s_scriptdata.server.g_ftweaponcount);
GUI_SetSkirmishCvarInt( NULL, "gui_ftdelay", s_scriptdata.server.g_ftdelay);
GUI_SetSkirmishCvarInt( NULL, "gui_ftspeed", s_scriptdata.server.g_ftspeed);
GUI_SetSkirmishCvarInt( NULL, "gui_ftbounce", s_scriptdata.server.g_ftbounce);
GUI_SetSkirmishCvarInt( NULL, "gui_ftgravity", s_scriptdata.server.g_ftgravity);
GUI_SetSkirmishCvarInt( NULL, "gui_fttimeout", s_scriptdata.server.g_fttimeout);
GUI_SetSkirmishCvarInt( NULL, "gui_ftsradius", s_scriptdata.server.g_ftsradius);
GUI_SetSkirmishCvarInt( NULL, "gui_ftsdamage", s_scriptdata.server.g_ftsdamage);
GUI_SetSkirmishCvarInt( NULL, "gui_ftdamage", s_scriptdata.server.g_ftdamage);
GUI_SetSkirmishCvarInt( NULL, "gui_ftvampire", s_scriptdata.server.g_ftvampire);
GUI_SetSkirmishCvarInt( NULL, "gui_ftinf", s_scriptdata.server.g_ftinf);
GUI_SetSkirmishCvar( NULL, "gui_ftbouncemodifier", s_scriptdata.server.g_ftbouncemodifier);
GUI_SetSkirmishCvar( NULL, "gui_ftknockback", s_scriptdata.server.g_ftknockback);
GUI_SetSkirmishCvarInt( NULL, "gui_fthoming", s_scriptdata.server.g_fthoming);
GUI_SetSkirmishCvarInt( NULL, "gui_ftguided", s_scriptdata.server.g_ftguided);
// Антиматтер
GUI_SetSkirmishCvarInt( NULL, "gui_amweaponcount", s_scriptdata.server.g_amweaponcount);
GUI_SetSkirmishCvarInt( NULL, "gui_amdelay", s_scriptdata.server.g_amdelay);
GUI_SetSkirmishCvarInt( NULL, "gui_amspeed", s_scriptdata.server.g_amspeed);
GUI_SetSkirmishCvarInt( NULL, "gui_ambounce", s_scriptdata.server.g_ambounce);
GUI_SetSkirmishCvarInt( NULL, "gui_amgravity", s_scriptdata.server.g_amgravity);
GUI_SetSkirmishCvarInt( NULL, "gui_amtimeout", s_scriptdata.server.g_amtimeout);
GUI_SetSkirmishCvarInt( NULL, "gui_amsradius", s_scriptdata.server.g_amsradius);
GUI_SetSkirmishCvarInt( NULL, "gui_amsdamage", s_scriptdata.server.g_amsdamage);
GUI_SetSkirmishCvarInt( NULL, "gui_amdamage", s_scriptdata.server.g_amdamage);
GUI_SetSkirmishCvarInt( NULL, "gui_amvampire", s_scriptdata.server.g_amvampire);
GUI_SetSkirmishCvarInt( NULL, "gui_aminf", s_scriptdata.server.g_aminf);
GUI_SetSkirmishCvar( NULL, "gui_ambouncemodifier", s_scriptdata.server.g_ambouncemodifier);
GUI_SetSkirmishCvar( NULL, "gui_amknockback", s_scriptdata.server.g_amknockback);
GUI_SetSkirmishCvarInt( NULL, "gui_amhoming", s_scriptdata.server.g_amhoming);
GUI_SetSkirmishCvarInt( NULL, "gui_amguided", s_scriptdata.server.g_amguided);

	//
	// save gametype specific data
	//

	gametype = s_scriptdata.gametype;
	s = gametype_cvar_base[ gametype ];

	//
	// save state values
	//

	GUI_SetSkirmishCvar(s, "hostname", s_scriptdata.server.hostname);
	//GUI_SetSkirmishCvarInt(s, "ForceRespawn", s_scriptdata.server.forceRespawn);

	GUI_SetSkirmishCvarInt(s, "doWarmup", s_scriptdata.server.allowWarmup);
	GUI_SetSkirmishCvarInt(s, "WarmUp", s_scriptdata.server.warmupTime);
	GUI_SetSkirmishCvarInt(s, "viewdistance", s_scriptdata.server.viewdistance);

	if (gametype >= GT_TEAM && !(gametype == GT_LMS) ) {
		// ff is an existing cvar, so we use the existing cvar 
		friendly = s_scriptdata.server.friendlyFire;
		if (gametype == GT_TEAM) {
			trap_Cvar_SetValue( "ui_team_friendly", friendly );
		}
		else if (gametype == GT_CTF) {
			trap_Cvar_SetValue( "ui_ctf_friendly", friendly );
		}

		GUI_SetSkirmishCvarInt(s, "AutoJoin", s_scriptdata.server.autoJoin);
		GUI_SetSkirmishCvarInt(s, "TeamBalance", s_scriptdata.server.teamBalance);
	}
}



/*
==========================================================================

INIT SCRIPT DATA

==========================================================================
*/



/*
=================
StartServer_LoadScriptDataFromType

Loads script data for the give game type
=================
*/
void StartServer_LoadScriptDataFromType(int gametype)
{
	s_scriptdata.gametype = gametype;

	StartServer_LoadMapScriptData();
	StartServer_LoadBotScriptData();
	StartServer_LoadItemScriptData();
	StartServer_LoadServerScriptData();
}




/*
=================
StartServer_InitScriptData

Loads all script data
=================
*/
void StartServer_InitScriptData(qboolean multiplayer)
{
	memset(&s_scriptdata, 0, sizeof(scriptdata_t));

	GUI_StartServer_LoadSkirmishCvars();

	s_scriptdata.multiplayer = multiplayer;
	StartServer_LoadScriptDataFromType((int)Com_Clamp( 0, MAX_GAME_TYPE, GUI_GetSkirmishCvarInt( NULL, "gui_gametype" ) ));
	StartServer_BuildMapDistribution();
}





/*
=================
StartServer_SaveScriptData

Saves all script data
=================
*/
void StartServer_SaveScriptData(void)
{
	GUI_SetSkirmishCvarInt( NULL, "gui_gametype", s_scriptdata.gametype );

	StartServer_SaveMapScriptData();
	StartServer_SaveBotScriptData();
	StartServer_SaveItemScriptData();
	StartServer_SaveServerScriptData();

	GUI_StartServer_SaveSkirmishCvars();
}




