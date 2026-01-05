// Copyright (C) 2023-2026 Noire.dev
// OpenSandbox — GPLv2; see LICENSE for details.

#include "js_local.h"

js_args_t vmargs;
js_result_t vmresult;
js_args_t jsargs;
js_result_t jsresult;

#define VALUEINDEX 2
static int foundEntry = qfalse;

void JS_StackClean(void) { memset(&jsargs, 0, sizeof(jsargs)); }
void JS_Function(int func_id) {
	memset(&jsresult, 0, sizeof(jsresult));
	JSCall(func_id, &jsargs, &jsresult);
}

void JS_Int(int number) {
    vmresult.t = JS_TYPE_INT;
    vmresult.v.i = number;
}

void JS_Float(float number) {
    vmresult.t = JS_TYPE_FLOAT;
    vmresult.v.f = number;
}

void JS_String(const char *string) {
    vmresult.t = JS_TYPE_STRING;
    Q_StringCopy(vmresult.v.s, string, MAX_JS_STRINGSIZE);
}

void JSE_Int(int* number) {
    foundEntry = qtrue;
    if(vmargs.t[VALUEINDEX] != JS_TYPE_NONE) *number = vmargs.v[VALUEINDEX].i;
    vmresult.t = JS_TYPE_INT;
    vmresult.v.i = *number;
}

void JSE_Float(float* number) {
    foundEntry = qtrue;
    if(vmargs.t[VALUEINDEX] != JS_TYPE_NONE) *number = vmargs.v[VALUEINDEX].f;
    vmresult.t = JS_TYPE_FLOAT;
    vmresult.v.f = *number;
}

void JSE_String(char* string, size_t maxSize) {
    foundEntry = qtrue;
    if(vmargs.t[VALUEINDEX] != JS_TYPE_NONE) Q_StringCopy(string, vmargs.v[VALUEINDEX].s, maxSize);
    vmresult.t = JS_TYPE_STRING;
    Q_StringCopy(vmresult.v.s, string, MAX_JS_STRINGSIZE);
}

static void JSEntry(const char *entry, const char *name, int idx1, int idx2, int idx3, int idx4) {
    foundEntry = qfalse;
#ifndef GAME
    if(!strcmp(entry, "cgui")) {
        if(!strcmp(name, "defaultFont[]")) JSE_Int(&cgui.defaultFont[idx1]);
        if(!strcmp(name, "whiteShader")) JSE_Int(&cgui.whiteShader);
        if(!strcmp(name, "corner")) JSE_Int(&cgui.corner);
        if(!strcmp(name, "scale")) JSE_Float(&cgui.scale);
        if(!strcmp(name, "bias")) JSE_Float(&cgui.bias);
        if(!strcmp(name, "wideoffset")) JSE_Float(&cgui.wideoffset);
        if(!strcmp(name, "colors[][]")) JSE_Float(&cgui.colors[idx1][idx2]);
    }
#endif
#ifdef UI
    if(!strcmp(entry, "uis")) {
        if(!strcmp(name, "onmap")) JSE_Int(&uis.onmap);
    }
    if(!strcmp(entry, "console")) {
        if(!strcmp(name, "lines[]")) JSE_String(console.lines[idx1], sizeof(console.lines[idx1]));
        if(!strcmp(name, "linescount")) JSE_Int(&console.linescount);
    }
#endif
    if(!foundEntry) trap_Print(va("^1Invalid entry: %s in %s \n", name, entry));
}

void VMCall(int func_id) {
    switch(func_id) {
    case VM_ENTRY: JSEntry(vmargs.v[0].s, vmargs.v[1].s, vmargs.v[3].i, vmargs.v[4].i, vmargs.v[5].i, vmargs.v[6].i); break;
#ifdef CGAME
    case VM_DRAWSTRING: ST_DrawString(vmargs.v[0].f, vmargs.v[1].f, vmargs.v[2].s, vmargs.v[3].i, color_white, vmargs.v[4].f); break;
#endif
#ifdef UI
    case VM_DRAWSTRING: ST_DrawString(vmargs.v[0].f, vmargs.v[1].f, vmargs.v[2].s, vmargs.v[3].i, color_white, vmargs.v[4].f); break;
    case VM_DRAWRECT: ST_DrawRoundedRect(vmargs.v[0].f, vmargs.v[1].f, vmargs.v[2].f, vmargs.v[3].f, vmargs.v[4].f, color_dim); break;
    case VM_CREATEBUTTON: UI_CButton(vmargs.v[0].i, vmargs.v[1].f, vmargs.v[2].f, vmargs.v[3].f, vmargs.v[4].f, vmargs.v[5].s, vmargs.v[6].i, vmargs.v[7].f, vmargs.v[8].i, vmargs.v[9].i, vmargs.v[10].i, vmargs.v[11].i, vmargs.v[12].s, NULL); break;
#endif
	}
}
