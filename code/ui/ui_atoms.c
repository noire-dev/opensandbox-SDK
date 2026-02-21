// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// OpenSandbox — GPLv2; see LICENSE for details.

#include "../shared/javascript.h"

uiStatic_t uis;

static void UI_OnMapStatus(void) {
	char svinfo[MAX_INFO_STRING];

	trap_GetConfigString(CS_SERVERINFO, svinfo, MAX_INFO_STRING);
	if(strlen(svinfo) <= 0) {
		uis.onmap = qfalse;
	} else {
		uis.onmap = qtrue;
	}
}

static void UI_Close(void) {
	trap_Key_SetCatcher(trap_Key_GetCatcher() & ~KEYCATCH_UI);
	trap_Key_ClearStates();
}

void UI_SetActiveMenu(uiMenuCommand_t menu) {
    updateCGUI();
    UI_OnMapStatus();

	switch(menu) {
	case UI_DISABLE: UI_Close(); return;
	case UI_ENABLE: UI_Menu(); return;
	default: print("UI_SetActiveMenu: bad enum %d\n", menu); break;
	}
}

void UI_KeyEvent(int key, int down) {
	if(!down) return;
	Menu_DefaultKey(key);
}

void UI_MouseEvent(int dx, int dy) {
	int i;

	uis.cursorx += dx;
	if(uis.cursorx < 0 - cgui.wideoffset) uis.cursorx = 0 - cgui.wideoffset;
	else if(uis.cursorx > 640 + cgui.wideoffset) uis.cursorx = 640 + cgui.wideoffset;

	uis.cursory += dy;
	if(uis.cursory < 0) uis.cursory = 0;
	else if(uis.cursory > 480) uis.cursory = 480;

	for(i = 0; i < MAX_MENUITEMS; i++) {
		if(!Menu_CursorOnItem(uis.items[i].id)) continue;
		uis.currentItem = i;
		return;
	}
}

static char *UI_Argv(int arg) {
	static char buffer[MAX_STRING_CHARS];

	trap_Argv(arg, buffer, sizeof(buffer));
	return buffer;
}

qboolean UI_ConsoleCommand(int realTime) {
	char *cmd;
	cmd = UI_Argv(0);
	
	if(Q_stricmp(cmd, "closemenu") == 0) {
		UI_Close();
		return qtrue;
	}

	return qfalse;
}

void UI_Shutdown(void) {}

void UI_Init(void) {
	UI_CreateCvars();
	registerCvars();
	initCGUI("default");
}

void UI_Refresh(int realtime) {
	if(!(trap_Key_GetCatcher() & KEYCATCH_UI)) return;

	updateCGUI();
	UI_OnMapStatus();
	consoleSync(&console, console.linescount);

	if(!uis.onmap) drawShader(0-cgui.wideoffset, 0, 640+(cgui.wideoffset*2), 480, "menu/menu");
	drawShader(0-cgui.wideoffset, 0, 640+(cgui.wideoffset*2), 480, "menu/overlay");

	MenuDraw();

	trap_R_SetColor(NULL);
	drawShader(uis.cursorx - 12, uis.cursory - 12, 24, 24, "menu/cursor");
}
