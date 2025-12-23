// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// OpenSandbox — GPLv2; see LICENSE for details.



typedef struct {
	menuframework_s menu;
	menuelement_s e[OSUI_MAX_ELEMENTS];
} options_t;

static options_t options;

void UI_Options(void) {
	int y;

	memset(&options, 0, sizeof(options));
	options.menu.fullscreen = qtrue;

	UI_CText(&options.e[0], 58 - uis.wideoffset, 64 + 24, "OPTIONS", UI_LEFT, 1.80);

	y = OSUI_STANDARD_Y;
	UI_CButton(&options.e[1], 64 - uis.wideoffset, y, "Player", UI_LEFT, 1.00, color_white, NULL, NULL, UI_PlayerModelMenu, NULL, 0);
	y += OSUI_SPACING_Y;
	UI_CButton(&options.e[2], 64 - uis.wideoffset, y, "Controls", UI_LEFT, 1.00, color_white, NULL, NULL, UI_Controls, NULL, 0);
	y += OSUI_SPACING_Y;
	UI_CButton(&options.e[3], 64 - uis.wideoffset, y, "Settings", UI_LEFT, 1.00, color_white, NULL, NULL, UI_Settings, NULL, 0);
	y += OSUI_BIGSPACING_Y;

	UI_CButton(&options.e[5], 64 - uis.wideoffset, y, "Back", UI_LEFT, 1.00, color_white, NULL, NULL, UI_PopMenu, NULL, 0);

	UI_CreateUI(&options.menu, options.e);
	UI_PushMenu(&options.menu);
}
