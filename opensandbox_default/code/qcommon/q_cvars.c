// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// OpenSandbox — GPLv2; see LICENSE for details.

#include "q_shared.h"

typedef struct {
	char		*cvarName;
	char		*defaultString;
	int			cvarFlags;
} cvarTable_t;

cvar_t cvarStorage[MAX_CVARS];

static cvarTable_t cvarTable[] = {
//game.qvm
	{ "gamename",                   "opensandbox",  0 },
	{ "gamedate",                   __DATE__,       0 },
	{ "sv_cheats",                  "0",            0 },
	{ "g_maxClients",               "128",          CVAR_SERVERINFO|CVAR_ARCHIVE },
	{ "g_maxEntities",              "4096",         CVAR_SERVERINFO|CVAR_ARCHIVE },
	{ "fraglimit",                  "20",           CVAR_SERVERINFO|CVAR_ARCHIVE },
	{ "timelimit",                  "0",            CVAR_SERVERINFO|CVAR_ARCHIVE },
	{ "capturelimit",               "8",            CVAR_SERVERINFO|CVAR_ARCHIVE },
	{ "g_gametype",                 "0",            CVAR_SERVERINFO|CVAR_USERINFO|CVAR_LATCH },
	{ "g_entitypack",               "default",      CVAR_ARCHIVE },
	{ "g_jumpheight",               "270",          0 },
	{ "g_friendlyFire",             "0",            CVAR_ARCHIVE },
	{ "g_password",                 "",             CVAR_USERINFO },
	{ "dedicated",                  "0",            0 },
	{ "g_speed",                    "320",          CVAR_ARCHIVE },
	{ "g_gravity",                  "800",          CVAR_ARCHIVE },
	{ "g_knockback",                "1000",         0 },
	{ "com_blood",                  "1",            0 },
	{ "g_enableDust",               "0",            CVAR_SERVERINFO },
	{ "g_enableBreath",             "0",            CVAR_SERVERINFO },
	{ "sv_fps",                     "60",           CVAR_SYSTEMINFO|CVAR_ARCHIVE },
//cgame.qvm
    { "headR",                        "100",          CVAR_USERINFO|CVAR_ARCHIVE },
    { "headG",                        "100",          CVAR_USERINFO|CVAR_ARCHIVE },
    { "headB",                        "100",          CVAR_USERINFO|CVAR_ARCHIVE },
    { "modelR",                       "100",          CVAR_USERINFO|CVAR_ARCHIVE },
    { "modelG",                       "100",          CVAR_USERINFO|CVAR_ARCHIVE },
    { "modelB",                       "100",          CVAR_USERINFO|CVAR_ARCHIVE },
    { "legsR",                        "100",          CVAR_USERINFO|CVAR_ARCHIVE },
    { "legsG",                        "100",          CVAR_USERINFO|CVAR_ARCHIVE },
    { "legsB",                        "100",          CVAR_USERINFO|CVAR_ARCHIVE },
    { "physR",                        "128",          CVAR_USERINFO|CVAR_ARCHIVE },
    { "physG",                        "225",          CVAR_USERINFO|CVAR_ARCHIVE },
    { "physB",                        "255",          CVAR_USERINFO|CVAR_ARCHIVE },
	{ "cg_effectsTime",               "60",           CVAR_ARCHIVE },
	{ "spawn_cmd",                 	"0",            0 },
	{ "toolgun_cmd",                 	"0",            0 },
	{ "toolgun_mod1",                 "0",            0 },
	{ "toolgun_mod2",                 "0",            0 },
	{ "toolgun_mod3",                 "0",            0 },
	{ "toolgun_mod4",                 "0",            0 },
	{ "toolgun_mod5",                 "0",            0 },
	{ "toolgun_tool",                 "0",            CVAR_USERINFO },
	{ "toolgun_tooltext",             "",             0 },
	{ "toolgun_tooltip1",             "",             0 },
	{ "toolgun_tooltip2",             "",             0 },
	{ "toolgun_tooltip3",             "",             0 },
	{ "toolgun_tooltip4",             "",             0 },
	{ "toolgun_toolmode1",            "",             0 },
	{ "toolgun_toolmode2",            "",             0 },
	{ "toolgun_toolmode3",            "",             0 },
	{ "toolgun_toolmode4",            "",             0 },
	{ "script.error",                 "0",            0 },
	{ "cg_postprocess",               "",             0 },
	{ "cg_drawGun",                   "1",            CVAR_ARCHIVE },
	{ "cg_zoomfov",                   "22",           CVAR_ARCHIVE },
	{ "cg_fov",                       "110",          CVAR_ARCHIVE },
	{ "cg_shadows",                   "1",            CVAR_ARCHIVE },
	{ "cg_draw2D",                    "1",            CVAR_ARCHIVE },
	{ "cg_drawTimer",                 "0",            CVAR_ARCHIVE },
	{ "cg_drawFPS",                   "0",            CVAR_ARCHIVE },
	{ "cg_drawSpeed",                 "0",            CVAR_ARCHIVE },
	{ "cg_drawCrosshair",             "4",            CVAR_ARCHIVE },
	{ "cg_crosshairScale",            "24",           CVAR_ARCHIVE },
	{ "cg_addMarks",                  "1",            CVAR_ARCHIVE },
	{ "cg_enableBobbing",            	"1",            CVAR_ARCHIVE },
	{ "cg_thirdPersonRange",          "65",           CVAR_ARCHIVE },
	{ "cg_thirdPersonOffset",         "25",           CVAR_ARCHIVE },
	{ "cg_thirdPerson",               "0",            CVAR_ARCHIVE},
	{ "com_blood",                    "1",            CVAR_ARCHIVE },
	{ "g_enableDust",                 "0",            CVAR_SERVERINFO },
	{ "g_enableBreath",               "0",            CVAR_SERVERINFO },
	{ "cg_cameraEyes",                "0",            CVAR_ARCHIVE },
	{ "cg_cameraEyes_Fwd",            "-2",            CVAR_ARCHIVE },
	{ "cg_cameraEyes_Up",             "7",            CVAR_ARCHIVE },
	{ "sv_fps",                       "60",           CVAR_SYSTEMINFO },
	{ "cg_crosshairColorRed",         "0.5",          CVAR_ARCHIVE|CVAR_USERINFO },
    { "cg_crosshairColorGreen",       "0.75",         CVAR_ARCHIVE|CVAR_USERINFO },
    { "cg_crosshairColorBlue",        "1.0",          CVAR_ARCHIVE|CVAR_USERINFO },
//ui.qvm
	{ "spawn_preset",                 "spawn_cmd = sl prop props/%s %s 0 25 25 0 %s 1 1 \"none\" -1 0 0 0 0 1 1 1 1 0.40 1.00",  0 },
	{ "toolgun_toolset1",             "--------:",    0 },
	{ "toolgun_toolset2",             "--------:",    0 },
	{ "toolgun_toolset3",             "--------:",    0 },
	{ "toolgun_toolset4",             "--------:",    0 },
	{ "toolgun_tooltext",             "",             0 },
	{ "cl_selectedmod",               "default",      CVAR_ARCHIVE },
};

static int cvarTableSize = ARRAY_SIZE(cvarTable);

void ST_RegisterCvars(void) {
	cvarTable_t	*cv;
    int			i;

    //Add cvartable
	for (i = 0, cv = cvarTable; i < cvarTableSize; i++, cv++) {
		cvarRegister( cv->cvarName, cv->defaultString, cv->cvarFlags );
	}
	cvarReload();
	ST_UpdateCvars();
}

void ST_UpdateCvars(void) {
    int			i;

	for ( i = 0; i < MAX_CVARS ; i++ ) {
		cvarUpdate( &cvarStorage[i], i );
	}
}

//Debug: trap_Print(va("%i:%s = %i\n", id, name, cvarStorage[id].integer));

int cvarInt(const char *name) {
    int id = cvarID(name);
    if (id == -1) return 0;
    cvarUpdate(&cvarStorage[id], id);
    return cvarStorage[id].integer;
}

float cvarFloat(const char *name) {
    int id = cvarID(name);
    if (id == -1) return 0.0f;
    cvarUpdate(&cvarStorage[id], id);
    return cvarStorage[id].value;
}

char* cvarString(const char *name) {
    int id = cvarID(name);
    if (id == -1) return "0";
    cvarUpdate(&cvarStorage[id], id);
    return cvarStorage[id].string;
}
