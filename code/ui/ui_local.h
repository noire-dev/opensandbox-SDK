// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// OpenSandbox — GPLv2; see LICENSE for details.

#ifndef __UI_LOCAL_H__
#define __UI_LOCAL_H__

#define LOW_MEMORY (5 * 1024 * 1024)

#define OSUI_STANDARD_Y 145
#define OSUI_SPACING_Y 18
#define OSUI_BIGSPACING_Y 36

#define OSUI_MAX_ELEMENTS 101

#define LST_SIMPLE 0
#define LST_ICONS 1
#define LST_GRID 2

#define RBT_NONE 0
#define RBT_NORMAL 1
#define RBT_INVERSE 2

#define AST_OSLOGO 1
#define AST_MOD 2
#define AST_LINK 3

#define SLIDER_RANGE 8
#define MAX_EDIT_LINE 256

#define MAX_MENUDEPTH 8
#define MAX_MENUITEMS 128

#define MTYPE_NULL 0

#define MTYPE_SLIDER 1
#define MTYPE_ACTION 2
#define MTYPE_SPINCONTROL 3
#define MTYPE_FIELD 4
#define MTYPE_RADIOBUTTON 5
#define MTYPE_BITMAP 6
#define MTYPE_SCROLLLIST 7
#define MTYPE_PTEXT 8
#define MTYPE_TEXT 9

#define MTYPE_MAX 10

#define QMF_LEFT_JUSTIFY (1U << 0)
#define QMF_CENTER_JUSTIFY (1U << 1)
#define QMF_RIGHT_JUSTIFY (1U << 2)
#define QMF_HIGHLIGHT (1U << 3)
#define QMF_HIGHLIGHT_IF_FOCUS (1U << 4) // steady focus
#define QMF_PULSEIFFOCUS (1U << 5)       // pulse if focus
#define QMF_HASMOUSEFOCUS (1U << 6)
#define QMF_HIDDEN (1U << 7)         // skips drawing
#define QMF_GRAYED (1U << 8)         // grays and disables
#define QMF_INACTIVE (1U << 9)       // disables any input
#define QMF_NODEFAULTINIT (1U << 10) // skip default initialization
#define QMF_PULSE (1U << 11)

#define QM_GOTFOCUS 1
#define QM_LOSTFOCUS 2
#define QM_ACTIVATED 3

// control event handler
typedef void (*callbackFunc)(void *self, int event);

typedef struct _tag_menuframework {
	int cursor;
	int cursor_prev;

	int nitems;
	void *items[MAX_MENUITEMS];

	void (*draw)(void);
	sfxHandle_t (*key)(int key);

	qboolean fullscreen;
} menuframework_s;

typedef struct {
	int type;
	int style;
	qhandle_t shader;
	qhandle_t model;

	// extended callback
	void (*excallback)(void *self, int event);
	int excallbacktype;
	char *cmd;
	char *var;
	void (*func)(void);

	const char *name;
	int id;
	int callid;
	int x, y;
	int left;
	int top;
	int right;
	int bottom;
	menuframework_s *parent;
	int menuPosition;
	unsigned flags;

	int mode;
	char *buffer;
	int *value;

	void (*callback)(void *self, int event);
	void (*statusbar)(void *self);
	void (*ownerdraw)(void *self);
} menucommon_s;

typedef struct {
	int cursor;
	int scroll;
	int widthInChars;
	char buffer[MAX_EDIT_LINE];
	int maxchars;
} mfield_t;

typedef struct {
	menucommon_s generic;
	char *string;
	int style;
	float size;
	float *curColor;
	float *color;
	float *color2;

	char *focuspic;
	char *errorpic;
	qhandle_t shader;
	qhandle_t focusshader;
	int width;
	int height;

	int curvalue;

	int oldvalue;
	int numitems;
	int top;
	const char **itemnames;
	int columns;
	char *file;
	char *extension;

	float minvalue;
	float maxvalue;
	float range;

	mfield_t field;

	int corner;

	qboolean drawText;
	float padding_x;
	float padding_y;
} menuelement_s;

extern sfxHandle_t menu_move_sound;
extern sfxHandle_t menu_out_sound;
extern sfxHandle_t menu_buzz_sound;
extern sfxHandle_t menu_null_sound;

typedef struct {
	int oldFrame;
	int oldFrameTime; // time when ->oldFrame was exactly on

	int frame;
	int frameTime; // time when ->frame will be exactly on

	float backlerp;

	float yawAngle;
	qboolean yawing;
	float pitchAngle;
	qboolean pitching;

	int animationNumber; // may include ANIM_TOGGLEBIT
	animation_t *animation;
	int animationTime; // time when the first frame of the animation will be exact
} lerpFrame_t;

typedef struct {
	// model info
	qhandle_t legsModel;
	qhandle_t legsSkin;
	qhandle_t legsShader;
	lerpFrame_t legs;

	qhandle_t torsoModel;
	qhandle_t torsoSkin;
	qhandle_t torsoShader;
	lerpFrame_t torso;

	qhandle_t headModel;
	qhandle_t headSkin;
	qhandle_t headShader;

	animation_t animations[MAX_ANIMATIONS];

	// currently in use drawing parms
	vec3_t viewAngles;
	vec3_t moveAngles;
	int legsAnim;
	int torsoAnim;

	qboolean fixedlegs;  // true if legs yaw is always the same as torso yaw
	qboolean fixedtorso; // true if torso never changes yaw

	qboolean newModel;

	int oldFrame;
} playerInfo_t;

#define MODELNAME_BUFFER MAX_QPATH

typedef struct {
	playerInfo_t player;
	char modelskin[MODELNAME_BUFFER];
	char headskin[MODELNAME_BUFFER];
	char legsskin[MODELNAME_BUFFER];
	char team_modelskin[MODELNAME_BUFFER];
	char team_headskin[MODELNAME_BUFFER];
	char team_legsskin[MODELNAME_BUFFER];

	menuelement_s bitmap;

	int anim;
	int playerLegs;
	int playerTorso;
	int playerWeapon;
	vec3_t viewangles;
	vec3_t moveangles;

	int cursorx;
	int cursory;
	qboolean bDoingIdleAnim;

	qboolean bUnknownModel;
	qboolean bUnknownHeadModel;
	qboolean bUnknownLegsModel;
	qboolean bForceUpdate;
} modelAnim_t;

enum { DRAWMODEL_DM, DRAWMODEL_TEAM };

extern qboolean drawTeamModel;

typedef struct {
	int frametime;
	int realtime;
	int cursorx;
	int cursory;
	int menusp;
	menuframework_s *activemenu;
	menuframework_s *stack[MAX_MENUDEPTH];
	qboolean debug;
	qhandle_t whiteShader;
	qhandle_t menuWallpapers;
	qhandle_t cursor;
	qhandle_t corner;
	qhandle_t rb_on;
	qhandle_t rb_off;
	float scale;
	float bias;
	float wideoffset;
	qboolean firstdraw;
	qboolean onmap;
} uiStatic_t;

// ui_atoms.c
void QDECL Com_Error(int level, const char *error, ...);
void QDECL Com_Printf(const char *msg, ...);
void UI_UpdateState(void);
void UI_PushMenu(menuframework_s *menu);
void UI_PopMenu(void);
void UI_ForceMenuOff(void);
qboolean UI_IsFullscreen(void);
void UI_SetActiveMenu(uiMenuCommand_t menu);
void UI_KeyEvent(int key, int down);
void UI_MouseEvent(int dx, int dy);
qboolean UI_ConsoleCommand(int realTime);
void UI_Shutdown(void);
void UI_Init(void);
void UI_AdjustFrom640(float *x, float *y, float *w, float *h);
void UI_DrawHandlePic(float x, float y, float w, float h, qhandle_t hShader);
void UI_DrawPictureElement(float x, float y, float w, float h, const char *file);
void UI_DrawModelElement(float x, float y, float w, float h, const char *model, float scale);
void UI_DrawRoundedRect(float x, float y, float width, float height, float radius, const float *color);

// ui_connect.c
void UI_DrawConnectScreen(qboolean overlay);

// ui_controls.c
void UI_Controls(void);

// ui_gameinfo.c
void *UI_Alloc(int size);
void UI_InitMemory(void);
void UI_LoadArenas(void);
void UI_LoadBots(void);
char *UI_GetBotInfoByName(const char *name);
int UI_GetNumBots(void);
void UI_FillListOfMaps(menuelement_s *e, char *gametype, char *names, int namesSize, char **configlist);
void UI_FillListOfBots(menuelement_s *e, char *names, int namesSize, char **configlist);
int UI_CountOfMaps(char *gametype);

// ui_main.c
void UI_CreateCvars(void);

// ui_menu.c
void UI_MainMenu(void);

// ui_newgame.c
void UI_NewGame(void);

// ui_options.c
void UI_Options(void);

// ui_playermodel.c
void UI_PlayerModelMenu(void);

// ui_players.c
void GUI_PlayerInfo_InitModel(modelAnim_t *m);
void GUI_PlayerInfo_DrawTeamModel(modelAnim_t *m, qboolean teamModel);
const char *GUI_ModelName(const char *modelname);
void GUI_PlayerInfo_AnimateModel(modelAnim_t *m);

// ui_qmenu.c
sfxHandle_t ScrollList_Key(menuelement_s *l, int key);
void Menu_AddItem(menuframework_s *menu, menuelement_s *item);
void Menu_SetCursor(menuframework_s *m, int cursor);
void Menu_Draw(menuframework_s *menu);
void *Menu_ItemAtCursor(menuframework_s *m);
sfxHandle_t Menu_DefaultKey(menuframework_s *m, int key);
void Menu_Cache(void);
void UI_FillList(menuelement_s *e, char *location, char *itemsLocation, char *extension, char *names, int namesSize, char **configlist);
int UI_CountFiles(const char *location, const char *extension);
void UI_FillListFromArray(menuelement_s *e, char **configlist, char **items, int maxItems);
void UI_FillListOfItems(menuelement_s *e, char *names, int namesSize, char **configlist);
void UI_FillListPlayers(menuelement_s *e, char **configlist, char *names, int namesSize);
int UI_ListPlayerCount(void);
void UI_SetHitbox(menuelement_s *e, float x, float y, float w, float h);
void UI_CreateUI(menuframework_s *menu, menuelement_s *e);
void UI_CButton(menuelement_s *e, float x, float y, char *text, int style, float size, float *color, char *cmd, char *var, void (*func)(void), void (*callback)(void *self, int event), int callid);
void UI_CSlider(menuelement_s *e, float x, float y, char *text, char *var, float min, float max, float mod, void (*callback)(void *self, int event), int callid);
void UI_CRadioButton(menuelement_s *e, float x, float y, char *text, char *var, int mod, void (*callback)(void *self, int event), int callid);
void UI_CSpinControl(menuelement_s *e, float x, float y, char *text, const char **list, char *var, void (*callback)(void *self, int event), int callid);
void UI_CList(menuelement_s *e, float x, float y, float size, int h, int w, float pad_x, float pad_y, int style, qboolean drawText, int corner, float *color, void (*callback)(void *self, int event), int callid);
void UI_CField(menuelement_s *e, float x, float y, char *text, int w, int maxchars, float *color, char *var, void (*callback)(void *self, int event), int callid);
void UI_CText(menuelement_s *e, float x, float y, char *text, int style, float size);
void UI_CBitmap(menuelement_s *e, float x, float y, float w, float h, int pic, int flags, char *cmd, char *var, void (*func)(void), void (*callback)(void *self, int event), int callid);
void UI_CPicture(menuelement_s *e, float x, float y, float w, float h, int pic, int flags, char *cmd, char *var, void (*func)(void), void (*callback)(void *self, int event), int callid);

// ui_servers.c
void UI_ArenaServersMenu(void);

// ui_settings.c
void UI_Settings(void);

// ui_spawnmenu.c
void UI_SpawnMenu(void);

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
void trap_Key_KeynumToStringBuf(int keynum, char *buf, int buflen);
void trap_Key_GetBindingBuf(int keynum, char *buf, int buflen);
void trap_Key_SetBinding(int keynum, const char *binding);
qboolean trap_Key_IsDown(int keynum);
qboolean trap_Key_GetOverstrikeMode(void);
void trap_Key_SetOverstrikeMode(qboolean state);
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
