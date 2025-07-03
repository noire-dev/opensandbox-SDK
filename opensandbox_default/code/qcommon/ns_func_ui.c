// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

#include "ns_local.h"

void NS_getCvar(VarValue *modify, VarType type, const char *cvarName) {
    char cvarValue[MAX_VAR_CHAR_BUF];
    switch(type) {
        case TYPE_CHAR:
            trap_Cvar_VariableStringBuffer(cvarName, modify->c, sizeof(modify->c));
            break;
        case TYPE_INT:
            trap_Cvar_VariableStringBuffer(cvarName, cvarValue, sizeof(cvarValue));
            modify->i = atoi(cvarValue);
            break;
        case TYPE_FLOAT:
            trap_Cvar_VariableStringBuffer(cvarName, cvarValue, sizeof(cvarValue));
            modify->f = atof(cvarValue);
            break;
        default:
            return;
    }
}

int get_cvar_int(const char *name) {
    char cvarValue[MAX_VAR_CHAR_BUF];

    trap_Cvar_VariableStringBuffer(name, cvarValue, sizeof(cvarValue));
    return atoi(cvarValue);
}
float get_cvar_float(const char *name) {
    char cvarValue[MAX_VAR_CHAR_BUF];

    trap_Cvar_VariableStringBuffer(name, cvarValue, sizeof(cvarValue));
    return atof(cvarValue);
}

void NS_setCvar(const char *cvarName, const char *cvarValue) {
    trap_Cvar_Set(cvarName, cvarValue);

    if (!strcmp(cvarName, "r_postprocess"))
        trap_Cvar_Set("r_fx_blur", "0");
}
