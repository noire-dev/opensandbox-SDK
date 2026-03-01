// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2023-2025 blqck.Cøre
// Void Sandbox — GPLv2; see LICENSE for details.

#include "javascript.h"

/*
====================
Font, Text, Colors, CGUI
====================
*/

#ifndef GAME
vec4_t color_black = {0.00f, 0.00f, 0.00f, 1.00f};
vec4_t color_white = {1.00f, 1.00f, 1.00f, 1.00f};
vec4_t color_grey = {0.30f, 0.30f, 0.30f, 1.00f};
vec4_t color_dim = {0.00f, 0.00f, 0.00f, 0.40f};
vec4_t color_disabled = {0.10f, 0.10f, 0.20f, 1.00f};
vec4_t color_select = {0.35f, 0.45f, 0.95f, 0.40f};
vec4_t color_highlight = {0.50f, 0.50f, 0.50f, 1.00f};

vec4_t customcolor_crosshair = {1.00f, 1.00f, 1.00f, 1.00f};

cgui_t cgui;
glconfig_t glconfig;

int ST_ColorEscapes(const char *str) {
	int count = 0;
	while(str && *str) {
		if(*str == Q_COLOR_ESCAPE && *(str + 1) && *(str + 1) >= '0' && *(str + 1) <= '9') {
			count++;
			str += 2;
		} else {
			str++;
		}
	}
	return count;
}

static int ST_GetFontRes(float fontSize) {
	float fontScale;

	fontScale = glconfig.vidHeight / 480.0;

	if(fontSize * fontScale > 128) return 4; // 4096
	if(fontSize * fontScale > 64) return 3;  // 2048
	if(fontSize * fontScale > 32) return 2;  // 1024
	if(fontSize * fontScale > 16) return 1;  // 512
	return 0;                                // 256 default
}

void initCGUI(const char *font) {
    int i;

    trap_GetGlconfig(&glconfig);
    cgui.scale = (glconfig.vidWidth * (1.0 / 640.0) < glconfig.vidHeight * (1.0 / 480.0)) ? glconfig.vidWidth * (1.0 / 640.0) : glconfig.vidHeight * (1.0 / 480.0);
    cgui.wideoffset = 0;
    cgui.bias = 0;
    
    if(glconfig.vidWidth * 480 > glconfig.vidHeight * 640) // wide screen
		cgui.bias = 0.5 * (glconfig.vidWidth - (glconfig.vidHeight * (640.0 / 480.0)));
	
	if(((float)glconfig.vidWidth / ((float)glconfig.vidHeight / 480) - 640) / 2 >= 0) 
		cgui.wideoffset = ((float)glconfig.vidWidth / ((float)glconfig.vidHeight / 480) - 640) / 2;
	
	for(i = 0; i <= 4; i++) cgui.defaultFont[i] = trap_R_RegisterShaderNoMip(va("%s_font%i", font, i));
	cgui.whiteShader = trap_R_RegisterShaderNoMip("white");
	cgui.corner = trap_R_RegisterShaderNoMip("menu/corner");
	
	for(i = 0; i < 256; i++) trap_Key_GetBindingBuf(i, cgui.binds[i], 256);
}

void updateCGUI(void) {
	customcolor_crosshair[0] = cvarFloat("cg_crosshairColorRed");
	customcolor_crosshair[1] = cvarFloat("cg_crosshairColorGreen");
	customcolor_crosshair[2] = cvarFloat("cg_crosshairColorBlue");
}

int stringCount(const char *str) {
	const char *s;
	char ch;
	int prev_unicode = 0;
	int i;

	s = str;
	i = 0;
	while(*s) {
		if(Q_IsColorString(s)) {
			s += 2;
			continue;
		}

		ch = *s & 255;

		// Unicode Russian support
		if(ch < 0) {
			if((ch == -48) || (ch == -47)) {
				prev_unicode = ch;
				s++;
				continue;
			}
			if(ch >= -112) {
				if((ch == -111) && (prev_unicode == -47)) {
					ch = ch - 13;
				} else {
					ch = ch + 48;
				}
			} else {
				if((ch == -127) && (prev_unicode == -48)) {
					// ch = ch +
				} else {
					ch = ch + 112; // +64 offset of damn unicode
				}
			}
		}
		i++;
		s++;
	}

	return i;
}

static void drawChars(int x, int y, const char *str, vec4_t color, int charw, int charh, int style, qboolean drawShadow) {
	const char *s;
	char ch;
	int prev_unicode = 0;
	vec4_t tempcolor;
	float ax;
	float ay;
	float aw;
	float ah;
	float frow;
	float fcol;
	float alignstate = 0;
	int fontRes = ST_GetFontRes(charh);

	if(style & UI_CENTER) {
		alignstate = 0.5;
	}
	if(style & UI_RIGHT) {
		alignstate = 1;
	}

	// Set color for the text
	trap_R_SetColor(color);

	ax = x;
	ay = y;
	aw = charw;
	ah = charh;
	
	adjustFrom640(&ax, &ay, &aw, &ah);

	s = str;
	while(*s) {
		if((*s == -48) || (*s == -47)) {
			ax = ax + aw * alignstate;
		}
		s++;
	}

	s = str;
	while(*s) {
		if(Q_IsColorString(s)) {
			memcpy(tempcolor, g_color_table[ColorIndex(s[1])], sizeof(tempcolor));
			tempcolor[3] = color[3];
			if(!drawShadow) trap_R_SetColor(tempcolor);
			s += 2;
			continue;
		}

		if(*s != ' ') {
			ch = *s & 255;

			// Unicode Russian support
			if(ch < 0) {
				if((ch == -48) || (ch == -47)) {
					prev_unicode = ch;
					s++;
					continue;
				}
				if(ch >= -112) {
					if((ch == -111) && (prev_unicode == -47)) {
						ch = ch - 13;
					} else {
						ch = ch + 48;
					}
				} else {
					if((ch == -127) && (prev_unicode == -48)) {
						// ch = ch +
					} else {
						ch = ch + 112; // +64 offset of damn unicode
					}
				}
			}

			frow = (ch >> 4) * 0.0625;
			fcol = (ch & 15) * 0.0625;
			trap_R_DrawStretchPic(ax, ay, aw, ah, fcol, frow, fcol + 0.0625, frow + 0.0625, cgui.defaultFont[fontRes]);
		}

		ax += aw * FONT_WIDTH;
		s++;
	}

	trap_R_SetColor(NULL);
}

void drawChar(float x, float y, int ch, int style, float *color, float size) {
	char buff[2];

	buff[0] = ch;
	buff[1] = '\0';

	drawString(x, y, buff, style, color, size);
}

float stringWidth(const char *str, float size) {
	const char *s;
	float width;

	s = str;
	width = 0;
	while(*s) {
		if(Q_IsColorString(s)) {
			s += 2;
			continue;
		}
		width += BASEFONT_INDENT * size;
		s++;
	}

	if(ifstrlenru(str)) return (width * 0.5) + BASEFONT_HEIGHT*(1.00-FONT_WIDTH);
	else return width + BASEFONT_HEIGHT*(1.00-FONT_WIDTH);
}

void drawString(float x, float y, const char *str, int style, float *color, float fontSize) {
	float charw;
	float charh;
	float *drawcolor;
	vec4_t dropcolor;
	int len = stringCount(str);
	int esc = ST_ColorEscapes(str);

	if(!str) return;

	charw = BASEFONT_WIDTH * fontSize;
	charh = BASEFONT_HEIGHT * fontSize;

	if(style & UI_PULSE) {
		drawcolor = color_highlight;
	} else {
		drawcolor = color;
	}

	switch(style & UI_FORMATMASK) {
	case UI_CENTER:
		x = x - len * (charw * FONT_WIDTH) / 2;
		x += esc * (charw * FONT_WIDTH);
		break;

	case UI_RIGHT: x = x - len * (charw * FONT_WIDTH); break;

	default:
		// nothing to do
		break;
	}

	if(style & UI_DROPSHADOW) {
		dropcolor[0] = dropcolor[1] = dropcolor[2] = 0;
		dropcolor[3] = drawcolor[3];
		drawChars(x + 1, y + 1, str, dropcolor, charw, charh, style, qtrue);
	}

	drawChars(x, y, str, drawcolor, charw, charh, style, qfalse);
}

void adjustFrom640(float *x, float *y, float *w, float *h) {
	*x = *x * cgui.scale + cgui.bias;
	*y *= cgui.scale;
	*w *= cgui.scale;
	*h *= cgui.scale;
}

void drawRoundedRect(float x, float y, float width, float height, float radius, float *color) {
	adjustFrom640(&x, &y, &width, &height);

	if(radius * 2 > height) radius = height * 0.5;
	if(radius * 2 > width) radius = width * 0.5;

	radius *= cgui.scale;

	trap_R_SetColor(color);
	trap_R_DrawStretchPic(x, y, radius, radius, 1, 0, 0, 1, cgui.corner);                                    // Левый верхний угол
	trap_R_DrawStretchPic(x + width - radius, y, radius, radius, 0, 0, 1, 1, cgui.corner);                   // Правый верхний угол
	trap_R_DrawStretchPic(x, y + height - radius, radius, radius, 1, 1, 0, 0, cgui.corner);                  // Левый нижний угол
	trap_R_DrawStretchPic(x + width - radius, y + height - radius, radius, radius, 0, 1, 1, 0, cgui.corner); // Правый нижний угол

	trap_R_DrawStretchPic(x, y + radius, radius, height - (radius * 2), 0, 0, 0, 0, cgui.whiteShader);                  // Левая сторона
	trap_R_DrawStretchPic(x + width - radius, y + radius, radius, height - (radius * 2), 0, 0, 0, 0, cgui.whiteShader); // Правая сторона
	trap_R_DrawStretchPic(x + radius, y, width - (radius * 2), height, 0, 0, 0, 0, cgui.whiteShader);                   // Верхняя сторона
	trap_R_SetColor(NULL);
}

void drawShader(float x, float y, float w, float h, const char *file) {
	adjustFrom640(&x, &y, &w, &h);
	trap_R_DrawStretchPic(x, y, w, h, 0, 0, 1, 1, trap_R_RegisterShaderNoMip(file));
}

static float SliderProgress(float value, float min, float max) {
    float result;
    float clamp;
    
    if(value < min) clamp = min;
    else if(value > max) clamp = max;
    else clamp = value;
    
    result = (clamp - min) / (max - min);
    
    if(result <= 0.0f) return 0.0f;
    if(result >= 1.0f) return 1.0f;
    
    return result;
}

void drawButton(float x, float y, float w, float h, int corner, int tcolor, int colorbg, float size, int margin, int style, const char *text) {
    float text_y = y+(h-(BASEFONT_HEIGHT*size))*0.5;
    
    drawRoundedRect(x, y, w, h, corner, cgui.colors[colorbg]);
    if(style & UI_CENTER) drawString(x+(w*0.5), text_y, text, UI_CENTER, cgui.colors[tcolor], size);
    else if(style & UI_RIGHT) drawString((x+w)-margin, text_y, text, UI_RIGHT, cgui.colors[tcolor], size);
    else drawString(x+margin, text_y, text, UI_LEFT, cgui.colors[tcolor], size);
}

void drawCheckbox(float x, float y, float w, float h, int corner, int tcolor, int scolor, int colorbg, float size, int margin, int style, const char *text) {
    float text_y = y+(h-(BASEFONT_HEIGHT*size))*0.5;

    drawRoundedRect(x, y, w, h, corner, cgui.colors[colorbg]);
    drawRoundedRect((x+w)-(h*0.90), (y+(h*0.5))-h*0.35, h*0.70, h*0.70, corner, cgui.colors[scolor]);
    drawString(x+margin, text_y, text, UI_LEFT, cgui.colors[tcolor], size);
}

void drawSlider(float x, float y, float w, float h, int corner, int tcolor, int colorbg, float size, int margin, int style, const char *text, float value, float min, float max) {
    float text_y = y+(h-(BASEFONT_HEIGHT*size))*0.5;
    
    drawRoundedRect(x, y, w, h, corner, cgui.colors[colorbg]);
    drawRoundedRect(x+corner, (y+h)-h*0.20, w-(corner*2), h*0.10, 0, cgui.colors[JSCOLOR_DISABLED]);
    drawRoundedRect(x+corner, (y+h)-h*0.20, (w-(corner*2))*SliderProgress(value, min, max), h*0.10, 0, cgui.colors[JSCOLOR_ENABLED]);
    drawString(x+margin, text_y, text, UI_LEFT, cgui.colors[tcolor], size);
}

void drawAction(float x, float y, float w, float h, int corner, int tcolor, int colorbg, float size, int margin, int style, const char *text, const char *bindkey) {
    float text_y = y+(h-(BASEFONT_HEIGHT*size))*0.5;

    drawRoundedRect(x, y, w, h, corner, cgui.colors[colorbg]);
    drawRoundedRect((x+w)-(h*0.90), (y+(h*0.5))-h*0.35, h*0.70, h*0.70, corner, cgui.colors[scolor]);
    drawString(x+margin, text_y, text, UI_LEFT, cgui.colors[tcolor], size);
    drawString((x+w)-(h*0.90)-2, text_y, bindkey, UI_LEFT, cgui.colors[tcolor], size);
}
#endif

/*
====================
Console Variables
====================
*/

cvar_t cvarStorage[MAX_CVARS];

void registerCvars(void) {
	cvarReload();
	updateCvars();
}

void updateCvars(void) {
	int i;

	for(i = 0; i < MAX_CVARS; i++) {
		cvarUpdate(&cvarStorage[i], i);
	}
}

int cvarInt(const char *name) {
	int id = cvarID(name);
	if(id == -1) return 0;
	cvarUpdate(&cvarStorage[id], id);
	return cvarStorage[id].integer;
}

float cvarFloat(const char *name) {
	int id = cvarID(name);
	if(id == -1) return 0.0f;
	cvarUpdate(&cvarStorage[id], id);
	return cvarStorage[id].value;
}

char *cvarString(const char *name) {
	int id = cvarID(name);
	if(id == -1) return "0";
	cvarUpdate(&cvarStorage[id], id);
	return cvarStorage[id].string;
}
