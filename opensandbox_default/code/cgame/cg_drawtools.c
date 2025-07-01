// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// OpenSandbox — GPLv2; see LICENSE for details.

#include "cg_local.h"

void CG_AdjustFrom640(float *x, float *y, float *w, float *h) {
	*x = *x * cgs.scale + cgs.bias;
	*y *= cgs.scale;
	*w *= cgs.scale;
	*h *= cgs.scale;
}

void CG_DrawRoundedRect(float x, float y, float width, float height, float radius, const float *color) {
	CG_AdjustFrom640(&x, &y, &width, &height);

	if(radius * 2 > height) radius = height * 0.5;
	if(radius * 2 > width) radius = width * 0.5;

	radius *= cgs.scale;

	trap_R_SetColor(color);
	trap_R_DrawStretchPic(x, y, radius, radius, 1, 0, 0, 1, cgs.media.corner);
	trap_R_DrawStretchPic(x + width - radius, y, radius, radius, 0, 0, 1, 1, cgs.media.corner);
	trap_R_DrawStretchPic(x, y + height - radius, radius, radius, 1, 1, 0, 0, cgs.media.corner);
	trap_R_DrawStretchPic(x + width - radius, y + height - radius, radius, radius, 0, 1, 1, 0, cgs.media.corner);

	trap_R_DrawStretchPic(x, y + radius, radius, height - (radius * 2), 0, 0, 0, 0, cgs.media.whiteShader);
	trap_R_DrawStretchPic(x + width - radius, y + radius, radius, height - (radius * 2), 0, 0, 0, 0, cgs.media.whiteShader);
	trap_R_DrawStretchPic(x + radius, y, width - (radius * 2), height, 0, 0, 0, 0, cgs.media.whiteShader);
	trap_R_SetColor(NULL);
}

void CG_DrawProgressBar(float x, float y, float width, float height, float progress, float segmentWidth, const float *barColor, const float *bgColor) {
	int numSegments;
	int filledSegments;
	float segmentX;
	int i;
	int xy_offset = 2 * cgs.scale;
	int w_offset = 3 * cgs.scale;
	int h_offset = 3.75 * cgs.scale;

	if(progress < 0.0f) progress = 0.0f;
	if(progress > 1.0f) progress = 1.0f;

	CG_AdjustFrom640(&x, &y, &width, &height);

	segmentWidth *= cgs.scale;

	trap_R_SetColor(bgColor);
	trap_R_DrawStretchPic(x, y, width, height, 0, 0, 1, 1, cgs.media.whiteShader);
	trap_R_SetColor(NULL);

	numSegments = (int)(width / segmentWidth);
	filledSegments = (int)(numSegments * progress);

	trap_R_SetColor(barColor);
	for(i = 0; i < filledSegments; i++) {
		segmentX = x + i * segmentWidth;
		trap_R_DrawStretchPic(segmentX + xy_offset, y + xy_offset, segmentWidth - w_offset, height - h_offset, 0, 0, 1, 1, cgs.media.whiteShader);
	}
	trap_R_SetColor(NULL);
}

void CG_DrawPic(float x, float y, float width, float height, qhandle_t hShader) {
	CG_AdjustFrom640(&x, &y, &width, &height);
	trap_R_DrawStretchPic(x, y, width, height, 0, 0, 1, 1, hShader);
}

void CG_Draw3DString(float x, float y, float z, const char *str, int style, vec4_t color, float fontSize, float min, float max, qboolean useTrace) {
	vec3_t dir;
	float localX, localY, localZ;
	float tanFovX, tanFovY;
	float finalx, finaly;
	float dist;
	vec3_t worldPos;
	vec4_t finalColor;
	trace_t trace;

	worldPos[0] = x;
	worldPos[1] = y;
	worldPos[2] = z;

	VectorSubtract(worldPos, cg.refdef.vieworg, dir);

	if(useTrace) {
		CG_Trace(&trace, cg.refdef.vieworg, vec3_origin, vec3_origin, worldPos, cg.snap->ps.clientNum, CONTENTS_SOLID);
		if(trace.fraction < 1.0f) return;
	}

	localX = -DotProduct(dir, cg.refdef.viewaxis[1]);
	localY = DotProduct(dir, cg.refdef.viewaxis[2]);
	localZ = DotProduct(dir, cg.refdef.viewaxis[0]);

	if(localZ <= 0) return;

	tanFovX = tan(DEG2RAD(cg.refdef.fov_x * 0.5f));
	tanFovY = tan(DEG2RAD(cg.refdef.fov_y * 0.5f));

	finalx = (localX / (localZ * tanFovX)) * (320 + cgs.wideoffset) + 320;
	finaly = (-localY / (localZ * tanFovY)) * 240 + 240;

	dist = VectorLength(dir);

	Vector4Copy(color, finalColor);

	if(dist > min) {
		finalColor[3] = color[3] * (1.0f - (dist - min) / (max - min));
		if(finalColor[3] < 0.0f) finalColor[3] = 0.0f;
	}

	ST_DrawString(finalx, finaly, str, style | UI_CENTER, finalColor, fontSize);
}

float *CG_FadeColor(int startMsec, int totalMsec) {
	static vec4_t color;
	int t;

	if(startMsec == 0) return NULL;

	t = cg.time - startMsec;

	if(t >= totalMsec) return NULL;

	// fade out
	if(totalMsec - t < FADE_TIME) {
		color[3] = (totalMsec - t) * 1.0 / FADE_TIME;
	} else {
		color[3] = 1.0;
	}
	color[0] = color[1] = color[2] = 1;

	return color;
}
