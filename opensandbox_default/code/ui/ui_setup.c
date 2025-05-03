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

#define ID_DEFAULTS 	100
#define ID_LANGUAGE 	99

typedef struct {
	menuframework_s	menu;
	menuelement_s	e[OSUI_MAX_ELEMENTS];
} setup_t;

static setup_t	setup;

static void Setup_Defaults_Action( qboolean result ) {
	if( !result ) return;
	
	trap_Cmd_ExecuteText( EXEC_APPEND, "exec default.cfg\n");
	trap_Cmd_ExecuteText( EXEC_APPEND, "cvar_restart\n");
	trap_Cmd_ExecuteText( EXEC_APPEND, "vid_restart\n" );
}

static void UI_Setup_Event( void *ptr, int event ) {
	if( event != QM_ACTIVATED ) { return; }

	switch( ((menucommon_s*)ptr)->callid ) {
	case ID_DEFAULTS:
		UI_ConfirmMenu( "SET TO DEFAULTS?", Setup_Defaults_Action );
		break;

	case ID_LANGUAGE:
		//Make language list
		break;
	}
}

void UI_Setup( void ) {
	int				y;

	memset( &setup, 0, sizeof(setup) );
	setup.menu.native		= qfalse;
	setup.menu.fullscreen 	= qtrue;

	UI_CText(&setup.e[0], OSUI_LOGO_X, OSUI_LOGO_Y+24, "OPTIONS", UI_LEFT, 1.80);

	y = OSUI_STANDARD_Y;
	UI_CButton(&setup.e[1], 64 - uis.wideoffset, y, "Player", UI_LEFT, 1.00, NULL, NULL, UI_PlayerSettingsMenu, NULL, 0); y += OSUI_SPACING_Y;
	UI_CButton(&setup.e[2], 64 - uis.wideoffset, y, "Model", UI_LEFT, 1.00, NULL, NULL, UI_PlayerModelMenu, NULL, 0); y += OSUI_BIGSPACING_Y;

	UI_CButton(&setup.e[3], 64 - uis.wideoffset, y, "Controls", UI_LEFT, 1.00, NULL, NULL, UI_Controls, NULL, 0); y += OSUI_SPACING_Y;
	UI_CButton(&setup.e[4], 64 - uis.wideoffset, y, "System", UI_LEFT, 1.00, NULL, NULL, UI_GraphicsOptionsMenu, NULL, 0); y += OSUI_SPACING_Y;
	UI_CButton(&setup.e[5], 64 - uis.wideoffset, y, "Game Options", UI_LEFT, 1.00, NULL, NULL, UI_PreferencesMenu, NULL, 0); y += OSUI_SPACING_Y;
	UI_CButton(&setup.e[6], 64 - uis.wideoffset, y, "Advanced", UI_LEFT, 1.00, NULL, NULL, UI_AdvancedMenu, NULL, 0); y += OSUI_BIGSPACING_Y;

	UI_CButton(&setup.e[7], 64 - uis.wideoffset, y, "Load Config", UI_LEFT, 1.00, NULL, NULL, UI_LoadConfigMenu, NULL, 0); y += OSUI_SPACING_Y;
	UI_CButton(&setup.e[8], 64 - uis.wideoffset, y, "Save Config", UI_LEFT, 1.00, NULL, NULL, UI_SaveConfigMenu, NULL, 0); y += OSUI_BIGSPACING_Y;

	UI_CButton(&setup.e[ID_DEFAULTS], 64 - uis.wideoffset, y, "Defaults", UI_LEFT, 1.00, NULL, NULL, NULL, UI_Setup_Event, ID_DEFAULTS); y += OSUI_SPACING_Y;
	UI_CButton(&setup.e[ID_LANGUAGE], 64 - uis.wideoffset, y, "Language", UI_LEFT, 1.00, NULL, NULL, NULL, UI_Setup_Event, ID_LANGUAGE); y += OSUI_BIGSPACING_Y;

	UI_CButton(&setup.e[9], 64 - uis.wideoffset, y, "Back", UI_LEFT, 1.00, NULL, NULL, UI_PopMenu, NULL, 0);

	UI_CreateUI( &setup.menu, setup.e);
	UI_PushMenu( &setup.menu );
}
