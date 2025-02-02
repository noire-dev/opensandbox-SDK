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

LOAD CONFIG MENU

=============================================================================
*/

#include "ui_local.h"


#define ART_BACK0			"menu/assets/back_0"
#define ART_BACK1			"menu/assets/back_1"
#define ART_FIGHT0			"menu/assets/load_0"
#define ART_FIGHT1			"menu/assets/load_1"
#define ART_ARROWS			"menu/assets/arrows_horz_0"
#define ART_ARROWLEFT		"menu/assets/arrows_horz_left"
#define ART_ARROWRIGHT		"menu/assets/arrows_horz_right"
#define ART_BACKGROUND		"menu/assets/cut_frame"

#define MAX_MAPFILES		4096
#define MAPNAMEBUFSIZE			( MAX_MAPFILES * 16 )

#define ID_BACK				10
#define ID_GO				11
#define ID_LIST				12
#define ID_LEFT				13
#define ID_RIGHT			14

#define ARROWS_WIDTH		128
#define ARROWS_HEIGHT		48


typedef struct {
	menuframework_s	menu;

	menutext_s		banner;

	menuobject_s	list;

	menubitmap_s	arrows;
	menubitmap_s	left;
	menubitmap_s	right;
	menubitmap_s	back;
	menubitmap_s	go;
	menubitmap_s	background;

	char			names[524288];
	char*			configlist[65536];
} s_loadMapEd_t;

static s_loadMapEd_t	s_loadMapEd;


/*
===============
loadMapEd_MenuEvent
===============
*/
static void loadMapEd_MenuEvent( void *ptr, int event ) {

	if( event != QM_ACTIVATED ) {
		return;
	}

	switch ( ((menucommon_s*)ptr)->id ) {
	case ID_GO:
		trap_Cmd_ExecuteText( EXEC_APPEND, va( "loadmap maps/%s.add\n", s_loadMapEd.list.itemnames[s_loadMapEd.list.curvalue] ) );
		UI_PopMenu();
		break;

	case ID_BACK:
		UI_PopMenu();
		break;

	case ID_LEFT:
		UIObject_Key( &s_loadMapEd.list, K_LEFTARROW );
		break;

	case ID_RIGHT:
		UIObject_Key( &s_loadMapEd.list, K_RIGHTARROW );
		break;
	}
}


/*
===============
loadMapEd_MenuInit
===============
*/
static void loadMapEd_MenuInit( void ) {
	int		i;
	int		len;
	char	*configname;

	UI_loadMapEd_Cache();

	memset( &s_loadMapEd, 0 ,sizeof(s_loadMapEd_t) );
	s_loadMapEd.menu.wrapAround = qtrue;
	s_loadMapEd.menu.native 	= qfalse;
	s_loadMapEd.menu.fullscreen = qfalse;

	s_loadMapEd.banner.generic.type	= MTYPE_BTEXT;
	s_loadMapEd.banner.generic.x		= 320;
	s_loadMapEd.banner.generic.y		= 16;
	if(cl_language.integer == 0){
	s_loadMapEd.banner.string			= "Load Map";
	}
	if(cl_language.integer == 1){
	s_loadMapEd.banner.string			= "Загрузить Карту";
	}
	s_loadMapEd.banner.color			= color_white;
	s_loadMapEd.banner.style			= UI_CENTER;
	
	s_loadMapEd.background.generic.type		= MTYPE_BITMAP;
	s_loadMapEd.background.generic.name		= ART_BACKGROUND;
	s_loadMapEd.background.generic.flags		= QMF_INACTIVE;
	s_loadMapEd.background.generic.x			= -10000000;
	s_loadMapEd.background.generic.y			= -1000;
	s_loadMapEd.background.width				= 46600000;
	s_loadMapEd.background.height			= 33200000;

	s_loadMapEd.arrows.generic.type	= MTYPE_BITMAP;
	s_loadMapEd.arrows.generic.name	= ART_ARROWS;
	s_loadMapEd.arrows.generic.flags	= QMF_INACTIVE;
	s_loadMapEd.arrows.generic.x		= 320-ARROWS_WIDTH/2;
	s_loadMapEd.arrows.generic.y		= 400;
	s_loadMapEd.arrows.width			= ARROWS_WIDTH;
	s_loadMapEd.arrows.height			= ARROWS_HEIGHT;

	s_loadMapEd.left.generic.type		= MTYPE_BITMAP;
	s_loadMapEd.left.generic.flags	= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS|QMF_MOUSEONLY;
	s_loadMapEd.left.generic.x		= 320-ARROWS_WIDTH/2;
	s_loadMapEd.left.generic.y		= 400;
	s_loadMapEd.left.generic.id		= ID_LEFT;
	s_loadMapEd.left.generic.callback	= loadMapEd_MenuEvent;
	s_loadMapEd.left.width			= ARROWS_WIDTH/2;
	s_loadMapEd.left.height			= ARROWS_HEIGHT;
	s_loadMapEd.left.focuspic			= ART_ARROWLEFT;

	s_loadMapEd.right.generic.type	= MTYPE_BITMAP;
	s_loadMapEd.right.generic.flags	= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS|QMF_MOUSEONLY;
	s_loadMapEd.right.generic.x		= 320;
	s_loadMapEd.right.generic.y		= 400;
	s_loadMapEd.right.generic.id		= ID_RIGHT;
	s_loadMapEd.right.generic.callback = loadMapEd_MenuEvent;
	s_loadMapEd.right.width			= ARROWS_WIDTH/2;
	s_loadMapEd.right.height			= ARROWS_HEIGHT;
	s_loadMapEd.right.focuspic		= ART_ARROWRIGHT;

	s_loadMapEd.back.generic.type		= MTYPE_BITMAP;
	s_loadMapEd.back.generic.name		= ART_BACK0;
	s_loadMapEd.back.generic.flags	= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_loadMapEd.back.generic.id		= ID_BACK;
	s_loadMapEd.back.generic.callback	= loadMapEd_MenuEvent;
	s_loadMapEd.back.generic.x		= 0 - uis.wideoffset;
	s_loadMapEd.back.generic.y		= 480-64;
	s_loadMapEd.back.width			= 128;
	s_loadMapEd.back.height			= 64;
	s_loadMapEd.back.focuspic			= ART_BACK1;

	s_loadMapEd.go.generic.type		= MTYPE_BITMAP;
	s_loadMapEd.go.generic.name		= ART_FIGHT0;
	s_loadMapEd.go.generic.flags		= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_loadMapEd.go.generic.id			= ID_GO;
	s_loadMapEd.go.generic.callback	= loadMapEd_MenuEvent;
	s_loadMapEd.go.generic.x			= 640 + uis.wideoffset;
	s_loadMapEd.go.generic.y			= 480-64;
	s_loadMapEd.go.width				= 128;
	s_loadMapEd.go.height				= 64;
	s_loadMapEd.go.focuspic			= ART_FIGHT1;

	// scan for configs
	s_loadMapEd.list.generic.type			= MTYPE_UIOBJECT;
	s_loadMapEd.list.type					= 5;
	s_loadMapEd.list.styles					= 2;
	s_loadMapEd.list.fontsize				= 1;
	s_loadMapEd.list.string					= "screenshots/maps/";
	s_loadMapEd.list.generic.flags	= QMF_PULSEIFFOCUS;
	s_loadMapEd.list.generic.callback	= loadMapEd_MenuEvent;
	s_loadMapEd.list.generic.id		= ID_LIST;
	s_loadMapEd.list.generic.x		= 20;
	s_loadMapEd.list.generic.y		= 50;
	s_loadMapEd.list.width			= 15;
	s_loadMapEd.list.height			= 3;
	s_loadMapEd.list.numitems			= trap_FS_GetFileList( "maps", "add", s_loadMapEd.names, 524288 );
	s_loadMapEd.list.itemnames		= (const char **)s_loadMapEd.configlist;
	s_loadMapEd.list.columns			= 5;

	if (!s_loadMapEd.list.numitems) {
		if(cl_language.integer == 0){
		strcpy(s_loadMapEd.names,"No MapFiles Found.");
		}
		if(cl_language.integer == 1){
		strcpy(s_loadMapEd.names,"Файлы карт не найдены.");
		}
		s_loadMapEd.list.numitems = 1;

		//degenerate case, not selectable
		s_loadMapEd.go.generic.flags |= (QMF_INACTIVE|QMF_HIDDEN);
	}
	else if (s_loadMapEd.list.numitems > MAX_MAPFILES)
		s_loadMapEd.list.numitems = MAX_MAPFILES;

	configname = s_loadMapEd.names;
	for ( i = 0; i < s_loadMapEd.list.numitems; i++ ) {
		s_loadMapEd.list.itemnames[i] = configname;

		// strip extension
		len = strlen( configname );
		if (!Q_stricmp(configname +  len - 4,".add"))
			configname[len-4] = '\0';

		configname += len + 1;
	}

	Menu_AddItem( &s_loadMapEd.menu, &s_loadMapEd.background );
	Menu_AddItem( &s_loadMapEd.menu, &s_loadMapEd.banner );
	Menu_AddItem( &s_loadMapEd.menu, &s_loadMapEd.list );
	//Menu_AddItem( &s_loadMapEd.menu, &s_loadMapEd.arrows );
	//Menu_AddItem( &s_loadMapEd.menu, &s_loadMapEd.left );
	//Menu_AddItem( &s_loadMapEd.menu, &s_loadMapEd.right );
	Menu_AddItem( &s_loadMapEd.menu, &s_loadMapEd.back );
	Menu_AddItem( &s_loadMapEd.menu, &s_loadMapEd.go );
}

/*
=================
UI_loadMapEd_Cache
=================
*/
void UI_loadMapEd_Cache( void ) {
	trap_R_RegisterShaderNoMip( ART_BACK0 );
	trap_R_RegisterShaderNoMip( ART_BACK1 );
	trap_R_RegisterShaderNoMip( ART_FIGHT0 );
	trap_R_RegisterShaderNoMip( ART_ARROWS );
	trap_R_RegisterShaderNoMip( ART_ARROWLEFT );
	trap_R_RegisterShaderNoMip( ART_ARROWRIGHT );
}


/*
===============
UI_loadMapEdMenu
===============
*/
void UI_loadMapEdMenu( void ) {
	loadMapEd_MenuInit();
	UI_PushMenu( &s_loadMapEd.menu );
}
