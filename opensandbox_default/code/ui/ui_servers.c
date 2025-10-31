// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

#include "ui_local.h"

#define MAX_GLOBALSERVERS		128
#define MAX_PINGREQUESTS		32
#define MAX_ADDRESSLENGTH		64
#define MAX_LISTBOXITEMS		128
#define MAX_LOCALSERVERS		128
#define MAX_STATUSLENGTH		64
#define MAX_LISTBOXWIDTH		111

#define ART_UNKNOWNMAP			"menu/assets/unknownmap"

#define ID_MASTER			10
#define ID_CONNECT			11
#define ID_LIST				12
#define ID_REFRESH			13

#define AS_LOCAL			0
#define AS_GLOBAL			1

static const char *master_items[] = {
	"Local",
	"Internet",
	0
};

static char* netnames[] = {
	"????",
	"IPv4",
	"IPv6",
	NULL
};

typedef struct {
	char adrstr[MAX_ADDRESSLENGTH];
	int start;
} pinglist_t;

typedef struct servernode_s {
	char adrstr[MAX_ADDRESSLENGTH];
	char hostname[22 + 3];
	char addonname[22];
	char mapname[22];
	int numclients;
	int maxclients;
	int pingtime;
	int gametype;
	char gamename[22];
	int nettype;
	int minPing;
	int maxPing;
} servernode_t;

typedef struct {
	char buff[MAX_LISTBOXWIDTH];
	servernode_t* servernode;
} table_t;

typedef struct {
	menuframework_s menu;
	menuelement_s status;
	menuelement_s master;
	menuelement_s domain;
	menuelement_s connect;
	menuelement_s list;
	menuelement_s mappic;
	menuelement_s refresh;

	pinglist_t pinglist[MAX_PINGREQUESTS];
	table_t table[MAX_LISTBOXITEMS];
	char* items[MAX_LISTBOXITEMS];
	int numqueriedservers;
	int* numservers;
	servernode_t* serverlist;
	int currentping;
	qboolean refreshservers;
	int nextpingtime;
	int maxservers;
	int refreshtime;
} arenaservers_t;

static arenaservers_t g_arenaservers;
static servernode_t g_globalserverlist[MAX_GLOBALSERVERS];
static int g_numglobalservers;
static servernode_t g_localserverlist[MAX_LOCALSERVERS];
static int g_numlocalservers;
static int g_servertype;

static int ArenaServers_MaxPing(void) {
	int maxPing;

	maxPing = (int)trap_Cvar_VariableValue("cl_maxPing");
	if(maxPing < 100) {
		maxPing = 100;
	}
	return maxPing;
}

static void ArenaServers_Go(void) {
	servernode_t* servernode;

	servernode = g_arenaservers.table[g_arenaservers.list.curvalue].servernode;
	if(servernode) {
		trap_Cmd_ExecuteText(EXEC_APPEND, va("cl_selectedmod = %s\n", servernode->addonname));
		trap_Cmd_ExecuteText(EXEC_APPEND, va("connect %s\n", servernode->adrstr));
	}
}

static void ArenaServers_UpdatePicture(void) {
	static char picname[64];
	servernode_t* servernodeptr;

	if(!g_arenaservers.list.numitems) {
		g_arenaservers.mappic.string = NULL;
	} else {
		servernodeptr = g_arenaservers.table[g_arenaservers.list.curvalue].servernode;
		Com_sprintf(picname, sizeof(picname), "levelshots/%s", servernodeptr->mapname);
		g_arenaservers.mappic.string = picname;
	}

	// force shader update during draw
	g_arenaservers.mappic.shader = 0;
}

static void ArenaServers_UpdateMenu(void) {
	int i;
	int j;
	int count;
	char* buff;
	servernode_t* servernodeptr;
	table_t* tableptr;

	if(g_arenaservers.numqueriedservers > 0) {
		// servers found
		if(g_arenaservers.refreshservers && (g_arenaservers.currentping <= g_arenaservers.numqueriedservers)) {
			// show progress
			Com_sprintf(g_arenaservers.status.string, MAX_STATUSLENGTH, "%d of %d Servers.", g_arenaservers.currentping, g_arenaservers.numqueriedservers);
		} else {
			// all servers pinged - enable controls
			g_arenaservers.master.generic.flags &= ~QMF_GRAYED;
			g_arenaservers.list.generic.flags &= ~QMF_GRAYED;
			g_arenaservers.refresh.generic.flags &= ~QMF_GRAYED;
		}
	} else {
		// no servers found
		if(g_arenaservers.refreshservers) {
			strcpy(g_arenaservers.status.string, "Scanning For Servers...");

			// disable controls during refresh
			g_arenaservers.master.generic.flags |= QMF_GRAYED;
			g_arenaservers.list.generic.flags |= QMF_GRAYED;
			g_arenaservers.refresh.generic.flags |= QMF_GRAYED;
		} else {
			if(g_arenaservers.numqueriedservers < 0) {
				strcpy(g_arenaservers.status.string, "No Response From Master Server.");
			} else {
				strcpy(g_arenaservers.status.string, "No Servers Found.");
			}

			// end of refresh - set control state
			g_arenaservers.master.generic.flags &= ~QMF_GRAYED;
			g_arenaservers.list.generic.flags |= QMF_GRAYED;
			g_arenaservers.refresh.generic.flags &= ~QMF_GRAYED;
		}

		// zero out list box
		g_arenaservers.list.numitems = 0;
		g_arenaservers.list.curvalue = 0;
		g_arenaservers.list.top = 0;

		// update picture
		ArenaServers_UpdatePicture();
		return;
	}

	// build list box strings - apply culling filters
	servernodeptr = g_arenaservers.serverlist;
	count = *g_arenaservers.numservers;
	for(i = 0, j = 0; i < count; i++, servernodeptr++) {
		tableptr = &g_arenaservers.table[j];
		tableptr->servernode = servernodeptr;
		buff = tableptr->buff;

		// can only cull valid results
		if(!servernodeptr->numclients) continue;

		if(servernodeptr->numclients == servernodeptr->maxclients) continue;

		Com_sprintf(buff, MAX_LISTBOXWIDTH, "%-20.20s %-20.20s %-20.20s %2d/%2d %-20.20s %4s %10d", servernodeptr->hostname, servernodeptr->addonname, servernodeptr->mapname, servernodeptr->numclients, servernodeptr->maxclients, servernodeptr->gamename, netnames[servernodeptr->nettype], servernodeptr->pingtime);
		j++;
	}

	g_arenaservers.list.numitems = j;
	g_arenaservers.list.curvalue = 0;
	g_arenaservers.list.top = 0;

	// update picture
	ArenaServers_UpdatePicture();
}

static void ArenaServers_Insert(char* adrstr, char* info, int pingtime) {
	servernode_t* servernodeptr;
	int i;

	if((pingtime >= ArenaServers_MaxPing())) return; //slow global or local servers do not get entered

	if(*g_arenaservers.numservers >= g_arenaservers.maxservers) {
		// list full;
		servernodeptr = g_arenaservers.serverlist + (*g_arenaservers.numservers) - 1;
	} else {
		// next slot
		servernodeptr = g_arenaservers.serverlist + (*g_arenaservers.numservers);
		(*g_arenaservers.numservers)++;
	}

	Q_strncpyz(servernodeptr->adrstr, adrstr, MAX_ADDRESSLENGTH);

	Q_strncpyz(servernodeptr->hostname, Info_ValueForKey(info, "hostname"), 22);
	Q_CleanStr(servernodeptr->hostname);
	Q_strupr(servernodeptr->hostname);

	Q_strncpyz(servernodeptr->mapname, Info_ValueForKey(info, "mapname"), 22);
	Q_CleanStr(servernodeptr->mapname);
	Q_strupr(servernodeptr->mapname);

	Q_strncpyz(servernodeptr->addonname, Info_ValueForKey(info, "addonname"), 22);
	Q_CleanStr(servernodeptr->addonname);
	Q_strupr(servernodeptr->addonname);

	servernodeptr->numclients = atoi(Info_ValueForKey(info, "clients"));
	servernodeptr->maxclients = atoi(Info_ValueForKey(info, "g_maxClients"));
	servernodeptr->pingtime = pingtime;
	servernodeptr->minPing = atoi(Info_ValueForKey(info, "minPing"));
	servernodeptr->maxPing = atoi(Info_ValueForKey(info, "maxPing"));

	servernodeptr->nettype = atoi(Info_ValueForKey(info, "nettype"));

	i = atoi(Info_ValueForKey(info, "gametype"));
	servernodeptr->gametype = i;
	Q_strncpyz(servernodeptr->gamename, gametype_items[i], sizeof(servernodeptr->gamename));
}

static void ArenaServers_StopRefresh(void) {
	if(!g_arenaservers.refreshservers) return;

	g_arenaservers.refreshservers = qfalse;

	// final tally
	if(g_arenaservers.numqueriedservers >= 0) {
		g_arenaservers.currentping = *g_arenaservers.numservers;
		g_arenaservers.numqueriedservers = *g_arenaservers.numservers;
	}

	ArenaServers_UpdateMenu();
}

static void ArenaServers_DoRefresh(void) {
	int i;
	int j;
	int time;
	int maxPing;
	char adrstr[MAX_ADDRESSLENGTH];
	char info[MAX_INFO_STRING];

	if(uis.realtime < g_arenaservers.refreshtime) {
		if(g_servertype == AS_LOCAL) {
			if(!trap_LAN_GetServerCount(g_servertype)) return;
		}
		if(trap_LAN_GetServerCount(g_servertype) < 0) return; //still waiting for response
	}

	if(uis.realtime < g_arenaservers.nextpingtime) return; //wait for time trigger

	// trigger at 10Hz intervals
	g_arenaservers.nextpingtime = uis.realtime + 10;

	// process ping results
	maxPing = ArenaServers_MaxPing();
	for(i = 0; i < MAX_PINGREQUESTS; i++) {
		trap_LAN_GetPing(i, adrstr, MAX_ADDRESSLENGTH, &time);
		if(!adrstr[0]) continue; //ignore empty or pending pings

		// find ping result in our local list
		for(j = 0; j < MAX_PINGREQUESTS; j++)
			if(!Q_stricmp(adrstr, g_arenaservers.pinglist[j].adrstr)) break;

		if(j < MAX_PINGREQUESTS) {
			// found it
			if(!time) {
				time = uis.realtime - g_arenaservers.pinglist[j].start;
				if(time < maxPing) continue; //still waiting
			}

			if(time > maxPing) { // stale it out
				info[0] = '\0';
				time = maxPing;
			} else {
				trap_LAN_GetPingInfo(i, info, MAX_INFO_STRING);
			}

			// insert ping results
			ArenaServers_Insert(adrstr, info, time);

			// clear this query from internal list
			g_arenaservers.pinglist[j].adrstr[0] = '\0';
		}

		// clear this query from external list
		trap_LAN_ClearPing(i);
	}

	// get results of servers query
	// counts can increase as servers respond
	g_arenaservers.numqueriedservers = trap_LAN_GetServerCount(g_servertype);

	// send ping requests in reasonable bursts
	// iterate ping through all found servers
	for(i = 0; i < MAX_PINGREQUESTS && g_arenaservers.currentping < g_arenaservers.numqueriedservers; i++) {
		if(trap_LAN_GetPingQueueCount() >= MAX_PINGREQUESTS) break; //ping queue is full

		// find empty slot
		for(j = 0; j < MAX_PINGREQUESTS; j++)
			if(!g_arenaservers.pinglist[j].adrstr[0]) break;

		if(j >= MAX_PINGREQUESTS) break; //no empty slots available yet - wait for timeout

		// get an address to ping
		trap_LAN_GetServerAddressString(g_servertype, g_arenaservers.currentping, adrstr, MAX_ADDRESSLENGTH);

		strcpy(g_arenaservers.pinglist[j].adrstr, adrstr);
		g_arenaservers.pinglist[j].start = uis.realtime;

		trap_Cmd_ExecuteText(EXEC_NOW, va("ping %s\n", adrstr));

		// advance to next server
		g_arenaservers.currentping++;
	}

	if(!trap_LAN_GetPingQueueCount()) {
		// all pings completed
		ArenaServers_StopRefresh();
		return;
	}

	// update the user interface with ping status
	ArenaServers_UpdateMenu();
}

static void ArenaServers_StartRefresh(void) {
	int i;
	char myargs[32];

	memset(g_arenaservers.serverlist, 0, g_arenaservers.maxservers * sizeof(table_t));

	for(i = 0; i < MAX_PINGREQUESTS; i++) {
		g_arenaservers.pinglist[i].adrstr[0] = '\0';
		trap_LAN_ClearPing(i);
	}

	g_arenaservers.refreshservers = qtrue;
	g_arenaservers.currentping = 0;
	g_arenaservers.nextpingtime = 0;
	*g_arenaservers.numservers = 0;
	g_arenaservers.numqueriedservers = 0;

	// allow max 5 seconds for responses
	g_arenaservers.refreshtime = uis.realtime + 5000;

	// place menu in zeroed state
	ArenaServers_UpdateMenu();

	if(g_servertype == AS_LOCAL) {
		trap_Cmd_ExecuteText(EXEC_APPEND, "localservers\n");
		return;
	}

	if(g_servertype == AS_GLOBAL) {
		i = 0;
		trap_Cmd_ExecuteText(EXEC_APPEND, va("globalservers %d %d%s\n", i, (int)trap_Cvar_VariableValue("protocol"), myargs));
	}
}

static void ArenaServers_SetType(int type) {
	if(g_servertype == type) return;

	g_servertype = type;

	trap_Print(va("%i", type));

	switch(type) {
		default:
		case AS_LOCAL:
			g_arenaservers.serverlist = g_localserverlist;
			g_arenaservers.numservers = &g_numlocalservers;
			g_arenaservers.maxservers = MAX_LOCALSERVERS;
			break;

		case AS_GLOBAL:
			g_arenaservers.serverlist = g_globalserverlist;
			g_arenaservers.numservers = &g_numglobalservers;
			g_arenaservers.maxservers = MAX_GLOBALSERVERS;
			break;
	}

	if(!*g_arenaservers.numservers) {
		ArenaServers_StartRefresh();
	} else {
		// avoid slow operation, use existing results
		g_arenaservers.currentping = *g_arenaservers.numservers;
		g_arenaservers.numqueriedservers = *g_arenaservers.numservers;
		ArenaServers_UpdateMenu();
	}
	strcpy(g_arenaservers.status.string, "hit Refresh to update");
}

static void ArenaServers_Event(void* ptr, int event) {
	int id;
	int value;

	id = ((menucommon_s*)ptr)->id;

	if(event != QM_ACTIVATED && id != ID_LIST) return;

	switch(id) {
		case ID_MASTER:
			value = g_arenaservers.master.curvalue;
			trap_Cvar_SetValue("ui_browserMaster", value);
			ArenaServers_SetType(value);
			break;

		case ID_CONNECT: trap_Cmd_ExecuteText( EXEC_INSERT, va("connect %s\n", g_arenaservers.domain.field.buffer)); break;

		case ID_LIST:
			if(event == QM_GOTFOCUS) ArenaServers_UpdatePicture();
			if(event == QM_ACTIVATED) ArenaServers_Go();
			break;

		case ID_REFRESH: ArenaServers_StartRefresh(); break;
	}
}

static void ArenaServers_MenuDraw(void) {
	if(g_arenaservers.refreshservers) ArenaServers_DoRefresh();

	Menu_Draw(&g_arenaservers.menu);
}

static void ArenaServers_MenuInit(void) {
	int i, y, value, type;
	static char statusbuffer[MAX_STATUSLENGTH];

	// zero set all our globals
	memset(&g_arenaservers, 0, sizeof(arenaservers_t));

	trap_R_RegisterShaderNoMip(ART_UNKNOWNMAP);

	g_arenaservers.menu.fullscreen = qtrue;
	g_arenaservers.menu.draw = ArenaServers_MenuDraw;

	g_arenaservers.status.generic.type = MTYPE_TEXT;
	g_arenaservers.status.generic.x = 320;
	g_arenaservers.status.generic.y = 15;
	g_arenaservers.status.string = statusbuffer;
	g_arenaservers.status.style = UI_CENTER;
	g_arenaservers.status.color = color_white;

	g_arenaservers.master.generic.type = MTYPE_SPINCONTROL;
	g_arenaservers.master.generic.flags = QMF_PULSEIFFOCUS;
	g_arenaservers.master.generic.callback = ArenaServers_Event;
	g_arenaservers.master.generic.id = ID_MASTER;
	g_arenaservers.master.string = "Servers:";
	g_arenaservers.master.itemnames = master_items;
	g_arenaservers.master.generic.x = 160;
	g_arenaservers.master.generic.y = 32;

	g_arenaservers.domain.generic.type = MTYPE_FIELD;
	g_arenaservers.domain.string = "Address:";
	g_arenaservers.domain.generic.flags = QMF_PULSEIFFOCUS;
	g_arenaservers.domain.generic.x = 160;
	g_arenaservers.domain.generic.y	= 44;
	g_arenaservers.domain.field.widthInChars = 38;
	g_arenaservers.domain.field.maxchars = 80;

	g_arenaservers.connect.generic.type = MTYPE_PTEXT;
	g_arenaservers.connect.string = "Connect";
	g_arenaservers.connect.generic.flags = QMF_PULSEIFFOCUS;
	g_arenaservers.connect.generic.callback = ArenaServers_Event;
	g_arenaservers.connect.generic.id = ID_CONNECT;
	g_arenaservers.connect.generic.x = 160;
	g_arenaservers.connect.generic.y = 56;
	g_arenaservers.connect.size = 1.00;
	g_arenaservers.connect.color = color_white;

	y += 3 * BASEFONT_HEIGHT;
	g_arenaservers.list.generic.type = MTYPE_SCROLLLIST;
	g_arenaservers.list.generic.flags = QMF_HIGHLIGHT_IF_FOCUS;
	g_arenaservers.list.generic.id = ID_LIST;
	g_arenaservers.list.generic.callback = ArenaServers_Event;
	g_arenaservers.list.generic.x = 0;
	g_arenaservers.list.generic.y = 88;
	g_arenaservers.list.width = MAX_LISTBOXWIDTH;
	g_arenaservers.list.height = 30;
	g_arenaservers.list.itemnames = (const char**)g_arenaservers.items;
	for(i = 0; i < MAX_LISTBOXITEMS; i++) {
		g_arenaservers.items[i] = g_arenaservers.table[i].buff;
	}

	g_arenaservers.mappic.generic.type = MTYPE_BITMAP;
	g_arenaservers.mappic.generic.flags = QMF_LEFT_JUSTIFY|QMF_INACTIVE;
	g_arenaservers.mappic.generic.x = 0;
	g_arenaservers.mappic.generic.y = 8;
	g_arenaservers.mappic.width = 100;
	g_arenaservers.mappic.height = 75;
	g_arenaservers.mappic.errorpic = ART_UNKNOWNMAP;

	g_arenaservers.refresh.generic.type = MTYPE_PTEXT;
	g_arenaservers.refresh.string = "Refresh";
	g_arenaservers.refresh.generic.flags = QMF_PULSEIFFOCUS|QMF_CENTER_JUSTIFY;
	g_arenaservers.refresh.generic.callback = ArenaServers_Event;
	g_arenaservers.refresh.generic.id = ID_REFRESH;
	g_arenaservers.refresh.generic.x = 320;
	g_arenaservers.refresh.generic.y = 450;
	g_arenaservers.refresh.size = 1.50;
	g_arenaservers.refresh.style = UI_CENTER;
	g_arenaservers.refresh.color = color_white;

	Menu_AddItem(&g_arenaservers.menu, (void*)&g_arenaservers.status);
	Menu_AddItem(&g_arenaservers.menu, (void*)&g_arenaservers.master);
	Menu_AddItem(&g_arenaservers.menu, (void*)&g_arenaservers.domain);
	Menu_AddItem(&g_arenaservers.menu, (void*)&g_arenaservers.connect);
	Menu_AddItem(&g_arenaservers.menu, (void*)&g_arenaservers.list);
	Menu_AddItem(&g_arenaservers.menu, (void*)&g_arenaservers.mappic);
	Menu_AddItem(&g_arenaservers.menu, (void*)&g_arenaservers.refresh);

	g_servertype = 0;
	value = g_servertype;
	g_arenaservers.master.curvalue = value;

	// force to initial state and refresh
	type = g_servertype;
	g_servertype = -1;
	ArenaServers_SetType(type);
}

void UI_ArenaServersMenu(void) {
	ArenaServers_MenuInit();
	UI_PushMenu(&g_arenaservers.menu);
}
