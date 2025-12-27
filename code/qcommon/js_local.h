// Copyright (C) 2023-2026 Noire.dev
// OpenSandbox — GPLv2; see LICENSE for details.

#include "q_shared.h"
#ifdef GAME
#include "../game/bg_public.h"
#include "../game/g_public.h"
#include "../game/g_local.h"
#include "../game/ai_main.h"
#endif
#ifdef CGAME
#include "../renderer/tr_types.h"
#include "../game/bg_public.h"
#include "../cgame/cg_public.h"
#include "../cgame/cg_local.h"
#endif
#ifdef UI
#include "../renderer/tr_types.h"
#include "../game/bg_public.h"
#include "../ui/ui_public.h"
#include "../client/keycodes.h"
#include "../ui/ui_local.h"
#endif

#define MAX_JS_ARGS 32
#define MAX_JS_STRINGSIZE 4096
#define GETVMCONTEXT 1000
#define VMCALL 1001

typedef enum {
	JSCALL_TEST = 0,
	JSCALL_MENUINIT = 1,
	JSCALL_MENUDRAW = 2,
} jscall_t;

typedef enum {
	VMCALL_TEST = 0,
	VMCALL_DRAWSTRING = 1,
} vmcall_t;

typedef enum { JS_TYPE_NONE, JS_TYPE_INT, JS_TYPE_FLOAT, JS_TYPE_BOOL, JS_TYPE_STRING } js_type_t;

typedef union {
	int i;
	float f;
	qboolean b;
	char s[MAX_JS_STRINGSIZE];
} js_value_t;

typedef struct {
	js_type_t type[MAX_JS_ARGS];
	js_value_t value[MAX_JS_ARGS];
} js_args_t;

typedef struct {
	js_type_t type;
	js_value_t value;
} js_result_t;

void JS_StackClean(void);
void JS_Function(int func_id);

// JS Functions
const char *JS_Test(void);
void JS_MenuDraw(void);

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
