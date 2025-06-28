// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

#include "g_local.h"

static int g_numBots;
static char *g_botInfos[MAX_BOTS];

static int G_ParseInfos(char *buf, int max, char *infos[]) {
	char *token;
	int count;
	char key[MAX_TOKEN_CHARS];
	char info[MAX_INFO_STRING];

	count = 0;

	while(1) {
		token = COM_Parse(&buf);
		if(!token[0]) {
			break;
		}
		if(strcmp(token, "{")) {
			Com_Printf("Missing { in info file\n");
			break;
		}

		if(count == max) {
			Com_Printf("Max infos exceeded\n");
			break;
		}

		info[0] = '\0';
		while(1) {
			token = COM_ParseExt(&buf, qtrue);
			if(!token[0]) {
				Com_Printf("Unexpected end of info file\n");
				break;
			}
			if(!strcmp(token, "}")) {
				break;
			}
			Q_strncpyz(key, token, sizeof(key));

			token = COM_ParseExt(&buf, qfalse);
			if(!token[0]) {
				strcpy(token, "<NULL>");
			}
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

qboolean G_BotConnect(int clientNum, qboolean restart) {
	bot_settings_t settings;
	char userinfo[MAX_INFO_STRING];

	trap_GetUserinfo(clientNum, userinfo, sizeof(userinfo));

	Q_strncpyz(settings.characterfile, Info_ValueForKey(userinfo, "characterfile"), sizeof(settings.characterfile));
	settings.skill = atof(Info_ValueForKey(userinfo, "skill"));
	Q_strncpyz(settings.team, Info_ValueForKey(userinfo, "team"), sizeof(settings.team));

	if(!BotAISetupClient(clientNum, &settings, restart)) {
		trap_DropClient(clientNum, "BotAISetupClient failed");
		return qfalse;
	}

	return qtrue;
}

static char *G_GetBotInfoByName(const char *name) {
	int n;
	char *value;

	for(n = 0; n < g_numBots; n++) {
		value = Info_ValueForKey(g_botInfos[n], "name");
		if(!Q_stricmp(value, name)) return g_botInfos[n];
	}

	return NULL;
}

static void G_BotSandboxCheck(gentity_t *self) {
	if(self->parent && self->parent->health <= 0) {
		self->think = 0;
		self->nextthink = level.time + 1;
		G_FreeEntity(self);
	}
	VectorCopy(self->parent->s.pos.trBase, self->s.origin);
	self->think = G_BotSandboxCheck;
	self->nextthink = level.time + 1;
}

void G_AddBot(const char *name, float skill, const char *team, char *altname, gentity_t *spawn) {
	int clientNum;
	char *botinfo;
	gentity_t *bot;
	char *key;
	char *s;
	char *botname;
	char *model;
	char *headmodel;
	char userinfo[MAX_INFO_STRING];

	// get the botinfo from bots.txt
	botinfo = G_GetBotInfoByName(name);
	if(!botinfo) {
		G_Printf(S_COLOR_RED "Error: Bot '%s' not defined\n", name);
		return;
	}

	// create the bot's userinfo
	userinfo[0] = '\0';

	botname = Info_ValueForKey(botinfo, "funname");
	if(!botname[0]) botname = Info_ValueForKey(botinfo, "name");

	// check for an alternative name
	if(altname && altname[0]) botname = altname;

	Info_SetValueForKey(userinfo, "name", botname);
	Info_SetValueForKey(userinfo, "rate", "25000");
	Info_SetValueForKey(userinfo, "snaps", "20");
	Info_SetValueForKey(userinfo, "skill", va("%1.2f", skill));

	key = "model";
	model = Info_ValueForKey(botinfo, key);
	if(!*model) {
		model = "visor/default";
	}
	Info_SetValueForKey(userinfo, key, model);
	key = "team_model";
	Info_SetValueForKey(userinfo, key, model);

	key = "headmodel";
	headmodel = Info_ValueForKey(botinfo, key);
	if(!*headmodel) {
		headmodel = model;
	}
	Info_SetValueForKey(userinfo, key, headmodel);
	key = "team_headmodel";
	Info_SetValueForKey(userinfo, key, headmodel);

	key = "gender";
	s = Info_ValueForKey(botinfo, key);
	if(!*s) {
		s = "male";
	}
	Info_SetValueForKey(userinfo, "sex", s);

	s = Info_ValueForKey(botinfo, "aifile");
	if(!*s) {
		trap_Printf(S_COLOR_RED "Error: bot has no aifile specified\n");
		return;
	}

	// dynamic limit
	if(level.numConnectedClients >= g_maxClients.integer) {
		G_Printf(S_COLOR_YELLOW "Server is full, increase g_maxClients.\n");
		return;
	}

	// have the server allocate a client slot
	clientNum = trap_BotAllocateClient();
	if(clientNum == -1) {
		G_Printf(S_COLOR_YELLOW "Server is full, increase g_maxClients.\n");
		return;
	}

	// initialize the bot settings
	if(!team || !*team) {
		if(g_gametype.integer >= GT_TEAM) {
			if(PickTeam(clientNum) == TEAM_RED) {
				team = "red";
			} else {
				team = "blue";
			}
		} else {
			team = "red";
		}
	}
	Info_SetValueForKey(userinfo, "characterfile", Info_ValueForKey(botinfo, "aifile"));
	Info_SetValueForKey(userinfo, "skill", va("%5.2f", skill));
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
		spawn->nextthink = level.time + 1;
	}

	// register the userinfo
	trap_SetUserinfo(clientNum, userinfo);

	// have it connect to the game as a normal client
	if(ClientConnect(clientNum, qtrue, qtrue)) return;

	ClientBegin(clientNum);
}

void Svcmd_AddBot_f(void) {
	float skill;
	char name[MAX_TOKEN_CHARS];
	char altname[MAX_TOKEN_CHARS];
	char string[MAX_TOKEN_CHARS];
	char team[MAX_TOKEN_CHARS];

	// are bots enabled?
	if(!trap_Cvar_VariableIntegerValue("bot_enable")) return;

	// name
	trap_Argv(1, name, sizeof(name));
	if(!name[0]) {
		trap_Printf("Usage: Addbot <botname> [skill 1-5] [team] [altname]\n");
		return;
	}

	// skill
	trap_Argv(2, string, sizeof(string));
	if(!string[0]) {
		skill = 5;
	} else {
		skill = atof(string);
	}

	// team
	trap_Argv(3, team, sizeof(team));

	// alternative name
	trap_Argv(4, altname, sizeof(altname));
	G_AddBot(name, skill, team, altname, NULL);
}

static void G_LoadBotsFromFile(char *filename) {
	int len;
	fileHandle_t f;
	char buf[MAX_BOTS_TEXT];

	len = trap_FS_FOpenFile(filename, &f, FS_READ);

	if(!f) return;

	if(len >= MAX_BOTS_TEXT) {
		trap_Printf(va(S_COLOR_RED "file too large: %s is %i, max allowed is %i\n", filename, len, MAX_BOTS_TEXT));
		trap_FS_FCloseFile(f);
		return;
	}

	trap_FS_Read(buf, len, f);
	buf[len] = 0;
	trap_FS_FCloseFile(f);

	g_numBots += G_ParseInfos(buf, MAX_BOTS - g_numBots, &g_botInfos[g_numBots]);
}

void G_LoadBots(void) {
	int numdirs;
	char filename[128];
	char dirlist[1024];
	char *dirptr;
	int i;
	int dirlen;

	if(!trap_Cvar_VariableIntegerValue("bot_enable")) return;

	// get all bots from .bot files
	g_numBots = 0;
	numdirs = trap_FS_GetFileList("scripts", ".bot", dirlist, 1024);
	dirptr = dirlist;
	for(i = 0; i < numdirs; i++, dirptr += dirlen + 1) {
		dirlen = strlen(dirptr);
		strcpy(filename, "scripts/");
		strcat(filename, dirptr);
		G_LoadBotsFromFile(filename);
	}
	trap_Printf(va("%i bots loaded!\n", g_numBots));
}

void SandboxBotSpawn(gentity_t *bot, char spawnid[]) {
	gentity_t *t;
	int entityNum;

	if(!bot || !spawnid || !strcmp(spawnid, "")) return;

	entityNum = atoi(spawnid);

	t = NULL;
	bot->botspawn = NULL;
	while((t = G_Find(t, FOFS(classname), "sandbox_npc")) != NULL) {
		if(t->s.number == entityNum) bot->botspawn = t;
	}
}
