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

#define ART_MODEL0			"menu/assets/model_0"
#define ART_MODEL1			"menu/assets/model_1"
#define ART_BACK0			"menu/assets/back_0"
#define ART_BACK1			"menu/assets/back_1"
#define ART_FX_BASE			"menu/assets/fx_base"
#define ART_FX_BLUE			"menu/assets/fx_blue"
#define ART_FX_CYAN			"menu/assets/fx_cyan"
#define ART_FX_GREEN		"menu/assets/fx_grn"
#define ART_FX_RED			"menu/assets/fx_red"
#define ART_FX_TEAL			"menu/assets/fx_teal"
#define ART_FX_WHITE		"menu/assets/fx_white"
#define ART_FX_YELLOW		"menu/assets/fx_yel"

#define ID_NAME			10
#define ID_EFFECTS		11
#define ID_BACK			12
#define ID_MODEL		13
#define ID_MODELTYPE	14

#define MAX_NAMELENGTH	32

#define MENUTEXT_COLUMN 144

typedef struct {
	menuframework_s		menu;
	menuelement_s		e[OSUI_MAX_ELEMENTS];

	menuelement_s			banner;

	menuelement_s			name;
	menuelement_s			effects;

	menuelement_s		back;
	menuelement_s		model;
	menuelement_s		item_null;
	menuelement_s			modeltype;

	qhandle_t			fxBasePic;
	qhandle_t			fxPic[7];
	int					current_fx;
	modelAnim_t			player;

} playersettings_t;


static playersettings_t	s_playersettings;

static int gamecodetoui[] = {4,2,3,0,5,1,6};
static int uitogamecode[] = {4,6,2,3,1,5,7};

/*
=================
PlayerSettings_SetPlayerModelType
=================
*/
static void PlayerSettings_SetPlayerModelType( void )
{
	if (drawTeamModel)
	{
		if(cl_language.integer == 0){
		s_playersettings.modeltype.string = "Team Model";
		}
		if(cl_language.integer == 1){
		s_playersettings.modeltype.string = "Командная Модель";
		}
	}
	else
	{
		if(cl_language.integer == 0){
		s_playersettings.modeltype.string = "DM Model";
		}
		if(cl_language.integer == 1){
		s_playersettings.modeltype.string = "Обычная Модель";
		}
	}

	PText_Init(&s_playersettings.modeltype);
}



/*
=================
PlayerSettings_ToggleModelType
=================
*/
static void PlayerSettings_ToggleModelType( void )
{
	qboolean type;

	if (drawTeamModel) {
		type = qfalse;
	}
	else {
		type = qtrue;
	}

	GUI_PlayerInfo_DrawTeamModel(&s_playersettings.player, type);
	PlayerSettings_SetPlayerModelType();
}




/*
=================
PlayerSettings_DrawName
=================
*/
static void PlayerSettings_DrawName( void *self ) {
	menuelement_s		*f;
	qboolean		focus;
	int				style;
	char			*txt;
	char			c;
	float			*color;
	int				n;
	int				basex, x, y;
	char			name[32];

	f = (menuelement_s*)self;
	basex = f->generic.x;
	y = f->generic.y;
	focus = (f->generic.parent->cursor == f->generic.menuPosition);

	style = UI_LEFT|UI_SMALLFONT;
	color = text_color_normal;
	if( focus ) {
		style |= UI_PULSE;
		color = color_highlight;
	}

if(cl_language.integer == 0){
	UI_DrawString( basex, y, "Name", style, color );
}
if(cl_language.integer == 1){
	UI_DrawString( basex, y, "Имя", style, color );
}

	// draw the actual name
	basex += 64;
	y += PROP_HEIGHT;
	txt = f->field.buffer;
	color = g_color_table[ColorIndex(COLOR_WHITE)];
	x = basex;
	while ( (c = *txt) != 0 ) {
		if ( !focus && Q_IsColorString( txt ) ) {
			n = ColorIndex( *(txt+1) );
			if( n == 0 ) {
				n = 7;
			}
			color = g_color_table[n];
			txt += 2;
			continue;
		}
		UI_DrawChar( x, y, c, style, color );
		txt++;
		x += SMALLCHAR_WIDTH;
	}

	// draw cursor if we have focus
	if( focus ) {
		if ( trap_Key_GetOverstrikeMode() ) {
			c = 11;
		} else {
			c = 10;
		}

		style &= ~UI_PULSE;
		style |= UI_BLINK;

		UI_DrawChar( basex + f->field.cursor * SMALLCHAR_WIDTH, y, c, style, color_white );
	}

	// draw at bottom also using proportional font
	Q_strncpyz( name, f->field.buffer, sizeof(name) );
	Q_CleanStr( name );
	UI_DrawString( 320, 440, name, UI_CENTER|UI_BIGFONT, text_color_normal );
}

/*
=================
PlayerSettings_DrawEffects
=================
*/
static void PlayerSettings_DrawEffects( void *self ) {
	menuelement_s		*item;
	qboolean		focus;
	int				style;
	float			*color;
	int				textlen;

	item = (menuelement_s *)self;
	focus = (item->generic.parent->cursor == item->generic.menuPosition);

	style = UI_LEFT|UI_SMALLFONT;
	color = text_color_normal;
	if( focus ) {
		style |= UI_PULSE;
		color = color_highlight;
	}

	if(cl_language.integer == 0){
	if (item->generic.id == ID_EFFECTS) {
		UI_DrawString( item->generic.x, item->generic.y, "Rail core:", style, color );
	} else {
		UI_DrawString( item->generic.x, item->generic.y, "Rail ring:", style, color );
	}
	}
	if(cl_language.integer == 1){
	if (item->generic.id == ID_EFFECTS) {
		UI_DrawString( item->generic.x, item->generic.y, "Рэйл луч:", style, color );
	} else {
		UI_DrawString( item->generic.x, item->generic.y, "Рейл кольца:", style, color );
	}
	}

	if(cl_language.integer == 0){
		textlen = UI_ProportionalStringWidth("Rail core:", 1.00) * 1.00;
	}
	if(cl_language.integer == 1){
		textlen = UI_ProportionalStringWidth("Рэйл луч:", 1.00) * 1.00;
	}

	UI_DrawHandlePic( item->generic.x + textlen, item->generic.y + 4, 128, 8, s_playersettings.fxBasePic );
	UI_DrawHandlePic( item->generic.x + textlen + item->curvalue * 16 + 8, item->generic.y + 2, 16, 12, s_playersettings.fxPic[item->curvalue] );
}


/*
=================
PlayerSettings_DrawPlayer
=================
*/
static void PlayerSettings_DrawPlayer( void *self ) {
	GUI_PlayerInfo_AnimateModel(&s_playersettings.player);
}


/*
=================
PlayerSettings_DrawMenu
=================
*/
static void PlayerSettings_MenuDraw(void)
{
	if (uis.firstdraw)
		PlayerSettings_SetPlayerModelType();

	// standard menu drawing
	Menu_Draw( &s_playersettings.menu );
}


/*
=================
PlayerSettings_SaveChanges
=================
*/
static void PlayerSettings_SaveChanges( void ) {
	// name
	trap_Cvar_Set( "name", s_playersettings.name.field.buffer );

	// effects color
	trap_Cvar_SetValue( "color1", uitogamecode[s_playersettings.effects.curvalue] );
}


/*
=================
PlayerSettings_MenuKey
=================
*/
static sfxHandle_t PlayerSettings_MenuKey( int key ) {
	if( key == K_MOUSE2 || key == K_ESCAPE ) {
		PlayerSettings_SaveChanges();
	}
	return Menu_DefaultKey( &s_playersettings.menu, key );
}


/*
=================
PlayerSettings_SetMenuItems
=================
*/
static void PlayerSettings_SetMenuItems( void ) {
	vec3_t	viewangles;
	int		c;
	int		h;

	// name
	Q_strncpyz( s_playersettings.name.field.buffer, UI_Cvar_VariableString("name"), sizeof(s_playersettings.name.field.buffer) );

	// effects color
	c = trap_Cvar_VariableValue( "color1" ) - 1;
	if( c < 0 || c > 6 ) {
		c = 6;
	}
	s_playersettings.effects.curvalue = gamecodetoui[c];

	// model/skin
	GUI_PlayerInfo_InitModel(&s_playersettings.player);
}

/*
=================
PlayerSettings_MenuEvent
=================
*/
static void PlayerSettings_MenuEvent( void* ptr, int event ) {
	if( event != QM_ACTIVATED ) {
		return;
	}

	switch( ((menucommon_s*)ptr)->id ) {

	case ID_MODEL:
		PlayerSettings_SaveChanges();
		UI_PlayerModelMenu();
		break;

	case ID_MODELTYPE:
		PlayerSettings_ToggleModelType();
		break;

	case ID_BACK:
		PlayerSettings_SaveChanges();
		UI_PopMenu();
		break;
	}
}


/*
=================
PlayerSettings_MenuInit
=================
*/
static void PlayerSettings_MenuInit( void ) {
	int		y;
	float 	sizeScale;

	memset(&s_playersettings,0,sizeof(playersettings_t));

	s_playersettings.menu.fullscreen = qtrue;
	s_playersettings.menu.native 	   = qfalse;
	s_playersettings.menu.draw = PlayerSettings_MenuDraw;

	PlayerSettings_Cache();

	s_playersettings.menu.key        = PlayerSettings_MenuKey;

	sizeScale = 1.00;

	s_playersettings.banner.generic.type  = MTYPE_TEXT;
	s_playersettings.banner.generic.x     = 320;
	s_playersettings.banner.generic.y     = 16;
	if(cl_language.integer == 0){
	s_playersettings.banner.string        = "PLAYER SETTINGS";
	}
	if(cl_language.integer == 1){
	s_playersettings.banner.string        = "НАСТРОЙКИ ИГРОКА";
	}
	s_playersettings.banner.color         = color_white;
	s_playersettings.banner.style         = UI_CENTER;

	y = 50;
	s_playersettings.name.generic.type			= MTYPE_FIELD;
	s_playersettings.name.generic.flags			= QMF_NODEFAULTINIT;
	s_playersettings.name.generic.ownerdraw		= PlayerSettings_DrawName;
	s_playersettings.name.field.widthInChars	= MAX_NAMELENGTH;
	s_playersettings.name.field.maxchars		= MAX_NAMELENGTH;
	s_playersettings.name.generic.x				= MENUTEXT_COLUMN;
	s_playersettings.name.generic.y				= y;
	s_playersettings.name.generic.left			= MENUTEXT_COLUMN - 8;
	s_playersettings.name.generic.top			= y - 8;
	s_playersettings.name.generic.right			= MENUTEXT_COLUMN + 250;
	s_playersettings.name.generic.bottom		= y + 2 * PROP_HEIGHT;

	y += 5 * PROP_HEIGHT / 2;
	s_playersettings.effects.generic.type		= MTYPE_SPINCONTROL;
	s_playersettings.effects.generic.flags		= QMF_NODEFAULTINIT;
	s_playersettings.effects.generic.id			= ID_EFFECTS;
	s_playersettings.effects.generic.ownerdraw	= PlayerSettings_DrawEffects;
	s_playersettings.effects.generic.x			= MENUTEXT_COLUMN;
	s_playersettings.effects.generic.y			= y;
	s_playersettings.effects.generic.left		= MENUTEXT_COLUMN - 8;
	s_playersettings.effects.generic.top		= y - 4;
	s_playersettings.effects.generic.right		= MENUTEXT_COLUMN + 250;
	s_playersettings.effects.generic.bottom		= y + PROP_HEIGHT;
	s_playersettings.effects.numitems			= 7;

	s_playersettings.model.generic.type			= MTYPE_BITMAP;
	s_playersettings.model.string			= ART_MODEL0;
	s_playersettings.model.generic.flags		= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_playersettings.model.generic.id			= ID_MODEL;
	s_playersettings.model.generic.callback		= PlayerSettings_MenuEvent;
	s_playersettings.model.generic.x			= 640 + uis.wideoffset;
	s_playersettings.model.generic.y			= 480-64;
	s_playersettings.model.width				= 128;
	s_playersettings.model.height				= 64;
	s_playersettings.model.focuspic				= ART_MODEL1;

	s_playersettings.player.bitmap.generic.type		= MTYPE_BITMAP;
	s_playersettings.player.bitmap.generic.flags		= QMF_INACTIVE;
	s_playersettings.player.bitmap.generic.ownerdraw	= PlayerSettings_DrawPlayer;
	s_playersettings.player.bitmap.generic.x			= PLAYERMODEL_X;
	s_playersettings.player.bitmap.generic.y			= PLAYERMODEL_Y;
	s_playersettings.player.bitmap.width				= PLAYERMODEL_WIDTH;
	s_playersettings.player.bitmap.height				= PLAYERMODEL_HEIGHT;

	y = 170;
	UI_CSlider(&s_playersettings.e[0], 220, y, "^1Head:", "headR", 0, 255, 1, NULL, 0); y += 18;
	UI_CSlider(&s_playersettings.e[1], 220, y, "^2Head:", "headG", 0, 255, 1, NULL, 0); y += 18;
	UI_CSlider(&s_playersettings.e[2], 220, y, "^4Head:", "headB", 0, 255, 1, NULL, 0); y += 36;

	UI_CSlider(&s_playersettings.e[3], 220, y, "^1Torso:", "modelR", 0, 255, 1, NULL, 0); y += 18;
	UI_CSlider(&s_playersettings.e[4], 220, y, "^2Torso:", "modelG", 0, 255, 1, NULL, 0); y += 18;
	UI_CSlider(&s_playersettings.e[5], 220, y, "^4Torso:", "modelB", 0, 255, 1, NULL, 0); y += 36;

	UI_CSlider(&s_playersettings.e[6], 220, y, "^1Legs:", "legsR", 0, 255, 1, NULL, 0); y += 18;
	UI_CSlider(&s_playersettings.e[7], 220, y, "^2Legs:", "legsG", 0, 255, 1, NULL, 0); y += 18;
	UI_CSlider(&s_playersettings.e[8], 220, y, "^4Legs:", "legsB", 0, 255, 1, NULL, 0); y += 36;

	s_playersettings.back.generic.type			= MTYPE_BITMAP;
	s_playersettings.back.string			= ART_BACK0;
	s_playersettings.back.generic.flags			= QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_playersettings.back.generic.id			= ID_BACK;
	s_playersettings.back.generic.callback		= PlayerSettings_MenuEvent;
	s_playersettings.back.generic.x				= 0 - uis.wideoffset;
	s_playersettings.back.generic.y				= 480-64;
	s_playersettings.back.width					= 128;
	s_playersettings.back.height				= 64;
	s_playersettings.back.focuspic				= ART_BACK1;

	s_playersettings.item_null.generic.type		= MTYPE_BITMAP;
	s_playersettings.item_null.generic.flags	= QMF_LEFT_JUSTIFY|QMF_MOUSEONLY|QMF_SILENT;
	s_playersettings.item_null.generic.x		= 0;
	s_playersettings.item_null.generic.y		= 0;
	s_playersettings.item_null.width			= 640;
	s_playersettings.item_null.height			= 480;

	s_playersettings.modeltype.generic.type			= MTYPE_PTEXT;
	s_playersettings.modeltype.generic.flags			= QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_playersettings.modeltype.generic.x				= 480;
	s_playersettings.modeltype.generic.y				= PLAYERMODEL_TEXTHEIGHT + PROP_HEIGHT*sizeScale;
	s_playersettings.modeltype.generic.id				= ID_MODELTYPE;
	s_playersettings.modeltype.generic.callback		= PlayerSettings_MenuEvent;
	s_playersettings.modeltype.string					= "";
	s_playersettings.modeltype.color					= text_color_normal;
	s_playersettings.modeltype.style					= UI_CENTER|UI_DROPSHADOW|UI_SMALLFONT;

	Menu_AddItem( &s_playersettings.menu, &s_playersettings.banner );

	Menu_AddItem( &s_playersettings.menu, &s_playersettings.name );
	Menu_AddItem( &s_playersettings.menu, &s_playersettings.effects );
	Menu_AddItem( &s_playersettings.menu, &s_playersettings.model );
	Menu_AddItem( &s_playersettings.menu, &s_playersettings.back );
	UI_CreateUI( &s_playersettings.menu, s_playersettings.e);
	Menu_AddItem( &s_playersettings.menu, &s_playersettings.modeltype );

	Menu_AddItem( &s_playersettings.menu, &s_playersettings.player.bitmap );

	// kills selected item when cursor moves off it, no item
	// registered after this will activate
	Menu_AddItem( &s_playersettings.menu, &s_playersettings.item_null );

	PlayerSettings_SetMenuItems();
}


/*
=================
PlayerSettings_Cache
=================
*/
void PlayerSettings_Cache( void ) {
	trap_R_RegisterShaderNoMip( ART_MODEL0 );
	trap_R_RegisterShaderNoMip( ART_MODEL1 );
	trap_R_RegisterShaderNoMip( ART_BACK0 );
	trap_R_RegisterShaderNoMip( ART_BACK1 );

	s_playersettings.fxBasePic = trap_R_RegisterShaderNoMip( ART_FX_BASE );
	s_playersettings.fxPic[0] = trap_R_RegisterShaderNoMip( ART_FX_RED );
	s_playersettings.fxPic[1] = trap_R_RegisterShaderNoMip( ART_FX_YELLOW );
	s_playersettings.fxPic[2] = trap_R_RegisterShaderNoMip( ART_FX_GREEN );
	s_playersettings.fxPic[3] = trap_R_RegisterShaderNoMip( ART_FX_TEAL );
	s_playersettings.fxPic[4] = trap_R_RegisterShaderNoMip( ART_FX_BLUE );
	s_playersettings.fxPic[5] = trap_R_RegisterShaderNoMip( ART_FX_CYAN );
	s_playersettings.fxPic[6] = trap_R_RegisterShaderNoMip( ART_FX_WHITE );
}


/*
=================
UI_PlayerSettingsMenu
=================
*/
void UI_PlayerSettingsMenu( void ) {
	PlayerSettings_MenuInit();
	UI_PushMenu( &s_playersettings.menu );
}
