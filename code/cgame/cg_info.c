// Copyright (C) 1999-2005 ID Software, Inc.
// OpenSandbox by noire.dev — GPLv2

#include "../shared/javascript.h"

void CG_LoadingString(const char *s, float value) {
	StringCopy(cg.infoScreenText, va("Loading... %s", s), sizeof(cg.infoScreenText));
	if(value != -1) cg.infoScreenValue = value;
	trap_UpdateScreen();
}

void CG_LoadingClient(int clientNum) {
	const char *info;
	char personality[MAX_QPATH];

	info = CG_ConfigString(CS_PLAYERS + clientNum);
	StringCopy(personality, Info_ValueForKey(info, "n"), sizeof(personality));
	CG_LoadingString(personality, -1);
}

void CG_DrawInformation(void) {
	const char *s;
	const char *info;
	int y;
	char buf[64];
	vec4_t color_white = {1.00f, 1.00f, 1.00f, 1.00f};
	vec4_t color_whiteblack = {0.90f, 0.90f, 0.90f, 1.00f};
	vec4_t color_grey = {0.30f, 0.30f, 0.30f, 1.00f};
	vec4_t color_lightgrey = {0.50f, 0.50f, 0.50f, 1.00f};

	info = CG_ConfigString(CS_SERVERINFO);
	s = Info_ValueForKey(info, "sv_mapname");

	drawShader(0, 0, glconfig.width, glconfig.height, va("levelshots/%s", s));
	drawShader(0, 0, glconfig.width, glconfig.height, "menu/vignette_loading");
	drawRoundedRect(410 + cgui.wideoffset, 445, 220, 30, 1, color_lightgrey, 0);
	CG_DrawProgressBar(415 + cgui.wideoffset, 459, 210, 12, cg.infoScreenValue, 8, color_white, color_grey);
	drawString(415 + cgui.wideoffset, 449, cg.infoScreenText, UI_LEFT, color_whiteblack, 0.80);
	drawShader(320 - 50, 240 - 75, 100, 100, "menu/logo");
	drawShader(320 - 24, 320 - 48, 48, 48, "menu/loading");
	drawShader(0 - cgui.wideoffset, 0, 300, 85, "menu/gradiant_l");
    drawShader(5 - cgui.wideoffset, 5, 100, 75, va("levelshots/%s", s));


	y = 10;
	StringCopy(buf, Info_ValueForKey(info, "sv_hostname"), 64);
	drawString(110 - cgui.wideoffset, y, buf, UI_LEFT, color_white, 1.50);
	y += 25;

	StringCopy(buf, Info_ValueForKey(info, "sv_mapname"), 64);
	drawString(110 - cgui.wideoffset, y, buf, UI_LEFT, color_white, 1.50);
	y += 25;

	switch(cgs.gametype) {
	case GT_SANDBOX: s = "Sandbox"; break;
	case GT_FFA: s = "Free For All"; break;
	case GT_TEAM: s = "Team Deathmatch"; break;
	case GT_CTF: s = "Capture The Flag"; break;
	case GT_1FCTF: s = "One Flag CTF"; break;
	case GT_OBELISK: s = "Overload"; break;
	case GT_HARVESTER: s = "Harvester"; break;
	default: s = "Unknown Gametype"; break;
	}

	drawString(110 - cgui.wideoffset, y, s, UI_LEFT, color_white, 1.50);
}
