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
//
#include "ui_local.h"


#define ART_BACK0		"menu/assets/back_0"
#define ART_BACK1		"menu/assets/back_1"	

#define VERTICAL_SPACING	30

#define ID_BACK			10
#define ID_CIN_IDLOGO	11
#define ID_CIN_INTRO	12
#define ID_CIN_TIER1	13
#define ID_CIN_TIER2	14
#define ID_CIN_TIER3	15
#define ID_CIN_TIER4	16
#define ID_CIN_TIER5	17
#define ID_CIN_TIER6	18


typedef struct {
	menuframework_s	menu;
	menuelement_s		banner;
	menuelement_s		cin_idlogo;
	menuelement_s		cin_intro;
	menuelement_s		cin_tier1;
	menuelement_s		cin_tier2;
	menuelement_s		cin_tier3;
	menuelement_s		cin_tier4;
	menuelement_s		cin_tier5;
	menuelement_s		cin_tier6;
	menuelement_s	back;
} savesMenuInfo_t;

static savesMenuInfo_t	savesMenuInfo;

static char *saves[] = {
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8"
};

/*
===============
UI_SavesMenu_BackEvent
===============
*/
static void UI_SavesMenu_BackEvent( void *ptr, int event ) {
	if( event != QM_ACTIVATED ) {
		return;
	}
	UI_PopMenu();
}


/*
===============
UI_SavesMenu_Event
===============
*/
static void UI_SavesMenu_Event( void *ptr, int event ) {
	int		n;

	if (event != QM_ACTIVATED)
		return;

	trap_Cmd_ExecuteText( EXEC_APPEND, "sp_save\n" );

	n = ((menucommon_s*)ptr)->id - ID_CIN_IDLOGO;
	trap_Cmd_ExecuteText( EXEC_APPEND, va( "writeconfig save0%i\n", atoi(saves[n]) ) );
	UI_PopMenu();
}

/*
===============
UI_SavesMenu_Event2
===============
*/
static void UI_SavesMenu_Event2( void *ptr, int event ) {
	int		n;

	if (event != QM_ACTIVATED)
		return;

	n = ((menucommon_s*)ptr)->id - ID_CIN_IDLOGO;
	trap_Cmd_ExecuteText( EXEC_APPEND, va( "exec save0%i.cfg\n", atoi(saves[n]) ) );
	trap_Cmd_ExecuteText( EXEC_APPEND, "wait 50\n" );
	UI_StartServerMenu();
	StartServer_LoadServer(NULL);
}


/*
===============
UI_SavesMenu_Init
===============
*/
static void UI_SavesMenu_Init( int load ) {
	int		y;

	UI_SavesMenu_Cache();

	memset( &savesMenuInfo, 0, sizeof(savesMenuInfo) );
	savesMenuInfo.menu.fullscreen = qtrue;

	savesMenuInfo.banner.generic.type		= MTYPE_TEXT;
	savesMenuInfo.banner.generic.x			= 320;
	savesMenuInfo.banner.generic.y			= 16;
	if(cl_language.integer == 0){
	if(!load){
	savesMenuInfo.banner.string			= "SAVE GAME";
	} else {
	savesMenuInfo.banner.string			= "LOAD GAME";
	}
	}
	if(cl_language.integer == 1){
	if(!load){
	savesMenuInfo.banner.string			= "СОХРАНИТЬ ИГРУ";
	} else {
	savesMenuInfo.banner.string			= "ЗАГРУЗИТЬ ИГРУ";
	}
	}
	savesMenuInfo.banner.color				= color_white;
	savesMenuInfo.banner.style				= UI_CENTER;

	y = 100;
	savesMenuInfo.cin_idlogo.generic.type		= MTYPE_PTEXT;
	savesMenuInfo.cin_idlogo.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	savesMenuInfo.cin_idlogo.generic.x			= 320;
	savesMenuInfo.cin_idlogo.generic.y			= y;
	savesMenuInfo.cin_idlogo.generic.id		= ID_CIN_IDLOGO;
	savesMenuInfo.cin_idlogo.string			= "SLOT 1";
	savesMenuInfo.cin_idlogo.color				= color_white;
	savesMenuInfo.cin_idlogo.style				= UI_CENTER;

	y += VERTICAL_SPACING;
	savesMenuInfo.cin_intro.generic.type		= MTYPE_PTEXT;
	savesMenuInfo.cin_intro.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	savesMenuInfo.cin_intro.generic.x			= 320;
	savesMenuInfo.cin_intro.generic.y			= y;
	savesMenuInfo.cin_intro.generic.id			= ID_CIN_INTRO;
	savesMenuInfo.cin_intro.string				= "SLOT 2";
	savesMenuInfo.cin_intro.color				= color_white;
	savesMenuInfo.cin_intro.style				= UI_CENTER;

	y += VERTICAL_SPACING;
	savesMenuInfo.cin_tier1.generic.type		= MTYPE_PTEXT;
	savesMenuInfo.cin_tier1.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	savesMenuInfo.cin_tier1.generic.x			= 320;
	savesMenuInfo.cin_tier1.generic.y			= y;
	savesMenuInfo.cin_tier1.generic.id			= ID_CIN_TIER1; 
	savesMenuInfo.cin_tier1.string				= "SLOT 3";
	savesMenuInfo.cin_tier1.color				= color_white;
	savesMenuInfo.cin_tier1.style				= UI_CENTER;

	y += VERTICAL_SPACING;
	savesMenuInfo.cin_tier2.generic.type		= MTYPE_PTEXT;
	savesMenuInfo.cin_tier2.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	savesMenuInfo.cin_tier2.generic.x			= 320;
	savesMenuInfo.cin_tier2.generic.y			= y;
	savesMenuInfo.cin_tier2.generic.id			= ID_CIN_TIER2;
	savesMenuInfo.cin_tier2.string				= "SLOT 4";
	savesMenuInfo.cin_tier2.color				= color_white;
	savesMenuInfo.cin_tier2.style				= UI_CENTER;

	y += VERTICAL_SPACING;
	savesMenuInfo.cin_tier3.generic.type		= MTYPE_PTEXT;
	savesMenuInfo.cin_tier3.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	savesMenuInfo.cin_tier3.generic.x			= 320;
	savesMenuInfo.cin_tier3.generic.y			= y;
	savesMenuInfo.cin_tier3.generic.id			= ID_CIN_TIER3;
	savesMenuInfo.cin_tier3.string				= "SLOT 5";
	savesMenuInfo.cin_tier3.color				= color_white;
	savesMenuInfo.cin_tier3.style				= UI_CENTER;

	y += VERTICAL_SPACING;
	savesMenuInfo.cin_tier4.generic.type		= MTYPE_PTEXT;
	savesMenuInfo.cin_tier4.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	savesMenuInfo.cin_tier4.generic.x			= 320;
	savesMenuInfo.cin_tier4.generic.y			= y;
	savesMenuInfo.cin_tier4.generic.id			= ID_CIN_TIER4;
	savesMenuInfo.cin_tier4.string				= "SLOT 6";
	savesMenuInfo.cin_tier4.color				= color_white;
	savesMenuInfo.cin_tier4.style				= UI_CENTER;

	y += VERTICAL_SPACING;
	savesMenuInfo.cin_tier5.generic.type		= MTYPE_PTEXT;
	savesMenuInfo.cin_tier5.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	savesMenuInfo.cin_tier5.generic.x			= 320;
	savesMenuInfo.cin_tier5.generic.y			= y;
	savesMenuInfo.cin_tier5.generic.id			= ID_CIN_TIER5;
	savesMenuInfo.cin_tier5.string				= "SLOT 7";
	savesMenuInfo.cin_tier5.color				= color_white;
	savesMenuInfo.cin_tier5.style				= UI_CENTER;

	y += VERTICAL_SPACING;
	savesMenuInfo.cin_tier6.generic.type		= MTYPE_PTEXT;
	savesMenuInfo.cin_tier6.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	savesMenuInfo.cin_tier6.generic.x			= 320;
	savesMenuInfo.cin_tier6.generic.y			= y;
	savesMenuInfo.cin_tier6.generic.id			= ID_CIN_TIER6;	
	savesMenuInfo.cin_tier6.string				= "SLOT 8";
	savesMenuInfo.cin_tier6.color				= color_white;
	savesMenuInfo.cin_tier6.style				= UI_CENTER;
	
	
	if(!load){
	savesMenuInfo.cin_tier6.generic.callback	= UI_SavesMenu_Event;
	savesMenuInfo.cin_tier5.generic.callback	= UI_SavesMenu_Event; 
	savesMenuInfo.cin_tier4.generic.callback	= UI_SavesMenu_Event; 
	savesMenuInfo.cin_tier3.generic.callback	= UI_SavesMenu_Event;
	savesMenuInfo.cin_tier2.generic.callback	= UI_SavesMenu_Event; 
	savesMenuInfo.cin_tier1.generic.callback	= UI_SavesMenu_Event; 
	savesMenuInfo.cin_intro.generic.callback	= UI_SavesMenu_Event; 
	savesMenuInfo.cin_idlogo.generic.callback	= UI_SavesMenu_Event;	
	} else {
	savesMenuInfo.cin_tier6.generic.callback	= UI_SavesMenu_Event2;
	savesMenuInfo.cin_tier5.generic.callback	= UI_SavesMenu_Event2; 
	savesMenuInfo.cin_tier4.generic.callback	= UI_SavesMenu_Event2; 
	savesMenuInfo.cin_tier3.generic.callback	= UI_SavesMenu_Event2;
	savesMenuInfo.cin_tier2.generic.callback	= UI_SavesMenu_Event2; 
	savesMenuInfo.cin_tier1.generic.callback	= UI_SavesMenu_Event2; 
	savesMenuInfo.cin_intro.generic.callback	= UI_SavesMenu_Event2; 
	savesMenuInfo.cin_idlogo.generic.callback	= UI_SavesMenu_Event2;	
	}
	

	savesMenuInfo.back.generic.type		= MTYPE_BITMAP;
	savesMenuInfo.back.string		= ART_BACK0;
	savesMenuInfo.back.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	savesMenuInfo.back.generic.id			= ID_BACK;
	savesMenuInfo.back.generic.callback	= UI_SavesMenu_BackEvent;
	savesMenuInfo.back.generic.x			= 0 - uis.wideoffset;
	savesMenuInfo.back.generic.y			= 480-64;
	savesMenuInfo.back.width				= 128;
	savesMenuInfo.back.height				= 64;
	savesMenuInfo.back.focuspic			= ART_BACK1;

	Menu_AddItem( &savesMenuInfo.menu, &savesMenuInfo.banner );
	Menu_AddItem( &savesMenuInfo.menu, &savesMenuInfo.cin_idlogo );
	Menu_AddItem( &savesMenuInfo.menu, &savesMenuInfo.cin_intro );
	Menu_AddItem( &savesMenuInfo.menu, &savesMenuInfo.cin_tier1 );
	Menu_AddItem( &savesMenuInfo.menu, &savesMenuInfo.cin_tier2 );
	Menu_AddItem( &savesMenuInfo.menu, &savesMenuInfo.cin_tier3 );
	Menu_AddItem( &savesMenuInfo.menu, &savesMenuInfo.cin_tier4 );
	Menu_AddItem( &savesMenuInfo.menu, &savesMenuInfo.cin_tier5 );
	Menu_AddItem( &savesMenuInfo.menu, &savesMenuInfo.cin_tier6 );
	Menu_AddItem( &savesMenuInfo.menu, &savesMenuInfo.back );
}


/*
=================
UI_SavesMenu_Cache
=================
*/
void UI_SavesMenu_Cache( void ) {
	trap_R_RegisterShaderNoMip( ART_BACK0 );
	trap_R_RegisterShaderNoMip( ART_BACK1 );
}


/*
===============
UI_SavesMenu
===============
*/
void UI_SavesMenu_Load( void ) {
	UI_SavesMenu_Init(0);
	UI_PushMenu( &savesMenuInfo.menu );
}
void UI_SavesMenu_Save( void ) {
	UI_SavesMenu_Init(1);
	UI_PushMenu( &savesMenuInfo.menu );
}
