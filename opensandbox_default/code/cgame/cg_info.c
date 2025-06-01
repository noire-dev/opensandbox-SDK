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

/*
======================
CG_LoadingString

======================
*/
void CG_LoadingString( const char *s, float value ) {
	Q_strncpyz( cg.infoScreenText, va("Loading... %s", s), sizeof( cg.infoScreenText ) );
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
	
	CG_LoadingString( item->pickup_name, -1 );
}

/*
===================
CG_LoadingClient
===================
*/
void CG_LoadingClient( int clientNum ) {
	const char		*info;
	char			personality[MAX_QPATH];

	info = CG_ConfigString( CS_PLAYERS + clientNum );

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

	CG_DrawPic( 0-cgs.wideoffset, 0, 640+(cgs.wideoffset*2), 480, levelshot );
	CG_DrawPic( 0-cgs.wideoffset, 0, 640+(cgs.wideoffset*2), 480, detail );

	CG_DrawRoundedRect(410+cgs.wideoffset, 445, 220, 30, 1, color_lightgrey);
	CG_DrawProgressBar(415+cgs.wideoffset, 459, 210, 12, cg.infoScreenValue, 8, color_white, color_grey);
	CG_DrawString( 413+cgs.wideoffset, 449, cg.infoScreenText, color_whiteblack, qfalse, qfalse, 8, 7, 128, -3.0 );

	CG_DrawPic( 320-50, 240-75, 100, 100, logo );
	CG_DrawPic( 320-24, 320-48, 48, 48, loading );

	CG_DrawPic( 0-cgs.wideoffset, 0, 300, 85, fade );
	CG_DrawPic( 5-cgs.wideoffset, 5, 100, 75, levelshot );

	y = 15;

	// server hostname
	Q_strncpyz(buf, Info_ValueForKey( info, "sv_hostname" ), 1024);
	Q_CleanStr(buf);
	CG_DrawBigString( 110-cgs.wideoffset, y, buf, 1.0F );
	y += PROP_HEIGHT;

	// server mapname
	Q_strncpyz(buf, Info_ValueForKey( info, "mapname" ), 1024);
	Q_CleanStr(buf);
	CG_DrawBigString( 110-cgs.wideoffset, y, buf, 1.0F );
	y += PROP_HEIGHT;

	// game type
	switch ( cgs.gametype ) {
	case GT_SANDBOX:
		s = "Sandbox";
		break;
	case GT_MAPEDITOR:
		s = "Map Editor";
		break;
	case GT_FFA:
		s = "Free For All";
		break;
	case GT_TOURNAMENT:
		s = "Tournament";
		break;
	case GT_LMS:
		s = "Last Man Standing";
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
		s = "CTF Elimination";
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
	CG_DrawBigString( 110-cgs.wideoffset, y, s, 1.0F );
}
