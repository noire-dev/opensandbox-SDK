// Copyright (C) 2023-2026 Noire.dev
// OpenSandbox — GPLv2; see LICENSE for details.

#include "js_local.h"

js_args_t vmargs;
js_result_t vmresult;
js_args_t jsargs;
js_result_t jsresult;

void JS_StackClean(void) { memset(&jsargs, 0, sizeof(jsargs)); }
void JS_Function(int func_id) {
	memset(&jsresult, 0, sizeof(jsresult));
	JSCall(func_id, &jsargs, &jsresult);
}

void VMCall(int func_id) {
    switch(func_id) {
#ifdef GAME
    case VMCALL_TEST: trap_Print("QVM Test String from game.qvm!\n");
#endif
#ifdef CGAME
    case VMCALL_TEST: trap_Print("QVM Test String from cgame.qvm!\n");
    case VMCALL_DRAWSTRING: ST_DrawString(vmargs.value[0].f, vmargs.value[1].f, vmargs.value[2].s, vmargs.value[3].i, color_white, vmargs.value[4].f);
#endif
#ifdef UI
    case VMCALL_TEST: trap_Print("QVM Test String from ui.qvm!\n");
    case VMCALL_DRAWSTRING: ST_DrawString(vmargs.value[0].f, vmargs.value[1].f, vmargs.value[2].s, vmargs.value[3].i, color_white, vmargs.value[4].f);
#endif
	}
}
