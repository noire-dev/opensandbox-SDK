// Copyright (C) 2023-2026 Noire's Mod — GPLv2

#ifndef __UI_LOCAL_H__
#define __UI_LOCAL_H__

#define SHELL_STATE_FILE "nmre_state.bin"
#define UI_WINDOW_TITLE_HEIGHT 40.0

#define ETYPE_NULL 0
#define ETYPE_PICTURE 1
#define ETYPE_BUTTON 2
#define ETYPE_CHECKBOX 3
#define ETYPE_SLIDER 4
#define ETYPE_ACTION 5
#define ETYPE_SPIN 6
#define ETYPE_FIELD 7
#define ETYPE_LIST 8
#define ETYPE_LISTFILES 9
#define ETYPE_WINDOWBUTTON 10

#define EMODE_NONE 0
#define EMODE_INT 1
#define EMODE_FLOAT 2

#define EMODE_NUMBER 1
#define EMODE_STRING 2

#define LTYPE_NONE 0
#define LTYPE_APPS 1

#define ELEMENT_OPTIONLENGTH 64
#define ELEMENT_OPTIONSCOUNT 64
typedef struct {
    void *parentWindow;
	int type;
	int id;
	float baseX, baseY, baseW, baseH, x, y, w, h;
	char text[MAX_JS_STRINGSIZE];
	int style;
	int hoverStyle;
	float baseScale, scale;
	int colorText, colorBackground;
	int baseCorner, corner;
	int baseMargin, margin;
	char cvar[MAX_JS_STRINGSIZE];

	float value;
	float valueMod;

	float min, max;
	int mode;

	char bind[MAX_JS_STRINGSIZE];

	char options[ELEMENT_OPTIONSCOUNT][ELEMENT_OPTIONLENGTH];
	int optionsCount;

	char field[MAX_JS_STRINGSIZE];
	int fieldPosition;

    int listType;
    int listSubtype;
    int scroll;
    float itemW, itemH;
	int col, row;
	
	int listID;
	
	float baseMargin4[4], margin4[4];
} element_s;

#define FILELIST_MAX_LENGTH (1024 * 512)
typedef struct {
    char list[FILELIST_MAX_LENGTH];
    int count;
    char folder[MAX_JS_STRINGSIZE];
    char ext[MAX_JS_STRINGSIZE];
	char drawDir[MAX_JS_STRINGSIZE];
} fileList_s;

#define WINDOW_MAX_ELEMENTS 256
#define WINDOW_MAX_LISTS 2
typedef struct {
	qboolean created;
	qboolean minimized;
	int id;
	char nameID[MAX_JS_STRINGSIZE];
	char name[MAX_JS_STRINGSIZE];
	char icon[MAX_JS_STRINGSIZE];
	int style;
	float baseW, baseH, x, y, w, h;
	int colorText, colorTitle, colorBackground;
	
	float scaleFactor;
	int focusedElement;
	int keyCapture;
	element_s element[WINDOW_MAX_ELEMENTS];
	
	fileList_s fileList[WINDOW_MAX_LISTS];
} window_s;

typedef struct {
	char name[MAX_JS_STRINGSIZE];
	char icon[MAX_JS_STRINGSIZE];
} app_s;

typedef struct {
	qboolean minimized;
	char nameID[MAX_JS_STRINGSIZE];
	float x, y;
	float scaleFactor;
	int zOrder;
} saveState_s;

#define SHELL_MAX_APPS 4096
#define SHELL_MAX_WINDOWS 16
typedef struct {
	int cursorX;
	int cursorY;
	qboolean cursorIsDragging;
	int debug;
	float scale;
	int corner;
	
	qboolean onMap;

    app_s app[SHELL_MAX_APPS];
	int appCount;

	int focusedWindow;
	int activeWindow;
	int zOrder[SHELL_MAX_WINDOWS];
	window_s window[SHELL_MAX_WINDOWS];
	saveState_s saveState[SHELL_MAX_WINDOWS];
} shell_s;
extern shell_s shell;

#define CONSOLE_MAXLINES 16384
#define CONSOLE_MAXLINE 256
typedef struct console_s {
	char lines[CONSOLE_MAXLINES][CONSOLE_MAXLINE];
	int linesCount;
} console_s;
extern console_s console;

// Дополнительные структуры из движка
typedef struct {
	connstate_t connState;
	int connectPacketCount;
	int clientNum;
	char servername[MAX_STRING_CHARS];
	char updateInfoString[MAX_STRING_CHARS];
	char messageString[MAX_STRING_CHARS];
} uiClientState_t;

// clang-format off
typedef enum {
    UI_DISABLE,
    UI_ENABLE
} uiCommand_t;
// clang-format on

// clang-format off
typedef enum {
    UI_INIT = 0,
    UI_SHUTDOWN,
    UI_KEY_EVENT,
    UI_MOUSE_EVENT,
    UI_REFRESH,
    UI_IS_FULLSCREEN,
    UI_SET_ACTIVE_MENU,
    UI_CONSOLE_COMMAND,
    UI_DRAW_CONNECT_SCREEN
} uiExport_t;
// clang-format on

// Функции вызываемые из движка
int UI_Init(void);
int UI_Shutdown(void);
int UI_KeyEvent(int key, int down, int isChar);
int UI_MouseEvent(int dx, int dy);
int UI_Refresh(void);
int UI_IsFullscreen(void);
int UI_SetActiveMenu(uiCommand_t cmd);
qboolean UI_ConsoleCommand(void);
int UI_DrawConnectScreen(void);

// Функции подключенные к JS
void UI_CloseWindow(int windowID);
void UI_ClearWindow(int windowID, int min, int max);
int UI_GetElementFree(int windowID);
int UI_Window(int windowID, char* nameid, char* name, char* icon, int style, float x, float y, float w, float h, int colorText, int colorTitle, int colorBackground);
int UI_WindowButton(int windowID, int elementID, float x, char* text, int style, int color, float scale);
int UI_Picture(int windowID, int elementID, float x, float y, float w, float h, char* picture, int style, int color);
int UI_Button(int windowID, int elementID, float x, float y, float w, float h, char* text, int style, int color, float scale);
int UI_Checkbox(int windowID, int elementID, float x, float y, float w, float h, char* text, int style, int color, float scale, char* cvar);
int UI_Slider(int windowID, int elementID, float x, float y, float w, float h, char* text, int style, int color, float scale, char* cvar, float min, float max, int mode);
int UI_Action(int windowID, int elementID, float x, float y, float w, float h, char* text, int style, int color, float scale, char* cvar);
int UI_Spin(int windowID, int elementID, float x, float y, float w, float h, char* text, int style, int color, float scale, char* cvar, int mode);
int UI_Field(int windowID, int elementID, float x, float y, float w, float h, char* text, int style, int color, float scale, char* cvar);
int UI_List(int windowID, int elementID, float x, float y, float w, float h, float scale, int col, int row, int listType, int listSubtype);
int UI_ListFiles(int windowID, int elementID, float x, float y, float w, float h, float scale, int col, int row, int listID);

void UI_FillListFiles(int windowID, char* folder, char* ext, char *drawDir, int listID);
void UI_SetMargin(int windowID, int elementID, float x, float y, float w, float h);

// Вызовы движка
void trap_Key_SetBinding(int keynum, const char* binding);
void trap_Key_ClearStates(void);
void trap_Key_SetCatcher(int catcher);
void trap_GetClipboardData(char* buf, int bufsize);
void trap_GetClientState(uiClientState_t* state);
int trap_GetConfigString(int index, char* buff, int buffsize);
int trap_LAN_GetPingQueueCount(void);
void trap_LAN_ClearPing(int n);
void trap_LAN_GetPing(int n, char* buf, int buflen, int* pingtime);
void trap_LAN_GetPingInfo(int n, char* buf, int buflen);
int trap_MemoryRemaining(void);
int trap_LAN_GetServerCount(int source);
void trap_LAN_GetServerAddressString(int source, int n, char* buf, int buflen);
void trap_LAN_GetServerAddressString(int source, int n, char* buf, int buflen);
void consoleSync(console_s* vmconsole, int currentLines);
#endif
