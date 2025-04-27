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

#include "ui_local.h"

#define SPACING		34

#define ID_DEFAULTS 0
#define ID_LANGUAGE 1
#define ID_MUSIC 	2

typedef struct {
	menuframework_s	menu;
	menuelement_s	e[OSUI_MAX_ELEMENTS];

	char			names1[524288];
	char*			list1[524288];
} setup_t;

static setup_t	setup;

static void Setup_Defaults_Action( qboolean result ) {
	if( !result ) return;
	trap_Cmd_ExecuteText( EXEC_APPEND, "exec default.cfg\n");
	trap_Cmd_ExecuteText( EXEC_APPEND, "cvar_restart\n");
	trap_Cmd_ExecuteText( EXEC_APPEND, "vid_restart\n" );
}

static void UI_SetupMenu_Event( void *ptr, int event ) {
	if( event != QM_ACTIVATED ) {
		return;
	}

	switch( ((menucommon_s*)ptr)->callid ) {
	case ID_DEFAULTS:
		UI_ConfirmMenu( "SET TO DEFAULTS?", Setup_Defaults_Action );
		break;

	case ID_LANGUAGE:
		//Make language list
		break;

	case ID_MUSIC:
		trap_Cmd_ExecuteText( EXEC_APPEND, va("music \"music/%s\" \n", setup.e[12].itemnames[setup.e[12].curvalue]) );
		break;

	}
}

void UI_SetupMenu( void ) {
	int				y;

	memset( &setup, 0, sizeof(setup) );
	setup.menu.native		= qfalse;
	setup.menu.fullscreen 	= qtrue;

	UI_CText(&setup.e[0], 320, 16, "OPTIONS", UI_CENTER, 1.80);

	y = 128 - SPACING;
	UI_CTextButton(&setup.e[1], 320, y, "PLAYER", UI_CENTER, 1.30, NULL, NULL, UI_PlayerSettingsMenu, NULL, 0); y += SPACING;
	UI_CTextButton(&setup.e[2], 320, y, "MODEL", UI_CENTER, 1.30, NULL, NULL, UI_PlayerModelMenu, NULL, 0); y += SPACING;
	UI_CTextButton(&setup.e[3], 320, y, "CONTROLS", UI_CENTER, 1.30, NULL, NULL, UI_ControlsMenu, NULL, 0); y += SPACING;
	UI_CTextButton(&setup.e[4], 320, y, "SYSTEM", UI_CENTER, 1.30, NULL, NULL, UI_GraphicsOptionsMenu, NULL, 0); y += SPACING;
	UI_CTextButton(&setup.e[5], 320, y, "GAME OPTIONS", UI_CENTER, 1.30, NULL, NULL, UI_PreferencesMenu, NULL, 0); y += SPACING;
	UI_CTextButton(&setup.e[6], 320, y, "ADVANCED", UI_CENTER, 1.30, NULL, NULL, UI_AdvancedMenu, NULL, 0); y += SPACING;
	UI_CTextButton(&setup.e[7], 320, y, "LOAD/EXEC CONFIG", UI_CENTER, 1.30, NULL, NULL, UI_LoadConfigMenu, NULL, 0); y += SPACING;
	UI_CTextButton(&setup.e[8], 320, y, "SAVE CONFIG", UI_CENTER, 1.30, NULL, NULL, UI_SaveConfigMenu, NULL, 0); y += SPACING;
	UI_CTextButton(&setup.e[9], 320, y, "DEFAULTS", UI_CENTER, 1.30, NULL, NULL, NULL, UI_SetupMenu_Event, ID_DEFAULTS); y += SPACING;
	UI_CTextButton(&setup.e[10], 320, y, "РУССКИЙ", UI_CENTER, 1.30, NULL, NULL, NULL, UI_SetupMenu_Event, ID_LANGUAGE);

	UI_CBitmap(&setup.e[11], 0 - uis.wideoffset, 480-64, 128, 64, AST_BACK, 0, NULL, NULL, UI_PopMenu, NULL, 0);

	UI_CList(&setup.e[12], 0-uis.wideoffset, 20, 24, 10, LST_ICONS, 1.00, 1, UI_SetupMenu_Event, ID_MUSIC);
	UI_FillList(&setup.e[12], "music/", "", setup.names1, 524288, setup.list1);

	UI_CreateUI( &setup.menu, setup.e);
	UI_PushMenu( &setup.menu );
}
