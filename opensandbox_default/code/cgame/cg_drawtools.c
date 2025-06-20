// 
// OpenSandbox
// 
// Copyright (C) 1999-2005 ID Software, Inc.
// Copyright (C) 2008-2012 OpenArena Team
// Copyright (C) 2023-2024 Noire.dev
// Copyright (C) 2025 OpenSandbox Team
// 
// This file is part of OpenSandbox.
// 
// OpenSandbox is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License, version 2,
// as published by the Free Software Foundation.
// 
// This modified code is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this project. If not, see <http://www.gnu.org/licenses/>.
// 
// Contact: opensandboxteam@gmail.com
// 
//
// cg_drawtools.c -- helper functions called by cg_draw, cg_scoreboard, cg_info, etc
#include "cg_local.h"

/*
================
CG_AdjustFrom640

Adjusted for resolution and screen aspect ratio
================
*/
void CG_AdjustFrom640( float *x, float *y, float *w, float *h ) {
	*x = *x * cgs.scale + cgs.bias;
	*y *= cgs.scale;
	*w *= cgs.scale;
	*h *= cgs.scale;
}

/*
================
CG_FillRect

Coordinates are 640*480 virtual values
=================
*/
void CG_FillRect( float x, float y, float width, float height, const float *color ) {
	trap_R_SetColor( color );

	CG_AdjustFrom640( &x, &y, &width, &height );
	trap_R_DrawStretchPic( x, y, width, height, 0, 0, 0, 0, cgs.media.whiteShader );

	trap_R_SetColor( NULL );
}

/*
================
CG_DrawRoundedRect

Coordinates are 640*480 virtual values
=================
*/
void CG_DrawRoundedRect(float x, float y, float width, float height, float radius, const float *color) {
    
	CG_AdjustFrom640( &x, &y, &width, &height );
	
	if(radius*2 > height){ radius=height*0.5; }
	if(radius*2 > width){ radius=width*0.5; }
	
	radius *= cgs.scale;
	
	trap_R_SetColor( color );
    trap_R_DrawStretchPic(x, y, radius, radius, 1, 0, 0, 1, cgs.media.corner);
    trap_R_DrawStretchPic(x + width - radius, y, radius, radius, 0, 0, 1, 1, cgs.media.corner);
    trap_R_DrawStretchPic(x, y + height - radius, radius, radius, 1, 1, 0, 0, cgs.media.corner);
    trap_R_DrawStretchPic(x + width - radius, y + height - radius, radius, radius, 0, 1, 1, 0, cgs.media.corner);

	trap_R_DrawStretchPic( x, y + radius, radius, height - (radius * 2), 0, 0, 0, 0, cgs.media.whiteShader );
	trap_R_DrawStretchPic( x + width - radius, y + radius, radius, height - (radius * 2), 0, 0, 0, 0, cgs.media.whiteShader );
	trap_R_DrawStretchPic( x + radius, y, width - (radius * 2), height, 0, 0, 0, 0, cgs.media.whiteShader );
	trap_R_SetColor( NULL );
}

void CG_DrawProgressBar(float x, float y, float width, float height, float progress, float segmentWidth, const float *barColor, const float *bgColor) {
    int numSegments;
    int filledSegments;
	float segmentX;
	int i;
	int xy_offset = 2 * cgs.scale;
	int w_offset = 3 * cgs.scale;
	int h_offset = 3.75 * cgs.scale;

	if (progress < 0.0f) progress = 0.0f;
    if (progress > 1.0f) progress = 1.0f;

    CG_AdjustFrom640(&x, &y, &width, &height);

	segmentWidth *= cgs.scale;

    trap_R_SetColor(bgColor);
    trap_R_DrawStretchPic(x, y, width, height, 0, 0, 1, 1, cgs.media.whiteShader);
    trap_R_SetColor(NULL);
    
    numSegments = (int)(width / segmentWidth);
    filledSegments = (int)(numSegments * progress);
    
    trap_R_SetColor(barColor);
    for (i = 0; i < filledSegments; i++) {
        segmentX = x + i * segmentWidth;
        trap_R_DrawStretchPic(segmentX+xy_offset, y+xy_offset, segmentWidth - w_offset, height-h_offset, 0, 0, 1, 1, cgs.media.whiteShader);
    }
    trap_R_SetColor(NULL);
}

/*
================
CG_DrawPic

Coordinates are 640*480 virtual values
=================
*/
void CG_DrawPic( float x, float y, float width, float height, qhandle_t hShader ) {
	CG_AdjustFrom640( &x, &y, &width, &height );
	trap_R_DrawStretchPic( x, y, width, height, 0, 0, 1, 1, hShader );
}

/*
================
CG_Draw3DString

Render 3D string from ST_DrawString
=================
*/
void CG_Draw3DString(float x, float y, float z, const char* str, int style, vec4_t color, float fontSize, float min, float max, qboolean useTrace) {
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

    if (useTrace) {
        CG_Trace(&trace, cg.refdef.vieworg, vec3_origin, vec3_origin, worldPos, cg.snap->ps.clientNum, CONTENTS_SOLID);
        if (trace.fraction < 1.0f)
            return;
    }

    localX = -DotProduct(dir, cg.refdef.viewaxis[1]);
    localY = DotProduct(dir, cg.refdef.viewaxis[2]);
    localZ = DotProduct(dir, cg.refdef.viewaxis[0]);

    if (localZ <= 0)
        return;

    tanFovX = tan(DEG2RAD(cg.refdef.fov_x * 0.5f));
    tanFovY = tan(DEG2RAD(cg.refdef.fov_y * 0.5f));

	finalx = (localX / (localZ * tanFovX)) * (320+cgs.wideoffset) + 320;
	finaly = (-localY / (localZ * tanFovY)) * 240 + 240;

    dist = VectorLength(dir);

	Vector4Copy(color, finalColor);

    if (dist > min) {
        finalColor[3] = color[3] * (1.0f - (dist - min) / (max - min));
        if (finalColor[3] < 0.0f)
            finalColor[3] = 0.0f;
    }

    ST_DrawString(finalx, finaly, str, style | UI_CENTER, finalColor, fontSize);
}

/*
===============
CG_DrawChar

Coordinates and size in 640*480 virtual screen size
===============
*/
void CG_DrawChar( int x, int y, int width, int height, int ch ) {
	int	q;
	int row, col;
	float frow, fcol;
	float size;
	float	ax, ay, aw, ah;

	q = 0;
	if(height > 16){
	q = 1;	
	}
	if(height > 32){
	q = 2;	
	}

	ch &= 255;

	if ( ch == ' ' ) {
		return;
	}

	ax = x;
	ay = y;
	aw = width;
	ah = height;
	CG_AdjustFrom640( &ax, &ay, &aw, &ah );

	row = ch>>4;
	col = ch&15;

	frow = row*0.0625;
	fcol = col*0.0625;
	size = 0.0625;

	trap_R_DrawStretchPic( ax, ay, aw, ah, fcol, frow, fcol + size, frow + size, cgs.media.defaultFont[q] );
}

/*
==================
CG_DrawString

Draws a multi-colored string with a drop shadow, optionally forcing
to a fixed color.

Coordinates are at 640 by 480 virtual resolution
==================
*/
void CG_DrawString(int x, int y, const char *string, const float *setColor,
                      qboolean forceColor, qboolean shadow, int charWidth, 
                      int charHeight, int maxChars, float offset) {
    vec4_t color;
    const char *s;
    int xx;
    int cnt;
    char ch;
    int prev_unicode = 0;

    // Set maxChars to a default if it's non-positive
    if (maxChars <= 0) {
        maxChars = 32767; // Render all characters
    }

    // Draw the drop shadow if enabled
    if (shadow) {
        color[0] = color[1] = color[2] = 0; // Set shadow color to black
        color[3] = setColor[3]; // Preserve alpha
        trap_R_SetColor(color);

        s = string;
        xx = x;
        cnt = 0;

        while (*s && cnt < maxChars) {
            if (Q_IsColorString(s)) {
                s += 2; // Skip color codes
                continue;
            }

            ch = *s & 255; // Handle character encoding
            if (ch < 0) {
                // Handle potential Unicode characters
                if ((ch == -48) || (ch == -47)) {
                    prev_unicode = ch;
                    s++;
                    cnt++;
                    continue;
                }
                if (ch >= -112) {
                    ch = (ch == -111 && prev_unicode == -47) ? ch - 13 : ch + 48;
                } else {
                    ch = (ch == -127 && prev_unicode == -48) ? ch : ch + 112; // Adjust for Unicode offset
                }
            }

            // Draw the shadow character
            CG_DrawChar(xx + 1, y + 1, charWidth, charHeight, ch);
            cnt++;
            xx += charWidth+offset;
            s++;
        }
    }

    // Draw the colored text
    s = string;
    xx = x;
    cnt = 0;
    trap_R_SetColor(setColor); // Set text color

    while (*s && cnt < maxChars) {
        if (Q_IsColorString(s)) {
            if (!forceColor) {
                memcpy(color, g_color_table[ColorIndex(*(s + 1))], sizeof(color));
                color[3] = setColor[3]; // Preserve alpha
                trap_R_SetColor(color);
            }
            s += 2; // Skip color codes
            continue;
        }

        ch = *s & 255; // Handle character encoding
        // Handle potential Unicode characters
        if (ch < 0) {
            if ((ch == -48) || (ch == -47)) {
                prev_unicode = ch;
                s++;
                cnt++;
                continue;
            }
            if (ch >= -112) {
                ch = (ch == -111 && prev_unicode == -47) ? ch - 13 : ch + 48;
            } else {
                ch = (ch == -127 && prev_unicode == -48) ? ch : ch + 112; // Adjust for Unicode offset
            }
        }

        // Draw the character
        CG_DrawChar(xx, y, charWidth, charHeight, ch);
        xx += charWidth+offset;
        cnt++;
        s++;
    }

    // Reset color
    trap_R_SetColor(NULL);
}


void CG_DrawBigString( int x, int y, const char *s, float alpha ) {
	float	color[4];

	color[0] = color[1] = color[2] = 1.0;
	color[3] = alpha;
	CG_DrawString( x, y, s, color, qfalse, qtrue, BIGCHAR_WIDTH, BIGCHAR_HEIGHT, 0, 0 );
}

void CG_DrawGiantString( int x, int y, const char *s, float alpha ) {
	float	color[4];

	color[0] = color[1] = color[2] = 1.0;
	color[3] = alpha;
	CG_DrawString( x, y, s, color, qfalse, qtrue, GIANTCHAR_WIDTH, GIANTCHAR_HEIGHT, 0, 0 );
}

void CG_DrawSmallString( int x, int y, const char *s, float alpha ) {
	float	color[4];

	color[0] = color[1] = color[2] = 1.0;
	color[3] = alpha;
	CG_DrawString( x, y, s, color, qfalse, qfalse, BASEFONT_INDENT, BASEFONT_HEIGHT, 0, 0 );
}

/*
=================
CG_DrawStrlen

Returns character count, skiping color escape codes
=================
*/
int CG_DrawStrlen( const char *str ) {
	const char *s = str;
	int count = 0;

	while ( *s ) {
		if ( Q_IsColorString( s ) ) {
			s += 2;
		} else {
                        if ((*s == -48) || (*s == -47)) {
                           s++;
                        } else {
                           count++;
			   s++;
                        }
		}
	}

	return count;
}

/*
================
CG_FadeColor
================
*/
float *CG_FadeColor( int startMsec, int totalMsec ) {
	static vec4_t		color;
	int			t;

	if ( startMsec == 0 ) {
		return NULL;
	}

	t = cg.time - startMsec;

	if ( t >= totalMsec ) {
		return NULL;
	}

	// fade out
	if ( totalMsec - t < FADE_TIME ) {
		color[3] = ( totalMsec - t ) * 1.0/FADE_TIME;
	} else {
		color[3] = 1.0;
	}
	color[0] = color[1] = color[2] = 1;

	return color;
}

qboolean CG_InsideBox( vec3_t mins, vec3_t maxs, vec3_t pos ){
	if (pos[0] < mins[0] || pos[0] > maxs[0]) return qfalse;
	if (pos[1] < mins[1] || pos[1] > maxs[1]) return qfalse;
	if (pos[2] < mins[2] || pos[2] > maxs[2]) return qfalse;

	return qtrue;
}
