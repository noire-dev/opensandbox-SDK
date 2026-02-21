// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2026 noire.dev
// Project Sandbox — GPLv2; see LICENSE for details.

#ifndef __UI_LOCAL_H__
#define __UI_LOCAL_H__

#define MAX_MENUITEMS 500

#define MTYPE_NULL 0
#define MTYPE_BUTTON 1
#define MTYPE_CHECKBOX 2
#define MTYPE_SLIDER 3
#define MTYPE_ACTION 4

#define MTYPE_MAX 10

#define UI_STRINGLENGTH 128
#define UI_ACTIONLENGTH 256

#define EMODE_NONE 0
#define EMODE_INT 1
#define EMODE_FLOAT 2
typedef struct {
	int type;
	int id;
	float x, y, w, h;
	char text[UI_STRINGLENGTH];
	int style;
	float size;
	int colortext, colorbg;
	int corner;
	int margin;
	char action[UI_ACTIONLENGTH];
	
	float value;
	
	float min, max;
	int mode;
} menuelement_s;

typedef struct {
	int cursorx;
	int cursory;
	qboolean onmap;
	
	int currentItem;
	menuelement_s items[MAX_MENUITEMS];
} uiStatic_t;

// ui_atoms.c
void UI_SetActiveMenu(uiMenuCommand_t menu);
void UI_KeyEvent(int key, int down);
void UI_MouseEvent(int dx, int dy);
qboolean UI_ConsoleCommand(int realTime);
void UI_Shutdown(void);
void UI_Init(void);

// ui_connect.c
void UI_DrawConnectScreen(qboolean overlay);

// ui_main.c
void UI_CreateCvars(void);

// ui_menu.c
void MenuDraw(void);
void UI_Menu(void);
void UI_HotReloadJS(void);

// ui_qmenu.c
qboolean Menu_CursorOnItem(int id);
void Menu_ElementsDraw(void);
void Menu_DefaultKey(int key);
int UI_CButton(int id, float x, float y, float w, float h, char *text, int style, float size, int colortext, int colorbg, int corner, int margin, char *action);
int UI_CCheckbox(int id, float x, float y, float w, float h, char *text, float size, int colortext, int colorbg, int corner, int margin, char *action);
int UI_CSlider(int id, float x, float y, float w, float h, char *text, float size, int colortext, int colorbg, int corner, int margin, char *action, float min, float max, int mode);

extern uiStatic_t uis;
extern const char *gametype_items[GT_MAX_GAME_TYPE + 1];

#define CON_MAXLINES 16384
#define CON_MAXLINE 256

typedef struct console_s {
	char lines[CON_MAXLINES][CON_MAXLINE];
	int linescount;
} console_t;

extern console_t console;
extern int consoleLines;

// syscalls
void trap_Key_SetBinding(int keynum, const char *binding);
void trap_Key_ClearStates(void);
void trap_Key_SetCatcher(int catcher);
void trap_GetClipboardData(char *buf, int bufsize);
void trap_GetClientState(uiClientState_t *state);
int trap_GetConfigString(int index, char *buff, int buffsize);
int trap_LAN_GetPingQueueCount(void);
void trap_LAN_ClearPing(int n);
void trap_LAN_GetPing(int n, char *buf, int buflen, int *pingtime);
void trap_LAN_GetPingInfo(int n, char *buf, int buflen);
int trap_MemoryRemaining(void);
int trap_LAN_GetServerCount(int source);
void trap_LAN_GetServerAddressString(int source, int n, char *buf, int buflen);
void trap_LAN_GetServerAddressString(int source, int n, char *buf, int buflen);
void consoleSync(console_t *vmconsole, int currentLines);
#endif
