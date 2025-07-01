// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

#include "../qcommon/ns_local.h"

static queued3DString_t queued3DStrings[MAX_3D_STRING_QUEUE];
static int queued3DStringCount = 0;
void CG_Add3DString(float x, float y, float z, const char *str, int style, const vec4_t color, float fontSize, float min, float max, qboolean useTrace) {
	queued3DString_t *q = &queued3DStrings[queued3DStringCount++];

	if(queued3DStringCount >= MAX_3D_STRING_QUEUE) return;
	q->x = x;
	q->y = y;
	q->z = z;
	q->str = str;
	q->style = style;
	Vector4Copy(color, q->color);
	q->fontSize = fontSize;
	q->min = min;
	q->max = max;
	q->useTrace = useTrace;
}

static void CG_Draw3DStringQueue(void) {
	int i;
	for(i = 0; i < queued3DStringCount; i++) {
		queued3DString_t *q = &queued3DStrings[i];
		CG_Draw3DString(q->x, q->y, q->z, q->str, q->style, q->color, q->fontSize, q->min, q->max, q->useTrace);
	}
	queued3DStringCount = 0;
}

static void CG_Draw3DModelToolgun(float x, float y, float w, float h, qhandle_t model, char *texlocation, char *material) {
	refdef_t refdef;
	refEntity_t ent;
	vec3_t origin, angles;

	CG_AdjustFrom640(&x, &y, &w, &h);

	memset(&refdef, 0, sizeof(refdef));
	memset(&ent, 0, sizeof(ent));

	origin[0] = 70;
	origin[1] = 0;
	origin[2] = 0;

	angles[0] = 0;
	angles[1] = (cg.time & 4095) * 360 / 4096.0;
	angles[2] = 0;

	AnglesToAxis(angles, ent.axis);
	VectorCopy(origin, ent.origin);
	ent.hModel = model;
	ent.shaderRGBA[0] = 0;
	ent.shaderRGBA[1] = 0;
	ent.shaderRGBA[2] = 0;
	ent.shaderRGBA[3] = 255;

	refdef.rdflags = RDF_NOWORLDMODEL;
	AxisClear(refdef.viewaxis);
	refdef.fov_x = 90;
	refdef.fov_y = 90;

	ent.reType = RT_MODEL;
	ent.customSkin = trap_R_RegisterSkin(va("ptex/%s/%s.skin", texlocation, material));
	if(atoi(material) > 0) ent.customShader = trap_R_RegisterShader(va("ptex/%s/%s", texlocation, material));
	if(atoi(material) == 255) ent.customShader = cgs.media.ptexShader[1];

	refdef.x = x;
	refdef.y = y;
	refdef.width = w;
	refdef.height = h;
	refdef.time = cg.time;

	trap_R_ClearScene();
	trap_R_AddRefEntityToScene(&ent);
	trap_R_RenderScene(&refdef);
}

void CG_DrawHead(float x, float y, float w, float h, int clientNum) {
	clientInfo_t *ci;

	ci = &cgs.clientinfo[clientNum];
	CG_DrawPic(x, y, w, h, ci->modelIcon);
}

static void SplitStringBySpace(const char *str, char result[MAX_ENTITYINFO][64]) {
	int i = 0;
	int wordIndex = 0;
	int charIndex = 0;

	while(str[i] != '\0' && wordIndex < MAX_ENTITYINFO) {
		while(isspace(str[i])) {
			i++;
		}

		while(str[i] != '\0' && !isspace(str[i])) {
			result[wordIndex][charIndex++] = str[i++];
		}

		result[wordIndex][charIndex] = '\0';

		wordIndex++;
		charIndex = 0;
	}
}

#define WICON_SIDE 32 * 0.60
#define WICON_SELECT 32 * 0.90
#define WICON_SPACE 3
#define WICONS_SIDES 6
static void CG_DrawWeaponSelect(void) {
	float y, x, originalX, originalY;
	int i, j;

	if(cg.snap->ps.pm_flags & PMF_FOLLOW) return;

	if(cg.predictedPlayerState.stats[STAT_HEALTH] <= 0) return;

	if(!cg_draw2D.integer || cg.showScores) return;

	originalX = (530 + cgs.wideoffset) - ((WICON_SIDE + WICON_SPACE) * WICONS_SIDES) - ((WICON_SELECT / 2) + WICON_SPACE) - 1.5;
	originalY = 440;

	if(ST_AnimValue(&weaponSelectIn, cg.time)) {
		originalY += ST_AnimValue(&weaponSelectIn, cg.time) * 40;
	}

	if(cg.time - anim_weaponSelect >= 400 + 2000 && anim_weaponSelect) {
		if(!ST_AnimValue(&weaponSelectOut, cg.time)) {
			anim_weaponSelect = 0;
			ST_AnimStart(&weaponSelectOut, cg.time, 600);
		}
	}

	if(ST_AnimValue(&weaponSelectOut, cg.time)) {
		originalY += 40;
		originalY -= ST_AnimValue(&weaponSelectOut, cg.time) * 40;
	} else {
		if(!anim_weaponSelect) originalY += 40;
	}

	y = originalY + (WICON_SELECT * 0.25);
	x = originalX + (WICON_SELECT / 2) + WICON_SPACE;

	CG_DrawRoundedRect((originalX - WICONS_SIDES) - ((WICON_SIDE + WICON_SPACE) * WICONS_SIDES) - ((WICON_SELECT / 2) + WICON_SPACE), originalY, ((WICON_SIDE + WICON_SPACE) * WICONS_SIDES * 2) + WICON_SELECT + WICON_SPACE * 2 + WICONS_SIDES, WICON_SELECT + WICON_SPACE, 4, color_dim);

	for(i = cg.weaponSelect + 1, j = 0; i <= WEAPONS_NUM; i++) {
		if(j >= WICONS_SIDES) {
			continue;
		} else if(i >= WEAPONS_NUM) {
			i = 1;
		}

		if(!cg.swep_listcl[i]) continue;

		CG_RegisterWeapon(i);
		CG_DrawPic(x, y, WICON_SIDE, WICON_SIDE, cg_weapons[i].weaponIcon);

		if(cg.swep_listcl[i] == WS_NOAMMO) CG_DrawPic(x, y, WICON_SIDE, WICON_SIDE, cgs.media.noammoShader);

		x += WICON_SIDE + WICON_SPACE;
		j++;
	}

	x = originalX - (WICON_SELECT / 2) - (WICON_SIDE + WICON_SPACE);
	for(i = cg.weaponSelect - 1, j = 0; i >= 0; i--) {
		if(j >= WICONS_SIDES) {
			continue;
		} else if(i <= 0) {
			i = WEAPONS_NUM - 1;
		}

		if(!cg.swep_listcl[i]) continue;

		CG_RegisterWeapon(i);
		CG_DrawPic(x, y, WICON_SIDE, WICON_SIDE, cg_weapons[i].weaponIcon);

		if(cg.swep_listcl[i] == WS_NOAMMO) CG_DrawPic(x, y, WICON_SIDE, WICON_SIDE, cgs.media.noammoShader);

		x -= WICON_SIDE + WICON_SPACE;
		j++;
	}

	CG_DrawPic(originalX - WICON_SELECT / 2, originalY + 2, WICON_SELECT, WICON_SELECT, cg_weapons[cg.weaponSelect].weaponIcon);
	CG_DrawPic(originalX - WICON_SELECT / 2, originalY + 2, WICON_SELECT, WICON_SELECT, cgs.media.selectShader);
	if(cg.swep_listcl[cg.weaponSelect] == WS_NOAMMO) CG_DrawPic(originalX - WICON_SELECT / 2, originalY + 2, WICON_SELECT, WICON_SELECT, cgs.media.noammoShader);
}

static void CG_DrawToolgun() {
	char entityInfos[MAX_ENTITYINFO][64];
	float x, y;
	item_t *it;
	int count = 0;
	int i = 0;
	vec4_t colorblk;

	colorblk[0] = 0.0f;
	colorblk[1] = 0.0f;
	colorblk[2] = 0.0f;
	colorblk[3] = 0.75f;

	if(toolgun_tool.integer == TL_CREATE) {
		trap_R_RemapShader("models/weapons/toolgun/screen", "models/weapons/toolgun/screen", "0.005");
	} else {
		if(trap_R_RegisterShader(va("models/weapons/toolgun/tool%i", toolgun_tool.integer)) != 0) {
			trap_R_RemapShader("models/weapons/toolgun/screen", va("models/weapons/toolgun/tool%i", toolgun_tool.integer), "0.005");
		} else {
			trap_R_RemapShader("models/weapons/toolgun/screen", "models/weapons/toolgun/toolerror", "0.005");
		}
	}

	CG_DrawPic(-1 - cgs.wideoffset, 40, 300, 125, trap_R_RegisterShaderNoMip("menu/assets/blacktrans"));
	ST_DrawString(0 - cgs.wideoffset, 42, toolgun_tooltext.string, UI_LEFT, color_white, 2.50);
	if(toolgun_mod19.integer == 0) {
		ST_DrawString(0 - cgs.wideoffset, 72, toolgun_toolmode1.string, UI_LEFT, color_white, 1.32);
	} else if(toolgun_mod19.integer == 1) {
		ST_DrawString(0 - cgs.wideoffset, 72, toolgun_toolmode2.string, UI_LEFT, color_white, 1.32);
	} else if(toolgun_mod19.integer == 2) {
		ST_DrawString(0 - cgs.wideoffset, 72, toolgun_toolmode3.string, UI_LEFT, color_white, 1.32);
	} else if(toolgun_mod19.integer == 3) {
		ST_DrawString(0 - cgs.wideoffset, 72, toolgun_toolmode4.string, UI_LEFT, color_white, 1.32);
	}
	y = 90;
	ST_DrawString(0 - cgs.wideoffset, y, toolgun_tooltip1.string, UI_LEFT, color_white, 1.32);
	y += 15;
	ST_DrawString(0 - cgs.wideoffset, y, toolgun_tooltip2.string, UI_LEFT, color_white, 1.32);
	y += 15;
	ST_DrawString(0 - cgs.wideoffset, y, toolgun_tooltip3.string, UI_LEFT, color_white, 1.32);
	y += 15;
	ST_DrawString(0 - cgs.wideoffset, y, toolgun_tooltip4.string, UI_LEFT, color_white, 1.32);

	if(strlen(cg.entityInfo) > 0) {
		SplitStringBySpace(cg.entityInfo, entityInfos);

		x = 340;
		y = 260 + 48;
		CG_DrawRoundedRect(x - 5, y - 48 - 5, 300, 48 + 15 + (MAX_ENTITYINFO * 10), 6, colorblk);

		if(strcmp(entityInfos[0], "<NULL>")) {
			if(!BG_CheckClassname(entityInfos[0])) {
				// nothing
			} else {
				for(it = gameInfoItems + 1; it->classname; it++) {
					if(!Q_stricmp(it->classname, entityInfos[0])) {
						CG_DrawPic(x, y - 48, 48, 48, trap_R_RegisterShaderNoMip(it->icon));
					}
				}
			}
			ST_DrawString(x, y + 3 + (count * 10), va("Class: %s", entityInfos[0]), UI_LEFT, color_white, 1.00);
			count++;
		}

		if(strcmp(entityInfos[1], "<NULL>")) {
			if(!BG_CheckClassname(entityInfos[0])) {
				CG_Draw3DModelToolgun(x, y - 48, 48, 48, trap_R_RegisterModel(entityInfos[1]), entityInfos[1], entityInfos[2]);
			} else {
				// nothing
			}
			if(!strcmp(entityInfos[0], "player")) {
				CG_DrawHead(x, y - 48, 48, 48, atoi(entityInfos[1]));
				count--;
			} else {
				ST_DrawString(x, y + 3 + (count * 10), va("Model: %s", entityInfos[1]), UI_LEFT, color_white, 1.00);
			}
			count++;
		}

		if(strcmp(entityInfos[2], "<NULL>")) {
			ST_DrawString(x, y + 3 + (count * 10), va("Material: %s", entityInfos[2]), UI_LEFT, color_white, 1.00);
			count++;
		}

		if(strcmp(entityInfos[3], "<NULL>")) {
			if(strcmp(entityInfos[3], "0")) {
				ST_DrawString(x, y + 3 + (count * 10), va("Count: %s", entityInfos[3]), UI_LEFT, color_white, 1.00);
				count++;
			} else {
				for(it = gameInfoItems + 1; it->classname; it++) {
					if(!Q_stricmp(it->classname, entityInfos[0])) {
						ST_DrawString(x, y + 3 + (count * 10), va("Count: %i", it->quantity), UI_LEFT, color_white, 1.00);
						count++;
					}
				}
			}
		}
	}
}

static void CG_DrawStatusElement(float x, float y, int value, const char *text) {
	CG_DrawRoundedRect(x, y, 100, 32, 4, color_dim);
	ST_DrawString(x + 4, y + 18, text, UI_LEFT, customcolor_crosshair, 0.90);
	ST_DrawString(x + 42, y + 6, va("%i", value), UI_LEFT, customcolor_crosshair, 2.32);
}

static void CG_DrawCounterElement(float x, float y, const char *value, const char *text) {
	CG_DrawRoundedRect(x, y, 80, 15, 2, color_dim);
	ST_DrawString(x + 2, y + 5, text, UI_LEFT, customcolor_crosshair, 0.90);
	ST_DrawString(x + 40, y + 3, value, UI_LEFT, customcolor_crosshair, 1.20);
}

static void CG_DrawStatusBar(void) {
	centity_t *cent;
	playerState_t *ps;
	int value;
	vec_t *vel;

	vel = cg.snap->ps.velocity;
	cent = &cg_entities[cg.snap->ps.clientNum];
	ps = &cg.snap->ps;

	if(cg.showScores || !cg_draw2D.integer) return;
	if(cent->currentState.weapon == WP_TOOLGUN) CG_DrawToolgun();

	if(cg.snap->ps.stats[STAT_HOLDABLE_ITEM] || cg.snap->ps.stats[STAT_PERSISTANT_POWERUP] || cg.predictedPlayerState.powerups[PW_REDFLAG] || cg.predictedPlayerState.powerups[PW_BLUEFLAG] || cg.predictedPlayerState.powerups[PW_NEUTRALFLAG]) CG_DrawRoundedRect(10 - cgs.wideoffset, 420 - 2, (ICON_SIZE + 4) * 3, ICON_SIZE + 4, 4, color_dim);

	value = cg.snap->ps.stats[STAT_HOLDABLE_ITEM];
	if(value) CG_DrawPic((10 - cgs.wideoffset) + 4, 420, ICON_SIZE, ICON_SIZE, cg_items[value].icon);

	value = cg.snap->ps.stats[STAT_PERSISTANT_POWERUP];
	if(value) CG_DrawPic(10 - cgs.wideoffset + (ICON_SIZE + 6), 420, ICON_SIZE, ICON_SIZE, cg_items[value].icon);

	if(cg.predictedPlayerState.powerups[PW_REDFLAG]) {
		CG_DrawPic(10 - cgs.wideoffset + ((ICON_SIZE + 4) * 2), 420, ICON_SIZE, ICON_SIZE, cg_items[ITEM_INDEX(BG_FindItemForPowerup(PW_REDFLAG))].icon);
	} else if(cg.predictedPlayerState.powerups[PW_BLUEFLAG]) {
		CG_DrawPic(10 - cgs.wideoffset + ((ICON_SIZE + 4) * 2), 420, ICON_SIZE, ICON_SIZE, cg_items[ITEM_INDEX(BG_FindItemForPowerup(PW_BLUEFLAG))].icon);
	} else if(cg.predictedPlayerState.powerups[PW_NEUTRALFLAG]) {
		CG_DrawPic(10 - cgs.wideoffset + ((ICON_SIZE + 4) * 2), 420, ICON_SIZE, ICON_SIZE, cg_items[ITEM_INDEX(BG_FindItemForPowerup(PW_NEUTRALFLAG))].icon);
	}

	if(!ps->stats[STAT_VEHICLE]) {
		if(cent->currentState.weapon) {  // VEHICLE-SYSTEM: vehicle's speedmeter for all
			value = ps->stats[STAT_AMMO];
			if(value <= 0 && value != -1) {  // OpenSandbox weapon predict
				cg.swep_listcl[ps->weapon] = WS_NOAMMO;
			} else {
				cg.swep_listcl[ps->weapon] = WS_HAVE;
			}
			CG_DrawWeaponSelect();
			if(value > -1) CG_DrawStatusElement(630 + cgs.wideoffset - 100, 440, value, "AMMO");
		}
	} else {
		value = sqrt(vel[0] * vel[0] + vel[1] * vel[1]) * 0.20;
		CG_DrawStatusElement(630 + cgs.wideoffset - 100, 440, value, "KM/H");
	}

	if(!ps->stats[STAT_VEHICLE]) {  // VEHICLE-SYSTEM: vehicle's hp instead player
		value = ps->stats[STAT_HEALTH];
	} else {
		value = ps->stats[STAT_VEHICLEHP];
	}
	CG_DrawStatusElement(10 - cgs.wideoffset, 440, value, "HEALTH");

	value = ps->stats[STAT_ARMOR];
	if(value > 0) CG_DrawStatusElement(20 - cgs.wideoffset + 100, 440, value, "ARMOR");
}

qboolean n_tip1 = qfalse;
qboolean n_tip2 = qfalse;

#define FPS_FRAMES 4
static void CG_DrawCounters(void) {
	int y;
	int value;
	int t, frameTime;
	static int previousTimes[FPS_FRAMES];
	static int index;
	static int previous;
	int i, total;
	int mins, seconds, tens;
	int msec;

	msec = cg.time - cgs.levelStartTime;
	seconds = msec / 1000;

	if(seconds < 3) {
		n_tip1 = qfalse;
		n_tip2 = qfalse;
	}

	if(cgs.gametype == GT_SANDBOX) {
		if(seconds == 3 && !n_tip1) {
			CG_AddNotify("Welcome to OpenSandbox", NOTIFY_INFO, 0);
			n_tip1 = qtrue;
		}

		if(seconds == 6 && !n_tip2) {
			CG_AddNotify("Press [Q] to open the spawn menu", NOTIFY_INFO, 0);
			n_tip2 = qtrue;
		}
	}

	y = 4;

	// Speed
	value = sqrt(cg.snap->ps.velocity[0] * cg.snap->ps.velocity[0] + cg.snap->ps.velocity[1] * cg.snap->ps.velocity[1]);
	if(cg_drawSpeed.integer == 1) {
		CG_DrawCounterElement(640 + cgs.wideoffset - 84, y, va("%i", value), "Speed");
		y += 20;
	}

	// FPS
	t = trap_Milliseconds();
	frameTime = t - previous;
	previous = t;
	previousTimes[index % FPS_FRAMES] = frameTime;
	index++;
	if(index > FPS_FRAMES) {
		// average multiple frames together to smooth changes out a bit
		total = 0;
		for(i = 0; i < FPS_FRAMES; i++) {
			total += previousTimes[i];
		}
		if(!total) total = 1;
		value = 1000 * FPS_FRAMES / total;
	}
	if(value <= 15 && seconds >= 3) trap_Cvar_Set("ns_haveerror", "2");
	if(cg_drawFPS.integer == 1) {
		CG_DrawCounterElement(640 + cgs.wideoffset - 84, y, va("%i", value), "FPS");
		y += 20;
	}

	// Timer
	msec = cg.time - cgs.levelStartTime;
	seconds = msec / 1000;
	mins = seconds / 60;
	seconds -= mins * 60;
	tens = seconds / 10;
	seconds -= tens * 10;
	if(cg_drawTimer.integer == 1) {
		CG_DrawCounterElement(640 + cgs.wideoffset - 84, y, va("%i:%i%i", mins, tens, seconds), "Time");
		y += 20;
	}

	// Skulls!
	if(cgs.gametype == GT_HARVESTER) {
		value = cg.snap->ps.generic1;
		if(value > 0) CG_DrawCounterElement(640 + cgs.wideoffset - 84, y, va("%i", value), "Skulls");
	}
}

#define SCORES_WIDTH 32
#define SCORES_HEIGHT 20
static void CG_DrawScores(void) {
	const char *s;
	int s1, s2, score;
	int x;
	int v;
	vec4_t colorb, colorr, colorg, colorgr;
	float y = 0;

	colorb[0] = 0.0f;
	colorb[1] = 0.0f;
	colorb[2] = 1.0f;
	colorb[3] = 0.30f;

	colorr[0] = 1.0f;
	colorr[1] = 0.0f;
	colorr[2] = 0.0f;
	colorr[3] = 0.30f;

	colorg[0] = 0.5f;
	colorg[1] = 0.5f;
	colorg[2] = 0.5f;
	colorg[3] = 0.30f;

	colorgr[0] = 0.0f;
	colorgr[1] = 1.0f;
	colorgr[2] = 0.0f;
	colorgr[3] = 0.30f;

	s1 = cgs.scores1;
	s2 = cgs.scores2;

	// draw from the right side to left
	if(cgs.gametype >= GT_TEAM) {
		// blue score
		x = 320;
		s = va("%i", s2);
		CG_DrawRoundedRect(x, y, SCORES_WIDTH, SCORES_HEIGHT, 0, colorb);
		ST_DrawString(x + 16, y + 3, s, UI_CENTER, color_white, 1.20);

		// blue flag
		if(cgs.gametype == GT_CTF) {
			if(BG_FindItemForPowerup(PW_BLUEFLAG)) {
				if(cgs.blueflag >= 0 && cgs.blueflag <= 2) {
					CG_DrawPic(x + SCORES_WIDTH, y, SCORES_WIDTH, SCORES_HEIGHT, cgs.media.blueFlagShader[cgs.blueflag]);
				}
			}
		}

		if(cgs.gametype == GT_OBELISK) {
			s = va("%i%%", cg.blueObeliskHealth);
			CG_DrawRoundedRect(x + SCORES_WIDTH, y, SCORES_WIDTH, SCORES_HEIGHT - 4, 0, color_dim);
			ST_DrawString(x + (SCORES_WIDTH * 1.50), y + 2, s, UI_CENTER, color_white, 1.20);
		}

		// red score
		s = va("%i", s1);
		x = 320 - SCORES_WIDTH;
		CG_DrawRoundedRect(x, y, SCORES_WIDTH, SCORES_HEIGHT, 0, colorr);
		ST_DrawString(x + 16, y + 3, s, UI_CENTER, color_white, 1.20);

		// red flag
		if(cgs.gametype == GT_CTF) {
			if(BG_FindItemForPowerup(PW_REDFLAG)) {
				if(cgs.redflag >= 0 && cgs.redflag <= 2) {
					CG_DrawPic(x - SCORES_WIDTH, y, SCORES_WIDTH, SCORES_HEIGHT, cgs.media.redFlagShader[cgs.redflag]);
				}
			}
		}

		if(cgs.gametype == GT_OBELISK) {
			s = va("%i%%", cg.redObeliskHealth);
			CG_DrawRoundedRect(x - SCORES_WIDTH, y, SCORES_WIDTH, SCORES_HEIGHT - 4, 0, color_dim);
			ST_DrawString(x - (SCORES_WIDTH * 0.50), y + 2, s, UI_CENTER, color_white, 1.20);
		}

		// limit
		if(cgs.gametype >= GT_CTF) {
			v = cgs.capturelimit;
		} else {
			v = cgs.fraglimit;
		}
		if(v) {
			s = va("%i", v);
			x = 320 - 32;
			CG_DrawRoundedRect(x, y + SCORES_HEIGHT, SCORES_WIDTH * 2, SCORES_HEIGHT, 0, color_dim);
			ST_DrawString(x + SCORES_WIDTH, y + 23, s, UI_CENTER, color_white, 1.20);
		}

	} else {
		x = 320;
		score = cg.snap->ps.persistant[PERS_SCORE];

		// always show your score in the second box if not in first place
		if(s1 != score) {
			s2 = score;
		}

		// player place
		if(s2 != SCORE_NOT_PRESENT) {
			s = va("%i", s2);
			if(s1 != score) {
				CG_DrawRoundedRect(x, y, SCORES_WIDTH, SCORES_HEIGHT, 0, colorgr);
			} else {
				CG_DrawRoundedRect(x, y, SCORES_WIDTH, SCORES_HEIGHT, 0, colorg);
			}
			ST_DrawString(x + 16, y + 3, s, UI_CENTER, color_white, 1.20);
		}

		// first place
		if(s1 != SCORE_NOT_PRESENT) {
			s = va("%i", s1);
			if(s1 == score) {
				CG_DrawRoundedRect(x - SCORES_WIDTH, y, SCORES_WIDTH, SCORES_HEIGHT, 0, colorgr);
			} else {
				CG_DrawRoundedRect(x - SCORES_WIDTH, y, SCORES_WIDTH, SCORES_HEIGHT, 0, colorg);
			}
			CG_DrawRoundedRect(x - SCORES_WIDTH, y, SCORES_WIDTH, SCORES_HEIGHT, 0, colorg);
			ST_DrawString((x + 16) - SCORES_WIDTH, y + 3, s, UI_CENTER, color_white, 1.20);
		}

		// limit
		if(cgs.fraglimit) {
			s = va("%i", cgs.fraglimit);
			x = 320 - 32;
			CG_DrawRoundedRect(x, y + SCORES_HEIGHT, 64, 20, 0, color_dim);
			ST_DrawString(x + SCORES_WIDTH, y + 23, s, UI_CENTER, color_white, 1.20);
		}
	}
}

static void CG_DrawPowerups(void) {
	playerState_t *ps = &cg.snap->ps;
	item_t *item;
	int i, t;
	float y = 125;

	if(ps->stats[STAT_HEALTH] <= 0) return;

	for(i = 0; i < MAX_POWERUPS; i++) {
		if(!ps->powerups[i]) continue;

		t = ps->powerups[i] - cg.time;
		if(t < 0 || t > 99999000) continue;

		item = BG_FindItemForPowerup(i);
		if(!item || item->giType == IT_RUNE) continue;

		y += ICON_SIZE + 2;
		CG_DrawRoundedRect(2 - cgs.wideoffset, y, 64, ICON_SIZE, 4, color_dim);
		CG_DrawPic(3 - cgs.wideoffset, y, ICON_SIZE, ICON_SIZE, trap_R_RegisterShader(item->icon));
		ST_DrawString((4 - cgs.wideoffset) + ICON_SIZE, y + 3, va("%i", t / 1000), UI_LEFT, color_white, 1.25);
	}
}

static void CG_ConsoleUpdateIdx(console_t *console, int chatHeight) {
	if(console->insertIdx < console->displayIdx) console->displayIdx = console->insertIdx;

	if(console->insertIdx - console->displayIdx > chatHeight) console->displayIdx = console->insertIdx - chatHeight;
}

static void CG_DrawGenericConsole(console_t *console, int maxlines, int time, int x, int y, float size) {
	int i, j;
	vec4_t hcolor;

	CG_ConsoleUpdateIdx(console, maxlines);
	hcolor[0] = hcolor[1] = hcolor[2] = hcolor[3] = 1.0f;

	j = 0;
	for(i = console->displayIdx; i < console->insertIdx; ++i) {
		if(console->msgTimes[i % CONSOLE_MAXHEIGHT] + time < cg.time) continue;
		ST_DrawString(x + 1, y + (j * (BASEFONT_HEIGHT * size)), console->msgs[i % CONSOLE_MAXHEIGHT], UI_LEFT | UI_DROPSHADOW, color_white, size);
		j++;
	}
}

void CG_AddToGenericConsole(const char *str, console_t *console) {
	int len;
	char *p, *ls;
	int lastcolor;

	len = 0;

	p = console->msgs[console->insertIdx % CONSOLE_MAXHEIGHT];
	*p = 0;

	lastcolor = '7';

	ls = NULL;
	while(*str) {
		if(*str == '\n' || len > CONSOLE_WIDTH - 1) {
			if(*str == '\n') {
				str++;
				if(*str == '\0') continue;
			} else if(ls) {
				str -= (p - ls);
				str++;
				p -= (p - ls);
			}
			*p = 0;

			console->msgTimes[console->insertIdx % CONSOLE_MAXHEIGHT] = cg.time;

			console->insertIdx++;
			p = console->msgs[console->insertIdx % CONSOLE_MAXHEIGHT];
			*p = 0;
			*p++ = Q_COLOR_ESCAPE;
			*p++ = lastcolor;
			len = 0;
			ls = NULL;
		}

		if(Q_IsColorString(str)) {
			*p++ = *str++;
			lastcolor = *str;
			*p++ = *str++;
			continue;
		}
		if(*str == ' ') ls = p;
		*p++ = *str++;
		len++;
	}
	*p = 0;

	console->msgTimes[console->insertIdx % CONSOLE_MAXHEIGHT] = cg.time;
	console->insertIdx++;
}

void CG_CenterPrint(const char *str, int y, int charWidth) {
	char *s;

	Q_strncpyz(cg.centerPrint, str, sizeof(cg.centerPrint));

	cg.centerPrintTime = cg.time;
	cg.centerPrintY = y;
	cg.centerPrintCharWidth = charWidth;

	// count the number of lines for centering
	cg.centerPrintLines = 1;
	s = cg.centerPrint;
	while(*s) {
		if(*s == '\n') cg.centerPrintLines++;
		s++;
	}
}

static void CG_DrawCenterString(void) {
	char *start;
	int l, y;
	float *color;

	if(!cg.centerPrintTime) return;

	color = CG_FadeColor(cg.centerPrintTime, 1000 * 5);
	if(!color) return;

	trap_R_SetColor(color);

	start = cg.centerPrint;
	y = cg.centerPrintY - cg.centerPrintLines * BIGCHAR_HEIGHT / 2;

	while(1) {
		char linebuffer[1024];

		for(l = 0; l < 50; l++) {
			if(!start[l] || start[l] == '\n') break;
			linebuffer[l] = start[l];
		}
		linebuffer[l] = 0;

		ST_DrawString(SCREEN_WIDTH / 2, y, linebuffer, UI_CENTER|UI_DROPSHADOW, color_white, 1.25);

		y += cg.centerPrintCharWidth * 1.5;
		while(*start && (*start != '\n')) {
			start++;
		}
		if(!*start) break;
		start++;
	}

	trap_R_SetColor(NULL);
}

static void CG_Draw1FCTF(void) {
	if(cgs.gametype != GT_1FCTF) return;

	switch(cgs.flagStatus) {
		case 2: CG_DrawPic(320 - (ICON_SIZE * 0.75), 50, ICON_SIZE * 1.50, ICON_SIZE * 1.50, cg_items[ITEM_INDEX(BG_FindItemForPowerup(PW_REDFLAG))].icon); break;
		case 3: CG_DrawPic(320 - (ICON_SIZE * 0.75), 50, ICON_SIZE * 1.50, ICON_SIZE * 1.50, cg_items[ITEM_INDEX(BG_FindItemForPowerup(PW_BLUEFLAG))].icon); break;
		case 4: CG_DrawPic(320 - (ICON_SIZE * 0.75), 50, ICON_SIZE * 1.50, ICON_SIZE * 1.50, cg_items[ITEM_INDEX(BG_FindItemForPowerup(PW_NEUTRALFLAG))].icon); break;
		default: return;
	};
}

static void CG_ScanForCrosshairEntity(void) {
	trace_t trace;
	vec3_t start, end;
	int content;

	VectorCopy(cg.refdef.vieworg, start);
	VectorMA(start, 131072, cg.refdef.viewaxis[0], end);

	CG_Trace(&trace, start, vec3_origin, vec3_origin, end, cg.snap->ps.clientNum, CONTENTS_SOLID | CONTENTS_BODY);
	if(trace.entityNum >= MAX_CLIENTS) return;

	content = trap_CM_PointContents(trace.endpos, 0);
	if(content & CONTENTS_FOG) return;

	if(cg_entities[trace.entityNum].currentState.powerups & (1 << PW_INVIS)) return;

	cg.crosshairClientNum = trace.entityNum;
}

static void CG_DrawCrosshair(void) {
	float cSize;
	qhandle_t hShader;
	
	if(!cg_drawCrosshair.integer || cg.renderingThirdPerson || cg.snap->ps.persistant[PERS_TEAM] == TEAM_SPECTATOR) return;

	trap_R_SetColor(customcolor_crosshair);
	cSize = cg_crosshairScale.value;
	if(cg_drawCrosshair.integer > 0) {
		hShader = cgs.media.crosshairShader[cg_drawCrosshair.integer % NUM_CROSSHAIRS];
		if(!hShader) hShader = cgs.media.crosshairShader[0];
		CG_DrawPic(320-(cSize*0.5), 240-(cSize*0.5), cSize, cSize, hShader);
	}
	trap_R_SetColor(NULL);
}

static void CG_Draw3DCrosshair(float x, float y, float z) {
	vec3_t dir;
	float localX, localY, localZ;
	float tanFovX, tanFovY;
	float finalx, finaly;
	float cSize;
	vec3_t worldPos;
	qhandle_t hShader;

	worldPos[0] = x;
	worldPos[1] = y;
	worldPos[2] = z;

	VectorSubtract(worldPos, cg.refdef.vieworg, dir);

	localX = -DotProduct(dir, cg.refdef.viewaxis[1]);
	localY = DotProduct(dir, cg.refdef.viewaxis[2]);
	localZ = DotProduct(dir, cg.refdef.viewaxis[0]);

	if(localZ <= 0) return;

	tanFovX = tan(DEG2RAD(cg.refdef.fov_x * 0.5f));
	tanFovY = tan(DEG2RAD(cg.refdef.fov_y * 0.5f));

	finalx = (localX / (localZ * tanFovX)) * (320 + cgs.wideoffset) + 320;
	finaly = (-localY / (localZ * tanFovY)) * 240 + 240;

	trap_R_SetColor(customcolor_crosshair);
	cSize = cg_crosshairScale.value;
	hShader = cgs.media.crosshairShader[cg_drawCrosshair.integer % NUM_CROSSHAIRS];
	CG_DrawPic(finalx-(cSize*0.5), finaly-(cSize*0.5), cSize, cSize, hShader);
	trap_R_SetColor(NULL);
}

static void CG_DrawCrosshair3D(void) {
	trace_t trace;
	vec3_t origin, endpos;
	vec3_t axis[3];

	if(!cg_drawCrosshair.integer || cg.snap->ps.persistant[PERS_TEAM] == TEAM_SPECTATOR) return;

	if(cg.predictedPlayerState.pm_type == PM_DEAD || cg.predictedPlayerState.pm_type == PM_INTERMISSION) return;

	VectorCopy(cg.predictedPlayerState.origin, origin);
	origin[2] += cg.predictedPlayerState.viewheight;
	AnglesToAxis(cg.predictedPlayerState.viewangles, axis);
	VectorMA(origin, 65536, axis[0], endpos);

	CG_Trace(&trace, origin, NULL, NULL, endpos, 0, MASK_SHOT);
	CG_Draw3DCrosshair(trace.endpos[0], trace.endpos[1], trace.endpos[2]);
}

static void CG_DrawIntermission(void) { cg.scoreBoardShowing = CG_DrawScoreboard(); }

static qboolean CG_DrawFollow(void) {
	const char *name;

	if(!(cg.snap->ps.pm_flags & PMF_FOLLOW)) return qfalse;

	name = cgs.clientinfo[cg.snap->ps.clientNum].name;
	ST_DrawString(320, 10, name, UI_CENTER, color_white, 1.00);
	return qtrue;
}

static qhandle_t CG_FindModImage(int mod) {
	if(mod > 0 && mod < MOD_WEAPONS) return cg_items[ITEM_INDEX(BG_FindItemForWeapon(mod))].icon;
	if(mod == MOD_WATER) return trap_R_RegisterShaderNoMip("icons/d_water");
	if(mod == MOD_SLIME) return trap_R_RegisterShaderNoMip("icons/d_slime");
	if(mod == MOD_LAVA) return trap_R_RegisterShaderNoMip("icons/d_lava");
	if(mod == MOD_CRUSH) return trap_R_RegisterShaderNoMip("icons/d_death");
	if(mod == MOD_TELEFRAG) return trap_R_RegisterShaderNoMip("icons/teleporter");
	if(mod == MOD_FALLING) return trap_R_RegisterShaderNoMip("icons/d_fall");
	if(mod == MOD_SUICIDE) return trap_R_RegisterShaderNoMip("icons/d_death");
	if(mod == MOD_TRIGGER_HURT) return trap_R_RegisterShaderNoMip("icons/d_death");
	if(mod == MOD_KAMIKAZE) return trap_R_RegisterShaderNoMip("icons/kamikaze");
	if(mod == MOD_JUICED) return trap_R_RegisterShaderNoMip("icons/d_death");
	if(mod == MOD_CAR) return trap_R_RegisterShaderNoMip("icons/d_car");
	if(mod == MOD_CAREXPLODE) return trap_R_RegisterShaderNoMip("icons/d_car");
	if(mod == MOD_PROP) return trap_R_RegisterShaderNoMip("icons/iconw_propgun");
	return trap_R_RegisterShaderNoMip("icons/d_death");
}

static int NSErrorTime = 0;
static void CG_NSErrors(void) {
	const int fadeInDuration = 1000;
	const int visibleDuration = 1000;
	const int fadeOutDuration = 1000;
	float alpha = 1.0f, sizeX = 0;
	int elapsed;

	vec4_t color = {0.92f, 0.72f, 0.20f, 1.00f};
	vec4_t colord = {0.30f, 0.24f, 0.06f, 1.00f};
	vec4_t colortex = {0.20f, 0.20f, 0.20f, 1.00f};
	vec4_t colorpic = {1.00f, 1.00f, 1.00f, 1.00f};

	if(!ns_haveerror.integer) return;

	if(!NSErrorTime) NSErrorTime = cg.time;

	elapsed = cg.time - NSErrorTime;

	if(elapsed >= fadeInDuration + visibleDuration + fadeOutDuration) {
		trap_Cvar_Set("ns_haveerror", "0");
		NSErrorTime = 0;
		return;
	}

	if(elapsed < fadeInDuration) {
		alpha = (float)elapsed / fadeInDuration;
	} else if(elapsed > fadeInDuration + visibleDuration) {
		alpha = 1.0f - ((float)(elapsed - fadeInDuration - visibleDuration) / fadeOutDuration);
	}

	color[3] *= alpha;
	colord[3] *= alpha;
	colortex[3] *= alpha;
	colorpic[3] *= alpha;

	if(ns_haveerror.integer == 1) sizeX = 180;
	if(ns_haveerror.integer == 2) sizeX = 205;
	if(ns_haveerror.integer == 3) sizeX = 210;
	CG_DrawRoundedRect(21 - cgs.wideoffset, 21, sizeX, 16, 0, colord);
	CG_DrawRoundedRect(20 - cgs.wideoffset, 20, sizeX, 16, 0, color);
	trap_R_SetColor(colorpic);
	CG_DrawPic(23 - cgs.wideoffset, 23, 10, 10, cgs.media.errIcon);
	trap_R_SetColor(NULL);
	if(ns_haveerror.integer == 1) ST_DrawString(38 - cgs.wideoffset, 24, "Something is creating script errors", UI_LEFT, colortex, 0.80f);
	if(ns_haveerror.integer == 2) ST_DrawString(38 - cgs.wideoffset, 24, "Low performance, physics may be unstable", UI_LEFT, colortex, 0.80f);
	if(ns_haveerror.integer == 3) ST_DrawString(38 - cgs.wideoffset, 24, "Network unstable, switching to chunk mode", UI_LEFT, colortex, 0.80f);
}

void CG_AddNotify(const char *text, int type, int number) {
	int i;
	int id = -1;

	for(i = MAX_NOTIFICATIONS - 1; i > 0; i--) cg.notifications[i] = cg.notifications[i - 1];

	Q_strncpyz(cg.notifications[0].text, text, 127);
	cg.notifications[0].text[127] = '\0';
	cg.notifications[0].type = type;
	cg.notifications[0].number = number;
	cg.notifications[0].startTime = cg.time;
	cg.notifications[0].active = qtrue;

	if(type == NOTIFY_INFO) trap_S_StartLocalSound(cgs.media.notifySound, CHAN_LOCAL_SOUND);
	if(type == NOTIFY_UNDO) trap_S_StartLocalSound(cgs.media.undoSound, CHAN_LOCAL_SOUND);
}

static void CG_Notify(void) {
	vec4_t backgroundColor = {0.0f, 0.0f, 0.0f, 0.30f};
	vec4_t textColor = {1.0f, 1.0f, 1.0f, 1.0f};
	vec4_t picColor = {1.0f, 1.0f, 1.0f, 1.0f};
	float margin = 4.0f, startX = 640 + cgs.wideoffset - 15, startY = 410 - margin, alpha, offsetX, yOffset, boxWidth;
	int i, timeElapsed, typeTime = 5000;

	for(i = 0; i < MAX_NOTIFICATIONS; i++) {
		if(!cg.notifications[i].active) continue;

		timeElapsed = cg.time - cg.notifications[i].startTime;
		if(timeElapsed > typeTime) {
			cg.notifications[i].active = qfalse;
			continue;
		}

		alpha = 1.0f;
		offsetX = 0.0f;

		if(timeElapsed < NOTIFICATION_FADE_TIME) {
			alpha = (float)timeElapsed / NOTIFICATION_FADE_TIME;
			offsetX = Lerp(300.0f, 0.0f, alpha);
		} else if(timeElapsed > typeTime - NOTIFICATION_FADE_TIME) {
			alpha = (float)(typeTime - timeElapsed) / NOTIFICATION_FADE_TIME;
		}

		backgroundColor[3] = alpha * 0.30f;
		textColor[3] = alpha;
		picColor[3] = alpha;

		boxWidth = (BASEFONT_INDENT * ST_StringCount(cg.notifications[i].text)) + 32;

		yOffset = startY - (20 + margin) * i;

		CG_DrawRoundedRect(startX - boxWidth + offsetX, yOffset, boxWidth, 20, 6, backgroundColor);

		trap_R_SetColor(picColor);
		if(cg.notifications[i].type == NOTIFY_INFO && cgs.media.notifyIcon) CG_DrawPic(startX - boxWidth + offsetX + 5, yOffset + 2.5, 16, 16, cgs.media.notifyIcon);
		if(cg.notifications[i].type == NOTIFY_UNDO && cgs.media.undoIcon) CG_DrawPic(startX - boxWidth + offsetX + 5, yOffset + 2.5, 16, 16, cgs.media.undoIcon);
		if(cg.notifications[i].type == NOTIFY_ITEM) CG_DrawPic(startX - boxWidth + offsetX + 5, yOffset + 2.5, 16, 16, cg_items[cg.notifications[i].number].icon);
		if(cg.notifications[i].type == NOTIFY_KILL) CG_DrawPic(startX - boxWidth + offsetX + 5, yOffset + 2.5, 16, 16, CG_FindModImage(cg.notifications[i].number));
		trap_R_SetColor(NULL);

		ST_DrawString(startX - boxWidth + offsetX + 24, yOffset + 5, cg.notifications[i].text, UI_LEFT, textColor, 1.00);
	}
}

static void CG_DrawDeathMessage(void) {
	if(((double)cg.respawnTime - (double)cg.time) / 1000.0 <= 0) {
		ST_DrawString(SCREEN_WIDTH / 2, 465, "Press fire key to respawn", UI_CENTER|UI_DROPSHADOW, color_white, 1.00);
	} else {
		ST_DrawString(SCREEN_WIDTH / 2, 465, va("Respawn: %6.2f", ((double)cg.respawnTime - (double)cg.time) / 1000.0), UI_CENTER|UI_DROPSHADOW, color_white, 1.00);
	}
}

static void CG_DrawPostProcess(void) {
	if(strlen(cg_postprocess.string)) {
		cgs.media.postProcess = trap_R_RegisterShaderNoMip(cg_postprocess.string);
	} else {
		cgs.media.postProcess = 0;
	}

	if(strlen(cg_postprocess.string) && cgs.media.postProcess) CG_DrawPic(0 - (cgs.wideoffset + 1), 0, SCREEN_WIDTH + (cgs.wideoffset * 2) + 2, SCREEN_HEIGHT, cgs.media.postProcess);
}

static void CG_Draw2D(void) {
	int catcher = trap_Key_GetCatcher();

	if(!cg_draw2D.integer) return;

	if(!(catcher & KEYCATCH_MESSAGE)) CG_DrawGenericConsole(&cgs.console, 5, 10000, 0 - cgs.wideoffset, 0, 1.00);
	CG_DrawGenericConsole(&cgs.teamChat, 5, 10000, 0 - cgs.wideoffset, 80, 1.00);
	CG_DrawGenericConsole(&cgs.chat, 5, 10000, 0 - cgs.wideoffset, 350, 1.00);

	CG_DrawCrosshair();
	CG_ScanForCrosshairEntity();
	CG_DrawCounters();
	CG_DrawPowerups();
	CG_DrawFollow();
	CG_Notify();

	if(!cg.scoreBoardShowing) CG_Draw1FCTF();

	CG_NSErrors();

	if(cg.snap->ps.pm_type == PM_INTERMISSION) {
		CG_DrawIntermission();
		return;
	}

	if(!cg.scoreBoardShowing) CG_DrawCenterString();

	if(cg.snap->ps.pm_type != PM_INTERMISSION && cg.snap->ps.pm_type != PM_DEAD && cg.snap->ps.pm_type != PM_SPECTATOR) CG_DrawStatusBar();

	cg.scoreBoardShowing = CG_DrawScoreboard();

	if(cgs.gametype != GT_SANDBOX) CG_DrawScores();

	if(cg.snap->ps.pm_type == PM_DEAD) CG_DrawDeathMessage();
}

static char cgameThreadBuffer[MAX_CYCLE_SIZE];
static void RunScriptThreads(int time) {
	int i;

	for(i = 0; i < threadsCount; i++) {
		ScriptLoop *script = &threadsLoops[i];
		if(time - script->lastRunTime >= script->interval) {
			script->lastRunTime = time;

			Q_strncpyz(cgameThreadBuffer, script->code, MAX_CYCLE_SIZE - 1);
			cgameThreadBuffer[MAX_CYCLE_SIZE - 1] = '\0';

			Interpret(cgameThreadBuffer);
		}
	}
}

void CG_DrawActive(void) {
	if(!cg.snap) {
		CG_DrawInformation();
		return;
	}

	if(!cg.weaponSelect) cg.weaponSelect = cg.snap->ps.weapon;

	RunScriptThreads(cg.time);
	CG_ReloadPlayers();
	trap_R_RenderScene(&cg.refdef);
	CG_Draw3DStringQueue();
	CG_DrawPostProcess();
	CG_Draw2D();

	if(cg.renderingThirdPerson) CG_DrawCrosshair3D();
}
