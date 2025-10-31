// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

#include "ui_local.h"

#define SPAWNGRID_SIZE 80
#define ID_TAB_SELECT 60

typedef enum {
	TB_PROPS,
	TB_ENTITIES,
	TB_NPCS,
	TB_ITEMS,
	TB_SPAWNLISTS,
	TB_SCRIPTS,
	TB_TOOLS,
	TB_SAVES,
	TB_ADDBOTS,
	TB_REMOVEBOTS,
	TB_MUSIC,

	TB_MAX
} spawnmenu_tabs_t;

int spawnmenu_sv_top[TB_MAX];
int spawnmenu_sv_value[TB_MAX];

char* spawnmenu_tabnames[] = {
	"Props",
	"Entities",
	"NPCs",
	"Items",
	"Spawnlists",
	"Scripts",
	"Tools",
	"Saves",
	"Add Bots",
	"Remove Bots",
	"Music",
};

static const char* spawnmenu_npsclasses[] = {
	"enemy",
	"citizen",
	"guard",
	"partner",
	"nextbot",
	NULL
};

static const char* spawnmenu_npsweapons[] = {
	"none",
	"machinegun",
	"shotgun",
	"grenade",
	"rocket",
	"lightning",
	"railgun",
	"plasmagun",
	"bfg",
	"grapplinghook",
	"nailgun",
	"prox",
	"chaingun",

	"flamethrower",
	"antimatter",
	"thrower",
	"bouncer",
	"thunder",
	"exploder",
	"knocker",
	"propgun",
	"regenerator",
	"nuke",
	NULL
};

static const char* spawnmenu_team[] = {
	"red",
	"blue",
	NULL
};

static const char* spawnmenu_skill[] = {
	"1",
	"2",
	"3",
	"4",
	"5",
	NULL
};

typedef struct {
	menuframework_s	menu;
	menuelement_s e[OSUI_MAX_ELEMENTS];

	char names1[524288];
	char* list1[524288];
} spawnmenu_t;

static spawnmenu_t spawnmenu;
int menu_tab = 0;
int spawnmenu_tab = 0;
char spawnmenu_folder[32] = "";
char spawnmenu_path_folder[32] = "";
char spawnmenu_path_icons[32] = "";

static void SpawnMenu_Event(void* ptr, int event) {
	if(event != QM_ACTIVATED) return;

	if(((menucommon_s*)ptr)->callid == 0) {
		spawnmenu_sv_top[spawnmenu_tab] = spawnmenu.e[0].top;
		spawnmenu_sv_value[spawnmenu_tab] = spawnmenu.e[0].curvalue;
		if(spawnmenu_tab == TB_PROPS) {
			if(!strlen(spawnmenu_folder)) {
				if(UI_CountFiles(va("mtr/%s", spawnmenu.e[0].itemnames[spawnmenu.e[0].curvalue]), "$image") <= 1) {
					trap_Cmd_ExecuteText(EXEC_NOW, va(spawn_preset.string, spawnmenu.e[0].itemnames[spawnmenu.e[0].curvalue], "none", "0"));
					trap_Cmd_ExecuteText(EXEC_INSERT, "vstr spawn_cmd\n");
				} else {
					Q_strncpyz(spawnmenu_folder, spawnmenu.e[0].itemnames[spawnmenu.e[0].curvalue], sizeof(spawnmenu_folder));
					UI_SpawnMenu();
				}
			} else {
				trap_Cmd_ExecuteText(EXEC_NOW, va(spawn_preset.string, spawnmenu_folder, "none", spawnmenu.e[0].itemnames[spawnmenu.e[0].curvalue]));
				trap_Cmd_ExecuteText(EXEC_INSERT, "vstr spawn_cmd\n");
			}
		}
		if(spawnmenu_tab == TB_ENTITIES) {
			trap_Cmd_ExecuteText(EXEC_NOW, va(spawn_preset.string, "props/cube", spawnmenu.e[0].itemnames[spawnmenu.e[0].curvalue], "0"));
			trap_Cmd_ExecuteText(EXEC_INSERT, "vstr spawn_cmd\n");
		}
		if(spawnmenu_tab == TB_NPCS) {
			trap_Cmd_ExecuteText(EXEC_NOW, va("spawn_cmd = sl npc %s %s %s %s %s %s %i\n", spawnmenu.e[0].itemnames[spawnmenu.e[0].curvalue], spawnmenu_npsclasses[spawnmenu.e[30].curvalue], spawnmenu.e[32].field.buffer, spawnmenu.e[33].field.buffer, spawnmenu.e[34].field.buffer, spawnmenu.e[35].field.buffer, spawnmenu.e[31].curvalue + 1));
			trap_Cmd_ExecuteText(EXEC_INSERT, "vstr spawn_cmd\n");
		}
		if(spawnmenu_tab == TB_ITEMS) {
			trap_Cmd_ExecuteText(EXEC_INSERT, va("give %s\n", spawnmenu.e[0].itemnames[spawnmenu.e[0].curvalue]));
		}
		if(spawnmenu_tab == TB_SPAWNLISTS) {
			if(!strlen(spawnmenu_folder)) {
				Q_strncpyz(spawnmenu_folder, spawnmenu.e[0].itemnames[spawnmenu.e[0].curvalue], sizeof(spawnmenu_folder));
				UI_SpawnMenu();
			} else {
				trap_Cmd_ExecuteText(EXEC_NOW, va("spawn_cmd = ns_openscript_ui spawnlists/%s/%s.ns\n", spawnmenu_folder, spawnmenu.e[0].itemnames[spawnmenu.e[0].curvalue]));
				trap_Cmd_ExecuteText(EXEC_INSERT, "vstr spawn_cmd\n");
			}
		}
		if(spawnmenu_tab == TB_SCRIPTS) {
			trap_Cmd_ExecuteText(EXEC_INSERT, va("ns_openscript_ui dscripts/%s.ns\n", spawnmenu.e[0].itemnames[spawnmenu.e[0].curvalue]));
		}
		if(spawnmenu_tab == TB_TOOLS) {
			trap_Cmd_ExecuteText(EXEC_NOW, va("ns_openscript_ui tools/%s.ns\n", spawnmenu.e[0].itemnames[spawnmenu.e[0].curvalue]));
			trap_Cmd_ExecuteText(EXEC_INSERT, va("weapon %i\n", WP_TOOLGUN));
			NS_setCvar("toolgun_mod5", "0");
			UI_SpawnMenu();
		}
		if(spawnmenu_tab == TB_SAVES) {
			trap_Cmd_ExecuteText(EXEC_NOW, va("loadmap maps/%s.ent\n", spawnmenu.e[0].itemnames[spawnmenu.e[0].curvalue]));
			trap_Cmd_ExecuteText(EXEC_INSERT, "menuback\n");
		}
		if(spawnmenu_tab == TB_ADDBOTS) {
			trap_Cmd_ExecuteText(EXEC_NOW, va("addbot %s %s %s\n", spawnmenu.e[0].itemnames[spawnmenu.e[0].curvalue], spawnmenu_skill[spawnmenu.e[30].curvalue], spawnmenu_team[spawnmenu.e[31].curvalue]));
		}
		if(spawnmenu_tab == TB_REMOVEBOTS) {
			trap_Cmd_ExecuteText(EXEC_NOW, va("kick %s\n", spawnmenu.e[0].itemnames[spawnmenu.e[0].curvalue]));
			UI_SpawnMenu();
		}
		if(spawnmenu_tab == TB_MUSIC) {
			trap_Cmd_ExecuteText(EXEC_NOW, va("music \"music/%s\"\n", spawnmenu.e[0].itemnames[spawnmenu.e[0].curvalue]));
			UI_SpawnMenu();
		}
	}

	if(((menucommon_s*)ptr)->callid == 1) {
		trap_Cmd_ExecuteText(EXEC_INSERT, "vstr spawn_cmd\n");
	}

	if(((menucommon_s*)ptr)->callid == 2) {
		trap_Cmd_ExecuteText(EXEC_INSERT, va("weapon %i\n", WP_TOOLGUN));
	}

	if(((menucommon_s*)ptr)->callid == 3 && spawnmenu_tab == TB_SAVES) {
		trap_Cmd_ExecuteText(EXEC_APPEND, va("savemap maps/%s.ent\n", spawnmenu.e[30].field.buffer));
		UI_ForceMenuOff();
		trap_Cmd_ExecuteText(EXEC_APPEND, "cg_draw2D = 0\n");
		trap_Cmd_ExecuteText(EXEC_APPEND, "wait 10\n");
		trap_Cmd_ExecuteText(EXEC_APPEND, va("screenshotJPEG maps/%s\n", spawnmenu.e[30].field.buffer));
		trap_Cmd_ExecuteText(EXEC_APPEND, "wait 10\n");
		trap_Cmd_ExecuteText(EXEC_APPEND, "cg_draw2D = 1\n");
	}

	if(((menucommon_s*)ptr)->callid >= ID_TAB_SELECT) {
		spawnmenu_sv_top[spawnmenu_tab] = spawnmenu.e[0].top;
		spawnmenu_sv_value[spawnmenu_tab] = spawnmenu.e[0].curvalue;
		spawnmenu_tab = ((menucommon_s*)ptr)->callid - ID_TAB_SELECT;
		if(spawnmenu_tab == TB_PROPS) {
			Q_strncpyz(spawnmenu_folder, "", sizeof(spawnmenu_folder));
			spawnmenu_sv_top[spawnmenu_tab] = 0;
			spawnmenu_sv_value[spawnmenu_tab] = 0;
		}
		if(spawnmenu_tab == TB_SPAWNLISTS) {
			Q_strncpyz(spawnmenu_folder, "", sizeof(spawnmenu_folder));
			spawnmenu_sv_top[spawnmenu_tab] = 0;
			spawnmenu_sv_value[spawnmenu_tab] = 0;
		}
		UI_SpawnMenu();
	}
}

static void SpawnMenu_Save(void) {
	trap_Cmd_ExecuteText(EXEC_NOW, va("toolgun_cmd = tm %s %s %s %s", spawnmenu.e[40].field.buffer, spawnmenu.e[41].field.buffer, spawnmenu.e[42].field.buffer, spawnmenu.e[43].field.buffer));
	if(spawnmenu_tab == TB_PROPS) {
		if(!strlen(spawnmenu_folder)) {
			if(UI_CountFiles(va("mtr/%s", spawnmenu.e[0].itemnames[spawnmenu.e[0].curvalue]), "$image") <= 1) {
				trap_Cmd_ExecuteText(EXEC_NOW, va(spawn_preset.string, spawnmenu.e[0].itemnames[spawnmenu.e[0].curvalue], "none", "0"));
			} else {
				Q_strncpyz(spawnmenu_folder, spawnmenu.e[0].itemnames[spawnmenu.e[0].curvalue], sizeof(spawnmenu_folder));
				UI_SpawnMenu();
			}
		} else {
			trap_Cmd_ExecuteText(EXEC_NOW, va(spawn_preset.string, spawnmenu_folder, "none", spawnmenu.e[0].itemnames[spawnmenu.e[0].curvalue]));
		}
	}
	if(spawnmenu_tab == TB_ENTITIES) {
		trap_Cmd_ExecuteText(EXEC_NOW, va(spawn_preset.string, "props/cube", spawnmenu.e[0].itemnames[spawnmenu.e[0].curvalue], "0"));
	}
	if(spawnmenu_tab == TB_NPCS) {
		trap_Cmd_ExecuteText(EXEC_NOW, va("spawn_cmd = sl npc %s %s %s %s %s %s %i\n", spawnmenu.e[0].itemnames[spawnmenu.e[0].curvalue], spawnmenu_npsclasses[spawnmenu.e[30].curvalue], spawnmenu.e[32].field.buffer, spawnmenu.e[33].field.buffer, spawnmenu.e[34].field.buffer, spawnmenu.e[35].field.buffer, spawnmenu.e[31].curvalue + 1));
	}
	if(spawnmenu_tab == TB_SPAWNLISTS) {
		if(!strlen(spawnmenu_folder)) {
			Q_strncpyz(spawnmenu_folder, spawnmenu.e[0].itemnames[spawnmenu.e[0].curvalue], sizeof(spawnmenu_folder));
			UI_SpawnMenu();
		} else {
			trap_Cmd_ExecuteText(EXEC_NOW, va("spawn_cmd = ns_openscript_ui spawnlists/%s/%s.ns\n", spawnmenu_folder, spawnmenu.e[0].itemnames[spawnmenu.e[0].curvalue]));
		}
	}
	spawnmenu_sv_top[spawnmenu_tab] = spawnmenu.e[0].top;
	spawnmenu_sv_value[spawnmenu_tab] = spawnmenu.e[0].curvalue;
}

static sfxHandle_t SpawnMenu_Key(int key) {
	if(key == K_MOUSE2 || key == K_ESCAPE) {
		SpawnMenu_Save();
	}
	return Menu_DefaultKey(&spawnmenu.menu, key);
}

static void SpawnMenu_Draw(void) {
	int y, i;
	vec4_t color = {1.00, 1.00, 1.00, 1.00};
	vec4_t color2 = {0.85, 0.90, 1.00, 0.20};
	vec4_t color3 = {0.60, 0.75, 1.00, 0.40};
	vec4_t color4 = {0.50, 0.60, 0.80, 1.00};

	UI_DrawRoundedRect(10 - uis.wideoffset, 10, 440 + uis.wideoffset * 2, 480 - 38, 4, color_dim);
	UI_DrawRoundedRect(640 + uis.wideoffset - 180, 10, 170, 480 - 38, 4, color_dim);

	UI_DrawRoundedRect(640+uis.wideoffset-175, 15, 160, 16, 3, color3);
	ST_DrawString(640+uis.wideoffset-95, 18, toolgun_tooltext.string, UI_CENTER, color_white, 1.00);

	y = 15;
	for(i = 0; i < TB_MAX; i++) {
		if(i == spawnmenu_tab) {
			UI_DrawRoundedRect(15 - uis.wideoffset, y, 125, 16, 3, color3);
		} else {
			UI_DrawRoundedRect(15 - uis.wideoffset, y, 125, 16, 3, color2);
		}
		UI_DrawRoundedRect(124 - uis.wideoffset, y + 2, 14, 12, 2, color_dim);
		if(i == TB_PROPS) ST_DrawString(131 - uis.wideoffset, y + 5, va("%i", UI_CountFiles("props", ".md3")), UI_CENTER, color_white, 0.65);
		if(i == TB_ENTITIES) ST_DrawString(131 - uis.wideoffset, y + 5, va("%i", gameInfoSandboxSpawnsNum), UI_CENTER, color_white, 0.65);
		if(i == TB_NPCS) ST_DrawString(131 - uis.wideoffset, y + 5, va("%i", UI_GetNumBots()), UI_CENTER, color_white, 0.65);
		if(i == TB_ITEMS) ST_DrawString(131 - uis.wideoffset, y + 5, va("%i", gameInfoItemsNum - 1), UI_CENTER, color_white, 0.65);
		if(i == TB_SPAWNLISTS) ST_DrawString(131 - uis.wideoffset, y + 5, va("%i", UI_CountFiles("spawnlists", ".cfg")), UI_CENTER, color_white, 0.65);
		if(i == TB_SCRIPTS) ST_DrawString(131 - uis.wideoffset, y + 5, va("%i", UI_CountFiles("dscripts", ".ns")), UI_CENTER, color_white, 0.65);
		if(i == TB_TOOLS) ST_DrawString(131 - uis.wideoffset, y + 5, va("%i", UI_CountFiles("tools", ".ns")), UI_CENTER, color_white, 0.65);
		if(i == TB_SAVES) ST_DrawString(131 - uis.wideoffset, y + 5, va("%i", UI_CountFiles("maps", ".ent")), UI_CENTER, color_white, 0.65);
		if(i == TB_ADDBOTS) ST_DrawString(131 - uis.wideoffset, y + 5, va("%i", UI_GetNumBots()), UI_CENTER, color_white, 0.65);
		if(i == TB_REMOVEBOTS) ST_DrawString(131 - uis.wideoffset, y + 5, va("%i", UI_ListPlayerCount()), UI_CENTER, color_white, 0.65);
		if(i == TB_MUSIC) ST_DrawString(131 - uis.wideoffset, y + 5, va("%i", UI_CountFiles("music", "$sound")), UI_CENTER, color_white, 0.65);
		y += 20;
	}

	ST_DrawString(148 - uis.wideoffset, 20, spawnmenu_tabnames[spawnmenu_tab], UI_LEFT, color_white, 1.50);
	ST_DrawString(152 + (BASEFONT_INDENT * ST_StringCount(spawnmenu_tabnames[spawnmenu_tab]) * 1.50) - uis.wideoffset, 26, va("%i items", spawnmenu.e[0].numitems), UI_LEFT, color_grey, 0.80);

	UI_DrawRoundedRect(640 + uis.wideoffset - 145, 375, 100, 24, 3, color4);
	UI_DrawRoundedRect(640 + uis.wideoffset - 145, 415, 100, 24, 3, color4);

	Menu_Draw(&spawnmenu.menu);
}

void UI_SpawnMenu(void) {
	int y, i;

	memset(&spawnmenu, 0, sizeof(spawnmenu_t));
	spawnmenu.menu.draw = SpawnMenu_Draw;
	spawnmenu.menu.fullscreen = qtrue;
	spawnmenu.menu.key = SpawnMenu_Key;

	if(spawnmenu_tab == TB_SCRIPTS || spawnmenu_tab == TB_TOOLS || spawnmenu_tab == TB_MUSIC) {
		UI_CList(&spawnmenu.e[0], 150 - uis.wideoffset, 42, (((10 - uis.wideoffset + 440 + uis.wideoffset * 2) - 150 + uis.wideoffset) / BASEFONT_INDENT) - 2, 36, 1, 0, 0, LST_SIMPLE, qfalse, 0, color_white, SpawnMenu_Event, 0);
	} else {
		UI_CList(&spawnmenu.e[0], 150 - uis.wideoffset, 42, SPAWNGRID_SIZE, 5, ((10 - uis.wideoffset + 440 + uis.wideoffset * 2) - 150 + uis.wideoffset) / SPAWNGRID_SIZE, 6, 8, LST_GRID, qtrue, 4, color_white, SpawnMenu_Event, 0);
	}
	if(spawnmenu_tab == TB_PROPS) {
		if(!strlen(spawnmenu_folder)) {
			UI_FillList(&spawnmenu.e[0], "props", "props", ".md3", spawnmenu.names1, sizeof(spawnmenu.names1), spawnmenu.list1);
		} else {
			Com_sprintf(spawnmenu_path_folder, sizeof(spawnmenu_path_folder), "mtr/%s", spawnmenu_folder);
			UI_FillList(&spawnmenu.e[0], spawnmenu_path_folder, spawnmenu_path_folder, "$image", spawnmenu.names1, sizeof(spawnmenu.names1), spawnmenu.list1);
		}
	}
	if(spawnmenu_tab == TB_ENTITIES) UI_FillListFromArray(&spawnmenu.e[0], spawnmenu.list1, gameInfoSandboxSpawns, gameInfoSandboxSpawnsNum);
	if(spawnmenu_tab == TB_NPCS) UI_FillListOfBots(&spawnmenu.e[0], spawnmenu.names1, sizeof(spawnmenu.names1), spawnmenu.list1);
	if(spawnmenu_tab == TB_ITEMS) UI_FillListOfItems(&spawnmenu.e[0], spawnmenu.names1, sizeof(spawnmenu.names1), spawnmenu.list1);
	if(spawnmenu_tab == TB_SPAWNLISTS) {
		if(!strlen(spawnmenu_folder)) {
			UI_FillList(&spawnmenu.e[0], "spawnlists", "spawnlists/icons", ".cfg", spawnmenu.names1, sizeof(spawnmenu.names1), spawnmenu.list1);
		} else {
			Com_sprintf(spawnmenu_path_folder, sizeof(spawnmenu_path_folder), "spawnlists/%s", spawnmenu_folder);
			Com_sprintf(spawnmenu_path_icons, sizeof(spawnmenu_path_icons), "spawnlists/%s/icons", spawnmenu_folder);
			UI_FillList(&spawnmenu.e[0], spawnmenu_path_folder, spawnmenu_path_icons, ".ns", spawnmenu.names1, sizeof(spawnmenu.names1), spawnmenu.list1);
		}
	}
	if(spawnmenu_tab == TB_SCRIPTS) UI_FillList(&spawnmenu.e[0], "dscripts", "", ".ns", spawnmenu.names1, sizeof(spawnmenu.names1), spawnmenu.list1);
	if(spawnmenu_tab == TB_TOOLS) UI_FillList(&spawnmenu.e[0], "tools", "", ".ns", spawnmenu.names1, sizeof(spawnmenu.names1), spawnmenu.list1);
	if(spawnmenu_tab == TB_SAVES) UI_FillList(&spawnmenu.e[0], "maps", "screenshots/maps", ".ent", spawnmenu.names1, sizeof(spawnmenu.names1), spawnmenu.list1);
	if(spawnmenu_tab == TB_ADDBOTS) UI_FillListOfBots(&spawnmenu.e[0], spawnmenu.names1, sizeof(spawnmenu.names1), spawnmenu.list1);
	if(spawnmenu_tab == TB_REMOVEBOTS) UI_FillListPlayers(&spawnmenu.e[0], spawnmenu.list1, spawnmenu.names1, sizeof(spawnmenu.names1));
	if(spawnmenu_tab == TB_MUSIC) UI_FillList(&spawnmenu.e[0], "music", "music", "$sound", spawnmenu.names1, sizeof(spawnmenu.names1), spawnmenu.list1);

	y = 15;
	for(i = 0; i < TB_MAX; i++) {
		UI_CButton(&spawnmenu.e[ID_TAB_SELECT + i], 18 - uis.wideoffset, y + 4, spawnmenu_tabnames[i], UI_LEFT, 0.78, color_white, NULL, NULL, NULL, SpawnMenu_Event, ID_TAB_SELECT + i); y += 20;
	}

	if(spawnmenu_tab == TB_SAVES) {
		UI_CButton(&spawnmenu.e[1], (640 + uis.wideoffset - 155) + 60, 380, "Save", UI_CENTER, 1.45, color_white, NULL, NULL, NULL, SpawnMenu_Event, 3);
		UI_CButton(&spawnmenu.e[2], (640 + uis.wideoffset - 155) + 60, 420, "Load", UI_CENTER, 1.45, color_white, NULL, NULL, NULL, SpawnMenu_Event, 0);
	} else {
		UI_CButton(&spawnmenu.e[1], (640 + uis.wideoffset - 155) + 60, 380, "Spawn", UI_CENTER, 1.45, color_white, NULL, NULL, NULL, SpawnMenu_Event, 1);
		UI_CButton(&spawnmenu.e[2], (640 + uis.wideoffset - 155) + 60, 420, "Create", UI_CENTER, 1.45, color_white, NULL, NULL, NULL, SpawnMenu_Event, 2);
	}

	y = 38;
	if(spawnmenu_tab == TB_NPCS) {
		UI_CSpinControl(&spawnmenu.e[30], 640 + uis.wideoffset - 110, y, "Class:", spawnmenu_npsclasses, NULL, 0); y += 12;
		UI_CSpinControl(&spawnmenu.e[31], 640 + uis.wideoffset - 110, y, "Weapon:", spawnmenu_npsweapons, NULL, 0); y += 12;
		y += 12;
		NS_setCvar("uis_b1", "5");
		NS_setCvar("uis_b2", "100");
		NS_setCvar("uis_b3", "0");
		NS_setCvar("uis_b4", "0");
		UI_CField(&spawnmenu.e[32], 640 + uis.wideoffset - 110, y, "Skill:", 5, 5, color_white, "uis_b1", NULL, 0); y += 12;
		UI_CField(&spawnmenu.e[33], 640 + uis.wideoffset - 110, y, "Health:", 9, 9, color_white, "uis_b2", NULL, 0); y += 12;
		UI_CField(&spawnmenu.e[34], 640 + uis.wideoffset - 110, y, "Name:", 16, 16, color_white, "uis_b3", NULL, 0); y += 12;
		UI_CField(&spawnmenu.e[35], 640 + uis.wideoffset - 110, y, "Music:", 16, 16, color_white, "uis_b4", NULL, 0); y += 12;
		y += 20;
	}
	if(spawnmenu_tab == TB_SAVES) {
		UI_CField(&spawnmenu.e[30], (640 + uis.wideoffset - 150) + 14, 350, "Name:", 16, 16, color_white, "uis_sv1", NULL, 0);
		y += 12;
	}
	if(spawnmenu_tab == TB_ADDBOTS) {
		UI_CSpinControl(&spawnmenu.e[30], 640 + uis.wideoffset - 110, y, "Skill:", spawnmenu_skill, NULL, 0); y += 12;
		UI_CSpinControl(&spawnmenu.e[31], 640 + uis.wideoffset - 110, y, "Team:", spawnmenu_team, NULL, 0); y += 12;
		y += 20;
	}
	UI_CField(&spawnmenu.e[40], 640 + uis.wideoffset - 110, y, toolgun_toolset1.string, 16, 16, color_white, "uis_tg1", NULL, 0); y += 12;
	UI_CField(&spawnmenu.e[41], 640 + uis.wideoffset - 110, y, toolgun_toolset2.string, 16, 16, color_white, "uis_tg2", NULL, 0); y += 12;
	UI_CField(&spawnmenu.e[42], 640 + uis.wideoffset - 110, y, toolgun_toolset3.string, 16, 16, color_white, "uis_tg3", NULL, 0); y += 12;
	UI_CField(&spawnmenu.e[43], 640 + uis.wideoffset - 110, y, toolgun_toolset4.string, 16, 16, color_white, "uis_tg4", NULL, 0); y += 12;

	UI_CreateUI(&spawnmenu.menu, spawnmenu.e);

	y = 15;
	for(i = 0; i < TB_MAX; i++) {
		UI_SetHitbox(&spawnmenu.e[ID_TAB_SELECT + i], 15 - uis.wideoffset, y, 125, 16); y += 20;
	}
	UI_SetHitbox(&spawnmenu.e[1], 640 + uis.wideoffset - 145, 375, 100, 24);
	UI_SetHitbox(&spawnmenu.e[2], 640 + uis.wideoffset - 145, 415, 100, 24);

	spawnmenu.e[0].top = spawnmenu_sv_top[spawnmenu_tab];
	spawnmenu.e[0].curvalue = spawnmenu_sv_value[spawnmenu_tab];

	trap_Key_SetCatcher(KEYCATCH_UI);
	UI_PushMenu(&spawnmenu.menu);
}
