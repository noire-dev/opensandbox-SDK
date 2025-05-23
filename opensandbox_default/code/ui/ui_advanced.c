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
//last update: 2023 Dec 11
#include "ui_local.h"

#define ART_BACK0			"menu/assets/back_0"
#define ART_BACK1			"menu/assets/back_1"	
#define ART_FIGHT0			"menu/assets/accept_0"
#define ART_FIGHT1			"menu/assets/accept_1"

#define MAX_WSITEMS			256
#define NAMEBUFSIZE			( MAX_WSITEMS * 48 )
#define GAMEBUFSIZE			( MAX_WSITEMS * 16 )

#define ID_BACK				10
#define ID_GO				11
#define ID_LIST				12
#define ID_SEARCH			13
#define ID_VALUE			14


typedef struct {
	menuframework_s	menu;

	menuelement_s		banner;

	menuelement_s		list;
	menuelement_s		filter;
	menuelement_s		value;

	menuelement_s	back;
	menuelement_s	go;

	char			description[NAMEBUFSIZE];
	char			fs_game[GAMEBUFSIZE];

	char			*descriptionPtr;
	char			*fs_gamePtr;

	char			*descriptionList[MAX_WSITEMS];
	char			*fs_gameList[MAX_WSITEMS];
	char*			advanced_itemslist[524288];
} advanced_t;

static advanced_t	s_advanced;


/*
===============
UI_Advanced_MenuEvent
===============
*/
static void UI_Advanced_MenuEvent( void *ptr, int event ) {

	if( event != QM_ACTIVATED ) {
		return;
	}

	switch ( ((menucommon_s*)ptr)->id ) {
	case ID_GO:
		trap_Cmd_ExecuteText( EXEC_APPEND, va("set %s \"%s\"\n", s_advanced.list.itemnames[s_advanced.list.curvalue], s_advanced.value.field.buffer) );
		break;
		
	case ID_LIST:
		//trap_Cmd_ExecuteText( EXEC_APPEND, va("unset %s\n", s_advanced.list.itemnames[s_advanced.list.curvalue]) );
		Q_strncpyz( s_advanced.value.field.buffer, UI_Cvar_VariableString(s_advanced.list.itemnames[s_advanced.list.curvalue]), sizeof(s_advanced.value.field.buffer) );
		break;

	case ID_BACK:
		UI_PopMenu();
		UI_LoadArenas();
		//trap_Cmd_ExecuteText( EXEC_APPEND, "game_restart");
		break;
	}
}
static void UI_Advanced_MenuEvent2( void *ptr, int event ) {

	if( event != QM_LOSTFOCUS ) {
		return;
	}

	switch ( ((menucommon_s*)ptr)->id ) {
	case ID_SEARCH:

		break;
	case ID_VALUE:

		break;
	}
}


char* 			advanced_items[] = {

0
};


/*
===============
UI_Advanced_ParseInfos
===============
*/
	int advanced_i = 0;
	int advanced_j = 0;
void UI_Advanced_ParseInfos( void ) {
	for (advanced_i = 0; advanced_items[advanced_i] != 0; advanced_i++) {
	if(Q_stricmp (s_advanced.filter.field.buffer, "")){
	if ( !Q_stristr( advanced_items[advanced_i], s_advanced.filter.field.buffer ) ) {
		continue;
	}
	}
	s_advanced.list.itemnames[advanced_j] = advanced_items[advanced_i];
	advanced_j += 1;
	}
    s_advanced.list.numitems = advanced_j;
}


/*
===============
UI_Advanced_LoadItemsFromFile
===============
*/
void UI_Advanced_LoadItemsFromFile() {
advanced_i = 0;
advanced_j = 0;
s_advanced.list.curvalue = 0;
s_advanced.list.top      = 0;
UI_Advanced_ParseInfos();
}


/*
=================
Advanced_MenuKey
=================
*/
static sfxHandle_t Advanced_MenuKey( int key ) {
	if( key == K_MOUSE2 || key == K_ESCAPE ) {

	}
	if( key == K_F5 ) {
	UI_Advanced_LoadItemsFromFile();	
	}
	return Menu_DefaultKey( &s_advanced.menu, key );
}


static void UI_Advanced_Draw( void ) {
	int i;
	float			x, y, w, h;
	vec4_t			color1 = {0.85, 0.9, 1.0, 1};

	Menu_Draw( &s_advanced.menu );
	
	
	if(cl_language.integer == 0){
	UI_DrawString( 320, 3, "Press F5 to search", UI_CENTER|UI_SMALLFONT, color1 );
	}
	if(cl_language.integer == 1){
	UI_DrawString( 320, 3, "Нажмите F5 для поиска", UI_CENTER|UI_SMALLFONT, color1 );
	}
}


/*
===============
UI_Advanced_MenuInit
===============
*/
static void UI_Advanced_MenuInit( void ) {
	UI_AdvancedMenu_Cache();

	memset( &s_advanced, 0 ,sizeof(advanced_t) );
	s_advanced.menu.native 	   = qfalse;
	s_advanced.menu.fullscreen = qtrue;
	s_advanced.menu.key        = Advanced_MenuKey;
	s_advanced.menu.draw 	   = UI_Advanced_Draw;

	s_advanced.banner.generic.type		= MTYPE_TEXT;
	s_advanced.banner.generic.x			= 320;
	s_advanced.banner.generic.y			= 16;
	if(cl_language.integer == 0){
	s_advanced.banner.string			= "ADVANCED SETTINGS";
	}
	if(cl_language.integer == 1){
	s_advanced.banner.string			= "РАСШИРЕННЫЕ НАСТРОЙКИ";
	}
	s_advanced.banner.color				= color_white;
	s_advanced.banner.style				= UI_CENTER;

	s_advanced.back.generic.type		= MTYPE_BITMAP;
	s_advanced.back.string		= ART_BACK0;
	s_advanced.back.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_advanced.back.generic.id			= ID_BACK;
	s_advanced.back.generic.callback	= UI_Advanced_MenuEvent;
	s_advanced.back.generic.x			= 0 - uis.wideoffset;
	s_advanced.back.generic.y			= 480-64;
	s_advanced.back.width				= 128;
	s_advanced.back.height				= 64;
	s_advanced.back.focuspic			= ART_BACK1;

	s_advanced.go.generic.type			= MTYPE_BITMAP;
	s_advanced.go.string			= ART_FIGHT0;
	s_advanced.go.generic.flags			= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_advanced.go.generic.id			= ID_GO;
	s_advanced.go.generic.callback		= UI_Advanced_MenuEvent;
	s_advanced.go.generic.x				= 640 + uis.wideoffset;
	s_advanced.go.generic.y				= 480-64;
	s_advanced.go.width					= 128;
	s_advanced.go.height				= 64;
	s_advanced.go.focuspic				= ART_FIGHT1;
	
	s_advanced.filter.generic.type			= MTYPE_FIELD;
	s_advanced.filter.generic.flags			= QMF_PULSEIFFOCUS|QMF_SMALLFONT||QMF_CENTER_JUSTIFY;
	s_advanced.filter.generic.callback		= UI_Advanced_MenuEvent2;
	s_advanced.filter.generic.id			= ID_SEARCH;
	s_advanced.filter.field.widthInChars	= 16;
	s_advanced.filter.field.maxchars		= 16;
	if(cl_language.integer == 0){
	s_advanced.filter.string			= "Search:";
	}
	if(cl_language.integer == 1){
	s_advanced.filter.string			= "Поиск:";
	}
	s_advanced.filter.generic.x				= 240;
	s_advanced.filter.generic.y				= 430;
	
	s_advanced.value.generic.type			= MTYPE_FIELD;
	s_advanced.value.generic.flags			= QMF_PULSEIFFOCUS|QMF_SMALLFONT||QMF_CENTER_JUSTIFY;
	s_advanced.value.generic.callback		= UI_Advanced_MenuEvent2;
	s_advanced.value.generic.id				= ID_VALUE;
	s_advanced.value.field.widthInChars		= 20;
	s_advanced.value.field.maxchars			= 128;
	if(cl_language.integer == 0){
	s_advanced.value.string			= "Value:";
	}
	if(cl_language.integer == 1){
	s_advanced.value.string			= "Значение:";
	}
	s_advanced.value.generic.x				= 240;
	s_advanced.value.generic.y				= 450;

	// scan for items
	s_advanced.list.generic.type		= MTYPE_SCROLLLIST;
	s_advanced.list.generic.flags		= QMF_PULSEIFFOCUS|QMF_CENTER_JUSTIFY;
	s_advanced.list.generic.callback	= UI_Advanced_MenuEvent;
	s_advanced.list.generic.id			= ID_LIST;
	s_advanced.list.generic.x			= 320;
	s_advanced.list.generic.y			= 40;
	s_advanced.list.width				= 48;
	s_advanced.list.height				= 32;
	s_advanced.list.itemnames			= (const char **)s_advanced.advanced_itemslist;

	UI_Advanced_LoadItemsFromFile();

	Menu_AddItem( &s_advanced.menu, &s_advanced.banner );
	Menu_AddItem( &s_advanced.menu, &s_advanced.list );
	Menu_AddItem( &s_advanced.menu, &s_advanced.back );
	Menu_AddItem( &s_advanced.menu, &s_advanced.go );
	Menu_AddItem( &s_advanced.menu, &s_advanced.filter );
	Menu_AddItem( &s_advanced.menu, &s_advanced.value );
}

/*
=================
UI_Advanced_Cache
=================
*/
void UI_AdvancedMenu_Cache( void ) {
	trap_R_RegisterShaderNoMip( ART_BACK0 );
	trap_R_RegisterShaderNoMip( ART_BACK1 );
	trap_R_RegisterShaderNoMip( ART_FIGHT0 );
	trap_R_RegisterShaderNoMip( ART_FIGHT1 );
}


/*
===============
UI_AdvancedMenu
===============
*/
void UI_AdvancedMenu( void ) {
	UI_Advanced_MenuInit();
	UI_PushMenu( &s_advanced.menu );
}
