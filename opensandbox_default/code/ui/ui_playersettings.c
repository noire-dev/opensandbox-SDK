// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

#include "ui_local.h"
#if 0
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

} playersettings_t;

static playersettings_t	s_playersettings;

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

	s_playersettings.menu.fullscreen	= qtrue;

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
	s_playersettings.modeltype.color					= color_white;
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
#endif
