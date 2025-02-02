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

DEMOS MENU

=======================================================================
*/





#include "ui_local.h"


#define ART_BACK0			"menu/assets/back_0"
#define ART_BACK1			"menu/assets/back_1"	
#define ART_GO0				"menu/assets/fight_0"
#define ART_GO1				"menu/assets/fight_1"
#define ART_ARROWS			"menu/assets/arrows_horz_0"
#define ART_ARROWLEFT		"menu/assets/arrows_horz_left"
#define ART_ARROWRIGHT		"menu/assets/arrows_horz_right"

#define MAX_DEMOS			256
#define NAMEBUFSIZE			( MAX_DEMOS * 16 )
#define STATUSBAR_SIZE 32

#define ID_BACK				10
#define ID_GO				11
#define ID_LIST				12
#define ID_RIGHT			13
#define ID_LEFT				14
#define ID_TIMEDEMO			15

#define ARROWS_WIDTH		128
#define ARROWS_HEIGHT		48


typedef struct {
	menuframework_s	menu;

	menutext_s		banner;

	menulist_s		list;

	menuradiobutton_s	timedemo;

	menubitmap_s	arrows;
	menubitmap_s	left;
	menubitmap_s	right;
	menubitmap_s	back;
	menubitmap_s	go;

	int				numDemos;
	char			names[NAMEBUFSIZE];
	char			*demolist[MAX_DEMOS];
   int         protocol[MAX_DEMOS]; // stores the protocol of the detected map
   qboolean ranged_protocol;
   qboolean demos_found;
} demos_t;

static demos_t	s_demos;


/*
===============
Demos_MenuEvent
===============
*/
static void Demos_MenuEvent( void *ptr, int event ) {
	if( event != QM_ACTIVATED ) {
		return;
	}

	switch( ((menucommon_s*)ptr)->id ) {
	case ID_GO:
		UI_ForceMenuOff ();
		if (s_demos.timedemo.curvalue)
			trap_Cmd_ExecuteText( EXEC_APPEND, "timedemo 1;" );
		else
			trap_Cmd_ExecuteText( EXEC_APPEND, "timedemo 0;" );

		trap_Cmd_ExecuteText( EXEC_APPEND, va( "demo %s.dm_%i\n", s_demos.list.itemnames[s_demos.list.curvalue], s_demos.protocol[s_demos.list.curvalue] ) );
		break;

	case ID_BACK:
		UI_PopMenu();
		break;

	case ID_LEFT:
		ScrollList_Key( &s_demos.list, K_LEFTARROW );
		break;

	case ID_RIGHT:
		ScrollList_Key( &s_demos.list, K_RIGHTARROW );
		break;
	}
}



/*
=================
UI_DemosMenu_Key
=================
*/
static sfxHandle_t UI_DemosMenu_Key( int key ) {
	menucommon_s	*item;

	item = Menu_ItemAtCursor( &s_demos.menu );

	return Menu_DefaultKey( &s_demos.menu, key );
}



/*
=================
Demos_FillDemoList
=================
*/
static int Demos_FillDemoList(int protocol) {
   int count, oldcount;
   int length, offset;

   // PR1.32 allows for playing of earlier compatible demos (protocols 66,67)
   s_demos.ranged_protocol = qfalse;
   if (protocol == 68) {
      protocol = 66;
      s_demos.ranged_protocol = qtrue;
   }

   oldcount = 0;
   count = 0;
   offset = 0;
   do {
      oldcount = count;
      length = NAMEBUFSIZE - offset;
      count += trap_FS_GetFileList( "demos", va("dm_%i", protocol), &s_demos.names[offset], length );
      protocol++;

      // move the offset to the last demo
      while (oldcount < count) {
         oldcount++;
         offset += strlen(&s_demos.names[offset]) + 1;
      }

   } while (s_demos.ranged_protocol && protocol <= 68);

   return count;
}



/*
=================
Demos_ClipDemoExt
=================
*/
static int Demos_ClipDemoExt(char* demoname, int len, int protocol) {
   char* demoext;
   int demoext_len;

   if (s_demos.ranged_protocol) {
      protocol = 66;
   }

   do {
   	demoext = va(".dm_%i", protocol);
	   demoext_len = strlen(demoext);

      // strip extension
      if (!Q_stricmp(demoname +  len - demoext_len, demoext)) {
         demoname[len-demoext_len] = '\0';
         break;
      }
      protocol++;
   } while (s_demos.ranged_protocol && protocol <= 68);

   return protocol;
}


/*
=================
Demos_MenuDraw
=================
*/
static void Demos_MenuDraw(void)
{
   char statusbar[STATUSBAR_SIZE];

   if (s_demos.demos_found && s_demos.ranged_protocol &&
         s_demos.list.numitems > 0 && s_demos.list.curvalue >= 0) {
      Q_strncpyz(statusbar, va("Protocol %i", s_demos.protocol[s_demos.list.curvalue]), STATUSBAR_SIZE);
	   UI_DrawString(632 - strlen(statusbar) * SMALLCHAR_WIDTH, 480 - 64 - SMALLCHAR_HEIGHT,
         statusbar, UI_SMALLFONT, color_grey);
   }

   Menu_Draw(&s_demos.menu);
}


/*
===============
Demos_MenuInit
===============
*/
static void Demos_MenuInit( void ) {
	int		i;
	int		len;
	char	*demoname;
	int 	protocol;

	memset( &s_demos, 0 ,sizeof(demos_t) );
	s_demos.menu.key = UI_DemosMenu_Key;
	s_demos.menu.draw = Demos_MenuDraw;

   protocol = (int)trap_Cvar_VariableValue("protocol");

	Demos_Cache();

	s_demos.menu.fullscreen = qtrue;
	s_demos.menu.wrapAround = qtrue;
	s_demos.menu.native 	= qfalse;

	s_demos.banner.generic.type		= MTYPE_BTEXT;
	s_demos.banner.generic.x		= 320;
	s_demos.banner.generic.y		= 16;
	if(cl_language.integer == 0){
	s_demos.banner.string			= "DEMOS";
	}
	if(cl_language.integer == 1){
	s_demos.banner.string			= "ЗАПИСИ";
	}
	s_demos.banner.color			= color_white;
	s_demos.banner.style			= UI_CENTER;

	s_demos.timedemo.generic.type	= MTYPE_RADIOBUTTON;
	s_demos.timedemo.generic.x		= 480 - SMALLCHAR_WIDTH;
	s_demos.timedemo.generic.y		= 480 - 32 - SMALLCHAR_HEIGHT / 2;
	if(cl_language.integer == 0){
	s_demos.timedemo.generic.name 	= "Time demo:";
	}
	if(cl_language.integer == 1){
	s_demos.timedemo.generic.name 	= "Время записей:";
	}
	s_demos.timedemo.generic.flags	= QMF_SMALLFONT|QMF_PULSEIFFOCUS;
	s_demos.timedemo.generic.id		= ID_TIMEDEMO;

	s_demos.arrows.generic.type		= MTYPE_BITMAP;
	s_demos.arrows.generic.name		= ART_ARROWS;
	s_demos.arrows.generic.flags	= QMF_INACTIVE;
	s_demos.arrows.generic.x		= 320-ARROWS_WIDTH/2;
	s_demos.arrows.generic.y		= 400;
	s_demos.arrows.width			= ARROWS_WIDTH;
	s_demos.arrows.height			= ARROWS_HEIGHT;

	s_demos.left.generic.type		= MTYPE_BITMAP;
	s_demos.left.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS|QMF_MOUSEONLY;
	s_demos.left.generic.x			= 320-ARROWS_WIDTH/2;
	s_demos.left.generic.y			= 400;
	s_demos.left.generic.id			= ID_LEFT;
	s_demos.left.generic.callback	= Demos_MenuEvent;
	s_demos.left.width				= ARROWS_WIDTH/2;
	s_demos.left.height				= ARROWS_HEIGHT;
	s_demos.left.focuspic			= ART_ARROWLEFT;

	s_demos.right.generic.type		= MTYPE_BITMAP;
	s_demos.right.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS|QMF_MOUSEONLY;
	s_demos.right.generic.x			= 320;
	s_demos.right.generic.y			= 400;
	s_demos.right.generic.id		= ID_RIGHT;
	s_demos.right.generic.callback	= Demos_MenuEvent;
	s_demos.right.width				= ARROWS_WIDTH/2;
	s_demos.right.height			= ARROWS_HEIGHT;
	s_demos.right.focuspic			= ART_ARROWRIGHT;

	s_demos.back.generic.type		= MTYPE_BITMAP;
	s_demos.back.generic.name		= ART_BACK0;
	s_demos.back.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_demos.back.generic.id			= ID_BACK;
	s_demos.back.generic.callback	= Demos_MenuEvent;
	s_demos.back.generic.x			= 0 - uis.wideoffset;
	s_demos.back.generic.y			= 480-64;
	s_demos.back.width				= 128;
	s_demos.back.height				= 64;
	s_demos.back.focuspic			= ART_BACK1;

	s_demos.go.generic.type			= MTYPE_BITMAP;
	s_demos.go.generic.name			= ART_GO0;
	s_demos.go.generic.flags		= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_demos.go.generic.id			= ID_GO;
	s_demos.go.generic.callback		= Demos_MenuEvent;
	s_demos.go.generic.x			= 640 + uis.wideoffset;
	s_demos.go.generic.y			= 480-64;
	s_demos.go.width				= 128;
	s_demos.go.height				= 64;
	s_demos.go.focuspic				= ART_GO1;

	s_demos.list.generic.type		= MTYPE_SCROLLLIST;
	s_demos.list.generic.flags		= QMF_PULSEIFFOCUS;
	s_demos.list.generic.callback	= Demos_MenuEvent;
	s_demos.list.generic.id			= ID_LIST;
	s_demos.list.generic.x			= 118;
	s_demos.list.generic.y			= 130;
	s_demos.list.width				= 16;
	s_demos.list.height				= 14;
	s_demos.list.numitems			= Demos_FillDemoList(protocol);
	s_demos.list.itemnames			= (const char **)s_demos.demolist;
	s_demos.list.columns			= 3;

   s_demos.demos_found = qtrue;
	if (!s_demos.list.numitems) {
		if(cl_language.integer == 0){
		strcpy( s_demos.names, "No Demos Found." );
		}
		if(cl_language.integer == 1){
		strcpy( s_demos.names, "Записи не найдены." );
		}
		s_demos.list.numitems = 1;
      s_demos.demos_found = qfalse;

		//degenerate case, not selectable
		s_demos.go.generic.flags |= (QMF_INACTIVE|QMF_HIDDEN);
	}
	else if (s_demos.list.numitems > MAX_DEMOS) {
		s_demos.list.numitems = MAX_DEMOS;
   }

	demoname = s_demos.names;
	for ( i = 0; i < s_demos.list.numitems; i++ ) {
		// strip extension
		len = strlen( demoname );
      s_demos.protocol[i] = Demos_ClipDemoExt(demoname, len, protocol);

		s_demos.list.itemnames[i] = demoname;

		demoname += len + 1;
	}

	Menu_AddItem( &s_demos.menu, &s_demos.banner );
	Menu_AddItem( &s_demos.menu, &s_demos.list );
	Menu_AddItem( &s_demos.menu, &s_demos.arrows );
	Menu_AddItem( &s_demos.menu, &s_demos.left );
	Menu_AddItem( &s_demos.menu, &s_demos.right );
	Menu_AddItem( &s_demos.menu, &s_demos.back );
	Menu_AddItem( &s_demos.menu, &s_demos.timedemo );
	Menu_AddItem( &s_demos.menu, &s_demos.go );
}

/*
=================
Demos_Cache
=================
*/
void Demos_Cache( void ) {
	trap_R_RegisterShaderNoMip( ART_BACK0 );
	trap_R_RegisterShaderNoMip( ART_BACK1 );
	trap_R_RegisterShaderNoMip( ART_GO0 );
	trap_R_RegisterShaderNoMip( ART_GO1 );
	trap_R_RegisterShaderNoMip( ART_ARROWS );
	trap_R_RegisterShaderNoMip( ART_ARROWLEFT );
	trap_R_RegisterShaderNoMip( ART_ARROWRIGHT );
}

/*
===============
UI_DemosMenu
===============
*/
void UI_DemosMenu( void ) {
	Demos_MenuInit();
	UI_PushMenu( &s_demos.menu );
}
