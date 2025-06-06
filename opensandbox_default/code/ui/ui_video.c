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

void GraphicsOptions_MenuInit( void );

/*
=======================================================================

DRIVER INFORMATION MENU

=======================================================================
*/

#define DRIVERINFO_BACK0	"menu/assets/back_0"
#define DRIVERINFO_BACK1	"menu/assets/back_1"

static char* driverinfo_artlist[] = 
{
	DRIVERINFO_BACK0,
	DRIVERINFO_BACK1,
	NULL,
};

#define ID_DRIVERINFOBACK	100

typedef struct
{
	menuframework_s	menu;
	menuelement_s		banner;
	menuelement_s	back;
	char			stringbuff[1024];
	char*			strings[64];
	int				numstrings;
} driverinfo_t;

static driverinfo_t	s_driverinfo;

/*
=================
DriverInfo_Event
=================
*/
static void DriverInfo_Event( void* ptr, int event )
{
	if (event != QM_ACTIVATED)
		return;

	switch (((menucommon_s*)ptr)->id)
	{
		case ID_DRIVERINFOBACK:
			UI_PopMenu();
			break;
	}
}

/*
=================
DriverInfo_MenuDraw
=================
*/
static void DriverInfo_MenuDraw( void )
{
	int	i;
	int	y;

	Menu_Draw( &s_driverinfo.menu );

	ST_DrawString( 320, 80, "VENDOR", UI_CENTER|UI_SMALLFONT, color_white, 1.00 );
	ST_DrawString( 320, 152, "PIXELFORMAT", UI_CENTER|UI_SMALLFONT, color_white, 1.00 );
	ST_DrawString( 320, 192, "EXTENSIONS", UI_CENTER|UI_SMALLFONT, color_white, 1.00 );

	ST_DrawString( 320, 80+16, glconfig.vendor_string, UI_CENTER|UI_SMALLFONT, color_white, 1.00 );
	ST_DrawString( 320, 96+16, glconfig.version_string, UI_CENTER|UI_SMALLFONT, color_white, 1.00 );
	ST_DrawString( 320, 112+16, glconfig.renderer_string, UI_CENTER|UI_SMALLFONT, color_white, 1.00 );
	ST_DrawString( 320, 152+16, va ("color(%d-bits) Z(%d-bits) stencil(%d-bits)", glconfig.colorBits, glconfig.depthBits, glconfig.stencilBits), UI_CENTER|UI_SMALLFONT, color_white, 1.00 );

	// double column
	y = 192+16;
	for (i=0; i<s_driverinfo.numstrings/2; i++) {
		ST_DrawString( 320-4, y, s_driverinfo.strings[i*2], UI_RIGHT|UI_SMALLFONT, color_white, 1.00 );
		ST_DrawString( 320+4, y, s_driverinfo.strings[i*2+1], UI_LEFT|UI_SMALLFONT, color_white, 1.00 );
		y += BASEFONT_HEIGHT;
	}

	if (s_driverinfo.numstrings & 1)
		ST_DrawString( 320, y, s_driverinfo.strings[s_driverinfo.numstrings-1], UI_CENTER|UI_SMALLFONT, color_white, 1.00 );
}

/*
=================
DriverInfo_Cache
=================
*/
void DriverInfo_Cache( void )
{
	int	i;

	// touch all our pics
	for (i=0; ;i++)
	{
		if (!driverinfo_artlist[i])
			break;
		trap_R_RegisterShaderNoMip(driverinfo_artlist[i]);
	}
}

/*
=================
UI_DriverInfo_Menu
=================
*/
static void UI_DriverInfo_Menu( void )
{
	char*	eptr;
	int		i;
	int		len;

	// zero set all our globals
	memset( &s_driverinfo, 0 ,sizeof(driverinfo_t) );

	DriverInfo_Cache();

	s_driverinfo.menu.fullscreen = qtrue;
	s_driverinfo.menu.draw       = DriverInfo_MenuDraw;

	s_driverinfo.banner.generic.type  = MTYPE_TEXT;
	s_driverinfo.banner.generic.x	  = 320;
	s_driverinfo.banner.generic.y	  = 16;
	s_driverinfo.banner.color	      = color_white;
	s_driverinfo.banner.style	      = UI_CENTER;

	s_driverinfo.back.generic.type	   = MTYPE_BITMAP;
	s_driverinfo.back.string     = DRIVERINFO_BACK0;
	s_driverinfo.back.generic.flags    = QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_driverinfo.back.generic.callback = DriverInfo_Event;
	s_driverinfo.back.generic.id	   = ID_DRIVERINFOBACK;
	s_driverinfo.back.generic.x		   = 0 - uis.wideoffset;
	s_driverinfo.back.generic.y		   = 480-64;
	s_driverinfo.back.width  		   = 128;
	s_driverinfo.back.height  		   = 64;
	s_driverinfo.back.focuspic         = DRIVERINFO_BACK1;

  // TTimo: overflow with particularly long GL extensions (such as the gf3)
  // https://zerowing.idsoftware.com/bugzilla/show_bug.cgi?id=399
  // NOTE: could have pushed the size of stringbuff, but the list is already out of the screen
  // (no matter what your resolution)
  Q_strncpyz(s_driverinfo.stringbuff, glconfig.extensions_string, 1024);

	// build null terminated extension strings
	eptr = s_driverinfo.stringbuff;
	while ( s_driverinfo.numstrings<40 && *eptr )
	{
		while ( *eptr == ' ' )
			*eptr++ = '\0';

		// track start of valid string
		if (*eptr && *eptr != ' ')
			s_driverinfo.strings[s_driverinfo.numstrings++] = eptr;

		while ( *eptr && *eptr != ' ' )
			eptr++;
	}

	// safety length strings for display
	for (i=0; i<s_driverinfo.numstrings; i++) {
		len = strlen(s_driverinfo.strings[i]);
		if (len > 32) {
			s_driverinfo.strings[i][len-1] = '>';
			s_driverinfo.strings[i][len]   = '\0';
		}
	}

	Menu_AddItem( &s_driverinfo.menu, &s_driverinfo.banner );
	Menu_AddItem( &s_driverinfo.menu, &s_driverinfo.back );

	UI_PushMenu( &s_driverinfo.menu );
}

/*
=======================================================================

GRAPHICS OPTIONS MENU

=======================================================================
*/
#define GRAPHICSOPTIONS_BACK0	"menu/assets/back_0"
#define GRAPHICSOPTIONS_BACK1	"menu/assets/back_1"
#define GRAPHICSOPTIONS_ACCEPT0	"menu/assets/accept_0"
#define GRAPHICSOPTIONS_ACCEPT1	"menu/assets/accept_1"

#define ID_BACK2		101
#define ID_FULLSCREEN	102
#define ID_LIST			103
#define ID_MODE			104
#define ID_DRIVERINFO	105
#define ID_GRAPHICS		106
#define ID_DISPLAY		107
#define ID_SOUND		108
#define ID_NETWORK		109
#define ID_RATIO                110

typedef struct {
	menuframework_s	menu;

	menuelement_s		banner;

	menuelement_s		graphics;
	menuelement_s		display;
	menuelement_s		sound;
	menuelement_s		network;

	menuelement_s		list;
        menuelement_s              ratio;
	menuelement_s		mode;
	menuelement_s		detail;
	menuelement_s	tq;
	menuelement_s  	fs;
	menuelement_s  	envlevel;
        menuelement_s  	postfx;
	menuelement_s  	allow_extensions;
	menuelement_s  	texturebits;
	menuelement_s  	bloomlevel;
	menuelement_s  	filter;
        menuelement_s  	aniso;
        menuelement_s  	aniso2;
	menuelement_s  	hdr;
	menuelement_s		driverinfo;

	menuelement_s	apply;
	menuelement_s	back;
} graphicsoptions_t;

typedef struct
{
	int mode;
	qboolean fullscreen;
	int tq;
	int envlevel;
	qboolean postfx;
	qboolean hdr;
	int texturebits;
	int bloomlevel;
	int filter;
        int aniso;
        int aniso2;
	int detail;
	qboolean extensions;
} InitialVideoOptions_s;

static InitialVideoOptions_s	s_ivo;
static graphicsoptions_t		s_graphicsoptions;	

static const char *builtinResolutions[ ] =
{
"320x240",
"400x300",
"512x384",
"640x480",
"800x600",
"960x720",
"1024x768",
"1152x864",
"1280x1024",
"1600x1200",
"2048x1536",
"856x480",
"1280x960",
"1280x720",
"1280x800",
"1366x768",
"1440x900",
"1600x900",
"1680x1050",
"1920x1080",
"1920x1200",
"2560x1080",
"3440x1440",
"3840x2160",
"4096x2160",
 NULL
};

static const char *knownRatios[ ][2] =
{
        { "1.25:1", "5:4"   },
        { "1.33:1", "4:3"   },
        { "1.50:1", "3:2"   },
        { "1.56:1", "14:9"  },
        { "1.60:1", "16:10" },
        { "1.67:1", "5:3"   },
        { "1.78:1", "16:9"  },
        { NULL    , NULL    }
};

#define MAX_RESOLUTIONS 32

static const char* ratios[ MAX_RESOLUTIONS ];
static char ratioBuf[ MAX_RESOLUTIONS ][ 8 ];
static int ratioToRes[ MAX_RESOLUTIONS ];
static int resToRatio[ MAX_RESOLUTIONS ];

static char resbuf[ MAX_STRING_CHARS ];
static const char* detectedResolutions[ MAX_RESOLUTIONS ];
static char currentResolution[ 20 ];

static const char** resolutions = builtinResolutions;

/*
=================
GraphicsOptions_GetAspectRatios
=================
*/
static void GraphicsOptions_GetAspectRatios( void )
{
    int i, r;

    // build ratio list from resolutions
    for( r = 0; resolutions[r]; r++ )
    {
        int w, h;
        char *x;
        char str[ sizeof(ratioBuf[0]) ];

        // calculate resolution's aspect ratio
        x = strchr( resolutions[r], 'x' )+1;
        
        
        Q_strncpyz( str, resolutions[r], x-resolutions[r] );
        w = atoi( str );
        h = atoi( x );
        Com_sprintf( str, sizeof(str), "%.2f:1", (float)w / (float)h );
        
        // rename common ratios ("1.33:1" -> "4:3")
        for( i = 0; knownRatios[i][0]; i++ ) {
            if( Q_strequal( str, knownRatios[i][0] ) ) {
                Q_strncpyz( str, knownRatios[i][1], sizeof( str ) );
                break;
            }
        }

        // add ratio to list if it is new
        // establish res/ratio relationship
        for( i = 0; ratioBuf[i][0]; i++ )
        {
            if( Q_strequal( str, ratioBuf[i] ) )
                break;
        }
        if( !ratioBuf[i][0] )
        {
            Q_strncpyz( ratioBuf[i], str, sizeof(ratioBuf[i]) );
            ratioToRes[i] = r;
        }
        ratios[r] = ratioBuf[r];
        resToRatio[r] = i;
    }
    ratios[r] = NULL;
}

/*
=================
GraphicsOptions_GetInitialVideo
=================
*/
static void GraphicsOptions_GetInitialVideo( void )
{
	s_ivo.detail      = s_graphicsoptions.detail.curvalue;
	s_ivo.mode        = s_graphicsoptions.mode.curvalue;
	s_ivo.fullscreen  = s_graphicsoptions.fs.curvalue;
	s_ivo.extensions  = s_graphicsoptions.allow_extensions.curvalue;
	s_ivo.tq          = s_graphicsoptions.tq.curvalue;
	s_ivo.envlevel    = s_graphicsoptions.envlevel.curvalue;
	s_ivo.postfx      = s_graphicsoptions.postfx.curvalue;
	s_ivo.hdr     = s_graphicsoptions.hdr.curvalue;
	s_ivo.bloomlevel    = s_graphicsoptions.bloomlevel.curvalue;
	s_ivo.filter      = s_graphicsoptions.filter.curvalue;
        s_ivo.aniso      = s_graphicsoptions.aniso.curvalue;
        s_ivo.aniso2      = s_graphicsoptions.aniso2.curvalue;
	s_ivo.texturebits = s_graphicsoptions.texturebits.curvalue;
}

/*
=================
GraphicsOptions_UpdateMenuItems
=================
*/
static void GraphicsOptions_UpdateMenuItems( void )
{		
	if ( s_graphicsoptions.allow_extensions.curvalue == 0 )
	{
		if ( s_graphicsoptions.texturebits.curvalue == 0 )
		{
			s_graphicsoptions.texturebits.curvalue = 1;
		}
	}

	s_graphicsoptions.apply.generic.flags |= QMF_HIDDEN|QMF_INACTIVE;

	if ( s_ivo.mode != s_graphicsoptions.mode.curvalue )
	{
		s_graphicsoptions.apply.generic.flags &= ~(QMF_HIDDEN|QMF_INACTIVE);
	}
	if ( s_ivo.fullscreen != s_graphicsoptions.fs.curvalue )
	{
		s_graphicsoptions.apply.generic.flags &= ~(QMF_HIDDEN|QMF_INACTIVE);
	}
	if ( s_ivo.extensions != s_graphicsoptions.allow_extensions.curvalue )
	{
		s_graphicsoptions.apply.generic.flags &= ~(QMF_HIDDEN|QMF_INACTIVE);
	}
	if ( s_ivo.tq != s_graphicsoptions.tq.curvalue )
	{
		s_graphicsoptions.apply.generic.flags &= ~(QMF_HIDDEN|QMF_INACTIVE);
	}
	if ( s_ivo.envlevel != s_graphicsoptions.envlevel.curvalue )
	{
		s_graphicsoptions.apply.generic.flags &= ~(QMF_HIDDEN|QMF_INACTIVE);
	}
        if ( s_ivo.postfx != s_graphicsoptions.postfx.curvalue )
	{
		s_graphicsoptions.apply.generic.flags &= ~(QMF_HIDDEN|QMF_INACTIVE);
	}
	if ( s_ivo.hdr != s_graphicsoptions.hdr.curvalue )
	{
		s_graphicsoptions.apply.generic.flags &= ~(QMF_HIDDEN|QMF_INACTIVE);
	}
	if ( s_ivo.detail != s_graphicsoptions.detail.curvalue )
	{
		s_graphicsoptions.apply.generic.flags &= ~(QMF_HIDDEN|QMF_INACTIVE);
	}
	if ( s_ivo.texturebits != s_graphicsoptions.texturebits.curvalue )
	{
		s_graphicsoptions.apply.generic.flags &= ~(QMF_HIDDEN|QMF_INACTIVE);
	}
	if ( s_ivo.bloomlevel != s_graphicsoptions.bloomlevel.curvalue )
	{
		s_graphicsoptions.apply.generic.flags &= ~(QMF_HIDDEN|QMF_INACTIVE);
	}
	if ( s_ivo.filter != s_graphicsoptions.filter.curvalue )
	{
		s_graphicsoptions.apply.generic.flags &= ~(QMF_HIDDEN|QMF_INACTIVE);
	}
        if ( s_ivo.aniso != s_graphicsoptions.aniso.curvalue )
	{
		s_graphicsoptions.apply.generic.flags &= ~(QMF_HIDDEN|QMF_INACTIVE);
	}
        if ( s_ivo.aniso2 != s_graphicsoptions.aniso2.curvalue )
	{
		s_graphicsoptions.apply.generic.flags &= ~(QMF_HIDDEN|QMF_INACTIVE);
	}
}	

/*
=================
GraphicsOptions_ApplyChanges
=================
*/
static void GraphicsOptions_ApplyChanges( void *unused, int notification )
{
	if (notification != QM_ACTIVATED)
		return;

	switch ( s_graphicsoptions.texturebits.curvalue  )
	{
	case 0:
		trap_Cvar_SetValue( "r_texturebits", 0 );
		break;
	case 1:
		trap_Cvar_SetValue( "r_texturebits", 16 );
		break;
	case 2:
		trap_Cvar_SetValue( "r_texturebits", 32 );
		break;
	}

	switch ( s_graphicsoptions.detail.curvalue  )
	{
	case 0:
		trap_Cvar_SetValue( "r_subdivisions", 16 );
		trap_Cvar_SetValue( "r_lodBias", 2 );
		break;
	case 1:
		trap_Cvar_SetValue( "r_subdivisions", 8 );
		trap_Cvar_SetValue( "r_lodBias", 1 );
		break;
	case 2:
		trap_Cvar_SetValue( "r_subdivisions", 4 );
		trap_Cvar_SetValue( "r_lodBias", 0 );
		break;
	case 3:
		trap_Cvar_SetValue( "r_subdivisions", 2 );
		trap_Cvar_SetValue( "r_lodBias", -1 );
		break;
	case 4:
		trap_Cvar_SetValue( "r_subdivisions", 1 );
		trap_Cvar_SetValue( "r_lodBias", -2 );
		break;
	}
	trap_Cvar_SetValue( "r_picmip", 3 - s_graphicsoptions.tq.curvalue );
	trap_Cvar_SetValue( "r_allowExtensions", s_graphicsoptions.allow_extensions.curvalue );

	if(s_graphicsoptions.fs.curvalue == 0){
	trap_Cvar_SetValue( "r_fullscreen", 0 );
	trap_Cvar_SetValue( "r_mode", s_graphicsoptions.mode.curvalue );
	} else {
	trap_Cvar_SetValue( "r_fullscreen", 1 );
	trap_Cvar_SetValue( "r_mode", -2 );
	}

	if(s_graphicsoptions.envlevel.curvalue == 0){
		trap_Cvar_SetValue( "ui_effectslevel", 0 );
		trap_Cvar_SetValue( "cg_atmosphericLevel", 1 );
		trap_Cvar_SetValue( "cg_effectsTime", 10 );
		trap_Cvar_SetValue( "cg_effectsLimit", 4096 );
		trap_Cvar_SetValue( "cg_effectsGibs", 1 );
	}
	if(s_graphicsoptions.envlevel.curvalue == 1){
		trap_Cvar_SetValue( "ui_effectslevel", 1 );
		trap_Cvar_SetValue( "cg_atmosphericLevel", 1 );
		trap_Cvar_SetValue( "cg_effectsTime", 30 );
		trap_Cvar_SetValue( "cg_effectsLimit", 5120 );
		trap_Cvar_SetValue( "cg_effectsGibs", 3 );
	}
	if(s_graphicsoptions.envlevel.curvalue == 2){
		trap_Cvar_SetValue( "ui_effectslevel", 2 );
		trap_Cvar_SetValue( "cg_atmosphericLevel", 2 );
		trap_Cvar_SetValue( "cg_effectsTime", 60 );
		trap_Cvar_SetValue( "cg_effectsLimit", 6144 );
		trap_Cvar_SetValue( "cg_effectsGibs", 6 );
	}
	if(s_graphicsoptions.envlevel.curvalue == 3){
		trap_Cvar_SetValue( "ui_effectslevel", 3 );
		trap_Cvar_SetValue( "cg_atmosphericLevel", 2 );
		trap_Cvar_SetValue( "cg_effectsTime", 90 );
		trap_Cvar_SetValue( "cg_effectsLimit", 7168 );
		trap_Cvar_SetValue( "cg_effectsGibs", 3 );
	}
	if(s_graphicsoptions.envlevel.curvalue == 4){
		trap_Cvar_SetValue( "ui_effectslevel", 4 );
		trap_Cvar_SetValue( "cg_atmosphericLevel", 2 );
		trap_Cvar_SetValue( "cg_effectsTime", 300 );
		trap_Cvar_SetValue( "cg_effectsLimit", 8192 );
		trap_Cvar_SetValue( "cg_effectsGibs", 9 );
	}
	if(s_graphicsoptions.envlevel.curvalue == 5){
		trap_Cvar_SetValue( "ui_effectslevel", 5 );
		trap_Cvar_SetValue( "cg_atmosphericLevel", 2 );
		trap_Cvar_SetValue( "cg_effectsTime", 600 );
		trap_Cvar_SetValue( "cg_effectsLimit", 8192 );
		trap_Cvar_SetValue( "cg_effectsGibs", 16 );
	}
	trap_Cvar_SetValue( "r_postfx", s_graphicsoptions.postfx.curvalue );
	trap_Cvar_SetValue( "r_hdr", s_graphicsoptions.hdr.curvalue );

	//r_ext_texture_filter_anisotropic is special
	if(s_graphicsoptions.aniso.curvalue) {
		trap_Cvar_SetValue( "r_ext_max_anisotropy", s_graphicsoptions.aniso.curvalue*2 );
		trap_Cvar_SetValue( "r_ext_texture_filter_anisotropic", qtrue );
	}
	else {
		trap_Cvar_SetValue( "r_ext_texture_filter_anisotropic", qfalse );
	}
	
	//r_ext_texture_filter_anisotropic is special
	trap_Cvar_SetValue( "r_ext_multisample", s_graphicsoptions.aniso2.curvalue*2 );

	if ( s_graphicsoptions.bloomlevel.curvalue == 2 )
	{
		trap_Cvar_SetValue( "r_bloom_intensity", 0.15 );
	}
	else if ( s_graphicsoptions.bloomlevel.curvalue == 1 )
	{
		trap_Cvar_SetValue( "r_bloom_intensity", 0.10 );
	}
	else
	{
		trap_Cvar_SetValue( "r_bloom_intensity", 0.05 );
	}

	if ( s_graphicsoptions.filter.curvalue )
	{
		trap_Cvar_Set( "r_textureMode", "GL_LINEAR_MIPMAP_LINEAR" );
	}
	else
	{
		trap_Cvar_Set( "r_textureMode", "GL_NEAREST_MIPMAP_NEAREST" );
	}

	trap_Cmd_ExecuteText( EXEC_APPEND, "vid_restart\n" );
}

/*
=================
GraphicsOptions_Event
=================
*/
static void GraphicsOptions_Event( void* ptr, int event ) {
	InitialVideoOptions_s *ivo;

	if( event != QM_ACTIVATED ) {
	 	return;
	}

	switch( ((menucommon_s*)ptr)->id ) {
        case ID_RATIO:
            s_graphicsoptions.mode.curvalue = ratioToRes[ s_graphicsoptions.ratio.curvalue ];
            // fall through to apply mode constraints
	case ID_MODE:
        s_graphicsoptions.ratio.curvalue = resToRatio[ s_graphicsoptions.mode.curvalue ];
		break;

	case ID_LIST:
		trap_Cvar_SetValue("r_fx_filmic", s_graphicsoptions.list.curvalue);
		break;

	case ID_DRIVERINFO:
		UI_DriverInfo_Menu();
		break;

	case ID_BACK2:
		UI_PopMenu();
		break;

	case ID_GRAPHICS:
		break;

	case ID_DISPLAY:
		UI_PopMenu();
		UI_DisplayOptionsMenu();
		break;

	case ID_SOUND:
		UI_PopMenu();
		UI_SoundOptionsMenu();
		break;

	case ID_NETWORK:
		UI_PopMenu();
		UI_NetworkOptionsMenu();
		break;
	}
}


/*
================
GraphicsOptions_TQEvent
================
*/
static void GraphicsOptions_TQEvent( void *ptr, int event ) {
	if( event != QM_ACTIVATED ) {
	 	return;
	}
	s_graphicsoptions.tq.curvalue = (int)(s_graphicsoptions.tq.curvalue + 0.5);
}


/*
================
GraphicsOptions_MenuDraw
================
*/
void GraphicsOptions_MenuDraw (void)
{
//APSFIX - rework this
	GraphicsOptions_UpdateMenuItems();

	Menu_Draw( &s_graphicsoptions.menu );
}

/*
=================
GraphicsOptions_SetMenuItems
=================
*/
static void GraphicsOptions_SetMenuItems( void )
{
	s_graphicsoptions.mode.curvalue = trap_Cvar_VariableValue( "r_mode" );

	if ( s_graphicsoptions.mode.curvalue < 0 )
	{
		s_graphicsoptions.mode.curvalue = 3;
	}
	if(trap_Cvar_VariableValue("r_fullscreen") == 1){
		s_graphicsoptions.fs.curvalue = 1;
	} else {
		s_graphicsoptions.fs.curvalue = 0;
	}
	s_graphicsoptions.allow_extensions.curvalue = trap_Cvar_VariableValue("r_allowExtensions");
    s_graphicsoptions.postfx.curvalue = trap_Cvar_VariableValue("r_postfx");
    s_graphicsoptions.hdr.curvalue = trap_Cvar_VariableValue("r_hdr");
    if(trap_Cvar_VariableValue("r_ext_texture_filter_anisotropic")) {
        s_graphicsoptions.aniso.curvalue = trap_Cvar_VariableValue("r_ext_max_anisotropy")/2;
    }
    s_graphicsoptions.aniso2.curvalue = trap_Cvar_VariableValue("r_ext_multisample")/2;
	s_graphicsoptions.tq.curvalue = 3-trap_Cvar_VariableValue( "r_picmip");
	if ( s_graphicsoptions.tq.curvalue < 0 )
	{
		s_graphicsoptions.tq.curvalue = 0;
	}
	else if ( s_graphicsoptions.tq.curvalue > 3 )
	{
		s_graphicsoptions.tq.curvalue = 3;
	}

	s_graphicsoptions.envlevel.curvalue = ( int ) trap_Cvar_VariableValue( "ui_effectslevel" );

	switch ( ( int ) trap_Cvar_VariableValue( "r_texturebits" ) )
	{
	default:
	case 0:
		s_graphicsoptions.texturebits.curvalue = 0;
		break;
	case 16:
		s_graphicsoptions.texturebits.curvalue = 1;
		break;
	case 32:
		s_graphicsoptions.texturebits.curvalue = 2;
		break;
	}

	switch ( ( int ) trap_Cvar_VariableValue( "r_subdivisions" ) )
	{
	default:
	case 16:
		s_graphicsoptions.detail.curvalue = 0;
		break;
	case 8:
		s_graphicsoptions.detail.curvalue = 1;
		break;
	case 4:
		s_graphicsoptions.detail.curvalue = 2;
		break;
	case 2:
		s_graphicsoptions.detail.curvalue = 3;
		break;
	case 1:
		s_graphicsoptions.detail.curvalue = 4;
		break;
	}

	if ( Q_strequal( UI_Cvar_VariableString( "r_textureMode" ), "GL_NEAREST_MIPMAP_NEAREST" ) )
	{
		s_graphicsoptions.filter.curvalue = 0;
	}
	else
	{
		s_graphicsoptions.filter.curvalue = 1;
	}
	
	if ( trap_Cvar_VariableValue( "r_bloom_intensity" ) == 0.15 )
	{
		s_graphicsoptions.bloomlevel.curvalue = 2;
	}
	else if ( trap_Cvar_VariableValue( "r_bloom_intensity" ) == 0.10 )
	{
		s_graphicsoptions.bloomlevel.curvalue = 1;
	}
	else
	{
		s_graphicsoptions.bloomlevel.curvalue = 0;
	}
}

/*
================
GraphicsOptions_MenuInit
================
*/
void GraphicsOptions_MenuInit( void )
{
	static const char *s_detail_names[] =
	{
		"Lowest",
		"Low",
		"Medium",
		"High",
		"Ultra",
		NULL
	};

	static const char *s_detail_namesru[] =
	{
		"Низчайший",
		"Низкий",
		"Средний",
		"Высокий",
		"Ультра",
		NULL
	};

	static const char *tq_names[] =
	{
		"Default",
		"16 bit",
		"32 bit",
		NULL
	};
	static const char *tq_namesru[] =
	{
		"Стандарт",
		"16 бит",
		"32 бит",
		NULL
	};

	static const char *s_graphics_options_names[] =
	{
		"Off",
		"On",
		NULL
	};
	static const char *s_graphics_options_namesru[] =
	{
		"Откл",
		"Вкл",
		NULL
	};

	static const char *envlevel_names[] =
	{
		"Very low",
		"Low",
		"Medium",
		"High",
		"Very High",
		"Ultra",
		NULL
	};
	static const char *envlevel_namesru[] =
	{
		"Очень низкий",
		"Низкий",
		"Средний",
		"Высокий",
		"Очень высокий",
		"Ультра",
		NULL
	};


	static const char *filter_names[] =
	{
		"Off",
		"Trilinear",
		NULL
	};
	static const char *filter_namesru[] =
	{
		"Откл",
		"Трилинейная",
		NULL
	};
        
    static const char *aniso_names[] =
	{
		"Off",
		"2x",
        "4x",
        "6x",
        "8x",
		NULL
	};
	static const char *aniso_namesru[] =
	{
		"Откл",
		"2x",
        "4x",
        "6x",
        "8x",
		NULL
	};
        
	static const char *quality_names[] =
	{
		"Low",
		"Medium",
		"High",
		NULL
	};
	static const char *quality_namesru[] =
	{
		"Низкое",
		"Среднее",
		"Высокое",
		NULL
	};
	static const char *enabled_names[] =
	{
		"Off",
		"On",
		NULL
	};
	static const char *enabled_namesru[] =
	{
		"Откл",
		"Вкл",
		NULL
	};

	int y;

	// zero set all our globals
	memset( &s_graphicsoptions, 0 ,sizeof(graphicsoptions_t) );

	GraphicsOptions_GetAspectRatios();

	GraphicsOptions_Cache();

	s_graphicsoptions.menu.fullscreen = qtrue;
	s_graphicsoptions.menu.draw       = GraphicsOptions_MenuDraw;

	s_graphicsoptions.banner.generic.type  = MTYPE_TEXT;
	s_graphicsoptions.banner.generic.x	   = 320;
	s_graphicsoptions.banner.generic.y	   = 16;
	s_graphicsoptions.banner.color         = color_white;
	s_graphicsoptions.banner.style         = UI_CENTER;

	s_graphicsoptions.graphics.generic.type		= MTYPE_PTEXT;
	s_graphicsoptions.graphics.generic.flags	= QMF_RIGHT_JUSTIFY;
	s_graphicsoptions.graphics.generic.id		= ID_GRAPHICS;
	s_graphicsoptions.graphics.generic.callback	= GraphicsOptions_Event;
	s_graphicsoptions.graphics.generic.x		= 140 - uis.wideoffset;
	s_graphicsoptions.graphics.generic.y		= 240 - 2 * PROP_HEIGHT;
	s_graphicsoptions.graphics.style			= UI_RIGHT;
	s_graphicsoptions.graphics.color			= color_grey;

	s_graphicsoptions.display.generic.type		= MTYPE_PTEXT;
	s_graphicsoptions.display.generic.flags		= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_graphicsoptions.display.generic.id		= ID_DISPLAY;
	s_graphicsoptions.display.generic.callback	= GraphicsOptions_Event;
	s_graphicsoptions.display.generic.x			= 140 - uis.wideoffset;
	s_graphicsoptions.display.generic.y			= 240 - PROP_HEIGHT;
	s_graphicsoptions.display.style				= UI_RIGHT;
	s_graphicsoptions.display.color				= color_white;

	s_graphicsoptions.sound.generic.type		= MTYPE_PTEXT;
	s_graphicsoptions.sound.generic.flags		= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_graphicsoptions.sound.generic.id			= ID_SOUND;
	s_graphicsoptions.sound.generic.callback	= GraphicsOptions_Event;
	s_graphicsoptions.sound.generic.x			= 140 - uis.wideoffset;
	s_graphicsoptions.sound.generic.y			= 240;
	s_graphicsoptions.sound.style				= UI_RIGHT;
	s_graphicsoptions.sound.color				= color_white;

	s_graphicsoptions.network.generic.type		= MTYPE_PTEXT;
	s_graphicsoptions.network.generic.flags		= QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_graphicsoptions.network.generic.id		= ID_NETWORK;
	s_graphicsoptions.network.generic.callback	= GraphicsOptions_Event;
	s_graphicsoptions.network.generic.x			= 140 - uis.wideoffset;
	s_graphicsoptions.network.generic.y			= 240 + PROP_HEIGHT;
	s_graphicsoptions.network.style				= UI_RIGHT;
	s_graphicsoptions.network.color				= color_white;

	y = 240 - 7 * (BIGCHAR_HEIGHT + 2);
	s_graphicsoptions.list.generic.type     = MTYPE_SPINCONTROL;
	s_graphicsoptions.list.generic.flags    = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_graphicsoptions.list.generic.x        = 400;
	s_graphicsoptions.list.generic.y        = y;
	s_graphicsoptions.list.generic.callback = GraphicsOptions_Event;
	s_graphicsoptions.list.generic.id       = ID_LIST;
	y += 2 * ( BIGCHAR_HEIGHT + 2 );

	s_graphicsoptions.detail.generic.type  = MTYPE_SPINCONTROL;
	s_graphicsoptions.detail.generic.flags = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_graphicsoptions.detail.generic.x     = 400;
	s_graphicsoptions.detail.generic.y     = y;
	y += BIGCHAR_HEIGHT+2;

	// references/modifies "r_allowExtensions"
	s_graphicsoptions.allow_extensions.generic.type     = MTYPE_SPINCONTROL;
	s_graphicsoptions.allow_extensions.generic.flags	= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_graphicsoptions.allow_extensions.generic.x	    = 400;
	s_graphicsoptions.allow_extensions.generic.y	    = y;
	y += BIGCHAR_HEIGHT+2;

        s_graphicsoptions.ratio.generic.type     = MTYPE_SPINCONTROL;
        s_graphicsoptions.ratio.generic.flags    = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
        s_graphicsoptions.ratio.generic.x        = 400;
        s_graphicsoptions.ratio.generic.y        = y;
        s_graphicsoptions.ratio.itemnames        = ratios;
        s_graphicsoptions.ratio.generic.callback = GraphicsOptions_Event;
        s_graphicsoptions.ratio.generic.id       = ID_RATIO;
        y += BIGCHAR_HEIGHT+2;


	// references/modifies "r_mode"
	s_graphicsoptions.mode.generic.type     = MTYPE_SPINCONTROL;
	s_graphicsoptions.mode.generic.flags    = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_graphicsoptions.mode.generic.x        = 400;
	s_graphicsoptions.mode.generic.y        = y;
	s_graphicsoptions.mode.itemnames        = resolutions;
	s_graphicsoptions.mode.generic.callback = GraphicsOptions_Event;
	s_graphicsoptions.mode.generic.id       = ID_MODE;
	y += BIGCHAR_HEIGHT+2;

	// references/modifies "r_fullscreen"
	s_graphicsoptions.fs.generic.type     = MTYPE_SPINCONTROL;
	s_graphicsoptions.fs.generic.flags	  = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_graphicsoptions.fs.generic.x	      = 400;
	s_graphicsoptions.fs.generic.y	      = y;
	y += BIGCHAR_HEIGHT+2;

	// references/modifies "ui_effectslevel
	s_graphicsoptions.envlevel.generic.type  = MTYPE_SPINCONTROL;
	s_graphicsoptions.envlevel.generic.flags = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_graphicsoptions.envlevel.generic.x	 = 400;
	s_graphicsoptions.envlevel.generic.y	 = y;
	y += BIGCHAR_HEIGHT+2;
        
    // references/modifies "r_postfx"
	s_graphicsoptions.postfx.generic.type     = MTYPE_SPINCONTROL;
	s_graphicsoptions.postfx.generic.flags	  = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_graphicsoptions.postfx.generic.x	      = 400;
	s_graphicsoptions.postfx.generic.y	      = y;
	y += BIGCHAR_HEIGHT+2;

	s_graphicsoptions.hdr.generic.type	  = MTYPE_SPINCONTROL;
	s_graphicsoptions.hdr.generic.flags	  = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_graphicsoptions.hdr.generic.x	      = 400;
	s_graphicsoptions.hdr.generic.y	      = y;
	y += BIGCHAR_HEIGHT+2;

	// references/modifies "r_bloomIntensity"
	s_graphicsoptions.bloomlevel.generic.type  = MTYPE_SPINCONTROL;
	s_graphicsoptions.bloomlevel.generic.flags = QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_graphicsoptions.bloomlevel.generic.x	 = 400;
	s_graphicsoptions.bloomlevel.generic.y	 = y;
	y += BIGCHAR_HEIGHT+2;

	// references/modifies "r_picmip"
	s_graphicsoptions.tq.generic.type	= MTYPE_SLIDER;
	s_graphicsoptions.tq.generic.flags	= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_graphicsoptions.tq.generic.x		= 400;
	s_graphicsoptions.tq.generic.y		= y;
	s_graphicsoptions.tq.minvalue       = 0;
	s_graphicsoptions.tq.maxvalue       = 3;
	s_graphicsoptions.tq.generic.callback = GraphicsOptions_TQEvent;
	y += BIGCHAR_HEIGHT+2;

	// references/modifies "r_textureBits"
	s_graphicsoptions.texturebits.generic.type  = MTYPE_SPINCONTROL;
	s_graphicsoptions.texturebits.generic.flags	= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_graphicsoptions.texturebits.generic.x	    = 400;
	s_graphicsoptions.texturebits.generic.y	    = y;
	y += BIGCHAR_HEIGHT+2;

	// references/modifies "r_textureMode"
	s_graphicsoptions.filter.generic.type   = MTYPE_SPINCONTROL;
	s_graphicsoptions.filter.generic.flags	= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_graphicsoptions.filter.generic.x	    = 400;
	s_graphicsoptions.filter.generic.y	    = y;
	y += 2+BIGCHAR_HEIGHT;
        
    s_graphicsoptions.aniso.generic.type   = MTYPE_SPINCONTROL;
	s_graphicsoptions.aniso.generic.flags	= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_graphicsoptions.aniso.generic.x	    = 400;
	s_graphicsoptions.aniso.generic.y	    = y;
	y += 2*BIGCHAR_HEIGHT;
	
    s_graphicsoptions.aniso2.generic.type   = MTYPE_SPINCONTROL;
	s_graphicsoptions.aniso2.generic.flags	= QMF_PULSEIFFOCUS|QMF_SMALLFONT;
	s_graphicsoptions.aniso2.generic.x	    = 400;
	s_graphicsoptions.aniso2.generic.y	    = y;
	y += 2*BIGCHAR_HEIGHT;

	s_graphicsoptions.driverinfo.generic.type     = MTYPE_PTEXT;
	s_graphicsoptions.driverinfo.generic.flags    = QMF_CENTER_JUSTIFY|QMF_PULSEIFFOCUS;
	s_graphicsoptions.driverinfo.generic.callback = GraphicsOptions_Event;
	s_graphicsoptions.driverinfo.generic.id       = ID_DRIVERINFO;
	s_graphicsoptions.driverinfo.generic.x        = 320;
	s_graphicsoptions.driverinfo.generic.y        = y;
	s_graphicsoptions.driverinfo.style            = UI_CENTER|UI_SMALLFONT;
	s_graphicsoptions.driverinfo.color            = color_white;
	y += BIGCHAR_HEIGHT+2;

	s_graphicsoptions.back.generic.type	    = MTYPE_BITMAP;
	s_graphicsoptions.back.string     = GRAPHICSOPTIONS_BACK0;
	s_graphicsoptions.back.generic.flags    = QMF_LEFT_JUSTIFY|QMF_PULSEIFFOCUS;
	s_graphicsoptions.back.generic.callback = GraphicsOptions_Event;
	s_graphicsoptions.back.generic.id	    = ID_BACK2;
	s_graphicsoptions.back.generic.x		= 0 - uis.wideoffset;
	s_graphicsoptions.back.generic.y		= 480-64;
	s_graphicsoptions.back.width  		    = 128;
	s_graphicsoptions.back.height  		    = 64;
	s_graphicsoptions.back.focuspic         = GRAPHICSOPTIONS_BACK1;

	s_graphicsoptions.apply.generic.type     = MTYPE_BITMAP;
	s_graphicsoptions.apply.string     = GRAPHICSOPTIONS_ACCEPT0;
	s_graphicsoptions.apply.generic.flags    = QMF_RIGHT_JUSTIFY|QMF_PULSEIFFOCUS|QMF_HIDDEN|QMF_INACTIVE;
	s_graphicsoptions.apply.generic.callback = GraphicsOptions_ApplyChanges;
	s_graphicsoptions.apply.generic.x        = 640 + uis.wideoffset;
	s_graphicsoptions.apply.generic.y        = 480-64;
	s_graphicsoptions.apply.width  		     = 128;
	s_graphicsoptions.apply.height  		 = 64;
	s_graphicsoptions.apply.focuspic         = GRAPHICSOPTIONS_ACCEPT1;
	
	if(cl_language.integer == 0){
	s_driverinfo.banner.string		  = "DRIVER INFO";
	s_graphicsoptions.banner.string  	   = "SYSTEM SETUP";
	s_graphicsoptions.graphics.string			= "GRAPHICS";
	s_graphicsoptions.display.string			= "DISPLAY";
	s_graphicsoptions.sound.string				= "SOUND";
	s_graphicsoptions.network.string			= "NETWORK";
	s_graphicsoptions.list.string     = "Realistic rendering:";
	s_graphicsoptions.list.itemnames        = s_graphics_options_names;
	s_graphicsoptions.detail.string  = "Level of Detail:";
	s_graphicsoptions.detail.itemnames     = s_detail_names;
	s_graphicsoptions.allow_extensions.string	    = "GL Extensions:";
	s_graphicsoptions.allow_extensions.itemnames        = enabled_names;
	s_graphicsoptions.ratio.string     = "Aspect Ratio:";
	s_graphicsoptions.mode.string     = "Window resolution:";
	s_graphicsoptions.fs.string	  = "Fullscreen:";
	s_graphicsoptions.fs.itemnames	      = enabled_names;
	s_graphicsoptions.envlevel.string	 = "Effects level:";
	s_graphicsoptions.envlevel.itemnames     = envlevel_names;
	s_graphicsoptions.postfx.string	  = "Post-processing:";
	s_graphicsoptions.postfx.itemnames	      = enabled_names;
	s_graphicsoptions.hdr.string	  = "HDR:";
	s_graphicsoptions.hdr.itemnames	      = enabled_names;
	s_graphicsoptions.bloomlevel.string	 = "Bloom level:";
	s_graphicsoptions.bloomlevel.itemnames     = quality_names;
	s_graphicsoptions.tq.string	= "Texture Detail:";
	s_graphicsoptions.texturebits.string	= "Texture Quality:";
	s_graphicsoptions.texturebits.itemnames     = tq_names;
	s_graphicsoptions.filter.string	= "Texture Filter:";
	s_graphicsoptions.filter.itemnames      = filter_names;
	s_graphicsoptions.aniso.string	= "Anisotropy:";
	s_graphicsoptions.aniso.itemnames      = aniso_names;
	s_graphicsoptions.aniso2.string	= "Anti-Aliasing:";
	s_graphicsoptions.aniso2.itemnames      = aniso_names;
	s_graphicsoptions.driverinfo.string           = "Driver Info";	
	}
	if(cl_language.integer == 1){
	s_driverinfo.banner.string		  = "ИНФОРМАЦИЯ О ДРАЙВЕРЕ";
	s_graphicsoptions.banner.string  	   = "СИСТЕМНЫЕ НАСТРОЙКИ";
	s_graphicsoptions.graphics.string			= "ГРАФИКА";
	s_graphicsoptions.display.string			= "ЭКРАН";
	s_graphicsoptions.sound.string				= "ЗВУК";
	s_graphicsoptions.network.string			= "СЕТЬ";
	s_graphicsoptions.list.string     = "Реалистичный рендеринг:";
	s_graphicsoptions.list.itemnames        = s_graphics_options_namesru;
	s_graphicsoptions.detail.string  = "Уровень детализации:";
	s_graphicsoptions.detail.itemnames     = s_detail_namesru;
	s_graphicsoptions.allow_extensions.string	    = "GL Расширения:";
	s_graphicsoptions.allow_extensions.itemnames        = enabled_namesru;
	s_graphicsoptions.ratio.string     = "Соотношение Сторон:";
	s_graphicsoptions.mode.string     = "Разрешение окна:";
	s_graphicsoptions.fs.string	  = "Полный экран:";
	s_graphicsoptions.fs.itemnames	      = enabled_namesru;
	s_graphicsoptions.envlevel.string	 = "Уровень эффектов:";
	s_graphicsoptions.envlevel.itemnames     = envlevel_namesru;
	s_graphicsoptions.postfx.string	  = "Пост-обработка:";
	s_graphicsoptions.postfx.itemnames	      = enabled_namesru;
	s_graphicsoptions.hdr.string	  = "HDR:";
	s_graphicsoptions.hdr.itemnames	      = enabled_namesru;
	s_graphicsoptions.bloomlevel.string	 = "Сила свечения:";
	s_graphicsoptions.bloomlevel.itemnames     = quality_namesru;
	s_graphicsoptions.tq.string	= "Детализация текстур:";
	s_graphicsoptions.texturebits.string	= "Качество текстур:";
	s_graphicsoptions.texturebits.itemnames     = tq_namesru;
	s_graphicsoptions.filter.string	= "Текстурный фильтр:";
	s_graphicsoptions.filter.itemnames      = filter_namesru;
	s_graphicsoptions.aniso.string	= "Анизотропная фильтрация:";
	s_graphicsoptions.aniso.itemnames      = aniso_namesru;
	s_graphicsoptions.aniso2.string	= "Сглаживание:";
	s_graphicsoptions.aniso2.itemnames      = aniso_namesru;
	s_graphicsoptions.driverinfo.string           = "Информация о драйвере";	
	}

	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.banner );

	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.graphics );
	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.display );
	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.sound );
	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.network );

	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.list );
	s_graphicsoptions.list.curvalue = get_cvar_int("r_fx_filmic");
	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.detail );
	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.allow_extensions );
    Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.ratio );
	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.mode );
	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.fs );
	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.envlevel );
    Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.postfx );
	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.hdr );
	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.bloomlevel );
	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.tq );
	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.texturebits );
	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.filter );
    Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.aniso );
    Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.aniso2 );
	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.driverinfo );

	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.back );
	Menu_AddItem( &s_graphicsoptions.menu, ( void * ) &s_graphicsoptions.apply );

	GraphicsOptions_SetMenuItems();
	GraphicsOptions_GetInitialVideo();

}


/*
=================
GraphicsOptions_Cache
=================
*/
void GraphicsOptions_Cache( void ) {
	trap_R_RegisterShaderNoMip( GRAPHICSOPTIONS_BACK0 );
	trap_R_RegisterShaderNoMip( GRAPHICSOPTIONS_BACK1 );
	trap_R_RegisterShaderNoMip( GRAPHICSOPTIONS_ACCEPT0 );
	trap_R_RegisterShaderNoMip( GRAPHICSOPTIONS_ACCEPT1 );
}


/*
=================
UI_GraphicsOptionsMenu
=================
*/
void UI_GraphicsOptionsMenu( void ) {
	GraphicsOptions_MenuInit();
	UI_PushMenu( &s_graphicsoptions.menu );
	Menu_SetCursorToItem( &s_graphicsoptions.menu, &s_graphicsoptions.graphics );
}
