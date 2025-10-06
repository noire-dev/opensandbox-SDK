// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

#include "ui_local.h"

#define ID_LINK 100

typedef struct {
	menuframework_s menu;
	menuelement_s e[OSUI_MAX_ELEMENTS];

	char error[4096];
} mainmenu_t;

static mainmenu_t main;

static void Main_MenuEvent(void* ptr, int event) {
	if(event != QM_ACTIVATED) return;

	switch(((menucommon_s*)ptr)->callid) {
		case ID_LINK:
			if(trap_Cvar_VariableValue("os_windows")) trap_System("start https://opensandbox.neocities.org/");
			if(trap_Cvar_VariableValue("os_linux")) trap_System("xdg-open https://opensandbox.neocities.org/");
			if(trap_Cvar_VariableValue("os_macos")) trap_System("open https://opensandbox.neocities.org/");
			break;
	}
}

static void Main_MenuDraw(void) {
	vec4_t color = {0.85, 0.9, 1.0, 1};
	vec4_t modlistcolor = {0.00f, 0.00f, 0.00f, 0.40f};

	Menu_Draw(&main.menu);

	if(strlen(main.error)) ST_DrawString(0 - uis.wideoffset, 0, main.error, UI_DROPSHADOW, color, 1.00);
	ST_DrawString(635 + uis.wideoffset, 2, "2025.10.05", UI_RIGHT, color, 1.00);
}

void UI_MainMenu(void) {
	int y;
	char stp_entitypack[64];
	char stp_mapname[64];

	memset(&main, 0, sizeof(mainmenu_t));
	trap_Cvar_VariableStringBuffer("com_errorMessage", main.error, sizeof(main.error));
	main.menu.draw = Main_MenuDraw;
	main.menu.fullscreen = qtrue;

	y = OSUI_STANDARD_Y;
	if(uis.onmap) {
		UI_CButton(&main.e[0], 64 - uis.wideoffset, y, "Resume game", UI_LEFT, 1.00, color_white, NULL, NULL, UI_ForceMenuOff, NULL, 0); y += OSUI_BIGSPACING_Y;
	}
	UI_CButton(&main.e[1], 64 - uis.wideoffset, y, "Start New Game", UI_LEFT, 1.00, color_white, NULL, NULL, UI_NewGame, NULL, 0); y += OSUI_SPACING_Y;
	UI_CButton(&main.e[2], 64 - uis.wideoffset, y, "Find Multiplayer Game", UI_LEFT, 1.00, color_white, NULL, NULL, UI_ArenaServersMenu, NULL, 0); y += OSUI_BIGSPACING_Y;
	UI_CButton(&main.e[3], 64 - uis.wideoffset, y, "Profile", UI_LEFT, 1.00, color_white, NULL, NULL, UI_PlayerModelMenu, NULL, 0); y += OSUI_SPACING_Y;
	UI_CButton(&main.e[4], 64 - uis.wideoffset, y, "Options", UI_LEFT, 1.00, color_white, NULL, NULL, UI_Options, NULL, 0); y += OSUI_BIGSPACING_Y;
	if(uis.onmap) {
		UI_CButton(&main.e[5], 64 - uis.wideoffset, y, "Disconnect", UI_LEFT, 1.00, color_white, "disconnect \n", NULL, NULL, NULL, 0); y += OSUI_SPACING_Y;
	}
	UI_CButton(&main.e[6], 64 - uis.wideoffset, y, "Quit", UI_LEFT, 1.00, color_white, "quit \n", NULL, NULL, NULL, 0);

	if(uis.onmap) {
		y = OSUI_STANDARD_Y-64;
		trap_Cvar_VariableStringBuffer( "g_entitypack", stp_entitypack, 64 );
		trap_Cvar_VariableStringBuffer( "sv_mapname", stp_mapname, 64 );
		UI_CButton(&main.e[20], 630 + uis.wideoffset, y, "^1Red team", UI_RIGHT, 1.00, color_white, "team r; menuback\n", NULL, NULL, NULL, 0); y += OSUI_SPACING_Y;
		UI_CButton(&main.e[21], 630 + uis.wideoffset, y, "^4Blue team", UI_RIGHT, 1.00, color_white, "team b; menuback\n", NULL, NULL, NULL, 0); y += OSUI_SPACING_Y;
		UI_CButton(&main.e[22], 630 + uis.wideoffset, y, "^3Free team", UI_RIGHT, 1.00, color_white, "team f; menuback\n", NULL, NULL, NULL, 0); y += OSUI_SPACING_Y;
		UI_CButton(&main.e[23], 630 + uis.wideoffset, y, "^2Spectator", UI_RIGHT, 1.00, color_white, "team s; menuback\n", NULL, NULL, NULL, 0); y += OSUI_BIGSPACING_Y;
		UI_CButton(&main.e[24], 630 + uis.wideoffset, y, "Restart", UI_RIGHT, 1.00, color_white, "map_restart; menuback\n", NULL, NULL, NULL, 0); y += OSUI_BIGSPACING_Y;
		UI_CButton(&main.e[26], 630 + uis.wideoffset, y, "Save map", UI_RIGHT, 1.00, color_white, va("savemap maps/%s/%s.ent; menuback\n", stp_entitypack, stp_mapname), NULL, NULL, NULL, 0); y += OSUI_SPACING_Y;
		UI_CButton(&main.e[27], 630 + uis.wideoffset, y, "Delete map", UI_RIGHT, 1.00, color_white, va("deletemap maps/%s/%s.ent; menuback; wait 25; map_restart\n", stp_entitypack, stp_mapname), NULL, NULL, NULL, 0); y += OSUI_SPACING_Y;
		UI_CButton(&main.e[28], 630 + uis.wideoffset, y, "Clear map", UI_RIGHT, 1.00, color_white, "clearmap; menuback\n", NULL, NULL, NULL, 0);
	}

	UI_CPicture(&main.e[7], OSUI_LOGO_X, OSUI_LOGO_Y, 158, 55, AST_OSLOGO, 0, NULL, NULL, NULL, NULL, 0);

	UI_CBitmap(&main.e[8], 315 + 256 * 0.62 + 5 + uis.wideoffset, 450, 256 * 0.62, 38 * 0.62, AST_MOD, 0, NULL, NULL, NULL, Main_MenuEvent, 0);
	UI_CBitmap(&main.e[ID_LINK], 315 + uis.wideoffset, 450, 256 * 0.62, 38 * 0.62, AST_LINK, 0, NULL, NULL, NULL, Main_MenuEvent, ID_LINK);

	uis.menusp = 0;

	UI_CreateUI(&main.menu, main.e);
	trap_Key_SetCatcher(KEYCATCH_UI);
	UI_PushMenu(&main.menu);
}
