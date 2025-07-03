// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

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
extern  cvar_t    g_spawn_machinegun;
extern  cvar_t    g_spawn_shotgun;
extern  cvar_t    g_spawn_grenade;
extern  cvar_t    g_spawn_rocket;
extern  cvar_t    g_spawn_lightning;
extern  cvar_t    g_spawn_railgun;
extern  cvar_t    g_spawn_plasmagun;
extern  cvar_t    g_spawn_bfg;
extern  cvar_t    g_spawn_grapple;
extern  cvar_t    g_spawn_nailgun;
extern  cvar_t    g_spawn_prox;
extern  cvar_t    g_spawn_chaingun;
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
extern  cvar_t    spawn_cmd;
extern  cvar_t    toolgun_cmd;
extern  cvar_t    toolgun_mod1;
extern  cvar_t    toolgun_mod2;
extern  cvar_t    toolgun_mod3;
extern  cvar_t    toolgun_mod4;
extern  cvar_t    toolgun_mod5;
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
extern  cvar_t    ns_haveerror;
extern  cvar_t    cg_postprocess;
extern  cvar_t    cg_enableBobbing;
extern  cvar_t    cg_shadows;
extern  cvar_t    cg_drawTimer;
extern  cvar_t    cg_drawFPS;
extern  cvar_t    cg_drawCrosshair;
extern  cvar_t    cg_crosshairScale;
extern  cvar_t    cg_draw2D;
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
extern  cvar_t    cg_cameraEyes;
extern  cvar_t    cg_cameraEyes_Fwd;
extern  cvar_t    cg_cameraEyes_Up;
extern  cvar_t    cg_noProjectileTrail;
extern  cvar_t    cg_enableDust;
extern  cvar_t    cg_enableBreath;
extern  cvar_t    sv_fps;
extern  cvar_t    cg_crosshairColorRed;
extern  cvar_t    cg_crosshairColorGreen;
extern  cvar_t    cg_crosshairColorBlue;
#endif

#ifdef UI
extern  cvar_t    spawn_preset;
extern  cvar_t    toolgun_toolset1;
extern  cvar_t    toolgun_toolset2;
extern  cvar_t    toolgun_toolset3;
extern  cvar_t    toolgun_toolset4;
extern  cvar_t    toolgun_tooltext;
extern  cvar_t    ui_effectslevel;
extern  cvar_t    cl_selectedmod;
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
