// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// OpenSandbox — GPLv2; see LICENSE for details.

#include "g_local.h"

static int g_numBots;
static char *g_botInfos[MAX_BOTS];

static int G_ParseInfos(char *buf, int max, char *infos[]) {
	char *token;
	int count;
	char key[MAX_TOKEN_CHARS], info[MAX_INFO_STRING];

	count = 0;

	while(1) {
		token = COM_Parse(&buf);
		if(!token[0]) break;
		if(strcmp(token, "{")) { Com_Printf("Missing { in info file\n"); break; }
		if(count == max) { Com_Printf("Max infos exceeded\n"); break; }

		info[0] = '\0';
		while(1) {
			token = COM_ParseExt(&buf, qtrue);
			if(!token[0]) { Com_Printf("Unexpected end of info file\n"); break; }
			if(!strcmp(token, "}")) break;
			Q_strncpyz(key, token, sizeof(key));
			token = COM_ParseExt(&buf, qfalse);
			if(!token[0]) strcpy(token, "<NULL>");
			Info_SetValueForKey(info, key, token);
		}
		infos[count] = G_Alloc(strlen(info) + 1);
		if(infos[count]) {
			strcpy(infos[count], info);
			count++;
		}
	}
	return count;
}

qboolean G_BotConnect(int clientNum) {
	bot_settings_t settings;
	char userinfo[MAX_INFO_STRING];

	trap_GetUserinfo(clientNum, userinfo, sizeof(userinfo));

	Q_strncpyz(settings.team, Info_ValueForKey(userinfo, "team"), sizeof(settings.team));

	if(!BotAISetupClient(clientNum, &settings)) {
		trap_DropClient(clientNum, "BotAISetupClient failed");
		return qfalse;
	}

	return qtrue;
}

static void G_BotSandboxCheck(gentity_t *self) {
	if(!self->parent || !self->parent->client || self->parent->client->pers.connected == CON_DISCONNECTED) {
		G_FreeEntity(self);
		return;
	}
	VectorCopy(self->parent->s.pos.trBase, self->s.origin);
	self->think = G_BotSandboxCheck;
	self->nextthink = level.time + 1;
}

void G_AddBot(char *model, char *name, char *team, gentity_t *spawn) {
	int clientNum;
	gentity_t *bot;
	char userinfo[MAX_INFO_STRING];

	// create the bot's userinfo
	userinfo[0] = '\0';

	if(!strlen(name)) name = model;

	Info_SetValueForKey(userinfo, "name", name);
	Info_SetValueForKey(userinfo, "rate", "25000");
	Info_SetValueForKey(userinfo, "snaps", "20");

	if(!*model) model = "sarge/default";
	Info_SetValueForKey(userinfo, "model", model);

	// dynamic limit
	if(level.numConnectedClients >= cvarInt("g_maxClients")) {
		G_Printf(S_COLOR_YELLOW "Server is full, increase g_maxClients.\n");
		return;
	}

	// have the server allocate a client slot
	clientNum = trap_BotAllocateClient();
	if(clientNum == -1) {
		G_Printf(S_COLOR_YELLOW "Server is full, increase engine limit.\n");
		return;
	}

	// initialize the bot settings
	if(!team || !*team) {
		if(cvarInt("g_gametype") >= GT_TEAM) {
			if(PickTeam(clientNum) == TEAM_RED) {
				team = "red";
			} else {
				team = "blue";
			}
		} else {
			team = "red";
		}
	}
	Info_SetValueForKey(userinfo, "team", team);

	bot = &g_entities[clientNum];
	bot->r.svFlags |= SVF_BOT;
	bot->inuse = qtrue;

	// sandbox npc spawn
	if(spawn) {
		Info_SetValueForKey(userinfo, "spawnid", va("%i", spawn->s.number));
		Info_SetValueForKey(userinfo, "npcType", va("%i", spawn->type));
		spawn->parent = bot;
		spawn->think = G_BotSandboxCheck;
		spawn->nextthink = level.time;
	} else {
		Info_SetValueForKey(userinfo, "spawnid", "");
		Info_SetValueForKey(userinfo, "npcType", "");
	}

	// register the userinfo
	trap_SetUserinfo(clientNum, userinfo);

	// have it connect to the game as a normal client
	if(ClientConnect(clientNum, qtrue, qtrue)) return;

	ClientBegin(clientNum);
}

void Svcmd_AddBot_f(void) {
	char model[MAX_TOKEN_CHARS];
	char name[MAX_TOKEN_CHARS];
	char team[MAX_TOKEN_CHARS];

	trap_Argv(1, model, sizeof(model));
	if(!model[0]) {
		trap_Print("Usage: addbot <model/skin> [name] [team]\n");
		return;
	}

	trap_Argv(2, name, sizeof(name));
	trap_Argv(3, team, sizeof(team));
	G_AddBot(model, name, team, NULL);
}

static void G_LoadBotsFromFile(char *filename) {
	int len;
	fileHandle_t f;
	char buf[MAX_BOTS_TEXT];

	len = FS_Open(filename, &f, FS_READ);

	if(!f) return;

	if(len >= MAX_BOTS_TEXT) {
		trap_Print(va(S_COLOR_RED "file too large: %s is %i, max allowed is %i\n", filename, len, MAX_BOTS_TEXT));
		FS_Close(f);
		return;
	}

	FS_Read(buf, len, f);
	buf[len] = 0;
	FS_Close(f);

	g_numBots += G_ParseInfos(buf, MAX_BOTS - g_numBots, &g_botInfos[g_numBots]);
}

void G_LoadBots(void) {
	int numdirs, i, dirlen;
	char filename[128], dirlist[1024];
	char *dirptr;

	g_numBots = 0;

	numdirs = FS_List("scripts", ".bot", dirlist, 1024);
	dirptr = dirlist;
	for(i = 0; i < numdirs; i++, dirptr += dirlen + 1) {
		dirlen = strlen(dirptr);
		strcpy(filename, "scripts/");
		strcat(filename, dirptr);
		G_LoadBotsFromFile(filename);
	}
}

void SandboxBotSpawn(gentity_t *bot, char spawnid[]) {
	gentity_t *t;
	int entityNum;

	if(!bot || !spawnid) return;
	if(!strcmp(spawnid, "")) bot->botspawn = NULL;

	entityNum = atoi(spawnid);

	t = NULL;
	bot->botspawn = NULL;
	while((t = G_Find(t, FOFS(classname), "sandbox_npc")) != NULL) {
		if(t->s.number == entityNum) bot->botspawn = t;
	}
}
