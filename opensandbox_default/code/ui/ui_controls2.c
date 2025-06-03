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

#include "ui_local.h"

typedef struct {
	char	*command;
	char	*label;
	int		id;
	int		bind1;
	int		bind2;
} bind_t;

#define C_KEYS			0
#define C_SETTINGS		1
#define C_MAX			2

#define ID_BACK			88

#define ID_KEYS			90
#define ID_SETTINGS		91

#define KEYS_NUM		30
#define SETTINGS_NUM	KEYS_NUM+5

typedef struct {
	menuframework_s		menu;
	menuelement_s		e[OSUI_MAX_ELEMENTS];

	int					section;
	qboolean			waitingforkey;
} controls_t; 	

static controls_t controls;

static bind_t g_bindings[] = {
	{"+scores",					"show scores",			0,	-1, -1},
	{"+button2",				"use item",				1,	-1, -1},
	{"+speed", 					"run / walk",			2,	-1,	-1},
	{"+forward", 				"walk forward",			3,	-1, -1},
	{"+back", 					"walk back",			4,	-1, -1},
	{"+moveleft", 				"walk left",			5,	-1, -1},
	{"+moveright", 				"walk right",			6,	-1, -1},
	{"+moveup",					"up / jump",			7,	-1, -1},
	{"+movedown",				"down / crouch",		8,	-1, -1},
	{"+zoom", 					"zoom view",			9,	-1, -1},
	{"weapon 1",				"gauntlet",				10,	-1, -1},
	{"weapon 2",				"machinegun",			11,	-1, -1},
	{"weapon 3",				"shotgun",				12,	-1, -1},
	{"weapon 4",				"grenade launcher",		13,	-1, -1},
	{"weapon 5",				"rocket launcher",		14,	-1, -1},
	{"weapon 6",				"lightning",			15,	-1, -1},
	{"weapon 7",				"railgun",				16,	-1, -1},
	{"weapon 8",				"plasma gun",			17,	-1, -1},
	{"weapon 9",				"BFG",					18,	-1, -1},
	{"weapon 10",				"grappling hook",		19,	-1, -1},
	{"+attack", 				"attack",				20,	-1, -1},
	{"weapprev",				"prev weapon",			21,	-1, -1},
	{"weapnext", 				"next weapon",			22,	-1, -1},
	{"+button3", 				"gesture / horn",		23,	-1, -1},
	{"messagemode", 			"chat",					24,	-1, -1},
	{"messagemode2", 			"chat - team",			25,	-1, -1},
	{"messagemode3", 			"chat - target",		26,	-1, -1},
	{"flashlight", 				"flashlight",			27,	-1, -1},
	{"ui_sandbox", 				"sandbox menu",			28,	-1, -1},
	{"vstr uitoolmode", 		"sandbox tool mode",	29,	-1, -1},
	{"exitvehicle", 			"exit vehicle",			30,	-1, -1},
	{(char*)NULL,				(char*)NULL,			-1,	-1,	-1},
};

static const char* mousestyle_description[] = {
	"Win32",
	"Raw",
	NULL
};

static void Controls_Update( void ){
	int		i, j, y;

	// disable all controls in all groups
	for( i = 0; i < C_MAX; i++ ) {
		if(i == C_KEYS){
			for( j = 0;	j <= KEYS_NUM; j++ ){
				controls.e[j].generic.flags |= (QMF_HIDDEN|QMF_INACTIVE);
			}
		}
		if(i == C_SETTINGS){
			for( j = KEYS_NUM+1; j <= SETTINGS_NUM; j++ ){
				controls.e[j].generic.flags |= (QMF_HIDDEN|QMF_INACTIVE);
			}
		}
	}

	// enable controls in active group (and count number of items for vertical centering)
	if(controls.section == C_KEYS){
		for( j = 0;	j <= KEYS_NUM; j++ ){
			controls.e[j].generic.flags &= ~(QMF_GRAYED|QMF_HIDDEN|QMF_INACTIVE);
		}
	}
	if(controls.section == C_SETTINGS){
		for( j = KEYS_NUM+1; j <= SETTINGS_NUM; j++ ){
			controls.e[j].generic.flags &= ~(QMF_GRAYED|QMF_HIDDEN|QMF_INACTIVE);
		}
	}

	// position controls
	y = 80;
	if(controls.section == C_KEYS){
		for( j = 0; j <= KEYS_NUM; j++, y += BASEFONT_HEIGHT ){
			controls.e[j].generic.x      = SCREEN_WIDTH*0.64;
			controls.e[j].generic.y      = y-30;
			controls.e[j].generic.left   = SCREEN_WIDTH*0.64 - 19*BASEFONT_INDENT;
			controls.e[j].generic.right  = SCREEN_WIDTH*0.64 + 21*BASEFONT_INDENT;
			controls.e[j].generic.top    = y-30;
			controls.e[j].generic.bottom = y-30 + BASEFONT_HEIGHT;
		}
	}
	if(controls.section == C_SETTINGS){
		for( j = KEYS_NUM+1; j <= SETTINGS_NUM; j++, y += BASEFONT_HEIGHT ){
			controls.e[j].generic.x      = SCREEN_WIDTH*0.64;
			controls.e[j].generic.y      = y-30;
			controls.e[j].generic.left   = SCREEN_WIDTH*0.64 - 19*BASEFONT_INDENT;
			controls.e[j].generic.right  = SCREEN_WIDTH*0.64 + 21*BASEFONT_INDENT;
			controls.e[j].generic.top    = y-30;
			controls.e[j].generic.bottom = y-30 + BASEFONT_HEIGHT;
		}
	}

	if( controls.waitingforkey ){
		// disable everybody
		for( i = 0; i < controls.menu.nitems; i++ ){
			((menucommon_s*)(controls.menu.items[i]))->flags |= QMF_GRAYED;
		}

		// enable action item
		((menucommon_s*)(controls.menu.items[controls.menu.cursor]))->flags &= ~QMF_GRAYED;
		return;
	}

	// enable everybody
	for( i = 0; i < controls.menu.nitems; i++ ){
		((menucommon_s*)(controls.menu.items[i]))->flags &= ~QMF_GRAYED;
	}

	for( i = 90; i < 100; i++ ){
		controls.e[i].generic.flags &= ~(QMF_GRAYED | QMF_INACTIVE);
	}
	switch (controls.section){
		case C_KEYS:
			controls.e[ID_KEYS].generic.flags |= (QMF_GRAYED | QMF_INACTIVE);
			break;
		case C_SETTINGS:
			controls.e[ID_SETTINGS].generic.flags |= (QMF_GRAYED | QMF_INACTIVE);
			break;
	}
}

static void Controls_DrawKeyBinding( void *self ){
	menuelement_s*	a;
	int				x;
	int				y;
	int				b1;
	int				b2;
	qboolean		c;
	char			name[32];
	char			name2[32];

	a = (menuelement_s*) self;

	x =	a->generic.x;
	y = a->generic.y;

	c = (Menu_ItemAtCursor( a->generic.parent ) == a);

	b1 = g_bindings[a->generic.id].bind1;
	if (b1 == -1) {
		strcpy(name,"???");
	} else {
		trap_Key_KeynumToStringBuf( b1, name, 32 );
		Q_strupr(name);

		b2 = g_bindings[a->generic.id].bind2;
		if (b2 != -1) {
			trap_Key_KeynumToStringBuf( b2, name2, 32 );
			Q_strupr(name2);

			strcat( name, " or " );
			strcat( name, name2 );
		}
	}

	if (c){
		UI_FillRect( a->generic.left, a->generic.top, a->generic.right-a->generic.left+1, a->generic.bottom-a->generic.top+1, color_select_bluo ); 
		ST_DrawString( x - BASEFONT_INDENT, y, g_bindings[a->generic.id].label, UI_RIGHT|UI_SMALLFONT, color_highlight, 1.00 );
		ST_DrawString( x + BASEFONT_INDENT, y, name, UI_LEFT|UI_SMALLFONT|UI_PULSE, color_highlight, 1.00 );

		if (controls.waitingforkey){
			ST_DrawChar( x, y, '=', UI_CENTER|UI_BLINK|UI_SMALLFONT, color_highlight, 1.00);
			ST_DrawString(SCREEN_WIDTH*0.64, SCREEN_HEIGHT * 0.90, "Waiting for new key ... ESCAPE to cancel", UI_SMALLFONT|UI_CENTER|UI_PULSE, colorWhite, 1.00 );
		} else {
			ST_DrawChar( x, y, 13, UI_CENTER|UI_BLINK|UI_SMALLFONT, color_highlight, 1.00);
			ST_DrawString(SCREEN_WIDTH*0.64, SCREEN_HEIGHT * 0.90, "Press ENTER or CLICK to change", UI_SMALLFONT|UI_CENTER, colorWhite, 1.00 );
			ST_DrawString(SCREEN_WIDTH*0.64, SCREEN_HEIGHT * 0.95, "Press BACKSPACE to clear", UI_SMALLFONT|UI_CENTER, colorWhite, 1.00 );
		}
	} else {
		if (a->generic.flags & QMF_GRAYED){
			ST_DrawString( x - BASEFONT_INDENT, y, g_bindings[a->generic.id].label, UI_RIGHT|UI_SMALLFONT, color_disabled, 1.00 );
			ST_DrawString( x + BASEFONT_INDENT, y, name, UI_LEFT|UI_SMALLFONT, color_disabled, 1.00 );
		} else {
			ST_DrawString( x - BASEFONT_INDENT, y, g_bindings[a->generic.id].label, UI_RIGHT|UI_SMALLFONT, color_white, 1.00 );
			ST_DrawString( x + BASEFONT_INDENT, y, name, UI_LEFT|UI_SMALLFONT, color_white, 1.00 );
		}
	}
}

static void Controls_GetKeyAssignment (char *command, int *twokeys){
	int		count;
	int		j;
	char	b[256];

	twokeys[0] = twokeys[1] = -1;
	count = 0;

	for ( j = 0; j < 256; j++ ){
		trap_Key_GetBindingBuf( j, b, 256 );
		if ( *b == 0 ) {
			continue;
		}
		if ( !Q_stricmp( b, command ) ){
			twokeys[count] = j;
			count++;
			if (count == 2)
				break;
		}
	}
}

static void Controls_GetConfig( void ){
	int		i;
	int		twokeys[2];
	bind_t*	bindptr;

	// put the bindings into a local store
	bindptr = g_bindings;

	// iterate each command, get its numeric binding
	for (i=0; ;i++,bindptr++){
		if (!bindptr->label)
			break;

		Controls_GetKeyAssignment(bindptr->command, twokeys);

		bindptr->bind1 = twokeys[0];
		bindptr->bind2 = twokeys[1];
	}
	controls.e[KEYS_NUM+1].curvalue = (trap_Cvar_VariableValue( "in_mouse" ) > 0) ? 1:0;
}

static void Controls_SetConfig( void ){
	int		i;
	bind_t*	bindptr;

	// set the bindings from the local store
	bindptr = g_bindings;

	// iterate each command, get its numeric binding
	for (i=0; ;i++,bindptr++){
		if (!bindptr->label)
			break;

		if (bindptr->bind1 != -1){	
			trap_Key_SetBinding( bindptr->bind1, bindptr->command );

			if (bindptr->bind2 != -1)
				trap_Key_SetBinding( bindptr->bind2, bindptr->command );
		}
	}
}

static sfxHandle_t Controls_MenuKey( int key ){
	int			id;
	int			i;
	qboolean	found;
	bind_t*		bindptr;
	found = qfalse;

	if (!controls.waitingforkey) {
		switch (key) {
			case K_BACKSPACE:
			case K_DEL:
			case K_KP_DEL:
				key = -1;
				break;
		
			case K_MOUSE2:
			case K_ESCAPE:
				Controls_SetConfig();
				goto ignorekey;	

			default:
				goto ignorekey;
		}
	} else {
		if (key & K_CHAR_FLAG)
			goto ignorekey;

		switch (key) {
			case K_ESCAPE:
				controls.waitingforkey = qfalse;
				Controls_Update();
				return (menu_out_sound);
	
			case '`':
				goto ignorekey;
		}
	}
	
	if (key != -1) {
		// remove from any other bind
		bindptr = g_bindings;
		for (i=0; ;i++,bindptr++){
			if (!bindptr->label)	
				break;

			if (bindptr->bind2 == key)
				bindptr->bind2 = -1;

			if (bindptr->bind1 == key)
			{
				bindptr->bind1 = bindptr->bind2;	
				bindptr->bind2 = -1;
			}
		}
	}

	// assign key to local store
	id      = ((menucommon_s*)(controls.menu.items[controls.menu.cursor]))->id;
	bindptr = g_bindings;
	for (i=0; ;i++,bindptr++){
		if (!bindptr->label)	
			break;
		
		if (bindptr->id == id){
			found = qtrue;
			if (key == -1) {
				if( bindptr->bind1 != -1 ){
					trap_Key_SetBinding( bindptr->bind1, "" );
					bindptr->bind1 = -1;
				}
				if( bindptr->bind2 != -1 ){
					trap_Key_SetBinding( bindptr->bind2, "" );
					bindptr->bind2 = -1;
				}
			} else if (bindptr->bind1 == -1){
				bindptr->bind1 = key;
			} else if (bindptr->bind1 != key && bindptr->bind2 == -1){
				bindptr->bind2 = key;
			} else {
				trap_Key_SetBinding( bindptr->bind1, "" );
				trap_Key_SetBinding( bindptr->bind2, "" );
				bindptr->bind1 = key;
				bindptr->bind2 = -1;
			}						
			break;
		}
	}				
		
	controls.waitingforkey = qfalse;

	if (found) {	
		Controls_Update();
		return (menu_out_sound);
	}

ignorekey:
	return Menu_DefaultKey( &controls.menu, key );
}

static void Controls_MenuEvent( void* ptr, int event ){
	if (event != QM_ACTIVATED) { return; }
	switch (((menucommon_s*)ptr)->id){
		case ID_KEYS:
			controls.section = C_KEYS; 
			Controls_Update();
			break;

		case ID_SETTINGS:
			controls.section = C_SETTINGS; 
			Controls_Update();
			break;

		case ID_BACK:
			Controls_SetConfig();
			UI_PopMenu();
			break;

		case KEYS_NUM+1:
			trap_Cvar_SetValue( "in_mouse", (controls.e[KEYS_NUM+1].curvalue == 1) ? 1:-1);
			trap_Cmd_ExecuteText( EXEC_APPEND, "in_restart\n" );
			break;		
	}
}

static void Controls_ActionEvent( void* ptr, int event ){
	if ((event == QM_ACTIVATED) && !controls.waitingforkey){
		controls.waitingforkey = 1;
		Controls_Update();
	}
}

void UI_Controls( void ){
	int 		i;
	int			y;

	memset( &controls, 0 ,sizeof(controls_t) );
	controls.menu.key        					= Controls_MenuKey;
	controls.menu.fullscreen 					= qtrue;

	UI_CText(&controls.e[89], OSUI_LOGO_X, OSUI_LOGO_Y+24, "CONTROLS", UI_LEFT, 1.80);

	y = OSUI_STANDARD_Y;
	UI_CButton(&controls.e[ID_KEYS], 64 - uis.wideoffset, y, "Keys", UI_LEFT, 1.00, NULL, NULL, NULL, Controls_MenuEvent, ID_KEYS); y += OSUI_SPACING_Y;
	UI_CButton(&controls.e[ID_SETTINGS], 64 - uis.wideoffset, y, "Settings", UI_LEFT, 1.00, NULL, NULL, NULL, Controls_MenuEvent, ID_SETTINGS); y += OSUI_BIGSPACING_Y;

	UI_CButton(&controls.e[ID_BACK], 64 - uis.wideoffset, y, "Back", UI_LEFT, 1.00, NULL, NULL, NULL, Controls_MenuEvent, ID_BACK); y += OSUI_BIGSPACING_Y;

	for(i = 0; i <= KEYS_NUM; i++){
		controls.e[i].generic.type			= MTYPE_ACTION;
		controls.e[i].generic.flags			= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS|QMF_GRAYED|QMF_HIDDEN;
		controls.e[i].generic.callback		= Controls_ActionEvent;
		controls.e[i].generic.ownerdraw		= Controls_DrawKeyBinding;
		controls.e[i].generic.id				= i;
	}

	UI_CSpinControl(&controls.e[KEYS_NUM+1], SCREEN_WIDTH*0.64, 0, "input method", mousestyle_description, Controls_MenuEvent, KEYS_NUM+1);
	UI_CRadioButton(&controls.e[KEYS_NUM+2], SCREEN_WIDTH*0.64, 0, "free look", "cl_freelook", RBT_NORMAL, NULL, 0);
	UI_CRadioButton(&controls.e[KEYS_NUM+3], SCREEN_WIDTH*0.64, 0, "smooth mouse", "m_filter", RBT_NORMAL, NULL, 0);
	UI_CRadioButton(&controls.e[KEYS_NUM+4], SCREEN_WIDTH*0.64, 0, "always run", "cl_run", RBT_NORMAL, NULL, 0);
	UI_CSlider(&controls.e[KEYS_NUM+5], SCREEN_WIDTH*0.64, 0, "mouse speed", "sensitivity", 2, 30, 1, NULL, 0);

	UI_CreateUI( &controls.menu, controls.e);

	Controls_GetConfig();

	controls.section = C_KEYS;

	Controls_Update();

	UI_PushMenu( &controls.menu );
}
