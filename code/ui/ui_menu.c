// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// OpenSandbox — GPLv2; see LICENSE for details.

#include "../qcommon/js_local.h"

#define ID_LINK 100

static void MenuEvent(void *ptr, int event) {
	if(event != QM_ACTIVATED) return;

	switch(((menucommon_s *)ptr)->callid) {
	case ID_LINK:
		if(cvarInt("os_windows")) trap_System("start https://opensandbox.neocities.org/");
		if(cvarInt("os_linux")) trap_System("xdg-open https://opensandbox.neocities.org/");
		if(cvarInt("os_macos")) trap_System("open https://opensandbox.neocities.org/");
		break;
	}
}

static void MenuDraw(void) {
	vec4_t color = {0.85, 0.9, 1.0, 1};
	qboolean JSWork = JS_MenuCheck();
	int y, i;
	
	if(!JSWork){
	    ST_DrawString(320, 0, "JavaScript Error: UI not initialized!", UI_DROPSHADOW|UI_CENTER, color, 1.50);
    	y = 460 - BASEFONT_HEIGHT;
    	for(i = console.linescount; i > console.linescount - 32; i--) {
    		if(i <= 0) break;
    		ST_DrawString(15 - cgui.wideoffset, y, console.lines[i], UI_DROPSHADOW, color_white, 0.90);
    		y -= BASEFONT_HEIGHT;
    	}
    	return;
	}

	if(strlen(cvarString("com_errorMessage"))) ST_DrawString(0 - cgui.wideoffset, 0, cvarString("com_errorMessage"), UI_DROPSHADOW, color, 1.00);
	
	JS_MenuDraw();
	Menu_Draw(&ui.menu);
}

void UI_Menu(void) {
	memset(&ui, 0, sizeof(uimenu_t));
	ui.menu.draw = MenuDraw;
	ui.menu.fullscreen = qtrue;

	UI_CPicture(&ui.e[ID_LINK], 465 + cgui.wideoffset, 410, 158, 55, AST_OSLOGO, 0, NULL, NULL, NULL, MenuEvent, ID_LINK);

    JS_MenuInit();
	UI_CreateUI(&ui.menu, ui.e);

	uis.menusp = 0;

	trap_Key_SetCatcher(KEYCATCH_UI);
	UI_PushMenu(&ui.menu);
}
