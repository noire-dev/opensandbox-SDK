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
// cg_draw.c -- draw all of the graphical elements during
// active (after loading) gameplay

#include "../qcommon/ns_local.h"

int sortedTeamPlayers[TEAM_MAXOVERLAY];
int	numSortedTeamPlayers;

void CG_Draw3DModelToolgun( float x, float y, float w, float h, qhandle_t model, char *texlocation, char *material ) {
	refdef_t		refdef;
	refEntity_t		ent;
	vec3_t			origin, angles;

	CG_AdjustFrom640( &x, &y, &w, &h );

	memset( &refdef, 0, sizeof( refdef ) );

	memset( &ent, 0, sizeof( ent ) );

	origin[0] = 70;
	origin[1] = 0;
	origin[2] = 0;

	angles[0] = 0;
	angles[1] = ( cg.time & 4095 ) * 360 / 4096.0;
	angles[2] = 0;

	AnglesToAxis( angles, ent.axis );
	VectorCopy( origin, ent.origin );
	ent.hModel = model;
	ent.shaderRGBA[0] = 0;
	ent.shaderRGBA[1] = 0;
	ent.shaderRGBA[2] = 0;
	ent.shaderRGBA[3] = 255;

	refdef.rdflags = RDF_NOWORLDMODEL;

	AxisClear( refdef.viewaxis );

	refdef.fov_x = 90;
	refdef.fov_y = 90;

	ent.reType = RT_MODEL;
	ent.customSkin = trap_R_RegisterSkin(va("ptex/%s/%s.skin", texlocation, material));
	if(atoi(material) > 0){		
		ent.customShader = trap_R_RegisterShader(va("ptex/%s/%s", texlocation, material));
	}		
	if(atoi(material) == 255){	
		ent.customShader = cgs.media.ptexShader[1];
	}

	refdef.x = x;
	refdef.y = y;
	refdef.width = w;
	refdef.height = h;

	refdef.time = cg.time;

	trap_R_ClearScene();
	trap_R_AddRefEntityToScene( &ent );
	trap_R_RenderScene( &refdef );
}

/*
================
CG_DrawHead

Used for both the status bar and the scoreboard
================
*/
void CG_DrawHead( float x, float y, float w, float h, int clientNum ) {
	clientInfo_t	*ci;

	ci = &cgs.clientinfo[ clientNum ];
	CG_DrawPic( x, y, w, h, ci->modelIcon );
}

/*
================
CG_DrawFlagModel

Used for both the status bar and the scoreboard
================
*/
void CG_DrawFlagModel( float x, float y, float w, float h, int team ) {
	gitem_t *item;
	if( team == TEAM_RED ) {
		item = BG_FindItemForPowerup( PW_REDFLAG );
	} else if( team == TEAM_BLUE ) {
		item = BG_FindItemForPowerup( PW_BLUEFLAG );
	} else if( team == TEAM_FREE ) {
		item = BG_FindItemForPowerup( PW_NEUTRALFLAG );
	} else {
		return;
	}
	if (item) {
	  CG_DrawPic( x, y, w, h, cg_items[ ITEM_INDEX(item) ].icon );
	}
}

void SplitStringBySpace(const char *str, char result[MAX_ENTITYINFO][64]) {
    int i = 0;
    int wordIndex = 0;
    int charIndex = 0;

    while (str[i] != '\0' && wordIndex < MAX_ENTITYINFO) {
        while (isspace(str[i])) {
            i++;
        }

        while (str[i] != '\0' && !isspace(str[i])) {
            result[wordIndex][charIndex++] = str[i++];
        }

        result[wordIndex][charIndex] = '\0';

        wordIndex++;
        charIndex = 0;
    }
}

#define WICON_SIDE 		32*0.60
#define WICON_SELECT 	32*0.90
#define WICON_SPACE 	3
#define WICONS_SIDES 	6

/*
===================
CG_DrawWeaponSelect
===================
*/
void CG_DrawWeaponSelect( void ) {
	float y, x, originalX, originalY;
	int i, j;

	if ( cg.snap->ps.pm_flags & PMF_FOLLOW )
		return;

	if ( cg.predictedPlayerState.stats[STAT_HEALTH] <= 0 )
		return;

	if ( !cg_draw2D.integer || cg.showScores )
		return;

	originalX = (530+cgs.wideoffset) - ((WICON_SIDE+WICON_SPACE)*WICONS_SIDES) - ((WICON_SELECT/2)+WICON_SPACE) - 1.5;
	originalY = 440;

	if(ST_AnimValue(&weaponSelectIn, cg.time)){
		originalY += ST_AnimValue(&weaponSelectIn, cg.time)*40;
	}

	if(cg.time-anim_weaponSelect >= 400+2000 && anim_weaponSelect){
		if(!ST_AnimValue(&weaponSelectOut, cg.time)){
			anim_weaponSelect = 0;
			ST_AnimStart(&weaponSelectOut, cg.time, 600);
		}
	}

	if(ST_AnimValue(&weaponSelectOut, cg.time)){
		originalY += 40;
		originalY -= ST_AnimValue(&weaponSelectOut, cg.time)*40;
	} else {
		if(!anim_weaponSelect)
			originalY += 40;
	}

	y = originalY+(WICON_SELECT*0.25);
	x = originalX+(WICON_SELECT/2)+WICON_SPACE;

	CG_DrawRoundedRect((originalX - WICONS_SIDES) - ((WICON_SIDE+WICON_SPACE)*WICONS_SIDES) - ((WICON_SELECT/2)+WICON_SPACE), originalY, ((WICON_SIDE+WICON_SPACE)*WICONS_SIDES*2) + WICON_SELECT + WICON_SPACE*2 + WICONS_SIDES, WICON_SELECT+WICON_SPACE, 4, color_dim);

	for (i = cg.weaponSelect+1, j = 0; i <= WEAPONS_NUM; i++) {
		if(j >= WICONS_SIDES){
		    continue;
		} else if (i >= WEAPONS_NUM){
			i = 1;
		}

		if(!cg.swep_listcl[i])
		    continue;

		CG_RegisterWeapon(i);
		CG_DrawPic(x, y, WICON_SIDE, WICON_SIDE, cg_weapons[i].weaponIcon);

		if(cg.swep_listcl[i] == 2)
			CG_DrawPic(x, y, WICON_SIDE, WICON_SIDE, cgs.media.noammoShader);

		x += WICON_SIDE+WICON_SPACE;
		j++;
	}

	x = originalX-(WICON_SELECT/2)-(WICON_SIDE+WICON_SPACE);
	for (i = cg.weaponSelect-1, j = 0; i >= 0; i--) {
		if(j >= WICONS_SIDES){
		    continue;
		} else if (i <= 0){
			i = WEAPONS_NUM-1;
		}

		if(!cg.swep_listcl[i])
		    continue;

		CG_RegisterWeapon(i);
		CG_DrawPic(x, y, WICON_SIDE, WICON_SIDE, cg_weapons[i].weaponIcon);

		if(cg.swep_listcl[i] == 2)
			CG_DrawPic(x, y, WICON_SIDE, WICON_SIDE, cgs.media.noammoShader);

		x -= WICON_SIDE+WICON_SPACE;
		j++;
	}

	CG_DrawPic( originalX-WICON_SELECT/2, originalY+2, WICON_SELECT, WICON_SELECT, cg_weapons[cg.weaponSelect].weaponIcon );
	CG_DrawPic( originalX-WICON_SELECT/2, originalY+2, WICON_SELECT, WICON_SELECT, cgs.media.selectShader );
	if(cg.swep_listcl[cg.weaponSelect] == 2)
		CG_DrawPic(originalX-WICON_SELECT/2, originalY+2, WICON_SELECT, WICON_SELECT, cgs.media.noammoShader);
}

static void CG_DrawToolgun() {
	char entityInfos[MAX_ENTITYINFO][64];
	float x, y;
	gitem_t	*it;
	int		count = 0;
	int		i = 0;
	vec4_t  colorblk;

	colorblk[0]=0.0f;
	colorblk[1]=0.0f;
	colorblk[2]=0.0f;
	colorblk[3]=0.75f;
	
	if(toolgun_tool.integer == TL_CREATE){
		trap_R_RemapShader( "models/weapons/toolgun/screen", "models/weapons/toolgun/screen", "0.005" );
	} else {
		if(trap_R_RegisterShader(va("models/weapons/toolgun/tool%i", toolgun_tool.integer)) != 0){
			trap_R_RemapShader( "models/weapons/toolgun/screen", va("models/weapons/toolgun/tool%i", toolgun_tool.integer) , "0.005" );
		} else {
			trap_R_RemapShader( "models/weapons/toolgun/screen", "models/weapons/toolgun/toolerror", "0.005" );
		}
	}
	
	CG_DrawPic( -1 - cgs.wideoffset, 40, 300, 125, trap_R_RegisterShaderNoMip( "menu/assets/blacktrans" ) );
	ST_DrawString(0-cgs.wideoffset, 42, toolgun_tooltext.string, UI_LEFT, color_white, 2.50);
	if(toolgun_mod19.integer == 0){
		ST_DrawString(0-cgs.wideoffset, 72, toolgun_toolmode1.string, UI_LEFT, color_white, 1.32);
	} else if(toolgun_mod19.integer == 1){
		ST_DrawString(0-cgs.wideoffset, 72, toolgun_toolmode2.string, UI_LEFT, color_white, 1.32);
	} else if(toolgun_mod19.integer == 2){
		ST_DrawString(0-cgs.wideoffset, 72, toolgun_toolmode3.string, UI_LEFT, color_white, 1.32);
	} else if(toolgun_mod19.integer == 3){
		ST_DrawString(0-cgs.wideoffset, 72, toolgun_toolmode4.string, UI_LEFT, color_white, 1.32);
	}
	y = 90;  ST_DrawString(0-cgs.wideoffset, y, toolgun_tooltip1.string, UI_LEFT, color_white, 1.32);
	y += 15; ST_DrawString(0-cgs.wideoffset, y, toolgun_tooltip2.string, UI_LEFT, color_white, 1.32);
	y += 15; ST_DrawString(0-cgs.wideoffset, y, toolgun_tooltip3.string, UI_LEFT, color_white, 1.32);
	y += 15; ST_DrawString(0-cgs.wideoffset, y, toolgun_tooltip4.string, UI_LEFT, color_white, 1.32);
	
	if(strlen(cg.entityInfo) > 0){
    	SplitStringBySpace(cg.entityInfo, entityInfos);

		x = 340;
		y = 260+48;
		CG_DrawRoundedRect(x-5, y-48-5, 300, 48+15+(MAX_ENTITYINFO*10), 6, colorblk);

    	if (strcmp(entityInfos[0], "<NULL>")) {
			if(!BG_CheckClassname(entityInfos[0])){
				//nothing
			} else {
				for ( it = bg_itemlist + 1 ; it->classname ; it++ ) {
					if ( !Q_stricmp( it->classname, entityInfos[0] ) ){
						CG_DrawPic( x, y-48, 48, 48, trap_R_RegisterShaderNoMip( it->icon ) );
					}
				}
			}
			ST_DrawString(x, y+3 + (count * 10), va("Class: %s", entityInfos[0]), UI_LEFT, color_white, 1.00);
			count++;
    	}
		
		if (strcmp(entityInfos[1], "<NULL>")) {
			if(!BG_CheckClassname(entityInfos[0])){
				CG_Draw3DModelToolgun( x, y-48, 48, 48,
				trap_R_RegisterModel( entityInfos[1] ), entityInfos[1], entityInfos[2] );
			} else {
				//nothing
			}
			if(!strcmp(entityInfos[0], "player")) {
				CG_DrawHead( x, y-48, 48, 48, atoi(entityInfos[1]) );
				count--;
			} else {
				ST_DrawString(x, y+3 + (count * 10), va("Model: %s", entityInfos[1]), UI_LEFT, color_white, 1.00);
			}
			count++;
    	}

		if (strcmp(entityInfos[2], "<NULL>")) {
			ST_DrawString(x, y+3 + (count * 10), va("Material: %s", entityInfos[2]), UI_LEFT, color_white, 1.00);
			count++;
    	}

		if (strcmp(entityInfos[3], "<NULL>")) {
			if (strcmp(entityInfos[3], "0")) {
				ST_DrawString(x, y+3 + (count * 10), va("Count: %s", entityInfos[3]), UI_LEFT, color_white, 1.00);
				count++;
			} else {
				for ( it = bg_itemlist + 1 ; it->classname ; it++ ) {
					if ( !Q_stricmp( it->classname, entityInfos[0] ) ){
						ST_DrawString(x, y+3 + (count * 10), va("Count: %i", it->quantity), UI_LEFT, color_white, 1.00);
						count++;
					}
				}
			}
    	}
	}
}

static void CG_DrawStatusElement( float x, float y, int value, const char *text ) {
	CG_DrawRoundedRect(x, y, 100, 32, 4, color_dim);
	ST_DrawString( x+4, y+18, text, UI_LEFT, customcolor_crosshair, 0.90);
	ST_DrawString( x+42, y+6, va("%i", value), UI_LEFT, customcolor_crosshair, 2.32 );
}

static void CG_DrawCounterElement( float x, float y, const char *value, const char *text ) {
	CG_DrawRoundedRect(x, y, 80, 15, 2, color_dim);
	ST_DrawString( x+2, y+5, text, UI_LEFT, customcolor_crosshair, 0.90);
	ST_DrawString( x+40, y+3, value, UI_LEFT, customcolor_crosshair, 1.20);
}

static void CG_DrawDomElement( float x, float y, const char *value, const char *text ) {
	CG_DrawRoundedRect(x, y, 150, 16, 0, color_dim);
	ST_DrawString( x+2, y+6, text, UI_LEFT, customcolor_crosshair, 0.90);
	ST_DrawString( x+110, y+2, value, UI_LEFT, customcolor_crosshair, 1.20);
}

/*
================
CG_DrawStatusBar

================
*/
static void CG_DrawStatusBar( void ) {
	centity_t	*cent;
	playerState_t	*ps;
	int			value;
	clientInfo_t	*ci;
	int				weaphack;
	vec_t       *vel;

	vel = cg.snap->ps.velocity;

	cent = &cg_entities[cg.snap->ps.clientNum];
	ps = &cg.snap->ps;
	
	ci = &cgs.clientinfo[ cent->currentState.clientNum ];
	
	weaphack = ci->swepid;

	if ( cg.showScores || !cg_draw2D.integer ){
		return;
	}

	// draw any 3D icons first, so the changes back to 2D are minimized
	if (weaphack) {
		if(weaphack == WP_TOOLGUN){
			CG_DrawToolgun();
		}
	}

	if( cg.predictedPlayerState.powerups[PW_REDFLAG] ) {
		CG_DrawFlagModel( 0 - cgs.wideoffset, 480 - ICON_SIZE - 38, ICON_SIZE, ICON_SIZE, TEAM_RED );
	} else if( cg.predictedPlayerState.powerups[PW_BLUEFLAG] ) {
		CG_DrawFlagModel( 0 - cgs.wideoffset, 480 - ICON_SIZE - 38, ICON_SIZE, ICON_SIZE, TEAM_BLUE );
	} else if( cg.predictedPlayerState.powerups[PW_NEUTRALFLAG] ) {
		CG_DrawFlagModel( 0 - cgs.wideoffset, 480 - ICON_SIZE - 38, ICON_SIZE, ICON_SIZE, TEAM_FREE );
	}

	//
	// ammo
	//
	if(!ps->stats[STAT_VEHICLE]){
	if ( weaphack ) { //VEHICLE-SYSTEM: vehicle's speedmeter for all
		value = ps->stats[STAT_SWEPAMMO];
		if(value <= 0 && value != -1){	// OpenSandbox weapon predict
			cg.swep_listcl[ps->generic2] = 2;
		} else {
			cg.swep_listcl[ps->generic2] = 1;	
		}
		CG_DrawWeaponSelect();
		if (value > -1)
			CG_DrawStatusElement(630+cgs.wideoffset-100, 440, value, "AMMO");
	}
	} else {
		value = sqrt(vel[0] * vel[0] + vel[1] * vel[1]) * 0.20;
		CG_DrawStatusElement(630+cgs.wideoffset-100, 440, value, "KM/H");
	}

	//
	// health
	//
	if(!ps->stats[STAT_VEHICLE]){ //VEHICLE-SYSTEM: vehicle's hp instead player
		value = ps->stats[STAT_HEALTH];
	} else {
		value = ps->stats[STAT_VEHICLEHP];	
	}
	CG_DrawStatusElement(10-cgs.wideoffset, 440, value, "HEALTH");
		
	//
	// armor
	//
	value = ps->stats[STAT_ARMOR];
	if (value > 0 ) {
		CG_DrawStatusElement(20-cgs.wideoffset+100, 440, value, "ARMOR");
	}
}

/*
===========================================================================================

  UPPER RIGHT CORNER

===========================================================================================
*/

qboolean n_tip1 = qfalse;
qboolean n_tip2 = qfalse;

/*
================
CG_DrawCounters

================
*/
#define	FPS_FRAMES	4
static void CG_DrawCounters( void ) {
	int			y;
	int         value;
	int			t, frameTime;
	static int	previousTimes[FPS_FRAMES];
	static int	index;
	static int	previous;
	int			i, total;
	int			mins, seconds, tens;
	int			msec;
	int 		rst;

	y = 0;

	msec = cg.time - cgs.levelStartTime;

	seconds = msec / 1000;

	if(seconds < 3){
		n_tip1 = qfalse;
		n_tip2 = qfalse;
	}

	if(cgs.gametype == GT_SANDBOX || cgs.gametype == GT_MAPEDITOR){
		if(seconds == 3 && !n_tip1){
			CG_AddNotify ("Welcome to OpenSandbox", NOTIFY_INFO);
			n_tip1 = qtrue;
		}

		if(seconds == 6 && !n_tip2){
			CG_AddNotify ("Press [Q] to open the spawn menu", NOTIFY_INFO);
			n_tip2 = qtrue;
		}
	}

	if ( cg.scoreBoardShowing ) {
		return;
	}

	y += 4;

	//Speed
	value = sqrt(cg.snap->ps.velocity[0] * cg.snap->ps.velocity[0] + cg.snap->ps.velocity[1] * cg.snap->ps.velocity[1]);
	if (cg_drawSpeed.integer == 1) {
		CG_DrawCounterElement(640+cgs.wideoffset-84, y, va("%i", value), "Speed");
		y += 20;
	}

	//FPS
	t = trap_Milliseconds();
	frameTime = t - previous;
	previous = t;

	previousTimes[index % FPS_FRAMES] = frameTime;
	index++;
	if ( index > FPS_FRAMES ) {
		// average multiple frames together to smooth changes out a bit
		total = 0;
		for ( i = 0 ; i < FPS_FRAMES ; i++ ) {
			total += previousTimes[i];
		}
		if ( !total ) {
			total = 1;
		}
		value = 1000 * FPS_FRAMES / total;
	}
	if (cg_drawFPS.integer == 1) {
		CG_DrawCounterElement(640+cgs.wideoffset-84, y, va("%i", value), "FPS");
		y += 20;
	}

	//Timer
	msec = cg.time - cgs.levelStartTime;

	seconds = msec / 1000;
	mins = seconds / 60;
	seconds -= mins * 60;
	tens = seconds / 10;
	seconds -= tens * 10;

	if (cg_drawTimer.integer == 1) {
		CG_DrawCounterElement(640+cgs.wideoffset-84, y, va("%i:%i%i", mins, tens, seconds), "Time");
		y += 20;
	}

    //Skulls!
	if(cgs.gametype == GT_HARVESTER) {
        value = cg.snap->ps.generic1;
        if (value > 0 ) {
			CG_DrawCounterElement(640+cgs.wideoffset-84, y, va("%i", value), "Skulls");
        }
    }
	return;
}

/*
===========================================================================================

  LOWER RIGHT CORNER

===========================================================================================
*/

/*
=================
CG_DrawScores

Draw the small two score display
=================
*/
#define SCORES_WIDTH 	32
#define SCORES_HEIGHT 	20
static void CG_DrawScores( void ) {
	const char	*s;
	int			s1, s2, score;
	int			x;
	int			v;
	vec4_t		colorb, colorr, colorg, colorgr;
	float 		y = 0;

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

	if(cg.showScores){
		return;
	}

	// draw from the right side to left
	if(cgs.gametype >= GT_TEAM) {
		//blue score
		x = 320;
		s = va("%i", s2);
		CG_DrawRoundedRect(x, y, SCORES_WIDTH, SCORES_HEIGHT, 0, colorb);
		ST_DrawString( x+16, y+3, s, UI_CENTER, color_white, 1.20);

		//blue flag
		if(cgs.gametype == GT_CTF) {
			if (BG_FindItemForPowerup(PW_BLUEFLAG)) {
				if(cgs.blueflag >= 0 && cgs.blueflag <= 2) {
					CG_DrawPic( x+SCORES_WIDTH, y, SCORES_WIDTH, SCORES_HEIGHT, cgs.media.blueFlagShader[cgs.blueflag] );
				}
			}
		}

        if(cgs.gametype == GT_OBELISK) {
            s = va("%i%%",cg.blueObeliskHealth);
			CG_DrawRoundedRect(x+SCORES_WIDTH, y, SCORES_WIDTH, SCORES_HEIGHT-4, 0, color_dim);
			ST_DrawString( x+(SCORES_WIDTH*1.50), y+2, s, UI_CENTER, color_white, 1.20);
        }

		//red score
		s = va( "%i", s1 );
		x = 320-SCORES_WIDTH;
		CG_DrawRoundedRect(x, y, SCORES_WIDTH, SCORES_HEIGHT, 0, colorr);
		ST_DrawString( x+16, y+3, s, UI_CENTER, color_white, 1.20);

		//red flag
		if (cgs.gametype == GT_CTF) {
			if (BG_FindItemForPowerup(PW_REDFLAG)) {
				if( cgs.redflag >= 0 && cgs.redflag <= 2 ) {
					CG_DrawPic( x-SCORES_WIDTH, y, SCORES_WIDTH, SCORES_HEIGHT, cgs.media.redFlagShader[cgs.redflag] );
				}
			}
		}

        if (cgs.gametype == GT_OBELISK) {
            s = va("%i%%",cg.redObeliskHealth);
			CG_DrawRoundedRect(x-SCORES_WIDTH, y, SCORES_WIDTH, SCORES_HEIGHT-4, 0, color_dim);
			ST_DrawString( x-(SCORES_WIDTH*0.50), y+2, s, UI_CENTER, color_white, 1.20);
        }

		// limit
		if (cgs.gametype >= GT_CTF) {
			v = cgs.capturelimit;
		} else {
			v = cgs.fraglimit;
		}
		if (v) {
			s = va("%i", v);
			x = 320-32;
			CG_DrawRoundedRect(x, y+SCORES_HEIGHT, SCORES_WIDTH*2, SCORES_HEIGHT, 0, color_dim);
			ST_DrawString( x+SCORES_WIDTH, y+23, s, UI_CENTER, color_white, 1.20);
		}

	} else {
		x = 320;
		score = cg.snap->ps.persistant[PERS_SCORE];

		// always show your score in the second box if not in first place
		if(s1 != score) {
			s2 = score;
		}

		// player place
		if (s2 != SCORE_NOT_PRESENT) {
			s = va("%i", s2);
			if ( s1 != score ) {
				CG_DrawRoundedRect(x, y, SCORES_WIDTH, SCORES_HEIGHT, 0, colorgr);
			} else {
				CG_DrawRoundedRect(x, y, SCORES_WIDTH, SCORES_HEIGHT, 0, colorg);	
			}
			ST_DrawString( x+16, y+3, s, UI_CENTER, color_white, 1.20);
		}

		// first place
		if (s1 != SCORE_NOT_PRESENT) {
			s = va("%i", s1);
			if ( s1 == score ) {
				CG_DrawRoundedRect(x-SCORES_WIDTH, y, SCORES_WIDTH, SCORES_HEIGHT, 0, colorgr);
			} else {
				CG_DrawRoundedRect(x-SCORES_WIDTH, y, SCORES_WIDTH, SCORES_HEIGHT, 0, colorg);	
			}
			CG_DrawRoundedRect(x-SCORES_WIDTH, y, SCORES_WIDTH, SCORES_HEIGHT, 0, colorg);
			ST_DrawString((x+16)-SCORES_WIDTH, y+3, s, UI_CENTER, color_white, 1.20);
		}

		// limit
		if (cgs.fraglimit) {
			s = va("%i", cgs.fraglimit);
			x = 320-32;
			CG_DrawRoundedRect(x, y+SCORES_HEIGHT, 64, 20, 0, color_dim);
			ST_DrawString( x+SCORES_WIDTH, y+23, s, UI_CENTER, color_white, 1.20);
		}
	}
}

/*
================
CG_DrawPowerups
================
*/
#define PW_ICON_SIZE ICON_SIZE*0.60

static void CG_DrawPowerups( void ) {
	playerState_t *ps = &cg.snap->ps;
	gitem_t *item;
	int 	i, t;
	float 	y = 150;

	if (ps->stats[STAT_HEALTH] <= 0) {
		return;
	}

	for (i = 0; i < MAX_POWERUPS; i++) {
		if (!ps->powerups[i]) {
			continue;
		}

		t = ps->powerups[i] - cg.time;
		if (t < 0 || t > 99999000) {
			continue;
		}

		item = BG_FindItemForPowerup(i);
		if (!item || item->giType == IT_PERSISTANT_POWERUP) {
			continue;
		}

		y += PW_ICON_SIZE+2;
		CG_DrawRoundedRect(2-cgs.wideoffset, y, 64, PW_ICON_SIZE, 4, color_dim);
		CG_DrawPic(3-cgs.wideoffset, y, PW_ICON_SIZE, PW_ICON_SIZE, trap_R_RegisterShader(item->icon));
		ST_DrawString((4-cgs.wideoffset)+PW_ICON_SIZE, y+3, va("%i", t/1000), UI_LEFT, color_white, 1.25);
	}
}

/*
===================
CG_DrawPickupItem
===================
*/
static int CG_DrawPickupItem( int y ) {
	int		value;
	float	*fadeColor;

	if ( cg.snap->ps.stats[STAT_HEALTH] <= 0 ) {
		return y;
	}

	y -=  ICON_SIZE;

	value = cg.itemPickup;
	if ( value ) {
		fadeColor = CG_FadeColor( cg.itemPickupTime, 3000 );
		if ( fadeColor ) {
			CG_RegisterItemVisuals( value );
			trap_R_SetColor( fadeColor );
			CG_DrawPic( 8 - cgs.wideoffset, y, ICON_SIZE, ICON_SIZE, cg_items[ value ].icon );
			CG_DrawBigString( ICON_SIZE - cgs.wideoffset + 16, y + (ICON_SIZE/2 - BIGCHAR_HEIGHT/2), bg_itemlist[ value ].pickup_name, fadeColor[0] );
			trap_R_SetColor( NULL );
		}
	}

	return y;
}

/*
=====================
CG_DrawLowerLeft

=====================
*/
static void CG_DrawLowerLeft( void ) {
	float	y;

	y = 50 + ICON_SIZE;

	y = CG_DrawPickupItem( y );
}

void CG_ConsoleUpdateIdx(console_t *console, int chatHeight) {
	if (console->insertIdx < console->displayIdx) {
		console->displayIdx = console->insertIdx;
		}

	if (console->insertIdx - console->displayIdx > chatHeight) {
		console->displayIdx = console->insertIdx - chatHeight;
	}
}

static void CG_DrawGenericConsole( console_t *console, int maxlines, int time, int x, int y, float sizeX, float sizeY ) {
	int i, j;
	vec4_t	hcolor;

	if ( cg.showScores || cg.scoreBoardShowing ) {
		return;
	}

	CG_ConsoleUpdateIdx(console, maxlines);

	hcolor[0] = hcolor[1] = hcolor[2] = hcolor[3] = 1.0f;

	j = 0;
	for (i = console->displayIdx; i < console->insertIdx ; ++i) {
		if (console->msgTimes[i % CONSOLE_MAXHEIGHT] + time < cg.time) {
			continue;
		}
		CG_DrawString( x + 1, y + j * sizeY, console->msgs[i % CONSOLE_MAXHEIGHT], hcolor, qfalse, qtrue, sizeX, sizeY, 0, 0 );
		j++;

	}
}

void CG_AddToGenericConsole( const char *str, console_t *console ) {
	int len;
	char *p, *ls;
	int lastcolor;

	len = 0;

	p = console->msgs[console->insertIdx % CONSOLE_MAXHEIGHT];
	*p = 0;

	lastcolor = '7';

	ls = NULL;
	while (*str) {
		if (*str == '\n' || len > CONSOLE_WIDTH - 1) {
			if (*str == '\n') {
				str++;
				if (*str == '\0') {
					continue;
				}
			} else if (ls) {
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

		if ( Q_IsColorString( str ) ) {
			*p++ = *str++;
			lastcolor = *str;
			*p++ = *str++;
			continue;
		}
		if (*str == ' ') {
			ls = p;
		}
		*p++ = *str++;
		len++;
	}
	*p = 0;

	console->msgTimes[console->insertIdx % CONSOLE_MAXHEIGHT] = cg.time;
	console->insertIdx++;

}

/*
===================
CG_DrawHoldableItem
===================
*/
static void CG_DrawHoldableItem( void ) { 
	int		value;

	value = cg.snap->ps.stats[STAT_HOLDABLE_ITEM];

	if ( value ) {
		CG_RegisterItemVisuals( value );
		CG_DrawPic( 640-ICON_SIZE, (SCREEN_HEIGHT-ICON_SIZE)/2, ICON_SIZE, ICON_SIZE, cg_items[ value ].icon );
	}
}

/*
===================
CG_DrawPersistantPowerup
===================
*/
static void CG_DrawPersistantPowerup( void ) {
	int		value;

	value = cg.snap->ps.stats[STAT_PERSISTANT_POWERUP];
	if ( value ) {
		CG_RegisterItemVisuals( value );
		CG_DrawPic( 0 - cgs.wideoffset, (0+SCREEN_HEIGHT / 2) - ICON_SIZE, ICON_SIZE, ICON_SIZE, cg_items[ value ].icon );
	}
}

/*
===============================================================================

LAGOMETER

===============================================================================
*/

#define	LAG_SAMPLES			128
#define	MAX_LAGOMETER_PING	900
#define	MAX_LAGOMETER_RANGE	300

typedef struct {
	int		frameSamples[LAG_SAMPLES];
	int		frameCount;
	int		snapshotFlags[LAG_SAMPLES];
	int		snapshotSamples[LAG_SAMPLES];
	int		snapshotCount;
} lagometer_t;

lagometer_t		lagometer;

/*
==============
CG_AddLagometerFrameInfo

Adds the current interpolate / extrapolate bar for this frame
==============
*/
void CG_AddLagometerFrameInfo( void ) {
	int			offset;

	offset = cg.time - cg.latestSnapshotTime;
	lagometer.frameSamples[ lagometer.frameCount & ( LAG_SAMPLES - 1) ] = offset;
	lagometer.frameCount++;
}

/*
==============
CG_AddLagometerSnapshotInfo

Each time a snapshot is received, log its ping time and
the number of snapshots that were dropped before it.

Pass NULL for a dropped packet.
==============
*/
void CG_AddLagometerSnapshotInfo( snapshot_t *snap ) {
	lagometer.snapshotSamples[ lagometer.snapshotCount & ( LAG_SAMPLES - 1) ] = snap->ping;
	lagometer.snapshotFlags[ lagometer.snapshotCount & ( LAG_SAMPLES - 1) ] = snap->snapFlags;
	lagometer.snapshotCount++;
}

/*
==============
CG_DrawLagometer
==============
*/
static void CG_DrawLagometer( void ) {
	int		a, x, y, i;
	float	v;
	float	ax, ay, aw, ah, mid, range;
	int		color;
	float	vscale;

	if (!cg_lagometer.integer) {
		return;
	}

	//
	// draw the graph
	//
	x = 640 - 48;
	y = 480 - 48;

	trap_R_SetColor( NULL );
	CG_DrawPic( x + cgs.wideoffset, y, 48, 48, cgs.media.lagometerShader );

	ax = x + cgs.wideoffset;
	ay = y;
	aw = 48;
	ah = 48;
	CG_AdjustFrom640( &ax, &ay, &aw, &ah );

	color = -1;
	range = ah / 3;
	mid = ay + range;

	vscale = range / MAX_LAGOMETER_RANGE;

	// draw the frame interpoalte / extrapolate graph
	for ( a = 0 ; a < aw ; a++ ) {
		i = ( lagometer.frameCount - 1 - a ) & (LAG_SAMPLES - 1);
		v = lagometer.frameSamples[i];
		v *= vscale;
		if ( v > 0 ) {
			if ( color != 1 ) {
				color = 1;
				trap_R_SetColor( g_color_table[ColorIndex(COLOR_YELLOW)] );
			}
			if ( v > range ) {
				v = range;
			}
			trap_R_DrawStretchPic ( ax + aw - a, mid - v, 1, v, 0, 0, 0, 0, cgs.media.whiteShader );
		} else if ( v < 0 ) {
			if ( color != 2 ) {
				color = 2;
				trap_R_SetColor( g_color_table[ColorIndex(COLOR_BLUE)] );
			}
			v = -v;
			if ( v > range ) {
				v = range;
			}
			trap_R_DrawStretchPic( ax + aw - a, mid, 1, v, 0, 0, 0, 0, cgs.media.whiteShader );
		}
	}

	// draw the snapshot latency / drop graph
	range = ah / 2;
	vscale = range / MAX_LAGOMETER_PING;

	for ( a = 0 ; a < aw ; a++ ) {
		i = ( lagometer.snapshotCount - 1 - a ) & (LAG_SAMPLES - 1);
		v = lagometer.snapshotSamples[i];
		if ( v > 0 ) {
			if ( lagometer.snapshotFlags[i] & SNAPFLAG_RATE_DELAYED ) {
				if ( color != 5 ) {
					color = 5;	// YELLOW for rate delay
					trap_R_SetColor( g_color_table[ColorIndex(COLOR_MAGENTA)] );
				}
			} else {
				if ( color != 3 ) {
					color = 3;
					trap_R_SetColor( g_color_table[ColorIndex(COLOR_GREEN)] );
				}
			}
			v = v * vscale;
			if ( v > range ) {
				v = range;
			}
			trap_R_DrawStretchPic( ax + aw - a, ay + ah - v, 1, v, 0, 0, 0, 0, cgs.media.whiteShader );
		} else if ( v < 0 ) {
			if ( color != 4 ) {
				color = 4;		// RED for dropped snapshots
				trap_R_SetColor( g_color_table[ColorIndex(COLOR_RED)] );
			}
			trap_R_DrawStretchPic( ax + aw - a, ay + ah - range, 1, range, 0, 0, 0, 0, cgs.media.whiteShader );
		}
	}

	trap_R_SetColor( NULL );
}

/*
===============================================================================

CENTER PRINTING

===============================================================================
*/

/*
==============
CG_CenterPrint

Called for important messages that should stay in the center of the screen
for a few moments
==============
*/
void CG_CenterPrint( const char *str, int y, int charWidth ) {
	char	*s;

	Q_strncpyz( cg.centerPrint, str, sizeof(cg.centerPrint) );
	cg.centerPrintTimeC = CG_DrawStrlen( cg.centerPrint ) / 10;
	cg.centerPrintTime = cg.time;
	cg.centerPrintY = y;
	cg.centerPrintCharWidth = charWidth;

	// count the number of lines for centering
	cg.centerPrintLines = 1;
	s = cg.centerPrint;
	while( *s ) {
		if (*s == '\n')
			cg.centerPrintLines++;
		s++;
	}
}

/*
===================
CG_DrawCenterString
===================
*/
static void CG_DrawCenterString( void ) {
	char	*start;
	int		l;
	int		x, y, w;
	float	*color;

	if ( !cg.centerPrintTime ) {
		return;
	}

	if(cg.centerPrintTimeC >= 1){
	color = CG_FadeColor( cg.centerPrintTime, 1000 * cg.centerPrintTimeC );
	} else {
	color = CG_FadeColor( cg.centerPrintTime, 1000 * 5 );	
	}
	if ( !color ) {
		return;
	}

	trap_R_SetColor( color );

	start = cg.centerPrint;

	y = cg.centerPrintY - cg.centerPrintLines * BIGCHAR_HEIGHT / 2;

	while ( 1 ) {
		char linebuffer[1024];

		for ( l = 0; l < 1024; l++ ) {
			if ( !start[l] || start[l] == '\n' ) {
				break;
			}
			linebuffer[l] = start[l];
		}
		linebuffer[l] = 0;

		w = cg.centerPrintCharWidth * CG_DrawStrlen( linebuffer );

		x = ( SCREEN_WIDTH - w ) / 2;

		CG_DrawString( x, y, linebuffer, color, qfalse, qtrue, cg.centerPrintCharWidth, (int)(cg.centerPrintCharWidth * 1.5), 0, 0 );

		y += cg.centerPrintCharWidth * 1.5;
		while ( *start && ( *start != '\n' ) ) {
			start++;
		}
		if ( !*start ) {
			break;
		}
		start++;
	}

	trap_R_SetColor( NULL );
}

/*
=====================
CG_DrawCenter1FctfString
=====================
*/
static void CG_DrawCenter1FctfString( void ) {
    int		x, y, w;
    float       *color;
    char        *line;
    int status;

    if(cgs.gametype != GT_1FCTF)
        return;

    status = cgs.flagStatus;

    //Sago: TODO: Find the proper defines instead of hardcoded values.
    switch(status) {
        case 2:
            line = va("Red has the flag!");
            color = colorRed;
            break;
        case 3:
            line = va("Blue has the flag!");
            color = colorBlue;
            break;
        case 4:
            line = va("Flag dropped!");
            color = colorWhite;
            break;
        default:
            return;

    };
    y = 100;


    w = cg.centerPrintCharWidth * CG_DrawStrlen( line );

    x = ( SCREEN_WIDTH - w ) / 2;

    CG_DrawString( x, y, line, color, qfalse, qtrue,
		cg.centerPrintCharWidth, (int)(cg.centerPrintCharWidth * 1.5), 0, 0 );
}

/*
================================================================================

CROSSHAIR

================================================================================
*/

/*
=================
CG_DrawCrosshair
=================
*/
static void CG_DrawCrosshair(void) {
	float		w, h;
	qhandle_t	hShader;
	float		f;
	float		x, y;

	if (!cg_drawCrosshair.integer || cg.renderingThirdPerson || cg.snap->ps.persistant[PERS_TEAM] == TEAM_SPECTATOR) {
		return;
	}

	trap_R_SetColor( customcolor_crosshair );
	w = h = cg_crosshairScale.value;
	if(cgs.wideoffset > 0) {
		x = 0 - cgs.wideoffset;
	} else {
		x = 0;
	}
	y = 0;
	CG_AdjustFrom640( &x, &y, &w, &h );
	if(cg_drawCrosshair.integer > 0) {
		hShader = cgs.media.crosshairShader[ cg_drawCrosshair.integer % NUM_CROSSHAIRS ];

    	if(!hShader)
    	    hShader = cgs.media.crosshairShader[0];

		trap_R_DrawStretchPic( x + cg.refdef.x + 0.5 * (cg.refdef.width - w), y + cg.refdef.y + 0.5 * (cg.refdef.height - h), w, h, 0, 0, 1, 1, hShader );
	}
}

/*
=================
CG_DrawCrosshair3D
=================
*/
static void CG_DrawCrosshair3D(void) {
	float w, h;
	qhandle_t hShader;
	int ca;
	trace_t trace;
	vec3_t endpos;
	float zProj, maxdist;
	char rendererinfos[128];
	refEntity_t ent;

	if (!cg_drawCrosshair.integer || cg.snap->ps.persistant[PERS_TEAM] == TEAM_SPECTATOR)
		return;

	if (cg.predictedPlayerState.pm_type == PM_DEAD || cg.predictedPlayerState.pm_type == PM_INTERMISSION)
		return;

	w = h = cg_crosshairScale.value;

	ca = cg_drawCrosshair.integer;
	if (ca < 0) {
		ca = 0;
	}
	hShader = cgs.media.crosshairSh3d[ ca % NUM_CROSSHAIRS ];

	if (!hShader)
		hShader = cgs.media.crosshairSh3d[ ca % 10 ];

	// first get all the important renderer information
	trap_Cvar_VariableStringBuffer("r_zProj", rendererinfos, sizeof (rendererinfos));
	zProj = atof(rendererinfos);

    maxdist = 65536;

	memset(&ent, 0, sizeof (ent));
	ent.reType = RT_SPRITE;
	ent.renderfx = RF_DEPTHHACK | RF_CROSSHAIR | RF_FIRST_PERSON;

    VectorCopy(cg.predictedPlayerState.origin, ent.origin );
    ent.origin[2] = ent.origin[2]+cg.predictedPlayerState.viewheight;
    AnglesToAxis(cg.predictedPlayerState.viewangles, ent.axis);
    VectorMA(ent.origin, maxdist, ent.axis[0], endpos);

	CG_Trace(&trace, ent.origin, NULL, NULL, endpos, 0, MASK_SHOT);

	VectorCopy(trace.endpos, ent.origin);

	// scale the crosshair so it appears the same size for all distances
	ent.radius = w / 800 * zProj * tan(cg.refdef.fov_x * M_PI / 360.0f) * trace.fraction * maxdist / zProj;
	ent.customShader = hShader;
    ent.shaderRGBA[0] = cg_crosshairColorRed.value * 255;
	ent.shaderRGBA[1] = cg_crosshairColorGreen.value * 255;
	ent.shaderRGBA[2] = cg_crosshairColorBlue.value * 255;
    ent.shaderRGBA[3] = 255;
	trap_R_AddRefEntityToScene(&ent);
}

/*
=================
CG_ScanForCrosshairEntity
=================
*/
static void CG_ScanForCrosshairEntity( void ) {
	trace_t		trace;
	vec3_t		start, end;
	int			content;

	VectorCopy( cg.refdef.vieworg, start );
	VectorMA( start, 131072, cg.refdef.viewaxis[0], end );

	CG_Trace( &trace, start, vec3_origin, vec3_origin, end,
		cg.snap->ps.clientNum, CONTENTS_SOLID|CONTENTS_BODY );
	if ( trace.entityNum >= MAX_CLIENTS ) {
		return;
	}

	// if the player is in fog, don't show it
	content = CG_PointContents( trace.endpos, 0 );
	if ( content & CONTENTS_FOG ) {
		return;
	}

	// if the player is invisible, don't show it
	if ( cg_entities[ trace.entityNum ].currentState.powerups & ( 1 << PW_INVIS ) ) {
		return;
	}

	// update the fade timer
	cg.crosshairClientNum = trace.entityNum;
	cg.crosshairClientTime = cg.time;
}

/*
=====================
CG_DrawCrosshairNames
=====================
*/
static void CG_DrawCrosshairNames( void ) {
	float		*color;
	char		*name;

	if ( !cg_drawCrosshair.integer || !cg_drawCrosshairNames.integer ) {
		return;
	}

	CG_ScanForCrosshairEntity();
	color = CG_FadeColor(cg.crosshairClientTime, 1000);
	if ( !color ) {
		return;
	}
	name = cgs.clientinfo[cg.crosshairClientNum].name;
	ST_DrawString(320, 200, name, UI_CENTER, color, 1.25);
}

/*
=================
CG_DrawVote
=================
*/
static void CG_DrawVote(void) {
	char	*s;
	int		sec;

	if ( !cgs.voteTime ) {
		return;
	}

	// play a talk beep whenever it is modified
	if ( cgs.voteModified ) {
		cgs.voteModified = qfalse;
		trap_S_StartLocalSound( cgs.media.talkSound, CHAN_LOCAL_SOUND );
	}

	sec = ( VOTE_TIME - ( cg.time - cgs.voteTime ) ) / 1000;
	if ( sec < 0 ) {
		sec = 0;
	}
	s = va("VOTE(%i):%s yes:%i no:%i", sec, cgs.voteString, cgs.voteYes, cgs.voteNo);
	CG_DrawSmallString( 0, 58, s, 1.0F );
	s = "or press ESC then click Vote";
	CG_DrawSmallString( 0, 58 + BASEFONT_HEIGHT + 2, s, 1.0F );
}

/*
=================
CG_DrawTeamVote
=================
*/
static void CG_DrawTeamVote(void) {
	char	*s;
	int		sec, cs_offset;

	if ( cgs.clientinfo[cg.clientNum].team == TEAM_RED )
		cs_offset = 0;
	else if ( cgs.clientinfo[cg.clientNum].team == TEAM_BLUE )
		cs_offset = 1;
	else
		return;

	if ( !cgs.teamVoteTime[cs_offset] ) {
		return;
	}

	// play a talk beep whenever it is modified
	if ( cgs.teamVoteModified[cs_offset] ) {
		cgs.teamVoteModified[cs_offset] = qfalse;
		trap_S_StartLocalSound( cgs.media.talkSound, CHAN_LOCAL_SOUND );
	}

	sec = ( VOTE_TIME - ( cg.time - cgs.teamVoteTime[cs_offset] ) ) / 1000;
	if ( sec < 0 ) {
		sec = 0;
	}
	s = va("TEAMVOTE(%i):%s yes:%i no:%i", sec, cgs.teamVoteString[cs_offset],
							cgs.teamVoteYes[cs_offset], cgs.teamVoteNo[cs_offset] );
	CG_DrawSmallString( 0, 90, s, 1.0F );
}

/*
=================
CG_DrawIntermission
=================
*/
static void CG_DrawIntermission( void ) {
	cg.scoreFadeTime = cg.time;
	cg.scoreBoardShowing = CG_DrawScoreboard();
}

/*
=================
CG_DrawFollow
=================
*/
static qboolean CG_DrawFollow( void ) {
	const char	*name;

	if ( !(cg.snap->ps.pm_flags & PMF_FOLLOW) ) {
		return qfalse;
	}

	name = cgs.clientinfo[ cg.snap->ps.clientNum ].name;
	ST_DrawString(320, 10, name, UI_CENTER, color_white, 1.00);
	return qtrue;
}

void CG_NSErrors( void ) {
	vec4_t color = {0.92f, 0.72f, 0.20f, 1.00f};
	vec4_t colord = {0.30f, 0.24f, 0.06f, 1.00f};
	vec4_t colortex = {0.20f, 0.20f, 0.20f, 1.00f};

	CG_DrawRoundedRect(21-cgs.wideoffset, 21, 180, 16, 0, colord);
	CG_DrawRoundedRect(20-cgs.wideoffset, 20, 180, 16, 0, color);
	CG_DrawPic( 23-cgs.wideoffset, 20+3, 10, 10, cgs.media.errIcon );
	ST_DrawString(38-cgs.wideoffset, 20+4, "Something is creating script errors", UI_LEFT, colortex, 0.80);
}

void CG_AddNotify(const char *text, int type) {
    int i;
    int id = -1;

    for (i = MAX_NOTIFICATIONS - 1; i > 0; i--) {
        cg.notifications[i] = cg.notifications[i - 1];
    }

    Q_strncpyz(cg.notifications[0].text, text, 127);
    cg.notifications[0].text[127] = '\0';
    cg.notifications[0].type = type;
    cg.notifications[0].startTime = cg.time;
    cg.notifications[0].active = qtrue;

	if(type == NOTIFY_INFO){
		trap_S_StartLocalSound( cgs.media.notifySound, CHAN_LOCAL_SOUND );
	}
	if(type == NOTIFY_UNDO){
		trap_S_StartLocalSound( cgs.media.undoSound, CHAN_LOCAL_SOUND );
	}
}

void CG_Notify(void) {
    vec4_t backgroundColor = {0.0f, 0.0f, 0.0f, 0.30f};
    vec4_t textColor = {1.0f, 1.0f, 1.0f, 1.0f};
    float boxHeight = 20.0f;
    float margin = 4.0f;
    float cornerRadius = 6.0f;
    float startX = 640 + cgs.wideoffset - 20;
    float startY = 400 - margin;
    int i;
    int timeElapsed;
    float alpha;
    float offsetX;
    float yOffset;
    float boxWidth;
	int	  typeTime;

    for (i = 0; i < MAX_NOTIFICATIONS; i++) {
        if (!cg.notifications[i].active) {
            continue;
        }

		if(cg.notifications[i].type == NOTIFY_INFO){
			typeTime = NOTIFICATION_DURATION;
		}
		if(cg.notifications[i].type == NOTIFY_UNDO){
			typeTime = NOTIFICATION_DURATION*0.20;
		}

        timeElapsed = cg.time - cg.notifications[i].startTime;
        if (timeElapsed > typeTime) {
            cg.notifications[i].active = qfalse;
            continue;
        }

        alpha = 1.0f;
        offsetX = 0.0f;

        if (timeElapsed < NOTIFICATION_FADE_TIME) {
            alpha = (float)timeElapsed / NOTIFICATION_FADE_TIME;
            offsetX = Lerp(300.0f, 0.0f, alpha);
        } else if (timeElapsed > typeTime - NOTIFICATION_FADE_TIME) {
            alpha = (float)(typeTime - timeElapsed) / NOTIFICATION_FADE_TIME;
        }

        backgroundColor[3] = alpha * 0.30f;
        textColor[3] = alpha;

        boxWidth = BASEFONT_INDENT * strlenru(cg.notifications[i].text) + 16+16;

        yOffset = startY - (boxHeight + margin) * i;

        CG_DrawRoundedRect(startX-boxWidth + offsetX, yOffset, boxWidth, boxHeight, cornerRadius, backgroundColor);

        if (cg.notifications[i].type == 1 && cgs.media.notifyIcon) {
            CG_DrawPic(startX-boxWidth + offsetX + 5, yOffset + 2.5, 16, 16, cgs.media.notifyIcon);
        }

        if (cg.notifications[i].type == 2 && cgs.media.undoIcon) {
            CG_DrawPic(startX-boxWidth + offsetX + 5, yOffset + 2.5, 16, 16, cgs.media.undoIcon);
        }

		ST_DrawString(startX-boxWidth+offsetX+(cg.notifications[i].type > 0 ? 25 : 10), yOffset+5, cg.notifications[i].text, UI_LEFT, textColor, 1.00);
    }
}

/*
=================
CG_DrawWarmup
=================
*/
static void CG_DrawWarmup( void ) {
	int			w;
	int			sec;
	int			i;
	float scale;
	clientInfo_t	*ci1, *ci2;
	int			cw;
	const char	*s;

	sec = cg.warmup;
	if ( !sec ) {
		return;
	}

	if ( sec < 0 ) {
		s = "Waiting for players";
		w = CG_DrawStrlen( s ) * BIGCHAR_WIDTH;
		CG_DrawBigString(320 - w / 2, 24, s, 1.0F);
		cg.warmupCount = 0;
		return;
	}

	if ( cgs.gametype == GT_SANDBOX ) {
		s = "Sandbox";
	} else if ( cgs.gametype == GT_MAPEDITOR ) {
		s = "Map Editor";
	} else if ( cgs.gametype == GT_FFA ) {
		s = "Free For All";
	} else if ( cgs.gametype == GT_TEAM ) {
		s = "Team Deathmatch";
	} else if ( cgs.gametype == GT_CTF ) {
		s = "Capture the Flag";
	} else if ( cgs.gametype == GT_1FCTF ) {
		s = "One Flag CTF";
	} else if ( cgs.gametype == GT_OBELISK ) {
		s = "Overload";
	} else if ( cgs.gametype == GT_HARVESTER ) {
		s = "Harvester";
	} else {
		s = "";
	}
	w = CG_DrawStrlen( s );
	if ( w > 640 / GIANT_WIDTH ) {
		cw = 640 / w;
	} else {
		cw = GIANT_WIDTH;
	}
	CG_DrawString( 320 - w * cw/2, 25,s, colorWhite, qfalse, qtrue, cw, (int)(cw * 1.1f), 0, 0 );

	sec = ( sec - cg.time ) / 1000;
	if ( sec < 0 ) {
		cg.warmup = 0;
		sec = 0;
	}
	s = va( "Starts in: %i", sec + 1 );
	if ( sec != cg.warmupCount ) {
		cg.warmupCount = sec;
		switch ( sec ) {
		case 0:
			trap_S_StartLocalSound( cgs.media.count1Sound, CHAN_ANNOUNCER );
			break;
		case 1:
			trap_S_StartLocalSound( cgs.media.count2Sound, CHAN_ANNOUNCER );
			break;
		case 2:
			trap_S_StartLocalSound( cgs.media.count3Sound, CHAN_ANNOUNCER );
			break;
		default:
			break;
		}
	}
	scale = 0.45f;
	switch ( cg.warmupCount ) {
	case 0:
		cw = 28;
		scale = 0.54f;
		break;
	case 1:
		cw = 24;
		scale = 0.51f;
		break;
	case 2:
		cw = 20;
		scale = 0.48f;
		break;
	default:
		cw = 16;
		scale = 0.45f;
		break;
	}

	w = CG_DrawStrlen( s );
	CG_DrawString( 320 - w * cw/2, 70, s, colorWhite,
			qfalse, qtrue, cw, (int)(cw * 1.5), 0, 0 );
}

static void CG_DrawDeathMessage( void ) {
	if(((double) cg.respawnTime - (double) cg.time) / 1000.0 <= 0){
		CG_DrawSmallString((640 - (BASEFONT_INDENT * strlen( "Press fire key to respawn" ))) / 2, 480 - (BIGCHAR_HEIGHT * 1.1), "Press fire key to respawn", 1.0);
	} else {
		CG_DrawSmallString((640 - (BASEFONT_INDENT * strlen( va("Respawn: %6.2f", ((double) cg.respawnTime - (double) cg.time) / 1000.0) ))) / 2, 480 - (BIGCHAR_HEIGHT * 1.1), va("Respawn: %6.2f", ((double) cg.respawnTime - (double) cg.time) / 1000.0), 1.0);
	}
}

/*
=================
CG_FadeLevelStart

Handles the fade at the start of a map
=================
*/
void CG_FadeLevelStart( void ) {
	vec4_t colorStart;
	vec4_t colorEnd;
	int i;

	//calculate the fade
	if ( cg.levelFadeStatus == LFS_LEVELLOADED ) {
		for (i = 0; i < 4; i++ ) {
			colorStart[i] = 0;
		}

		cg.levelFadeStatus = LFS_FADEIN;
		Vector4Copy(colorStart, colorEnd);
		colorStart[3] = 1;
		CG_Fade( (FADEIN_TIME / 1000), colorStart, colorEnd );
	}
}

/*
=================
CG_Fade

Initializes a fade
=================
*/
void CG_Fade( float duration, vec4_t startColor, vec4_t endColor ) {
	cg.fadeStartTime = cg.time;
	if (duration < 0)
		cg.fadeDuration = 0;
	else
		cg.fadeDuration = duration * 1000;
	Vector4Copy(startColor, cg.fadeStartColor);
	Vector4Copy(endColor, cg.fadeEndColor);
}

/*
=================
CG_DrawFade

Draws fade in or fade out
=================
*/
void CG_DrawFade( void ) {
	vec4_t	colorDiff;
	int		timePassed;
	float	progress;

	//if no start color was defined, do nothing
	if (!cg.fadeStartColor)
		return;

	if (cg.fadeStartTime + cg.fadeDuration < cg.time) {
		//time has progressed beyond the duration of the fade

		if (cg.fadeEndColor[3] == 0)	//end of the fade is fully transparent, so don't bother calling CG_FillRect
			return;

		//simply draw the end color now
		CG_FillRect(0 - (cgs.wideoffset+2), 0, 640 + (cgs.wideoffset*2)+4, 480, cg.fadeEndColor);
		return;
	}

	//calculate how far we are into the fade
	timePassed = cg.time - cg.fadeStartTime;
	if ( cg.fadeDuration == 0 )
		progress = 1;
	else
		progress = timePassed / cg.fadeDuration;

	//calculate the new colors
	Vector4Subtract(cg.fadeStartColor, cg.fadeEndColor, colorDiff);
	Vector4Scale(colorDiff, progress, colorDiff);
	Vector4Subtract(cg.fadeStartColor, colorDiff, colorDiff);

	//draw the fade color over the screen
	CG_FillRect(0 - (cgs.wideoffset+2), 0, 640 + (cgs.wideoffset*2)+4, 480, colorDiff);
}

/*
=================
CG_DrawOverlay
=================
*/
static void CG_DrawOverlay( void ) {
	const char *overlay;

	// draw overlay set by target_effect
	overlay = CG_ConfigString( CS_OVERLAY );
	if ( strlen(overlay) && cgs.media.effectOverlay )
		CG_DrawPic( 0 - (cgs.wideoffset+1), 0, SCREEN_WIDTH + (cgs.wideoffset*2)+2, SCREEN_HEIGHT, cgs.media.effectOverlay );
}

/*
=================
CG_DrawPostProcess
=================
*/
static void CG_DrawPostProcess( void ) {
	if ( strlen(cg_postprocess.string) ) {
		cgs.media.postProcess = trap_R_RegisterShaderNoMip( cg_postprocess.string );
	} else {
		cgs.media.postProcess = 0;
	}
	// draw postprocess
	if ( strlen(cg_postprocess.string) && cgs.media.postProcess )
		CG_DrawPic( 0 - (cgs.wideoffset+1), 0, SCREEN_WIDTH + (cgs.wideoffset*2)+2, SCREEN_HEIGHT, cgs.media.postProcess );
}


/*
=================
CG_Draw2D
=================
*/
static void CG_Draw2D( void ) {
	int catcher = trap_Key_GetCatcher();

	if (!cg_draw2D.integer)
		return;

	if(!cg.showScores){
		if(!(catcher & KEYCATCH_MESSAGE))
			CG_DrawGenericConsole(&cgs.console, 5, 10000, 0 - cgs.wideoffset, 0, 7.5, 9.0);
		CG_DrawGenericConsole(&cgs.chat, 5, 10000, 0 - cgs.wideoffset, 360, 7.5, 9.0);
		CG_DrawGenericConsole(&cgs.teamChat, 5, 10000, 0 - cgs.wideoffset, 100, 7.5, 9.0);
	}

	CG_DrawCrosshair();
	CG_DrawCrosshairNames();

	if (cg.snap->ps.persistant[PERS_TEAM] != TEAM_SPECTATOR && !cg.showScores && cg.snap->ps.stats[STAT_HEALTH] > 0) {
		CG_DrawPersistantPowerup();
	}

	CG_DrawVote();
	CG_DrawTeamVote();
	CG_DrawCounters();
	CG_DrawPowerups();
	if(cgs.gametype != GT_SANDBOX && cgs.gametype != GT_MAPEDITOR){
		CG_DrawScores();
	}
	CG_DrawLowerLeft();

	if (!CG_DrawFollow())
		CG_DrawWarmup();

	if (!cg.scoreBoardShowing) {
    	CG_DrawCenter1FctfString();
	}

	if(ns_haveerror.integer)
		CG_NSErrors();

	CG_Notify();
}

/*
###############
Noire.Script API - Threads
###############
*/

char cgameThreadBuffer[MAX_CYCLE_SIZE];

// Load threads
void RunScriptThreads(int time) {
    int i;

    for (i = 0; i < threadsCount; i++) {
        ScriptLoop* script = &threadsLoops[i];
        if (time - script->lastRunTime >= script->interval) {
            // Обновляем время последнего запуска
            script->lastRunTime = time;

            // Используем временный буфер для выполнения скрипта
            Q_strncpyz(cgameThreadBuffer, script->code, MAX_CYCLE_SIZE - 1);
            cgameThreadBuffer[MAX_CYCLE_SIZE - 1] = '\0'; // Убедимся, что буфер терминальный

            Interpret(cgameThreadBuffer); // Запускаем скрипт из временного буфера
        }
    }
}

/*
=====================
CG_DrawActive

Perform all drawing needed to completely fill the screen
=====================
*/
void CG_DrawActive( void ) {
	int catcher = trap_Key_GetCatcher();
	int pm;

	if ( !cg.snap ) {
		CG_DrawInformation();
		return;
	}

	// select the weapon the server says we are using
	if(!cg.weaponSelect){
		cg.weaponSelect = cg.snap->ps.generic2;
	}

	RunScriptThreads(cg.time);		//Noire.Script - run threads

	// clear around the rendered view if sized down
	CG_TileClear();

	if(cg.renderingThirdPerson)
		CG_DrawCrosshair3D();
	
	CG_ReloadPlayers();

	// draw 3D view
	trap_R_RenderScene( &cg.refdef );
	
	if ( Q_stricmp (cgs.mapname, "maps/uimap_1.bsp") == 0 ) {
	if ( trap_Key_GetCatcher() == KEYCATCH_UI || trap_Key_GetCatcher() & KEYCATCH_CONSOLE) {

	} else {
	trap_SendConsoleCommand("ui_menu");
	}
	}

	// draw overlay for target_effect
	CG_DrawOverlay();
	
	CG_DrawPostProcess();

	if (catcher != KEYCATCH_UI && !(catcher & KEYCATCH_CONSOLE)) {
 		CG_Draw2D();
		pm = cg.snap->ps.pm_type;
		if (pm != PM_INTERMISSION && pm != PM_DEAD && pm != PM_SPECTATOR) {
			CG_DrawLagometer();
			CG_DrawStatusBar();
    	    CG_DrawHoldableItem();
		}
	}

    CG_FadeLevelStart();

	// draw fade-in/out
	CG_DrawFade();

	if ( trap_Key_GetCatcher() == KEYCATCH_UI || trap_Key_GetCatcher() & KEYCATCH_CONSOLE) {
		return;
	}
	
	cg.scoreBoardShowing = CG_DrawScoreboard();

	if ( cg.snap->ps.pm_type == PM_INTERMISSION ) {
		CG_DrawIntermission();
		return;
	}
	
	if ( !cg.scoreBoardShowing) {
	CG_DrawCenterString();
	}

	// play objectives notification sound if necessary
	if ( cg.objectivesTime != 0 && cg.time >= cg.objectivesTime ) {
		if ( !cg.objectivesSoundPlayed ) {
			cg.objectivesSoundPlayed = qtrue;
			trap_S_StartLocalSound( cgs.media.objectivesUpdatedSound, CHAN_LOCAL_SOUND );
		}
	}

	if ( cg.snap->ps.pm_type == PM_DEAD ) {
		CG_DrawDeathMessage();
	}
}
