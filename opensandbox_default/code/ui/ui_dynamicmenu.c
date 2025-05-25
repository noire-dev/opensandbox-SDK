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
#include "ui_dynamicmenu.h"


#define MAX_DYNAMICDEPTH 6
#define MAX_MENUSTRING 72

#define MAX_BOT_ICON 64

#define MENUICON_WIDTH 		20
#define MENUICON_HEIGHT 	20
#define MENUICON_GAP		2

/*
	Gap (screen pixels) between the border of a menu item and
	the border of the menu
*/
#define SUBMENU_SURROUNDGAP 1

/*
	Horizontal separation (screen pixels) between a sub menu and it's parent
*/
#define SUBMENU_GAP		0

/*
	Gap (virtual screen pixels) between left edge of
	menu item border and text/icon
*/
#define MENUSPACE_X		10*1.10

/*
	Vertical gap (virtual screen pixels) between text
	and top/bottom border of menu item
*/
#define MENUSPACE_Y		2*1.10

/*
	Scale factor for menu text and icons
*/
#define MENU_SCALE 0.7*1.10


typedef struct {
	char text[MAX_MENUSTRING];
	int index;
	int id;
	qhandle_t icon;
	createHandler createSubMenu;
	eventHandler runEvent;

	float trueX;    // used for pixel perfect positioning
	float trueY;
	float trueH;
	float trueW;
} dynamicitem_t;

typedef struct {
	menuframework_s menu;

	menuelement_s item[MAX_MENUITEMS];
	dynamicitem_t data[MAX_MENUITEMS];
	qhandle_t	background[MAX_MENUITEMS];

	int start[MAX_DYNAMICDEPTH];
	int end[MAX_DYNAMICDEPTH];	// indicates to (last item + 1)
	int active[MAX_DYNAMICDEPTH];
	qboolean icon[MAX_DYNAMICDEPTH];

	int gametype;
	int depth;
	char serverinfo[MAX_INFO_STRING];
} dynamicmenu_t;

static dynamicmenu_t s_dynamic;
static vec4_t dynamicmenu_edgecolor = { 1.0f, 1.0f, 1.0f, 0.5f };
static vec4_t dynamicmenu_infillcolor = { 1.0f, 1.0f, 1.0f, 0.33f };
static vec4_t dynamicmenu_backgrcolor = { 0.0f, 0.0f, 0.0f, 0.60f };

/*
=======================================================================

DYANMIC MENU CORE SERVICES

=======================================================================
*/

typedef struct {
	const char* longname;
	const char* shortname;
	const char* classname;
	qboolean loaded;
	const char* icon;

	int game;
} itemList_t;

// The machine gun is excluded from the list because it
// is the default weapon, maps usually don't have it as
// available for pickup.
static itemList_t dm_itemList[] = {
	{ "Red Armour", "red armour", "item_armor_body", qfalse, "ui_icons/iconr_red", 0 },
	{ "Yel Armour", "yellow armour", "item_armor_combat", qfalse, "ui_icons/iconr_yellow", 0 },
	{ "Mega", "mh", "item_health_mega", qfalse, "ui_icons/iconh_mega", 0},
	{ "Machine G", "machinegun", "weapon_machinegun", qfalse, "ui_icons/iconw_machinegun", 0 },
	{ "Shotgun", "shotgun", "weapon_shotgun", qfalse, "ui_icons/iconw_shotgun", 0 },
	{ "Grenade L", "grenade", "weapon_grenadelauncher", qfalse, "ui_icons/iconw_grenade", 0 },
	{ "Rocket L", "rocket", "weapon_rocketlauncher", qfalse, "ui_icons/iconw_rocket", 0 },
	{ "Plasma G", "plasmagun", "weapon_plasmagun", qfalse, "ui_icons/iconw_plasma", 0 },
	{ "Lightning", "lightning", "weapon_lightning", qfalse, "ui_icons/iconw_lightning", 0 },
	{ "Railgun", "railgun", "weapon_railgun", qfalse, "ui_icons/iconw_railgun", 0 },
	{ "BFG10k", "bfg", "weapon_bfg", qfalse, "ui_icons/iconw_bfg", 0 },
	{ "Nailgun", "nailgun", "weapon_nailgun", qfalse, "icons/iconw_nailgun", 0 },
	{ "Prox Launcher", "mines", "weapon_prox_launcher", qfalse, "icons/iconw_proxlauncher", 0 },
	{ "Chaingun", "chaingun", "weapon_chaingun", qfalse, "icons/iconw_chaingun", 0 },
	{ "Flamethrower", "flamethrower", "weapon_flamethrower", qfalse, "icons/iconw_flamethrower", 0 },
	{ "Dark Flare", "darkflare", "weapon_antimatter", qfalse, "icons/iconw_voodoo", 0 },
	{ "Quad", "quad", "item_quad", qfalse, "ui_icons/quad", 0 },
	{ "Regen", "regen", "item_regen", qfalse, "ui_icons/regen", 0 },
	{ "Invis", "invis", "item_invis", qfalse, "ui_icons/invis", 0 },
	{ "Btl Suit", "bsuit", "item_enviro", qfalse, "ui_icons/envirosuit", 0 },
	{ "Haste", "haste", "item_haste", qfalse, "ui_icons/haste", 0 }
};

static int dm_numMenuItems = sizeof(dm_itemList)/sizeof(dm_itemList[0]);

/*
=================
DynamicMenu_ServerGametype
=================
*/
int DynamicMenu_ServerGametype(void)
{
	char	info[MAX_INFO_STRING];

	trap_GetConfigString( CS_SERVERINFO, info, sizeof(info) );
	return atoi( Info_ValueForKey( info, "g_gametype" ) );
}

/*
=================
DynamicMenu_ItemShortname
=================
*/
const char* DynamicMenu_ItemShortname(int index)
{
	return dm_itemList[index].shortname;
}

/*
=================
DynamicMenu_AddListOfItems
=================
*/
void DynamicMenu_AddListOfItems( int exclude, createHandler crh, eventHandler evh )
{
	int i;

	for (i = 0; i < dm_numMenuItems; i++)
	{
		if (!dm_itemList[i].loaded)
			continue;

		if (dm_itemList[i].game && dm_itemList[i].game != s_dynamic.gametype)
			continue;

		if (!DynamicMenu_AddIconItem(dm_itemList[i].longname, i, dm_itemList[i].icon, crh, evh))
			continue;

		if (i == exclude)
		{
			// gray the item
			s_dynamic.item[s_dynamic.end[ s_dynamic.depth - 1] - 1].generic.flags |= QMF_GRAYED;
		}
	}
}

/*
=================
DynamicMenu_InitMapItems
=================
*/
static void DynamicMenu_InitMapItems( void )
{
	int		i, j;
	char	items[MAX_ITEMS+1];
	int 	type;

	for (i = 0; i < dm_numMenuItems; i++)
		dm_itemList[i].loaded = qfalse;

	trap_GetConfigString( CS_ITEMS, items, sizeof(items) );
	for ( i = 1; i < bg_numItems; i++)
	{
		if (items[i] != '1')
			continue;

		// locate item on our list
		type = bg_itemlist[i].giType;
		for (j = 0 ; j < dm_numMenuItems; j++)
		{
			if (!Q_stricmp(bg_itemlist[i].classname, dm_itemList[j].classname))
			{
				dm_itemList[j].loaded = qtrue;
				break;
			}
		}
	}
}

/*
=================
DynamicMenu_AddListOfPlayers
=================
*/
void DynamicMenu_AddListOfPlayers( int type, createHandler crh, eventHandler evh )
{
	uiClientState_t	cs;
	int		numPlayers;
	int		isBot;
	int		n;
	char	info[MAX_INFO_STRING];
	char	boticon[MAX_BOT_ICON];
	char 	name[64];
	int 	playerTeam, team;
	int 	depth;

	trap_GetConfigString( CS_SERVERINFO, info, sizeof(info) );
	numPlayers = atoi( Info_ValueForKey( info, "g_maxClients" ) );

	trap_GetClientState( &cs );
	trap_GetConfigString( CS_PLAYERS + cs.clientNum, info, MAX_INFO_STRING );
	playerTeam = atoi(Info_ValueForKey(info, "t"));

	depth = s_dynamic.depth - 1;
	for( n = 0; n < numPlayers; n++ ) {
		trap_GetConfigString( CS_PLAYERS + n, info, MAX_INFO_STRING );

	if (!(type & PT_ALL)){
		if (n == cs.clientNum)
			continue;
	}

		isBot = atoi( Info_ValueForKey( info, "skill" ) );
		if( (type & PT_BOTONLY) && !isBot ) {
			continue;
		}

		if( (type & PT_PLAYERONLY) && isBot ) {
			continue;
		}

		team = atoi(Info_ValueForKey(info, "t"));
		if ((type & PT_FRIENDLY) && team != playerTeam)
			continue;

		if ((type & PT_ENEMY) && team == playerTeam)
			continue;

		Q_strncpyz(name, Info_ValueForKey(info, "n"), 64);
		//Q_CleanStr(name);

		if (!name[0])
			continue;

		if (type & PT_EXCLUDEPARENT && depth >= 1)
		{
			// depth has been increased by init of (sub)menu
			if (!Q_stricmp(name, s_dynamic.data[s_dynamic.active[depth - 1]].text))
				continue;
		}

		if (type & PT_EXCLUDEGRANDPARENT && depth >= 2)
		{
			// depth has been increased by init of (sub)menu
			if (!Q_stricmp(name, s_dynamic.data[s_dynamic.active[depth - 2]].text))
				continue;
		}
	
		if (type & PT_ALL && 0)
		{
				continue;
		}

		UI_ServerPlayerIcon(Info_ValueForKey(info, "model"), boticon, MAX_BOT_ICON);

		DynamicMenu_AddIconItem(name, 0, boticon, crh, evh);
	}
}

/*
=======================================================================

DYANMIC MENU

=======================================================================
*/

/*
=================
DynamicMenu_Depth
=================
*/
int DynamicMenu_Depth(void)
{
	return s_dynamic.depth;
}

/*
=================
DynamicMenu_ActiveIdAtDepth
=================
*/
int DynamicMenu_ActiveIdAtDepth(int depth)
{
	return s_dynamic.data[s_dynamic.active[depth - 1]].id;
}

/*
=================
DynamicMenu_ActiveIndexAtDepth
=================
*/
int DynamicMenu_ActiveIndexAtDepth(int depth)
{
	return s_dynamic.active[depth - 1];
}

/*
=================
DynamicMenu_IdAtIndex
=================
*/
int DynamicMenu_IdAtIndex(int index)
{
	return s_dynamic.data[index].id;
}

/*
=================
DynamicMenu_StringAtIndex
=================
*/
const char* DynamicMenu_StringAtIndex(int index)
{
	return s_dynamic.data[index].text;
}

/*
=================
DynamicMenu_SetFlags
=================
*/
void DynamicMenu_SetFlags(int depth, int id, int flags)
{
	int i;

	for (i = s_dynamic.start[depth - 1]; i < s_dynamic.end[depth - 1]; i++) {
		if (s_dynamic.data[i].id == id) {
			s_dynamic.item[i].generic.flags |= flags;
			return;
		}
	}
}

/*
=================
DynamicMenu_InitSubMenu
=================
*/
qboolean DynamicMenu_SubMenuInit( void)
{
	int pos;

	if (s_dynamic.depth == MAX_DYNAMICDEPTH)
		return qfalse;

	if (s_dynamic.depth == 0)
		pos = 0;
	else
		pos = s_dynamic.end[s_dynamic.depth - 1];

	if (pos == MAX_MENUITEMS)
		return qfalse;

	s_dynamic.depth++;
	s_dynamic.active[s_dynamic.depth - 1] = -1;
	s_dynamic.start[s_dynamic.depth - 1] = pos;
	s_dynamic.end[s_dynamic.depth - 1] = pos;

    s_dynamic.background[s_dynamic.depth - 1] = 0;

	return qtrue;
}

/*
=================
DynamicMenu_AddIconItem
=================
*/
qboolean DynamicMenu_AddIconItem( const char* string, int id, const char* icon, createHandler crh, eventHandler evh)
{
	int index, depth;

	depth = s_dynamic.depth - 1;
	index = s_dynamic.end[depth];

	if (index == MAX_MENUITEMS)
		return qfalse;

	// can't have submenu and event attached to menu item
	if (crh && evh)
		return qfalse;

	if (!string || !string[0])
		string = "[no text]";

	s_dynamic.data[index].index = index;
	s_dynamic.data[index].id = id;
	s_dynamic.data[index].createSubMenu = crh;
	s_dynamic.data[index].runEvent = evh;
	Q_strncpyz(s_dynamic.data[index].text, string, MAX_MENUSTRING);

	if (icon) {
		s_dynamic.data[index].icon = trap_R_RegisterShaderNoMip(icon);
	}
	else {
		s_dynamic.data[index].icon = 0;
	}

	s_dynamic.end[depth]++;

	return qtrue;
}

/*
=================
DynamicMenu_AddItem
=================
*/
qboolean DynamicMenu_AddItem( const char* string, int id, createHandler crh, eventHandler evh)
{
	return DynamicMenu_AddIconItem(string, id, NULL, crh, evh);
}

/*
=================
DynamicMenu_AddBackground
=================
*/
void DynamicMenu_AddBackground( const char* background)
{
	if (!background || !background[0])
		return;

	s_dynamic.background[ s_dynamic.depth - 1 ] = trap_R_RegisterShaderNoMip(background);
}

/*
=================
DynamicMenu_IconSpace
=================
*/
static float DynamicMenu_IconSpace( void )
{
	return (MENUICON_WIDTH + 2*MENUICON_GAP);
}

/*
=================
DynamicMenu_FinishInitSubMenu

Where we need pixel accurate positioning we
need to use pixels of 1/uis.scale size in
the virtual 640x480 screen
=================
*/
void DynamicMenu_FinishSubMenuInit( void )
{
	int depth;
	int width;
	float maxwidth;
	float height, lineheight;
	float posx, posy;
	int i, count, start, active;
	float scale;
	menuelement_s* ptr;
	dynamicitem_t* dptr;
	qboolean submenu;
	qboolean icon;

	depth = s_dynamic.depth - 1;

	// find the widest item
	submenu = qfalse;
	icon = qfalse;
	maxwidth = 0;
	start = s_dynamic.start[depth];
	count = s_dynamic.end[depth] - start;
	for ( i = 0; i < count; i++)
	{
		width = UI_ProportionalStringWidth(s_dynamic.data[i + start].text, 1.00);
		if (width > maxwidth)
			maxwidth = width;

		if (s_dynamic.data[i + start].createSubMenu)
			submenu = qtrue;

		if (s_dynamic.data[i + start].icon)
			icon = qtrue;
	}

	if (submenu)
	{
		maxwidth += UI_ProportionalStringWidth(" \r", 1.00);	// space and submenu pointer
	}

	scale = 1.00;
	maxwidth *= scale;
	maxwidth *= (MENU_SCALE * 1.2);	// some adjustment for scaling of font used
    maxwidth += MENUSPACE_X;

	if (icon)
		maxwidth += DynamicMenu_IconSpace();

	s_dynamic.icon[depth] = icon;

	// determine the position of the menu
	lineheight = PROP_HEIGHT * scale + 2*MENUSPACE_Y;
	height = count * lineheight;

	if (depth == 0)
	{
		posy = 240 - height/2;
		posx = 0;
	}
	else
	{
		active = s_dynamic.active[depth - 1];
		posx = s_dynamic.data[active].trueX + s_dynamic.data[active].trueW + (SUBMENU_SURROUNDGAP + SUBMENU_GAP)/uis.scale;
		posy = s_dynamic.data[active].trueY;

		if (posy + height > 480 - 48)
			posy = 480 - 48 - height;
	}

	for (i = 0; i < count; i++)
	{
		ptr = &s_dynamic.item[start + i];
		dptr = &s_dynamic.data[start + i];

		dptr->trueX = posx + (SUBMENU_SURROUNDGAP + 1)/uis.scale;
		dptr->trueY = posy + i*lineheight;
		dptr->trueH = lineheight - 1 / uis.scale;
		dptr->trueW = maxwidth;

		ptr->generic.x = dptr->trueX;
		ptr->generic.y = dptr->trueY;

		ptr->generic.left = dptr->trueX;
		ptr->generic.right = dptr->trueX + dptr->trueW;
		ptr->generic.top = dptr->trueY;
		ptr->generic.bottom = dptr->trueY + dptr->trueH;

		ptr->generic.flags &= ~(QMF_HIDDEN|QMF_INACTIVE);
	}
}

/*
=================
DynamicMenu_OnActiveList
=================
*/
qboolean DynamicMenu_OnActiveList( int index )
{
	int depth;
	int i;

	depth = s_dynamic.depth;

	for ( i = 0; i < depth ; i++)
		if (s_dynamic.active[i] == index)
			return qtrue;

	return qfalse;
}

/*
=================
DynamicMenu_DrawBackground
=================
*/
static void DynamicMenu_DrawBackground( int depth )
{
	float fx, fy, fw, fh;
	int x, y, w, h;
	int first, last;
	float offset;
	dynamicitem_t* dptr;

	first = s_dynamic.start[depth];
	last = s_dynamic.end[depth] - 1;

	// menu border
	dptr = &s_dynamic.data[ first ];

	fx = dptr->trueX;
	fy = dptr->trueY;
	fw = dptr->trueW;
	fh = s_dynamic.data[ last ].trueY + s_dynamic.data[ last ].trueH - fy;

	offset = (SUBMENU_SURROUNDGAP + 1)/uis.scale;
	UI_DrawRect(fx - offset, fy - offset, fw + 2*offset, fh + 2*offset, dynamicmenu_edgecolor);

	// background graphic
	if (!s_dynamic.background[depth])
		return;

	x = s_dynamic.item[first].generic.left;
	y = s_dynamic.item[first].generic.top;

	w = s_dynamic.item[first].generic.right - x;
	h = s_dynamic.item[last].generic.bottom - y;

	UI_DrawHandlePic(x, y, w, h, s_dynamic.background[ depth ]);
}

/*
=================
DynamicMenu_MenuItemDraw
=================
*/
static void DynamicMenu_MenuItemDraw( void* self )
{
	float 	fx, fy, fh, fw;
	float 	*color;
	int		style;
	menuelement_s* t;
	dynamicitem_t *dptr;
	vec4_t	tmp_color;
	int 	depth;
	qboolean active;
	float scale, txt_y, offset;
	int charh;

	t = (menuelement_s*)self;
	dptr = &s_dynamic.data[ t->generic.id ];

	depth = DynamicMenu_DepthOfIndex(t->generic.id) - 1;

	if (s_dynamic.start[depth] == t->generic.id)
		DynamicMenu_DrawBackground(depth);

	fx = dptr->trueX;
	fy = dptr->trueY;
	fw = dptr->trueW;
	fh = dptr->trueH;

	// draw the border for the item;
	active = DynamicMenu_OnActiveList(t->generic.id);
	UI_DrawRect(fx, fy, fw, fh, dynamicmenu_edgecolor);

	if (active) {
		// draw selected infill
		offset = 1/uis.scale;	// correction for screen resolution
		UI_FillRect(fx + offset, fy + offset, fw - 2*offset, fh - 2*offset, dynamicmenu_infillcolor);
	}
	
	if (!active) {
		// draw selected infill
		offset = 1/uis.scale;	// correction for screen resolution
		UI_FillRect(fx + offset, fy + offset, fw - 2*offset, fh - 2*offset, dynamicmenu_backgrcolor);
	}

	// draw the icon if present
	fx += MENUSPACE_X;
	tmp_color[0] = 1.0;
	tmp_color[1] = 1.0;
	tmp_color[2] = 1.0;
	tmp_color[3] = 1.0;

	if (dptr->icon) {
		float icon_y = fy + 0.5*(fh - MENUICON_HEIGHT * MENU_SCALE);
		tmp_color[3] = 0.8;

		if (t->generic.flags & QMF_GRAYED)
			tmp_color[3] = 0.5;


		trap_R_SetColor(tmp_color);
		UI_DrawHandlePic(fx + MENUICON_GAP, icon_y, MENUICON_WIDTH * MENU_SCALE, MENUICON_HEIGHT * MENU_SCALE, dptr->icon);
		trap_R_SetColor(NULL);
	}

	if (s_dynamic.icon[depth])
		fx += DynamicMenu_IconSpace();


	// draw the text
	if (t->generic.flags & QMF_GRAYED)
		color = text_color_disabled;
	else
		color = t->color;

	style = t->style;
	if( t->generic.flags & QMF_PULSEIFFOCUS ) {
		if( Menu_ItemAtCursor( t->generic.parent ) == t ) {
			style |= UI_PULSE;
		}
		else {
			style |= UI_INVERSE;
		}
	}

	txt_y = fy + 0.5*(fh - PROP_HEIGHT * MENU_SCALE * 1.00);
	ST_DrawString( fx, txt_y, t->string, style, color, 1.00 );


	// draw the cursor for submenu if needed
	if (style & UI_SMALLFONT)
		charh = SMALLCHAR_HEIGHT;
	else
		charh = BIGCHAR_HEIGHT;	

	txt_y = fy + 0.5*(fh - charh); 
	if (dptr->createSubMenu)
	{
		ST_DrawChar( dptr->trueX + dptr->trueW, txt_y, 13, style|UI_RIGHT, color, 1.00);
	}
}

/*
=================
DynamicMenu_MenuDraw
=================
*/
static void DynamicMenu_MenuDraw( void )
{
	const char		*s;
	char			key[MAX_INFO_KEY];
	char			value[MAX_INFO_VALUE];
	int				y, x;

    const char* gametypes[] = {
		"Sandbox",
		"Map Editor",
		"Single Player",
		"Free For All",
		"Tournament",
		"Team Deathmatch",
		"Capture the Flag",
		"One Flag Capture",
		"Overload",
		"Harvester",
		"Elimination",
		"CTF Elimination",
		"Last Man Standing",
		"Double Domination",
		"Domination",
		0
    };

	if (uis.debug) {
		ST_DrawString(0, SMALLCHAR_HEIGHT, va("depth:%i", s_dynamic.depth), UI_SMALLFONT, color_white, 1.00);
		ST_DrawString(0, 32 + SMALLCHAR_HEIGHT, va("active: %i %i %i", s_dynamic.active[0], s_dynamic.active[1], s_dynamic.active[2] ), UI_SMALLFONT, color_white, 1.00);
	}
	y = 5;
	x = 635 + uis.wideoffset;
	s = s_dynamic.serverinfo;
	while ( s ) {
		Info_NextPair( &s, key, value );
		if ( !key[0] ) {
			break;
		}

		if (strcmp(key, "sv_hostname") == 0) {
			strcpy(key, "Hostname");
            ST_DrawString(x - 8, y, va("%s: %s", key, value), UI_RIGHT | UI_TINYFONT, color_white, 1.00);
			y += TINYCHAR_HEIGHT;
		}
		if (strcmp(key, "cl_selectedmod") == 0) {
			strcpy(key, "Mod");
            ST_DrawString(x - 8, y, va("%s: %s", key, value), UI_RIGHT | UI_TINYFONT, color_white, 1.00);
			y += TINYCHAR_HEIGHT;
		}
		if (strcmp(key, "mapname") == 0) {
			strcpy(key, "Map");
            ST_DrawString(x - 8, y, va("%s: %s", key, value), UI_RIGHT | UI_TINYFONT, color_white, 1.00);
			y += TINYCHAR_HEIGHT;
		}
		if (strcmp(key, "g_gametype") == 0) {
            int gametype = atoi(value);
			strcpy(key, "Gametype");
            if (gametype >= 0 && gametype < sizeof(gametypes) / sizeof(gametypes[0])) {
                ST_DrawString(x - 8, y, va("%s: %s", key, gametypes[gametype]), UI_RIGHT | UI_TINYFONT, color_white, 1.00);
                y += TINYCHAR_HEIGHT;
            }
        }
		if (strcmp(key, "fraglimit") == 0) {
			strcpy(key, "Fraglimit");
            ST_DrawString(x - 8, y, va("%s: %s", key, value), UI_RIGHT | UI_TINYFONT, color_white, 1.00);
			y += TINYCHAR_HEIGHT;
		}
		if (strcmp(key, "capturelimit") == 0) {
			strcpy(key, "Capturelimit");
            ST_DrawString(x - 8, y, va("%s: %s", key, value), UI_RIGHT | UI_TINYFONT, color_white, 1.00);
			y += TINYCHAR_HEIGHT;
		}
		if (strcmp(key, "timelimit") == 0) {
			strcpy(key, "Timelimit");
            ST_DrawString(x - 8, y, va("%s: %s", key, value), UI_RIGHT | UI_TINYFONT, color_white, 1.00);
			y += TINYCHAR_HEIGHT;
		}
		if (strcmp(key, "g_maxClients") == 0) {
			strcpy(key, "Max players");
            ST_DrawString(x - 8, y, va("%s: %s", key, value), UI_RIGHT | UI_TINYFONT, color_white, 1.00);
			y += TINYCHAR_HEIGHT;
		}
		if (strcmp(key, "sv_anticheatengine") == 0) {
			strcpy(key, "Anti-cheat status");
			if(atoi(value)){
            	ST_DrawString(x - 8, y, va("%s: enabled", key, value), UI_RIGHT | UI_TINYFONT, color_white, 1.00);
			} else {
				ST_DrawString(x - 8, y, va("%s: disabled", key, value), UI_RIGHT | UI_TINYFONT, color_white, 1.00);
			}
			y += TINYCHAR_HEIGHT;
		}
	}
	Menu_Draw(&s_dynamic.menu);
}

/*
=================
DynamicMenu_DepthOfIndex
=================
*/
int DynamicMenu_DepthOfIndex( int pos )
{
	int i;
	int maxdepth, depth;

	maxdepth = s_dynamic.depth;
	depth = 0;
	for (i = 0; i < maxdepth; i++)
	{
		if (pos < s_dynamic.end[i])
		{
			depth = i + 1;
			break;
		}
	}

	return depth;
}

/*
=================
DynamicMenu_SetFocus
=================
*/
void DynamicMenu_SetFocus( int pos )
{
	int i;
	int depth, newdepth;

	depth = s_dynamic.depth;
	newdepth = DynamicMenu_DepthOfIndex(pos);

	if (newdepth == 0)
	{
		Com_Printf("SetFocus: index %i outside menu\n", pos);
		return;
	}

	s_dynamic.active[ newdepth - 1 ] = pos;
	s_dynamic.depth = newdepth;

	// hide any previous submenus
	if (newdepth < depth)
	{
		for (i = s_dynamic.start[ newdepth ]; i < s_dynamic.end[depth - 1]; i++)
		{
			s_dynamic.item[i].generic.flags |= (QMF_HIDDEN|QMF_INACTIVE);
			s_dynamic.item[i].generic.flags &= ~QMF_GRAYED;
		}
	}

	s_dynamic.active[newdepth - 1] = pos;

	// show this one (if needed)
	if (s_dynamic.data[pos].createSubMenu)
		s_dynamic.data[pos].createSubMenu();
}

/*
=================
DynamicMenu_ClearFocus
=================
*/
void DynamicMenu_ClearFocus( int pos )
{
}

/*
=================
DynamicMenu_ActivateControl
=================
*/
static void DynamicMenu_ActivateControl( int pos )
{
	int i;
	int depth;

	depth = DynamicMenu_DepthOfIndex(pos);

	if (depth == 0)
	{
		Com_Printf("ActivateControl: index %i outside menu\n", pos);
		return;
	}

	// not at the deepest level, can't be a command
	if (depth < s_dynamic.depth)
		return;

	if (s_dynamic.data[pos].runEvent)
		s_dynamic.data[pos].runEvent(pos);
	else
		Com_Printf("ActivateControl: index %i has no event\n", pos);
}

/*
=================
DynamicMenu_MenuEvent
=================
*/
static void DynamicMenu_MenuEvent( void* self, int event )
{
	menuelement_s* t;

	t = (menuelement_s*)self;

	switch (event)
	{
	case QM_GOTFOCUS:
		DynamicMenu_SetFocus(t->generic.id);
		break;
	case QM_LOSTFOCUS:
		DynamicMenu_ClearFocus(t->generic.id);
		break;
	case QM_ACTIVATED:
		DynamicMenu_ActivateControl(t->generic.id);
		break;
	}
}

/*
=================
DynamicMenu_MenuInit
=================
*/
void DynamicMenu_MenuInit( qboolean full )
{
	int i;

	memset(&s_dynamic.menu, 0, sizeof(dynamicmenu_t));

	s_dynamic.gametype = (int)trap_Cvar_VariableValue("g_gametype");

	s_dynamic.menu.draw = DynamicMenu_MenuDraw;
	s_dynamic.menu.fullscreen = full;

	// start up the menu system
	s_dynamic.depth = 0;

	// force as top level menu
	trap_Key_SetCatcher( KEYCATCH_UI );
	uis.menusp = 0;

	// set menu cursor to a nice location
	uis.cursorx = 50;
	uis.cursory = 240;

	trap_GetConfigString( CS_SERVERINFO, s_dynamic.serverinfo, MAX_INFO_STRING );

	for (i = 0; i < MAX_MENUITEMS; i++)
	{
		s_dynamic.item[i].generic.type = MTYPE_PTEXT;
		s_dynamic.item[i].generic.flags = QMF_INACTIVE|QMF_HIDDEN|QMF_NODEFAULTINIT|QMF_PULSEIFFOCUS;
		s_dynamic.item[i].generic.ownerdraw = DynamicMenu_MenuItemDraw ;
		s_dynamic.item[i].generic.callback = DynamicMenu_MenuEvent ;
		s_dynamic.item[i].generic.id = i;
		s_dynamic.item[i].string = s_dynamic.data[i].text;
		s_dynamic.item[i].style = UI_SMALLFONT|UI_DROPSHADOW;
		s_dynamic.item[i].color = color_white;

		Menu_AddItem(&s_dynamic.menu, &s_dynamic.item[i]);
	}

	DynamicMenu_InitMapItems();

	UI_PushMenu( &s_dynamic.menu );
}
