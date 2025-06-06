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

	menuelement_s		banner;
	menuelement_s	background;
	menuelement_s		savename;
	menuelement_s	back;
	menuelement_s	save;
} saveMapEd_t;

static saveMapEd_t		saveMapEd;


/*
===============
UI_saveMapEdMenu_BackEvent
===============
*/
static void UI_saveMapEdMenu_BackEvent( void *ptr, int event ) {
	if( event != QM_ACTIVATED ) {
		return;
	}

	UI_PopMenu();
}


/*
===============
UI_saveMapEdMenu_SaveEvent
===============
*/
static void UI_saveMapEdMenu_SaveEvent( void *ptr, int event ) {
	char	configname[MAX_QPATH];

	if( event != QM_ACTIVATED ) {
		return;
	}

	if( !saveMapEd.savename.field.buffer[0] ) {
		return;
	}
	COM_StripExtension(saveMapEd.savename.field.buffer, configname, sizeof(configname));
	trap_Cmd_ExecuteText( EXEC_APPEND, va( "savemap maps/%s.add \n", configname ) );
	UI_ForceMenuOff();
	trap_Cmd_ExecuteText( EXEC_APPEND, "set cg_draw2D 0 \n" );
	trap_Cmd_ExecuteText( EXEC_APPEND, "wait 5 \n" );
	trap_Cmd_ExecuteText( EXEC_APPEND, va( "screenshotJPEG maps/%s \n", configname ) );
	trap_Cmd_ExecuteText( EXEC_APPEND, "wait 5 \n" );
	trap_Cmd_ExecuteText( EXEC_APPEND, "set cg_draw2D 1 \n" );
	UI_PopMenu();
}


/*
===============
UI_saveMapEdMenu_SavenameDraw
===============
*/
static void UI_saveMapEdMenu_SavenameDraw( void *self ) {
	menuelement_s		*f;
	int				style;
	float			*color;

	f = (menuelement_s *)self;

	if( f == Menu_ItemAtCursor( &saveMapEd.menu ) ) {
		style = UI_LEFT|UI_PULSE|UI_SMALLFONT;
		color = color_highlight;
	}
	else {
		style = UI_LEFT|UI_SMALLFONT;
		color = colorRed;
	}
	if(cl_language.integer == 0){
	ST_DrawString( 320, 192, "Enter filename:", UI_CENTER|UI_SMALLFONT, color_grey, 1.00 );
	}
	if(cl_language.integer == 1){
	ST_DrawString( 320, 192, "Введите имя файла:", UI_CENTER|UI_SMALLFONT, color_grey, 1.00 );
	}
	UI_FillRect( f->generic.x, f->generic.y, f->field.widthInChars*BASEFONT_INDENT, BASEFONT_HEIGHT, colorBlack );
	MField_Draw( &f->field, f->generic.x, f->generic.y, style, color, 1.00 );
}


/*
=================
UI_saveMapEdMenu_Init
=================
*/
static void UI_saveMapEdMenu_Init( void ) {
	memset( &saveMapEd, 0, sizeof(saveMapEd) );

	UI_saveMapEdMenu_Cache();
	saveMapEd.menu.fullscreen = qfalse;

	saveMapEd.banner.generic.type		= MTYPE_TEXT;
	saveMapEd.banner.generic.x			= 320;
	saveMapEd.banner.generic.y			= 16;
	if(cl_language.integer == 0){
	saveMapEd.banner.string			= "Save Map";
	}
	if(cl_language.integer == 1){
	saveMapEd.banner.string			= "Сохранение Карты";
	}
	saveMapEd.banner.color				= color_white;
	saveMapEd.banner.style				= UI_CENTER;

	saveMapEd.background.generic.type		= MTYPE_BITMAP;
	saveMapEd.background.string		= ART_BACKGROUND;
	saveMapEd.background.generic.flags		= QMF_INACTIVE;
	saveMapEd.background.generic.x			= -10000000;
	saveMapEd.background.generic.y			= -1000;
	saveMapEd.background.width				= 46600000;
	saveMapEd.background.height			= 33200000;

	saveMapEd.savename.generic.type		= MTYPE_FIELD;
	saveMapEd.savename.generic.flags		= QMF_NODEFAULTINIT|QMF_UPPERCASE;
	saveMapEd.savename.generic.ownerdraw	= UI_saveMapEdMenu_SavenameDraw;
	saveMapEd.savename.field.widthInChars	= 20;
	saveMapEd.savename.field.maxchars		= 20;
	saveMapEd.savename.generic.x			= 240;
	saveMapEd.savename.generic.y			= 155+72;
	saveMapEd.savename.generic.left		= 240;
	saveMapEd.savename.generic.top			= 155+72;
	saveMapEd.savename.generic.right		= 233 + 20*BASEFONT_INDENT;
	saveMapEd.savename.generic.bottom		= 155+72 + BASEFONT_HEIGHT+2;

	saveMapEd.back.generic.type		= MTYPE_BITMAP;
	saveMapEd.back.string		= ART_BACK0;
	saveMapEd.back.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	saveMapEd.back.generic.id			= ID_BACK;
	saveMapEd.back.generic.callback	= UI_saveMapEdMenu_BackEvent;
	saveMapEd.back.generic.x			= 0 - uis.wideoffset;
	saveMapEd.back.generic.y			= 480-64;
	saveMapEd.back.width				= 128;
	saveMapEd.back.height				= 64;
	saveMapEd.back.focuspic			= ART_BACK1;

	saveMapEd.save.generic.type		= MTYPE_BITMAP;
	saveMapEd.save.string		= ART_SAVE0;
	saveMapEd.save.generic.flags		= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	saveMapEd.save.generic.id			= ID_SAVE;
	saveMapEd.save.generic.callback	= UI_saveMapEdMenu_SaveEvent;
	saveMapEd.save.generic.x			= 640 + uis.wideoffset;
	saveMapEd.save.generic.y			= 480-64;
	saveMapEd.save.width  				= 128;
	saveMapEd.save.height  		    = 64;
	saveMapEd.save.focuspic			= ART_SAVE1;

	Menu_AddItem( &saveMapEd.menu, &saveMapEd.background );
	Menu_AddItem( &saveMapEd.menu, &saveMapEd.banner );
	Menu_AddItem( &saveMapEd.menu, &saveMapEd.savename );
	Menu_AddItem( &saveMapEd.menu, &saveMapEd.back );
	Menu_AddItem( &saveMapEd.menu, &saveMapEd.save );
}


/*
=================
UI_saveMapEdMenu_Cache
=================
*/
void UI_saveMapEdMenu_Cache( void ) {
	trap_R_RegisterShaderNoMip( ART_BACK0 );
	trap_R_RegisterShaderNoMip( ART_BACK1 );
	trap_R_RegisterShaderNoMip( ART_SAVE0 );
	trap_R_RegisterShaderNoMip( ART_SAVE1 );
	trap_R_RegisterShaderNoMip( ART_BACKGROUND );
}


/*
===============
UI_saveMapEdMenu
===============
*/
void UI_saveMapEdMenu( void ) {
	UI_saveMapEdMenu_Init();
	UI_PushMenu( &saveMapEd.menu );
}
