// 
// OpenSandbox
// 
// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2008-2012 OpenArena Team
// Copyright (C) 2023-2024 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// 
// This file is part of OpenSandbox.
// 
// OpenSandbox is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License, version 2,
// as published by the Free Software Foundation.
// 
// This modified code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this project. If not, see <http://www.gnu.org/licenses/>.
// 
// Contact: opensandboxteam@gmail.com
// 

#include "ns_local.h"

void NS_getCvar(VarValue *modify, VarType type, const char *cvarName)
{
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

// Функции для возвращения значений cvar
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
char* get_cvar_char(const char *name) {
    char cvarValue[MAX_VAR_CHAR_BUF];

    trap_Cvar_VariableStringBuffer(name, cvarValue, sizeof(cvarValue));
    return cvarValue;
}

void NS_setCvar(const char *cvarName, const char *cvarValue)
{
    trap_Cvar_Set(cvarName, cvarValue);
}

void NS_createCvar(const char *cvarName, const char *cvarValue)
{
    trap_Cvar_Register(NULL, cvarName, cvarValue, CVAR_ARCHIVE );
    trap_Cvar_Set(cvarName, cvarValue);
}

int NS_getPlayerCount(void)
{
    int playerCount = 0;
    int i;
    gclient_t *cl;

    for (i = 0; i < level.maxclients; i++, cl++) {
        cl = &level.clients[i];

        if (cl->pers.connected != CON_DISCONNECTED) {
            playerCount++;
        }
    }

    return playerCount;
}

/*void NS_getPlayerById(Variable *result, const char *field, int id, int index)
{
    gclient_t *cl;

    if (id < 0 || id >= level.maxclients) {
        return 0.0f;
    }

    cl = &level.clients[id];

    if (cl->pers.connected == CON_DISCONNECTED) {
        return 0.0f;
    }

    switch (dir) {
        case 0:
            return cl->ps.origin[0];
        case 1:
            return cl->ps.origin[1];
        case 2:
            return cl->ps.origin[2];
        default:
            return 0.0f;
    }
}

void NS_setPlayerPosById(int id, int dir, float value)
{
    gclient_t *cl;

    if (id < 0 || id >= level.maxclients) {
        return;
    }

    cl = &level.clients[id];

    if (cl->pers.connected == CON_DISCONNECTED) {
        return;
    }

    switch (dir) {
        case 0:
            cl->ps.origin[0] = value;
        case 1:
            cl->ps.origin[1] = value;
        case 2:
            cl->ps.origin[2] = value;
        default:
            return;
    }
}*/
