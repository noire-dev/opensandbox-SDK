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
#ifndef __Q_CVARS_H
#define __Q_CVARS_H

#define	MAX_CVAR_VALUE_STRING	256

typedef int	cvarHandle_t;

typedef struct {
	cvarHandle_t	handle;
	int			    modificationCount;
	float		    value;
	int			    integer;
	char		    string[MAX_CVAR_VALUE_STRING];
} vmCvar_t;

#ifdef GAME
extern  vmCvar_t    sv_cheats;
extern  vmCvar_t    g_maxClients;
extern  vmCvar_t    g_maxEntities;
extern  vmCvar_t    g_fraglimit;
extern  vmCvar_t    g_timelimit;
extern  vmCvar_t    g_capturelimit;
extern  vmCvar_t    g_gametype;
extern  vmCvar_t    g_entitypack;
extern  vmCvar_t    g_extendedsandbox;
extern  vmCvar_t    g_randomItems;
extern  vmCvar_t    g_jumpheight;
extern  vmCvar_t    g_friendlyFire;
extern  vmCvar_t    g_password;
extern  vmCvar_t    g_dedicated;
extern  vmCvar_t    g_speed;
extern  vmCvar_t    g_gravity;
extern  vmCvar_t    g_knockback;
extern  vmCvar_t    g_blood;
extern  vmCvar_t    g_enableDust;
extern  vmCvar_t    g_enableBreath;
extern  vmCvar_t    g_spawn_health;
extern  vmCvar_t    g_spawn_armor;
extern  vmCvar_t    g_spawn_gauntlet;
extern  vmCvar_t    g_spawn_machinegun;
extern  vmCvar_t    g_spawn_shotgun;
extern  vmCvar_t    g_spawn_grenade;
extern  vmCvar_t    g_spawn_rocket;
extern  vmCvar_t    g_spawn_lightning;
extern  vmCvar_t    g_spawn_railgun;
extern  vmCvar_t    g_spawn_plasmagun;
extern  vmCvar_t    g_spawn_bfg;
extern  vmCvar_t    g_spawn_grapple;
extern  vmCvar_t    g_spawn_nail;
extern  vmCvar_t    g_spawn_mine;
extern  vmCvar_t    g_spawn_chain;
extern  vmCvar_t    g_spawn_flame;
extern  vmCvar_t    g_spawn_antimatter;
extern  vmCvar_t    g_spawn_quad;
extern  vmCvar_t    g_spawn_haste;
extern  vmCvar_t    g_spawn_bsuit;
extern  vmCvar_t    g_spawn_invis;
extern  vmCvar_t    g_spawn_regen;
extern  vmCvar_t    g_spawn_flight;
extern  vmCvar_t    g_spawn_holdable;
extern  vmCvar_t    sv_fps;
#endif

#ifdef CGAME
extern  vmCvar_t    g_gametype;
extern  vmCvar_t    headR;
extern  vmCvar_t    headG;
extern  vmCvar_t    headB;
extern  vmCvar_t    modelR;
extern  vmCvar_t    modelG;
extern  vmCvar_t    modelB;
extern  vmCvar_t    legsR;
extern  vmCvar_t    legsG;
extern  vmCvar_t    legsB;
extern  vmCvar_t    physR;
extern  vmCvar_t    physG;
extern  vmCvar_t    physB;
extern  vmCvar_t    cg_effectsTime;
extern  vmCvar_t    cg_effectsLimit;
extern  vmCvar_t    cg_effectsGibs;
extern  vmCvar_t    toolgun_mod1;
extern  vmCvar_t    toolgun_mod2;
extern  vmCvar_t    toolgun_mod3;
extern  vmCvar_t    toolgun_mod4;
extern  vmCvar_t    toolgun_mod5;
extern  vmCvar_t    toolgun_mod6;
extern  vmCvar_t    toolgun_mod7;
extern  vmCvar_t    toolgun_mod8;
extern  vmCvar_t    toolgun_mod9;
extern  vmCvar_t    toolgun_mod10;
extern  vmCvar_t    toolgun_mod11;
extern  vmCvar_t    toolgun_mod12;
extern  vmCvar_t    toolgun_mod13;
extern  vmCvar_t    toolgun_mod14;
extern  vmCvar_t    toolgun_mod15;
extern  vmCvar_t    toolgun_mod16;
extern  vmCvar_t    toolgun_mod17;
extern  vmCvar_t    toolgun_mod18;
extern  vmCvar_t    toolgun_mod19;
extern  vmCvar_t    toolgun_tool;
extern  vmCvar_t    toolgun_toolcmd1;
extern  vmCvar_t    toolgun_toolcmd2;
extern  vmCvar_t    toolgun_toolcmd3;
extern  vmCvar_t    toolgun_toolcmd4;
extern  vmCvar_t    toolgun_tooltext;
extern  vmCvar_t    toolgun_tooltip1;
extern  vmCvar_t    toolgun_tooltip2;
extern  vmCvar_t    toolgun_tooltip3;
extern  vmCvar_t    toolgun_tooltip4;
extern  vmCvar_t    toolgun_toolmode1;
extern  vmCvar_t    toolgun_toolmode2;
extern  vmCvar_t    toolgun_toolmode3;
extern  vmCvar_t    toolgun_toolmode4;
extern  vmCvar_t    cg_hide255;
extern  vmCvar_t    ns_haveerror;
extern  vmCvar_t    cg_postprocess;
extern  vmCvar_t    cl_language;
extern  vmCvar_t    cg_disableBobbing;
extern  vmCvar_t    cg_shadows;
extern  vmCvar_t    cg_drawTimer;
extern  vmCvar_t    cg_drawFPS;
extern  vmCvar_t    cg_drawCrosshair;
extern  vmCvar_t    cg_crosshairScale;
extern  vmCvar_t    cg_draw2D;
extern  vmCvar_t    cg_debugEvents;
extern  vmCvar_t    cg_addMarks;
extern  vmCvar_t    cg_drawGun;
extern  vmCvar_t    cg_fov;
extern  vmCvar_t    cg_zoomFov;
extern  vmCvar_t    cg_thirdPerson;
extern  vmCvar_t    cg_thirdPersonRange;
extern  vmCvar_t    cg_thirdPersonOffset;
extern  vmCvar_t    cg_drawSpeed;
extern  vmCvar_t    cg_paused;
extern  vmCvar_t    cg_blood;
extern  vmCvar_t    cg_drawFriend;
extern  vmCvar_t    cg_cameraEyes;
extern  vmCvar_t    cg_cameraEyes_Fwd;
extern  vmCvar_t    cg_cameraEyes_Up;
extern  vmCvar_t    cg_noProjectileTrail;
extern  vmCvar_t    cg_enableDust;
extern  vmCvar_t    cg_enableBreath;
extern  vmCvar_t    sv_fps;
extern  vmCvar_t    cg_atmosphericLevel;
extern  vmCvar_t    cg_crosshairColorRed;
extern  vmCvar_t    cg_crosshairColorGreen;
extern  vmCvar_t    cg_crosshairColorBlue;
extern  vmCvar_t    cg_chatBeep;
extern  vmCvar_t    cg_teamChatBeep;
#endif

#ifdef UI
extern  vmCvar_t    sb_private;
extern  vmCvar_t    sb_texture;
extern  vmCvar_t    sb_grid;
extern  vmCvar_t    sb_modelnum;
extern  vmCvar_t    sb_classnum;
extern  vmCvar_t    sb_texturenum;
extern  vmCvar_t    sb_tab;
extern  vmCvar_t    spawn_preset;
extern  vmCvar_t    tool_spawnpreset;
extern  vmCvar_t    tool_modifypreset;
extern  vmCvar_t    tool_modifypreset2;
extern  vmCvar_t    tool_modifypreset3;
extern  vmCvar_t    tool_modifypreset4;
extern  vmCvar_t    sb_ctab_1;
extern  vmCvar_t    sb_ctab_2;
extern  vmCvar_t    sb_ctab_3;
extern  vmCvar_t    sb_ctab_4;
extern  vmCvar_t    sb_ctab_5;
extern  vmCvar_t    sb_ctab_6;
extern  vmCvar_t    sb_ctab_7;
extern  vmCvar_t    sb_ctab_8;
extern  vmCvar_t    sb_ctab_9;
extern  vmCvar_t    sb_ctab_10;
extern  vmCvar_t    toolgun_toolset1;
extern  vmCvar_t    toolgun_toolset2;
extern  vmCvar_t    toolgun_toolset3;
extern  vmCvar_t    toolgun_toolset4;
extern  vmCvar_t    toolgun_toolset5;
extern  vmCvar_t    toolgun_toolset6;
extern  vmCvar_t    toolgun_toolset7;
extern  vmCvar_t    toolgun_toolset8;
extern  vmCvar_t    toolgun_toolset9;
extern  vmCvar_t    toolgun_toolset10;
extern  vmCvar_t    toolgun_toolset11;
extern  vmCvar_t    toolgun_toolset12;
extern  vmCvar_t    toolgun_toolset13;
extern  vmCvar_t    toolgun_toolset14;
extern  vmCvar_t    toolgun_toolset15;
extern  vmCvar_t    toolgun_toolset16;
extern  vmCvar_t    toolgun_toolset17;
extern  vmCvar_t    toolgun_toolset18;
extern  vmCvar_t    toolgun_disabledarg1;
extern  vmCvar_t    toolgun_disabledarg2;
extern  vmCvar_t    toolgun_disabledarg3;
extern  vmCvar_t    toolgun_disabledarg4;
extern  vmCvar_t    sbt_color0_0;
extern  vmCvar_t    sbt_color0_1;
extern  vmCvar_t    sbt_color0_2;
extern  vmCvar_t    sbt_color0_3;
extern  vmCvar_t    sbt_color1_0;
extern  vmCvar_t    sbt_color1_1;
extern  vmCvar_t    sbt_color1_2;
extern  vmCvar_t    sbt_color1_3;
extern  vmCvar_t    sbt_color2_0;
extern  vmCvar_t    sbt_color2_1;
extern  vmCvar_t    sbt_color2_2;
extern  vmCvar_t    sbt_color2_3;
extern  vmCvar_t    sbt_color3_0;
extern  vmCvar_t    sbt_color3_1;
extern  vmCvar_t    sbt_color3_2;
extern  vmCvar_t    sbt_color3_3;
extern  vmCvar_t    sbt_wallpaper;
extern  vmCvar_t    ui_3dmap;
extern  vmCvar_t    ui_effectslevel;
extern  vmCvar_t    cl_selectedmod;
extern  vmCvar_t    cl_language;
extern  vmCvar_t    ui_loaded;
extern  vmCvar_t    sensitivitymenu;
extern  vmCvar_t    ui_browserMaster;
extern  vmCvar_t    ui_browserGameType;
extern  vmCvar_t    ui_browserSortKey;
extern  vmCvar_t    ui_browserShowFull;
extern  vmCvar_t    ui_browserShowEmpty;
extern  vmCvar_t    ui_brassTime;
extern  vmCvar_t    ui_drawCrosshair;
extern  vmCvar_t    ui_drawCrosshairNames;
extern  vmCvar_t    ui_marks;
extern  vmCvar_t    ui_server1;
extern  vmCvar_t    ui_server2;
extern  vmCvar_t    ui_server3;
extern  vmCvar_t    ui_server4;
extern  vmCvar_t    ui_server5;
extern  vmCvar_t    ui_server6;
extern  vmCvar_t    ui_server7;
extern  vmCvar_t    ui_server8;
extern  vmCvar_t    ui_server9;
extern  vmCvar_t    ui_server10;
extern  vmCvar_t    ui_server11;
extern  vmCvar_t    ui_server12;
extern  vmCvar_t    ui_server13;
extern  vmCvar_t    ui_server14;
extern  vmCvar_t    ui_server15;
extern  vmCvar_t    ui_server16;
extern  vmCvar_t    ui_server17;
extern  vmCvar_t    ui_server18;
extern  vmCvar_t    ui_server19;
extern  vmCvar_t    ui_server20;
extern  vmCvar_t    ui_server21;
extern  vmCvar_t    ui_server22;
extern  vmCvar_t    ui_server23;
extern  vmCvar_t    ui_server24;
extern  vmCvar_t    ui_server25;
extern  vmCvar_t    ui_server26;
extern  vmCvar_t    ui_server27;
extern  vmCvar_t    ui_server28;
extern  vmCvar_t    ui_server29;
extern  vmCvar_t    ui_server30;
extern  vmCvar_t    ui_server31;
extern  vmCvar_t    ui_server32;
extern  vmCvar_t    gui_animsfx;
extern  vmCvar_t    gui_mapicons;
extern  vmCvar_t    gui_autoclosebotmenu;
extern  vmCvar_t    gui_map_multisel;
extern  vmCvar_t    gui_map_list;
extern  vmCvar_t    gui_bot_multisel;
extern  vmCvar_t    gui_bot_list;
#endif

void ST_RegisterCvars(void);
void ST_UpdateCvars(void);

#endif
