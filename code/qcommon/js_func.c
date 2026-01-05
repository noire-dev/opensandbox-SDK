// Copyright (C) 2023-2026 Noire.dev
// OpenSandbox — GPLv2; see LICENSE for details.

#include "js_local.h"

int JS_MenuCheck(void) {
    JS_StackClean();
    JS_Function(JS_MENUCHECK);
    return jsresult.v.i;
}

void JS_MenuInit(void) {
    JS_StackClean();
    JS_Function(JS_MENUINIT);
}

void JS_MenuDraw(void) {
    JS_StackClean();
    JS_Function(JS_MENUDRAW);
}
