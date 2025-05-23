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

//#include "g_local.h"
#include "../qcommon/ns_local.h"	//load ns_local instead

level_locals_t	level;

typedef struct {
	vmCvar_t	*vmCvar;
	char		*cvarName;
	char		*defaultString;
	int			cvarFlags;
	int			modificationCount;  // for tracking changes
	qboolean	trackChange;	    // track this variable, and announce if changed
} cvarTable_t;

gentity_t		g_entities[MAX_GENTITIES];
gclient_t		g_clients[MAX_CLIENTS];
int				SourceTechEntityList[MAX_GENTITIES];

//OpenSandbox settings
vmCvar_t	g_physimpact;
vmCvar_t	g_physimpulse;
vmCvar_t	g_physdamage;

//gh set
vmCvar_t	g_ghspeed;
vmCvar_t	g_ghtimeout;
//g set
vmCvar_t	g_gdelay;
vmCvar_t	g_gdamage;
vmCvar_t	g_grange;
vmCvar_t	g_gknockback;
//mg set
vmCvar_t	g_mgdelay;
vmCvar_t	g_mgdamage;
vmCvar_t	g_mgspread;
vmCvar_t	g_mgexplode;
vmCvar_t	g_mgsdamage;
vmCvar_t	g_mgsradius;
vmCvar_t	g_mginf;
vmCvar_t	g_mgknockback;
//sg set
vmCvar_t	g_sgdelay;
vmCvar_t	g_sgdamage;
vmCvar_t	g_sgspread;
vmCvar_t	g_sgexplode;
vmCvar_t	g_sgsdamage;
vmCvar_t	g_sgsradius;
vmCvar_t	g_sgcount;
vmCvar_t	g_sginf;
vmCvar_t	g_sgknockback;
//gl set
vmCvar_t	g_gldelay;
vmCvar_t	g_glspeed;
vmCvar_t	g_gltimeout;
vmCvar_t	g_glsradius;
vmCvar_t	g_glsdamage;
vmCvar_t	g_gldamage;
vmCvar_t	g_glbounce;
vmCvar_t	g_glgravity;
vmCvar_t	g_glinf;
vmCvar_t	g_glbouncemodifier;
vmCvar_t	g_glknockback;
//rl set
vmCvar_t	g_rldelay;
vmCvar_t	g_rlspeed;
vmCvar_t	g_rltimeout;
vmCvar_t	g_rlsradius;
vmCvar_t	g_rlsdamage;
vmCvar_t	g_rldamage;
vmCvar_t	g_rlbounce;
vmCvar_t	g_rlgravity;
vmCvar_t	g_rlinf;
vmCvar_t	g_rlbouncemodifier;
vmCvar_t	g_rlknockback;
//lg set
vmCvar_t	g_lgdamage;
vmCvar_t	g_lgdelay;
vmCvar_t	g_lgrange;
vmCvar_t	g_lgexplode;
vmCvar_t	g_lgsdamage;
vmCvar_t	g_lgsradius;
vmCvar_t	g_lginf;
vmCvar_t	g_lgknockback;
//rg set
vmCvar_t	g_rgdelay;
vmCvar_t	g_rgdamage;
vmCvar_t	g_rginf;
vmCvar_t	g_rgknockback;
//pg set
vmCvar_t	g_pgdelay;
vmCvar_t	g_pgsradius;
vmCvar_t	g_pgspeed;
vmCvar_t	g_pgsdamage;
vmCvar_t	g_pgdamage;
vmCvar_t	g_pgtimeout;
vmCvar_t	g_pgbounce;
vmCvar_t	g_pggravity;
vmCvar_t	g_pginf;
vmCvar_t	g_pgbouncemodifier;
vmCvar_t	g_pgknockback;
//bfg set
vmCvar_t	g_bfgdelay;
vmCvar_t	g_bfgspeed;
vmCvar_t	g_bfgtimeout;
vmCvar_t	g_bfgsradius;
vmCvar_t	g_bfgsdamage;
vmCvar_t	g_bfgdamage;
vmCvar_t	g_bfgbounce;
vmCvar_t	g_bfggravity;
vmCvar_t	g_bfginf;
vmCvar_t	g_bfgbouncemodifier;
vmCvar_t	g_bfgknockback;
//ng set
vmCvar_t	g_ngdelay;
vmCvar_t	g_ngspeed;
vmCvar_t	g_ngspread;
vmCvar_t	g_ngdamage;
vmCvar_t	g_ngtimeout;
vmCvar_t	g_ngcount;
vmCvar_t	g_ngbounce;
vmCvar_t	g_nggravity;
vmCvar_t	g_ngrandom;
vmCvar_t	g_nginf;
vmCvar_t	g_ngbouncemodifier;
vmCvar_t	g_ngknockback;
//pl set
vmCvar_t	g_pldelay;
vmCvar_t	g_plspeed;
vmCvar_t	g_pltimeout;
vmCvar_t	g_plsradius;
vmCvar_t	g_plsdamage;
vmCvar_t	g_plgravity;
vmCvar_t	g_pldamage;
vmCvar_t	g_plinf;
vmCvar_t	g_plknockback;
//cg set
vmCvar_t	g_cgdelay;
vmCvar_t	g_cgdamage;
vmCvar_t	g_cgspread;
vmCvar_t	g_cginf;
vmCvar_t	g_cgknockback;
//ft set
vmCvar_t	g_ftdelay;
vmCvar_t	g_ftsradius;
vmCvar_t	g_ftspeed;
vmCvar_t	g_ftsdamage;
vmCvar_t	g_ftdamage;
vmCvar_t	g_fttimeout;
vmCvar_t	g_ftbounce;
vmCvar_t	g_ftgravity;
vmCvar_t	g_ftinf;
vmCvar_t	g_ftbouncemodifier;
vmCvar_t	g_ftknockback;
//am set
vmCvar_t	g_amdelay;
vmCvar_t	g_amsradius;
vmCvar_t	g_amspeed;
vmCvar_t	g_amsdamage;
vmCvar_t	g_amdamage;
vmCvar_t	g_amtimeout;
vmCvar_t	g_ambounce;
vmCvar_t	g_amgravity;
vmCvar_t	g_aminf;
vmCvar_t	g_ambouncemodifier;
vmCvar_t	g_amknockback;
//guided and homing
vmCvar_t	g_glhoming;
vmCvar_t	g_glguided;
vmCvar_t	g_rlhoming;
vmCvar_t	g_rlguided;
vmCvar_t	g_pghoming;
vmCvar_t	g_pgguided;
vmCvar_t	g_bfghoming;
vmCvar_t	g_bfgguided;
vmCvar_t	g_nghoming;
vmCvar_t	g_ngguided;
vmCvar_t	g_fthoming;
vmCvar_t	g_ftguided;
vmCvar_t	g_amhoming;
vmCvar_t	g_amguided;
//rune s set
vmCvar_t	g_scoutspeedfactor;
vmCvar_t	g_scoutfirespeed;
vmCvar_t	g_scoutdamagefactor;
vmCvar_t	g_scoutgravitymodifier;
vmCvar_t	g_scout_infammo;
vmCvar_t	g_scouthealthmodifier;
//rune d set
vmCvar_t	g_doublerspeedfactor;
vmCvar_t	g_doublerfirespeed;
vmCvar_t	g_doublerdamagefactor;
vmCvar_t	g_doublergravitymodifier;
vmCvar_t	g_doubler_infammo;
vmCvar_t	g_doublerhealthmodifier;
//rune g set
vmCvar_t	g_guardhealthmodifier;
vmCvar_t	g_guardspeedfactor;
vmCvar_t	g_guardfirespeed;
vmCvar_t	g_guarddamagefactor;
vmCvar_t	g_guardgravitymodifier;
vmCvar_t	g_guard_infammo;
//rune a set
vmCvar_t	g_ammoregenspeedfactor;
vmCvar_t	g_ammoregenfirespeed;
vmCvar_t	g_ammoregen_infammo;
vmCvar_t	g_ammoregendamagefactor;
vmCvar_t	g_ammoregengravitymodifier;
vmCvar_t	g_ammoregenhealthmodifier;
//ammocount,s
vmCvar_t	g_mgammocount;
vmCvar_t	g_sgammocount;
vmCvar_t	g_glammocount;
vmCvar_t	g_rlammocount;
vmCvar_t	g_lgammocount;
vmCvar_t	g_rgammocount;
vmCvar_t	g_pgammocount;
vmCvar_t	g_bfgammocount;
vmCvar_t	g_ngammocount;
vmCvar_t	g_plammocount;
vmCvar_t	g_cgammocount;
vmCvar_t	g_ftammocount;
//weaponcount,s
vmCvar_t	g_mgweaponcount;
vmCvar_t	g_sgweaponcount;
vmCvar_t	g_glweaponcount;
vmCvar_t	g_rlweaponcount;
vmCvar_t	g_lgweaponcount;
vmCvar_t	g_rgweaponcount;
vmCvar_t	g_pgweaponcount;
vmCvar_t	g_bfgweaponcount;
vmCvar_t	g_ngweaponcount;
vmCvar_t	g_plweaponcount;
vmCvar_t	g_cgweaponcount;
vmCvar_t	g_ftweaponcount;
vmCvar_t	g_amweaponcount;
//blueteam set
vmCvar_t	g_teamblue_speed;
vmCvar_t	g_teamblue_gravityModifier;
vmCvar_t	g_teamblue_firespeed;
vmCvar_t	g_teamblue_damage;
vmCvar_t	g_teamblue_infammo;
vmCvar_t	g_teamblue_respawnwait;
vmCvar_t	g_teamblue_pickupitems;
//redteam set
vmCvar_t	g_teamred_speed;
vmCvar_t	g_teamred_gravityModifier;
vmCvar_t	g_teamred_firespeed;
vmCvar_t	g_teamred_damage;
vmCvar_t	g_teamred_infammo;
vmCvar_t	g_teamred_respawnwait;
vmCvar_t	g_teamred_pickupitems;
//cvars
vmCvar_t	g_fogModel;
vmCvar_t	g_fogShader;
vmCvar_t	g_fogDistance;
vmCvar_t	g_fogInterval;
vmCvar_t	g_fogColorR;
vmCvar_t	g_fogColorG;
vmCvar_t	g_fogColorB;
vmCvar_t	g_fogColorA;
vmCvar_t	g_skyShader;
vmCvar_t	g_skyColorR;
vmCvar_t	g_skyColorG;
vmCvar_t	g_skyColorB;
vmCvar_t	g_skyColorA;

vmCvar_t	g_shitAngleTest0;
vmCvar_t	g_shitAngleTest1;
vmCvar_t	g_shitAngleTest2;
vmCvar_t	g_entitypack;
vmCvar_t	g_extendedsandbox;
vmCvar_t	g_allowprops;
vmCvar_t	g_allownpc;
vmCvar_t	g_allowitems;
vmCvar_t	g_allownoclip;
vmCvar_t	g_allowtoolgun;
vmCvar_t	g_allowphysgun;
vmCvar_t	g_allowgravitygun;
vmCvar_t	g_maxEntities;
vmCvar_t	cl_selectedmod;
vmCvar_t	cl_language;
vmCvar_t	g_regenarmor;
vmCvar_t	g_movetype;
vmCvar_t	g_accelerate;
vmCvar_t	g_randomItems;
vmCvar_t	g_kamikazeinf;
vmCvar_t	g_invulinf;
vmCvar_t	g_medkitinf;
vmCvar_t	g_teleporterinf;
vmCvar_t	g_portalinf;
vmCvar_t	g_medkitlimit;
vmCvar_t	g_waterdamage;
vmCvar_t	g_lavadamage;
vmCvar_t	g_slimedamage;
vmCvar_t	g_nextbot_speed;
vmCvar_t	g_randomteleport;
vmCvar_t	g_falldamagesmall;
vmCvar_t	g_falldamagebig;
vmCvar_t	g_noplayerclip;
vmCvar_t	g_flagrespawn;
vmCvar_t	g_portaltimeout;
vmCvar_t	g_portalhealth;
vmCvar_t	g_quadtime;
vmCvar_t	g_bsuittime;
vmCvar_t	g_hastetime;
vmCvar_t	g_invistime;
vmCvar_t	g_regentime;
vmCvar_t	g_flighttime;
vmCvar_t	g_invulmove;
vmCvar_t	g_invultime;
vmCvar_t	g_fasthealthregen;
vmCvar_t	g_slowhealthregen;
vmCvar_t	g_hastefirespeed;
vmCvar_t	g_medkitmodifier;
vmCvar_t	g_armorprotect;
vmCvar_t	g_respawnwait;
vmCvar_t	g_ammolimit;
vmCvar_t	g_jumpheight;
vmCvar_t	g_speedfactor;
vmCvar_t	g_drowndamage;
vmCvar_t	g_armorrespawn;
vmCvar_t	g_healthrespawn;
vmCvar_t	g_ammorespawn;
vmCvar_t	g_holdablerespawn;
vmCvar_t	g_megahealthrespawn;
vmCvar_t	g_poweruprespawn;
vmCvar_t	g_gametype;
vmCvar_t	g_elimflags;
vmCvar_t	g_fraglimit;
vmCvar_t	g_timelimit;
vmCvar_t	g_capturelimit;
vmCvar_t	g_friendlyFire;
vmCvar_t	g_password;
vmCvar_t	g_maxClients;
vmCvar_t	g_dedicated;
vmCvar_t	g_speed;
vmCvar_t	g_gravity;
vmCvar_t	g_gravityModifier;
vmCvar_t    g_damageModifier;
vmCvar_t	g_cheats;
vmCvar_t	g_knockback;
vmCvar_t	g_quadfactor;
vmCvar_t	g_respawntime;
vmCvar_t	g_weaponRespawn;
vmCvar_t	g_weaponTeamRespawn;
vmCvar_t	g_warmup;
vmCvar_t	g_doWarmup;
vmCvar_t	g_restarted;
vmCvar_t	g_blood;
vmCvar_t	g_allowVote;
vmCvar_t	g_teamAutoJoin;
vmCvar_t	g_teamForceBalance;
vmCvar_t	g_smoothClients;
vmCvar_t	g_obeliskHealth;
vmCvar_t	g_obeliskRegenPeriod;
vmCvar_t	g_obeliskRegenAmount;
vmCvar_t	g_obeliskRespawnDelay;
vmCvar_t	g_cubeTimeout;
vmCvar_t	g_enableDust;
vmCvar_t	g_enableBreath;
vmCvar_t	g_proxMineTimeout;
vmCvar_t	g_music;
vmCvar_t    g_spawnprotect;

//Settings for elimination:
vmCvar_t	g_elimination_selfdamage;
vmCvar_t	g_elimination_roundtime;
vmCvar_t	g_elimination_warmup;
vmCvar_t	g_elimination_activewarmup;
vmCvar_t    g_elimination;
vmCvar_t 	g_elimination_items;
vmCvar_t    g_elimination_lockspectator;
vmCvar_t	g_elimination_ctf_oneway;
vmCvar_t	g_elimination_red_respawn;
vmCvar_t	g_elimination_blue_respawn;

//Spawn settings for blue and white:
vmCvar_t	g_bluespawn_health;
vmCvar_t	g_bluespawn_armor;

vmCvar_t	g_bluespawn_gauntlet;
vmCvar_t	g_bluespawn_machinegun;
vmCvar_t	g_bluespawn_shotgun;
vmCvar_t	g_bluespawn_grenade;
vmCvar_t	g_bluespawn_rocket;
vmCvar_t	g_bluespawn_lightning;
vmCvar_t	g_bluespawn_railgun;
vmCvar_t	g_bluespawn_plasmagun;
vmCvar_t	g_bluespawn_bfg;
vmCvar_t	g_bluespawn_grapple;
vmCvar_t	g_bluespawn_nail;
vmCvar_t	g_bluespawn_mine;
vmCvar_t	g_bluespawn_chain;
vmCvar_t	g_bluespawn_flame;
vmCvar_t	g_bluespawn_antimatter;

vmCvar_t	g_bluespawn_quad;
vmCvar_t	g_bluespawn_haste;
vmCvar_t	g_bluespawn_bsuit;
vmCvar_t	g_bluespawn_invis;
vmCvar_t	g_bluespawn_regen;
vmCvar_t	g_bluespawn_flight;

vmCvar_t 	g_bluespawn_holdable;

//Spawn settings for red:
vmCvar_t	g_redspawn_health;
vmCvar_t	g_redspawn_armor;

vmCvar_t	g_redspawn_gauntlet;
vmCvar_t	g_redspawn_machinegun;
vmCvar_t	g_redspawn_shotgun;
vmCvar_t	g_redspawn_grenade;
vmCvar_t	g_redspawn_rocket;
vmCvar_t	g_redspawn_lightning;
vmCvar_t	g_redspawn_railgun;
vmCvar_t	g_redspawn_plasmagun;
vmCvar_t	g_redspawn_bfg;
vmCvar_t	g_redspawn_grapple;
vmCvar_t	g_redspawn_nail;
vmCvar_t	g_redspawn_mine;
vmCvar_t	g_redspawn_chain;
vmCvar_t	g_redspawn_flame;
vmCvar_t	g_redspawn_antimatter;

vmCvar_t	g_redspawn_quad;
vmCvar_t	g_redspawn_haste;
vmCvar_t	g_redspawn_bsuit;
vmCvar_t	g_redspawn_invis;
vmCvar_t	g_redspawn_regen;
vmCvar_t	g_redspawn_flight;

vmCvar_t 	g_redspawn_holdable;

vmCvar_t	g_vampire;
vmCvar_t	g_vampireMaxHealth;
//Regen
vmCvar_t	g_regen;
int	g_ffa_gt; //Are this a FFA gametype even if gametype is high?
vmCvar_t	g_lms_lives;
vmCvar_t	g_lms_mode;

vmCvar_t	sv_fps;

int 		mod_jumpheight;
int 		mod_ammolimit;
int			mod_ghtimeout;
int			mod_gdelay;
int			mod_mgdelay;
int			mod_mgspread;
int			mod_sgdelay;
int			mod_sgspread;
int			mod_sgcount;
int			mod_gldelay;
int			mod_rldelay;
int			mod_lgdelay;
int			mod_lgrange;
int			mod_pgdelay;
int			mod_rgdelay;
int			mod_bfgdelay;
int			mod_ngdelay;
int			mod_pldelay;
int			mod_cgdelay;
int			mod_cgspread;
int			mod_ftdelay;
int			mod_amdelay;
float 		mod_hastefirespeed;
float 		mod_ammoregenfirespeed;
float 		mod_scoutfirespeed;
int			mod_poweruptime;
float		mod_guardfirespeed;
float		mod_doublerfirespeed;
int			mod_quadtime;
int			mod_bsuittime;
int			mod_hastetime;
int			mod_invistime;
int			mod_regentime;
int			mod_flighttime;
int 		mod_noplayerclip;
int			mod_ammolimit;
int 		mod_invulmove;
float 		mod_teamblue_firespeed;
float 		mod_teamred_firespeed;
int 		mod_medkitlimit;
int			mod_medkitinf;
int			mod_teleporterinf;
int			mod_portalinf;
int 		mod_kamikazeinf;
int 		mod_invulinf;
int 		mod_teamblue_damage;
int 		mod_teamred_damage;
int			mod_accelerate;
int			mod_movetype;
int			mod_gravity;


// bk001129 - made static to avoid aliasing
static cvarTable_t		gameCvarTable[] = {
	// don't override the cheat state set by the system
	{ &g_cheats, "sv_cheats", "", 0, 0, qfalse },

	// noset vars
	{ NULL, "gamename", GAME_VERSION , CVAR_ROM, 0, qfalse  },
	{ NULL, "gamedate", __DATE__ , CVAR_ROM, 0, qfalse  },
	{ &g_restarted, "g_restarted", "0", CVAR_ROM, 0, qfalse  },

	// latched vars
	{ &g_gametype, "g_gametype", "0", CVAR_SERVERINFO | CVAR_USERINFO | CVAR_LATCH, 0, qfalse  },

	{ &g_maxClients, "g_maxClients", "128", CVAR_SERVERINFO | CVAR_ARCHIVE, 0, qfalse  },
	
	{ &g_physimpact, "g_physimpact", "0.80", 0, 0, qtrue  },
	{ &g_physimpulse, "g_physimpulse", "450", 0, 0, qtrue  },
	{ &g_physdamage, "g_physdamage", "0.60", 0, 0, qtrue  },
	
	//OpenSandbox setting

	//gh set
	{ &g_ghspeed, "g_ghspeed", "800", 0, 0, qtrue  },
	{ &g_ghtimeout, "g_ghtimeout", "10000", 0, 0, qtrue  },
	//g set
	{ &g_gdelay, "g_gdelay", "400", 0, 0, qtrue  },
	{ &g_gdamage, "g_gdamage", "50", 0, 0, qtrue  },
	{ &g_grange, "g_grange", "32", 0, 0, qtrue  },
	{ &g_gknockback, "g_gknockback", "1", 0, 0, qtrue  },
	//mg set
	{ &g_mgdelay, "g_mgdelay", "100", 0, 0, qtrue  },
	{ &g_mgdamage, "g_mgdamage", "7", 0, 0, qtrue  },
	{ &g_mgspread, "g_mgspread", "200", 0, 0, qtrue  },
	{ &g_mgexplode, "g_mgexplode", "0", 0, 0, qtrue  },
	{ &g_mgsradius, "g_mgsradius", "70", 0, 0, qtrue  },
	{ &g_mgsdamage, "g_mgsdamage", "50", 0, 0, qtrue  },
	{ &g_mginf, "g_mginf", "0", 0, 0, qtrue  },
	{ &g_mgknockback, "g_mgknockback", "1", 0, 0, qtrue  },
	//sg set
	{ &g_sgdelay, "g_sgdelay", "1000", 0, 0, qtrue  },
	{ &g_sgdamage, "g_sgdamage", "10", 0, 0, qtrue  },
	{ &g_sgspread, "g_sgspread", "700", 0, 0, qtrue  },
	{ &g_sgexplode, "g_sgexplode", "0", 0, 0, qtrue  },
	{ &g_sgsradius, "g_sgsradius", "35", 0, 0, qtrue  },
	{ &g_sgsdamage, "g_sgsdamage", "45", 0, 0, qtrue  },
	{ &g_sgcount, "g_sgcount", "11", 0, 0, qtrue  },
	{ &g_sginf, "g_sginf", "0", 0, 0, qtrue  },
	{ &g_sgknockback, "g_sgknockback", "1", 0, 0, qtrue  },
	//gl set
	{ &g_gldelay, "g_gldelay", "500", 0, 0, qtrue  },
	{ &g_glspeed, "g_glspeed", "700", 0, 0, qtrue  },
	{ &g_gltimeout, "g_gltimeout", "2500", 0, 0, qtrue  },
	{ &g_glsradius, "g_glsradius", "120", 0, 0, qtrue  },
	{ &g_glsdamage, "g_glsdamage", "100", 0, 0, qtrue  },
	{ &g_gldamage, "g_gldamage", "100", 0, 0, qtrue  },
	{ &g_glbounce, "g_glbounce", "1", 0, 0, qtrue  },
	{ &g_glgravity, "g_glgravity", "1", 0, 0, qtrue  },
	{ &g_glinf, "g_glinf", "0", 0, 0, qtrue  },
	{ &g_glbouncemodifier, "g_glbouncemodifier", "0.65", 0, 0, qtrue  },
	{ &g_glknockback, "g_glknockback", "1", 0, 0, qtrue  },
	//rl set
	{ &g_rldelay, "g_rldelay", "800", 0, 0, qtrue  },
	{ &g_rlspeed, "g_rlspeed", "900", 0, 0, qtrue  },
	{ &g_rltimeout, "g_rltimeout", "15000", 0, 0, qtrue  },
	{ &g_rlsradius, "g_rlsradius", "120", 0, 0, qtrue  },
	{ &g_rlsdamage, "g_rlsdamage", "100", 0, 0, qtrue  },
	{ &g_rldamage, "g_rldamage", "100", 0, 0, qtrue  },
	{ &g_rlbounce, "g_rlbounce", "0", 0, 0, qtrue  },
	{ &g_rlgravity, "g_rlgravity", "0", 0, 0, qtrue  },
	{ &g_rlinf, "g_rlinf", "0", 0, 0, qtrue  },
	{ &g_rlbouncemodifier, "g_rlbouncemodifier", "0.65", 0, 0, qtrue  },
	{ &g_rlknockback, "g_rlknockback", "1", 0, 0, qtrue  },
	//lg set
	{ &g_lgdelay, "g_lgdelay", "50", 0, 0, qtrue  },
	{ &g_lgdamage, "g_lgdamage", "8", 0, 0, qtrue  },
	{ &g_lgrange, "g_lgrange", "768", 0, 0, qtrue  },
	{ &g_lgexplode, "g_lgexplode", "0", 0, 0, qtrue  },
	{ &g_lgsradius, "g_lgsradius", "75", 0, 0, qtrue  },
	{ &g_lgsdamage, "g_lgsdamage", "70", 0, 0, qtrue  },
	{ &g_lginf, "g_lginf", "0", 0, 0, qtrue  },
	{ &g_lgknockback, "g_lgknockback", "1", 0, 0, qtrue  },
	//rg set
	{ &g_rgdelay, "g_rgdelay", "1500", 0, 0, qtrue  },
	{ &g_rgdamage, "g_rgdamage", "100", 0, 0, qtrue  },
	{ &g_rginf, "g_rginf", "0", 0, 0, qtrue  },
	{ &g_rgknockback, "g_rgknockback", "1", 0, 0, qtrue  },
	//pg set
	{ &g_pgsradius, "g_pgsradius", "20", 0, 0, qtrue  },
	{ &g_pgspeed, "g_pgspeed", "2000", 0, 0, qtrue  },
	{ &g_pgsdamage, "g_pgsdamage", "15", 0, 0, qtrue  },
	{ &g_pgdamage, "g_pgdamage", "20", 0, 0, qtrue  },
	{ &g_pgtimeout, "g_pgtimeout", "10000", 0, 0, qtrue  },
	{ &g_pgdelay, "g_pgdelay", "100", 0, 0, qtrue  },
	{ &g_pgbounce, "g_pgbounce", "0", 0, 0, qtrue  },
	{ &g_pggravity, "g_pggravity", "0", 0, 0, qtrue  },
	{ &g_pginf, "g_pginf", "0", 0, 0, qtrue  },
	{ &g_pgbouncemodifier, "g_pgbouncemodifier", "0.65", 0, 0, qtrue  },
	{ &g_pgknockback, "g_pgknockback", "1", 0, 0, qtrue  },
	//bfg set
	{ &g_bfgsradius, "g_bfgsradius", "120", 0, 0, qtrue  },
	{ &g_bfgspeed, "g_bfgspeed", "2000", 0, 0, qtrue  },
	{ &g_bfgsdamage, "g_bfgsdamage", "100", 0, 0, qtrue  },
	{ &g_bfgdamage, "g_bfgdamage", "100", 0, 0, qtrue  },
	{ &g_bfgtimeout, "g_bfgtimeout", "10000", 0, 0, qtrue  },
	{ &g_bfgdelay, "g_bfgdelay", "200", 0, 0, qtrue  },
	{ &g_bfgbounce, "g_bfgbounce", "0", 0, 0, qtrue  },
	{ &g_bfggravity, "g_bfggravity", "0", 0, 0, qtrue  },
	{ &g_bfginf, "g_bfginf", "0", 0, 0, qtrue  },
	{ &g_bfgbouncemodifier, "g_bfgbouncemodifier", "0.65", 0, 0, qtrue  },
	{ &g_bfgknockback, "g_bfgknockback", "1", 0, 0, qtrue  },
	//ng set
	{ &g_ngdelay, "g_ngdelay", "1000", 0, 0, qtrue  },
	{ &g_ngspeed, "g_ngspeed", "555", 0, 0, qtrue  },
	{ &g_ngspread, "g_ngspread", "500", 0, 0, qtrue  },
	{ &g_ngdamage, "g_ngdamage", "20", 0, 0, qtrue  },
	{ &g_ngtimeout, "g_ngtimeout", "10000", 0, 0, qtrue  },
	{ &g_ngcount, "g_ngcount", "15", 0, 0, qtrue  },
	{ &g_ngbounce, "g_ngbounce", "0", 0, 0, qtrue  },
	{ &g_nggravity, "g_nggravity", "0", 0, 0, qtrue  },
	{ &g_ngrandom, "g_ngrandom", "1800", 0, 0, qtrue  },
	{ &g_nginf, "g_nginf", "0", 0, 0, qtrue  },
	{ &g_ngbouncemodifier, "g_ngbouncemodifier", "0.65", 0, 0, qtrue  },
	{ &g_ngknockback, "g_ngknockback", "1", 0, 0, qtrue  },
	//pl set
	{ &g_pldelay, "g_pldelay", "800", 0, 0, qtrue  },
	{ &g_plspeed, "g_plspeed", "700", 0, 0, qtrue  },
	{ &g_pltimeout, "g_pltimeout", "3000", 0, 0, qtrue  },
	{ &g_plsradius, "g_plsradius", "150", 0, 0, qtrue  },
	{ &g_plsdamage, "g_plsdamage", "100", 0, 0, qtrue  },
	{ &g_plgravity, "g_plgravity", "1", 0, 0, qtrue  },
	{ &g_pldamage, "g_pldamage", "0", 0, 0, qtrue  },
	{ &g_plinf, "g_plinf", "0", 0, 0, qtrue  },
	{ &g_plknockback, "g_plknockback", "1", 0, 0, qtrue  },
	//cg set
	{ &g_cgdelay, "g_cgdelay", "30", 0, 0, qtrue  },
	{ &g_cgdamage, "g_cgdamage", "7", 0, 0, qtrue  },
	{ &g_cgspread, "g_cgspread", "600", 0, 0, qtrue  },
	{ &g_cginf, "g_cginf", "0", 0, 0, qtrue  },
	{ &g_cgknockback, "g_cgknockback", "1", 0, 0, qtrue  },
	//ft set
	{ &g_ftsradius, "g_ftsradius", "20", 0, 0, qtrue  },
	{ &g_ftspeed, "g_ftspeed", "600", 0, 0, qtrue  },
	{ &g_ftsdamage, "g_ftsdamage", "15", 0, 0, qtrue  },
	{ &g_ftdamage, "g_ftdamage", "20", 0, 0, qtrue  },
	{ &g_fttimeout, "g_fttimeout", "1500", 0, 0, qtrue  },
	{ &g_ftdelay, "g_ftdelay", "40", 0, 0, qtrue  },
	{ &g_ftbounce, "g_ftbounce", "1", 0, 0, qtrue  },
	{ &g_ftgravity, "g_ftgravity", "0", 0, 0, qtrue  },
	{ &g_ftinf, "g_ftinf", "0", 0, 0, qtrue  },
	{ &g_ftbouncemodifier, "g_ftbouncemodifier", "0.65", 0, 0, qtrue  },
	{ &g_ftknockback, "g_ftknockback", "1", 0, 0, qtrue  },
	//am set
	{ &g_amsradius, "g_amsradius", "0", 0, 0, qtrue  },
	{ &g_amspeed, "g_amspeed", "600", 0, 0, qtrue  },
	{ &g_amsdamage, "g_amsdamage", "0", 0, 0, qtrue  },
	{ &g_amdamage, "g_amdamage", "6", 0, 0, qtrue  },
	{ &g_amtimeout, "g_amtimeout", "1000", 0, 0, qtrue  },
	{ &g_amdelay, "g_amdelay", "40", 0, 0, qtrue  },
	{ &g_ambounce, "g_ambounce", "1", 0, 0, qtrue  },
	{ &g_amgravity, "g_amgravity", "1", 0, 0, qtrue  },
	{ &g_aminf, "g_aminf", "0", 0, 0, qtrue  },
	{ &g_ambouncemodifier, "g_ambouncemodifier", "0.65", 0, 0, qtrue  },
	{ &g_amknockback, "g_amknockback", "1", 0, 0, qtrue  },
	//guided and homing
	{ &g_glhoming, "g_glhoming", "0", 0, 0, qtrue  },
	{ &g_glguided, "g_glguided", "0", 0, 0, qtrue  },
	{ &g_rlhoming, "g_rlhoming", "0", 0, 0, qtrue  },
	{ &g_rlguided, "g_rlguided", "0", 0, 0, qtrue  },
	{ &g_pghoming, "g_pghoming", "0", 0, 0, qtrue  },
	{ &g_pgguided, "g_pgguided", "0", 0, 0, qtrue  },
	{ &g_bfghoming, "g_bfghoming", "0", 0, 0, qtrue  },
	{ &g_bfgguided, "g_bfgguided", "0", 0, 0, qtrue  },
	{ &g_nghoming, "g_nghoming", "0", 0, 0, qtrue  },
	{ &g_ngguided, "g_ngguided", "0", 0, 0, qtrue  },
	{ &g_fthoming, "g_fthoming", "0", 0, 0, qtrue  },
	{ &g_ftguided, "g_ftguided", "0", 0, 0, qtrue  },
	{ &g_amhoming, "g_amhoming", "0", 0, 0, qtrue  },
	{ &g_amguided, "g_amguided", "0", 0, 0, qtrue  },
	//rune s set
	{ &g_scoutspeedfactor, "g_scoutspeedfactor", "1.5", 0, 0, qtrue  },
	{ &g_scoutfirespeed, "g_scoutfirespeed", "0.60", 0, 0, qtrue  },
	{ &g_scoutdamagefactor, "g_scoutdamagefactor", "1", 0, 0, qtrue  },
	{ &g_scoutgravitymodifier, "g_scoutgravitymodifier", "1", 0, 0, qtrue  },
	{ &g_scout_infammo, "g_scout_infammo", "0", 0, 0, qtrue  },
	{ &g_scouthealthmodifier, "g_scouthealthmodifier", "1", 0, 0, qtrue  },
	//rune d set
	{ &g_doublerfirespeed, "g_doublerfirespeed", "1", 0, 0, qtrue  },
	{ &g_doublerdamagefactor, "g_doublerdamagefactor", "2.5", 0, 0, qtrue  },
	{ &g_doublerspeedfactor, "g_doublerspeedfactor", "1", 0, 0, qtrue  },
	{ &g_doublergravitymodifier, "g_doublergravitymodifier", "1", 0, 0, qtrue  },
	{ &g_doubler_infammo, "g_doubler_infammo", "0", 0, 0, qtrue  },
	{ &g_doublerhealthmodifier, "g_doublerhealthmodifier", "1", 0, 0, qtrue  },
	//rune g set
	{ &g_guardhealthmodifier, "g_guardhealthmodifier", "1.5", 0, 0, qtrue  },
	{ &g_guardfirespeed, "g_guardfirespeed", "1", 0, 0, qtrue  },
	{ &g_guarddamagefactor, "g_guarddamagefactor", "1", 0, 0, qtrue  },
	{ &g_guardspeedfactor, "g_guardspeedfactor", "1", 0, 0, qtrue  },
	{ &g_guardgravitymodifier, "g_guardgravitymodifier", "1", 0, 0, qtrue  },
	{ &g_guard_infammo, "g_guard_infammo", "0", 0, 0, qtrue  },
	//rune a set
	{ &g_ammoregenfirespeed, "g_ammoregenfirespeed", "0.65", 0, 0, qtrue  },
	{ &g_ammoregen_infammo, "g_ammoregen_infammo", "0", 0, 0, qtrue  },
	{ &g_ammoregendamagefactor, "g_ammoregendamagefactor", "1", 0, 0, qtrue  },
	{ &g_ammoregenspeedfactor, "g_ammoregenspeedfactor", "1", 0, 0, qtrue  },
	{ &g_ammoregengravitymodifier, "g_ammoregengravitymodifier", "1", 0, 0, qtrue  },
	{ &g_ammoregenhealthmodifier, "g_ammoregenhealthmodifier", "1", 0, 0, qtrue  },
	//ammocount,s
	{ &g_mgammocount, "g_mgammocount", "-1", 0, 0, qtrue  },
	{ &g_sgammocount, "g_sgammocount", "-1", 0, 0, qtrue  },
	{ &g_glammocount, "g_glammocount", "-1", 0, 0, qtrue  },
	{ &g_pgammocount, "g_pgammocount", "-1", 0, 0, qtrue  },
	{ &g_lgammocount, "g_lgammocount", "-1", 0, 0, qtrue  },
	{ &g_rlammocount, "g_rlammocount", "-1", 0, 0, qtrue  },
	{ &g_rgammocount, "g_rgammocount", "-1", 0, 0, qtrue  },
	{ &g_bfgammocount, "g_bfgammocount", "-1", 0, 0, qtrue  },
	{ &g_ngammocount, "g_ngammocount", "-1", 0, 0, qtrue  },
	{ &g_plammocount, "g_plammocount", "-1", 0, 0, qtrue  },
	{ &g_cgammocount, "g_cgammocount", "-1", 0, 0, qtrue  },
	{ &g_ftammocount, "g_ftammocount", "-1", 0, 0, qtrue  },
	//weaponcount,s
	{ &g_mgweaponcount, "g_mgweaponcount", "-1", 0, 0, qtrue  },
	{ &g_sgweaponcount, "g_sgweaponcount", "-1", 0, 0, qtrue  },
	{ &g_glweaponcount, "g_glweaponcount", "-1", 0, 0, qtrue  },
	{ &g_pgweaponcount, "g_pgweaponcount", "-1", 0, 0, qtrue  },
	{ &g_lgweaponcount, "g_lgweaponcount", "-1", 0, 0, qtrue  },
	{ &g_rlweaponcount, "g_rlweaponcount", "-1", 0, 0, qtrue  },
	{ &g_rgweaponcount, "g_rgweaponcount", "-1", 0, 0, qtrue  },
	{ &g_bfgweaponcount, "g_bfgweaponcount", "-1", 0, 0, qtrue  },
	{ &g_ngweaponcount, "g_ngweaponcount", "-1", 0, 0, qtrue  },
	{ &g_plweaponcount, "g_plweaponcount", "-1", 0, 0, qtrue  },
	{ &g_cgweaponcount, "g_cgweaponcount", "-1", 0, 0, qtrue  },
	{ &g_ftweaponcount, "g_ftweaponcount", "-1", 0, 0, qtrue  },
	{ &g_amweaponcount, "g_amweaponcount", "-1", 0, 0, qtrue  },
	//redteam set
	{ &g_teamred_speed, "g_teamred_speed", "320", 0, 0, qtrue  },
	{ &g_teamred_gravityModifier, "g_teamred_gravityModifier", "1", 0, 0, qtrue  },
	{ &g_teamred_firespeed, "g_teamred_firespeed", "1", 0, 0, qtrue  },
	{ &g_teamred_damage, "g_teamred_damage", "1", 0, 0, qtrue  },
	{ &g_teamred_infammo, "g_teamred_infammo", "0", 0, 0, qtrue  },
	{ &g_teamred_respawnwait, "g_teamred_respawnwait", "3000", 0, 0, qtrue  },
	{ &g_teamred_pickupitems, "g_teamred_pickupitems", "1", 0, 0, qtrue  },
	//blueteam set
	{ &g_teamblue_speed, "g_teamblue_speed", "320", 0, 0, qtrue  },
	{ &g_teamblue_gravityModifier, "g_teamblue_gravityModifier", "1", 0, 0, qtrue  },
	{ &g_teamblue_firespeed, "g_teamblue_firespeed", "1", 0, 0, qtrue  },
	{ &g_teamblue_damage, "g_teamblue_damage", "1", 0, 0, qtrue  },
	{ &g_teamblue_infammo, "g_teamblue_infammo", "0", 0, 0, qtrue  },
	{ &g_teamblue_respawnwait, "g_teamblue_respawnwait", "3000", 0, 0, qtrue  },
	{ &g_teamblue_pickupitems, "g_teamblue_pickupitems", "1", 0, 0, qtrue  },
	// change anytime vars
	{ &g_shitAngleTest0, "g_shitAngleTest0", "0", 0, 0, qtrue  },
	{ &g_shitAngleTest1, "g_shitAngleTest1", "0", 0, 0, qtrue  },
	{ &g_shitAngleTest2, "g_shitAngleTest2", "0", 0, 0, qtrue  },
	{ &g_entitypack, "g_entitypack", "default", 0, 0, qtrue  },
	{ &g_extendedsandbox, "g_extendedsandbox", "0", 0, 0, qtrue  },
	{ &g_fogModel, "g_fogModel", "1", 0, 0, qtrue  },
	{ &g_fogShader, "g_fogShader", "1", 0, 0, qtrue  },
	{ &g_fogDistance, "g_fogDistance", "8", 0, 0, qtrue  },
	{ &g_fogInterval, "g_fogInterval", "32", 0, 0, qtrue  },
	{ &g_fogColorR, "g_fogColorR", "210", 0, 0, qtrue  },
	{ &g_fogColorG, "g_fogColorG", "245", 0, 0, qtrue  },
	{ &g_fogColorB, "g_fogColorB", "255", 0, 0, qtrue  },
	{ &g_fogColorA, "g_fogColorA", "0", 0, 0, qtrue  },
	{ &g_skyShader, "g_skyShader", "1", 0, 0, qtrue  },
	{ &g_skyColorR, "g_skyColorR", "255", 0, 0, qtrue  },
	{ &g_skyColorG, "g_skyColorG", "255", 0, 0, qtrue  },
	{ &g_skyColorB, "g_skyColorB", "255", 0, 0, qtrue  },
	{ &g_skyColorA, "g_skyColorA", "0", 0, 0, qtrue  },
	{ &g_allowprops, "g_allowprops", "1", 0, 0, qtrue  },
	{ &g_allownpc, "g_allownpc", "1", 0, 0, qtrue  },
	{ &g_allowitems, "g_allowitems", "1", 0, 0, qtrue  },
	{ &g_allownoclip, "g_allownoclip", "1", 0, 0, qtrue  },
	{ &g_allowtoolgun, "g_allowtoolgun", "1", 0, 0, qtrue  },
	{ &g_allowphysgun, "g_allowphysgun", "1", 0, 0, qtrue  },
	{ &g_allowgravitygun, "g_allowgravitygun", "1", 0, 0, qtrue  },
	{ &cl_selectedmod, "cl_selectedmod", "default", CVAR_ARCHIVE, 0, qtrue},
	{ &cl_language, "cl_language", "0", CVAR_ARCHIVE, 0, qtrue},
	{ &g_maxEntities, "g_maxEntities", "1024", 0, 0, qtrue},
	{ &g_regenarmor, "g_regenarmor", "0", 0, 0, qtrue  },
	{ &g_movetype, "g_movetype", "0", 0, 0, qtrue  },
	{ &g_accelerate, "g_accelerate", "1", 0, 0, qtrue  },
	{ &g_randomItems, "g_randomItems", "0", 0, 0, qtrue  },
	{ &g_invulinf, "g_invulinf", "0", 0, 0, qtrue  },
	{ &g_kamikazeinf, "g_kamikazeinf", "0", 0, 0, qtrue  },
	{ &g_portalinf, "g_portalinf", "0", 0, 0, qtrue  },
	{ &g_teleporterinf, "g_teleporterinf", "0", 0, 0, qtrue  },
	{ &g_medkitinf, "g_medkitinf", "0", 0, 0, qtrue  },
	{ &g_medkitlimit, "g_medkitlimit", "200", 0, 0, qtrue  },
	{ &g_waterdamage, "g_waterdamage", "0", 0, 0, qtrue  },
	{ &g_lavadamage, "g_lavadamage", "30", 0, 0, qtrue  },
	{ &g_slimedamage, "g_slimedamage", "10", 0, 0, qtrue  },
	{ &g_nextbot_speed, "g_nextbot_speed", "2.00", 0, 0, qtrue  },
	{ &g_randomteleport, "g_randomteleport", "0", 0, 0, qtrue  },
	{ &g_falldamagesmall, "g_falldamagesmall", "5", 0, 0, qtrue  },
	{ &g_falldamagebig, "g_falldamagebig", "10", 0, 0, qtrue  },
	{ &g_noplayerclip, "g_noplayerclip", "0", 0, 0, qtrue  },
	{ &g_flagrespawn, "g_flagrespawn", "-1", 0, 0, qtrue  },
	{ &g_portaltimeout, "g_portaltimeout", "120", 0, 0, qtrue  },
	{ &g_portalhealth, "g_portalhealth", "200", 0, 0, qtrue  },
	{ &g_quadtime, "g_quadtime", "30", 0, 0, qtrue  },
	{ &g_bsuittime, "g_bsuittime", "30", 0, 0, qtrue  },
	{ &g_hastetime, "g_hastetime", "30", 0, 0, qtrue  },
	{ &g_invistime, "g_invistime", "30", 0, 0, qtrue  },
	{ &g_regentime, "g_regentime", "30", 0, 0, qtrue  },
	{ &g_flighttime, "g_flighttime", "60", 0, 0, qtrue  },
	{ &g_invulmove, "g_invulmove", "0", 0, 0, qtrue  },
	{ &g_invultime, "g_invultime", "10", 0, 0, qtrue  },
	{ &g_fasthealthregen, "g_fasthealthregen", "15", 0, 0, qtrue  },
	{ &g_slowhealthregen, "g_slowhealthregen", "5", 0, 0, qtrue  },
	{ &g_hastefirespeed, "g_hastefirespeed", "0.65", 0, 0, qtrue  },
	{ &g_medkitmodifier, "g_medkitmodifier", "100", 0, 0, qtrue  },
	{ &g_armorprotect, "g_armorprotect", "0.66", 0, 0, qtrue  },
	{ &g_respawnwait, "g_respawnwait", "3000", 0, 0, qtrue  },
	{ &g_jumpheight, "g_jumpheight", "270", 0, 0, qtrue  },
	{ &g_speedfactor, "g_speedfactor", "1.3", 0, 0, qtrue  },
	{ &g_drowndamage, "g_drowndamage", "1", 0, 0, qtrue  },
	{ &g_ammolimit, "g_ammolimit", "200", 0, 0, qtrue  },
	{ &g_armorrespawn, "g_armorrespawn", "25", 0, 0, qtrue  },
	{ &g_healthrespawn, "g_healthrespawn", "35", 0, 0, qtrue  },
	{ &g_ammorespawn, "g_ammorespawn", "30", 0, 0, qtrue  },
	{ &g_holdablerespawn, "g_holdablerespawn", "60", 0, 0, qtrue  },
	{ &g_megahealthrespawn, "g_megahealthrespawn", "35", 0, 0, qtrue  },
	{ &g_poweruprespawn, "g_poweruprespawn", "120", 0, 0, qtrue  },
    { &g_elimflags, "elimflags", "0", CVAR_SERVERINFO, 0, qfalse  },
	{ &g_fraglimit, "fraglimit", "20", CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_NORESTART, 0, qtrue },
	{ &g_timelimit, "timelimit", "0", CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_NORESTART, 0, qtrue },
	{ &g_capturelimit, "capturelimit", "8", CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_NORESTART, 0, qtrue },

	{ &g_friendlyFire, "g_friendlyFire", "0", CVAR_ARCHIVE, 0, qtrue  },

	{ &g_teamAutoJoin, "g_teamAutoJoin", "0", CVAR_ARCHIVE  },
	{ &g_teamForceBalance, "g_teamForceBalance", "0", CVAR_ARCHIVE  },

	{ &g_warmup, "g_warmup", "20", CVAR_ARCHIVE, 0, qtrue  },
	{ &g_doWarmup, "g_doWarmup", "0", CVAR_SERVERINFO | CVAR_ARCHIVE, 0, qtrue  },

	{ &g_password, "g_password", "", CVAR_USERINFO, 0, qfalse  },

	{ &g_dedicated, "dedicated", "0", 0, 0, qfalse  },

	{ &g_speed, "g_speed", "320", 0, 0, qtrue  },
	{ &g_gravity, "g_gravity", "800", 0, 0, qtrue  },
	{ &g_gravityModifier, "g_gravityModifier", "1", 0, 0, qtrue  },
        { &g_damageModifier, "g_damageModifier", "1", 0, 0, qtrue },
	{ &g_knockback, "g_knockback", "1000", 0, 0, qtrue  },
	{ &g_quadfactor, "g_quadfactor", "3", 0, 0, qtrue  },
	{ &g_weaponRespawn, "g_weaponrespawn", "5", 0, 0, qtrue  },
	{ &g_weaponTeamRespawn, "g_weaponTeamRespawn", "30", 0, 0, qtrue },
        { &g_respawntime, "g_respawntime", "0", CVAR_ARCHIVE, 0, qtrue },
	{ &g_blood, "com_blood", "1", 0, 0, qfalse },

	{ &g_allowVote, "g_allowVote", "1", CVAR_SERVERINFO | CVAR_ARCHIVE, 0, qfalse },

	{ &g_obeliskHealth, "g_obeliskHealth", "2500", 0, 0, qfalse },
	{ &g_obeliskRegenPeriod, "g_obeliskRegenPeriod", "1", 0, 0, qfalse },
	{ &g_obeliskRegenAmount, "g_obeliskRegenAmount", "15", 0, 0, qfalse },
	{ &g_obeliskRespawnDelay, "g_obeliskRespawnDelay", "10", CVAR_SERVERINFO, 0, qfalse },

	{ &g_cubeTimeout, "g_cubeTimeout", "30", 0, 0, qfalse },

	{ &g_enableDust, "g_enableDust", "0", CVAR_SERVERINFO, 0, qtrue },
	{ &g_enableBreath, "g_enableBreath", "0", CVAR_SERVERINFO, 0, qtrue },
	{ &g_proxMineTimeout, "g_proxMineTimeout", "180000", 0, 0, qfalse },

	{ &g_smoothClients, "g_smoothClients", "1", 0, 0, qfalse},

	{ &sv_fps, "sv_fps", "60", CVAR_SYSTEMINFO | CVAR_ARCHIVE, 0, qfalse },

    { &g_music, "g_music", "", 0, 0, qfalse},
    { &g_spawnprotect, "g_spawnprotect", "500", CVAR_ARCHIVE | CVAR_NORESTART, 0, qtrue},

	//Settings for elimination:
	{ &g_elimination_selfdamage, "g_elimination_selfdamage", "0", 0, 0, qtrue },
	{ &g_elimination_roundtime, "g_elimination_roundtime", "600", CVAR_SERVERINFO | CVAR_NORESTART, 0, qtrue },
	{ &g_elimination_warmup, "g_elimination_warmup", "0", CVAR_NORESTART , 0, qtrue },
	{ &g_elimination_activewarmup, "g_elimination_activewarmup", "0", CVAR_NORESTART , 0, qtrue },
    { &g_elimination, "g_elimination", "0", CVAR_NORESTART, 0, qfalse },
	{ &g_elimination_items, "g_elimination_items", "0", CVAR_LATCH, 0, qtrue },
	{ &g_elimination_lockspectator, "g_elimination_lockspectator", "0", CVAR_NORESTART, 0, qtrue },
	{ &g_elimination_ctf_oneway, "g_elimination_ctf_oneway", "0", CVAR_NORESTART, 0, qtrue },
	{ &g_elimination_red_respawn, "g_elimination_red_respawn", "0", 0, 0, qtrue  },
	{ &g_elimination_blue_respawn, "g_elimination_blue_respawn", "0", 0, 0, qtrue  },

	//Spawn settings for blue and white:
	{ &g_bluespawn_health, "g_bluespawn_health", "100", CVAR_NORESTART, 0, qtrue },
	{ &g_bluespawn_armor, "g_bluespawn_armor", "0", CVAR_NORESTART, 0, qtrue },
	{ &g_bluespawn_gauntlet, "g_bluespawn_gauntlet", "1", CVAR_NORESTART, 0, qtrue },
	{ &g_bluespawn_machinegun, "g_bluespawn_machinegun", "500", CVAR_NORESTART, 0, qtrue },
	{ &g_bluespawn_shotgun, "g_bluespawn_shotgun", "0", CVAR_NORESTART, 0, qtrue },
	{ &g_bluespawn_grenade, "g_bluespawn_grenade", "0", CVAR_NORESTART, 0, qtrue },
	{ &g_bluespawn_rocket, "g_bluespawn_rocket", "0", CVAR_NORESTART, 0, qtrue },
	{ &g_bluespawn_lightning, "g_bluespawn_lightning", "0",  CVAR_NORESTART, 0, qtrue },
	{ &g_bluespawn_railgun, "g_bluespawn_railgun", "0", CVAR_NORESTART, 0, qtrue },
	{ &g_bluespawn_plasmagun, "g_bluespawn_plasmagun", "0", CVAR_NORESTART, 0, qtrue },
	{ &g_bluespawn_bfg, "g_bluespawn_bfg", "0", CVAR_NORESTART, 0, qtrue },
    { &g_bluespawn_grapple, "g_bluespawn_grapple", "0", CVAR_NORESTART, 0, qtrue },
	{ &g_bluespawn_nail, "g_bluespawn_nail", "0", CVAR_NORESTART, 0, qtrue },
	{ &g_bluespawn_mine, "g_bluespawn_mine", "0",  CVAR_NORESTART, 0, qtrue },
	{ &g_bluespawn_chain, "g_bluespawn_chain", "0", CVAR_NORESTART, 0, qtrue },
	{ &g_bluespawn_flame, "g_bluespawn_flame", "0",  CVAR_NORESTART, 0, qtrue },
	{ &g_bluespawn_antimatter, "g_bluespawn_antimatter", "0", CVAR_NORESTART, 0, qtrue },

	{ &g_bluespawn_quad, "g_bluespawn_quad", "0", CVAR_NORESTART, 0, qtrue },
	{ &g_bluespawn_haste, "g_bluespawn_haste", "0", CVAR_NORESTART, 0, qtrue },
	{ &g_bluespawn_bsuit, "g_bluespawn_bsuit", "0", CVAR_NORESTART, 0, qtrue },
	{ &g_bluespawn_invis, "g_bluespawn_invis", "0", CVAR_NORESTART, 0, qtrue },
	{ &g_bluespawn_regen, "g_bluespawn_regen", "0", CVAR_NORESTART, 0, qtrue },
	{ &g_bluespawn_flight, "g_bluespawn_flight", "0", CVAR_NORESTART, 0, qtrue },

	{ &g_bluespawn_holdable, "g_bluespawn_holdable", "0", CVAR_LATCH, 0, qtrue },

	//Spawn settings for red:
	{ &g_redspawn_health, "g_redspawn_health", "100", CVAR_NORESTART, 0, qtrue },
	{ &g_redspawn_armor, "g_redspawn_armor", "0", CVAR_NORESTART, 0, qtrue },
	{ &g_redspawn_gauntlet, "g_redspawn_gauntlet", "1", CVAR_NORESTART, 0, qtrue },
	{ &g_redspawn_machinegun, "g_redspawn_machinegun", "500", CVAR_NORESTART, 0, qtrue },
	{ &g_redspawn_shotgun, "g_redspawn_shotgun", "0", CVAR_NORESTART, 0, qtrue },
	{ &g_redspawn_grenade, "g_redspawn_grenade", "0", CVAR_NORESTART, 0, qtrue },
	{ &g_redspawn_rocket, "g_redspawn_rocket", "0", CVAR_NORESTART, 0, qtrue },
	{ &g_redspawn_lightning, "g_redspawn_lightning", "0",  CVAR_NORESTART, 0, qtrue },
	{ &g_redspawn_railgun, "g_redspawn_railgun", "0", CVAR_NORESTART, 0, qtrue },
	{ &g_redspawn_plasmagun, "g_redspawn_plasmagun", "0", CVAR_NORESTART, 0, qtrue },
	{ &g_redspawn_bfg, "g_redspawn_bfg", "0", CVAR_NORESTART, 0, qtrue },
    { &g_redspawn_grapple, "g_redspawn_grapple", "0", CVAR_NORESTART, 0, qtrue },
	{ &g_redspawn_nail, "g_redspawn_nail", "0", CVAR_NORESTART, 0, qtrue },
	{ &g_redspawn_mine, "g_redspawn_mine", "0",  CVAR_NORESTART, 0, qtrue },
	{ &g_redspawn_chain, "g_redspawn_chain", "0", CVAR_NORESTART, 0, qtrue },
	{ &g_redspawn_flame, "g_redspawn_flame", "0",  CVAR_NORESTART, 0, qtrue },
	{ &g_redspawn_antimatter, "g_redspawn_antimatter", "0", CVAR_NORESTART, 0, qtrue },

	{ &g_redspawn_quad, "g_redspawn_quad", "0", CVAR_NORESTART, 0, qtrue },
	{ &g_redspawn_haste, "g_redspawn_haste", "0", CVAR_NORESTART, 0, qtrue },
	{ &g_redspawn_bsuit, "g_redspawn_bsuit", "0", CVAR_NORESTART, 0, qtrue },
	{ &g_redspawn_invis, "g_redspawn_invis", "0", CVAR_NORESTART, 0, qtrue },
	{ &g_redspawn_regen, "g_redspawn_regen", "0", CVAR_NORESTART, 0, qtrue },
	{ &g_redspawn_flight, "g_redspawn_flight", "0", CVAR_NORESTART, 0, qtrue },

	{ &g_redspawn_holdable, "g_redspawn_holdable", "0", CVAR_LATCH, 0, qtrue },

	{ &g_vampire, "g_vampire", "0.0", CVAR_NORESTART, 0, qtrue },
	{ &g_regen, "g_regen", "0", CVAR_NORESTART, 0, qtrue },
	{ &g_vampireMaxHealth, "g_vampire_max_health", "500", CVAR_NORESTART, 0, qtrue },
	{ &g_lms_lives, "g_lms_lives", "1", CVAR_NORESTART, 0, qtrue },
	{ &g_lms_mode, "g_lms_mode", "0", CVAR_SERVERINFO | CVAR_ARCHIVE | CVAR_NORESTART, 0, qtrue }
};

// bk001129 - made static to avoid aliasing
static int gameCvarTableSize = sizeof( gameCvarTable ) / sizeof( gameCvarTable[0] );


void G_InitGame( int levelTime, int randomSeed, int restart );
void G_RunFrame( int levelTime );
void G_ShutdownGame( int restart );
void CheckExitRules( void );


/*
================
vmMain

This is the only way control passes into the module.
This must be the very first function compiled into the .q3vm file
================
*/
intptr_t vmMain( int command, int arg0, int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int arg7, int arg8, int arg9, int arg10, int arg11  ) {
	switch ( command ) {
	case GAME_INIT:
		G_InitGame( arg0, arg1, arg2 );
		return 0;
	case GAME_SHUTDOWN:
		G_ShutdownGame( arg0 );
		return 0;
	case GAME_CLIENT_CONNECT:
		return (intptr_t)ClientConnect( arg0, arg1, arg2 );
	case GAME_CLIENT_THINK:
		ClientThink( arg0 );
		return 0;
	case GAME_CLIENT_USERINFO_CHANGED:
		ClientUserinfoChanged( arg0 );
		return 0;
	case GAME_CLIENT_DISCONNECT:
		ClientDisconnect( arg0 );
		return 0;
	case GAME_CLIENT_BEGIN:
		ClientBegin( arg0 );
		return 0;
	case GAME_CLIENT_COMMAND:
		ClientCommand( arg0 );
		return 0;
	case GAME_RUN_FRAME:
		G_RunFrame( arg0 );
		return 0;
	case GAME_CONSOLE_COMMAND:
		return ConsoleCommand();
	case BOTAI_START_FRAME:
		return BotAIStartFrame( arg0 );
	}

	return -1;
}


void QDECL G_Printf( const char *fmt, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, fmt);
	Q_vsnprintf (text, sizeof(text), fmt, argptr);
	va_end (argptr);

	trap_Printf( text );
}

void QDECL G_Error( const char *fmt, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, fmt);
	Q_vsnprintf (text, sizeof(text), fmt, argptr);
	va_end (argptr);

	trap_Error( text );
}

/*
================
G_FindTeams

Chain together all entities with a matching team field.
Entity teams are used for item groups and multi-entity mover groups.

All but the first will have the FL_TEAMSLAVE flag set and teammaster field set
All but the last will have the teamchain field set to the next one
================
*/
void G_FindTeams( void ) {
	gentity_t	*e, *e2;
	int		i, j;
	int		c, c2;

	c = 0;
	c2 = 0;
	for ( i=1, e=g_entities+i ; i < level.num_entities ; i++,e++ ){
		if (!e->inuse)
			continue;
		if (!e->team)
			continue;
		if (e->flags & FL_TEAMSLAVE)
			continue;
		e->teammaster = e;
		c++;
		c2++;
		for (j=i+1, e2=e+1 ; j < level.num_entities ; j++,e2++)
		{
			if (!e2->inuse)
				continue;
			if (!e2->team)
				continue;
			if (e2->flags & FL_TEAMSLAVE)
				continue;
			if (!strcmp(e->team, e2->team))
			{
				c2++;
				e2->teamchain = e->teamchain;
				e->teamchain = e2;
				e2->teammaster = e;
				e2->flags |= FL_TEAMSLAVE;

				// make sure that targets only point at the master
				if ( e2->targetname ) {
					e->targetname = e2->targetname;
					e2->targetname = NULL;
				}
			}
		}
	}
        G_Printf ("%i teams with %i entities\n", c, c2);
}

/*
=================
G_RegisterCvars
=================
*/
void G_RegisterCvars( void ) {
	int			i;
	cvarTable_t	*cv;

	for ( i = 0, cv = gameCvarTable ; i < gameCvarTableSize ; i++, cv++ ) {
		trap_Cvar_Register( cv->vmCvar, cv->cvarName,
			cv->defaultString, cv->cvarFlags );
		if ( cv->vmCvar )
			cv->modificationCount = cv->vmCvar->modificationCount;
	}

	// check some things
	if ( g_gametype.integer < 0 || g_gametype.integer >= GT_MAX_GAME_TYPE ) {
                G_Printf( "g_gametype %i is out of range, defaulting to 0\n", g_gametype.integer );
		trap_Cvar_Set( "g_gametype", "0" );
	}

	//set FFA status for high gametypes:
	if ( g_gametype.integer == GT_LMS ) {
		g_ffa_gt = 1;	//Last Man standig is a FFA gametype
	} else {
		g_ffa_gt = 0;	//If >GT_CTF use bases
	}

	level.warmupModificationCount = g_warmup.modificationCount;
}

/*
=================
G_UpdateCvars
=================
*/
void G_UpdateCvars( void ) {
	int			i;
	cvarTable_t	*cv;
	qboolean remapped = qfalse;

	for ( i = 0, cv = gameCvarTable ; i < gameCvarTableSize ; i++, cv++ ) {
		if ( cv->vmCvar ) {
			trap_Cvar_Update( cv->vmCvar );

			if ( cv->modificationCount != cv->vmCvar->modificationCount ) {
				cv->modificationCount = cv->vmCvar->modificationCount;
			}
		}
	}
}

/*
============
G_InitGame

============
*/
void G_InitGame( int levelTime, int randomSeed, int restart ) {
	int					i;
	vec4_t				rgba;
	char mapname[64];

	trap_Cvar_VariableStringBuffer("mapname", mapname, 64);

	//clear any fades set by target_effect
	for (i = 0; i < 4; i++) 
	rgba[i] = 0;
	G_Fade(0, rgba, rgba, -1);

    G_Printf ("------- Game Initialization -------\n");
    G_Printf ("gamename: %s\n", GAME_VERSION);
    G_Printf ("gamedate: %s\n", __DATE__);

	srand( randomSeed );

	G_RegisterCvars();

    //KK-OAX Changed to Tremulous's BG_InitMemory
	BG_InitMemory();

	// set some level globals
	memset( &level, 0, sizeof( level ) );

	level.time = levelTime;
	level.startTime = levelTime;

	level.snd_fry = G_SoundIndex("sound/player/fry.wav");	// FIXME standing in lava / slime

	G_InitWorldSession();

	// initialize all entities for this game
	memset( g_entities, 0, MAX_GENTITIES * sizeof(g_entities[0]) );
	level.gentities = g_entities;

	// initialize all clients for this game
	level.maxclients = MAX_CLIENTS;
	memset( g_clients, 0, MAX_CLIENTS * sizeof(g_clients[0]) );
	level.clients = g_clients;

	// set client fields on player ents
	for ( i=0 ; i<level.maxclients ; i++ ) {
		g_entities[i].client = level.clients + i;
	}

	// always leave room for the max number of clients,
	// even if they aren't all used, so numbers inside that
	// range are NEVER anything but clients
	level.num_entities = MAX_CLIENTS;

    for ( i=0 ; i<MAX_CLIENTS ; i++ ) {
        g_entities[i].classname = "clientslot";
    }

	// let the server system know where the entites are
	trap_LocateGameData( level.gentities, level.num_entities, sizeof( gentity_t ),
		&level.clients[0].ps, sizeof( level.clients[0] ) );

	// reserve some spots for dead player bodies
	InitBodyQue();

	ClearRegisteredItems();

	// parse the key/value pairs and spawn gentities
	G_SpawnEntitiesFromString();

	// general initialization
	G_FindTeams();

	// make sure we have flags for CTF, etc
	if( g_gametype.integer >= GT_TEAM && (g_ffa_gt!=1)) {
		G_CheckTeamItems();
	}

	SaveRegisteredItems();

    G_Printf ("-----------------------------------\n");

	if ( trap_Cvar_VariableIntegerValue( "bot_enable" ) ) {
		BotAISetup( restart );
		BotAILoadMap( restart );
		G_LoadBots();
	}

	NS_OpenScript("nscript/game/init.ns", NULL, 0);		//Noire.Script Init in qagame.qvm

	//elimination:
	level.roundNumber = 1;
	level.roundNumberStarted = 0;
	level.roundStartTime = level.time+g_elimination_warmup.integer*1000;
	level.roundRespawned = qfalse;
	level.eliminationSides = rand()%2; //0 or 1

	if(g_gametype.integer == GT_DOUBLE_D)
		Team_SpawnDoubleDominationPoints();

	if(g_gametype.integer == GT_DOMINATION ){
		level.dom_scoreGiven = 0;
		for(i=0;i<MAX_DOMINATION_POINTS;i++)
			level.pointStatusDom[i] = TEAM_NONE;

		level.domination_points_count = 0; //make sure its not too big
	}

    PlayerStoreInit();

	if(strlen(g_entitypack.string)){
		trap_SendConsoleCommand( EXEC_APPEND, va("loadmap maps/%s/%s.add \n", g_entitypack.string, mapname) );	//load map file
	}
}



/*
=================
G_ShutdownGame
=================
*/
void G_ShutdownGame( int restart ) {
	int i;

    G_Printf ("==== ShutdownGame ====\n");

	//drop all bots from game in single player
	for (i = 0; i < MAX_CLIENTS; i++ ) {
		if ( g_entities[i].singlebot >= 1 ) {
			DropClientSilently( g_entities[i].client->ps.clientNum );
		}
	}

	// write all the client session data so we can get it back
	G_WriteSessionData();

	if ( trap_Cvar_VariableIntegerValue( "bot_enable" ) ) {
		BotAIShutdown( restart );
	}
}

qboolean G_NpcFactionProp(int prop, gentity_t* ent){
switch(prop) {
	case NP_PICKUP:{
		switch(ent->singlebot) {
			case NPC_PLAYER: 	return qtrue;
			case NPC_ENEMY: 	return qfalse;
			case NPC_CITIZEN: 	return qfalse;
			case NPC_GUARD: 	return qfalse;
			case NPC_PARTNER: 	return qtrue;
			case NPC_PARTNERENEMY: 	return qtrue;
		}	
	break;}
	
	case NP_HARM:{
		switch(ent->singlebot) {
			case NPC_PLAYER: 	return qtrue;
			case NPC_ENEMY: 	return qfalse;
			case NPC_CITIZEN: 	return qfalse;
			case NPC_GUARD: 	return qfalse;
			case NPC_PARTNER: 	return qtrue;
			case NPC_PARTNERENEMY: 	return qtrue;
		}	
	break;}
}
return qfalse;
}

void QDECL Com_Error ( int level, const char *error, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, error);
	Q_vsnprintf (text, sizeof(text), error, argptr);
	va_end (argptr);

	G_Error( "%s", text);
}

void QDECL Com_Printf( const char *msg, ... ) {
	va_list		argptr;
	char		text[1024];

	va_start (argptr, msg);
	Q_vsnprintf (text, sizeof(text), msg, argptr);
	va_end (argptr);

        G_Printf ("%s", text);
}

/*
========================================================================

PLAYER COUNTING / SCORE SORTING

========================================================================
*/

/*
=============
AddTournamentPlayer

If there are less than two tournament players, put a
spectator in the game and restart
=============
*/
void AddTournamentPlayer( void ) {
	int			i;
	gclient_t	*client;
	gclient_t	*nextInLine;

	if ( level.numPlayingClients >= 2 ) {
		return;
	}

	// never change during intermission
	if ( level.intermissiontime ) {
		return;
	}

	nextInLine = NULL;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		client = &level.clients[i];
		if ( client->pers.connected != CON_CONNECTED ) {
			continue;
		}
		if ( client->sess.sessionTeam != TEAM_SPECTATOR ) {
			continue;
		}
		// never select the dedicated follow or scoreboard clients
		if ( client->sess.spectatorState == SPECTATOR_SCOREBOARD ||
			client->sess.spectatorClient < 0  ) {
			continue;
		}

		if(!nextInLine || client->sess.spectatorNum > nextInLine->sess.spectatorNum) {
			nextInLine = client;
		}
	}

	if ( !nextInLine ) {
		return;
	}

	level.warmupTime = -1;

	// set them to free-for-all team
	SetTeam( &g_entities[ nextInLine - level.clients ], "f" );
}

/*
=======================
AddTournamentQueue

Add client to end of tournament queue
=======================
*/
void AddTournamentQueue(gclient_t *client)
{
    int index;
    gclient_t *curclient;
    for(index = 0; index < level.maxclients; index++)
    {
        curclient = &level.clients[index];
        if(curclient->pers.connected != CON_DISCONNECTED)
        {
            if(curclient == client)
            curclient->sess.spectatorNum = 0;
            else if(curclient->sess.sessionTeam == TEAM_SPECTATOR)
            curclient->sess.spectatorNum++;
        }
    }
}

/*
=======================
RemoveTournamentLoser

Make the loser a spectator at the back of the line
=======================
*/
void RemoveTournamentLoser( void ) {
	int			clientNum;

	if ( level.numPlayingClients != 2 ) {
		return;
	}

	clientNum = level.sortedClients[1];

	if ( level.clients[ clientNum ].pers.connected != CON_CONNECTED ) {
		return;
	}

	// make them a spectator
	SetTeam( &g_entities[ clientNum ], "s" );
}

/*
=======================
RemoveTournamentWinner
=======================
*/
void RemoveTournamentWinner( void ) {
	int			clientNum;

	if ( level.numPlayingClients != 2 ) {
		return;
	}

	clientNum = level.sortedClients[0];

	if ( level.clients[ clientNum ].pers.connected != CON_CONNECTED ) {
		return;
	}

	// make them a spectator
	SetTeam( &g_entities[ clientNum ], "s" );
}

/*
=======================
AdjustTournamentScores
=======================
*/
void AdjustTournamentScores( void ) {
	int			clientNum;

	clientNum = level.sortedClients[0];
	if ( level.clients[ clientNum ].pers.connected == CON_CONNECTED ) {
		level.clients[ clientNum ].sess.wins++;
		ClientUserinfoChanged( clientNum );
	}

	clientNum = level.sortedClients[1];
	if ( level.clients[ clientNum ].pers.connected == CON_CONNECTED ) {
		level.clients[ clientNum ].sess.losses++;
		ClientUserinfoChanged( clientNum );
	}

}

/*
=============
SortRanks

=============
*/
int QDECL SortRanks( const void *a, const void *b ) {
	gclient_t	*ca, *cb;

	ca = &level.clients[*(int *)a];
	cb = &level.clients[*(int *)b];

	// sort special clients last
	if ( ca->sess.spectatorState == SPECTATOR_SCOREBOARD || ca->sess.spectatorClient < 0 ) {
		return 1;
	}
	if ( cb->sess.spectatorState == SPECTATOR_SCOREBOARD || cb->sess.spectatorClient < 0  ) {
		return -1;
	}

	// then connecting clients
	if ( ca->pers.connected == CON_CONNECTING ) {
		return 1;
	}
	if ( cb->pers.connected == CON_CONNECTING ) {
		return -1;
	}


	// then spectators
	if ( ca->sess.sessionTeam == TEAM_SPECTATOR && cb->sess.sessionTeam == TEAM_SPECTATOR ) {
		if ( ca->sess.spectatorNum > cb->sess.spectatorNum ) {
			return -1;
		}
		if ( ca->sess.spectatorNum < cb->sess.spectatorNum ) {
			return 1;
		}
		return 0;
	}
	if ( ca->sess.sessionTeam == TEAM_SPECTATOR ) {
		return 1;
	}
	if ( cb->sess.sessionTeam == TEAM_SPECTATOR ) {
		return -1;
	}

        //In elimination and CTF elimination, sort dead players last
        if((g_gametype.integer == GT_ELIMINATION || g_gametype.integer == GT_CTF_ELIMINATION)
                && level.roundNumber==level.roundNumberStarted && (ca->isEliminated != cb->isEliminated)) {
            if( ca->isEliminated )
                return 1;
            if( cb->isEliminated )
                return -1;
        }

	// then sort by score
	if ( ca->ps.persistant[PERS_SCORE]
		> cb->ps.persistant[PERS_SCORE] ) {
		return -1;
	}
	if ( ca->ps.persistant[PERS_SCORE]
		< cb->ps.persistant[PERS_SCORE] ) {
		return 1;
	}
	return 0;
}

/*
============
CalculateRanks

Recalculates the score ranks of all players
This will be called on every client connect, begin, disconnect, death,
and team change.
============
*/
void CalculateRanks( void ) {
	int		i;
	int		rank;
	int		score;
	int		newScore;
	gclient_t	*cl;

	level.follow1 = -1;
	level.follow2 = -1;
	level.numConnectedClients = 0;
	level.numNonSpectatorClients = 0;
	level.numPlayingClients = 0;
	level.numVotingClients = 0;		// don't count bots
	for ( i = 0; i < TEAM_NUM_TEAMS; i++ ) {
		level.numteamVotingClients[i] = 0;
	}
	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if ( level.clients[i].pers.connected != CON_DISCONNECTED ) {
			level.sortedClients[level.numConnectedClients] = i;
			level.numConnectedClients++;

			if ( level.clients[i].sess.sessionTeam != TEAM_SPECTATOR ) {
				level.numNonSpectatorClients++;
			
				// decide if this should be auto-followed
				if ( level.clients[i].pers.connected == CON_CONNECTED ) {
					level.numPlayingClients++;
					if ( !(g_entities[i].r.svFlags & SVF_BOT) ) {
						level.numVotingClients++;
						if ( level.clients[i].sess.sessionTeam == TEAM_RED )
							level.numteamVotingClients[0]++;
						else if ( level.clients[i].sess.sessionTeam == TEAM_BLUE )
							level.numteamVotingClients[1]++;
					}
					if ( level.follow1 == -1 ) {
						level.follow1 = i;
					} else if ( level.follow2 == -1 ) {
						level.follow2 = i;
					}
				}
			}
		}
	}

	qsort( level.sortedClients, level.numConnectedClients, 
		sizeof(level.sortedClients[0]), SortRanks );

	// set the rank value for all clients that are connected and not spectators
	if ( g_gametype.integer >= GT_TEAM && g_ffa_gt!=1 ) {
		// in team games, rank is just the order of the teams, 0=red, 1=blue, 2=tied
		for ( i = 0;  i < level.numConnectedClients; i++ ) {
			cl = &level.clients[ level.sortedClients[i] ];
			if ( level.teamScores[TEAM_RED] == level.teamScores[TEAM_BLUE] ) {
				cl->ps.persistant[PERS_RANK] = 2;
			} else if ( level.teamScores[TEAM_RED] > level.teamScores[TEAM_BLUE] ) {
				cl->ps.persistant[PERS_RANK] = 0;
			} else {
				cl->ps.persistant[PERS_RANK] = 1;
			}
		}
	} else {	
		rank = -1;
		score = 0;
		for ( i = 0;  i < level.numPlayingClients; i++ ) {
			cl = &level.clients[ level.sortedClients[i] ];
			newScore = cl->ps.persistant[PERS_SCORE];
			if ( i == 0 || newScore != score ) {
				rank = i;
				// assume we aren't tied until the next client is checked
				level.clients[ level.sortedClients[i] ].ps.persistant[PERS_RANK] = rank;
			} else {
				// we are tied with the previous client
				level.clients[ level.sortedClients[i-1] ].ps.persistant[PERS_RANK] = rank | RANK_TIED_FLAG;
				level.clients[ level.sortedClients[i] ].ps.persistant[PERS_RANK] = rank | RANK_TIED_FLAG;
			}
			score = newScore;
			if ( g_gametype.integer == GT_SINGLE && level.numPlayingClients == 1 ) {
				level.clients[ level.sortedClients[i] ].ps.persistant[PERS_RANK] = rank | RANK_TIED_FLAG;
			}
		}
	}

	// set the CS_SCORES1/2 configstrings, which will be visible to everyone
	if ( g_gametype.integer >= GT_TEAM && g_ffa_gt!=1 ) {
		trap_SetConfigstring( CS_SCORES1, va("%i", level.teamScores[TEAM_RED] ) );
		trap_SetConfigstring( CS_SCORES2, va("%i", level.teamScores[TEAM_BLUE] ) );
	} else {
		if ( level.numConnectedClients == 0 ) {
			trap_SetConfigstring( CS_SCORES1, va("%i", SCORE_NOT_PRESENT) );
			trap_SetConfigstring( CS_SCORES2, va("%i", SCORE_NOT_PRESENT) );
		} else if ( level.numConnectedClients == 1 ) {
			trap_SetConfigstring( CS_SCORES1, va("%i", level.clients[ level.sortedClients[0] ].ps.persistant[PERS_SCORE] ) );
			trap_SetConfigstring( CS_SCORES2, va("%i", SCORE_NOT_PRESENT) );
		} else {
			trap_SetConfigstring( CS_SCORES1, va("%i", level.clients[ level.sortedClients[0] ].ps.persistant[PERS_SCORE] ) );
			trap_SetConfigstring( CS_SCORES2, va("%i", level.clients[ level.sortedClients[1] ].ps.persistant[PERS_SCORE] ) );
		}
	}

	// see if it is time to end the level
	CheckExitRules();

	// if we are at the intermission, send the new info to everyone
	if ( level.intermissiontime ) {
		SendScoreboardMessageToAllClients();
	}
}

/*
========================================================================

MAP CHANGING

========================================================================
*/

/*
========================
SendScoreboardMessageToAllClients

Do this at BeginIntermission time and whenever ranks are recalculated
due to enters/exits/forced team changes
========================
*/
void SendScoreboardMessageToAllClients( void ) {
	int		i;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if ( level.clients[ i ].pers.connected == CON_CONNECTED ) {
			DeathmatchScoreboardMessage( g_entities + i );
			EliminationMessage( g_entities + i );
		}
	}
}

/*
========================
SendElimiantionMessageToAllClients

Used to send information important to Elimination
========================
*/
void SendEliminationMessageToAllClients( void ) {
	int		i;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if ( level.clients[ i ].pers.connected == CON_CONNECTED ) {
			EliminationMessage( g_entities + i );
		}
	}
}

/*
========================
SendDDtimetakenMessageToAllClients

Do this if a team just started dominating.
========================
*/
void SendDDtimetakenMessageToAllClients( void ) {
	int		i;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if ( level.clients[ i ].pers.connected == CON_CONNECTED ) {
			DoubleDominationScoreTimeMessage( g_entities + i );
		}
	}
}

/*
========================
SendAttackingTeamMessageToAllClients

Used for CTF Elimination oneway
========================
*/
void SendAttackingTeamMessageToAllClients( void ) {
	int		i;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if ( level.clients[ i ].pers.connected == CON_CONNECTED ) {
			AttackingTeamMessage( g_entities + i );
		}
	}
}

/*
========================
SendDominationPointsStatusMessageToAllClients

Used for Standard domination
========================
*/
void SendDominationPointsStatusMessageToAllClients( void ) {
	int		i;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if ( level.clients[ i ].pers.connected == CON_CONNECTED ) {
			DominationPointStatusMessage( g_entities + i );
		}
	}
}

/*
========================
MoveClientToIntermission

When the intermission starts, this will be called for all players.
If a new client connects, this will be called after the spawn function.
========================
*/
void MoveClientToIntermission( gentity_t *ent ) {
	// take out of follow mode if needed
	if ( ent->client->sess.spectatorState == SPECTATOR_FOLLOW ) {
		StopFollowing( ent );
	}

	FindIntermissionPoint();
	// move to the spot
	VectorCopy( level.intermission_origin, ent->s.origin );
	VectorCopy( level.intermission_origin, ent->client->ps.origin );
	VectorCopy (level.intermission_angle, ent->client->ps.viewangles);
	ent->client->ps.pm_type = PM_INTERMISSION;

	// clean up powerup info
	memset( ent->client->ps.powerups, 0, sizeof(ent->client->ps.powerups) );

	ent->client->ps.eFlags = 0;
	ent->s.eFlags = 0;
	ent->s.eType = ET_GENERAL;
	ent->s.modelindex = 0;
	ent->s.loopSound = 0;
	ent->s.event = 0;
	ent->r.contents = 0;
}

/*
==================
FindIntermissionPoint

This is also used for spectator spawns
==================
*/
void FindIntermissionPoint( void ) {
	gentity_t	*ent, *target;
	vec3_t		dir;

	// find the intermission spot
	ent = G_Find (NULL, FOFS(classname), "info_player_intermission");
	if ( !ent ) {	// the map creator forgot to put in an intermission point...
		SelectSpawnPoint ( vec3_origin, level.intermission_origin, level.intermission_angle );
	} else {
		VectorCopy (ent->s.origin, level.intermission_origin);
		VectorCopy (ent->s.angles, level.intermission_angle);
		// if it has a target, look towards it
		if ( ent->target ) {
			target = G_PickTarget( ent->target );
			if ( target ) {
				VectorSubtract( target->s.origin, level.intermission_origin, dir );
				vectoangles( dir, level.intermission_angle );
			}
		}
	}

}

/*
==================
BeginIntermission
==================
*/
void BeginIntermission( void ) {
	int			i;
	gentity_t	*client;

	if ( level.intermissiontime ) {
		return;		// already active
	}

	// if in tournement mode, change the wins / losses
	if ( g_gametype.integer == GT_TOURNAMENT ) {
		AdjustTournamentScores();
	}

	level.intermissiontime = level.time;
	// move all clients to the intermission point
	for (i=0 ; i< level.maxclients ; i++) {
		client = g_entities + i;
		if (!client->inuse)
			continue;
		// respawn if dead
		if (client->health <= 0) {
			ClientRespawn(client);
		}
		MoveClientToIntermission( client );
	}

	// send the current scoring to all clients
	SendScoreboardMessageToAllClients();

}

/*
=============
ExitLevel

When the intermission has been exited, the server is either killed
or moved to a new level based on the "nextmap" cvar

=============
*/
void ExitLevel (void) {
	int		 	i;
	gclient_t 	*cl;

	// if we are running a tournement map, kick the loser to spectator status,
	// which will automatically grab the next spectator and restart
	if ( g_gametype.integer == GT_TOURNAMENT  ) {
		if ( !level.restarted ) {
			RemoveTournamentLoser();
			trap_SendConsoleCommand( EXEC_APPEND, "map_restart 0\n" );
			level.restarted = qtrue;
			level.intermissiontime = 0;
		}
		return;
	}

	trap_SendConsoleCommand( EXEC_APPEND, "vstr nextmap\n" );
	level.intermissiontime = 0;

	// reset all the scores so we don't enter the intermission again
	level.teamScores[TEAM_RED] = 0;
	level.teamScores[TEAM_BLUE] = 0;
	for ( i=0 ; i< MAX_CLIENTS; i++ ) {
		cl = level.clients + i;
		if ( cl->pers.connected != CON_CONNECTED ) {
			continue;
		}
		cl->ps.persistant[PERS_SCORE] = 0;
	}

	// we need to do this here before chaning to CON_CONNECTING
	G_WriteSessionData();

	// change all client states to connecting, so the early players into the
	// next level will know the others aren't done reconnecting
	for (i=0 ; i< MAX_CLIENTS; i++) {
		if ( level.clients[i].pers.connected == CON_CONNECTED ) {
			level.clients[i].pers.connected = CON_CONNECTING;
		}
	}

}

/*
=================
CheckIntermissionExit

The level will stay at the intermission for a minimum of 5 seconds
If all players wish to continue, the level will then exit.
If one or more players have not acknowledged the continue, the game will
0 seconds before going on.
=================
*/
void CheckIntermissionExit( void ) {
	int			ready, notReady, playerCount;
	int			i;
	gclient_t	*cl;
	int			readyMask;

	// see which players are ready
	ready = 0;
	notReady = 0;
	readyMask = 0;
        playerCount = 0;
	for (i=0 ; i< MAX_CLIENTS; i++) {
		cl = level.clients + i;
		if ( cl->pers.connected != CON_CONNECTED ) {
			continue;
		}
		if ( g_entities[cl->ps.clientNum].r.svFlags & SVF_BOT ) {
			continue;
		}

                playerCount++;
		if ( cl->readyToExit ) {
			ready++;
			if ( i < 16 ) {
				readyMask |= 1 << i;
			}
		} else {
			notReady++;
		}
	}

	// copy the readyMask to each player's stats so
	// it can be displayed on the scoreboard
	for (i=0 ; i< MAX_CLIENTS; i++) {
		cl = level.clients + i;
		if ( cl->pers.connected != CON_CONNECTED ) {
			continue;
		}
		cl->ps.stats[STAT_CLIENTS_READY] = readyMask;
	}

	// never exit in less than five seconds
	if ( level.time < level.intermissiontime + 5000 ) {
		return;
	}

	// only test ready status when there are real players present
	if ( playerCount > 0 ) {
		// if nobody wants to go, clear timer
		if ( !ready ) {
			level.readyToExit = qfalse;
			return;
		}

		// if everyone wants to go, go now
		if ( !notReady ) {
			ExitLevel();
			return;
		}
	}

	// the first person to ready starts the ten second timeout
	if ( !level.readyToExit ) {
		level.readyToExit = qtrue;
		level.exitTime = level.time;
	}

	// if we have waited ten seconds since at least one player
	// wanted to exit, go ahead
	if ( level.time < level.exitTime + 10000 ) {
		return;
	}

	ExitLevel();
}

/*
=============
ScoreIsTied
=============
*/
qboolean ScoreIsTied( void ) {
	int		a, b;

	if ( level.numPlayingClients < 2 ) {
		return qfalse;
	}

        //Sago: In Elimination and Oneway Flag Capture teams must win by two points.
        if ( g_gametype.integer == GT_ELIMINATION ||
                (g_gametype.integer == GT_CTF_ELIMINATION && g_elimination_ctf_oneway.integer)) {
            return (level.teamScores[TEAM_RED] == level.teamScores[TEAM_BLUE]);
        }

	if ( g_gametype.integer >= GT_TEAM && g_ffa_gt!=1) {
		return level.teamScores[TEAM_RED] == level.teamScores[TEAM_BLUE];
	}

	a = level.clients[level.sortedClients[0]].ps.persistant[PERS_SCORE];
	b = level.clients[level.sortedClients[1]].ps.persistant[PERS_SCORE];

	return a == b;
}

/*
=================
CheckExitRules

There will be a delay between the time the exit is qualified for
and the time everyone is moved to the intermission spot, so you
can see the last frag.
=================
*/
void CheckExitRules( void ) {
 	int			i;
	gclient_t	*cl;
	// if at the intermission, wait for all non-bots to
	// signal ready, then go to next level
	if ( level.intermissiontime ) {
		CheckIntermissionExit ();
		return;
	} else {
            //sago: Find the reason for this to be neccesary.
            for (i=0 ; i< MAX_CLIENTS; i++) {
		cl = level.clients + i;
		if ( cl->pers.connected != CON_CONNECTED ) {
			continue;
                }
                cl->ps.stats[STAT_CLIENTS_READY] = 0;
            }
        }

	if ( level.intermissionQueued ) {
		if ( level.time - level.intermissionQueued >= INTERMISSION_DELAY_TIME ) {
			level.intermissionQueued = 0;
			BeginIntermission();
		}
		return;
	}

	// check for sudden death
	if ( ScoreIsTied() ) {
		// always wait for sudden death
		return;
	}

	if ( g_timelimit.integer > 0 && !level.warmupTime ) {
		if ( (level.time - level.startTime)/60000 >= g_timelimit.integer ) {
			trap_SendServerCommand( -1, "print \"Timelimit hit.\n\"");
			return;
		}
	}

	if ( level.numPlayingClients < 2 ) {
		return;
	}

	if ( (g_gametype.integer < GT_CTF || g_ffa_gt>0 ) && g_fraglimit.integer ) {
		if ( level.teamScores[TEAM_RED] >= g_fraglimit.integer ) {
			trap_SendServerCommand( -1, "print \"Red hit the fraglimit.\n\"" );
			return;
		}

		if ( level.teamScores[TEAM_BLUE] >= g_fraglimit.integer ) {
			trap_SendServerCommand( -1, "print \"Blue hit the fraglimit.\n\"" );
			return;
		}

		for ( i=0 ; i< MAX_CLIENTS; i++ ) {
			cl = level.clients + i;
			if ( cl->pers.connected != CON_CONNECTED ) {
				continue;
			}
			if ( cl->sess.sessionTeam != TEAM_FREE ) {
				continue;
			}

			if ( cl->ps.persistant[PERS_SCORE] >= g_fraglimit.integer ) {
				trap_SendServerCommand( -1, va("print \"%s" S_COLOR_WHITE " hit the fraglimit.\n\"",
					cl->pers.netname ) );
				return;
			}
		}
	}

	if ( (g_gametype.integer >= GT_CTF && g_ffa_gt<1) && g_capturelimit.integer ) {

		if ( level.teamScores[TEAM_RED] >= g_capturelimit.integer ) {
			trap_SendServerCommand( -1, "print \"Red hit the capturelimit.\n\"" );
			return;
		}

		if ( level.teamScores[TEAM_BLUE] >= g_capturelimit.integer ) {
			trap_SendServerCommand( -1, "print \"Blue hit the capturelimit.\n\"" );
			return;
		}
	}
}

//LMS - Last man Stading functions:
void StartLMSRound(void) {
	int		countsLiving;
	countsLiving = TeamLivingCount( -1, TEAM_FREE );
	if(countsLiving<2) {
		trap_SendServerCommand( -1, "print \"Not enough players to start the round\n\"");
		level.roundNumberStarted = level.roundNumber-1;
		level.roundStartTime = level.time+1000*g_elimination_warmup.integer;
		return;
	}

	//If we are enough to start a round:
	level.roundNumberStarted = level.roundNumber; //Set numbers

	SendEliminationMessageToAllClients();
	EnableWeapons();
}

void G_LevelLoadComplete(void) {

}

//the elimination start function
void StartEliminationRound(void) {

	int		countsLiving[TEAM_NUM_TEAMS];
	countsLiving[TEAM_BLUE] = TeamLivingCount( -1, TEAM_BLUE );
	countsLiving[TEAM_RED] = TeamLivingCount( -1, TEAM_RED );
	if((countsLiving[TEAM_BLUE]==0) || (countsLiving[TEAM_RED]==0))
	{
		trap_SendServerCommand( -1, "print \"Not enough players to start the round\n\"");
		level.roundNumberStarted = level.roundNumber-1;
		level.roundRespawned = qfalse;
		//Remember that one of the teams is empty!
		level.roundRedPlayers = countsLiving[TEAM_RED];
		level.roundBluePlayers = countsLiving[TEAM_BLUE];
		level.roundStartTime = level.time+1000*g_elimination_warmup.integer;
		return;
	}

	//If we are enough to start a round:
	level.roundNumberStarted = level.roundNumber; //Set numbers
	level.roundRedPlayers = countsLiving[TEAM_RED];
	level.roundBluePlayers = countsLiving[TEAM_BLUE];
	if(g_gametype.integer == GT_CTF_ELIMINATION) {
		Team_ReturnFlag( TEAM_RED );
		Team_ReturnFlag( TEAM_BLUE );
	}

	SendEliminationMessageToAllClients();
	if(g_elimination_ctf_oneway.integer)
		SendAttackingTeamMessageToAllClients(); //Ensure that evaryone know who should attack.
	EnableWeapons();
}

//things to do at end of round:
void EndEliminationRound(void)
{
	DisableWeapons();
	level.roundNumber++;
	level.roundStartTime = level.time+1000*g_elimination_warmup.integer;
	SendEliminationMessageToAllClients();
        CalculateRanks();
	level.roundRespawned = qfalse;
	if(g_elimination_ctf_oneway.integer)
		SendAttackingTeamMessageToAllClients();
}

//Things to do if we don't want to move the roundNumber
void RestartEliminationRound(void) {
	DisableWeapons();
	level.roundNumberStarted = level.roundNumber-1;
	level.roundStartTime = level.time+1000*g_elimination_warmup.integer;
        if(!level.intermissiontime)
            SendEliminationMessageToAllClients();
	level.roundRespawned = qfalse;
	if(g_elimination_ctf_oneway.integer)
		SendAttackingTeamMessageToAllClients();
}

//Things to do during match warmup
void WarmupEliminationRound(void) {
	EnableWeapons();
	level.roundNumberStarted = level.roundNumber-1;
	level.roundStartTime = level.time+1000*g_elimination_warmup.integer;
	SendEliminationMessageToAllClients();
	level.roundRespawned = qfalse;
	if(g_elimination_ctf_oneway.integer)
		SendAttackingTeamMessageToAllClients();
}

/*
========================================================================

FUNCTIONS CALLED EVERY FRAME

========================================================================
*/

/*
CheckDoubleDomination
*/

void CheckDoubleDomination( void ) {
	if ( level.numPlayingClients < 1 ) {
		return;
	}

	if ( level.warmupTime != 0) {
            if( ((level.pointStatusA == TEAM_BLUE && level.pointStatusB == TEAM_BLUE) ||
                 (level.pointStatusA == TEAM_RED && level.pointStatusB == TEAM_RED)) &&
                    level.timeTaken + 10*1000 <= level.time ) {
                        Team_RemoveDoubleDominationPoints();
                        level.roundStartTime = level.time + 10*1000;
                        SendScoreboardMessageToAllClients();
            }
            return;
        }

	if(g_gametype.integer != GT_DOUBLE_D)
		return;

	//Don't score if we are in intermission. Both points might have been taken when we went into intermission
	if(level.intermissiontime)
		return;

	if(level.pointStatusA == TEAM_RED && level.pointStatusB == TEAM_RED && level.timeTaken + 10*1000 <= level.time) {
		//Red scores
		trap_SendServerCommand( -1, "print \"Red team scores!\n\"");
		AddTeamScore(level.intermission_origin,TEAM_RED,1);
		Team_ForceGesture(TEAM_RED);
		Team_DD_bonusAtPoints(TEAM_RED);
		Team_RemoveDoubleDominationPoints();
		//We start again in 10 seconds:
		level.roundStartTime = level.time + 10*1000;
		SendScoreboardMessageToAllClients();
		CalculateRanks();
	}

	if(level.pointStatusA == TEAM_BLUE && level.pointStatusB == TEAM_BLUE && level.timeTaken + 10*1000 <= level.time) {
		//Blue scores
		trap_SendServerCommand( -1, "print \"Blue team scores!\n\"");
		AddTeamScore(level.intermission_origin,TEAM_BLUE,1);
		Team_ForceGesture(TEAM_BLUE);
		Team_DD_bonusAtPoints(TEAM_BLUE);
		Team_RemoveDoubleDominationPoints();
		//We start again in 10 seconds:
		level.roundStartTime = level.time + 10*1000;
		SendScoreboardMessageToAllClients();
		CalculateRanks();
	}

	if((level.pointStatusA == TEAM_NONE || level.pointStatusB == TEAM_NONE) && level.time>level.roundStartTime) {
		trap_SendServerCommand( -1, "print \"A new round has started\n\"");
		Team_SpawnDoubleDominationPoints();
		SendScoreboardMessageToAllClients();
	}
}

/*
CheckLMS
*/

void CheckLMS(void) {
	int mode;
	mode = g_lms_mode.integer;
	if ( level.numPlayingClients < 1 ) {
		return;
	}



	//We don't want to do anything in intermission
	if(level.intermissiontime) {
		if(level.roundRespawned) {
			RestartEliminationRound();
		}
		level.roundStartTime = level.time; //so that a player might join at any time to fix the bots+no humans+autojoin bug
		return;
	}

	if(g_gametype.integer == GT_LMS)
	{
		int		countsLiving[TEAM_NUM_TEAMS];
		//trap_SendServerCommand( -1, "print \"This is LMS!\n\"");
		countsLiving[TEAM_FREE] = TeamLivingCount( -1, TEAM_FREE );
		if(countsLiving[TEAM_FREE]==1 && level.roundNumber==level.roundNumberStarted)
		{
			if(mode <=1 )
			LMSpoint();
			trap_SendServerCommand( -1, "print \"We have a winner!\n\"");
			EndEliminationRound();
			Team_ForceGesture(TEAM_FREE);
		}

		if(countsLiving[TEAM_FREE]==0 && level.roundNumber==level.roundNumberStarted)
		{
			trap_SendServerCommand( -1, "print \"All death... how sad\n\"");
			EndEliminationRound();
		}

		if((g_elimination_roundtime.integer) && (level.roundNumber==level.roundNumberStarted)&&(g_lms_mode.integer == 1 || g_lms_mode.integer==3)&&(level.time>=level.roundStartTime+1000*g_elimination_roundtime.integer))
		{
			trap_SendServerCommand( -1, "print \"Time up - Overtime disabled\n\"");
			if(mode <=1 )
			LMSpoint();
			EndEliminationRound();
		}

		//This might be better placed another place:
		if(g_elimination_activewarmup.integer<2)
			g_elimination_activewarmup.integer=2; //We need at least 2 seconds to spawn all players
		if(g_elimination_activewarmup.integer >= g_elimination_warmup.integer) //This must not be true
			g_elimination_warmup.integer = g_elimination_activewarmup.integer+1; //Increase warmup

		//Force respawn
		if(level.roundNumber != level.roundNumberStarted && level.time>level.roundStartTime-1000*g_elimination_activewarmup.integer && !level.roundRespawned)
		{
			level.roundRespawned = qtrue;
			RespawnAll();
			DisableWeapons();
			SendEliminationMessageToAllClients();
		}

		if(level.time<=level.roundStartTime && level.time>level.roundStartTime-1000*g_elimination_activewarmup.integer)
		{
			RespawnDead();
			//DisableWeapons();
		}

		if(level.roundNumber == level.roundNumberStarted)
		{
			EnableWeapons();
		}

		if((level.roundNumber>level.roundNumberStarted)&&(level.time>=level.roundStartTime))
			StartLMSRound();

		if(level.time+1000*g_elimination_warmup.integer-500>level.roundStartTime && level.numPlayingClients < 2)
		{
			RespawnDead(); //Allow player to run around anyway
			WarmupEliminationRound(); //Start over
			return;
		}

		if(level.warmupTime != 0) {
			if(level.time+1000*g_elimination_warmup.integer-500>level.roundStartTime)
			{
				RespawnDead();
				WarmupEliminationRound();
			}
		}

	}
}

/*
=============
CheckElimination
=============
*/
void CheckElimination(void) {
	if ( level.numPlayingClients < 1 ) {
		if( (g_gametype.integer == GT_ELIMINATION || g_gametype.integer == GT_CTF_ELIMINATION) &&
			( level.time+1000*g_elimination_warmup.integer-500>level.roundStartTime ))
			RestartEliminationRound(); //For spectators
		return;
	}

	//We don't want to do anything in itnermission
	if(level.intermissiontime) {
		if(level.roundRespawned)
			RestartEliminationRound();
		level.roundStartTime = level.time+1000*g_elimination_warmup.integer;
		return;
	}

	if(g_gametype.integer == GT_ELIMINATION || g_gametype.integer == GT_CTF_ELIMINATION)
	{
		int		counts[TEAM_NUM_TEAMS];
		int		countsLiving[TEAM_NUM_TEAMS];
		int		countsHealth[TEAM_NUM_TEAMS];
		counts[TEAM_BLUE] = TeamCount( -1, TEAM_BLUE );
		counts[TEAM_RED] = TeamCount( -1, TEAM_RED );

		countsLiving[TEAM_BLUE] = TeamLivingCount( -1, TEAM_BLUE );
		countsLiving[TEAM_RED] = TeamLivingCount( -1, TEAM_RED );

		countsHealth[TEAM_BLUE] = TeamHealthCount( -1, TEAM_BLUE );
		countsHealth[TEAM_RED] = TeamHealthCount( -1, TEAM_RED );

		if(level.roundBluePlayers != 0 && level.roundRedPlayers != 0) { //Cannot score if one of the team never got any living players
			if((countsLiving[TEAM_BLUE]==0)&&(level.roundNumber==level.roundNumberStarted))
			{
				//Blue team has been eliminated!
				trap_SendServerCommand( -1, "print \"Blue Team eliminated!\n\"");
				AddTeamScore(level.intermission_origin,TEAM_RED,1);
				EndEliminationRound();
				Team_ForceGesture(TEAM_RED);
			}
			else if((countsLiving[TEAM_RED]==0)&&(level.roundNumber==level.roundNumberStarted))
			{
				//Red team eliminated!
				trap_SendServerCommand( -1, "print \"Red Team eliminated!\n\"");
				AddTeamScore(level.intermission_origin,TEAM_BLUE,1);
				EndEliminationRound();
				Team_ForceGesture(TEAM_BLUE);
			}
		}

		//Time up
		if((level.roundNumber==level.roundNumberStarted)&&(g_elimination_roundtime.integer)&&(level.time>=level.roundStartTime+1000*g_elimination_roundtime.integer))
		{
			trap_SendServerCommand( -1, "print \"No teams eliminated.\n\"");

			if(level.roundBluePlayers != 0 && level.roundRedPlayers != 0) {//We don't want to divide by zero. (should not be possible)
				if(g_gametype.integer == GT_CTF_ELIMINATION && g_elimination_ctf_oneway.integer) {
					//One way CTF, make defensice team the winner.
					if ( (level.eliminationSides+level.roundNumber)%2 == 0 ) { //Red was attacking
						trap_SendServerCommand( -1, "print \"Blue team defended the base\n\"");
						AddTeamScore(level.intermission_origin,TEAM_BLUE,1);
					}
					else {
						trap_SendServerCommand( -1, "print \"Red team defended the base\n\"");
						AddTeamScore(level.intermission_origin,TEAM_RED,1);
					}
				}
				else if(((double)countsLiving[TEAM_RED])/((double)level.roundRedPlayers)>((double)countsLiving[TEAM_BLUE])/((double)level.roundBluePlayers))
				{
					//Red team has higher procentage survivors
					trap_SendServerCommand( -1, "print \"Red team has most survivers!\n\"");
					AddTeamScore(level.intermission_origin,TEAM_RED,1);
				}
				else if(((double)countsLiving[TEAM_RED])/((double)level.roundRedPlayers)<((double)countsLiving[TEAM_BLUE])/((double)level.roundBluePlayers))
				{
					//Blue team has higher procentage survivors
					trap_SendServerCommand( -1, "print \"Blue team has most survivers!\n\"");
					AddTeamScore(level.intermission_origin,TEAM_BLUE,1);
				}
				else if(countsHealth[TEAM_RED]>countsHealth[TEAM_BLUE])
				{
					//Red team has more health
					trap_SendServerCommand( -1, "print \"Red team has more health left!\n\"");
					AddTeamScore(level.intermission_origin,TEAM_RED,1);
				}
				else if(countsHealth[TEAM_RED]<countsHealth[TEAM_BLUE])
				{
					//Blue team has more health
					trap_SendServerCommand( -1, "print \"Blue team has more health left!\n\"");
					AddTeamScore(level.intermission_origin,TEAM_BLUE,1);
				}
			}
			EndEliminationRound();
		}

		//This might be better placed another place:
		if(g_elimination_activewarmup.integer<1)
			g_elimination_activewarmup.integer=1; //We need at least 1 second to spawn all players
		if(g_elimination_activewarmup.integer >= g_elimination_warmup.integer) //This must not be true
			g_elimination_warmup.integer = g_elimination_activewarmup.integer+1; //Increase warmup

		//Force respawn
		if(level.roundNumber!=level.roundNumberStarted && level.time>level.roundStartTime-1000*g_elimination_activewarmup.integer && !level.roundRespawned)
		{
			level.roundRespawned = qtrue;
			RespawnAll();
			SendEliminationMessageToAllClients();
		}

		if(level.time<=level.roundStartTime && level.time>level.roundStartTime-1000*g_elimination_activewarmup.integer)
		{
			RespawnDead();
		}


		if((level.roundNumber>level.roundNumberStarted)&&(level.time>=level.roundStartTime))
			StartEliminationRound();

		if(level.time+1000*g_elimination_warmup.integer-500>level.roundStartTime)
		if(counts[TEAM_BLUE]<1 || counts[TEAM_RED]<1)
		{
			RespawnDead(); //Allow players to run around anyway
			WarmupEliminationRound(); //Start over
			return;
		}

		if(level.warmupTime != 0) {
			if(level.time+1000*g_elimination_warmup.integer-500>level.roundStartTime)
			{
				RespawnDead();
				WarmupEliminationRound();
			}
		}
	}
}

/*
=============
CheckDomination
=============
*/
void CheckDomination(void) {
	int i;
        int scoreFactor = 1;

	if ( (level.numPlayingClients < 1) || (g_gametype.integer != GT_DOMINATION) ) {
		return;
	}

	//Do nothing if warmup
	if(level.warmupTime != 0)
		return;

	//Don't score if we are in intermission. Just plain stupid
	if(level.intermissiontime)
		return;

	//Sago: I use if instead of while, since if the server stops for more than 2 seconds people should not be allowed to score anyway
	if(level.domination_points_count>3)
            scoreFactor = 2; //score more slowly if there are many points
        if(level.time>=level.dom_scoreGiven*DOM_SECSPERPOINT*scoreFactor) {
		for(i=0;i<level.domination_points_count;i++) {
			if ( level.pointStatusDom[i] == TEAM_RED )
				AddTeamScore(level.intermission_origin,TEAM_RED,1);
			if ( level.pointStatusDom[i] == TEAM_BLUE )
				AddTeamScore(level.intermission_origin,TEAM_BLUE,1);
		}
		level.dom_scoreGiven++;
		while(level.time>level.dom_scoreGiven*DOM_SECSPERPOINT*scoreFactor)
			level.dom_scoreGiven++;
		CalculateRanks();
	}
}

/*
=============
CheckTournament

Once a frame, check for changes in tournement player state
=============
*/
void CheckTournament( void ) {
	// check because we run 3 game frames before calling Connect and/or ClientBegin
	// for clients on a map_restart
	if ( level.numPlayingClients == 0 ) {
		return;
	}

	if ( g_gametype.integer == GT_TOURNAMENT ) {

		// pull in a spectator if needed
		if ( level.numPlayingClients < 2 ) {
			AddTournamentPlayer();
		}

		// if we don't have two players, go back to "waiting for players"
		if ( level.numPlayingClients != 2 ) {
			if ( level.warmupTime != -1 ) {
				level.warmupTime = -1;
				trap_SetConfigstring( CS_WARMUP, va("%i", level.warmupTime) );
			}
			return;
		}

		if ( level.warmupTime == 0 ) {
			return;
		}

		// if the warmup is changed at the console, restart it
		if ( g_warmup.modificationCount != level.warmupModificationCount ) {
			level.warmupModificationCount = g_warmup.modificationCount;
			level.warmupTime = -1;
		}

		// if all players have arrived, start the countdown
		if ( level.warmupTime < 0 ) {
			if ( level.numPlayingClients == 2 ) {
				// fudge by -1 to account for extra delays
				if ( g_warmup.integer > 1 ) {
					level.warmupTime = level.time + ( g_warmup.integer - 1 ) * 1000;
				} else {
					level.warmupTime = 0;
				}

				trap_SetConfigstring( CS_WARMUP, va("%i", level.warmupTime) );
			}
			return;
		}

		// if the warmup time has counted down, restart
		if ( level.time > level.warmupTime ) {
			level.warmupTime += 10000;
			trap_Cvar_Set( "g_restarted", "1" );
			trap_SendConsoleCommand( EXEC_APPEND, "map_restart 0\n" );
			level.restarted = qtrue;
			return;
		}
	} else if ( level.warmupTime != 0 ) {
		int		counts[TEAM_NUM_TEAMS];
		qboolean	notEnough = qfalse;

		if ( g_gametype.integer > GT_TEAM && !g_ffa_gt ) {
			counts[TEAM_BLUE] = TeamCount( -1, TEAM_BLUE );
			counts[TEAM_RED] = TeamCount( -1, TEAM_RED );

			if (counts[TEAM_RED] < 1 || counts[TEAM_BLUE] < 1) {
				notEnough = qtrue;
			}
		} else if ( level.numPlayingClients < 2 ) {
			notEnough = qtrue;
		}

		if ( notEnough ) {
			if ( level.warmupTime != -1 ) {
				level.warmupTime = -1;
				trap_SetConfigstring( CS_WARMUP, va("%i", level.warmupTime) );
			}
			return; // still waiting for team members
		}

		if ( level.warmupTime == 0 ) {
			return;
		}

		// if the warmup is changed at the console, restart it
		if ( g_warmup.modificationCount != level.warmupModificationCount ) {
			level.warmupModificationCount = g_warmup.modificationCount;
			level.warmupTime = -1;
		}

		// if all players have arrived, start the countdown
		if ( level.warmupTime < 0 ) {
			// fudge by -1 to account for extra delays
			level.warmupTime = level.time + ( g_warmup.integer - 1 ) * 1000;
			trap_SetConfigstring( CS_WARMUP, va("%i", level.warmupTime) );
			return;
		}

		// if the warmup time has counted down, restart
		if ( level.time > level.warmupTime ) {
			level.warmupTime += 10000;
			trap_Cvar_Set( "g_restarted", "1" );
			trap_SendConsoleCommand( EXEC_APPEND, "map_restart 0\n" );
			level.restarted = qtrue;
			return;
		}
	}
}

/*
==================
PrintTeam
==================
*/
void PrintTeam(int team, char *message) {
	int i;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if (level.clients[i].sess.sessionTeam != team)
			continue;
		trap_SendServerCommand( i, message );
	}
}

/*
==================
SetLeader
==================
*/
void SetLeader(int team, int client) {
	int i;

	if ( level.clients[client].pers.connected == CON_DISCONNECTED ) {
		PrintTeam(team, va("print \"%s is not connected\n\"", level.clients[client].pers.netname) );
		return;
	}
	if (level.clients[client].sess.sessionTeam != team) {
		PrintTeam(team, va("print \"%s is not on the team anymore\n\"", level.clients[client].pers.netname) );
		return;
	}
	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if (level.clients[i].sess.sessionTeam != team)
			continue;
		if (level.clients[i].sess.teamLeader) {
			level.clients[i].sess.teamLeader = qfalse;
			ClientUserinfoChanged(i);
		}
	}
	level.clients[client].sess.teamLeader = qtrue;
	ClientUserinfoChanged( client );
	PrintTeam(team, va("print \"%s is the new team leader\n\"", level.clients[client].pers.netname) );
}

/*
==================
CheckTeamLeader
==================
*/
void CheckTeamLeader( int team ) {
	int i;

	for ( i = 0 ; i < level.maxclients ; i++ ) {
		if (level.clients[i].sess.sessionTeam != team)
			continue;
		if (level.clients[i].sess.teamLeader)
			break;
	}
	if (i >= level.maxclients) {
		for ( i = 0 ; i < level.maxclients ; i++ ) {
			if (level.clients[i].sess.sessionTeam != team)
				continue;
			if (!(g_entities[i].r.svFlags & SVF_BOT)) {
				level.clients[i].sess.teamLeader = qtrue;
				break;
			}
		}
		for ( i = 0 ; i < level.maxclients ; i++ ) {
			if (level.clients[i].sess.sessionTeam != team)
				continue;
			level.clients[i].sess.teamLeader = qtrue;
			break;
		}
	}
}

/*
==================
CheckVote
==================
*/
void CheckVote( void ) {
	if ( level.voteExecuteTime && level.voteExecuteTime < level.time ) {
		level.voteExecuteTime = 0;
		trap_SendConsoleCommand( EXEC_APPEND, va("%s\n", level.voteString ) );
	}
	if ( !level.voteTime ) {
		return;
	}
	if ( level.time - level.voteTime >= VOTE_TIME ) {
		trap_SendServerCommand( -1, "print \"Vote failed.\n\"" );
	} else {
		// ATVI Q3 1.32 Patch #9, WNF
		if ( level.voteYes > level.numVotingClients/2 ) {
			// execute the command, then remove the vote
			trap_SendServerCommand( -1, "print \"Vote passed.\n\"" );
			level.voteExecuteTime = level.time + 3000;
		} else if ( level.voteNo >= level.numVotingClients/2 ) {
			// same behavior as a timeout
			trap_SendServerCommand( -1, "print \"Vote failed.\n\"" );
		} else {
			// still waiting for a majority
			return;
		}
	}
	level.voteTime = 0;
	trap_SetConfigstring( CS_VOTE_TIME, "" );

}

/*
==================
CheckTeamVote
==================
*/
void CheckTeamVote( int team ) {
	int cs_offset;

	if ( team == TEAM_RED )
		cs_offset = 0;
	else if ( team == TEAM_BLUE )
		cs_offset = 1;
	else
		return;

	if ( !level.teamVoteTime[cs_offset] ) {
		return;
	}
	if ( level.time - level.teamVoteTime[cs_offset] >= VOTE_TIME ) {
		trap_SendServerCommand( -1, "print \"Team vote failed.\n\"" );
	} else {
		if ( level.teamVoteYes[cs_offset] > level.numteamVotingClients[cs_offset]/2 ) {
			// execute the command, then remove the vote
			trap_SendServerCommand( -1, "print \"Team vote passed.\n\"" );
			//
			if ( !Q_strncmp( "leader", level.teamVoteString[cs_offset], 6) ) {
				//set the team leader
				SetLeader(team, atoi(level.teamVoteString[cs_offset] + 7));
			}
			else {
				trap_SendConsoleCommand( EXEC_APPEND, va("%s\n", level.teamVoteString[cs_offset] ) );
			}
		} else if ( level.teamVoteNo[cs_offset] >= level.numteamVotingClients[cs_offset]/2 ) {
			// same behavior as a timeout
			trap_SendServerCommand( -1, "print \"Team vote failed.\n\"" );
		} else {
			// still waiting for a majority
			return;
		}
	}
	level.teamVoteTime[cs_offset] = 0;
	trap_SetConfigstring( CS_TEAMVOTE_TIME + cs_offset, "" );

}

/*
=============
G_RunThink

Runs thinking code for this frame if necessary
=============
*/
void G_RunThink (gentity_t *ent) {
	float	thinktime;

	thinktime = ent->nextthink;
	if (thinktime <= 0) {
		return;
	}
	if (thinktime > level.time) {
		return;
	}

	ent->nextthink = 0;
	if (!ent->think) {
		G_Error ( "NULL ent->think");
	}
	ent->think (ent);
}

/*
=============
G_RunCutscene

Runs cutscene code for this frame. While the actual camera view is handled client side, the client is moved along serverside as well
so that the client is in the same VIS area as the camera is.
=============
*/
void G_RunCutscene( int levelTime ) {
//	gclient_s *client;
	char cutsceneData[MAX_INFO_STRING];
	float wait;
	int start_time;
	vec3_t destOrigin, destAngles;
	vec3_t newOrigin, newAngles;
	int timePassed;
	float progress;
	float diff;
	int doPan;
	int i;
	
	if ( !level.player || level.player->client->ps.pm_type != PM_CUTSCENE )
		return;

	trap_GetConfigstring( CS_CUTSCENE, cutsceneData, sizeof(cutsceneData) );

	doPan = atoi(Info_ValueForKey(cutsceneData, "m"));
	start_time = atoi(Info_ValueForKey(cutsceneData, "t"));
	wait = atof(Info_ValueForKey(cutsceneData, "w"));
	newOrigin[0] = atof(Info_ValueForKey(cutsceneData, "o10"));
	newOrigin[1] = atof(Info_ValueForKey(cutsceneData, "o11"));
	newOrigin[2] = atof(Info_ValueForKey(cutsceneData, "o12"));

	if ( doPan ) {
		destOrigin[0] = atof(Info_ValueForKey(cutsceneData, "o20"));
		destOrigin[1] = atof(Info_ValueForKey(cutsceneData, "o21"));
		destOrigin[2] = atof(Info_ValueForKey(cutsceneData, "o22"));

		//determine how long the current camera pan has taken
		timePassed = levelTime - start_time;
		progress = timePassed / (wait * 1000);

		//calculate new origin
		diff = destOrigin[0] - newOrigin[0];
		newOrigin[0] += diff * progress;

		diff = destOrigin[1] - newOrigin[1];
		newOrigin[1] += diff * progress;
		
		diff = destOrigin[2] - newOrigin[2];
		newOrigin[2] += diff * progress;
	}
	
	VectorCopy( newOrigin, level.player->client->ps.origin );
}

/*
###############
Noire.Script API - Threads
###############
*/

char qagameThreadBuffer[MAX_CYCLE_SIZE];

// Load threads
void RunScriptThreads(int time) {
    int i;

    for (i = 0; i < threadsCount; i++) {
        ScriptLoop* script = &threadsLoops[i];
        if (time - script->lastRunTime >= script->interval) {
            // Обновляем время последнего запуска
            script->lastRunTime = time;

            // Используем временный буфер для выполнения скрипта
            Q_strncpyz(qagameThreadBuffer, script->code, MAX_CYCLE_SIZE - 1);
            qagameThreadBuffer[MAX_CYCLE_SIZE - 1] = '\0'; // Убедимся, что буфер терминальный

            Interpret(qagameThreadBuffer); // Запускаем скрипт из временного буфера
        }
    }
}

/*
================
G_RunFrame

Advances the non-player objects in the world
================
*/
void G_RunFrame( int levelTime ) {
	int			i;
	gentity_t	*ent;
	int			mins, seconds, tens;
	int			msec;
	int start, end;

	// if we are waiting for the level to restart, do nothing
	if ( level.restarted ) {
		return;
	}

	level.framenum++;
	level.previousTime = level.time;
	level.time = levelTime;
	msec = level.time - level.previousTime;
	seconds = level.time / 1000;
	mins = seconds / 60;
	seconds -= mins * 60;
	tens = seconds / 10;
	seconds -= tens * 10;

	// get any cvar changes
	G_UpdateCvars();

	RunScriptThreads(level.time);	//Noire.Script - run threads

        if( (g_gametype.integer==GT_ELIMINATION || g_gametype.integer==GT_CTF_ELIMINATION) && !(g_elimflags.integer & EF_NO_FREESPEC) && g_elimination_lockspectator.integer>1)
            trap_Cvar_Set("elimflags",va("%i",g_elimflags.integer|EF_NO_FREESPEC));
        else
        if( (g_elimflags.integer & EF_NO_FREESPEC) && g_elimination_lockspectator.integer<2)
            trap_Cvar_Set("elimflags",va("%i",g_elimflags.integer&(~EF_NO_FREESPEC) ) );

        if( g_elimination_ctf_oneway.integer && !(g_elimflags.integer & EF_ONEWAY) ) {
            trap_Cvar_Set("elimflags",va("%i",g_elimflags.integer|EF_ONEWAY ) );
            //If the server admin has enabled it midgame imidiantly braodcast attacking team
            SendAttackingTeamMessageToAllClients();
        }
        else
        if( !g_elimination_ctf_oneway.integer && (g_elimflags.integer & EF_ONEWAY) ) {
            trap_Cvar_Set("elimflags",va("%i",g_elimflags.integer&(~EF_ONEWAY) ) );
        }

	//
	// go through all allocated objects
	//
	start = trap_Milliseconds();
	ent = &g_entities[0];
	for (i=0 ; i<level.num_entities ; i++, ent++) {
		if ( !ent->inuse ) {
			continue;
		}

		// clear events that are too old
		if ( level.time - ent->eventTime > EVENT_VALID_MSEC ) {
			if ( ent->s.event ) {
				ent->s.event = 0;	// &= EV_EVENT_BITS;
				if ( ent->client ) {
					ent->client->ps.externalEvent = 0;
				}
			}
			if ( ent->freeAfterEvent ) {
				// tempEntities or dropped items completely go away after their event
				G_FreeEntity( ent );
				continue;
			} else if ( ent->unlinkAfterEvent ) {
				// items that will respawn will hide themselves after their pickup event
				ent->unlinkAfterEvent = qfalse;
				trap_UnlinkEntity( ent );
			}
		}

		// temporary entities don't think
		if ( ent->freeAfterEvent ) {
			continue;
		}

		if ( !ent->r.linked && ent->neverFree ) {
			continue;
		}

		if ( ent->s.eType == ET_ITEM && !ent->sandboxObject || ent->physicsObject && !ent->sandboxObject ) {
			G_RunItem( ent );
			continue;
		}
		
		if ( ent->s.eType == ET_ITEM && ent->sandboxObject || ent->sandboxObject ) {
			Phys_Frame( ent );
			continue;
		}

		if ( ent->s.eType == ET_MOVER ) {
			G_RunMover( ent );
			continue;
		}

		if ( i < MAX_CLIENTS ) {
			G_RunClient( ent );
			continue;
		}

		G_RunThink( ent );
	}

//unlagged - backward reconciliation #2
	// NOW run the missiles, with all players backward-reconciled
	// to the positions they were in exactly 50ms ago, at the end
	// of the last server frame
	G_TimeShiftAllClients( level.previousTime, NULL );

	ent = &g_entities[0];
	for (i=0 ; i<level.num_entities ; i++, ent++) {
		if ( !ent->inuse ) {
			continue;
		}

		// temporary entities don't think
		if ( ent->freeAfterEvent ) {
			continue;
		}

		if ( ent->s.eType == ET_MISSILE ) {
			G_RunMissile( ent );
		}
	}

	G_UnTimeShiftAllClients( NULL );
//unlagged - backward reconciliation #2

end = trap_Milliseconds();

start = trap_Milliseconds();
	// perform final fixups on the players
	ent = &g_entities[0];
	for (i=0 ; i < level.maxclients ; i++, ent++ ) {
		if ( ent->inuse ) {
			ClientEndFrame( ent );
		}
	}
end = trap_Milliseconds();

	// see if it is time to do a tournement restart
	CheckTournament();

	//Check Elimination state
	CheckElimination();
	CheckLMS();

	//Check Double Domination
	CheckDoubleDomination();

	CheckDomination();

	//Sago: I just need to think why I placed this here... they should only spawn once
	if(g_gametype.integer == GT_DOMINATION)
		Team_Dom_SpawnPoints();

	// see if it is time to end the level
	CheckExitRules();

	// update to team status?
	CheckTeamStatus();

	// cancel vote if timed out
	CheckVote();

	// check team votes
	CheckTeamVote( TEAM_RED );
	CheckTeamVote( TEAM_BLUE );

	UpdateGameCvars ();

//unlagged - backward reconciliation #4
	// record the time at the end of this frame - it should be about
	// the time the next frame begins - when the server starts
	// accepting commands from connected clients
	level.frameStartTime = trap_Milliseconds();
//unlagged - backward reconciliation #4
    //cutscene
	G_RunCutscene( levelTime );
}
void UpdateGameCvars( void ){
mod_jumpheight = g_jumpheight.integer;
mod_ammolimit = g_ammolimit.integer;
mod_gdelay = g_gdelay.integer;
mod_mgdelay = g_mgdelay.integer;
mod_mgspread = g_mgspread.integer;
mod_sgdelay = g_sgdelay.integer;
mod_sgspread = g_sgspread.integer;
mod_sgcount = g_sgcount.integer;
mod_gldelay = g_gldelay.integer;
mod_rldelay = g_rldelay.integer;
mod_lgdelay = g_lgdelay.integer;
mod_lgrange = g_lgrange.integer;
mod_pgdelay = g_pgdelay.integer;
mod_rgdelay = g_rgdelay.integer;
mod_bfgdelay = g_bfgdelay.integer;
mod_ngdelay = g_ngdelay.integer;
mod_pldelay = g_pldelay.integer;
mod_cgdelay = g_cgdelay.integer;
mod_cgspread = g_cgspread.integer;
mod_ftdelay = g_ftdelay.integer;
mod_amdelay = g_amdelay.integer;
mod_ghtimeout = g_ghtimeout.integer;
mod_ammoregenfirespeed = g_ammoregenfirespeed.value;
mod_hastefirespeed = g_hastefirespeed.value;
mod_scoutfirespeed = g_scoutfirespeed.value;
mod_doublerfirespeed = g_doublerfirespeed.value;
mod_guardfirespeed = g_guardfirespeed.value;
mod_quadtime = g_quadtime.integer;
mod_bsuittime = g_bsuittime.integer;
mod_hastetime = g_hastetime.integer;
mod_invistime = g_invistime.integer;
mod_regentime = g_regentime.integer;
mod_flighttime = g_flighttime.integer;
mod_noplayerclip = g_noplayerclip.integer;
mod_invulmove = g_invulmove.integer;
mod_teamblue_firespeed = g_teamblue_firespeed.integer;
mod_teamred_firespeed = g_teamred_firespeed.integer;
mod_medkitlimit = g_medkitlimit.integer;
mod_medkitinf = g_medkitinf.integer;
mod_teleporterinf = g_teleporterinf.integer;
mod_portalinf = g_portalinf.integer;
mod_kamikazeinf = g_kamikazeinf.integer;
mod_invulinf = g_invulinf.integer;
mod_teamblue_damage = g_teamblue_damage.value;
mod_teamred_damage = g_teamred_damage.value;
mod_accelerate = g_accelerate.value;
mod_movetype = g_movetype.value;
mod_gravity = g_gravity.value;
}
