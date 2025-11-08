// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

#include "ui_local.h"

#define MAX_RESOLUTIONS 64
#define RESBUF_SIZE 1024

typedef struct {
	menuframework_s menu;
	menuelement_s e[OSUI_MAX_ELEMENTS];
} settings_t;

static settings_t settings;

static const char *modeList[] = {
	"Window",
	"Borderless window",
	"Fullscreen",
	"Full desktop",
	NULL
};

static const char *limitList[] = {
	"Very low",
	"Low",
	"Medium",
	"High",
	"Very High",
	"Ultra",
	NULL
};

static const char *textureList[] = {
	"High",
	"Medium",
	"Low",
	"Very low",
	"Ultra low",
	NULL
};

static const char *aaList[] = {
	"Off",
	"2x",
    "4x",
    "6x",
    "8x",
	NULL
};

static const char *bloomList[] = {
	"Off",
	"Low",
    "Medium",
    "High",
    "Ultra",
	NULL
};

static const char *sdriverList[] = {
	"dsound",
	"wasapi",
	NULL
};

static char* resbuf;
static char* detectedResolutions[MAX_RESOLUTIONS];
static char currentResolution[32];
static char** resolutions = NULL;
static qboolean resolutionsDetected = qfalse;

static void Settings_MenuEvent(void* ptr, int event) {
	if(event != QM_ACTIVATED) return;
	if(((menucommon_s*)ptr)->id == 3) cvarSet("r_resolution", resolutions[settings.e[3].curvalue]);
	if(((menucommon_s*)ptr)->id == 4) cvarSetValue("r_fullscreen", settings.e[4].curvalue);
	if(((menucommon_s*)ptr)->id == 8) {
		if(settings.e[8].curvalue == 0) {
			cvarSetValue("ui_effectslevel", 0);
			cvarSetValue("cg_effectsTime", 10);
			cvarSetValue("cg_effectsLimit", 4096);
			cvarSetValue("cg_effectsGibs", 1);
		}
		if(settings.e[8].curvalue == 1) {
			cvarSetValue("ui_effectslevel", 1);
			cvarSetValue("cg_effectsTime", 30);
			cvarSetValue("cg_effectsLimit", 5120);
			cvarSetValue("cg_effectsGibs", 3);
		}
		if(settings.e[8].curvalue == 2) {
			cvarSetValue("ui_effectslevel", 2);
			cvarSetValue("cg_effectsTime", 60);
			cvarSetValue("cg_effectsLimit", 6144);
			cvarSetValue("cg_effectsGibs", 6);
		}
		if(settings.e[8].curvalue == 3) {
			cvarSetValue("ui_effectslevel", 3);
			cvarSetValue("cg_effectsTime", 90);
			cvarSetValue("cg_effectsLimit", 7168);
			cvarSetValue("cg_effectsGibs", 3);
		}
		if(settings.e[8].curvalue == 4) {
			cvarSetValue("ui_effectslevel", 4);
			cvarSetValue("cg_effectsTime", 300);
			cvarSetValue("cg_effectsLimit", 8192);
			cvarSetValue("cg_effectsGibs", 9);
		}
		if(settings.e[8].curvalue == 5) {
			cvarSetValue("ui_effectslevel", 5);
			cvarSetValue("cg_effectsTime", 600);
			cvarSetValue("cg_effectsLimit", 8192);
			cvarSetValue("cg_effectsGibs", 16);
		}
	}
	if(((menucommon_s*)ptr)->id == 9) cvarSetValue("r_picmip", settings.e[9].curvalue);
	if(((menucommon_s*)ptr)->id == 10) cvarSetValue("r_ext_multisample", settings.e[10].curvalue * 2);
	if(((menucommon_s*)ptr)->id == 11) cvarSetValue("r_bloom_intensity", settings.e[11].curvalue * 0.05);
	if(((menucommon_s*)ptr)->id == 32) cvarSet("s_driver", sdriverList[settings.e[32].curvalue]);
}

static void Settings_GetResolutions(void) {
	resbuf = cvarString("r_availableModes");

	if(*resbuf) {
		char* s = resbuf;
		unsigned int i = 0;
		unsigned int j = 0;

		while(s && i < MAX_RESOLUTIONS - 1) {
			detectedResolutions[i++] = s;
			s = strchr(s, ' ');
			if(s) {
				*s = '\0';
				s++;
			}
		}
		detectedResolutions[i] = NULL;

		if(i < MAX_RESOLUTIONS - 1) {
			Com_sprintf(currentResolution, sizeof(currentResolution), "%dx%d", glconfig.vidWidth, glconfig.vidHeight);

			for(j = 0; j < i; j++) {
				if(!strcmp(detectedResolutions[j], currentResolution)) goto done;
			}

			detectedResolutions[i++] = currentResolution;
			detectedResolutions[i] = NULL;
		}

	done:
		resolutions = detectedResolutions;
		resolutionsDetected = qtrue;
	}
}

void UI_Settings(void) {
	int x, y, i; //ПАСХАЛКЭ

	memset(&settings, 0, sizeof(settings));
	settings.menu.fullscreen = qtrue;

	Settings_GetResolutions();

	UI_CText(&settings.e[0], 10, 12, "Graphics", UI_LEFT, 1.00);
	UI_CText(&settings.e[1], 320, 12, "Sound", UI_CENTER, 1.00);
	UI_CText(&settings.e[2], 630, 12, "Other", UI_RIGHT, 1.00);

	y = 30;
	x = 120;
	UI_CSpinControl(&settings.e[3], x, y, "Resolution:", (const char**)resolutions, Settings_MenuEvent, 0); y += 12;
	for(i = 0; resolutions[i]; i++) {
		if(!strcmp(resolutions[i], cvarString("r_resolution"))) {
			settings.e[3].curvalue = i;
			break;
		}
	}

	UI_CSpinControl(&settings.e[4], x, y, "Mode:", modeList, Settings_MenuEvent, 0); y += 12;
	settings.e[4].curvalue = cvarInt("r_fullscreen");

	UI_CRadioButton(&settings.e[5], x, y, "Anisotropy:", "r_ext_texture_filter_anisotropic", RBT_NORMAL, NULL, 0); y += 12;
	UI_CRadioButton(&settings.e[6], x, y, "Post-processing:", "r_postfx", RBT_NORMAL, NULL, 0); y += 12;
	UI_CRadioButton(&settings.e[7], x, y, "HDR:", "r_hdr", RBT_NORMAL, NULL, 0); y += 12;

	UI_CSpinControl(&settings.e[8], x, y, "Entity limit:", limitList, Settings_MenuEvent, 0); y += 12;
	settings.e[8].curvalue = cvarInt("ui_effectsLevel");

	UI_CSpinControl(&settings.e[9], x, y, "Texture quality:", textureList, Settings_MenuEvent, 0); y += 12;
	settings.e[9].curvalue = cvarInt("r_picmip");

	UI_CSpinControl(&settings.e[10], x, y, "Anti-aliasing:", aaList, Settings_MenuEvent, 0); y += 12;
	settings.e[10].curvalue = cvarInt("r_ext_multisample")/2;

	UI_CSpinControl(&settings.e[11], x, y, "Bloom level:", bloomList, Settings_MenuEvent, 0); y += 12;
	settings.e[11].curvalue = cvarInt("r_bloom_intensity")/0.05;

	UI_CSlider(&settings.e[12], x, y, "Gamma:", "r_gamma", 50, 300, 100, NULL, 0); y += 12;
	y += 12;
	UI_CRadioButton(&settings.e[13], x, y, "V-Sync:", "r_swapInterval", RBT_NORMAL, NULL, 0); y += 12;
	UI_CField(&settings.e[14], x, y, "Max FPS:", 4, 4, color_white, "com_maxfps", NULL, 0); y += 12;

	y = 30;
	x = 330;
	UI_CSlider(&settings.e[30], x, y, "Sound volume:", "s_volume", 0, 100, 100, NULL, 0); y += 12;
	UI_CSlider(&settings.e[31], x, y, "Music volume:", "s_musicVolume", 0, 100, 100, NULL, 0); y += 12;

	UI_CSpinControl(&settings.e[32], x, y, "Sound driver:", sdriverList, Settings_MenuEvent, 0); y += 12;
	for(i = 0; sdriverList[i]; i++) {
		if(!strcmp(sdriverList[i], cvarString("s_driver"))) {
			settings.e[32].curvalue = i;
			break;
		}
	}

	y = 30;
	x = 560;
	UI_CRadioButton(&settings.e[60], x, y, "Draw Speed:", "cg_drawSpeed", RBT_NORMAL, NULL, 0); y += 12;
	UI_CRadioButton(&settings.e[61], x, y, "Draw FPS:", "cg_drawFPS", RBT_NORMAL, NULL, 0); y += 12;
	UI_CRadioButton(&settings.e[62], x, y, "Draw Timer:", "cg_drawTimer", RBT_NORMAL, NULL, 0); y += 12;
	y += 12;
	UI_CSlider(&settings.e[63], x, y, "^1Hud:", "cg_crosshairColorRed", 0, 100, 100, NULL, 0); y += 12;
	UI_CSlider(&settings.e[64], x, y, "^2Hud:", "cg_crosshairColorGreen", 0, 100, 100, NULL, 0); y += 12;
	UI_CSlider(&settings.e[65], x, y, "^4Hud:", "cg_crosshairColorBlue", 0, 100, 100, NULL, 0); y += 12;
	y += 12;
	UI_CSlider(&settings.e[66], x, y, "Crosshair size:", "cg_crosshairScale", 10, 100, 1, NULL, 0); y += 12;
	UI_CSlider(&settings.e[67], x, y, "Crosshair type:", "cg_drawCrosshair", 0, 9, 1, NULL, 0); y += 12;
	y += 12;
	UI_CRadioButton(&settings.e[68], x, y, "3rd person:", "cg_thirdPerson", RBT_NORMAL, NULL, 0); y += 12;
	UI_CSlider(&settings.e[69], x, y, "3rd person offset:", "cg_thirdPersonOffset", -50, 50, 1, NULL, 0); y += 12;
	UI_CSlider(&settings.e[70], x, y, "3rd person range:", "cg_thirdPersonRange", 20, 100, 1, NULL, 0); y += 12;
	y += 12;
	UI_CRadioButton(&settings.e[71], x, y, "Camera eyes:", "cg_cameraEyes", RBT_NORMAL, NULL, 0); y += 12;
	y += 12;
	UI_CRadioButton(&settings.e[72], x, y, "Camera shake:", "cg_enableBobbing", RBT_NORMAL, NULL, 0); y += 12;
	UI_CRadioButton(&settings.e[73], x, y, "Marks:", "cg_addMarks", RBT_NORMAL, NULL, 0); y += 12;
	UI_CRadioButton(&settings.e[74], x, y, "Blood:", "com_blood", RBT_NORMAL, NULL, 0); y += 12;
	y += 12;
	UI_CField(&settings.e[75], x, y, "FoV:", 4, 4, color_white, "cg_fov", NULL, 0); y += 12;
	UI_CField(&settings.e[76], x, y, "Zoom FoV:", 4, 4, color_white, "cg_zoomfov", NULL, 0); y += 12;

	UI_CButton(&settings.e[100], 320, 450, "Apply", UI_CENTER, 1.50, color_white, "vid_restart;", NULL, NULL, NULL, 0);

	UI_CreateUI(&settings.menu, settings.e);
	UI_PushMenu(&settings.menu);
}
