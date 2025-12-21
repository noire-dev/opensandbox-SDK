// Copyright (C) 2023-2026 Noire.dev
// OpenSandbox — GPLv2; see LICENSE for details.

#include "js_local.h"

const char* JS_Test(void) {
    JS_Function(JSCALL_TEST);
    return jsresult.value.s;
}
