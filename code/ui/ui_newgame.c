// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// OpenSandbox — GPLv2; see LICENSE for details.

#include "../qcommon/js_local.h"

#define BROWSERGRID_SIZE 100

#define ID_GAMETYPE_SELECT 50
#define ID_MODE_SELECT (ID_GAMETYPE_SELECT + GT_MAX_GAME_TYPE)
#define ID_GAMETYPE_ALLMAPS 100

typedef struct {
	menuframework_s menu;
	menuelement_s e[OSUI_MAX_ELEMENTS];

	char names1[524288];
	char *list1[524288];
} newgame_t;

typedef enum {
	NTB_NONE,
	NTB_ADDONS,
	NTB_DEMOS,

	NTB_MAX
} newgame_tabs_t;

char *newgame_tabnames[] = {
    "None",
    "Addons",
    "Demos",
};

static newgame_t newgame;
int newgame_gametype = 0;
qboolean newgame_allmaps = qfalse;
int newgame_mode = 0;

static void NewGame_Event(void *ptr, int event) {
	if(event != QM_ACTIVATED) return;

	if(((menucommon_s *)ptr)->callid == 0) {
		if(newgame_mode == NTB_NONE) trap_Cmd(EXEC_INSERT, va("g_gametype = %i; map %s", newgame_gametype, newgame.e[0].itemnames[newgame.e[0].curvalue]));
		if(newgame_mode == NTB_ADDONS) trap_Cmd(EXEC_INSERT, va("exec addons/%s", newgame.e[0].itemnames[newgame.e[0].curvalue]));
		if(newgame_mode == NTB_DEMOS) trap_Cmd(EXEC_INSERT, va("demo %s.demo", newgame.e[0].itemnames[newgame.e[0].curvalue]));
	}

	if(((menucommon_s *)ptr)->callid == ID_GAMETYPE_ALLMAPS) {
		newgame_allmaps = !newgame_allmaps;
		newgame_mode = NTB_NONE;
		UI_NewGame();
	}

	if(((menucommon_s *)ptr)->callid >= ID_GAMETYPE_SELECT && ((menucommon_s *)ptr)->callid <= ID_GAMETYPE_SELECT + GT_MAX_GAME_TYPE) {
		newgame_gametype = ((menucommon_s *)ptr)->callid - ID_GAMETYPE_SELECT;
		newgame_mode = NTB_NONE;
		UI_NewGame();
	}

	if(((menucommon_s *)ptr)->callid >= ID_MODE_SELECT && ((menucommon_s *)ptr)->callid <= ID_MODE_SELECT + NTB_MAX) {
		newgame_mode = ((menucommon_s *)ptr)->callid - ID_MODE_SELECT;
		UI_NewGame();
	}
}

static void NewGame_Draw(void) {
	int y, i;
	vec4_t color = {1.00, 1.00, 1.00, 1.00};
	vec4_t color2 = {0.85, 0.90, 1.00, 0.20};
	vec4_t color2all = {0.35, 0.25, 0.30, 1.00};
	vec4_t color2allselected = {0.50, 0.20, 0.30, 1.00};
	vec4_t color2mode = {0.25, 0.45, 0.30, 1.00};
	vec4_t color3 = {0.60, 0.75, 1.00, 0.40};
	vec4_t color4 = {0.50, 0.60, 0.80, 1.00};

	ST_DrawRoundedRect(10 - cgui.wideoffset, 10, 440 + cgui.wideoffset * 2, 480 - 38, 4, color_dim);
	ST_DrawRoundedRect(640 + cgui.wideoffset - 180, 10, 170, 480 - 38, 4, color_dim);

	y = 15;
	for(i = 0; i < GT_MAX_GAME_TYPE; i++) {
		if(i == newgame_gametype) {
			ST_DrawRoundedRect(15 - cgui.wideoffset, y, 125, 16, 3, color3);
		} else {
			ST_DrawRoundedRect(15 - cgui.wideoffset, y, 125, 16, 3, color2);
		}
		y += 20;
	}

	y += 20;
	if(newgame_allmaps) {
		ST_DrawRoundedRect(15 - cgui.wideoffset, y, 125, 16, 3, color2allselected);
	} else {
		ST_DrawRoundedRect(15 - cgui.wideoffset, y, 125, 16, 3, color2all);
	}
	y += 40;

	for(i = 1; i < NTB_MAX; i++) {
		ST_DrawRoundedRect(15 - cgui.wideoffset, y, 125, 16, 3, color2mode);
		y += 20;
	}

	if(newgame_mode == NTB_NONE) {
		ST_DrawString(148 - cgui.wideoffset, 20, gametypes_names[newgame_gametype], UI_LEFT, color_white, 1.50);
		ST_DrawString(152 + (BASEFONT_INDENT * ST_StringCount(gametypes_names[newgame_gametype]) * 1.50) - cgui.wideoffset, 26, va("%i maps", newgame.e[0].numitems), UI_LEFT, color_grey, 0.80);
	} else {
		ST_DrawString(148 - cgui.wideoffset, 20, newgame_tabnames[newgame_mode], UI_LEFT, color_white, 1.50);
		ST_DrawString(152 + (BASEFONT_INDENT * ST_StringCount(newgame_tabnames[newgame_mode]) * 1.50) - cgui.wideoffset, 26, va("%i items", newgame.e[0].numitems), UI_LEFT, color_grey, 0.80);
	}

	ST_DrawRoundedRect(640 + cgui.wideoffset - 175, 15, 160, 16, 3, color3);
	ST_DrawRoundedRect(640 + cgui.wideoffset - 155, 410, 120, 32, 3, color4);

	Menu_Draw(&newgame.menu);
}

void UI_NewGame(void) {
	int y, i;

	memset(&newgame, 0, sizeof(newgame_t));
	newgame.menu.draw = NewGame_Draw;
	newgame.menu.fullscreen = qtrue;

	if(newgame_mode == NTB_DEMOS) {
		UI_CList(&newgame.e[0], 150 - cgui.wideoffset, 42, (((10 - cgui.wideoffset + 440 + cgui.wideoffset * 2) - 150 + cgui.wideoffset) / BASEFONT_INDENT) - 2, 36, 1, 0, 0, LST_SIMPLE, qfalse, 0, color_white, NewGame_Event, 0);
	} else {
		UI_CList(&newgame.e[0], 150 - cgui.wideoffset, 42, BROWSERGRID_SIZE, 4, ((10 - cgui.wideoffset + 440 + cgui.wideoffset * 2) - 150 + cgui.wideoffset) / BROWSERGRID_SIZE, 4, 10, LST_GRID, qtrue, 4, color_white, NewGame_Event, 0);
	}
	if(newgame_mode == NTB_NONE) {
		if(newgame_allmaps) {
			UI_FillListOfMaps(&newgame.e[0], "all", newgame.names1, sizeof(newgame.names1), newgame.list1);
		} else {
			UI_FillListOfMaps(&newgame.e[0], gametypes_mapnames[newgame_gametype], newgame.names1, sizeof(newgame.names1), newgame.list1);
		}
	}
	if(newgame_mode == NTB_ADDONS) {
		UI_FillList(&newgame.e[0], "addons", "addons", ".cfg", newgame.names1, sizeof(newgame.names1), newgame.list1);
	}
	if(newgame_mode == NTB_DEMOS) {
		UI_FillList(&newgame.e[0], "demos", "demos", ".demo", newgame.names1, sizeof(newgame.names1), newgame.list1);
	}

	y = 15;
	for(i = 0; i < GT_MAX_GAME_TYPE; i++) {
		UI_CButton(ID_GAMETYPE_SELECT + i, 18 - cgui.wideoffset, y + 4, gametypes_names[i], UI_LEFT, 0.78, color_white, NULL, NULL, NULL, NewGame_Event, ID_GAMETYPE_SELECT + i);
		y += 20;
	}
	y += 20;
	UI_CButton(ID_GAMETYPE_ALLMAPS, 18 - cgui.wideoffset, y + 4, "All", UI_LEFT, 0.78, color_white, NULL, NULL, NULL, NewGame_Event, ID_GAMETYPE_ALLMAPS);
	y += 40;
	for(i = 1; i < NTB_MAX; i++) {
		UI_CButton(ID_MODE_SELECT + i, 18 - cgui.wideoffset, y + 4, newgame_tabnames[i], UI_LEFT, 0.78, color_white, NULL, NULL, NULL, NewGame_Event, ID_MODE_SELECT + i);
		y += 20;
	}

	UI_CButton(2, (640 + cgui.wideoffset - 155) + 60, 418, "Start game", UI_CENTER, 1.45, color_white, NULL, NULL, NULL, NewGame_Event, 0);

	y = 18;
	UI_CField(&newgame.e[1], 640 + cgui.wideoffset - 170, y, "", 25, 25, color_white, "sv_hostname", NULL, 0);
	y += 12;
	if(newgame_mode == NTB_NONE) {
		y += 8;
		UI_CField(&newgame.e[10], 640 + cgui.wideoffset - 90, y, "Save file:", 16, 16, color_white, "g_entitypack", NULL, 0);
		y += 12;
		y += 8;
		UI_CField(&newgame.e[11], 640 + cgui.wideoffset - 90, y, "Fraglimit:", 5, 5, color_white, "fraglimit", NULL, 0);
		y += 12;
		UI_CField(&newgame.e[12], 640 + cgui.wideoffset - 90, y, "Capturelimit:", 5, 5, color_white, "capturelimit", NULL, 0);
		y += 12;
		UI_CField(&newgame.e[13], 640 + cgui.wideoffset - 90, y, "Timelimit:", 5, 5, color_white, "timelimit", NULL, 0);
		y += 12;
		y += 8;
		UI_CField(&newgame.e[14], 640 + cgui.wideoffset - 90, y, "Max entities:", 4, 4, color_white, "g_maxEntities", NULL, 0);
		y += 12;
		UI_CField(&newgame.e[15], 640 + cgui.wideoffset - 90, y, "Max clients:", 3, 3, color_white, "g_maxClients", NULL, 0);
		y += 12;
		y += 8;
		UI_CField(&newgame.e[16], 640 + cgui.wideoffset - 90, y, "Speed:", 9, 9, color_white, "g_speed", NULL, 0);
		y += 12;
		UI_CField(&newgame.e[17], 640 + cgui.wideoffset - 90, y, "Gravity:", 9, 9, color_white, "g_gravity", NULL, 0);
		y += 12;
		y += 8;
		UI_CField(&newgame.e[18], 640 + cgui.wideoffset - 90, y, "Health:", 4, 4, color_white, "g_spawn_health", NULL, 0);
		y += 12;
		UI_CField(&newgame.e[19], 640 + cgui.wideoffset - 90, y, "Armor:", 4, 4, color_white, "g_spawn_armor", NULL, 0);
		y += 12;
		y += 8;
		UI_CField(&newgame.e[20], 640 + cgui.wideoffset - 90, y, "Machinegun:", 4, 4, color_white, "g_spawn_machinegun", NULL, 0);
		y += 12;
		UI_CField(&newgame.e[21], 640 + cgui.wideoffset - 90, y, "Shotgun:", 4, 4, color_white, "g_spawn_shotgun", NULL, 0);
		y += 12;
		UI_CField(&newgame.e[22], 640 + cgui.wideoffset - 90, y, "Grenade:", 4, 4, color_white, "g_spawn_grenade", NULL, 0);
		y += 12;
		UI_CField(&newgame.e[23], 640 + cgui.wideoffset - 90, y, "Rocket:", 4, 4, color_white, "g_spawn_rocket", NULL, 0);
		y += 12;
		UI_CField(&newgame.e[24], 640 + cgui.wideoffset - 90, y, "Lightning:", 4, 4, color_white, "g_spawn_lightning", NULL, 0);
		y += 12;
		UI_CField(&newgame.e[25], 640 + cgui.wideoffset - 90, y, "Railgun:", 4, 4, color_white, "g_spawn_railgun", NULL, 0);
		y += 12;
		UI_CField(&newgame.e[26], 640 + cgui.wideoffset - 90, y, "Plasmagun:", 4, 4, color_white, "g_spawn_plasmagun", NULL, 0);
		y += 12;
		UI_CField(&newgame.e[27], 640 + cgui.wideoffset - 90, y, "BFG:", 4, 4, color_white, "g_spawn_bfg", NULL, 0);
		y += 12;
		UI_CField(&newgame.e[28], 640 + cgui.wideoffset - 90, y, "Hook:", 4, 4, color_white, "g_spawn_grapple", NULL, 0);
		y += 12;
		UI_CField(&newgame.e[29], 640 + cgui.wideoffset - 90, y, "Nailgun:", 4, 4, color_white, "g_spawn_nailgun", NULL, 0);
		y += 12;
		UI_CField(&newgame.e[30], 640 + cgui.wideoffset - 90, y, "Mines:", 4, 4, color_white, "g_spawn_prox", NULL, 0);
		y += 12;
		UI_CField(&newgame.e[31], 640 + cgui.wideoffset - 90, y, "Chaingun:", 4, 4, color_white, "g_spawn_chaingun", NULL, 0);
		y += 12;
	}

	UI_CreateUI(&newgame.menu, newgame.e);

	y = 15;
	for(i = 0; i < GT_MAX_GAME_TYPE; i++) {
		UI_SetHitbox(&newgame.e[ID_GAMETYPE_SELECT + i], 15 - cgui.wideoffset, y, 125, 16);
		y += 20;
	}
	y += 20;
	UI_SetHitbox(&newgame.e[ID_GAMETYPE_ALLMAPS], 15 - cgui.wideoffset, y, 125, 16);
	y += 40;
	for(i = 1; i < NTB_MAX; i++) {
		UI_SetHitbox(&newgame.e[ID_MODE_SELECT + i], 15 - cgui.wideoffset, y, 125, 16);
		y += 20;
	}

	UI_SetHitbox(&newgame.e[1], 640 + cgui.wideoffset - 175, 15, 160, 16);
	UI_SetHitbox(&newgame.e[2], 640 + cgui.wideoffset - 155, 410, 120, 32);

	trap_Key_SetCatcher(KEYCATCH_UI);
	UI_PushMenu(&newgame.menu);
}
