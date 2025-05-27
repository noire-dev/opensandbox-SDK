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

GAME OPTIONS MENU

=======================================================================
*/

#include "ui_local.h"

#define ART_BACK0				"menu/assets/back_0"
#define ART_BACK1				"menu/assets/back_1"

#define PREFERENCES_X_POS		360
#define PREFERENCES_X_LEFT		(160 + 5*BASEFONT_INDENT)
#define PREFERENCES_X_RIGHT		(480 + 5*BASEFONT_INDENT)

#define LINE_GAP		(BIGCHAR_HEIGHT + 2)


#define CONTROL_X (SCREEN_WIDTH/2 + 40)
#define SMALLCHAR_LINEHEIGHT (BASEFONT_HEIGHT + 2)


#define ID_HUD					50
#define ID_RENDER				51
#define ID_OBJECTS				52
#define ID_MISC					53

#define ID_CROSSHAIR			127
#define ID_SIMPLEITEMS			128
#define ID_HIGHQUALITYSKY		129
#define ID_EJECTINGBRASS		130
#define ID_WALLMARKS			131
#define ID_DYNAMICLIGHTS		132
#define ID_IDENTIFYTARGET		133
#define ID_SYNCEVERYFRAME		134
#define ID_ALLOWDOWNLOAD		135
#define ID_BACK					136
#define ID_BOTMENU				137
#define ID_GIBS					138
#define ID_BLOOD				139
#define ID_DRAWFPS				140
#define ID_DRAWTIMER			141
#define ID_LAGOMETER			142
#define ID_PREDICTITEMS			143
#define ID_SHADOWS				144
#define ID_TEAMCHATHEIGHT		145
#define ID_FOV					146
#define ID_ZOOMFOV				147
#define ID_AMMOWARNING			148
#define ID_DRAWGUN				149
#define ID_COLORRED             151
#define ID_COLORGREEN           152
#define ID_COLORBLUE            153
#define ID_WEAPONBAR            154

#define	NUM_CROSSHAIRS			10


typedef struct {
	menuframework_s		menu;

	menuelement_s			banner;

	menuelement_s			hud;
	menuelement_s			render;
	menuelement_s			objects;
    menuelement_s			misc;

	menuelement_s			crosshair;
    //Crosshair colors:
    menuelement_s            crosshairColorRed;
    menuelement_s            crosshairColorGreen;
    menuelement_s            crosshairColorBlue;
	menuelement_s	simpleitems;
    menuelement_s	alwaysweaponbar;
	menuelement_s	brass;
	menuelement_s	wallmarks;
	menuelement_s	dynamiclights;
	menuelement_s	identifytarget;
	menuelement_s	highqualitysky;
	menuelement_s	synceveryframe;
	menuelement_s	allowdownload;
	menuelement_s	botmenu;

	menuelement_s	gibs;
	menuelement_s	blood;
	menuelement_s	drawfps;
	menuelement_s	drawtimer;
	menuelement_s	drawlagometer;
	menuelement_s	drawgun;
	menuelement_s	predictitems;
	menuelement_s			shadows;

	menuelement_s			teamchatheight;
	menuelement_s			fov;
	menuelement_s			zoomfov;

	menuelement_s		back;

	qhandle_t			crosshairShader[NUM_CROSSHAIRS];
	int section;
	int max_control_text;
} preferences_t;

static preferences_t s_preferences;

static const char *teamoverlay_names[] =
{
	"off",
	"upper right",
	"lower right",
	"lower left",
	0
};

static const char *shadow_types[] =
{
	"off",
	"disc",
	"stencil",
	"simple stencil",
	0
};

static const char *shadow_typesru[] =
{
	"выкл",
	"диск",
	"тень",
	"простая тень",
	0
};

static menucommon_s* g_hud_controls[] = {
	(menucommon_s*) &s_preferences.crosshair,
	(menucommon_s*) &s_preferences.crosshairColorRed,
	(menucommon_s*) &s_preferences.crosshairColorGreen,
	(menucommon_s*) &s_preferences.crosshairColorBlue,
	(menucommon_s*) &s_preferences.alwaysweaponbar,
	(menucommon_s*) &s_preferences.identifytarget,
	(menucommon_s*) &s_preferences.teamchatheight,
	(menucommon_s*) &s_preferences.drawfps,
	(menucommon_s*) &s_preferences.synceveryframe,
	(menucommon_s*) &s_preferences.drawtimer,
	(menucommon_s*) &s_preferences.drawlagometer,
	(menucommon_s*) &s_preferences.drawgun,
	NULL
};


static menucommon_s* g_render_controls[] = {
	(menucommon_s*) &s_preferences.shadows,
	(menucommon_s*) &s_preferences.highqualitysky,
	(menucommon_s*) &s_preferences.blood,
	(menucommon_s*) &s_preferences.fov,
	(menucommon_s*) &s_preferences.zoomfov,
	NULL
};

static menucommon_s* g_object_controls[] = {
	(menucommon_s*) &s_preferences.simpleitems,
	(menucommon_s*) &s_preferences.wallmarks,
	(menucommon_s*) &s_preferences.brass,
	(menucommon_s*) &s_preferences.dynamiclights,
	(menucommon_s*) &s_preferences.gibs,
	NULL
};

static menucommon_s* g_prefmisc_controls[] = {
	(menucommon_s*) &s_preferences.predictitems,
	(menucommon_s*) &s_preferences.allowdownload,
	(menucommon_s*) &s_preferences.botmenu,
	NULL
};


#define C_HUD		0
#define C_RENDER	1
#define C_OBJECTS	2
#define C_MISC		3

#define C_MAX 		4

static menucommon_s** g_control_list[] = {
	g_hud_controls,
	g_render_controls,
	g_object_controls,
	g_prefmisc_controls
};

static void Preferences_SetMenuItems( void ) {
	s_preferences.crosshair.curvalue		= (int)trap_Cvar_VariableValue( "cg_drawCrosshair" ) % NUM_CROSSHAIRS;
    s_preferences.crosshairColorRed.curvalue        = trap_Cvar_VariableValue( "cg_crosshairColorRed")*255.0f;
    s_preferences.crosshairColorGreen.curvalue      = trap_Cvar_VariableValue( "cg_crosshairColorGreen")*255.0f;
    s_preferences.crosshairColorBlue.curvalue       = trap_Cvar_VariableValue( "cg_crosshairColorBlue")*255.0f;
    s_preferences.alwaysweaponbar.curvalue		= trap_Cvar_VariableValue( "cg_alwaysWeaponBar" ) != 0;
	s_preferences.identifytarget.curvalue	= trap_Cvar_VariableValue( "cg_drawCrosshairNames" ) != 0;
	s_preferences.dynamiclights.curvalue	= trap_Cvar_VariableValue( "r_dynamiclight" ) != 0;
	s_preferences.highqualitysky.curvalue	= trap_Cvar_VariableValue ( "r_fastsky" ) == 0;
	s_preferences.synceveryframe.curvalue	= trap_Cvar_VariableValue( "r_swapInterval" ) != 0;
	s_preferences.allowdownload.curvalue	= trap_Cvar_VariableValue( "cl_allowDownload" ) != 0;
	s_preferences.botmenu.curvalue			= trap_Cvar_VariableValue( "gui_autoclosebotmenu" ) != 0;

	s_preferences.blood.curvalue			= trap_Cvar_VariableValue( "com_blood" ) != 0;
	s_preferences.drawfps.curvalue			= trap_Cvar_VariableValue( "cg_drawFPS" ) != 0;
	s_preferences.drawtimer.curvalue		= trap_Cvar_VariableValue( "cg_drawTimer" ) != 0;
	s_preferences.drawlagometer.curvalue	= trap_Cvar_VariableValue( "cg_lagometer" ) != 0;
	s_preferences.shadows.curvalue			= Com_Clamp( 0, 3, trap_Cvar_VariableValue( "cg_shadows" ) );


	trap_Cvar_VariableStringBuffer("com_maxfps", s_preferences.teamchatheight.field.buffer, 4);
	trap_Cvar_VariableStringBuffer("cg_fov", s_preferences.fov.field.buffer, 4);
	trap_Cvar_VariableStringBuffer("cg_zoomfov", s_preferences.zoomfov.field.buffer, 4);
}

/*
=================
Control_Update
=================
*/
static void Control_Update( void )
{
	int i, j;
	menucommon_s** list;
	menucommon_s*	control;
	int	count, y;

	// hide all controls
	for (i = 0; i < C_MAX; i++)
	{
		list = g_control_list[i];
		for ( j = 0; (control = list[j]); j++)
		{
			control->flags |= (QMF_HIDDEN|QMF_INACTIVE);
		}
	}

	// count controls in current selection
	list = g_control_list[s_preferences.section];
	for (count = 0; list[count]; count++) {
		list[count]->flags &= ~ (QMF_HIDDEN|QMF_INACTIVE|QMF_GRAYED);
	}

	// show current controls, centering vertically
	y = (SCREEN_HEIGHT - (count * SMALLCHAR_LINEHEIGHT)) / 2;
	for (j = 0; (control = list[j]); j++, y += (SMALLCHAR_LINEHEIGHT) )
	{
		control->x = CONTROL_X;
		control->y = y;
		control->left   = CONTROL_X - s_preferences.max_control_text * BASEFONT_INDENT;
		control->right  = CONTROL_X + 21*BASEFONT_INDENT;
		control->top    = y;
		control->bottom = y + BASEFONT_HEIGHT;
	}

	// set correct flags on tab controls
	s_preferences.hud.generic.flags |= (QMF_PULSEIFFOCUS);
	s_preferences.render.generic.flags |= (QMF_PULSEIFFOCUS);
	s_preferences.objects.generic.flags |= (QMF_PULSEIFFOCUS);
	s_preferences.misc.generic.flags |= (QMF_PULSEIFFOCUS);

	s_preferences.hud.generic.flags &= ~ (QMF_GRAYED|QMF_HIGHLIGHT|QMF_HIGHLIGHT_IF_FOCUS);
	s_preferences.render.generic.flags &= ~ (QMF_GRAYED|QMF_HIGHLIGHT|QMF_HIGHLIGHT_IF_FOCUS);
	s_preferences.objects.generic.flags &= ~ (QMF_GRAYED|QMF_HIGHLIGHT|QMF_HIGHLIGHT_IF_FOCUS);
	s_preferences.misc.generic.flags &= ~ (QMF_GRAYED|QMF_HIGHLIGHT|QMF_HIGHLIGHT_IF_FOCUS);

	switch (s_preferences.section) {
	case C_HUD:
		s_preferences.hud.generic.flags &= ~(QMF_PULSEIFFOCUS);
		s_preferences.hud.generic.flags |= (QMF_HIGHLIGHT|QMF_HIGHLIGHT_IF_FOCUS);
		break;

	case C_RENDER:
		s_preferences.render.generic.flags &= ~(QMF_PULSEIFFOCUS);
		s_preferences.render.generic.flags |= (QMF_HIGHLIGHT|QMF_HIGHLIGHT_IF_FOCUS);
		break;

	case C_OBJECTS:
		s_preferences.objects.generic.flags &= ~(QMF_PULSEIFFOCUS);
		s_preferences.objects.generic.flags |= (QMF_HIGHLIGHT|QMF_HIGHLIGHT_IF_FOCUS);
		break;

	case C_MISC:
		s_preferences.misc.generic.flags &= ~(QMF_PULSEIFFOCUS);
		s_preferences.misc.generic.flags |= (QMF_HIGHLIGHT|QMF_HIGHLIGHT_IF_FOCUS);
		break;
	}
}

/*
=================
Control_Init
=================
*/
static void Control_Init( void )
{
	int i, j, len, max;
	menucommon_s **list;
	menucommon_s *control;

	// count longest string title and save it
	max = 0;
	for (i = 0; i < C_MAX; i++)
	{
		list = g_control_list[i];
		for (j = 0; (control = list[j]); j++)
		{
			if (control->name) {
				len = strlenru(control->name) + 1;
				if (len > max)
					max = len;
			}
		}
	}

	s_preferences.max_control_text = max + 1;

	// prepare current tab
	s_preferences.section = C_HUD;
	Control_Update();
}

static void Preferences_InputEvent( void* ptr, int notification ) {
	if( notification != QM_LOSTFOCUS ) {
		return;
	}

	switch( ((menucommon_s*)ptr)->id ) {

	case ID_TEAMCHATHEIGHT:
		trap_Cmd_ExecuteText( EXEC_APPEND, va("seta com_maxfps %s\n", s_preferences.teamchatheight.field.buffer));
		break;

	case ID_FOV:
		if (atoi(s_preferences.fov.field.buffer) <= 0) {
			Q_strncpyz(s_preferences.fov.field.buffer, "90", 3);
		}
		trap_Cvar_Set( "cg_fov", s_preferences.fov.field.buffer);
		break;

	case ID_ZOOMFOV:
		if (atoi(s_preferences.zoomfov.field.buffer) <= 0) {
			Q_strncpyz(s_preferences.zoomfov.field.buffer, "22", 3);
		}
		trap_Cvar_Set( "cg_zoomfov", s_preferences.zoomfov.field.buffer);
		break;
	}

}

static void Preferences_Event( void* ptr, int notification ) {
	if( notification != QM_ACTIVATED ) {
		return;
	}

	switch( ((menucommon_s*)ptr)->id ) {
	case ID_HUD:
		s_preferences.section = C_HUD;
		Control_Update();
		break;

	case ID_RENDER:
		s_preferences.section = C_RENDER;
		Control_Update();
		break;

	case ID_OBJECTS:
		s_preferences.section = C_OBJECTS;
		Control_Update();
		break;

	case ID_MISC:
		s_preferences.section = C_MISC;
		Control_Update();
		break;

	case ID_CROSSHAIR:
		s_preferences.crosshair.curvalue++;
		if( s_preferences.crosshair.curvalue == NUM_CROSSHAIRS ) {
			s_preferences.crosshair.curvalue = 0;
		}
		trap_Cvar_SetValue( "cg_drawCrosshair", s_preferences.crosshair.curvalue );
		break;

        case ID_COLORRED:
                trap_Cvar_SetValue( "cg_crosshairColorRed", ((float)s_preferences.crosshairColorRed.curvalue)/255.f );
                break;

        case ID_COLORGREEN:
                trap_Cvar_SetValue( "cg_crosshairColorGreen", ((float)s_preferences.crosshairColorGreen.curvalue)/255.f );
                break;

        case ID_COLORBLUE:
                trap_Cvar_SetValue( "cg_crosshairColorBlue", ((float)s_preferences.crosshairColorBlue.curvalue)/255.f );
                break;

                
        case ID_WEAPONBAR:
		trap_Cvar_SetValue( "cg_alwaysWeaponBar", s_preferences.alwaysweaponbar.curvalue );
		break;

	case ID_HIGHQUALITYSKY:
		trap_Cvar_SetValue( "r_fastsky", !s_preferences.highqualitysky.curvalue );
		break;

	case ID_DYNAMICLIGHTS:
		trap_Cvar_SetValue( "r_dynamiclight", s_preferences.dynamiclights.curvalue );
		break;		

	case ID_IDENTIFYTARGET:
		trap_Cvar_SetValue( "cg_drawCrosshairNames", s_preferences.identifytarget.curvalue );
		break;

	case ID_SYNCEVERYFRAME:
		trap_Cvar_SetValue( "r_swapInterval", s_preferences.synceveryframe.curvalue );
		break;

	case ID_ALLOWDOWNLOAD:
		trap_Cvar_SetValue( "cl_allowDownload", s_preferences.allowdownload.curvalue );
		trap_Cvar_SetValue( "sv_allowDownload", s_preferences.allowdownload.curvalue );
		break;

	case ID_BOTMENU:
		trap_Cvar_SetValue( "gui_autoclosebotmenu", s_preferences.botmenu.curvalue );
		break;

	case ID_BLOOD:
		trap_Cvar_SetValue( "com_blood", s_preferences.blood.curvalue );
		break;

	case ID_DRAWFPS:
		trap_Cvar_SetValue( "cg_drawFPS", s_preferences.drawfps.curvalue );
		break;

	case ID_DRAWTIMER:
		trap_Cvar_SetValue( "cg_drawTimer", s_preferences.drawtimer.curvalue );
		break;

	case ID_LAGOMETER:
		trap_Cvar_SetValue( "cg_lagometer", s_preferences.drawlagometer.curvalue );
		break;

	case ID_SHADOWS:
		trap_Cvar_SetValue( "cg_shadows", s_preferences.shadows.curvalue );
		break;

	case ID_BACK:
		UI_PopMenu();
		break;
	}
}

/*
=================
Crosshair_Draw
=================
*/
static void Crosshair_Draw( void *self ) {
	menuelement_s	*s;
	float		*color;
	int			x, y;
	int			style;
	qboolean	focus;
        vec4_t          color4;

	s = (menuelement_s *)self;
	x = s->generic.x;
	y =	s->generic.y;

	style = UI_SMALLFONT;
	focus = (s->generic.parent->cursor == s->generic.menuPosition);

	if ( s->generic.flags & QMF_GRAYED )
		color = color_disabled;
	else if ( focus )
	{
		color = color_highlight;
		style |= UI_PULSE;
	}
	else if ( s->generic.flags & QMF_BLINK )
	{
		color = color_highlight;
		style |= UI_BLINK;
	}
	else
		color = color_white;

	if ( focus ){
		// draw cursor
		ST_DrawChar( x, y, 13, UI_CENTER|UI_BLINK|UI_SMALLFONT, color, 1.00);
	}

	ST_DrawString( x - BASEFONT_INDENT, y, s->string, style|UI_RIGHT, color, 1.00 );
	if( !s->curvalue ) {
		return;
	}
        color4[0]=((float)s_preferences.crosshairColorRed.curvalue)/255.f;
        color4[1]=((float)s_preferences.crosshairColorGreen.curvalue)/255.f;
        color4[2]=((float)s_preferences.crosshairColorBlue.curvalue)/255.f;
        color4[3]=1.0f;
	trap_R_SetColor( color4 );
	UI_DrawHandlePic( x + BASEFONT_INDENT, y - 4, 24, 24, s_preferences.crosshairShader[s->curvalue] );
}

/*
=================
Preferences_Statusbar
=================
*/
static void Preferences_Statusbar(void* self)
{
	char* s;

if(cl_language.integer == 0){
	switch( ((menucommon_s*)self)->id ) {
		case ID_SYNCEVERYFRAME:
			s = "on = game waits for OpenGL calls to finish";
			break;
		case ID_FOV:
			s = "Field of view angle (default = 90)";
			break;
		case ID_ZOOMFOV:
			s = "Zoomed view angle";
			break;
		case ID_SIMPLEITEMS:
			s = "Picture instead of 3D item";
			break;
		case ID_HIGHQUALITYSKY:
			s = "off = no portal view of destination as well";
			break;
		case ID_TEAMCHATHEIGHT:
			s = "FPS limit - it is recommended to set a supported value";
			break;
		case ID_PREDICTITEMS:
			s = "Item pickup predicted";
			break;
	}
}
if(cl_language.integer == 1){
	switch( ((menucommon_s*)self)->id ) {
		case ID_SYNCEVERYFRAME:
			s = "вкл = игра ожидает вызов завершения OpenGL";
			break;
		case ID_FOV:
			s = "Поле зрения (стандарт = 90)";
			break;
		case ID_ZOOMFOV:
			s = "Поле зрения в приближении";
			break;
		case ID_SIMPLEITEMS:
			s = "Картинка вместо 3D предмета";
			break;
		case ID_HIGHQUALITYSKY:
			s = "выкл = нету визуализированых порталов";
			break;
		case ID_TEAMCHATHEIGHT:
			s = "Лимит FPS - рекомендуется ставить как у монитора";
			break;
		case ID_PREDICTITEMS:
			s = "Предметы на стороне клиента";
			break;
	}
}

	if (s)
		ST_DrawString(320, 480 - 64, s, UI_CENTER|UI_SMALLFONT, color_white, 1.00);
}

static void Preferences_MenuInit( void )
{
    UI_SetDefaultCvar("cg_crosshairColorRed","1");
    UI_SetDefaultCvar("cg_crosshairColorBlue","1");
    UI_SetDefaultCvar("cg_crosshairColorGreen","1");

	memset( &s_preferences, 0 ,sizeof(preferences_t) );

	Preferences_Cache();

	s_preferences.menu.fullscreen = qtrue;

	s_preferences.banner.generic.type  = MTYPE_TEXT;
	s_preferences.banner.generic.x	   = 320;
	s_preferences.banner.generic.y	   = 16;
	s_preferences.banner.color         = color_white;
	s_preferences.banner.style         = UI_CENTER;

	s_preferences.hud.generic.type     = MTYPE_PTEXT;
	s_preferences.hud.generic.flags    = QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_preferences.hud.generic.id	    = ID_HUD;
	s_preferences.hud.generic.callback	= Preferences_Event;
	s_preferences.hud.generic.x	    = 152 - uis.wideoffset;
	s_preferences.hud.generic.y	    = 240 - 2 * PROP_HEIGHT;
	s_preferences.hud.style			= UI_RIGHT;
	s_preferences.hud.color			= color_white;

	s_preferences.render.generic.type     = MTYPE_PTEXT;
	s_preferences.render.generic.flags    = QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_preferences.render.generic.id	    = ID_RENDER;
	s_preferences.render.generic.callback	= Preferences_Event;
	s_preferences.render.generic.x	    = 152 - uis.wideoffset;
	s_preferences.render.generic.y	    = 240 - PROP_HEIGHT;
	s_preferences.render.style			= UI_RIGHT;
	s_preferences.render.color			= color_white;

	s_preferences.objects.generic.type     = MTYPE_PTEXT;
	s_preferences.objects.generic.flags    = QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_preferences.objects.generic.id	    = ID_OBJECTS;
	s_preferences.objects.generic.callback	= Preferences_Event;
	s_preferences.objects.generic.x	    = 152 - uis.wideoffset;
	s_preferences.objects.generic.y	    = 240;
	s_preferences.objects.style			= UI_RIGHT;
	s_preferences.objects.color			= color_white;

	s_preferences.misc.generic.type     = MTYPE_PTEXT;
	s_preferences.misc.generic.flags    = QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_preferences.misc.generic.id	    = ID_MISC;
	s_preferences.misc.generic.callback	= Preferences_Event;
	s_preferences.misc.generic.x	    = 152 - uis.wideoffset;
	s_preferences.misc.generic.y	    = 240 + PROP_HEIGHT;
	s_preferences.misc.style			= UI_RIGHT;
	s_preferences.misc.color			= color_white;

	s_preferences.back.generic.type	    = MTYPE_BITMAP;
	s_preferences.back.string     = ART_BACK0;
	s_preferences.back.generic.flags    = QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_preferences.back.generic.callback = Preferences_Event;
	s_preferences.back.generic.id	    = ID_BACK;
	s_preferences.back.generic.x		= 0 - uis.wideoffset;
	s_preferences.back.generic.y		= 480-64;
	s_preferences.back.width  		    = 128;
	s_preferences.back.height  		    = 64;
	s_preferences.back.focuspic         = ART_BACK1;


	// tabbed controls
	s_preferences.crosshair.generic.type		= MTYPE_TEXT;
	s_preferences.crosshair.generic.flags		= QMF_PULSEIFFOCUS|QMF_SMALLFONT|QMF_NODEFAULTINIT|QMF_OWNERDRAW;
	s_preferences.crosshair.generic.callback	= Preferences_Event;
	s_preferences.crosshair.generic.ownerdraw	= Crosshair_Draw;
	s_preferences.crosshair.generic.id			= ID_CROSSHAIR;

    s_preferences.crosshairColorRed.generic.type		= MTYPE_SLIDER;
	s_preferences.crosshairColorRed.generic.flags		= QMF_PULSEIFFOCUS|QMF_SMALLFONT|QMF_NODEFAULTINIT|QMF_OWNERDRAW;
	s_preferences.crosshairColorRed.generic.callback	= Preferences_Event;
	s_preferences.crosshairColorRed.generic.id		= ID_COLORRED;
	s_preferences.crosshairColorRed.minvalue			= 0.0f;
	s_preferences.crosshairColorRed.maxvalue			= 255.0f;

    s_preferences.crosshairColorGreen.generic.type		= MTYPE_SLIDER;
	s_preferences.crosshairColorGreen.generic.flags		= QMF_PULSEIFFOCUS|QMF_SMALLFONT|QMF_NODEFAULTINIT|QMF_OWNERDRAW;
	s_preferences.crosshairColorGreen.generic.callback	= Preferences_Event;
	s_preferences.crosshairColorGreen.generic.id		= ID_COLORGREEN;
	s_preferences.crosshairColorGreen.minvalue			= 0.0f;
	s_preferences.crosshairColorGreen.maxvalue			= 255.0f;

    s_preferences.crosshairColorBlue.generic.type		= MTYPE_SLIDER;
	s_preferences.crosshairColorBlue.generic.flags		= QMF_PULSEIFFOCUS|QMF_SMALLFONT|QMF_NODEFAULTINIT|QMF_OWNERDRAW;
	s_preferences.crosshairColorBlue.generic.callback	= Preferences_Event;
	s_preferences.crosshairColorBlue.generic.id		= ID_COLORBLUE;
	s_preferences.crosshairColorBlue.minvalue			= 0.0f;
	s_preferences.crosshairColorBlue.maxvalue			= 255.0f;

    //Elimination
	s_preferences.alwaysweaponbar.generic.type        = MTYPE_RADIOBUTTON;
	s_preferences.alwaysweaponbar.generic.flags		= QMF_PULSEIFFOCUS|QMF_SMALLFONT|QMF_NODEFAULTINIT|QMF_OWNERDRAW;
	s_preferences.alwaysweaponbar.generic.callback    = Preferences_Event;
	s_preferences.alwaysweaponbar.generic.id          = ID_WEAPONBAR;

	s_preferences.simpleitems.generic.type        = MTYPE_RADIOBUTTON;
	s_preferences.simpleitems.generic.flags	      = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_preferences.simpleitems.generic.callback    = Preferences_Event;
	s_preferences.simpleitems.generic.statusbar= Preferences_Statusbar;
	s_preferences.simpleitems.generic.id          = ID_SIMPLEITEMS;

	s_preferences.wallmarks.generic.type          = MTYPE_RADIOBUTTON;
	s_preferences.wallmarks.generic.flags	      = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_preferences.wallmarks.generic.callback      = Preferences_Event;
	s_preferences.wallmarks.generic.id            = ID_WALLMARKS;

	s_preferences.brass.generic.type              = MTYPE_RADIOBUTTON;
	s_preferences.brass.generic.flags	          = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_preferences.brass.generic.callback          = Preferences_Event;
	s_preferences.brass.generic.id                = ID_EJECTINGBRASS;

	s_preferences.dynamiclights.generic.type      = MTYPE_RADIOBUTTON;
	s_preferences.dynamiclights.generic.flags     = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_preferences.dynamiclights.generic.callback  = Preferences_Event;
	s_preferences.dynamiclights.generic.id        = ID_DYNAMICLIGHTS;

	s_preferences.highqualitysky.generic.type     = MTYPE_RADIOBUTTON;
	s_preferences.highqualitysky.generic.flags	  = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_preferences.highqualitysky.generic.callback = Preferences_Event;
	s_preferences.highqualitysky.generic.statusbar= Preferences_Statusbar;
	s_preferences.highqualitysky.generic.id       = ID_HIGHQUALITYSKY;

	s_preferences.shadows.generic.type     = MTYPE_SPINCONTROL;
	s_preferences.shadows.generic.flags	   = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_preferences.shadows.generic.callback = Preferences_Event;
	s_preferences.shadows.generic.id       = ID_SHADOWS;

	s_preferences.gibs.generic.type     = MTYPE_RADIOBUTTON;
	s_preferences.gibs.generic.flags	   = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_preferences.gibs.generic.callback = Preferences_Event;
	s_preferences.gibs.generic.id       = ID_GIBS;

	s_preferences.blood.generic.type     = MTYPE_RADIOBUTTON;
	s_preferences.blood.generic.flags	   = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_preferences.blood.generic.callback = Preferences_Event;
	s_preferences.blood.generic.id       = ID_BLOOD;

	s_preferences.identifytarget.generic.type     = MTYPE_RADIOBUTTON;
	s_preferences.identifytarget.generic.flags    = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_preferences.identifytarget.generic.callback = Preferences_Event;
	s_preferences.identifytarget.generic.id       = ID_IDENTIFYTARGET;

	s_preferences.drawfps.generic.type     = MTYPE_RADIOBUTTON;
	s_preferences.drawfps.generic.flags	   = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_preferences.drawfps.generic.callback = Preferences_Event;
	s_preferences.drawfps.generic.id       = ID_DRAWFPS;

	s_preferences.drawtimer.generic.type     = MTYPE_RADIOBUTTON;
	s_preferences.drawtimer.generic.flags	   = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_preferences.drawtimer.generic.callback = Preferences_Event;
	s_preferences.drawtimer.generic.id       = ID_DRAWTIMER;

	s_preferences.drawlagometer.generic.type     = MTYPE_RADIOBUTTON;
	s_preferences.drawlagometer.generic.flags	   = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_preferences.drawlagometer.generic.callback = Preferences_Event;
	s_preferences.drawlagometer.generic.id       = ID_LAGOMETER;

	s_preferences.drawgun.generic.type     = MTYPE_RADIOBUTTON;
	s_preferences.drawgun.generic.flags	   = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_preferences.drawgun.generic.callback = Preferences_Event;
	s_preferences.drawgun.generic.id       = ID_DRAWGUN;

	s_preferences.teamchatheight.generic.type       = MTYPE_FIELD;
	s_preferences.teamchatheight.generic.id		= ID_TEAMCHATHEIGHT;
	s_preferences.teamchatheight.generic.callback	= Preferences_InputEvent;
	s_preferences.teamchatheight.generic.statusbar= Preferences_Statusbar;
	s_preferences.teamchatheight.generic.flags      = QMF_SMALLFONT|QMF_PULSEIFFOCUS|QMF_NUMBERSONLY;
	s_preferences.teamchatheight.field.widthInChars = 3;
	s_preferences.teamchatheight.field.maxchars     = 3;

	s_preferences.fov.generic.type       = MTYPE_FIELD;
	s_preferences.fov.generic.id		= ID_FOV;
	s_preferences.fov.generic.callback	= Preferences_InputEvent;
	s_preferences.fov.generic.statusbar= Preferences_Statusbar;
	s_preferences.fov.generic.flags      = QMF_SMALLFONT|QMF_PULSEIFFOCUS|QMF_NUMBERSONLY;
	s_preferences.fov.field.widthInChars = 3;
	s_preferences.fov.field.maxchars     = 3;

	s_preferences.zoomfov.generic.type       = MTYPE_FIELD;
	s_preferences.zoomfov.generic.id		= ID_ZOOMFOV;
	s_preferences.zoomfov.generic.callback	= Preferences_InputEvent;
	s_preferences.zoomfov.generic.statusbar= Preferences_Statusbar;
	s_preferences.zoomfov.generic.flags      = QMF_SMALLFONT|QMF_PULSEIFFOCUS|QMF_NUMBERSONLY;
	s_preferences.zoomfov.field.widthInChars = 3;
	s_preferences.zoomfov.field.maxchars     = 3;

	s_preferences.predictitems.generic.type     = MTYPE_RADIOBUTTON;
	s_preferences.predictitems.generic.flags	   = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_preferences.predictitems.generic.callback = Preferences_Event;
	s_preferences.predictitems.generic.statusbar= Preferences_Statusbar;
	s_preferences.predictitems.generic.id       = ID_PREDICTITEMS;

	s_preferences.synceveryframe.generic.type     = MTYPE_RADIOBUTTON;
	s_preferences.synceveryframe.generic.flags	  = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_preferences.synceveryframe.generic.callback = Preferences_Event;
	s_preferences.synceveryframe.generic.id       = ID_SYNCEVERYFRAME;

	s_preferences.allowdownload.generic.type     = MTYPE_RADIOBUTTON;
	s_preferences.allowdownload.generic.flags	   = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_preferences.allowdownload.generic.callback = Preferences_Event;
	s_preferences.allowdownload.generic.id       = ID_ALLOWDOWNLOAD;

	s_preferences.botmenu.generic.type     = MTYPE_RADIOBUTTON;
	s_preferences.botmenu.generic.flags	  = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_preferences.botmenu.generic.callback = Preferences_Event;
	s_preferences.botmenu.generic.id       = ID_BOTMENU;
	
if(cl_language.integer == 0){
s_preferences.banner.string		   = "GAME OPTIONS";
s_preferences.hud.string			= "HUD";
s_preferences.render.string			= "RENDER";
s_preferences.objects.string			= "OBJECTS";
s_preferences.misc.string			= "MISC";
s_preferences.crosshair.string		= "Crosshair:";
s_preferences.crosshairColorRed.string		= "Color red:";
s_preferences.crosshairColorGreen.string		= "Color green:";
s_preferences.crosshairColorBlue.string		= "Color blue:";
s_preferences.alwaysweaponbar.string	      = "Always show weapons:";
s_preferences.simpleitems.string	      = "Simple Items:";
s_preferences.wallmarks.string	      = "Marks on Walls:";
s_preferences.brass.string	          = "Ejecting Brass:";
s_preferences.dynamiclights.string	  = "Dynamic Lights:";
s_preferences.highqualitysky.string	  = "High Quality Sky:";
s_preferences.shadows.string	   = "Shadow type:";
s_preferences.gibs.string	   = "Body Gibs:";
s_preferences.blood.string	   = "Blood:";
s_preferences.identifytarget.string	  = "Identify Target:";
s_preferences.drawfps.string	   = "Draw FPS:";
s_preferences.drawtimer.string	   = "Draw Timer:";
s_preferences.drawlagometer.string	   = "Draw Lagometer:";
s_preferences.drawgun.string	   = "Secret setting =P:";
s_preferences.teamchatheight.string       = "FPS Limit:";
s_preferences.fov.string       = "FOV:";
s_preferences.zoomfov.string       = "Zoom FOV:";
s_preferences.predictitems.string	   = "Predict Items:";
s_preferences.synceveryframe.string	  = "V-Sync:";
s_preferences.allowdownload.string	   = "Automatic Downloading:";
s_preferences.botmenu.string	  = "AutoClose Bot Menu:";
s_preferences.shadows.itemnames			= shadow_types;
}
if(cl_language.integer == 1){
s_preferences.banner.string		   = "ИГРОВЫЕ ОПЦИИ";
s_preferences.hud.string			= "HUD";
s_preferences.render.string			= "РЕНДЕР";
s_preferences.objects.string			= "ОБЪЕКТЫ";
s_preferences.misc.string			= "ДРУГОЕ";
s_preferences.crosshair.string		= "Прицел:";
s_preferences.crosshairColorRed.string		= "Цвет красный:";
s_preferences.crosshairColorGreen.string		= "Цвет зеленый:";
s_preferences.crosshairColorBlue.string		= "Цвет синий:";
s_preferences.alwaysweaponbar.string	      = "Всегда показывать оружие:";
s_preferences.simpleitems.string	      = "Простые предметы:";
s_preferences.wallmarks.string	      = "Отметки на стенах:";
s_preferences.brass.string	          = "Гильзы от пуль:";
s_preferences.dynamiclights.string	  = "Динамическое освещение:";
s_preferences.highqualitysky.string	  = "Небо высокого качества:";
s_preferences.shadows.string	   = "Тип теней:";
s_preferences.gibs.string	   = "Куски тел:";
s_preferences.blood.string	   = "Кровь:";
s_preferences.identifytarget.string	  = "Определить цель:";
s_preferences.drawfps.string	   = "Отображение FPS:";
s_preferences.drawtimer.string	   = "Отображение Времени:";
s_preferences.drawlagometer.string	   = "Отображение Сети:";
s_preferences.drawgun.string	   = "Секретная настройка =P:";
s_preferences.teamchatheight.string       = "Лимит FPS:";
s_preferences.fov.string       = "Поле зрения:";
s_preferences.zoomfov.string       = "Увеличение:";
s_preferences.predictitems.string	   = "Предметы клиент:";
s_preferences.synceveryframe.string	  = "Вертикальная синхронизация:";
s_preferences.allowdownload.string	   = "Автоматическое скачивание:";
s_preferences.botmenu.string	  = "Авто закрытие Бот Меню:";
s_preferences.shadows.itemnames			= shadow_typesru;
}

	Menu_AddItem( &s_preferences.menu, &s_preferences.banner );

	Menu_AddItem( &s_preferences.menu, &s_preferences.hud);
	Menu_AddItem( &s_preferences.menu, &s_preferences.render);
	Menu_AddItem( &s_preferences.menu, &s_preferences.objects);
	Menu_AddItem( &s_preferences.menu, &s_preferences.misc);

	Menu_AddItem( &s_preferences.menu, &s_preferences.crosshair );
    Menu_AddItem( &s_preferences.menu, &s_preferences.crosshairColorRed );
    Menu_AddItem( &s_preferences.menu, &s_preferences.crosshairColorGreen );
    Menu_AddItem( &s_preferences.menu, &s_preferences.crosshairColorBlue );
	Menu_AddItem( &s_preferences.menu, &s_preferences.simpleitems );
    Menu_AddItem( &s_preferences.menu, &s_preferences.alwaysweaponbar );
	Menu_AddItem( &s_preferences.menu, &s_preferences.wallmarks );
	Menu_AddItem( &s_preferences.menu, &s_preferences.brass );
	Menu_AddItem( &s_preferences.menu, &s_preferences.dynamiclights );
	Menu_AddItem( &s_preferences.menu, &s_preferences.identifytarget );
	Menu_AddItem( &s_preferences.menu, &s_preferences.highqualitysky );
	Menu_AddItem( &s_preferences.menu, &s_preferences.synceveryframe );
	Menu_AddItem( &s_preferences.menu, &s_preferences.allowdownload );
	Menu_AddItem( &s_preferences.menu, &s_preferences.botmenu);

	Menu_AddItem( &s_preferences.menu, &s_preferences.gibs);
	Menu_AddItem( &s_preferences.menu, &s_preferences.blood);
	Menu_AddItem( &s_preferences.menu, &s_preferences.drawfps);
	Menu_AddItem( &s_preferences.menu, &s_preferences.drawtimer);
	Menu_AddItem( &s_preferences.menu, &s_preferences.drawlagometer);
	Menu_AddItem( &s_preferences.menu, &s_preferences.predictitems);
	Menu_AddItem( &s_preferences.menu, &s_preferences.shadows);
	Menu_AddItem( &s_preferences.menu, &s_preferences.drawgun);
	Menu_AddItem( &s_preferences.menu, &s_preferences.teamchatheight);
	Menu_AddItem( &s_preferences.menu, &s_preferences.fov);
	Menu_AddItem( &s_preferences.menu, &s_preferences.zoomfov);

	Menu_AddItem( &s_preferences.menu, &s_preferences.back );

	Preferences_SetMenuItems();
	Control_Init();
}


/*
===============
Preferences_Cache
===============
*/
void Preferences_Cache( void ) {
	int		n;

	trap_R_RegisterShaderNoMip( ART_BACK0 );
	trap_R_RegisterShaderNoMip( ART_BACK1 );
	for( n = 0; n < NUM_CROSSHAIRS; n++ ) {
		s_preferences.crosshairShader[n] = trap_R_RegisterShaderNoMip( va("gfx/2d/crosshair%c", 'a' + n ) );
	}
}


/*
===============
UI_PreferencesMenu
===============
*/
void UI_PreferencesMenu( void ) {
	Preferences_MenuInit();
	UI_PushMenu( &s_preferences.menu );
}
