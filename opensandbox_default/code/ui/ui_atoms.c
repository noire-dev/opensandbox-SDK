// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

#include "../qcommon/ns_local.h"

uiStatic_t uis;

void QDECL Com_Error(int level, const char *error, ...) {
	va_list argptr;
	char text[1024];
	va_start(argptr, error);
	Q_vsnprintf(text, sizeof(text), error, argptr);
	va_end(argptr);

	trap_Error(va("%s", text));
}

void QDECL Com_Printf(const char *msg, ...) {
	va_list argptr;
	char text[1024];

	va_start(argptr, msg);
	Q_vsnprintf(text, sizeof(text), msg, argptr);
	va_end(argptr);

	trap_Print(va("%s", text));
}

void UI_UpdateState(void) {
	float scrx;
	float scry;
	char svinfo[MAX_INFO_STRING];

	scrx = glconfig.vidWidth;
	scry = glconfig.vidHeight;

	if((scrx / (scry / 480) - 640) / 2 >= 0) {
		uis.wideoffset = (scrx / (scry / 480) - 640) / 2;
	} else {
		uis.wideoffset = 0;
	}

	trap_GetConfigString(CS_SERVERINFO, svinfo, MAX_INFO_STRING);
	if(strlen(svinfo) <= 0) {
		uis.onmap = qfalse;
	} else {
		uis.onmap = qtrue;
	}

	if(trap_Cvar_VariableValue("r_postfx")) {
		uis.postfx_status = qtrue;
	} else {
		uis.postfx_status = qfalse;
	}

	if(glconfig.vidHeight / 480.0f > 0) {
		trap_Cvar_SetValue("con_scale", 1.85);
	}
	trap_Cvar_Set("cl_conColor", "8 8 8 192");
}

void UI_PushMenu(menuframework_s *menu) {
	int i;
	menucommon_s *item;

	if(uis.onmap) {
		trap_Cvar_Set("r_fx_blur", "1");  // blur UI postFX
	} else {
		trap_Cvar_Set("r_fx_blur", "0");  // blur UI postFX
	}

	// avoid stacking menus invoked by hotkeys
	for(i = 0; i < uis.menusp; i++) {
		if(uis.stack[i] == menu) {
			uis.menusp = i;
			break;
		}
	}

	if(i == uis.menusp) {
		if(uis.menusp >= MAX_MENUDEPTH) trap_Error("UI_PushMenu: menu stack overflow");

		uis.stack[uis.menusp++] = menu;
	}

	uis.activemenu = menu;

	// default cursor position
	menu->cursor = 0;
	menu->cursor_prev = 0;

	trap_Key_SetCatcher(KEYCATCH_UI);

	// force first available item to have focus
	for(i = 0; i < menu->nitems; i++) {
		item = (menucommon_s *)menu->items[i];
		if(!(item->flags & (QMF_GRAYED | QMF_INACTIVE))) {
			menu->cursor_prev = -1;
			Menu_SetCursor(menu, i);
			break;
		}
	}

	uis.firstdraw = qtrue;
}

void UI_PopMenu(void) {
	trap_S_StartLocalSound(menu_out_sound, CHAN_LOCAL_SOUND);

	uis.menusp--;

	if(uis.menusp < 0) UI_ForceMenuOff();

	if(uis.menusp) {
		uis.activemenu = uis.stack[uis.menusp - 1];
		uis.firstdraw = qtrue;
	} else {
		UI_ForceMenuOff();
	}
}

void UI_ForceMenuOff(void) {
	uis.menusp = 0;
	uis.activemenu = NULL;

	trap_Key_SetCatcher(trap_Key_GetCatcher() & ~KEYCATCH_UI);
	trap_Key_ClearStates();
	trap_Cvar_Set("cl_paused", "0");
	trap_Cvar_Set("r_fx_blur", "0");  // blur UI postFX
}

qboolean UI_IsFullscreen(void) {
	if(uis.activemenu && (trap_Key_GetCatcher() & KEYCATCH_UI)) {
		if(!uis.onmap) {
			return uis.activemenu->fullscreen;
		} else {
			return 0;
		}
	}

	return qfalse;
}

void UI_SetActiveMenu(uiMenuCommand_t menu) {
	// this should be the ONLY way the menu system is brought up
	// enusure minumum menu data is cached
	Menu_Cache();

	switch(menu) {
		case UIMENU_NONE: UI_ForceMenuOff(); return;
		case UIMENU_MAIN: UI_MainMenu(); return;
		case UIMENU_INGAME:
			trap_Cvar_Set("cl_paused", "1");
			UI_MainMenu();
			return;

		default: Com_Printf("UI_SetActiveMenu: bad enum %d\n", menu); break;
	}
}

void UI_KeyEvent(int key, int down) {
	sfxHandle_t s;

	if(!uis.activemenu) {
		return;
	}

	if(!down) {
		return;
	}

	if(uis.activemenu->key)
		s = uis.activemenu->key(key);
	else
		s = Menu_DefaultKey(uis.activemenu, key);

	if((s > 0) && (s != menu_null_sound)) trap_S_StartLocalSound(s, CHAN_LOCAL_SOUND);
}

void UI_MouseEvent(int dx, int dy) {
	int i;
	menucommon_s *m;

	if(!uis.activemenu) return;

	// update mouse screen position
	uis.cursorx += dx * sensitivitymenu.value;
	if(uis.cursorx < 0 - uis.wideoffset)
		uis.cursorx = 0 - uis.wideoffset;
	else if(uis.cursorx > 640 + uis.wideoffset)
		uis.cursorx = 640 + uis.wideoffset;

	uis.cursory += dy * sensitivitymenu.value;
	if(uis.cursory < 0)
		uis.cursory = 0;
	else if(uis.cursory > 480)
		uis.cursory = 480;

	// region test the active menu items
	for(i = 0; i < uis.activemenu->nitems; i++) {
		m = (menucommon_s *)uis.activemenu->items[i];

		if(m->flags & (QMF_GRAYED | QMF_INACTIVE)) continue;

		if((uis.cursorx < m->left) || (uis.cursorx > m->right) || (uis.cursory < m->top) || (uis.cursory > m->bottom)) continue;  // cursor out of item bounds

		// set focus to item at cursor
		if(uis.activemenu->cursor != i) {
			Menu_SetCursor(uis.activemenu, i);
			((menucommon_s *)(uis.activemenu->items[uis.activemenu->cursor_prev]))->flags &= ~QMF_HASMOUSEFOCUS;
		}

		((menucommon_s *)(uis.activemenu->items[uis.activemenu->cursor]))->flags |= QMF_HASMOUSEFOCUS;
		return;
	}

	if(uis.activemenu->nitems > 0) {
		((menucommon_s *)(uis.activemenu->items[uis.activemenu->cursor]))->flags &= ~QMF_HASMOUSEFOCUS;  // out of any region
	}
}

static char *UI_Argv(int arg) {
	static char buffer[MAX_STRING_CHARS];

	trap_Argv(arg, buffer, sizeof(buffer));

	return buffer;
}

static char *UI_ConcatArgs(int start) {
	int i, c, tlen;
	static char line[MAX_STRING_CHARS];
	int len;
	char arg[MAX_STRING_CHARS];

	len = 0;
	c = trap_Argc();
	for(i = start; i < c; i++) {
		trap_Argv(i, arg, sizeof(arg));
		tlen = strlen(arg);
		if(len + tlen >= MAX_STRING_CHARS - 1) {
			break;
		}
		memcpy(line + len, arg, tlen);
		len += tlen;
		if(i != c - 1) {
			line[len] = ' ';
			len++;
		}
	}

	line[len] = 0;

	return line;
}

char *UI_Cvar_VariableString(const char *var_name) {
	static char buffer[MAX_STRING_CHARS];

	trap_Cvar_VariableStringBuffer(var_name, buffer, sizeof(buffer));

	return buffer;
}

qboolean UI_ConsoleCommand(int realTime) {
	char *cmd;
	cmd = UI_Argv(0);

	// ensure minimum menu data is available
	Menu_Cache();

	if(Q_stricmp(UI_Argv(0), "ui_spawnmenu") == 0) {
		UI_SpawnMenu();
		return qtrue;
	}

	if(Q_stricmp(UI_Argv(0), "ns_openscript_ui") == 0) {
		NS_OpenScript(UI_Argv(1), NULL, 0);
		return qtrue;
	}

	if(Q_stricmp(UI_Argv(0), "ns_interpret_ui") == 0) {
		Interpret(UI_ConcatArgs(1));
		return qtrue;
	}

	if(Q_stricmp(UI_Argv(0), "ns_variablelist_ui") == 0) {
		print_variables();
		return qtrue;
	}

	if(Q_stricmp(UI_Argv(0), "ns_threadlist_ui") == 0) {
		print_threads();
		return qtrue;
	}

	if(Q_stricmp(UI_Argv(0), "ns_sendvariable_ui") == 0) {
		if(!variable_exists(UI_Argv(1))) {
			create_variable(UI_Argv(1), UI_Argv(2), atoi(UI_Argv(3)));
		}

		set_variable_value(UI_Argv(1), UI_Argv(2), atoi(UI_Argv(3)));
	}

	if(Q_stricmp(cmd, "menuback") == 0) {
		UI_PopMenu();
		return qtrue;
	}

	return qfalse;
}

void UI_Shutdown(void) {}

void UI_Init(void) {
	// init cvars and gameinfo
	UI_CreateCvars();
	ST_RegisterCvars();

	UI_InitMemory();
	UI_LoadArenas();
	UI_LoadBots();

	// cache glconfig
	trap_GetGlconfig(&glconfig);

	uis.scale = (glconfig.vidWidth * (1.0 / 640.0) < glconfig.vidHeight * (1.0 / 480.0)) ? glconfig.vidWidth * (1.0 / 640.0) : glconfig.vidHeight * (1.0 / 480.0);

	if(glconfig.vidWidth * 480 > glconfig.vidHeight * 640) {
		// wide screen
		uis.bias = 0.5 * (glconfig.vidWidth - (glconfig.vidHeight * (640.0 / 480.0)));
	} else if(glconfig.vidWidth * 480 < glconfig.vidHeight * 640) {
		// 5:4 screen
		uis.bias = 0;
	} else {
		// no wide screen
		uis.bias = 0;
	}

	// initialize the menu system
	Menu_Cache();

	ST_RegisterFont("default");

	uis.activemenu = NULL;
	uis.menusp = 0;

	NS_OpenScript("nscript/ui/init.ns", NULL, 0);  // Noire.Script Init in ui.qvm
}

/*
================
UI_AdjustFrom640

Adjusted for resolution and screen aspect ratio
================
*/
void UI_AdjustFrom640(float *x, float *y, float *w, float *h) {
	*x = *x * uis.scale + uis.bias;
	*y *= uis.scale;
	*w *= uis.scale;
	*h *= uis.scale;
}

void UI_DrawHandlePic(float x, float y, float w, float h, qhandle_t hShader) {
	float s0;
	float s1;
	float t0;
	float t1;

	if(w < 0) {  // flip about vertical
		w = -w;
		s0 = 1;
		s1 = 0;
	} else {
		s0 = 0;
		s1 = 1;
	}

	if(h < 0) {  // flip about horizontal
		h = -h;
		t0 = 1;
		t1 = 0;
	} else {
		t0 = 0;
		t1 = 1;
	}

	UI_AdjustFrom640(&x, &y, &w, &h);
	trap_R_DrawStretchPic(x, y, w, h, s0, t0, s1, t1, hShader);
}

void UI_DrawPictureElement(float x, float y, float w, float h, const char *file) {
	float s0;
	float s1;
	float t0;
	float t1;
	int file_len;
	qhandle_t hShader;

	file_len = strlen(file);

	hShader = trap_R_RegisterShaderNoMip("menu/element");

	if(w < 0) {  // flip about vertical
		w = -w;
		s0 = 1;
		s1 = 0;
	} else {
		s0 = 0;
		s1 = 1;
	}

	if(h < 0) {  // flip about horizontal
		h = -h;
		t0 = 1;
		t1 = 0;
	} else {
		t0 = 0;
		t1 = 1;
	}

	UI_AdjustFrom640(&x, &y, &w, &h);
	trap_R_DrawStretchPic(x, y, w, h, s0, t0, s1, t1, hShader);
}

void UI_DrawModelElement(float x, float y, float w, float h, const char *model, float scale) {
	refdef_t refdef;
	refEntity_t ent;
	vec3_t origin;
	vec3_t angles;

	memset(&refdef, 0, sizeof(refdef));
	refdef.rdflags = RDF_NOWORLDMODEL;
	AxisClear(refdef.viewaxis);

	UI_AdjustFrom640(&x, &y, &w, &h);
	refdef.x = x;
	refdef.y = y;
	refdef.width = w;
	refdef.height = h;

	refdef.fov_x = 90;
	refdef.fov_y = 90;

	refdef.time = uis.realtime;

	origin[0] = scale;
	origin[1] = 0;
	origin[2] = 0;

	angles[0] = 0;
	angles[1] = (uis.realtime & 4095) * 360 / 4096.0;
	angles[2] = 0;

	trap_R_ClearScene();

	memset(&ent, 0, sizeof(ent));

	AnglesToAxis(angles, ent.axis);
	ent.hModel = trap_R_RegisterModel(model);
	ent.shaderRGBA[0] = 128;
	ent.shaderRGBA[1] = 128;
	ent.shaderRGBA[2] = 128;
	ent.shaderRGBA[3] = 255;
	VectorCopy(origin, ent.origin);
	VectorCopy(origin, ent.lightingOrigin);
	ent.renderfx = RF_LIGHTING_ORIGIN;
	VectorCopy(ent.origin, ent.oldorigin);
	ent.customSkin = trap_R_RegisterSkin(va("mtr/%s/%i.skin", model, 0));

	trap_R_AddRefEntityToScene(&ent);
	trap_R_RenderScene(&refdef);
}

void UI_DrawRoundedRect(float x, float y, float width, float height, float radius, const float *color) {
	UI_AdjustFrom640(&x, &y, &width, &height);

	if(radius * 2 > height) {
		radius = height * 0.5;
	}
	if(radius * 2 > width) {
		radius = width * 0.5;
	}

	radius *= uis.scale;

	trap_R_SetColor(color);
	trap_R_DrawStretchPic(x, y, radius, radius, 1, 0, 0, 1, uis.corner);                                     // Левый верхний угол
	trap_R_DrawStretchPic(x + width - radius, y, radius, radius, 0, 0, 1, 1, uis.corner);                    // Правый верхний угол
	trap_R_DrawStretchPic(x, y + height - radius, radius, radius, 1, 1, 0, 0, uis.corner);                   // Левый нижний угол
	trap_R_DrawStretchPic(x + width - radius, y + height - radius, radius, radius, 0, 1, 1, 0, uis.corner);  // Правый нижний угол

	trap_R_DrawStretchPic(x, y + radius, radius, height - (radius * 2), 0, 0, 0, 0, uis.whiteShader);                   // Левая сторона
	trap_R_DrawStretchPic(x + width - radius, y + radius, radius, height - (radius * 2), 0, 0, 0, 0, uis.whiteShader);  // Правая сторона
	trap_R_DrawStretchPic(x + radius, y, width - (radius * 2), height, 0, 0, 0, 0, uis.whiteShader);                    // Верхняя сторона
	trap_R_SetColor(NULL);
}

void UI_UpdateScreen(void) { trap_UpdateScreen(); }

static char uiThreadBuffer[MAX_CYCLE_SIZE];
void RunScriptThreads(int time) {
	int i;

	for(i = 0; i < threadsCount; i++) {
		ScriptLoop *script = &threadsLoops[i];
		if(time - script->lastRunTime >= script->interval) {
			script->lastRunTime = time;

			Q_strncpyz(uiThreadBuffer, script->code, MAX_CYCLE_SIZE - 1);
			uiThreadBuffer[MAX_CYCLE_SIZE - 1] = '\0';

			Interpret(uiThreadBuffer);
		}
	}
}

void UI_Refresh(int realtime) {
	int x;
	uis.frametime = realtime - uis.realtime;
	uis.realtime = realtime;

	if(!(trap_Key_GetCatcher() & KEYCATCH_UI)) {
		return;
	}

	ST_UpdateCvars();
	ST_UpdateColors();
	RunScriptThreads(uis.realtime);  // Noire.Script - run threads

	if(uis.activemenu) {
		if(uis.activemenu->fullscreen) {
			if(!uis.onmap) {
				trap_R_DrawStretchPic(0.0, 0.0, glconfig.vidWidth, glconfig.vidHeight, 0, 0, 1, 1, uis.menuWallpapers);
			}
			if(!uis.onmap || !uis.postfx_status) {
				trap_R_DrawStretchPic(0.0, 0.0, glconfig.vidWidth, glconfig.vidHeight, 0, 0, 0.5, 1, trap_R_RegisterShaderNoMip("menu/assets/blacktrans"));
			} else {
				trap_R_DrawStretchPic(0.0, 0.0, glconfig.vidWidth, glconfig.vidHeight, 0, 0, 0.5, 1, trap_R_RegisterShaderNoMip("menu/assets/blacktrans2"));
			}
		}

		if(uis.activemenu->draw)
			uis.activemenu->draw();
		else
			Menu_Draw(uis.activemenu);

		if(uis.firstdraw) {
			UI_MouseEvent(0, 0);
			uis.firstdraw = qfalse;
		}
	}

	// draw cursor
	trap_R_SetColor(NULL);
	UI_DrawHandlePic(uis.cursorx - 16, uis.cursory - 16, 32, 32, uis.cursor);

	if(uis.debug) {
		x = 0 - uis.wideoffset;
		ST_DrawString(x, 0, va("cursor xy: (%d,%d)", uis.cursorx, uis.cursory), UI_LEFT, color_white, 1.00);
		ST_DrawString(x, 10, va("screen: %ix%i", glconfig.vidWidth, glconfig.vidHeight), UI_LEFT, color_white, 1.00);
		ST_DrawString(x, 20, va("map running: %i", uis.onmap), UI_LEFT, color_white, 1.00);
	}
}

qboolean UI_CursorInRect(int x, int y, int width, int height) {
	if(uis.cursorx < x || uis.cursory < y || uis.cursorx > x + width || uis.cursory > y + height) return qfalse;

	return qtrue;
}
