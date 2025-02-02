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

#include "ui_startserver.h"

#define GAMESERVER_BACK0 "menu/assets/back_0"
#define GAMESERVER_BACK1 "menu/assets/back_1"
#define GAMESERVER_FIGHT0 "menu/assets/fight_0"
#define GAMESERVER_FIGHT1 "menu/assets/fight_1"
#define GAMESERVER_SERVER0 "menu/assets/server_0"
#define GAMESERVER_SERVER1 "menu/assets/server_1"
#define GAMESERVER_WEAPONS0 "menu/assets/weapons_0"
#define GAMESERVER_WEAPONS1 "menu/assets/weapons_1"
#define GAMESERVER_ITEMS0 "menu/assets/items_0"
#define GAMESERVER_ITEMS1 "menu/assets/items_1"
#define GAMESERVER_BOTS0 "menu/assets/bots_0"
#define GAMESERVER_BOTS1 "menu/assets/bots_1"
#define GAMESERVER_MAPS0 "menu/assets/maps_0"
#define GAMESERVER_MAPS1 "menu/assets/maps_1"
#define GAMESERVER_VARROWS "menu/assets/arrows_vert_0"
#define GAMESERVER_UP "menu/assets/arrows_vert_top"
#define GAMESERVER_DOWN "menu/assets/arrows_vert_bot"
#define GAMESERVER_DEL0 "menu/assets/del_0"
#define GAMESERVER_DEL1 "menu/assets/del_1"
#define GAMESERVER_SELECTED0 "menu/assets/switch_off"
#define GAMESERVER_SELECTED1 "menu/assets/switch_on"
#define GAMESERVER_ACTION0 "menu/assets/action0"
#define GAMESERVER_ACTION1 "menu/assets/action1"

#define FRAME_SPLASH "menu/assets/cut_frame"
#define FRAME_EXCLUDED "menu/assets/excluded"


// control id's are grouped for ease of allocation
// < 100 are common to all pages
#define ID_SERVERCOMMON_BACK 	10
#define ID_SERVERCOMMON_FIGHT 	11
#define ID_SERVERCOMMON_SERVER 	12
#define ID_SERVERCOMMON_ITEMS 	13
#define ID_SERVERCOMMON_BOTS 	14
#define ID_SERVERCOMMON_MAPS 	15
#define ID_SERVERCOMMON_WEAPON 	16


#define COLUMN_LEFT (SMALLCHAR_WIDTH * 18)
#define COLUMN_RIGHT (320 + (SMALLCHAR_WIDTH * 20))

#define LINE_HEIGHT (SMALLCHAR_HEIGHT + 2)

#define GAMETYPECOLUMN_X 400
#define GAMETYPEICON_X (GAMETYPECOLUMN_X - (20 * SMALLCHAR_WIDTH))
#define GAMETYPEROW_Y (64 + (LINE_HEIGHT/2))


#define MAPPAGE_TEXT 40


#define MAPSELECT_SELECT	"menu/assets/mapfocus"
#define MAP_FADETIME 1000


#define TABCONTROLCENTER_Y (240 + LINE_HEIGHT) + 20



typedef void (*CtrlCallback_t)(void * self, int event);


// all the controls common to menus
typedef struct commoncontrols_s {
	menubitmap_s back;
	menubitmap_s fight;
	menubitmap_s server;
	menubitmap_s weapon;
	menubitmap_s items;
	menubitmap_s maps;
	menubitmap_s bots;

	menutext_s servertext;
	menutext_s itemtext;
	menutext_s maptext;
	menutext_s bottext;
	menutext_s weapontext;

} commoncontrols_t;



enum { COMMONCTRL_BOTS, COMMONCTRL_MAPS, COMMONCTRL_SERVER, COMMONCTRL_ITEMS, COMMONCTRL_WEAPON };



// global data

extern vec4_t pulsecolor;
extern vec4_t fading_red;





//
// declarations
//

// ui_startserver_botsel.c
void UI_BotSelect_Index( char *bot , int index);

// ui_startserver_mapsel.c
void UI_StartMapMenu( int gametype, int index, const char* mapname);


// ui_startserver_common.c
qboolean StartServer_CheckFightReady(commoncontrols_t* c);
void StartServer_CommonControls_Init(menuframework_s* ,commoncontrols_t* ,CtrlCallback_t ,int );
void StartServer_BackgroundDraw(qboolean excluded);
void StartServer_SelectionDraw(void* self );



// ui_startserver_map.c
void StartServer_MapPage_MenuInit(void);
void StartServer_MapPage_Cache( void );
void StartServer_MapPage_CopyCustomLimitsToControls(void);

// ui_startserver_bot.c
void StartServer_BotPage_MenuInit(void);
void StartServer_BotPage_Cache( void );

// ui_startserver_server.c
void StartServer_ServerPage_MenuInit(void);
void StartServer_ServerPage_Mods(void);
void StartServer_ServerPage_Cache( void );

// ui_startserver_item.c
void StartServer_BothItemMenus_Cache( void );
void StartServer_ItemPage_MenuInit(void);
