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
=============================================================================

SAVE CONFIG MENU

=============================================================================
*/

#include "ui_local.h"


#define ART_BACK0			"menu/assets/back_0"
#define ART_BACK1			"menu/assets/back_1"	
#define ART_SAVE0			"menu/assets/save_0"
#define ART_SAVE1			"menu/assets/save_1"
#define ART_BACKGROUND		"menu/assets/cut_frame"

#define ID_NAME			10
#define ID_BACK			11
#define ID_SAVE			12


typedef struct {
	menuframework_s	menu;

	menutext_s		banner;
	menubitmap_s	background;
	menufield_s		savename;
	menubitmap_s	back;
	menubitmap_s	save;
} saveConfig_t;

static saveConfig_t		saveConfig;


/*
===============
UI_SaveConfigMenu_BackEvent
===============
*/
static void UI_SaveConfigMenu_BackEvent( void *ptr, int event ) {
	if( event != QM_ACTIVATED ) {
		return;
	}

	UI_PopMenu();
}


/*
===============
UI_SaveConfigMenu_SaveEvent
===============
*/
static void UI_SaveConfigMenu_SaveEvent( void *ptr, int event ) {
	char	configname[MAX_QPATH];

	if( event != QM_ACTIVATED ) {
		return;
	}

	if( !saveConfig.savename.field.buffer[0] ) {
		return;
	}

	COM_StripExtensionOld(saveConfig.savename.field.buffer, configname );
	trap_Cmd_ExecuteText( EXEC_APPEND, va( "writeconfig %s.cfg\n", configname ) );
	UI_PopMenu();
}


/*
===============
UI_SaveConfigMenu_SavenameDraw
===============
*/
static void UI_SaveConfigMenu_SavenameDraw( void *self ) {
	menufield_s		*f;
	int				style;
	float			*color;

	f = (menufield_s *)self;

	if( f == Menu_ItemAtCursor( &saveConfig.menu ) ) {
		style = UI_LEFT|UI_PULSE|UI_SMALLFONT;
		color = color_highlight;
	}
	else {
		style = UI_LEFT|UI_SMALLFONT;
		color = colorRed;
	}
	if(cl_language.integer == 0){
	UI_DrawString( 320, 192, "Enter filename:", UI_CENTER|UI_SMALLFONT, color_grey );
	}
	if(cl_language.integer == 1){
	UI_DrawString( 320, 192, "Введите имя файла:", UI_CENTER|UI_SMALLFONT, color_grey );
	}
	UI_FillRect( f->generic.x, f->generic.y, f->field.widthInChars*SMALLCHAR_WIDTH, SMALLCHAR_HEIGHT, colorBlack );
	MField_Draw( &f->field, f->generic.x, f->generic.y, style, color );
}


/*
=================
UI_SaveConfigMenu_Init
=================
*/
static void UI_SaveConfigMenu_Init( void ) {
	memset( &saveConfig, 0, sizeof(saveConfig) );

	UI_SaveConfigMenu_Cache();
	saveConfig.menu.wrapAround = qtrue;
	saveConfig.menu.native 	   = qfalse;
	saveConfig.menu.fullscreen = qtrue;

	saveConfig.banner.generic.type		= MTYPE_BTEXT;
	saveConfig.banner.generic.x			= 320;
	saveConfig.banner.generic.y			= 16;
	if(cl_language.integer == 0){
	saveConfig.banner.string			= "SAVE CONFIG";
	}
	if(cl_language.integer == 1){
	saveConfig.banner.string			= "СОХРАНИТЬ ФАЙЛ";
	}
	saveConfig.banner.color				= color_white;
	saveConfig.banner.style				= UI_CENTER;

	saveConfig.background.generic.type		= MTYPE_BITMAP;
	saveConfig.background.generic.name		= ART_BACKGROUND;
	saveConfig.background.generic.flags		= QMF_INACTIVE;
	saveConfig.background.generic.x			= 142;
	saveConfig.background.generic.y			= 118;
	saveConfig.background.width				= 359;
	saveConfig.background.height			= 256;

	saveConfig.savename.generic.type		= MTYPE_FIELD;
	saveConfig.savename.generic.flags		= QMF_NODEFAULTINIT|QMF_UPPERCASE;
	saveConfig.savename.generic.ownerdraw	= UI_SaveConfigMenu_SavenameDraw;
	saveConfig.savename.field.widthInChars	= 20;
	saveConfig.savename.field.maxchars		= 20;
	saveConfig.savename.generic.x			= 240;
	saveConfig.savename.generic.y			= 155+72;
	saveConfig.savename.generic.left		= 240;
	saveConfig.savename.generic.top			= 155+72;
	saveConfig.savename.generic.right		= 233 + 20*SMALLCHAR_WIDTH;
	saveConfig.savename.generic.bottom		= 155+72 + SMALLCHAR_HEIGHT+2;

	saveConfig.back.generic.type		= MTYPE_BITMAP;
	saveConfig.back.generic.name		= ART_BACK0;
	saveConfig.back.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	saveConfig.back.generic.id			= ID_BACK;
	saveConfig.back.generic.callback	= UI_SaveConfigMenu_BackEvent;
	saveConfig.back.generic.x			= 0 - uis.wideoffset;
	saveConfig.back.generic.y			= 480-64;
	saveConfig.back.width				= 128;
	saveConfig.back.height				= 64;
	saveConfig.back.focuspic			= ART_BACK1;

	saveConfig.save.generic.type		= MTYPE_BITMAP;
	saveConfig.save.generic.name		= ART_SAVE0;
	saveConfig.save.generic.flags		= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	saveConfig.save.generic.id			= ID_SAVE;
	saveConfig.save.generic.callback	= UI_SaveConfigMenu_SaveEvent;
	saveConfig.save.generic.x			= 640 + uis.wideoffset;
	saveConfig.save.generic.y			= 480-64;
	saveConfig.save.width  				= 128;
	saveConfig.save.height  		    = 64;
	saveConfig.save.focuspic			= ART_SAVE1;

	Menu_AddItem( &saveConfig.menu, &saveConfig.banner );
	Menu_AddItem( &saveConfig.menu, &saveConfig.background );
	Menu_AddItem( &saveConfig.menu, &saveConfig.savename );
	Menu_AddItem( &saveConfig.menu, &saveConfig.back );
	Menu_AddItem( &saveConfig.menu, &saveConfig.save );
}


/*
=================
UI_SaveConfigMenu_Cache
=================
*/
void UI_SaveConfigMenu_Cache( void ) {
	trap_R_RegisterShaderNoMip( ART_BACK0 );
	trap_R_RegisterShaderNoMip( ART_BACK1 );
	trap_R_RegisterShaderNoMip( ART_SAVE0 );
	trap_R_RegisterShaderNoMip( ART_SAVE1 );
	trap_R_RegisterShaderNoMip( ART_BACKGROUND );
}


/*
===============
UI_SaveConfigMenu
===============
*/
void UI_SaveConfigMenu( void ) {
	UI_SaveConfigMenu_Init();
	UI_PushMenu( &saveConfig.menu );
}
