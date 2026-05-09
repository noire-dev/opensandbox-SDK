// Copyright (C) 2023-2026 Noire's Mod — GPLv2

/*
====================
Font, Text, Colors, CGUI
====================
*/

#ifndef GAME
typedef enum {
	K_TAB = 9,
	K_ENTER = 13,
	K_ESCAPE = 27,
	K_SPACE = 32,
	K_QUOTE = '\'',
	K_PLUS = '+',
	K_COMMA = ',',
	K_MINUS = '-',
	K_DOT = '.',
	K_SLASH = '/',
	K_SEMICOLON = ';',
	K_EQUAL = '=',
	K_BACKSLASH = '\\',
	K_UNDERSCORE = '_',
	K_BRACKET_OPEN = '[',
	K_BRACKET_CLOSE = ']',

	K_A = 'a',
	K_B = 'b',
	K_C = 'c',
	K_D = 'd',
	K_E = 'e',
	K_F = 'f',
	K_G = 'g',
	K_H = 'h',
	K_I = 'i',
	K_J = 'j',
	K_K = 'k',
	K_L = 'l',
	K_M = 'm',
	K_N = 'n',
	K_O = 'o',
	K_P = 'p',
	K_Q = 'q',
	K_R = 'r',
	K_S = 's',
	K_T = 't',
	K_U = 'u',
	K_V = 'v',
	K_W = 'w',
	K_X = 'x',
	K_Y = 'y',
	K_Z = 'z',

	K_BACKSPACE = 127,
	K_COMMAND,
	K_CAPSLOCK,
	K_POWER,
	K_PAUSE,

	K_UPARROW,
	K_DOWNARROW,
	K_LEFTARROW,
	K_RIGHTARROW,

	K_ALT,
	K_CTRL,
	K_SHIFT,
	K_INS,
	K_DEL,
	K_PGDN,
	K_PGUP,
	K_HOME,
	K_END,

	K_F1,
	K_F2,
	K_F3,
	K_F4,
	K_F5,
	K_F6,
	K_F7,
	K_F8,
	K_F9,
	K_F10,
	K_F11,
	K_F12,
	K_F13,
	K_F14,
	K_F15,

	K_KP_HOME,
	K_KP_UPARROW,
	K_KP_PGUP,
	K_KP_LEFTARROW,
	K_KP_5,
	K_KP_RIGHTARROW,
	K_KP_END,
	K_KP_DOWNARROW,
	K_KP_PGDN,
	K_KP_ENTER,
	K_KP_INS,
	K_KP_DEL,
	K_KP_SLASH,
	K_KP_MINUS,
	K_KP_PLUS,
	K_KP_NUMLOCK,
	K_KP_STAR,
	K_KP_EQUALS,

	K_MOUSE1,
	K_MOUSE2,
	K_MOUSE3,
	K_MOUSE4,
	K_MOUSE5,
	K_MWHEELDOWN,
	K_MWHEELUP,

	K_SUPER,
	K_COMPOSE,
	K_MODE,
	K_HELP,
	K_PRINT,
	K_SYSREQ,
	K_SCROLLOCK,
	K_BREAK,
	K_MENU,
	K_EURO,
	K_UNDO,

	MAX_KEYS
} keyNum_t;

extern vec4_t color_black;
extern vec4_t color_white;
extern vec4_t color_grey;
extern vec4_t color_dim;
extern vec4_t color_disabled;
extern vec4_t color_select;
extern vec4_t color_highlight;

extern vec4_t customcolor_crosshair;

#define CGUI_COLORCOUNT 1024

typedef struct {
	qhandle_t whiteShader;
	qhandle_t corner;
	float scale;
	float wideoffset;
	float colors[CGUI_COLORCOUNT][4];
	char binds[MAX_KEYS][256];
} cgui_t;
extern cgui_t cgui;

// Element style
// Text
#define UI_LEFT 0
#define UI_CENTER 1
#define UI_RIGHT 2
#define UI_BOLD 4
#define UI_ITALIC 8
#define UI_UNDERLINE 16
#define UI_STRIKETHROUGH 32
#define UI_MAGIC 64
#define UI_SHAKE 128
#define UI_LOCKSTYLE 256
#define UI_LOCKEFFECTS 512
#define UI_LOCKCOLOR 1024
#define UI_LOCKEMOJI 2048
// Other
#define UI_NODRAW 4096
#define NO_TOP_LEFT 8192
#define NO_TOP_RIGHT 16384
#define NO_BOTTOM_LEFT 32768
#define NO_BOTTOM_RIGHT 65536
#define UI_ACCENT 131072

// Window style
#define UI_NOTITLE 1
#define UI_NOSCALE 2
#define UI_NOZORDER 4

#define BASEFONT_WIDTH 24.00
#define BASEFONT_HEIGHT 24.00
#define FONT_SIZE 24.00
#define FONT_WIDTH 0.50

int getPlayerMove(int buttons, int type);
void drawRoundedRect(float x, float y, float width, float height, float radius, float *color, int style);
void drawOutline(float x, float y, float width, float height, float thickness, float *color);
void drawShader(float x, float y, float w, float h, const char *file);
void drawColoredShader(float x, float y, float w, float h, const char *file, float *color);
void CL_Init(void);
float stringWidth(const char *str, float fontSize, int style);
void drawString(float x, float y, const char *str, int style, float *color, float fontSize);
void drawStringField(float x, float y, const char *str, int style, float *color, float fontSize, int position);
#endif

/*
====================
Console Variables
====================
*/

#define MAX_CVAR_STRING 256

typedef int cvarHandle_t;

typedef struct {
	float value;
	int integer;
	char string[MAX_CVAR_STRING];
} cvar_t;

void VM_RegisterCvars(void);
void updateCvars(void);
int cvarInt(const char *name);
float cvarFloat(const char *name);
char *cvarString(const char *name);

/*
====================
Shared Syscalls
====================
*/

void trap_Print(const char *string);
void trap_Error(const char *string);
int trap_Milliseconds(void);
void cvarRegister(const char *name, const char *defaultValue, int flags);
int cvarID(const char *name);
void cvarUpdate(cvar_t *vmCvar, int cvarID);
void cvarReload(void);
void cvarSet(const char *name, const char *value);
int trap_Argc(void);
void trap_Argv(int n, char *buffer, int bufferLength);
void trap_Args(char *buffer, int bufferLength);
int FS_Open(const char *qpath, fileHandle_t *f, fsMode_t mode);
void FS_Read(void *buffer, int len, fileHandle_t f);
void FS_Write(const void *buffer, int len, fileHandle_t f);
void FS_Close(fileHandle_t f);
int FS_List(const char *path, const char *extension, char *listbuf, int bufsize);
void trap_Cmd(int exec_when, const char *text);
void trap_RealTime(qtime_t *qtime);
void trap_System(const char *command);

void *memset(void *dest, int c, size_t count);
void *memcpy(void *dest, const void *src, size_t count);
char *strncpy(char *strDest, const char *strSource, size_t count);
double sin(double x);
double cos(double x);
double acos(double x);
double atan2(double y, double x);
double sqrt(double x);
