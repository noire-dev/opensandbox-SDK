// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

#include "ui_local.h"

typedef struct {
	menuframework_s menu;
	menuelement_s e[OSUI_MAX_ELEMENTS];
} options_t;

static options_t options;

static void Options_Defaults_Action(void) {
	trap_Cmd_ExecuteText(EXEC_APPEND, "exec default.cfg\n");
	trap_Cmd_ExecuteText(EXEC_APPEND, "cvar_restart\n");
	trap_Cmd_ExecuteText(EXEC_APPEND, "vid_restart\n");
}

void UI_Options(void) {
	int y;

	memset(&options, 0, sizeof(options));
	options.menu.fullscreen = qtrue;

	UI_CText(&options.e[0], OSUI_LOGO_X, OSUI_LOGO_Y + 24, "OPTIONS", UI_LEFT, 1.80);

	y = OSUI_STANDARD_Y;
	UI_CButton(&options.e[1], 64 - uis.wideoffset, y, "Player", UI_LEFT, 1.00, color_white, NULL, NULL, UI_PlayerModelMenu, NULL, 0); y += OSUI_SPACING_Y;
	UI_CButton(&options.e[2], 64 - uis.wideoffset, y, "Controls", UI_LEFT, 1.00, color_white, NULL, NULL, UI_Controls, NULL, 0); y += OSUI_SPACING_Y;
	UI_CButton(&options.e[3], 64 - uis.wideoffset, y, "Settings", UI_LEFT, 1.00, color_white, NULL, NULL, UI_Settings, NULL, 0); y += OSUI_BIGSPACING_Y;

	UI_CButton(&options.e[4], 64 - uis.wideoffset, y, "Defaults", UI_LEFT, 1.00, color_white, NULL, NULL, Options_Defaults_Action, NULL, 0); y += OSUI_BIGSPACING_Y;

	UI_CButton(&options.e[5], 64 - uis.wideoffset, y, "Back", UI_LEFT, 1.00, color_white, NULL, NULL, UI_PopMenu, NULL, 0);

	UI_CreateUI(&options.menu, options.e);
	UI_PushMenu(&options.menu);
}
