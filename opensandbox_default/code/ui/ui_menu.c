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

#define ID_LINK			100

typedef struct {
	menuframework_s	menu;
	menuelement_s	e[OSUI_MAX_ELEMENTS];

	char			names1[524288];
	char*			list1[524288];
	char 			error[4096];
} mainmenu_t;

static mainmenu_t main;

static void MainMenu_ReloadAction( qboolean result ) {
	if( !result ) { return; }

	trap_Cmd_ExecuteText( EXEC_APPEND, "game_restart;" );
}

static void MainMenu_ReloadGame( void )
{
	if(cl_language.integer == 0){
		UI_ConfirmMenu( "RELOAD GAME?", MainMenu_ReloadAction );
	}
	if(cl_language.integer == 1){
		UI_ConfirmMenu( "ПЕРЕЗАГРУЗИТЬ ИГРУ?", MainMenu_ReloadAction );
	}
}

static void Main_MenuEvent (void* ptr, int event) {
	if( event != QM_ACTIVATED ) { return; }

	switch( ((menucommon_s*)ptr)->callid ) {
	case ID_LINK:
		if(trap_Cvar_VariableValue("os_windows")){
			trap_System("start https://opensandbox.neocities.org/");
		}
		if(trap_Cvar_VariableValue("os_linux")){
			trap_System("xdg-open https://opensandbox.neocities.org/");
		}
		if(trap_Cvar_VariableValue("os_macos")){
			trap_System("open https://opensandbox.neocities.org/");
		}
		break;

	}
}

static void Main_MenuDraw( void ) {
	vec4_t			color = {0.85, 0.9, 1.0, 1};
	vec4_t			modlistcolor = {0.00f, 0.00f, 0.00f, 0.40f};

	Menu_Draw( &main.menu );

	if (strlen(main.error)){
		ST_DrawString( 0-uis.wideoffset, 0, main.error, UI_SMALLFONT|UI_DROPSHADOW, color, 1.00 );
	}
	ST_DrawString( 610 + uis.wideoffset, 2, "2025.04.14", UI_RIGHT|UI_SMALLFONT, color, 1.00 );
}

void UI_MainMenu( void ) {
	int		y;

	memset( &main, 0 ,sizeof(mainmenu_t) );
	trap_Cvar_VariableStringBuffer( "com_errorMessage", main.error, sizeof(main.error) );
	main.menu.draw = Main_MenuDraw;
	main.menu.fullscreen = qtrue;

	y = OSUI_STANDARD_Y;
	if(uis.onmap){
		UI_CButton(&main.e[0], 64 - uis.wideoffset, y, "Resume game", UI_LEFT, 1.00, NULL, NULL, UI_ForceMenuOff, NULL, 0); y += OSUI_BIGSPACING_Y;
	}
	//UI_CButton(&main.e[1], 64 - uis.wideoffset, y, "Start New Game", UI_LEFT, 1.00, NULL, NULL, UI_StartServerMenu, NULL, 0); y += OSUI_SPACING_Y;
	//UI_CButton(&main.e[2], 64 - uis.wideoffset, y, "Find Multiplayer Game", UI_LEFT, 1.00, NULL, NULL, UI_ArenaServersMenu, NULL, 0); y += OSUI_BIGSPACING_Y;
	UI_CButton(&main.e[3], 64 - uis.wideoffset, y, "Profile", UI_LEFT, 1.00, NULL, NULL, UI_PlayerModelMenu, NULL, 0); y += OSUI_SPACING_Y;
	UI_CButton(&main.e[4], 64 - uis.wideoffset, y, "Mods", UI_LEFT, 1.00, NULL, NULL, UI_ModsMenu, NULL, 0); y += OSUI_SPACING_Y;
	UI_CButton(&main.e[5], 64 - uis.wideoffset, y, "Demos", UI_LEFT, 1.00, NULL, NULL, UI_DemosMenu, NULL, 0); y += OSUI_BIGSPACING_Y;
	UI_CButton(&main.e[6], 64 - uis.wideoffset, y, "Options", UI_LEFT, 1.00, NULL, NULL, UI_Options, NULL, 0); y += OSUI_BIGSPACING_Y;
	if(uis.onmap){
		UI_CButton(&main.e[7], 64 - uis.wideoffset, y, "Disconnect", UI_LEFT, 1.00, "disconnect \n", NULL, NULL, NULL, 0); y += OSUI_SPACING_Y;
	}
	UI_CButton(&main.e[8], 64 - uis.wideoffset, y, "Quit", UI_LEFT, 1.00, "quit \n", NULL, NULL, NULL, 0);

	UI_CPicture(&main.e[9], OSUI_LOGO_X, OSUI_LOGO_Y, 158, 55, AST_OSLOGO, 0, NULL, NULL, NULL, NULL, 0);

	//UI_CBitmap(&main.e[10], 315 + 256*0.62 + 5 + uis.wideoffset, 450, 256*0.62, 38*0.62, AST_MOD, 0, NULL, NULL, StartServer_ServerPage_Mods, NULL, 0);
	UI_CBitmap(&main.e[ID_LINK], 315 + uis.wideoffset, 450, 256*0.62, 38*0.62, AST_LINK, 0, NULL, NULL, NULL, Main_MenuEvent, ID_LINK);

	uis.menusp = 0;

	UI_CreateUI( &main.menu, main.e);
	trap_Key_SetCatcher( KEYCATCH_UI );
	UI_PushMenu ( &main.menu );
}
