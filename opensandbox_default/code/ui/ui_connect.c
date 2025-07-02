// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

#include "ui_local.h"

qboolean passwordNeeded = qtrue;
menuelement_s passwordField;

static connstate_t lastConnState;
static char lastLoadingText[MAX_INFO_VALUE];

static void UI_ReadableSize(char *buf, int bufsize, int value) {
	if(value > 1024 * 1024 * 1024) {  // gigs
		Com_sprintf(buf, bufsize, "%d", value / (1024 * 1024 * 1024));
		Com_sprintf(buf + strlen(buf), bufsize - strlen(buf), ".%02d GB", (value % (1024 * 1024 * 1024)) * 100 / (1024 * 1024 * 1024));
	} else if(value > 1024 * 1024) {  // megs
		Com_sprintf(buf, bufsize, "%d", value / (1024 * 1024));
		Com_sprintf(buf + strlen(buf), bufsize - strlen(buf), ".%02d MB", (value % (1024 * 1024)) * 100 / (1024 * 1024));
	} else if(value > 1024) {  // kilos
		Com_sprintf(buf, bufsize, "%d KB", value / 1024);
	} else {  // bytes
		Com_sprintf(buf, bufsize, "%d bytes", value);
	}
}

static void UI_PrintTime(char *buf, int bufsize, int time) {
	time /= 1000;  // change to seconds

	if(time > 3600) {  // in the hours range
		Com_sprintf(buf, bufsize, "%d hr %d min", time / 3600, (time % 3600) / 60);
	} else if(time > 60) {  // mins
		Com_sprintf(buf, bufsize, "%d min %d sec", time / 60, time % 60);
	} else {  // secs
		Com_sprintf(buf, bufsize, "%d sec", time);
	}
}

static void UI_DisplayDownloadInfo(const char *downloadName) {
	static char dlText[] = "Downloading:";
	static char etaText[] = "Estimated time left:";
	static char xferText[] = "Transfer rate:";

	int downloadSize, downloadCount, downloadTime;
	char dlSizeBuf[64], totalSizeBuf[64], xferRateBuf[64], dlTimeBuf[64];
	int xferRate;
	int width, leftWidth;
	int style = UI_LEFT | UI_SMALLFONT | UI_DROPSHADOW;
	const char *s;

	downloadSize = trap_Cvar_VariableValue("cl_downloadSize");
	downloadCount = trap_Cvar_VariableValue("cl_downloadCount");
	downloadTime = trap_Cvar_VariableValue("cl_downloadTime");

	leftWidth = width = ST_StringWidth(dlText, 1.00) * 1.00;
	width = ST_StringWidth(etaText, 1.00) * 1.00;
	if(width > leftWidth) leftWidth = width;
	width = ST_StringWidth(xferText, 1.00) * 1.00;
	if(width > leftWidth) leftWidth = width;
	leftWidth += 16;

	ST_DrawString(8, 128, dlText, style, color_white, 1.00);
	ST_DrawString(8, 160, etaText, style, color_white, 1.00);
	ST_DrawString(8, 224, xferText, style, color_white, 1.00);

	if(downloadSize > 0) {
		s = va("%s (%d%%)", downloadName, downloadCount * 100 / downloadSize);
	} else {
		s = downloadName;
	}

	ST_DrawString(leftWidth, 128, s, style, color_white, 1.00);

	UI_ReadableSize(dlSizeBuf, sizeof dlSizeBuf, downloadCount);
	UI_ReadableSize(totalSizeBuf, sizeof totalSizeBuf, downloadSize);

	if(downloadCount < 4096 || !downloadTime) {
		ST_DrawString(leftWidth, 160, "estimating", style, color_white, 1.00);
		ST_DrawString(leftWidth, 192, va("(%s of %s copied)", dlSizeBuf, totalSizeBuf), style, color_white, 1.00);
	} else {
		if((uis.realtime - downloadTime) / 1000) {
			xferRate = downloadCount / ((uis.realtime - downloadTime) / 1000);
		} else {
			xferRate = 0;
		}

		UI_ReadableSize(xferRateBuf, sizeof xferRateBuf, xferRate);

		// Extrapolate estimated completion time
		if(downloadSize && xferRate) {
			int n = downloadSize / xferRate;

			// We do it in K (/1024) because we'd overflow around 4MB
			n = (n - (((downloadCount / 1024) * n) / (downloadSize / 1024))) * 1000;

			UI_PrintTime(dlTimeBuf, sizeof dlTimeBuf, n);  // bk010104

			ST_DrawString(leftWidth, 160, dlTimeBuf, style, color_white, 1.00);
			ST_DrawString(leftWidth, 192, va("(%s of %s copied)", dlSizeBuf, totalSizeBuf), style, color_white, 1.00);
		} else {
			ST_DrawString(leftWidth, 160, "estimating", style, color_white, 1.00);
			if(downloadSize) {
				ST_DrawString(leftWidth, 192, va("(%s of %s copied)", dlSizeBuf, totalSizeBuf), style, color_white, 1.00);
			} else {
				ST_DrawString(leftWidth, 192, va("(%s copied)", dlSizeBuf), style, color_white, 1.00);
			}
		}

		if(xferRate) {
			ST_DrawString(leftWidth, 224, va("%s/Sec", xferRateBuf), style, color_white, 1.00);
		}
	}
}

/*
========================
UI_DrawConnectScreen

This will also be overlaid on the cgame info screen during loading
to prevent it from blinking away too rapidly on local or lan games.
========================
*/
void UI_DrawConnectScreen(qboolean overlay) {
	char *s;
	uiClientState_t cstate;
	char info[MAX_INFO_VALUE];
	int strWidth;
	qhandle_t black;
	qhandle_t logo;
	qhandle_t loading;

	UI_UpdateState();
	trap_Cvar_Set("r_fx_blur", "0");  // blur UI postFX

	// see what information we should display
	trap_GetClientState(&cstate);

	info[0] = '\0';
	trap_GetConfigString(CS_SERVERINFO, info, sizeof(info));

	Menu_Cache();

	black = trap_R_RegisterShaderNoMip("gfx/colors/black");
	logo = trap_R_RegisterShaderNoMip("menu/logo");
	loading = trap_R_RegisterShaderNoMip("menu/assets/loading");

	// print any server info (server full, bad version, etc)
	if(cstate.connState < CA_CONNECTED) {
		ST_DrawString(2 - uis.wideoffset, 2, cstate.messageString, UI_CENTER | UI_SMALLFONT | UI_DROPSHADOW, color_white, 1.00);
	}
	if(lastConnState > cstate.connState) {
		lastLoadingText[0] = '\0';
	}
	lastConnState = cstate.connState;

	switch(cstate.connState) {
		case CA_CONNECTING: s = va("Awaiting challenge...%i", cstate.connectPacketCount); break;
		case CA_CHALLENGING: s = va("Awaiting connection...%i", cstate.connectPacketCount); break;
		case CA_CONNECTED: {
			char downloadName[MAX_INFO_VALUE];

			trap_Cvar_VariableStringBuffer("cl_downloadName", downloadName, sizeof(downloadName));
			if(*downloadName) {
				UI_DisplayDownloadInfo(downloadName);
				return;
			}
		}
			s = "Awaiting gamestate...";
			break;
		case CA_LOADING: return;
		case CA_PRIMED: return;
		default: return;
	}

	UI_DrawHandlePic(0 - (uis.wideoffset + 1), 0, SCREEN_WIDTH + (uis.wideoffset * 2) + 2, SCREEN_HEIGHT * 3, black);
	UI_DrawHandlePic(320 - 50, 240 - 75, 100, 100, logo);
	UI_DrawHandlePic(320 - 24, 320 - 48, 48, 48, loading);

	ST_DrawString(2 - uis.wideoffset, 2, s, UI_SMALLFONT | UI_DROPSHADOW, color_white, 1.00);
}
