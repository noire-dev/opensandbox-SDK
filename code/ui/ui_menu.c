// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// OpenSandbox — GPLv2; see LICENSE for details.

#include "../qcommon/js_local.h"

#define ID_LINK 100

typedef struct {
	menuframework_s menu;
	menuelement_s e[OSUI_MAX_ELEMENTS];

	char *error;
} mainmenu_t;

static mainmenu_t main;

int mainmenu_items;

static void Main_MenuEvent(void *ptr, int event) {
	if(event != QM_ACTIVATED) return;

	switch(((menucommon_s *)ptr)->callid) {
	case ID_LINK:
		if(cvarInt("os_windows")) trap_System("start https://opensandbox.neocities.org/");
		if(cvarInt("os_linux")) trap_System("xdg-open https://opensandbox.neocities.org/");
		if(cvarInt("os_macos")) trap_System("open https://opensandbox.neocities.org/");
		break;
	}
}

static void Main_MenuDraw(void) {
	int y, i;
	vec4_t color = {0.85, 0.9, 1.0, 1};
	vec4_t color2 = {0.85, 0.90, 1.00, 0.20};

	Menu_Draw(&main.menu);

	if(strlen(main.error)) ST_DrawString(0 - uis.wideoffset, 0, main.error, UI_DROPSHADOW, color, 1.00);
	ST_DrawString(635 + uis.wideoffset, 2, "2025.10.26", UI_RIGHT, color, 1.00);

	UI_DrawRoundedRect(10 - uis.wideoffset, 10, 135, (mainmenu_items * (20)) + 6, 4, color_dim);

	y = 15;
	for(i = 0; i < mainmenu_items; i++) {
		UI_DrawRoundedRect(15 - uis.wideoffset, y, 125, 16, 3, color2);
		y += 20;
	}

	Menu_Draw(&main.menu);
}

void UI_MainMenu(void) {
	int y, i;

	memset(&main, 0, sizeof(mainmenu_t));
	main.error = cvarString("com_errorMessage");
	main.menu.draw = Main_MenuDraw;
	main.menu.fullscreen = qtrue;

	y = 15;
	mainmenu_items = 0;
	if(uis.onmap) {
		UI_CButton(&main.e[0], 18 - uis.wideoffset, y + 4, "Resume game", UI_LEFT, 0.90, color_white, NULL, NULL, UI_ForceMenuOff, NULL, 0);
		y += 20;
		mainmenu_items++;
	}
	UI_CButton(&main.e[1], 18 - uis.wideoffset, y + 4, "Start New Game", UI_LEFT, 0.90, color_white, NULL, NULL, UI_NewGame, NULL, 0);
	y += 20;
	mainmenu_items++;
	UI_CButton(&main.e[2], 18 - uis.wideoffset, y + 4, "Find Multiplayer Game", UI_LEFT, 0.90, color_white, NULL, NULL, UI_ArenaServersMenu, NULL, 0);
	y += 20;
	mainmenu_items++;
	UI_CButton(&main.e[3], 18 - uis.wideoffset, y + 4, "Profile", UI_LEFT, 0.90, color_white, NULL, NULL, UI_PlayerModelMenu, NULL, 0);
	y += 20;
	mainmenu_items++;
	UI_CButton(&main.e[4], 18 - uis.wideoffset, y + 4, "Options", UI_LEFT, 0.90, color_white, NULL, NULL, UI_Options, NULL, 0);
	y += 20;
	mainmenu_items++;
	if(uis.onmap) {
		UI_CButton(&main.e[5], 18 - uis.wideoffset, y + 4, "Disconnect", UI_LEFT, 0.90, color_white, "disconnect \n", NULL, NULL, NULL, 0);
		y += 20;
		mainmenu_items++;
	}
	UI_CButton(&main.e[6], 18 - uis.wideoffset, y + 4, "Quit", UI_LEFT, 0.90, color_white, "quit \n", NULL, NULL, NULL, 0);
	mainmenu_items++;

	if(uis.onmap) {
		y = OSUI_STANDARD_Y - 64;
		UI_CButton(&main.e[20], 630 + uis.wideoffset, y, "^1Red team", UI_RIGHT, 1.00, color_white, "team r; menuback\n", NULL, NULL, NULL, 0);
		y += OSUI_SPACING_Y;
		UI_CButton(&main.e[21], 630 + uis.wideoffset, y, "^4Blue team", UI_RIGHT, 1.00, color_white, "team b; menuback\n", NULL, NULL, NULL, 0);
		y += OSUI_SPACING_Y;
		UI_CButton(&main.e[22], 630 + uis.wideoffset, y, "^3Free team", UI_RIGHT, 1.00, color_white, "team f; menuback\n", NULL, NULL, NULL, 0);
		y += OSUI_SPACING_Y;
		UI_CButton(&main.e[23], 630 + uis.wideoffset, y, "^2Spectator", UI_RIGHT, 1.00, color_white, "team s; menuback\n", NULL, NULL, NULL, 0);
		y += OSUI_BIGSPACING_Y;
		UI_CButton(&main.e[24], 630 + uis.wideoffset, y, "Restart", UI_RIGHT, 1.00, color_white, "map_restart; menuback\n", NULL, NULL, NULL, 0);
		y += OSUI_BIGSPACING_Y;
		UI_CButton(&main.e[26], 630 + uis.wideoffset, y, "Save map", UI_RIGHT, 1.00, color_white, va("savemap maps/%s/%s.ent; menuback\n", cvarString("g_entitypack"), cvarString("sv_mapname")), NULL, NULL, NULL, 0);
		y += OSUI_SPACING_Y;
		UI_CButton(&main.e[27], 630 + uis.wideoffset, y, "Delete map", UI_RIGHT, 1.00, color_white, va("deletemap maps/%s/%s.ent; menuback; wait 25; map_restart\n", cvarString("g_entitypack"), cvarString("sv_mapname")), NULL, NULL, NULL, 0);
		y += OSUI_SPACING_Y;
		UI_CButton(&main.e[28], 630 + uis.wideoffset, y, "Clear map", UI_RIGHT, 1.00, color_white, "clearmap; menuback\n", NULL, NULL, NULL, 0);
	}

	UI_CPicture(&main.e[ID_LINK], 465 + uis.wideoffset, 410, 158, 55, AST_OSLOGO, 0, NULL, NULL, NULL, Main_MenuEvent, ID_LINK);

	UI_CreateUI(&main.menu, main.e);

	y = 15;
	for(i = 0; i <= 6; i++) {
		UI_SetHitbox(&main.e[i], 15 - uis.wideoffset, y, 125, 16);
		if(main.e[i].generic.type) {
			y += 20;
		}
	}

	uis.menusp = 0;

	trap_Key_SetCatcher(KEYCATCH_UI);
	UI_PushMenu(&main.menu);
}
