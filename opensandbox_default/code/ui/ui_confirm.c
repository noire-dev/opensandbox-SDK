// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.
//
/*
=======================================================================

CONFIRMATION MENU

=======================================================================
*/


#include "ui_local.h"


#define ART_CONFIRM_FRAME	"menu/assets/cut_frame"

#define ID_CONFIRM_NO		10
#define ID_CONFIRM_YES		11


typedef struct {
	menuframework_s menu;

	menuelement_s		no;
	menuelement_s		yes;

	int				slashX;
	const char *	question;
	void			(*draw)( void );
	void			(*action)( qboolean result );
	
	int style;
	const char **lines;
} confirmMenu_t;


static confirmMenu_t	s_confirm;


/*
=================
ConfirmMenu_Event
=================
*/
static void ConfirmMenu_Event( void* ptr, int event ) {
	qboolean	result;

	if( event != QM_ACTIVATED ) {
		return;
	}

	UI_PopMenu();

	if( ((menucommon_s*)ptr)->id == ID_CONFIRM_NO ) {
		result = qfalse;
	}
	else {
		result = qtrue;
	}

	if( s_confirm.action ) {
		s_confirm.action( result );
	}
}


/*
=================
ConfirmMenu_Key
=================
*/
static sfxHandle_t ConfirmMenu_Key( int key ) {
	switch ( key ) {
	case K_KP_LEFTARROW:
	case K_LEFTARROW:
	case K_KP_RIGHTARROW:
	case K_RIGHTARROW:
		key = K_TAB;
		break;

	case 'n':
	case 'N':
		ConfirmMenu_Event( &s_confirm.no, QM_ACTIVATED );
		break;

	case 'y':
	case 'Y':
		ConfirmMenu_Event( &s_confirm.yes, QM_ACTIVATED );
		break;
	}

	return Menu_DefaultKey( &s_confirm.menu, key );
}


/*
=================
MessaheMenu_Draw
=================
*/
static void MessageMenu_Draw( void ) {
	int i,y;
	
	UI_DrawNamedPic( 142, 118, 359, 256, ART_CONFIRM_FRAME );
	
	y = 188;
	for(i=0; s_confirm.lines[i]; i++)
	{
		ST_DrawString( 320, y, s_confirm.lines[i], s_confirm.style, color_white, 1.00 );
		y += 18;
	}

	Menu_Draw( &s_confirm.menu );

	if( s_confirm.draw ) {
		s_confirm.draw();
	}
}

/*
=================
ConfirmMenu_Draw
=================
*/
static void ConfirmMenu_Draw( void ) {
	UI_DrawNamedPic( 142, 118, 359, 256, ART_CONFIRM_FRAME );
	ST_DrawString( 320, 204, s_confirm.question, s_confirm.style, color_white, 1.00 );
	ST_DrawString( s_confirm.slashX, 265, "/", UI_LEFT|UI_INVERSE, color_white, 1.00 );

	Menu_Draw( &s_confirm.menu );

	if( s_confirm.draw ) {
		s_confirm.draw();
	}
}


/*
=================
ConfirmMenu_Cache
=================
*/
void ConfirmMenu_Cache( void ) {
	trap_R_RegisterShaderNoMip( ART_CONFIRM_FRAME );
}


/*
=================
UI_ConfirmMenu_Stlye
=================
*/
void UI_ConfirmMenu_Style( const char *question, int style, void (*action)( qboolean result ) ) {
	uiClientState_t	cstate;
	int	n1, n2, n3;
	int	l1, l2, l3;

	// zero set all our globals
	memset( &s_confirm, 0, sizeof(s_confirm) );

	ConfirmMenu_Cache();

if(cl_language.integer == 0){
	n1 = ST_StringWidth( "YES/NO", 1.00 );
	n2 = ST_StringWidth( "YES", 1.00 );
	n3 = ST_StringWidth( "/", 1.00 );
}
if(cl_language.integer == 1){
	n1 = ST_StringWidth( "ДА/НЕТ", 1.00 );
	n2 = ST_StringWidth( "ДА", 1.00 );
	n3 = ST_StringWidth( "/", 1.00 );
}
	l1 = 320 - ( n1 / 2 );
	l2 = l1 + n2;
	l3 = l2 + n3;
	s_confirm.slashX = l2;

	s_confirm.question = question;
	s_confirm.action = action;
	s_confirm.style = style;

	s_confirm.menu.draw       = ConfirmMenu_Draw;
	s_confirm.menu.key        = ConfirmMenu_Key;

	trap_GetClientState( &cstate );
	if ( cstate.connState >= CA_CONNECTED ) {
		s_confirm.menu.fullscreen = qfalse;
	}
	else {
		s_confirm.menu.fullscreen = qtrue;
	}

	s_confirm.yes.generic.type		= MTYPE_PTEXT;      
	s_confirm.yes.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS; 
	s_confirm.yes.generic.callback	= ConfirmMenu_Event;
	s_confirm.yes.generic.id		= ID_CONFIRM_YES;
	s_confirm.yes.generic.x			= l1;
	s_confirm.yes.generic.y			= 264;
	if(cl_language.integer == 0){
	s_confirm.yes.string			= "YES";
	}
	if(cl_language.integer == 1){
	s_confirm.yes.string			= "ДА";
	}
	s_confirm.yes.color				= color_white;
	s_confirm.yes.style				= UI_LEFT;

	s_confirm.no.generic.type		= MTYPE_PTEXT;      
	s_confirm.no.generic.flags		= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS; 
	s_confirm.no.generic.callback	= ConfirmMenu_Event;
	s_confirm.no.generic.id			= ID_CONFIRM_NO;
	s_confirm.no.generic.x		    = l3;
	s_confirm.no.generic.y		    = 264;
	if(cl_language.integer == 0){
	s_confirm.no.string				= "NO";
	}
	if(cl_language.integer == 1){
	s_confirm.no.string				= "НЕТ";
	}
	s_confirm.no.color			    = color_white;
	s_confirm.no.style			    = UI_LEFT;

	Menu_AddItem( &s_confirm.menu,	&s_confirm.yes );             
	Menu_AddItem( &s_confirm.menu,	&s_confirm.no );

	UI_PushMenu( &s_confirm.menu );

	Menu_SetCursorToItem( &s_confirm.menu, &s_confirm.no );
}

/*
=================
UI_ConfirmMenu
=================
*/
void UI_ConfirmMenu( const char *question, void (*action)( qboolean result ) ) {
	UI_ConfirmMenu_Style(question, UI_CENTER|UI_INVERSE, action);
}
