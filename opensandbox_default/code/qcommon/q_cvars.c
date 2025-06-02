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
vmCvar_t    g_maxClients;
vmCvar_t    g_maxEntities;
vmCvar_t    g_fraglimit;
vmCvar_t    g_timelimit;
vmCvar_t    g_capturelimit;
vmCvar_t    g_gametype;
vmCvar_t    g_entitypack;
vmCvar_t    g_extendedsandbox;
vmCvar_t    g_randomItems;
vmCvar_t    g_jumpheight;
vmCvar_t    g_friendlyFire;
vmCvar_t    g_password;
vmCvar_t    g_dedicated;
vmCvar_t    g_speed;
vmCvar_t    g_gravity;
vmCvar_t    g_knockback;
vmCvar_t    g_blood;
vmCvar_t    g_enableDust;
vmCvar_t    g_enableBreath;
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
vmCvar_t    sv_fps;

static cvarTable_t cvarTable[] = {
	{ NULL,                         "gamename",                     GAME_VERSION,   CVAR_ROM },
	{ NULL,                         "gamedate",                     __DATE__,       CVAR_ROM },
	{ &g_cheats,                    "sv_cheats",                    "",             0 },
	{ &g_maxClients,                "g_maxClients",                 "128",          CVAR_SERVERINFO|CVAR_ARCHIVE },
	{ &g_maxEntities,               "g_maxEntities",                "1024",         CVAR_SERVERINFO|CVAR_ARCHIVE },
	{ &g_fraglimit,                 "fraglimit",                    "20",           CVAR_SERVERINFO|CVAR_ARCHIVE|CVAR_NORESTART },
	{ &g_timelimit,                 "timelimit",                    "0",            CVAR_SERVERINFO|CVAR_ARCHIVE|CVAR_NORESTART },
	{ &g_capturelimit,              "capturelimit",                 "8",            CVAR_SERVERINFO|CVAR_ARCHIVE|CVAR_NORESTART },
	{ &g_gametype,                  "g_gametype",                   "0",            CVAR_SERVERINFO|CVAR_USERINFO|CVAR_LATCH },
	{ &g_entitypack,                "g_entitypack",                 "default",      0 },
	{ &g_extendedsandbox,           "g_extendedsandbox",            "0",            0 },
	{ &g_randomItems,               "g_randomItems",                "0",            0 },
	{ &g_jumpheight,                "g_jumpheight",                 "270",          0 },
	{ &g_friendlyFire,              "g_friendlyFire",               "0",            CVAR_ARCHIVE },
	{ &g_password,                  "g_password",                   "",             CVAR_USERINFO },
	{ &g_dedicated,                 "dedicated",                    "0",            0 },
	{ &g_speed,                     "g_speed",                      "320",          0 },
	{ &g_gravity,                   "g_gravity",                    "800",          0 },
	{ &g_knockback,                 "g_knockback",                  "1000",         0 },
	{ &g_blood,                     "com_blood",                    "1",            0 },
	{ &g_enableDust,                "g_enableDust",                 "0",            CVAR_SERVERINFO },
	{ &g_enableBreath,              "g_enableBreath",               "0",            CVAR_SERVERINFO },
	{ &g_spawn_health,           	"g_spawn_health",            	"100",          CVAR_NORESTART },
	{ &g_spawn_armor,            	"g_spawn_armor",             	"0",            CVAR_NORESTART },
	{ &g_spawn_gauntlet,         	"g_spawn_gauntlet",          	"1",            CVAR_NORESTART },
	{ &g_spawn_machinegun,       	"g_spawn_machinegun",        	"500",          CVAR_NORESTART },
	{ &g_spawn_shotgun,          	"g_spawn_shotgun",           	"0",            CVAR_NORESTART },
	{ &g_spawn_grenade,          	"g_spawn_grenade",           	"0",            CVAR_NORESTART },
	{ &g_spawn_rocket,           	"g_spawn_rocket",            	"0",            CVAR_NORESTART },
	{ &g_spawn_lightning,        	"g_spawn_lightning",         	"0",            CVAR_NORESTART },
	{ &g_spawn_railgun,          	"g_spawn_railgun",           	"0",            CVAR_NORESTART },
	{ &g_spawn_plasmagun,        	"g_spawn_plasmagun",         	"0",            CVAR_NORESTART },
	{ &g_spawn_bfg,              	"g_spawn_bfg",               	"0",            CVAR_NORESTART },
    { &g_spawn_grapple,          	"g_spawn_grapple",           	"0",            CVAR_NORESTART },
	{ &g_spawn_nail,             	"g_spawn_nail",              	"0",            CVAR_NORESTART },
	{ &g_spawn_mine,             	"g_spawn_mine",              	"0",            CVAR_NORESTART },
	{ &g_spawn_chain,            	"g_spawn_chain",             	"0",            CVAR_NORESTART },
	{ &g_spawn_flame,            	"g_spawn_flame",             	"0",            CVAR_NORESTART },
	{ &g_spawn_antimatter,       	"g_spawn_antimatter",        	"0",            CVAR_NORESTART },
	{ &g_spawn_quad,             	"g_spawn_quad",              	"0",            CVAR_NORESTART },
	{ &g_spawn_haste,            	"g_spawn_haste",             	"0",            CVAR_NORESTART },
	{ &g_spawn_bsuit,            	"g_spawn_bsuit",             	"0",            CVAR_NORESTART },
	{ &g_spawn_invis,            	"g_spawn_invis",             	"0",            CVAR_NORESTART },
	{ &g_spawn_regen,            	"g_spawn_regen",             	"0",            CVAR_NORESTART },
	{ &g_spawn_flight,           	"g_spawn_flight",            	"0",            CVAR_NORESTART },
	{ &g_spawn_holdable,         	"g_spawn_holdable",          	"0",            CVAR_NORESTART },
	{ &sv_fps,                      "sv_fps",                    	"60",           CVAR_SYSTEMINFO|CVAR_ARCHIVE }
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
