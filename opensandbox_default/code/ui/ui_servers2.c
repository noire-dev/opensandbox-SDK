// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

#include "ui_local.h"


#define MAX_GLOBALSERVERS		128
#define MAX_PINGREQUESTS		32
#define MAX_ADDRESSLENGTH		64
#define MAX_HOSTNAMELENGTH		22
#define MAX_ADDONNAMELENGTH		32
#define MAX_MAPNAMELENGTH		32
#define MAX_LISTBOXITEMS		128
#define MAX_LOCALSERVERS		128
#define MAX_STATUSLENGTH		64
#define MAX_LEAGUELENGTH		28
#define MAX_LISTBOXWIDTH		82

#define ART_SPECIFY0			"menu/assets/specify_0"
#define ART_SPECIFY1			"menu/assets/specify_1"
#define ART_REFRESH0			"menu/assets/refresh_0"
#define ART_REFRESH1			"menu/assets/refresh_1"
#define ART_CONNECT0			"menu/assets/fight_0"
#define ART_CONNECT1			"menu/assets/fight_1"
#define ART_UNKNOWNMAP			"menu/assets/unknownmap"
#define ART_REMOVE0				"menu/assets/delete_0"
#define ART_REMOVE1				"menu/assets/delete_1"
#define ART_SAVE0   			"menu/assets/save_0"
#define ART_SAVE1   			"menu/assets/save_1"

#define ID_MASTER			10
#define ID_LIST				15
#define ID_SCROLL_UP		16
#define ID_SCROLL_DOWN		17
#define ID_REFRESH			19
#define ID_SPECIFY			20
#define ID_CONNECT			22
#define ID_REMOVE			23
#define ID_SAVE 			24

#define GR_LOGO				30
#define GR_LETTERS			31

#define AS_LOCAL			0
#define AS_GLOBAL			1
#define AS_FAVORITES		2

static const char *master_items[] = {
	"Local",
	"Internet",
	"Favorites",
	0
};

static char* netnames[] = {
	"???",
	"IPv4",
	"IPv6",
	NULL
};

static char opensandboxMessage[] = "Visit https://opensandbox.neocities.org/ - News, Community, Events, Files";

typedef struct {
	char adrstr[MAX_ADDRESSLENGTH];
	int start;
} pinglist_t;

typedef struct servernode_s {
	char adrstr[MAX_ADDRESSLENGTH];
	char hostname[MAX_HOSTNAMELENGTH + 3];
	char addonname[MAX_ADDONNAMELENGTH];
	char mapname[MAX_MAPNAMELENGTH];
	int numclients;
	int maxclients;
	int pingtime;
	int gametype;
	char gamename[30];
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

	menuelement_s banner;

	menuelement_s master;

	menuelement_s list;
	menuelement_s mappic;
	menuelement_s status;
	menuelement_s statusbar;
	menuelement_s save;

	menuelement_s remove;
	menuelement_s refresh;
	menuelement_s specify;
	menuelement_s go;

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
	char favoriteaddresses[MAX_FAVORITESERVERS][MAX_ADDRESSLENGTH];
	int numfavoriteaddresses;
} arenaservers_t;

static arenaservers_t g_arenaservers;
static servernode_t g_globalserverlist[MAX_GLOBALSERVERS];
static int g_numglobalservers;
static servernode_t g_localserverlist[MAX_LOCALSERVERS];
static int g_numlocalservers;
static servernode_t g_favoriteserverlist[MAX_FAVORITESERVERS];
static int g_numfavoriteservers;
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
		trap_Cmd_ExecuteText(EXEC_APPEND, va("set cl_selectedmod %s\n", servernode->addonname));
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
		Com_sprintf(picname, sizeof(picname), "levelshots/%s.tga", servernodeptr->mapname);
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
	char* pingColor;

	if(g_arenaservers.numqueriedservers > 0) {
		// servers found
		if(g_arenaservers.refreshservers && (g_arenaservers.currentping <= g_arenaservers.numqueriedservers)) {
			// show progress
			Com_sprintf(g_arenaservers.status.string, MAX_STATUSLENGTH, "%d of %d Servers.", g_arenaservers.currentping, g_arenaservers.numqueriedservers);
			g_arenaservers.statusbar.string = "Press SPACE to stop";
		} else {
			// all servers pinged - enable controls
			g_arenaservers.save.generic.flags &= ~QMF_GRAYED;
			g_arenaservers.master.generic.flags &= ~QMF_GRAYED;
			g_arenaservers.list.generic.flags &= ~QMF_GRAYED;
			g_arenaservers.refresh.generic.flags &= ~QMF_GRAYED;
			g_arenaservers.go.generic.flags &= ~QMF_GRAYED;

			// update status bar
			if(g_servertype == AS_GLOBAL) {
				g_arenaservers.statusbar.string = opensandboxMessage;
			} else {
				g_arenaservers.statusbar.string = "";
			}
		}
	} else {
		// no servers found
		if(g_arenaservers.refreshservers) {
			strcpy(g_arenaservers.status.string, "Scanning For Servers...");
			g_arenaservers.statusbar.string = "Press SPACE to stop";

			// disable controls during refresh
			g_arenaservers.save.generic.flags |= QMF_GRAYED;
			g_arenaservers.master.generic.flags |= QMF_GRAYED;
			g_arenaservers.list.generic.flags |= QMF_GRAYED;
			g_arenaservers.refresh.generic.flags |= QMF_GRAYED;
			g_arenaservers.go.generic.flags |= QMF_GRAYED;
		} else {
			if(g_arenaservers.numqueriedservers < 0) {
				strcpy(g_arenaservers.status.string, "No Response From Master Server.");
			} else {
				strcpy(g_arenaservers.status.string, "No Servers Found.");
			}

			// update status bar
			if(g_servertype == AS_GLOBAL) {
				g_arenaservers.statusbar.string = opensandboxMessage;
			} else {
				g_arenaservers.statusbar.string = "";
			}

			// end of refresh - set control state
			g_arenaservers.save.generic.flags |= QMF_GRAYED;
			g_arenaservers.master.generic.flags &= ~QMF_GRAYED;
			g_arenaservers.list.generic.flags |= QMF_GRAYED;
			g_arenaservers.refresh.generic.flags &= ~QMF_GRAYED;
			g_arenaservers.go.generic.flags |= QMF_GRAYED;
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
		if(!servernodeptr->numclients) {
			continue;
		}

		if(servernodeptr->numclients == servernodeptr->maxclients) {
			continue;
		}

		if(servernodeptr->pingtime < servernodeptr->minPing) {
			pingColor = S_COLOR_BLUE;
		} else if(servernodeptr->maxPing && servernodeptr->pingtime > servernodeptr->maxPing) {
			pingColor = S_COLOR_BLUE;
		} else if(servernodeptr->pingtime < 200) {
			pingColor = S_COLOR_GREEN;
		} else if(servernodeptr->pingtime < 400) {
			pingColor = S_COLOR_YELLOW;
		} else {
			pingColor = S_COLOR_RED;
		}
		Com_sprintf(buff, MAX_LISTBOXWIDTH, "%-20.20s %-12.12s %2d/%2d %-30.30s %3s %s%3d", servernodeptr->hostname, servernodeptr->mapname, servernodeptr->numclients, servernodeptr->maxclients, servernodeptr->gamename, netnames[servernodeptr->nettype], pingColor, servernodeptr->pingtime);
		j++;
	}

	//	Com_sprintf( g_arenaservers.status.string, MAX_STATUSLENGTH, "%d of %d Arena Servers.", j, *g_arenaservers.numservers );

	g_arenaservers.list.numitems = j;
	g_arenaservers.list.curvalue = 0;
	g_arenaservers.list.top = 0;

	// update picture
	ArenaServers_UpdatePicture();
}

static void ArenaServers_Remove(void) {
	int i;
	servernode_t* servernodeptr;
	table_t* tableptr;

	if(!g_arenaservers.list.numitems) return;

	// remove selected item from display list
	// items are in scattered order due to sort and cull
	// perform delete on list box contents, resync all lists

	tableptr = &g_arenaservers.table[g_arenaservers.list.curvalue];
	servernodeptr = tableptr->servernode;

	// find address in master list
	for(i = 0; i < g_arenaservers.numfavoriteaddresses; i++)
		if(!Q_stricmp(g_arenaservers.favoriteaddresses[i], servernodeptr->adrstr)) break;

	// delete address from master list
	if(i <= g_arenaservers.numfavoriteaddresses - 1) {
		if(i < g_arenaservers.numfavoriteaddresses - 1) {
			// shift items up
			memcpy(&g_arenaservers.favoriteaddresses[i], &g_arenaservers.favoriteaddresses[i + 1], (g_arenaservers.numfavoriteaddresses - i - 1) * MAX_ADDRESSLENGTH);
		}
		g_arenaservers.numfavoriteaddresses--;
	}

	// find address in server list
	for(i = 0; i < g_numfavoriteservers; i++)
		if(&g_favoriteserverlist[i] == servernodeptr) break;

	// delete address from server list
	if(i <= g_numfavoriteservers - 1) {
		if(i < g_numfavoriteservers - 1) {
			// shift items up
			memcpy(&g_favoriteserverlist[i], &g_favoriteserverlist[i + 1], (g_numfavoriteservers - i - 1) * sizeof(servernode_t));
		}
		g_numfavoriteservers--;
	}

	g_arenaservers.numqueriedservers = g_arenaservers.numfavoriteaddresses;
	g_arenaservers.currentping = g_arenaservers.numfavoriteaddresses;
}

static void ArenaServers_Insert(char* adrstr, char* info, int pingtime) {
	servernode_t* servernodeptr;
	char* s;
	int i;

	if((pingtime >= ArenaServers_MaxPing()) && (g_servertype != AS_FAVORITES)) {
		// slow global or local servers do not get entered
		return;
	}

	if(*g_arenaservers.numservers >= g_arenaservers.maxservers) {
		// list full;
		servernodeptr = g_arenaservers.serverlist + (*g_arenaservers.numservers) - 1;
	} else {
		// next slot
		servernodeptr = g_arenaservers.serverlist + (*g_arenaservers.numservers);
		(*g_arenaservers.numservers)++;
	}

	Q_strncpyz(servernodeptr->adrstr, adrstr, MAX_ADDRESSLENGTH);

	Q_strncpyz(servernodeptr->hostname, Info_ValueForKey(info, "hostname"), MAX_HOSTNAMELENGTH);
	Q_CleanStr(servernodeptr->hostname);
	Q_strupr(servernodeptr->hostname);

	Q_strncpyz(servernodeptr->mapname, Info_ValueForKey(info, "mapname"), MAX_MAPNAMELENGTH);
	Q_CleanStr(servernodeptr->mapname);
	Q_strupr(servernodeptr->mapname);

	Q_strncpyz(servernodeptr->addonname, Info_ValueForKey(info, "addonname"), MAX_ADDONNAMELENGTH);
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

/*
=================
ArenaServers_InsertFavorites

Insert nonresponsive address book entries into display lists.
=================
*/
void ArenaServers_InsertFavorites(void) {
	int i;
	int j;
	char info[MAX_INFO_STRING];

	// resync existing results with new or deleted cvars
	info[0] = '\0';
	Info_SetValueForKey(info, "hostname", "No connection");
	for(i = 0; i < g_arenaservers.numfavoriteaddresses; i++) {
		// find favorite address in refresh list
		for(j = 0; j < g_numfavoriteservers; j++)
			if(!Q_stricmp(g_arenaservers.favoriteaddresses[i], g_favoriteserverlist[j].adrstr)) break;

		if(j >= g_numfavoriteservers) {
			// not in list, add it
			ArenaServers_Insert(g_arenaservers.favoriteaddresses[i], info, ArenaServers_MaxPing());
		}
	}
}

/*
=================
ArenaServers_LoadFavorites

Load cvar address book entries into local lists.
=================
*/
void ArenaServers_LoadFavorites(void) {
	int i;
	int j;
	int numtempitems;
	char emptyinfo[MAX_INFO_STRING];
	char adrstr[MAX_ADDRESSLENGTH];
	servernode_t templist[MAX_FAVORITESERVERS];
	qboolean found;

	found = qfalse;
	emptyinfo[0] = '\0';

	// copy the old
	memcpy(templist, g_favoriteserverlist, sizeof(servernode_t) * MAX_FAVORITESERVERS);
	numtempitems = g_numfavoriteservers;

	// clear the current for sync
	memset(g_favoriteserverlist, 0, sizeof(servernode_t) * MAX_FAVORITESERVERS);
	g_numfavoriteservers = 0;

	// resync existing results with new or deleted cvars
	for(i = 0; i < MAX_FAVORITESERVERS; i++) {
		trap_Cvar_VariableStringBuffer(va("server%d", i + 1), adrstr, MAX_ADDRESSLENGTH);
		if(!adrstr[0]) continue;

		// quick sanity check to avoid slow domain name resolving
		// first character must be numeric
		if(adrstr[0] < '0' || adrstr[0] > '9') continue;

		// favorite server addresses must be maintained outside refresh list
		// this mimics local and global netadr's stored in client
		// these can be fetched to fill ping list
		strcpy(g_arenaservers.favoriteaddresses[g_numfavoriteservers], adrstr);

		// find this server in the old list
		for(j = 0; j < numtempitems; j++)
			if(!Q_stricmp(templist[j].adrstr, adrstr)) break;

		if(j < numtempitems) {
			// found server - add exisiting results
			memcpy(&g_favoriteserverlist[g_numfavoriteservers], &templist[j], sizeof(servernode_t));
			found = qtrue;
		} else {
			// add new server
			Q_strncpyz(g_favoriteserverlist[g_numfavoriteservers].adrstr, adrstr, MAX_ADDRESSLENGTH);
			g_favoriteserverlist[g_numfavoriteservers].pingtime = ArenaServers_MaxPing();
		}

		g_numfavoriteservers++;
	}

	g_arenaservers.numfavoriteaddresses = g_numfavoriteservers;

	if(!found) {
		// no results were found, reset server list
		// list will be automatically refreshed when selected
		g_numfavoriteservers = 0;
	}
}

static void ArenaServers_StopRefresh(void) {
	if(!g_arenaservers.refreshservers)
		// not currently refreshing
		return;

	g_arenaservers.refreshservers = qfalse;

	if(g_servertype == AS_FAVORITES) {
		// nonresponsive favorites must be shown
		ArenaServers_InsertFavorites();
	}

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
		if(g_servertype != AS_FAVORITES) {
			if(g_servertype == AS_LOCAL) {
				if(!trap_LAN_GetServerCount(g_servertype)) {
					return;
				}
			}
			if(trap_LAN_GetServerCount(g_servertype) < 0) {
				// still waiting for response
				return;
			}
		}
	}

	if(uis.realtime < g_arenaservers.nextpingtime) {
		// wait for time trigger
		return;
	}

	// trigger at 10Hz intervals
	g_arenaservers.nextpingtime = uis.realtime + 10;

	// process ping results
	maxPing = ArenaServers_MaxPing();
	for(i = 0; i < MAX_PINGREQUESTS; i++) {
		trap_LAN_GetPing(i, adrstr, MAX_ADDRESSLENGTH, &time);
		if(!adrstr[0]) {
			// ignore empty or pending pings
			continue;
		}

		// find ping result in our local list
		for(j = 0; j < MAX_PINGREQUESTS; j++)
			if(!Q_stricmp(adrstr, g_arenaservers.pinglist[j].adrstr)) break;

		if(j < MAX_PINGREQUESTS) {
			// found it
			if(!time) {
				time = uis.realtime - g_arenaservers.pinglist[j].start;
				if(time < maxPing) {
					// still waiting
					continue;
				}
			}

			if(time > maxPing) {
				// stale it out
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
	if(g_servertype == AS_FAVORITES) {
		g_arenaservers.numqueriedservers = g_arenaservers.numfavoriteaddresses;
	} else {
		g_arenaservers.numqueriedservers = trap_LAN_GetServerCount(g_servertype);
	}

	// send ping requests in reasonable bursts
	// iterate ping through all found servers
	for(i = 0; i < MAX_PINGREQUESTS && g_arenaservers.currentping < g_arenaservers.numqueriedservers; i++) {
		if(trap_LAN_GetPingQueueCount() >= MAX_PINGREQUESTS) {
			// ping queue is full
			break;
		}

		// find empty slot
		for(j = 0; j < MAX_PINGREQUESTS; j++)
			if(!g_arenaservers.pinglist[j].adrstr[0]) break;

		if(j >= MAX_PINGREQUESTS)
			// no empty slots available yet - wait for timeout
			break;

		// get an address to ping

		if(g_servertype == AS_FAVORITES) {
			strcpy(adrstr, g_arenaservers.favoriteaddresses[g_arenaservers.currentping]);
		} else {
			trap_LAN_GetServerAddressString(g_servertype, g_arenaservers.currentping, adrstr, MAX_ADDRESSLENGTH);
		}

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
	char myargs[32], protocol[32];

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

void ArenaServers_SaveChanges(void) {
	int i;

	for(i = 0; i < g_arenaservers.numfavoriteaddresses; i++) trap_Cvar_Set(va("server%d", i + 1), g_arenaservers.favoriteaddresses[i]);

	for(; i < MAX_FAVORITESERVERS; i++) trap_Cvar_Set(va("server%d", i + 1), "");
}

void ArenaServers_SetType(int type) {
	if(g_servertype == type) return;

	g_servertype = type;

	trap_Print(va("%i", type));

	switch(type) {
		default:
		case AS_LOCAL:
			g_arenaservers.save.generic.flags &= ~(QMF_INACTIVE | QMF_HIDDEN);
			g_arenaservers.remove.generic.flags |= (QMF_INACTIVE | QMF_HIDDEN);
			g_arenaservers.serverlist = g_localserverlist;
			g_arenaservers.numservers = &g_numlocalservers;
			g_arenaservers.maxservers = MAX_LOCALSERVERS;
			break;

		case AS_GLOBAL:
			g_arenaservers.save.generic.flags &= ~(QMF_INACTIVE | QMF_HIDDEN);
			g_arenaservers.remove.generic.flags |= (QMF_INACTIVE | QMF_HIDDEN);
			g_arenaservers.serverlist = g_globalserverlist;
			g_arenaservers.numservers = &g_numglobalservers;
			g_arenaservers.maxservers = MAX_GLOBALSERVERS;
			break;

		case AS_FAVORITES:
			g_arenaservers.save.generic.flags |= (QMF_INACTIVE | QMF_HIDDEN);
			g_arenaservers.remove.generic.flags &= ~(QMF_INACTIVE | QMF_HIDDEN);
			g_arenaservers.serverlist = g_favoriteserverlist;
			g_arenaservers.numservers = &g_numfavoriteservers;
			g_arenaservers.maxservers = MAX_FAVORITESERVERS;
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

static void ArenaServers_AddToFavorites(void) {
	servernode_t* servernodeptr;
	int i;

	// check favourite server list isn't full
	if(g_numfavoriteservers == MAX_FAVORITESERVERS) return;

	// check we have a server list available
	if(!g_arenaservers.list.numitems) return;

	// check the server isn't on the favourites list already
	servernodeptr = g_arenaservers.table[g_arenaservers.list.curvalue].servernode;
	for(i = 0; i < g_numfavoriteservers; i++)
		if(!Q_stricmp(g_arenaservers.favoriteaddresses[i], servernodeptr->adrstr)) return;

	// we already have a responsive server, no further sanity checks required
	strcpy(g_arenaservers.favoriteaddresses[g_numfavoriteservers], servernodeptr->adrstr);

	// copy over server details
	memcpy(&g_favoriteserverlist[g_numfavoriteservers], servernodeptr, sizeof(servernode_t));

	g_numfavoriteservers++;
	g_arenaservers.numfavoriteaddresses = g_numfavoriteservers;
}

static void ArenaServers_Event(void* ptr, int event) {
	int id;
	int value;

	id = ((menucommon_s*)ptr)->id;

	if(event != QM_ACTIVATED && id != ID_LIST) {
		return;
	}

	switch(id) {
		case ID_MASTER:
			value = g_arenaservers.master.curvalue;
			trap_Cvar_SetValue("ui_browserMaster", value);
			ArenaServers_SetType(value);
			break;

		case ID_LIST:
			if(event == QM_GOTFOCUS) {
				ArenaServers_UpdatePicture();
			}
			break;

		case ID_SAVE:
			ArenaServers_AddToFavorites();
			ArenaServers_SaveChanges();
			break;

		case ID_REFRESH: ArenaServers_StartRefresh(); break;

		case ID_SPECIFY: UI_SpecifyServerMenu(); break;

		case ID_CONNECT: ArenaServers_Go(); break;

		case ID_REMOVE:
			ArenaServers_Remove();
			ArenaServers_UpdateMenu();
			break;
	}
}

static void ArenaServers_MenuDraw(void) {
	if(g_arenaservers.refreshservers) ArenaServers_DoRefresh();

	Menu_Draw(&g_arenaservers.menu);
}

static sfxHandle_t ArenaServers_MenuKey(int key) {
	if(key == K_SPACE && g_arenaservers.refreshservers) {
		ArenaServers_StopRefresh();
		return menu_move_sound;
	}

	if((key == K_DEL || key == K_KP_DEL) && (g_servertype == AS_FAVORITES) && (Menu_ItemAtCursor(&g_arenaservers.menu) == &g_arenaservers.list)) {
		ArenaServers_Remove();
		ArenaServers_UpdateMenu();
		return menu_move_sound;
	}

	if(key == K_MOUSE2 || key == K_ESCAPE) {
		ArenaServers_StopRefresh();
		ArenaServers_SaveChanges();
	}

	return Menu_DefaultKey(&g_arenaservers.menu, key);
}

static void ArenaServers_MenuInit(void) {
	int i;
	int type;
	int y;
	int value;
	static char statusbuffer[MAX_STATUSLENGTH];

	// zero set all our globals
	memset(&g_arenaservers, 0, sizeof(arenaservers_t));

	ArenaServers_Cache();

	g_arenaservers.menu.fullscreen = qtrue;
	g_arenaservers.menu.draw = ArenaServers_MenuDraw;
	g_arenaservers.menu.key = ArenaServers_MenuKey;

	g_arenaservers.banner.generic.type = MTYPE_TEXT;
	g_arenaservers.banner.generic.flags = QMF_CENTER_JUSTIFY;
	g_arenaservers.banner.generic.x = 320;
	g_arenaservers.banner.generic.y = 16;
	g_arenaservers.banner.style = UI_CENTER;
	g_arenaservers.banner.color = color_white;

	y = 80;
	g_arenaservers.master.generic.type = MTYPE_SPINCONTROL;
	g_arenaservers.master.generic.flags = QMF_PULSEIFFOCUS | QMF_SMALLFONT;
	g_arenaservers.master.generic.callback = ArenaServers_Event;
	g_arenaservers.master.generic.id = ID_MASTER;
	g_arenaservers.master.generic.x = 320;
	g_arenaservers.master.generic.y = y;

	y += 3 * BASEFONT_HEIGHT;
	g_arenaservers.list.generic.type = MTYPE_SCROLLLIST;
	g_arenaservers.list.generic.flags = QMF_HIGHLIGHT_IF_FOCUS;
	g_arenaservers.list.generic.id = ID_LIST;
	g_arenaservers.list.generic.callback = ArenaServers_Event;
	g_arenaservers.list.generic.x = 22 - uis.wideoffset;
	g_arenaservers.list.generic.y = y;
	g_arenaservers.list.width = MAX_LISTBOXWIDTH;
	g_arenaservers.list.height = 11;
	g_arenaservers.list.itemnames = (const char**)g_arenaservers.items;
	for(i = 0; i < MAX_LISTBOXITEMS; i++) {
		g_arenaservers.items[i] = g_arenaservers.table[i].buff;
	}

	g_arenaservers.mappic.generic.type = MTYPE_BITMAP;
	g_arenaservers.mappic.generic.flags = QMF_LEFT_JUSTIFY | QMF_INACTIVE;
	g_arenaservers.mappic.generic.x = 22 - uis.wideoffset;
	g_arenaservers.mappic.generic.y = 80 - 12;
	g_arenaservers.mappic.width = 128;
	g_arenaservers.mappic.height = 96;
	g_arenaservers.mappic.errorpic = ART_UNKNOWNMAP;

	y = 376;
	g_arenaservers.status.generic.type = MTYPE_TEXT;
	g_arenaservers.status.generic.x = 320;
	g_arenaservers.status.generic.y = y;
	g_arenaservers.status.string = statusbuffer;
	g_arenaservers.status.style = UI_CENTER | UI_SMALLFONT;
	g_arenaservers.status.color = color_white;

	y += BASEFONT_HEIGHT;
	g_arenaservers.statusbar.generic.type = MTYPE_TEXT;
	g_arenaservers.statusbar.generic.x = 320;
	g_arenaservers.statusbar.generic.y = y;
	g_arenaservers.statusbar.string = "";
	g_arenaservers.statusbar.style = UI_CENTER | UI_SMALLFONT;
	g_arenaservers.statusbar.color = color_white;

	g_arenaservers.remove.generic.type = MTYPE_BITMAP;
	g_arenaservers.remove.string = ART_REMOVE0;
	g_arenaservers.remove.generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
	g_arenaservers.remove.generic.callback = ArenaServers_Event;
	g_arenaservers.remove.generic.id = ID_REMOVE;
	g_arenaservers.remove.generic.x = 450 + uis.wideoffset;
	g_arenaservers.remove.generic.y = 86;
	g_arenaservers.remove.width = 96;
	g_arenaservers.remove.height = 48;
	g_arenaservers.remove.focuspic = ART_REMOVE1;

	g_arenaservers.save.generic.type = MTYPE_BITMAP;
	g_arenaservers.save.string = ART_SAVE0;
	g_arenaservers.save.generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
	g_arenaservers.save.generic.callback = ArenaServers_Event;
	g_arenaservers.save.generic.id = ID_SAVE;
	g_arenaservers.save.generic.x = g_arenaservers.remove.generic.x;
	g_arenaservers.save.generic.y = g_arenaservers.remove.generic.y;
	g_arenaservers.save.width = g_arenaservers.remove.width;
	g_arenaservers.save.height = g_arenaservers.remove.height;
	g_arenaservers.save.focuspic = ART_SAVE1;

	g_arenaservers.specify.generic.type = MTYPE_BITMAP;
	g_arenaservers.specify.string = ART_SPECIFY0;
	g_arenaservers.specify.generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
	g_arenaservers.specify.generic.callback = ArenaServers_Event;
	g_arenaservers.specify.generic.id = ID_SPECIFY;
	g_arenaservers.specify.generic.x = 128 - uis.wideoffset / 2;
	g_arenaservers.specify.generic.y = 480 - 64;
	g_arenaservers.specify.width = 128;
	g_arenaservers.specify.height = 64;
	g_arenaservers.specify.focuspic = ART_SPECIFY1;

	g_arenaservers.refresh.generic.type = MTYPE_BITMAP;
	g_arenaservers.refresh.string = ART_REFRESH0;
	g_arenaservers.refresh.generic.flags = QMF_LEFT_JUSTIFY | QMF_PULSEIFFOCUS;
	g_arenaservers.refresh.generic.callback = ArenaServers_Event;
	g_arenaservers.refresh.generic.id = ID_REFRESH;
	g_arenaservers.refresh.generic.x = 256;
	g_arenaservers.refresh.generic.y = 480 - 64;
	g_arenaservers.refresh.width = 128;
	g_arenaservers.refresh.height = 64;
	g_arenaservers.refresh.focuspic = ART_REFRESH1;

	g_arenaservers.go.generic.type = MTYPE_BITMAP;
	g_arenaservers.go.string = ART_CONNECT0;
	g_arenaservers.go.generic.flags = QMF_RIGHT_JUSTIFY | QMF_PULSEIFFOCUS;
	g_arenaservers.go.generic.callback = ArenaServers_Event;
	g_arenaservers.go.generic.id = ID_CONNECT;
	g_arenaservers.go.generic.x = 640 + uis.wideoffset;
	g_arenaservers.go.generic.y = 480 - 64;
	g_arenaservers.go.width = 128;
	g_arenaservers.go.height = 64;
	g_arenaservers.go.focuspic = ART_CONNECT1;

	g_arenaservers.banner.string = "SERVERS";
	g_arenaservers.master.string = "Servers:";
	g_arenaservers.master.itemnames = master_items;

	Menu_AddItem(&g_arenaservers.menu, (void*)&g_arenaservers.banner);

	Menu_AddItem(&g_arenaservers.menu, (void*)&g_arenaservers.master);

	Menu_AddItem(&g_arenaservers.menu, (void*)&g_arenaservers.mappic);
	Menu_AddItem(&g_arenaservers.menu, (void*)&g_arenaservers.list);
	Menu_AddItem(&g_arenaservers.menu, (void*)&g_arenaservers.status);
	Menu_AddItem(&g_arenaservers.menu, (void*)&g_arenaservers.statusbar);
	Menu_AddItem(&g_arenaservers.menu, (void*)&g_arenaservers.save);

	Menu_AddItem(&g_arenaservers.menu, (void*)&g_arenaservers.remove);
	Menu_AddItem(&g_arenaservers.menu, (void*)&g_arenaservers.specify);
	Menu_AddItem(&g_arenaservers.menu, (void*)&g_arenaservers.refresh);
	Menu_AddItem(&g_arenaservers.menu, (void*)&g_arenaservers.go);

	ArenaServers_LoadFavorites();

	g_servertype = 0;
	value = g_servertype;
	g_arenaservers.master.curvalue = value;

	// force to initial state and refresh
	type = g_servertype;
	g_servertype = -1;
	ArenaServers_SetType(type);
}

void ArenaServers_Cache(void) {
	trap_R_RegisterShaderNoMip(ART_SPECIFY0);
	trap_R_RegisterShaderNoMip(ART_SPECIFY1);
	trap_R_RegisterShaderNoMip(ART_REFRESH0);
	trap_R_RegisterShaderNoMip(ART_REFRESH1);
	trap_R_RegisterShaderNoMip(ART_CONNECT0);
	trap_R_RegisterShaderNoMip(ART_CONNECT1);
	trap_R_RegisterShaderNoMip(ART_UNKNOWNMAP);
	trap_R_RegisterShaderNoMip(ART_SAVE0);
	trap_R_RegisterShaderNoMip(ART_SAVE1);
}

void UI_ArenaServersMenu(void) {
	ArenaServers_MenuInit();
	UI_PushMenu(&g_arenaservers.menu);
}
