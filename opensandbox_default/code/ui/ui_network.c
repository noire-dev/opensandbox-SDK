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
/*
=======================================================================

NETWORK OPTIONS MENU

=======================================================================
*/

#include "ui_local.h"

#define ART_BACK0			"menu/assets/back_0"
#define ART_BACK1			"menu/assets/back_1"

#define ID_GRAPHICS			10
#define ID_DISPLAY			11
#define ID_SOUND			12
#define ID_NETWORK			13
#define ID_RATE				14
#define ID_BACK				15


static const char *rate_items[] = {
	"<= 28.8K",
	"33.6K",
	"56K",
	"ISDN",
	"LAN/Cable/xDSL",
	0
};

typedef struct {
	menuframework_s	menu;

	menutext_s		banner;

	menutext_s		graphics;
	menutext_s		display;
	menutext_s		sound;
	menutext_s		network;

	menulist_s		rate;

	menubitmap_s	back;
} networkOptionsInfo_t;

static networkOptionsInfo_t	networkOptionsInfo;


/*
=================
UI_NetworkOptionsMenu_Event
=================
*/
static void UI_NetworkOptionsMenu_Event( void* ptr, int event ) {
	if( event != QM_ACTIVATED ) {
		return;
	}

	switch( ((menucommon_s*)ptr)->id ) {
	case ID_GRAPHICS:
		UI_PopMenu();
		UI_GraphicsOptionsMenu();
		break;

	case ID_DISPLAY:
		UI_PopMenu();
		UI_DisplayOptionsMenu();
		break;

	case ID_SOUND:
		UI_PopMenu();
		UI_SoundOptionsMenu();
		break;

	case ID_NETWORK:
		break;

	case ID_RATE:
		if( networkOptionsInfo.rate.curvalue == 0 ) {
			trap_Cvar_SetValue( "rate", 2500 );
		}
		else if( networkOptionsInfo.rate.curvalue == 1 ) {
			trap_Cvar_SetValue( "rate", 3000 );
		}
		else if( networkOptionsInfo.rate.curvalue == 2 ) {
			trap_Cvar_SetValue( "rate", 4000 );
		}
		else if( networkOptionsInfo.rate.curvalue == 3 ) {
			trap_Cvar_SetValue( "rate", 5000 );
		}
		else if( networkOptionsInfo.rate.curvalue == 4 ) {
			trap_Cvar_SetValue( "rate", 25000 );
		}
		break;

	case ID_BACK:
		UI_PopMenu();
		break;
	}
}


/*
===============
UI_NetworkOptionsMenu_Init
===============
*/
static void UI_NetworkOptionsMenu_Init( void ) {
	int		y;
	int		rate;

	memset( &networkOptionsInfo, 0, sizeof(networkOptionsInfo) );

	UI_NetworkOptionsMenu_Cache();
	networkOptionsInfo.menu.wrapAround = qtrue;
	networkOptionsInfo.menu.native 	   = qfalse;
	networkOptionsInfo.menu.fullscreen = qtrue;

	networkOptionsInfo.banner.generic.type		= MTYPE_BTEXT;
	networkOptionsInfo.banner.generic.flags		= QMF_CENTER_JUSTIFY;
	networkOptionsInfo.banner.generic.x			= 320;
	networkOptionsInfo.banner.generic.y			= 16;
	if(cl_language.integer == 0){
	networkOptionsInfo.banner.string			= "SYSTEM SETUP";
	}
	if(cl_language.integer == 1){
	networkOptionsInfo.banner.string			= "СИСТЕМНЫЕ НАСТРОЙКИ";
	}
	networkOptionsInfo.banner.color				= color_white;
	networkOptionsInfo.banner.style				= UI_CENTER;

	networkOptionsInfo.graphics.generic.type		= MTYPE_PTEXT;
	networkOptionsInfo.graphics.generic.flags		= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	networkOptionsInfo.graphics.generic.id			= ID_GRAPHICS;
	networkOptionsInfo.graphics.generic.callback	= UI_NetworkOptionsMenu_Event;
	networkOptionsInfo.graphics.generic.x			= 140 - uis.wideoffset;
	networkOptionsInfo.graphics.generic.y			= 240 - 2 * PROP_HEIGHT;
	if(cl_language.integer == 0){
	networkOptionsInfo.graphics.string				= "GRAPHICS";
	}
	if(cl_language.integer == 1){
	networkOptionsInfo.graphics.string				= "ГРАФИКА";
	}
	networkOptionsInfo.graphics.style				= UI_RIGHT;
	networkOptionsInfo.graphics.color				= color_white;

	networkOptionsInfo.display.generic.type			= MTYPE_PTEXT;
	networkOptionsInfo.display.generic.flags		= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	networkOptionsInfo.display.generic.id			= ID_DISPLAY;
	networkOptionsInfo.display.generic.callback		= UI_NetworkOptionsMenu_Event;
	networkOptionsInfo.display.generic.x			= 140 - uis.wideoffset;
	networkOptionsInfo.display.generic.y			= 240 - PROP_HEIGHT;
	if(cl_language.integer == 0){
	networkOptionsInfo.display.string				= "DISPLAY";
	}
	if(cl_language.integer == 1){
	networkOptionsInfo.display.string				= "ЭКРАН";
	}
	networkOptionsInfo.display.style				= UI_RIGHT;
	networkOptionsInfo.display.color				= color_white;

	networkOptionsInfo.sound.generic.type			= MTYPE_PTEXT;
	networkOptionsInfo.sound.generic.flags			= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	networkOptionsInfo.sound.generic.id				= ID_SOUND;
	networkOptionsInfo.sound.generic.callback		= UI_NetworkOptionsMenu_Event;
	networkOptionsInfo.sound.generic.x				= 140 - uis.wideoffset;
	networkOptionsInfo.sound.generic.y				= 240;
	if(cl_language.integer == 0){
	networkOptionsInfo.sound.string					= "SOUND";
	}
	if(cl_language.integer == 1){
	networkOptionsInfo.sound.string					= "ЗВУК";
	}
	networkOptionsInfo.sound.style					= UI_RIGHT;
	networkOptionsInfo.sound.color					= color_white;

	networkOptionsInfo.network.generic.type			= MTYPE_PTEXT;
	networkOptionsInfo.network.generic.flags		= QMF_RIGHT_JUSTIFY;
	networkOptionsInfo.network.generic.id			= ID_NETWORK;
	networkOptionsInfo.network.generic.callback		= UI_NetworkOptionsMenu_Event;
	networkOptionsInfo.network.generic.x			= 140 - uis.wideoffset;
	networkOptionsInfo.network.generic.y			= 240 + PROP_HEIGHT;
	if(cl_language.integer == 0){
	networkOptionsInfo.network.string				= "NETWORK";
	}
	if(cl_language.integer == 1){
	networkOptionsInfo.network.string				= "СЕТЬ";
	}
	networkOptionsInfo.network.style				= UI_RIGHT;
	networkOptionsInfo.network.color				= color_grey;

	y = 240 - 1 * (BIGCHAR_HEIGHT+2);
	networkOptionsInfo.rate.generic.type		= MTYPE_SPINCONTROL;
	if(cl_language.integer == 0){
	networkOptionsInfo.rate.generic.name		= "Data Rate:";
	}
	if(cl_language.integer == 1){
	networkOptionsInfo.rate.generic.name		= "Скорость Сети:";
	}
	networkOptionsInfo.rate.generic.flags		= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	networkOptionsInfo.rate.generic.callback	= UI_NetworkOptionsMenu_Event;
	networkOptionsInfo.rate.generic.id			= ID_RATE;
	networkOptionsInfo.rate.generic.x			= 400;
	networkOptionsInfo.rate.generic.y			= y;
	networkOptionsInfo.rate.itemnames			= rate_items;

	networkOptionsInfo.back.generic.type		= MTYPE_BITMAP;
	networkOptionsInfo.back.generic.name		= ART_BACK0;
	networkOptionsInfo.back.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	networkOptionsInfo.back.generic.callback	= UI_NetworkOptionsMenu_Event;
	networkOptionsInfo.back.generic.id			= ID_BACK;
	networkOptionsInfo.back.generic.x			= 0 - uis.wideoffset;
	networkOptionsInfo.back.generic.y			= 480-64;
	networkOptionsInfo.back.width				= 128;
	networkOptionsInfo.back.height				= 64;
	networkOptionsInfo.back.focuspic			= ART_BACK1;

	Menu_AddItem( &networkOptionsInfo.menu, ( void * ) &networkOptionsInfo.banner );
	Menu_AddItem( &networkOptionsInfo.menu, ( void * ) &networkOptionsInfo.graphics );
	Menu_AddItem( &networkOptionsInfo.menu, ( void * ) &networkOptionsInfo.display );
	Menu_AddItem( &networkOptionsInfo.menu, ( void * ) &networkOptionsInfo.sound );
	Menu_AddItem( &networkOptionsInfo.menu, ( void * ) &networkOptionsInfo.network );
	Menu_AddItem( &networkOptionsInfo.menu, ( void * ) &networkOptionsInfo.rate );
	Menu_AddItem( &networkOptionsInfo.menu, ( void * ) &networkOptionsInfo.back );

	rate = trap_Cvar_VariableValue( "rate" );
	if( rate <= 2500 ) {
		networkOptionsInfo.rate.curvalue = 0;
	}
	else if( rate <= 3000 ) {
		networkOptionsInfo.rate.curvalue = 1;
	}
	else if( rate <= 4000 ) {
		networkOptionsInfo.rate.curvalue = 2;
	}
	else if( rate <= 5000 ) {
		networkOptionsInfo.rate.curvalue = 3;
	}
	else {
		networkOptionsInfo.rate.curvalue = 4;
	}
}


/*
===============
UI_NetworkOptionsMenu_Cache
===============
*/
void UI_NetworkOptionsMenu_Cache( void ) {
	trap_R_RegisterShaderNoMip( ART_BACK0 );
	trap_R_RegisterShaderNoMip( ART_BACK1 );
}


/*
===============
UI_NetworkOptionsMenu
===============
*/
void UI_NetworkOptionsMenu( void ) {
	UI_NetworkOptionsMenu_Init();
	UI_PushMenu( &networkOptionsInfo.menu );
	Menu_SetCursorToItem( &networkOptionsInfo.menu, &networkOptionsInfo.network );
}
