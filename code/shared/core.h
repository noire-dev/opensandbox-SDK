// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 blqck.Cøre
// Void Sandbox — GPLv2; see LICENSE for details.

/*
====================
Font, Text, Colors, CGUI
====================
*/

#ifndef GAME
extern vec4_t color_black;
extern vec4_t color_white;
extern vec4_t color_grey;
extern vec4_t color_dim;
extern vec4_t color_disabled;
extern vec4_t color_select;
extern vec4_t color_highlight;

extern vec4_t customcolor_crosshair;

#define CGUI_COLORCOUNT 200
typedef struct {
    qhandle_t defaultFont[5];
	qhandle_t whiteShader;
	qhandle_t corner;
	float scale;
	float bias;
	float wideoffset;
	float colors[CGUI_COLORCOUNT][4];
	char binds[256][256];
} cgui_t;
extern cgui_t cgui;

#define BASEFONT_WIDTH 9
#define BASEFONT_HEIGHT 11
#define FONT_WIDTH 0.64
#define BASEFONT_INDENT (BASEFONT_WIDTH * FONT_WIDTH)

int getPlayerMove(int buttons, int type);
void adjustFrom640(float *x, float *y, float *w, float *h);
void drawRoundedRect(float x, float y, float width, float height, float radius, float *color);
void drawShader(float x, float y, float w, float h, const char *file);
int colorEscapes(const char *str);
void initCGUI(const char *font);
void updateCGUI(void);
int stringCount(const char *str);
void drawChar(float x, float y, int ch, int style, float *color, float size);
float stringWidth(const char *str, float size);
void drawString(float x, float y, const char *str, int style, float *color, float fontSize);
void drawButton(float x, float y, float w, float h, int corner, int tcolor, int colorbg, float size, int margin, int style, const char *text);
void drawCheckbox(float x, float y, float w, float h, int corner, int tcolor, int scolor, int colorbg, float size, int margin, int style, const char *text);
void drawSlider(float x, float y, float w, float h, int corner, int tcolor, int colorbg, float size, int margin, int style, const char *text, float value, float min, float max);
void drawAction(float x, float y, float w, float h, int corner, int tcolor, int colorbg, float size, int margin, int style, const char *text, const char *bindkey);
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

void registerCvars(void);
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
int FS_Open(const char *qpatph, fileHandle_t *f, fsMode_t mode);
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
