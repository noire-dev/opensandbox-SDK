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
#ifdef GAME
	switch(func_id) {
	case VMCALL_TEST: G_Printf("QVM Test String!\n");
	}
#endif
}
