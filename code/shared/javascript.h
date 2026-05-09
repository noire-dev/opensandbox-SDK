// Copyright (C) 2023-2026 Noire's Mod — GPLv2

#define MAX_JS_ARGS 16
#define MAX_JS_STRINGSIZE 256
#define GETVMCONTEXT 1000
#define VMCALL 1001

#include "system.h"
#include "engine.h"
#include "core.h"
#ifdef GAME
#include "../game/bg_public.h"
#include "../game/g_public.h"
#include "../game/g_local.h"
#include "../game/ai_main.h"
#endif
#ifdef CGAME
#include "../shared/client/renderer.h"
#include "../game/bg_public.h"
#include "../cgame/cg_public.h"
#include "../cgame/cg_local.h"
#endif
#ifdef UI
#include "../shared/client/renderer.h"
#include "../game/bg_public.h"
#include "../ui/ui_local.h"
#endif

typedef enum {
	JS_SHELLINIT,
	JS_SHELLDRAW,
	JS_SHELLKEY,
	JS_SHELLCALLBACK,
	JS_SHELLUPDATE,
	JS_GETAPP,
	JS_GETAPPNAME,
	JS_GETAPPICON,
	JS_LAUNCHAPP,
} jscall_t;

typedef enum {
    // General
    VM_CMD,
    
    // cgame.qvm + ui.qvm
    VM_APICGUI,
    VM_APIGLCONFIG,
    VM_DRAWSTRING,
	VM_DRAWRECTANGLE,
	VM_DRAWSHADER,
	VM_DRAWCOLOREDSHADER,
    
    // ui.qvm
    VM_APISHELL,
    VM_APIWINDOW,
    VM_APIELEMENT,
    VM_APICONSOLE,
	VM_CLOSEWINDOW,
	VM_CLEARWINDOW,
	VM_GETFREEELEMENT,
	VM_CREATEWINDOW,
	VM_CREATEWINDOWBUTTON,
	VM_CREATEPICTURE,
	VM_CREATEBUTTON,
	VM_CREATECHECKBOX,
	VM_CREATESLIDER,
	VM_CREATEACTION,
	VM_CREATESPIN,
	VM_CREATEFIELD,
	VM_CREATELIST,
	VM_CREATELISTFILES,
	VM_FILLLISTFILES,
	VM_SETMARGIN,
} vmcall_t;

typedef enum { JS_TYPE_NONE, JS_TYPE_INT, JS_TYPE_FLOAT, JS_TYPE_STRING } js_type_t;

typedef struct {
    int i;
    float f;
    char s[MAX_JS_STRINGSIZE];
} js_value_t;

typedef struct {
    js_type_t t[MAX_JS_ARGS];
    js_value_t v[MAX_JS_ARGS];
} js_args_t;

typedef struct {
    js_type_t t;
    js_value_t v;
} js_result_t;

void JS_StackClean(void);
void JS_Function(int func_id);

// JS Functions
void JS_ShellInit(void);
void JS_ShellDraw(void);
void JS_ShellKey(int key, int windowID);
void JS_ShellCallback(int windowID, int elementID, int key);
void JS_ShellUpdate(int windowID);
int JS_GetApp(int id);
const char *JS_GetAppName(int id);
const char *JS_GetAppIcon(int id);
void JS_LaunchApp(char *nameID);

// JS shared colors
#define JSC_EMPTY 0
#define JSC_BLACK 1
#define JSC_WHITE 2
#define JSC_GREY 3
#define JSC_RED 4
#define JSC_GREEN 5
#define JSC_BLUE 6
#define JSC_DISABLED 7
#define JSC_ENABLED 8
#define JSC_CONTEXTMENU 9
#define JSC_WINDOWBUTTON 10
#define JSC_DEBUG1 11
#define JSC_DEBUG2 12

extern js_args_t vmargs;
extern js_result_t vmresult;
extern js_args_t jsargs;
extern js_result_t jsresult;

// syscalls
void VMContext(js_args_t *args, js_result_t *result);
qboolean JSOpenFile(const char *filename);
void JSLoadScripts(const char *path, const char *name);
qboolean JSEval(const char *code, js_result_t *result);
qboolean JSCall(int id, js_args_t *args, js_result_t *result);
