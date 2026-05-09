// Copyright (C) 2023-2026 Noire's Mod — GPLv2

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

void CL_Init(void) { 
    int i;

    trap_GetGlconfig(&glconfig);
    cgui.scale = (glconfig.width * (1.0 / 960.0) < glconfig.height * (1.0 / 720.0)) ? glconfig.width * (1.0 / 960.0) : glconfig.height * (1.0 / 720.0);
  	cgui.wideoffset = 0;
	
	cgui.whiteShader = trap_R_RegisterShaderNoMip("white");
	cgui.corner = trap_R_RegisterShaderNoMip("menu/corner");
	
	for(i = 0; i < MAX_KEYS; i++) trap_Key_GetBindingBuf(i, cgui.binds[i], 256);
}

void drawShader(float x, float y, float w, float h, const char *file) {
    qhandle_t shaderID = trap_R_RegisterShaderNoMip(file);
	if(shaderID) {
	    trap_R_DrawStretchPic(x, y, w, h, 0, 0, 1, 1, shaderID);
	} else {
	    if(w > h) trap_R_DrawStretchPic(x+((w-h)*0.5), y, h, h, 0, 0, 1, 1, trap_R_RegisterShaderNoMip("menu/loading"));
	    else if(w < h) trap_R_DrawStretchPic(x, y+((h-w)*0.5), w, w, 0, 0, 1, 1, trap_R_RegisterShaderNoMip("menu/loading"));
	    else trap_R_DrawStretchPic(x, y, w, h, 0, 0, 1, 1, trap_R_RegisterShaderNoMip("menu/loading"));
	}
}

void drawColoredShader(float x, float y, float w, float h, const char *file, float *color) {
    trap_R_SetColor(color);
    drawShader(x, y, w, h, file);
    trap_R_SetColor(NULL);
}

static void drawSymbol(float x, float y, float w, float h, float s1, float t1, float s2, float t2, qhandle_t fontIndex) {
    trap_R_DrawStretchPic(x, y, w, h, s1, t1, s2, t2, fontIndex);
}

static int getFontRes(float fontScale) {
	if(fontScale * BASEFONT_HEIGHT > 128) return 4; // 4096
	if(fontScale * BASEFONT_HEIGHT > 64) return 3;  // 2048
	if(fontScale * BASEFONT_HEIGHT > 32) return 2;  // 1024
	if(fontScale * BASEFONT_HEIGHT > 16) return 1;  // 512
	return 0;                                       // 256 default
}

static const int emojiPages[] = { 32, 33, 35, 36, 37, 38, 39, 41, 43, 48, 50, 496, 497, 498, 499, 500, 501, 502, 503, 505, 506, 4072, -1 };

static qboolean isEmojiPage(int pageID) {
    int i;
    for(i = 0; emojiPages[i] != -1; i++) {
        if(emojiPages[i] == pageID) return qtrue;
    }
    return qfalse;
}

static int getUTF8Font(int code, const char *style, float fontSize) {
    int sheetIndex = code / 256;
    if(isEmojiPage(sheetIndex)) style = "Regular";
    return trap_R_RegisterShaderNoMip(va("menu/fonts/%s_%s_%i_%i", "default", style, sheetIndex, getFontRes(fontSize)));
}

static int isHex(char c) {
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}

static int hexToInt(char c) {
    if(c >= '0' && c <= '9') return c - '0';
    if(c >= 'A' && c <= 'F') return 10 + (c - 'A');
    if(c >= 'a' && c <= 'f') return 10 + (c - 'a');
    return 0;
}

static float drawChars(int x, int y, const char *str, float *color, float fontScale, int style, qboolean returnWidth, int position) {
	const char *s;
	float ax, ay, aw, ah;
	float frow, fcol;
	int codepoint, bytesRead;
	int fontIndex = 0, currentIndex = 0;
	float glyphsPerRow = 4096.0 / 256.0;
	float glyphTexSize = 256.0 / 4096.0;
	const char *formatStyle = "Regular";
	const char *savedStyle = "Regular";
	qboolean formatUnderline = qfalse;
	qboolean formatStrikethrough = qfalse;
	qboolean formatMagic = qfalse;
	qboolean formatShake = qfalse;
	float formatColor[4] = {1.00, 1.00, 1.00, 1.00};
	float *currentColor = color;
	float debugColor[4] = {0.75, 0.75, 0.75, 0.50};
	qboolean drawCursor = qtrue;

	ax = x;
	ay = y;
	aw = BASEFONT_WIDTH * fontScale;
	ah = BASEFONT_HEIGHT * fontScale;
	
	if(style & UI_BOLD) savedStyle = "Bold";
	if(style & UI_ITALIC) savedStyle = "Italic";
	if(style & UI_BOLD && style & UI_ITALIC) savedStyle = "BoldItalic";
    if(style & UI_UNDERLINE) formatUnderline = qtrue;
    if(style & UI_STRIKETHROUGH) formatStrikethrough = qtrue;
    if(style & UI_MAGIC) formatMagic = qtrue;
    if(style & UI_SHAKE) formatShake = qtrue;
    
    formatStyle = savedStyle;

	s = str;
	while(*s) {
	    if(*s == '&' && *(s+1) != '\0') {
            char fmtCode = *(s+1);
            
           switch(fmtCode) {
                case 'l': formatStyle = "Bold"; break;
                case 'o': formatStyle = "Italic"; break;
                case 'p': formatStyle = "BoldItalic"; break;
                case 'n': formatUnderline = qtrue; break;
                case 'm': formatStrikethrough = qtrue; break;
                case 'k': formatMagic = qtrue; break;
                case 's': formatShake = qtrue; break;
                case 'r':
                    formatStyle = savedStyle;
                    formatUnderline = style & UI_UNDERLINE;
                    formatStrikethrough = style & UI_STRIKETHROUGH;
                    formatMagic = style & UI_MAGIC;
                    formatShake = style & UI_SHAKE;
                    currentColor = color;
                    break;
                    
                default: break;
            }
            
            s += 2;
            currentIndex += 2;
            continue;
        }
        
        if(style & UI_LOCKSTYLE) formatStyle = savedStyle;
        if(style & UI_LOCKEFFECTS) {
            formatUnderline = style & UI_UNDERLINE;
            formatStrikethrough = style & UI_STRIKETHROUGH;
            formatMagic = style & UI_MAGIC;
            formatShake = style & UI_SHAKE;
        }
        
        if(*s == '#' && isHex(*(s+1)) && isHex(*(s+2)) && isHex(*(s+3))) {
            int r = hexToInt(*(s+1));
            int g = hexToInt(*(s+2));
            int b = hexToInt(*(s+3));
            
            if(!(style & UI_LOCKCOLOR)) {
                formatColor[0] = (r * 16 + r) / 255.0f;
                formatColor[1] = (g * 16 + g) / 255.0f;
                formatColor[2] = (b * 16 + b) / 255.0f;
                formatColor[3] = 1.00f;
                
                currentColor = formatColor;
            }
            
            s += 4;
            currentIndex += 4;
            continue;
        }
        
        if(style & UI_ACCENT) currentColor = cgui.colors[JSC_ENABLED];
        
		if((*s & 0x80) == 0) {
			codepoint = *s;
			bytesRead = 1;
		} else if((*s & 0xE0) == 0xC0) {
			if(*(s+1) != '\0') {
				codepoint = ((*s & 0x1F) << 6) | (*(s+1) & 0x3F);
				bytesRead = 2;
			} else {
				codepoint = '?';
				bytesRead = 1;
			}
		} else if((*s & 0xF0) == 0xE0) {
			if(*(s+1) != '\0' && *(s+2) != '\0') {
				codepoint = ((*s & 0x0F) << 12) | ((*(s+1) & 0x3F) << 6) | (*(s+2) & 0x3F);
				bytesRead = 3;
			} else {
				codepoint = '?';
				bytesRead = 1;
			}
		} else if((*s & 0xF8) == 0xF0) {
			if(*(s+1) != '\0' && *(s+2) != '\0' && *(s+3) != '\0') {
				codepoint = ((*s & 0x07) << 18) | ((*(s+1) & 0x3F) << 12) | ((*(s+2) & 0x3F) << 6) | (*(s+3) & 0x3F);
				bytesRead = 4;
			} else {
				codepoint = '?';
				bytesRead = 1;
			}
		} else {
			codepoint = '?';
			bytesRead = 1;
		}
		
		if(codepoint >= 0xD800 && codepoint <= 0xDBFF) {
			const char *next_s = s + bytesRead;
			int next_cp = 0;
			int next_bytes = 1;
			
			if(*next_s) {
				if((*next_s & 0x80) == 0) { next_cp = *next_s; }
				else if((*next_s & 0xE0) == 0xC0) { next_cp = ((*next_s & 0x1F) << 6) | (*(next_s+1) & 0x3F); next_bytes = 2; }
				else if((*next_s & 0xF0) == 0xE0) { next_cp = ((*next_s & 0x0F) << 12) | ((*(next_s+1) & 0x3F) << 6) | (*(next_s+2) & 0x3F); next_bytes = 3; }
				else if((*next_s & 0xF8) == 0xF0) { next_cp = ((*next_s & 0x07) << 18) | ((*(next_s+1) & 0x3F) << 12) | ((*(next_s+2) & 0x3F) << 6) | (*(next_s+3) & 0x3F); next_bytes = 4; }
			}

			if(next_cp >= 0xDC00 && next_cp <= 0xDFFF) {
				codepoint = 0x10000 + ((codepoint - 0xD800) << 10) + (next_cp - 0xDC00);
				bytesRead += next_bytes;
			}
		}
		
	    if(formatMagic) codepoint = 912 + (rand() % (976 - 912 + 1));
		if(!returnWidth) fontIndex = getUTF8Font(codepoint, formatStyle, fontScale);
		
		if(fontIndex > 0) {
			int glyphIndex = codepoint % 256;
			int row = glyphIndex / (int)glyphsPerRow;
			int col = glyphIndex % (int)glyphsPerRow;
			float xoff = 0.00;
			float yoff = 0.00;
			
			if(formatShake) {
                xoff = 2 * fontScale + (rand() % 5) - 2;
                yoff = 2 * fontScale + (rand() % 5) - 2;
            } else {
                xoff = 0.00;
                yoff = 0.00;
            }
			
			fcol = col * glyphTexSize;
			frow = row * glyphTexSize;
			
			if(codepoint <= 1280) {
			    trap_R_SetColor(currentColor);
			    drawSymbol(ax+xoff, ay+yoff, aw*FONT_WIDTH, ah, fcol+0.015625, frow, (fcol + glyphTexSize)-0.015625, frow + glyphTexSize, fontIndex);
			    if(formatUnderline) drawRoundedRect(ax, y+(ah*0.925), aw*FONT_WIDTH, ah*0.10, 0.00, currentColor, 0);
                if(formatStrikethrough) drawRoundedRect(ax, y+(ah*0.50), aw*FONT_WIDTH, ah*0.10, 0.00, currentColor, 0);
                if(position != -1 && currentIndex >= position && drawCursor) { drawRoundedRect(ax, y, aw*0.05, ah, 0.00, currentColor, 9); drawCursor = qfalse; }
			} else if(isEmojiPage(codepoint / 256) && !(style & UI_LOCKEMOJI)) {
			    trap_R_SetColor(formatColor);
			    drawSymbol(ax+xoff, ay+yoff, ah, ah, fcol, frow, fcol + glyphTexSize, frow + glyphTexSize, fontIndex);
			    if(formatUnderline) drawRoundedRect(ax, y+(ah*0.925), aw, ah*0.10, 0.00, formatColor, 0);
                if(formatStrikethrough) drawRoundedRect(ax, y+(ah*0.50), aw, ah*0.10, 0.00, formatColor, 0);
                if(position != -1 && currentIndex >= position && drawCursor) { drawRoundedRect(ax, y, aw*0.05, ah, 0.00, formatColor, 0); drawCursor = qfalse; }
			}
		}

		if(codepoint <= 1280) ax += aw * FONT_WIDTH;
		if(isEmojiPage(codepoint / 256) && !(style & UI_LOCKEMOJI)) ax += ah;
		
		s += bytesRead;
		currentIndex += bytesRead;
	}
	
	trap_R_SetColor(NULL);
	
	//if(!returnWidth) drawRoundedRect(x, y, stringWidth(str, fontScale, style), BASEFONT_HEIGHT * fontScale, 0.00, debugColor, 0);
	
	return ax - x;
}

float stringWidth(const char *str, float fontScale, int style) {
    return drawChars(0, 0, str, cgui.colors[0], fontScale, style, qtrue, -1);
}

void drawString(float x, float y, const char *str, int style, float *color, float fontSize) {
    if(!str || !strlen(str)) return;
    
    if(style & UI_RIGHT) x -= stringWidth(str, fontSize, style) * 1.00;
	else if(style & UI_CENTER) x -= stringWidth(str, fontSize, style) * 0.50;

	drawChars(x, y, str, color, fontSize, style, qfalse, -1);
}

void drawStringField(float x, float y, const char *str, int style, float *color, float fontSize, int position) {
    if(!str || !strlen(str)) return;

	if(style & UI_CENTER) x -= stringWidth(str, fontSize, style) * 0.50;
	if(style & UI_RIGHT) x -= stringWidth(str, fontSize, style) * 1.00;

	drawChars(x, y, str, color, fontSize, style, qfalse, position);
}

void drawRoundedRect(float x, float y, float width, float height, float radius, float *color, int style) {
    /* Объявления всех локальных переменных */
    float cx, cy, cw, ch;
    float top_x, top_y, top_w, top_h;
    float bot_x, bot_y, bot_w, bot_h;
    float left_x, left_y, left_w, left_h;
    float right_x, right_y, right_w, right_h;
    int tl, tr, bl, br;

    /* Ранний выход, если полностью прозрачно */
    if (color[3] < 0.01f) return;

    /* Ограничение радиуса */
    if (radius * 2.0f > height) radius = height * 0.5f;
    if (radius * 2.0f > width)  radius = width * 0.5f;

    trap_R_SetColor(color);

    /* Если радиус нулевой – одна заливка */
    if (radius <= 0.0f) {
        trap_R_DrawStretchPic(x, y, width, height, 0, 0, 0, 0, cgui.whiteShader);
        trap_R_SetColor(NULL);
        return;
    }

    /* Какие углы скруглять (флаг НЕ установлен) */
    tl = !(style & NO_TOP_LEFT);
    tr = !(style & NO_TOP_RIGHT);
    bl = !(style & NO_BOTTOM_LEFT);
    br = !(style & NO_BOTTOM_RIGHT);

    /* ----- Центральный прямоугольник ----- */
    cx = x + radius;
    cy = y + radius;
    cw = width - 2.0f * radius;
    ch = height - 2.0f * radius;

    /* ----- Верхняя сторона ----- */
    top_x = x + radius;
    top_y = y;
    top_w = width - (radius*2);
    top_h = radius;

    /* ----- Нижняя сторона ----- */
    bot_x = x + radius;
    bot_y = y + height - radius;
    bot_w = width - (radius*2);
    bot_h = radius;

    /* ----- Левая сторона ----- */
    left_x = x;
    left_y = y + (tl ? radius : 0.0f);
    left_w = radius;
    left_h = height - (tl ? radius : 0.0f) - (bl ? radius : 0.0f);

    /* ----- Правая сторона ----- */
    right_x = x + width - radius;
    right_y = y + (tr ? radius : 0.0f);
    right_w = radius;
    right_h = height - (tr ? radius : 0.0f) - (br ? radius : 0.0f);

    /* ----- Отрисовка заливки (центр + стороны) ----- */
    if (cw > 0.0f && ch > 0.0f) trap_R_DrawStretchPic(cx, cy, cw, ch, 0, 0, 0, 0, cgui.whiteShader);
    if (top_w > 0.0f && top_h > 0.0f) trap_R_DrawStretchPic(top_x, top_y, top_w, top_h, 0, 0, 0, 0, cgui.whiteShader);
    if (bot_w > 0.0f && bot_h > 0.0f) trap_R_DrawStretchPic(bot_x, bot_y, bot_w, bot_h, 0, 0, 0, 0, cgui.whiteShader);
    if (left_w > 0.0f && left_h > 0.0f) trap_R_DrawStretchPic(left_x, left_y, left_w, left_h, 0, 0, 0, 0, cgui.whiteShader);
    if (right_w > 0.0f && right_h > 0.0f) trap_R_DrawStretchPic(right_x, right_y, right_w, right_h, 0, 0, 0, 0, cgui.whiteShader);

    /* ----- Скруглённые углы (поверх заливки) ----- */
    if (tl) trap_R_DrawStretchPic(x, y, radius, radius, 1, 0, 0, 1, cgui.corner);
    if (tr) trap_R_DrawStretchPic(x + width - radius, y, radius, radius, 0, 0, 1, 1, cgui.corner);
    if (bl) trap_R_DrawStretchPic(x, y + height - radius, radius, radius, 1, 1, 0, 0, cgui.corner);
    if (br) trap_R_DrawStretchPic(x + width - radius, y + height - radius, radius, radius, 0, 1, 1, 0, cgui.corner);

    trap_R_SetColor(NULL);
}

void drawOutline(float x, float y, float width, float height, float thickness, float *color) {
    trap_R_SetColor(color);
    
    trap_R_DrawStretchPic(x, y, width, thickness, 0, 0, 0, 0, cgui.whiteShader);
    trap_R_DrawStretchPic(x, y + height - thickness, width, thickness, 0, 0, 0, 0, cgui.whiteShader);
    trap_R_DrawStretchPic(x, y, thickness, height, 0, 0, 0, 0, cgui.whiteShader);
    trap_R_DrawStretchPic(x + width - thickness, y, thickness, height, 0, 0, 0, 0, cgui.whiteShader);
    
    trap_R_SetColor(NULL);
}
#endif

/*
====================
Console Variables
====================
*/

cvar_t cvarStorage[MAX_CVARS];

void VM_RegisterCvars(void) {
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
