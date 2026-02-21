// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// OpenSandbox — GPLv2; see LICENSE for details.

#ifndef __UI_PUBLIC_H__
#define __UI_PUBLIC_H__

typedef struct {
	connstate_t connState;
	int connectPacketCount;
	int clientNum;
	char servername[MAX_STRING_CHARS];
	char updateInfoString[MAX_STRING_CHARS];
	char messageString[MAX_STRING_CHARS];
} uiClientState_t;

// clang-format off
typedef enum {
    UI_DISABLE,
    UI_ENABLE
} uiMenuCommand_t;
// clang-format on

// clang-format off
typedef enum {
    UI_INIT = 0,
    UI_SHUTDOWN,
    UI_KEY_EVENT,
    UI_MOUSE_EVENT,
    UI_REFRESH,
    UI_IS_FULLSCREEN,
    UI_SET_ACTIVE_MENU,
    UI_CONSOLE_COMMAND,
    UI_DRAW_CONNECT_SCREEN
} uiExport_t;
// clang-format on

#endif
