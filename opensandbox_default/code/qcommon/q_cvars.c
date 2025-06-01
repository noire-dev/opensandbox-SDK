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
#include "ns_local.h"

typedef struct {
	vmCvar_t	*vmCvar;
	char		*cvarName;
	char		*defaultString;
	int			cvarFlags;
} cvarTable_t;

#ifdef GAME
vmCvar_t    g_cheats;
vmCvar_t    g_physimpact;
vmCvar_t    g_physimpulse;
vmCvar_t    g_physdamage;
vmCvar_t    g_ghspeed;
vmCvar_t    g_ghtimeout;
vmCvar_t    g_gdelay;
vmCvar_t    g_gdamage;
vmCvar_t    g_grange;
vmCvar_t    g_gknockback;
vmCvar_t    g_mgdelay;
vmCvar_t    g_mgdamage;
vmCvar_t    g_mgspread;
vmCvar_t    g_mgexplode;
vmCvar_t    g_mgsdamage;
vmCvar_t    g_mgsradius;
vmCvar_t    g_mginf;
vmCvar_t    g_mgknockback;
vmCvar_t    g_sgdelay;
vmCvar_t    g_sgdamage;
vmCvar_t    g_sgspread;
vmCvar_t    g_sgexplode;
vmCvar_t    g_sgsdamage;
vmCvar_t    g_sgsradius;
vmCvar_t    g_sgcount;
vmCvar_t    g_sginf;
vmCvar_t    g_sgknockback;
vmCvar_t    g_gldelay;
vmCvar_t    g_glspeed;
vmCvar_t    g_gltimeout;
vmCvar_t    g_glsradius;
vmCvar_t    g_glsdamage;
vmCvar_t    g_gldamage;
vmCvar_t    g_glbounce;
vmCvar_t    g_glgravity;
vmCvar_t    g_glinf;
vmCvar_t    g_glbouncemodifier;
vmCvar_t    g_glknockback;
vmCvar_t    g_rldelay;
vmCvar_t    g_rlspeed;
vmCvar_t    g_rltimeout;
vmCvar_t    g_rlsradius;
vmCvar_t    g_rlsdamage;
vmCvar_t    g_rldamage;
vmCvar_t    g_rlbounce;
vmCvar_t    g_rlgravity;
vmCvar_t    g_rlinf;
vmCvar_t    g_rlbouncemodifier;
vmCvar_t    g_rlknockback;
vmCvar_t    g_lgdamage;
vmCvar_t    g_lgdelay;
vmCvar_t    g_lgrange;
vmCvar_t    g_lgexplode;
vmCvar_t    g_lgsdamage;
vmCvar_t    g_lgsradius;
vmCvar_t    g_lginf;
vmCvar_t    g_lgknockback;
vmCvar_t    g_rgdelay;
vmCvar_t    g_rgdamage;
vmCvar_t    g_rginf;
vmCvar_t    g_rgknockback;
vmCvar_t    g_pgdelay;
vmCvar_t    g_pgsradius;
vmCvar_t    g_pgspeed;
vmCvar_t    g_pgsdamage;
vmCvar_t    g_pgdamage;
vmCvar_t    g_pgtimeout;
vmCvar_t    g_pgbounce;
vmCvar_t    g_pggravity;
vmCvar_t    g_pginf;
vmCvar_t    g_pgbouncemodifier;
vmCvar_t    g_pgknockback;
vmCvar_t    g_bfgdelay;
vmCvar_t    g_bfgspeed;
vmCvar_t    g_bfgtimeout;
vmCvar_t    g_bfgsradius;
vmCvar_t    g_bfgsdamage;
vmCvar_t    g_bfgdamage;
vmCvar_t    g_bfgbounce;
vmCvar_t    g_bfggravity;
vmCvar_t    g_bfginf;
vmCvar_t    g_bfgbouncemodifier;
vmCvar_t    g_bfgknockback;
vmCvar_t    g_ngdelay;
vmCvar_t    g_ngspeed;
vmCvar_t    g_ngspread;
vmCvar_t    g_ngdamage;
vmCvar_t    g_ngtimeout;
vmCvar_t    g_ngcount;
vmCvar_t    g_ngbounce;
vmCvar_t    g_nggravity;
vmCvar_t    g_ngrandom;
vmCvar_t    g_nginf;
vmCvar_t    g_ngbouncemodifier;
vmCvar_t    g_ngknockback;
vmCvar_t    g_pldelay;
vmCvar_t    g_plspeed;
vmCvar_t    g_pltimeout;
vmCvar_t    g_plsradius;
vmCvar_t    g_plsdamage;
vmCvar_t    g_plgravity;
vmCvar_t    g_pldamage;
vmCvar_t    g_plinf;
vmCvar_t    g_plknockback;
vmCvar_t    g_cgdelay;
vmCvar_t    g_cgdamage;
vmCvar_t    g_cgspread;
vmCvar_t    g_cginf;
vmCvar_t    g_cgknockback;
vmCvar_t    g_ftdelay;
vmCvar_t    g_ftsradius;
vmCvar_t    g_ftspeed;
vmCvar_t    g_ftsdamage;
vmCvar_t    g_ftdamage;
vmCvar_t    g_fttimeout;
vmCvar_t    g_ftbounce;
vmCvar_t    g_ftgravity;
vmCvar_t    g_ftinf;
vmCvar_t    g_ftbouncemodifier;
vmCvar_t    g_ftknockback;
vmCvar_t    g_amdelay;
vmCvar_t    g_amsradius;
vmCvar_t    g_amspeed;
vmCvar_t    g_amsdamage;
vmCvar_t    g_amdamage;
vmCvar_t    g_amtimeout;
vmCvar_t    g_ambounce;
vmCvar_t    g_amgravity;
vmCvar_t    g_aminf;
vmCvar_t    g_ambouncemodifier;
vmCvar_t    g_amknockback;
vmCvar_t    g_glhoming;
vmCvar_t    g_glguided;
vmCvar_t    g_rlhoming;
vmCvar_t    g_rlguided;
vmCvar_t    g_pghoming;
vmCvar_t    g_pgguided;
vmCvar_t    g_bfghoming;
vmCvar_t    g_bfgguided;
vmCvar_t    g_nghoming;
vmCvar_t    g_ngguided;
vmCvar_t    g_fthoming;
vmCvar_t    g_ftguided;
vmCvar_t    g_amhoming;
vmCvar_t    g_amguided;
vmCvar_t    g_scoutspeedfactor;
vmCvar_t    g_scoutfirespeed;
vmCvar_t    g_scoutdamagefactor;
vmCvar_t    g_scoutgravitymodifier;
vmCvar_t    g_scout_infammo;
vmCvar_t    g_scouthealthmodifier;
vmCvar_t    g_doublerspeedfactor;
vmCvar_t    g_doublerfirespeed;
vmCvar_t    g_doublerdamagefactor;
vmCvar_t    g_doublergravitymodifier;
vmCvar_t    g_doubler_infammo;
vmCvar_t    g_doublerhealthmodifier;
vmCvar_t    g_guardhealthmodifier;
vmCvar_t    g_guardspeedfactor;
vmCvar_t    g_guardfirespeed;
vmCvar_t    g_guarddamagefactor;
vmCvar_t    g_guardgravitymodifier;
vmCvar_t    g_guard_infammo;
vmCvar_t    g_ammoregenspeedfactor;
vmCvar_t    g_ammoregenfirespeed;
vmCvar_t    g_ammoregen_infammo;
vmCvar_t    g_ammoregendamagefactor;
vmCvar_t    g_ammoregengravitymodifier;
vmCvar_t    g_ammoregenhealthmodifier;
vmCvar_t    g_teamblue_speed;
vmCvar_t    g_teamblue_gravityModifier;
vmCvar_t    g_teamblue_firespeed;
vmCvar_t    g_teamblue_damage;
vmCvar_t    g_teamblue_infammo;
vmCvar_t    g_teamblue_respawnwait;
vmCvar_t    g_teamblue_pickupitems;
vmCvar_t    g_teamred_speed;
vmCvar_t    g_teamred_gravityModifier;
vmCvar_t    g_teamred_firespeed;
vmCvar_t    g_teamred_damage;
vmCvar_t    g_teamred_infammo;
vmCvar_t    g_teamred_respawnwait;
vmCvar_t    g_teamred_pickupitems;
vmCvar_t    g_skyColorR;
vmCvar_t    g_skyColorG;
vmCvar_t    g_skyColorB;
vmCvar_t    g_skyColorA;
vmCvar_t    g_entitypack;
vmCvar_t    g_extendedsandbox;
vmCvar_t    g_allowprops;
vmCvar_t    g_allownpc;
vmCvar_t    g_allowitems;
vmCvar_t    g_allownoclip;
vmCvar_t    g_allowtoolgun;
vmCvar_t    g_allowphysgun;
vmCvar_t    g_allowgravitygun;
vmCvar_t    g_maxEntities;
vmCvar_t    cl_selectedmod;
vmCvar_t    cl_language;
vmCvar_t    g_regenarmor;
vmCvar_t    g_movetype;
vmCvar_t    g_accelerate;
vmCvar_t    g_randomItems;
vmCvar_t    g_kamikazeinf;
vmCvar_t    g_invulinf;
vmCvar_t    g_medkitinf;
vmCvar_t    g_teleporterinf;
vmCvar_t    g_portalinf;
vmCvar_t    g_medkitlimit;
vmCvar_t    g_waterdamage;
vmCvar_t    g_lavadamage;
vmCvar_t    g_slimedamage;
vmCvar_t    g_nextbot_speed;
vmCvar_t    g_randomteleport;
vmCvar_t    g_falldamagesmall;
vmCvar_t    g_falldamagebig;
vmCvar_t    g_noplayerclip;
vmCvar_t    g_flagrespawn;
vmCvar_t    g_portaltimeout;
vmCvar_t    g_portalhealth;
vmCvar_t    g_quadtime;
vmCvar_t    g_bsuittime;
vmCvar_t    g_hastetime;
vmCvar_t    g_invistime;
vmCvar_t    g_regentime;
vmCvar_t    g_flighttime;
vmCvar_t    g_invulmove;
vmCvar_t    g_invultime;
vmCvar_t    g_fasthealthregen;
vmCvar_t    g_slowhealthregen;
vmCvar_t    g_hastefirespeed;
vmCvar_t    g_medkitmodifier;
vmCvar_t    g_armorprotect;
vmCvar_t    g_respawnwait;
vmCvar_t    g_ammolimit;
vmCvar_t    g_jumpheight;
vmCvar_t    g_speedfactor;
vmCvar_t    g_drowndamage;
vmCvar_t    g_gametype;
vmCvar_t    g_fraglimit;
vmCvar_t    g_timelimit;
vmCvar_t    g_capturelimit;
vmCvar_t    g_friendlyFire;
vmCvar_t    g_password;
vmCvar_t    g_maxClients;
vmCvar_t    g_dedicated;
vmCvar_t    g_speed;
vmCvar_t    g_gravity;
vmCvar_t    g_gravityModifier;
vmCvar_t    g_damageModifier;
vmCvar_t    g_knockback;
vmCvar_t    g_quadfactor;
vmCvar_t    g_respawntime;
vmCvar_t    g_warmup;
vmCvar_t    g_doWarmup;
vmCvar_t    g_restarted;
vmCvar_t    g_blood;
vmCvar_t    g_teamAutoJoin;
vmCvar_t    g_teamForceBalance;
vmCvar_t    g_obeliskHealth;
vmCvar_t    g_obeliskRegenPeriod;
vmCvar_t    g_obeliskRegenAmount;
vmCvar_t    g_obeliskRespawnDelay;
vmCvar_t    g_cubeTimeout;
vmCvar_t    g_enableDust;
vmCvar_t    g_enableBreath;
vmCvar_t    g_proxMineTimeout;
vmCvar_t    g_spawnprotect;
vmCvar_t    g_spawn_health;
vmCvar_t    g_spawn_armor;
vmCvar_t    g_spawn_gauntlet;
vmCvar_t    g_spawn_machinegun;
vmCvar_t    g_spawn_shotgun;
vmCvar_t    g_spawn_grenade;
vmCvar_t    g_spawn_rocket;
vmCvar_t    g_spawn_lightning;
vmCvar_t    g_spawn_railgun;
vmCvar_t    g_spawn_plasmagun;
vmCvar_t    g_spawn_bfg;
vmCvar_t    g_spawn_grapple;
vmCvar_t    g_spawn_nail;
vmCvar_t    g_spawn_mine;
vmCvar_t    g_spawn_chain;
vmCvar_t    g_spawn_flame;
vmCvar_t    g_spawn_antimatter;
vmCvar_t    g_spawn_quad;
vmCvar_t    g_spawn_haste;
vmCvar_t    g_spawn_bsuit;
vmCvar_t    g_spawn_invis;
vmCvar_t    g_spawn_regen;
vmCvar_t    g_spawn_flight;
vmCvar_t    g_spawn_holdable;
vmCvar_t    g_vampire;
vmCvar_t    g_vampireMaxHealth;
vmCvar_t    g_regen;
vmCvar_t    sv_fps;

static cvarTable_t cvarTable[] = {
	{ &g_cheats,                    "sv_cheats",                    "",             0 },
	{ NULL,                         "gamename",                     GAME_VERSION,   CVAR_ROM },
	{ NULL,                         "gamedate",                     __DATE__,       CVAR_ROM },
	{ &g_restarted,                 "g_restarted",                  "0",            CVAR_ROM },
	{ &g_gametype,                  "g_gametype",                   "0",            CVAR_SERVERINFO|CVAR_USERINFO|CVAR_LATCH },
	{ &g_maxClients,                "g_maxClients",                 "128",          CVAR_SERVERINFO|CVAR_ARCHIVE },
	{ &g_physimpact,                "g_physimpact",                 "0.80",         0 },
	{ &g_physimpulse,               "g_physimpulse",                "450",          0 },
	{ &g_physdamage,                "g_physdamage",                 "0.60",         0 },
	{ &g_ghspeed,                   "g_ghspeed",                    "800",          0 },
	{ &g_ghtimeout,                 "g_ghtimeout",                  "10000",        0 },
	{ &g_gdelay,                    "g_gdelay",                     "400",          0 },
	{ &g_gdamage,                   "g_gdamage",                    "50",           0 },
	{ &g_grange,                    "g_grange",                     "32",           0 },
	{ &g_gknockback,                "g_gknockback",                 "1",            0 },
	{ &g_mgdelay,                   "g_mgdelay",                    "100",          0 },
	{ &g_mgdamage,                  "g_mgdamage",                   "7",            0 },
	{ &g_mgspread,                  "g_mgspread",                   "200",          0 },
	{ &g_mgexplode,                 "g_mgexplode",                  "0",            0 },
	{ &g_mgsradius,                 "g_mgsradius",                  "70",           0 },
	{ &g_mgsdamage,                 "g_mgsdamage",                  "50",           0 },
	{ &g_mginf,                     "g_mginf",                      "0",            0 },
	{ &g_mgknockback,               "g_mgknockback",                "1",            0 },
	{ &g_sgdelay,                   "g_sgdelay",                    "1000",         0 },
	{ &g_sgdamage,                  "g_sgdamage",                   "10",           0 },
	{ &g_sgspread,                  "g_sgspread",                   "700",          0 },
	{ &g_sgexplode,                 "g_sgexplode",                  "0",            0 },
	{ &g_sgsradius,                 "g_sgsradius",                  "35",           0 },
	{ &g_sgsdamage,                 "g_sgsdamage",                  "45",           0 },
	{ &g_sgcount,                   "g_sgcount",                    "11",           0 },
	{ &g_sginf,                     "g_sginf",                      "0",            0 },
	{ &g_sgknockback,               "g_sgknockback",                "1",            0 },
	{ &g_gldelay,                   "g_gldelay",                    "500",          0 },
	{ &g_glspeed,                   "g_glspeed",                    "700",          0 },
	{ &g_gltimeout,                 "g_gltimeout",                  "2500",         0 },
	{ &g_glsradius,                 "g_glsradius",                  "120",          0 },
	{ &g_glsdamage,                 "g_glsdamage",                  "100",          0 },
	{ &g_gldamage,                  "g_gldamage",                   "100",          0 },
	{ &g_glbounce,                  "g_glbounce",                   "1",            0 },
	{ &g_glgravity,                 "g_glgravity",                  "1",            0 },
	{ &g_glinf,                     "g_glinf",                      "0",            0 },
	{ &g_glbouncemodifier,          "g_glbouncemodifier",           "0.65",         0 },
	{ &g_glknockback,               "g_glknockback",                "1",            0 },
	{ &g_rldelay,                   "g_rldelay",                    "800",          0 },
	{ &g_rlspeed,                   "g_rlspeed",                    "900",          0 },
	{ &g_rltimeout,                 "g_rltimeout",                  "15000",        0 },
	{ &g_rlsradius,                 "g_rlsradius",                  "120",          0 },
	{ &g_rlsdamage,                 "g_rlsdamage",                  "100",          0 },
	{ &g_rldamage,                  "g_rldamage",                   "100",          0 },
	{ &g_rlbounce,                  "g_rlbounce",                   "0",            0 },
	{ &g_rlgravity,                 "g_rlgravity",                  "0",            0 },
	{ &g_rlinf,                     "g_rlinf",                      "0",            0 },
	{ &g_rlbouncemodifier,          "g_rlbouncemodifier",           "0.65",         0 },
	{ &g_rlknockback,               "g_rlknockback",                "1",            0 },
	{ &g_lgdelay,                   "g_lgdelay",                    "50",           0 },
	{ &g_lgdamage,                  "g_lgdamage",                   "8",            0 },
	{ &g_lgrange,                   "g_lgrange",                    "768",          0 },
	{ &g_lgexplode,                 "g_lgexplode",                  "0",            0 },
	{ &g_lgsradius,                 "g_lgsradius",                  "75",           0 },
	{ &g_lgsdamage,                 "g_lgsdamage",                  "70",           0 },
	{ &g_lginf,                     "g_lginf",                      "0",            0 },
	{ &g_lgknockback,               "g_lgknockback",                "1",            0 },
	{ &g_rgdelay,                   "g_rgdelay",                    "1500",         0 },
	{ &g_rgdamage,                  "g_rgdamage",                   "100",          0 },
	{ &g_rginf,                     "g_rginf",                      "0",            0 },
	{ &g_rgknockback,               "g_rgknockback",                "1",            0 },
	{ &g_pgsradius,                 "g_pgsradius",                  "20",           0 },
	{ &g_pgspeed,                   "g_pgspeed",                    "2000",         0 },
	{ &g_pgsdamage,                 "g_pgsdamage",                  "15",           0 },
	{ &g_pgdamage,                  "g_pgdamage",                   "20",           0 },
	{ &g_pgtimeout,                 "g_pgtimeout",                  "10000",        0 },
	{ &g_pgdelay,                   "g_pgdelay",                    "100",          0 },
	{ &g_pgbounce,                  "g_pgbounce",                   "0",            0 },
	{ &g_pggravity,                 "g_pggravity",                  "0",            0 },
	{ &g_pginf,                     "g_pginf",                      "0",            0 },
	{ &g_pgbouncemodifier,          "g_pgbouncemodifier",           "0.65",         0 },
	{ &g_pgknockback,               "g_pgknockback",                "1",            0 },
	{ &g_bfgsradius,                "g_bfgsradius",                 "120",          0 },
	{ &g_bfgspeed,                  "g_bfgspeed",                   "2000",         0 },
	{ &g_bfgsdamage,                "g_bfgsdamage",                 "100",          0 },
	{ &g_bfgdamage,                 "g_bfgdamage",                  "100",          0 },
	{ &g_bfgtimeout,                "g_bfgtimeout",                 "10000",        0 },
	{ &g_bfgdelay,                  "g_bfgdelay",                   "200",          0 },
	{ &g_bfgbounce,                 "g_bfgbounce",                  "0",            0 },
	{ &g_bfggravity,                "g_bfggravity",                 "0",            0 },
	{ &g_bfginf,                    "g_bfginf",                     "0",            0 },
	{ &g_bfgbouncemodifier,         "g_bfgbouncemodifier",          "0.65",         0 },
	{ &g_bfgknockback,              "g_bfgknockback",               "1",            0 },
	{ &g_ngdelay,                   "g_ngdelay",                    "1000",         0 },
	{ &g_ngspeed,                   "g_ngspeed",                    "555",          0 },
	{ &g_ngspread,                  "g_ngspread",                   "500",          0 },
	{ &g_ngdamage,                  "g_ngdamage",                   "20",           0 },
	{ &g_ngtimeout,                 "g_ngtimeout",                  "10000",        0 },
	{ &g_ngcount,                   "g_ngcount",                    "15",           0 },
	{ &g_ngbounce,                  "g_ngbounce",                   "0",            0 },
	{ &g_nggravity,                 "g_nggravity",                  "0",            0 },
	{ &g_ngrandom,                  "g_ngrandom",                   "1800",         0 },
	{ &g_nginf,                     "g_nginf",                      "0",            0 },
	{ &g_ngbouncemodifier,          "g_ngbouncemodifier",           "0.65",         0 },
	{ &g_ngknockback,               "g_ngknockback",                "1",            0 },
	{ &g_pldelay,                   "g_pldelay",                    "800",          0 },
	{ &g_plspeed,                   "g_plspeed",                    "700",          0 },
	{ &g_pltimeout,                 "g_pltimeout",                  "3000",         0 },
	{ &g_plsradius,                 "g_plsradius",                  "150",          0 },
	{ &g_plsdamage,                 "g_plsdamage",                  "100",          0 },
	{ &g_plgravity,                 "g_plgravity",                  "1",            0 },
	{ &g_pldamage,                  "g_pldamage",                   "0",            0 },
	{ &g_plinf,                     "g_plinf",                      "0",            0 },
	{ &g_plknockback,               "g_plknockback",                "1",            0 },
	{ &g_cgdelay,                   "g_cgdelay",                    "30",           0 },
	{ &g_cgdamage,                  "g_cgdamage",                   "7",            0 },
	{ &g_cgspread,                  "g_cgspread",                   "600",          0 },
	{ &g_cginf,                     "g_cginf",                      "0",            0 },
	{ &g_cgknockback,               "g_cgknockback",                "1",            0 },
	{ &g_ftsradius,                 "g_ftsradius",                  "20",           0 },
	{ &g_ftspeed,                   "g_ftspeed",                    "600",          0 },
	{ &g_ftsdamage,                 "g_ftsdamage",                  "15",           0 },
	{ &g_ftdamage,                  "g_ftdamage",                   "20",           0 },
	{ &g_fttimeout,                 "g_fttimeout",                  "1500",         0 },
	{ &g_ftdelay,                   "g_ftdelay",                    "40",           0 },
	{ &g_ftbounce,                  "g_ftbounce",                   "1",            0 },
	{ &g_ftgravity,                 "g_ftgravity",                  "0",            0 },
	{ &g_ftinf,                     "g_ftinf",                      "0",            0 },
	{ &g_ftbouncemodifier,          "g_ftbouncemodifier",           "0.65",         0 },
	{ &g_ftknockback,               "g_ftknockback",                "1",            0 },
	{ &g_amsradius,                 "g_amsradius",                  "0",            0 },
	{ &g_amspeed,                   "g_amspeed",                    "600",          0 },
	{ &g_amsdamage,                 "g_amsdamage",                  "0",            0 },
	{ &g_amdamage,                  "g_amdamage",                   "6",            0 },
	{ &g_amtimeout,                 "g_amtimeout",                  "1000",         0 },
	{ &g_amdelay,                   "g_amdelay",                    "40",           0 },
	{ &g_ambounce,                  "g_ambounce",                   "1",            0 },
	{ &g_amgravity,                 "g_amgravity",                  "1",            0 },
	{ &g_aminf,                     "g_aminf",                      "0",            0 },
	{ &g_ambouncemodifier,          "g_ambouncemodifier",           "0.65",         0 },
	{ &g_amknockback,               "g_amknockback",                "1",            0 },
	{ &g_glhoming,                  "g_glhoming",                   "0",            0 },
	{ &g_glguided,                  "g_glguided",                   "0",            0 },
	{ &g_rlhoming,                  "g_rlhoming",                   "0",            0 },
	{ &g_rlguided,                  "g_rlguided",                   "0",            0 },
	{ &g_pghoming,                  "g_pghoming",                   "0",            0 },
	{ &g_pgguided,                  "g_pgguided",                   "0",            0 },
	{ &g_bfghoming,                 "g_bfghoming",                  "0",            0 },
	{ &g_bfgguided,                 "g_bfgguided",                  "0",            0 },
	{ &g_nghoming,                  "g_nghoming",                   "0",            0 },
	{ &g_ngguided,                  "g_ngguided",                   "0",            0 },
	{ &g_fthoming,                  "g_fthoming",                   "0",            0 },
	{ &g_ftguided,                  "g_ftguided",                   "0",            0 },
	{ &g_amhoming,                  "g_amhoming",                   "0",            0 },
	{ &g_amguided,                  "g_amguided",                   "0",            0 },
	{ &g_scoutspeedfactor,          "g_scoutspeedfactor",           "1.5",          0 },
	{ &g_scoutfirespeed,            "g_scoutfirespeed",             "0.60",         0 },
	{ &g_scoutdamagefactor,         "g_scoutdamagefactor",          "1",            0 },
	{ &g_scoutgravitymodifier,      "g_scoutgravitymodifier",       "1",            0 },
	{ &g_scout_infammo,             "g_scout_infammo",              "0",            0 },
	{ &g_scouthealthmodifier,       "g_scouthealthmodifier",        "1",            0 },
	{ &g_doublerfirespeed,          "g_doublerfirespeed",           "1",            0 },
	{ &g_doublerdamagefactor,       "g_doublerdamagefactor",        "2.5",          0 },
	{ &g_doublerspeedfactor,        "g_doublerspeedfactor",         "1",            0 },
	{ &g_doublergravitymodifier,    "g_doublergravitymodifier",     "1",            0 },
	{ &g_doubler_infammo,           "g_doubler_infammo",            "0",            0 },
	{ &g_doublerhealthmodifier,     "g_doublerhealthmodifier",      "1",            0 },
	{ &g_guardhealthmodifier,       "g_guardhealthmodifier",        "1.5",          0 },
	{ &g_guardfirespeed,            "g_guardfirespeed",             "1",            0 },
	{ &g_guarddamagefactor,         "g_guarddamagefactor",          "1",            0 },
	{ &g_guardspeedfactor,          "g_guardspeedfactor",           "1",            0 },
	{ &g_guardgravitymodifier,      "g_guardgravitymodifier",       "1",            0 },
	{ &g_guard_infammo,             "g_guard_infammo",              "0",            0 },
	{ &g_ammoregenfirespeed,        "g_ammoregenfirespeed",         "0.65",         0 },
	{ &g_ammoregen_infammo,         "g_ammoregen_infammo",          "0",            0 },
	{ &g_ammoregendamagefactor,     "g_ammoregendamagefactor",      "1",            0 },
	{ &g_ammoregenspeedfactor,      "g_ammoregenspeedfactor",       "1",            0 },
	{ &g_ammoregengravitymodifier,  "g_ammoregengravitymodifier",   "1",            0 },
	{ &g_ammoregenhealthmodifier,   "g_ammoregenhealthmodifier",    "1",            0 },
	{ &g_teamred_speed,             "g_teamred_speed",              "320",          0 },
	{ &g_teamred_gravityModifier,   "g_teamred_gravityModifier",    "1",            0 },
	{ &g_teamred_firespeed,         "g_teamred_firespeed",          "1",            0 },
	{ &g_teamred_damage,            "g_teamred_damage",             "1",            0 },
	{ &g_teamred_infammo,           "g_teamred_infammo",            "0",            0 },
	{ &g_teamred_respawnwait,       "g_teamred_respawnwait",        "3000",         0 },
	{ &g_teamred_pickupitems,       "g_teamred_pickupitems",        "1",            0 },
	{ &g_teamblue_speed,            "g_teamblue_speed",             "320",          0 },
	{ &g_teamblue_gravityModifier,  "g_teamblue_gravityModifier",   "1",            0 },
	{ &g_teamblue_firespeed,        "g_teamblue_firespeed",         "1",            0 },
	{ &g_teamblue_damage,           "g_teamblue_damage",            "1",            0 },
	{ &g_teamblue_infammo,          "g_teamblue_infammo",           "0",            0 },
	{ &g_teamblue_respawnwait,      "g_teamblue_respawnwait",       "3000",         0 },
	{ &g_teamblue_pickupitems,      "g_teamblue_pickupitems",       "1",            0 },
	{ &g_entitypack,                "g_entitypack",                 "default",      0 },
	{ &g_extendedsandbox,           "g_extendedsandbox",            "0",            0 },
	{ &g_skyColorR,                 "g_skyColorR",                  "255",          0 },
	{ &g_skyColorG,                 "g_skyColorG",                  "255",          0 },
	{ &g_skyColorB,                 "g_skyColorB",                  "255",          0 },
	{ &g_skyColorA,                 "g_skyColorA",                  "0",            0 },
	{ &g_allowprops,                "g_allowprops",                 "1",            0 },
	{ &g_allownpc,                  "g_allownpc",                   "1",            0 },
	{ &g_allowitems,                "g_allowitems",                 "1",            0 },
	{ &g_allownoclip,               "g_allownoclip",                "1",            0 },
	{ &g_allowtoolgun,              "g_allowtoolgun",               "1",            0 },
	{ &g_allowphysgun,              "g_allowphysgun",               "1",            0 },
	{ &g_allowgravitygun,           "g_allowgravitygun",            "1",            0 },
	{ &cl_selectedmod,              "cl_selectedmod",               "default",      CVAR_ARCHIVE },
	{ &cl_language,                 "cl_language",                  "0",            CVAR_ARCHIVE },
	{ &g_maxEntities,               "g_maxEntities",                "1024",         0 },
	{ &g_regenarmor,                "g_regenarmor",                 "0",            0 },
	{ &g_movetype,                  "g_movetype",                   "0",            0 },
	{ &g_accelerate,                "g_accelerate",                 "1",            0 },
	{ &g_randomItems,               "g_randomItems",                "0",            0 },
	{ &g_invulinf,                  "g_invulinf",                   "0",            0 },
	{ &g_kamikazeinf,               "g_kamikazeinf",                "0",            0 },
	{ &g_portalinf,                 "g_portalinf",                  "0",            0 },
	{ &g_teleporterinf,             "g_teleporterinf",              "0",            0 },
	{ &g_medkitinf,                 "g_medkitinf",                  "0",            0 },
	{ &g_medkitlimit,               "g_medkitlimit",                "200",          0 },
	{ &g_waterdamage,               "g_waterdamage",                "0",            0 },
	{ &g_lavadamage,                "g_lavadamage",                 "30",           0 },
	{ &g_slimedamage,               "g_slimedamage",                "10",           0 },
	{ &g_nextbot_speed,             "g_nextbot_speed",              "2.00",         0 },
	{ &g_randomteleport,            "g_randomteleport",             "0",            0 },
	{ &g_falldamagesmall,           "g_falldamagesmall",            "5",            0 },
	{ &g_falldamagebig,             "g_falldamagebig",              "10",           0 },
	{ &g_noplayerclip,              "g_noplayerclip",               "0",            0 },
	{ &g_flagrespawn,               "g_flagrespawn",                "-1",           0 },
	{ &g_portaltimeout,             "g_portaltimeout",              "120",          0 },
	{ &g_portalhealth,              "g_portalhealth",               "200",          0 },
	{ &g_quadtime,                  "g_quadtime",                   "30",           0 },
	{ &g_bsuittime,                 "g_bsuittime",                  "30",           0 },
	{ &g_hastetime,                 "g_hastetime",                  "30",           0 },
	{ &g_invistime,                 "g_invistime",                  "30",           0 },
	{ &g_regentime,                 "g_regentime",                  "30",           0 },
	{ &g_flighttime,                "g_flighttime",                 "60",           0 },
	{ &g_invulmove,                 "g_invulmove",                  "0",            0 },
	{ &g_invultime,                 "g_invultime",                  "10",           0 },
	{ &g_fasthealthregen,           "g_fasthealthregen",            "15",           0 },
	{ &g_slowhealthregen,           "g_slowhealthregen",            "5",            0 },
	{ &g_hastefirespeed,            "g_hastefirespeed",             "0.65",         0 },
	{ &g_medkitmodifier,            "g_medkitmodifier",             "100",          0 },
	{ &g_armorprotect,              "g_armorprotect",               "0.66",         0 },
	{ &g_respawnwait,               "g_respawnwait",                "3000",         0 },
	{ &g_jumpheight,                "g_jumpheight",                 "270",          0 },
	{ &g_speedfactor,               "g_speedfactor",                "1.3",          0 },
	{ &g_drowndamage,               "g_drowndamage",                "1",            0 },
	{ &g_ammolimit,                 "g_ammolimit",                  "200",          0 },
	{ &g_fraglimit,                 "fraglimit",                    "20",           CVAR_SERVERINFO|CVAR_ARCHIVE|CVAR_NORESTART },
	{ &g_timelimit,                 "timelimit",                    "0",            CVAR_SERVERINFO|CVAR_ARCHIVE|CVAR_NORESTART },
	{ &g_capturelimit,              "capturelimit",                 "8",            CVAR_SERVERINFO|CVAR_ARCHIVE|CVAR_NORESTART },
	{ &g_friendlyFire,              "g_friendlyFire",               "0",            CVAR_ARCHIVE },
	{ &g_teamAutoJoin,              "g_teamAutoJoin",               "0",            CVAR_ARCHIVE },
	{ &g_teamForceBalance,          "g_teamForceBalance",           "0",            CVAR_ARCHIVE },
	{ &g_warmup,                    "g_warmup",                     "20",           CVAR_ARCHIVE },
	{ &g_doWarmup,                  "g_doWarmup",                   "0",            CVAR_SERVERINFO|CVAR_ARCHIVE },
	{ &g_password,                  "g_password",                   "",             CVAR_USERINFO },
	{ &g_dedicated,                 "dedicated",                    "0",            0 },
	{ &g_speed,                     "g_speed",                      "320",          0 },
	{ &g_gravity,                   "g_gravity",                    "800",          0 },
	{ &g_gravityModifier,           "g_gravityModifier",            "1",            0 },
    { &g_damageModifier,            "g_damageModifier",             "1",            0 },
	{ &g_knockback,                 "g_knockback",                  "1000",         0 },
	{ &g_quadfactor,                "g_quadfactor",                 "3",            0 },
    { &g_respawntime,               "g_respawntime",                "0",            CVAR_ARCHIVE },
	{ &g_blood,                     "com_blood",                    "1",            0 },
	{ &g_obeliskHealth,             "g_obeliskHealth",              "2500",         0 },
	{ &g_obeliskRegenPeriod,        "g_obeliskRegenPeriod",         "1",            0 },
	{ &g_obeliskRegenAmount,        "g_obeliskRegenAmount",         "15",           0 },
	{ &g_obeliskRespawnDelay,       "g_obeliskRespawnDelay",        "10",           CVAR_SERVERINFO },
	{ &g_cubeTimeout,               "g_cubeTimeout",                "30",           0 },
	{ &g_enableDust,                "g_enableDust",                 "0",            CVAR_SERVERINFO },
	{ &g_enableBreath,              "g_enableBreath",               "0",            CVAR_SERVERINFO },
	{ &g_proxMineTimeout,           "g_proxMineTimeout",            "180000",       0 },
	{ &sv_fps,                      "sv_fps",                       "60",           CVAR_SYSTEMINFO|CVAR_ARCHIVE },
    { &g_spawnprotect,              "g_spawnprotect",               "500",          CVAR_ARCHIVE|CVAR_NORESTART },
	{ &g_spawn_health,           	"g_spawn_health",            "100",          CVAR_NORESTART },
	{ &g_spawn_armor,            	"g_spawn_armor",             "0",            CVAR_NORESTART },
	{ &g_spawn_gauntlet,         	"g_spawn_gauntlet",          "1",            CVAR_NORESTART },
	{ &g_spawn_machinegun,       	"g_spawn_machinegun",        "500",          CVAR_NORESTART },
	{ &g_spawn_shotgun,          	"g_spawn_shotgun",           "0",            CVAR_NORESTART },
	{ &g_spawn_grenade,          	"g_spawn_grenade",           "0",            CVAR_NORESTART },
	{ &g_spawn_rocket,           	"g_spawn_rocket",            "0",            CVAR_NORESTART },
	{ &g_spawn_lightning,        	"g_spawn_lightning",         "0",            CVAR_NORESTART },
	{ &g_spawn_railgun,          	"g_spawn_railgun",           "0",            CVAR_NORESTART },
	{ &g_spawn_plasmagun,        	"g_spawn_plasmagun",         "0",            CVAR_NORESTART },
	{ &g_spawn_bfg,              	"g_spawn_bfg",               "0",            CVAR_NORESTART },
    { &g_spawn_grapple,          	"g_spawn_grapple",           "0",            CVAR_NORESTART },
	{ &g_spawn_nail,             	"g_spawn_nail",              "0",            CVAR_NORESTART },
	{ &g_spawn_mine,             	"g_spawn_mine",              "0",            CVAR_NORESTART },
	{ &g_spawn_chain,            	"g_spawn_chain",             "0",            CVAR_NORESTART },
	{ &g_spawn_flame,            	"g_spawn_flame",             "0",            CVAR_NORESTART },
	{ &g_spawn_antimatter,       	"g_spawn_antimatter",        "0",            CVAR_NORESTART },
	{ &g_spawn_quad,             	"g_spawn_quad",              "0",            CVAR_NORESTART },
	{ &g_spawn_haste,            	"g_spawn_haste",             "0",            CVAR_NORESTART },
	{ &g_spawn_bsuit,            	"g_spawn_bsuit",             "0",            CVAR_NORESTART },
	{ &g_spawn_invis,            	"g_spawn_invis",             "0",            CVAR_NORESTART },
	{ &g_spawn_regen,            	"g_spawn_regen",             "0",            CVAR_NORESTART },
	{ &g_spawn_flight,           	"g_spawn_flight",            "0",            CVAR_NORESTART },
	{ &g_spawn_holdable,         	"g_spawn_holdable",          "0",            CVAR_NORESTART },
	{ &g_vampire,                   "g_vampire",                    "0.0",          CVAR_NORESTART },
	{ &g_regen,                     "g_regen",                      "0",            CVAR_NORESTART },
	{ &g_vampireMaxHealth,          "g_vampire_max_health",         "500",          CVAR_NORESTART }
};
#endif

#ifdef CGAME
vmCvar_t    g_gametype;
vmCvar_t    headR;
vmCvar_t    headG;
vmCvar_t    headB;
vmCvar_t    modelR;
vmCvar_t    modelG;
vmCvar_t    modelB;
vmCvar_t    legsR;
vmCvar_t    legsG;
vmCvar_t    legsB;
vmCvar_t    physR;
vmCvar_t    physG;
vmCvar_t    physB;
vmCvar_t    cg_effectsTime;
vmCvar_t    cg_effectsLimit;
vmCvar_t    cg_effectsGibs;
vmCvar_t    toolgun_mod1;
vmCvar_t    toolgun_mod2;
vmCvar_t    toolgun_mod3;
vmCvar_t    toolgun_mod4;
vmCvar_t    toolgun_mod5;
vmCvar_t    toolgun_mod6;
vmCvar_t    toolgun_mod7;
vmCvar_t    toolgun_mod8;
vmCvar_t    toolgun_mod9;
vmCvar_t    toolgun_mod10;
vmCvar_t    toolgun_mod11;
vmCvar_t    toolgun_mod12;
vmCvar_t    toolgun_mod13;
vmCvar_t    toolgun_mod14;
vmCvar_t    toolgun_mod15;
vmCvar_t    toolgun_mod16;
vmCvar_t    toolgun_mod17;
vmCvar_t    toolgun_mod18;
vmCvar_t    toolgun_mod19;
vmCvar_t    toolgun_tool;
vmCvar_t    toolgun_toolcmd1;
vmCvar_t    toolgun_toolcmd2;
vmCvar_t    toolgun_toolcmd3;
vmCvar_t    toolgun_toolcmd4;
vmCvar_t    toolgun_tooltext;
vmCvar_t    toolgun_tooltip1;
vmCvar_t    toolgun_tooltip2;
vmCvar_t    toolgun_tooltip3;
vmCvar_t    toolgun_tooltip4;
vmCvar_t    toolgun_toolmode1;
vmCvar_t    toolgun_toolmode2;
vmCvar_t    toolgun_toolmode3;
vmCvar_t    toolgun_toolmode4;
vmCvar_t    cg_hide255;
vmCvar_t    ns_haveerror;
vmCvar_t    cg_postprocess;
vmCvar_t    cl_language;
vmCvar_t    cg_disableBobbing;
vmCvar_t    cg_shadows;
vmCvar_t    cg_drawTimer;
vmCvar_t    cg_drawFPS;
vmCvar_t    cg_drawCrosshair;
vmCvar_t    cg_drawCrosshairNames;
vmCvar_t    cg_crosshairScale;
vmCvar_t    cg_draw2D;
vmCvar_t    cg_debugEvents;
vmCvar_t    cg_addMarks;
vmCvar_t    cg_drawGun;
vmCvar_t    cg_fov;
vmCvar_t    cg_zoomFov;
vmCvar_t    cg_thirdPerson;
vmCvar_t    cg_thirdPersonRange;
vmCvar_t    cg_thirdPersonOffset;
vmCvar_t    cg_drawSpeed;
vmCvar_t    cg_paused;
vmCvar_t    cg_blood;
vmCvar_t    cg_drawFriend;
vmCvar_t    cg_cameraEyes;
vmCvar_t    cg_cameraEyes_Fwd;
vmCvar_t    cg_cameraEyes_Up;
vmCvar_t    cg_noProjectileTrail;
vmCvar_t    cg_obeliskRespawnDelay;
vmCvar_t    cg_enableDust;
vmCvar_t    cg_enableBreath;
vmCvar_t    sv_fps;
vmCvar_t    cg_atmosphericLevel;
vmCvar_t    cg_crosshairColorRed;
vmCvar_t    cg_crosshairColorGreen;
vmCvar_t    cg_crosshairColorBlue;
vmCvar_t    cg_chatBeep;
vmCvar_t    cg_teamChatBeep;

static cvarTable_t cvarTable[] = {
	{ &g_gametype,                      "g_gametype",                    "0",           0 },
    { &headR,                           "headR",                        "100",          CVAR_USERINFO|CVAR_ARCHIVE },
    { &headG,                           "headG",                        "100",          CVAR_USERINFO|CVAR_ARCHIVE },
    { &headB,                           "headB",                        "100",          CVAR_USERINFO|CVAR_ARCHIVE },
    { &modelR,                          "modelR",                       "100",          CVAR_USERINFO|CVAR_ARCHIVE },
    { &modelG,                          "modelG",                       "100",          CVAR_USERINFO|CVAR_ARCHIVE },
    { &modelB,                          "modelB",                       "100",          CVAR_USERINFO|CVAR_ARCHIVE },
    { &legsR,                           "legsR",                        "100",          CVAR_USERINFO|CVAR_ARCHIVE },
    { &legsG,                           "legsG",                        "100",          CVAR_USERINFO|CVAR_ARCHIVE },
    { &legsB,                           "legsB",                        "100",          CVAR_USERINFO|CVAR_ARCHIVE },
    { &physR,                           "physR",                        "128",          CVAR_USERINFO|CVAR_ARCHIVE },
    { &physG,                           "physG",                        "225",          CVAR_USERINFO|CVAR_ARCHIVE },
    { &physB,                           "physB",                        "255",          CVAR_USERINFO|CVAR_ARCHIVE },
	{ &cg_effectsTime,                  "cg_effectsTime",               "10",           CVAR_ARCHIVE },
	{ &cg_effectsLimit,                 "cg_effectsLimit",              "4096",         CVAR_ARCHIVE },
	{ &cg_effectsGibs,                  "cg_effectsGibs",               "1",            CVAR_ARCHIVE },
	{ &toolgun_mod1,                    "toolgun_mod1",                 "0",            0 },
	{ &toolgun_mod2,                    "toolgun_mod2",                 "0",            0 },
	{ &toolgun_mod3,                    "toolgun_mod3",                 "0",            0 },
	{ &toolgun_mod4,                    "toolgun_mod4",                 "0",            0 },
	{ &toolgun_mod5,                    "toolgun_mod5",                 "0",            0 },
	{ &toolgun_mod6,                    "toolgun_mod6",                 "0",            0 },
	{ &toolgun_mod7,                    "toolgun_mod7",                 "0",            0 },
	{ &toolgun_mod8,                    "toolgun_mod8",                 "0",            0 },
	{ &toolgun_mod9,                    "toolgun_mod9",                 "0",            0 },
	{ &toolgun_mod10,                   "toolgun_mod10",                "0",            0 },
	{ &toolgun_mod11,                   "toolgun_mod11",                "0",            0 },
	{ &toolgun_mod12,                   "toolgun_mod12",                "0",            0 },
	{ &toolgun_mod13,                   "toolgun_mod13",                "0",            0 },
	{ &toolgun_mod14,                   "toolgun_mod14",                "0",            0 },
	{ &toolgun_mod15,                   "toolgun_mod15",                "0",            0 },
	{ &toolgun_mod16,                   "toolgun_mod16",                "0",            0 },
	{ &toolgun_mod17,                   "toolgun_mod17",                "0",            0 },
	{ &toolgun_mod18,                   "toolgun_mod18",                "0",            0 },
	{ &toolgun_mod19,                   "toolgun_mod19",                "0",            0 },
	{ &toolgun_tool,                    "toolgun_tool",                 "0",            CVAR_USERINFO },
	{ &toolgun_toolcmd1,                "toolgun_toolcmd1",             "",             0 },
	{ &toolgun_toolcmd2,                "toolgun_toolcmd2",             "",             0 },
	{ &toolgun_toolcmd3,                "toolgun_toolcmd3",             "",             0 },
	{ &toolgun_toolcmd4,                "toolgun_toolcmd4",             "",             0 },
	{ &toolgun_tooltext,                "toolgun_tooltext",             "",             0 },
	{ &toolgun_tooltip1,                "toolgun_tooltip1",             "",             0 },
	{ &toolgun_tooltip2,                "toolgun_tooltip2",             "",             0 },
	{ &toolgun_tooltip3,                "toolgun_tooltip3",             "",             0 },
	{ &toolgun_tooltip4,                "toolgun_tooltip4",             "",             0 },
	{ &toolgun_toolmode1,               "toolgun_toolmode1",            "",             0 },
	{ &toolgun_toolmode2,               "toolgun_toolmode2",            "",             0 },
	{ &toolgun_toolmode3,               "toolgun_toolmode3",            "",             0 },
	{ &toolgun_toolmode4,               "toolgun_toolmode4",            "",             0 },
	{ &cg_hide255,                      "cg_hide255",                   "0",            0 },
	{ &ns_haveerror,                    "ns_haveerror",                 "0",            0 },
	{ &cg_postprocess,                  "cg_postprocess",               "",             0 },
	{ &cl_language,                     "cl_language",                  "0",            CVAR_ARCHIVE },
	{ &cg_drawGun,                      "cg_drawGun",                   "1",            CVAR_ARCHIVE },
	{ &cg_zoomFov,                      "cg_zoomfov",                   "22",           CVAR_ARCHIVE },
	{ &cg_fov,                          "cg_fov",                       "110",          CVAR_ARCHIVE },
	{ &cg_shadows,                      "cg_shadows",                   "1",            CVAR_ARCHIVE },
	{ &cg_draw2D,                       "cg_draw2D",                    "1",            CVAR_ARCHIVE },
	{ &cg_drawTimer,                    "cg_drawTimer",                 "0",            CVAR_ARCHIVE },
	{ &cg_drawFPS,                      "cg_drawFPS",                   "0",            CVAR_ARCHIVE },
	{ &cg_drawSpeed,                    "cg_drawSpeed",                 "0",            CVAR_ARCHIVE },
	{ &cg_drawCrosshair,                "cg_drawCrosshair",             "4",            CVAR_ARCHIVE },
	{ &cg_drawCrosshairNames,           "cg_drawCrosshairNames",        "1",            CVAR_ARCHIVE },
	{ &cg_crosshairScale,               "cg_crosshairScale",            "24",           CVAR_ARCHIVE },
	{ &cg_addMarks,                     "cg_addMarks",                  "1",            CVAR_ARCHIVE },
	{ &cg_disableBobbing,               "cg_disableBobbing",            "0",            CVAR_ARCHIVE },
	{ &cg_debugEvents,                  "cg_debugevents",               "0",            CVAR_CHEAT },
	{ &cg_thirdPersonRange,             "cg_thirdPersonRange",          "65",           CVAR_ARCHIVE },
	{ &cg_thirdPersonOffset,            "cg_thirdPersonOffset",         "25",           CVAR_ARCHIVE },
	{ &cg_thirdPerson,                  "cg_thirdPerson",               "0",            CVAR_ARCHIVE},
	{ &cg_atmosphericLevel,             "cg_atmosphericLevel",          "1",            CVAR_ARCHIVE },
	{ &cg_drawFriend,                   "cg_drawFriend",                "1",            CVAR_ARCHIVE },
	{ &cg_paused,                       "cl_paused",                    "0",            CVAR_ROM },
	{ &cg_blood,                        "com_blood",                    "1",            CVAR_ARCHIVE },
	{ &cg_enableDust,                   "g_enableDust",                 "0",            CVAR_SERVERINFO },
	{ &cg_enableBreath,                 "g_enableBreath",               "0",            CVAR_SERVERINFO },
	{ &cg_obeliskRespawnDelay,          "g_obeliskRespawnDelay",        "10",           0 },
	{ &cg_noProjectileTrail,            "cg_noProjectileTrail",         "0",            CVAR_ARCHIVE },
	{ &cg_cameraEyes,                   "cg_cameraEyes",                "0",            CVAR_ARCHIVE },
	{ &cg_cameraEyes_Fwd,               "cg_cameraEyes_Fwd",            "0",            CVAR_ARCHIVE },
	{ &cg_cameraEyes_Up,                "cg_cameraEyes_Up",             "7",            CVAR_ARCHIVE },
	{ &sv_fps,                          "sv_fps",                       "60",           CVAR_SYSTEMINFO },
	{ &cg_crosshairColorRed,            "cg_crosshairColorRed",         "0.5",          CVAR_ARCHIVE|CVAR_USERINFO },
    { &cg_crosshairColorGreen,          "cg_crosshairColorGreen",       "0.75",         CVAR_ARCHIVE|CVAR_USERINFO },
    { &cg_crosshairColorBlue,           "cg_crosshairColorBlue",        "1.0",          CVAR_ARCHIVE|CVAR_USERINFO },
    { &cg_chatBeep,                     "cg_chatBeep",                  "1",            CVAR_ARCHIVE },
    { &cg_teamChatBeep,                 "cg_teamChatBeep",              "1",            CVAR_ARCHIVE }
};
#endif

#ifdef UI
vmCvar_t    sb_private;
vmCvar_t    sb_texture;
vmCvar_t    sb_grid;
vmCvar_t    sb_modelnum;
vmCvar_t    sb_classnum;
vmCvar_t    sb_texturenum;
vmCvar_t    sb_tab;
vmCvar_t    spawn_preset;
vmCvar_t    tool_spawnpreset;
vmCvar_t    tool_modifypreset;
vmCvar_t    tool_modifypreset2;
vmCvar_t    tool_modifypreset3;
vmCvar_t    tool_modifypreset4;
vmCvar_t    sb_ctab_1;
vmCvar_t    sb_ctab_2;
vmCvar_t    sb_ctab_3;
vmCvar_t    sb_ctab_4;
vmCvar_t    sb_ctab_5;
vmCvar_t    sb_ctab_6;
vmCvar_t    sb_ctab_7;
vmCvar_t    sb_ctab_8;
vmCvar_t    sb_ctab_9;
vmCvar_t    sb_ctab_10;
vmCvar_t    toolgun_toolset1;
vmCvar_t    toolgun_toolset2;
vmCvar_t    toolgun_toolset3;
vmCvar_t    toolgun_toolset4;
vmCvar_t    toolgun_toolset5;
vmCvar_t    toolgun_toolset6;
vmCvar_t    toolgun_toolset7;
vmCvar_t    toolgun_toolset8;
vmCvar_t    toolgun_toolset9;
vmCvar_t    toolgun_toolset10;
vmCvar_t    toolgun_toolset11;
vmCvar_t    toolgun_toolset12;
vmCvar_t    toolgun_toolset13;
vmCvar_t    toolgun_toolset14;
vmCvar_t    toolgun_toolset15;
vmCvar_t    toolgun_toolset16;
vmCvar_t    toolgun_toolset17;
vmCvar_t    toolgun_toolset18;
vmCvar_t    toolgun_disabledarg1;
vmCvar_t    toolgun_disabledarg2;
vmCvar_t    toolgun_disabledarg3;
vmCvar_t    toolgun_disabledarg4;
vmCvar_t    sbt_color0_0;
vmCvar_t    sbt_color0_1;
vmCvar_t    sbt_color0_2;
vmCvar_t    sbt_color0_3;
vmCvar_t    sbt_color1_0;
vmCvar_t    sbt_color1_1;
vmCvar_t    sbt_color1_2;
vmCvar_t    sbt_color1_3;
vmCvar_t    sbt_color2_0;
vmCvar_t    sbt_color2_1;
vmCvar_t    sbt_color2_2;
vmCvar_t    sbt_color2_3;
vmCvar_t    sbt_color3_0;
vmCvar_t    sbt_color3_1;
vmCvar_t    sbt_color3_2;
vmCvar_t    sbt_color3_3;
vmCvar_t    sbt_wallpaper;
vmCvar_t    ui_3dmap;
vmCvar_t    ui_effectslevel;
vmCvar_t    ui_singlemode;
vmCvar_t    cl_selectedmod;
vmCvar_t    cl_language;
vmCvar_t    ui_loaded;
vmCvar_t    sensitivitymenu;
vmCvar_t    ui_spScores1;
vmCvar_t    ui_spScores2;
vmCvar_t    ui_spScores3;
vmCvar_t    ui_spScores4;
vmCvar_t    ui_spScores5;
vmCvar_t    ui_spAwards;
vmCvar_t    ui_spVideos;
vmCvar_t    ui_spSkill;
vmCvar_t    ui_spSelection;
vmCvar_t    ui_browserMaster;
vmCvar_t    ui_browserGameType;
vmCvar_t    ui_browserSortKey;
vmCvar_t    ui_browserShowFull;
vmCvar_t    ui_browserShowEmpty;
vmCvar_t    ui_brassTime;
vmCvar_t    ui_drawCrosshair;
vmCvar_t    ui_drawCrosshairNames;
vmCvar_t    ui_marks;
vmCvar_t    ui_server1;
vmCvar_t    ui_server2;
vmCvar_t    ui_server3;
vmCvar_t    ui_server4;
vmCvar_t    ui_server5;
vmCvar_t    ui_server6;
vmCvar_t    ui_server7;
vmCvar_t    ui_server8;
vmCvar_t    ui_server9;
vmCvar_t    ui_server10;
vmCvar_t    ui_server11;
vmCvar_t    ui_server12;
vmCvar_t    ui_server13;
vmCvar_t    ui_server14;
vmCvar_t    ui_server15;
vmCvar_t    ui_server16;
vmCvar_t    ui_server17;
vmCvar_t    ui_server18;
vmCvar_t    ui_server19;
vmCvar_t    ui_server20;
vmCvar_t    ui_server21;
vmCvar_t    ui_server22;
vmCvar_t    ui_server23;
vmCvar_t    ui_server24;
vmCvar_t    ui_server25;
vmCvar_t    ui_server26;
vmCvar_t    ui_server27;
vmCvar_t    ui_server28;
vmCvar_t    ui_server29;
vmCvar_t    ui_server30;
vmCvar_t    ui_server31;
vmCvar_t    ui_server32;
vmCvar_t    gui_animsfx;
vmCvar_t    gui_mapicons;
vmCvar_t    gui_autoclosebotmenu;
vmCvar_t    gui_map_multisel;
vmCvar_t    gui_map_list;
vmCvar_t    gui_bot_multisel;
vmCvar_t    gui_bot_list;

static cvarTable_t cvarTable[] = {
	{ &sb_private,                      "sb_private",                   "0",            0 },
	{ &sb_grid,                         "sb_grid",                      "25",           0 },
	{ &sb_texture,                      "sb_texture",                   "0",            0 },
	{ &sb_modelnum,                     "sb_modelnum",                  "0",            CVAR_ARCHIVE },
	{ &sb_classnum,                     "sb_classnum",                  "0",            CVAR_ARCHIVE },
	{ &sb_texturenum,                   "sb_texturenum",                "0",            CVAR_ARCHIVE },
	{ &sb_tab,                          "sb_tab",                       "1",            0 },
	{ &spawn_preset,                    "spawn_preset",                 "set toolcmd_spawn sl prop %s %s %i 25 %s 0 %s 1 0 \"none\" -1 0 0 0 0 1 1 1 0 0 0.5",  0 },
	{ &tool_spawnpreset,                "tool_spawnpreset",             "set toolcmd_spawn sl prop %s %s %i 25 %s 0 %s 1 0 \"none\" -1 0 0 0 0 1 1 1 0 0 0.5",  0 },
	{ &tool_modifypreset,               "tool_modifypreset",            "set toolcmd_modify tm %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s 0",        0 },
	{ &tool_modifypreset2,              "tool_modifypreset2",           "set toolcmd_modify2 tm %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s 1",       0 },
	{ &tool_modifypreset3,              "tool_modifypreset3",           "set toolcmd_modify3 tm %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s 2",       0 },
	{ &tool_modifypreset4,              "tool_modifypreset4",           "set toolcmd_modify4 tm %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s 3",       0 },
	{ &sb_ctab_1,                       "sb_ctab_1",                    "",             CVAR_ARCHIVE },
	{ &sb_ctab_2,                       "sb_ctab_2",                    "",             CVAR_ARCHIVE },
	{ &sb_ctab_3,                       "sb_ctab_3",                    "",             CVAR_ARCHIVE },
	{ &sb_ctab_4,                       "sb_ctab_4",                    "",             CVAR_ARCHIVE },
	{ &sb_ctab_5,                       "sb_ctab_5",                    "",             CVAR_ARCHIVE },
	{ &sb_ctab_6,                       "sb_ctab_6",                    "",             CVAR_ARCHIVE },
	{ &sb_ctab_7,                       "sb_ctab_7",                    "",             CVAR_ARCHIVE },
	{ &sb_ctab_8,                       "sb_ctab_8",                    "",             CVAR_ARCHIVE },
	{ &sb_ctab_9,                       "sb_ctab_9",                    "",             CVAR_ARCHIVE },
	{ &sb_ctab_10,                      "sb_ctab_10",                   "",             CVAR_ARCHIVE },
	{ &toolgun_toolset1,                "toolgun_toolset1",             "--------:",    0 },
	{ &toolgun_toolset2,                "toolgun_toolset2",             "--------:",    0 },
	{ &toolgun_toolset3,                "toolgun_toolset3",             "--------:",    0 },
	{ &toolgun_toolset4,                "toolgun_toolset4",             "--------:",    0 },
	{ &toolgun_toolset5,                "toolgun_toolset5",             "--------:",    0 },
	{ &toolgun_toolset6,                "toolgun_toolset6",             "--------:",    0 },
	{ &toolgun_toolset7,                "toolgun_toolset7",             "--------:",    0 },
	{ &toolgun_toolset8,                "toolgun_toolset8",             "--------:",    0 },
	{ &toolgun_toolset9,                "toolgun_toolset9",             "--------:",    0 },
	{ &toolgun_toolset10,               "toolgun_toolset10",            "--------:",    0 },
	{ &toolgun_toolset11,               "toolgun_toolset11",            "--------:",    0 },
	{ &toolgun_toolset12,               "toolgun_toolset12",            "--------:",    0 },
	{ &toolgun_toolset13,               "toolgun_toolset13",            "--------:",    0 },
	{ &toolgun_toolset14,               "toolgun_toolset14",            "--------:",    0 },
	{ &toolgun_toolset15,               "toolgun_toolset15",            "--------:",    0 },
	{ &toolgun_toolset16,               "toolgun_toolset16",            "--------:",    0 },
	{ &toolgun_toolset17,               "toolgun_toolset17",            "--------:",    0 },
	{ &toolgun_toolset18,               "toolgun_toolset18",            "--------:",    0 },
	{ &toolgun_disabledarg1,            "toolgun_disabledarg1",         "0",            0 },
	{ &toolgun_disabledarg2,            "toolgun_disabledarg2",         "0",            0 },
	{ &toolgun_disabledarg3,            "toolgun_disabledarg3",         "0",            0 },
	{ &toolgun_disabledarg4,            "toolgun_disabledarg4",         "0",            0 },
	{ &sbt_color0_0,                    "sbt_color0_0",                 "0.00", 	    CVAR_ARCHIVE },
	{ &sbt_color0_1,                    "sbt_color0_1",                 "0.00", 	    CVAR_ARCHIVE },
	{ &sbt_color0_2,                    "sbt_color0_2",                 "0.00", 	    CVAR_ARCHIVE },
	{ &sbt_color0_3,                    "sbt_color0_3",                 "0.45",         CVAR_ARCHIVE },
	{ &sbt_color1_0,                    "sbt_color1_0",                 "0.25",         CVAR_ARCHIVE },
	{ &sbt_color1_1,                    "sbt_color1_1",                 "0.25",         CVAR_ARCHIVE },
	{ &sbt_color1_2,                    "sbt_color1_2",                 "0.25",         CVAR_ARCHIVE },
	{ &sbt_color1_3,                    "sbt_color1_3",                 "0.45",         CVAR_ARCHIVE },
	{ &sbt_color2_0,                    "sbt_color2_0",                 "0.30",         CVAR_ARCHIVE },
	{ &sbt_color2_1,                    "sbt_color2_1",                 "0.10",         CVAR_ARCHIVE },
	{ &sbt_color2_2,                    "sbt_color2_2",                 "0.90",         CVAR_ARCHIVE },
	{ &sbt_color2_3,                    "sbt_color2_3",                 "0.90",         CVAR_ARCHIVE },
	{ &sbt_color3_0,                    "sbt_color3_0",                 "0.90", 	    CVAR_ARCHIVE },
	{ &sbt_color3_1,                    "sbt_color3_1",                 "1.00", 	    CVAR_ARCHIVE },
	{ &sbt_color3_2,                    "sbt_color3_2",                 "1.00", 	    CVAR_ARCHIVE },
	{ &sbt_color3_3,                    "sbt_color3_3",                 "1.00", 	    CVAR_ARCHIVE },
	{ &sbt_wallpaper,                   "sbt_wallpaper",                "trans", 	    CVAR_ARCHIVE },
	{ &ui_3dmap,                        "ui_3dmap",                     "",             CVAR_ARCHIVE },
	{ &ui_effectslevel,                 "ui_effectslevel",              "0",            CVAR_ARCHIVE },
	{ &ui_singlemode,                   "ui_singlemode",                "0",            CVAR_ARCHIVE },
	{ &ui_loaded,                       "ui_loaded",                    "0",            0 },
	{ &cl_selectedmod,                  "cl_selectedmod",               "default",      CVAR_ARCHIVE },
	{ &cl_language,                     "cl_language",                  "0",            CVAR_ARCHIVE },
	{ &sensitivitymenu,                 "sensitivitymenu",              "1",            CVAR_ARCHIVE },
	{ &ui_spScores1,                    "g_spScores1",                  "",             CVAR_ARCHIVE|CVAR_ROM },
	{ &ui_spScores2,                    "g_spScores2",                  "",             CVAR_ARCHIVE|CVAR_ROM },
	{ &ui_spScores3,                    "g_spScores3",                  "",             CVAR_ARCHIVE|CVAR_ROM },
	{ &ui_spScores4,                    "g_spScores4",                  "",             CVAR_ARCHIVE|CVAR_ROM },
	{ &ui_spScores5,                    "g_spScores5",                  "",             CVAR_ARCHIVE|CVAR_ROM },
	{ &ui_spAwards,                     "g_spAwards",                   "",             CVAR_ARCHIVE|CVAR_ROM },
	{ &ui_spVideos,                     "g_spVideos",                   "",             CVAR_ARCHIVE|CVAR_ROM },
	{ &ui_spSkill,                      "g_spSkill",                    "1",            0 },
	{ &ui_spSelection,                  "ui_spSelection",               "",             CVAR_ROM },
	{ &ui_browserMaster,                "ui_browserMaster",             "0",            CVAR_ARCHIVE },
	{ &ui_browserGameType,              "ui_browserGameType",           "0",            CVAR_ARCHIVE },
	{ &ui_browserSortKey,               "ui_browserSortKey",            "4",            CVAR_ARCHIVE },
	{ &ui_browserShowFull,              "ui_browserShowFull",           "1",            CVAR_ARCHIVE },
	{ &ui_browserShowEmpty,             "ui_browserShowEmpty",          "1",            CVAR_ARCHIVE },
	{ &ui_drawCrosshair,                "cg_drawCrosshair",             "4",            CVAR_ARCHIVE },
	{ &ui_drawCrosshairNames,           "cg_drawCrosshairNames",        "1",            CVAR_ARCHIVE },
	{ &ui_server1,                      "server1",                      "",             CVAR_ARCHIVE },
	{ &ui_server2,                      "server2",                      "",             CVAR_ARCHIVE },
	{ &ui_server3,                      "server3",                      "",             CVAR_ARCHIVE },
	{ &ui_server4,                      "server4",                      "",             CVAR_ARCHIVE },
	{ &ui_server5,                      "server5",                      "",             CVAR_ARCHIVE },
	{ &ui_server6,                      "server6",                      "",             CVAR_ARCHIVE },
	{ &ui_server7,                      "server7",                      "",             CVAR_ARCHIVE },
	{ &ui_server8,                      "server8",                      "",             CVAR_ARCHIVE },
	{ &ui_server9,                      "server9",                      "",             CVAR_ARCHIVE },
	{ &ui_server10,                     "server10",                     "",             CVAR_ARCHIVE },
	{ &ui_server11,                     "server11",                     "",             CVAR_ARCHIVE },
	{ &ui_server12,                     "server12",                     "",             CVAR_ARCHIVE },
	{ &ui_server13,                     "server13",                     "",             CVAR_ARCHIVE },
	{ &ui_server14,                     "server14",                     "",             CVAR_ARCHIVE },
	{ &ui_server15,                     "server15",                     "",             CVAR_ARCHIVE },
	{ &ui_server16,                     "server16",                     "",             CVAR_ARCHIVE },
	{ &ui_server17,                     "server17",                     "",             CVAR_ARCHIVE },
	{ &ui_server18,                     "server18",                     "",             CVAR_ARCHIVE },
	{ &ui_server19,                     "server19",                     "",             CVAR_ARCHIVE },
	{ &ui_server20,                     "server20",                     "",             CVAR_ARCHIVE },
	{ &ui_server21,                     "server21",                     "",             CVAR_ARCHIVE },
	{ &ui_server22,                     "server22",                     "",             CVAR_ARCHIVE },
	{ &ui_server23,                     "server23",                     "",             CVAR_ARCHIVE },
	{ &ui_server24,                     "server24",                     "",             CVAR_ARCHIVE },
	{ &ui_server25,                     "server25",                     "",             CVAR_ARCHIVE },
	{ &ui_server26,                     "server26",                     "",             CVAR_ARCHIVE },
	{ &ui_server27,                     "server27",                     "",             CVAR_ARCHIVE },
	{ &ui_server28,                     "server28",                     "",             CVAR_ARCHIVE },
	{ &ui_server29,                     "server29",                     "",             CVAR_ARCHIVE },
	{ &ui_server30,                     "server30",                     "",             CVAR_ARCHIVE },
	{ &ui_server31,                     "server31",                     "",             CVAR_ARCHIVE },
	{ &ui_server32,                     "server32",                     "",             CVAR_ARCHIVE },
	{ &gui_map_multisel,                "gui_map_multisel",             "0",            CVAR_ROM|CVAR_ARCHIVE },
	{ &gui_map_list,                    "gui_map_list",                 "0",            CVAR_ROM|CVAR_ARCHIVE },
	{ &gui_bot_multisel,                "gui_bot_multisel",             "0",            CVAR_ROM|CVAR_ARCHIVE },
	{ &gui_bot_list,                    "gui_bot_list",                 "0",            CVAR_ROM|CVAR_ARCHIVE },
	{ &gui_animsfx,                     "gui_s_animsfx",                "1",            CVAR_ROM|CVAR_ARCHIVE },
	{ &gui_mapicons,                    "gui_mapicons",                 "0",            CVAR_ROM|CVAR_ARCHIVE },
	{ &gui_autoclosebotmenu,            "gui_autoclosebotmenu",         "0",            CVAR_ROM|CVAR_ARCHIVE },
};
#endif

static int cvarTableSize = sizeof( cvarTable ) / sizeof( cvarTable[0] );

/*
=================
ST_RegisterCvars
=================
*/
void ST_RegisterCvars(void) {
	cvarTable_t	*cv;
    int			i;

	for (i = 0, cv = cvarTable; i < cvarTableSize; i++, cv++) {
		trap_Cvar_Register( cv->vmCvar, cv->cvarName, cv->defaultString, cv->cvarFlags );
	}
}

/*
=================
ST_UpdateCvars
=================
*/
void ST_UpdateCvars(void) {
	cvarTable_t	*cv;
    int			i;

	for ( i = 0, cv = cvarTable ; i < cvarTableSize ; i++, cv++ ) {
		if ( cv->vmCvar ) {
			trap_Cvar_Update( cv->vmCvar );
		}
	}
}
