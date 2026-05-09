// Copyright (C) 2023-2026 Noire's Mod — GPLv2

#include "javascript.h"

void JS_ShellInit(void) {
    JS_StackClean();
    JS_Function(JS_SHELLINIT);
}

void JS_ShellDraw(void) {
    JS_StackClean();
    JS_Function(JS_SHELLDRAW);
}

void JS_ShellKey(int key, int windowID) {
    JS_StackClean();
    jsargs.t[0] = JS_TYPE_INT;
    jsargs.v[0].i = key;
    jsargs.t[1] = JS_TYPE_INT;
    jsargs.v[1].i = windowID;
    JS_Function(JS_SHELLKEY);
}

void JS_ShellCallback(int windowID, int elementID, int key) {
    JS_StackClean();
    jsargs.t[0] = JS_TYPE_INT;
    jsargs.v[0].i = windowID;
    jsargs.t[1] = JS_TYPE_INT;
    jsargs.v[1].i = elementID;
    jsargs.t[2] = JS_TYPE_INT;
    jsargs.v[2].i = key;
    JS_Function(JS_SHELLCALLBACK);
}

void JS_ShellUpdate(int windowID) {
    JS_StackClean();
    jsargs.t[0] = JS_TYPE_INT;
    jsargs.v[0].i = windowID;
    JS_Function(JS_SHELLUPDATE);
}

int JS_GetApp(int id) {
    JS_StackClean();
    jsargs.t[0] = JS_TYPE_INT;
    jsargs.v[0].i = id;
    JS_Function(JS_GETAPP);
    return jsresult.v.i;
}

const char *JS_GetAppName(int id) {
    JS_StackClean();
    jsargs.t[0] = JS_TYPE_INT;
    jsargs.v[0].i = id;
    JS_Function(JS_GETAPPNAME);
    return jsresult.v.s;
}

const char *JS_GetAppIcon(int id) {
    JS_StackClean();
    jsargs.t[0] = JS_TYPE_INT;
    jsargs.v[0].i = id;
    JS_Function(JS_GETAPPICON);
    return jsresult.v.s;
}

void JS_LaunchApp(char *nameID) {
    JS_StackClean();
    jsargs.t[0] = JS_TYPE_STRING;
    StringCopy(jsargs.v[0].s, nameID, MAX_JS_STRINGSIZE);
    JS_Function(JS_LAUNCHAPP);
}
