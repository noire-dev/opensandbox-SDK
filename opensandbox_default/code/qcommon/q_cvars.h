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
} cvar_t;

#ifdef GAME
extern  cvar_t    sv_cheats;
extern  cvar_t    g_maxClients;
extern  cvar_t    g_maxEntities;
extern  cvar_t    g_fraglimit;
extern  cvar_t    g_timelimit;
extern  cvar_t    g_capturelimit;
extern  cvar_t    g_gametype;
extern  cvar_t    g_entitypack;
extern  cvar_t    g_extendedsandbox;
extern  cvar_t    g_randomItems;
extern  cvar_t    g_jumpheight;
extern  cvar_t    g_friendlyFire;
extern  cvar_t    g_password;
extern  cvar_t    g_dedicated;
extern  cvar_t    g_speed;
extern  cvar_t    g_gravity;
extern  cvar_t    g_knockback;
extern  cvar_t    g_blood;
extern  cvar_t    g_enableDust;
extern  cvar_t    g_enableBreath;
extern  cvar_t    g_spawn_health;
extern  cvar_t    g_spawn_armor;
extern  cvar_t    g_spawn_gauntlet;
extern  cvar_t    g_spawn_machinegun;
extern  cvar_t    g_spawn_shotgun;
extern  cvar_t    g_spawn_grenade;
extern  cvar_t    g_spawn_rocket;
extern  cvar_t    g_spawn_lightning;
extern  cvar_t    g_spawn_railgun;
extern  cvar_t    g_spawn_plasmagun;
extern  cvar_t    g_spawn_bfg;
extern  cvar_t    g_spawn_grapple;
extern  cvar_t    g_spawn_nail;
extern  cvar_t    g_spawn_mine;
extern  cvar_t    g_spawn_chain;
extern  cvar_t    g_spawn_flame;
extern  cvar_t    g_spawn_antimatter;
extern  cvar_t    g_spawn_quad;
extern  cvar_t    g_spawn_haste;
extern  cvar_t    g_spawn_bsuit;
extern  cvar_t    g_spawn_invis;
extern  cvar_t    g_spawn_regen;
extern  cvar_t    g_spawn_flight;
extern  cvar_t    g_spawn_holdable;
extern  cvar_t    sv_fps;
#endif

#ifdef CGAME
extern  cvar_t    g_gametype;
extern  cvar_t    headR;
extern  cvar_t    headG;
extern  cvar_t    headB;
extern  cvar_t    modelR;
extern  cvar_t    modelG;
extern  cvar_t    modelB;
extern  cvar_t    legsR;
extern  cvar_t    legsG;
extern  cvar_t    legsB;
extern  cvar_t    physR;
extern  cvar_t    physG;
extern  cvar_t    physB;
extern  cvar_t    cg_effectsTime;
extern  cvar_t    cg_effectsLimit;
extern  cvar_t    cg_effectsGibs;
extern  cvar_t    toolgun_mod1;
extern  cvar_t    toolgun_mod2;
extern  cvar_t    toolgun_mod3;
extern  cvar_t    toolgun_mod4;
extern  cvar_t    toolgun_mod5;
extern  cvar_t    toolgun_mod6;
extern  cvar_t    toolgun_mod7;
extern  cvar_t    toolgun_mod8;
extern  cvar_t    toolgun_mod9;
extern  cvar_t    toolgun_mod10;
extern  cvar_t    toolgun_mod11;
extern  cvar_t    toolgun_mod12;
extern  cvar_t    toolgun_mod13;
extern  cvar_t    toolgun_mod14;
extern  cvar_t    toolgun_mod15;
extern  cvar_t    toolgun_mod16;
extern  cvar_t    toolgun_mod17;
extern  cvar_t    toolgun_mod18;
extern  cvar_t    toolgun_mod19;
extern  cvar_t    toolgun_tool;
extern  cvar_t    toolgun_toolcmd1;
extern  cvar_t    toolgun_toolcmd2;
extern  cvar_t    toolgun_toolcmd3;
extern  cvar_t    toolgun_toolcmd4;
extern  cvar_t    toolgun_tooltext;
extern  cvar_t    toolgun_tooltip1;
extern  cvar_t    toolgun_tooltip2;
extern  cvar_t    toolgun_tooltip3;
extern  cvar_t    toolgun_tooltip4;
extern  cvar_t    toolgun_toolmode1;
extern  cvar_t    toolgun_toolmode2;
extern  cvar_t    toolgun_toolmode3;
extern  cvar_t    toolgun_toolmode4;
extern  cvar_t    cg_hide255;
extern  cvar_t    ns_haveerror;
extern  cvar_t    cg_postprocess;
extern  cvar_t    cl_language;
extern  cvar_t    cg_disableBobbing;
extern  cvar_t    cg_shadows;
extern  cvar_t    cg_drawTimer;
extern  cvar_t    cg_drawFPS;
extern  cvar_t    cg_drawCrosshair;
extern  cvar_t    cg_crosshairScale;
extern  cvar_t    cg_draw2D;
extern  cvar_t    cg_debugEvents;
extern  cvar_t    cg_addMarks;
extern  cvar_t    cg_drawGun;
extern  cvar_t    cg_fov;
extern  cvar_t    cg_zoomFov;
extern  cvar_t    cg_thirdPerson;
extern  cvar_t    cg_thirdPersonRange;
extern  cvar_t    cg_thirdPersonOffset;
extern  cvar_t    cg_drawSpeed;
extern  cvar_t    cg_paused;
extern  cvar_t    cg_blood;
extern  cvar_t    cg_drawFriend;
extern  cvar_t    cg_cameraEyes;
extern  cvar_t    cg_cameraEyes_Fwd;
extern  cvar_t    cg_cameraEyes_Up;
extern  cvar_t    cg_noProjectileTrail;
extern  cvar_t    cg_enableDust;
extern  cvar_t    cg_enableBreath;
extern  cvar_t    sv_fps;
extern  cvar_t    cg_atmosphericLevel;
extern  cvar_t    cg_crosshairColorRed;
extern  cvar_t    cg_crosshairColorGreen;
extern  cvar_t    cg_crosshairColorBlue;
extern  cvar_t    cg_chatBeep;
extern  cvar_t    cg_teamChatBeep;
#endif

#ifdef UI
extern  cvar_t    sb_private;
extern  cvar_t    sb_texture;
extern  cvar_t    sb_grid;
extern  cvar_t    sb_modelnum;
extern  cvar_t    sb_classnum;
extern  cvar_t    sb_texturenum;
extern  cvar_t    sb_tab;
extern  cvar_t    spawn_preset;
extern  cvar_t    tool_spawnpreset;
extern  cvar_t    tool_modifypreset;
extern  cvar_t    tool_modifypreset2;
extern  cvar_t    tool_modifypreset3;
extern  cvar_t    tool_modifypreset4;
extern  cvar_t    sb_ctab_1;
extern  cvar_t    sb_ctab_2;
extern  cvar_t    sb_ctab_3;
extern  cvar_t    sb_ctab_4;
extern  cvar_t    sb_ctab_5;
extern  cvar_t    sb_ctab_6;
extern  cvar_t    sb_ctab_7;
extern  cvar_t    sb_ctab_8;
extern  cvar_t    sb_ctab_9;
extern  cvar_t    sb_ctab_10;
extern  cvar_t    toolgun_toolset1;
extern  cvar_t    toolgun_toolset2;
extern  cvar_t    toolgun_toolset3;
extern  cvar_t    toolgun_toolset4;
extern  cvar_t    toolgun_toolset5;
extern  cvar_t    toolgun_toolset6;
extern  cvar_t    toolgun_toolset7;
extern  cvar_t    toolgun_toolset8;
extern  cvar_t    toolgun_toolset9;
extern  cvar_t    toolgun_toolset10;
extern  cvar_t    toolgun_toolset11;
extern  cvar_t    toolgun_toolset12;
extern  cvar_t    toolgun_toolset13;
extern  cvar_t    toolgun_toolset14;
extern  cvar_t    toolgun_toolset15;
extern  cvar_t    toolgun_toolset16;
extern  cvar_t    toolgun_toolset17;
extern  cvar_t    toolgun_toolset18;
extern  cvar_t    toolgun_disabledarg1;
extern  cvar_t    toolgun_disabledarg2;
extern  cvar_t    toolgun_disabledarg3;
extern  cvar_t    toolgun_disabledarg4;
extern  cvar_t    sbt_color0_0;
extern  cvar_t    sbt_color0_1;
extern  cvar_t    sbt_color0_2;
extern  cvar_t    sbt_color0_3;
extern  cvar_t    sbt_color1_0;
extern  cvar_t    sbt_color1_1;
extern  cvar_t    sbt_color1_2;
extern  cvar_t    sbt_color1_3;
extern  cvar_t    sbt_color2_0;
extern  cvar_t    sbt_color2_1;
extern  cvar_t    sbt_color2_2;
extern  cvar_t    sbt_color2_3;
extern  cvar_t    sbt_color3_0;
extern  cvar_t    sbt_color3_1;
extern  cvar_t    sbt_color3_2;
extern  cvar_t    sbt_color3_3;
extern  cvar_t    sbt_wallpaper;
extern  cvar_t    ui_3dmap;
extern  cvar_t    ui_effectslevel;
extern  cvar_t    cl_selectedmod;
extern  cvar_t    cl_language;
extern  cvar_t    ui_loaded;
extern  cvar_t    sensitivitymenu;
extern  cvar_t    ui_browserMaster;
extern  cvar_t    ui_browserGameType;
extern  cvar_t    ui_browserSortKey;
extern  cvar_t    ui_browserShowFull;
extern  cvar_t    ui_browserShowEmpty;
extern  cvar_t    ui_brassTime;
extern  cvar_t    ui_drawCrosshair;
extern  cvar_t    ui_drawCrosshairNames;
extern  cvar_t    ui_marks;
extern  cvar_t    ui_server1;
extern  cvar_t    ui_server2;
extern  cvar_t    ui_server3;
extern  cvar_t    ui_server4;
extern  cvar_t    ui_server5;
extern  cvar_t    ui_server6;
extern  cvar_t    ui_server7;
extern  cvar_t    ui_server8;
extern  cvar_t    ui_server9;
extern  cvar_t    ui_server10;
extern  cvar_t    ui_server11;
extern  cvar_t    ui_server12;
extern  cvar_t    ui_server13;
extern  cvar_t    ui_server14;
extern  cvar_t    ui_server15;
extern  cvar_t    ui_server16;
extern  cvar_t    ui_server17;
extern  cvar_t    ui_server18;
extern  cvar_t    ui_server19;
extern  cvar_t    ui_server20;
extern  cvar_t    ui_server21;
extern  cvar_t    ui_server22;
extern  cvar_t    ui_server23;
extern  cvar_t    ui_server24;
extern  cvar_t    ui_server25;
extern  cvar_t    ui_server26;
extern  cvar_t    ui_server27;
extern  cvar_t    ui_server28;
extern  cvar_t    ui_server29;
extern  cvar_t    ui_server30;
extern  cvar_t    ui_server31;
extern  cvar_t    ui_server32;
extern  cvar_t    gui_animsfx;
extern  cvar_t    gui_mapicons;
extern  cvar_t    gui_autoclosebotmenu;
extern  cvar_t    gui_map_multisel;
extern  cvar_t    gui_map_list;
extern  cvar_t    gui_bot_multisel;
extern  cvar_t    gui_bot_list;
#endif

void ST_RegisterCvars(void);
void ST_UpdateCvars(void);

#endif
