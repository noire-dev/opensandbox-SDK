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
/*
=======================================================================

SYSTEM CONFIGURATION MENU

=======================================================================
*/

#include "ui_local.h"

#define ART_BACK0			"menu/assets/back_0"
#define ART_BACK1			"menu/assets/back_1"

#define ID_GRAPHICS			10
#define ID_DISPLAY			11
#define ID_SOUND			12
#define ID_NETWORK			13
#define ID_BACK				14

#define VERTICAL_SPACING	34

typedef struct {
	menuframework_s	menu;

	menutext_s		banner;

	menutext_s		graphics;
	menutext_s		display;
	menutext_s		sound;
	menutext_s		network;
	menubitmap_s	back;
} optionsmenu_t;

static optionsmenu_t	s_options;


/*
=================
Options_Event
=================
*/
static void Options_Event( void* ptr, int event ) {
	if( event != QM_ACTIVATED ) {
		return;
	}

	switch( ((menucommon_s*)ptr)->id ) {
	case ID_GRAPHICS:
		UI_GraphicsOptionsMenu();
		break;

	case ID_DISPLAY:
		UI_DisplayOptionsMenu();
		break;

	case ID_SOUND:
		UI_SoundOptionsMenu();
		break;

	case ID_NETWORK:
		UI_NetworkOptionsMenu();
		break;

	case ID_BACK:
		UI_PopMenu();
		break;
	}
}


/*
===============
SystemConfig_Cache
===============
*/
void SystemConfig_Cache( void ) {
	trap_R_RegisterShaderNoMip( ART_BACK0 );
	trap_R_RegisterShaderNoMip( ART_BACK1 );
}

/*
===============
Options_MenuInit
===============
*/
void Options_MenuInit( void ) {
	int				y;
	uiClientState_t	cstate;

	memset( &s_options, 0, sizeof(optionsmenu_t) );

	SystemConfig_Cache();
	s_options.menu.wrapAround = qtrue;
	s_options.menu.native 	   = qfalse;

	trap_GetClientState( &cstate );
	if ( cstate.connState >= CA_CONNECTED ) {
		s_options.menu.fullscreen = qfalse;
	}
	else {
		s_options.menu.fullscreen = qtrue;
	}

	s_options.banner.generic.type	= MTYPE_BTEXT;
	s_options.banner.generic.flags	= QMF_CENTER_JUSTIFY;
	s_options.banner.generic.x		= 320;
	s_options.banner.generic.y		= 16;
	if(cl_language.integer == 0){
	s_options.banner.string		    = "SYSTEM SETUP";
	}
	if(cl_language.integer == 1){
	s_options.banner.string		    = "СИСТЕМНЫЕ НАСТРОЙКИ";
	}
	s_options.banner.color			= color_white;
	s_options.banner.style			= UI_CENTER;

	y = 168;
	s_options.graphics.generic.type		= MTYPE_PTEXT;
	s_options.graphics.generic.flags	= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_options.graphics.generic.callback	= Options_Event;
	s_options.graphics.generic.id		= ID_GRAPHICS;
	s_options.graphics.generic.x		= 320;
	s_options.graphics.generic.y		= y;
	if(cl_language.integer == 0){
	s_options.graphics.string			= "GRAPHICS";
	}
	if(cl_language.integer == 1){
	s_options.graphics.string			= "ГРАФИКА";
	}
	s_options.graphics.color			= color_white;
	s_options.graphics.style			= UI_CENTER;

	y += VERTICAL_SPACING;
	s_options.display.generic.type		= MTYPE_PTEXT;
	s_options.display.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_options.display.generic.callback	= Options_Event;
	s_options.display.generic.id		= ID_DISPLAY;
	s_options.display.generic.x			= 320;
	s_options.display.generic.y			= y;
	if(cl_language.integer == 0){
	s_options.display.string			= "DISPLAY";
	}
	if(cl_language.integer == 1){
	s_options.display.string			= "ЭКРАН";
	}
	s_options.display.color				= color_white;
	s_options.display.style				= UI_CENTER;

	y += VERTICAL_SPACING;
	s_options.sound.generic.type		= MTYPE_PTEXT;
	s_options.sound.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_options.sound.generic.callback	= Options_Event;
	s_options.sound.generic.id			= ID_SOUND;
	s_options.sound.generic.x			= 320;
	s_options.sound.generic.y			= y;
	if(cl_language.integer == 0){
	s_options.sound.string				= "SOUND";
	}
	if(cl_language.integer == 1){
	s_options.sound.string				= "ЗВУК";
	}
	s_options.sound.color				= color_white;
	s_options.sound.style				= UI_CENTER;

	y += VERTICAL_SPACING;
	s_options.network.generic.type		= MTYPE_PTEXT;
	s_options.network.generic.flags		= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_options.network.generic.callback	= Options_Event;
	s_options.network.generic.id		= ID_NETWORK;
	s_options.network.generic.x			= 320;
	s_options.network.generic.y			= y;
	if(cl_language.integer == 0){
	s_options.network.string			= "NETWORK";
	}
	if(cl_language.integer == 1){
	s_options.network.string			= "СЕТЬ";
	}
	s_options.network.color				= color_white;
	s_options.network.style				= UI_CENTER;

	s_options.back.generic.type	    = MTYPE_BITMAP;
	s_options.back.generic.name     = ART_BACK0;
	s_options.back.generic.flags    = QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_options.back.generic.callback = Options_Event;
	s_options.back.generic.id	    = ID_BACK;
	s_options.back.generic.x		= 0;
	s_options.back.generic.y		= 480-64;
	s_options.back.width  		    = 128;
	s_options.back.height  		    = 64;
	s_options.back.focuspic         = ART_BACK1;

	Menu_AddItem( &s_options.menu, ( void * ) &s_options.banner );
	Menu_AddItem( &s_options.menu, ( void * ) &s_options.graphics );
	Menu_AddItem( &s_options.menu, ( void * ) &s_options.display );
	Menu_AddItem( &s_options.menu, ( void * ) &s_options.sound );
	Menu_AddItem( &s_options.menu, ( void * ) &s_options.network );
	Menu_AddItem( &s_options.menu, ( void * ) &s_options.back );
}


/*
===============
UI_SystemConfigMenu
===============
*/
void UI_SystemConfigMenu( void ) {
	Options_MenuInit();
	UI_PushMenu ( &s_options.menu );
}
