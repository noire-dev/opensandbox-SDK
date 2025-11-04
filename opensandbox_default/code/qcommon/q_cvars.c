// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

#include "ns_local.h"

typedef struct {
	cvar_t	*vmCvar;
	char		*cvarName;
	char		*defaultString;
	int			cvarFlags;
} cvarTable_t;

#ifdef GAME
cvar_t    sv_cheats;
cvar_t    g_maxClients;
cvar_t    g_maxEntities;
cvar_t    g_fraglimit;
cvar_t    g_timelimit;
cvar_t    g_capturelimit;
cvar_t    g_gametype;
cvar_t    g_entitypack;
cvar_t    g_jumpheight;
cvar_t    g_friendlyFire;
cvar_t    g_password;
cvar_t    g_dedicated;
cvar_t    g_speed;
cvar_t    g_gravity;
cvar_t    g_knockback;
cvar_t    g_blood;
cvar_t    g_enableDust;
cvar_t    g_enableBreath;
cvar_t    g_spawn_health;
cvar_t    g_spawn_armor;
cvar_t    g_spawn_machinegun;
cvar_t    g_spawn_shotgun;
cvar_t    g_spawn_grenade;
cvar_t    g_spawn_rocket;
cvar_t    g_spawn_lightning;
cvar_t    g_spawn_railgun;
cvar_t    g_spawn_plasmagun;
cvar_t    g_spawn_bfg;
cvar_t    g_spawn_grapple;
cvar_t    g_spawn_nailgun;
cvar_t    g_spawn_prox;
cvar_t    g_spawn_chaingun;
cvar_t    sv_fps;

static cvarTable_t cvarTable[] = {
	{ NULL,                         "gamename",                     "opensandbox",  CVAR_ROM },
	{ NULL,                         "gamedate",                     __DATE__,       CVAR_ROM },
	{ &sv_cheats,                   "sv_cheats",                  	"0",            0 },
	{ &g_maxClients,                "g_maxClients",                 "128",          CVAR_SERVERINFO|CVAR_ARCHIVE },
	{ &g_maxEntities,               "g_maxEntities",                "4096",         CVAR_SERVERINFO|CVAR_ARCHIVE },
	{ &g_fraglimit,                 "fraglimit",                    "20",           CVAR_SERVERINFO|CVAR_ARCHIVE },
	{ &g_timelimit,                 "timelimit",                    "0",            CVAR_SERVERINFO|CVAR_ARCHIVE },
	{ &g_capturelimit,              "capturelimit",                 "8",            CVAR_SERVERINFO|CVAR_ARCHIVE },
	{ &g_gametype,                  "g_gametype",                   "0",            CVAR_SERVERINFO|CVAR_USERINFO|CVAR_LATCH },
	{ &g_entitypack,                "g_entitypack",                 "default",      CVAR_ARCHIVE },
	{ &g_jumpheight,                "g_jumpheight",                 "270",          0 },
	{ &g_friendlyFire,              "g_friendlyFire",               "0",            CVAR_ARCHIVE },
	{ &g_password,                  "g_password",                   "",             CVAR_USERINFO },
	{ &g_dedicated,                 "dedicated",                    "0",            0 },
	{ &g_speed,                     "g_speed",                      "320",          CVAR_ARCHIVE },
	{ &g_gravity,                   "g_gravity",                    "800",          CVAR_ARCHIVE },
	{ &g_knockback,                 "g_knockback",                  "1000",         0 },
	{ &g_blood,                     "com_blood",                    "1",            0 },
	{ &g_enableDust,                "g_enableDust",                 "0",            CVAR_SERVERINFO },
	{ &g_enableBreath,              "g_enableBreath",               "0",            CVAR_SERVERINFO },
	{ &g_spawn_health,           	"g_spawn_health",            	"100",          CVAR_ARCHIVE },
	{ &g_spawn_armor,            	"g_spawn_armor",             	"0",            CVAR_ARCHIVE },
	{ &g_spawn_machinegun,       	"g_spawn_machinegun",        	"100",          CVAR_ARCHIVE },
	{ &g_spawn_shotgun,          	"g_spawn_shotgun",           	"0",            CVAR_ARCHIVE },
	{ &g_spawn_grenade,          	"g_spawn_grenade",           	"0",            CVAR_ARCHIVE },
	{ &g_spawn_rocket,           	"g_spawn_rocket",            	"0",            CVAR_ARCHIVE },
	{ &g_spawn_lightning,        	"g_spawn_lightning",         	"0",            CVAR_ARCHIVE },
	{ &g_spawn_railgun,          	"g_spawn_railgun",           	"0",            CVAR_ARCHIVE },
	{ &g_spawn_plasmagun,        	"g_spawn_plasmagun",         	"0",            CVAR_ARCHIVE },
	{ &g_spawn_bfg,              	"g_spawn_bfg",               	"0",            CVAR_ARCHIVE },
    { &g_spawn_grapple,          	"g_spawn_grapple",           	"0",            CVAR_ARCHIVE },
	{ &g_spawn_nailgun,             "g_spawn_nailgun",              "0",            CVAR_ARCHIVE },
	{ &g_spawn_prox,             	"g_spawn_prox",              	"0",            CVAR_ARCHIVE },
	{ &g_spawn_chaingun,            "g_spawn_chaingun",             "0",            CVAR_ARCHIVE },
	{ &sv_fps,                      "sv_fps",                    	"60",           CVAR_SYSTEMINFO|CVAR_ARCHIVE }
};
#endif

#ifdef CGAME
cvar_t    g_gametype;
cvar_t    headR;
cvar_t    headG;
cvar_t    headB;
cvar_t    modelR;
cvar_t    modelG;
cvar_t    modelB;
cvar_t    legsR;
cvar_t    legsG;
cvar_t    legsB;
cvar_t    physR;
cvar_t    physG;
cvar_t    physB;
cvar_t    cg_effectsTime;
cvar_t    cg_effectsLimit;
cvar_t    cg_effectsGibs;
cvar_t    spawn_cmd;
cvar_t    toolgun_cmd;
cvar_t    toolgun_mod1;
cvar_t    toolgun_mod2;
cvar_t    toolgun_mod3;
cvar_t    toolgun_mod4;
cvar_t    toolgun_mod5;
cvar_t    toolgun_tool;
cvar_t    toolgun_tooltext;
cvar_t    toolgun_tooltip1;
cvar_t    toolgun_tooltip2;
cvar_t    toolgun_tooltip3;
cvar_t    toolgun_tooltip4;
cvar_t    toolgun_toolmode1;
cvar_t    toolgun_toolmode2;
cvar_t    toolgun_toolmode3;
cvar_t    toolgun_toolmode4;
cvar_t    ns_haveerror;
cvar_t    cg_postprocess;
cvar_t    cg_enableBobbing;
cvar_t    cg_shadows;
cvar_t    cg_drawTimer;
cvar_t    cg_drawFPS;
cvar_t    cg_drawCrosshair;
cvar_t    cg_crosshairScale;
cvar_t    cg_draw2D;
cvar_t    cg_addMarks;
cvar_t    cg_drawGun;
cvar_t    cg_fov;
cvar_t    cg_zoomFov;
cvar_t    cg_thirdPerson;
cvar_t    cg_thirdPersonRange;
cvar_t    cg_thirdPersonOffset;
cvar_t    cg_drawSpeed;
cvar_t    cg_paused;
cvar_t    cg_blood;
cvar_t    cg_cameraEyes;
cvar_t    cg_cameraEyes_Fwd;
cvar_t    cg_cameraEyes_Up;
cvar_t    cg_noProjectileTrail;
cvar_t    cg_enableDust;
cvar_t    cg_enableBreath;
cvar_t    sv_fps;
cvar_t    cg_crosshairColorRed;
cvar_t    cg_crosshairColorGreen;
cvar_t    cg_crosshairColorBlue;

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
	{ &spawn_cmd,                    	"spawn_cmd",                 	"0",            0 },
	{ &toolgun_cmd,                    	"toolgun_cmd",                 	"0",            0 },
	{ &toolgun_mod1,                    "toolgun_mod1",                 "0",            0 },
	{ &toolgun_mod2,                    "toolgun_mod2",                 "0",            0 },
	{ &toolgun_mod3,                    "toolgun_mod3",                 "0",            0 },
	{ &toolgun_mod4,                    "toolgun_mod4",                 "0",            0 },
	{ &toolgun_mod5,                    "toolgun_mod5",                 "0",            0 },
	{ &toolgun_tool,                    "toolgun_tool",                 "0",            CVAR_USERINFO },
	{ &toolgun_tooltext,                "toolgun_tooltext",             "",             0 },
	{ &toolgun_tooltip1,                "toolgun_tooltip1",             "",             0 },
	{ &toolgun_tooltip2,                "toolgun_tooltip2",             "",             0 },
	{ &toolgun_tooltip3,                "toolgun_tooltip3",             "",             0 },
	{ &toolgun_tooltip4,                "toolgun_tooltip4",             "",             0 },
	{ &toolgun_toolmode1,               "toolgun_toolmode1",            "",             0 },
	{ &toolgun_toolmode2,               "toolgun_toolmode2",            "",             0 },
	{ &toolgun_toolmode3,               "toolgun_toolmode3",            "",             0 },
	{ &toolgun_toolmode4,               "toolgun_toolmode4",            "",             0 },
	{ &ns_haveerror,                    "ns_haveerror",                 "0",            0 },
	{ &cg_postprocess,                  "cg_postprocess",               "",             0 },
	{ &cg_drawGun,                      "cg_drawGun",                   "1",            CVAR_ARCHIVE },
	{ &cg_zoomFov,                      "cg_zoomfov",                   "22",           CVAR_ARCHIVE },
	{ &cg_fov,                          "cg_fov",                       "110",          CVAR_ARCHIVE },
	{ &cg_shadows,                      "cg_shadows",                   "1",            CVAR_ARCHIVE },
	{ &cg_draw2D,                       "cg_draw2D",                    "1",            CVAR_ARCHIVE },
	{ &cg_drawTimer,                    "cg_drawTimer",                 "0",            CVAR_ARCHIVE },
	{ &cg_drawFPS,                      "cg_drawFPS",                   "0",            CVAR_ARCHIVE },
	{ &cg_drawSpeed,                    "cg_drawSpeed",                 "0",            CVAR_ARCHIVE },
	{ &cg_drawCrosshair,                "cg_drawCrosshair",             "4",            CVAR_ARCHIVE },
	{ &cg_crosshairScale,               "cg_crosshairScale",            "24",           CVAR_ARCHIVE },
	{ &cg_addMarks,                     "cg_addMarks",                  "1",            CVAR_ARCHIVE },
	{ &cg_enableBobbing,               	"cg_enableBobbing",            	"1",            CVAR_ARCHIVE },
	{ &cg_thirdPersonRange,             "cg_thirdPersonRange",          "65",           CVAR_ARCHIVE },
	{ &cg_thirdPersonOffset,            "cg_thirdPersonOffset",         "25",           CVAR_ARCHIVE },
	{ &cg_thirdPerson,                  "cg_thirdPerson",               "0",            CVAR_ARCHIVE},
	{ &cg_paused,                       "cl_paused",                    "0",            CVAR_ROM },
	{ &cg_blood,                        "com_blood",                    "1",            CVAR_ARCHIVE },
	{ &cg_enableDust,                   "g_enableDust",                 "0",            CVAR_SERVERINFO },
	{ &cg_enableBreath,                 "g_enableBreath",               "0",            CVAR_SERVERINFO },
	{ &cg_noProjectileTrail,            "cg_noProjectileTrail",         "0",            CVAR_ARCHIVE },
	{ &cg_cameraEyes,                   "cg_cameraEyes",                "0",            CVAR_ARCHIVE },
	{ &cg_cameraEyes_Fwd,               "cg_cameraEyes_Fwd",            "-2",            CVAR_ARCHIVE },
	{ &cg_cameraEyes_Up,                "cg_cameraEyes_Up",             "7",            CVAR_ARCHIVE },
	{ &sv_fps,                          "sv_fps",                       "60",           CVAR_SYSTEMINFO },
	{ &cg_crosshairColorRed,            "cg_crosshairColorRed",         "0.5",          CVAR_ARCHIVE|CVAR_USERINFO },
    { &cg_crosshairColorGreen,          "cg_crosshairColorGreen",       "0.75",         CVAR_ARCHIVE|CVAR_USERINFO },
    { &cg_crosshairColorBlue,           "cg_crosshairColorBlue",        "1.0",          CVAR_ARCHIVE|CVAR_USERINFO }
};
#endif

#ifdef UI
cvar_t    spawn_preset;
cvar_t    toolgun_toolset1;
cvar_t    toolgun_toolset2;
cvar_t    toolgun_toolset3;
cvar_t    toolgun_toolset4;
cvar_t    toolgun_tooltext;
cvar_t    ui_effectslevel;
cvar_t    cl_selectedmod;
cvar_t    ui_loaded;
cvar_t    sensitivitymenu;
cvar_t    ui_browserMaster;
cvar_t    ui_browserGameType;
cvar_t    ui_browserSortKey;
cvar_t    ui_browserShowFull;
cvar_t    ui_browserShowEmpty;
cvar_t    ui_brassTime;
cvar_t    ui_drawCrosshair;
cvar_t    ui_drawCrosshairNames;
cvar_t    ui_marks;
cvar_t    ui_server1;
cvar_t    ui_server2;
cvar_t    ui_server3;
cvar_t    ui_server4;
cvar_t    ui_server5;
cvar_t    ui_server6;
cvar_t    ui_server7;
cvar_t    ui_server8;
cvar_t    ui_server9;
cvar_t    ui_server10;
cvar_t    ui_server11;
cvar_t    ui_server12;
cvar_t    ui_server13;
cvar_t    ui_server14;
cvar_t    ui_server15;
cvar_t    ui_server16;
cvar_t    ui_server17;
cvar_t    ui_server18;
cvar_t    ui_server19;
cvar_t    ui_server20;
cvar_t    ui_server21;
cvar_t    ui_server22;
cvar_t    ui_server23;
cvar_t    ui_server24;
cvar_t    ui_server25;
cvar_t    ui_server26;
cvar_t    ui_server27;
cvar_t    ui_server28;
cvar_t    ui_server29;
cvar_t    ui_server30;
cvar_t    ui_server31;
cvar_t    ui_server32;
cvar_t    gui_animsfx;
cvar_t    gui_mapicons;
cvar_t    gui_autoclosebotmenu;
cvar_t    gui_map_multisel;
cvar_t    gui_map_list;
cvar_t    gui_bot_multisel;
cvar_t    gui_bot_list;

static cvarTable_t cvarTable[] = {
	{ &spawn_preset,                    "spawn_preset",                 "spawn_cmd = sl prop props/%s %s 0 25 25 0 %s 1 1 \"none\" -1 0 0 0 0 1 1 1 1 0.40 1.00",  0 },
	{ &toolgun_toolset1,                "toolgun_toolset1",             "--------:",    0 },
	{ &toolgun_toolset2,                "toolgun_toolset2",             "--------:",    0 },
	{ &toolgun_toolset3,                "toolgun_toolset3",             "--------:",    0 },
	{ &toolgun_toolset4,                "toolgun_toolset4",             "--------:",    0 },
	{ &toolgun_tooltext,                "toolgun_tooltext",             "",             0 },
	{ &ui_effectslevel,                 "ui_effectslevel",              "0",            CVAR_ARCHIVE },
	{ &ui_loaded,                       "ui_loaded",                    "0",            0 },
	{ &cl_selectedmod,                  "cl_selectedmod",               "default",      CVAR_ARCHIVE },
	{ &sensitivitymenu,                 "sensitivitymenu",              "1",            CVAR_ARCHIVE },
	{ &ui_browserMaster,                "ui_browserMaster",             "0",            CVAR_ARCHIVE },
	{ &ui_browserGameType,              "ui_browserGameType",           "0",            CVAR_ARCHIVE },
	{ &ui_browserSortKey,               "ui_browserSortKey",            "4",            CVAR_ARCHIVE },
	{ &ui_browserShowFull,              "ui_browserShowFull",           "1",            CVAR_ARCHIVE },
	{ &ui_browserShowEmpty,             "ui_browserShowEmpty",          "1",            CVAR_ARCHIVE },
	{ &ui_drawCrosshair,                "cg_drawCrosshair",             "4",            CVAR_ARCHIVE },
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

static int cvarTableSize = ARRAY_SIZE(cvarTable);

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
