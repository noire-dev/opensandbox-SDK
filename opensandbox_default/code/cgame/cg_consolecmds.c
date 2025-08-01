// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

#include "../qcommon/ns_local.h"

static void CG_ScoresDown_f(void) {
	if(cg.scoresRequestTime + 2000 < cg.time) {
		cg.scoresRequestTime = cg.time;
		trap_SendClientCommand("score");

		if(!cg.showScores) {
			cg.showScores = qtrue;
			cg.numScores = 0;
		}
	} else {
		cg.showScores = qtrue;
	}
}

static void CG_ScoresUp_f(void) {
	if(cg.showScores) cg.showScores = qfalse;
}

static void CG_NS_OpenScript_f(void) {
	char filename[64];
	if(trap_Argc() == 1) {
		CG_Printf("usage: ns_openscript_cl <filename>\n");
		return;
	}

	trap_Argv(1, filename, sizeof(filename));
	NS_OpenScript(filename, NULL, 0);
}

static void CG_NS_Interpret_f(void) {
	char code[2048];
	trap_Args(code, 2048);
	Interpret(code);
}

static void CG_NS_VariableList_f(void) { print_variables(); }

static void CG_NS_ThreadList_f(void) { print_threads(); }

static void CG_NS_SendVariable_f(void) {
	char varName[MAX_VAR_NAME];
	char varValue[MAX_VAR_CHAR_BUF];
	char varType[8];

	trap_Argv(1, varName, sizeof(varName));
	trap_Argv(2, varValue, sizeof(varValue));
	trap_Argv(3, varType, sizeof(varType));

	if(!variable_exists(varName)) create_variable(varName, varValue, atoi(varType));

	set_variable_value(varName, varValue, atoi(varType));
}

static void CG_ReplaceTexture_f(void) {
	char oldtexture[1024];
	char newtexture[1024];

	trap_Argv(1, oldtexture, sizeof(oldtexture));
	trap_Argv(2, newtexture, sizeof(newtexture));

	trap_R_RemapShader(oldtexture, newtexture, "0.005");
}

typedef struct {
	char *cmd;
	void (*function)(void);
} consoleCommand_t;

static consoleCommand_t commands[] = {
	{"+scores", CG_ScoresDown_f},
    {"-scores", CG_ScoresUp_f},
    {"+zoom", CG_ZoomDown_f},
    {"-zoom", CG_ZoomUp_f},
    {"weapnext", CG_NextWeapon_f},
    {"weapprev", CG_PrevWeapon_f},
    {"weapon", CG_Weapon_f},
    {"changetexture", CG_ReplaceTexture_f},
    // Noire.Script
    {"ns_openscript_cl", CG_NS_OpenScript_f},
    {"ns_interpret_cl", CG_NS_Interpret_f},
    {"ns_variablelist_cl", CG_NS_VariableList_f},
    {"ns_threadlist_cl", CG_NS_ThreadList_f},
    {"ns_sendvariable_cl", CG_NS_SendVariable_f},
};

qboolean CG_ConsoleCommand(void) {
	const char *cmd;
	int i;

	cmd = CG_Argv(0);

	for(i = 0; i < ARRAY_SIZE(commands); i++) {
		if(!Q_stricmp(cmd, commands[i].cmd)) {
			commands[i].function();
			return qtrue;
		}
	}

	return qfalse;
}

void CG_InitConsoleCommands(void) {
	int i;

	for(i = 0; i < ARRAY_SIZE(commands); i++) {
		trap_AddCommand(commands[i].cmd);
	}

	// can be used even during intermission
	trap_AddCommand("say");
	trap_AddCommand("say_team");
	trap_AddCommand("tell");
	trap_AddCommand("team");
	trap_AddCommand("score");

	// cheats
	trap_AddCommand("give");
	trap_AddCommand("god");
	trap_AddCommand("tp");
	trap_AddCommand("noclip");

	// interaction
	trap_AddCommand("exitvehicle");
	trap_AddCommand("kill");
	trap_AddCommand("flashlight");
	trap_AddCommand("undo");
	trap_AddCommand("activate");

	// game commands
	trap_AddCommand("follownext");
	trap_AddCommand("followprev");
	trap_AddCommand("teamtask");
	trap_AddCommand("freespectator");

	// server commands
	trap_AddCommand("entityList");
	trap_AddCommand("forceTeam");
	trap_AddCommand("addbot");
	trap_AddCommand("savemap");
	trap_AddCommand("deletemap");
	trap_AddCommand("clearmap");
	trap_AddCommand("loadmap");

	// Noire.Script
	// server
	trap_AddCommand("ns_openscript");
	trap_AddCommand("ns_interpret");
	trap_AddCommand("ns_variablelist");
	trap_AddCommand("ns_threadlist");
	trap_AddCommand("ns_sendvariable");

	// client
	trap_AddCommand("ns_openscript_cl");
	trap_AddCommand("ns_interpret_cl");
	trap_AddCommand("ns_variablelist_cl");
	trap_AddCommand("ns_threadlist_cl");
	trap_AddCommand("ns_sendvariable_cl");

	// ui
	trap_AddCommand("ns_openscript_ui");
	trap_AddCommand("ns_interpret_ui");
	trap_AddCommand("ns_variablelist_ui");
	trap_AddCommand("ns_threadlist_ui");
	trap_AddCommand("ns_sendvariable_ui");
}
