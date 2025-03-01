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
// cg_info.c -- display information while data is being loading

#include "cg_local.h"

#define MAX_LOADING_PLAYER_ICONS	64
#define MAX_LOADING_ITEM_ICONS		256

static int			loadingPlayerIconCount;
static int			loadingItemIconCount;
static qhandle_t	loadingPlayerIcons[MAX_LOADING_PLAYER_ICONS];
static qhandle_t	loadingItemIcons[MAX_LOADING_ITEM_ICONS];

/*
======================
CG_LoadingString

======================
*/
void CG_LoadingString( const char *s, float value ) {
	if(cl_language.integer == 0){
		Q_strncpyz( cg.infoScreenText, va("Loading... %s", s), sizeof( cg.infoScreenText ) );
	}
	if(cl_language.integer == 1){
		Q_strncpyz( cg.infoScreenText, va("Загрузка... %s", s), sizeof( cg.infoScreenText ) );
	}
	if(value != -1){
	cg.infoScreenValue = value;
	}

	trap_UpdateScreen();
}

/*
===================
CG_LoadingItem
===================
*/
void CG_LoadingItem( int itemNum ) {
	gitem_t		*item;

	item = &bg_itemlist[itemNum];
	
	if(cl_language.integer == 0){
	CG_LoadingString( item->pickup_name, -1 );
	}
	if(cl_language.integer == 1){
	CG_LoadingString( item->pickup_nameru, -1 );
	}
}

/*
===================
CG_LoadingClient
===================
*/
void CG_LoadingClient( int clientNum ) {
	const char		*info;
	char			*skin;
	char			personality[MAX_QPATH];
	char			model[MAX_QPATH];
	char			iconName[MAX_QPATH];

	info = CG_ConfigString( CS_PLAYERS + clientNum );

	if ( loadingPlayerIconCount < MAX_LOADING_PLAYER_ICONS ) {
		Q_strncpyz( model, Info_ValueForKey( info, "model" ), sizeof( model ) );
		skin = strrchr( model, '/' );
		if ( skin ) {
			*skin++ = '\0';
		} else {
			skin = "default";
		}

		Com_sprintf( iconName, MAX_QPATH, "models/players/%s/icon_%s.tga", model, skin );
		
		loadingPlayerIcons[loadingPlayerIconCount] = trap_R_RegisterShaderNoMip( iconName );
		if ( !loadingPlayerIcons[loadingPlayerIconCount] ) {
			Com_sprintf( iconName, MAX_QPATH, "models/players/characters/%s/icon_%s.tga", model, skin );
			loadingPlayerIcons[loadingPlayerIconCount] = trap_R_RegisterShaderNoMip( iconName );
		}
		if ( !loadingPlayerIcons[loadingPlayerIconCount] ) {
			Com_sprintf( iconName, MAX_QPATH, "models/players/%s/icon_%s.tga", DEFAULT_MODEL, "default" );
			loadingPlayerIcons[loadingPlayerIconCount] = trap_R_RegisterShaderNoMip( iconName );
		}
		if ( loadingPlayerIcons[loadingPlayerIconCount] ) {
			loadingPlayerIconCount++;
		}
	}

	Q_strncpyz( personality, Info_ValueForKey( info, "n" ), sizeof(personality) );
	Q_CleanStr( personality );

	CG_LoadingString( personality, -1 );
}


/*
====================
CG_DrawInformation

Draw all the status / pacifier stuff during level loading
====================
*/
void CG_DrawInformation( void ) {
	const char	*s;
	const char	*info;
	int			y;
	qhandle_t	levelshot;
	qhandle_t	detail;
	qhandle_t	fade;
	qhandle_t	logo;
	qhandle_t	loading;
	char		buf[1024];
	vec4_t 		color_white	    	= {1.00f, 1.00f, 1.00f, 1.00f};
	vec4_t 		color_whiteblack	= {0.90f, 0.90f, 0.90f, 1.00f};
	vec4_t 		color_grey	    	= {0.30f, 0.30f, 0.30f, 1.00f};
	vec4_t 		color_lightgrey	    = {0.50f, 0.50f, 0.50f, 1.00f};

	info = CG_ConfigString( CS_SERVERINFO );

	s = Info_ValueForKey( info, "mapname" );
	levelshot = trap_R_RegisterShaderNoMip( va( "levelshots/%s", s ) );
	detail = trap_R_RegisterShaderNoMip( "menu/assets/loadingoverlay" );
	fade = trap_R_RegisterShaderNoMip( "menu/assets/blacktrans" );
	logo = trap_R_RegisterShaderNoMip( "menu/logo" );
	loading = trap_R_RegisterShaderNoMip( "menu/assets/loading" );
	if ( !levelshot ) {
		levelshot = trap_R_RegisterShaderNoMip( "menu/assets/unknownmap" );
	}
	trap_R_SetColor( NULL );

	CG_DrawPic( 0-cl_screenoffset.value, 0, 640+(cl_screenoffset.value*2), 480, levelshot );
	CG_DrawPic( 0-cl_screenoffset.value, 0, 640+(cl_screenoffset.value*2), 480, detail );

	CG_DrawRoundedRect(410+cl_screenoffset.value, 445, 220, 30, 1, color_lightgrey);
	CG_DrawProgressBar(415+cl_screenoffset.value, 459, 210, 12, cg.infoScreenValue, 8, color_white, color_grey);
	CG_DrawStringExt( 413+cl_screenoffset.value, 449, cg.infoScreenText, color_whiteblack, qfalse, qfalse, 8, 7, 128, -3.0 );

	CG_DrawPic( 320-50, 240-75, 100, 100, logo );
	CG_DrawPic( 320-24, 320-48, 48, 48, loading );

	CG_DrawPic( 0-cl_screenoffset.value, 0, 300, 85, fade );
	CG_DrawPic( 5-cl_screenoffset.value, 5, 100, 75, levelshot );

	y = 15;

	// server hostname
	Q_strncpyz(buf, Info_ValueForKey( info, "sv_hostname" ), 1024);
	Q_CleanStr(buf);
	CG_DrawBigString( 110-cl_screenoffset.value, y, buf, 1.0F );
	y += PROP_HEIGHT;

	// server mapname
	Q_strncpyz(buf, Info_ValueForKey( info, "mapname" ), 1024);
	Q_CleanStr(buf);
	CG_DrawBigString( 110-cl_screenoffset.value, y, buf, 1.0F );
	y += PROP_HEIGHT;

if(cl_language.integer == 0){
	// game type
	switch ( cgs.gametype ) {
	case GT_SANDBOX:
		s = "Sandbox";
		break;
	case GT_FFA:
		s = "Free For All";
		break;
	case GT_SINGLE:
		s = "Single Player";
		break;
	case GT_TOURNAMENT:
		s = "Tournament";
		break;
	case GT_TEAM:
		s = "Team Deathmatch";
		break;
	case GT_CTF:
		s = "Capture The Flag";
		break;
	case GT_1FCTF:
		s = "One Flag CTF";
		break;
	case GT_OBELISK:
		s = "Overload";
		break;
	case GT_HARVESTER:
		s = "Harvester";
		break;
	case GT_ELIMINATION:
		s = "Elimination";
		break;
	case GT_CTF_ELIMINATION:
		s = " CTF Elimination";
		break;
	case GT_LMS:
		s = "Last Man Standing";
		break;
	case GT_DOUBLE_D:
		s = "Double Domination";
		break;
        case GT_DOMINATION:
		s = "Domination";
		break;
	default:
		s = "Unknown Gametype";
		break;
	}
}
if(cl_language.integer == 1){
	// game type
	switch ( cgs.gametype ) {
	case GT_SANDBOX:
		s = "Песочница";
		break;
	case GT_FFA:
		s = "Все против всех";
		break;
	case GT_SINGLE:
		s = "Одиночная Игра";
		break;
	case GT_TOURNAMENT:
		s = "Турнир";
		break;
	case GT_TEAM:
		s = "Командный бой";
		break;
	case GT_CTF:
		s = "Захват флага";
		break;
	case GT_1FCTF:
		s = "Один флаг";
		break;
	case GT_OBELISK:
		s = "Атака базы";
		break;
	case GT_HARVESTER:
		s = "Жнец";
		break;
	case GT_ELIMINATION:
		s = "Устранение";
		break;
	case GT_CTF_ELIMINATION:
		s = "Устранение: Захват флага";
		break;
	case GT_LMS:
		s = "Последний оставшийся";
		break;
	case GT_DOUBLE_D:
		s = "Двойное доминирование";
		break;
        case GT_DOMINATION:
		s = "Доминирование";
		break;
	default:
		s = "Неизвесный режим";
		break;
	}
}
	CG_DrawBigString( 110-cl_screenoffset.value, y, s, 1.0F );
}

