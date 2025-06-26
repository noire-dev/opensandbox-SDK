// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.
//
#include "ui_local.h"

/*********************************************************************************
	SPECIFY SERVER
*********************************************************************************/

#define SPECIFYSERVER_BACK0		"menu/assets/back_0"
#define SPECIFYSERVER_BACK1		"menu/assets/back_1"
#define SPECIFYSERVER_FIGHT0	"menu/assets/fight_0"
#define SPECIFYSERVER_FIGHT1	"menu/assets/fight_1"

#define ID_SPECIFYSERVERBACK	102
#define ID_SPECIFYSERVERGO		103

static char* specifyserver_artlist[] =
{
	SPECIFYSERVER_BACK0,	
	SPECIFYSERVER_BACK1,	
	SPECIFYSERVER_FIGHT0,
	SPECIFYSERVER_FIGHT1,
	NULL
};

typedef struct
{
	menuframework_s	menu;
	menuelement_s		banner;
	menuelement_s		domain;
	menuelement_s		port;
	menuelement_s	go;
	menuelement_s	back;
} specifyserver_t;

static specifyserver_t	s_specifyserver;

/*
=================
SpecifyServer_Event
=================
*/
static void SpecifyServer_Event( void* ptr, int event )
{
	char	buff[256];
	int i;

	switch (((menucommon_s*)ptr)->id)
	{
		case ID_SPECIFYSERVERGO:
			if (event != QM_ACTIVATED)
				break;

			if (s_specifyserver.domain.field.buffer[0])
			{
				strcpy(buff,s_specifyserver.domain.field.buffer);
				if (s_specifyserver.port.field.buffer[0])
					Com_sprintf( buff+strlen(buff), 128, ":%s", s_specifyserver.port.field.buffer );

				for( i = 1; i < MAX_FAVORITESERVERS; i++){
				if(!trap_Cvar_VariableValue(va("server%i", i))){
				trap_Cmd_ExecuteText( EXEC_APPEND, va("seta server%i %s\n", i, buff));
				trap_Cmd_ExecuteText( EXEC_APPEND, va("connect %s\n", buff));
				UI_PopMenu();
				UI_PopMenu();
				return;
				}
				}
				
				
			}
			break;

		case ID_SPECIFYSERVERBACK:
			if (event != QM_ACTIVATED)
				break;

			UI_PopMenu();
			break;
	}
}

/*
=================
SpecifyServer_MenuInit
=================
*/
void SpecifyServer_MenuInit( void )
{
	// zero set all our globals
	memset( &s_specifyserver, 0 ,sizeof(specifyserver_t) );

	SpecifyServer_Cache();

	s_specifyserver.menu.fullscreen = qtrue;

	s_specifyserver.banner.generic.type	 = MTYPE_TEXT;
	s_specifyserver.banner.generic.x     = 320;
	s_specifyserver.banner.generic.y     = 16;
	if(cl_language.integer == 0){
	s_specifyserver.banner.string		 = "ADD TO FAVORITES";
	}
	if(cl_language.integer == 1){
	s_specifyserver.banner.string		 = "ДОБАВИТЬ В ЛЮБИМЫЕ";
	}
	s_specifyserver.banner.color  		 = color_white;
	s_specifyserver.banner.style  		 = UI_CENTER;

	s_specifyserver.domain.generic.type       = MTYPE_FIELD;
	if(cl_language.integer == 0){
	s_specifyserver.domain.string       = "Address:";
	}
	if(cl_language.integer == 1){
	s_specifyserver.domain.string       = "Адрес:";
	}
	s_specifyserver.domain.generic.flags      = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_specifyserver.domain.generic.x	      = 206;
	s_specifyserver.domain.generic.y	      = 220;
	s_specifyserver.domain.field.widthInChars = 38;
	s_specifyserver.domain.field.maxchars     = 80;

	s_specifyserver.port.generic.type       = MTYPE_FIELD;
	if(cl_language.integer == 0){
	s_specifyserver.port.string	    = "Port:";
	}
	if(cl_language.integer == 1){
	s_specifyserver.port.string	    = "Порт:";
	}
	s_specifyserver.port.generic.flags	    = QMF_PULSEIFFOCUS|QMF_SMALLFONT|QMF_NUMBERSONLY;
	s_specifyserver.port.generic.x	        = 206;
	s_specifyserver.port.generic.y	        = 250;
	s_specifyserver.port.field.widthInChars = 6;
	s_specifyserver.port.field.maxchars     = 5;

	s_specifyserver.go.generic.type	    = MTYPE_BITMAP;
	s_specifyserver.go.string     = SPECIFYSERVER_FIGHT0;
	s_specifyserver.go.generic.flags    = QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_specifyserver.go.generic.callback = SpecifyServer_Event;
	s_specifyserver.go.generic.id	    = ID_SPECIFYSERVERGO;
	s_specifyserver.go.generic.x		= 640 + uis.wideoffset;
	s_specifyserver.go.generic.y		= 480-64;
	s_specifyserver.go.width  		    = 128;
	s_specifyserver.go.height  		    = 64;
	s_specifyserver.go.focuspic         = SPECIFYSERVER_FIGHT1;

	s_specifyserver.back.generic.type	  = MTYPE_BITMAP;
	s_specifyserver.back.string     = SPECIFYSERVER_BACK0;
	s_specifyserver.back.generic.flags    = QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_specifyserver.back.generic.callback = SpecifyServer_Event;
	s_specifyserver.back.generic.id	      = ID_SPECIFYSERVERBACK;
	s_specifyserver.back.generic.x		  = 0 - uis.wideoffset;
	s_specifyserver.back.generic.y		  = 480-64;
	s_specifyserver.back.width  		  = 128;
	s_specifyserver.back.height  		  = 64;
	s_specifyserver.back.focuspic         = SPECIFYSERVER_BACK1;

	Menu_AddItem( &s_specifyserver.menu, &s_specifyserver.banner );
	Menu_AddItem( &s_specifyserver.menu, &s_specifyserver.domain );
	Menu_AddItem( &s_specifyserver.menu, &s_specifyserver.port );
	Menu_AddItem( &s_specifyserver.menu, &s_specifyserver.go );
	Menu_AddItem( &s_specifyserver.menu, &s_specifyserver.back );

	Com_sprintf( s_specifyserver.port.field.buffer, 6, "%i", 27960 );
}

/*
=================
SpecifyServer_Cache
=================
*/
void SpecifyServer_Cache( void )
{
	int	i;

	// touch all our pics
	for (i=0; ;i++)
	{
		if (!specifyserver_artlist[i])
			break;
		trap_R_RegisterShaderNoMip(specifyserver_artlist[i]);
	}
}

/*
=================
UI_SpecifyServerMenu
=================
*/
void UI_SpecifyServerMenu( void )
{
	SpecifyServer_MenuInit();
	UI_PushMenu( &s_specifyserver.menu );
}

